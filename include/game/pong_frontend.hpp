/**
 * pong_frontend.hpp — Frontend Menu State Machine Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements the hierarchical state machine for the frontend menu system.
 * Handles player selection, character loading, and menu navigation.
 * 
 * Key Classes:
 * - pongFrontendContext: HSM context managing menu state and UI pages
 * - pongFrontendState: HSM state handling frontend logic and character loading
 * - pongFrontendLive: Xbox Live integration for frontend (minimal)
 * 
 * Architecture:
 * The frontend uses RAGE's hierarchical state machine (HSM) pattern.
 * Context objects hold persistent data across state transitions.
 * State objects implement the actual behavior and transition logic.
 */

#pragma once

#include <stdint.h>

// Forward declarations
namespace rage {
    class hsmContext;
    class hsmState;
}

class pongPageGroup;  // UI page group container

// ─────────────────────────────────────────────────────────────────────────────
// pongFrontendContext @ vtable 0x8205F7E4 / 0x8205F848 (MI)
// 
// HSM context for the frontend menu system.
// Manages UI pages, player selection state, and character loading.
// 
// Multiple inheritance layout:
//   +0x00  primary vtable ptr (26 slots)
//   +0x04  primary base data
//   +0x14  secondary vtable ptr (hsmContext MI base @ 0x8205F848)
//   +0x18  context-specific data begins
// 
// Size: ~0x180 bytes
// ─────────────────────────────────────────────────────────────────────────────
class pongFrontendContext {
public:
    // ── Constructor / Destructor ──
    
    /**
     * Constructor @ 0x82188280 | size: 0x6C
     * 
     * Initializes the frontend context with default values.
     * Sets up player data arrays and UI state.
     * 
     * @param pHSMContext Parent HSM context for state transitions
     */
    pongFrontendContext(rage::hsmContext* pHSMContext);
    
    /**
     * Destructor @ 0x8230D110 | size: 0x84
     * 
     * Cleans up frontend context resources.
     */
    virtual ~pongFrontendContext();
    
    // ── Virtual Methods (HSM Context Interface) ──
    
    /**
     * CanTransition @ 0x8230D198 | size: 0xDC
     * 
     * Checks if the frontend can transition to another state.
     * Validates player selection and character loading status.
     * 
     * @return true if transition is allowed
     */
    virtual bool CanTransition();  // [11]
    
    /**
     * OnEnter @ 0x8230D958 | size: 0x114
     * 
     * Called when entering the frontend state.
     * Initializes UI pages and sets up player selection.
     */
    virtual void OnEnter();  // [12]
    
    /**
     * Update @ 0x8230DA70 | size: 0xC
     * 
     * Per-frame update for frontend logic.
     */
    virtual void Update();  // [14]
    
    /**
     * OnPlayerSelected @ 0x8230EAE8 | size: 0x118
     * 
     * Called when a player selects a character.
     * Triggers character loading and updates UI state.
     * 
     * @param playerIndex Which player made the selection (0 or 1)
     */
    virtual void OnPlayerSelected(int playerIndex);  // [17]
    
    /**
     * OnExit @ 0x8230EC00 | size: 0x200
     * 
     * Called when exiting the frontend state.
     * Cleans up UI pages and player data.
     */
    virtual void OnExit();  // [18]
    
    // ── Helper Methods ──
    
    /**
     * IsPlayerCharacterReady @ 0x82188280 | size: 0x6C
     * 
     * Checks if a specific player's character is loaded and ready.
     * 
     * Validates:
     * 1. Player index is valid (0-1)
     * 2. Character index is valid
     * 3. Character data pointer is non-null
     * 4. Character load state is >= 0 (loaded)
     * 5. Character is not marked as inactive
     * 
     * @param pContext The frontend context
     * @param playerIndex Player index (0 or 1)
     * @param characterIndex Character selection index
     * @return true if character is loaded and ready
     */
    static bool IsPlayerCharacterReady(pongFrontendContext* pContext, 
                                       int playerIndex, 
                                       int characterIndex);
    
    /**
     * DA80 @ 0x8230DA80 | size: 0xB0
     * 
     * Helper function for frontend state management.
     * (Purpose to be determined from further analysis)
     */
    void DA80_Helper();
    
private:
    // ── Data Members ──
    
