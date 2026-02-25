// pong_drill.cpp — Training drill implementations | Rockstar Presents Table Tennis (Xbox 360, 2006)

/**
 * Training Drill Classes Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "pong_drill.hpp"
#include <stdio.h>

// External function declarations
extern "C" {
    void SinglesNetworkClient_A818_g(void* client, int value);
    void pg_E480(int param1, int param2, const char* param3, int param4);
    void nop_8240E6D0(const char* message);
}

// External globals
extern void* g_singles_network_client;  // @ 0x82036614

/**
 * pongTrainingDrill::Init
 * @ 0x8210CDB8 | size: 0x30
 * 
 * Initializes/resets the training drill to default state.
 * Sets all counters to zero, loads default time limit, and resets target index.
 * 
 * Assembly details:
 *   - Loads float constant from 0x825C5938
 *   - Initializes 7 fields to known values
 *   - Sets target index to -1 (no target)
 */
void pongTrainingDrill::Init() {
    // Initialize counters and state
    m_numAttempts = 0;
    m_numSuccesses = 0;
    m_prevSuccesses = 0;
    m_numFailures = 0;
    
    // Load default time limit from data section @ 0x825C5938
    // Original: lis r10,-32164; lfs f0,22840(r10)
    m_timeRemaining = 0.0f;  // TODO: Load actual constant value
    
    m_completionFlag = 0;
    m_targetIndex = 0xFFFFFFFF;  // -1 = no target selected
}

/**
 * pongTrainingDrill::IncreaseNumSuccesses
 * @ 0x8210D400 | size: 0x84
 * 
 * Increases the success counter by the specified amount.
 * Clamps to maximum allowed successes from config.
 * Updates network state and UI.
 * 
 * Error message @ 0x82031970:
 *   "pongTrainingDrill::IncreaseNumSuccesses() - winning more than total possible, something is"
 * 
 * @param increment Number of successes to add
 */
void pongTrainingDrill::IncreaseNumSuccesses(int increment) {
    // Add increment to current successes
    m_numSuccesses += increment;
    
    // Get maximum allowed successes from config
    uint32_t maxSuccesses = m_pConfig ? *(uint32_t*)((uint8_t*)m_pConfig + 16) : 0xFFFFFFFF;
    
    // Clamp to maximum
    if (m_numSuccesses > maxSuccesses) {
        // Error: winning more than total possible
        nop_8240E6D0("pongTrainingDrill::IncreaseNumSuccesses() - winning more than total possible, something is");
        m_numSuccesses = maxSuccesses;
    }
    
    // Update network state
    // Original: lwz r3,26132(r11) where r11 = -32160 (lis)
    // Address: 0x82036614 (g_singles_network_client)
    if (g_singles_network_client) {
        SinglesNetworkClient_A818_g(g_singles_network_client, m_numSuccesses);
    }
    
    // Update UI
    // Original: li r3,20488; li r4,64; addi r5,r11,30872; li r6,1
    pg_E480(20488, 64, nullptr, 1);
}

/**
 * pongDrillMovement::Init
 * @ 0x8210DB48 | size: 0x54
 * 
 * Initializes the movement drill with extended state.
 * Calls base class Init() and sets up movement-specific fields.
 * 
 * Assembly details:
 *   - Loads config pointer from +56 (0x38)
 *   - Reads distance value from config +108
 *   - Initializes movement-specific fields at +36, +40, +44, +48, +52
 */
void pongDrillMovement::Init() {
    // Call base class initialization
    pongTrainingDrill::Init();
    
    // Movement-specific initialization
    m_movementFlag = 0;
    
    // Load target distance from config
    // Original: lwz r9,56(r3); lfs f13,108(r9)
    if (m_pConfig) {
        void* configExt = (void*)((uint8_t*)m_pConfig + 56);
        if (configExt) {
            m_targetDistance = *(float*)((uint8_t*)configExt + 108);
        }
    }
    
    m_movementState = MOVEMENT_STATE_0;
    m_moveTimer = 0.0f;  // Load from data section @ 0x82031970
    m_isComplete = 0;
}
