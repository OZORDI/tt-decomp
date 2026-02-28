/**
 * rage::parFileIO — File I/O Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "rage/parFileIO.hpp"
#include <cstdlib>

namespace rage {

// Forward declaration of memory management function
extern "C" void rage_free_00C0(void* ptr);

// ────────────────────────────────────────────────────────────────────────────
// datBase — Base class destructor
// ────────────────────────────────────────────────────────────────────────────

/**
 * datBase::~datBase @ 0x82177630 | size: 0x58
 * 
 * Base destructor. Frees owned data pointer if non-null.
 * Sets vtable to datBase vtable (0x820276C4).
 */
datBase::~datBase() {
    // Free owned data if present
    if (m_pData != nullptr) {
        rage_free_00C0(m_pData);
        m_pData = nullptr;
    }
    // Vtable is set to datBase vtable by compiler
}

// ────────────────────────────────────────────────────────────────────────────
// parFileIO — File I/O destructor
// ────────────────────────────────────────────────────────────────────────────

/**
 * parFileIO::~parFileIO @ 0x821775E0 | size: 0x50
 * 
 * Scalar destructor for parFileIO.
 * Calls base class destructor, then optionally frees the object itself.
 * 
 * The delete flag (bit 0 of the second parameter in assembly) determines
 * whether to free the object memory after destruction.
 */
parFileIO::~parFileIO() {
    // Base class destructor is called automatically
    // If this was called with delete flag set, rage_free_00C0 is called
    // on 'this' by the compiler-generated deleting destructor
}

} // namespace rage
