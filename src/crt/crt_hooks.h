/**
 * crt_hooks.h — Native CRT hook registration
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * The Xbox 360 ships its own C runtime compiled directly into the game
 * binary. Without hooks, routines like memcpy, strlen, and sprintf run
 * as translated PowerPC code through the recompiler — correct, but slow.
 *
 * This header defines:
 *   - The address table for every hooked CRT function
 *   - The CrtHookEntry descriptor type used for runtime registration
 *   - crt_hooks_register() — call once at startup before any game code runs
 *
 * ADDING A NEW HOOK
 * -----------------
 * 1. Add the address constant below (CRT_HOOK_ADDR_*).
 * 2. Implement a PPC_FUNC_IMPL wrapper in crt_hooks.cpp.
 * 3. Add an entry to g_crt_hook_table[] in crt_hooks.cpp.
 * 4. The registration loop in crt_hooks_register() handles the rest.
 *
 * MEMORY TRANSLATION
 * ------------------
 * Guest code passes 32-bit virtual addresses. The recomp runtime maps the
 * entire Xbox 360 address space into a contiguous host buffer. Use the
 * helper macros below to translate between guest and host pointers:
 *
 *   GUEST_PTR(r)   →  (void*)(g_mem_base + (r).u32)
 *   HOST_TO_GUEST(p) → (uint32_t)((uintptr_t)(p) - g_mem_base)
 *
 * NULL guest (r == 0) stays NULL; always guard against zero before use.
 *
 * PPC CALLING CONVENTION CHEAT-SHEET
 * ------------------------------------
 *   arg1 = ctx.r3    arg5 = ctx.r7    float arg1 = ctx.f1
 *   arg2 = ctx.r4    arg6 = ctx.r8    float arg2 = ctx.f2
 *   arg3 = ctx.r5    arg7 = ctx.r9
 *   arg4 = ctx.r6    arg8 = ctx.r10
 *   return (int/ptr) → ctx.r3
 *   return (float)   → ctx.f1
 */

#pragma once

#include <stdint.h>
#include "crt/ppc_recomp.h"   /* PPC_FUNC_IMPL, PPCRegContext, g_mem_base */

/* ── Guest address constants ──────────────────────────────────────── */

/* string.c */
#define CRT_HOOK_ADDR_strlen        0x820C0000u
#define CRT_HOOK_ADDR_strcmp        0x82432970u
#define CRT_HOOK_ADDR_strncmp       0x824320D0u   /* size 0x4c, verified via FLIRT */
#define CRT_HOOK_ADDR_strncpy       0x82430120u
#define CRT_HOOK_ADDR_wcslen        0x82433748u   /* size 0x24, verified via FLIRT */

/* memory.c */
#define CRT_HOOK_ADDR_memset        0x8242FED0u
#define CRT_HOOK_ADDR_memset_aligned 0x8242FEF0u
#define CRT_HOOK_ADDR_memmove       0x8242FF70u
#define CRT_HOOK_ADDR_memmove_back  0x8242FFACu
#define CRT_HOOK_ADDR_memcpy        0x82434100u

/* stdio / format */
#define CRT_HOOK_ADDR_snprintf      0x820C0B40u
#define CRT_HOOK_ADDR_sprintf       0x825863BCu
#define CRT_HOOK_ADDR_vsnprintf     0x825863CCu

/* ppc_helpers.c — VMX save/restore belt (compiler-emitted, see ppc_helpers.c).
 * Every entry is a single `stvx`/`lvx` + `blr`; canonical impl is a no-op
 * because the static recomp preserves v14..v127 through ctx automatically.
 *
 *   __savevmx belt: 0x82569450..0x825696E8 (covers v14..v31, v64..v127)
 *   __restvmx belt: 0x825696E8..0x82569980 (covers v14..v31, v64..v84)
 */
