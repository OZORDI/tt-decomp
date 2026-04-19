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
#include "rage/types.h"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// Vtable Address Constants
// ─────────────────────────────────────────────────────────────────────────────

static void* const VTABLE_pongFrontendContext_Primary   = (void*)0x8205F7E4;  // vtable for pongFrontendContext (primary, 26 slots)
static void* const VTABLE_pongFrontendContext_Secondary = (void*)0x8205F848;  // vtable for pongFrontendContext (MI/virtual base, 3 slots)
static void* const VTABLE_pongFrontendState_Primary     = (void*)0x8205F79C;  // vtable for pongFrontendState
// NOTE: 0x8205F6E0 is NOT a vtable — it is the debug string
// "pongFrontendState::WaitForCharLoadToEnd() - waiting for player[%d] load to finish"
// stored in .rdata. The original binary stores this address at +0x14 in pongFrontendState,
// which may be a debug/RTTI pointer rather than a secondary vtable.
static void* const ADDR_pongFrontendState_DebugStr      = (void*)0x8205F6E0;  // debug string ptr, not a vtable
static void* const VTABLE_PageInfo                      = (void*)0x8203A928;  // vtable for rage::xmlTree

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
extern pongPageGroup* g_currentPageGroup;  // @ 0x8260662C - current active page group

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
    void sysMemAllocator_InitMainThread();                                 // @ 0x820C0038 - thread init
}

// ─────────────────────────────────────────────────────────────────────────────
// Page Group Vtable Dispatch Helpers
// ─────────────────────────────────────────────────────────────────────────────
// pongPageGroup is forward-declared only. These helpers encapsulate the
// repeated vtable dispatch patterns used on page group objects throughout
// the frontend code, improving readability without needing the full class.

/// Calls vtable slot 0 (destructor) with freeMemory flag.
static inline void PageGroup_Destroy(pongPageGroup* pg, int freeMemory) {
    typedef void (*Fn)(pongPageGroup*, int);
    ((Fn)VTABLE(pg)[0])(pg, freeMemory);
}

/// Calls vtable slot 4 (SetMode).
static inline void PageGroup_SetMode(pongPageGroup* pg, int mode) {
    typedef void (*Fn)(pongPageGroup*, int);
    ((Fn)VTABLE(pg)[4])(pg, mode);
}

/// Calls vtable slot 5 (Cleanup/Shutdown).
static inline void PageGroup_Cleanup(pongPageGroup* pg) {
    VCALL(pg, 5);
}

/// Sets the parent pointer at offset +24 in a page group.
static inline void PageGroup_SetParent(pongPageGroup* pg, pongPageGroup* parent) {
    *(pongPageGroup**)((uint8_t*)pg + 24) = parent;
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
    m_pSecondaryVTable = VTABLE_pongFrontendContext_Secondary;  // MI base vtable
    
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
    m_pageInfo.m_pVTable = VTABLE_PageInfo;  // vtable for rage::xmlTree
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
    uint8_t activeFlag = ((uint8_t*)charData)[PlayerDataOffsets::ACTIVE_FLAG];
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
    // Register this context with the event queue system
    void* pSecondaryVtable = (this != nullptr) ? m_pSecondaryVTable : nullptr;

    // Push onto event queue
    uint32_t queueIdx = g_eventQueueIndex;
    g_eventQueueParams[queueIdx] = EventQueue::TYPE_FRONTEND;
    g_eventQueueData[queueIdx] = pSecondaryVtable;
    g_eventQueueTypes[queueIdx] = EventQueue::ID_FRONTEND_ENTER;
    g_eventQueueIndex = queueIdx + 1;

    // Look up the frontend state by name
    extern struct gdGameData* g_gameDataMgr;  // @ 0x8271A314
    int32_t stateIdx = FindStateIndexByName(*(void**)&g_gameDataMgr, "FrontEnd");
    m_gameStateResult = (uint32_t)stateIdx;

    // Initialize page group 3: SetMode(0) via vtable slot 4
    if (m_pPageGroup3) {
        PageGroup_SetMode(m_pPageGroup3, 0);
    }

    // Initialize page group 5: SetMode(0) via vtable slot 4
    if (m_pPageGroup5) {
        PageGroup_SetMode(m_pPageGroup5, 0);
    }

    // Link page group 5's parent to page group 4
    if (m_pPageGroup5 && m_pPageGroup4) {
        PageGroup_SetParent(m_pPageGroup5, m_pPageGroup4);
    }

    // Store page group 5 in global renderer state
    g_currentPageGroup = m_pPageGroup5;

    // Initialize selected character indices to -1 (no selection)
    m_playerIndices[0] = -1;
    m_playerIndices[1] = -1;

    return true;
}

/**
 * pongFrontendContext::OnEnter @ 0x8230D198 | size: 0xDC (220 bytes)
 *
 * Vtable slot 11 on pongFrontendContext. Called when entering the
 * frontend state — posts a frontend-enter event (ID 22) to the event
 * queue, resolves the "Frontend" state index by name (string ref
 * @ 0x8205B45E inside this function), initializes page groups, and
 * stores the main page group in a global.
 *
 * NOTE: Previously doc-commented as @ 0x8230D958 (vfn_12). That is
 * wrong: the "Frontend" string xref originates in vfn_11 @ 0x8230D198
 * (220 B), and vfn_12 @ 0x8230D958 (276 B) is a cleanup routine that
 * nullifies fields +40/+44/+48/+52/+56 via vtable slot 5, holds no
 * string references, and is not implemented in this file.
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
    // String @ 0x8205F3AC: "Frontend"
    static const char* STR_STATE_TRANSITION = "Frontend";  // @ 0x8205F3AC
    m_gameStateResult = FindStateIndexByName(this, STR_STATE_TRANSITION);
    
    // Initialize page groups
    if (m_pPageGroup3) {
        PageGroup_SetMode(m_pPageGroup3, 0);
    }

    if (m_pPageGroup5) {
        PageGroup_SetMode(m_pPageGroup5, 0);
    }

    // Link page groups: page group 5 references page group 4
    if (m_pPageGroup5 && m_pPageGroup4) {
        PageGroup_SetParent(m_pPageGroup5, m_pPageGroup4);
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
 * Sets the frame counter/timer to 30. Resets the idle timeout
 * for the frontend menu each frame.
 */
