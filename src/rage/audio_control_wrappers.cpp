/**
 * rage::audControlWrapper, audControl3dWrapper, audControl2dWrapper
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implementation of audio control wrapper classes.
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
 * audControlWrapper::cleanupControl @ 0x823F7C28 | size: 0x5C
 * 
 * Non-virtual cleanup helper. This is the core cleanup logic.
 * 
 * Assembly behavior:
 *   mr r31,r3                    ; Save 'this'
 *   lis r11,-32249               ; Load upper 16 bits
 *   addi r11,r11,19336           ; r11 = 0x82074B88 (base vtable)
 *   lwz r3,4(r31)                ; Load m_pControl
 *   stw r11,0(r31)               ; Reset vtable to base class
 *   cmplwi cr6,r3,0              ; Check if m_pControl is null
 *   beq cr6,loc_823F7C70         ; Skip if null
 *   lwz r11,0(r3)                ; Load wrapped object's vtable
 *   li r4,1                      ; flags = 1 (delete)
 *   lwz r10,0(r11)               ; Load destructor (slot 0)
 *   bctrl                        ; Call destructor
 *   li r9,0                      ; NULL
 *   stw r9,4(r31)                ; m_pControl = NULL
 * 
 * Note: Misnamed as SinglesNetworkClient_7C28_p46 in symbol table
 */
void audControlWrapper::cleanupControl() {
    // Reset vtable to base class (for proper destruction order)
    // This is done automatically by C++ during destruction
    
    // Clean up wrapped control if present
    if (m_pControl != nullptr) {
        // Call virtual destructor on the wrapped control
        audControl* control = m_pControl;
        if (control->vtable != nullptr) {
            // Virtual destructor call with delete flag
            delete control;
        }
        m_pControl = nullptr;
    }
}

/**
 * audControlWrapper::~audControlWrapper @ 0x823F7C88 | size: 0x50
 * 
 * Base wrapper destructor.
 * 
 * Assembly behavior:
 *   mr r31,r3                    ; Save 'this'
 *   mr r30,r4                    ; Save flags parameter
 *   bl 0x823f7c28                ; Call cleanupControl()
 *   clrlwi r11,r30,31            ; Extract bit 0 (delete flag)
 *   mr r3,r31                    ; Restore 'this'
 *   cmplwi cr6,r11,0             ; Check delete flag
 *   beq cr6,0x823f7cc0           ; Skip if not set
 *   bl 0x820c00c0                ; rage_free_00C0(this)
 *   mr r3,r31                    ; Return 'this'
 */
audControlWrapper::~audControlWrapper() {
    // Call cleanup helper
    cleanupControl();
    
    // Note: The wrapper object itself is freed by the caller
    // based on the flags parameter (handled by compiler-generated code)
}

/**
 * audControl3dWrapper::~audControl3dWrapper @ 0x823F8138 | size: 0xD0
 * 
 * 3D wrapper destructor with array support.
 * 
 * Assembly behavior (array delete path):
 *   mr r26,r4                    ; Save flags
 *   mr r31,r3                    ; Save 'this'
 *   rlwinm r11,r26,0,30,30       ; Extract bit 1 (array flag)
 *   cmplwi cr6,r11,0
 *   beq cr6,0x823f81e0           ; Jump to single-object path
 *   
 *   ; Array delete:
 *   addi r28,r31,-4              ; Point to count
 *   lwz r11,0(r28)               ; Load count
 *   rlwinm r10,r11,1,0,30        ; count * 2
 *   addi r30,r11,-1              ; loop counter = count - 1
 *   add r10,r11,r10              ; count * 3
 *   rlwinm r11,r10,2,0,29        ; count * 12
 *   add r31,r11,r31              ; r31 = end of array
 *   
 *   loc_823F8188:                ; Loop
 *   addi r31,r31,-12             ; Move to previous element
 *   lwz r3,4(r31)                ; Load m_pControl
 *   stw r29,0(r31)               ; Set vtable to base
 *   cmplwi cr6,r3,0
 *   beq cr6,0x823f81b4
 *   lwz r9,0(r3)                 ; Load wrapped object vtable
 *   li r4,1                      ; flags = 1
 *   lwz r8,0(r9)                 ; Load destructor
 *   bctrl                        ; Call destructor
 *   stw r27,4(r31)               ; Clear m_pControl
 *   
 *   loc_823F81B4:
 *   addi r30,r30,-1              ; Decrement counter
 *   cmpwi cr6,r30,0
 *   bge cr6,0x823f8188           ; Loop if >= 0
 *   
 *   clrlwi r7,r26,31             ; Extract bit 0 (delete flag)
 *   cmplwi cr6,r7,0
 *   beq cr6,0x823f81d4
 *   mr r3,r28                    ; Point to count
 *   bl 0x820c00c0                ; rage_free_00C0
 */
audControl3dWrapper::~audControl3dWrapper() {
    // The assembly checks bit 1 of the flags parameter (r4)
    // to determine if this is an array delete
    // 
    // For standard C++ delete, we use the simpler single-object path
    // Array handling is done by the compiler's delete[] operator
    
    // Call base class destructor (handles m_pControl cleanup)
    // This is automatic in C++
    
    // Full array delete implementation (for reference):
    // if (flags & 0x2) {  // Array delete flag
    //     if (m_pControl != nullptr) {
    //         // Read count from -4 offset
    //         uint32_t* countPtr = (uint32_t*)((char*)m_pControl - 4);
    //         uint32_t count = *countPtr;
    //         
    //         // Calculate end of array: count * 12 bytes per element
    //         char* arrayEnd = (char*)m_pControl + (count * 12);
    //         
    //         // Destroy each element in reverse order
    //         for (char* elem = arrayEnd - 12; count > 0; elem -= 12, count--) {
    //             audControl3dWrapper* wrapper = (audControl3dWrapper*)elem;
    //             // Reset vtable to base
    //             wrapper->vtable = (void**)0x82074B88;
    //             // Destroy wrapped control
    //             if (wrapper->m_pControl != nullptr) {
    //                 delete wrapper->m_pControl;
    //                 wrapper->m_pControl = nullptr;
    //             }
    //         }
    //         
    //         // Free the array (including count prefix)
    //         rage_free_00C0(countPtr);
    //     }
    // }
}

} // namespace rage
