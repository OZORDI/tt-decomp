/**
 * atList.h — RAGE Engine Doubly-Linked List Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Intrusive doubly-linked list implementation used throughout the RAGE engine.
 * These are template-like classes that provide type-safe list node management.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
struct atDLNode;
struct atSafeDLListSimple;
struct atDLList;

/**
 * atDLNode — Doubly-Linked List Node
 * @ 0x82073D3C (vtable)
 * 
 * Basic intrusive list node with prev/next pointers.
 * Nodes are embedded directly in the objects they link.
 */
struct atDLNode {
    void**          m_pVTable;      // +0x00 Virtual table
    struct atDLNode* m_pPrev;       // +0x04 Previous node in list
    struct atDLNode* m_pNext;       // +0x08 Next node in list
    void*           m_pOwner;       // +0x0C Owner/parent list
    uint8_t         m_flags;        // +0x10 Node state flags
    uint8_t         _pad[3];
};

/**
 * atSafeDLListSimple — Thread-Safe Doubly-Linked List
 * @ 0x82073D3C (vtable)
 * 
 * A doubly-linked list with head/tail tracking and element counting.
 * Used extensively for managing collections of game objects.
 * 
 * The "Safe" prefix suggests thread-safety or validation checks.
 */
struct atSafeDLListSimple {
    void**          m_pVTable;      // +0x00 Virtual table
    void*           m_pOwner;       // +0x04 Owner object (parent container)
    struct atDLNode* m_pHead;       // +0x08 First node in list
    struct atDLNode* m_pTail;       // +0x0C Last node in list
    struct atDLNode* m_pParent;     // +0x10 Parent list node (for nested lists)
    uint32_t        m_count;        // +0x14 Number of elements
    uint8_t         m_flags;        // +0x18 List state flags
    uint8_t         _pad[3];
};

/**
 * atDLList — Standard Doubly-Linked List
 * 
 * Non-thread-safe version for single-threaded contexts.
 * Simpler and faster than atSafeDLListSimple.
 */
struct atDLList {
    void**          m_pVTable;      // +0x00 Virtual table
    struct atDLNode* m_pHead;       // +0x04 First node
    struct atDLNode* m_pTail;       // +0x08 Last node
    uint32_t        m_count;        // +0x0C Element count
};

// ── atDLNode Methods ──

/**
 * Initialize a list node
 */
void atDLNode_Init(struct atDLNode* node);

/**
 * Check if node is linked to a list
 */
bool atDLNode_IsLinked(const struct atDLNode* node);

/**
 * Remove node from its list
 */
void atDLNode_Unlink(struct atDLNode* node);

// ── atSafeDLListSimple Methods ──

/**
 * atSafeDLListSimple_ctor_8EF8
 * @ 0x82368EF8 | size: 0x80
 * 
 * Constructor - initializes an empty list
 */
void atSafeDLListSimple_Init(struct atSafeDLListSimple* list);

/**
 * atSafeDLListSimple_9D68
 * @ 0x82369D68 | size: 0x60
 * 
 * Destructor - removes all nodes and cleans up
 */
void atSafeDLListSimple_Destroy(struct atSafeDLListSimple* list);

/**
 * atSafeDLListSimple_35A8_g
 * @ 0x823735A8 | size: 0xA4
 * 
 * Remove a node from the list.
 * Updates head/tail pointers and decrements count.
 * Marks the node as unlinked.
 */
void atSafeDLListSimple_RemoveNode(struct atSafeDLListSimple* list, struct atDLNode* node);

/**
 * Add a node to the end of the list
 */
void atSafeDLListSimple_AddTail(struct atSafeDLListSimple* list, struct atDLNode* node);

/**
 * Add a node to the beginning of the list
 */
void atSafeDLListSimple_AddHead(struct atSafeDLListSimple* list, struct atDLNode* node);

/**
 * Check if list is empty
 */
bool atSafeDLListSimple_IsEmpty(const struct atSafeDLListSimple* list);

/**
 * Get the number of elements in the list
 */
uint32_t atSafeDLListSimple_GetCount(const struct atSafeDLListSimple* list);

/**
 * Clear all nodes from the list
 */
void atSafeDLListSimple_Clear(struct atSafeDLListSimple* list);

// ── atDLList Methods ──

/**
 * Initialize a standard doubly-linked list
 */
void atDLList_Init(struct atDLList* list);

/**
 * Add node to tail
 */
void atDLList_AddTail(struct atDLList* list, struct atDLNode* node);

/**
 * Remove node
 */
void atDLList_RemoveNode(struct atDLList* list, struct atDLNode* node);

/**
 * Check if empty
 */
bool atDLList_IsEmpty(const struct atDLList* list);

#ifdef __cplusplus
}
#endif
