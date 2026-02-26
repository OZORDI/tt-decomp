/**
 * Xbox 360 Kernel API Implementation - Cross-Platform
 * Rockstar Presents Table Tennis
 * 
 * Implements Xbox 360 kernel functions using POSIX threads and standard C library.
 * Provides TLS, synchronization primitives, and memory management.
 */

#include "xam/kernel.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

//=============================================================================
// Thread-Local Storage Implementation
//=============================================================================

#define MAX_TLS_SLOTS 64

static struct {
    pthread_key_t key;
    uint8_t allocated;
} g_tls_slots[MAX_TLS_SLOTS] = {0};

static pthread_mutex_t g_tls_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * KeTlsAlloc @ 0x8258623C
 */
uint32_t KeTlsAlloc(void) {
    pthread_mutex_lock(&g_tls_mutex);
    
    for (uint32_t i = 0; i < MAX_TLS_SLOTS; i++) {
        if (!g_tls_slots[i].allocated) {
            if (pthread_key_create(&g_tls_slots[i].key, NULL) == 0) {
                g_tls_slots[i].allocated = 1;
                pthread_mutex_unlock(&g_tls_mutex);
                return i;
            }
        }
    }
    
    pthread_mutex_unlock(&g_tls_mutex);
    return 0xFFFFFFFF;  // No slots available
}

/**
 * KeTlsFree @ 0x8258624C
 */
uint32_t KeTlsFree(uint32_t dwTlsIndex) {
    if (dwTlsIndex >= MAX_TLS_SLOTS) {
        return 0;
    }
    
    pthread_mutex_lock(&g_tls_mutex);
    
    if (g_tls_slots[dwTlsIndex].allocated) {
        pthread_key_delete(g_tls_slots[dwTlsIndex].key);
        g_tls_slots[dwTlsIndex].allocated = 0;
        pthread_mutex_unlock(&g_tls_mutex);
        return 1;
    }
    
    pthread_mutex_unlock(&g_tls_mutex);
    return 0;
}

/**
 * KeTlsGetValue @ 0x8258621C
 */
void* KeTlsGetValue(uint32_t dwTlsIndex) {
    if (dwTlsIndex >= MAX_TLS_SLOTS || !g_tls_slots[dwTlsIndex].allocated) {
        return NULL;
    }
    
    return pthread_getspecific(g_tls_slots[dwTlsIndex].key);
}

/**
 * KeTlsSetValue @ 0x8258622C
 */
uint32_t KeTlsSetValue(uint32_t dwTlsIndex, void* lpTlsValue) {
    if (dwTlsIndex >= MAX_TLS_SLOTS || !g_tls_slots[dwTlsIndex].allocated) {
        return 0;
    }
    
    return pthread_setspecific(g_tls_slots[dwTlsIndex].key, lpTlsValue) == 0 ? 1 : 0;
}

//=============================================================================
// Event Synchronization Implementation
//=============================================================================

// Event object structure (maps to pthread condition variable + mutex)
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int32_t signaled;
    int32_t manual_reset;  // 0 = auto-reset, 1 = manual-reset
} KernelEvent;

/**
 * KeSetEvent @ 0x825864EC
 */
int32_t KeSetEvent(void* Event, int32_t Increment, uint32_t Wait) {
    (void)Increment;  // Priority increment not used in cross-platform
    (void)Wait;
    
    if (!Event) {
        return 0;
    }
    
    KernelEvent* event = (KernelEvent*)Event;
    pthread_mutex_lock(&event->mutex);
    
    int32_t previous_state = event->signaled;
    event->signaled = 1;
    
    // Wake up waiting threads
    if (event->manual_reset) {
        pthread_cond_broadcast(&event->cond);
    } else {
        pthread_cond_signal(&event->cond);
    }
    
    pthread_mutex_unlock(&event->mutex);
    return previous_state;
}

/**
 * KeResetEvent @ 0x825864DC
 */
int32_t KeResetEvent(void* Event) {
    if (!Event) {
        return 0;
    }
    
    KernelEvent* event = (KernelEvent*)Event;
    pthread_mutex_lock(&event->mutex);
    
    int32_t previous_state = event->signaled;
    event->signaled = 0;
    
    pthread_mutex_unlock(&event->mutex);
    return previous_state;
}

/**
 * KeWaitForSingleObject @ 0x8258652C
 */
