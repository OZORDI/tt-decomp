/**
 * hud.cpp — HUD and Flash-based UI implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "hud.hpp"
#include <cstdint>

// External references
extern void SinglesNetworkClient_A950_g(void* client, int param, float value);
extern uint8_t* g_pGlobalGameState;  // @ 0x82606430 (SDA r13+25648)

// Float constants
static const float g_floatOne = 1.0f;      // @ 0x8202D108+8
static const float g_floatZero = 0.0f;     // @ 0x8202D108


// ─────────────────────────────────────────────────────────────────────────────
// hudTrainingHUD::OnEvent @ 0x822FA518 | size: 0xE0
//
// Handles training HUD events by checking event codes and dispatching to
// the network client with appropriate parameters.
//
// Event codes:
//   2146 (0x862) - Training event with param=1, value=1.0f
//                  Also clears a flag at global state offset +6208+32
//   2147 (0x863) - Training event with param=0, value=1.0f  
//   2080 (0x820) - Training event with param=0, value=0.0f
//
// Parameters:
//   eventData - Pointer to event data structure (first uint16_t is event code)
// ─────────────────────────────────────────────────────────────────────────────
void hudTrainingHUD::OnEvent(void* eventData)
{
    // Read event code from first uint16_t of event data
    uint16_t eventCode = *(uint16_t*)eventData;
    
    // Get pointer to this object's network client interface (at offset -96)
    void* networkClient = (char*)this - 96;
    
    // Handle event code 2146 (0x862)
    if (eventCode == 2146) {
        // Access global game state
        uint8_t* globalState = *(uint8_t**)g_pGlobalGameState;
        
        // Get player slot array at offset +132
        uint32_t slotIndex = *(uint32_t*)(globalState + 132);
        
        // Calculate array index: (slotIndex + 29) * 4
        uint32_t arrayIndex = (slotIndex + 29) * 4;
        
        // Get player object from array
        void* playerObj = *(void**)(globalState + arrayIndex);
        
        // Check if training state object exists at offset +6208
        void* trainingState = *(void**)((char*)playerObj + 6208);
        
        if (trainingState != nullptr) {
            // Clear flag at offset +32 in training state
            *(uint32_t*)((char*)trainingState + 32) = 0;
        }
        
        // Call network client with param=1, value=1.0f
        SinglesNetworkClient_A950_g(networkClient, 1, g_floatOne);
        return;
    }
    
    // Handle event code 2147 (0x863)
    if (eventCode == 2147) {
        // Call network client with param=0, value=1.0f
        SinglesNetworkClient_A950_g(networkClient, 0, g_floatOne);
        return;
    }
    
    // Handle event code 2080 (0x820)
    if (eventCode == 2080) {
        // Call network client with param=0, value=0.0f
        SinglesNetworkClient_A950_g(networkClient, 0, g_floatZero);
        return;
    }
    
    // No matching event code - return without action
}
