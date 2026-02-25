/**
 * Timer and TimerAlert Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "rage/timer.hpp"
#include <stdio.h>

/**
 * TimerAlert::TimerAlert
 * @ 0x821DC708 | size: 0x48
 * 
 * Constructor - Initializes all TimerAlert fields to zero.
 * 
 * Assembly pattern:
 *   - Loads 0.0f constant from data section
 *   - Stores to m_time at +4
 *   - Copies 16 bytes of zeros to m_callbackData at +8
 *   - Stores 0 to m_flags at +24
 */
TimerAlert::TimerAlert() {
    m_time = 0.0f;
    memset(m_callbackData, 0, sizeof(m_callbackData));
    m_flags = 0;
    memset(m_padding, 0, sizeof(m_padding));
}

/**
 * Timer::SetAlert
 * @ 0x821DC7A0 | size: 0xE0
 * 
 * Adds an alert to the timer's alert array.
 * Maximum of 4 alerts can be registered. If limit is reached,
 * prints error message and returns without adding.
 * 
 * Assembly details:
 *   - Checks m_alertCount at +172 against limit of 4
 *   - Uses rlwinm r8,r11,5,0,26 to calculate offset (count * 32)
 *   - Copies 32 bytes from source alert to array slot
 *   - Increments alert count
 *   - Error message at 0x8203F210: "Timer::SetAlert - too many alerts"
 * 
 * @param alert Pointer to TimerAlert to copy into the array
 */
void Timer::SetAlert(TimerAlert* alert) {
    // Check if we've reached the maximum number of alerts (4)
    if (m_alertCount >= 4) {
        // Error: "Timer::SetAlert - too many alerts" @ 0x8203F210
        // Original calls nop_8240E6D0 (likely a debug print function)
        fprintf(stderr, "Timer::SetAlert - too many alerts\n");
        return;
    }
    
    // Calculate offset into alert array
    // Original: rlwinm r8,r11,5,0,26 = (count << 5) & 0xFFFFFFE0
    // This multiplies count by 32 (size of each alert slot)
    uint32_t offset = m_alertCount * 32;
    
    // Copy the alert data into the array (32 bytes)
    // Original copies in multiple 4-byte chunks
    uint8_t* dest = reinterpret_cast<uint8_t*>(&m_alerts[0]) + offset;
    memcpy(dest, alert, 32);
    
    // Increment alert count
    m_alertCount++;
}
