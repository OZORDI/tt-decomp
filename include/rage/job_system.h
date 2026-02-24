#pragma once

#include <cstdint>
#include <functional>

/**
 * rage/job_system.h
 * 
 * RAGE Engine Job System
 * Multi-threaded task execution with work-stealing queues
 * 
 * Original Xbox 360 implementation used hardware threads.
 * This version uses std::thread for cross-platform compatibility.
 */

// Forward declarations
struct Job;
struct JobQueue;
struct WorkerThread;

// Job priority levels
enum JobPriority {
    JOB_PRIORITY_LOW = 0,
    JOB_PRIORITY_NORMAL = 1,
    JOB_PRIORITY_HIGH = 2,
    JOB_PRIORITY_CRITICAL = 3
};

// Job function signature
typedef void (*JobFunction)(void* userData);

// Job system lifecycle
void jobSystem_Init(uint32_t numWorkers);
void jobSystem_Shutdown(void);

// Job creation and submission
Job* jobSystem_CreateJob(JobFunction func, void* userData, JobPriority priority);
void jobSystem_SubmitJob(Job* job);
void jobSystem_WaitForJob(Job* job);
void jobSystem_WaitForAll(void);

// Worker thread management
uint32_t jobSystem_GetWorkerCount(void);
uint32_t jobSystem_GetActiveJobCount(void);

// Utility functions
void jobSystem_ExecuteOnMainThread(JobFunction func, void* userData);
bool jobSystem_IsMainThread(void);
