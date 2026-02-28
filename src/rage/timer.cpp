#include "rage/timer.hpp"

#include <cstring>

extern "C" void nop_8240E6D0(const char* fmt, ...);

namespace {
constexpr const char* kAlertQueueFullMessage =
    "Timer::SetAlert - could not set alert as the alert queue is full!";
}  // namespace

TimerAlert::TimerAlert() : m_triggerTime(0.0f), m_flags(0) {
    std::memset(m_callbackPayload.m_aWords, 0, sizeof(m_callbackPayload.m_aWords));
}

void Timer::SetAlert(const TimerAlert* pAlert) {
    if (m_numQueuedAlerts >= kMaxAlerts) {
        nop_8240E6D0(kAlertQueueFullMessage);
        return;
    }

    TimerAlert& queuedAlert = m_aAlerts[m_numQueuedAlerts];

    // The original queue write only copies payload fields, then clears runtime state.
    queuedAlert.m_triggerTime = pAlert->m_triggerTime;
    queuedAlert.m_callbackPayload = pAlert->m_callbackPayload;
    queuedAlert.m_flags = pAlert->m_flags;
    queuedAlert.m_runtimeState.m_isQueued = false;

    ++m_numQueuedAlerts;
}
