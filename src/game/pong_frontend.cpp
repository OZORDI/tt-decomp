/**
 * pong_frontend.cpp — Frontend Menu State Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements the frontend menu system using RAGE's hierarchical state machine.
 * Handles player selection, character loading, and menu navigation.
 * 
 * Key Components:
 * - pongFrontendContext: HSM context managing menu state and UI pages
 * - pongFrontendState: HSM state handling frontend logic and character loading
 * 
 * Architecture Notes:
 * - Uses event queue system for state transitions (@ 0x825F0000 base)
 * - Manages 5 page groups for UI hierarchy
 * - Polls character loading status with 15-frame intervals
 * - Supports 2-player character selection
 * 
 * Decompilation Quality:
 * This implementation has been refined from static recomp output to use:
 * - Named constants instead of magic numbers
 * - Structured control flow instead of goto statements
 * - Meaningful variable names instead of register names
 * - Proper type definitions for pointer arithmetic
 * - Comprehensive documentation of complex offset calculations
 */

#include "game/pong_frontend.hpp"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External Globals
// ─────────────────────────────────────────────────────────────────────────────

// Main game object singleton
// Contains player data arrays, character views, and game state
extern void* g_game_obj_ptr;  // @ 0x82606604

// ── Event Queue System (@ 0x825F0000 base) ──
// 
// The event queue is a circular buffer that stores game events for processing.
// Each event has three components stored in parallel arrays:
//   1. Type byte (g_eventQueueParams) - identifies event category
//   2. Data pointer (g_eventQueueData) - context-specific data
//   3. Event ID (g_eventQueueTypes) - specific event identifier
//
// The queue index (g_eventQueueIndex) tracks the next write position.
// Events are processed by the game loop and consumed by state machines.

extern uint32_t g_eventQueueIndex;      // @ 0x825F3864 - current event queue write index
extern void*    g_eventQueueData[];     // @ 0x825F3868 - event data pointers array
extern uint32_t g_eventQueueTypes[];    // @ 0x825F3968 - event type IDs array
extern uint8_t  g_eventQueueParams[];   // @ 0x825F3A68 - event parameter bytes array

// Frontend page group global
// Points to the currently active UI page group (menu hierarchy root)
extern void* g_currentPageGroup;        // @ 0x8260662C - current active page group

// ─────────────────────────────────────────────────────────────────────────────
// External Function Declarations
// ─────────────────────────────────────────────────────────────────────────────

extern "C" {
    // Character view management
    void CharView_SetPlayerIndex(void* charView, int playerIdx);     // @ 0x8242C3B8 - set character view player
    void CharView_Activate(void* charView, int mode, int flag);      // @ 0x82566DC0 - activate character view
    void CharView_Deactivate(void* charView);                        // @ 0x82566ED8 - deactivate character view
    
    // Player data management
    void Player_DeactivateForLoad(void* gameObj, int playerIdx);     // @ 0x8218A340 - deactivate player for loading
    void Player_ResetState(void* gameObj, int playerIdx);            // @ 0x8218A5E0 - reset player state
    
    // Debug logging (no-op in release builds)
    void DebugLog(const char* msg, int param);                       // @ 0x8240E6D0 - debug log (no-op)
    
    // Frame timing
    void WaitFrames(int frames);                                     // @ 0x82566C80 - wait N frames
    
    // Game state management
    int32_t FindStateIndexByName(void* ctx, const char* stateName); // @ 0x821DDB70 - find state by name
    
    // Memory allocation
    void* rage_malloc(uint32_t size, uint32_t alignment);            // @ 0x820C0038 - RAGE allocator
    void* rage_GetAllocator();                                       // Get current thread allocator
    
    // HSM initialization
    void HSM_InitializeState();                                      // @ 0x8230D278 - HSM state init
    void xe_main_thread_init_0038();                                 // @ 0x820C0038 - thread init
}

