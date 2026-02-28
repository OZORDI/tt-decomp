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

//=============================================================================
// String Functions
//=============================================================================

// ANSI_STRING structure (NT kernel counted string)
typedef struct _ANSI_STRING {
    uint16_t Length;        // Length of string in bytes (not including null terminator)
    uint16_t MaximumLength; // Maximum length of buffer in bytes
    char* Buffer;           // Pointer to string buffer
} ANSI_STRING, *PANSI_STRING;

// UNICODE_STRING structure (NT kernel counted wide string)
typedef struct _UNICODE_STRING {
    uint16_t Length;        // Length of string in bytes (not including null terminator)
    uint16_t MaximumLength; // Maximum length of buffer in bytes
    uint16_t* Buffer;       // Pointer to wide string buffer
} UNICODE_STRING, *PUNICODE_STRING;

/**
 * RtlInitAnsiString @ 0x8258664C | ordinal: varies
 * 
 * Initializes an ANSI_STRING structure.
 * Calculates the length of the source string and sets up the structure.
 * 
 * @param DestinationString Pointer to ANSI_STRING to initialize
 * @param SourceString      Pointer to null-terminated source string (can be NULL)
 */
void RtlInitAnsiString(ANSI_STRING* DestinationString, const char* SourceString);

//=============================================================================
// Error Code Conversion Functions
//=============================================================================

/**
 * RtlNtStatusToDosError @ 0x82585E5C | ordinal: varies
 * 
 * Converts an NTSTATUS code to a Win32/DOS error code.
 * 
 * @param Status NTSTATUS code to convert
 * @return Corresponding Win32 error code
 */
uint32_t RtlNtStatusToDosError(NTSTATUS Status);

//=============================================================================
// XEX Header Functions
//=============================================================================

// XEX header field IDs
#define XEX_HEADER_RESOURCE_INFO        0x000002FF
#define XEX_HEADER_FILE_FORMAT_INFO     0x000003FF
#define XEX_HEADER_BASE_REFERENCE       0x00000405
#define XEX_HEADER_DELTA_PATCH_DESCRIPTOR 0x000005FF
#define XEX_HEADER_BOUNDING_PATH        0x000080FF
#define XEX_HEADER_DEVICE_ID            0x00008105
#define XEX_HEADER_ORIGINAL_BASE_ADDRESS 0x00010001
#define XEX_HEADER_ENTRY_POINT          0x00010100
#define XEX_HEADER_IMAGE_BASE_ADDRESS   0x00010201
#define XEX_HEADER_IMPORT_LIBRARIES     0x000103FF
#define XEX_HEADER_CHECKSUM_TIMESTAMP   0x00018002
#define XEX_HEADER_ENABLED_FOR_CALLCAP  0x00018102
#define XEX_HEADER_ENABLED_FOR_FASTCAP  0x00018200
#define XEX_HEADER_ORIGINAL_PE_NAME     0x000183FF
#define XEX_HEADER_STATIC_LIBRARIES     0x000200FF
#define XEX_HEADER_TLS_INFO             0x00020104
#define XEX_HEADER_DEFAULT_STACK_SIZE   0x00020200
#define XEX_HEADER_DEFAULT_FILESYSTEM_CACHE_SIZE 0x00020301
#define XEX_HEADER_DEFAULT_HEAP_SIZE    0x00020401
#define XEX_HEADER_PAGE_HEAP_SIZE_AND_FLAGS 0x00028002
#define XEX_HEADER_SYSTEM_FLAGS         0x00030000
#define XEX_HEADER_EXECUTION_INFO       0x00040006
#define XEX_HEADER_TITLE_WORKSPACE_SIZE 0x00040201
#define XEX_HEADER_GAME_RATINGS         0x00040310
#define XEX_HEADER_LAN_KEY              0x00040404
#define XEX_HEADER_XBOX360_LOGO         0x000405FF
#define XEX_HEADER_MULTIDISC_MEDIA_IDS  0x000406FF
#define XEX_HEADER_ALTERNATE_TITLE_IDS  0x000407FF
#define XEX_HEADER_ADDITIONAL_TITLE_MEMORY 0x00040801

/**
 * RtlImageXexHeaderField @ 0x82585E3C | ordinal: varies
 * 
 * Retrieves a field from the XEX (Xbox Executable) header.
 * 
 * @param XexHeaderBase Pointer to XEX header
 * @param ImageField    Field ID to retrieve
 * @return Pointer to field data, or NULL if not found
 */
void* RtlImageXexHeaderField(void* XexHeaderBase, uint32_t ImageField);

