/**
 * crt_hooks.cpp — Native CRT hook implementations
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Each function here is a PPC_FUNC_IMPL wrapper that:
 *   1. Reads arguments from ctx.r3–r10 (PPC integer calling convention)
 *      or ctx.f1–f8 (float/double arguments).
 *   2. Translates 32-bit guest addresses → host pointers via GUEST_PTR().
 *   3. Calls the native libc/platform implementation directly.
 *   4. Writes the return value back into ctx.r3 (or ctx.f1 for floats).
 *
 * WHY NOT HOOK THE RAGE HEAP?
 * ----------------------------
 * sysMemAllocator_Allocate / Free / rage_free are intentionally left
 * un-hooked. The RAGE heap subsystem owns its own virtual address ranges,
 * uses per-allocator ownership tables (atSingleton), and stores a raw
 * pointer 4 bytes before every user allocation for bookkeeping. Routing
 * those through native malloc() would bypass all of that and corrupt the
 * ownership invariants the engine relies on.
 *
 * Pure data-transform functions (memcpy, strlen, sprintf, …) have no RAGE
 * state and are safe — and beneficial — to replace natively.
 *
 * ADDING NEW HOOKS
 * -----------------
 * Follow the pattern below:
 *   PPC_FUNC_IMPL(crt_hook_yourfunc) {
 *       PPC_FUNC_PROLOGUE();
 *       type arg1 = (type)GUEST_PTR(ctx.r3);   // or ctx.r3.u32 for integers
 *       ctx.r3.u32 = (uint32_t)HOST_TO_GUEST( yourfunc(arg1, ...) );
 *   }
 * Then add an entry in g_crt_hook_table[] at the bottom.
 */

#include "crt_hooks.h"

#include <string.h>    /* memcpy, memset, memmove, strlen, strcmp, strncpy */
#include <stdio.h>     /* sprintf, snprintf, vsnprintf                     */
#include <stdarg.h>    /* va_list                                           */
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>     /* wcslen                                            */

/* Canonical VMX save/restore no-op, defined in ppc_helpers.c. One impl,
 * N registrations — see the bottom of this file and ppc_helpers.c. */
extern "C" PPC_FUNC_IMPL(ppc_helper_vmx_noop);

/* =====================================================================
 * PPC VARIADIC RECONSTRUCTION
 *
 * The Xbox 360 PPC64 SysV-derived ABI lays out variadic calls as:
 *
 *   GPR args :  r3..r10     (8 slots, 64-bit each)
 *   FPR args :  f1..f13     (13 slots, 64-bit each — only f1..f8
 *                            practically reachable from CRT format hooks)
 *   Overflow :  caller's stack frame, pushed right-to-left after regs
 *
 * va_list on this ABI is a 12-byte struct in guest memory:
 *
 *   offset 0 : uint8_t  gpr_left       (GPR slots remaining, starts at 8)
 *   offset 1 : uint8_t  fpr_left       (FPR slots remaining, starts at 13)
 *   offset 2 : uint16_t reserved       (padding, zero)
 *   offset 4 : uint32_t overflow_ea    (guest ptr to overflow stack area)
 *   offset 8 : uint32_t reg_save_ea    (guest ptr to saved-register block)
 *
 * The register save area in guest memory is laid out as:
 *
 *   reg_save_ea + 0x00 : r3..r10    (8 × 8 bytes = 64 bytes)
 *   reg_save_ea + 0x40 : f1..f13    (13 × 8 bytes = 104 bytes)
 *
 * va_arg(ap, int)    advances gpr_left  (reads low 32 bits of GPR slot)
 * va_arg(ap, double) advances fpr_left  (reads full 64-bit FPR slot)
 * When a register class is exhausted, further args come from overflow_ea.
 *
 * Byte offsets verified via Python:
 *   sizeof(va_list) = 12; gpr_save=64B; fpr_save=104B; total_save=168B.
 * ===================================================================== */