// Debug string literals (preserved from binary)
static const char* STR_WAITING_FOR_LOAD = 
    "pongFrontendState::WaitForCharLoadToEnd() - waiting for player[%d] load to finish";
static const char* STR_NEED_DEACTIVATE = 
    "pongFrontendState::WaitForCharLoadToEnd() - need to deactivate player[%d]";

// ── Game Object Offsets ──
// 
// These offsets are used to access player and character data within the main game object.
// The game object is a large structure containing all game state, including:
//   - Player data arrays (2 players)
//   - Character view pointers (for rendering/animation)
//   - Character data pointers (stats, attributes, state)
//
// All offsets are in bytes from the game object base pointer.

namespace GameObjectOffsets {
    constexpr uint32_t PLAYER_DATA_BASE = 0x84;      // Base offset for player data array (132)
    constexpr uint32_t PLAYER_DATA_STRIDE = 0x04;    // Bytes between player entries (4)
    
    constexpr uint32_t CHAR_VIEW_OFFSET = 0xE0;      // Character view pointer (224 = 56*4)
    constexpr uint32_t CHAR_VIEW_CLEANUP = 0xC8;     // Character view cleanup pointer (200 = 50*4)
    constexpr uint32_t CHAR_DATA_OFFSET = 0x74;      // Character data pointer (116 = 29*4)
}

// ── Player Data Offsets ──
// 
// These offsets are relative to a player data structure pointer.
// Each player has a data structure containing:
//   - Character selection and loading state
//   - Active/inactive flags
//   - Character ID and attributes
//
// All offsets are in bytes from the player data base pointer.

namespace PlayerDataOffsets {
    constexpr uint32_t LOAD_COMPLETE_FLAG = 0xE8;    // Character load complete flag (232)
    constexpr uint32_t DEACTIVATE_FLAG = 0xB4;       // Deactivation pending flag (180)
    constexpr uint32_t CHAR_ID = 0x28;               // Character ID field (40)
    constexpr uint32_t LOAD_STATE = 0x18;            // Load state field (24)
    constexpr uint32_t ACTIVE_FLAG = 0x20;           // Active flag (32)
}

// ── Event Queue Constants ──
// 
// Event types and IDs used by the event queue system.
// Frontend events use type 128 (0x80) as a category marker.

namespace EventQueue {
    constexpr uint8_t TYPE_FRONTEND = 128;           // Frontend event type marker
    constexpr uint32_t ID_FRONTEND_ENTER = 22;       // Frontend state enter event ID
}

// ── Frame Timing ──
// 
// Frame counts for polling and waiting operations.
// The game runs at 60 FPS, so 15 frames = 250ms.

constexpr int CHAR_LOAD_POLL_FRAMES = 15;           // Frames to wait between load status checks

// ─────────────────────────────────────────────────────────────────────────────
// pongFrontendContext Implementation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongFrontendContext::pongFrontendContext @ 0x82188280 | size: 0x6C
 * 
 * Constructor - initializes frontend context with default values.
 */
pongFrontendContext::pongFrontendContext(rage::hsmContext* pHSMContext) {
    // Initialize base class fields
    memset(_pad_base, 0, sizeof(_pad_base));
    
    // Set up vtables
    // Primary vtable @ 0x8205F7E4
    // Secondary vtable @ 0x8205F848 (also used as event data pointer)
    m_pSecondaryVTable = (void*)0x8205F848;
    
    // Initialize context data
    m_pHSMContext = pHSMContext;
    m_field_1C = 0;
    m_field_20 = 0;
    m_field_24 = 0;
    m_pPageGroup1 = nullptr;
    m_pPageGroup2 = nullptr;
    m_pPageGroup3 = nullptr;
    m_pPageGroup4 = nullptr;
    m_pPageGroup5 = nullptr;
    m_field_3C = 0;
    m_field_40 = 0;
    m_bInitialized = true;
    m_selectedCharacter = -1;
    m_field_48 = 0;
    m_field_49 = 0;
    m_field_4A = 0;
    m_field_4B = 0;
    
    // Initialize player data
    for (int i = 0; i < 2; i++) {
        m_playerData[i].m_bActive = false;
        memset(m_playerData[i]._pad, 0, sizeof(m_playerData[i]._pad));
    }
    
    // Initialize UI page info
    m_field_15C = 0;
    m_gameStateResult = 0;
    m_field_160 = 0;
    m_pageInfo.m_pVTable = (void*)0x8203A928;
    m_pageInfo.m_field_04 = 0;
    m_pageInfo.m_field_08 = 0;
    m_pageInfo.m_field_0C = 0;
    m_pageInfo.m_field_0E = 0;
    m_field_174 = 0;
    m_field_178 = 0;
    m_playerIndices[0] = -1;
    m_playerIndices[1] = -1;
}

