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
#define CRT_HOOK_ADDR_strncmp       0x82432A00u   /* TODO: verify size */
#define CRT_HOOK_ADDR_strncpy       0x82430120u

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
