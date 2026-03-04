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
    struct TransitionParams {
        float value;
        uint32_t vtableOrState;  // Stores vtable pointer from this
        uint32_t padding;
    } params;
    
    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;  // Copy vtable/state
    params.padding = 0;
    
    struct TransitionFlags {
        uint32_t flag1;  // Set to 3
        uint32_t flag2;  // Set to 1
    } flags;
    
    flags.flag1 = 3;
    flags.flag2 = 1;
    
    // Call validation function (pongCameraMgr_6E08)
    // Parameters: gameState, &params, 7, &flags, 1
    // TODO: Replace with proper function signature when available
    extern uint8_t pongCameraMgr_ValidateTransition(void* gameState, TransitionParams* params, 
                                                     int mode, TransitionFlags* flags, int enable);
    
    uint8_t validationResult = pongCameraMgr_ValidateTransition(gameState, &params, 7, &flags, 1);
    
    if (validationResult != 0) {
        // Validation succeeded, check if we need to apply the transition
        float modifiedValue = params.value;
        
        if (modifiedValue <= clampedMax) {
            // Value is within acceptable range, apply transition with different mode
            // TODO: Replace with proper function signature when available
            extern uint8_t pg_ApplyTransition(void* gameState, TransitionParams* params,
                                             int mode, TransitionFlags* flags, int enable);
            
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
    struct TransitionParams {
        float value;
        uint32_t vtableOrState;
        uint32_t padding;
    } params;
    
    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;
    params.padding = 0;
    
    // Call validation function
    extern uint8_t pongCameraMgr_6E08(void* gameState, TransitionParams* params, 
                                      int mode, void* unused1, int unused2);
    
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
    struct TransitionParams {
        float value;
        uint32_t vtableOrState;
        uint32_t padding;
    } params;
    
    struct TransitionFlags {
        uint32_t flag1;  // Set to 3
        uint32_t flag2;  // Set to 1
    } flags;
    
    params.value = clampedMin;
    params.vtableOrState = *(uint32_t*)this;
    params.padding = 0;
    
    flags.flag1 = 3;
    flags.flag2 = 1;
    
    // Call validation function with flags
    extern uint8_t pongCameraMgr_6E08(void* gameState, TransitionParams* params, 
                                      int mode, TransitionFlags* flags, int enable);
    
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
    // Call validation function directly with this object as parameter
    extern uint8_t pongCameraMgr_6E08(void* gameState, pongCameraMgr* mgr, 
                                      int mode, void* unused1, int unused2);
    
    uint8_t validationResult = pongCameraMgr_6E08(gameState, this, mode, nullptr, 0);
    
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
