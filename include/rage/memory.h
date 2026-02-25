#pragma once

/**
 * memory.h — RAGE Engine Memory Management
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Core memory allocation functions for the RAGE engine.
 * These wrap the sysMemAllocator vtable-based system with
 * singleton ownership checks to prevent cross-heap corruption.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rage_free_00C0 @ 0x820C00C0 | size: 0x60
 *
 * The canonical RAGE heap free function.
 *
 * Safety guards:
 *  1. NULL check — silently ignores NULL pointers
 *  2. atSingleton ownership check — skips free if pointer belongs
 *     to another singleton's memory region (prevents cross-heap corruption)
 *  3. Forwards to active sysMemAllocator vtable slot 2 (Free method)
 *
 * This is NOT the same as crt_free():
 *  - rage_free_00C0 checks singleton ownership
 *  - rage_free_00C0 uses RAGE allocator vtable from TLS
 *  - crt_free() is a simple wrapper for standard free()
 *
 * @param ptr  Raw heap pointer to free (may be NULL)
 */
void rage_free_00C0(void* ptr);

/**
 * sysMemAllocator_Allocate_61A0 @ 0x821861A0 | size: 0x84
 *
 * Allocate memory from the active RAGE heap allocator.
 *
 * Checks vtable slot 17 (IsAddressOwned) to decide allocation path:
 *  - If allocator owns the address range: use vtable slot 1 (Allocate)
 *  - Otherwise: call xe_EC88 for raw block, store raw ptr at [-4]
 *
 * @param ptr   Hint/base address for allocation (may be NULL)
 * @param size  Requested allocation size in bytes
 * @return      Aligned user pointer, or NULL on failure
 */
void* sysMemAllocator_Allocate(void* ptr, size_t size);

/**
 * sysMemAllocator_Free_6228 @ 0x82186228 | size: 0x74
 *
 * Free a pointer previously returned by sysMemAllocator_Allocate.
 *
 * Guards:
 *  1. NULL check
 *  2. atSingleton ownership check
 *  3. Recovers raw pointer from [-4] and calls rage_free_00C0
 *
 * @param ptr  User pointer to free (as returned by Allocate)
 */
void sysMemAllocator_Free(void* ptr);

#ifdef __cplusplus
}

// C++ namespace wrapper for cleaner call sites
namespace rage {
    /**
     * Free memory allocated by the RAGE allocator system.
     * Wrapper for rage_free_00C0 with C++ namespace.
     */
    inline void Free(void* ptr) {
        rage_free_00C0(ptr);
    }
}
#endif
