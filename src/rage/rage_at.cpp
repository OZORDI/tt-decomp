/**
 * rage_at.cpp — atSingleton and atSafeDLList implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE engine "at" subsystem - singleton pattern and safe doubly-linked lists
 */

#include "rage_at.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ConditionalVirtualDispatch @ 0x82132BA0 | size: 0x74
// 
// Conditionally calls either vtable slot 37 or slot 11 based on a flag at +0x4.
// If the flag is non-zero, calls slot 37 twice (once directly, once through
// another virtual dispatch). Otherwise, calls slot 11.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::ConditionalVirtualDispatch() {
    // Check flag at offset +0x4 (likely m_bInitialized or similar state flag)
    uint8_t flag = *(uint8_t*)((char*)this + 4);
    
    if (flag != 0) {
        // Path 1: Flag is set - call vtable slot 37
        // This appears to be a cleanup or finalization method
        typedef void (*VTableFunc37)(void*);
        void** vtable = *(void***)this;
        VTableFunc37 func37 = (VTableFunc37)vtable[37];
        func37(this);
        
        // Call slot 37 again through another virtual dispatch
        // The second parameter (0x82027D04) is a 2-byte .rdata constant
        // likely used as a parameter or flag
        vtable = *(void***)this;
        func37 = (VTableFunc37)vtable[37];
        // Note: Original assembly suggests this might take a parameter
        // but the exact signature is unclear from the scaffold
        func37(this);
    } else {
        // Path 2: Flag is clear - call vtable slot 11
        // This appears to be a different operation, possibly initialization
        typedef void (*VTableFunc11)(void*);
        void** vtable = *(void***)this;
        VTableFunc11 func11 = (VTableFunc11)vtable[11];
        func11(this);
    }
}
