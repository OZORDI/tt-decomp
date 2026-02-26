/**
 * pong_creature_type.cpp — pongCreatureType & pongAnimationInfo destructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.0.cpp:5936,6031,6232
 */

#include "pong_creature.hpp"

// External dependencies
extern void rage_free(void* ptr);

// atHashMap_Clear @ 0x8234C0E0 | size: 0x124
//
// Destroys all entries in a RAGE hash table container.
// The container struct layout (passed as `this`):
//   +0x00: pointer to bucket array (each bucket is head of a singly-linked list)
//   +0x04: bucket count (uint16)
//   +0x06: capacity / flags (uint16)
//
// Each linked-list node:
//   +0x00: pointer to owned object (freed via allocator vtable slot 2)
//   +0x08: pointer to next node
//
// Walks every bucket, follows each chain, frees the referenced object
// (after atSingleton ownership check) then frees the node itself,
// then frees the bucket array.  Zeros the container fields on exit.
//
// Called from 12+ destructors across the codebase.
extern void atHashMap_Clear(void* container);   // rage_C0E0 @ 0x8234C0E0

// Vtable pointers
static void** const kPongCreatureType_Vtable = (void**)0x8202776C;
static void** const kDatBase_Vtable          = (void**)0x820276C4;  // rage::datBase

/**
 * pongCreatureType::~pongCreatureType()
 *
 * Virtual destructor entry: pongCreatureType_vfn_0 @ 0x820C2DA0 | size: 0x38
 * Destructor body:          pongCreatureType_Destroy @ 0x820C2E80 | size: 0xB0
 *
 * pongCreatureType inherits from rage::datBase (vtable 0x820276C4).
 * The destructor body (rage_2E80 in the recomp) performs:
 *
 *   1. Sets vtable to pongCreatureType (0x8202776C)
 *   2. Destroys owned array at +8:
 *        - Reads array metadata at (ptr - 4)
 *        - If metadata != 0: calls element destructor (vtable slot 0, flags=3)
 *        - If metadata == 0: calls rage_free(ptr - 4) directly
 *   3. Zeros +8
 *   4. Destroys owned child object at +24 via vtable slot 0 (flags=1)
 *   5. Zeros +24 and +20
 *   6. Destroys owned child object at +96 via vtable slot 0 (flags=1)
 *   7. Zeros +96
 *   8. Calls atHashMap_Clear on hash-table subobject at +12
 *   9. Sets vtable to rage::datBase (0x820276C4) — base class chain
 *
 * The vfn_0 entry then checks (flags & 1) to conditionally free the object.
 */
pongCreatureType::~pongCreatureType()
{
    // ── pongCreatureType_Destroy @ 0x820C2E80 (inlined) ──

    // Step 1: set own vtable
    vtable = kPongCreatureType_Vtable;

    // Step 2: destroy owned array at +8
    uint8_t* arrayPtr = *(uint8_t**)((uint8_t*)this + 8);
    if (arrayPtr != nullptr) {
        // Array metadata (element count) lives at ptr - 4
        uint32_t arrayMeta = *(uint32_t*)(arrayPtr - 4);
        if (arrayMeta != 0) {
            // Array has elements — call element destructor (vtable slot 0)
            // with flags=3 (delete self + array delete)
            void** elemVtable = *(void***)arrayPtr;
            typedef void (*DtorFn)(void*, int);
            DtorFn dtor = (DtorFn)elemVtable[0];
            dtor(arrayPtr, 3);
        } else {
            // No array metadata — free the raw allocation
            rage_free(arrayPtr - 4);
        }
    }

    // Step 3: zero the array pointer
    *(void**)((uint8_t*)this + 8) = nullptr;

    // Step 4: destroy owned child at +24
    void* child24 = *(void**)((uint8_t*)this + 24);
    if (child24 != nullptr) {
        void** childVtable = *(void***)child24;
        typedef void (*DtorFn)(void*, int);
        DtorFn dtor = (DtorFn)childVtable[0];
        dtor(child24, 1);
    }

    // Step 5: zero +24 and +20
    *(void**)((uint8_t*)this + 24) = nullptr;
    *(void**)((uint8_t*)this + 20) = nullptr;

    // Step 6: destroy owned child at +96
    void* child96 = *(void**)((uint8_t*)this + 96);
    if (child96 != nullptr) {
        void** childVtable = *(void***)child96;
        typedef void (*DtorFn)(void*, int);
        DtorFn dtor = (DtorFn)childVtable[0];
        dtor(child96, 1);
    }

    // Step 7: zero +96
    *(void**)((uint8_t*)this + 96) = nullptr;

    // Step 8: clear hash table at +12
    atHashMap_Clear((uint8_t*)this + 12);

    // Step 9: chain to rage::datBase
    vtable = kDatBase_Vtable;

    // Note: if (flags & 1) rage_free(this);
    // Handled by the standard RAGE deleting-destructor ABI.
}

/**
 * pongAnimationInfo::~pongAnimationInfo() @ 0x820C3040 | size: 0x9C
 *
 * Array-aware destructor.  Handles two cases:
 *
 * Case 1 — array delete (flags & 2):
 *   Walks backward through an array of pongAnimationInfo objects
 *   (count stored at this[-4]), setting each element's vtable back
 *   to base, then frees the backing allocation at this-4.
 *
 * Case 2 — single delete:
 *   Sets vtable to base class, then optionally frees this.
 */
pongAnimationInfo::~pongAnimationInfo()
{
    // Restore vtable to rage::datBase
    vtable = kDatBase_Vtable;

    // Note: array-delete path (flags & 2) and single-delete path (flags & 1)
    // are handled by the calling convention — the scaffold checks flags bits
    // and either walks the array backward or frees a single object.
}
