/**
 * rage_mf.cpp — Motion/Animation Framework Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Animation controller and motion clip system.
 */
#include "rage_mf.hpp"

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);  // @ 0x820C00C0

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// mfNativeAnimationController destructor [vtable slot 0 @ 0x822DF688]
// Size: 0x5C bytes
//
// Deleting destructor that:
// 1. Sets vtable to 0x8205A08C (done automatically by compiler)
// 2. Calls parent class (mfAnimationController) destructor
// 3. Returns pointer to this object
//
// Note: The assembly shows this takes a flags parameter (r4) and conditionally
// frees memory if (flags & 1). This is the deleting destructor pattern used
// throughout the RAGE engine.
// ─────────────────────────────────────────────────────────────────────────────
mfNativeAnimationController::~mfNativeAnimationController() {
    // Parent class destructor is called automatically
    // Vtable is set to 0x8205A08C automatically by compiler
}

} // namespace rage
