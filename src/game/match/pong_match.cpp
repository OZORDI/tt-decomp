/**
 * pong_match.cpp — Match logic and game mode implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_match.hpp"
#include <cstdint>
#include <cstddef>

// Forward declarations for external functions
extern "C" {
    void hsmContext_SetNextState(void* context, int stateId);
    int PongNetGameModeCoordinator_GetGameState(void* coordinator);
    void atSingleton_GetValue(void* event, void* typeInfo, void* outValue);
    void nop_LogDebug(const char* message);
}

// External globals
extern uint8_t g_spectatorModeEnabled;
extern void* g_pongGameInstance;
extern void* g_playerDataArray;

// Event type constants
enum GameEventType {
    EVENT_SPECTATOR_MODE_TOGGLE = 2083,
    EVENT_SPECTATOR_EXIT = 2145,
    EVENT_NETWORK_PLAYER_JOINED = 14369,
    EVENT_NETWORK_PLAYER_LEFT = 14370,
    EVENT_RETURN_TO_LOBBY = 14376,
    EVENT_SPECTATOR_READY = 2060
};

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::ProcessEvent() [vtable slot 18 @ 0x8238D660]
// 
// Event handler for spectator game mode logic. Processes various game events
// including spectator mode toggles, network player join/leave events, and
// state transitions for the spectator viewing system.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSpectator::ProcessEvent(void* event) {
    // Get current state handler from state array
    void* currentStateHandler = nullptr;
    if (m_currentStateIndex <= 17) {
        currentStateHandler = m_stateArray[m_currentStateIndex];
    }
    
    // Read event type from event object (first 2 bytes)
    uint16_t eventType = *(uint16_t*)event;
    
    // Handle spectator mode toggle
    if (eventType == EVENT_SPECTATOR_MODE_TOGGLE) {
        g_spectatorModeEnabled = 1;
        goto dispatch_to_state;
    }
    
    // Handle spectator exit - transition to state 17
    if (eventType == EVENT_SPECTATOR_EXIT) {
        hsmContext_SetNextState(this, 17);
        goto dispatch_to_state;
    }
    
    // Handle network player joined event
    if (eventType == EVENT_NETWORK_PLAYER_JOINED) {
        // Calculate player coordinator offset: playerIndex * 2784 + 36256
        uint32_t playerIndex = *(uint32_t*)((char*)g_pongGameInstance + 4);
        void* playerCoordinator = nullptr;
        
        if (playerIndex <= 3) {
            uint32_t offset = playerIndex * 2784 + 36256;
            playerCoordinator = (char*)g_playerDataArray + offset;
        }
        
        // Check if game state is >= 5 (in-game)
        int gameState = PongNetGameModeCoordinator_GetGameState(playerCoordinator);
        if (gameState < 5) {
            goto dispatch_to_state;
        }
        
        // Check if game state is <= 6 (not post-game)
        gameState = PongNetGameModeCoordinator_GetGameState(playerCoordinator);
        if (gameState > 6) {
            goto dispatch_to_state;
        }
        
        // Check if game object exists and has valid replay buffer
        if (m_pGameObject == nullptr) {
            goto dispatch_to_state;
        }
        
        int replayBufferIndex = *(int*)((char*)m_pGameObject + 992);
        if (replayBufferIndex == -1) {
            goto dispatch_to_state;
        }
        
        // Extract player ID from event
        int eventPlayerId;
        atSingleton_GetValue(event, (void*)0x82027898, &eventPlayerId);
        
        // Set spectator mode active
        m_spectatorModeActive = 1;
        
        // Check if event player matches game object's player
        int gameObjectPlayerId = *(int*)((char*)m_pGameObject + 992);
        m_isSpectatingLocalPlayer = (eventPlayerId == gameObjectPlayerId) ? 1 : 0;
        
        goto dispatch_to_state;
    }
    
    // Handle network player left event
    if (eventType == EVENT_NETWORK_PLAYER_LEFT) {
        // Similar logic to player joined
        uint32_t playerIndex = *(uint32_t*)((char*)g_pongGameInstance + 4);
        void* playerCoordinator = nullptr;
        
        if (playerIndex <= 3) {
            uint32_t offset = playerIndex * 2784 + 36256;
            playerCoordinator = (char*)g_playerDataArray + offset;
        }
        
        int gameState = PongNetGameModeCoordinator_GetGameState(playerCoordinator);
        if (gameState < 5) {
            goto dispatch_to_state;
        }
        
        gameState = PongNetGameModeCoordinator_GetGameState(playerCoordinator);
        if (gameState > 6) {
            goto dispatch_to_state;
        }
        
        // Extract player IDs from event
        int playerId1, playerId2;
        atSingleton_GetValue(event, (void*)0x82030084, &playerId1);
        atSingleton_GetValue(event, (void*)0x82030088, &playerId2);
        
        // Set spectator mode based on player 2 presence
        m_spectatorModeActive = (playerId1 != 0) ? 2 : 3;
        m_isSpectatingLocalPlayer = (playerId2 != 0) ? 1 : 0;
        
        goto dispatch_to_state;
    }
    
    // Handle return to lobby - transition to state 11
    if (eventType == EVENT_RETURN_TO_LOBBY) {
        hsmContext_SetNextState(this, 11);
        goto dispatch_to_state;
    }
    
    // Handle spectator ready event
    if (eventType == EVENT_SPECTATOR_READY) {
        if (m_spectatorModeActive != 0) {
            nop_LogDebug("Spectator ready - exiting spectator mode");
            m_spectatorModeActive = 0;
        }
    }
    
dispatch_to_state:
    // Dispatch event to current state handler if it exists
    if (currentStateHandler != nullptr) {
        // Virtual call to state handler's ProcessEvent method (vtable slot 16)
        void** vtable = *(void***)currentStateHandler;
        typedef void (*ProcessEventFn)(void*, void*);
        ProcessEventFn processEvent = (ProcessEventFn)vtable[16];
        processEvent(currentStateHandler, event);
    }
    
    // Also dispatch to game object if it exists
    if (m_pGameObject != nullptr) {
        void** vtable = *(void***)m_pGameObject;
        typedef void (*ProcessEventFn)(void*, void*);
        ProcessEventFn processEvent = (ProcessEventFn)vtable[5];
        processEvent(m_pGameObject, event);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// statePostGame — Post-game state machine functions
//
// Shared between gmLogicSinglesMatch::statePostGame [vtable @ 0x8203168C]
// and gmLogicSpectator::statePostGame [vtable @ 0x8206BDDC].
// Both vtables point to the same function addresses.
//
// statePostGame struct layout:
//   +0x00: void** vtable
//   +0x04: void*  m_pHsmContext      — owning HSM context for state transitions
//   +0x08: void*  m_pParentLogic     — owning gmLogic instance
//   +0x10: uint8_t m_bScoresProcessed — post-game scores submitted
//   +0x11: uint8_t m_bExitReady       — ready to transition out
// ═════════════════════════════════════════════════════════════════════════════

// External functions used by statePostGame methods
extern "C" {
    void stateServeBounce_OnEnter_Spectator(void* state); // @ 0x8238D9A8
    void PostPageGroupMessage(int msgCode, int mask,
                              int paramCount, void* params);  // pg_E6E0 @ 0x8225E6E0
    bool IsNetworkClientActive(void* client);  // pg_10E0_g @ 0x821010E0
    void RemoveNodeFromList(void* container, void* node); // SinglesNetworkClient_F508_g @ 0x823CF508
    void* FindPostPointMessage(void* container);           // statePostPoint_E7E0_g @ 0x823CE7E0
    void ShowUnlockProgressDialog(void* unlockData, int mode, int count,
                                  uint64_t p1, uint64_t p2,
                                  uint64_t p3, uint64_t p4, int flags); // util_94B8 @ 0x823794B8
}

// Globals for post-game logic
extern void* g_input_obj_ptr;      // @ 0x825EAB28 — match context singleton
extern void* g_render_obj_ptr;     // @ 0x825EAB2C — network client singleton
extern void* g_pMatchResultData;   // @ 0x8271A320 — unlock eligibility (+16,+20 = slot IDs)
extern void* g_pTournamentData;    // @ 0x8271A334 — tournament progress (+44=total,+128=current)
extern void* g_pNetworkStateFlags; // @ 0x8271A340 — network session state (+8=mode)

// ExitPostGameMessage class identifier for linked list lookups
extern uint32_t g_exitPostGameMsgClassId; // @ 0x825D1218 — compared against vfn_6 return
// Message handler pool descriptor (+8 = pool manager ptr, 12-byte node stride)
extern char g_messagePoolInfo[];           // @ 0x825D1224
// Dialog parameters (two uint64_t values)
extern uint64_t g_unlockDialogParams[2];   // @ 0x8261A0C0

// Post-game message codes
static constexpr uint16_t MSG_ENTER_POST_GAME     = 8197;
static constexpr uint16_t MSG_EXIT_POST_GAME      = 8200;
static constexpr uint16_t MSG_POST_GAME_SPECTATOR = 10252;
static constexpr uint16_t MSG_EXIT_MATCH          = 2139;
static constexpr uint16_t MSG_FADE_TRANSITION     = 10254;
static constexpr uint16_t MSG_CLEANUP_MATCH       = 2075;

// Forward declarations for batch functions
void* statePostGame_FindExitMessage(void* container);   // E948_g
void  statePostGame_RemoveExitMessage(void* gameObj);    // CC58
void* statePostGame_AllocateMessageNode();               // 9820_g

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::GetName  [vtable slot 13 @ 0x8210BD48] | size: 0x0C
// Returns the state name string for HSM debugging/identification.
// ─────────────────────────────────────────────────────────────────────────────
const char* statePostGame_GetName(void* /*self*/) { // @ 0x8210BD48
    return "PostGame";
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::CanTransitionTo  [vtable slot 15 @ 0x8210BD20] | size: 0x28
// Returns true if this state accepts transitions to the given target state.
// Singles match variant: accepts PostMatch(28), PostTourney(29), MainMenu(30).
// ─────────────────────────────────────────────────────────────────────────────
bool statePostGame_CanTransitionTo(void* /*self*/, int targetStateId) { // @ 0x8210BD20
    return (targetStateId == 28 || targetStateId == 29 || targetStateId == 30);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::CanTransitionTo (spectator)  [vtable slot 15 @ 0x82390258] | size: 0x28
// Spectator variant: accepts states 3, 16, 17.
// ─────────────────────────────────────────────────────────────────────────────
bool statePostGame_CanTransitionTo_Spectator(void* /*self*/, int targetStateId) { // @ 0x82390258
    return (targetStateId == 16 || targetStateId == 3 || targetStateId == 17);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame_FindExitMessage @ 0x823CE948 | size: 0x74
// Searches the message handler linked list (at container+92) for an
// ExitPostGameMessage node. Returns the handler object if found, else nullptr.
// ─────────────────────────────────────────────────────────────────────────────
void* statePostGame_FindExitMessage(void* container) { // @ 0x823CE948
    // Linked list head at container+92
    // Node layout: +0=handler object ptr, +12=next node ptr
    void* node = *(void**)((char*)container + 92);

    while (node != nullptr) {
        void* handler = *(void**)node;

        // Call vfn_6 (GetClassId) on the handler object
        void** vtable = *(void***)handler;
        typedef uint32_t (*GetClassIdFn)(void*);
        uint32_t classId = ((GetClassIdFn)vtable[6])(handler);

        if (classId == g_exitPostGameMsgClassId) {
            return handler;
        }

        node = *(void**)((char*)node + 12);
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame_RemoveExitMessage @ 0x823CCC58 | size: 0x7C
// Finds the ExitPostGameMessage handler in the game object's driver list,
// calls its cleanup virtual (vfn_4), and removes it from the linked list.
// ─────────────────────────────────────────────────────────────────────────────
void statePostGame_RemoveExitMessage(void* gameObj) { // @ 0x823CCC58
    char* container = (char*)gameObj + 4364;
    void* node = *(void**)((char*)container + 92);

    while (node != nullptr) {
        void* handler = *(void**)node;

        void** vtable = *(void***)handler;
        typedef uint32_t (*GetClassIdFn)(void*);
        uint32_t classId = ((GetClassIdFn)vtable[6])(handler);

        if (classId == g_exitPostGameMsgClassId) {
            // Cleanup the handler via vfn_4
            typedef void (*CleanupFn)(void*);
            ((CleanupFn)vtable[4])(handler);

            // Remove node from the linked list
            RemoveNodeFromList(container, node);
            return;
        }

        node = *(void**)((char*)node + 12);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame_AllocateMessageNode @ 0x823B9820 | size: 0x8C
// Allocates a node from the ExitPostGameMessage pool. The pool uses an
// intrusive free list with uint16_t indices and 12-byte node stride.
// Returns the allocated node address, or nullptr if pool is empty.
// ─────────────────────────────────────────────────────────────────────────────
void* statePostGame_AllocateMessageNode() { // @ 0x823B9820
    // Pool manager pointer is at g_messagePoolInfo+8
    char* poolMgr = *(char**)(g_messagePoolInfo + 8);

    // Free list head index at poolMgr+28
    uint16_t freeHead = *(uint16_t*)(poolMgr + 28);
    if (freeHead == 0xFFFF) {
        return nullptr;
    }

    // Compute node address: poolMgr + freeHead * 12
    char* node = poolMgr + (uint32_t)freeHead * 12;

    // Read the next free index from node+10
    uint16_t nextFree = *(uint16_t*)(node + 10);

    // Update free list head
    *(uint16_t*)(poolMgr + 28) = nextFree;

    // If next free node exists, clear its prev-link
    if (nextFree != 0xFFFF) {
        char* nextNode = poolMgr + (uint32_t)nextFree * 12;
        *(uint16_t*)(nextNode + 8) = 0xFFFF;
    }

    // Clear this node's next-free link
    *(uint16_t*)(node + 10) = 0xFFFF;

    // Decrement free count at poolMgr+24
    *(uint16_t*)(poolMgr + 24) -= 1;

    return node;
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::OnEvent (spectator)  [vtable slot 16 @ 0x823901A0] | size: 0xB8
// Handles MSG_EXIT_POST_GAME (8200) events in spectator context.
// If network is active, transitions to state 16.
// If offline, sends cleanup messages and transitions to state 3.
// ─────────────────────────────────────────────────────────────────────────────
void statePostGame_OnEvent_Spectator(void* self, void* event) { // @ 0x823901A0
    uint16_t eventType = *(uint16_t*)event;

    if (eventType != MSG_EXIT_POST_GAME) {
        return;
    }

    // Check if network client is currently active
    if (IsNetworkClientActive(g_render_obj_ptr)) {
        // Network active — transition to PostGame-with-network state
        void* hsmContext = *(void**)((char*)self + 0x04);
        hsmContext_SetNextState(hsmContext, 16);
    } else {
        // Offline — send cleanup messages and return to lobby
        PostPageGroupMessage(MSG_EXIT_MATCH, 64, 0, nullptr);

        uint32_t fadeParam = 1;
        PostPageGroupMessage(MSG_FADE_TRANSITION, 64, 1, &fadeParam);

        PostPageGroupMessage(MSG_CLEANUP_MATCH, 64, 0, nullptr);

        void* hsmContext = *(void**)((char*)self + 0x04);
        hsmContext_SetNextState(hsmContext, 3);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Helper: process ExitPostGameMessage dispatching for spectator states.
// Shared logic between OnEnter_Spectator and Update_Spectator.
// ─────────────────────────────────────────────────────────────────────────────
static void ProcessExitPostGameDrivers(void* self) {
    // Access the match object through parentLogic: *(parentLogic+44)
    void* parentLogic = *(void**)((char*)self + 0x08);
    void* matchObj = *(void**)((char*)parentLogic + 44);

    if (matchObj == nullptr) {
        return;
    }

    char* driverContainer = (char*)matchObj + 4364;

    // Check if ExitPostGameMessage handler already exists
    void* exitHandler = statePostGame_FindExitMessage(driverContainer);

    if (exitHandler != nullptr) {
        // Handler exists — clean it up and remove it
        statePostGame_RemoveExitMessage(matchObj);
        return;
    }

    // No exit handler yet — check if a PostPoint handler exists
    void* postPointHandler = FindPostPointMessage(driverContainer);
    if (postPointHandler == nullptr) {
        return;
    }

    // Allocate a new message node and dispatch it
    void* newNode = statePostGame_AllocateMessageNode();
    if (newNode == nullptr) {
        return;
    }

    // Initialize via vfn_4, then process via vfn_5
    void** vtable = *(void***)newNode;
    typedef void (*VoidFn)(void*);
    ((VoidFn)vtable[4])(newNode);
    ((VoidFn)vtable[5])(newNode);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::Update (spectator)  [vtable slot 10 @ 0x823900D8] | size: 0xC4
// Per-frame update for spectator post-game state. Processes pending
// ExitPostGameMessage handlers in the driver list.
// ─────────────────────────────────────────────────────────────────────────────
void statePostGame_Update_Spectator(void* self) { // @ 0x823900D8
    ProcessExitPostGameDrivers(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::OnEnter (spectator)  [vtable slot 11 @ 0x8238FFD0] | size: 0x108
// State entry for spectator post-game. Initializes parent state, sends
// post-game UI messages, and processes ExitPostGameMessage driver setup.
// ─────────────────────────────────────────────────────────────────────────────
void statePostGame_OnEnter_Spectator(void* self) { // @ 0x8238FFD0
    // Call parent state's OnEnter (spectator variant)
    stateServeBounce_OnEnter_Spectator(self);

    // Send "enter post-game" message
    PostPageGroupMessage(MSG_ENTER_POST_GAME, 64, 0, nullptr);

    // If not in network mode, also send the spectator-specific message
    void* networkFlags = g_pNetworkStateFlags;
    int networkMode = *(int*)((char*)networkFlags + 8);
    if (networkMode == 0) {
        PostPageGroupMessage(MSG_POST_GAME_SPECTATOR, 64, 0, nullptr);
    }

    // Process ExitPostGameMessage driver setup
    ProcessExitPostGameDrivers(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostGame::PostGameCheck @ 0x8210C738 | size: 0x134
// Determines post-game unlock/progression state. Called as vfn_14 in the
// singles match context. Checks whether the player won the final game of a
// tournament, has pending unlocks, or needs a progression dialog shown.
// Always sets m_bExitReady when complete.
// ─────────────────────────────────────────────────────────────────────────────
void statePostGame_PostGameCheck(void* self) { // @ 0x8210C738
    // Skip if already processed
    uint8_t* pExitReady = (uint8_t*)((char*)self + 0x11);
    if (*pExitReady) {
        return;
    }

    void* matchCtx = g_input_obj_ptr;
    int gameMode = *(int*)((char*)matchCtx + 8);

    bool isFinalGameWinner = false;

    // In network mode (gameMode==3), check if player won the final game
    if (gameMode == 3) {
        void* netClient = g_render_obj_ptr;
        int player1Score = *(int*)((char*)netClient + 28);
        int player2Score = *(int*)((char*)netClient + 32);

        if (player1Score > player2Score) {
            // Player 1 won this game — check if it's the final game
            void* tournamentData = g_pTournamentData;
            int totalGames = *(int*)((char*)tournamentData + 44);
            int currentGame = *(int*)((char*)tournamentData + 128);

            if (currentGame == totalGames - 1) {
                isFinalGameWinner = true;
            }
        }
    }

    if (!isFinalGameWinner) {
        // Check unlock eligibility: two slot IDs at +16 and +20
        void* unlockData = g_pMatchResultData;
        int unlockSlot1 = *(int*)((char*)unlockData + 16);
        int unlockSlot2 = *(int*)((char*)unlockData + 20);

        // If either slot has content (not -1), pending unlocks exist
        bool hasUnlockPending = !(unlockSlot1 == -1 && unlockSlot2 == -1);

        if (hasUnlockPending) {
            // Content to unlock — will be handled by subsequent state
        } else {
            // No unlocks pending — show progression/continue dialog
            ShowUnlockProgressDialog(
                unlockData, 2, 1,
                g_unlockDialogParams[0], g_unlockDialogParams[1],
                g_unlockDialogParams[0], g_unlockDialogParams[1],
                0);
        }
    }

    // Mark post-game check as complete
    *pExitReady = 1;
}


// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSwingPractice::stateMainMenu — HSM state for swing practice main menu
//
// Vtable @ 0x820304FC. Inherits from hsmState.
// State struct layout:
//   +0x00: void** vtable
//   +0x04: void*  m_pHsmContext     — owning HSM context
//   +0x08: void*  m_pParentLogic    — owning gmLogicSwingPractice instance
// ═════════════════════════════════════════════════════════════════════════════

// External functions for stateMainMenu
extern "C" {
    void stateServeBounce_OnEnter(void* self, int param);           // @ 0x82106A60
    void gmLogicSwingPractice_OnExit(void* parentLogic);            // pongPlayer_2DF0_h @ 0x82102DF0
}

// Mutable global: swing practice fade time
extern float g_swingPracticeFadeTime;  // @ 0x825C9A28

// ─────────────────────────────────────────────────────────────────────────────
// stateMainMenu::GetName  [vtable slot 13 @ 0x82101F20] | size: 0x0C
// Returns the state name string for HSM debugging/identification.
// ─────────────────────────────────────────────────────────────────────────────
const char* stateMainMenu_GetName(void* /*self*/) { // @ 0x82101F20
    return "MainMenu";
}

// ─────────────────────────────────────────────────────────────────────────────
// stateMainMenu::OnEnter  [vtable slot 11 @ 0x821037D8] | size: 0x28
// Enters the main menu state. Retrieves the parent logic's UI page group
// (at parentLogic+148) and calls its Open method (vtable slot 4) to display
// the main menu screen overlay.
// ─────────────────────────────────────────────────────────────────────────────
void stateMainMenu_OnEnter(void* self) { // @ 0x821037D8
    void* parentLogic = *(void**)((char*)self + 8);
    void* pageGroup = *(void**)((char*)parentLogic + 148);

    if (pageGroup == nullptr) {
        return;
    }

    // Call Open on the page group sub-object at +384
    // vtable slot 4 (Open/Show)
    void* pageGroupUI = (void*)((char*)pageGroup + 384);
    void** vtable = *(void***)pageGroupUI;
    typedef void (*OpenFn)(void*);
    ((OpenFn)vtable[4])(pageGroupUI);
}

// ─────────────────────────────────────────────────────────────────────────────
// stateMainMenu::OnExit  [vtable slot 12 @ 0x82103800] | size: 0x08
// Exits the main menu state by delegating to the parent logic's exit handler.
// ─────────────────────────────────────────────────────────────────────────────
void stateMainMenu_OnExit(void* self) { // @ 0x82103800
    void* parentLogic = *(void**)((char*)self + 8);
    gmLogicSwingPractice_OnExit(parentLogic);
}

// ─────────────────────────────────────────────────────────────────────────────
// stateMainMenu::CanTransitionTo  [vtable slot 15 @ 0x82103808] | size: 0x18
// Returns true if the target state ID is 3 (the Aim state in swing practice).
// ─────────────────────────────────────────────────────────────────────────────
bool stateMainMenu_CanTransitionTo(void* /*self*/, int targetStateId) { // @ 0x82103808
    return (targetStateId == 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// stateMainMenu::OnEvent  [vtable slot 16 @ 0x82103820] | size: 0x58
// Handles event 2063 (MSG_START_PRACTICE). When received, transitions the
// HSM context to state 3 (Aim) and sets the fade time to 0.5 seconds.
// ─────────────────────────────────────────────────────────────────────────────
void stateMainMenu_OnEvent(void* self, void* event) { // @ 0x82103820
    uint16_t eventType = *(uint16_t*)event;

    if (eventType != 2063) {
        return;
    }

    // Transition to Aim state (state 3)
    void* hsmContext = *(void**)((char*)self + 8);
    hsmContext_SetNextState(hsmContext, 3);

    // Set fade time to 0.5 seconds for the transition
    g_swingPracticeFadeTime = 0.5f;
}

// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSinglesMatch::statePostMatch — HSM state after match completion
//
// Vtable @ 0x82031844. Inherits from hsmState.
// State struct layout:
//   +0x00: void** vtable
//   +0x04: void*  m_pHsmContext      — owning HSM context
//   +0x08: void*  m_pParentLogic     — owning gmLogicSinglesMatch instance
//   +0x10: uint8_t m_bReadyToExit    — set when exit event received
//   +0x11: uint8_t m_bReturnToLobby  — set when returning to lobby
// ═════════════════════════════════════════════════════════════════════════════

// External functions for statePostMatch
extern "C" {
    void statePostMatch_ProcessExit(void* self);    // pg_C5B0 @ 0x8210C5B0
    void statePostMatch_ProcessReturn(void* self);  // statePostGame_C738_p46 @ 0x8210C738
}

// External globals for statePostMatch
// g_input_obj_ptr @ 0x825EAB28 — already declared above
// g_render_obj_ptr @ 0x825EAB2C — already declared above
// g_pNetworkStateFlags @ 0x8271A340 — already declared above

// Player data array with per-player state blocks
extern char g_playerDataArray2[]; // lbl_8261A3D0 @ 0x8261A3D0 (832 bytes)

// ─────────────────────────────────────────────────────────────────────────────
// statePostMatch::GetName  [vtable slot 13 @ 0x8210C5A0] | size: 0x0C
// Returns the state name string for HSM debugging/identification.
// ─────────────────────────────────────────────────────────────────────────────
const char* statePostMatch_GetName(void* /*self*/) { // @ 0x8210C5A0
    return "PostMatch";
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostMatch::CanTransitionTo  [vtable slot 15 @ 0x8210C588] | size: 0x18
// Returns true if the target state ID is 33 (return to main menu / lobby).
// ─────────────────────────────────────────────────────────────────────────────
bool statePostMatch_CanTransitionTo(void* /*self*/, int targetStateId) { // @ 0x8210C588
    return (targetStateId == 33);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostMatch::OnEvent  [vtable slot 16 @ 0x8210C558] | size: 0x2C
// Handles event MSG_EXIT_POST_MATCH (8201). Sets the m_bReadyToExit flag
// so the Update function knows to begin exit processing.
// ─────────────────────────────────────────────────────────────────────────────
void statePostMatch_OnEvent(void* self, void* event) { // @ 0x8210C558
    uint16_t eventType = *(uint16_t*)event;

    if (eventType != 8201) {
        return;
    }

    // Signal that exit was requested
    *(uint8_t*)((char*)self + 16) = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostMatch::Update  [vtable slot 10 @ 0x8210C530] | size: 0x24
// Checks exit flags each frame. If m_bReturnToLobby (+17) is set, processes
// the lobby return. If m_bReadyToExit (+16) is set, processes the exit.
// ─────────────────────────────────────────────────────────────────────────────
void statePostMatch_Update(void* self) { // @ 0x8210C530
    uint8_t returnToLobby = *(uint8_t*)((char*)self + 17);
    if (returnToLobby != 0) {
        statePostMatch_ProcessExit(self);
        return;
    }

    uint8_t readyToExit = *(uint8_t*)((char*)self + 16);
    if (readyToExit == 0) {
        return;
    }

    statePostMatch_ProcessReturn(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// statePostMatch::OnEnter  [vtable slot 11 @ 0x8210C368] | size: 0x1C4
// Enters the post-match state. Calls the base OnEnter (stateServeBounce_vfn_11),
// clears the exit flags, determines whether it's a network match (mode 4 or 5),
// sends match-end page group messages, and configures per-player state blocks.
// ─────────────────────────────────────────────────────────────────────────────
void statePostMatch_OnEnter(void* self) { // @ 0x8210C368
    // Call base OnEnter with param=0
    void* origR3 = self;
    // Reuse self as r3, r4=0 for stateServeBounce_vfn_11
    stateServeBounce_OnEnter(self, 0);

    // Clear exit flags
    char* state = (char*)origR3;
    *(uint8_t*)(state + 16) = 0;
    *(uint8_t*)(state + 17) = 0;

    // Check network game mode from g_input_obj_ptr
    void* inputObj = g_input_obj_ptr;
    int gameMode = *(int*)((char*)inputObj + 8);

    // Determine if this is a network match (mode 4 or 5)
    bool isNetworkMatch = (gameMode == 4) || (gameMode == 5);

    // Send match completion message (2130) to page group system
    PostPageGroupMessage(2130, 64, 0, nullptr);

    // Re-read game mode for post-match logic
    void* inputObj2 = g_input_obj_ptr;
    int gameMode2 = *(int*)((char*)inputObj2 + 8);

    // Check if mode 4 or 5 (network modes)
    bool isNetworkMode = (gameMode2 == 4) || (gameMode2 == 5);

    if (!isNetworkMode) {
        // Check if mode 3 (local exhibition match)
        bool isExhibition = (gameMode2 == 3);

        if (isExhibition) {
            // For exhibition mode, check tournament progress
            void* renderObj = g_render_obj_ptr;
            int currentRound = *(int*)((char*)renderObj + 28);
            int totalRounds = *(int*)((char*)renderObj + 32);

            // Only show match stats if not the final round winner
            if (currentRound <= totalRounds) {
                isExhibition = false;
            }
        }

        if (!isExhibition) {
            // Non-network, non-final-exhibition: show match stats
            isNetworkMode = true;
        }
    }

    // Configure per-player result fields in the match context
    // inputObj2+25 = show result banner flag
    // inputObj2+24 = result submitted flag  
    // inputObj2+48 = post-match UI mode
    *(uint8_t*)((char*)inputObj2 + 25) = isNetworkMode ? 0 : 1;
    *(uint8_t*)((char*)inputObj2 + 24) = 0;

    if (isNetworkMatch) {
        // Network match: set UI mode to 4 (network post-match)
        *(uint32_t*)((char*)inputObj2 + 48) = 4;
    } else {
        // Local/offline match: set UI mode to 1 (local post-match)
        *(uint32_t*)((char*)inputObj2 + 48) = 1;
    }

    // Send post-match enter message (8198) to page group
    PostPageGroupMessage(8198, 64, 0, nullptr);

    // Check network session state
    void* networkState = g_pNetworkStateFlags;
    uint8_t networkMode = *(uint8_t*)((char*)networkState + 8);

    if (networkMode == 0) {
        // Offline mode: also send spectator dismiss message (10253)
        PostPageGroupMessage(10253, 64, 0, nullptr);
    }

    // Configure per-player data blocks
    // Block 1 at g_playerDataArray2 + 260/261
    g_playerDataArray2[261] = 0;    // clear secondary flag
    g_playerDataArray2[260] = 1;    // set primary flag

    // Block 2 at g_playerDataArray2 + 676/677
    g_playerDataArray2[677] = 0;    // clear secondary flag
    g_playerDataArray2[676] = 1;    // set primary flag
}

// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSinglesMatch::stateHelpTips — HSM state for in-match help tip display
//
// Vtable @ 0x8203131C. Inherits from hsmState.
// State struct layout:
//   +0x00: void** vtable
//   +0x04: void*  m_pHsmContext     — owning HSM context
//   +0x08: void*  m_pParentLogic    — owning gmLogicSinglesMatch instance
//   +0x10: uint8_t m_bTipChanged    — set when a new tip is queued
//   +0x11: uint8_t m_bDismissed     — set after dismiss message sent
// ═════════════════════════════════════════════════════════════════════════════

// External function for stateHelpTips
extern "C" {
    void stateHelpTips_ShowTip(void* self);  // stateHelpTips_AA50 @ 0x8210AA50
}

// ─────────────────────────────────────────────────────────────────────────────
// stateHelpTips::GetName  [vtable slot 13 @ 0x8210AAE8] | size: 0x0C
// Returns the state name string for HSM debugging/identification.
// ─────────────────────────────────────────────────────────────────────────────
const char* stateHelpTips_GetName(void* /*self*/) { // @ 0x8210AAE8
    return "HelpTips";
}

// ─────────────────────────────────────────────────────────────────────────────
// stateHelpTips::OnLeave  [vtable slot 14 @ 0x8210AA40] | size: 0x0C
// Clears the dismissed flag when leaving the state.
// ─────────────────────────────────────────────────────────────────────────────
void stateHelpTips_OnLeave(void* self) { // @ 0x8210AA40
    *(uint8_t*)((char*)self + 17) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// stateHelpTips::OnEnter  [vtable slot 11 @ 0x8210A8E8] | size: 0x44
// Enters the help tips state. Calls base OnEnter, clears both flags,
// then immediately shows the first tip.
// ─────────────────────────────────────────────────────────────────────────────
void stateHelpTips_OnEnter(void* self) { // @ 0x8210A8E8
    // Call base OnEnter with param=0
    stateServeBounce_OnEnter(self, 0);

    // Clear flags
    *(uint8_t*)((char*)self + 17) = 0;
    *(uint8_t*)((char*)self + 16) = 0;

    // Show the initial tip
    stateHelpTips_ShowTip(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// stateHelpTips::Update  [vtable slot 10 @ 0x8210A930] | size: 0x7C
// Called each frame. If m_bTipChanged is set, refreshes the tip display
// and clears the flag. Otherwise, if not yet dismissed, sends the dismiss
// message (2138) and sets the dismissed flag.
// ─────────────────────────────────────────────────────────────────────────────
void stateHelpTips_Update(void* self) { // @ 0x8210A930
    char* state = (char*)self;

    // Check if a new tip was queued
    uint8_t tipChanged = *(uint8_t*)(state + 16);
    if (tipChanged != 0) {
        // Refresh the tip display
        stateHelpTips_ShowTip(self);
        *(uint8_t*)(state + 16) = 0;
        return;
    }

    // If already dismissed, nothing more to do
    uint8_t dismissed = *(uint8_t*)(state + 17);
    if (dismissed != 0) {
        return;
    }

    // Send dismiss help tips message (2138) to page group system
    PostPageGroupMessage(2138, 64, 0, nullptr);

    // Mark as dismissed
    *(uint8_t*)(state + 17) = 1;
}

// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSinglesMatch::stateAnticipationCam — HSM state for pre-serve camera
//
// Vtable @ 0x82030E8C. Inherits from hsmState.
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// stateAnticipationCam::GetName  [vtable slot 13 @ 0x82107708] | size: 0x0C
// Returns the state name string for HSM debugging/identification.
// ─────────────────────────────────────────────────────────────────────────────
const char* stateAnticipationCam_GetName(void* /*self*/) { // @ 0x82107708
    return "AnticipationCam";
}


// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSinglesMatch — Singles match state machine
//
// Anchored by the state-transition log string
//   "gmLogicSinglesMatch's next state was %s, is %s"
// and by
//   "gmLogicSinglesMatch::MidGame(): Unknown PointsPerGame enum value = %d"
//
// The class's state family (from RTTI typeinfos) spans 33 sub-states, grouped
// into: setup (stateInit, statePreMatchSync, statePreMatch, statePreGame,
// stateAnticipationCam, statePrePreServe, statePreServeSpectatorSyncState,
// stateServeBounce, stateServe, ...), mid-game (stateReplay, stateReplayNego-
// tiation(PreSync), statePostPoint, statePostPointCS, statePostGame), and
// closure (statePostMatch, statePostTourney, statePostTourneyMovie,
// statePostPostGame, stateSaveDialog, stateUnlockDialog). All substates
// inherit from hsmState and live in the vtable @ 0x82030CC4 family.
//
// Struct layout (fields actually touched by the vfns below):
//   +0x04  uint32_t  m_flags               (or state count?)
//   +0x08  void**    m_stateArray           (array of hsmState*)
//   +0x0c  int32_t   m_currentStateIndex    (-1 == none)
//   +0x1c  float     m_elapsedTotal         (vfn_11 accumulator)
//   +0x20  uint8_t   m_updateReplayTimer
//   +0x24  float     m_elapsedReplay
//   +0x28  float     m_elapsedBonus         (only if state==3)
//   +0x30  void*     m_pNetworkClient       (nullable)
//   +0x34  uint8_t   m_isTrainingDrill      (enables substate 21)
//   +0x36  uint8_t   m_bFlagB
//   +0x38  uint32_t  m_someCounter
// ═════════════════════════════════════════════════════════════════════════════

// Named externs for the match log/error strings (documented anchors)
extern char g_str_gmLogicSinglesMatch_nextState[];       // "gmLogicSinglesMatch's next state was %s, is %s"
extern char g_str_gmLogicSinglesMatch_MidGameBadEnum[];  // "gmLogicSinglesMatch::MidGame(): Unknown PointsPerGame enum value = %d"
extern char g_str_gmLogicSinglesMatch_replayOnEnter[];   // "gmLogicSinglesMatch::stateReplay::onEnterState"

// External helpers referenced by the vfns below (real PPC recomp names)
extern "C" {
    void  singles_Timer_58E0_h(void* self);                 // +0x821058E0
    void  singles_5860(void* self);                         // +0x82105860 (base_onExit chain)
    void  singles_5B70(void* self);                         // +0x82105B70 (base_onEnter chain)
    void  singles_5D38_dispatch(void* self);                // +0x82105D38
    void* SinglesNetworkClient_25D0_v12(void* netClient);   // +0x823D25D0 — network tick
    void  PostGlobalMessage(int msgId, int mask, int argc, void* args); // pg_E6E0 @ 0x8225E6E0
    void  hsmContext_SetNextState(void* self, int stateId);
    uint8_t IsNetworkActiveAndNotDisconnected();            // pg_10E0_g @ 0x821010E0
    bool  CanDispatchTrainingEvent_21(void* self, int evtId); // atSingleton_67E0_g @ 0x821067E0
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::vfn_10  [slot 10 @ 0x82105AA8] | size: 0x4C
// Mini-destructor / reset hook. Chains base reset + timer reset + clears
// training-drill flags and counters (+44, +54, +56).
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSinglesMatch_vfn_10(void* self) { // @ 0x82105AA8
    singles_5860(self);                     // base deinit
    singles_5B70(self);                     // timer/serve reset
    singles_Timer_58E0_h(self);             // secondary timer reset
    *(uint8_t*)((char*)self + 44) = 0;
    *(uint8_t*)((char*)self + 54) = 0;
    *(uint32_t*)((char*)self + 56) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::vfn_12  [slot 12 @ 0x821057C0] | size: 0x9C
// Per-frame dispatch. If a network client is attached at +0x30, tick it and,
// if its session is positive, run the internal dispatch chain. Also gates a
// "state 21" (training drill) event on m_isTrainingDrill.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSinglesMatch_vfn_12(void* self) { // @ 0x821057C0
    void* netClient = *(void**)((char*)self + 48);
    if (netClient != nullptr) {
        // The game returns vtable@0x82030D60 via the v12 helper, effectively
        // (char*)netClient + 0x0D60 — a subobject handled by SinglesNetwork-
        // Client_25D0_v12. Treat as "if tick returned > 0, run dispatch".
        SinglesNetworkClient_25D0_v12((char*)netClient + 3424);
        int32_t tickResult = *(int32_t*)((char*)netClient + 3424);
        if (tickResult > 0) {
            singles_5D38_dispatch(self);
        }
    }

    // Training-drill pipe: +0x34 flag, sub-state index at +0x0C must be [1,19]
    if (*(uint8_t*)((char*)self + 52) != 0) {
        int32_t idx = *(int32_t*)((char*)self + 12);
        if (idx >= 1 && idx < 20) {
            CanDispatchTrainingEvent_21(self, 21);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::vfn_13  [slot 13 @ 0x82105AF8] | size: 0x78
// CanTransitionTo(stateId). Rejects out-of-range ids, fast-paths the training-
// drill virtual state (21), then delegates to the current sub-state's
// vtable slot 15 (CanTransitionTo). Accepts state 3 when no sub-state set.
// ─────────────────────────────────────────────────────────────────────────────
bool gmLogicSinglesMatch_vfn_13(void* self, int stateId) { // @ 0x82105AF8
    int32_t stateCount = *(int32_t*)((char*)self + 4);
    if (stateId >= stateCount) {
        return false;
    }

    // Training-drill virtual state
    if (*(uint8_t*)((char*)self + 52) != 0 && stateId == 21) {
        return true;
    }

    int32_t curIdx = *(int32_t*)((char*)self + 12);
    if (curIdx != -1) {
        void** stateArray = *(void***)((char*)self + 8);
        void* curState = stateArray[curIdx];
        if (curState != nullptr) {
            void** vt = *(void***)curState;
            typedef bool (*CanTransFn)(void*, int);
            return ((CanTransFn)vt[15])(curState, stateId);
        }
    }

    // No active state: only allow transition to state 3 (stateAnticipationCam)
    return stateId == 3;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::vfn_20  [slot 20 @ 0x82106788] | size: 0x58
// Teardown / "force exit" hook. If a network client is attached, posts
// event 14372 (MSG_NETWORK_FORCE_EXIT) and nulls the +0x30 pointer.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSinglesMatch_vfn_20(void* self) { // @ 0x82106788
    if (*(void**)((char*)self + 48) != nullptr) {
        uint32_t arg = 1;
        PostGlobalMessage(14372, 0, 1, &arg);
        *(void**)((char*)self + 48) = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::stateReplay::onEnterState  @ 0x8210AFD0 | size: 0x168
// Anchored by the log string "gmLogicSinglesMatch::stateReplay::onEnterState".
//
// Enters replay mode. Steps:
//   1. Chain to stateServeBounce_vfn_11 (base onEnter).
//   2. Post global message 2128 (MSG_ENTER_REPLAY) to world channel 64.
//   3. Peek flag at [match-context + 29] — if set, hot-path:
//        a. Pull replay-buffer pointer from the global match context;
//        b. If flags A/B at fixed bit-offsets are both clear, look up and
//           activate the "replay player" actor (via game_76A8);
//        c. If flag C is set, copy a pointer (+272 in match) into the
//           replay slot and invoke util_CE00 (replay prep / playback init);
//        d. If flag D is set, mark replay session "active" (write 1 into
//           replay+36084);
//        e. Set the HSM "is-replay-running" byte in the match struct (+37)
//           and the state-local "entered" byte (this+16).
//      Cold path: clear (this+16) and call stateReplay_B320 (replay fail-
//      over / bail-out which drops state to stateServeBounce).
//
// In addition to +16, zeroes three status bytes at this+17/18/19 (replay
// timers / scrub flags) and sets the class-wide "replay active" flag at
// the singleton (lbl_82104650 + 26224 <- 0).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    void stateServeBounce_vfn_11_base(void* self);      // 0x82106A60
    void ReplayActor_Activate(void* actor, void* slot); // game_76A8 @ 0x821676A8
    void ReplayInit_CE00(void* replaySlot);             // util_CE00 @ 0x821CCE00
    void stateReplay_B320_fallback(void* self);         // @ 0x8210B320
    void game_3860_unknown(void);                       // @ 0x821D3860

    // Matchcontext / replay globals (reconstructed from the lis/addi pairs)
    extern void* g_pActiveMatchContext;  // derived from r27 (-23812), +29 = flag
    extern char  g_ReplayActive;         // r10 (-32160) + 26224
    extern void* g_pReplayStateData;     // r28 (-23828) — base of replay slot
    extern void* g_pHsmContextCurrent;   // -23772
}

void gmLogicSinglesMatch_stateReplay_OnEnter(void* self) { // @ 0x8210AFD0
    stateServeBounce_vfn_11_base(self);

    // Broadcast: "replay entered" to world (group mask 64).
    PostGlobalMessage(2128, 64, 0, nullptr);

    void* matchCtx = g_pActiveMatchContext;
    uint8_t flagEntry = *((uint8_t*)matchCtx + 29);

    // Local status bytes on this state
    *((uint8_t*)self + 17) = 0;
    *((uint8_t*)self + 18) = 0;
    *((uint8_t*)self + 19) = 0;

    // Class-wide flag: "replay active" pushed to false before re-arming
    g_ReplayActive = 0;
    game_3860_unknown();

    if (flagEntry == 0) {
        // Cold path — no replay buffer ready; bail to service fallback.
        *((uint8_t*)self + 16) = 0;
        stateReplay_B320_fallback(self);
        return;
    }

    // Hot path — replay buffer ready.
    char* replaySlot = (char*)g_pReplayStateData;

    // Probe flags A (byte +36091) and B (byte +36094) in the replay slot.
    if (replaySlot[36091] == 0) {
        if (replaySlot[36094] == 0) {
            void* actorPtr = *(void**)(replaySlot + 36112);
            if (actorPtr != nullptr) {
                uint16_t actorIndex = *(uint16_t*)((char*)actorPtr + 4);
                // (actorIndex*16) walk into a per-actor table reachable via
                // g_replayActorBase -> slot +44 -> table[0] + (idx<<4) - 16
                void* table   = /* g_replayActorBase->slot44 */
                    *(void**)(*(char**)(*(char**)((char*)&g_ReplayActive + 25628)) + 44);
                void* dataSlot = (char*)(*(void**)((char*)table + 0))
                                 + ((uint32_t)actorIndex << 4) - 16;
                ReplayActor_Activate(actorPtr, dataSlot);
            }
        }
        replaySlot = (char*)g_pReplayStateData;
    }

    // Flag C — replay owns a match-side pointer; copy HSM context+272 in.
    if (replaySlot[36092] != 0) {
        void* hsmCtx = g_pHsmContextCurrent;
        *(void**)(replaySlot + 36096) = *(void**)((char*)hsmCtx + 272);
    }

    // Final init
    ReplayInit_CE00(replaySlot);

    // Flag D — arm replay "active" byte
    if (replaySlot[36089] != 0) {
        *(uint32_t*)(replaySlot + 36084) = 1;
    }

    // Mark both local and match-wide "replay running" flags
    *((uint8_t*)self + 16) = 1;
    *((uint8_t*)matchCtx + 37) = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch::MidGame  @ 0x8238CA38 | size: 0x10 (thunk)
//
// Tiny dispatch thunk — the strings index points at the typeinfo that fires
// the error log "MidGame(): Unknown PointsPerGame enum value = %d". The real
// body is a tail-call to a jump table over m_PointsPerGame (0..N-1).
// Shape observed in similar 0x10-byte vfn thunks across this codebase:
//     r11 = this->vtable;
//     r3  = this + 8;        // private "match state" context
//     mtctr [r11 + <slot>];
//     bctr
// Documented here as a tail-call. The out-of-range case hits the error str.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSinglesMatch_MidGame(void* self) { // @ 0x8238CA38
    // Tail-call through vtable into the PointsPerGame jump-table impl.
    // Default (out-of-range) slot logs g_str_gmLogicSinglesMatch_MidGameBadEnum.
    void** vt = *(void***)self;
    typedef void (*MidGameImplFn)(void*);
    ((MidGameImplFn)vt[/*slot filled by the dispatcher*/ 0])((char*)self + 8);
}


// ═════════════════════════════════════════════════════════════════════════════
// gmLogicSwingPractice — single-player swing practice mode
//
// Vtable @ 0x820303F4. Hierarchical state machine with 8 RTTI-tagged states:
//   statePreInit, stateInit, stateMainMenu, stateAim, stateShoot,
//   stateAwaitReturnHit, stateReturnHit, statePostPoint
//
// Key object layout (confirmed via callee offsets):
//   +8    sub-object holding gmPracticeTarget pointer @ +152, flags @ +88..+89
//   +12   float parameter (aim arc / shot power), sampled via cmSampleCamActions
//   +16   int sub-state (-1 = none)
//   +84   current HSM state index (valid range 0..10)
//   +88   flag: whether target is armed
//   +148  locomotion state anim controller
//   +152  gmPracticeTarget sub-struct (fields: +22, +23, +96 pos, +100 latch)
//   +156  "awaiting confirmation" byte flag
//
// Method dispatch pattern: states live at this+40..this+80 (slots 10..20) and
// are invoked via `lwzx r3, r11, this` with a computed slot offset. Each state
// slot holds a vtable pointer; slots 3/6/8/9/16 in the state vtable are:
//   3  → Init / arm      6  → OnTick       8  → OnMessage
//   9  → OnEnterState    16 → OnChildStateMessage
// ═════════════════════════════════════════════════════════════════════════════

// Anchor: "gmLogicSwingPractice::stateAim::onEnterState" @ 0x820303B8
extern const char* g_str_hsm_gmLogicSwingPractice_stateAim_onEnterState;

// Per-mode globals identified via string xrefs
static uint8_t* const g_SwingPracticeTargetDirty = /* 0x8205A37A */ (uint8_t*)0x8205A37A;
static const float* const g_SwingPractice_AimDefaults = /* 0x8202D108 */ (const float*)0x8202D108;

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::~gmLogicSwingPractice  [vtable slot 0 @ 0x82101F80]
// Destructor — delegates to atSingleton teardown, then frees if owned (bit 0).
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_Destructor(void* self, uint32_t flags) { // @ 0x82101f80
    atSingleton_Destructor(self);
    if (flags & 1) {
        rage_free(self);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::ResetAim  [vtable slot 10 @ 0x82102A58]
// Called on return to aim state. Clears the target latch, re-arms the target,
// clears the "was hit" flag, and resets the HSM state index to -1.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_ResetAim(void* self) { // @ 0x82102a58
    void* target = *(void**)((char*)self + 152);
    *((uint8_t*)target + 100) = 0;   // clear "hit" latch
    *((uint8_t*)target + 23)  = 1;   // arm
    void* target2 = *(void**)((char*)self + 152);
    *((uint8_t*)target2 + 22) = 0;   // clear "served" flag

    // If the "awaiting confirmation" flag is set, leave state index alone.
    if (*((uint8_t*)self + 156) != 0) {
        return;
    }
    *(int32_t*)((char*)self + 84) = -1;
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::Tick  [vtable slot 11 @ 0x82102A90]
// Per-frame update: advances aim-target position, ticks locomotion, then
// forwards OnTick (state vtable slot 6) to the active child state.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_Tick(void* self) { // @ 0x82102a90
    // If aim input is active and target exists and is not latched…
    if (*((uint8_t*)self + 88) != 0) {
        void* target = *(void**)((char*)self + 152);
        if (target != nullptr && *((uint8_t*)target + 100) == 0) {
            // Integrate aim position by (default step * frame time)
            float step     = g_SwingPractice_AimDefaults[2];   // [8]
            float curPos   = *(float*)((char*)target + 96);
            float frameDt  = rage_FrameTimeSeconds;           // -16340(-32253)
            float newPos   = step * frameDt + curPos;
            *(float*)((char*)target + 96) = newPos;

            // Clamp via cmSampleCamActions to valid aim arc
            newPos = cmSampleCamActions_Clamp(newPos, /*min*/ -0.0f, /*max*/ +0.0f);
            *(float*)((char*)target + 96) = newPos;
        }
    }

    // Tick locomotion/anim controller if present
    void* locomotion = *(void**)((char*)self + 148);
    if (locomotion != nullptr) {
        LocomotionStateAnim_Tick(locomotion);
    }

    // Dispatch OnTick (slot 6) to current child state if index is valid.
    int32_t stateIdx = *(int32_t*)((char*)self + 84);
    if (stateIdx > -1 && stateIdx < 11) {
        void** stateSlot = (void**)((char*)self + (stateIdx + 10) * 4);
        void* state = *stateSlot;
        VCALL_SLOT(state, 6);  // OnTick
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::GetSubState  [vtable slot 13 @ 0x82102B60]
// Returns the vtable-dispatched sub-state object by index, or a default
// 1-vs-(not-1) flag if the sub-state isn't instantiated.
// Layout of (this): +4 = count, +8 = state-array base, +12 = active-idx.
// ─────────────────────────────────────────────────────────────────────────────
void* gmLogicSwingPractice_GetSubState(void* self, int32_t idx) { // @ 0x82102b60
    int32_t count = *(int32_t*)((char*)self + 4);
    if (idx >= count) {
        return nullptr;
    }
    int32_t activeIdx = *(int32_t*)((char*)self + 12);
    if (activeIdx != -1) {
        void** arr = *(void***)((char*)self + 8);
        void* state = arr[activeIdx];
        if (state != nullptr) {
            // Tail-call slot 15 (offset +60 in state vtable)
            return VCALL_SLOT_RET(state, 15);
        }
    }
    // Fallback: return (idx == 1) as a boolean-style flag
    return (void*)(uintptr_t)(idx == 1 ? 1u : 0u);
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::OnChildExit  [vtable slot 15 @ 0x82102BC0]
// Forwards child-state exit notification to the nested state (vtable slot 2
// at offset +8). No-op if the owning sub-struct or its target is null.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_OnChildExit(void* self) { // @ 0x82102bc0
    if (*((uint8_t*)self + 88) == 0) return;
    void* target = *(void**)((char*)self + 152);
    if (target == nullptr) return;
    VCALL_SLOT(target, 2);  // OnChildExit on practice target
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::OnMessage  [vtable slot 18 @ 0x82102BF0]
// Central message router. Translates HSM messages:
//   msg 0x500C (20492) → "begin awaiting confirmation": set +156=1, call vfn_3
//   msg 0x500D (20493) → "cancel awaiting":             clear +156
//   otherwise          → forward to current sub-state (vfn_16) and current
//                        HSM state (slot 8 = OnMessage)
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_OnMessage(void* self, const void* msg) { // @ 0x82102bf0
    int32_t activeIdx = *(int32_t*)((char*)self + 12);
    void* subState = nullptr;
    if (activeIdx != -1) {
        void** arr = *(void***)((char*)self + 8);
        subState = arr[activeIdx];
    }

    uint16_t msgId = *(const uint16_t*)msg;

    if (msgId == 20492) {
        // "Await confirmation" start — gate the aim state & invoke self-vfn_3
        int32_t stateIdx = *(int32_t*)((char*)self + 84);
        if (stateIdx > -1 && stateIdx < 11) {
            *((uint8_t*)self + 156) = 1;
            VCALL_SLOT(self, 3);  // self.OnAwaitConfirmStart
        }
        return;
    }

    if (msgId == 20493) {
        // "Await confirmation" cancel
        *((uint8_t*)self + 156) = 0;
        return;
    }

    // General case: first forward to sub-state's vfn_16 unless it already has
    // a pending response (+16 != -1), then forward OnMessage to current HSM.
    if (subState != nullptr) {
        int32_t pending = *(int32_t*)((char*)self + 16);
        if (pending == -1) {
            VCALL_SLOT_ARG(subState, 16, msg);
        }
    }
    int32_t stateIdx = *(int32_t*)((char*)self + 84);
    if (stateIdx > -1 && stateIdx < 11) {
        void** stateSlot = (void**)((char*)self + (stateIdx + 10) * 4);
        VCALL_SLOT_ARG(*stateSlot, 8, msg);   // state.OnMessage
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gmLogicSwingPractice::stateAim::onEnterState  [@ 0x82103878, size 0x15C]
// String anchor: "gmLogicSwingPractice::stateAim::onEnterState" @ 0x820303B8
//
// Enters the aim state. Resets aim parameter (+12) from defaults, clears
// "shot fired" (+16), registers the aim camera, and if a target-update is
// pending, refreshes the practice-target meter and clears the dirty flag.
// ─────────────────────────────────────────────────────────────────────────────
void gmLogicSwingPractice_stateAim_OnEnterState(void* self) { // @ 0x82103878
    float aimDefault = g_SwingPractice_AimDefaults[2];         // [8]
    *(float*)((char*)self + 12) = aimDefault;
    *((uint8_t*)self + 16) = 0;

    // Log entry (hooked HSM log)
    game_3860(g_HsmLogObject);

    // Post "enter-aim" page-group message (id 2108, param 64)
    PostPageGroupMessage(2108, 64, 0, nullptr);

    // If a global target-dirty flag is set, rewrite meter & clear flag.
    if (*g_SwingPracticeTargetDirty != 0) {
        void* owner = *(void**)((char*)self + 8);
        void* target = *(void**)((char*)owner + 152);
        *((uint8_t*)target + 100) = 0;   // clear latch
        *((uint8_t*)target + 23)  = 1;   // arm
        void* owner2 = *(void**)((char*)self + 8);
        void* target2 = *(void**)((char*)owner2 + 152);
        gmPracticeTarget_RefreshMeter(target2);
        *g_SwingPracticeTargetDirty = 0;
    }

    // If target + camera pose are valid, register a camera pose event.
    void* owner3 = *(void**)((char*)self + 8);
    if (*((uint8_t*)owner3 + 89) != 0) {
        float px = *(float*)((char*)owner3 + 112);
        float py = *(float*)((char*)owner3 + 116);
        float pz = *(float*)((char*)owner3 + 120);
        PostPageGroupMessage(4119, 1, /*strRef*/ 0x8202D594, px, py, pz);
    }

    // Always post the aim-hint message using default aim params.
    float paramA = g_SwingPractice_AimDefaults[0];
    PostPageGroupMessage(41, 128, /*strRef*/ 0x8202D500, paramA, aimDefault);

    // Delegate to the parent HSM state if current index is valid (slot 9).
    void* owner4 = *(void**)((char*)self + 8);
    int32_t stateIdx = *(int32_t*)((char*)owner4 + 84);
    if (stateIdx > -1 && stateIdx < 11) {
        void** stateSlot = (void**)((char*)owner4 + (stateIdx + 10) * 4);
        VCALL_SLOT(*stateSlot, 9);  // parent.OnEnterState
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// gdaiMeterLogic — AI node: reads the meter (power bar) globals
//
// Vtable @ 0x82041E84. Probes three named globals against `other`:
//   +24756 @ 0x825D3DB4 : live meter ptr
//   +(-32708) @ 0x825CFBFC, +(-32712) @ 0x825CFBF8 : alternates
// Used by AI to decide "is this the target the meter is tracking?"
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// gdaiMeterLogic::IsSameTarget  [vtable slot 20 @ 0x821E91C0]
// Returns 1 if `other` matches any of 3 well-known meter-target globals.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t gdaiMeterLogic_IsSameTarget(void* /*self*/, void* other) { // @ 0x821e91c0
    void* meterA = *(void**)0x825D3DB4;
    if (other == meterA) return 1;
    void* meterB = *(void**)0x825CFBFC;
    if (other == meterB) return 1;
    void* meterC = *(void**)0x825CFBF8;
    return (other == meterC) ? 1 : 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// gdaiMeterLogic::RegisterSerializationFields  [vtable slot 21 @ 0x821E92B0]
// Registers 5 serialization fields (offsets +16/+20/+24/+28/+32) against the
// AI-logic schema object @ 0x825CAF94. Field names live in .rdata @ various
// offsets of lbl_82050E... (decode to "MinHits", "MaxHits", "MinPower", etc.).
// ─────────────────────────────────────────────────────────────────────────────
void gdaiMeterLogic_RegisterSerializationFields(void* self) { // @ 0x821e92b0
    void* schema = (void*)0x825CAF94;
    RegisterSerializationField(self, (const char*)0x82050E8C, (char*)self + 16, schema, 0);
    RegisterSerializationField(self, (const char*)0x82050F0C, (char*)self + 20, schema, 0);
    RegisterSerializationField(self, (const char*)0x82050F18, (char*)self + 24, schema, 0);
    RegisterSerializationField(self, (const char*)0x82050F28, (char*)self + 28, schema, 0);
    RegisterSerializationField(self, (const char*)0x82050E98, (char*)self + 32, schema, 0);
}

// ═════════════════════════════════════════════════════════════════════════════
// gdaiComeBackLogic::BuildShotLists  [vtable slot 3 @ 0x821EA1B8]
//
// Scans the linked-list of candidate shots hanging off (this+12) and partitions
// them into two buckets by calling `slot 20` (IsClassified) on each with two
// different class tags stored at globals:
//   g_AIClass_A @ 0x825D3E18  (weak/defensive shot)
//   g_AIClass_B @ 0x825D3E14  (aggressive/return shot)
//
// Result layout inside `this`:
//   +16 : array handle for class-B hits (size = var_r28)
//   +24 : array handle for class-A hits (size = var_r25)
// Each element also writes back its own index into entry+28.
// Entries that match NEITHER get logged via nop_8240E6D0 with their slot-19
// name string (debug trace).
// ═════════════════════════════════════════════════════════════════════════════
void gdaiComeBackLogic_BuildShotLists(void* self) { // @ 0x821ea1b8
    uint16_t countA = 0, countB = 0;
    void* classA = *(void**)0x825D3E18;
    void* classB = *(void**)0x825D3E14;

    // Pass 1: count classifications and log unknowns.
    void* node = *(void**)((char*)self + 12);
    while (node != nullptr) {
        if (VCALL_SLOT_ARG_RET_U8(node, 20, classA)) {
            countA++;
        } else if (VCALL_SLOT_ARG_RET_U8(node, 20, classB)) {
            countB++;
        } else {
            const char* name = (const char*)VCALL_SLOT_RET(node, 19);
            nop_8240E6D0((const char*)0x8203F3C8, name);  // "leAnim%s" fmt
        }
        node = *(void**)((char*)node + 8);
    }

    // Allocate/clear result arrays.
    void* arrB = (char*)self + 16;
    if (countB != 0) {
        game_DB80(arrB, countB);
    } else {
        *(uint32_t*)arrB = 0;
        *((uint16_t*)arrB + 3) = 0;
    }

    void* arrA = (char*)self + 24;
    if (countA != 0) {
        game_DB80(arrA, countA);
    } else {
        *(uint32_t*)arrA = 0;
        *((uint16_t*)arrA + 3) = 0;
    }

    // Pass 2: populate the arrays in the original list order.
    uint32_t idxA = 0, idxB = 0;
    node = *(void**)((char*)self + 12);
    while (node != nullptr) {
        if (VCALL_SLOT_ARG_RET_U8(node, 20, classA)) {
            // Append to class-A bucket (+24)
            uint16_t slot = *((uint16_t*)arrA + 2);   // write cursor
            void** base   = *(void***)arrA;
            *((uint16_t*)arrA + 2) = slot + 1;
            base[slot] = node;
            *(uint32_t*)((char*)node + 28) = idxA++;
        } else if (VCALL_SLOT_ARG_RET_U8(node, 20, classB)) {
            // Append to class-B bucket (+16)
            uint16_t slot = *((uint16_t*)arrB + 2);
            void** base   = *(void***)arrB;
            *((uint16_t*)arrB + 2) = slot + 1;
            base[slot] = node;
            *(uint32_t*)((char*)node + 28) = idxB++;
        }
        node = *(void**)((char*)node + 8);
    }
}


// ============================================================================
// Match state stubs — moved from stubs.cpp
// ============================================================================

/**
 * game_3860 @ 0x82483860 | size: 0x144
 * Match state setup with string lookup — initializes a match state
 * context with the given parameters and name string.
 * TODO: Full lift (324 bytes).
 */
void game_3860(void* matchState, int stateId, const char* stateName) {
    (void)matchState; (void)stateId; (void)stateName;
}