void pongFrontendContext::Update() {
    m_field_3C = 30;
}

/**
 * pongFrontendContext::OnPlayerSelected @ 0x8230EAE8 | size: 0x118
 *
 * Called when a player selects a character. Checks if the game is in
 * online/multiplayer mode (via HSM manager flag). If not online, iterates
 * through player slots checking if the character is loaded and ready.
 * For each ready player whose selection flag is set, triggers character
 * activation via vtable slot 6 on the player's character data.
 */
void pongFrontendContext::OnPlayerSelected(int playerIndex) {
    // Check if game is in online mode
    // g_loop_obj_ptr @ 0x825EAB30 contains the main loop object
    extern void* g_loop_obj_ptr;
    uint8_t* hsmMgr = (uint8_t*)g_loop_obj_ptr;
    bool isOnline = (hsmMgr[495] != 0);

    if (!isOnline) {
        // Check secondary online flag @ 0x8271A374
        extern void* g_networkStatePtr;
        uint8_t* netState = (uint8_t*)g_networkStatePtr;
        if (netState[24] == 0) {
            return;  // Not ready
        }
    }

    // Iterate through 2 player slots
    uint32_t* pGameObj = *(uint32_t**)&g_game_obj_ptr;

    for (int slot = 0; slot < 2; slot++) {
        // Get player's character data index from game object
        uint32_t charDataIdx = pGameObj[(GameObjectOffsets::PLAYER_DATA_BASE / 4) + slot];

        // Look up character entry in player table
        uint32_t entryOffset = (charDataIdx + 29) * 4;
        void* charEntry = (void*)pGameObj[entryOffset / 4];

        if (!charEntry) continue;

        // Verify character matches and is loaded
        // Check character ID at +48 of computed entry
        uint32_t stride = charDataIdx * 3;
        uint32_t tableOffset = stride * 16;
        uint32_t* entryData = (uint32_t*)((uint8_t*)pGameObj + tableOffset);

        void* charData = (void*)entryData[64 / 4];
        if (charData != charEntry) continue;

        int32_t loadState = (int32_t)entryData[48 / 4];
        if (loadState < 0) continue;

        uint8_t activeFlag = ((uint8_t*)entryData)[56];
        if (activeFlag == 0) continue;

        // Check player selection flag in playerData array
        if (m_playerData[slot].m_bActive == 0) continue;

        // Trigger character activation (vtable slot 6 = Activate)
        // Re-fetch character entry for the activation call
        uint32_t charDataIdx2 = pGameObj[(GameObjectOffsets::PLAYER_DATA_BASE / 4) + slot];
        uint32_t entryOffset2 = (charDataIdx2 + 29) * 4;
        void* activateTarget = (void*)pGameObj[entryOffset2 / 4];

        typedef void (*ActivateFn)(void*, int, int);
        ((ActivateFn)VTABLE(activateTarget)[6])(activateTarget, 1, 1);
    }
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
    // Clean up simple page groups (destroy only)
    if (m_pPageGroup1) {
        PageGroup_Destroy(m_pPageGroup1, 1);
    }
    m_pPageGroup1 = nullptr;

    if (m_pPageGroup2) {
        PageGroup_Destroy(m_pPageGroup2, 1);
    }
    m_pPageGroup2 = nullptr;

    if (m_pPageGroup4) {
        PageGroup_Destroy(m_pPageGroup4, 1);
    }
    m_pPageGroup4 = nullptr;

    // Page group 3: cleanup (slot 5) then destroy
    if (m_pPageGroup3) {
        PageGroup_Cleanup(m_pPageGroup3);
        PageGroup_Destroy(m_pPageGroup3, 1);
    }
    m_pPageGroup3 = nullptr;

    // Page group 5 (main): cleanup (slot 5) then destroy
    if (m_pPageGroup5) {
        PageGroup_Cleanup(m_pPageGroup5);
        PageGroup_Destroy(m_pPageGroup5, 1);
    }
    m_pPageGroup5 = nullptr;
}

/**
 * pongFrontendContext::DA80_Helper @ 0x8230DA80 | size: 0xB0
 *
 * Checks if any player character is ready in the game data manager.
 * If the game object pointer is null, logs an error and falls through
 * to the character count check. Otherwise, looks up the player's
 * resolved state index and checks it against the character count.
 *
 * @return true if a character is available (index < characterCount)
 */
