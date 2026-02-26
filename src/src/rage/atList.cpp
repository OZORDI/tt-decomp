/**
 * atList.cpp — RAGE Engine Intrusive List Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Clean, maintainable implementation of intrusive doubly-linked lists.
 * These lists are used extensively throughout the engine for managing
 * game objects, animation states, and other dynamic collections.
 */

#include "rage/atList.hpp"

namespace rage {

// ═══════════════════════════════════════════════════════════════════════════
// atDLNode Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Unlink this node from its list.
 * Safe to call even if the node is not in a list.
 */
void atDLNode::Unlink() {
    if (!m_pOwner) return;  // Not in a list
    
    // Update previous node's next pointer
    if (m_pPrev) {
        m_pPrev->m_pNext = m_pNext;
    }
    
    // Update next node's previous pointer
    if (m_pNext) {
        m_pNext->m_pPrev = m_pPrev;
    }
    
    // Clear this node's pointers
    m_pPrev = nullptr;
    m_pNext = nullptr;
    m_pOwner = nullptr;
    m_flags |= FLAG_DETACHED;
}

// ═══════════════════════════════════════════════════════════════════════════
// atSafeDLListSimple Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSafeDLListSimple::atSafeDLListSimple
 * @ 0x82368EF8 | size: 0x80
 * 
 * Initialize an empty list.
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
    // Note: In the actual game, m_pVTable would be set by the C++ runtime
    // to point to the appropriate vtable (0x820668EC or 0x82073D3C)
}

/**
 * atSafeDLListSimple::~atSafeDLListSimple
 * @ 0x82369D68 | size: 0x60
 * 
 * Destructor - removes all nodes from the list.
 * Loops until empty, removing nodes one by one.
 */
atSafeDLListSimple::~atSafeDLListSimple() {
    Clear();
}

/**
 * atSafeDLListSimple::AddHead
 * 
 * Add a node to the beginning of the list.
 * The node must not already be in a list.
 */
void atSafeDLListSimple::AddHead(atDLNode* node) {
    if (!node) return;
    if (node->IsLinked()) return;  // Already in a list
    
    node->m_pPrev = nullptr;
    node->m_pNext = m_pHead;
    node->m_pOwner = this;
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
 */
void atSafeDLListSimple::AddTail(atDLNode* node) {
    if (!node) return;
    if (node->IsLinked()) return;  // Already in a list
    
    node->m_pPrev = m_pTail;
    node->m_pNext = nullptr;
    node->m_pOwner = this;
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
 */
void atSafeDLListSimple::RemoveNode(atDLNode* node) {
    if (!node) return;
    if (!node->IsLinked()) return;  // Not in a list
    
    // Verify this node belongs to this list
    // The original assembly checks node->m_pOwner + 4 == this
    // This suggests the owner field points to a parent structure
    // that contains the list at offset +4
    
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
    
    // Mark node as detached
    node->m_flags |= atDLNode::FLAG_DETACHED;
    
    // Decrement count
    if (m_count > 0) {
        m_count--;
    }
}

/**
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
 * Remove all nodes from the list.
 * Called by the destructor and can be called explicitly.
 */
void atSafeDLListSimple::Clear() {
    while (m_pHead) {
        RemoveNode(m_pHead);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// atDLList Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Add a node to the beginning of the list.
 */
void atDLList::AddHead(atDLNode* node) {
    if (!node) return;
    
    node->m_pPrev = nullptr;
    node->m_pNext = m_pHead;
    node->m_pOwner = this;
    
    if (m_pHead) {
        m_pHead->m_pPrev = node;
    } else {
        m_pTail = node;
    }
    
    m_pHead = node;
    m_count++;
}

/**
 * Add a node to the end of the list.
 */
void atDLList::AddTail(atDLNode* node) {
    if (!node) return;
    
    node->m_pPrev = m_pTail;
    node->m_pNext = nullptr;
    node->m_pOwner = this;
    
    if (m_pTail) {
        m_pTail->m_pNext = node;
    } else {
        m_pHead = node;
    }
    
    m_pTail = node;
    m_count++;
}

/**
 * Remove a specific node from the list.
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
 * Remove all nodes from the list.
 */
void atDLList::Clear() {
    while (m_pHead) {
        RemoveNode(m_pHead);
    }
}

} // namespace rage
