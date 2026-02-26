/**
 * atList.hpp — RAGE Engine Intrusive Doubly-Linked List
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Intrusive list implementation where nodes are embedded directly in objects.
 * This avoids separate allocations and provides O(1) insertion/removal.
 * 
 * Usage pattern:
 *   struct MyObject {
 *       atDLNode m_listNode;  // Embedded node
 *       int m_data;
 *   };
 * 
 *   atSafeDLList<MyObject> list;
 *   MyObject obj;
 *   list.AddTail(&obj.m_listNode);
 */

#pragma once
#include <cstdint>
#include <cstddef>

namespace rage {

// Forward declarations
class atDLNode;
class atSafeDLListSimple;

/**
 * atDLNode — Intrusive List Node
 * @ 0x82073D3C (vtable)
 * 
 * Embedded in objects to link them into lists.
 * Maintains prev/next pointers and owner list reference.
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

protected:
    friend class atSafeDLListSimple;
    
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
 * @ 0x820668EC (vtable), @ 0x82073D3C (vtable)
 * 
 * Manages a doubly-linked list with head/tail tracking and element counting.
 * The "Safe" prefix suggests thread-safety or validation checks.
 * 
 * This is the primary list type used throughout the RAGE engine for
 * managing collections of game objects, animation states, etc.
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
    uint8_t     _pad[3];
};

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