/**
 * pongFrontendContext::~pongFrontendContext @ 0x8230D110 | size: 0x84
 * 
 * Destructor - cleans up frontend context resources.
 */
pongFrontendContext::~pongFrontendContext() {
    // Cleanup handled by base class and automatic destructors
}

/**
 * pongFrontendContext::IsPlayerCharacterReady @ 0x82188280 | size: 0x6C
 * 
 * Checks if a player's selected character is loaded and ready.
 * 
 * This function performs a complex series of pointer dereferences through
 * the game object structure to validate character loading status.
 * 
 * Validation checks:
 * 1. Context pointer is valid
 * 2. Player data pointer exists
 * 3. Character data pointer exists
 * 4. Character ID matches selection
 * 5. Load state is complete (>= 0)
 * 6. Character is marked as active
 * 
 * Note: The original assembly uses complex offset arithmetic with
 * multiplication and masking. This implementation preserves the exact
 * calculation pattern for ABI compatibility.
 * 
 * @param pContext The frontend context
 * @param playerIndex Player index (0 or 1)
 * @param characterIndex Character selection index
 * @return true if character is loaded and ready
 */
bool pongFrontendContext::IsPlayerCharacterReady(pongFrontendContext* pContext,
                                                  int playerIndex,
                                                  int characterIndex) {
    if (!pContext) {
        return false;
    }
    
    // The original assembly uses a complex indexing scheme:
    // 1. Calculate base index: playerIndex + 33
    // 2. Multiply by 4 and mask to get array offset
    // 3. Load pointer from context array
    // 4. Repeat for character data with different base (29)
    //
    // This appears to be accessing nested arrays within the context structure.
    // The exact layout is unclear without full struct definitions, but we
    // preserve the calculation pattern.
    
    // Access context as uint32_t array for pointer arithmetic
    uint32_t* contextArray = (uint32_t*)pContext;
    
    // Calculate player data index (base 33)
    int playerDataIndex = playerIndex + 33;
    int playerDataOffset = (playerDataIndex * 4) & 0xFFFFFFFC;
    
    // Load player data pointer
    uint32_t playerDataPtr = contextArray[playerDataOffset / 4];
    if (!playerDataPtr) {
        return false;
    }
    
    // Calculate character data index (base 29)
    int charDataIndex = playerDataPtr + 29;
    int charDataOffset = (charDataIndex * 4) & 0xFFFFFFFC;
    
    // Load character data pointer
    uint32_t charDataPtr = contextArray[charDataOffset / 4];
    if (!charDataPtr) {
        return false;
    }
    
    // Access character data structure
    uint32_t* charData = (uint32_t*)charDataPtr;
    
    // Check character ID match (offset +40)
    uint32_t charId = charData[PlayerDataOffsets::CHAR_ID / 4];
    if (charId != (uint32_t)characterIndex) {
        return false;
    }
    
    // Check load state (offset +24) - must be >= 0
    int32_t loadState = (int32_t)charData[PlayerDataOffsets::LOAD_STATE / 4];
    if (loadState < 0) {
        return false;
    }
    
    // Check active flag (offset +32) - must be non-zero
    uint8_t* charDataBytes = (uint8_t*)charDataPtr;
    uint8_t activeFlag = charDataBytes[PlayerDataOffsets::ACTIVE_FLAG];
    if (activeFlag == 0) {
        return false;
    }
    
    return true;
}

