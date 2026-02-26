/**
 * atList.hpp — RAGE Engine Intrusive Doubly-Linked List
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Intrusive list implementation where nodes are embedded directly in objects.
 * This avoids separate allocations and provides O(1) insertion/removal.
 * 
 * The RAGE engine uses this pattern extensively for managing collections of:
 * - Game objects (players, balls, props)
 * - Animation states and blenders (rlGamer, pcrFaceAnimBlender)
 * - Network messages and sessions (rlGamer online system)
 * - Thread work items (sysThreadWorker)
 * - Resource pools and memory allocators
 * 
 * Key characteristics:
 * - Zero allocations (nodes embedded in objects)
 * - O(1) insertion, removal, and size queries
 * - Thread-safe variant (atSafeDLListSimple) with owner tracking
 * - 15 different vtable specializations found in binary (template-like behavior)
 * - Used in rlGamer @ +8, +132, sysThreadWorker, and many other systems
 * 
 * Usage pattern:
 *   struct MyObject {
 *       atDLNode m_listNode;  // Embedded node
 *       int m_data;
 *   };
 * 
 *   atSafeDLListSimple list;
 *   MyObject obj;
 *   list.AddTail(&obj.m_listNode);
 *   
 *   // Iterate
 *   for (atDLNode* node : list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       // Use obj...
 *   }
 */

#pragma once
#include <cstdint>
#include <cstddef>

#ifndef NDEBUG
#include <cassert>
#define AT_LIST_ASSERT(x) assert(x)
#else
#define AT_LIST_ASSERT(x) ((void)0)
#endif

namespace rage {

// Forward declarations
class atDLNode;
class atSafeDLListSimple;
class atDLList;

/**
 * atDLNode — Intrusive List Node
 * @ 0x82073D3C (vtable)
 * 
 * Embedded in objects to link them into lists.
 * Maintains prev/next pointers and owner list reference.
 * 
 * Memory layout (16 bytes):
 *   +0x00: m_pPrev  - Previous node in list
 *   +0x04: m_pNext  - Next node in list
 *   +0x08: m_pOwner - Owner list pointer
 *   +0x0C: m_flags  - Node state flags (bit 7 = detached)
 * 
 * The node must be embedded in your object, not allocated separately:
 *   struct MyObject {
 *       atDLNode m_listNode;  // Correct - embedded
 *       // NOT: atDLNode* m_pListNode;  // Wrong - pointer!
 *   };
 * 
 * Safety features:
 * - IsLinked() prevents double-insertion
 * - FLAG_DETACHED (0x80) marks removed nodes
 * - Owner pointer tracks which list owns the node
 */
class atDLNode {
public:
    atDLNode() : m_pPrev(nullptr), m_pNext(nullptr), m_pOwner(nullptr), m_flags(0) {}
    
    // Check if this node is currently in a list
    bool IsLinked() const { return m_pOwner != nullptr; }
    
    // Remove this node from its list (if any)
    void Unlink();
    
    // Get next/prev nodes for iteration
    atDLNode* GetNext() const { return m_pNext; }
    atDLNode* GetPrev() const { return m_pPrev; }
    
    // Get the list this node belongs to
    void* GetOwner() const { return m_pOwner; }
    
    // Check if node is detached (removed but not cleared)
    bool IsDetached() const { return (m_flags & FLAG_DETACHED) != 0; }

protected:
    friend class atSafeDLListSimple;
    friend class atDLList;
    
    atDLNode*   m_pPrev;    // +0x00 Previous node in list
    atDLNode*   m_pNext;    // +0x04 Next node in list  
    void*       m_pOwner;   // +0x08 Owner list pointer
    uint8_t     m_flags;    // +0x0C Node state flags
    uint8_t     _pad[3];
    
    // Node state flags
    static constexpr uint8_t FLAG_DETACHED = 0x80;
};

/**
 * atSafeDLListSimple — Thread-Safe Doubly-Linked List
 * 
 * Vtables found (15 specializations):
 *   @ 0x820668EC, 0x82068140, 0x82068310, 0x82071F30, 0x82073D3C
 *   @ 0x820785C8, 0x820785E0, 0x82079220, 0x82079330, 0x820793A0
 *   @ 0x8207946C, 0x8207953C, 0x82079604, 0x820796E8, 0x82079728
 * 
 * Manages a doubly-linked list with head/tail tracking and element counting.
 * The "Safe" prefix suggests thread-safety or validation checks.
 * 
 * This is the primary list type used throughout the RAGE engine for
 * managing collections of game objects, animation states, etc.
 * 
 * Memory layout (28 bytes):
 *   +0x00: m_pVTable - Virtual table pointer (15 different vtables found)
 *   +0x04: m_pOwner  - Owner object (parent container)
 *   +0x08: m_pHead   - First node in list
 *   +0x0C: m_pTail   - Last node in list
 *   +0x10: m_pParent - Parent list node (for nested lists)
 *   +0x14: m_count   - Number of elements
 *   +0x18: m_flags   - List state flags
 * 
 * Known usage locations:
 * - rlGamer class @ +8 (online gaming system)
 * - rlGamer class @ +132 (embedded sysThreadWorker)
 * - Animation blender systems
 * - Network message queues
 * - Multiple lists at offsets +44, +3368, +3412, +3456 in parent structures
 * 
 * Functions:
 * - Constructor: @ 0x82368EF8 | size: 0x80
 * - Destructor:  @ 0x82369D68 | size: 0x60
 * - RemoveNode:  @ 0x823735A8 | size: 0xA4
 * 
 * Debug strings found:
 * - "fiDevice::GetDevice - Filename '%s' not in mount list, no default device"
 * - "sysMemSimpleAllocator::Allocate - memory node at %p had guard word trashed!"
 * - "NetBallHitManagerFreeList::Get() failed"
 */
class atSafeDLListSimple {
public:
    atSafeDLListSimple();
    ~atSafeDLListSimple();
    
