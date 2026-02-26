/**
 * atList.hpp — RAGE Engine Intrusive Doubly-Linked List
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Intrusive list implementation where nodes are embedded directly in objects.
 * This avoids separate allocations and provides O(1) insertion/removal.
 * 
 * USAGE PATTERNS IN THE GAME:
 * 
 * 1. rlGamer (Online Player Management)
 *    - Vtable @ 0x820668C0
 *    - List vtable @ 0x820668EC
 *    - Manages active player sessions
 * 
 * 2. sysThreadWorker (Thread Work Queues)
 *    - Vtable @ 0x82069FB0
 *    - Manages pending work items for thread pool
 * 
 * 3. Animation System
 *    - Vtable @ 0x82079728
 *    - Tracks active blend states and transitions
 * 
 * 4. NetBallHitManagerFreeList (Network Packet Pooling)
 *    - Functions @ 0x823CF878 - 0x823D06E0
 *    - Debug string: "NetBallHitManagerFreeList::Get() failed"
 *    - Manages free list of network hit packets
 * 
 * BASIC USAGE:
 * 
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
 *   AT_LIST_FOR_EACH(node, &list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       // Use obj...
 *   }
 * 
 * ADVANCED USAGE (Safe Removal During Iteration):
 * 
 *   AT_LIST_FOR_EACH_SAFE(node, next, &list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       if (shouldRemove) {
 *           list.RemoveNode(node);
 *       }
 *   }
 * 
 * MEMORY LAYOUT NOTES:
 * 
 * atDLNode is 20 bytes (0x14):
 *   +0x00: m_pPrev  - Previous node
 *   +0x04: m_pOwner - Owning list
 *   +0x08: m_pNext  - Next node
 *   +0x0C: padding  - Alignment
 *   +0x10: m_flags  - State flags (FLAG_DETACHED = 0x80)
 * 
 * atSafeDLListSimple is 28 bytes (0x1C):
 *   +0x00: m_pVTable - Virtual table (15 specializations)
 *   +0x04: m_pOwner  - Parent container
 *   +0x08: m_pHead   - First node
 *   +0x0C: m_pTail   - Last node
 *   +0x10: m_pParent - Parent list node (for nested lists)
 *   +0x14: m_count   - Element count
 *   +0x18: m_flags   - List state flags
 * 
 * VTABLE SPECIALIZATIONS (15 found):
 *   0x820668EC - rlGamer lists
 *   0x82068140 - rlSession lists
 *   0x82073D3C - Generic lists
 *   0x82079728 - Animation system
 *   ... (11 more)
 */

#pragma once
#include <cstdint>
#include <cstddef>

namespace rage {

// Forward declarations
class atDLNode;
class atSafeDLListSimple;

/**
 * atDLNode — Intrusive List Node (20 bytes)
 * @ 0x82073D3C (vtable)
 * 
 * Embedded in objects to link them into lists.
 * Maintains prev/next pointers and owner list reference.
 * 
 * Memory Layout (verified from assembly @ 0x823735A8):
 *   +0x00: m_pPrev  - Previous node in list
 *   +0x04: m_pOwner - Pointer to owning list
 *   +0x08: m_pNext  - Next node in list
 *   +0x0C: padding  - Alignment (4 bytes)
 *   +0x10: m_flags  - Node state flags (1 byte)
 *   +0x11: padding  - Alignment (3 bytes)
 * 
 * Total: 20 bytes (0x14)
 */
class atDLNode {
public:
    atDLNode() : m_pPrev(nullptr), m_pOwner(nullptr), m_pNext(nullptr), m_padding(0), m_flags(0) {
        m_pad[0] = m_pad[1] = m_pad[2] = 0;
    }
    
    // Check if this node is currently in a list
    bool IsLinked() const { return m_pOwner != nullptr; }
    
    // Remove this node from its list (if any)
    void Unlink();
    
    // Get next/prev nodes for iteration
    atDLNode* GetNext() const { return m_pNext; }
    atDLNode* GetPrev() const { return m_pPrev; }
    
    // Get the list this node belongs to
    void* GetOwner() const { return m_pOwner; }

protected:
    friend class atSafeDLListSimple;
    friend class atDLList;
    
    atDLNode*   m_pPrev;     // +0x00 Previous node in list
    void*       m_pOwner;    // +0x04 Owner list pointer
    atDLNode*   m_pNext;     // +0x08 Next node in list
    uint32_t    m_padding;   // +0x0C Alignment padding
    uint8_t     m_flags;     // +0x10 Node state flags
    uint8_t     m_pad[3];    // +0x11 Alignment padding
    
    // Node state flags
    static constexpr uint8_t FLAG_DETACHED = 0x80;
};

// Compile-time size verification
static_assert(sizeof(atDLNode) == 20, "atDLNode must be exactly 20 bytes");

/**
 * atSafeDLListSimple — Thread-Safe Doubly-Linked List (28 bytes)
 * 
 * Vtables (15 specializations found):
 *   @ 0x820668EC - Used by rlGamer
 *   @ 0x82068140 - Used by rlSession
 *   @ 0x82073D3C - Generic list vtable
 *   @ 0x82079728 - Animation system
 *   ... (11 more specializations)
 * 
 * Manages a doubly-linked list with head/tail tracking and element counting.
 * The "Safe" prefix suggests thread-safety or validation checks.
 * 
 * This is the primary list type used throughout the RAGE engine for:
 * - rlGamer: Online player management
 * - sysThreadWorker: Thread work queues
 * - Animation system: Blend state lists
 * - Network queues: NetBallHitManagerFreeList
 * 
 * Memory Layout (verified from assembly @ 0x823735A8):
 *   +0x00: m_pVTable - Virtual table pointer
 *   +0x04: m_pOwner  - Owner object (parent container)
 *   +0x08: m_pHead   - First node in list
 *   +0x0C: m_pTail   - Last node in list
 *   +0x10: m_pParent - Parent list node (for nested lists)
 *   +0x14: m_count   - Number of elements
 *   +0x18: m_flags   - List state flags
 *   +0x19: padding   - Alignment (3 bytes)
 * 
 * Total: 28 bytes (0x1C)
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
    
    // Iteration support
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
    uint8_t     _pad[3];    // +0x19 Alignment padding
};

// Compile-time size verification
static_assert(sizeof(atSafeDLListSimple) == 28, "atSafeDLListSimple must be exactly 28 bytes");

/**
 * atDLList — Standard Doubly-Linked List
 * 
 * Simpler, non-thread-safe version for single-threaded contexts.
 * Faster than atSafeDLListSimple due to fewer safety checks.
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
    for (atDLNode* node = (list)->GetHead(); node != nullptr; node = node->GetNext())

/**
 * Safe iteration that allows removing the current node.
 * 
 * Usage:
 *   AT_LIST_FOR_EACH_SAFE(node, next, &list) {
 *       MyObject* obj = AT_LIST_ENTRY(node, MyObject, m_listNode);
 *       if (shouldRemove) list.RemoveNode(node);
 *   }
 */
#define AT_LIST_FOR_EACH_SAFE(node, next, list) \
    for (atDLNode* node = (list)->GetHead(), *next = node ? node->GetNext() : nullptr; \
         node != nullptr; \
         node = next, next = node ? node->GetNext() : nullptr)

} // namespace rage
