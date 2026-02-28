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
