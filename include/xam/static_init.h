#pragma once

/**
 * Xbox 360 Static Initialization System
 * Rockstar Presents Table Tennis
 * 
 * Manages static initialization of XAM subsystems using a linked list pattern.
 * Each static initializer registers itself with a global singleton on first call.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Static Initialization Node Structure
//=============================================================================

/**
 * Node in the static initialization linked list.
 * Each XAM subsystem has a static node that gets registered on first use.
 */
typedef struct xam_init_node {
    uint32_t reserved[2];      // +0x00: Reserved fields
    struct xam_init_node* next; // +0x08: Next node in linked list
    uint32_t data;             // +0x0C: Node-specific data
} xam_init_node;

//=============================================================================
// Singleton Manager Structure
//=============================================================================

/**
 * Global singleton that manages the linked list of initialization nodes.
 * Initialized once on first access, then reused for all subsequent calls.
 */
typedef struct xam_init_singleton {
    uint32_t reserved[2];      // +0x00: Reserved fields
    xam_init_node* head;       // +0x08: Head of linked list
    uint32_t count;            // +0x0C: Number of registered nodes
    uint32_t reserved2[2];     // +0x10: Additional reserved fields
} xam_init_singleton;

//=============================================================================
// Singleton Initialization Function
//=============================================================================

/**
 * xam_singleton_init_8D60 @ 0x82238D60 | size: 0x94
 * 
 * Initializes the global XAM initialization singleton on first call.
 * Subsequent calls return the existing singleton pointer.
 * 
 * Uses a global flag to ensure initialization happens only once.
 * Registers an atexit handler for cleanup.
 * 
 * @return Pointer to the global xam_init_singleton
 */
xam_init_singleton* xam_singleton_init_8D60(void);

//=============================================================================
// Static Initialization Functions
//=============================================================================

/**
 * xam_static_init_1770 @ 0x82581770 | size: 0x40
 * 
 * Registers a static initialization node with the global singleton.
 * Links the node into the head of the linked list and increments the count.
 * 
 * Pattern used by XAM subsystems for lazy initialization.
 */
void xam_static_init_1770(void);

#ifdef __cplusplus
}
#endif
