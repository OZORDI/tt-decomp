/**
 * atList.cpp — RAGE Engine Intrusive List Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Clean, maintainable implementation of intrusive doubly-linked lists.
 * These lists are used extensively throughout the engine for managing
 * game objects, animation states, and other dynamic collections.
 * 
 * Key usage patterns:
 * - rlGamer: Online player session management
 * - sysThreadWorker: Thread work queue management
 * - Animation system: Blend state tracking
 * - NetBallHitManagerFreeList: Network packet pooling
 * 
 * Debug strings found:
 * - "NetBallHitManagerFreeList::Get() failed"
 * - "fiDevice::GetDevice - Filename '%s' not in mount list"
 * - "sysMemSimpleAllocator::Allocate - memory node at %p had guard word trashed!"
 */

#include "rage/atList.hpp"
#include <cassert>

namespace rage {

// ═══════════════════════════════════════════════════════════════════════════
// atDLNode Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atDLNode::Unlink
 * 
 * Unlink this node from its list.
 * Safe to call even if the node is not in a list.
 * 
 * This is called internally by list operations and can also be called
 * directly on a node to remove it from whatever list it's in.
 */
void atDLNode::Unlink() {
    if (!m_pOwner) return;  // Not in a list
    
    // Cast owner to list type to access its members
    atSafeDLListSimple* list = static_cast<atSafeDLListSimple*>(m_pOwner);
    
    // Update previous node's next pointer
    if (m_pPrev) {
        m_pPrev->m_pNext = m_pNext;
    } else {
        // This was the head node
        list->m_pHead = m_pNext;
    }
    
    // Update next node's previous pointer
    if (m_pNext) {
        m_pNext->m_pPrev = m_pPrev;
    } else {
        // This was the tail node
        list->m_pTail = m_pPrev;
    }
    
    // Clear this node's pointers
    m_pPrev = nullptr;
    m_pNext = nullptr;
    m_pOwner = nullptr;
    m_flags |= FLAG_DETACHED;
    
    // Decrement list count
    if (list->m_count > 0) {
        list->m_count--;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// atSafeDLListSimple Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSafeDLListSimple::atSafeDLListSimple
 * @ 0x82368EF8 | size: 0x80
 * 
 * Initialize an empty list.
 * 
 * Original assembly shows this is called from rlGamer constructor,
 * which sets up the vtable pointer to one of 15 specializations.
 */
atSafeDLListSimple::atSafeDLListSimple()
    : m_pVTable(nullptr)
    , m_pOwner(nullptr)
    , m_pHead(nullptr)
    , m_pTail(nullptr)
    , m_pParent(nullptr)
    , m_count(0)
    , m_flags(0)
{
    _pad[0] = _pad[1] = _pad[2] = 0;
    
    // Note: In the actual game, m_pVTable would be set by the C++ runtime
    // to point to the appropriate vtable based on the template specialization:
    // - 0x820668EC for rlGamer lists
    // - 0x82068140 for rlSession lists
    // - 0x82073D3C for generic lists
    // - etc. (15 total specializations)
}

/**
 * atSafeDLListSimple::~atSafeDLListSimple
 * @ 0x82369D68 | size: 0x60
 * 
 * Destructor - removes all nodes from the list.
 * Loops until empty, removing nodes one by one.
 * 
 * Original assembly shows a simple loop:
 *   while (m_pHead != nullptr) {
 *       RemoveNode(m_pHead);
 *   }
 */
atSafeDLListSimple::~atSafeDLListSimple() {
    Clear();
}

/**
 * atSafeDLListSimple::AddHead
 * 
 * Add a node to the beginning of the list.
 * The node must not already be in a list.
 * 
 * This is O(1) and updates:
 * - Node's prev/next/owner pointers
 * - List's head pointer (and tail if list was empty)
 * - List's count
 */
void atSafeDLListSimple::AddHead(atDLNode* node) {
    if (!node) return;
    if (node->IsLinked()) return;  // Already in a list
    
    node->m_pPrev = nullptr;
    node->m_pOwner = this;
    node->m_pNext = m_pHead;
    node->m_flags &= ~atDLNode::FLAG_DETACHED;
    
    if (m_pHead) {
        m_pHead->m_pPrev = node;
    } else {
        // List was empty - this is also the tail
        m_pTail = node;
    }
    
    m_pHead = node;
    m_count++;
}

/**
 * atSafeDLListSimple::AddTail
 * 
 * Add a node to the end of the list.
 * The node must not already be in a list.
 * 
 * This is O(1) and updates:
 * - Node's prev/next/owner pointers
 * - List's tail pointer (and head if list was empty)
 * - List's count
 */
void atSafeDLListSimple::AddTail(atDLNode* node) {
    if (!node) return;
    if (node->IsLinked()) return;  // Already in a list
    
    node->m_pPrev = m_pTail;
    node->m_pOwner = this;
    node->m_pNext = nullptr;
    node->m_flags &= ~atDLNode::FLAG_DETACHED;
    
    if (m_pTail) {
        m_pTail->m_pNext = node;
    } else {
        // List was empty - this is also the head
        m_pHead = node;
    }
    
    m_pTail = node;
    m_count++;
}

/**
 * atSafeDLListSimple::RemoveNode
 * @ 0x823735A8 | size: 0xA4
 * 
 * Remove a specific node from the list.
 * Updates all pointers and decrements the count.
 * 
 * This is the core list manipulation function that handles:
 * - Head/tail pointer updates when removing first/last nodes
 * - Prev/next link updates for interior nodes
 * - Count decrement
 * - Node flag marking (detached state)
 * 
 * Assembly analysis shows this function:
 * 1. Loads m_pOwner from node+4
 * 2. Accesses list structure at owner+16 (m_pParent field)
 * 3. Updates prev/next linkage
 * 4. Sets FLAG_DETACHED (0x80) at node+16
 * 5. Decrements count at owner+20 (relative to owner+16 base)
 */
void atSafeDLListSimple::RemoveNode(atDLNode* node) {
    if (!node) return;
    if (!node->IsLinked()) return;  // Not in a list
    if (node->m_pOwner != this) return;  // Not in THIS list
    
    atDLNode* prev = node->m_pPrev;
    atDLNode* next = node->m_pNext;
    
    // Update head pointer if removing first node
    if (prev == nullptr) {
        m_pHead = next;
        if (next) {
            next->m_pPrev = nullptr;
        } else {
            // List is now empty
            m_pTail = nullptr;
        }
    } else {
        // Update previous node's next pointer
        prev->m_pNext = next;
        if (next) {
            next->m_pPrev = prev;
        } else {
            // Removing tail node
            m_pTail = prev;
        }
    }
    
    // Clear node pointers
    node->m_pPrev = nullptr;
    node->m_pNext = nullptr;
    node->m_pOwner = nullptr;
    
    // Mark node as detached (matches assembly: ori r11,r3,128)
    node->m_flags |= atDLNode::FLAG_DETACHED;
    
    // Decrement count
    if (m_count > 0) {
        m_count--;
    }
}

/**
 * atSafeDLListSimple::RemoveHead
 * 
 * Remove and return the first node in the list.
 * Returns nullptr if the list is empty.
 */
atDLNode* atSafeDLListSimple::RemoveHead() {
    if (!m_pHead) return nullptr;
    
    atDLNode* node = m_pHead;
    RemoveNode(node);
    return node;
}

/**
 * atSafeDLListSimple::RemoveTail
 * 
 * Remove and return the last node in the list.
 * Returns nullptr if the list is empty.
 */
atDLNode* atSafeDLListSimple::RemoveTail() {
    if (!m_pTail) return nullptr;
    
    atDLNode* node = m_pTail;
    RemoveNode(node);
    return node;
}

/**
 * atSafeDLListSimple::Clear
 * 
 * Remove all nodes from the list.
 * Called by the destructor and can be called explicitly.
 * 
 * This is safe even if nodes are removed during iteration
 * because we always remove from the head.
 */
void atSafeDLListSimple::Clear() {
    while (m_pHead) {
        RemoveNode(m_pHead);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// atDLList Implementation (Simpler Non-Thread-Safe Variant)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atDLList::AddHead
 * 
 * Add a node to the beginning of the list.
 * Simpler version without thread-safety checks.
 */
void atDLList::AddHead(atDLNode* node) {
    if (!node) return;
    
    node->m_pPrev = nullptr;
    node->m_pOwner = this;
    node->m_pNext = m_pHead;
    
    if (m_pHead) {
        m_pHead->m_pPrev = node;
    } else {
        m_pTail = node;
    }
    
    m_pHead = node;
    m_count++;
}

/**
 * atDLList::AddTail
 * 
 * Add a node to the end of the list.
 * Simpler version without thread-safety checks.
 */
void atDLList::AddTail(atDLNode* node) {
    if (!node) return;
    
    node->m_pPrev = m_pTail;
    node->m_pOwner = this;
    node->m_pNext = nullptr;
    
    if (m_pTail) {
        m_pTail->m_pNext = node;
    } else {
        m_pHead = node;
    }
    
    m_pTail = node;
    m_count++;
}

/**
 * atDLList::RemoveNode
 * 
 * Remove a specific node from the list.
 * Simpler version without thread-safety checks.
 */
void atDLList::RemoveNode(atDLNode* node) {
    if (!node) return;
    
    // Update prev node's next pointer
    if (node->m_pPrev) {
        node->m_pPrev->m_pNext = node->m_pNext;
    } else {
        m_pHead = node->m_pNext;
    }
    
    // Update next node's prev pointer
    if (node->m_pNext) {
        node->m_pNext->m_pPrev = node->m_pPrev;
    } else {
        m_pTail = node->m_pPrev;
    }
    
    // Clear node pointers
    node->m_pPrev = nullptr;
    node->m_pNext = nullptr;
    node->m_pOwner = nullptr;
    
    // Decrement count
    if (m_count > 0) {
        m_count--;
    }
}

/**
 * atDLList::Clear
 * 
 * Remove all nodes from the list.
 */
void atDLList::Clear() {
    while (m_pHead) {
        RemoveNode(m_pHead);
    }
}

} // namespace rage

// ═══════════════════════════════════════════════════════════════════════════
// Template Specialization Destructors
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSafeDLListSimple destructor variant @ 0x82369D68
 * 
 * Specialized destructor for one of the 15 template instantiations.
 * Sets vtable to 0x820668F8 and clears all nodes from the list.
 * 
 * This pattern appears across all template specializations:
 * 1. Set the appropriate vtable pointer for this specialization
 * 2. Loop until list is empty, removing nodes one by one
 * 
 * The vtable address identifies which template type this is:
 * - Different vtables for rlGamer, rlSession, sysThreadWorker, etc.
 */
void atSafeDLListSimple_9D68(atSafeDLListSimple* list) {
    // Set vtable for this specialization
    list->m_pVTable = (void*)0x820668F8;
    
    // Clear all nodes from the list
    while (list->m_pHead != nullptr) {
        RemoveNode(list->m_pHead);
    }
}

/**
 * atSafeDLListSimple destructor variant @ 0x82371D30
 * 
 * Specialized destructor with vtable 0x8206814C.
 * Identical logic to other variants, just different vtable address.
 */
void atSafeDLListSimple_1D30(atSafeDLListSimple* list) {
    list->m_pVTable = (void*)0x8206814C;
    
    while (list->m_pHead != nullptr) {
        RemoveNode(list->m_pHead);
    }
}

/**
 * atSafeDLListSimple destructor variant @ 0x824244F0
 * 
 * Specialized destructor with vtable 0x8207922C.
 */
void atSafeDLListSimple_44F0_g(atSafeDLListSimple* list) {
    list->m_pVTable = (void*)0x8207922C;
    
    while (list->m_pHead != nullptr) {
        RemoveNode(list->m_pHead);
    }
}

/**
 * atSafeDLListSimple destructor variant @ 0x823E8FB0
 * 
 * Specialized destructor with vtable 0x82073D48.
 * Used by snSession and related networking classes.
 */
void atSafeDLListSimple_8FB0(atSafeDLListSimple* list) {
    list->m_pVTable = (void*)0x82073D48;
    
    while (list->m_pHead != nullptr) {
        RemoveNode(list->m_pHead);
    }
}

/**
 * atSafeDLListSimple destructor variant @ 0x823DB8A8
 * 
 * More complex destructor with vtable 0x82071F3C.
 * 
 * This variant manually unlinks nodes from their owner list before clearing.
 * The extra complexity suggests this is used for lists that may have nodes
 * shared across multiple lists or with special cleanup requirements.
 * 
 * Assembly shows:
 * - Loads node at offset +16 (m_pHead)
 * - Checks if node has owner at offset +4
 * - Manually updates prev/next pointers
 * - Sets FLAG_DETACHED (0x80) at node+16
 * - Decrements count in owner list
 */
void atSafeDLListSimple_B8A8(atSafeDLListSimple* list) {
    list->m_pVTable = (void*)0x82071F3C;
    
    while (list->m_pHead != nullptr) {
        atDLNode* node = list->m_pHead;
        
        // Check if node has an owner list
        atSafeDLListSimple* owner = static_cast<atSafeDLListSimple*>(node->m_pOwner);
        if (owner != nullptr) {
            // Manually unlink from owner list
            atDLNode* prev = node->m_pPrev;
            atDLNode* next = node->m_pNext;
            
            if (prev == nullptr) {
                // This is the head node
                owner->m_pHead = next;
                if (next) {
                    next->m_pPrev = nullptr;
                } else {
                    owner->m_pTail = nullptr;
                }
            } else {
                // Interior or tail node
                prev->m_pNext = next;
                if (next) {
                    next->m_pPrev = prev;
                } else {
                    owner->m_pTail = prev;
                }
            }
            
            // Clear node pointers
            node->m_pNext = nullptr;
            node->m_pPrev = nullptr;
            
            // Decrement owner's count
            if (owner->m_count > 0) {
                owner->m_count--;
            }
        }
        
        // Clear node owner and mark as detached
        node->m_pOwner = nullptr;
        node->m_flags |= atDLNode::FLAG_DETACHED;
    }
}

/**
 * atSafeDLListSimple destructor with delete @ 0x82424550
 * 
 * Destructor that optionally frees the list object itself.
 * 
 * Parameters:
 *   list - The list to destroy
 *   shouldDelete - If non-zero (bit 0 set), also free the list memory
 * 
 * This is the standard C++ destructor pattern where the second parameter
 * indicates whether this is a delete (1) or just destructor call (0).
 */
void atSafeDLListSimple_rtti_9220_0(atSafeDLListSimple* list, uint32_t shouldDelete) {
    // Set vtable
    list->m_pVTable = (void*)0x82079220;
    
    // Call the base destructor to clear the list
    atSafeDLListSimple_44F0_g(list);
    
    // If delete flag is set, free the memory
    if (shouldDelete & 0x1) {
        rage_free_00C0(list);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// Constructor Variants
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSafeDLListSimple complex constructor @ 0x8236D610
 * 
 * Constructor for a large composite object containing multiple embedded lists
 * and thread worker objects. This appears to be part of the session management
 * system based on the "to join a session" debug string.
 * 
 * The constructor:
 * 1. Sets initial vtable to 0x82069F88
 * 2. Initializes three sysThreadWorker objects at offsets +3368, +3412, +3456
 * 3. Initializes an embedded atSafeDLListSimple at offset +44
 * 4. Initializes another embedded list at offset +8
 * 
 * This is likely the constructor for a session join manager or similar
 * networking component that manages multiple worker threads.
 */
void atSafeDLListSimple_ctor_D610(atSafeDLListSimple* obj) {
    // Set initial vtable
    obj->m_pVTable = (void*)0x82069F88;
    
    // Initialize the base object
    atSingleton_D6F0_w(obj);
    
    // Initialize three sysThreadWorker objects
    // These are at offsets +3368, +3412, +3456 (44-byte stride)
    for (int i = 0; i < 3; i++) {
        uint8_t* workerBase = (uint8_t*)obj + 3368 + (i * 44);
        
        // Set worker vtable (points to "to join a session" string)
        *(void**)workerBase = (void*)0x82069FB0;
        
        // Call cleanup on worker's thread handle at offset +8
        void* threadHandle = *(void**)(workerBase + 8);
        pg_C3B8_g(threadHandle, 0xFFFFFFFF);
        
        // Release worker's resources at offsets +4 and +8
        void* resource1 = *(void**)(workerBase + 4);
        pg_6F10_g(resource1);
        
        void* resource2 = *(void**)(workerBase + 8);
        pg_6F10_g(resource2);
    }
    
    // Initialize embedded list at offset +44
    atSafeDLListSimple* embeddedList1 = (atSafeDLListSimple*)((uint8_t*)obj + 44);
    embeddedList1->m_pVTable = (void*)0x82068134;
    
    // Check if list has a parent and unregister if needed
    void* parent = *(void**)((uint8_t*)embeddedList1 + 20);
    if (parent != nullptr) {
        NotifyHandler_3D80_g(parent, embeddedList1);
        *(void**)((uint8_t*)embeddedList1 + 20) = nullptr;
    }
    
    // Initialize embedded list at offset +8
    atSafeDLListSimple* embeddedList2 = (atSafeDLListSimple*)((uint8_t*)obj + 8);
    embeddedList2->m_pVTable = (void*)0x82068140;
    
    // Call destructor on the embedded list
    atSafeDLListSimple_1D30(embeddedList2);
}

/**
 * atSafeDLListSimple complex constructor @ 0x823E6750
 * 
 * Constructor for another large composite object, likely part of the
 * session or network management system.
 * 
 * This constructor:
 * 1. Sets vtable to 0x82073D34
 * 2. Initializes multiple embedded subsystems
 * 3. Sets up notification handlers
 * 4. Initializes embedded lists at various offsets
 */
void atSafeDLListSimple_ctor_6750(atSafeDLListSimple* obj) {
    // Set initial vtable
    obj->m_pVTable = (void*)0x82073D34;
    
    // Initialize base system
    rage_6968(obj);
    
    // Initialize embedded session object at offset +3792
    snSession_4EB0_w((uint8_t*)obj + 3792);
    
    // Initialize utility object at offset +3772
    util_A768((uint8_t*)obj + 3772);
    
    // Initialize game object at offset +3756
    game_60B8((uint8_t*)obj + 3756);
    
    // Initialize embedded list at offset +232
    atSafeDLListSimple_ctor_D610((atSafeDLListSimple*)((uint8_t*)obj + 232));
    
    // Initialize and clear list at offset +212
    uint8_t* list212 = (uint8_t*)obj + 212;
    do {
        util_D188(list212);
    } while (util_D188(list212) != 0);
    
    // Clear fields at offset +212
    *(uint32_t*)(list212 + 0) = 0;
    *(uint32_t*)(list212 + 4) = 0;
    
    // Initialize notification handler at offset +136
    uint8_t* handler = (uint8_t*)obj + 136;
    *(void**)handler = (void*)0x82069C50;
    
    // Unregister from parent if present
    void* parent = *(void**)(handler + 20);
    if (parent != nullptr) {
        NotifyHandler_3D80_g((uint8_t*)parent + 672, handler);
        *(void**)(handler + 20) = nullptr;
    }
    
    // Initialize embedded list at offset +108
    atSafeDLListSimple* embeddedList = (atSafeDLListSimple*)((uint8_t*)obj + 108);
    embeddedList->m_pVTable = (void*)0x82073D3C;
    
    // Call destructor on embedded list
    atSafeDLListSimple_8FB0(embeddedList);
    
    // Final initialization
    util_D6A8(obj);
}

/**
 * atSafeDLListSimple complex constructor @ 0x823DA798
 * 
 * Constructor for a game-related composite object.
 * 
 * This constructor:
 * 1. Sets vtable to 0x82071F30
 * 2. Initializes embedded list at offset +672
 * 3. Initializes game subsystems at offsets +648 and +636
 * 4. Sets up notification handler at offset +612
 */
void atSafeDLListSimple_ctor_A798(atSafeDLListSimple* obj) {
    // Initialize embedded list at offset +672
    atSafeDLListSimple* embeddedList = (atSafeDLListSimple*)((uint8_t*)obj + 672);
    embeddedList->m_pVTable = (void*)0x82071F30;
    
    // Call destructor to clear it
    atSafeDLListSimple_B8A8(embeddedList);
    
    // Initialize game subsystems
    game_CCE0((uint8_t*)obj + 648);
    game_C250((uint8_t*)obj + 636);
    
    // Initialize notification handler at offset +612
    uint8_t* handler = (uint8_t*)obj + 612;
    *(void**)handler = (void*)0x82071E40;
    
    // Unregister from parent if present
    void* parent = *(void**)(handler + 20);
    if (parent != nullptr) {
        NotifyHandler_3D80_g((uint8_t*)parent + 464, handler);
        *(void**)(handler + 20) = nullptr;
    }
    
    // Final initialization
    rage_3490(obj);
}

// ═══════════════════════════════════════════════════════════════════════════
// Utility Functions
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSafeDLListSimple data copy function @ 0x82424F38
 * 
 * Copies structured data into a buffer with string and binary components.
 * 
 * This function:
 * 1. Stores 4 uint32_t values at offsets 0, 4, 8, 12
 * 2. Copies up to 15 characters from a source string to offset +16
 * 3. Copies 24 uint32_t values (96 bytes) from one buffer to offset +32
 * 4. Calls memcpy to copy 256 bytes to offset +128
 * 
 * The structure appears to be:
 *   +0:   4 uint32_t header fields
 *   +16:  16-byte string buffer (null-terminated, max 15 chars)
 *   +32:  96 bytes of structured data (24 uint32_t values)
 *   +128: 256 bytes of additional data
 * 
 * Total structure size: at least 384 bytes
 */
void atSafeDLListSimple_4F38_p39(
    uint8_t* dest,
    uint32_t field0,
    uint32_t field1,
    uint32_t field2,
    uint32_t field3,
    const char* sourceString,
    const uint32_t* sourceData24,
    const void* sourceData256
) {
    // Store header fields
    *(uint32_t*)(dest + 0) = field0;
    *(uint32_t*)(dest + 4) = field1;
    *(uint32_t*)(dest + 8) = field2;
    *(uint32_t*)(dest + 12) = field3;
    
    // Copy string to offset +16 (max 15 characters + null terminator)
    char* destString = (char*)(dest + 16);
    int charsCopied = 0;
    int maxChars = 15;
    
    while (charsCopied < maxChars && sourceString[charsCopied] != '\0') {
        destString[charsCopied] = sourceString[charsCopied];
        charsCopied++;
    }
    
    // Null-terminate the string
    destString[charsCopied] = '\0';
    
    // Copy 24 uint32_t values (96 bytes) to offset +32
    uint32_t* dest32 = (uint32_t*)(dest + 32);
    for (int i = 0; i < 24; i++) {
        dest32[i] = sourceData24[i];
    }
    
    // Copy 256 bytes to offset +128
    memcpy(dest + 128, sourceData256, 256);
}

} // namespace rage