//=============================================================================
// Physical Memory Functions
//=============================================================================

/**
 * MmGetPhysicalAddress @ 0x8258630C | ordinal: varies
 * 
 * Converts a virtual address to a physical address.
 * On Xbox 360, memory is directly mapped.
 * 
 * @param BaseAddress Virtual address to convert
 * @return Physical address (on Xbox 360, typically same as virtual)
 */
uint64_t MmGetPhysicalAddress(void* BaseAddress);

//=============================================================================
// Critical Section Functions (Extended)
//=============================================================================

// RTL_CRITICAL_SECTION structure (matches Windows CRITICAL_SECTION)
typedef struct _RTL_CRITICAL_SECTION {
    void* DebugInfo;
    int32_t LockCount;
    int32_t RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    uint32_t SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

/**
 * RtlEnterCriticalSection @ 0x82585E0C | ordinal: varies
 * 
 * Enters a critical section, blocking if necessary.
 * 
 * @param CriticalSection Pointer to critical section
 */
void RtlEnterCriticalSection(RTL_CRITICAL_SECTION* CriticalSection);

/**
 * RtlLeaveCriticalSection @ 0x82585DFC | ordinal: varies
 * 
 * Leaves a critical section.
 * 
 * @param CriticalSection Pointer to critical section
 */
void RtlLeaveCriticalSection(RTL_CRITICAL_SECTION* CriticalSection);

/**
 * RtlTryEnterCriticalSection @ 0x8258625C | ordinal: varies
 * 
 * Attempts to enter a critical section without blocking.
 * 
 * @param CriticalSection Pointer to critical section
 * @return TRUE (1) if acquired, FALSE (0) if already owned by another thread
 */
uint32_t RtlTryEnterCriticalSection(RTL_CRITICAL_SECTION* CriticalSection);

//=============================================================================
// Spinlock Functions
//=============================================================================

// Spinlock type (maps to pthread_spinlock_t or mutex)
typedef uint32_t KSPIN_LOCK;

/**
 * KeAcquireSpinLockAtRaisedIrql @ 0x82000760 | ordinal: varies
 * 
 * Acquires a spinlock at raised IRQL (already at DISPATCH_LEVEL).
 * Caller must have already raised IRQL using KeRaiseIrqlToDpcLevel.
 * 
 * @param SpinLock Pointer to spinlock
 */
void KeAcquireSpinLockAtRaisedIrql(KSPIN_LOCK* SpinLock);

/**
 * KeReleaseSpinLockFromRaisedIrql @ 0x82000754 | ordinal: varies
 * 
 * Releases a spinlock acquired at raised IRQL.
 * Does not lower IRQL - caller must do that separately.
 * 
 * @param SpinLock Pointer to spinlock
 */
void KeReleaseSpinLockFromRaisedIrql(KSPIN_LOCK* SpinLock);

/**
 * KeTryToAcquireSpinLockAtRaisedIrql @ 0x82000800 | ordinal: varies
 * 
 * Attempts to acquire a spinlock without blocking.
 * 
 * @param SpinLock Pointer to spinlock
 * @return TRUE (1) if acquired, FALSE (0) if already held
 */
uint32_t KeTryToAcquireSpinLockAtRaisedIrql(KSPIN_LOCK* SpinLock);

/**
 * KeRaiseIrqlToDpcLevel @ 0x820007ec | ordinal: varies
 * 
 * Raises IRQL to DISPATCH_LEVEL (DPC level).
 * Returns previous IRQL for later restoration.
 * 
 * @return Previous IRQL value
 */
uint32_t KeRaiseIrqlToDpcLevel(void);

//=============================================================================
// L2 Cache Lock Functions
//=============================================================================

/**
 * KeLockL2 @ 0x820007e0 | ordinal: varies
 * 
 * Locks the L2 cache for exclusive access.
 * Used for cache-coherent operations on Xbox 360.
 * 
 * Cross-platform: No-op (modern CPUs handle cache coherency automatically)
 */
void KeLockL2(void);

/**
 * KeUnlockL2 @ 0x820007e4 | ordinal: varies
 * 
 * Unlocks the L2 cache.
 * 
 * Cross-platform: No-op
 */
void KeUnlockL2(void);

//=============================================================================
// System Time Functions
//=============================================================================

/**
 * KeQuerySystemTime @ 0x82000850 | ordinal: varies
 * 
 * Queries the current system time.
 * Returns time as 100-nanosecond intervals since January 1, 1601 (Windows FILETIME).
 * 
 * @param CurrentTime Pointer to receive current time
 */
void KeQuerySystemTime(LARGE_INTEGER* CurrentTime);

//=============================================================================
// Thread Delay Functions
//=============================================================================

/**
 * KeDelayExecutionThread @ 0x82000874 | ordinal: varies
 * 
 * Delays execution of the current thread.
 * 
 * @param WaitMode    0 = KernelMode, 1 = UserMode
 * @param Alertable   TRUE if wait is alertable
 * @param Interval    Delay interval in 100-nanosecond units (negative = relative, positive = absolute)
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS KeDelayExecutionThread(uint32_t WaitMode, uint32_t Alertable, LARGE_INTEGER* Interval);

//=============================================================================
// Event Functions (Extended)
//=============================================================================

/**
 * NtCreateEvent @ 0x820008ec | ordinal: varies
 * 
 * Creates an event object.
 * 
 * @param EventHandle      Pointer to receive event handle
 * @param DesiredAccess    Access rights (usually 0x001F0003)
 * @param ObjectAttributes Object attributes (can be NULL)
 * @param EventType        0 = NotificationEvent (manual-reset), 1 = SynchronizationEvent (auto-reset)
 * @param InitialState     TRUE if initially signaled
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS NtCreateEvent(HANDLE* EventHandle, uint32_t DesiredAccess, void* ObjectAttributes,
                       uint32_t EventType, uint32_t InitialState);

/**
 * NtSetEvent @ 0x8200082c | ordinal: varies
 * 
 * Sets an event to the signaled state.
 * 
 * @param EventHandle   Event handle
 * @param PreviousState Pointer to receive previous state (can be NULL)
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS NtSetEvent(HANDLE EventHandle, int32_t* PreviousState);

/**
 * NtClearEvent @ 0x82000844 | ordinal: varies
 * 
 * Clears an event to the non-signaled state.
 * 
 * @param EventHandle Event handle
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS NtClearEvent(HANDLE EventHandle);

//=============================================================================
// Thread Management Functions
//=============================================================================

/**
 * ExCreateThread @ 0x820007dc | ordinal: varies
 * 
 * Creates a new thread.
 * 
 * @param ThreadHandle  Pointer to receive thread handle
 * @param StackSize     Stack size in bytes (0 = default)
 * @param ThreadId      Pointer to receive thread ID (can be NULL)
 * @param StartAddress  Thread start function
 * @param Parameter     Parameter to pass to thread function
 * @param CreationFlags Creation flags (0 = start immediately, 1 = create suspended)
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS ExCreateThread(HANDLE* ThreadHandle, uint32_t StackSize, uint32_t* ThreadId,
                        void* StartAddress, void* Parameter, uint32_t CreationFlags);

/**
 * ExTerminateThread @ 0x82000840 | ordinal: varies
 * 
 * Terminates a thread.
 * 
 * @param ThreadHandle Thread handle
 * @param ExitStatus   Exit status code
 * @return STATUS_SUCCESS or error code
 */
NTSTATUS ExTerminateThread(HANDLE ThreadHandle, NTSTATUS ExitStatus);

//=============================================================================
// Exception Handling Functions
//=============================================================================

// CONTEXT structure (simplified - full structure is platform-specific)
typedef struct _CONTEXT {
    // General purpose registers
    uint32_t Gpr[32];
    
    // Floating point registers
    double Fpr[32];
    
    // Special registers
    uint32_t Cr;   // Condition register
    uint32_t Lr;   // Link register
    uint32_t Ctr;  // Count register
    uint32_t Xer;  // Fixed-point exception register
    uint32_t Pc;   // Program counter
    uint32_t Msr;  // Machine state register
    
    // Additional state
    uint32_t ContextFlags;
} CONTEXT, *PCONTEXT;

/**
 * RtlCaptureContext @ 0x8258628C | ordinal: varies
 * 
 * Captures the current CPU context (register state).
 * Used for exception handling and stack traces.
 * 
 * @param ContextRecord Pointer to CONTEXT structure to fill
 */
void RtlCaptureContext(CONTEXT* ContextRecord);

/**
 * RtlUnwind @ 0x8258629C | ordinal: varies
 * 
 * Unwinds the stack for exception handling.
 * Calls exception handlers during the unwind process.
 * 
 * @param TargetFrame   Target frame to unwind to (NULL = unwind all)
 * @param TargetIp      Target instruction pointer
 * @param ExceptionRecord Exception record (can be NULL)
 * @param ReturnValue   Value to return from exception
 */
void RtlUnwind(void* TargetFrame, void* TargetIp, void* ExceptionRecord, void* ReturnValue);

#ifdef __cplusplus
}
#endif
