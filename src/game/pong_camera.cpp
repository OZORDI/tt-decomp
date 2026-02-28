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
