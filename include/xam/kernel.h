#pragma once

/**
 * Xbox 360 Kernel API - Core System Functions
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks for NT kernel, Kernel Executive (Ke), and XAM functions.
 * Critical for threading, synchronization, TLS, and memory management.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Xbox 360 error codes
#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS                0
#endif
#define STATUS_SUCCESS               0
#define STATUS_TIMEOUT               0x00000102
#define STATUS_PENDING               0x00000103
#define STATUS_NO_MEMORY             0xC0000017
#define STATUS_INVALID_PARAMETER     0xC000000D

// Wait result codes
#define WAIT_OBJECT_0                0
#define WAIT_TIMEOUT                 0x00000102
#define WAIT_FAILED                  0xFFFFFFFF

// Wait timeout constants
#define INFINITE                     0xFFFFFFFF

// Forward declarations
typedef void* HANDLE;
typedef void* PVOID;
typedef uint32_t DWORD;
typedef int32_t NTSTATUS;
typedef int64_t LARGE_INTEGER;

// Event types
typedef enum _EVENT_TYPE {
    NotificationEvent = 0,
    SynchronizationEvent = 1
} EVENT_TYPE;

//=============================================================================
// Thread-Local Storage (TLS) Functions
//=============================================================================

/**
 * KeTlsAlloc @ 0x8258623C | ordinal: varies
 * 
 * Allocates a thread-local storage (TLS) slot.
 * 
 * @return TLS index (0-63), or 0xFFFFFFFF if no slots available
 */
uint32_t KeTlsAlloc(void);

/**
 * KeTlsFree @ 0x8258624C | ordinal: varies
 * 
 * Frees a previously allocated TLS slot.
 * 
 * @param dwTlsIndex TLS index to free
 * @return 1 if successful, 0 if failed
 */
uint32_t KeTlsFree(uint32_t dwTlsIndex);

/**
 * KeTlsGetValue @ 0x8258621C | ordinal: varies
 * 
 * Retrieves the value stored in the TLS slot for the calling thread.
 * 
 * @param dwTlsIndex TLS index
 * @return Value stored in TLS slot, or NULL if not set
 */
void* KeTlsGetValue(uint32_t dwTlsIndex);

/**
 * KeTlsSetValue @ 0x8258622C | ordinal: varies
 * 
 * Stores a value in the TLS slot for the calling thread.
 * 
 * @param dwTlsIndex TLS index
 * @param lpTlsValue Value to store
 * @return 1 if successful, 0 if failed
 */
uint32_t KeTlsSetValue(uint32_t dwTlsIndex, void* lpTlsValue);

//=============================================================================
// Event Synchronization Functions
//=============================================================================

/**
 * KeSetEvent @ 0x825864EC | ordinal: varies
 * 
 * Sets an event object to the signaled state.
 * 
 * @param Event      Pointer to event object
 * @param Increment  Priority increment for waiting threads
 * @param Wait       TRUE if caller will immediately wait
 * @return Previous state of the event (0 = not signaled, 1 = signaled)
 */
int32_t KeSetEvent(void* Event, int32_t Increment, uint32_t Wait);

/**
 * KeResetEvent @ 0x825864DC | ordinal: varies
 * 
 * Resets an event object to the non-signaled state.
 * 
 * @param Event Pointer to event object
 * @return Previous state of the event (0 = not signaled, 1 = signaled)
 */
int32_t KeResetEvent(void* Event);

/**
 * KeWaitForSingleObject @ 0x8258652C | ordinal: varies
 * 
 * Waits for a kernel object to be signaled.
 * 
 * @param Object        Pointer to object to wait on
 * @param WaitReason    Reason for waiting (usually 0)
 * @param WaitMode      0 = KernelMode, 1 = UserMode
 * @param Alertable     TRUE if wait is alertable
 * @param Timeout       Timeout in 100-nanosecond intervals (NULL = infinite)
 * @return STATUS_SUCCESS, STATUS_TIMEOUT, or error code
 */
NTSTATUS KeWaitForSingleObject(void* Object, uint32_t WaitReason, uint32_t WaitMode,
                                uint32_t Alertable, LARGE_INTEGER* Timeout);

//=============================================================================
// Semaphore Functions
//=============================================================================

/**
 * NtCreateSemaphore @ 0x8258665C | ordinal: varies
 * 
 * Creates a semaphore object.
 * 
 * @param SemaphoreHandle Pointer to receive semaphore handle
 * @param DesiredAccess   Access rights (usually 0x001F0003)
 * @param ObjectAttributes Object attributes (usually NULL)
 * @param InitialCount    Initial count
 * @param MaximumCount    Maximum count
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS NtCreateSemaphore(HANDLE* SemaphoreHandle, uint32_t DesiredAccess,
                           void* ObjectAttributes, int32_t InitialCount, int32_t MaximumCount);

/**
 * NtReleaseSemaphore @ 0x8258666C | ordinal: varies
 * 
 * Releases a semaphore object.
 * 
 * @param SemaphoreHandle Semaphore handle
 * @param ReleaseCount    Number to add to semaphore count
 * @param PreviousCount   Pointer to receive previous count (can be NULL)
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS NtReleaseSemaphore(HANDLE SemaphoreHandle, int32_t ReleaseCount, int32_t* PreviousCount);

//=============================================================================
// Critical Region Functions
//=============================================================================

/**
 * KeEnterCriticalRegion @ 0x825864AC | ordinal: varies
 * 
 * Disables normal kernel APC delivery to the current thread.
 * Used to protect critical code sections from asynchronous interruption.
 */
