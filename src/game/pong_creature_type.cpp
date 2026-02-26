/**
 * pong_creature_type.cpp — pongCreatureType & pongAnimationInfo destructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted from recomp/structured_pass5_final/tt-decomp_recomp.0.cpp:5936,6232
 */

#include "pong_creature.hpp"

// External dependencies
extern void rage_free(void* ptr);

// rage_2E80 @ 0x820C2E80 — base class (rage::datBase-derived) destructor
// Sets vtable to 0x8202776C (base), cleans up child objects at +8 and +24,
// then zeroes +8.
extern void rage_2E80(void* obj);

// Vtable pointers
static void* const kPongAnimInfo_Vtable = (void*)0x820276C4;  // base vtable

/**
 * pongCreatureType::~pongCreatureType() @ 0x820C2DA0 | size: 0x38
 *
 * Calls the base-class cleanup function (rage_2E80) which:
 *   1. Sets vtable to base class
 *   2. Destroys child object at +8 via its vtable slot 0
 *   3. Destroys child object at +24 via its vtable slot 0
 *   4. Zeroes the child pointer at +8
 *
 * If the delete-self flag (bit 0 of r4) is set, frees this.
 */
pongCreatureType::~pongCreatureType()
{
    rage_2E80(this);

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
    // Restore vtable to base class
    vtable = (void**)kPongAnimInfo_Vtable;

    // Note: array-delete path (flags & 2) and single-delete path (flags & 1)
    // are handled by the calling convention — the scaffold checks flags bits
    // and either walks the array backward or frees a single object.
}