#define CRT_HOOK_ADDR_savevmx_14    0x82569450u
#define CRT_HOOK_ADDR_savevmx_15    0x82569458u
#define CRT_HOOK_ADDR_savevmx_16    0x82569460u
#define CRT_HOOK_ADDR_savevmx_17    0x82569468u
#define CRT_HOOK_ADDR_savevmx_18    0x82569470u
#define CRT_HOOK_ADDR_savevmx_19    0x82569478u
#define CRT_HOOK_ADDR_savevmx_20    0x82569480u
#define CRT_HOOK_ADDR_savevmx_21    0x82569488u
#define CRT_HOOK_ADDR_savevmx_22    0x82569490u
#define CRT_HOOK_ADDR_savevmx_23    0x82569498u
#define CRT_HOOK_ADDR_savevmx_24    0x825694A0u
#define CRT_HOOK_ADDR_savevmx_25    0x825694A8u
#define CRT_HOOK_ADDR_savevmx_26    0x825694B0u
#define CRT_HOOK_ADDR_savevmx_27    0x825694B8u
#define CRT_HOOK_ADDR_savevmx_28    0x825694C0u
#define CRT_HOOK_ADDR_savevmx_29    0x825694C8u
#define CRT_HOOK_ADDR_savevmx_30    0x825694D0u
#define CRT_HOOK_ADDR_savevmx_31    0x825694D8u
#define CRT_HOOK_ADDR_savevmx_64    0x825694E4u
#define CRT_HOOK_ADDR_savevmx_65    0x825694ECu
#define CRT_HOOK_ADDR_savevmx_66    0x825694F4u
#define CRT_HOOK_ADDR_savevmx_67    0x825694FCu
#define CRT_HOOK_ADDR_savevmx_68    0x82569504u
#define CRT_HOOK_ADDR_savevmx_69    0x8256950Cu
#define CRT_HOOK_ADDR_savevmx_70    0x82569514u
#define CRT_HOOK_ADDR_savevmx_71    0x8256951Cu
#define CRT_HOOK_ADDR_savevmx_72    0x82569524u
#define CRT_HOOK_ADDR_savevmx_73    0x8256952Cu
#define CRT_HOOK_ADDR_savevmx_74    0x82569534u
#define CRT_HOOK_ADDR_savevmx_75    0x8256953Cu
#define CRT_HOOK_ADDR_savevmx_76    0x82569544u
#define CRT_HOOK_ADDR_savevmx_77    0x8256954Cu
#define CRT_HOOK_ADDR_savevmx_78    0x82569554u
#define CRT_HOOK_ADDR_savevmx_79    0x8256955Cu
#define CRT_HOOK_ADDR_savevmx_80    0x82569564u
#define CRT_HOOK_ADDR_savevmx_81    0x8256956Cu
#define CRT_HOOK_ADDR_savevmx_82    0x82569574u
#define CRT_HOOK_ADDR_savevmx_83    0x8256957Cu
#define CRT_HOOK_ADDR_savevmx_84    0x82569584u
#define CRT_HOOK_ADDR_savevmx_85    0x8256958Cu
#define CRT_HOOK_ADDR_savevmx_86    0x82569594u
#define CRT_HOOK_ADDR_savevmx_87    0x8256959Cu
#define CRT_HOOK_ADDR_savevmx_88    0x825695A4u
#define CRT_HOOK_ADDR_savevmx_89    0x825695ACu
#define CRT_HOOK_ADDR_savevmx_90    0x825695B4u
#define CRT_HOOK_ADDR_savevmx_91    0x825695BCu
#define CRT_HOOK_ADDR_savevmx_92    0x825695C4u
#define CRT_HOOK_ADDR_savevmx_93    0x825695CCu
#define CRT_HOOK_ADDR_savevmx_94    0x825695D4u
#define CRT_HOOK_ADDR_savevmx_95    0x825695DCu
#define CRT_HOOK_ADDR_savevmx_96    0x825695E4u
#define CRT_HOOK_ADDR_savevmx_97    0x825695ECu
#define CRT_HOOK_ADDR_savevmx_98    0x825695F4u
#define CRT_HOOK_ADDR_savevmx_99    0x825695FCu
#define CRT_HOOK_ADDR_savevmx_100   0x82569604u
#define CRT_HOOK_ADDR_savevmx_101   0x8256960Cu
#define CRT_HOOK_ADDR_savevmx_102   0x82569614u
#define CRT_HOOK_ADDR_savevmx_103   0x8256961Cu
#define CRT_HOOK_ADDR_savevmx_104   0x82569624u
#define CRT_HOOK_ADDR_savevmx_105   0x8256962Cu
#define CRT_HOOK_ADDR_savevmx_106   0x82569634u
#define CRT_HOOK_ADDR_savevmx_107   0x8256963Cu
#define CRT_HOOK_ADDR_savevmx_108   0x82569644u
#define CRT_HOOK_ADDR_savevmx_109   0x8256964Cu
#define CRT_HOOK_ADDR_savevmx_110   0x82569654u
#define CRT_HOOK_ADDR_savevmx_111   0x8256965Cu
#define CRT_HOOK_ADDR_savevmx_112   0x82569664u
#define CRT_HOOK_ADDR_savevmx_113   0x8256966Cu
#define CRT_HOOK_ADDR_savevmx_114   0x82569674u
#define CRT_HOOK_ADDR_savevmx_115   0x8256967Cu
#define CRT_HOOK_ADDR_savevmx_116   0x82569684u
#define CRT_HOOK_ADDR_savevmx_117   0x8256968Cu
#define CRT_HOOK_ADDR_savevmx_118   0x82569694u
#define CRT_HOOK_ADDR_savevmx_119   0x8256969Cu
#define CRT_HOOK_ADDR_savevmx_120   0x825696A4u
#define CRT_HOOK_ADDR_savevmx_121   0x825696ACu
#define CRT_HOOK_ADDR_savevmx_122   0x825696B4u
#define CRT_HOOK_ADDR_savevmx_123   0x825696BCu
#define CRT_HOOK_ADDR_savevmx_124   0x825696C4u
#define CRT_HOOK_ADDR_savevmx_125   0x825696CCu
#define CRT_HOOK_ADDR_savevmx_126   0x825696D4u
#define CRT_HOOK_ADDR_savevmx_127   0x825696DCu

