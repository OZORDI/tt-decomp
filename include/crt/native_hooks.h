/**
 * Native CRT Hooks Header
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Hook table and declarations for native CRT function replacements.
 * This allows ReXGlue to redirect CRT calls to optimized host implementations.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Native CRT Function Declarations
// ============================================================================

// String functions
size_t NATIVE_strlen(const char* str);

// Memory functions
void* NATIVE_memcpy(void* dest, const void* src, size_t count);
void* NATIVE_memset(void* dest, int value, size_t count);
void* NATIVE_memset_aligned(void* dest, int value, size_t count);
void* NATIVE_memmove(void* dest, const void* src, size_t count);
int NATIVE_memcmp(const void* ptr1, const void* ptr2, size_t count);

// Memory allocation
void* NATIVE_malloc(size_t size);
void NATIVE_free(void* ptr);
void* NATIVE_calloc(size_t num, size_t size);
void* NATIVE_realloc(void* ptr, size_t new_size);

// ============================================================================
// CRT Hook Table Structure
// ============================================================================

typedef struct {
    uint32_t ppc_addr;          // Original PowerPC address
    const char* name;           // Function name
    void* native_impl;          // Native implementation pointer
    uint32_t ppc_size;          // Size of original PPC function
    const char* source_file;    // Source file from splits.txt
} CRTHook;

// ============================================================================
// CRT Hook Table
// Auto-generated from splits.txt and symbols.txt
// Total: 31 CRT functions identified, 5 with native implementations
// ============================================================================

static const CRTHook g_crt_hooks[] = {
    // src/crt/entry.c (5 functions - Xbox 360 specific, no native hooks)
    { 0x820C0038, "xe_main_thread_init_0038", NULL, 0x88, "src/crt/entry.c" },
    { 0x820C0128, "__crt_main_entry", NULL, 0x44, "src/crt/entry.c" },
    { 0x82186CA8, "xe_alloc_thread_ctx_6CA8", NULL, 0x94, "src/crt/entry.c" },
    { 0x82186D40, "xe_thread_ctx_init_6D40", NULL, 0x58, "src/crt/entry.c" },
    { 0x825836E8, "xe_get_thread_ctx_36E8", NULL, 0xC, "src/crt/entry.c" },

    // src/crt/exit.c (3 functions - CRT lifecycle, no native hooks)
    { 0x8242F6B0, "_onexit", NULL, 0x100, "src/crt/exit.c" },
    { 0x8242F7D0, "atexit", NULL, 0x2C, "src/crt/exit.c" },
    { 0x8242F9D0, "_doexit", NULL, 0x10C, "src/crt/exit.c" },

    // src/crt/heap.c (6 functions - Xbox 360 allocators, no native hooks)
    { 0x821861A0, "sysMemAllocator_Allocate_61A0", NULL, 0x84, "src/crt/heap.c" },
    { 0x82186228, "sysMemAllocator_Free_6228", NULL, 0x74, "src/crt/heap.c" },
    { 0x82186E40, "sysMemSimpleAllocator_Allocate", NULL, 0x338, "src/crt/heap.c" },
    { 0x821871C0, "sysMemSimpleAllocator_Free", NULL, 0x314, "src/crt/heap.c" },
    { 0x8242C178, "_crt_critsec_init", NULL, 0x6C, "src/crt/heap.c" },
    { 0x8242C1E8, "_heap_init_impl", NULL, 0xD8, "src/crt/heap.c" },

    // src/crt/memory.c (3 functions - ✅ NATIVE HOOKS AVAILABLE)
    { 0x8242FED0, "memset", (void*)NATIVE_memset, 0x20, "src/crt/memory.c" },
    { 0x8242FF70, "memmove", (void*)NATIVE_memmove, 0x3C, "src/crt/memory.c" },
    { 0x82434100, "memcpy", (void*)NATIVE_memcpy, 0x420, "src/crt/memory.c" },

    // src/crt/startup.c (11 functions - CRT initialization, no native hooks)
    { 0x8242BD20, "__mainCRTStartup", NULL, 0x1C8, "src/crt/startup.c" },
    { 0x8242BEE8, "except_AA94_2", NULL, 0x24, "src/crt/startup.c" },
    { 0x8242BF10, "thunk_game_C330", NULL, 0x4, "src/crt/startup.c" },
    { 0x8242BF18, "pg_BF18_g", NULL, 0x98, "src/crt/startup.c" },
    { 0x8242BFB0, "_get_xex_cmdline", NULL, 0xC, "src/crt/startup.c" },
    { 0x8242BFC0, "_initterm_e2", NULL, 0xE0, "src/crt/startup.c" },
    { 0x8242C0A0, "_initterm", NULL, 0x74, "src/crt/startup.c" },
    { 0x8242C118, "_initterm_e", NULL, 0x60, "src/crt/startup.c" },
    { 0x8242C2C0, "_heap_init_check", NULL, 0x58, "src/crt/startup.c" },
    { 0x8242FB60, "_doexit_entry", NULL, 0x10, "src/crt/startup.c" },
    { 0x8242FDC8, "_cinit_setup", NULL, 0x108, "src/crt/startup.c" },

    // src/crt/string.c (2 functions - ✅ strlen has native hook)
    { 0x820C0000, "strlen", (void*)NATIVE_strlen, 0x38, "src/crt/string.c" },
    { 0x82186358, "rage_stricmp_6358", NULL, 0xC0, "src/crt/string.c" },

    // src/crt/version.c (1 function - Xbox 360 specific, no native hook)
    { 0x8242BB70, "_check_xdk_version", NULL, 0x1A4, "src/crt/version.c" },
    
    // Sentinel
    { 0, NULL, NULL, 0, NULL }
};

#define CRT_HOOK_COUNT (sizeof(g_crt_hooks) / sizeof(CRTHook) - 1)

// ============================================================================
// Hook Lookup Functions
// ============================================================================

/**
 * Find a CRT hook by PowerPC address.
 * Returns NULL if no hook exists for this address.
 */
