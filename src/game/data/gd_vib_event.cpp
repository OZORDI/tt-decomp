/**
 * gdVibEvent - Vibration Event System Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "game/gd_vib_event.hpp"
#include "game/pong_strings.hpp"

// External dependencies
#include "../game/pong_network_io.hpp"

extern "C" void rage_free(void* ptr);
extern "C" bool ValidateControllerVibration(void* controller);
extern "C" void rage::ReleaseSingleton(void* obj);

// Global arrays
void* g_controllerArray = nullptr;
void* g_playerStateArray = nullptr;

// Global state pointers
extern uint8_t* g_vibrationStateArray;  // @ 0x8271A364
extern uint8_t* g_loop_obj_ptr;         // @ 0x825EAB30
extern float g_defaultVibrationIntensity; // @ 0x8202D110

/**
 * ApplyControllerVibration (game_F480)
 * @ 0x8220F480 | size: 0xD0
 * 
 * Apply vibration pattern to a specific controller.
 * Checks if vibration is enabled globally and per-controller before applying.
 * 
 * @param controllers Base array of controller pointers
 * @param controllerIndex Controller index (will be offset by +7 internally)
 * @param vibrationPattern Pointer to vibration pattern data
 */
extern "C" void ApplyControllerVibration(
    void** controllers,
    uint32_t controllerIndex,
    void* vibrationPattern
) {
    // Calculate actual index: controllerIndex + 7
    uint32_t actualIndex = controllerIndex + 7;
    
    // Get controller pointer from array
    void* controller = controllers[actualIndex];
    
    // Read controller data at offset +24 (some ID or state index)
    uint32_t controllerData = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(controller) + 24
    );
    
    // Calculate offset into vibration state array: controllerData * 12
    uint32_t stateOffset = controllerData * 12;
    
    // Check if vibration is enabled for this controller
    // Access global vibration state array at calculated offset + 80
    uint8_t vibrationEnabled = g_vibrationStateArray[stateOffset + 80];
    
    bool shouldApplyVibration = false;
    
    if (vibrationEnabled != 0) {
        // Check global vibration enable flag at g_loop_obj_ptr + 494
        uint8_t globalVibrationEnabled = g_loop_obj_ptr[494];
        
        if (globalVibrationEnabled != 0) {
            shouldApplyVibration = true;
        }
    }
    
    if (!shouldApplyVibration) {
        return;
    }
    
    // Check if controller is initialized (byte at offset +4)
    uint8_t controllerInitialized = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uint8_t*>(controller) + 4
    );
    
    if (controllerInitialized == 0) {
        // Controller not initialized, validate it first
        if (!ValidateControllerVibration(controller)) {
            return;
        }
    }
    
    // Store vibration pattern at controller + 16
    *reinterpret_cast<void**>(
        reinterpret_cast<uint8_t*>(controller) + 16
    ) = vibrationPattern;
    
    // Store default vibration intensity at controller + 20
    *reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(controller) + 20
    ) = g_defaultVibrationIntensity;
}

/**
 * ValidateControllerVibration (game_F820)
 * @ 0x8220F820 | size: 0x90
 * 
 * Validate that controller vibration can be applied.
 * Checks vibration timing and intensity constraints.
 * 
 * @param controller Controller pointer
 * @return true if vibration can be applied, false otherwise
 */
extern "C" bool ValidateControllerVibration(void* controller) {
    // Read vibration pattern pointer at offset +16
    void* vibrationPattern = *reinterpret_cast<void**>(
        reinterpret_cast<uint8_t*>(controller) + 16
    );
    
    // If no pattern set, allow vibration
    if (vibrationPattern == nullptr) {
        return true;
    }
    
    // Initialize max intensity to default
    float maxIntensity = g_defaultVibrationIntensity;
    
    // Check two vibration slots at offsets +16 and +20 from pattern
    for (int i = 0; i < 2; i++) {
        uint32_t slotOffset = 16 + (i * 4);
        void* slot = *reinterpret_cast<void**>(
            reinterpret_cast<uint8_t*>(vibrationPattern) + slotOffset
        );
        
        if (slot != nullptr) {
            // Read count at offset +24
            uint16_t count = *reinterpret_cast<uint16_t*>(
                reinterpret_cast<uint8_t*>(slot) + 24
            );
            
            if (count > 0) {
                // Read data array pointer at offset +20
                void** dataArray = *reinterpret_cast<void***>(
                    reinterpret_cast<uint8_t*>(slot) + 20
                );
                
                // Get last entry intensity (at index count-1)
                void* lastEntry = dataArray[count - 1];
                float intensity = *reinterpret_cast<float*>(
                    reinterpret_cast<uint8_t*>(lastEntry) + 16
                );
                
                // Track maximum intensity
                if (intensity > maxIntensity) {
                    maxIntensity = intensity;
                }
            }
        }
    }
    
    // Read current intensity at controller + 20
    float currentIntensity = *reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(controller) + 20
    );
    
    // Allow vibration if max intensity exceeds current
    return maxIntensity > currentIntensity;
}

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
        rage_free(m_vibrationData2);
    }
    
    // Free vibration data 1
    if (m_vibrationData1 != nullptr) {
        rage_free(m_vibrationData1);
    }
    
    // Call singleton cleanup
    rage::ReleaseSingleton(this);
    
    // Note: Memory freeing handled by caller based on destructor flags
    // If bit 0 is set, rage_free(this) is called
}

