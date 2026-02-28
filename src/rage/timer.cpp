#include "rage/timer.hpp"

#include <cstring>

extern "C" void nop_8240E6D0(const char* fmt, ...);

namespace {
constexpr uint32_t kMaxQueuedAlerts = 4;
constexpr size_t kAlertRuntimeStateIndex = 2;  // +0x1C within each 32-byte slot
constexpr const char* kAlertQueueFullMessage =
    "Timer::SetAlert - could not set alert as the alert queue is full!";
}  // namespace

TimerAlert::TimerAlert() : m_time(0.0f), m_flags(0) {
    std::memset(m_callbackData, 0, sizeof(m_callbackData));
}

void Timer::SetAlert(TimerAlert* alert) {
    if (m_alertCount >= kMaxQueuedAlerts) {
        nop_8240E6D0(kAlertQueueFullMessage);
        return;
    }

    TimerAlert& queuedAlert = m_alerts[m_alertCount];

    // The original queue write only copies payload fields, then clears runtime state.
    queuedAlert.m_time = alert->m_time;
    std::memcpy(queuedAlert.m_callbackData, alert->m_callbackData,
                sizeof(queuedAlert.m_callbackData));
    queuedAlert.m_flags = alert->m_flags;
    queuedAlert.m_padding[kAlertRuntimeStateIndex] = 0;

    ++m_alertCount;
}