void KeEnterCriticalRegion(void);

/**
 * KeLeaveCriticalRegion @ 0x8258649C | ordinal: varies
 * 
 * Re-enables normal kernel APC delivery to the current thread.
 * Must be called after KeEnterCriticalRegion.
 */
void KeLeaveCriticalRegion(void);

//=============================================================================
// Memory Management Functions (XAM)
//=============================================================================

/**
 * XamAlloc @ 0x8258676C | ordinal: varies
 * 
 * Allocates memory from the XAM heap.
 * 
 * @param dwFlags Allocation flags (usually 0)
 * @param dwSize  Size in bytes to allocate
 * @return Pointer to allocated memory, or NULL if failed
 */
void* XamAlloc(uint32_t dwFlags, uint32_t dwSize);

/**
 * XamFree @ 0x8258675C | ordinal: varies
 * 
 * Frees memory allocated by XamAlloc.
 * 
 * @param pAddress Pointer to memory to free
 */
void XamFree(void* pAddress);

//=============================================================================
// Performance Counter Functions
//=============================================================================

/**
 * KeQueryPerformanceFrequency @ 0x825865EC | ordinal: varies
 * 
 * Returns the frequency of the performance counter in Hz.
 * On Xbox 360, this is typically 50,000,000 Hz (50 MHz).
 * 
 * @return Performance counter frequency
 */
uint64_t KeQueryPerformanceFrequency(void);

//=============================================================================
// Yield/Sleep Functions
//=============================================================================

/**
 * NtYieldExecution @ 0x8258659C | ordinal: varies
 * 
 * Yields the remainder of the thread's time slice to another ready thread.
 * 
 * @return STATUS_SUCCESS if another thread was scheduled, STATUS_NO_YIELD_PERFORMED otherwise
 */
NTSTATUS NtYieldExecution(void);

//=============================================================================
// Memory Management Functions (Physical Memory)
//=============================================================================

// Memory statistics structure
typedef struct _MM_STATISTICS {
    uint32_t TotalPhysicalPages;
    uint32_t AvailablePages;
    uint32_t CachedPages;
    uint32_t PoolPages;
    uint32_t StackPages;
    uint32_t ImagePages;
    uint32_t HeapPages;
    uint32_t VirtualPages;
    uint32_t PagedPoolPages;
    uint32_t NonPagedPoolPages;
    uint32_t AvailableExtendedPages;
} MM_STATISTICS;

/**
 * MmAllocatePhysicalMemoryEx @ 0x8258660C | ordinal: varies
 * 
 * Allocates physical memory with extended options.
 * Used for audio buffers, video memory, and other hardware-accessible memory.
 * 
 * @param dwFlags         Allocation flags
 * @param dwMinAddress    Minimum physical address
 * @param dwMaxAddress    Maximum physical address
 * @param dwAlignment     Alignment requirement (must be power of 2)
 * @param dwProtect       Memory protection flags
 * @return Physical address of allocated memory, or 0 if failed
 */
void* MmAllocatePhysicalMemoryEx(uint32_t dwFlags, uint32_t dwMinAddress,
                                  uint32_t dwMaxAddress, uint32_t dwAlignment,
                                  uint32_t dwProtect);

/**
 * MmFreePhysicalMemory @ 0x8258635C | ordinal: varies
 * 
 * Frees physical memory allocated by MmAllocatePhysicalMemoryEx.
 * 
 * @param dwFlags   Flags (usually 0)
 * @param pAddress  Physical address to free
 */
void MmFreePhysicalMemory(uint32_t dwFlags, void* pAddress);

/**
 * MmQueryAllocationSize @ 0x8258661C | ordinal: varies
 * 
 * Queries the size of a physical memory allocation.
 * 
 * @param pAddress Physical address
 * @return Size of allocation in bytes, or 0 if invalid
 */
uint32_t MmQueryAllocationSize(void* pAddress);

/**
 * MmQueryStatistics @ 0x825865FC | ordinal: varies
 * 
 * Queries system memory statistics.
 * 
 * @param pStatistics Pointer to receive statistics
 * @return STATUS_SUCCESS if successful
 */
NTSTATUS MmQueryStatistics(MM_STATISTICS* pStatistics);

/**
 * MmQueryAddressProtect @ 0x825862AC | ordinal: varies
 * 
 * Queries the protection flags for a memory address.
 * 
 * @param pAddress Address to query
 * @return Protection flags
 */
uint32_t MmQueryAddressProtect(void* pAddress);

//=============================================================================
// Debug Functions
//=============================================================================

/**
 * DbgBreakPoint @ 0x8258662C | ordinal: varies
 * 
 * Triggers a breakpoint for debugging.
 * On Xbox 360, this breaks into the debugger if attached.
 */
void DbgBreakPoint(void);

#ifdef __cplusplus
}
#endif