    // Add nodes to the list
    void AddHead(atDLNode* node);
    void AddTail(atDLNode* node);
    
    // Remove a specific node
    void RemoveNode(atDLNode* node);
    
    // Remove and return head/tail
    atDLNode* RemoveHead();
    atDLNode* RemoveTail();
    
    // Clear all nodes
    void Clear();
    
    // Query operations
    bool IsEmpty() const { return m_pHead == nullptr; }
    uint32_t GetCount() const { return m_count; }
    
    // Access head/tail
    atDLNode* GetHead() const { return m_pHead; }
    atDLNode* GetTail() const { return m_pTail; }
    
    // Get owner/parent for nested lists
    void* GetOwner() const { return m_pOwner; }
    atDLNode* GetParent() const { return m_pParent; }
    
    // Iteration support (C++11 range-based for loops)
    class Iterator {
    public:
        Iterator(atDLNode* node) : m_pCurrent(node) {}
        
        atDLNode* operator*() const { return m_pCurrent; }
        atDLNode* operator->() const { return m_pCurrent; }
        
        Iterator& operator++() {
            if (m_pCurrent) m_pCurrent = m_pCurrent->GetNext();
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return m_pCurrent != other.m_pCurrent;
        }
        
    private:
        atDLNode* m_pCurrent;
    };
    
    Iterator begin() const { return Iterator(m_pHead); }
    Iterator end() const { return Iterator(nullptr); }

protected:
    void*       m_pVTable;  // +0x00 Virtual table pointer
    void*       m_pOwner;   // +0x04 Owner object (parent container)
    atDLNode*   m_pHead;    // +0x08 First node in list
    atDLNode*   m_pTail;    // +0x0C Last node in list
    atDLNode*   m_pParent;  // +0x10 Parent list node (for nested lists)
    uint32_t    m_count;    // +0x14 Number of elements
    uint8_t     m_flags;    // +0x18 List state flags
    uint8_t     _pad[3];
};

/**
 * atDLList — Standard Doubly-Linked List
 * 
 * Simpler, non-thread-safe version for single-threaded contexts.
 * Faster than atSafeDLListSimple due to fewer safety checks.
 * 
 * Memory layout (16 bytes):
 *   +0x00: m_pVTable - Virtual table pointer
 *   +0x04: m_pHead   - First node
 *   +0x08: m_pTail   - Last node
 *   +0x0C: m_count   - Element count
 */
class atDLList {
public:
    atDLList() : m_pVTable(nullptr), m_pHead(nullptr), m_pTail(nullptr), m_count(0) {}
    ~atDLList() { Clear(); }
    
    void AddHead(atDLNode* node);
    void AddTail(atDLNode* node);
    void RemoveNode(atDLNode* node);
    void Clear();
    
    bool IsEmpty() const { return m_pHead == nullptr; }
    uint32_t GetCount() const { return m_count; }
    
    atDLNode* GetHead() const { return m_pHead; }
    atDLNode* GetTail() const { return m_pTail; }

protected:
    void*       m_pVTable;  // +0x00 Virtual table pointer
    atDLNode*   m_pHead;    // +0x04 First node
    atDLNode*   m_pTail;    // +0x08 Last node
    uint32_t    m_count;    // +0x0C Element count
};

/**
 * Helper macro to get the containing object from a list node.
 * 
 * This implements the "container_of" pattern used throughout the Linux kernel
 * and many C/C++ projects. It calculates the object address by subtracting
 * the member offset from the node address.
 * 
 * Usage:
 *   atDLNode* node = list.GetHead();
 *   MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 */
#define AT_LIST_ENTRY(node, type, member) \
    ((type*)((char*)(node) - offsetof(type, member)))

/**
 * Iterate over all nodes in a list.
 * 
 * Usage:
 *   AT_LIST_FOR_EACH(node, &list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       // Use obj...
 *   }
 */
#define AT_LIST_FOR_EACH(node, list) \
    for (rage::atDLNode* node = (list)->GetHead(); node != nullptr; node = node->GetNext())

/**
 * Safe iteration that allows removing the current node.
 * 
 * This macro saves the next pointer before processing each node,
 * so you can safely remove the current node without breaking iteration.
 * 
 * Usage:
 *   AT_LIST_FOR_EACH_SAFE(node, next, &list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       if (shouldRemove) list.RemoveNode(node);
 *   }
 */
#define AT_LIST_FOR_EACH_SAFE(node, next, list) \
    for (rage::atDLNode* node = (list)->GetHead(), *next = node ? node->GetNext() : nullptr; \
         node != nullptr; \
         node = next, next = node ? node->GetNext() : nullptr)

} // namespace rage
