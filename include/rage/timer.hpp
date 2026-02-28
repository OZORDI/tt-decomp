/**
 * Timer and TimerAlert Classes
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Core timer system with alert/callback functionality.
 * Supports up to 4 simultaneous alerts per timer.
 */

#pragma once

#include <stdint.h>
#include <string.h>

// Forward declarations
class Timer;

/**
 * TimerAlert
 * @ RTTI: 0x82030CB8
 * 
 * Represents a timed alert/callback that can be registered with a Timer.
 * Contains timing information, callback data, and flags.
 */
class TimerAlert {
public:
    // @ 0x821DC708 | size: 0x48
    TimerAlert();
    virtual ~TimerAlert() {}
    float m_time;                    // +0x04 - Time value for the alert
    uint8_t m_callbackData[16];      // +0x08 - Callback function pointer and context data
    uint16_t m_flags;                // +0x18 - Alert flags/state
    uint8_t m_padding[6];            // +0x1A - Padding to 32-byte boundary
};

/**
 * Timer
 * @ RTTI: 0x8203F258 (Timer::sAlerts)
 * 
 * Timer class that manages up to 4 TimerAlert objects.
 * Alerts are stored in a fixed-size array with 32-byte slots.
 */
class Timer {
public:
    // @ 0x821DC7A0 | size: 0xE0
    void SetAlert(TimerAlert* alert);

    uint8_t m_padding[44];           // +0x00 to +0x2B - Unknown fields
    TimerAlert m_alerts[4];          // +0x2C (44) - Array of 4 alerts, 32 bytes each = 128 bytes
    uint32_t m_alertCount;           // +0xAC (172) - Number of active alerts (max 4)
};

/* size verification removed: offsetof requires public members */