bool pongFrontendContext::DA80_Helper() {
    uint32_t* pGameObj = *(uint32_t**)&g_game_obj_ptr;

    // Check if a player character is ready (calls pongFrontendContext_8280_g)
    extern "C" bool CheckPlayerCharReady(void* gameObj, void* ctx, int flag);
    bool isReady = CheckPlayerCharReady(pGameObj, this, 1);

    if (!isReady) {
        return false;
    }

    if (pGameObj == nullptr) {
        // Log error — game object not available
        DebugLog("pongFrontendContext::DA80_Helper() - game object is null", 0);
        DebugLog("pongFrontendContext::DA80_Helper() - game object is null", 0);

        // Fall through to character count check
        extern void* g_gameDataMgrPtr;  // @ 0x8271A2E4
        uint32_t* dataMgr = (uint32_t*)g_gameDataMgrPtr;
        int32_t charCount = (int32_t)dataMgr[28 / 4];  // offset +28 = character count
        return (0 < charCount);
    }

    // Get resolved index from player state
    extern "C" int32_t GetPlayerStateIndex(void* ctx);
    int32_t stateIdx = GetPlayerStateIndex(this);

    if (stateIdx < 0) {
        return false;
    }

    // Check against character count
    extern void* g_gameDataMgrPtr;
    uint32_t* dataMgr = (uint32_t*)g_gameDataMgrPtr;
    int32_t charCount = (int32_t)dataMgr[28 / 4];  // offset +28 = character count
    return (stateIdx < charCount);
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
 * - Vtable pointer (primary @ 0x8205F79C = pongFrontendState)
 * - Debug string pointer (@ 0x8205F6E0, not a vtable)
 * - HSM context reference
 * - Player data arrays (2 players, 128 bytes each)
 * - UI page management structures
 * - Initial state flags
 */
pongFrontendState::pongFrontendState(rage::hsmContext* pHSMContext) {
    // Set up vtables
    m_pVTable = VTABLE_pongFrontendState_Primary;   // vtable for pongFrontendState
    m_pVTable2 = ADDR_pongFrontendState_DebugStr;  // debug string ptr, not a vtable
    
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
    // Open: 976-byte body @ 0x82310750 initialises the front-end UI,
    // kicks off character-data loading, and seeds player-selection state.
    // Deferred until hudFrontEnd::vfn_2 @ 0x823240A0 (1604 bytes) is
    // lifted — the two share field +0x5C (m_pPageGroup) and +0x74
    // (m_pFlashCtx) plus the character-data streamer invoked from
    // WaitForCharsLoaded above.
    (void)prevStateIdx;
}

/**
 * pongFrontendState::OnExit @ 0x82310B20 | size: 0x1DC
 * 
 * Called when exiting the frontend state.
 */
void pongFrontendState::OnExit(int nextStateIdx) {
    // Open: 476-byte body @ 0x82310B20 tears the front-end screen down
    // in the inverse order of OnEnter: releases character-data handles
    // and unbinds UI page groups. Deferred on the same dependency
    // (hudFrontEnd::vfn_2) as OnEnter above.
    (void)nextStateIdx;
}

/**
 * pongFrontendState::GetContext @ 0x82310EA8 | size: 0xC
 * 
 * Returns the frontend context.
 */
pongFrontendContext* pongFrontendState::GetContext() {
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
    extern void sysMemAllocator_InitMainThread();
    sysMemAllocator_InitMainThread();
    
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


// ─────────────────────────────────────────────────────────────────────────────
// Match Setup Display Functions
// ─────────────────────────────────────────────────────────────────────────────

// Open: pg_CBAC_sp @ 0x821BCBAC is a tiny 36-byte helper that has not yet
// been lifted. Historically the ~100-line body below was attributed to
// this label, but the size mismatch (36 bytes vs ~100 lines of C) and the
// string-xref trail ("%s:%s_VS_%s", "%s:SINGLEGAME_%dPOINTS") both confirm
// the real owner is pg_2DA8_g @ 0x821B2DA8 (512 bytes), lifted below.
// The 36-byte helper is almost certainly a thin forwarding thunk into
// pg_2DA8_g; pending dedicated lift.

/**
 * pg_2DA8_g @ 0x821B2DA8 | size: 0x200 (512 bytes)
 *
 * Formats and displays game mode configuration string for match setup screen.
 * Generates localized strings like "POINTS_5MINUTES", "BESTOF3_11POINTS", etc.
 *
 * String refs confirmed from the binary to live inside this function:
 *   0x8203B5BC  "%s:%s_VS_%s"
 *   0x8203B5C8  "%s:SINGLEGAME_%dPOINTS"
 *   (plus adjacent format strings for BESTOF / MINUTES variants)
 *
 * NOTE: Previously mislabeled as pg_CBAC_sp @ 0x821BCBAC. That function
 * is only 36 bytes in the binary and cannot hold this body; see the
 * open-item note above for the correct 36-byte function still to be lifted.
 *
 * Flow:
 * 1. Get UI display context
 * 2. Get player 0 and player 1 controller objects
 * 3. Randomly select character name variant (50% chance if players differ)
 * 4. Format and display character names with separator
 * 5. Format and display game mode string based on match settings
 *
 * Match settings from g_input_obj_ptr:
 *   +12:  bool isTimedMatch
 *   +16:  float timeLimit (seconds)
 *   +300: uint32_t matchType (0=5pts, 1=7pts, 2=11pts, other=21pts)
 *   +304: uint32_t bestOfMode (0=standard, 1=best-of-3, 2=best-of-5)
 *
 * Random number generation:
 *   Uses linear congruential generator with multiplier 0x5CDCFAA75CDCFAA7
 *   Extracts 23 bits and converts to float [0.0, 1.0) for 50/50 decision
 */
void pg_2DA8_g_cxx() {
    char buffer[176];
    
    // Get UI display context (returns handle for text display)
    extern void* sub_82439698();
    void* displayCtx = sub_82439698();
    
    // Get player controller objects
    extern void* g_input_obj_ptr;
    extern void* sub_821D3A00(void* inputSys, int playerIdx);
    
    void* player0Ctrl = sub_821D3A00(g_input_obj_ptr, 0);
    void* player1Ctrl = sub_821D3A00(g_input_obj_ptr, 1);
    
    // Decide which character name field to use (random 50/50 if players differ)
    bool useAlternateName = false;
    
    if (player0Ctrl != player1Ctrl) {
        // Linear congruential generator for random selection
        extern uint64_t g_randomSeed;  // @ 0x825CA2C8
        
        uint32_t seedHigh = (uint32_t)(g_randomSeed >> 32);
        uint64_t product = 0x5CDCFAA75CDCFAA7ULL * seedHigh;
        uint32_t newSeed = (uint32_t)(product >> 32);
        
        // Update both halves of the 64-bit seed
        g_randomSeed = ((uint64_t)newSeed << 32) | newSeed;
        
        // Convert to float [0.0, 1.0) and check if >= 0.5
        uint32_t bits = newSeed & 0x7FFFFF;
        float randValue = (float)bits * 1.1920929e-07f;
        useAlternateName = (randValue >= 0.5f);
    }
    
    // Get character name IDs from player controllers
    // Controller +52 -> character data
    // Character data +16 or +20 -> name ID (depends on random choice)
    int nameFieldOffset = useAlternateName ? 20 : 16;
    
    uint8_t* ctrl0 = (uint8_t*)player0Ctrl;
    uint8_t* ctrl1 = (uint8_t*)player1Ctrl;
    void* char0Data = *(void**)(ctrl0 + 52);
    void* char1Data = *(void**)(ctrl1 + 52);

    uint32_t char0NameId = *(uint32_t*)((uint8_t*)char0Data + nameFieldOffset);
    uint32_t char1NameId = *(uint32_t*)((uint8_t*)char1Data + nameFieldOffset);
    
    // Format character names with separator
    extern int sub_820CA940(char* buf, size_t size, const char* fmt, ...);
    extern uint32_t g_stringTableBase;  // @ 0x82606410

    const char* strTableBase = (const char*)(g_stringTableBase + 704);
    
    // First string format (character names)
    // Format string @ 0x8203B5BC: "%s:%s_VS_%s"
    static const char* STR_VS_FORMAT = "%s:%s_VS_%s";  // @ 0x8203B5BC
    sub_820CA940(buffer, 128, STR_VS_FORMAT, strTableBase, char0NameId, char1NameId);
    
    // Display character names
    extern void sub_821BD8E0(void* ctx);
    sub_821BD8E0(displayCtx);
    
    // Get match configuration
    uint8_t* inputObj = (uint8_t*)g_input_obj_ptr;
    uint32_t matchType = *(uint32_t*)(inputObj + 300);
    uint32_t bestOfMode = *(uint32_t*)(inputObj + 304);
    
    // Determine points to win
    int pointsToWin;
    if (matchType == 0) {
        pointsToWin = 5;
    } else if (matchType == 1) {
        pointsToWin = 7;
    } else if (matchType == 2) {
        pointsToWin = 11;
    } else {
        pointsToWin = 21;
    }
    
    // Format game mode string
    bool isTimedMatch = inputObj[12] != 0;

    if (isTimedMatch) {
        // Timed match: "POINTS_XMINUTES"
        float timeSeconds = *(float*)(inputObj + 16);
        int timeMinutes = (int)(timeSeconds * 0.016666668f);
        
        sub_820CA940(buffer, 128, "%s:%dPOINTS_%dMINUTES", 
                     strTableBase, pointsToWin, timeMinutes);
    } else {
        if (bestOfMode == 0) {
            // Standard match: just points
            static const char* STR_SCORE_SEPARATOR = "%s:SINGLEGAME_%dPOINTS";  // @ 0x8203B5C8
            sub_820CA940(buffer, 128, STR_SCORE_SEPARATOR, strTableBase, pointsToWin);
            sub_821BD8E0(displayCtx);
            return;
        }
        
        // Best-of match: "BESTOFX_YPOINTS"
        int bestOfCount = (bestOfMode == 1) ? 3 : 5;
        sub_820CA940(buffer, 128, "%s:BESTOF%d_%dPOINTS", 
                     strTableBase, bestOfCount, pointsToWin);
    }
    
    // Display game mode string
    sub_821BD8E0(displayCtx);
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalMoviePlayer — Small Function Implementations (≤64B)
// ─────────────────────────────────────────────────────────────────────────────
//
// CCalMoviePlayer is the movie/video playback system for the game.
// It uses Xbox 360 kernel synchronization primitives (KEVENT objects)
// for thread-safe signaling between the decode thread and game thread.
//
// The class contains an array of 8 KEVENT objects starting at offset +84,
// each 16 bytes apart (stride 16). These events synchronize:
//   - Frame decode completion
//   - Buffer availability
//   - Playback state transitions
//
// Field layout (partial):
//   +0x00   : vtable pointer
//   +0x30   : buffer base pointer (offset 48)
//   +0x34   : total buffer count (offset 52)
//   +0x38   : read index (offset 56)
//   +0x3C   : write index (offset 60)
//   +0x40   : change callback function pointer (offset 64)
//   +0x44   : semaphore / ref count (offset 68)
//   +0x50   : callback context pointer (offset 80)
//   +0x54   : event object 0 (offset 84)  — KEVENT
//   +0x64   : event object 1 (offset 100)
//   +0x74   : event object 2 (offset 116)
//   +0x84   : event object 3 (offset 132)
//   +0x94   : event object 4 (offset 148)
//   +0xA4   : event object 5 (offset 164)
//   +0xB4   : event object 6 (offset 180)
//   +0xC4   : event object 7 (offset 196)
//   +0xD4   : state field A (offset 212)
//   +0xD8   : state field B (offset 216)
//   +0xDC   : state field C (offset 220)
//   +0xE0   : state field D (offset 224)
//   +0xE4   : state field E (offset 228)
//   +0x7C   : width * height (offset 124/128 for 3F00_h)
//   +0x2888 : fiber/thread context pointer (offset 10376 for EB30/EB70)

// Forward declarations for Xbox 360 kernel synchronization primitives
extern "C" {
    // Xbox kernel event functions
    long KeWaitForSingleObject(void* object, int waitReason, int waitMode, int alertable, void* timeout);
    long KeSetEvent(void* event, long increment, int wait);
    long KeResetEvent(void* event);

    // Debug output
    void DbgPrint(const char* format, ...);

    // TLS fiber setup — returns current fiber/thread context
    void* _crt_tls_fiber_setup();
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 1: Field Getters (8B each — load word + return)
// ─────────────────────────────────────────────────────────────────────────────

/// Returns state field at offset +212 (width or state A).
/// CCalMoviePlayer::GetStateA @ 0x8248EEE8 | size: 0x08
uint32_t CCalMoviePlayer::GetStateA() {
    return m_statusA;
}

/// Returns state field at offset +216 (height or state B).
/// CCalMoviePlayer::GetStateB @ 0x8248EEF0 | size: 0x08
uint32_t CCalMoviePlayer::GetStateB() {
    return m_statusB;
}

/// Returns state field at offset +220 (frame rate or state C).
/// CCalMoviePlayer::GetStateC @ 0x8248EEF8 | size: 0x08
uint32_t CCalMoviePlayer::GetStateC() {
    return m_isPlaying;
}

/// Returns state field at offset +224.
/// CCalMoviePlayer::GetStateD @ 0x8248EF00 | size: 0x08
uint32_t CCalMoviePlayer::GetStateD() {
    return m_statusFieldB;
}

/// Returns state field at offset +228.
/// CCalMoviePlayer::GetStateE @ 0x8248EF08 | size: 0x08
uint32_t CCalMoviePlayer::GetStateE() {
    return m_statusFieldC;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 2: KeWaitForSingleObject wrappers (24B each)
// Wait on KEVENT objects with WaitReason=3, WaitMode=1, Alertable=0, Timeout=NULL
// ─────────────────────────────────────────────────────────────────────────────

/// Waits on event object 0 (offset +84).
/// CCalMoviePlayer::WaitForEvent0 @ 0x8248ED88 | size: 0x18
void CCalMoviePlayer::WaitForEvent0() {
    KeWaitForSingleObject(m_event0, 3, 1, 0, nullptr);
}

/// Waits on event object 1 (offset +100).
/// CCalMoviePlayer::WaitForEvent1 @ 0x8248EDA0 | size: 0x18
void CCalMoviePlayer::WaitForEvent1() {
    KeWaitForSingleObject(m_event1, 3, 1, 0, nullptr);
}

/// Waits on event object 2 (offset +116).
/// CCalMoviePlayer::WaitForEvent2 @ 0x8248EDB8 | size: 0x18
void CCalMoviePlayer::WaitForEvent2() {
    KeWaitForSingleObject(m_event2, 3, 1, 0, nullptr);
}

/// Waits on event object 3 (offset +132).
/// CCalMoviePlayer::WaitForEvent3 @ 0x8248EDD0 | size: 0x18
void CCalMoviePlayer::WaitForEvent3() {
    KeWaitForSingleObject(m_event3, 3, 1, 0, nullptr);
}

/// Waits on event object 4 (offset +148).
/// CCalMoviePlayer::WaitForEvent4 @ 0x8248EDE8 | size: 0x18
void CCalMoviePlayer::WaitForEvent4() {
    KeWaitForSingleObject(m_event4, 3, 1, 0, nullptr);
}

/// Waits on event object 5 (offset +164).
/// CCalMoviePlayer::WaitForEvent5 @ 0x8248EE00 | size: 0x18
void CCalMoviePlayer::WaitForEvent5() {
    KeWaitForSingleObject(m_event5, 3, 1, 0, nullptr);
}

/// Waits on event object 6 (offset +180).
/// CCalMoviePlayer::WaitForEvent6 @ 0x8248EE18 | size: 0x18
void CCalMoviePlayer::WaitForEvent6() {
    KeWaitForSingleObject(m_event6, 3, 1, 0, nullptr);
}

/// Waits on event object 7 (offset +196).
/// CCalMoviePlayer::WaitForEvent7 @ 0x8248EE30 | size: 0x18
void CCalMoviePlayer::WaitForEvent7() {
    KeWaitForSingleObject(m_event7, 3, 1, 0, nullptr);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 3: KeSetEvent wrappers (16B each)
// Signal KEVENT objects with Increment=1, Wait=0
// ─────────────────────────────────────────────────────────────────────────────

/// Signals event object 0 (offset +84).
/// CCalMoviePlayer::SignalEvent0 @ 0x8248EE48 | size: 0x10
void CCalMoviePlayer::SignalEvent0() {
    KeSetEvent(m_event0, 1, 0);
}

/// Signals event object 1 (offset +100).
/// CCalMoviePlayer::SignalEvent1 @ 0x8248EE58 | size: 0x10
void CCalMoviePlayer::SignalEvent1() {
    KeSetEvent(m_event1, 1, 0);
}

/// Signals event object 2 (offset +116).
/// CCalMoviePlayer::SignalEvent2 @ 0x8248EE68 | size: 0x10
void CCalMoviePlayer::SignalEvent2() {
    KeSetEvent(m_event2, 1, 0);
}

/// Signals event object 3 (offset +132).
/// CCalMoviePlayer::SignalEvent3 @ 0x8248EE78 | size: 0x10
void CCalMoviePlayer::SignalEvent3() {
    KeSetEvent(m_event3, 1, 0);
}

/// Signals event object 4 (offset +148).
/// CCalMoviePlayer::SignalEvent4 @ 0x8248EE88 | size: 0x10
void CCalMoviePlayer::SignalEvent4() {
    KeSetEvent(m_event4, 1, 0);
}

/// Signals event object 5 (offset +164).
/// CCalMoviePlayer::SignalEvent5 @ 0x8248EE98 | size: 0x10
void CCalMoviePlayer::SignalEvent5() {
    KeSetEvent(m_event5, 1, 0);
}

/// Signals event object 6 (offset +180).
/// CCalMoviePlayer::SignalEvent6 @ 0x8248EEA8 | size: 0x10
void CCalMoviePlayer::SignalEvent6() {
    KeSetEvent(m_event6, 1, 0);
}

/// Signals event object 7 (offset +196).
/// CCalMoviePlayer::SignalEvent7 @ 0x8248EEB8 | size: 0x10
void CCalMoviePlayer::SignalEvent7() {
    KeSetEvent(m_event7, 1, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 4: KeResetEvent wrappers (8B each)
// Reset KEVENT objects at offsets +148, +164, +180, +196
// ─────────────────────────────────────────────────────────────────────────────

/// Resets event object at offset +148.
/// CCalMoviePlayer::ResetEvent4 @ 0x8248EEC8 | size: 0x08
void CCalMoviePlayer::ResetEvent4() {
    KeResetEvent(m_event4);
}

/// Resets event object at offset +164.
/// CCalMoviePlayer::ResetEvent5 @ 0x8248EED0 | size: 0x08
void CCalMoviePlayer::ResetEvent5() {
    KeResetEvent(m_event5);
}

/// Resets event object at offset +180.
/// CCalMoviePlayer::ResetEvent6 @ 0x8248EED8 | size: 0x08
void CCalMoviePlayer::ResetEvent6() {
    KeResetEvent(m_event6);
}

/// Resets event object at offset +196.
/// CCalMoviePlayer::ResetEvent7 @ 0x8248EEE0 | size: 0x08
void CCalMoviePlayer::ResetEvent7() {
    KeResetEvent(m_event7);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 5: Field Subtraction Getter (16B)
// ─────────────────────────────────────────────────────────────────────────────

/// Returns remaining buffer count: total (offset +52) minus consumed (offset +68).
/// CCalMoviePlayer::GetRemainingBuffers @ 0x8248DBD0 | size: 0x10
int32_t CCalMoviePlayer::GetRemainingBuffers() {
    return m_totalBuffers - m_semaphore;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 6: Setters with Change Callback (44B each)
// Store new value, and if changed, invoke callback via function pointer
// ─────────────────────────────────────────────────────────────────────────────

/// Sets state field A (offset +212); invokes change callback if value changed.
/// CCalMoviePlayer::SetStateA @ 0x8248EF10 | size: 0x2C
void CCalMoviePlayer::SetStateA(int32_t value) {
    int32_t old = m_statusA;
    m_statusA = value;
    if (old == value) return;

    typedef void (*CallbackFn)(void* context);
    CallbackFn callback = (CallbackFn)m_pChangeCallback;
    if (!callback) return;

    callback(m_pCallbackContext);
}

/// Sets state field B (offset +216); invokes change callback if value changed.
/// CCalMoviePlayer::SetStateB @ 0x8248EF40 | size: 0x2C
void CCalMoviePlayer::SetStateB(int32_t value) {
    int32_t old = m_statusB;
    m_statusB = value;
    if (old == value) return;

    typedef void (*CallbackFn)(void* context);
    CallbackFn callback = (CallbackFn)m_pChangeCallback;
    if (!callback) return;

    callback(m_pCallbackContext);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 7: Virtual Dispatch Thunks (16-28B)
// Thin wrappers that load vtable and tail-call a virtual function
// ─────────────────────────────────────────────────────────────────────────────

/// Dispatches to vtable slot 14 (offset +56) with this passed as r6.
/// CCalMoviePlayer::DispatchVSlot14 @ 0x8248E2D0 | size: 0x14
void CCalMoviePlayer::DispatchVSlot14() {
    VCALL(this, 14);
}

/// Dispatches to vtable slot 19 (offset +76), unpacking args from r4.
/// r4 -> { +0: ?, +4: second arg, +8: third arg }
/// CCalMoviePlayer::DispatchVSlot19WithArgs @ 0x8248E418 | size: 0x1C
void CCalMoviePlayer::DispatchVSlot19WithArgs(void* args) {
    typedef void (*VFunc)(void* self, uint32_t param, void* extra, void* context);
    uint8_t* argBytes = (uint8_t*)args;
    uint32_t param = *(uint32_t*)(argBytes + 4);
    void* extra = argBytes + 8;
    ((VFunc)VTABLE(this)[19])(this, param, extra, args);
}

/// Dispatches to vtable slot 19 (offset +76) with param=0.
/// CCalMoviePlayer::DispatchVSlot19NoArgs @ 0x8248E438 | size: 0x14
void CCalMoviePlayer::DispatchVSlot19NoArgs() {
    typedef void (*VFunc)(void* self, uint32_t param, uint32_t extra, uint32_t context);
    ((VFunc)VTABLE(this)[19])(this, 0, 0, 0);
}

/// Dispatches to vtable slot 32 -- MediaControl::Run.
/// CCalMoviePlayer::DispatchVSlot32 @ 0x8248EC68 | size: 0x10
void CCalMoviePlayer::DispatchVSlot32() {
    MediaControl_Run();
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 8: Virtual Call + Return Zero (48B each)
// Call a virtual function then return 0 (STATUS_SUCCESS pattern)
// ─────────────────────────────────────────────────────────────────────────────

/// Calls MediaControl::Pause and returns 0.
/// CCalMoviePlayer::CallVSlot34ReturnZero @ 0x8248EC88 | size: 0x30
int32_t CCalMoviePlayer::CallVSlot34ReturnZero() {
    MediaControl_Pause();
    return 0;
}

/// Calls MediaControl::Stop and returns 0.
/// CCalMoviePlayer::CallVSlot35ReturnZero @ 0x8248ECB8 | size: 0x30
int32_t CCalMoviePlayer::CallVSlot35ReturnZero() {
    MediaControl_Stop();
    return 0;
}

/// Calls MediaControl::GetState and returns 0.
/// CCalMoviePlayer::CallVSlot36ReturnZero @ 0x8248ECE8 | size: 0x30
int32_t CCalMoviePlayer::CallVSlot36ReturnZero() {
    MediaControl_GetState();
    return 0;
}

/// Calls MediaControl::StopWhenReady and returns 0.
/// CCalMoviePlayer::CallVSlot37ReturnZero @ 0x8248ED18 | size: 0x30
int32_t CCalMoviePlayer::CallVSlot37ReturnZero() {
    MediaControl_StopWhenReady();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 9: Debug Print Stubs (56B each)
// Print a debug message and return STATUS_NOT_IMPLEMENTED (0x80004001)
// These are unimplemented media control functions.
// ─────────────────────────────────────────────────────────────────────────────

// Debug format string and parameter strings for unimplemented media controls
static const char* STR_DBG_NOT_IMPL_FMT = "CAL: NYI %s";  // @ 0x82008ACC
static const char* STR_DBG_REWIND = "CCalMoviePlayer::Rewind";                                      // @ 0x82009280
static const char* STR_DBG_SEEK = "CCalMoviePlayer::Seek";                                          // @ 0x82009298

constexpr uint32_t STATUS_NOT_IMPLEMENTED = 0x80004001;

/// Rewind stub -- prints debug message, returns STATUS_NOT_IMPLEMENTED.
/// CCalMoviePlayer::Rewind @ 0x8248E758 | size: 0x38
uint32_t CCalMoviePlayer::Rewind() {
    DbgPrint(STR_DBG_NOT_IMPL_FMT, STR_DBG_REWIND);
    return STATUS_NOT_IMPLEMENTED;
}

/// Seek stub -- prints debug message, returns STATUS_NOT_IMPLEMENTED.
/// CCalMoviePlayer::Seek @ 0x8248E790 | size: 0x38
uint32_t CCalMoviePlayer::Seek() {
    DbgPrint(STR_DBG_NOT_IMPL_FMT, STR_DBG_SEEK);
    return STATUS_NOT_IMPLEMENTED;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 10: Frame Size Calculation + Virtual Dispatch (40B)
// ─────────────────────────────────────────────────────────────────────────────

/// Computes frame byte size from width*height, then dispatches to vtable slot 20.
/// Frame size = (width * height * 3) / 2  (YUV 4:2:0 format: 12 bits per pixel).
/// The multiply-by-3-shift-right-1 is done via (n + n*2) >> 1.
/// CCalMoviePlayer::GetFrameBuffer @ 0x82483F00 | size: 0x28
void* CCalMoviePlayer::GetFrameBuffer() {
    // Width and height at offsets +124/+128 (within event/buffer region).
    // Kept as raw-offset loads here: the CCalMoviePlayer struct definition
    // is shared with CCal movie-event code that uses the same range as an
    // event ring buffer, so we cannot promote these to named fields without
    // racing the other lifters of this class. Revisit once the full class
    // layout is consolidated.
    uint8_t* self = (uint8_t*)this;
    uint32_t width = *(uint32_t*)(self + 124);
    uint32_t height = *(uint32_t*)(self + 128);
    uint32_t frameSize = (width * height * 3) >> 1;  // YUV 4:2:0: 1.5 bytes per pixel

    // Dispatch to vtable slot 20 with computed frame size
    typedef void* (*VFunc)(void* self, uint32_t size);
    return ((VFunc)VTABLE(this)[20])(this, frameSize);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 11: Fiber Context Management (64B + 16B)
// Manage the fiber/thread context pointer at offset +10376 (0x2888)
// ─────────────────────────────────────────────────────────────────────────────

/// Replaces the fiber context. Releases old context if present, acquires new one.
/// CCalMoviePlayer::ReplaceFiberContext @ 0x8235EB30 | size: 0x40
void CCalMoviePlayer::ReplaceFiberContext() {
    if (m_pFiberContext) {
        _crt_tls_fiber_setup();  // Release/cleanup existing context
    }
    m_pFiberContext = _crt_tls_fiber_setup();
}

/// Clears the fiber context pointer (with memory barrier).
/// CCalMoviePlayer::ClearFiberContext @ 0x8235EB70 | size: 0x10
void CCalMoviePlayer::ClearFiberContext() {
    // eieio -- enforce in-order execution of I/O (memory barrier)
    m_pFiberContext = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 12: Constructor / Destructor Helpers (60-64B)
// ─────────────────────────────────────────────────────────────────────────────

// External helper functions used by constructors/destructors
extern "C" {
    void CCalMoviePlayer_BaseCtorHelper(void* obj);  // @ 0x82488580 - base class constructor helper
    void CCalMoviePlayer_BaseDtorHelper(void* obj);  // @ 0x824885C8 - base class destructor helper
    void CCalMoviePlayer_InitEvents(void* obj);      // @ 0x8248E0E8 - event initialization
    void CCalMoviePlayer_CleanupEvents(void* obj);   // @ 0x8248FA48 - event cleanup
}

// Vtable addresses for CCalMoviePlayer hierarchy
static void* const VTABLE_CCalMoviePlayer_Base    = (void*)0x820092C0;  // vtable for CCalMoviePlayer (intermediate/base, 292 bytes)
static void* const VTABLE_CCalMoviePlayer_Derived = (void*)0x82008B80;  // vtable for CCalMoviePlayer (derived, 296 bytes)

/// CCalMoviePlayer intermediate constructor -- initializes base, sets vtable, inits events.
/// CCalMoviePlayer::CtorIntermediate @ 0x8248ED48 | size: 0x40
void CCalMoviePlayer::CtorIntermediate() {
    CCalMoviePlayer_BaseCtorHelper(this);
    *(void**)this = VTABLE_CCalMoviePlayer_Base;  // vtable for CCalMoviePlayer (base)
    CCalMoviePlayer_InitEvents(this);
}

/// CCalMoviePlayer derived constructor -- calls intermediate ctor, sets derived vtable.
/// CCalMoviePlayer::CtorDerived @ 0x82487200 | size: 0x3C
void CCalMoviePlayer::CtorDerived() {
    CtorIntermediate();
    *(void**)this = VTABLE_CCalMoviePlayer_Derived;  // vtable for CCalMoviePlayer (derived)
}

/// CCalMoviePlayer derived destructor -- sets intermediate vtable, cleans up events, destroys base.
/// CCalMoviePlayer::DtorDerived @ 0x824915C8 | size: 0x40
void CCalMoviePlayer::DtorDerived() {
    *(void**)this = VTABLE_CCalMoviePlayer_Base;  // revert to base vtable for CCalMoviePlayer
    CCalMoviePlayer_CleanupEvents(this);
    CCalMoviePlayer_BaseDtorHelper(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 13: Buffer Management with Virtual Call (60B)
// Calculate buffer element pointer and call virtual init on it
// ─────────────────────────────────────────────────────────────────────────────

/// Returns the current read buffer element after calling vtable slot 1 (init/reset).
/// Element pointer = base[+48] + readIndex[+56] * 60.
/// CCalMoviePlayer::GetCurrentReadBuffer @ 0x8248DBE0 | size: 0x14 (symbol size; actual scaffold 76B)
void* CCalMoviePlayer::GetCurrentReadBuffer() {
    char* element = m_pBufferBase + m_readIndex * 60;
    VCALL(element, 1);  // init/reset buffer element
    return element;
}

/// Returns the current write buffer element after calling vtable slot 1 (init/reset).
/// Element pointer = base[+48] + writeIndex[+60] * 60.
/// CCalMoviePlayer::GetCurrentWriteBuffer @ 0x8248DC30 | size: 0x14 (symbol size; actual scaffold 76B)
void* CCalMoviePlayer::GetCurrentWriteBuffer() {
    char* element = m_pBufferBase + m_writeIndex * 60;
    VCALL(element, 1);  // init/reset buffer element
    return element;
}

/// Returns a buffer element by index after calling vtable slot 1 (init/reset).
/// Element pointer = base[+48] + index * 60.
/// CCalMoviePlayer::GetBufferByIndex @ 0x8248DC80 | size: 0x14 (symbol size; actual scaffold 72B)
void* CCalMoviePlayer::GetBufferByIndex(uint32_t index) {
    char* element = m_pBufferBase + index * 60;
    VCALL(element, 1);  // init/reset buffer element
    return element;
}

// ─────────────────────────────────────────────────────────────────────────────
// Group 14: Ring Buffer Advance with Atomic Semaphore (104B each)
// Advance read/write index; if wrapping, atomically adjust semaphore
// ─────────────────────────────────────────────────────────────────────────────

/// Advances read index; atomically decrements semaphore (offset +68) on wrap.
/// CCalMoviePlayer::AdvanceReadIndex @ 0x8248DCC8 | size: 0x68
void CCalMoviePlayer::AdvanceReadIndex() {
    volatile uint32_t* pReadIdx = (volatile uint32_t*)&m_readIndex;
    volatile uint32_t* pTotal = (volatile uint32_t*)&m_totalBuffers;
    volatile int32_t* pSemaphore = (volatile int32_t*)&m_semaphore;

    uint32_t idx = *pReadIdx + 1;
    *pReadIdx = idx;

    if (idx >= *pTotal) {
        *pReadIdx = 0;
    }

    // Atomically decrement semaphore
    int32_t old_val;
    do {
        old_val = *pSemaphore;
    } while (!__sync_bool_compare_and_swap(pSemaphore, old_val, old_val - 1));
}

/// Advances write index; atomically increments semaphore (offset +68) on wrap.
/// CCalMoviePlayer::AdvanceWriteIndex @ 0x8248DD30 | size: 0x68
void CCalMoviePlayer::AdvanceWriteIndex() {
    volatile uint32_t* pWriteIdx = (volatile uint32_t*)&m_writeIndex;
    volatile uint32_t* pTotal = (volatile uint32_t*)&m_totalBuffers;
    volatile int32_t* pSemaphore = (volatile int32_t*)&m_semaphore;

    uint32_t idx = *pWriteIdx + 1;
    *pWriteIdx = idx;

    if (idx >= *pTotal) {
        *pWriteIdx = 0;
    }

    // Atomically increment semaphore
    int32_t old_val;
    do {
        old_val = *pSemaphore;
    } while (!__sync_bool_compare_and_swap(pSemaphore, old_val, old_val + 1));
}

