/**
 * timer.hpp - RAGE Timer System
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Timer structure (placeholder)
typedef struct Timer {
    uint32_t alertTime;
    uint32_t flags;
} Timer;

// Timer functions
void Timer_SetAlert(Timer* timer, uint32_t time);

#ifdef __cplusplus
}
#endif
