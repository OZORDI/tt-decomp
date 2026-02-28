/**
 * rage_array_destructor.cpp — RAGE dynamic array destructor
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>

// Forward declarations
extern "C" void rage_8118(void* element);
extern "C" void rage_free_00C0(void* ptr);

/**
 * rage_2980 @ 0x820C2980 | size: 0x5C
 * 
 * Destructs and frees a dynamically allocated array with element destructors.
 * 
 * Array layout:
 *   -4 bytes: uint32_t element count
 *   +0 bytes: Element array (12 bytes per element)
 * 
 * This function:
 * 1. Adjusts pointer back 4 bytes to access the count header
 * 2. Iterates through elements in reverse order
 * 3. Calls rage_8118 destructor on each element
 * 4. Frees the entire allocation including header
 * 
 * Parameters:
 *   pArray - Pointer to first element (NOT the header)
 * 
 * Returns:
 *   Pointer to the freed memory block (header address)
 */
extern "C" void* rage_2980(void* pArray) {
    // Adjust pointer back to access the count header at -4 bytes
    uint8_t* pHeader = static_cast<uint8_t*>(pArray) - 4;
    
    // Load element count from header
    uint32_t count = *reinterpret_cast<uint32_t*>(pHeader);
    
    // Calculate total array size: count * 12 bytes per element
    // Assembly does: count + (count << 1) = count * 3, then << 2 = count * 12
    uint32_t totalSize = count * 12;
    
    // Start at end of array and work backwards
    uint8_t* pElement = static_cast<uint8_t*>(pArray) + totalSize;
    
    // Destruct each element in reverse order
    for (int32_t i = count - 1; i >= 0; i--) {
        pElement -= 12;  // Move to previous element
        rage_8118(pElement);  // Call element destructor
    }
    
    // Free the entire allocation (including header)
    rage_free_00C0(pHeader);
    
    // Return pointer to freed block
    return pHeader;
}
