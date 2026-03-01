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