/**
 * pongFrontendContext::CanTransition @ 0x8230D198 | size: 0xDC
 * 
 * Checks if frontend can transition to next state.
 */
bool pongFrontendContext::CanTransition() {
    // TODO: Implement transition validation
    // Check player selection status
    // Check character loading status
    // Check UI state
    return true;
}

/**
 * pongFrontendContext::OnEnter @ 0x8230D958 | size: 0x114
 * 
 * Called when entering frontend state.
 * 
 * Sets up the event queue, initializes page groups, and prepares
 * the frontend for player selection.
 * 
 * Event Queue Entry:
 * - Type: 128 (0x80) - frontend event marker
 * - ID: 22 - frontend state enter event
 * - Data: pointer to secondary vtable (context + 0x14)
 */
void pongFrontendContext::OnEnter() {
    // Get current event queue index
    uint32_t eventIdx = g_eventQueueIndex;
    
    // Store event type (frontend event marker)
    g_eventQueueParams[eventIdx] = EventQueue::TYPE_FRONTEND;
    
    // Calculate array index for event data
    uint32_t arrayIdx = eventIdx * 4;  // Each entry is 4 bytes
    
    // Store event data pointer (secondary vtable / context+0x14)
    g_eventQueueData[arrayIdx / 4] = m_pSecondaryVTable;
    
    // Store event ID (frontend enter)
    g_eventQueueTypes[arrayIdx / 4] = EventQueue::ID_FRONTEND_ENTER;
    
    // Increment event queue index
    g_eventQueueIndex = eventIdx + 1;
    
    // Find and store the frontend state index by name
    // String parameter @ 0x8205F3AC (likely "frontend" or similar state name)
    const char* stateName = (const char*)0x8205F3AC;
    m_gameStateResult = FindStateIndexByName(this, stateName);
    
    // Initialize page groups
    // Page group 3 (context+48): set mode to 0
    if (m_pPageGroup3) {
        // Virtual call: slot 4 (byte offset +16)
        // This likely calls SetVisible(false) or similar
        void** vtable = *(void***)m_pPageGroup3;
        typedef void (*SetModeFn)(void*, int);
        SetModeFn setMode = (SetModeFn)vtable[4];
        setMode(m_pPageGroup3, 0);
    }
    
    // Page group 5 (context+56): set mode to 0
    if (m_pPageGroup5) {
        void** vtable = *(void***)m_pPageGroup5;
        typedef void (*SetModeFn)(void*, int);
        SetModeFn setMode = (SetModeFn)vtable[4];
        setMode(m_pPageGroup5, 0);
    }
    
    // Link page groups: page group 5 references page group 4
    if (m_pPageGroup5 && m_pPageGroup4) {
        // Store page group 4 pointer at offset +24 in page group 5
        uint32_t* pg5Data = (uint32_t*)m_pPageGroup5;
        pg5Data[24 / 4] = (uint32_t)(uintptr_t)m_pPageGroup4;
    }
    
    // Reset player selection indices
    m_playerIndices[0] = -1;
    m_playerIndices[1] = -1;
    
    // Store main page group in global
    g_currentPageGroup = m_pPageGroup5;
}

/**
 * pongFrontendContext::Update @ 0x8230DA70 | size: 0xC
 * 
 * Per-frame update for frontend.
 */
void pongFrontendContext::Update() {
    // TODO: Implement per-frame logic
    // Update UI
    // Poll input
    // Update character loading
}

/**
 * pongFrontendContext::OnPlayerSelected @ 0x8230EAE8 | size: 0x118
 * 
 * Called when player selects a character.
 */
