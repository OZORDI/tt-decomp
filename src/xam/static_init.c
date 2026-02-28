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
 * xam_singleton_init_8D60 @ 0x82238D60 | size: 0x94
 * 
 * Original assembly:
 *   - Checks global flag (lbl_8271B2A0) bit 0
 *   - If not initialized:
 *     - Sets flag bit 0
 *     - Zeros out singleton structure (lbl_8271A860)
 *     - Registers atexit handler
 *   - Returns pointer to singleton
 */
xam_init_singleton* xam_singleton_init_8D60(void) {
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
 *   1. Call xam_singleton_init_8D60 to get singleton pointer
 *   2. Load current head pointer from singleton+8
 *   3. Store it in node+8 (link to previous head)
 *   4. Store node address in singleton+8 (make node new head)
 *   5. Increment count at singleton+12
 * 
 * This implements a classic linked list insertion at head pattern.
 */
void xam_static_init_1770(void) {
    // Get the global singleton
    xam_init_singleton* singleton = xam_singleton_init_8D60();
    
    // Load current head pointer
    xam_init_node* old_head = singleton->head;
    
    // Link our node to the old head
    g_xam_node_1770.next = old_head;
    
    // Make our node the new head
    singleton->head = &g_xam_node_1770;
    
    // Increment the node count
    singleton->count++;
}