NTSTATUS KeWaitForSingleObject(void* Object, uint32_t WaitReason, uint32_t WaitMode,
                                uint32_t Alertable, LARGE_INTEGER* Timeout) {
    (void)WaitReason;
    (void)WaitMode;
    (void)Alertable;
    
    if (!Object) {
        return STATUS_INVALID_PARAMETER;
    }
    
    KernelEvent* event = (KernelEvent*)Object;
    pthread_mutex_lock(&event->mutex);
    
    // If already signaled, return immediately
    if (event->signaled) {
        if (!event->manual_reset) {
            event->signaled = 0;  // Auto-reset
        }
        pthread_mutex_unlock(&event->mutex);
        return STATUS_SUCCESS;
    }
    
    // Wait for signal
    int result;
    if (Timeout == NULL) {
        // Infinite wait
        while (!event->signaled) {
            result = pthread_cond_wait(&event->cond, &event->mutex);
            if (result != 0) {
                pthread_mutex_unlock(&event->mutex);
                return STATUS_TIMEOUT;
            }
        }
    } else {
        // Timed wait (Timeout is in 100-nanosecond intervals)
        int64_t timeout_ns = (*Timeout) * 100;
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout_ns / 1000000000LL;
        ts.tv_nsec += timeout_ns % 1000000000LL;
        if (ts.tv_nsec >= 1000000000LL) {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000LL;
        }
        
        while (!event->signaled) {
            result = pthread_cond_timedwait(&event->cond, &event->mutex, &ts);
            if (result == ETIMEDOUT) {
                pthread_mutex_unlock(&event->mutex);
                return STATUS_TIMEOUT;
            } else if (result != 0) {
                pthread_mutex_unlock(&event->mutex);
                return STATUS_TIMEOUT;
            }
        }
    }
    
    if (!event->manual_reset) {
        event->signaled = 0;  // Auto-reset
    }
    
    pthread_mutex_unlock(&event->mutex);
    return STATUS_SUCCESS;
}

//=============================================================================
// Semaphore Implementation
//=============================================================================

// Semaphore structure (maps to pthread condition variable + counter)
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int32_t count;
    int32_t max_count;
} KernelSemaphore;

/**
 * NtCreateSemaphore @ 0x8258665C
 */
NTSTATUS NtCreateSemaphore(HANDLE* SemaphoreHandle, uint32_t DesiredAccess,
                           void* ObjectAttributes, int32_t InitialCount, int32_t MaximumCount) {
    (void)DesiredAccess;
    (void)ObjectAttributes;
    
    if (!SemaphoreHandle || InitialCount < 0 || MaximumCount <= 0 || InitialCount > MaximumCount) {
        return STATUS_INVALID_PARAMETER;
    }
    
    KernelSemaphore* sem = (KernelSemaphore*)malloc(sizeof(KernelSemaphore));
    if (!sem) {
        return STATUS_NO_MEMORY;
    }
    
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
    sem->count = InitialCount;
    sem->max_count = MaximumCount;
    
    *SemaphoreHandle = (HANDLE)sem;
    return STATUS_SUCCESS;
}

/**
 * NtReleaseSemaphore @ 0x8258666C
 */
NTSTATUS NtReleaseSemaphore(HANDLE SemaphoreHandle, int32_t ReleaseCount, int32_t* PreviousCount) {
    if (!SemaphoreHandle || ReleaseCount <= 0) {
        return STATUS_INVALID_PARAMETER;
    }
    
    KernelSemaphore* sem = (KernelSemaphore*)SemaphoreHandle;
    pthread_mutex_lock(&sem->mutex);
    
    if (PreviousCount) {
        *PreviousCount = sem->count;
    }
    
    // Check for overflow
    if (sem->count + ReleaseCount > sem->max_count) {
        pthread_mutex_unlock(&sem->mutex);
        return STATUS_INVALID_PARAMETER;
    }
    
    sem->count += ReleaseCount;
    
    // Wake up waiting threads
    for (int32_t i = 0; i < ReleaseCount; i++) {
        pthread_cond_signal(&sem->cond);
    }
    
    pthread_mutex_unlock(&sem->mutex);
    return STATUS_SUCCESS;
}

//=============================================================================
// Critical Region Implementation
//=============================================================================

// Critical regions on Xbox 360 disable kernel APCs
// For cross-platform, we use a simple counter per thread
static __thread int32_t g_critical_region_count = 0;

/**
 * KeEnterCriticalRegion @ 0x825864AC
 */
void KeEnterCriticalRegion(void) {
    g_critical_region_count++;
}

/**
 * KeLeaveCriticalRegion @ 0x8258649C
 */
void KeLeaveCriticalRegion(void) {
    if (g_critical_region_count > 0) {
        g_critical_region_count--;
    }
}

//=============================================================================
// Memory Management Implementation (XAM)
//=============================================================================

/**
 * XamAlloc @ 0x8258676C
 */
void* XamAlloc(uint32_t dwFlags, uint32_t dwSize) {
    (void)dwFlags;
    
    if (dwSize == 0) {
        return NULL;
    }
    
    void* ptr = malloc(dwSize);
    if (ptr) {
        memset(ptr, 0, dwSize);  // XamAlloc zero-initializes
    }
    
    return ptr;
}

