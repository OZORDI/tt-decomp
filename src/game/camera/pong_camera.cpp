/**
 * pong_camera.cpp
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Camera management system implementation
 */

#include "pong_camera.hpp"
#include <cmath>

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
        float* timingArray = &m_timingArray[0];  // offset +60
        
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
    
    m_timingArray[clearIndex1] = 0.0f;
    m_timingArray[clearIndex2] = 0.0f;
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
    extern bool pongCameraMgr_35A0_fw(void* cameraMgr, void* gameState, int mode);
    
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
    
    float verticalFOV = *(float*)((char*)this + 828);  // +0x33C
    
    // Check if vertical FOV is zero (using constant comparison)
    if (verticalFOV == g_floatOne) {
        float horizontalFOV = *(float*)((char*)this + 804);  // +0x324
        
        if (horizontalFOV != g_floatOne) {
            // Calculate aspect ratio from viewport dimensions
            int32_t viewportWidth = *(int32_t*)((char*)this + 816);   // +0x330
            int32_t viewportHeight = *(int32_t*)((char*)this + 820);  // +0x334
            float baseWidth = *(float*)((char*)this + 800);           // +0x320
            
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
    void* buffer = pongCameraMgr_B9B0_g((char*)this + 4, 16);
    
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
