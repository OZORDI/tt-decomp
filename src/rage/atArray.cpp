/**
 * atArray.cpp — RAGE atArray dynamic array implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * atArray is RAGE's template-based dynamic array system used throughout
 * the engine for managing collections of objects with automatic cleanup.
 * Also used by atSingleton for singleton management and XML parsing.
 */

#include <cstdint>

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);

/**
 * atArrayElement — 12-byte array element with optional chaining
 * 
 * Used in RAGE's atArray template for storing elements with metadata.
 * The pNext field allows elements to form linked chains for hash tables
 * or other data structures built on top of arrays.
 */
struct atArrayElement {
    void* pData;              // +0x00: Pointer to element data (has vtable)
    uint32_t flags;           // +0x04: Element flags/attributes
    atArrayElement* pNext;    // +0x08: Next element in chain (optional)
};

static_assert(sizeof(atArrayElement) == 12, "atArrayElement must be 12 bytes");

/**
 * xmlTree_vfn_1 @ 0x821A8118 | size: 0x68
 * Original symbol: rage_8118
 * 
 * Virtual destructor for XML tree nodes (vtable slot 1).
 * 
 * Recursively destructs an XML tree by:
 * 1. Traversing the child node chain via pNext pointers
 * 2. Calling vtable[0] destructor on each child's data
 * 3. Freeing the root node's data
 * 
 * This is used by xmlTree and xmlNodeStruct classes for cleanup.
 * The atArrayElement structures themselves are not freed here
 * as they're part of an array managed by the caller.
 * 
 * Parameters:
 *   pElement - Pointer to atArrayElement (XML tree node)
 */
extern "C" void xmlTree_vfn_1(atArrayElement* pElement) {
    // Traverse and destruct all child nodes in the chain
    atArrayElement* pChild = pElement->pNext;
    
    while (pChild != nullptr) {
        // Get vtable from the child's data object
        void** vtable = *reinterpret_cast<void***>(pChild->pData);
        
        // Save next pointer before destroying current child
        atArrayElement* pNextChild = pChild->pNext;
        
        // Call vtable[0] (destructor) with parameter 1 (free flag)
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = reinterpret_cast<DestructorFn>(vtable[0]);
        destructor(pChild->pData, 1);
        
        // Move to next child
        pChild = pNextChild;
    }
    
    // Free the root element's data
    rage_free_00C0(pElement->pData);
}

// Alias for compatibility
extern "C" void rage_8118(atArrayElement* pElement) {
    xmlTree_vfn_1(pElement);
}

/**
 * atArray_Destructor @ 0x820C2980 | size: 0x5C
 * Original symbol: rage_2980
 * Also known as: atSingleton_29E0_g
 * 
 * Destructs and frees a dynamically allocated atArray.
 * 
 * Array memory layout:
 *   -4 bytes: uint32_t element count
 *   +0 bytes: atArrayElement array (12 bytes per element)
 * 
 * This function:
 * 1. Adjusts pointer back 4 bytes to access the count header
 * 2. Iterates through elements in reverse order
 * 3. Calls xmlTree_vfn_1 to destruct each element and its children
 * 4. Frees the entire allocation including header
 * 
 * Used by:
 * - atSingleton for singleton cleanup
 * - xmlNodeStructArray for XML node array cleanup
 * - Various RAGE subsystems using atArray
 * 
 * Parameters:
 *   pArray - Pointer to first element (NOT the header)
 * 
 * Returns:
 *   Pointer to the freed memory block (header address)
 */
extern "C" void* atArray_Destructor(void* pArray) {
    // Adjust pointer back to access the count header at -4 bytes
    uint8_t* pHeader = static_cast<uint8_t*>(pArray) - 4;
    
    // Load element count from header
    uint32_t count = *reinterpret_cast<uint32_t*>(pHeader);
    
    // Calculate total array size: count * 12 bytes per element
    // Assembly: count + (count << 1) = count * 3, then << 2 = count * 12
    uint32_t totalSize = count * 12;
    
    // Start at end of array and work backwards
    uint8_t* pElement = static_cast<uint8_t*>(pArray) + totalSize;
    
    // Destruct each element in reverse order
    for (int32_t i = count - 1; i >= 0; i--) {
        pElement -= 12;  // Move to previous element
        xmlTree_vfn_1(reinterpret_cast<atArrayElement*>(pElement));
    }
    
    // Free the entire allocation (including header)
    rage_free_00C0(pHeader);
    
    // Return pointer to freed block
    return pHeader;
}

// Aliases for compatibility
extern "C" void* rage_2980(void* pArray) {
    return atArray_Destructor(pArray);
}

extern "C" void* atSingleton_29E0_g(void* pArray) {
    return atArray_Destructor(pArray);
}
