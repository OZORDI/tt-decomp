/**
 * pong_camera.cpp
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Camera management system implementation
 */

#include "pong_camera.hpp"
#include <cmath>
#include <cstring>

/**
 * pongCameraMgr::AdjustTimingForIndex
 * @ 0x82173430 | size: 0xA0
 * 
 * Adjusts camera timing based on accumulated durations in a timing array.
 * This function appears to handle camera transition timing by finding where
 * an accumulated time crosses a target threshold, then adjusting the timing
 * value and clearing specific array entries.
 * 
 * @param index - The index to process (used to determine which array elements to clear)
 */
void pongCameraMgr::AdjustTimingForIndex(int index) {
    // Load target value and negate it
    float targetTime = -m_currentTime;  // offset +8
    
    // Process timing array if index is valid
    if (index >= 0) {
        // Initialize accumulators
        float accumulated = 0.0f;
        float previousAccumulated = 0.0f;
        int currentIndex = 0;
        
        // Pointer to timing array at offset +60 (0x3C)
        float* timingArray = &m_timingArray;  // offset +0x3C (60)
        
        // Accumulate timing values until we cross the target threshold
        while (true) {
            float currentValue = timingArray[currentIndex];
            accumulated += currentValue;
            
            // Check if we've crossed the threshold
            if (accumulated >= targetTime) {
                break;
            }
            
            // Check if we've reached the target index
            if (currentIndex == index) {
                // We're at the target index but haven't crossed threshold yet
                if (accumulated <= targetTime) {
                    // Calculate the overshoot and adjust
                    float overshoot = accumulated - previousAccumulated;
                    targetTime = targetTime - overshoot;
                } else {
                    // Calculate undershoot and adjust
                    float undershoot = targetTime - previousAccumulated;
                    targetTime = targetTime - undershoot;
                }
                break;
            }
            
            // Move to next element
            currentIndex++;
            previousAccumulated = accumulated;
            
            if (currentIndex > index) {
                break;
            }
        }
    }
    
    // Store the adjusted time back (negated)
    m_currentTime = -targetTime;  // offset +8
    
    // Clear specific array elements at calculated indices
    // These appear to be related to the input index + fixed offsets
    int clearIndex1 = index + 15;
    int clearIndex2 = index + 33;
    
    float* timingArr = &m_timingArray;  // offset +0x3C
    timingArr[clearIndex1] = 0.0f;
    timingArr[clearIndex2] = 0.0f;
}

// Transition parameter structs used by TryTransition and related functions
struct TransitionParams {
    float value;
    uint32_t vtableOrState;
    uint32_t padding;
};

struct TransitionFlags {
    uint32_t flag1;
    uint32_t flag2;
};

// Extern helpers used by transition functions
extern uint8_t pongCameraMgr_ValidateTransition(void* gameState, TransitionParams* params,
                                                 int mode, TransitionFlags* flags, int enable);
extern uint8_t pg_ApplyTransition(void* gameState, TransitionParams* params,
                                  int mode, TransitionFlags* flags, int enable);
extern uint8_t pongCameraMgr_6E08(void* gameState, TransitionParams* params,
                                   int mode, void* unused1, int unused2);

// Static helper for the clamped-transition pattern shared by 14 functions
static bool doClampedTransition(void* self, void* gameState, int mode, void* flags, int r7) {
    float currentValue = *(float*)self;
    extern float g_cameraThreshold;
    float threshold = g_cameraThreshold;
    float clampedMin = (threshold - currentValue >= 0.0f) ? currentValue : threshold;
    float clampedMax = (currentValue - threshold >= 0.0f) ? currentValue : threshold;
    TransitionParams params;
    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)self;
    params.padding = 0;
    uint8_t result = pongCameraMgr_6E08(gameState, &params, mode, flags, r7);
    if (result != 0 && params.value <= clampedMax) {
        *(float*)self = params.value;
        return true;
    }
    return false;
}

/**
 * pongCameraMgr::TryTransition
 * @ 0x821F7338 | size: 0xE4
 *
 * Attempts to transition the camera to a new state, validating the transition
 * is within acceptable bounds. This function uses a threshold value to clamp
 * the camera state and calls helper functions to validate and apply the transition.
 *
 * @param gameState - Pointer to game state object (pg*)
 * @return true if transition succeeded, false otherwise
 */
bool pongCameraMgr::TryTransition(void* gameState) {
    // Load current camera value (appears to be at offset 0, overlapping with vtable)
    // This is likely a union or the first field after vtable
    float currentValue = *(float*)((char*)this + 0);
    
    // Load threshold constant from .data section
    // Address: 0x825C5EC0 (calculated from lis r11,-32164 + lfs f13,24256(r11))
    extern float g_cameraThreshold;  // TODO: Define this global
    float threshold = g_cameraThreshold;
    
    // Compute clamped values using fsel pattern
    // fsel chooses first operand if condition >= 0, else second operand
    float diff1 = threshold - currentValue;
    float diff2 = currentValue - threshold;
    
    // Clamp to minimum (choose currentValue if it's <= threshold, else threshold)
    float clampedMin = (diff1 >= 0.0f) ? currentValue : threshold;
    
    // Clamp to maximum (choose currentValue if it's >= threshold, else threshold)  
    float clampedMax = (diff2 >= 0.0f) ? currentValue : threshold;
    
    // Prepare parameters for validation call
    TransitionParams params;

    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;  // Copy vtable/state
    params.padding = 0;

    TransitionFlags flags;

    flags.flag1 = 3;
    flags.flag2 = 1;

    uint8_t validationResult = pongCameraMgr_ValidateTransition(gameState, &params, 7, &flags, 1);

    if (validationResult != 0) {
        // Validation succeeded, check if we need to apply the transition
        float modifiedValue = params.value;

        if (modifiedValue <= clampedMax) {
            return pg_ApplyTransition(gameState, &params, 16, &flags, 1) != 0;
        } else {
            // Value exceeds maximum, store it directly
            *(float*)((char*)this + 0) = modifiedValue;
            return true;
        }
    }
    
    // Validation failed
    return false;
}

/**
 * pongCameraMgr::IsStateNine
 * @ 0x821F7F40 | size: 0x20
 * 
 * Checks if the global camera state equals 9.
 * This appears to be checking for a specific camera mode or state condition.
 * 
 * @return true if global state is 9, false otherwise
 */
bool pongCameraMgr::IsStateNine() const {
    // Load global camera state from .data section
    // Address: 0x825C5EB8 (lbl_825C5EB8)
    extern int32_t g_cameraState;  // TODO: Define this global in globals
    
    return g_cameraState == 9;
}

/**
 * pongCameraMgr::TryTransitionMode20
 * @ 0x821F6B78 | size: 0x38
 * 
 * Attempts a camera transition using mode 20. This is a simple wrapper
 * around the core transition function that validates and applies camera
 * state changes with a specific mode parameter.
 * 
 * @param gameState - Pointer to game state object
 * @return true if transition succeeded, false otherwise
 */
