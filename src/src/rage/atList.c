/**
 * atList.c — RAGE Engine Doubly-Linked List Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Intrusive doubly-linked list used for managing collections of game objects.
 * These lists are embedded directly in objects rather than allocating separate nodes.
 */

#include "rage/atList.h"
#include <string.h>

// Node state flags
#define AT_NODE_FLAG_LINKED    0x01
#define AT_NODE_FLAG_DETACHED  0x80

// ── atDLNode Implementation ──

void atDLNode_Init(struct atDLNode* node) {
    if (!node) return;
    
    node->m_pPrev = NULL;
    node->m_pNext = NULL;
    node->m_pOwner = NULL;
    node->m_flags = 0;
}

bool atDLNode_IsLinked(const struct atDLNode* node) {
    if (!node) return false;
    return (node->m_pOwner != NULL);
}

void atDLNode_Unlink(struct atDLNode* node) {
    if (!node || !node->m_pOwner) return;
    
    // Update prev node's next pointer
    if (node->m_pPrev) {
        node->m_pPrev->m_pNext = node->m_pNext;
    }
    
    // Update next node's prev pointer
    if (node->m_pNext) {
        node->m_pNext->m_pPrev = node->m_pPrev;
    }
    
    // Clear node pointers
    node->m_pPrev = NULL;
    node->m_pNext = NULL;
    node->m_pOwner = NULL;
    node->m_flags |= AT_NODE_FLAG_DETACHED;
}

// ── atSafeDLListSimple Implementation ──

/**
 * atSafeDLListSimple_Init
 * @ 0x82368EF8 | size: 0x80
 * 
 * Initializes an empty list. Sets up vtable and clears all pointers.
 */
void atSafeDLListSimple_Init(struct atSafeDLListSimple* list) {
    if (!list) return;
    
    // Note: vtable would be set by C++ constructor
    // For C implementation, caller must set m_pVTable
    
    list->m_pOwner = NULL;
    list->m_pHead = NULL;
    list->m_pTail = NULL;
    list->m_pParent = NULL;
    list->m_count = 0;
    list->m_flags = 0;
}

/**
 * atSafeDLListSimple_Destroy
 * @ 0x82369D68 | size: 0x60
 * 
 * Destructor - removes all nodes from the list.
 * Loops until the list is empty, removing nodes one by one.
 */
void atSafeDLListSimple_Destroy(struct atSafeDLListSimple* list) {
    if (!list) return;
    
    // Remove all nodes until list is empty
    while (list->m_pHead != NULL) {
        atSafeDLListSimple_RemoveNode(list, list->m_pHead);
    }
}

/**
 * atSafeDLListSimple_RemoveNode
 * @ 0x823735A8 | size: 0xA4
 * 
 * Removes a node from the list.
 * This is the core list manipulation function - handles all pointer updates.
 * 
 * Original assembly shows careful handling of:
 * - Head/tail pointer updates
 * - Prev/next link updates
 * - Count decrement
 * - Node flag marking (detached state)
 */
void atSafeDLListSimple_RemoveNode(struct atSafeDLListSimple* list, struct atDLNode* node) {
    if (!list || !node) return;
    if (!node->m_pOwner) return;  // Node not in a list
    
    // Get parent list from node's owner field
    struct atSafeDLListSimple* parentList = (struct atSafeDLListSimple*)((char*)node->m_pOwner + 4);
    
    if (parentList) {
        struct atDLNode* prev = node->m_pPrev;
        struct atDLNode* next = node->m_pNext;
        
        // Update head pointer if removing first node
        if (prev == NULL) {
            parentList->m_pHead = next;
            if (next) {
                next->m_pPrev = NULL;
            } else {
                // List is now empty
                parentList->m_pTail = NULL;
            }
        } else {
            // Update previous node's next pointer
            prev->m_pNext = next;
            if (next) {
                next->m_pPrev = prev;
            } else {
                // Removing tail node
                parentList->m_pTail = prev;
            }
        }
        
        // Clear node pointers
        node->m_pPrev = NULL;
        node->m_pNext = NULL;
        
        // Decrement count
        if (parentList->m_count > 0) {
            parentList->m_count--;
        }
    }
    
    // Mark node as detached
    uint8_t flags = node->m_flags;
    node->m_flags = flags | AT_NODE_FLAG_DETACHED;
    
    // Clear owner pointer
    node->m_pOwner = NULL;
}

void atSafeDLListSimple_AddTail(struct atSafeDLListSimple* list, struct atDLNode* node) {
    if (!list || !node) return;
    if (node->m_pOwner) return;  // Already in a list
    
    node->m_pPrev = list->m_pTail;
    node->m_pNext = NULL;
    node->m_pOwner = list;
    node->m_flags &= ~AT_NODE_FLAG_DETACHED;
    
    if (list->m_pTail) {
        list->m_pTail->m_pNext = node;
    } else {
        // List was empty
        list->m_pHead = node;
    }
    
    list->m_pTail = node;
    list->m_count++;
}

void atSafeDLListSimple_AddHead(struct atSafeDLListSimple* list, struct atDLNode* node) {
    if (!list || !node) return;
    if (node->m_pOwner) return;  // Already in a list
    
    node->m_pPrev = NULL;
    node->m_pNext = list->m_pHead;
    node->m_pOwner = list;
    node->m_flags &= ~AT_NODE_FLAG_DETACHED;
    
    if (list->m_pHead) {
        list->m_pHead->m_pPrev = node;
    } else {
        // List was empty
        list->m_pTail = node;
    }
    
    list->m_pHead = node;
    list->m_count++;
}

bool atSafeDLListSimple_IsEmpty(const struct atSafeDLListSimple* list) {
    return (list == NULL || list->m_pHead == NULL);
}

uint32_t atSafeDLListSimple_GetCount(const struct atSafeDLListSimple* list) {
    return list ? list->m_count : 0;
}

void atSafeDLListSimple_Clear(struct atSafeDLListSimple* list) {
    if (!list) return;
    
    while (list->m_pHead) {
        atSafeDLListSimple_RemoveNode(list, list->m_pHead);
    }
}

// ── atDLList Implementation ──

void atDLList_Init(struct atDLList* list) {
    if (!list) return;
    
    list->m_pHead = NULL;
    list->m_pTail = NULL;
    list->m_count = 0;
}

void atDLList_AddTail(struct atDLList* list, struct atDLNode* node) {
    if (!list || !node) return;
    
    node->m_pPrev = list->m_pTail;
    node->m_pNext = NULL;
    node->m_pOwner = list;
    
    if (list->m_pTail) {
        list->m_pTail->m_pNext = node;
    } else {
        list->m_pHead = node;
    }
    
    list->m_pTail = node;
    list->m_count++;
}

void atDLList_RemoveNode(struct atDLList* list, struct atDLNode* node) {
    if (!list || !node) return;
    
    if (node->m_pPrev) {
        node->m_pPrev->m_pNext = node->m_pNext;
    } else {
        list->m_pHead = node->m_pNext;
    }
    
    if (node->m_pNext) {
        node->m_pNext->m_pPrev = node->m_pPrev;
    } else {
        list->m_pTail = node->m_pPrev;
    }
    
    node->m_pPrev = NULL;
    node->m_pNext = NULL;
    node->m_pOwner = NULL;
    
    if (list->m_count > 0) {
        list->m_count--;
    }
}

bool atDLList_IsEmpty(const struct atDLList* list) {
    return (list == NULL || list->m_pHead == NULL);
}
