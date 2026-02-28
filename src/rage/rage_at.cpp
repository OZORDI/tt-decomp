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

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::LookupStringIndex @ 0x8225F250 | size: 0x70
// 
// Searches for a string in a global lookup table and returns its index + 0x1001.
// The lookup table is at 0x825C5320 and contains 35 string pointer entries.
// Returns 0 if not found, otherwise returns (index + 0x1001) masked to 16 bits.
// ─────────────────────────────────────────────────────────────────────────────
uint16_t atSingleton::LookupStringIndex(const char* searchStr) {
    // Global string lookup table at 0x825C5320
    // Each entry is 8 bytes: [const char* str, uint32_t data]
    extern const char* g_atSingletonStringTable[];  // 35 entries
    
    const char** tablePtr = (const char**)0x825C5320;
    const char** tableEnd = (const char**)((uintptr_t)tablePtr + 280);
    
    int index = 0;
    
    // Iterate through the lookup table
    while (tablePtr < tableEnd) {
        const char* tableStr = *tablePtr;
        const char* searchPtr = searchStr;
        const char* tableStrPtr = tableStr;
        
        // Compare strings character by character
        int diff = 0;
        while (*tableStrPtr != '\0') {
            diff = *tableStrPtr - *searchPtr;
            if (diff != 0) {
                break;
            }
            tableStrPtr++;
            searchPtr++;
        }
        
        // If strings match (diff == 0 after null terminator)
        if (diff == 0) {
            // Return index + 0x1001, masked to 16 bits
            return (uint16_t)((index + 0x1001) & 0xFFFF);
        }
        
        // Move to next entry (8 bytes per entry)
        tablePtr = (const char**)((uintptr_t)tablePtr + 8);
        index++;
    }
    
    // Not found
    return 0;
}
