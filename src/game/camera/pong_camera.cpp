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

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::~pongCameraMgr()  [vtable slot 0 @ 0x821658E0]
// Destructor - unwinds vtables to base classes and optionally frees memory
// ─────────────────────────────────────────────────────────────────────────────
pongCameraMgr::~pongCameraMgr() {
    // Unwind vtables back to base classes during destruction
    // +16 gets msgMsgSink vtable, +0 gets sgNode vtable
    extern void* g_sgNodeVtable;       // @ 0x8203338C (.rdata)
    extern void* g_msgMsgSinkVtable;   // @ 0x82027B34 (.rdata)
    extern uint32_t g_cameraMgrInstancePtr;  // @ 0x8260641C (.data)

    // Reset instance tracking global
    g_cameraMgrInstancePtr = 0;

    // Call base class cleanup
    extern void atSingleton_Cleanup(void* self);  // @ 0x821A9420
    atSingleton_Cleanup(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ResetCameraState()  [vtable slot 25 @ 0x82166068]
// Resets camera state: clears shot index, resets interpolation params, and
// updates the active camera transform pointer
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::ResetCameraState() {
    extern float g_cameraDefaultParam;  // @ 0x8202D110 (.rdata, likely 0.0f)
    extern void* g_cameraContext;       // @ 0x8271A318 (.data)
    extern void* g_activeTransformPtr;  // @ 0x8271A32C (.data)

    // Clear the current shot index
    void* sceneGraph = *(void**)((char*)this + 40);     // +0x28 m_pSceneGraph
    field_0x0038 = 0;                                    // +0x38 m_currentShotIndex = 0

    // Call virtual slot 8 on the scene graph (likely Reset/Clear)
    // VCALL(sceneGraph, 8)
    typedef void (*VFn)(void*);
    void** sgVtable = *(void***)sceneGraph;
    ((VFn)sgVtable[8])(sceneGraph);

    // Get camera state object from scene graph
    void* cameraState = *(void**)((char*)this + 44);     // +0x2C m_pCameraState

    // Clear interpolation counters
    *(uint32_t*)((char*)cameraState + 4) = 0;
    *(uint32_t*)((char*)cameraState + 8) = 0;

    // Reset interpolation parameters on both blend targets
    void* blendTarget0 = *(void**)((char*)cameraState + 16);  // +0x10
    *(float*)((char*)blendTarget0 + 8) = g_cameraDefaultParam;
    *(float*)((char*)blendTarget0 + 12) = g_cameraDefaultParam;

    void* blendTarget1 = *(void**)((char*)cameraState + 20);  // +0x14
    *(float*)((char*)blendTarget1 + 8) = g_cameraDefaultParam;
    *(float*)((char*)blendTarget1 + 12) = g_cameraDefaultParam;

    // Update active transform pointer from current camera shot
    void* shotMgr = *(void**)((char*)this + 40);         // +0x28
    void* shotState = *(void**)((char*)shotMgr + 44);    // +0x2C
    void* transformBase = (void*)((char*)shotState + 16); // +0x10
    g_activeTransformPtr = transformBase;

    // Call virtual slot 3 on camera context (likely Refresh/Sync)
    void* ctx = (void*)((char*)g_cameraContext + 4);
    void** ctxVtable = *(void***)ctx;
    typedef void (*VFn3)(void*);
    ((VFn3)ctxVtable[3])(ctx);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateTransitionMode7()  @ 0x821F83B8
// Validates a camera transition using mode 7 with flags {3, 0}.
// Returns true if validation fails OR modified value is within bounds.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateTransitionMode7(void* gameState) {
    // Copy the 8-byte value from this (float + padding treated as u64)
    uint64_t packedValue = *(uint64_t*)this;

    // Prepare validation params on stack
    TransitionParams params;
    params.value = *(float*)&packedValue;
    *(uint64_t*)&params = packedValue;

    TransitionFlags flags;
    flags.flag1 = 3;
    flags.flag2 = 0;

    uint8_t result = pongCameraMgr_6E08(gameState, &params, 7, (void*)&flags, 1);

    if (result != 0) {
        float modifiedValue = params.value;
        // f1 is the upper bound returned by pongCameraMgr_6E08
        // If modified value exceeds upper bound, return false
        if (modifiedValue <= *(float*)((char*)&params + 0)) {
            return false;
        }
    }

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateTransitionMode11()  @ 0x821F5848
// Validates and applies a transition with mode 11, using threshold clamping.
// Stores modified value back if within acceptable range.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateTransitionMode11(void* gameState) {
    extern float g_cameraThreshold;  // @ 0x825C5EC0 (.data)

    float currentValue = *(float*)this;
    float threshold = g_cameraThreshold;

    // Clamp using fsel pattern: min/max against threshold
    float clampedMin = (threshold - currentValue >= 0.0f) ? currentValue : threshold;
    float clampedMax = (currentValue - threshold >= 0.0f) ? currentValue : threshold;

    // Prepare validation params
    uint64_t packedValue = *(uint64_t*)this;
    TransitionParams params;
    *(uint64_t*)&params = packedValue;
    params.value = clampedMin;

    TransitionFlags flags;
    flags.flag1 = 3;
    flags.flag2 = 1;

    // Extra flag words
    uint32_t extraFlags[3] = { 1, 1, 1 };

    uint8_t result = pongCameraMgr_6E08(gameState, &params, 11,
                                         (void*)&flags, 2);

    if (result != 0) {
        float modifiedValue = params.value;
        if (modifiedValue > clampedMax) {
            return false;
        }
        // Store accepted value back
        *(float*)this = modifiedValue;
        return true;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ValidateTransitionMode12()  @ 0x821F6338
// Validates transition with mode 12, using threshold clamping and a
// conditional debug/replay flag that controls extra parameters.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::ValidateTransitionMode12(void* gameState) {
    extern float g_cameraThreshold;     // @ 0x825C5EC0 (.data)
    extern uint8_t g_cameraReplayFlag;  // @ 0x826064CC (.data, SDA +25804)

    float currentValue = *(float*)this;
    float threshold = g_cameraThreshold;

    // Clamp using fsel pattern
    float clampedMin = (threshold - currentValue >= 0.0f) ? currentValue : threshold;
    float clampedMax = (currentValue - threshold >= 0.0f) ? currentValue : threshold;

    // Prepare validation params
    uint64_t packedValue = *(uint64_t*)this;
    TransitionParams params;
    *(uint64_t*)&params = packedValue;
    params.value = clampedMin;

    TransitionFlags flags;
    flags.flag1 = 3;
    flags.flag2 = 1;

    // Conditional extra flags based on replay state
    bool isNotReplay = (g_cameraReplayFlag == 0);
    uint32_t extraFlags[4];
    extraFlags[0] = 1;       // enabled
    extraFlags[1] = 1;       // enabled
    extraFlags[2] = 2;       // type
    extraFlags[3] = isNotReplay ? 1 : 0;  // replay-dependent

    uint8_t result = pongCameraMgr_6E08(gameState, &params, 12,
                                         (void*)&flags, 3);

    if (result != 0) {
        float modifiedValue = params.value;
        if (modifiedValue > clampedMax) {
            return false;
        }
        *(float*)this = modifiedValue;
        return true;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::CopyShotParameters()  @ 0x82166280
// Copies camera parameters (FOV, aspect, dimensions) from the current active
// shot to the camera slot selected by a global index, then updates the
// camera's transform matrix.
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::CopyShotParameters() {
    extern uint32_t g_activeCameraSlotIndex;  // @ 0x8271A2EC + 26076 offset
    extern void phBoundCapsule_ApplyParams(void* camera);     // @ 0x82153598
    extern void pongCameraMgr_UpdateTransform(void* camera, void* transformSrc);  // @ 0x821539F0

    // Navigate: this->+40 (m_pShotMgr) -> +44 -> +152 to get active shot
    void* shotMgr = *(void**)((char*)this + 40);
    void* shotState = *(void**)((char*)shotMgr + 44);
    void* activeShot = *(void**)((char*)shotState + 152);

    // Save current FOV parameters from active shot
    float savedHeight = *(float*)((char*)activeShot + 836);  // +0x344
    float savedWidth = *(float*)((char*)activeShot + 832);   // +0x340

    // Get aspect ratio from the active shot (calls GetAspectRatio)
    float aspectRatio = GetAspectRatio();

    // Calculate destination camera slot address
    void* cameraArray = *(void**)((char*)this + 24);         // +0x18 m_pCameraArray
    uint32_t slotIndex = g_activeCameraSlotIndex;
    void* destCamera = (void*)((char*)cameraArray + slotIndex * 912);

    // Copy parameters to destination slot
    float fov = *(float*)((char*)activeShot + 824);          // +0x338 m_fov
    *(float*)((char*)destCamera + 824) = fov;
    *(uint8_t*)((char*)destCamera + 864) = 1;                // +0x360 mark as active
    *(float*)((char*)destCamera + 828) = aspectRatio;        // +0x33C
    *(float*)((char*)destCamera + 832) = savedWidth;         // +0x340
    *(float*)((char*)destCamera + 836) = savedHeight;        // +0x344

    // Apply physics/bounds parameters
    phBoundCapsule_ApplyParams(destCamera);

    // Recalculate destination for transform update
    slotIndex = g_activeCameraSlotIndex;
    cameraArray = *(void**)((char*)this + 24);
    destCamera = (void*)((char*)cameraArray + slotIndex * 912);

    // Copy transform matrix from active shot (+64 offset)
    void* transformSrc = (void*)((char*)activeShot + 64);
    pongCameraMgr_UpdateTransform(destCamera, transformSrc);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::ApplyCameraConfig()  @ 0x82168CB0
// Applies a camera configuration from the data table to the active camera.
// When a pending config flag is set, copies parameters from the config table
// to the destination camera structure, then clears the flag.
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::ApplyCameraConfig() {
    extern uint8_t g_cameraPendingConfigFlag;   // @ 0x8260641A (.data, SDA +25626)
    extern uint32_t g_cameraObjectPtr;          // @ 0x8271A2EC (.data)
    extern float g_cameraConfigTable[];         // @ 0x8202D108 (.rdata) - base of config data
    extern float g_defaultFieldOfView;          // @ 0x8202C4E4 (.rdata)

    // Check if there's a pending camera configuration to apply
    if (g_cameraPendingConfigFlag != 0) {
        // Source: config table at 0x8202D10C (r10 base)
        // These are .rdata float constants for camera setup
        float fovBase = g_cameraConfigTable[0];        // +0x8202D108 (r10-4)
        float panSpeed = g_cameraConfigTable[5];       // +0x8202D128 (r10+28)
        float zoomRange = g_cameraConfigTable[30];     // +0x8202D184 (r10+120)
        float tiltSpeed = g_cameraConfigTable[1];      // +0x8202D110 (r10+4)
        float rotateSpeed = g_cameraConfigTable[0];    // +0x8202D10C (r10+0)

        // Calculate destination: g_cameraObjectPtr + 7*65536 - 29404
        // This is a camera parameter block in the game object
        uint32_t destBase = g_cameraObjectPtr + 458752 - 29404;
        char* dest = (char*)(uintptr_t)destBase;

        // Store to destination config offset via global + 429292
        *(float*)((char*)(uintptr_t)g_cameraObjectPtr + 429292) = fovBase;

        // Copy parameters to destination camera params
        *(float*)(dest + 8) = panSpeed;
        *(float*)(dest + 12) = fovBase;
        *(float*)(dest + 16) = fovBase;
        *(float*)(dest + 20) = zoomRange;
        *(float*)(dest + 24) = fovBase;
        *(float*)(dest + 4) = tiltSpeed;
        *(float*)(dest + 0) = rotateSpeed;

        // Clear the pending config flag
        g_cameraPendingConfigFlag = 0;
    }

    // Store default FOV to secondary config slot
    *(float*)((char*)(uintptr_t)g_cameraObjectPtr + 429316) = g_defaultFieldOfView;

    // Delegate to game camera setup with the flags field
    void* flagsPtr = *(void**)((char*)this + 4);
    extern void game_SetupCamera(void* flagsPtr, void* self);  // @ 0x821685F0
    game_SetupCamera(flagsPtr, this);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::FindActiveCameraNode()  @ 0x821677E8
// Walks the camera tree to find the first active (non-bypassed) camera node.
// Returns true if an active camera node is found, false otherwise.
// ─────────────────────────────────────────────────────────────────────────────
bool pongCameraMgr::FindActiveCameraNode(void* shotNode) {
    // Get the camera node array from the flags structure
    void* flagsObj = *(void**)((char*)this + 4);            // +0x04 flags
    uint32_t nodeIndex = *(uint32_t*)((char*)this + 8);     // +0x08 current index

    // Calculate array offset: index * 4
    void* nodeArray = *(void**)((char*)flagsObj + 48);      // +0x30 node lookup table
    void* currentNode = *(void**)((char*)nodeArray + nodeIndex * 4);

    // Check if current node is valid (call pongCameraMgr_C450_g)
    extern bool pongCameraMgr_IsNodeValid(void* node);       // @ 0x8220C450
    bool isValid = pongCameraMgr_IsNodeValid(currentNode);

    while (isValid) {
        // Check if node has child count > 0
        uint16_t childCount = *(uint16_t*)((char*)currentNode + 36);  // +0x24
        if (childCount != 0) {
            // Node has children, it's not a leaf - return false
            return false;
        }

        // Check bypass flag
        uint8_t isBypassed = *(uint8_t*)((char*)currentNode + 28);    // +0x1C
        if (isBypassed != 0) {
            // Node is active (bypass flag set) - found it
            return true;
        }

        // Follow the redirect chain: node->+40->+16 gives next node index
        void* redirect = *(void**)((char*)currentNode + 40);          // +0x28
        uint32_t nextNodeIndex = *(uint32_t*)((char*)redirect + 16);  // +0x10

        // Look up next node from shot node's lookup table
        void* shotLookupTable = *(void**)((char*)shotNode + 32);      // +0x20
        void* nextLookup = *(void**)((char*)shotLookupTable + nextNodeIndex * 4);

        void* nextFlagsObj = *(void**)((char*)nextLookup + 4);        // +0x04
        uint32_t nextIndex = *(uint32_t*)((char*)nextLookup + 8);     // +0x08

        void* nextArray = *(void**)((char*)nextFlagsObj + 48);        // +0x30
        currentNode = *(void**)((char*)nextArray + nextIndex * 4);

        isValid = pongCameraMgr_IsNodeValid(currentNode);
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::InitializeCameraShot()  @ 0x82167C18
// Sets up camera with shot parameters: clears global tracking state,
// loads initial camera position, and optionally applies detailed camera
// configuration from the shot hierarchy.
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::InitializeCameraShot(void* shotNode, void* cameraTarget, bool applyDetailedConfig) {
    extern uint32_t g_cameraObjectPtr;          // @ 0x8271A2EC (.data)
    extern uint32_t g_cameraShotTrackingA;      // @ 0x826064C0 (.data, SDA +25792)
    extern uint32_t g_cameraShotTrackingB;      // @ 0x826064C4 (.data, SDA +25796)
    extern float g_cameraShotFOV;               // @ 0x826064BC (.data, SDA +25788)
    extern uint32_t g_cameraShotDetailFlag;     // @ 0x826064D0 (.data, SDA +25808)

    // Clear shot tracking globals
    g_cameraShotTrackingA = 0;
    g_cameraShotTrackingB = 0;

    // Load initial FOV from the camera object at the config offset
    float initialFOV = *(float*)((char*)(uintptr_t)g_cameraObjectPtr + 429284);
    g_cameraShotFOV = initialFOV;

    // Clear detail flag
    g_cameraShotDetailFlag = 0;

    // Set camera index from shot node's parameter
    uint32_t shotParam = *(uint32_t*)((char*)shotNode + 16);  // +0x10
    *(uint32_t*)((char*)this + 8) = shotParam;

    // Apply recursive camera tree traversal
    extern void pongCameraMgr_TraverseCameraTree(void* self, void* target, uint32_t param);
    pongCameraMgr_TraverseCameraTree(this, cameraTarget, shotParam);

    if (applyDetailedConfig) {
        // Apply detailed configuration from shot hierarchy
        extern void pongCameraMgr_ApplyDetailedConfig(void* self, void* target, int detailLevel);
        pongCameraMgr_ApplyDetailedConfig(this, cameraTarget, 0);
        return;
    }

    // Check bypass flag on current node
    uint8_t isBypassed = *(uint8_t*)((char*)shotNode + 28);  // +0x1C
    if (isBypassed == 0) {
        // Node is not bypassed - look up redirect and apply config
        void* redirect = *(void**)((char*)shotNode + 40);          // +0x28
        uint32_t redirectIndex = *(uint32_t*)((char*)redirect + 16); // +0x10

        void* lookupTable = *(void**)((char*)cameraTarget + 32);   // +0x20
        void* lookedUpNode = *(void**)((char*)lookupTable + redirectIndex * 4);

        extern void pongCameraMgr_ApplyDetailedConfig(void* self, void* target, int detailLevel);
        pongCameraMgr_ApplyDetailedConfig(lookedUpNode, cameraTarget, 1);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCameraMgr::TraverseCameraTree()  @ 0x82167DF0
// Recursively traverses the camera shot tree, processing each node and its
// children. For leaf nodes with bypass flags, processes their redirect targets.
// For nodes with children, iterates through child list and processes sub-trees.
// ─────────────────────────────────────────────────────────────────────────────
void pongCameraMgr::TraverseCameraTree(void* cameraTarget, uint32_t shotIndex) {
    // Look up the camera node from the flags structure
    void* flagsObj = *(void**)((char*)this + 4);            // +0x04
    void* nodeArray = *(void**)((char*)flagsObj + 48);      // +0x30
    void* currentNode = *(void**)((char*)nodeArray + shotIndex * 4);

    // Store current shot index
    *(uint32_t*)((char*)this + 8) = shotIndex;

    // Check if current node's bypass flag is clear (not bypassed)
    uint8_t isBypassed = *(uint8_t*)((char*)currentNode + 28);  // +0x1C
    if (isBypassed == 0) {
        // Follow redirect: node->+40->+16 gives next index
        void* redirect = *(void**)((char*)currentNode + 40);     // +0x28
        uint32_t nextIndex = *(uint32_t*)((char*)redirect + 16); // +0x10

        // Look up from camera target's table
        void* lookupTable = *(void**)((char*)cameraTarget + 32); // +0x20
        void* nextNode = *(void**)((char*)lookupTable + nextIndex * 4);

        // Recurse into redirect target
        TraverseCameraTree(cameraTarget, nextIndex);
    }

    // Process children
    int16_t childCount = *(int16_t*)((char*)currentNode + 36);  // +0x24
    if (childCount > 0) {
        for (int i = 0; i < childCount; i++) {
            // Get child from children array
            void* childArray = *(void**)((char*)currentNode + 32);  // +0x20
            void* child = *(void**)((char*)childArray + i * 4);

            // Get the camera node from child's reference
            void* childCamNode = *(void**)((char*)child + 52);      // +0x34

            // Check if child's bypass flag is clear
            uint8_t childBypassed = *(uint8_t*)((char*)childCamNode + 28);  // +0x1C
            if (childBypassed == 0) {
                // Process child's redirect target
                void* childRedirect = *(void**)((char*)childCamNode + 40);  // +0x28
                extern void pg_ProcessCameraNode(void* target, void* redirect);  // @ 0x82167CE0
                pg_ProcessCameraNode(cameraTarget, childRedirect);
            }
        }
    }
}
