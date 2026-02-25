/**
 * heap.c — RAGE heap allocator wrappers (CRT layer)
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * These are thin wrappers around the active sysMemAllocator instance.
 * The allocator pointer lives at SDA[0][4] — effectively a thread-local
 * slot that's set up by xe_main_thread_init_0038 at startup.
 *
 * sysMemAllocator vtable layout (relevant slots):
 *   slot  1 (+4)  : Allocate(void* ptr, size_t size, size_t align)
 *   slot  2 (+8)  : Free(void* ptr)
 *   slot 17 (+68) : IsAddressOwned(void* ptr) → bool
 *
 * Allocation layout:
 *   Each allocation stores the raw heap pointer 4 bytes before the
 *   returned user pointer:  [raw_ptr][...user data...]
 *                            ^-4      ^returned
 *   Free reads [-4] to recover the raw pointer for the underlying free.
 *
 * rage_free is the canonical "safe free": it checks atSingleton
 * ownership before freeing so it won't corrupt memory from another
 * heap subsystem.
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
#include "rage/memory.h"

/* SDA-relative allocator context.  SDA[0] points to a struct where
 * offset +4 holds the active sysMemAllocator*. */
extern uint32_t* g_sda_base;   /* r13-relative, @ 0x82600000 */

/* Forward declarations */
bool atSingleton_Find_90D0(void* ptr);  /* returns nonzero if ptr belongs to another singleton */
void xe_EC88(uint32_t total, void* base);


/**
 * sysMemAllocator_Allocate_61A0 @ 0x821861A0 | size: 0x84
 *
 * Allocate `size` bytes with `align` alignment from the active RAGE heap.
 *
 * Checks vtable slot 17 (IsAddressOwned) to decide which heap path to use:
 *   - If the allocator claims ownership of (ptr + size): use vtable slot 1
 *     (the allocator's own Allocate method).
 *   - Otherwise: call xe_EC88 to obtain a raw block, store the raw pointer
 *     at [-4], then return the aligned user pointer.
 *
 * @param ptr   Hint/base address for the allocation (may be 0)
 * @param size  Requested allocation size in bytes
 * @return      Aligned user pointer, or NULL on failure
 */
void* sysMemAllocator_Allocate(void* ptr, size_t size)
{
    uint32_t* pAllocCtx = (uint32_t*)g_sda_base[0];
    void*     pAlloc    = (void*)pAllocCtx[1];      /* active sysMemAllocator* */

    /* Ask the allocator if it owns the address range we need */
    bool owned = sysMemAllocator_vtable_IsOwned(pAlloc);  /* vslot 17 */

    if (owned) {
        /* Delegated allocation — let the allocator handle it directly */
        return sysMemAllocator_vtable_Allocate(pAlloc, ptr, size);  /* vslot 1 */
    }

    /* Direct heap path: get a raw block, store raw ptr at [-4] for Free */
    uintptr_t raw  = (uintptr_t)xe_EC88((uint32_t)(uintptr_t)ptr + (uint32_t)size, NULL);
    uintptr_t aligned = (raw + size + (size - 1)) & ~(size - 1);
    *(uint32_t*)(aligned - 4) = (uint32_t)raw;
    return (void*)aligned;
}


/**
 * sysMemAllocator_Free_6228 @ 0x82186228 | size: 0x74
 *
 * Free a pointer previously returned by sysMemAllocator_Allocate.
 *
 * Guards:
 *  1. NULL check — silently ignores NULL.
 *  2. atSingleton_Find_90D0 — if the pointer belongs to another singleton
 *     subsystem (e.g. network pool), skip the free to avoid double-free.
 *  3. Reads the raw pointer from [-4] and calls rage_free.
 *
 * @param ptr  User pointer to free (as returned by Allocate)
 */
void sysMemAllocator_Free(void* ptr)
{
    uint32_t* pAllocCtx = (uint32_t*)g_sda_base[0];
    void*     pAlloc    = (void*)pAllocCtx[1];

    /* Check if the allocator claims this address */
    bool owned = sysMemAllocator_vtable_IsOwned(pAlloc);  /* vslot 17 */

    if (!owned) {
        /* Check atSingleton ownership — if another subsystem owns this
         * pointer, we must not free it here */
        if (atSingleton_Find_90D0(ptr))
            return;

        if (!ptr)
            return;

        /* Recover the raw heap pointer stored at user_ptr[-4] */
        void* rawPtr = (void*)(uintptr_t)(*(uint32_t*)((uintptr_t)ptr - 4));
        rage_free(rawPtr);
    }
}


/**
 * rage_free @ 0x820C00C0 | size: 0x60
 *
 * The canonical RAGE heap free function. Forwards to the active
 * sysMemAllocator's vtable slot 2 (Free).
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

    /* Load the active allocator from SDA[0][+4] and call vslot 2 (Free) */
    uint32_t* pAllocCtx = (uint32_t*)g_sda_base[0];
    void*     pAlloc    = (void*)pAllocCtx[1];
    sysMemAllocator_vtable_Free(pAlloc, ptr);   /* vslot 2 */
}
