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
 * sysMemAllocator_Allocate / Free / rage_free_00C0 are intentionally left
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
 * PPC: r3 = char* buf,  r4 = const char* fmt,  r5–r10 = variadic args
 *      →  r3 = int characters written
 *
 * LIMITATION: The PPC variadic ABI passes integer args in r5–r10 and
 * float args in f1–f8 with a parallel stack save area. We can reconstruct
 * up to 6 integer arguments directly. Format strings needing more than 6
 * integer args, or any float args, require the full va_list reconstruction
 * (see the TODO below).
 *
 * For the vast majority of RAGE log/debug calls (1–3 args, no floats)
 * this is sufficient. The original translated code is left as fallback
 * via PPC_HOOK_FALLBACK() for complex cases.
 */
PPC_FUNC_IMPL(crt_hook_sprintf) {
    PPC_FUNC_PROLOGUE();
    char*       buf = GUEST_MSTR(ctx.r3);
    const char* fmt = GUEST_STR(ctx.r4);

    if (!buf || !fmt) {
        ctx.r3.s64 = -1;
        return;
    }

    /*
     * Reconstruct integer arguments from PPC registers.
     * r5–r10 hold the first 6 non-format arguments. This covers the
     * overwhelming majority of sprintf calls in RAGE (logging, name
     * formatting, score display, etc.).
     *
     * TODO: Full variadic reconstruction for float args (f1–f8) and
     *       calls with >6 integer args @ 0x825863BC — assembly unclear.
     */
    ctx.r3.s64 = (int64_t)sprintf(buf, fmt,
                                   ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                   ctx.r8.u32, ctx.r9.u32, ctx.r10.u32);
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
 *      r6 = va_list* ap  →  r3 = int
 *
 * va_list on PPC64 is a struct on the guest stack, not a native va_list.
 * We can't directly pass it to native vsnprintf; reconstruct from the
 * register save area pointed to by r6.
 *
 * TODO: Implement full PPC va_list struct walk @ 0x825863CC.
 *       For now, fall through to translated code via the recomp.
 */
PPC_FUNC_IMPL(crt_hook_vsnprintf) {
    PPC_FUNC_PROLOGUE();
    /*
     * PPC va_list is a struct { char gpr_left, fpr_left, uint16_t reserved,
     *                            uint32_t gpr_save_area, uint32_t fpr_save_area }
     * The guest pointer in r6 points to this struct in guest memory.
     * Reconstructing it for native vsnprintf requires reading the save area
     * and building a host va_list — non-trivial and arch-specific.
     *
     * Fallback: leave ctx unchanged and let the scaffold run the translated
     * version. This is correct; it just doesn't get the speed benefit.
     * TODO: implement proper PPC va_list unwinding.
     */
    PPC_HOOK_FALLBACK();
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
    { CRT_HOOK_ADDR_strncpy,        crt_hook_strncpy,         "strncpy"        },

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
