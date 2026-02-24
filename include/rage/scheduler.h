#pragma once

#include <cstdint>

// Forward declaration
typedef void (*JobFunction)(void* userData);

// Scheduler initialization and shutdown
void scheduler_Init(void);
void scheduler_Shutdown(void);

// Frame execution phases
void scheduler_StepFrame(void);
void scheduler_Update(void);
void scheduler_Tick(void);
void scheduler_BeginRender(void);
void scheduler_EndRender(void);
void scheduler_PostRender(void);
void scheduler_Present(void);

// Timing queries
float scheduler_GetDeltaTime(void);
uint32_t scheduler_GetFrameCount(void);

// Task registration
void scheduler_RegisterUpdateTask(JobFunction task);
void scheduler_RegisterTickTask(JobFunction task);
void scheduler_RegisterRenderTask(JobFunction task);