    // Base class data (hsmContext)
    uint8_t         _pad_base[20];          // +0x04..+0x13  base class fields
    
    // Secondary vtable for multiple inheritance
    void*           m_pSecondaryVTable;     // +0x14  @ 0x8205F848 (event data pointer)
    
    // Context-specific data
    void*           m_pHSMContext;          // +0x18  parent HSM context
    uint32_t        m_field_1C;             // +0x1C
    uint32_t        m_field_20;             // +0x20
    uint32_t        m_field_24;             // +0x24
    void*           m_pPageGroup1;          // +0x28  page group pointer 1
    void*           m_pPageGroup2;          // +0x2C  page group pointer 2
    void*           m_pPageGroup3;          // +0x30  page group pointer 3
    void*           m_pPageGroup4;          // +0x34  page group pointer 4
    void*           m_pPageGroup5;          // +0x38  page group pointer 5 (main)
    uint32_t        m_field_3C;             // +0x3C
    uint8_t         m_field_40;             // +0x40
    uint8_t         m_bInitialized;         // +0x41  initialization complete flag
    uint32_t        m_selectedCharacter;    // +0x44  selected character index
    uint8_t         m_field_48;             // +0x48
    uint8_t         m_field_49;             // +0x49
    uint8_t         m_field_4A;             // +0x4A
    uint8_t         m_field_4B;             // +0x4B
    
    // Player data arrays (2 players)
    struct PlayerData {
        uint8_t     m_bActive;              // +0x00  player slot active
        uint8_t     _pad[127];              // +0x01..+0x7F  (128 bytes per player)
    };
    
    PlayerData      m_playerData[2];        // +0x56  (offset 86, 128 bytes each)
    
    uint8_t         m_field_15C;            // +0x15C (offset 348)
    uint32_t        m_gameStateResult;      // +0x158 (offset 344) result from game_DB70
    uint32_t        m_field_160;            // +0x160 (offset 352)
    
    // UI page management
    struct PageInfo {
        void*       m_pVTable;              // +0x00  vtable @ 0x8203A928
        uint32_t    m_field_04;             // +0x04
        uint32_t    m_field_08;             // +0x08
        uint16_t    m_field_0C;             // +0x0C
        uint16_t    m_field_0E;             // +0x0E
    };
    
    PageInfo        m_pageInfo;             // +0x164 (offset 356)
    uint32_t        m_field_174;            // +0x174 (offset 372)
    uint32_t        m_field_178;            // +0x178 (offset 376)
    int32_t         m_playerIndices[2];     // +0x17C (offset 380, 384) - player selection indices
};

// ─────────────────────────────────────────────────────────────────────────────
// pongFrontendState @ vtable 0x8205F79C
// 
// HSM leaf state that drives the frontend menu sequence.
// Handles character loading, player selection, and menu transitions.
// 
// Inherits from hsmState (or similar base).
// 
// Size: ~0xEC (236 bytes)
// ─────────────────────────────────────────────────────────────────────────────
class pongFrontendState {
public:
    // ── Constructor / Destructor ──
    
    /**
     * Constructor @ 0x8230D020 | size: 0xEC
     * 
     * Initializes the frontend state with default values.
     * Sets up vtable pointers and player data structures.
     * 
     * @param pHSMContext Parent HSM context for state management
     */
    pongFrontendState(rage::hsmContext* pHSMContext);
    
    // ── Virtual Methods (HSM State Interface) ──
    
    /**
     * OnEnter @ 0x82310750 | size: 0x3D0
     * 
     * Called when entering the frontend state.
     * Initializes UI, loads character data, and sets up player selection.
     * 
     * @param prevStateIdx Index of the previous state
     */
    virtual void OnEnter(int prevStateIdx);  // [11]
    
    /**
     * OnExit @ 0x82310B20 | size: 0x1DC
     * 
     * Called when exiting the frontend state.
     * Cleans up character data and UI resources.
     * 
     * @param nextStateIdx Index of the next state
     */
    virtual void OnExit(int nextStateIdx);  // [12]
    
    /**
     * GetContext @ 0x82310EA8 | size: 0xC
     * 
     * Returns the frontend context associated with this state.
     * 
     * @return Pointer to pongFrontendContext
     */
    virtual void* GetContext();  // [13]
    
