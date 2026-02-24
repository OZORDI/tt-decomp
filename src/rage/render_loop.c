/**
 * rage/render_loop.c — RAGE engine heap free and render-loop bootstrap
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Contains the engine-level memory free wrapper, which validates the pointer
 * through the atSingleton registry before dispatching to the allocator vtable.
 */

#include "rage_system.hpp"

// ---------------------------------------------------------------------------
// External declarations
// ---------------------------------------------------------------------------

// atSingleton registry lookup — returns non-zero (error code) when not found.
extern uint8_t atSingleton_Find_90D0(void* ptr);   // @ 0x820F90D0

// Global allocator object — stored at SDA base + 4.
// Vtable slot 2 = Free(void* ptr).
extern struct rageAllocator* g_pAllocator;  // @ SDA:0x82600000 + 4


// ===========================================================================
// rage_free  @ 0x820C00C0 | size: 0x60
//
// Engine-level heap free wrapper.
//
// Guards against double-free or invalid-pointer frees by first checking the
// atSingleton registry.  If the singleton check returns a non-zero status
// (pointer is owned by a registered singleton, not the heap), the free is
// silently skipped.  Otherwise, delegates to the global allocator's Free
// virtual method (vtable slot 2).
// ===========================================================================
void rage_free(void* ptr) {
    if (!ptr) {
        return;
    }

    // Check whether this pointer is registered as a singleton-owned block.
    // A non-zero return means the allocator should NOT free it.
    uint8_t singletonStatus = atSingleton_Find_90D0(ptr);
    if (singletonStatus != 0) {
        return;
    }

    // Dispatch to the global allocator's virtual Free method (vtable slot 2).
    g_pAllocator->vtable->Free(g_pAllocator, ptr);
}