void pongFrontendContext::OnPlayerSelected(int playerIndex) {
    // TODO: Implement player selection
    // Trigger character load
    // Update UI state
    // Notify other systems
}

/**
 * pongFrontendContext::OnExit @ 0x8230EC00 | size: 0x200
 * 
 * Called when exiting frontend state.
 * 
 * Cleans up page groups by calling their destructors and clearing pointers.
 * This follows a specific order to properly tear down the UI hierarchy.
 */
void pongFrontendContext::OnExit() {
    // Clean up page group 1
    if (m_pPageGroup1) {
        // Call virtual destructor (slot 0, byte offset +0)
        void** vtable = *(void***)m_pPageGroup1;
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = (DestructorFn)vtable[0];
        destructor(m_pPageGroup1, 1);  // 1 = free memory
    }
    m_pPageGroup1 = nullptr;
    
    // Clean up page group 2
    if (m_pPageGroup2) {
        void** vtable = *(void***)m_pPageGroup2;
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = (DestructorFn)vtable[0];
        destructor(m_pPageGroup2, 1);
    }
    m_pPageGroup2 = nullptr;
    
    // Clean up page group 4
    if (m_pPageGroup4) {
        void** vtable = *(void***)m_pPageGroup4;
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = (DestructorFn)vtable[0];
        destructor(m_pPageGroup4, 1);
    }
    m_pPageGroup4 = nullptr;
    
    // Clean up page group 3 (has additional virtual call before destruction)
    if (m_pPageGroup3) {
        // Call virtual method slot 5 (byte offset +20)
        void** vtable = *(void***)m_pPageGroup3;
        typedef void (*CleanupFn)(void*);
        CleanupFn cleanup = (CleanupFn)vtable[5];
        cleanup(m_pPageGroup3);
        
        // Then call destructor
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = (DestructorFn)vtable[0];
        destructor(m_pPageGroup3, 1);
    }
    m_pPageGroup3 = nullptr;
    
    // Clean up page group 5 (main page group, also has additional cleanup)
    if (m_pPageGroup5) {
        // Call virtual method slot 5 (byte offset +20)
        void** vtable = *(void***)m_pPageGroup5;
        typedef void (*CleanupFn)(void*);
        CleanupFn cleanup = (CleanupFn)vtable[5];
        cleanup(m_pPageGroup5);
        
        // Then call destructor
        typedef void (*DestructorFn)(void*, int);
        DestructorFn destructor = (DestructorFn)vtable[0];
        destructor(m_pPageGroup5, 1);
    }
    m_pPageGroup5 = nullptr;
}

/**
 * pongFrontendContext::DA80_Helper @ 0x8230DA80 | size: 0xB0
 * 
 * Helper function for frontend state management.
 */
void pongFrontendContext::DA80_Helper() {
    // TODO: Implement helper logic
}

// ─────────────────────────────────────────────────────────────────────────────
// pongFrontendState Implementation
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongFrontendState::pongFrontendState @ 0x8230D020 | size: 0xEC
 * 
 * Constructor - initializes frontend state with default values.
 * 
 * This constructor sets up:
 * - Vtable pointers (primary @ 0x8205F79C, secondary @ 0x8205F6E0)
 * - HSM context reference
 * - Player data arrays (2 players, 128 bytes each)
 * - UI page management structures
 * - Initial state flags
 */