/**
 * XamFree @ 0x8258675C
 */
void XamFree(void* pAddress) {
    if (pAddress) {
        free(pAddress);
    }
}

//=============================================================================
// Performance Counter Implementation
//=============================================================================

/**
 * KeQueryPerformanceFrequency @ 0x825865EC
 */
uint64_t KeQueryPerformanceFrequency(void) {
#ifdef __APPLE__
    // macOS: Use mach_timebase_info
    static mach_timebase_info_data_t timebase = {0};
    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    // Convert to Hz (nanoseconds to seconds)
    return 1000000000ULL * timebase.denom / timebase.numer;
#else
    // Linux/Windows: Assume nanosecond precision
    return 1000000000ULL;  // 1 GHz
#endif
}

//=============================================================================
// Yield/Sleep Implementation
//=============================================================================

/**
 * NtYieldExecution @ 0x8258659C
 */
NTSTATUS NtYieldExecution(void) {
    // Yield to other threads
    #ifdef _WIN32
        SwitchToThread();
    #else
        sched_yield();
    #endif
    
    return STATUS_SUCCESS;
}

//=============================================================================
// Memory Management Implementation (Physical Memory)
//=============================================================================

/**
 * MmAllocatePhysicalMemoryEx @ 0x8258660C
 * 
 * Allocates physical memory with extended options.
 * For cross-platform, we use aligned_alloc or posix_memalign.
 */
void* MmAllocatePhysicalMemoryEx(uint32_t dwFlags, uint32_t dwMinAddress,
                                  uint32_t dwMaxAddress, uint32_t dwAlignment,
                                  uint32_t dwProtect) {
    (void)dwFlags;
    (void)dwMinAddress;
    (void)dwMaxAddress;
    (void)dwProtect;
    
    // Ensure alignment is power of 2 and at least sizeof(void*)
    if (dwAlignment == 0) {
        dwAlignment = 16;  // Default to 16-byte alignment
    }
    
    // Calculate size (not provided, use a reasonable default for audio buffers)
    size_t size = 65536;  // 64KB default
    
    void* ptr = NULL;
    
#ifdef _WIN32
    ptr = _aligned_malloc(size, dwAlignment);
#else
    if (posix_memalign(&ptr, dwAlignment, size) != 0) {
        ptr = NULL;
    }
#endif
    
    if (ptr) {
        memset(ptr, 0, size);
    }
    
    return ptr;
}

/**
 * MmFreePhysicalMemory @ 0x8258635C
 * 
 * Frees physical memory.
 */
void MmFreePhysicalMemory(uint32_t dwFlags, void* pAddress) {
    (void)dwFlags;
    
    if (!pAddress) {
        return;
    }
    
#ifdef _WIN32
    _aligned_free(pAddress);
#else
    free(pAddress);
#endif
}

/**
 * MmQueryAllocationSize @ 0x8258661C
 * 
 * Queries the size of a physical memory allocation.
 * 
 * Note: This is difficult to implement accurately without tracking allocations.
 * Return a reasonable estimate.
 */
uint32_t MmQueryAllocationSize(void* pAddress) {
    if (!pAddress) {
        return 0;
    }
    
    // Return default size (we don't track allocations)
    return 65536;  // 64KB
}

/**
 * MmQueryStatistics @ 0x825865FC
 * 
 * Queries system memory statistics.
 */
NTSTATUS MmQueryStatistics(MM_STATISTICS* pStatistics) {
    if (!pStatistics) {
        return STATUS_INVALID_PARAMETER;
    }
    
    // Return fake statistics
    memset(pStatistics, 0, sizeof(MM_STATISTICS));
    
    // Fake values (Xbox 360 has 512MB RAM)
    pStatistics->TotalPhysicalPages = 131072;  // 512MB / 4KB pages
    pStatistics->AvailablePages = 65536;       // 256MB available
    pStatistics->CachedPages = 16384;          // 64MB cached
    pStatistics->PoolPages = 8192;             // 32MB pool
    
    return STATUS_SUCCESS;
}

/**
 * MmQueryAddressProtect @ 0x825862AC
 * 
 * Queries the protection flags for a memory address.
 */
uint32_t MmQueryAddressProtect(void* pAddress) {
    (void)pAddress;
    
    // Return read/write/execute (typical for game memory)
    return 0x00000007;  // PAGE_EXECUTE_READWRITE
}

//=============================================================================
// Debug Functions Implementation
//=============================================================================

/**
 * DbgBreakPoint @ 0x8258662C
 * 
 * Triggers a breakpoint for debugging.
 */
void DbgBreakPoint(void) {
#ifdef _WIN32
    __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
    __builtin_trap();
#else
    // Fallback: raise SIGTRAP
    raise(SIGTRAP);
#endif
}
