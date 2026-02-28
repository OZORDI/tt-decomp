/**
 * gdVibEvent - Vibration Event System
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Controller vibration events triggered by gameplay actions.
 */

#pragma once

#include <cstdint>

/**
 * gdVibEvent - Vibration event class
 * @ 0x8220F8B0 (vtable)
 * 
 * Triggers controller vibration based on gameplay events.
 * Checks player ID and controller state before applying vibration.
 */
class gdVibEvent {
public:
    // Virtual destructor @ 0x8220F8B0
    virtual ~gdVibEvent();
    
    // Match event type @ 0x8220FA50 (slot 20)
    virtual bool MatchesType(uint32_t eventType);
    
    // Process event @ 0x8220FAA8 (slot 21)
    virtual void Process();
    
    // Stub function @ 0x8220F9B0 (slot 22)
    virtual void Stub22();
    
    // Trigger vibration @ 0x8220F918 (slot 23)
    virtual void TriggerVibration(void* playerContext);
    
    // Post-load property setup @ 0x8220FB18
    void PostLoadProperties();
    
protected:
    void* m_vtable;              // +0x00: Virtual function table
    uint32_t m_field_04;         // +0x04
    uint32_t m_field_08;         // +0x08
    uint32_t m_field_0C;         // +0x0C
    void* m_vibrationData1;      // +0x10: Vibration data (freed in destructor)
    uint32_t m_field_14;         // +0x14
    void* m_vibrationData2;      // +0x18: Vibration data (freed in destructor)
    uint32_t m_vibrationPattern; // +0x1C: Vibration pattern/intensity
};

// External dependencies
extern "C" void SinglesNetworkClient_GetPlayerID_E408(void* client, uint32_t param);
extern "C" void ApplyControllerVibration(void** controllers, uint32_t controllerIndex, void* vibrationPattern);
extern "C" bool ValidateControllerVibration(void* controller);
extern "C" void rage::ReleaseSingleton(void* obj);

// Global controller state arrays
extern void* g_controllerArray;      // @ 0x8271A3A8 (glob:lbl_8271A3A8)
extern void* g_playerStateArray;     // @ 0x8271A31C (glob:lbl_8271A31C)

// Global vibration state
extern uint8_t* g_vibrationStateArray;  // @ 0x8271A364
extern uint8_t* g_loop_obj_ptr;         // @ 0x825EAB30
extern float g_defaultVibrationIntensity; // @ 0x8202D110
