/**
 * rage_array_destructor.cpp — RAGE linked list and array destructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>

// Forward declarations
extern "C" void rage_free_00C0(void* ptr);

/**
 * ListNode — 12-byte linked list node structure
 * 
 * Used in RAGE dynamic arrays where each element can have
 * an associated linked list of additional data.
 */
struct ListNode {
    void* pData;        // +0x00: Pointer to data object (has vtable)
    uint32_t field_04;  // +0x04: Unknown field
    ListNode* pNext;    // +0x08: Next node in linked list
};

static_assert(sizeof(ListNode) == 12, "ListNode must be 12 bytes");

/**
 * rage_8118 @ 0x821A8118 | size: 0x68
 * 
 * Destructs a linked list starting from a node.
 * 
 * This function:
 * 1. Traverses the linked list starting from node->pNext
 * 2. For each node, calls vtable[0] with parameter 1 (destructor with free)
 * 3. After traversing the list, frees the initial node's pData
 * 
 * The function does NOT free the ListNode structures themselves,
 * only the data they point to. The nodes are part of an array
 * that will be freed by the caller.
 * 
 * Parameters:
 *   pNode - Pointer to ListNode to destruct
 */
extern "C" void rage_8118(ListNode* pNode) {
    // Traverse and destruct all nodes in the linked list
    ListNode* pCurrent = pNode->pNext;
    
    while (pCurrent != nullptr) {
        // Get vtable from the data object
        void** vtable = *reinterpret_cast<void***>(pCurrent->pData);
        
        // Save next pointer before destroying current
        ListNode* pNext = pCurrent->pNext;
        
        // Call vtable[0] (destructor) with parameter 1 (free flag)
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = reinterpret_cast<DestructorFn>(vtable[0]);
        destructor(pCurrent->pData, 1);
        
        // Move to next node
        pCurrent = pNext;
    }
    
    // Free the initial node's data
    rage_free_00C0(pNode->pData);
}

/**
 * rage_2980 @ 0x820C2980 | size: 0x5C
 * 
 * Destructs and frees a dynamically allocated array of ListNodes.
 * 
 * Array layout:
 *   -4 bytes: uint32_t element count
 *   +0 bytes: ListNode array (12 bytes per element)
 * 
 * This function:
 * 1. Adjusts pointer back 4 bytes to access the count header
 * 2. Iterates through elements in reverse order
 * 3. Calls rage_8118 to destruct each ListNode and its linked list
 * 4. Frees the entire allocation including header
 * 
 * Called by:
 * - rage_8070: Part of a larger object destructor
 * - atSingleton_8588_g: During array reallocation/growth
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
        rage_8118(reinterpret_cast<ListNode*>(pElement));
    }
    
    // Free the entire allocation (including header)
    rage_free_00C0(pHeader);
    
    // Return pointer to freed block
    return pHeader;
}