#define CRT_HOOK_ADDR_restvmx_14    0x825696E8u
#define CRT_HOOK_ADDR_restvmx_15    0x825696F0u
#define CRT_HOOK_ADDR_restvmx_16    0x825696F8u
#define CRT_HOOK_ADDR_restvmx_17    0x82569700u
#define CRT_HOOK_ADDR_restvmx_18    0x82569708u
#define CRT_HOOK_ADDR_restvmx_19    0x82569710u
#define CRT_HOOK_ADDR_restvmx_20    0x82569718u
#define CRT_HOOK_ADDR_restvmx_21    0x82569720u
#define CRT_HOOK_ADDR_restvmx_22    0x82569728u
#define CRT_HOOK_ADDR_restvmx_23    0x82569730u
#define CRT_HOOK_ADDR_restvmx_24    0x82569738u
#define CRT_HOOK_ADDR_restvmx_25    0x82569740u
#define CRT_HOOK_ADDR_restvmx_26    0x82569748u
#define CRT_HOOK_ADDR_restvmx_27    0x82569750u
#define CRT_HOOK_ADDR_restvmx_28    0x82569758u
#define CRT_HOOK_ADDR_restvmx_29    0x82569760u
#define CRT_HOOK_ADDR_restvmx_30    0x82569768u
#define CRT_HOOK_ADDR_restvmx_31    0x82569770u
#define CRT_HOOK_ADDR_restvmx_64    0x8256977Cu
#define CRT_HOOK_ADDR_restvmx_65    0x82569784u
#define CRT_HOOK_ADDR_restvmx_66    0x8256978Cu
#define CRT_HOOK_ADDR_restvmx_67    0x82569794u
#define CRT_HOOK_ADDR_restvmx_68    0x8256979Cu
#define CRT_HOOK_ADDR_restvmx_69    0x825697A4u
#define CRT_HOOK_ADDR_restvmx_70    0x825697ACu
#define CRT_HOOK_ADDR_restvmx_71    0x825697B4u
#define CRT_HOOK_ADDR_restvmx_72    0x825697BCu
#define CRT_HOOK_ADDR_restvmx_73    0x825697C4u
#define CRT_HOOK_ADDR_restvmx_74    0x825697CCu
#define CRT_HOOK_ADDR_restvmx_75    0x825697D4u
#define CRT_HOOK_ADDR_restvmx_76    0x825697DCu
#define CRT_HOOK_ADDR_restvmx_77    0x825697E4u
#define CRT_HOOK_ADDR_restvmx_78    0x825697ECu
#define CRT_HOOK_ADDR_restvmx_79    0x825697F4u
#define CRT_HOOK_ADDR_restvmx_80    0x825697FCu
#define CRT_HOOK_ADDR_restvmx_81    0x82569804u
#define CRT_HOOK_ADDR_restvmx_82    0x8256980Cu
#define CRT_HOOK_ADDR_restvmx_83    0x82569814u
#define CRT_HOOK_ADDR_restvmx_84    0x8256981Cu

/* Rtl / NT last-error helpers — TLS LastError slot lives at
 * PCR(r13)+256 -> ETHREAD + 352 (LastError DWORD). Four addresses:
 *   RtlSetLastError   — writes r3 into the slot when the guarded bit is 0
 *   RtlSetLastNTError — calls RtlNtStatusToDosError, then stores result
 *   RtlGetLastError   — reads the slot (or returns 0 when guarded)
 *   RtlSizeHeap       — RAGE-side heap block-size probe (calls
 *                       KeGetCurrentProcessType / KeBugCheckEx)
 * These are existing NT/Rtl leaf stubs; the hooks in crt_hooks.cpp
 * re-express them so translated call-sites skip the PPC body. */
