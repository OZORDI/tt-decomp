/**
 * gdVibEvent - Vibration Event System Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "game/gd_vib_event.hpp"

// External dependencies
extern "C" void rage_free_00C0(void* ptr);
extern "C" void SinglesNetworkClient_GetPlayerID_E408(void* client, uint32_t param);
extern "C" void game_F480(void* controller, void* vibPattern, void* vibData);
extern "C" void atSingleton_9420(void* obj);

// Global arrays
void* g_controllerArray = nullptr;
void* g_playerStateArray = nullptr;

/**
 * gdVibEvent::~gdVibEvent
 * @ 0x8220F8B0 | size: 0x64
 * 
 * Virtual destructor for vibration events.
 * Frees vibration data buffers and calls singleton cleanup.
 */
gdVibEvent::~gdVibEvent() {
    // Free vibration data 2
    if (m_vibrationData2 != nullptr) {
        rage_free_00C0(m_vibrationData2);
    }
    
    // Free vibration data 1
    if (m_vibrationData1 != nullptr) {
        rage_free_00C0(m_vibrationData1);
    }
    
    // Call singleton cleanup
    atSingleton_9420(this);
    
    // Note: Memory freeing handled by caller based on destructor flags
    // If bit 0 is set, rage_free_00C0(this) is called
}

/**
 * gdVibEvent::MatchesType
 * @ 0x8220FA50 | size: 0x58
 * 
 * Check if this event matches the given event type.
 */
bool gdVibEvent::MatchesType(uint32_t eventType) {
    // TODO: Implement event type matching
    return false;
}

/**
 * gdVibEvent::Process
 * @ 0x8220FAA8 | size: 0x6C
 * 
 * Process the vibration event.
 */
void gdVibEvent::Process() {
    // TODO: Implement event processing
}

/**
 * gdVibEvent::Stub22
 * @ 0x8220F9B0 | size: 0xC
 * 
 * Stub virtual function (slot 22).
 */
void gdVibEvent::Stub22() {
    // Empty stub
}

/**
 * gdVibEvent::TriggerVibration
 * @ 0x8220F918 | size: 0x94
 * 
 * Trigger controller vibration for the appropriate player.
 * Checks if vibration pattern exists and if player is local before applying.
 */
void gdVibEvent::TriggerVibration(void* playerContext) {
    // Check if vibration pattern is set
    if (m_vibrationPattern == 0) {
        return;
    }
    
    // Get player ID from context
    // playerContext is passed as r4, we need to get player ID
    uint32_t playerID;
    SinglesNetworkClient_GetPlayerID_E408(playerContext, 1);
    
    // playerID is returned in r3
    // Calculate array index: playerID + 14
    uint32_t arrayIndex = playerID + 14;
    uint32_t offset = arrayIndex * 4;  // 4 bytes per entry
    
    // Load player state array base
    uint32_t** playerStateBase = reinterpret_cast<uint32_t**>(
        reinterpret_cast<uintptr_t>(&g_playerStateArray) - 21720
    );
    
    // Check if player is local (state == 1)
    uint32_t* playerStates = *playerStateBase;
    uint32_t playerState = playerStates[arrayIndex];
    
    if (playerState != 1) {
        return;  // Not a local player
    }
    
    // Get controller for this player
    // Controller index = playerID + 1
    uint32_t controllerIndex = playerID + 1;
    uint32_t controllerOffset = controllerIndex * 4;
    
    // Load controller array base
    uint32_t** controllerBase = reinterpret_cast<uint32_t**>(
        reinterpret_cast<uintptr_t>(&g_controllerArray) - 23640
    );
    uint32_t* controllers = *controllerBase;
    
    // Get controller pointer array base
    uint32_t** controllerPtrBase = reinterpret_cast<uint32_t**>(
        reinterpret_cast<uintptr_t>(&g_controllerArray) - 23780
    );
    uint32_t* controllerPtrs = *controllerPtrBase;
    
    // Get controller pointer
    void* controller = reinterpret_cast<void*>(controllerPtrs[controllerIndex]);
    
    // Trigger vibration
    game_F480(controllers, controller, reinterpret_cast<void*>(m_vibrationPattern));
}

/**
 * gdVibEvent::PostLoadProperties
 * @ 0x8220FB18 | size: 0x104
 * 
 * Set up properties after loading from file.
 */
void gdVibEvent::PostLoadProperties() {
    // TODO: Implement post-load property setup
}
