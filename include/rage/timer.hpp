/**
 * Timer and TimerAlert declarations.
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <cstdint>

// 16-byte callback payload copied verbatim by Timer::SetAlert.
struct TimerAlertCallbackPayload {
    uint32_t m_aWords[4];
};

// Runtime bytes at the tail of each 32-byte alert slot.
struct TimerAlertRuntimeState {
    uint8_t m_phase;
    uint8_t m_subPhase;
    bool m_isQueued;
    uint8_t m_aReserved[3];
};

class TimerAlert {
public:
    TimerAlert();
    virtual ~TimerAlert() = default;

    float m_triggerTime;
    TimerAlertCallbackPayload m_callbackPayload;
    uint16_t m_flags;
    TimerAlertRuntimeState m_runtimeState;
};

class Timer {
public:
    static constexpr uint32_t kMaxAlerts = 4;

    void SetAlert(const TimerAlert* pAlert);

    // Fields before +0x2C are still only partially understood.
    bool m_isEnabled;
    uint8_t m_aReserved00[3];
    uint32_t m_alertMode;
    float m_currentTime;
    float m_previousTime;
    uint8_t m_aInternalState[28];

    TimerAlert m_aAlerts[kMaxAlerts];
    uint32_t m_numQueuedAlerts;
};