pongFrontendState::pongFrontendState(rage::hsmContext* pHSMContext) {
    // Set up vtables
    m_pVTable = (void*)0x8205F79C;
    m_pVTable2 = (void*)0x8205F6E0;
    
    // Initialize HSM context
    m_pHSMContext = pHSMContext;
    
    // Initialize all fields to zero/default
    m_field_08 = 0;
    m_field_0C = 0;
    m_field_10 = 0;
    
    // Context will be allocated in Init()
    m_pContext = nullptr;
    
    m_field_1C = 0;
    m_field_20 = 0;
    m_field_24 = 0;
    m_field_28 = 0;
    m_field_2C = 0;
    m_field_30 = 0;
    m_field_34 = 0;
    m_field_38 = 0;
    m_field_3C = 0;
    m_field_40 = 0;
    m_bActive = true;
    m_selectedCharacter = -1;
    m_field_48 = 0;
    m_field_49 = 0;
    m_field_4A = 0;
    m_field_4B = 0;
    m_field_4C = 0;
    m_field_50 = 0;
    m_field_54 = 0;
    
    // Initialize player data arrays
    // Loop unrolled in assembly: 2 iterations, 128 bytes apart
    for (int i = 0; i < 2; i++) {
        m_playerData[i].m_bActive = false;
        memset(m_playerData[i]._pad, 0, sizeof(m_playerData[i]._pad));
    }
    
    // Initialize remaining fields
    m_field_15C = 0;
    m_field_160 = 0;
    m_field_164 = 0;
    m_field_168 = 0;
    m_field_16C = 0;
    m_field_170 = 0;
    m_field_174 = 0;
    m_field_178 = 0;
    
    // Initialize player indices to -1 (no selection)
    m_playerIndices[0] = -1;
    m_playerIndices[1] = -1;
}

/**
 * pongFrontendState::WaitForCharLoadToEnd @ 0x82310D00 | size: 0x1A4
 * 
 * Waits for all player character loads to complete.
 * 
 * This is a blocking wait loop that polls character loading status
 * and handles player deactivation when loads finish.
 * 
 * The function operates in two phases:
 * 1. Wait for character loading to complete
 * 2. Wait for character deactivation to complete
 * 
 * Debug output:
 * - "pongFrontendState::WaitForCharLoadToEnd() - waiting for player[%d] load to finish"
 * - "pongFrontendState::WaitForCharLoadToEnd() - need to deactivate player[%d]"
 */
void pongFrontendState::WaitForCharLoadToEnd() {
    // Access game object through global pointer
    // The game object contains player data arrays and character view pointers
    void* gameObj = g_game_obj_ptr;
    
    // ── Phase 1: Wait for character loading ──
    
    bool anyLoading = true;
    while (anyLoading) {
        anyLoading = false;
        
        // Check each player (0-1)
        for (int playerIdx = 0; playerIdx < 2; playerIdx++) {
            // Get player data pointer
            uint32_t playerDataOffset = GameObjectOffsets::PLAYER_DATA_BASE + 
                                       (playerIdx * GameObjectOffsets::PLAYER_DATA_STRIDE);
            void** playerDataPtr = (void**)((uint8_t*)gameObj + playerDataOffset);
            void* playerData = *playerDataPtr;
            
            if (!playerData) {
                continue;
            }
            
            // Get character view pointer
            void** charViewPtr = (void**)((uint8_t*)gameObj + GameObjectOffsets::CHAR_VIEW_OFFSET);
            void* charView = *charViewPtr;
            
            // Set character view player index
            CharView_SetPlayerIndex(charView, -1);
            
            // Refresh character view pointer (may have changed)
            charView = *charViewPtr;
            
            // Activate character view for loading check
            CharView_Activate(charView, 1, 0);
            
            // Check if character load is complete
            uint8_t* loadCompletePtr = (uint8_t*)playerData + PlayerDataOffsets::LOAD_COMPLETE_FLAG;
            uint8_t loadComplete = *loadCompletePtr;
            
            if (loadComplete != 0) {
                // Character is still loading
                anyLoading = true;
                
                // Deactivate player for loading
                Player_DeactivateForLoad(gameObj, playerIdx);
                
                // Log debug message
                DebugLog(STR_WAITING_FOR_LOAD, playerIdx);
            }
        }
        
        if (anyLoading) {
            // Wait before checking again
            WaitFrames(CHAR_LOAD_POLL_FRAMES);
        }
    }
    
    // ── Phase 2: Final cleanup - deactivate all character views ──
    
    for (int playerIdx = 0; playerIdx < 2; playerIdx++) {
        // Get character view pointer for cleanup
        void** charViewPtr = (void**)((uint8_t*)gameObj + GameObjectOffsets::CHAR_VIEW_CLEANUP);
        void* charView = *charViewPtr;
        
        // Deactivate character view
        CharView_Deactivate(charView);
    }
    
    // ── Phase 3: Wait for character data cleanup ──
    
    anyLoading = true;
    while (anyLoading) {
        anyLoading = false;
        
        for (int playerIdx = 0; playerIdx < 2; playerIdx++) {
            // Reset player state
            Player_ResetState(gameObj, playerIdx);
            
            // Check if player still has active character data
            uint32_t playerDataOffset = GameObjectOffsets::PLAYER_DATA_BASE + 
                                       (playerIdx * GameObjectOffsets::PLAYER_DATA_STRIDE);
            void** playerDataPtr = (void**)((uint8_t*)gameObj + playerDataOffset);
            void* playerData = *playerDataPtr;
            
            if (!playerData) {
                continue;
            }
            
            // Check character data pointer
            void** charDataPtr = (void**)((uint8_t*)gameObj + GameObjectOffsets::CHAR_DATA_OFFSET);
            void* charData = *charDataPtr;
            
            if (charData != nullptr) {
                // Character data still exists, needs deactivation
                anyLoading = true;
                
                // Log debug message
                DebugLog(STR_NEED_DEACTIVATE, playerIdx);
                
                // Set deactivation flag
                uint8_t* deactivatePtr = (uint8_t*)playerData + PlayerDataOffsets::DEACTIVATE_FLAG;
                *deactivatePtr = 1;
            }
        }
        
        if (anyLoading) {
            // Wait before checking again
            WaitFrames(CHAR_LOAD_POLL_FRAMES);
        }
    }
}

