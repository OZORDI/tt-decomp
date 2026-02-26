#pragma once

/**
 * rage::audControlWrapper, audControl3dWrapper, audControl2dWrapper
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Wrapper classes for audio control objects. These provide RAII-style
 * management of audControl, audControl3d, and audControl2d instances.
 * 
 * Structure sizes:
 * - audControlWrapper: 8 bytes (vtable + pointer)
 * - Array element size: 12 bytes (8 bytes + 4 bytes padding)
 * - Vtable sizes: 16 bytes (4 slots) for base and 3d, 12 bytes (3 slots) for 2d
 * 
 * Note: These structures manually manage vtables (no C++ virtual keyword).
 * The vtable member is the actual vtable pointer from the binary.
 */

#include <stdint.h>

namespace rage {

// Forward declarations
struct audControl;
struct audControl3d;
struct audControl2d;

/**
 * audControlWrapper @ vtable 0x82074B88 (size: 0x10 = 4 slots)
 * 
 * Base wrapper class for audio control objects.
 * Manages dynamically allocated audio controls.
 * 
 * Structure: 8 bytes
 *   +0x00: vtable pointer
 *   +0x04: m_pControl pointer
 * 
 * Array element size: 12 bytes (includes 4 bytes padding)
 * 
 * Vtable layout:
 *   [0] = destructor @ 0x823F7C88
 *   [1] = scalar deleting destructor
 *   [2] = reserved
 *   [3] = reserved
 */
struct audControlWrapper {
    void** vtable;              // +0x00 @ 0x82074B88
    audControl* m_pControl;     // +0x04 - Pointer to wrapped control(s)
    
    /**
     * Destructor @ 0x823F7C88 | size: 0x50
     * 
     * Calls cleanup helper @ 0x823F7C28, then optionally frees wrapper.
     * Cleanup helper resets vtable, destroys wrapped control, nulls pointer.
     */
    ~audControlWrapper();
    
    /**
     * Cleanup helper @ 0x823F7C28 | size: 0x5C
     * 
     * Non-virtual helper that performs the actual cleanup:
     * 1. Resets vtable to base class (0x82074B88)
     * 2. If m_pControl != null: calls its destructor with flags=1
     * 3. Sets m_pControl to null
     * 
     * Note: Misnamed as SinglesNetworkClient_7C28_p46 in symbols
     */
    void cleanupControl();
};

/**
 * audControl3dWrapper @ vtable 0x82074B98 (size: 0x10 = 4 slots)
 * 
 * Wrapper for 3D positional audio controls.
 * Supports both single object and array deletion.
 * 
 * Array memory layout:
 *   [count:4][element0:12][element1:12]...[elementN:12]
 *   ^         ^
 *   alloc     m_pControl points here
 * 
 * Vtable layout:
 *   [0] = destructor @ 0x823F8138
 *   [1] = scalar deleting destructor
 *   [2] = reserved
 *   [3] = reserved
 */
struct audControl3dWrapper : public audControlWrapper {
    /**
     * Destructor @ 0x823F8138 | size: 0xD0
     * 
     * Flags parameter (r4):
     *   Bit 0: Delete wrapper itself after cleanup
     *   Bit 1: Array delete (vs single object)
     * 
     * Array delete path:
     *   - Reads count from offset -4
     *   - Loops backwards destroying each 12-byte element
     *   - Frees memory starting from count location
     * 
     * Single object path:
     *   - Calls cleanup helper
     *   - Optionally frees wrapper
     */
    ~audControl3dWrapper();
};

/**
 * audControl2dWrapper @ vtable 0x82074BA8 (size: 0x0C = 3 slots)
 * 
 * Wrapper for 2D (non-positional) audio controls.
 * Uses base class destructor (no override).
 * 
 * Vtable layout:
 *   [0] = destructor (inherited from base)
 *   [1] = scalar deleting destructor
 *   [2] = reserved
 */
struct audControl2dWrapper : public audControlWrapper {
    // No additional members or methods
};

// Size assertions for ABI compatibility (PowerPC32 sizes)
static_assert(sizeof(void*) == 8 || sizeof(audControlWrapper) == 8, 
              "audControlWrapper must be 8 bytes on 32-bit systems");
static_assert(sizeof(void*) == 8 || sizeof(audControl3dWrapper) == 8,
              "audControl3dWrapper must be 8 bytes on 32-bit systems");
static_assert(sizeof(void*) == 8 || sizeof(audControl2dWrapper) == 8,
              "audControl2dWrapper must be 8 bytes on 32-bit systems");

} // namespace rage
