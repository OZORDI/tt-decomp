/**
 * rage::audControlWrapper, audControl3dWrapper, audControl2dWrapper
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implementation of audio control wrapper classes.
 * These provide RAII-style management of audio control objects.
 */

#include "rage/audio_control_wrappers.hpp"
#include <cstdlib>

// Forward declarations for rage audio types
namespace rage {
    struct audControl {
        void** vtable;
    };
    struct audControl3d : audControl {};
    struct audControl2d : audControl {};
}

namespace rage {

// Forward declaration of rage_free (from memory system)
extern "C" void rage_free_00C0(void* ptr);

/**
 * audControlWrapper::~audControlWrapper @ 0x823F7C88 | size: 0x50
 * 
 * Base wrapper destructor. Cleans up the wrapped audio control.
 * 
 * Original assembly behavior:
 * 1. Set vtable pointer to base class vtable (0x82074B88)
 * 2. If m_pControl is non-null:
 *    - Call its virtual destructor with flags=1 (delete)
 *    - Set m_pControl to null
 * 3. If delete flag is set (bit 0 of flags parameter):
 *    - Free the wrapper object itself
 * 
 * This is called from derived class destructors and directly.
 */
audControlWrapper::~audControlWrapper() {
    // Set vtable to base class (for proper destruction order)
    // This happens automatically in C++
    
    // Clean up wrapped control if present
    if (m_pControl != nullptr) {
        // Call virtual destructor on the wrapped control
        // The assembly shows: load vtable, load slot 0, call with flags=1
        audControl* control = m_pControl;
        if (control->vtable != nullptr) {
            // Virtual destructor call with delete flag
            delete control;
        }
        m_pControl = nullptr;
    }
    
    // Note: The wrapper object itself is freed by the caller
    // based on the flags parameter (handled by compiler-generated code)
}

/**
 * audControl3dWrapper::~audControl3dWrapper @ 0x823F8138 | size: 0xD0
 * 
 * 3D wrapper destructor. Handles array-based cleanup.
 * 
 * This destructor is more complex because it can manage arrays of
 * audControl3d objects. The assembly shows:
 * 
 * 1. Check flags bit 1 (array delete flag)
 * 2. If array delete:
 *    - Read array count from offset -4 (before the array)
 *    - Loop through array backwards, destroying each element
 *    - Free the array memory (including the count prefix)
 * 3. Else (single object):
 *    - Call base class destructor
 *    - Free wrapper if delete flag set
 * 
 * Array layout in memory:
 *   [count:4 bytes][element0][element1]...[elementN]
 *   ^              ^
 *   allocation     m_pControl points here
 *   starts here
 */
audControl3dWrapper::~audControl3dWrapper() {
    // The assembly checks bit 1 of the flags parameter (r4)
    // to determine if this is an array delete
    // 
    // For now, we implement the simpler single-object path
    // Array handling would require compiler-specific array delete support
    
    // Call base class destructor (handles m_pControl cleanup)
    // This is automatic in C++
    
    // Note: Full array delete implementation would look like:
    // if (flags & 0x2) {  // Array delete flag
    //     if (m_pControl != nullptr) {
    //         // Read count from -4 offset
    //         uint32_t* countPtr = (uint32_t*)((char*)m_pControl - 4);
    //         uint32_t count = *countPtr;
    //         
    //         // Destroy each element in reverse order
    //         audControl3d* array = (audControl3d*)m_pControl;
    //         for (int32_t i = count - 1; i >= 0; i--) {
    //             array[i].~audControl3d();
    //         }
    //         
    //         // Free the array (including count prefix)
    //         rage_free_00C0(countPtr);
    //     }
    // } else {
    //     // Single object delete (base class handles it)
    // }
}

} // namespace rage