/**
 * gdVibEvent::MatchesType
 * @ 0x8220FA50 | size: 0x58
 * 
 * Check if this event matches the given event type.
 */
bool gdVibEvent::MatchesType(uint32_t eventType) {
    extern uint32_t g_gdVibEvent_typeId;       // @ 0x825C5DF4
    extern uint32_t g_gdEventBase_typeId;      // @ 0x825C5800
    extern uint32_t g_xmlNodeStruct_typeId;    // @ 0x825C803C
    extern uint32_t g_xmlNodeStruct_typeId2;   // @ 0x825C8038

    if (eventType == g_gdVibEvent_typeId) return true;
    if (eventType == g_gdEventBase_typeId) return true;
    if (eventType == g_xmlNodeStruct_typeId) return true;
    return (eventType == g_xmlNodeStruct_typeId2);
}

/**
 * gdVibEvent::Process
 * @ 0x8220FAA8 | size: 0x6C
 * 
 * Process the vibration event.
 */
/**
 * gdVibEvent::RegisterFields @ 0x8220FAA8 | size: 0x6C
 *
 * Registers EffectName (+16) and VibPattern (+24) for XML serialization.
 */
void gdVibEvent::Process() {
    extern void* g_stringFieldType;  // @ 0x825CAF88
    extern "C" void RegisterSerializedField(void*, const char*, void*, void*, int);

    RegisterSerializedField(this, "EffectName",  (char*)this + 16, g_stringFieldType, 0);
    RegisterSerializedField(this, "VibPattern",  (char*)this + 24, g_stringFieldType, 0);
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
    GetPlayerID(playerContext, 1);
    
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
    ApplyControllerVibration(controllers, controller, reinterpret_cast<void*>(m_vibrationPattern));
}

/**
 * gdVibEvent::PostLoadProperties
 * @ 0x8220FB18 | size: 0x104
 * 
 * Set up properties after loading from file.
 */
/**
 * gdVibEvent::PostLoadProperties @ 0x8220FB18 | size: 0x104
 *
 * Called after XML properties are loaded. Resolves the effect name string
 * (+16) to a numeric effect ID (+20) using a name lookup function.
 * Then searches the vibration manager's effect array for a matching entry
 * by name, storing the resolved pointer at +28.
 *
 * Debug string: "gdVibEvent::PostLoadProperties - Could not find effect '%s'"
 */
void gdVibEvent::PostLoadProperties() {
    extern "C" void xmlNodeStruct_vfn_2(void*);
    extern "C" uint16_t LookupEffectId(const char* name);  // @ 0x8225EA68
    extern "C" void nop_8240E6D0(const char* msg, ...);
    extern void* g_vibrationMgr;  // @ 0x8271A3A8

    // Call base class Initialize
    xmlNodeStruct_vfn_2(this);

    // Resolve effect name to ID
    const char* effectName = *(const char**)((char*)this + 16);
    uint16_t effectId = LookupEffectId(effectName);
    *(uint16_t*)((char*)this + 20) = effectId;

    if (effectId == 0) {
        nop_8240E6D0("gdVibEvent::PostLoadProperties - Could not find effect '%s'", effectName);
    }

    // Search vibration manager's array for matching effect by name
    const char* vibPatternName = *(const char**)((char*)this + 24);
    void* vibMgr = *(void**)&g_vibrationMgr;
    int16_t arrayCount = *(int16_t*)((char*)vibMgr + 24);
    void** arrayBase = *(void***)((char*)vibMgr + 20);

    int32_t foundIndex = -1;
    for (int32_t i = 0; i < arrayCount; i++) {
        // Get entry name at entry[0] -> +24
        void* entry = arrayBase[i];
        const char* entryName = *(const char**)((char*)entry + 24);

        // String compare (inline strcmp)
        const char* s1 = vibPatternName;
        const char* s2 = entryName;
        while (*s1 && (*s1 == *s2)) { s1++; s2++; }
        if (*s1 == *s2) {
            foundIndex = i;
            break;
        }
    }

    // Store resolved entry pointer
    void* resolvedEntry = nullptr;
    if (foundIndex >= 0 && foundIndex < arrayCount) {
        resolvedEntry = arrayBase[foundIndex];
    }
    *(void**)((char*)this + 28) = resolvedEntry;

    if (resolvedEntry == nullptr) {
        nop_8240E6D0("gdVibEvent::PostLoadProperties - Could not find vibration pattern '%s'", vibPatternName);
    }
}
