/**
 * heap.c — RAGE heap allocator functions (CRT layer)
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * These functions implement the RAGE engine's memory allocation layer.
 * They sit above the raw CRT (malloc/free) and below the C++ rage::Allocator
 * interface. All three use vtable dispatch through the global sysMemAllocator
 * instance stored in the SDA (Small Data Area) at r13-relative offset 0.
 *
 * sysMemAllocator vtable layout:
 *   slot  0 (+0)  : destructor
 *   slot  1 (+4)  : Allocate(self, ptr, size)
 *   slot  2 (+8)  : Free(self, ptr)
 *   slot 17 (+68) : IsAddressOwned(self) -> bool
 *
 * The allocator pointer is obtained via:
 *   allocator = *(uint32_t*)(SDA_base[0] + 4)
 * where SDA_base is the value at r13+0 (global allocator context).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "rage/rage_allocator.h"
#include "rage/singleton.h"

/* ── External declarations ──────────────────────────────────────────── */

/* Raw memory allocation (low-level, below the sysMemAllocator layer).
 * Used by sysMemAllocator_Allocate when IsAddressOwned returns false. */
extern void* xe_EC88(uint32_t size);                /* @ 0x820DEC88 */

/* Global allocator context — the value at SDA[r13+0].
 * Dereferencing offset +4 from this yields the active sysMemAllocator*. */
extern rageAllocator* g_pAllocator;                 /* @ SDA:0x82600000 + 4 */


/* ============================================================================
 * rage_free @ 0x820C00C0 | size: 0x60
 *
 * The canonical RAGE heap free function.
 *
 * Guards:
 *  1. NULL check — silently ignores NULL pointers.
 *  2. atSingleton ownership check — if the pointer belongs to a registered
 *     singleton, the free is silently skipped (prevents cross-heap corruption).
 *  3. Dispatches to the global allocator's virtual Free method (vtable slot 2).
 *
 * C++ code should use rage::Free(ptr) from rage/memory.h.
 * ============================================================================
 */
void rage_free(void* ptr)
{
    if (!ptr)
        return;

    /* If the singleton registry claims this pointer, don't free it */
    uint8_t singletonStatus = rage_FindSingleton(ptr);
    if (singletonStatus != 0)
        return;

    /* Dispatch to the global allocator's Free method (vtable slot 2) */
    g_pAllocator->vtable->Free(g_pAllocator, ptr);
}


/* ============================================================================
 * sysMemAllocator_Allocate @ 0x821861A0 | size: 0x84
 *
 * Allocate memory from the active RAGE heap allocator.
 *
 * Checks vtable slot 17 (IsAddressOwned) on the active allocator:
 *   - If the allocator owns this address range: dispatch to vtable slot 1
 *     (Allocate) with the ptr hint and size.
 *   - Otherwise: call xe_EC88 to get a raw block of (ptr + size) bytes,
 *     then align the result up to `size` boundary and store the raw pointer
 *     at user_ptr[-4] for later recovery during Free.
 *
 * @param ptr   Alignment hint / base address
 * @param size  Requested allocation size in bytes
 * @return      Aligned user pointer, or raw pointer from xe_EC88
 * ============================================================================
 */
void* sysMemAllocator_Allocate(void* ptr, size_t size)
{
    /* Get the active allocator from the SDA context */
    struct rageAllocator* allocator = (struct rageAllocator*)g_pAllocator;

    /* Check if the allocator owns this address range (vtable slot 17) */
    bool isOwned = allocator->vtable->IsAddressOwned(allocator);

    if (isOwned) {
        /* Owned: use the allocator's Allocate method (vtable slot 1) */
        return allocator->vtable->Allocate(allocator, ptr, size);
    }

    /* Not owned: raw allocation via xe_EC88 with manual alignment.
     * Allocate (ptr + size) bytes to leave room for alignment. */
    uint32_t rawSize = (uint32_t)((uintptr_t)ptr + size);
    void* rawPtr = xe_EC88(rawSize);

    /* Align: userPtr = (rawPtr + size) & ~(size - 1) */
    uintptr_t aligned = ((uintptr_t)rawPtr + size) & ~(size - 1);

    /* Store the raw pointer at userPtr[-4] for recovery during Free */
    ((uint32_t*)aligned)[-1] = (uint32_t)(uintptr_t)rawPtr;

    return (void*)aligned;
}


/* ============================================================================
 * sysMemAllocator_Free @ 0x82186228 | size: 0x74
 *
 * Free a pointer previously returned by sysMemAllocator_Allocate.
 *
 * Checks vtable slot 17 (IsAddressOwned) on the active allocator:
 *   - If owned (nonzero): return immediately (allocator manages its own memory).
 *   - If not owned (zero): perform singleton check and NULL check, then
 *     recover the raw pointer from user_ptr[-4] and pass it to rage_free.
 *
 * @param ptr  User pointer to free (as returned by sysMemAllocator_Allocate)
 * ============================================================================
 */
void sysMemAllocator_Free(void* ptr)
{
    /* Get the active allocator from the SDA context */
    struct rageAllocator* allocator = (struct rageAllocator*)g_pAllocator;

    /* Check if the allocator owns this address range (vtable slot 17) */
    bool isOwned = allocator->vtable->IsAddressOwned(allocator);

    if (!isOwned) {
        /* Not owned by allocator — check singleton registry */
        uint8_t singletonStatus = rage_FindSingleton(ptr);
        if (singletonStatus != 0)
            return;

        if (!ptr)
            return;

        /* Recover the raw pointer stored at user_ptr[-4] by Allocate */
        void* rawPtr = (void*)(uintptr_t)((uint32_t*)ptr)[-1];
        rage_free(rawPtr);
    }
    /* If owned, the allocator manages its own memory — nothing to do */
}