/**
 * pongFrontendState::OnEnter @ 0x82310750 | size: 0x3D0
 * 
 * Called when entering the frontend state.
 */
void pongFrontendState::OnEnter(int prevStateIdx) {
    // TODO: Implement state entry
    // Initialize UI
    // Load character data
    // Set up player selection
}

/**
 * pongFrontendState::OnExit @ 0x82310B20 | size: 0x1DC
 * 
 * Called when exiting the frontend state.
 */
void pongFrontendState::OnExit(int nextStateIdx) {
    // TODO: Implement state exit
    // Clean up character data
    // Unload UI resources
}

/**
 * pongFrontendState::GetContext @ 0x82310EA8 | size: 0xC
 * 
 * Returns the frontend context.
 */
void* pongFrontendState::GetContext() {
    return m_pContext;
}

/**
 * pongFrontendState::Init @ 0x823106E0 | size: 0x70
 * 
 * Initializes the frontend state.
 * 
 * Allocates the frontend context using RAGE's memory allocator
 * and calls its constructor.
 */
void pongFrontendState::Init() {
    // Initialize main thread (if needed)
    // This is likely a no-op or thread-local storage setup
    extern void xe_main_thread_init_0038();
    xe_main_thread_init_0038();
    
    // Get RAGE allocator from TLS (thread-local storage)
    // r13 is the TLS base register on PowerPC
    // Load allocator pointer from TLS+0, then offset by 4
    extern void* rage_GetAllocator();
    void* allocator = rage_GetAllocator();
    
    // Allocate memory for pongFrontendContext
    // Size: 388 bytes (0x184)
    // Alignment: 16 bytes
    void* contextMem = rage_malloc(388, 16);
    
    if (contextMem) {
        // Call pongFrontendContext constructor
        // Pass this state's HSM context as parameter
        m_pContext = new (contextMem) pongFrontendContext(m_pHSMContext);
    } else {
        m_pContext = nullptr;
    }
    
    // Store context pointer at offset +8
    // (This is redundant with m_pContext assignment above, but matches assembly)
    
    // Call additional HSM setup
    HSM_InitializeState();
}