    /**
     * Init @ 0x823106E0 | size: 0x70
     * 
     * Initializes the frontend state.
     * Allocates context and sets up initial state.
     */
    virtual void Init();  // [14]
    
    // ── Non-Virtual Methods ──
    
    /**
     * WaitForCharLoadToEnd @ 0x82310D00 | size: 0x1A4
     * 
     * Waits for all player character loads to complete.
     * Polls character loading status and handles deactivation.
     * 
     * This is a blocking wait loop that:
     * 1. Checks each player's character load status
     * 2. Deactivates players whose loads have finished
     * 3. Waits for all loads to complete before returning
     * 
     * Debug strings:
     * - "pongFrontendState::WaitForCharLoadToEnd() - waiting for player[%d] load to finish"
     * - "pongFrontendState::WaitForCharLoadToEnd() - need to deactivate player[%d]"
     */
    void WaitForCharLoadToEnd();
    
private:
    // ── Data Members ──
    
    // Vtable and base class data
    void*           m_pVTable;              // +0x00  @ 0x8205F79C
    void*           m_pHSMContext;          // +0x04  rage::hsmContext*
    uint32_t        m_field_08;             // +0x08
    uint32_t        m_field_0C;             // +0x0C
    uint32_t        m_field_10;             // +0x10
    
    // Secondary vtable for MI
    void*           m_pVTable2;             // +0x14  @ 0x8205F6E0
    
    // Context and state data
    pongFrontendContext* m_pContext;        // +0x18  frontend context
    uint32_t        m_field_1C;             // +0x1C
    uint32_t        m_field_20;             // +0x20
    uint32_t        m_field_24;             // +0x24
    uint32_t        m_field_28;             // +0x28
    uint32_t        m_field_2C;             // +0x2C
    uint32_t        m_field_30;             // +0x30
    uint32_t        m_field_34;             // +0x34
    uint32_t        m_field_38;             // +0x38
    uint32_t        m_field_3C;             // +0x3C
    uint8_t         m_field_40;             // +0x40
    uint8_t         m_bActive;              // +0x41
    int32_t         m_selectedCharacter;    // +0x44
    uint8_t         m_field_48;             // +0x48
    uint8_t         m_field_49;             // +0x49
    uint8_t         m_field_4A;             // +0x4A
    uint8_t         m_field_4B;             // +0x4B
    uint32_t        m_field_4C;             // +0x4C
    uint32_t        m_field_50;             // +0x50
    uint16_t        m_field_54;             // +0x54
    
    // Player data (2 players, 128 bytes each)
    struct PlayerLoadData {
        uint8_t     m_bActive;              // +0x00  player active flag
        uint8_t     _pad[127];              // +0x01..+0x7F
    };
    
    PlayerLoadData  m_playerData[2];        // +0x56 (offset 86)
    
    uint8_t         m_field_15C;            // +0x15C (offset 348)
    uint32_t        m_field_160;            // +0x160 (offset 352)
    uint32_t        m_field_164;            // +0x164 (offset 356)
    uint32_t        m_field_168;            // +0x168
    uint32_t        m_field_16C;            // +0x16C
    uint32_t        m_field_170;            // +0x170
    uint32_t        m_field_174;            // +0x174
    uint32_t        m_field_178;            // +0x178
    int32_t         m_playerIndices[2];     // +0x17C (offset 380, 384)
};

// ─────────────────────────────────────────────────────────────────────────────
// pongFrontendLive @ vtable 0x8205DEB4
// 
// Xbox Live integration for frontend menus.
// Handles online player profiles and matchmaking UI.
// 
// This is a minimal stub class for Xbox 360 Live functionality.
// Most methods are likely no-ops or stubs in the PC port.
// ─────────────────────────────────────────────────────────────────────────────
class pongFrontendLive {
public:
    void*   m_pVTable;  // +0x00  @ 0x8205DEB4
    
    // Additional fields to be determined from further analysis
    // This class has RTTI data at 0x822FBCA8 and 0x822FD3F0
};

// ─────────────────────────────────────────────────────────────────────────────
// Helper Functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * Helper shared by multiple frontend functions.
 * Sets page group visibility and updates context state.
 * 
 * @param ctx Frontend context
 * @param visible Visibility flag
 */
void pongFrontendState_9D68_h(pongFrontendContext* ctx, uint8_t visible);

