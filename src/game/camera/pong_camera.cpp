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