namespace ppc_varargs {

/* va_list struct layout in guest memory. */
static constexpr size_t kVaListGprLeftOff    = 0;
static constexpr size_t kVaListFprLeftOff    = 1;
static constexpr size_t kVaListOverflowOff   = 4;
static constexpr size_t kVaListRegSaveOff    = 8;

/* Register save area layout. */
static constexpr size_t kGprSlotCount        = 8;   /* r3..r10 */
static constexpr size_t kFprSlotCount        = 13;  /* f1..f13 */
static constexpr size_t kGprSlotWidth        = 8;   /* 64-bit GPR */
static constexpr size_t kFprSlotWidth        = 8;   /* 64-bit FPR */
static constexpr size_t kGprSaveAreaBytes    = kGprSlotCount * kGprSlotWidth;   /* 64  */
static constexpr size_t kFprSaveAreaOffset   = kGprSaveAreaBytes;               /* 0x40 */

/* Big-endian byte-swap helpers (guest is BE, host is typically LE). */
static inline uint32_t bswap32_be(uint32_t v) {
#if defined(__BIG_ENDIAN__)
    return v;
#else
    return __builtin_bswap32(v);
#endif
}
static inline uint64_t bswap64_be(uint64_t v) {
#if defined(__BIG_ENDIAN__)
    return v;
#else
    return __builtin_bswap64(v);
#endif
}

/* Read a big-endian 32-bit word from guest memory. */
static inline uint32_t guest_read_be32(uint32_t guest_ea) {
    const uint32_t* p = (const uint32_t*)((uintptr_t)g_mem_base + guest_ea);
    return bswap32_be(*p);
}

/* Read a big-endian 64-bit word from guest memory. */
static inline uint64_t guest_read_be64(uint32_t guest_ea) {
    const uint64_t* p = (const uint64_t*)((uintptr_t)g_mem_base + guest_ea);
    return bswap64_be(*p);
}

/* Guest view of a PPC va_list, resolved to host pointers. */
struct PpcVaList {
    uint8_t* gpr_left_p;         /* byte: GPR slots remaining  */
    uint8_t* fpr_left_p;         /* byte: FPR slots remaining  */
    uint32_t reg_save_ea;        /* guest addr of save block   */
    uint32_t overflow_ea;        /* guest addr of overflow     */
};

/* Resolve a guest va_list* into host-addressable fields. */
static inline PpcVaList resolve(uint32_t va_list_ea) {
    PpcVaList va{};
    uint8_t* base = (uint8_t*)((uintptr_t)g_mem_base + va_list_ea);
    va.gpr_left_p  = base + kVaListGprLeftOff;
    va.fpr_left_p  = base + kVaListFprLeftOff;
    va.overflow_ea = guest_read_be32(va_list_ea + kVaListOverflowOff);
    va.reg_save_ea = guest_read_be32(va_list_ea + kVaListRegSaveOff);
    return va;
}

/* Pull the next integer-class slot as a 64-bit value.
 * Advances either the GPR register save area or the stack overflow area. */
static inline uint64_t next_int64(PpcVaList& va) {
    if (*va.gpr_left_p > 0) {
        const uint32_t used = kGprSlotCount - *va.gpr_left_p;
        const uint64_t v    = guest_read_be64(va.reg_save_ea + used * kGprSlotWidth);
        --(*va.gpr_left_p);
        return v;
    }
    /* Overflow: integers occupy 8-byte slots on this ABI. */
    const uint64_t v = guest_read_be64(va.overflow_ea);
    va.overflow_ea += kGprSlotWidth;
    return v;
}

/* Pull the next floating-class slot as a host double.
 * Advances either the FPR register save area or the stack overflow area. */
static inline double next_double(PpcVaList& va) {
    uint64_t bits;
    if (*va.fpr_left_p > 0) {
        const uint32_t used = kFprSlotCount - *va.fpr_left_p;
        bits = guest_read_be64(va.reg_save_ea + kFprSaveAreaOffset
                                              + used * kFprSlotWidth);
        --(*va.fpr_left_p);
    } else {
        bits = guest_read_be64(va.overflow_ea);
        va.overflow_ea += kFprSlotWidth;
    }
    double out;
    static_assert(sizeof(out) == sizeof(bits), "double must be 64-bit");
    memcpy(&out, &bits, sizeof(out));
    return out;
}

/* Parse a printf conversion spec starting at '%' and classify the arg class.
 * Returns the number of characters consumed and writes the arg class. */
enum class ArgClass { None, Int32, Int64, Double, Pointer };

static size_t classify_conversion(const char* p, ArgClass& out_class) {
    out_class = ArgClass::None;
    if (*p != '%') return 0;
    const char* start = p;
    ++p;
    /* flags */
    while (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0') ++p;
    /* width */
    if (*p == '*') { out_class = ArgClass::Int32; return (size_t)(p + 1 - start); }
    while (*p >= '0' && *p <= '9') ++p;
    /* precision */
    if (*p == '.') {
        ++p;
        if (*p == '*') { out_class = ArgClass::Int32; return (size_t)(p + 1 - start); }
        while (*p >= '0' && *p <= '9') ++p;
    }
    /* length modifier */
    bool is_long_long = false;
    bool is_long      = false;
    if (p[0] == 'l' && p[1] == 'l') { is_long_long = true; p += 2; }
    else if (p[0] == 'I' && p[1] == '6' && p[2] == '4') { is_long_long = true; p += 3; }
    else if (*p == 'l') { is_long = true; ++p; }
    else if (*p == 'h') { ++p; if (*p == 'h') ++p; }
    else if (*p == 'z' || *p == 'j' || *p == 't') ++p;
    /* conversion */
    const char c = *p;
    switch (c) {
        case 'd': case 'i': case 'u': case 'o':
        case 'x': case 'X': case 'c':
            out_class = (is_long_long ? ArgClass::Int64 : ArgClass::Int32);
            break;
        case 'f': case 'F': case 'e': case 'E':
        case 'g': case 'G': case 'a': case 'A':
            out_class = ArgClass::Double;
            break;
        case 's': case 'p': case 'n':
            out_class = ArgClass::Pointer;
            break;
        case '%':
            out_class = ArgClass::None;
            break;
        default:
            out_class = ArgClass::Int32;   /* conservative fallback */
            break;
    }
    (void)is_long;
    return (size_t)(p + 1 - start);
}

/* Maximum formatted output we ever produce from a hook.
 * Xbox 360 debug paths assume unbounded sprintf but practical RAGE
 * call-sites top out well below this ceiling. */
static constexpr size_t kFormatScratchSize = 4096;

/* Format a single conversion spec (copied verbatim from the user format)
 * using the next value pulled from the PPC va_list. Returns bytes written. */
static int format_one(char* dst, size_t dst_cap,
                      const char* spec, size_t spec_len,
                      ArgClass cls, PpcVaList& va)
{
    char spec_buf[64];
    if (spec_len >= sizeof(spec_buf)) spec_len = sizeof(spec_buf) - 1;
    memcpy(spec_buf, spec, spec_len);
    spec_buf[spec_len] = '\0';

    switch (cls) {
        case ArgClass::Int32: {
            const uint32_t v = (uint32_t)next_int64(va);
            return snprintf(dst, dst_cap, spec_buf, v);
        }
        case ArgClass::Int64: {
            const uint64_t v = next_int64(va);
            return snprintf(dst, dst_cap, spec_buf, (long long)v);
        }
        case ArgClass::Double: {
            const double d = next_double(va);
            return snprintf(dst, dst_cap, spec_buf, d);
        }
        case ArgClass::Pointer: {
            const uint32_t guest_ea = (uint32_t)next_int64(va);
            const void* host_p = guest_ea ? (const void*)((uintptr_t)g_mem_base + guest_ea)
                                          : NULL;
            /* %s wants const char*; %p/%n also fit into a pointer slot. */
            return snprintf(dst, dst_cap, spec_buf, host_p);
        }
        case ArgClass::None:
        default:
            if (dst_cap > 0) {
                size_t n = spec_len < dst_cap - 1 ? spec_len : dst_cap - 1;
                memcpy(dst, spec_buf, n);
                dst[n] = '\0';
                return (int)n;
            }
            return 0;
    }
}

/* Core engine: walk fmt, emit literal bytes verbatim, and for every
 * conversion spec pull the next arg out of the supplied va_list and
 * delegate to the host snprintf for that single spec. */
static int format_engine(char* out, size_t out_cap,
                         const char* fmt, PpcVaList& va)
{
    if (!out || out_cap == 0 || !fmt) return -1;
    size_t w = 0;
    const char* p = fmt;
    while (*p && w + 1 < out_cap) {
        if (*p != '%') {
            out[w++] = *p++;
            continue;
        }
        ArgClass cls;
        size_t consumed = classify_conversion(p, cls);
        if (consumed == 0) { out[w++] = *p++; continue; }
        int n = format_one(out + w, out_cap - w, p, consumed, cls, va);
        if (n < 0) { out[w] = '\0'; return -1; }
        w += (size_t)n;
        if (w >= out_cap) { w = out_cap - 1; break; }
        p += consumed;
    }
    out[w] = '\0';
    return (int)w;
}

/* Build a synthetic va_list for sprintf from the live PPC context.
 * sprintf itself has no guest va_list — the caller loaded arg values
 * directly into r5..r10 and f1..f8. We materialize a save area on the
 * host stack so the same format_engine path can be used. */
struct SyntheticSaveArea {
    uint64_t gpr_slots[kGprSlotCount];
    uint64_t fpr_slots[kFprSlotCount];
    uint8_t  gpr_left;
    uint8_t  fpr_left;
};

static void build_synthetic_from_ctx(SyntheticSaveArea& area,
                                     PPCRegContext& ctx,
                                     unsigned gpr_start_index)
{
    /* gpr_start_index is 0 for sprintf (r5 is first varg at slot 0-ish
     * after r3=buf, r4=fmt — BUT the PPC ABI places r5..r10 in save
     * slots independent of callee use, so we copy r5..r10 + the
     * variadic tail from r3,r4 semantics unchanged).
     *
     * For the sprintf(buf, fmt, ...) signature the first variadic arg
     * is r5. We populate synthetic slots in GPR order r5..r10, mapping
     * directly onto save-area indices 0..5 and marking slots 6,7 as
     * already consumed so overflow kicks in if the format needs more. */
    const uint64_t gprs[kGprSlotCount] = {
        (uint64_t)ctx.r5.u64, (uint64_t)ctx.r6.u64,
        (uint64_t)ctx.r7.u64, (uint64_t)ctx.r8.u64,
        (uint64_t)ctx.r9.u64, (uint64_t)ctx.r10.u64,
        0ULL,                 0ULL,
    };
    for (size_t i = 0; i < kGprSlotCount; ++i) area.gpr_slots[i] = gprs[i];

    /* Copy f1..f8 bit patterns into the FPR save slots. */
    const double fprs[8] = {
        ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64,
        ctx.f5.f64, ctx.f6.f64, ctx.f7.f64, ctx.f8.f64,
    };
    for (size_t i = 0; i < 8; ++i)
        memcpy(&area.fpr_slots[i], &fprs[i], sizeof(uint64_t));
    for (size_t i = 8; i < kFprSlotCount; ++i) area.fpr_slots[i] = 0ULL;

    area.gpr_left = 6;   /* r5..r10 available (8 - 2 used for buf+fmt) */
    area.fpr_left = 8;   /* f1..f8 carry float args */
    (void)gpr_start_index;
}

/* Run format_engine against a synthesized save area (host memory, native
 * endian). We build a tiny shim va_list in local host memory with the
 * engine unchanged by reading host-native slots. */
static int format_engine_synthetic(char* out, size_t out_cap,
                                   const char* fmt,
                                   SyntheticSaveArea& area)
{
    if (!out || out_cap == 0 || !fmt) return -1;
    size_t w = 0;
    uint8_t gpr_left = area.gpr_left;
    uint8_t fpr_left = area.fpr_left;
    size_t  overflow_idx = 0;  /* unused for sprintf — no stack recon */
    const char* p = fmt;
    while (*p && w + 1 < out_cap) {
        if (*p != '%') { out[w++] = *p++; continue; }
        ArgClass cls;
        size_t consumed = classify_conversion(p, cls);
        if (consumed == 0) { out[w++] = *p++; continue; }

        char spec_buf[64];
        size_t sl = consumed < sizeof(spec_buf) ? consumed : sizeof(spec_buf) - 1;
        memcpy(spec_buf, p, sl);
        spec_buf[sl] = '\0';

        int n = 0;
        switch (cls) {
            case ArgClass::Int32: {
                uint64_t v = 0;
                if (gpr_left > 0) { v = area.gpr_slots[kGprSlotCount - gpr_left]; --gpr_left; }
                n = snprintf(out + w, out_cap - w, spec_buf, (uint32_t)v);
                break;
            }
            case ArgClass::Int64: {
                uint64_t v = 0;
                if (gpr_left > 0) { v = area.gpr_slots[kGprSlotCount - gpr_left]; --gpr_left; }
                n = snprintf(out + w, out_cap - w, spec_buf, (long long)v);
                break;
            }
            case ArgClass::Double: {
                double d = 0.0;
                if (fpr_left > 0) {
                    memcpy(&d, &area.fpr_slots[kFprSlotCount - fpr_left], sizeof(double));
                    --fpr_left;
                }
                n = snprintf(out + w, out_cap - w, spec_buf, d);
                break;
            }
            case ArgClass::Pointer: {
                uint32_t guest_ea = 0;
                if (gpr_left > 0) {
                    guest_ea = (uint32_t)area.gpr_slots[kGprSlotCount - gpr_left];
                    --gpr_left;
                }
                const void* host_p = guest_ea
                    ? (const void*)((uintptr_t)g_mem_base + guest_ea) : NULL;
                n = snprintf(out + w, out_cap - w, spec_buf, host_p);
                break;
            }
            default:
                if (w + sl < out_cap) { memcpy(out + w, spec_buf, sl); n = (int)sl; }
                break;
        }
        if (n < 0) { out[w] = '\0'; return -1; }
        w += (size_t)n;
        if (w >= out_cap) { w = out_cap - 1; break; }
        p += consumed;
        (void)overflow_idx;
    }
    out[w] = '\0';
    return (int)w;
}

}  /* namespace ppc_varargs */

/* =====================================================================
 * STRING FUNCTIONS
 * ===================================================================== */

/**
 * strlen hook — 0x820C0000
 *
 * PPC: r3 = const char* str  →  r3 = size_t length
 */
PPC_FUNC_IMPL(crt_hook_strlen) {
    PPC_FUNC_PROLOGUE();
    const char* str = GUEST_STR(ctx.r3);
    ctx.r3.u64 = str ? (uint64_t)strlen(str) : 0ULL;
}

/**
 * strcmp hook — 0x82432970
 *
 * PPC: r3 = const char* a,  r4 = const char* b  →  r3 = int result
 */
PPC_FUNC_IMPL(crt_hook_strcmp) {
    PPC_FUNC_PROLOGUE();
    const char* a = GUEST_STR(ctx.r3);
    const char* b = GUEST_STR(ctx.r4);
    if (!a || !b) {
        /* Graceful degradation: treat NULL as empty string. */
        ctx.r3.s64 = (a ? 1 : 0) - (b ? 1 : 0);
        return;
    }
    ctx.r3.s64 = (int64_t)strcmp(a, b);
}

/**
 * strncpy hook — 0x82430120
 *
 * PPC: r3 = char* dst,  r4 = const char* src,  r5 = size_t n
 *      →  r3 = char* dst  (same pointer, as per C standard)
 */
PPC_FUNC_IMPL(crt_hook_strncpy) {
    PPC_FUNC_PROLOGUE();
    char*       dst = GUEST_MSTR(ctx.r3);
    const char* src = GUEST_STR(ctx.r4);
    size_t      n   = (size_t)ctx.r5.u32;

    if (dst && src)
        strncpy(dst, src, n);

    /* Return value = dst (same guest address, unchanged). */
    /* ctx.r3 already holds the guest address — no change needed. */
}

/**
 * strncmp hook — 0x824320D0
 *
 * PPC: r3 = const char* a,  r4 = const char* b,  r5 = size_t n
 *      →  r3 = int result  (negative/zero/positive, C-standard semantics)
 *
 * The guest body is the classic Xenon unrolled bdnzf-loop. Native strncmp()
 * on host libc is trivial to dispatch — and handles the n==0 fast path
 * (returns 0) identically to the guest's early `li r3,0; blr`.
 */
PPC_FUNC_IMPL(crt_hook_strncmp) {
    PPC_FUNC_PROLOGUE();
    const char* a = GUEST_STR(ctx.r3);
    const char* b = GUEST_STR(ctx.r4);
    size_t      n = (size_t)ctx.r5.u32;

    if (n == 0) {
        ctx.r3.s64 = 0;
        return;
    }
    if (!a || !b) {
        /* Graceful degradation: treat NULL as empty string. */
        ctx.r3.s64 = (a ? 1 : 0) - (b ? 1 : 0);
        return;
    }
    ctx.r3.s64 = (int64_t)strncmp(a, b, n);
}

/**
 * wcslen hook — 0x82433748
 *
 * PPC: r3 = const wchar_t* str  →  r3 = size_t length (UTF-16 code units)
 *
 * The Xbox 360 treats `wchar_t` as 16-bit (UTF-16). The guest body walks
 * 2 bytes per step and returns (bytes_scanned/2) - 1. Dispatching to host
 * wcslen() here requires that host `wchar_t` is also 16-bit — which is
 * true on Windows (16-bit wchar_t) but NOT on macOS/Linux (32-bit). We
 * therefore walk the guest string directly as uint16_t to stay ABI-safe
 * across hosts.
 */
PPC_FUNC_IMPL(crt_hook_wcslen) {
    PPC_FUNC_PROLOGUE();
    const uint16_t* s = (const uint16_t*)GUEST_CPTR(ctx.r3);
    if (!s) {
        ctx.r3.u64 = 0ULL;
        return;
    }
    /* Guest memory is big-endian; a zero code unit is 0x0000 regardless of
     * endianness, so a raw byte-pair compare works without byteswap. */
    size_t n = 0;
    while (s[n] != 0) ++n;
    ctx.r3.u64 = (uint64_t)n;
}

/* =====================================================================
 * MEMORY FUNCTIONS
 * ===================================================================== */

/**
 * memset hook — 0x8242FED0
 *
 * PPC: r3 = void* dst,  r4 = int c (byte value),  r5 = size_t count
 *      →  r3 = void* dst
 */
PPC_FUNC_IMPL(crt_hook_memset) {
    PPC_FUNC_PROLOGUE();
    void*  dst   = GUEST_PTR(ctx.r3);
    int    c     = (int)ctx.r4.s32;
    size_t count = (size_t)ctx.r5.u32;

    if (dst && count)
        memset(dst, c, count);

    /* ctx.r3 already holds dst guest address — unchanged. */
}

/**
 * memset_aligned hook — 0x8242FEF0
 *
 * A variant used when the destination is known to be word-aligned;
 * semantics are otherwise identical. Route to the same native memset.
 */
PPC_FUNC_IMPL(crt_hook_memset_aligned) {
    PPC_FUNC_PROLOGUE();
    void*  dst   = GUEST_PTR(ctx.r3);
    int    c     = (int)ctx.r4.s32;
    size_t count = (size_t)ctx.r5.u32;

    if (dst && count)
        memset(dst, c, count);
}

/**
 * memmove hook — 0x8242FF70
 *
 * PPC: r3 = void* dst,  r4 = const void* src,  r5 = size_t count
 *      →  r3 = void* dst
 *
 * Handles overlapping regions correctly (unlike memcpy).
 */
PPC_FUNC_IMPL(crt_hook_memmove) {
    PPC_FUNC_PROLOGUE();
    void*       dst   = GUEST_PTR(ctx.r3);
    const void* src   = GUEST_CPTR(ctx.r4);
    size_t      count = (size_t)ctx.r5.u32;

    if (dst && src && count)
        memmove(dst, src, count);
}

/**
 * memmove_backward hook — 0x8242FFAc
 *
 * The game's explicit backwards-copy variant (for overlapping dst > src).
 * memmove() already handles this correctly, so we route here too.
 */
PPC_FUNC_IMPL(crt_hook_memmove_backward) {
    PPC_FUNC_PROLOGUE();
    void*       dst   = GUEST_PTR(ctx.r3);
    const void* src   = GUEST_CPTR(ctx.r4);
    size_t      count = (size_t)ctx.r5.u32;

    if (dst && src && count)
        memmove(dst, src, count);
}

/**
 * memcpy hook — 0x82434100
 *
 * PPC: r3 = void* dst,  r4 = const void* src,  r5 = size_t count
 *      →  r3 = void* dst
 *
 * The original is a heavily unrolled 128-byte-block Xenon loop with dcbt
 * prefetch. Native memcpy on modern hosts (AVX2/NEON) will beat it easily.
 *
 * NOTE: The game's original memcpy assumes non-overlapping buffers.
 * We use memcpy() here which has the same assumption; memmove() handles the
 * overlapping case and has its own hook above.
 */
PPC_FUNC_IMPL(crt_hook_memcpy) {
    PPC_FUNC_PROLOGUE();
    void*       dst   = GUEST_PTR(ctx.r3);
    const void* src   = GUEST_CPTR(ctx.r4);
    size_t      count = (size_t)ctx.r5.u32;

    if (dst && src && count)
        memcpy(dst, src, count);
}

/* =====================================================================
 * FORMATTED OUTPUT
 * ===================================================================== */

/**
 * sprintf hook — 0x825863BC
 *
 * PPC: r3 = char* buf,  r4 = const char* fmt,  r5..r10 = integer vargs,
 *      f1..f8 = float vargs  →  r3 = int characters written
 *
 * Full variadic reconstruction: we synthesize a PPC save area from live
 * register state (integers from r5..r10, doubles from f1..f8) and run
 * our format engine against it so conversions like %f, %g, %e and mixed
 * %d/%f sequences format correctly. See ppc_varargs:: for the ABI notes.
 */
PPC_FUNC_IMPL(crt_hook_sprintf) {
    PPC_FUNC_PROLOGUE();
    char*       buf = GUEST_MSTR(ctx.r3);
    const char* fmt = GUEST_STR(ctx.r4);

    if (!buf || !fmt) {
        ctx.r3.s64 = -1;
        return;
    }

    ppc_varargs::SyntheticSaveArea area;
    ppc_varargs::build_synthetic_from_ctx(area, ctx, /*gpr_start_index=*/0);

    const int written = ppc_varargs::format_engine_synthetic(
        buf, ppc_varargs::kFormatScratchSize, fmt, area);
    ctx.r3.s64 = (int64_t)written;
}

/**
 * _snprintf hook — 0x820C0B40
 *
 * PPC: r3 = char* buf,  r4 = size_t n,  r5 = const char* fmt,
 *      r6–r10 = variadic args  →  r3 = int characters written
 *
 * Same integer-only limitation as sprintf above.
 */
PPC_FUNC_IMPL(crt_hook_snprintf) {
    PPC_FUNC_PROLOGUE();
    char*       buf = GUEST_MSTR(ctx.r3);
    size_t      n   = (size_t)ctx.r4.u32;
    const char* fmt = GUEST_STR(ctx.r5);

    if (!buf || !fmt) {
        ctx.r3.s64 = -1;
        return;
    }

    ctx.r3.s64 = (int64_t)snprintf(buf, n, fmt,
                                    ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
                                    ctx.r9.u32, ctx.r10.u32);
}

/**
 * _vsnprintf hook — 0x825863CC
 *
 * PPC: r3 = char* buf,  r4 = size_t n,  r5 = const char* fmt,
 *      r6 = va_list* ap  →  r3 = int characters written
 *
 * va_list on this ABI is a 12-byte guest-memory struct — we cannot hand
 * it to the host's native vsnprintf. Instead we walk it slot-by-slot
 * through our format engine, which pulls integer/pointer args from the
 * GPR save area and double args from the FPR save area, falling back to
 * the overflow stack when either register class is exhausted. Layout
 * documented in ppc_varargs:: at the top of this file.
 */
PPC_FUNC_IMPL(crt_hook_vsnprintf) {
    PPC_FUNC_PROLOGUE();
    char*       buf   = GUEST_MSTR(ctx.r3);
    const size_t n    = (size_t)ctx.r4.u32;
    const char* fmt   = GUEST_STR(ctx.r5);
    const uint32_t ap = ctx.r6.u32;

    if (!buf || !fmt || n == 0 || ap == 0) {
        ctx.r3.s64 = -1;
        return;
    }

    ppc_varargs::PpcVaList va = ppc_varargs::resolve(ap);
    const int written = ppc_varargs::format_engine(buf, n, fmt, va);
    ctx.r3.s64 = (int64_t)written;
}

/* =====================================================================
 * HOOK TABLE
 * ===================================================================== */

/**
 * Global CRT hook table.
 *
 * The recomp host iterates this at startup and replaces each function
 * pointer in the dispatch table for `guest_addr` with `native_impl`.
 * Terminated by a zero-address sentinel.
 */
const CrtHookEntry g_crt_hook_table[] = {
    /* ── String ─────────────────────────────────────────────────── */
    { CRT_HOOK_ADDR_strlen,         crt_hook_strlen,          "strlen"         },
    { CRT_HOOK_ADDR_strcmp,         crt_hook_strcmp,          "strcmp"         },
    { CRT_HOOK_ADDR_strncmp,        crt_hook_strncmp,         "strncmp"        },
    { CRT_HOOK_ADDR_strncpy,        crt_hook_strncpy,         "strncpy"        },
    { CRT_HOOK_ADDR_wcslen,         crt_hook_wcslen,          "wcslen"         },

    /* ── Memory ──────────────────────────────────────────────────── */
    { CRT_HOOK_ADDR_memset,         crt_hook_memset,          "memset"         },
    { CRT_HOOK_ADDR_memset_aligned, crt_hook_memset_aligned,  "memset_aligned" },
    { CRT_HOOK_ADDR_memmove,        crt_hook_memmove,         "memmove"        },
    { CRT_HOOK_ADDR_memmove_back,   crt_hook_memmove_backward,"memmove_back"   },
    { CRT_HOOK_ADDR_memcpy,         crt_hook_memcpy,          "memcpy"         },

    /* ── Formatted output ────────────────────────────────────────── */
    { CRT_HOOK_ADDR_sprintf,        crt_hook_sprintf,         "sprintf"        },
    { CRT_HOOK_ADDR_snprintf,       crt_hook_snprintf,        "_snprintf"      },
    { CRT_HOOK_ADDR_vsnprintf,      crt_hook_vsnprintf,       "_vsnprintf"     },

    /* ── PPC VMX save/restore belt (compiler-emitted, no-op under recomp) ── */
    { CRT_HOOK_ADDR_savevmx_103,    ppc_helper_vmx_noop,      "__savevmx_103"  },
    { CRT_HOOK_ADDR_savevmx_104,    ppc_helper_vmx_noop,      "__savevmx_104"  },
    { CRT_HOOK_ADDR_savevmx_105,    ppc_helper_vmx_noop,      "__savevmx_105"  },
    { CRT_HOOK_ADDR_savevmx_106,    ppc_helper_vmx_noop,      "__savevmx_106"  },
    { CRT_HOOK_ADDR_savevmx_107,    ppc_helper_vmx_noop,      "__savevmx_107"  },
    { CRT_HOOK_ADDR_savevmx_108,    ppc_helper_vmx_noop,      "__savevmx_108"  },
    { CRT_HOOK_ADDR_savevmx_109,    ppc_helper_vmx_noop,      "__savevmx_109"  },
    { CRT_HOOK_ADDR_savevmx_110,    ppc_helper_vmx_noop,      "__savevmx_110"  },
    { CRT_HOOK_ADDR_savevmx_111,    ppc_helper_vmx_noop,      "__savevmx_111"  },
    { CRT_HOOK_ADDR_savevmx_112,    ppc_helper_vmx_noop,      "__savevmx_112"  },
    { CRT_HOOK_ADDR_savevmx_113,    ppc_helper_vmx_noop,      "__savevmx_113"  },
    { CRT_HOOK_ADDR_savevmx_114,    ppc_helper_vmx_noop,      "__savevmx_114"  },
    { CRT_HOOK_ADDR_savevmx_115,    ppc_helper_vmx_noop,      "__savevmx_115"  },
    { CRT_HOOK_ADDR_savevmx_116,    ppc_helper_vmx_noop,      "__savevmx_116"  },
    { CRT_HOOK_ADDR_savevmx_117,    ppc_helper_vmx_noop,      "__savevmx_117"  },
    { CRT_HOOK_ADDR_savevmx_118,    ppc_helper_vmx_noop,      "__savevmx_118"  },
    { CRT_HOOK_ADDR_savevmx_119,    ppc_helper_vmx_noop,      "__savevmx_119"  },
    { CRT_HOOK_ADDR_savevmx_120,    ppc_helper_vmx_noop,      "__savevmx_120"  },
    { CRT_HOOK_ADDR_savevmx_121,    ppc_helper_vmx_noop,      "__savevmx_121"  },
    { CRT_HOOK_ADDR_savevmx_122,    ppc_helper_vmx_noop,      "__savevmx_122"  },
    { CRT_HOOK_ADDR_savevmx_123,    ppc_helper_vmx_noop,      "__savevmx_123"  },
    { CRT_HOOK_ADDR_savevmx_124,    ppc_helper_vmx_noop,      "__savevmx_124"  },
    { CRT_HOOK_ADDR_savevmx_125,    ppc_helper_vmx_noop,      "__savevmx_125"  },
    { CRT_HOOK_ADDR_savevmx_126,    ppc_helper_vmx_noop,      "__savevmx_126"  },
    { CRT_HOOK_ADDR_savevmx_127,    ppc_helper_vmx_noop,      "__savevmx_127"  },

    { CRT_HOOK_ADDR_restvmx_14,     ppc_helper_vmx_noop,      "__restvmx_14"   },
    { CRT_HOOK_ADDR_restvmx_15,     ppc_helper_vmx_noop,      "__restvmx_15"   },
    { CRT_HOOK_ADDR_restvmx_16,     ppc_helper_vmx_noop,      "__restvmx_16"   },
    { CRT_HOOK_ADDR_restvmx_17,     ppc_helper_vmx_noop,      "__restvmx_17"   },
    { CRT_HOOK_ADDR_restvmx_18,     ppc_helper_vmx_noop,      "__restvmx_18"   },
    { CRT_HOOK_ADDR_restvmx_19,     ppc_helper_vmx_noop,      "__restvmx_19"   },
    { CRT_HOOK_ADDR_restvmx_20,     ppc_helper_vmx_noop,      "__restvmx_20"   },
    { CRT_HOOK_ADDR_restvmx_21,     ppc_helper_vmx_noop,      "__restvmx_21"   },
    { CRT_HOOK_ADDR_restvmx_22,     ppc_helper_vmx_noop,      "__restvmx_22"   },
    { CRT_HOOK_ADDR_restvmx_23,     ppc_helper_vmx_noop,      "__restvmx_23"   },
    { CRT_HOOK_ADDR_restvmx_24,     ppc_helper_vmx_noop,      "__restvmx_24"   },
    { CRT_HOOK_ADDR_restvmx_25,     ppc_helper_vmx_noop,      "__restvmx_25"   },
    { CRT_HOOK_ADDR_restvmx_26,     ppc_helper_vmx_noop,      "__restvmx_26"   },
    { CRT_HOOK_ADDR_restvmx_27,     ppc_helper_vmx_noop,      "__restvmx_27"   },
    { CRT_HOOK_ADDR_restvmx_28,     ppc_helper_vmx_noop,      "__restvmx_28"   },
    { CRT_HOOK_ADDR_restvmx_29,     ppc_helper_vmx_noop,      "__restvmx_29"   },
    { CRT_HOOK_ADDR_restvmx_30,     ppc_helper_vmx_noop,      "__restvmx_30"   },
    { CRT_HOOK_ADDR_restvmx_31,     ppc_helper_vmx_noop,      "__restvmx_31"   },
    { CRT_HOOK_ADDR_restvmx_64,     ppc_helper_vmx_noop,      "__restvmx_64"   },
    { CRT_HOOK_ADDR_restvmx_65,     ppc_helper_vmx_noop,      "__restvmx_65"   },
    { CRT_HOOK_ADDR_restvmx_66,     ppc_helper_vmx_noop,      "__restvmx_66"   },
    { CRT_HOOK_ADDR_restvmx_67,     ppc_helper_vmx_noop,      "__restvmx_67"   },
    { CRT_HOOK_ADDR_restvmx_68,     ppc_helper_vmx_noop,      "__restvmx_68"   },
    { CRT_HOOK_ADDR_restvmx_69,     ppc_helper_vmx_noop,      "__restvmx_69"   },
    { CRT_HOOK_ADDR_restvmx_70,     ppc_helper_vmx_noop,      "__restvmx_70"   },
    { CRT_HOOK_ADDR_restvmx_71,     ppc_helper_vmx_noop,      "__restvmx_71"   },
    { CRT_HOOK_ADDR_restvmx_72,     ppc_helper_vmx_noop,      "__restvmx_72"   },
    { CRT_HOOK_ADDR_restvmx_73,     ppc_helper_vmx_noop,      "__restvmx_73"   },
    { CRT_HOOK_ADDR_restvmx_74,     ppc_helper_vmx_noop,      "__restvmx_74"   },
    { CRT_HOOK_ADDR_restvmx_75,     ppc_helper_vmx_noop,      "__restvmx_75"   },
    { CRT_HOOK_ADDR_restvmx_76,     ppc_helper_vmx_noop,      "__restvmx_76"   },
    { CRT_HOOK_ADDR_restvmx_77,     ppc_helper_vmx_noop,      "__restvmx_77"   },
    { CRT_HOOK_ADDR_restvmx_78,     ppc_helper_vmx_noop,      "__restvmx_78"   },
    { CRT_HOOK_ADDR_restvmx_79,     ppc_helper_vmx_noop,      "__restvmx_79"   },
    { CRT_HOOK_ADDR_restvmx_80,     ppc_helper_vmx_noop,      "__restvmx_80"   },
    { CRT_HOOK_ADDR_restvmx_81,     ppc_helper_vmx_noop,      "__restvmx_81"   },
    { CRT_HOOK_ADDR_restvmx_82,     ppc_helper_vmx_noop,      "__restvmx_82"   },
    { CRT_HOOK_ADDR_restvmx_83,     ppc_helper_vmx_noop,      "__restvmx_83"   },
    { CRT_HOOK_ADDR_restvmx_84,     ppc_helper_vmx_noop,      "__restvmx_84"   },

    /* Sentinel */
    { 0, NULL, NULL }
};

/**
 * crt_hooks_register — Install all native CRT hooks
 *
 * Iterates g_crt_hook_table and calls the recomp host's
 * recomp_register_hook() for each entry. Returns the number of
 * hooks installed.
 *
 * Call once, before any translated game code executes.
 */
int crt_hooks_register(void) {
    int count = 0;
    for (const CrtHookEntry* e = g_crt_hook_table; e->guest_addr != 0; ++e) {
        if (recomp_register_hook(e->guest_addr, e->native_impl)) {
            ++count;
#if defined(CRT_HOOKS_VERBOSE)
            printf("[crt_hooks] hooked %-20s @ 0x%08X\n", e->name, e->guest_addr);
#endif
        }
    }
    return count;
}
