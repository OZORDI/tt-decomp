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
// mfNativeAnimationController::~mfNativeAnimationController()  [vtable slot 0 @ 0x822DF688]
// Destructor for native animation controller
// Size: 0x5C bytes
//
// This is the deleting destructor that:
// 1. Sets vtable to 0x8205A08C
// 2. Calls parent class (mfAnimationController) destructor
// 3. Conditionally frees memory if flags & 1
// ─────────────────────────────────────────────────────────────────────────────
mfNativeAnimationController::~mfNativeAnimationController() {
    // Vtable is automatically set to 0x8205A08C by the compiler
    // Parent class destructor (mfAnimationController) is called automatically
}

// Scalar deleting destructor - handles memory deallocation
// Called by operator delete when the object was dynamically allocated
void mfNativeAnimationController::ScalarDtor(int flags) {
    // Call the destructor
    this->~mfNativeAnimationController();
    
    // If bit 0 is set in flags, free the memory
    // This is the standard C++ deleting destructor pattern
    if (flags & 1) {
        rage_free_00C0(this);
    }
}

} // namespace rage
