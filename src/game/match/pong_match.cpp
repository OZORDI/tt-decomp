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
