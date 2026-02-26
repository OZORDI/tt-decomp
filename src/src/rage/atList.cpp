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