#define CRT_HOOK_ADDR_RtlSetLastError    0x8242C318u
#define CRT_HOOK_ADDR_RtlSetLastNTError  0x8242C330u
#define CRT_HOOK_ADDR_RtlGetLastError    0x8242C368u
#define CRT_HOOK_ADDR_RtlSizeHeap        0x8242D3E0u

/* PPC GPR/LR save/restore belt — compiler-emitted, same fall-through
 * pattern as the VMX belt above. `__savegprlr` (0x8242F860, 80 bytes)
 * and `__restgprlr` (0x8242F8B0, 84 bytes) are each a sequence of
 * `std rN,-(ofs)(r1)` / `ld rN,-(ofs)(r1)` + `std r0,16(r1)` /
 * `ld r0,16(r1)` + `blr`. FLIRT exposes every 4-byte interior offset
 * as a separate `_14`..`_31` alias. The static recomp preserves
 * ctx.r14..r31 and ctx.lr across any call because callees operate on
 * the same ctx block, so the belt has no guest-visible effect. We
 * route every interior entry through the same no-op
 * (`ppc_helper_gprlr_noop` in ppc_helpers.c). */
#define CRT_HOOK_ADDR_savegprlr_14    0x8242F860u
#define CRT_HOOK_ADDR_savegprlr_15    0x8242F864u
#define CRT_HOOK_ADDR_savegprlr_16    0x8242F868u
#define CRT_HOOK_ADDR_savegprlr_17    0x8242F86Cu
#define CRT_HOOK_ADDR_savegprlr_18    0x8242F870u
#define CRT_HOOK_ADDR_savegprlr_19    0x8242F874u
#define CRT_HOOK_ADDR_savegprlr_20    0x8242F878u
#define CRT_HOOK_ADDR_savegprlr_21    0x8242F87Cu
#define CRT_HOOK_ADDR_savegprlr_22    0x8242F880u
#define CRT_HOOK_ADDR_savegprlr_23    0x8242F884u
#define CRT_HOOK_ADDR_savegprlr_31    0x8242F8A4u

#define CRT_HOOK_ADDR_restgprlr_14    0x8242F8B0u
#define CRT_HOOK_ADDR_restgprlr_15    0x8242F8B4u
#define CRT_HOOK_ADDR_restgprlr_16    0x8242F8B8u
#define CRT_HOOK_ADDR_restgprlr_17    0x8242F8BCu
#define CRT_HOOK_ADDR_restgprlr_18    0x8242F8C0u
#define CRT_HOOK_ADDR_restgprlr_19    0x8242F8C4u
#define CRT_HOOK_ADDR_restgprlr_20    0x8242F8C8u
#define CRT_HOOK_ADDR_restgprlr_21    0x8242F8CCu
#define CRT_HOOK_ADDR_restgprlr_22    0x8242F8D0u

/* ── Translation helpers ──────────────────────────────────────────── */

/* Translate a 32-bit guest address in a ctx register to a host pointer. */
#define GUEST_PTR(reg)        ( (reg).u32 ? (void*)((uintptr_t)g_mem_base + (reg).u32) : NULL )
#define GUEST_CPTR(reg)       ( (reg).u32 ? (const void*)((uintptr_t)g_mem_base + (reg).u32) : NULL )
#define GUEST_STR(reg)        ( (reg).u32 ? (const char*)((uintptr_t)g_mem_base + (reg).u32) : NULL )
#define GUEST_MSTR(reg)       ( (reg).u32 ? (char*)((uintptr_t)g_mem_base + (reg).u32) : NULL )

/* Translate a host pointer back to a 32-bit guest address. */
#define HOST_TO_GUEST(ptr)    ( (uint32_t)((uintptr_t)(ptr) - (uintptr_t)g_mem_base) )

/* ── Hook registration ────────────────────────────────────────────── */

/**
 * Descriptor for a single native CRT hook.
 * The recomp host replaces the function pointer for `guest_addr` with
 * `native_impl` so that translated call-sites go directly to native code.
 */
typedef struct {
    uint32_t        guest_addr;   /**< PPC virtual address being replaced    */
    PPC_FUNC_PTR    native_impl;  /**< Replacement PPC_FUNC_IMPL to install  */
    const char*     name;         /**< Human-readable name for logging       */
} CrtHookEntry;

/**
 * Install all native CRT hooks.
 *
 * Call this once, very early in the host init sequence — before any
 * translated game code runs. Safe to call multiple times (idempotent).
 *
 * @return  Number of hooks successfully installed.
 */
int crt_hooks_register(void);

/**
 * The global hook table (defined in crt_hooks.cpp).
 * Terminated by an entry with guest_addr == 0.
 */
extern const CrtHookEntry g_crt_hook_table[];
