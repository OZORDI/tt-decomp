/**
 * rage_misc.cpp — Miscellaneous RAGE engine utility functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "rage_misc.hpp"
#include <cstdint>

// Forward declaration for rage_3AC8
extern "C" void rage_3AC8(void* obj, int param);

/**
 * rage_52B0_1 @ 0x822E52B0 | size: 0x5C
 * 
 * Initializes a rage subsystem object by:
 * - Calling rage_3AC8 on a nested object at offset +12
 * - Setting various state fields to initial values
 * - Setting buffer size to 32 (likely max capacity)
 * - Setting state to 2 (initialized state enum)
 */
extern "C" void rage_52B0_1(void* thisPtr) {
    uint8_t* obj = static_cast<uint8_t*>(thisPtr);
    
    // Initialize nested object at offset +12
    void* nestedObj = *reinterpret_cast<void**>(obj + 12);
    rage_3AC8(nestedObj, 0);
    
    // Initialize state fields
    *reinterpret_cast<uint32_t*>(obj + 24) = 0;      // Clear field_24
    *reinterpret_cast<uint32_t*>(obj + 8) = 1;       // Set initialized flag
    *reinterpret_cast<uint32_t*>(obj + 16) = 32;     // Set buffer size/capacity
    *reinterpret_cast<uint8_t*>(obj + 28) = 0;       // Clear byte field_28
    *reinterpret_cast<uint32_t*>(obj + 156) = 0;     // Clear field_156
    *reinterpret_cast<uint32_t*>(obj + 20) = 2;      // Set state to 2
}
