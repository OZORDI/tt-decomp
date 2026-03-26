/**
 * Xbox 360 Static Initialization System - Implementation
 * Rockstar Presents Table Tennis
 * 
 * Implements the XAM static initialization pattern using a global singleton
 * and linked list of initialization nodes.
 */

#include "xam/static_init.h"
#include <stdlib.h>
#include <string.h>

//=============================================================================
// Global State
//=============================================================================

// Global initialization flag (lbl_8271B2A0 @ 0x8271B2A0)
static uint32_t g_xam_init_flag = 0;

// Global singleton instance (lbl_8271A860 @ 0x8271A860, size: 0x18)
static xam_init_singleton g_xam_singleton = {0};

// Static node for xam_static_init_1770 (lbl_825D1698 @ 0x825D1698, size: 0xC)
static xam_init_node g_xam_node_1770 = {0};

//=============================================================================
// Cleanup Handler
//=============================================================================

/**
 * Cleanup handler registered with atexit.
 * Called when the program exits to clean up the singleton.
 */
static void xam_singleton_cleanup(void) {
    // Reset the singleton state
    memset(&g_xam_singleton, 0, sizeof(xam_init_singleton));
    g_xam_init_flag = 0;
}

//=============================================================================
// Singleton Initialization
//=============================================================================

/**
 * xam_GetInitSingleton @ 0x82238D60 | size: 0x94
 * 
 * Original assembly:
 *   - Checks global flag (lbl_8271B2A0) bit 0
 *   - If not initialized:
 *     - Sets flag bit 0
 *     - Zeros out singleton structure (lbl_8271A860)
 *     - Registers atexit handler
 *   - Returns pointer to singleton
 */
xam_init_singleton* xam_GetInitSingleton(void) {
    // Check if already initialized (bit 0 of flag)
    if ((g_xam_init_flag & 0x1) == 0) {
        // Set initialized flag
        g_xam_init_flag |= 0x1;
        
        // Zero out the singleton structure
        memset(&g_xam_singleton, 0, sizeof(xam_init_singleton));
        
        // Register cleanup handler
        atexit(xam_singleton_cleanup);
    }
    
    return &g_xam_singleton;
}

//=============================================================================
// Static Initialization Functions
//=============================================================================

/**
 * xam_static_init_1770 @ 0x82581770 | size: 0x40
 * 
 * Original assembly:
 *   1. Call xam_GetInitSingleton to get singleton pointer
 *   2. Load current head pointer from singleton+8
 *   3. Store it in node+8 (link to previous head)
 *   4. Store node address in singleton+8 (make node new head)
 *   5. Increment count at singleton+12
 * 
 * This implements a classic linked list insertion at head pattern.
 */
void xam_static_init_1770(void) {
    // Get the global singleton
    xam_init_singleton* singleton = xam_GetInitSingleton();
    
    // Load current head pointer
    xam_init_node* old_head = singleton->head;
    
    // Link our node to the old head
    g_xam_node_1770.next = old_head;
    
    // Make our node the new head
    singleton->head = &g_xam_node_1770;
    
    // Increment the node count
    singleton->count++;
}

// Static nodes for batch of 10 initialization functions
static xam_init_node g_xam_node_F628 = {0};  // @ 0x825D0FF0
static xam_init_node g_xam_node_F6A0 = {0};  // @ 0x825D1008
static xam_init_node g_xam_node_F718 = {0};  // @ 0x825D1020
static xam_init_node g_xam_node_F790 = {0};  // @ 0x825D1038
static xam_init_node g_xam_node_F808 = {0};  // @ 0x825D1050
static xam_init_node g_xam_node_F880 = {0};  // @ 0x825D1068
static xam_init_node g_xam_node_F8F8 = {0};  // @ 0x825D1080
static xam_init_node g_xam_node_F970 = {0};  // @ 0x825D1098
static xam_init_node g_xam_node_F9E8 = {0};  // @ 0x825D10B0
static xam_init_node g_xam_node_0FF0 = {0};  // @ 0x825D1518

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F628 @ 0x8257F628 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F628(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F628.next = old_head;
    singleton->head = &g_xam_node_F628;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F6A0 @ 0x8257F6A0 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F6A0(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F6A0.next = old_head;
    singleton->head = &g_xam_node_F6A0;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F718 @ 0x8257F718 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F718(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F718.next = old_head;
    singleton->head = &g_xam_node_F718;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F790 @ 0x8257F790 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F790(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F790.next = old_head;
    singleton->head = &g_xam_node_F790;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F808 @ 0x8257F808 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F808(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F808.next = old_head;
    singleton->head = &g_xam_node_F808;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F880 @ 0x8257F880 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F880(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F880.next = old_head;
    singleton->head = &g_xam_node_F880;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F8F8 @ 0x8257F8F8 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F8F8(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F8F8.next = old_head;
    singleton->head = &g_xam_node_F8F8;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F970 @ 0x8257F970 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F970(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F970.next = old_head;
    singleton->head = &g_xam_node_F970;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_F9E8 @ 0x8257F9E8 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_F9E8(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_F9E8.next = old_head;
    singleton->head = &g_xam_node_F9E8;
    singleton->count++;
}

// ─────────────────────────────────────────────────────────────────────────────
// xam_static_init_0FF0 @ 0x82580FF0 | size: 0x40
// Inserts static node into global initialization linked list
// ─────────────────────────────────────────────────────────────────────────────
void xam_static_init_0FF0(void) {
    xam_init_singleton* singleton = xam_GetInitSingleton();
    xam_init_node* old_head = singleton->head;
    g_xam_node_0FF0.next = old_head;
    singleton->head = &g_xam_node_0FF0;
    singleton->count++;
}