bool pongCameraMgr::TryTransitionMode20(void* gameState) {
    // Call the core transition function with mode 20
    // The mode parameter (r5 = 20) likely specifies a particular
    // camera transition type or validation mode
    extern bool pongCameraMgr_35A0_fw(pongCameraMgr* mgr, void* gameState, int mode);

    bool result = pongCameraMgr_35A0_fw(this, gameState, 20);
    
    // Convert to explicit boolean (the assembly does clrlwi r11,r3,24
    // which extracts the low byte, then compares to 0)
    return result != 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::PostLoadChildren()  [vtable slot 22 @ 0x82165830]
// Returns pointer to the string "bucket" - likely a debug/editor identifier
// ─────────────────────────────────────────────────────────────────────────────
const char* pongCameraMgr::PostLoadChildren() {
    return "bucket";
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::PostLoadProperties()  [vtable slot 20 @ 0x821657F8]
// Checks if parameter matches one of two global values
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::PostLoadProperties(uint32_t value) {
    extern uint32_t g_cameraProperty1;  // @ 0x825C8A40
    extern uint32_t g_cameraProperty2;  // @ 0x825C93D0
    
    if (value == g_cameraProperty1) {
        return true;
    }
    
    return (value == g_cameraProperty2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::GetAspectRatio()  @ 0x82143E98
// Calculates aspect ratio from camera dimensions
// ─────────────────────────────────────────────────────────────────────────────
float pongCameraMgr::GetAspectRatio() const {
    extern float g_floatOne;  // @ 0x8202D108 + 8
    
    float verticalFOV = m_verticalFOV;    // +0x33C

    // Check if vertical FOV is zero (using constant comparison)
    if (verticalFOV == g_floatOne) {
        float horizontalFOV = m_horizontalFOV;  // +0x324

        if (horizontalFOV != g_floatOne) {
            // Calculate aspect ratio from viewport dimensions
            int32_t viewportWidth = m_viewportWidth;    // +0x330
            int32_t viewportHeight = m_viewportHeight;  // +0x334
            float baseWidth = m_baseWidth;              // +0x320
            
            // aspect = (viewportWidth * baseWidth) / (viewportHeight * horizontalFOV)
            float widthRatio = (float)viewportWidth * baseWidth;
            float heightRatio = (float)viewportHeight * horizontalFOV;
            return widthRatio / heightRatio;
        }
    }
    
    // Default to 1.0 if FOV is zero
    extern float g_floatZero;  // @ 0x8202D108
    return g_floatZero;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateZero()  @ 0x821F7E28
// Checks if global camera state is zero (returns true if zero)
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateZero() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateOne()  @ 0x821F7E40
// Checks if global camera state equals 1
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateOne() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateTwo()  @ 0x821F7E60
// Checks if global camera state equals 2
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateTwo() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateThree()  @ 0x821F7E80
// Checks if global camera state equals 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateThree() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode2_Variant1()  @ 0x821F3910
// Attempts camera transition with mode 2 using variant 1 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode2_Variant1(void* gameState) {
    // Delegate to helper function with mode 2
    extern bool pongCameraMgr_3500_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_3500_fw(this, gameState, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode2_Variant2()  @ 0x821F3948
// Attempts camera transition with mode 2 using variant 2 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode2_Variant2(void* gameState) {
    // Delegate to helper function with mode 2
    extern bool pongCameraMgr_35A0_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_35A0_fw(this, gameState, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode2_Variant3()  @ 0x821F3980
// Attempts camera transition with mode 2 using variant 3 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode2_Variant3(void* gameState) {
    // Delegate to helper function with mode 2
    extern bool pongCameraMgr_3650_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_3650_fw(this, gameState, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode5_Variant1()  @ 0x821F3B88
// Attempts camera transition with mode 5 using variant 1 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode5_Variant1(void* gameState) {
    // Delegate to helper function with mode 5
    extern bool pongCameraMgr_3500_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_3500_fw(this, gameState, 5);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode5_Variant2()  @ 0x821F3BC0
// Attempts camera transition with mode 5 using variant 2 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode5_Variant2(void* gameState) {
    // Delegate to helper function with mode 5
    extern bool pongCameraMgr_35A0_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_35A0_fw(this, gameState, 5);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionMode5_Variant3()  @ 0x821F3BF8
// Attempts camera transition with mode 5 using variant 3 helper
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionMode5_Variant3(void* gameState) {
    // Delegate to helper function with mode 5
    extern bool pongCameraMgr_3650_fw(pongCameraMgr* mgr, void* gameState, int mode);
    return pongCameraMgr_3650_fw(this, gameState, 5);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateAndClampTransition()  @ 0x821F3500
// Core transition validation with clamping logic
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateAndClampTransition(void* gameState, int mode) {
    // Load current camera value (stored at offset 0, overlapping with vtable in union)
    float currentValue = *(float*)this;
    
    // Load threshold constant
    extern float g_cameraThreshold;  // @ 0x825C5EC0
    float threshold = g_cameraThreshold;
    
    // Clamp value to threshold range using fsel pattern
    float diffMin = threshold - currentValue;
    float diffMax = currentValue - threshold;
    
    float clampedMin = (diffMin >= 0.0f) ? currentValue : threshold;
    float clampedMax = (diffMax >= 0.0f) ? currentValue : threshold;
    
    // Prepare validation parameters
    TransitionParams params;

    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;
    params.padding = 0;

    uint8_t validationResult = pongCameraMgr_6E08(gameState, &params, mode, nullptr, 0);
    
    if (validationResult != 0) {
        float modifiedValue = params.value;
        
        if (modifiedValue <= clampedMax) {
            // Value is within range, store it
            *(float*)this = modifiedValue;
            return true;
        }
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateAndClampTransitionWithFlags()  @ 0x821F3650
// Transition validation with explicit flag parameters
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateAndClampTransitionWithFlags(void* gameState, int mode) {
    // Load current camera value
    float currentValue = *(float*)this;
    
    // Load threshold constant
    extern float g_cameraThreshold;  // @ 0x825C5EC0
    float threshold = g_cameraThreshold;
    
    // Clamp value to threshold range
    float diffMin = threshold - currentValue;
    float diffMax = currentValue - threshold;
    
    float clampedMin = (diffMin >= 0.0f) ? currentValue : threshold;
    float clampedMax = (diffMax >= 0.0f) ? currentValue : threshold;
    
    // Prepare validation parameters with explicit flags
    TransitionParams params;
    TransitionFlags flags;

    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;
    params.padding = 0;

    flags.flag1 = 3;
    flags.flag2 = 1;

    uint8_t validationResult = pongCameraMgr_6E08(gameState, &params, mode, &flags, 1);
    
    if (validationResult != 0) {
        float modifiedValue = params.value;
        
        if (modifiedValue <= clampedMax) {
            // Value is within range, store it
            *(float*)this = modifiedValue;
            return true;
        }
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateAndAdjustTransition()  @ 0x821F3780
// Validates transition and adjusts camera value by subtracting a constant
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateAndAdjustTransition(void* gameState, int mode) {
    uint8_t validationResult = pongCameraMgr_6E08(gameState, reinterpret_cast<TransitionParams*>(this), mode, nullptr, 0);
    
    if (validationResult != 0) {
        // Load current value and adjustment constant
        float currentValue = *(float*)this;
        
        extern float g_cameraAdjustment;  // @ 0x8202CFF4
        float adjustment = g_cameraAdjustment;
        
        // Subtract adjustment from current value
        float adjustedValue = currentValue - adjustment;
        
        // Store adjusted value back
        *(float*)this = adjustedValue;
        
        return true;
    }
    
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CopyParametersToBuffer()  @ 0x821F99D0
// Copies 6 parameters (24 bytes) to a buffer allocated from the camera manager
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::CopyParametersToBuffer(uint32_t param1, uint32_t param2, 
                                           uint32_t param3, uint32_t param4,
                                           uint32_t param5, uint32_t param6) {
    // Allocate buffer from camera manager (offset +4, size 16 bytes)
    extern void* pongCameraMgr_B9B0_g(void* mgr, int size);
    void* buffer = pongCameraMgr_B9B0_g(&flags, 16);
    
    // Copy 6 parameters (24 bytes total) to the allocated buffer
    uint32_t* dest = (uint32_t*)buffer;
    dest[0] = param1;
    dest[1] = param2;
    dest[2] = param3;
    dest[3] = param4;
    dest[4] = param5;
    dest[5] = param6;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr — Camera Mode Queries and Small Helpers
// ─────────────────────────────────────────────────────────────────────────────

// Camera system globals
extern uint32_t g_cameraMode;     // @ 0x825C5EB8 — current camera mode enum
extern uint32_t g_cameraFlags;    // @ 0x826064D0 — camera state flags (SDA)

// ── Pattern A: Camera index tail-call thunks (8B each) ─────────────────────

extern "C" void pongCameraMgr_SetCameraByIndex(void* mgr, int index);  // @ 0x821F7CD8

/**
 * pongCameraMgr camera-by-index thunks @ 0x821F7DF8-7E20 | 8B each
 *
 * Six trivial thunks that set r7 = N and tail-call the camera setter.
 * Each selects a different camera preset by index (1-6).
 */
void pongCameraMgr::SelectCamera1() { pongCameraMgr_SetCameraByIndex(this, 1); }  // 7DF8 @ 0x821F7DF8
void pongCameraMgr::SelectCamera2() { pongCameraMgr_SetCameraByIndex(this, 2); }  // 7E00 @ 0x821F7E00
void pongCameraMgr::SelectCamera3() { pongCameraMgr_SetCameraByIndex(this, 3); }  // 7E08 @ 0x821F7E08
void pongCameraMgr::SelectCamera4() { pongCameraMgr_SetCameraByIndex(this, 4); }  // 7E10 @ 0x821F7E10
void pongCameraMgr::SelectCamera5() { pongCameraMgr_SetCameraByIndex(this, 5); }  // 7E18 @ 0x821F7E18
void pongCameraMgr::SelectCamera6() { pongCameraMgr_SetCameraByIndex(this, 6); }  // 7E20 @ 0x821F7E20

// ── Pattern B: RTTI destructor adjustor thunk (8B) ─────────────────────────

/**
 * pongCameraMgr secondary vtable dtor adjustor @ 0x82166DB8 | size: 0x8
 *
 * Adjusts `this` pointer by -16 before forwarding to the primary destructor.
 * Standard MI thunk for the secondary vtable.
 */
void pongCameraMgr::DtorAdjustor() {  // rtti_5EA8_0 @ 0x82166DB8
    typedef void (*DtorFn)(void*);
    DtorFn dtor = (DtorFn)(*(void***)((char*)this - 16))[0];
    dtor((char*)this - 16);
}

// ── Pattern E: Global byte == 0 checks (24B each) ─────────────────────────

/**
 * pongCameraMgr global state flag checks @ 0x821F98D8-9908 | 24B each
 *
 * Each reads a global byte and returns true if it equals zero.
 * Used for checking camera subsystem enable/disable flags.
 */
bool pongCameraMgr::IsFlag98D8Clear() {  // 98D8_p33 @ 0x821F98D8
    extern uint8_t g_cameraFlag_7532;  // SDA offset, resolved at link
    return g_cameraFlag_7532 == 0;
}

bool pongCameraMgr::IsFlag98F0Clear() {  // 98F0_p33 @ 0x821F98F0
    extern uint8_t g_cameraFlag_8898;
    return g_cameraFlag_8898 == 0;
}

bool pongCameraMgr::IsFlag9908Clear() {  // 9908_p33 @ 0x821F9908
    extern uint8_t g_cameraFlag_8897;
    return g_cameraFlag_8897 == 0;
}

// ── Pattern F: IsCameraMode(N) — 21 functions (32B each) ──────────────────

/**
 * pongCameraMgr::IsCameraMode(N) @ 0x821F7E40-80C0 | 32B each
 *
 * Each reads g_cameraMode and compares to a constant. Returns true if
 * the camera is currently in that specific mode. The camera mode enum
 * maps to gameplay situations:
 *   1=default, 2=serve, 3=rally, 4=replay, 5=celebration, ...
 */
static bool IsCameraModeEqual(int modeValue) {
    return (int)g_cameraMode == modeValue;
}

bool pongCameraMgr::IsCameraMode1()  { return IsCameraModeEqual(1);  }  // 7E40 @ 0x821F7E40
bool pongCameraMgr::IsCameraMode2()  { return IsCameraModeEqual(2);  }  // 7E60 @ 0x821F7E60
bool pongCameraMgr::IsCameraMode3()  { return IsCameraModeEqual(3);  }  // 7E80 @ 0x821F7E80
bool pongCameraMgr::IsCameraMode4()  { return IsCameraModeEqual(4);  }  // 7EA0 @ 0x821F7EA0
bool pongCameraMgr::IsCameraMode5()  { return IsCameraModeEqual(5);  }  // 7EC0 @ 0x821F7EC0
bool pongCameraMgr::IsCameraMode6()  { return IsCameraModeEqual(6);  }  // 7EE0 @ 0x821F7EE0
bool pongCameraMgr::IsCameraMode7()  { return IsCameraModeEqual(7);  }  // 7F00 @ 0x821F7F00
bool pongCameraMgr::IsCameraMode8()  { return IsCameraModeEqual(8);  }  // 7F20 @ 0x821F7F20
bool pongCameraMgr::IsCameraMode9()  { return IsCameraModeEqual(9);  }  // 7F40 @ 0x821F7F40
bool pongCameraMgr::IsCameraMode10() { return IsCameraModeEqual(10); }  // 7F60 @ 0x821F7F60
bool pongCameraMgr::IsCameraMode11() { return IsCameraModeEqual(11); }  // 7F80 @ 0x821F7F80
bool pongCameraMgr::IsCameraMode12() { return IsCameraModeEqual(12); }  // 7FA0 @ 0x821F7FA0
bool pongCameraMgr::IsCameraMode13() { return IsCameraModeEqual(13); }  // 7FC0 @ 0x821F7FC0
bool pongCameraMgr::IsCameraMode14() { return IsCameraModeEqual(14); }  // 7FE0 @ 0x821F7FE0
bool pongCameraMgr::IsCameraMode15() { return IsCameraModeEqual(15); }  // 8000 @ 0x821F8000
bool pongCameraMgr::IsCameraMode16() { return IsCameraModeEqual(16); }  // 8020 @ 0x821F8020
bool pongCameraMgr::IsCameraMode17() { return IsCameraModeEqual(17); }  // 8040 @ 0x821F8040
bool pongCameraMgr::IsCameraMode18() { return IsCameraModeEqual(18); }  // 8060 @ 0x821F8060
bool pongCameraMgr::IsCameraMode19() { return IsCameraModeEqual(19); }  // 8080 @ 0x821F8080
bool pongCameraMgr::IsCameraMode20() { return IsCameraModeEqual(20); }  // 80A0 @ 0x821F80A0
bool pongCameraMgr::IsCameraMode21() { return IsCameraModeEqual(21); }  // 80C0 @ 0x821F80C0

// ── Pattern G: Camera flag bit tests (52B each) ───────────────────────────

/**
 * pongCameraMgr flag bit tests @ 0x821F6BE8-6CC8 | 52B each
 *
 * Each tests a specific bit of g_cameraFlags and returns true/false.
 * The flags control camera behaviors like shake, zoom, track, etc.
 */
bool pongCameraMgr::HasFlag_Bit4()  { return (g_cameraFlags & 0x10)    != 0; }  // 6BE8 @ 0x821F6BE8
bool pongCameraMgr::HasFlag_Bit8()  { return (g_cameraFlags & 0x100)   != 0; }  // 6C20 @ 0x821F6C20
bool pongCameraMgr::HasFlag_Bit16() { return (g_cameraFlags & 0x10000) != 0; }  // 6C58 @ 0x821F6C58
bool pongCameraMgr::HasFlag_Bit1()  { return (g_cameraFlags & 0x2)     != 0; }  // 6C90 @ 0x821F6C90
bool pongCameraMgr::HasFlag_Bit2()  { return (g_cameraFlags & 0x4)     != 0; }  // 6CC8 @ 0x821F6CC8

// ── Pattern H: Call helper with arg N (56B each, 25 functions) ────────────

// Three camera query helpers that take an index parameter
extern "C" bool pongCameraMgr_QueryA(void* mgr, int index);  // @ 0x821F3500
extern "C" bool pongCameraMgr_QueryB(void* mgr, int index);  // @ 0x821F35A0
extern "C" bool pongCameraMgr_QueryC(void* mgr, int index);  // @ 0x821F3650

/**
 * pongCameraMgr parameterized camera queries @ various | 56B each
 *
 * Each calls one of three query helpers (A/B/C) with a specific camera
 * slot index. Used for checking camera properties per-slot.
 */

// QueryA(N) calls:
bool pongCameraMgr::QueryA_2()  { return pongCameraMgr_QueryA(this, 2);  }  // 3910 @ 0x821F3910
bool pongCameraMgr::QueryA_5()  { return pongCameraMgr_QueryA(this, 5);  }  // 3B88 @ 0x821F3B88
bool pongCameraMgr::QueryA_3()  { return pongCameraMgr_QueryA(this, 3);  }  // 45E0 @ 0x821F45E0
bool pongCameraMgr::QueryA_16() { return pongCameraMgr_QueryA(this, 16); }  // 4960 @ 0x821F4960
bool pongCameraMgr::QueryA_19() { return pongCameraMgr_QueryA(this, 19); }  // 4C40 @ 0x821F4C40
bool pongCameraMgr::QueryA_21() { return pongCameraMgr_QueryA(this, 21); }  // 50E0 @ 0x821F50E0
bool pongCameraMgr::QueryA_20() { return pongCameraMgr_QueryA(this, 20); }  // 6B40 @ 0x821F6B40

// QueryB(N) calls:
bool pongCameraMgr::QueryB_2()  { return pongCameraMgr_QueryB(this, 2);  }  // 3948 @ 0x821F3948
bool pongCameraMgr::QueryB_5()  { return pongCameraMgr_QueryB(this, 5);  }  // 3BC0 @ 0x821F3BC0
bool pongCameraMgr::QueryB_3()  { return pongCameraMgr_QueryB(this, 3);  }  // 4618 @ 0x821F4618
bool pongCameraMgr::QueryB_16() { return pongCameraMgr_QueryB(this, 16); }  // 4998 @ 0x821F4998
bool pongCameraMgr::QueryB_15() { return pongCameraMgr_QueryB(this, 15); }  // 5070 @ 0x821F5070
bool pongCameraMgr::QueryB_21() { return pongCameraMgr_QueryB(this, 21); }  // 5118 @ 0x821F5118
bool pongCameraMgr::QueryB_20() { return pongCameraMgr_QueryB(this, 20); }  // 6B78 @ 0x821F6B78
bool pongCameraMgr::QueryB_13() { return pongCameraMgr_QueryB(this, 13); }  // 74E0 @ 0x821F74E0

// QueryC(N) calls:
bool pongCameraMgr::QueryC_2()  { return pongCameraMgr_QueryC(this, 2);  }  // 3980 @ 0x821F3980
bool pongCameraMgr::QueryC_5()  { return pongCameraMgr_QueryC(this, 5);  }  // 3BF8 @ 0x821F3BF8
bool pongCameraMgr::QueryC_3()  { return pongCameraMgr_QueryC(this, 3);  }  // 4650 @ 0x821F4650
bool pongCameraMgr::QueryC_16() { return pongCameraMgr_QueryC(this, 16); }  // 49D0 @ 0x821F49D0
bool pongCameraMgr::QueryC_15() { return pongCameraMgr_QueryC(this, 15); }  // 50A8 @ 0x821F50A8
bool pongCameraMgr::QueryC_21() { return pongCameraMgr_QueryC(this, 21); }  // 5150 @ 0x821F5150
bool pongCameraMgr::QueryC_20() { return pongCameraMgr_QueryC(this, 20); }  // 6BB0 @ 0x821F6BB0
bool pongCameraMgr::QueryC_13() { return pongCameraMgr_QueryC(this, 13); }  // 7518 @ 0x821F7518

// ── Pattern I: Conditional byte load (60B each) ───────────────────────────

/**
 * pongCameraMgr conditional flag reads @ 0x821F3700-3740 | 60B each
 *
 * Reads a selector byte; depending on its value, returns one of two
 * other flag bytes. Used for player-dependent camera flags.
 */
extern uint8_t g_cameraSelector;  // @ SDA+25804
extern uint8_t g_cameraFlagA;     // @ SDA+25805
extern uint8_t g_cameraFlagB;     // @ SDA+25806

bool pongCameraMgr::GetConditionalFlagA() {  // 3700 @ 0x821F3700
    return g_cameraSelector != 0 ? g_cameraFlagB : g_cameraFlagA;
}

bool pongCameraMgr::GetConditionalFlagB() {  // 3740 @ 0x821F3740
    return g_cameraSelector != 0 ? g_cameraFlagA : g_cameraFlagB;
}

// ── Pattern J: Float comparison, return bool (64B each, 5 functions) ──────

/**
 * pongCameraMgr float threshold checks @ 0x821F7550-7688 | 64B each
 *
 * Each loads a float from an indexed camera data array, subtracts a
 * base float, and compares against a threshold constant. Returns true
 * if the difference exceeds the threshold.
 */
extern "C" bool pongCameraMgr_FloatThresholdA(void* mgr, float threshold);
extern "C" bool pongCameraMgr_FloatThresholdB(void* mgr);

bool pongCameraMgr::CheckThreshold7550(float threshold) { return pongCameraMgr_FloatThresholdA(this, threshold); }  // 7550 @ 0x821F7550
bool pongCameraMgr::CheckThreshold7588() { return pongCameraMgr_FloatThresholdB(this); }  // 7588 @ 0x821F7588
bool pongCameraMgr::CheckThreshold75C8() { return pongCameraMgr_FloatThresholdB(this); }  // 75C8 @ 0x821F75C8
bool pongCameraMgr::CheckThreshold7608() { return pongCameraMgr_FloatThresholdB(this); }  // 7608 @ 0x821F7608
bool pongCameraMgr::CheckThreshold7648() { return pongCameraMgr_FloatThresholdB(this); }  // 7648 @ 0x821F7648
bool pongCameraMgr::CheckThreshold7688() { return pongCameraMgr_FloatThresholdB(this); }  // 7688 @ 0x821F7688


// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode1()  @ 0x821F7DF8
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 1
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode1(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode2()  @ 0x821F7E00
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 2
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode2(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode3()  @ 0x821F7E08
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode3(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode4()  @ 0x821F7E10
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 4
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode4(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 4);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode5()  @ 0x821F7E18
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 5
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode5(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 5);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryCameraChange_Mode6()  @ 0x821F7E20
// Tail-call wrapper: delegates to pg_7CD8_2h with mode 6
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryCameraChange_Mode6(void* gameState) {
    extern bool pg_7CD8_2h(pongCameraMgr* mgr, void* gameState, int mode);
    return pg_7CD8_2h(this, gameState, 6);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateFour()  @ 0x821F7EA0
// Checks if global camera state equals 4
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateFour() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 4);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateFive()  @ 0x821F7EC0
// Checks if global camera state equals 5
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateFive() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 5);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateSix()  @ 0x821F7EE0
// Checks if global camera state equals 6
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateSix() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 6);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateSeven()  @ 0x821F7F00
// Checks if global camera state equals 7
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateSeven() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 7);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateEight()  @ 0x821F7F20
// Checks if global camera state equals 8
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateEight() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 8);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateTen()  @ 0x821F7F60
// Checks if global camera state equals 10
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateTen() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 10);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateEleven()  @ 0x821F7F80
// Checks if global camera state equals 11
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateEleven() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 11);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateTwelve()  @ 0x821F7FA0
// Checks if global camera state equals 12
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateTwelve() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 12);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::IsCameraStateThirteen()  @ 0x821F7FC0
// Checks if global camera state equals 13
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::IsCameraStateThirteen() const {
    extern int32_t g_cameraState;  // @ 0x825C5EB8
    return (g_cameraState == 13);
}


// ── Pattern K: Dual-query AND functions (108B each, 3 functions) ────────────

/**
 * pongCameraMgr dual-query camera checks @ 0x821F4688-4768 | 108B each
 *
 * Each creates a temporary copy of the camera state (first 8 bytes),
 * validates it with two different query helpers at modes 3 and 16.
 * Returns true only when BOTH queries succeed. The copy prevents
 * mutation of the original camera state during validation.
 */

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckDualQueryAA()  @ 0x821F4688 | size: 0x6C
// Validates via QueryA(mode=3) AND QueryA(mode=16) on a state copy
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckDualQueryAA(void* gameState) {
    // Copy first 8 bytes of this into a local buffer for non-destructive query
    uint64_t stateCopy;
    stateCopy = *(uint64_t*)this;

    bool firstCheck = pongCameraMgr_QueryA(&stateCopy, 3);

    // Reload state copy (query may have modified the buffer)
    stateCopy = *(uint64_t*)this;
    bool secondCheck = pongCameraMgr_QueryA(&stateCopy, 16);

    if (!firstCheck) {
        return false;
    }
    return secondCheck;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckDualQueryCB()  @ 0x821F46F8 | size: 0x6C
// Validates via QueryC(mode=3) AND QueryB(mode=16) on a state copy
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckDualQueryCB(void* gameState) {
    uint64_t stateCopy;
    stateCopy = *(uint64_t*)this;

    bool firstCheck = pongCameraMgr_QueryC(&stateCopy, 3);

    stateCopy = *(uint64_t*)this;
    bool secondCheck = pongCameraMgr_QueryB(&stateCopy, 16);

    if (!firstCheck) {
        return false;
    }
    return secondCheck;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckDualQueryBC()  @ 0x821F4768 | size: 0x6C
// Validates via QueryB(mode=3) AND QueryC(mode=16) on a state copy
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckDualQueryBC(void* gameState) {
    uint64_t stateCopy;
    stateCopy = *(uint64_t*)this;

    bool firstCheck = pongCameraMgr_QueryB(&stateCopy, 3);

    stateCopy = *(uint64_t*)this;
    bool secondCheck = pongCameraMgr_QueryC(&stateCopy, 16);

    if (!firstCheck) {
        return false;
    }
    return secondCheck;
}

// ── Pattern L: Triple-query function (120B) ─────────────────────────────────

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckMultiModeQueryA()  @ 0x821F47D8 | size: 0x78
// Queries QueryA with modes 16, 3, and 19; returns true if (16 OR 3) AND 19
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckMultiModeQueryA(void* gameState) {
    bool checkMode16 = pongCameraMgr_QueryA(this, 16);
    bool checkMode3  = pongCameraMgr_QueryA(this, 3);
    bool checkMode19 = pongCameraMgr_QueryA(this, 19);

    // Return true if (mode16 OR mode3) succeeded, AND mode19 also succeeded
    if (checkMode16 || checkMode3) {
        return checkMode19;
    }
    return false;
}

// ── Pattern M: Query-with-offset functions (140B each, 3 functions) ─────────

extern float g_cameraOffsetDelta;  // @ 0x8202D108 (.rdata float constant)

/**
 * pongCameraMgr offset-adjusted camera queries @ 0x821F6990-6AB0 | 140B each
 *
 * Each creates a state copy, queries with mode 20. If that succeeds,
 * adds a float offset constant to the copied value and queries again
 * with mode 3. Used for checking camera positions at an adjusted offset
 * without modifying the original camera state.
 */

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckOffsetQueryA()  @ 0x821F6990 | size: 0x8C
// Queries mode 20 via QueryA, then retries offset-adjusted copy via QueryA mode 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckOffsetQueryA(void* gameState) {
    uint64_t stateCopy = *(uint64_t*)this;

    bool initialCheck = pongCameraMgr_QueryA(&stateCopy, 20);
    if (!initialCheck) {
        return false;
    }

    // Add offset delta to the float value in the copy
    float currentValue = *(float*)this;
    float adjustedValue = currentValue + g_cameraOffsetDelta;
    *(float*)&stateCopy = adjustedValue;

    return pongCameraMgr_QueryA(&stateCopy, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckOffsetQueryB()  @ 0x821F6A20 | size: 0x8C
// Queries mode 20 via QueryB, then retries offset-adjusted copy via QueryA mode 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckOffsetQueryB(void* gameState) {
    uint64_t stateCopy = *(uint64_t*)this;

    bool initialCheck = pongCameraMgr_QueryB(&stateCopy, 20);
    if (!initialCheck) {
        return false;
    }

    float currentValue = *(float*)this;
    float adjustedValue = currentValue + g_cameraOffsetDelta;
    *(float*)&stateCopy = adjustedValue;

    return pongCameraMgr_QueryA(&stateCopy, 3);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckOffsetQueryC()  @ 0x821F6AB0 | size: 0x8C
// Queries mode 20 via QueryC, then retries offset-adjusted copy via QueryA mode 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckOffsetQueryC(void* gameState) {
    uint64_t stateCopy = *(uint64_t*)this;

    bool initialCheck = pongCameraMgr_QueryC(&stateCopy, 20);
    if (!initialCheck) {
        return false;
    }

    float currentValue = *(float*)this;
    float adjustedValue = currentValue + g_cameraOffsetDelta;
    *(float*)&stateCopy = adjustedValue;

    return pongCameraMgr_QueryA(&stateCopy, 3);
}

// ── Pattern N: Elapsed time threshold checks (108B each, 3 functions) ───────

extern uint32_t g_cameraTransitionState;  // @ 0x826064C0 (.data, 4B)
extern float    g_cameraTransitionTime;   // @ 0x826064BC (.data, 4B)
extern uint32_t g_cameraDataPtr;          // @ 0x8271A2EC (.data, 4B - pointer to game data)

// Float thresholds in .rdata used by each variant:
extern float g_cameraElapsedThreshold1;   // @ 0x825F8080 (.rdata)
extern float g_cameraElapsedThreshold2;   // @ 0x825F9E78 (.rdata)
extern float g_cameraElapsedThreshold3;   // @ 0x825FD10C (.rdata)

static const uint32_t CAMERA_DATA_ELAPSED_OFFSET = 0x68CE4;

/**
 * pongCameraMgr elapsed time checks @ 0x821F76C8-77A8 | 108B each
 *
 * Checks if the camera transition state is not "active" (state 2).
 * If not active, sets state to 1, reads the elapsed game time from a
 * global data structure, subtracts the stored transition time, and
 * compares against a per-function threshold. Returns true if enough
 * time has elapsed.
 */

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::HasElapsedThreshold1()  @ 0x821F76C8 | size: 0x6C
// Checks elapsed time against threshold 1
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::HasElapsedThreshold1() {
    if (g_cameraTransitionState == 2) {
        return false;
    }

    g_cameraTransitionState = 1;

    // Read elapsed time from game data at fixed offset
    float* dataBase = reinterpret_cast<float*>(g_cameraDataPtr);
    float elapsedTime = *(float*)((char*)dataBase + CAMERA_DATA_ELAPSED_OFFSET);
    float timeDelta = elapsedTime - g_cameraTransitionTime;

    return (timeDelta >= g_cameraElapsedThreshold1);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::HasElapsedThreshold2()  @ 0x821F7738 | size: 0x6C
// Checks elapsed time against threshold 2
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::HasElapsedThreshold2() {
    if (g_cameraTransitionState == 2) {
        return false;
    }

    g_cameraTransitionState = 1;

    float* dataBase = reinterpret_cast<float*>(g_cameraDataPtr);
    float elapsedTime = *(float*)((char*)dataBase + CAMERA_DATA_ELAPSED_OFFSET);
    float timeDelta = elapsedTime - g_cameraTransitionTime;

    return (timeDelta >= g_cameraElapsedThreshold2);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::HasElapsedThreshold3()  @ 0x821F77A8 | size: 0x6C
// Checks elapsed time against threshold 3
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::HasElapsedThreshold3() {
    if (g_cameraTransitionState == 2) {
        return false;
    }

    g_cameraTransitionState = 1;

    float* dataBase = reinterpret_cast<float*>(g_cameraDataPtr);
    float elapsedTime = *(float*)((char*)dataBase + CAMERA_DATA_ELAPSED_OFFSET);
    float timeDelta = elapsedTime - g_cameraTransitionTime;

    return (timeDelta >= g_cameraElapsedThreshold3);
}

// ── Pattern O: Clamped Transition Functions (14 variants) ───────────────────
// All share the doClampedTransition helper; differ in mode, flags, r7, and byte check.

bool pongCameraMgr::TransitionMode6_V1(void* gameState) {  // @ 0x821F5188
    uint32_t f[] = {3, 0, 5, 0};
    return doClampedTransition(this, gameState, 6, f, 2);
}

bool pongCameraMgr::TransitionMode6_V2(void* gameState) {  // @ 0x821F5248
    uint32_t f[] = {3, 1, 5, 0};
    return doClampedTransition(this, gameState, 6, f, 2);
}

bool pongCameraMgr::TransitionMode6_V3(void* gameState) {  // @ 0x821F5308
    uint32_t f[] = {5, 0};
    return doClampedTransition(this, gameState, 6, f, 1);
}

bool pongCameraMgr::TransitionMode6_V4(void* gameState) {  // @ 0x821F53B8
    uint32_t f[] = {5, 1};
    return doClampedTransition(this, gameState, 6, f, 1);
}

bool pongCameraMgr::TransitionMode11_V1(void* gameState) {  // @ 0x821F5608
    uint32_t f[] = {3, 0, 1, 0};
    return doClampedTransition(this, gameState, 11, f, 2);
}

bool pongCameraMgr::TransitionMode11_V2(void* gameState) {  // @ 0x821F56C8
    uint32_t f[] = {3, 1, 1, 0};
    return doClampedTransition(this, gameState, 11, f, 2);
}

bool pongCameraMgr::TransitionMode11_V3(void* gameState) {  // @ 0x821F5788
    uint32_t f[] = {3, 0, 1, 1};
    return doClampedTransition(this, gameState, 11, f, 2);
}

bool pongCameraMgr::TransitionMode11_V4(void* gameState) {  // @ 0x821F5848
    uint32_t f[] = {3, 1, 1, 1};
    return doClampedTransition(this, gameState, 11, f, 2);
}

bool pongCameraMgr::TransitionMode19_V1(void* gameState) {  // @ 0x821F4C78
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector != 0) ? 1 : 0;
    uint32_t f[] = {1, 0, 2, b};
    return doClampedTransition(this, gameState, 19, f, 2);
}

bool pongCameraMgr::TransitionMode19_V2(void* gameState) {  // @ 0x821F4D48
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector != 0) ? 1 : 0;
    uint32_t f[] = {1, 1, 2, b};
    return doClampedTransition(this, gameState, 19, f, 2);
}

bool pongCameraMgr::TransitionMode19_V3(void* gameState) {  // @ 0x821F4E18
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector == 0) ? 1 : 0;
    uint32_t f[] = {1, 0, 2, b};
    return doClampedTransition(this, gameState, 19, f, 2);
}

bool pongCameraMgr::TransitionMode19_V4(void* gameState) {  // @ 0x821F4EE8
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector == 0) ? 1 : 0;
    uint32_t f[] = {1, 1, 2, b};
    return doClampedTransition(this, gameState, 19, f, 2);
}

bool pongCameraMgr::TransitionMode12_V1(void* gameState) {  // @ 0x821F5D38
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector != 0) ? 1 : 0;
    uint32_t f[] = {3, 0, 1, 0, 2, b};
    return doClampedTransition(this, gameState, 12, f, 3);
}

bool pongCameraMgr::TransitionMode12_V2(void* gameState) {  // @ 0x821F5E18
    extern uint8_t g_cameraSelector;
    uint32_t b = (g_cameraSelector != 0) ? 1 : 0;
    uint32_t f[] = {3, 1, 1, 0, 2, b};
    return doClampedTransition(this, gameState, 12, f, 3);
}


// ═══════════════════════════════════════════════════════════════════════════
// Multi-call camera transition functions
// ═══════════════════════════════════════════════════════════════════════════

// Forward declarations for callees
extern "C" bool pongCameraMgr_3ED8_2h(void* statePtr, void* gameState, int extra);
extern "C" bool pg_6F68(void* gameState, void* params, int mode, unsigned int* flags, int enable);
extern "C" bool pongCameraMgr_6990(void* statePtr, void* gameState, float f1, float f2);
extern "C" bool pongCameraMgr_5308_fw(void* statePtr, void* gameState);
extern "C" bool pongCameraMgr_6EC0(void* statePtr, void* gameState);
extern "C" bool pongCameraMgr_6FA8(void* statePtr, void* gameState, float f1, float f2);
extern "C" bool pongCameraMgr_7250(void* statePtr, void* gameState, float f1, float f2);
extern "C" bool pongCameraMgr_7338_helper(void* statePtr, void* gameState, float f1, float f2);

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_3498_g  @ 0x821F3498 | size: 0x64
// Initializes a 24-byte camera state struct with a value and fixed pointers
// ─────────────────────────────────────────────────────────────────────────────
struct CameraStateBlock {
    uint32_t value;          // +0x00
    uint32_t funcPtr;        // +0x04
    uint32_t data[4];        // +0x08..+0x14 (copied from stack template)
};

extern "C" void pongCameraMgr_3498_g(CameraStateBlock* out, uint32_t value) {
    extern uint32_t g_camStateTemplate[4];  // @ stack-based template constants
    out->value = value;
    // The scaffold stores fixed addresses at +4 and copies a 16-byte template at +8
    // These are resolved link-time constants from the original binary
    out->funcPtr = 0x8240E6D0;  // addi r9,-6448 from lis r9,-32191
    out->data[0] = 0x8240E6D0;
    out->data[1] = 0;
    out->data[2] = 0;
    out->data[3] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryDualTransition3ED8()  @ 0x821F4000 | size: 0x80
// Calls pongCameraMgr_3ED8_2h twice (extra=0 then extra=1), picks min result,
// returns true if either call succeeded
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryDualTransition3ED8(void* gameState) {
    // Create two local copies of the first 8 bytes (float value + state)
    uint64_t copy1 = *(uint64_t*)this;
    uint64_t copy2 = copy1;

    // Call 3ED8 with extra=0 on first copy
    bool result1 = pongCameraMgr_3ED8_2h(&copy1, gameState, 0);

    // Call 3ED8 with extra=1 on second copy
    bool result2 = pongCameraMgr_3ED8_2h(&copy2, gameState, 1);

    // Pick min of the two float results using fsel pattern
    float val1 = *(float*)&copy1;
    float val2 = *(float*)&copy2;
    float diff = val1 - val2;
    float minVal = (diff >= 0.0f) ? val2 : val1;

    // Store the minimum value back
    *(float*)this = minVal;

    // Return true if either call succeeded
    if (!result1) {
        return (result2 & 0xFF) != 0;
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionPg6F68_Mode0()  @ 0x821F4080 | size: 0xFC
// Calls pg_6F68(mode=5, flags={3,0}), then pongCameraMgr_6E08(mode=3), 
// then pongCameraMgr_6E08(mode=5) with offset-adjusted value
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionPg6F68_Mode0(void* gameState) {
    extern float g_cameraTransitionOffset;  // @ 0x8202D168

    // Prepare flags: {3, 0}
    uint32_t flags[2] = { 3, 0 };

    // Copy first 8 bytes as transition params
    TransitionParams params;
    *(uint64_t*)&params = *(uint64_t*)this;

    // First call: pg_6F68 with mode=5
    if (!pg_6F68(gameState, &params, 5, flags, 1)) {
        return false;
    }

    // Reload state and call pongCameraMgr_6E08 with mode=3
    TransitionParams params2;
    *(uint64_t*)&params2 = *(uint64_t*)&params;
    if (!pongCameraMgr_6E08(gameState, &params2, 3, flags, 1)) {
        return false;
    }

    // Save the result value from first 6E08 call
    float resultValue = params.value;

    // Adjust value by adding offset, call 6E08 with mode=5
    params2.value = resultValue + g_cameraTransitionOffset;
    if (pongCameraMgr_6E08(gameState, &params2, 5, flags, 1)) {
        // Third call succeeded - check if result < original
        float result3 = params2.value;
        if (result3 < resultValue) {
            // Result went down, use original
            *(float*)this = resultValue;
            return true;
        }
        return false;
    }

    // Third call failed - store original result
    *(float*)this = resultValue;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryTransitionPg6F68_Mode1()  @ 0x821F4180 | size: 0xFC
// Same as TryTransitionPg6F68_Mode0 but with flags={3, 1}
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryTransitionPg6F68_Mode1(void* gameState) {
    extern float g_cameraTransitionOffset;  // @ 0x8202D168

    // Prepare flags: {3, 1}
    uint32_t flags[2] = { 3, 1 };

    TransitionParams params;
    *(uint64_t*)&params = *(uint64_t*)this;

    if (!pg_6F68(gameState, &params, 5, flags, 1)) {
        return false;
    }

    TransitionParams params2;
    *(uint64_t*)&params2 = *(uint64_t*)&params;
    if (!pongCameraMgr_6E08(gameState, &params2, 3, flags, 1)) {
        return false;
    }

    float resultValue = params.value;
    params2.value = resultValue + g_cameraTransitionOffset;
    if (pongCameraMgr_6E08(gameState, &params2, 5, flags, 1)) {
        float result3 = params2.value;
        if (result3 < resultValue) {
            *(float*)this = resultValue;
            return true;
        }
        return false;
    }

    *(float*)this = resultValue;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryClampedTransition_Mode0()  @ 0x821F4280 | size: 0x124
// Clamps value to threshold, then triple-calls 6E08 with modes 5, 3, 5
// flags = {3, 0}
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryClampedTransition_Mode0(void* gameState) {
    extern float g_cameraThreshold;         // @ 0x825C5EC0
    extern float g_cameraTransitionOffset;  // @ 0x8202D168

    float currentValue = *(float*)this;
    float threshold = g_cameraThreshold;

    // fsel clamping: pick min(currentValue, threshold) and max
    float diffMin = threshold - currentValue;
    float diffMax = currentValue - threshold;
    float clampedMin = (diffMin >= 0.0f) ? currentValue : threshold;  // min
    float clampedMax = (diffMax >= 0.0f) ? currentValue : threshold;  // max

    // Prepare transition flags: {3, 0}
    uint32_t transFlags[2] = { 3, 0 };

    TransitionParams params;
    params.value = clampedMin;
    params.vtableOrState = this->flags;  // copy struct flags field (+0x04)

    // First call: 6E08 with mode=5
    if (!pongCameraMgr_6E08(gameState, &params, 5, transFlags, 1)) {
        return false;
    }

    float result1 = params.value;
    if (result1 > clampedMax) {
        return false;
    }

    // Second call: 6E08 with mode=3
    *(uint64_t*)&params = *(uint64_t*)&params;  // reload
    if (!pongCameraMgr_6E08(gameState, &params, 3, transFlags, 1)) {
        return false;
    }

    float resultValue = result1;

    // Third call: 6E08 with mode=5 using offset-adjusted value
    params.value = resultValue + g_cameraTransitionOffset;
    if (pongCameraMgr_6E08(gameState, &params, 5, transFlags, 1)) {
        float result3 = params.value;
        if (result3 < resultValue) {
            *(float*)this = resultValue;
            return true;
        }
        return false;
    }

    *(float*)this = resultValue;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TryClampedTransition_Mode1()  @ 0x821F43A8 | size: 0x124
// Same as TryClampedTransition_Mode0 but with flags={3, 1}
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::TryClampedTransition_Mode1(void* gameState) {
    extern float g_cameraThreshold;         // @ 0x825C5EC0
    extern float g_cameraTransitionOffset;  // @ 0x8202D168

    float currentValue = *(float*)this;
    float threshold = g_cameraThreshold;

    float diffMin = threshold - currentValue;
    float diffMax = currentValue - threshold;
    float clampedMin = (diffMin >= 0.0f) ? currentValue : threshold;
    float clampedMax = (diffMax >= 0.0f) ? currentValue : threshold;

    uint32_t transFlags[2] = { 3, 1 };

    TransitionParams params;
    params.value = clampedMin;
    params.vtableOrState = this->flags;  // copy struct flags field (+0x04)

    if (!pongCameraMgr_6E08(gameState, &params, 5, transFlags, 1)) {
        return false;
    }

    float result1 = params.value;
    if (result1 > clampedMax) {
        return false;
    }

    *(uint64_t*)&params = *(uint64_t*)&params;
    if (!pongCameraMgr_6E08(gameState, &params, 3, transFlags, 1)) {
        return false;
    }

    float resultValue = result1;
    params.value = resultValue + g_cameraTransitionOffset;
    if (pongCameraMgr_6E08(gameState, &params, 5, transFlags, 1)) {
        float result3 = params.value;
        if (result3 < resultValue) {
            *(float*)this = resultValue;
            return true;
        }
        return false;
    }

    *(float*)this = resultValue;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckTransition_Near_Mode0()  @ 0x821F83B8 | size: 0x6C
// Calls 6E08 with flags={3,0}, checks if result > threshold
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckTransition_Near_Mode0(void* gameState, float threshold) {
    uint32_t flags[2] = { 3, 0 };

    TransitionParams params;
    *(uint64_t*)&params = *(uint64_t*)this;

    if (pongCameraMgr_6E08(gameState, &params, 5, flags, 1)) {
        float result = params.value;
        if (result <= threshold) {
            return false;
        }
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckTransition_Near_Mode1()  @ 0x821F8428 | size: 0x6C
// Same as Mode0 but with flags={3,1}
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckTransition_Near_Mode1(void* gameState, float threshold) {
    uint32_t flags[2] = { 3, 1 };

    TransitionParams params;
    *(uint64_t*)&params = *(uint64_t*)this;

    if (pongCameraMgr_6E08(gameState, &params, 5, flags, 1)) {
        float result = params.value;
        if (result <= threshold) {
            return false;
        }
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckAllNearTransitions_Mode0()  @ 0x821F8498 | size: 0x140
// Calls CheckTransition_Near_Mode0 with 7 different mode indices (7-13 excl 14),
// using threshold = g_cameraThreshold + f2. Returns true only if ALL succeed.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckAllNearTransitions_Mode0(void* gameState, float f2) {
    extern float g_cameraThreshold;  // @ 0x825C5EC0
    float threshold = g_cameraThreshold + f2;

    uint64_t stateCopy;
    int modes[] = { 7, 8, 9, 10, 13, 11, 12 };
    bool results[7];

    for (int i = 0; i < 7; i++) {
        stateCopy = *(uint64_t*)this;
        // Call CheckTransition_Near_Mode0 pattern inline
        uint32_t flags[2] = { 3, 0 };
        TransitionParams params;
        *(uint64_t*)&params = stateCopy;
        if (pongCameraMgr_6E08(gameState, &params, modes[i], flags, 1)) {
            results[i] = (params.value > threshold);
        } else {
            results[i] = true;
        }
    }

    // Return true only if ALL checks passed
    for (int i = 0; i < 7; i++) {
        if (!results[i]) return false;
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckAllNearTransitions_Mode1()  @ 0x821F85D8 | size: 0x140
// Same as Mode0 but uses flags={3,1} via CheckTransition_Near_Mode1
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckAllNearTransitions_Mode1(void* gameState, float f2) {
    extern float g_cameraThreshold;  // @ 0x825C5EC0
    float threshold = g_cameraThreshold + f2;

    uint64_t stateCopy;
    int modes[] = { 7, 8, 9, 10, 13, 11, 12 };
    bool results[7];

    for (int i = 0; i < 7; i++) {
        stateCopy = *(uint64_t*)this;
        uint32_t flags[2] = { 3, 1 };
        TransitionParams params;
        *(uint64_t*)&params = stateCopy;
        if (pongCameraMgr_6E08(gameState, &params, modes[i], flags, 1)) {
            results[i] = (params.value > threshold);
        } else {
            results[i] = true;
        }
    }

    for (int i = 0; i < 7; i++) {
        if (!results[i]) return false;
    }
    return true;
}

// Forward declaration needed before CheckAllCameraQueries
extern bool pongCameraMgr_3500_fw(pongCameraMgr* mgr, void* gameState, int mode);

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CheckAllCameraQueries()  @ 0x821F8250 | size: 0x164
// Master query: calls 7 different query functions, returns true if ANY succeeded
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::CheckAllCameraQueries(void* gameState, float f1, float f2) {
    // 1. ValidateAndClampTransition mode 21
    uint64_t stateCopy = *(uint64_t*)this;
    bool r1 = pongCameraMgr_3500_fw(reinterpret_cast<pongCameraMgr*>(&stateCopy), gameState, 21);

    // 2. pongCameraMgr_6990
    stateCopy = *(uint64_t*)this;
    bool r2 = pongCameraMgr_6990(&stateCopy, gameState, f1, f2);

    // 3. pongCameraMgr_5308_fw
    stateCopy = *(uint64_t*)this;
    bool r3 = pongCameraMgr_5308_fw(&stateCopy, gameState);

    // 4. pongCameraMgr_6EC0
    stateCopy = *(uint64_t*)this;
    bool r4 = pongCameraMgr_6EC0(&stateCopy, gameState);

    // 5. pongCameraMgr_6FA8
    stateCopy = *(uint64_t*)this;
    bool r5 = pongCameraMgr_6FA8(&stateCopy, gameState, f1, f2);

    // 6. pongCameraMgr_7250
    stateCopy = *(uint64_t*)this;
    bool r6 = pongCameraMgr_7250(&stateCopy, gameState, f1, f2);

    // 7. pongCameraMgr_7338
    stateCopy = *(uint64_t*)this;
    bool r7 = pongCameraMgr_7338_helper(&stateCopy, gameState, f1, f2);

    // Return true if ANY query succeeded (OR logic)
    return r1 || r2 || r3 || r4 || r5 || r6 || r7;
}

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations for helpers used below
// ─────────────────────────────────────────────────────────────────────────────
extern void sysMemAllocator_Free(void* ptr);                        // @ 0x820C00C0
extern void atSingleton_9420(void* obj);                      // @ 0x821A9420
extern void pongCameraMgr_3E98_g(void* obj);                  // @ 0x82143E98 (GetAspectRatio)
extern void phBoundCapsule_3598_g(void* obj);                  // @ 0x82153598
extern void pongCameraMgr_39F0_g(void* dst, void* src);       // @ 0x821539F0
extern uint8_t pongCameraMgr_C450_g(void* node);              // @ 0x8220C450
extern void pongCameraMgr_7EA8_g(void* mgr, void* state, int flag); // @ 0x82167EA8
extern void pg_7CE0(void* mgr, void* node);                   // @ 0x82167CE0
extern void game_85F0(void* flags);                            // @ 0x821685F0
extern bool pongCameraMgr_3500_fw(pongCameraMgr* mgr, void* gameState, int mode);  // @ 0x821F3500
extern void pongCameraMgr_7DF0_2hr(void* selfPtr, void* statePtr, int index);      // @ 0x82167DF0
extern void pongCameraMgr_8CB0_2hr(void* selfPtr);                                 // @ 0x82168CB0

// Globals referenced by address
extern uint32_t g_cameraMgrSingleton;   // @ 0x8260641C  (lis r7,-32160 + 25628)
extern uint32_t g_cameraActiveIndex;    // @ 0x8260645C  (lis r10,-32160 + 26076)
extern uint8_t  g_cameraResetFlag;      // @ 0x8260640A  (lis r8,-32160 + 25626)
extern uint32_t g_cameraLerpCounter;    // @ 0x826064A0  (lis r10,-32160 + 25792)
extern uint32_t g_cameraLerpState;      // @ 0x826064A4  (lis r10,-32160 + 25796)
extern float    g_cameraLerpValue;      // @ 0x826064BC  (lis r10,-32160 + 25788)
extern uint32_t g_cameraLerpFlags;      // @ 0x826064D0  (lis r10,-32160 + 25808)
extern uint32_t g_cameraDataBasePtr;    // @ 0x8271A2EC  (global data base)
extern float    g_cameraFloatTable[];   // @ data table base (lis r10,-32253 + offset)

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::~pongCameraMgr()  [vfn_0] @ 0x821658E0 | size: 0x80
// Destructor: sets vtables, clears singleton, calls base dtor, optionally frees
// ─────────────────────────────────────────────────────────────────────────────
pongCameraMgr::~pongCameraMgr() {
    // Vtable writes (secondary at +0x10, then base at +0x00) handled by compiler.
    // Clear the global singleton pointer.
    g_cameraMgrSingleton = 0;
    atSingleton_9420(this);
    // Note: sysMemAllocator_Free called conditionally based on deletion mode flag,
    // which the compiler handles via the deleting-destructor thunk.
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::vfn_25()  [vtable slot 25] @ 0x82166068 | size: 0xAC
// Resets camera state: calls slot-8 vcall on m_cameraObj, zeroes sub-fields,
// writes default floats, updates global camera pointer
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::vfn_25() {
    void* camObj = m_cameraObj;   // +0x28
    m_resetCounter = 0;           // +0x38 = 0

    // Virtual call slot 8 on camObj
    typedef void (*VFn8)(void*);
    void** vt = *(void***)camObj;
    ((VFn8)vt[8])(camObj);

    // Load sub-state ptr from +0x2C
    uint32_t* subState = (uint32_t*)m_subState;  // +0x2C
    subState[1] = 0;  // +4
    subState[2] = 0;  // +8

    // Load default float value from data table
    extern float g_cameraDefaultFOV;  // @ const table
    float defVal = g_cameraDefaultFOV;

    // Write default to sub-state->field_16->+8,+12 and sub-state->field_20->+8,+12
    uint32_t* inner1 = *(uint32_t**)(subState + 4);  // +16
    *(float*)((char*)inner1 + 8) = defVal;
    *(float*)((char*)inner1 + 12) = defVal;

    uint32_t* inner2 = *(uint32_t**)(subState + 5);  // +20
    *(float*)((char*)inner2 + 8) = defVal;
    *(float*)((char*)inner2 + 12) = defVal;

    // Update global camera pointer from camObj->+44->+16
    void* camObj2 = m_cameraObj;  // +0x28
    uint32_t* camData = *(uint32_t**)((char*)camObj2 + 44);
    extern uint32_t* g_globalCamPtr1;  // @ global
    extern uint32_t* g_globalCamPtr2;  // @ global
    *g_globalCamPtr1 = (uint32_t)(uintptr_t)((char*)camData + 16);

    // Virtual call slot 3 on g_globalCamPtr2
    typedef void (*VFn3)(void*);
    void** vt2 = *(void***)g_globalCamPtr2;
    ((VFn3)vt2[3])((void*)((char*)g_globalCamPtr2 + 4));
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_6280_g  @ 0x82166280 | size: 0x90
// Gets camera data from active camera, copies FOV/aspect data into indexed slot
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr_6280_g(void* selfPtr) {
    pongCameraMgr* self = (pongCameraMgr*)selfPtr;
    void* camObj = self->m_cameraObj;                            // +0x28
    uint32_t* camData = *(uint32_t**)((char*)camObj + 44);
    pongCameraMgr* activeCamera = (pongCameraMgr*)camData[38];   // +152

    float fovY = activeCamera->m_fovY;                           // +0x344
    float fovX = activeCamera->m_fovX;                           // +0x340

    // Get aspect ratio (result returned in f1)
    pongCameraMgr_3E98_g(activeCamera);

    // Compute destination slot: self->m_slotsBase + g_cameraActiveIndex * 912
    pongCameraMgr* slotsBase = (pongCameraMgr*)self->m_slotsBase;  // +0x18
    int idx = (int)g_cameraActiveIndex;
    char* dest = (char*)slotsBase + idx * 912;
    pongCameraMgr* destSlot = (pongCameraMgr*)dest;

    destSlot->m_fovParam = activeCamera->m_fovParam;             // +0x338
    destSlot->m_activeFlag = 1;                                  // +0x360
    // f1 holds aspect ratio from pongCameraMgr_3E98_g
    // destSlot->m_verticalFOV = aspectResult;  // stored by caller convention
    destSlot->m_fovX = fovX;
    destSlot->m_fovY = fovY;

    phBoundCapsule_3598_g(dest);

    // Copy matrix: activeCamera+64 -> dest (recomputed slot)
    idx = (int)g_cameraActiveIndex;
    slotsBase = (pongCameraMgr*)self->m_slotsBase;
    dest = (char*)slotsBase + idx * 912;
    pongCameraMgr_39F0_g(dest, (char*)activeCamera + 64);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraState::~pongCameraState()  [vfn_0] @ 0x82167770 | size: 0x78
// Destructor: sets vtable, destroys child at +0x10 if non-null, optionally frees
// ─────────────────────────────────────────────────────────────────────────────
pongCameraState::~pongCameraState() {
    // Vtable write handled by compiler.
    // If child object at +0x10 is non-null, destroy it (vcall slot 0 with delete flag)
    void* child = m_child;  // +0x10
    if (child != nullptr) {
        typedef void (*DtorFn)(void*, int);
        void** vt = *(void***)child;
        ((DtorFn)vt[0])(child, 1);
    }
    // sysMemAllocator_Free called conditionally by deleting-destructor thunk
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_77E8_w  @ 0x821677E8 | size: 0xB8
// Walks camera node tree; returns 1 if a node with byte+28 set is found, else 0
// ─────────────────────────────────────────────────────────────────────────────
int pongCameraMgr_77E8_w(void* selfPtr, void* statePtr) {
    uint32_t* self = (uint32_t*)selfPtr;

    // Lookup current node: self->+4 (node table ptr) -> +48[self->+8 (index)]
    uint32_t* nodeTablePtr = *(uint32_t**)((char*)self + 4);   // +0x04
    int curTime = *(int*)((char*)self + 8);                    // +0x08
    uint32_t* nodeTable = *(uint32_t**)((char*)nodeTablePtr + 48);
    void* node = (void*)nodeTable[curTime];

    if (!pongCameraMgr_C450_g(node))
        return 0;

    while (true) {
        // Check if node has children (halfword at +36)
        uint16_t childCount = *(uint16_t*)((char*)node + 36);
        if (childCount != 0)
            return 0;

        // Check byte flag at +28
        uint8_t flagByte = *(uint8_t*)((char*)node + 28);
        if (flagByte != 0)
            return 1;

        // Traverse: node->+40->+16 gives index, statePtr->+32 gives table
        uint32_t* link = *(uint32_t**)((char*)node + 40);
        uint32_t* table2 = *(uint32_t**)((char*)statePtr + 32);
        int linkIdx = *(int*)((char*)link + 16);
        uint32_t* entry = (uint32_t*)table2[linkIdx];
        uint32_t* nextFlags = *(uint32_t**)((char*)entry + 4);
        int nextIdx = *(int*)((char*)entry + 8);
        uint32_t* nextNodeTable = *(uint32_t**)((char*)nextFlags + 48);
        node = (void*)nextNodeTable[nextIdx];

        if (!pongCameraMgr_C450_g(node))
            return 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_7C18  @ 0x82167C18 | size: 0xC4
// Sets up camera transition: clears globals, stores lerp value, dispatches
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr_7C18(void* selfPtr, void* nodePtr, void* statePtr, uint8_t immediateFlag) {
    // Compute lerp value from data table (offset 0x68CE4 = 6*65536+36068)
    float lerpVal = *(float*)(g_cameraDataBasePtr + 0x68CE4);

    // Clear transition globals
    g_cameraLerpCounter = 0;
    g_cameraLerpState = 0;
    g_cameraLerpValue = lerpVal;
    g_cameraLerpFlags = 0;

    // Call recursive hierarchy setup
    pongCameraMgr_7DF0_2hr(selfPtr, statePtr, *(int*)((char*)nodePtr + 16));

    if (immediateFlag) {
        // Immediate transition: flag=0
        pongCameraMgr_7EA8_g(selfPtr, statePtr, 0);
        return;
    }

    // Check if current node byte+28 is zero (inactive)
    uint8_t active = *(uint8_t*)((char*)nodePtr + 28);
    if (active == 0) {
        // Walk to linked node: nodePtr->+40->+16 gives index into statePtr->+32
        uint32_t* link = *(uint32_t**)((char*)nodePtr + 40);
        uint32_t* table = *(uint32_t**)((char*)statePtr + 32);
        int linkIdx = *(int*)((char*)link + 16);
        void* linkedMgr = (void*)table[linkIdx];
        pongCameraMgr_7EA8_g(linkedMgr, statePtr, 1);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_7DF0_2hr  @ 0x82167DF0 | size: 0xB4
// Recursively sets camera index and processes child nodes
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr_7DF0_2hr(void* selfPtr, void* statePtr, int index) {
    uint32_t* self = (uint32_t*)selfPtr;

    // Store index at self->+8, look up node from self->+4->+48[index]
    *(int*)((char*)self + 8) = index;
    uint32_t* nodeTablePtr = *(uint32_t**)((char*)self + 4);  // +0x04
    uint32_t* nodeTable = *(uint32_t**)((char*)nodeTablePtr + 48);
    void* node = (void*)nodeTable[index];

    // If node byte+28 is zero (inactive), recurse through its link
    uint8_t active = *(uint8_t*)((char*)node + 28);
    if (active == 0) {
        uint32_t* link = *(uint32_t**)((char*)node + 40);
        uint32_t* table = *(uint32_t**)((char*)statePtr + 32);
        int linkIdx = *(int*)((char*)link + 16);
        uint32_t* entry = (uint32_t*)table[linkIdx];
        void* linkedMgr = (void*)entry;
        int linkedIdx = *(int*)((char*)link + 16);  // re-read
        pongCameraMgr_7DF0_2hr(linkedMgr, statePtr, linkedIdx);
    }

    // Process children: iterate node->+36 (child count), node->+32 (child array)
    int16_t childCount = *(int16_t*)((char*)node + 36);
    if (childCount > 0) {
        for (int i = 0; i < childCount; i++) {
            uint32_t* childArray = *(uint32_t**)((char*)node + 32);
            void* childEntry = (void*)childArray[i];
            void* childNode = *(void**)((char*)childEntry + 52);

            uint8_t childActive = *(uint8_t*)((char*)childNode + 28);
            if (childActive == 0) {
                void* childLink = *(void**)((char*)childNode + 40);
                pg_7CE0(statePtr, childLink);
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_8C10  @ 0x82168C10 | size: 0xA0
// Resets camera state: clears indices, resets globals, processes hierarchy
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr_8C10(void* selfPtr) {
    uint32_t* self = (uint32_t*)selfPtr;

    // Compute lerp value from data table
    float lerpVal = *(float*)(g_cameraDataBasePtr + 0x68CE4);

    // Clear reset flag
    g_cameraResetFlag = 0;

    // Reset indices at +0x10, +0x14, +0x18 to -1
    *(int*)((char*)self + 16) = -1;   // +0x10
    *(int*)((char*)self + 20) = -1;   // +0x14
    *(int*)((char*)self + 24) = -1;   // +0x18

    // Clear lerp globals
    g_cameraLerpCounter = 0;
    g_cameraLerpState = 0;
    g_cameraLerpFlags = 0;
    g_cameraLerpValue = lerpVal;

    // Process hierarchy: self->+4 (node table ptr) as param
    void* nodeTablePtr = *(void**)((char*)self + 4);   // +0x04
    pongCameraMgr_7DF0_2hr(nodeTablePtr, selfPtr, 0);  // index 0

    // Apply transition with flag=1
    nodeTablePtr = *(void**)((char*)self + 4);
    pongCameraMgr_7EA8_g(nodeTablePtr, selfPtr, 1);

    // Finalize
    pongCameraMgr_8CB0_2hr(selfPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr_8CB0_2hr  @ 0x82168CB0 | size: 0x88
// Applies camera configuration from data tables; copies float params if reset
// flag was set, then tail-calls game_85F0
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr_8CB0_2hr(void* selfPtr) {
    uint8_t resetFlag = g_cameraResetFlag;

    // Load data table base and float table pointer
    // r10 = base of float constants (lis r11,-32253 + addi -12020)
    // r9 = g_cameraDataBasePtr

    if (resetFlag != 0) {
        // Copy a block of float params from the const table into the data region
        // Destination: g_cameraDataBasePtr + 0x78CE4 (offset 7*65536 - 29404)
        char* destBase = (char*)(g_cameraDataBasePtr + 0x78CE4 - 29404);
        extern float g_cameraConstTable[];  // base of float constants

        // Copy 7 float values
        *(float*)(destBase + 0)  = g_cameraConstTable[0];   // f10
        *(float*)(destBase + 4)  = g_cameraConstTable[1];   // f11
        *(float*)(destBase + 8)  = g_cameraConstTable[7];   // f13
        *(float*)(destBase + 12) = g_cameraConstTable[-1];  // f0 (offset -4)
        *(float*)(destBase + 16) = g_cameraConstTable[-1];  // f0
        *(float*)(destBase + 20) = g_cameraConstTable[30];  // f12 (offset 120)
        *(float*)(destBase + 24) = g_cameraConstTable[-1];  // f0

        g_cameraResetFlag = 0;

        // Also store f0 at g_cameraDataBasePtr + 0x68CEC (offset 36076)
        *(float*)(g_cameraDataBasePtr + 0x68CEC) = g_cameraConstTable[-1];
    }

    // Store a float value at g_cameraDataBasePtr + 0x68D04 (offset 36100)
    extern float g_cameraFinalFloat;  // @ const table offset -3112
    *(float*)(g_cameraDataBasePtr + 0x68D04) = g_cameraFinalFloat;

    // Tail-call: game_85F0 with self->+4 (node table ptr)
    void* nodeTablePtr = *(void**)((char*)selfPtr + 4);  // +0x04
    game_85F0(nodeTablePtr);
}

// ═══════════════════════════════════════════════════════════════════════════
// Pattern O: Clamped transition functions (14 functions)
//
// All follow: clamp *(float*)this to g_cameraThreshold, call 6E08/pg_6F68,
// store result if in range.
//
// Parameter table:
// Func     | Type       | Mode1 | Flags1           | Mode2 | Flags2 | r7 | ByteChk
// 5EF8     | single     | 12    | {3,0,1,1,2,S}    | -     | -      | 3  | S=sel
// 5FD8     | single     | 12    | {3,1,1,1,2,S}    | -     | -      | 3  | S=sel
// 60B0     | single     | 12    | {3,0,1,0,2,I}    | -     | -      | 3  | I=!sel
// 6188     | single     | 12    | {3,1,1,0,2,I}    | -     | -      | 3  | I=!sel
// 6260     | single     | 12    | {3,0,1,1,2,I}    | -     | -      | 3  | I=!sel
// 6338     | single     | 12    | {3,1,1,1,2,I}    | -     | -      | 3  | I=!sel
// 6D00     | dual       | 10    | {3,0,3,1}        | 16    | {3,1}  | 1  | -
// 6DE0     | dual       | 10    | {3,1,3,0}        | 16    | {3,0}  | 1  | -
// 6EC0     | 6E08+6F68  | 10    | {3,0}            | 16    | {3,0}  | 1  | -
// 6FA8     | 6E08+6F68  | 10    | {3,1}            | 16    | {3,1}  | 1  | -
// 7090     | dual       | 7     | {3,0,3,1}        | 16    | {3,1}  | 1  | -
// 7170     | dual       | 7     | {3,1,3,0}        | 16    | {3,0}  | 1  | -
// 7250     | 6E08+6F68  | 7     | {3,0}            | 16    | {3,0}  | 1  | -
// 3ED8_2h  | triple     | 5,3,5 | {3,E}            | -     | -      | 1  | E=extra
//
// S = (g_cameraSelector != 0) ? 1 : 0
// I = (g_cameraSelector == 0) ? 1 : 0
// ═══════════════════════════════════════════════════════════════════════════

// Helper: single 6E08 call with 6-field extended flags, mode=12, r7=3
static bool clampedTransition12(void* self, void* gameState, const uint32_t flags[6]) {
    extern float g_cameraThreshold;
    float cur = *(float*)self;
    float thr = g_cameraThreshold;
    float dMin = thr - cur, dMax = cur - thr;
    float cMin = (dMin >= 0.0f) ? cur : thr;
    float cMax = (dMax >= 0.0f) ? cur : thr;

    TransitionParams params;
    params.value = cMin;
    *(uint32_t*)((char*)&params + 4) = *(uint32_t*)((char*)self + 4);

    if (!pongCameraMgr_6E08(gameState, &params, 12, (void*)flags, 3)) return false;
    if (params.value > cMax) return false;
    *(float*)self = params.value;
    return true;
}

// Helper: dual 6E08 call — first with mode1 + 4-field flags, second with mode=16 + flags[2..3]
static bool clampedDualTransition(void* self, void* gameState, int mode1, const uint32_t flags[4]) {
    extern float g_cameraThreshold;
    float cur = *(float*)self;
    float thr = g_cameraThreshold;
    float dMin = thr - cur, dMax = cur - thr;
    float cMin = (dMin >= 0.0f) ? cur : thr;
    float cMax = (dMax >= 0.0f) ? cur : thr;

    TransitionParams params;
    params.value = cMin;
    *(uint32_t*)((char*)&params + 4) = *(uint32_t*)((char*)self + 4);
    uint64_t saved = *(uint64_t*)&params;

    if (!pongCameraMgr_6E08(gameState, &params, mode1, (void*)flags, 1)) return false;
    if (params.value > cMax) return false;

    *(uint64_t*)&params = saved;  // restore original clamped params for second call
    if (!pongCameraMgr_6E08(gameState, &params, 16, (void*)&flags[2], 1)) return false;
    if (params.value > cMax) return false;
    *(float*)self = params.value;
    return true;
}

// Helper: 6E08 call then pg_6F68 call, both clamped
static bool clampedTransition_pg6F68(void* self, void* gameState, int mode1, uint32_t flags[2]) {
    extern float g_cameraThreshold;
    float cur = *(float*)self;
    float thr = g_cameraThreshold;
    float dMin = thr - cur, dMax = cur - thr;
    float cMin = (dMin >= 0.0f) ? cur : thr;
    float cMax = (dMax >= 0.0f) ? cur : thr;

    TransitionParams params;
    params.value = cMin;
    *(uint32_t*)((char*)&params + 4) = *(uint32_t*)((char*)self + 4);

    if (!pongCameraMgr_6E08(gameState, &params, mode1, flags, 1)) return false;
    if (params.value > cMax) return false;

    if (!pg_6F68(gameState, &params, 16, flags, 1)) return false;
    if (params.value > cMax) return false;
    *(float*)self = params.value;
    return true;
}

// ── Single-call pattern (6 functions): mode=12, 6-field flags, r7=3 ─────

// pongCameraMgr_5EF8_fw @ 0x821F5EF8 — flags={3,0,1,1,2,sel}
bool pongCameraMgr_5EF8_fw_impl(void* self, void* gameState) {
    uint32_t s = (g_cameraSelector != 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 0, 1, 1, 2, s};
    return clampedTransition12(self, gameState, flags);
}

// pongCameraMgr_5FD8_fw @ 0x821F5FD8 — flags={3,1,1,1,2,sel}
bool pongCameraMgr_5FD8_fw_impl(void* self, void* gameState) {
    uint32_t s = (g_cameraSelector != 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 1, 1, 1, 2, s};
    return clampedTransition12(self, gameState, flags);
}

// pongCameraMgr_60B0_fw @ 0x821F60B0 — flags={3,0,1,0,2,!sel}
bool pongCameraMgr_60B0_fw_impl(void* self, void* gameState) {
    uint32_t inv = (g_cameraSelector == 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 0, 1, 0, 2, inv};
    return clampedTransition12(self, gameState, flags);
}

// pongCameraMgr_6188_fw @ 0x821F6188 — flags={3,1,1,0,2,!sel}
bool pongCameraMgr_6188_fw_impl(void* self, void* gameState) {
    uint32_t inv = (g_cameraSelector == 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 1, 1, 0, 2, inv};
    return clampedTransition12(self, gameState, flags);
}

// pongCameraMgr_6260_fw @ 0x821F6260 — flags={3,0,1,1,2,!sel}
bool pongCameraMgr_6260_fw_impl(void* self, void* gameState) {
    uint32_t inv = (g_cameraSelector == 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 0, 1, 1, 2, inv};
    return clampedTransition12(self, gameState, flags);
}

// pongCameraMgr_6338_fw @ 0x821F6338 — flags={3,1,1,1,2,!sel}
bool pongCameraMgr_6338_fw_impl(void* self, void* gameState) {
    uint32_t inv = (g_cameraSelector == 0) ? 1u : 0u;
    uint32_t flags[6] = {3, 1, 1, 1, 2, inv};
    return clampedTransition12(self, gameState, flags);
}

// ── Dual-call pattern (4 functions): 6E08(mode1) then 6E08(mode=16) ────

// pongCameraMgr_6D00_fw @ 0x821F6D00 — mode=10, flags={3,0,3,1}
bool pongCameraMgr_6D00_fw_impl(void* self, void* gameState) {
    uint32_t flags[4] = {3, 0, 3, 1};
    return clampedDualTransition(self, gameState, 10, flags);
}

// pongCameraMgr_6DE0_fw @ 0x821F6DE0 — mode=10, flags={3,1,3,0}
bool pongCameraMgr_6DE0_fw_impl(void* self, void* gameState) {
    uint32_t flags[4] = {3, 1, 3, 0};
    return clampedDualTransition(self, gameState, 10, flags);
}

// pongCameraMgr_7090_fw @ 0x821F7090 — mode=7, flags={3,0,3,1}
bool pongCameraMgr_7090_fw_impl(void* self, void* gameState) {
    uint32_t flags[4] = {3, 0, 3, 1};
    return clampedDualTransition(self, gameState, 7, flags);
}

// pongCameraMgr_7170_fw @ 0x821F7170 — mode=7, flags={3,1,3,0}
bool pongCameraMgr_7170_fw_impl(void* self, void* gameState) {
    uint32_t flags[4] = {3, 1, 3, 0};
    return clampedDualTransition(self, gameState, 7, flags);
}

// ── 6E08 + pg_6F68 pattern (3 functions) ────────────────────────────────

// pongCameraMgr_6EC0 @ 0x821F6EC0 — mode=10, flags={3,0}
extern "C" bool pongCameraMgr_6EC0(void* self, void* gameState) {
    uint32_t flags[2] = {3, 0};
    return clampedTransition_pg6F68(self, gameState, 10, flags);
}

// pongCameraMgr_6FA8 @ 0x821F6FA8 — mode=10, flags={3,1}
extern "C" bool pongCameraMgr_6FA8(void* self, void* gameState, float /*f1*/, float /*f2*/) {
    uint32_t flags[2] = {3, 1};
    return clampedTransition_pg6F68(self, gameState, 10, flags);
}

// pongCameraMgr_7250 @ 0x821F7250 — mode=7, flags={3,0}
extern "C" bool pongCameraMgr_7250(void* self, void* gameState, float /*f1*/, float /*f2*/) {
    uint32_t flags[2] = {3, 0};
    return clampedTransition_pg6F68(self, gameState, 7, flags);
}

// ── Triple-call pattern: 3ED8_2h (modes 5, 3, 5) ───────────────────────

// pongCameraMgr_3ED8_2h @ 0x821F3ED8 — 3 args: (this, gameState, extraFlag)
// First 6E08(mode=5), if OK → 6E08(mode=3), if OK → 6E08(mode=5, val+offset)
// Store first-call result if third call fails or produces value < first.
bool pongCameraMgr_3ED8_2h(void* self, void* gameState, int extra) {
    extern float g_cameraThreshold;
    extern float g_cameraTransitionOffset;  // @ 0x8202D168

    float cur = *(float*)self;
    float thr = g_cameraThreshold;
    float dMin = thr - cur, dMax = cur - thr;
    float cMin = (dMin >= 0.0f) ? cur : thr;
    float cMax = (dMax >= 0.0f) ? cur : thr;

    TransitionParams params;
    params.value = cMin;
    *(uint32_t*)((char*)&params + 4) = *(uint32_t*)((char*)self + 4);
    uint32_t flags[2] = {3, (uint32_t)extra};

    // First call: mode 5
    if (!pongCameraMgr_6E08(gameState, &params, 5, flags, 1)) return false;
    float val1 = params.value;
    if (val1 > cMax) return false;

    // Second call: mode 3 (params preserved from first call)
    uint64_t snap = *(uint64_t*)&params;
    *(uint64_t*)&params = snap;
    if (!pongCameraMgr_6E08(gameState, &params, 3, flags, 1)) return false;

    // Third call: mode 5 with offset-adjusted value
    params.value = val1 + g_cameraTransitionOffset;
    if (pongCameraMgr_6E08(gameState, &params, 5, flags, 1)) {
        if (params.value >= val1) return false;  // adjusted result too high
    }

    // Store first-call result (third call failed or produced lower value)
    *(float*)self = val1;
    return true;
}

// ── Threshold Query Functions ───────────────────────────────────────────────
// Four variants: mode 17/18 × flag2 0/1
// Each builds TransitionFlags/Params, calls pongCameraMgr_6E08, then checks
// result against [*(float*)this, g_cameraThreshold + delta]. NaN → false.
// ─────────────────────────────────────────────────────────────────────────────

static bool CheckThresholdQuery(void* thisPtr, void* gameState, float delta,
                                int mode, uint32_t flag2) {
    extern float g_cameraThreshold;  // @ 0x825C5EC0

    // Build params from first 8 bytes of object (value + state)
    TransitionParams params;
    *(uint64_t*)&params = *(uint64_t*)thisPtr;

    TransitionFlags flags;
    flags.flag1 = 3;
    flags.flag2 = flag2;

    uint8_t result = pongCameraMgr_6E08(gameState, &params, mode, &flags, 1);
    if (!result) return false;

    // Check range: params.value must be in [lower, upper]
    // NaN comparisons: !(a <= b) catches both a > b AND NaN (bso/bns pattern)
    float upper = g_cameraThreshold + delta;
    float val = params.value;
    if (!(val <= upper)) return false;

    float lower = *(float*)thisPtr;
    if (!(val >= lower)) return false;

    return true;
}

// @ 0x821F9648 | 0x98 bytes — mode 17, flag2=0
bool pongCameraMgr::ThresholdQuery_Mode17(void* gameState, float delta) {
    return CheckThresholdQuery(this, gameState, delta, 17, 0);
}

// @ 0x821F96E0 | 0x98 bytes — mode 17, flag2=1
bool pongCameraMgr::ThresholdQuery_Mode17_Reverse(void* gameState, float delta) {
    return CheckThresholdQuery(this, gameState, delta, 17, 1);
}

// @ 0x821F9778 | 0x98 bytes — mode 18, flag2=0
bool pongCameraMgr::ThresholdQuery_Mode18(void* gameState, float delta) {
    return CheckThresholdQuery(this, gameState, delta, 18, 0);
}

// @ 0x821F9810 | 0x98 bytes — mode 18, flag2=1
bool pongCameraMgr::ThresholdQuery_Mode18_Reverse(void* gameState, float delta) {
    return CheckThresholdQuery(this, gameState, delta, 18, 1);
}

// ── Float Clamp Utility Functions ───────────────────────────────────────────
// TINY functions (0x14 = 5 PPC instructions each), fsel-based float clamping
// ─────────────────────────────────────────────────────────────────────────────

// @ 0x821F3100 | 0x14 bytes — clampMin: *ptr = max(*ptr, limit)
void pongCameraMgr_3100_p45(float* ptr, float limit) {
    if (*ptr < limit) {
        *ptr = limit;
    }
}

// @ 0x821F3114 | 0x14 bytes — clampMax: *ptr = min(*ptr, limit)
void pongCameraMgr_3114_p45(float* ptr, float limit) {
    if (*ptr > limit) {
        *ptr = limit;
    }
}

// ── Global Camera Data Accessor Functions ───────────────────────────────────
// Navigate: g_sceneRenderObj2 → +0x34 → +0x04 → +0x34 → vtable dispatch
// All use vtable[3](target, param) then vtable[2](target, result)
// Differ only in final data extraction from the result pointer
// ─────────────────────────────────────────────────────────────────────────────

extern void* g_sceneRenderObj2;  // @ 0x8260641C (SDA offset 25628)

// Helper: navigate the global pointer chain to the target object
static void* GetCameraChainTarget() {
    void* scene = *(void**)((char*)g_sceneRenderObj2 + 52);   // +0x34
    void* sub   = *(void**)((char*)scene + 4);                // +0x04
    return *(void**)((char*)sub + 52);                        // +0x34
}

// Helper: virtual dispatch through the chain target
// Calls vtable[3](target, queryParam), then vtable[2](target, result1)
typedef void* (*VTableFn2)(void*, void*);

static void* DispatchCameraChain(void* queryParam) {
    void* target = GetCameraChainTarget();
    void** vtable = *(void***)target;

    // First dispatch: vtable[3](target, queryParam)
    VTableFn2 fn3 = (VTableFn2)vtable[3];
    void* result1 = fn3(target, queryParam);

    // Reload target (global could change between calls)
    target = GetCameraChainTarget();
    vtable = *(void***)target;

    // Second dispatch: vtable[2](target, result1)
    VTableFn2 fn2 = (VTableFn2)vtable[2];
    return fn2(target, result1);
}

// @ 0x821734D0 | 0x78 bytes — returns float from chain result
float pongCameraMgr_34D0_2hr(void* queryParam) {
    void* result = DispatchCameraChain(queryParam);
    return *(float*)result;
}

// @ 0x82173548 | 0x78 bytes — returns uint32 from chain result
uint32_t pongCameraMgr_3548_2hr(void* queryParam) {
    void* result = DispatchCameraChain(queryParam);
    return *(uint32_t*)result;
}

// @ 0x821735C0 | 0x78 bytes — returns byte from chain result
uint8_t pongCameraMgr_35C0_2h(void* queryParam) {
    void* result = DispatchCameraChain(queryParam);
    return *(uint8_t*)result;
}

// @ 0x82173638 | 0x8C bytes — copies 16-byte vector from chain result to dest
void pongCameraMgr_3638_2h(void* dest) {
    // For this variant, no explicit queryParam — vtable[3] is called
    // with whatever r4 the caller provides (propagated from calling context)
    void* target = GetCameraChainTarget();
    void** vtable = *(void***)target;

    // First dispatch: vtable[3](target)
    typedef void* (*VTableFn1)(void*);
    VTableFn1 fn3 = (VTableFn1)vtable[3];
    void* result1 = fn3(target);

    target = GetCameraChainTarget();
    vtable = *(void***)target;

    VTableFn2 fn2 = (VTableFn2)vtable[2];
    void* result2 = fn2(target, result1);

    // Copy 16 bytes (aligned vector) from result to dest
    // Original uses lvx128/stvx128 (16-byte aligned vector load/store)
    memcpy(dest, result2, 16);
}

// ── pongCameraMgr_9DE8_g — Camera Sample Object Factory ────────────────────
// @ 0x821F9DE8 | 0xC0 bytes
// Allocates a 40-byte camera sample object, initializes fields, and inserts
// it into the camera sample machine bank on the owner.
// ─────────────────────────────────────────────────────────────────────────────

extern "C" void xe_main_thread_init_0038();
extern "C" void* atFreeList_Alloc(void* bankBase, uint32_t entrySize);

extern float g_cameraSampleInitFloat1;  // .rdata @ 0x82079B4C
extern float g_cameraSampleInitFloat2;  // .rdata @ 0x8202D110

void* pongCameraMgr_9DE8_g(void* owner, uint32_t param1, uint32_t /*unused*/,
                            uint8_t param2, uint32_t param3) {
    xe_main_thread_init_0038();

    // Navigate SDA to get allocator: SDA[0] → globalPtr → globalPtr+4 → allocator
    extern void* g_sdaBase;     // @ 0x82600000 (r13+0)
    void* globalPtr = g_sdaBase;
    void* allocator = *(void**)((char*)globalPtr + 4);

    // Call allocator->vtable[1](allocator, size=40, align=16)
    void** allocVtable = *(void***)allocator;
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn allocFn = (AllocFn)allocVtable[1];
    void* newObj = allocFn(allocator, 40, 16);

    if (newObj != nullptr) {
        // Set vtable pointer (gdPatternSet vtable @ 0x8204911C)
        extern void* g_cameraSampleVtable;  // vtable @ 0x8204911C
        *(void**)((char*)newObj + 0) = &g_cameraSampleVtable;

        // Initialize fields
        *(uint32_t*)((char*)newObj + 4) = 0;       // flags
        *(uint16_t*)((char*)newObj + 8) = 0;       // field_0x08
        *(uint16_t*)((char*)newObj + 10) = 0;      // field_0x0A
        *(float*)((char*)newObj + 12) = g_cameraSampleInitFloat2;  // field_0x0C
        *(float*)((char*)newObj + 16) = g_cameraSampleInitFloat1;  // field_0x10
        *(float*)((char*)newObj + 20) = g_cameraSampleInitFloat2;  // field_0x14
        *(float*)((char*)newObj + 24) = g_cameraSampleInitFloat2;  // field_0x18
        *(uint32_t*)((char*)newObj + 28) = param1;    // field_0x1C
        *(uint8_t*)((char*)newObj + 32) = param2;     // field_0x20
        *(uint32_t*)((char*)newObj + 36) = param3;    // field_0x24
    }

    // Insert into the camera sample machine bank at owner+4
    void** slot = (void**)atFreeList_Alloc((char*)owner + 4, 16);
    *slot = newObj;

    return newObj;
}


// ═════════════════════════════════════════════════════════════════════════════
// camViewCS — 6 virtual methods (slots 3, 6, 7, 8, 9, 10)
// ═════════════════════════════════════════════════════════════════════════════

// Param-hook registration helper shared by many camera/view classes.
// Signature inferred from calls at 0x82177B60: (host, kind, field, addr, enabled, tag, pad).
extern void pongCameraView_RegisterParamHook(void* paramHost, int32_t kind,
                                             void* fieldAddr, const char* name,
                                             int32_t enabled, void* userData,
                                             int32_t flags);

// String literals pulled from .rdata (annotated in pass5_final):
extern const char kCamViewCS_ParamFlag[];     // lbl_82036304 → "UseOverrideExtents"-style flag tag
extern const char kCamViewCS_ParamRadius[];   // lbl_82036314 → override-extent-X tag
extern const char kCamViewCS_ParamHeight[];   // lbl_82036320 → override-extent-Y tag
extern const char kCamViewCS_TypeName[];      // lbl_820362E8 → returned by GetTypeName
extern const char kCamViewCS_DebugLabel[];    // lbl_820362F8 → returned by GetDebugLabel

// Zero-float constant used by OnReset (lfs f0 from .rdata).
extern const float g_camViewCS_ResetFloat;    // lbl_8202D090 → 0.0f

// Bound-camera position writer (pongCameraMgr helper at 0x821539F0).
extern void pongCameraMgr_WriteBoundPosition(void* mgr, void* boundCamera);

// Capsule-style FOV solver from the phBoundCapsule helper bank.
// At 0x82228EA0 it takes (loopObj, extentY_in_f7, extentX_in_f6) and returns
// the horizontal FOV in f1 plus propagates f6/f7 through to the caller.
extern float phBoundCapsule_SolveFOV(void* loopObj, float extentY, float extentX);

// Final bound-capsule publish step (0x82153598) — commits the 824/828/832/836
// float block plus the 864 active flag to the pongCameraMgr render state.
extern void pongCameraMgr_PublishViewExtents(void* mgr);

// Loop-object global pulled from 0x825EAB30 (annotated as g_loop_obj_ptr).
extern void* g_loopObject;

/**
 * camViewCS::RegisterParamHooks  [vtable slot 3]  @ 0x8216CEB0
 *
 * Registers three parameter hooks with the supplied host: the override-extents
 * toggle at +0x38 and the two override floats at +0x40/+0x3C. Forwarded straight
 * to pongHairData_7B60_g with kinds 1/6/6 respectively.
 */
void camViewCS::RegisterParamHooks(void* paramHost) {
    pongCameraView_RegisterParamHook(paramHost, 1,
        &this->m_useOverrideExtents, kCamViewCS_ParamFlag,   1, nullptr, 0);
    pongCameraView_RegisterParamHook(paramHost, 6,
        &this->m_overrideExtentY,    kCamViewCS_ParamRadius, 1, nullptr, 0);
    pongCameraView_RegisterParamHook(paramHost, 6,
        &this->m_overrideExtentX,    kCamViewCS_ParamHeight, 1, nullptr, 0);
}

/**
 * camViewCS::GetTypeName  [vtable slot 6]  @ 0x82177830
 * Returns the "camViewCS" class-name string baked into .rdata.
 */
const char* camViewCS::GetTypeName() {
    return kCamViewCS_TypeName;
}

/**
 * camViewCS::GetDebugLabel  [vtable slot 7]  @ 0x8216CAE8
 * Returns a human-readable debug label also baked into .rdata.
 */
const char* camViewCS::GetDebugLabel() {
    return kCamViewCS_DebugLabel;
}

/**
 * camViewCS::OnReset  [vtable slot 8]  @ 0x8216CE40
 *
 * Dispatches reset to the active source (and, if present, a secondary source
 * at +0x34), then zeroes the time accumulator and the view-flags word.
 * The active-source vtable byte-offset 32 corresponds to slot 8 (OnReset).
 */
void camViewCS::OnReset() {
    // Secondary source (copy held in +0x34) is reset first if non-null.
    if (this->m_boundCamera != nullptr) {
        typedef void (*ResetFn)(void*);
        ResetFn reset = ((ResetFn**)this->m_boundCamera)[0][8];
        reset(this->m_boundCamera);
    }
    // Primary active source — always dispatched.
    typedef void (*ResetFn)(void*);
    ResetFn reset = ((ResetFn**)this->m_activeSource)[0][8];
    reset(this->m_activeSource);

    this->m_timeAccumulator = g_camViewCS_ResetFloat;
    // Original also clears a 4-byte scratch slot at +0x30 (inside the padding
    // region) plus the u16 view-flags. Kept inline so the offset doesn't leak
    // into the header as a named field until we learn what +0x30 actually is.
    uint32_t* scratchAtOffset0x30 = (uint32_t*)((char*)this + 0x30);
    *scratchAtOffset0x30 = 0u;
    this->m_viewFlags = 0u;
}

/**
 * camViewCS::OnSourceChanged  [vtable slot 9]  @ 0x8216CD30
 *
 * Forwards a source-changed notification to the active source (vtable slot 9),
 * then invalidates m_viewFlags when the primary and secondary source pointers
 * no longer match — signalling that the cached view state is stale.
 */
void camViewCS::OnSourceChanged() {
    typedef void (*ChangedFn)(void*);
    ChangedFn changed = ((ChangedFn**)this->m_activeSource)[0][9];
    changed(this->m_activeSource);

    if (this->m_activeSource != this->m_boundCamera) {
        this->m_viewFlags = 0u;
    }
}

/**
 * camViewCS::Update  [vtable slot 10]  @ 0x8216CD88
 *
 * Per-frame update. Commits the bound camera's transform to the camera manager,
 * then — depending on the override-extents flag — feeds either the override
 * floats or the bound camera's native extents (+168/+172) into the capsule FOV
 * solver. The resulting FOV/aspect/extents block is written to the manager at
 * +824..+836 with the active flag set at +864, before the publish helper runs.
 */
void camViewCS::Update() {
    pongCameraMgr_WriteBoundPosition(g_loopObject, this->m_activeSource);

    float extentX;
    float extentY;
    if (this->m_useOverrideExtents != 0) {
        extentY = this->m_overrideExtentY;
        extentX = this->m_overrideExtentX;
    } else {
        // +168 / +172 on the active source supply native extents.
        extentY = *(float*)((char*)this->m_activeSource + 172);
        extentX = *(float*)((char*)this->m_activeSource + 168);
    }

    float horizontalFOV = phBoundCapsule_SolveFOV(g_loopObject, extentY, extentX);

    // The active-source-derived base-fov lives at +164.
    float baseFOV = *(float*)((char*)this->m_activeSource + 164);

    void* mgr = this->m_cameraMgr;
    *(float*)  ((char*)mgr + 824) = baseFOV;
    *(uint8_t*)((char*)mgr + 864) = 1u;
    *(float*)  ((char*)mgr + 828) = horizontalFOV;
    *(float*)  ((char*)mgr + 832) = extentX;
    *(float*)  ((char*)mgr + 836) = extentY;

    pongCameraMgr_PublishViewExtents(mgr);
}

// ═════════════════════════════════════════════════════════════════════════════
// camShotMgr — 4 virtual methods (slots 4, 8, 9, 10)
// ═════════════════════════════════════════════════════════════════════════════

// Base-class OnEnter forwarded-to at the tail of slot 4.
extern void gmLogic_OnEnter(void* self);

// rage memory allocator (xe_main_thread_init_0038 @ 0x820C0038 maps to the SDA
// allocator object loaded from r13+0 — the vtable slot 1 invoked afterwards is
// rage::sysMemAllocator::Allocate(size, align)).
extern void* rage_sysMemAllocator_Get();
extern void  rage_sysMemAllocator_Free(void* ptr);    // wraps rage_free_00C0

// Vtable address of the camShot class (annotated at lbl_82035CFC in pass5_final).
extern void* const kCamShotVTable;

/**
 * camShotMgr::OnEnter  [vtable slot 4]  @ 0x82165458
 *
 * Copies the pre-configured source state id (loaded during construction) into
 * the active-shot-index slot and chains to gmLogic::OnEnter so the base class
 * can run its own entry bookkeeping.
 */
void camShotMgr::OnEnter() {
    this->m_currentShotIndex = this->m_sourceStateId;
    gmLogic_OnEnter(this);
}

/**
 * camShotMgr::BuildShotArray  [vtable slot 8]  @ 0x82165468
 *
 * Allocates a 4-byte-per-slot pointer array sized for m_shotCount+1 entries
 * (so call sites can over-index by one to detect end). Each slot is populated
 * with a freshly allocated 16-byte camShot whose vtable is stamped, remaining
 * fields zeroed. If an individual camShot allocation returns null, the slot
 * is left as nullptr rather than aborting the whole build.
 */
void camShotMgr::BuildShotArray() {
    // Count-plus-one, clamped if the saturated-add overflows int32.
    int64_t expandedCount = (int64_t)this->m_shotCount + 1;
    int32_t byteSize = (int32_t)(expandedCount * 4);
    if (expandedCount > 0x3FFFFFFF) {
        byteSize = -1;
    }
    this->m_shotCount = (int32_t)expandedCount;

    void* allocator = rage_sysMemAllocator_Get();
    typedef void* (*AllocFn)(void*, int32_t, int32_t);
    AllocFn allocate = ((AllocFn**)allocator)[0][1];

    void** shotArray = (void**)allocate(allocator, byteSize, 16);
    this->m_shotArray = shotArray;

    const int32_t total = this->m_shotCount;
    for (int32_t i = 0; i < total; ++i) {
        void* shot = allocate(allocator, 16, 16);
        if (shot != nullptr) {
            *(void**)  ((char*)shot + 0)  = kCamShotVTable;
            *(uint32_t*)((char*)shot + 4)  = 0u;
            *(uint32_t*)((char*)shot + 8)  = 0u;
            *(uint32_t*)((char*)shot + 12) = 0u;
        }
        // Slot index is byte-offset i*4 within m_shotArray (see pass5_final
        // loop: r28 advances by 4, current total written via stwx).
        this->m_shotArray[i] = shot;
    }
}

/**
 * camShotMgr::DestroyShotArray  [vtable slot 9]  @ 0x82165558
 *
 * Virtually destructs each live camShot entry (vtable slot 0 with the
 * delete-flag = 1) and then frees the backing pointer array itself.
 */
void camShotMgr::DestroyShotArray() {
    const int32_t count = this->m_shotCount;
    for (int32_t i = 0; i < count; ++i) {
        void* shot = this->m_shotArray[i];
        if (shot != nullptr) {
            typedef void (*DtorFn)(void*, int32_t);
            DtorFn dtor = ((DtorFn**)shot)[0][0];
            dtor(shot, 1);
        }
    }

    if (this->m_shotArray != nullptr) {
        rage_sysMemAllocator_Free(this->m_shotArray);
    }
}

/**
 * camShotMgr::ResetShotIndices  [vtable slot 10]  @ 0x821655D8
 *
 * Clears the current/previous/next shot indices back to -1 (the "no shot
 * selected" sentinel used throughout the camShot machinery).
 */
void camShotMgr::ResetShotIndices() {
    this->m_currentShotIndex  = (uint32_t)-1;
    this->m_previousShotIndex = (uint32_t)-1;
    this->m_nextShotIndex     = (uint32_t)-1;
}

// ═══════════════════════════════════════════════════════════════════════════
// charViewCS — character-view camera subsystem
// 10 methods lifted from RTTI vtable @ 0x820362C0 + related helpers.
// Per-frame character-position updater for the in-match character view.
// ═══════════════════════════════════════════════════════════════════════════

// Forward declarations for RAGE runtime helpers called by this subsystem.
extern "C" {
    void  util_1568(void* owner);                       // @ 0x82161568 — cleanup embedded owner
    void  rage_7630(void* self);                        // @ 0x82177630 — base class dtor
    void  rage_free_00C0(void* p);                      // @ 0x820c00c0 — operator delete wrapper
    void  pongHairData_7B60_g(void* host, int valueKind, const char* name,
                              void* addr, int flag1, int unused, int zero); // @ 0x82177b60 — register tunable param hook
    void  phBoundCapsule_DB10_g(void* self);            // @ 0x8216db10 — capsule-collision pass (vfn_4 target)
}

// Externally-owned string table slots (for GetTypeName / GetDebugLabel).
// Their exact addresses are 0x82036E40 and 0x82045380 — filled at link time.
extern const char  charViewCS_typeName[];   // @ 0x82036E40 — "charViewCS"
extern const char  charViewCS_debugLabel[]; // @ 0x82045380 — debug-name text

// Static parameter names used by RegisterAxisHooks.
// These are fixed .rdata literals whose layout is dictated by PPC addi-strides
// of 12 bytes between successive literal slots.
extern const char  charViewCS_paramName0[]; // @ 0x82036E44 slot 0
extern const char  charViewCS_paramName1[]; // @ 0x82036E50 slot 1
extern const char  charViewCS_paramName2[]; // @ 0x82036E5C slot 2
extern const char  charViewCS_paramName3[]; // @ 0x82036E68 slot 3
extern const char  charViewCS_paramName4[]; // @ 0x82036E74 slot 4
extern const char  charViewCS_paramName5[]; // @ 0x82036E80 slot 5 (last, kind=1 instead of 6)

// Tuning-table base used by vfn_9 / ApplyAxisScaled to pick a scale factor.
// When m_enableModeB4 is set, we pick the secondary entry (offset +84 from base);
// otherwise the primary entry (offset +8). Base is at 0x8204A5F8 (big data blob).
extern const float charViewCS_scaleTable[]; // @ 0x8204A5F8 — 100-byte tuning struct

// Misc scalar constants used by ProcessAxisInputs:
//   -25676 → bias for byte→signed conversion (= 127.0f)
//   -25680 → inverse-range divisor (= 1/127.5f)
//   @ -8 and +0 near 0x8202D160 → vector-lerp "zero" and "one" sentinels
extern const float charViewCS_byteBias;     // = 127.0f
extern const float charViewCS_byteScale;    // = 1.0f / 127.5f
extern const float charViewCS_oneF;         // = 1.0f
extern const float charViewCS_zeroF;        // = 0.0f

// Vtable stamped by the destructor (back-fill after base-dtor chain).
extern void** charViewCS_vtable;            // @ 0x820362C0

// ── Layout assumptions used below ────────────────────────────────────────
// +0x04..+0x0F are unused pad. The 6 axis hooks live at bytes 156/160/164/168/172/180,
// which matches the emitted addi r6,r31,{156,160,164,168,172,180} sequence. The
// per-slot state block lives at (this + 248) and each slot is 48 bytes; slot i
// starts at +248 + (i + i*2) * 16 = +248 + i*48. This is what the r9 * 48 and
// r29 = r11 + r10 arithmetic computes in vfn_9.
// ─────────────────────────────────────────────────────────────────────────

/**
 * charViewCS::~charViewCS (deleting destructor) — vtable slot 0
 * @ 0x8216bd20
 *
 * Runs cleanup on the embedded sub-object at +292 (m_ownsHeap), then stamps
 * the base vtable back in, chains to rage's base destructor, and — if the
 * caller passed the "delete" flag in the low bit of r4 — frees the object.
 */
charViewCS::~charViewCS() {
    // Clean up subsystem state owned at +292 (m_ownsHeap block).
    util_1568(&m_ownsHeap);

    // Re-stamp the vtable pointer at *this* before chaining: this lets any
    // overridden slots in the base destructor dispatch correctly.
    vtable = charViewCS_vtable;

    // Chain into the rage base class destructor.
    rage_7630(this);

    // The synthetic `deleteFlag` in the low bit of r4 (passed via the ABI's
    // hidden 2nd arg for deleting-destructors) asks the runtime whether to
    // release this object's heap storage. Preserved via a compiler-intrinsic
    // `__dtor_delete_flag` stand-in wired up in the recomp harness.
    // TODO: wire __dtor_delete_flag argument through the shim
    // if (__dtor_delete_flag & 1) rage_free_00C0(this);
}

/**
 * charViewCS::RegisterAxisHooks — vtable slot 3
 * @ 0x8216dbb8
 *
 * Registers six tunable parameters with a host "param registry" (typically a
 * pongHairData-style reflection object). The first five describe float axes
 * backed by m_axisHook0..4; the sixth describes an integer/flag at +180
 * (m_enableModeB4) with a different kind-tag (1 vs 6).
 */
void charViewCS::RegisterAxisHooks(void* paramHost) {
    // Kind-6 = float axis. flag1 = 1 everywhere (treat as tunable).
    pongHairData_7B60_g(paramHost, 6, charViewCS_paramName0, &m_axisHook0, 1, 0, 0);
    pongHairData_7B60_g(paramHost, 6, charViewCS_paramName1, &m_axisHook1, 1, 0, 0);
    pongHairData_7B60_g(paramHost, 6, charViewCS_paramName2, &m_axisHook2, 1, 0, 0);
    pongHairData_7B60_g(paramHost, 6, charViewCS_paramName3, &m_axisHook3, 1, 0, 0);
    pongHairData_7B60_g(paramHost, 6, charViewCS_paramName4, &m_axisHook4, 1, 0, 0);
    // Last entry uses kind-1 (bool/uint8) and points at the mode byte.
    pongHairData_7B60_g(paramHost, 1, charViewCS_paramName5, &m_enableModeB4, 1, 0, 0);
}

/**
 * charViewCS::ForwardToBoundCapsule — vtable slot 4
 * @ 0x8216dcb0
 *
 * Single-instruction tail-call: delegates to the bound physics-capsule's
 * vfn_0 (collision/bounds reset). The scaffold literally emits `b 0x8216db10`.
 */
void charViewCS::ForwardToBoundCapsule() {
    phBoundCapsule_DB10_g(this);
}

/**
 * charViewCS::GetTypeName — vtable slot 6
 * @ 0x8216db00
 *
 * Returns the class's RTTI display name. The PPC form is two instructions
 * (`lis r11,...; addi r3,r11,25592 ; blr`) resolving to a .rdata literal.
 */
const char* charViewCS::GetTypeName() {
    return charViewCS_typeName;  // lbl @ 0x82036E40
}

/**
 * charViewCS::GetDebugLabel — vtable slot 7
 * @ 0x82177840
 *
 * Sibling of GetTypeName; returns a different literal used by debug overlays
 * and log prefixes. Same shape (lis/addi/blr) with a different offset.
 */
const char* charViewCS::GetDebugLabel() {
    return charViewCS_debugLabel;  // lbl @ 0x82045380
}

/**
 * charViewCS::UpdatePositions — vtable slot 9
 * @ 0x8216c200 | size: ~0x820 bytes
 *
 * Per-frame position updater. Iterates seven character slots (the 7 player
 * roles in a mixed-doubles layout). For each slot, reads its 48-byte state
 * block at m_stateTable + slot*48, conditionally blends between a target
 * and a clamped delta, writes back the interpolated float pair into
 * (this + (slot+48)*4) and (this + (slot+64)*4).
 *
 * Because the decompiled form is ~800 lines of dense FP/vector arithmetic
 * with multiple fused-multiply-add chains, a faithful literal rewrite
 * would not add comprehension value here — the intent is a per-axis
 * bounded lerp toward a per-slot target, rate-limited by m_blendTime
 * and gated on m_transitionPhase.
 *
 * TODO: expand faithful body once PPC→C lifter supports lfsx/fsel/fmadds
 *       idiomatic reconstruction. For now the harness provides a stub
 *       wrapper that calls the original recomp via PPC_FUNC_HOOK.
 */
void charViewCS::UpdatePositions() {
    // TODO: ~811-line faithful implementation. Gated behind PPC hook for now.
    // Behavior: for slot in [0..6], if m_stateTable[slot].kind == 1:
    //   - special-case slot == 2 && m_transitionPhase == 1: parametric lerp
    //     from field_0x60 toward field_0x64 based on m_blendTime vs
    //     stateTable[slot].t0/t1.
    //   - otherwise: ratio-blend with clamped fsel pattern.
    //   Finally accumulates (this + (slot+48)*4) += axisScale * (this + (slot+64)*4).
}

/**
 * charViewCS::DispatchAllSlotInputs — vtable slot 10
 * @ 0x8216bed0
 *
 * Iterates all four input slots and dispatches ProcessAxisInputs for each,
 * except when the slot equals m_slotSourceIdx (+252) *unless* that index
 * is out-of-range (>= 4 → sentinel "no self"). Matches the original:
 *   for (i = 0; i < 4; ++i)
 *       if (i != m_slotSourceIdx || m_slotSourceIdx >= 4)
 *           ProcessAxisInputs(i);
 */
void charViewCS::DispatchAllSlotInputs() {
    for (int i = 0; i < 4; ++i) {
        const int source = m_slotSourceIdx;
        if (i == source && source < 4) continue;  // skip self when valid
        ProcessAxisInputs(i);
    }
}

/**
 * charViewCS::AccumulateDelta — vtable slot 11
 * @ 0x8216db88
 *
 * 128-bit SIMD accumulate: treats the objects at (this+64) and (this+128) as
 * `float[4]` and adds the caller-supplied `delta` vector (also float[4]) into
 * each. Used by the animation blend pipeline to shift the whole-body frame.
 *
 * @param delta 16-byte-aligned float[4] to add into both targets
 */
void charViewCS::AccumulateDelta(const void* delta) {
    const float* d    = static_cast<const float*>(delta);
    float*       slot0 = reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 64);
    float*       slot1 = reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 128);
    for (int i = 0; i < 4; ++i) {
        slot0[i] += d[i];
        slot1[i] += d[i];
    }
}

/**
 * charViewCS::ProcessAxisInputs — @ 0x8216bf18 (helper)
 *
 * Reads three raw axis bytes out of a packed per-slot input record
 * (`m_stateTable->inputs[slotIndex]` — stride 12 bytes plus +4 → r4*9
 * matches the `rlwinm r10,r4,3,0,28; add r9,r4,r10` prologue). Converts
 * each byte to a signed -1..+1 float (bias 127, divide 127.5), clamps to
 * 0..1 with an axis-specific deadzone, then — if the scaled result is
 * non-zero — hands it off to ApplyAxisScaled with axis index 0, 1 or 2.
 *
 * Axis byte offsets within the 12-byte input record:
 *   byte +12 → axis 0 (pitch)
 *   byte +15 → axis 1 (yaw-alt)
 *   byte +13 → axis 2 (roll)
 *
 * Deadzone source: m_stateTable->deadzoneFloat (+32 from stateTable base).
 */
void charViewCS::ProcessAxisInputs(int slotIndex) {
    // Locate the packed input record for this slot:
    //   stride = 12 (slot * 9 then *4 matches 3*4 = 12 payload bytes + pad)
    const uint8_t* stateBase = static_cast<const uint8_t*>(m_stateTable);
    const float    deadzone  = *reinterpret_cast<const float*>(stateBase + 32);
    const uint8_t* record    = stateBase - 25872 + slotIndex * 36;  // base at 0x82184F30 offset
    // NOTE: -25872 bias and slotIndex*9*4 stride are the exact PPC form;
    // the fixed base is a global table (not m_stateTable) — but the compiler
    // loaded it via a single `addis -32158`. Preserve the magic offsets.

    auto normalizeByte = [](uint8_t raw) -> float {
        const float biased  = static_cast<float>(raw) - charViewCS_byteBias;
        return biased * charViewCS_byteScale;  // ∈ [-1, +1]
    };

    auto clampToDeadzone = [deadzone](float v) -> float {
        if (v >  1.0f) v =  1.0f;
        if (v < -1.0f) v = -1.0f;
        // Remap value > deadzone to [0,1], value < -deadzone to [-1,0], else 0.
        if (v >  deadzone) return (v - deadzone) / (charViewCS_oneF - deadzone);
        if (v < -deadzone) return (v + deadzone) / (charViewCS_oneF - deadzone);
        return 0.0f;
    };

    // Axis 0 — byte +12 (pitch).
    {
        const float v = clampToDeadzone(normalizeByte(record[12]));
        if (v != 0.0f) ApplyAxisScaled(v * charViewCS_oneF, 0);
    }
    // Axis 1 — byte +15 (yaw-alt).
    {
        const float v = clampToDeadzone(normalizeByte(record[15]));
        if (v != 0.0f) ApplyAxisScaled(v * charViewCS_oneF, 1);
    }
    // Axis 2 — byte +13 (roll).
    {
        const float v = clampToDeadzone(normalizeByte(record[13]));
        if (v != 0.0f) ApplyAxisScaled(v * charViewCS_oneF, 2);
    }
}

/**
 * charViewCS::ApplyAxisScaled — @ 0x8216c100 (helper)
 *
 * Writes `factor * scale` into a specific entry of the float "position grid"
 * that lives inside this object, clamped against a per-slot delta cap.
 * Target slot index comes from `axis`; the cap is read from the stateTable
 * entry at (+68 from its base + axis*16).
 *
 * Chosen scale depends on m_enableModeB4:
 *   - set  → scale = charViewCS_scaleTable[+84/4]   (secondary)
 *   - clr  → scale = charViewCS_scaleTable[+8/4]    (primary)
 *
 * Write target: (this + (axis + 64) * 4), i.e. float *(this + 256 + axis*4).
 * Delta cap:    stateTable[axis*48 + 80] (weight) * scale_cap.
 */
void charViewCS::ApplyAxisScaled(float factor, int axis) {
    // Pick scale from tuning table.
    const float scale = (m_enableModeB4 != 0)
                      ? charViewCS_scaleTable[84 / sizeof(float)]
                      : charViewCS_scaleTable[ 8 / sizeof(float)];

    // Locate the per-axis state block inside m_stateTable.
    // stride = 48 bytes; slot bases at +0, +48, +96, +144.
    uint8_t* stateBase = static_cast<uint8_t*>(m_stateTable);
    uint8_t* axisState = stateBase + axis * 48;
    const float weight = *reinterpret_cast<const float*>(axisState + 80);
    const float cap    = *reinterpret_cast<const float*>(axisState + 68);

    // Target cell inside the grid.
    float* cell = reinterpret_cast<float*>(
        reinterpret_cast<char*>(this) + (axis + 64) * 4);

    // Incremented value, then clamp against ±cap around current value.
    const float delta    = weight * scale * factor;
    const float newValue = *cell + delta;
    const float outside  = newValue - cap;
    // Original used fsel triplets: final = (outside >= 0) ? cap : newValue,
    // further clamped by (value - cap) fsel. Preserve the guard.
    *cell = (outside >= 0.0f) ? cap : newValue;
}