static inline const CRTHook* crt_find_hook_by_addr(uint32_t ppc_addr) {
    for (size_t i = 0; i < CRT_HOOK_COUNT; i++) {
        if (g_crt_hooks[i].ppc_addr == ppc_addr) {
            return &g_crt_hooks[i];
        }
    }
    return NULL;
}

/**
 * Find a CRT hook by function name.
 * Returns NULL if no hook exists for this name.
 */
static inline const CRTHook* crt_find_hook_by_name(const char* name) {
    if (!name) return NULL;
    
    for (size_t i = 0; i < CRT_HOOK_COUNT; i++) {
        if (g_crt_hooks[i].name && strcmp(g_crt_hooks[i].name, name) == 0) {
            return &g_crt_hooks[i];
        }
    }
    return NULL;
}

/**
 * Check if an address should be hooked.
 */
static inline int crt_should_hook(uint32_t ppc_addr) {
    return crt_find_hook_by_addr(ppc_addr) != NULL;
}

// ============================================================================
// Statistics and Debugging
// ============================================================================

typedef struct {
    uint64_t total_calls;       // Total CRT function calls
    uint64_t hooked_calls;      // Calls redirected to native
    uint64_t translated_calls;  // Calls using translated PPC
    uint64_t cycles_saved;      // Estimated cycles saved by hooks
} CRTHookStats;

// Global statistics (optional, for profiling)
extern CRTHookStats g_crt_hook_stats;

/**
 * Initialize CRT hook statistics.
 */
static inline void crt_hook_stats_init(void) {
    g_crt_hook_stats.total_calls = 0;
    g_crt_hook_stats.hooked_calls = 0;
    g_crt_hook_stats.translated_calls = 0;
    g_crt_hook_stats.cycles_saved = 0;
}

/**
 * Print CRT hook statistics.
 */
void crt_hook_stats_print(void);

#ifdef __cplusplus
}
#endif
