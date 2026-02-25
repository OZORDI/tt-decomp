/**
 * heap.c — RAGE heap allocator wrappers (CRT layer)
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * NATIVE CRT HOOKS: These functions bypass the PowerPC recomp layer and call
 * native malloc/free directly for performance. This is the "native CRT hooks"
 * optimization mentioned in the ReXGlue SDK documentation.
 *
 * Original Xbox 360 behavior:
 *   - Allocator pointer lives at SDA[0][4] (thread-local slot)
 *   - Vtable dispatch to PowerPC allocator functions
 *   - Complex ownership tracking via atSingleton
 *
 * Native implementation:
 *   - Direct calls to native malloc/free
 *   - Simplified ownership model (native heap owns everything)
 *   - Maintains ABI compatibility with original function signatures
 *
 * sysMemAllocator vtable layout (original Xbox 360):
 *   slot  1 (+4)  : Allocate(void* ptr, size_t size, size_t align)
 *   slot  2 (+8)  : Free(void* ptr)
 *   slot 17 (+68) : IsAddressOwned(void* ptr) → bool
 *
 * MEMORY FUNCTION HIERARCHY:
 *   Level 1: Standard CRT (memory.c) - malloc/free/memset/memcpy
 *   Level 2: CRT Wrappers (crt_crossplatform.h) - crt_malloc/crt_free
 *   Level 3: RAGE Allocator (heap.c) - rage_free, sysMemAllocator_*
 *   Level 4: C++ Interface (rage/allocator.h) - rage::Allocator class
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>  /* malloc, free */
#include "rage/memory.h"

/* Forward declarations */
bool atSingleton_Find_90D0(void* ptr);  /* returns nonzero if ptr belongs to another singleton */

/* ============================================================================
 * NATIVE CRT HOOKS - Vtable Wrapper Functions
 * ============================================================================
 * These replace the original PowerPC vtable dispatch with direct native calls.
 * Performance benefit: native x86/ARM code instead of translated PowerPC.
 */

/**
 * sysMemAllocator_vtable_IsOwned
 * 
 * Native CRT hook: Always returns false to let native allocator handle everything.
 * Original behavior: Checked if allocator owned a specific address range.
 * 
 * @param allocator  Allocator instance (unused in native implementation)
 * @return           Always false (native heap owns all allocations)
 */
static inline bool sysMemAllocator_vtable_IsOwned(void* allocator) {
    (void)allocator;  /* unused */
    return false;     /* native heap handles everything */
}

/**
 * sysMemAllocator_vtable_Allocate
 * 
 * Native CRT hook: Calls native malloc() directly.
 * Original behavior: Vtable dispatch to PowerPC allocator function.
 * 
 * @param allocator  Allocator instance (unused in native implementation)
 * @param ptr        Hint address (unused in native implementation)
 * @param size       Allocation size in bytes
 * @return           Pointer to allocated memory, or NULL on failure
 */
static inline void* sysMemAllocator_vtable_Allocate(void* allocator, void* ptr, size_t size) {
    (void)allocator;  /* unused */
    (void)ptr;        /* unused */
    return malloc(size);
}

/**
 * sysMemAllocator_vtable_Free
 * 
 * Native CRT hook: Calls native free() directly.
 * Original behavior: Vtable dispatch to PowerPC free function.
 * 
 * @param allocator  Allocator instance (unused in native implementation)
 * @param ptr        Pointer to free
 */
static inline void sysMemAllocator_vtable_Free(void* allocator, void* ptr) {
    (void)allocator;  /* unused */
    free(ptr);
}


/* ============================================================================
 * RAGE Allocator Functions - Native CRT Implementation
 * ============================================================================
 */

/**
 * sysMemAllocator_Allocate @ 0x821861A0 | size: 0x84
 *
 * Allocate memory from the RAGE heap allocator.
 * 
 * NATIVE CRT HOOK: Simplified to call malloc() directly instead of complex
 * vtable dispatch and ownership checking.
 *
 * @param ptr   Hint/base address (unused in native implementation)
 * @param size  Requested allocation size in bytes
 * @return      Pointer to allocated memory, or NULL on failure
 */
void* sysMemAllocator_Allocate(void* ptr, size_t size)
{
    (void)ptr;  /* unused in native implementation */
    return malloc(size);
}


/**
 * sysMemAllocator_Free @ 0x82186228 | size: 0x74
 *
 * Free a pointer previously returned by sysMemAllocator_Allocate.
 *
 * NATIVE CRT HOOK: Simplified to call free() directly with singleton checks.
 *
 * Guards:
 *  1. NULL check — silently ignores NULL.
 *  2. atSingleton_Find_90D0 — if the pointer belongs to another singleton
 *     subsystem (e.g. network pool), skip the free to avoid double-free.
 *
 * @param ptr  User pointer to free
 */
void sysMemAllocator_Free(void* ptr)
{
    if (!ptr)
        return;

    /* Check atSingleton ownership — if another subsystem owns this
     * pointer, we must not free it here */
    if (atSingleton_Find_90D0(ptr))
        return;

    free(ptr);
}


/**
 * rage_free @ 0x820C00C0 | size: 0x60
 *
 * The canonical RAGE heap free function.
 *
 * NATIVE CRT HOOK: Calls native free() directly instead of vtable dispatch.
 *
 * Performs two safety guards:
 *  1. NULL check — silently ignores NULL pointers.
 *  2. atSingleton_Find_90D0 check — skips free if pointer belongs to
 *     another singleton's memory region (prevents cross-heap corruption).
 *
 * C++ code should use rage::Free(ptr) wrapper from rage/memory.h.
 *
 * @param ptr  Raw heap pointer to free
 */
void rage_free(void* ptr)
{
    if (!ptr)
        return;

    /* Skip if the pointer is owned by a different singleton memory region */
    if (atSingleton_Find_90D0(ptr))
        return;

    /* Native CRT hook: call free() directly */
    free(ptr);
}
