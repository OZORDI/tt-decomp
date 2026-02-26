#pragma once

/**
 * rage::audControlWrapper, audControl3dWrapper, audControl2dWrapper
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Wrapper classes for audio control objects. These provide RAII-style
 * management of audControl, audControl3d, and audControl2d instances.
 * 
 * The wrappers handle:
 * - Automatic cleanup of wrapped audio control objects
 * - Array-based storage for multiple control instances
 * - Proper destruction order (controls before wrapper)
 */

#include <stdint.h>

namespace rage {

// Forward declarations
struct audControl;
struct audControl3d;
struct audControl2d;

/**
 * audControlWrapper @ vtable 0x82074B88
 * 
 * Base wrapper class for audio control objects.
 * Manages a dynamically allocated array of audio controls.
 */
struct audControlWrapper {
    void** vtable;              // +0x00 @ 0x82074B88
    audControl* m_pControl;     // +0x04 - Pointer to wrapped control(s)
    
    /**
     * Destructor @ 0x823F7C88 | size: 0x50
     * 
     * Cleans up the wrapped audio control object.
     * If m_pControl is non-null, calls its virtual destructor.
     */
    virtual ~audControlWrapper();
};

/**
 * audControl3dWrapper @ vtable 0x82074B98
 * 
 * Wrapper for 3D positional audio controls.
 * Manages an array of audControl3d instances.
 */
struct audControl3dWrapper : public audControlWrapper {
    // Inherits m_pControl from base (reinterpreted as audControl3d*)
    
    /**
     * Destructor @ 0x823F8138 | size: 0xD0
     * 
     * Cleans up an array of audControl3d objects.
     * Handles both single and array deletion based on flags.
     */
    virtual ~audControl3dWrapper();
};

/**
 * audControl2dWrapper @ vtable 0x82074BA8
 * 
 * Wrapper for 2D (non-positional) audio controls.
 * Manages an array of audControl2d instances.
 */
struct audControl2dWrapper : public audControlWrapper {
    // Inherits m_pControl from base (reinterpreted as audControl2d*)
    
    // Uses base class destructor (no override needed)
};

} // namespace rage
