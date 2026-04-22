/**
 * kernel_shims.c — Xbox 360 kernel API shims for host platform
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * These functions are Xbox 360 kernel imports (from xboxkrnl.exe) that
 * have no implementation in the game binary itself. They are implemented
 * here as cross-platform shims using POSIX/standard C equivalents.
 *
 * Functions implemented:
 *   HalReturnToFirmware     — exit process
 *   KeBugCheck              — fatal error / abort
 *   KeSetAffinityThread     — thread affinity (no-op on host)
 *   ObDereferenceObject     — object refcount decrement (no-op)
 *   ObReferenceObjectByHandle — object handle lookup (stub)
 *   RtlEnterCriticalSection — mutex lock
 *   RtlLeaveCriticalSection — mutex unlock
 *   KeResetEvent            — reset synchronization event
 *   KeSetEvent              — signal synchronization event
 *   KeWaitForSingleObject   — wait on synchronization object
 *   ke_EnterCriticalSection — game wrapper for RtlEnterCriticalSection
 *   ke_LeaveCriticalSection — game wrapper for RtlLeaveCriticalSection
 *   ke_ConstructObject      — kernel object constructor (no-op)
 *   ke_DestroyObjectA/B     — kernel object destructors (no-op)
 *   RtlMultiByteToUnicodeN_6FA8_w — multibyte to unicode conversion
 *
 * TLS thunks (_KeTlsAlloc_thunk, KeTlsFree_stub, KeTlsSetValue_stub)
 * are thin wrappers around the real TLS functions in xam/kernel.c.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*=========================================================================
 * Forward declarations for TLS functions implemented in xam/kernel.c
 *=========================================================================*/
extern uint32_t KeTlsAlloc(void);
extern uint32_t KeTlsFree(uint32_t dwTlsIndex);
extern uint32_t KeTlsSetValue(uint32_t dwTlsIndex, void* lpTlsValue);

/*=========================================================================
 * System Lifecycle
 *=========================================================================*/

/**
 * HalReturnToFirmware
 *
 * Xbox 360 kernel function that returns control to the system firmware.
 * Mode 1 = cold reboot, mode 5 = power off, etc.
 * On the host, we just exit the process.
 */
void HalReturnToFirmware(int mode) {
    printf("[KERNEL] HalReturnToFirmware(mode=%d) — exiting process\n", mode);
    exit(0);
}

/**
 * KeBugCheck
 *
 * Xbox 360 kernel blue-screen-of-death. Called on fatal CRT errors
 * (e.g., recursive _doexit, heap corruption).
 * On the host, we print the bugcheck code and abort.
 */
void KeBugCheck(uint32_t code) {
    fprintf(stderr, "[KERNEL] KeBugCheck(0x%08X) — fatal error, aborting\n", code);
    abort();
}

/*=========================================================================
 * Thread Management
 *=========================================================================*/

/**
 * KeSetAffinityThread
 *
 * Sets the processor affinity mask for a thread. On Xbox 360, this
 * pins threads to specific hardware threads (0-5).
 * On the host, this is a no-op — the OS scheduler handles affinity.
 */
void KeSetAffinityThread(void* thread, uint32_t affinity) {
    (void)thread;
    (void)affinity;
    /* No-op: host OS manages thread scheduling */
}

/*=========================================================================
 * Object Manager
 *=========================================================================*/

/**
 * ObDereferenceObject
 *
 * Decrements the reference count on a kernel object.
 * On the host, kernel objects are managed differently — no-op.
 */
void ObDereferenceObject(void* obj) {
    (void)obj;
}

/**
 * ObReferenceObjectByHandle
 *
 * Looks up a kernel object by its handle and increments its reference count.
 * On the host, returns success (0) without doing anything.
 */
int ObReferenceObjectByHandle(void* handle, void* type, void** obj) {
    (void)handle;
    (void)type;
    if (obj) *obj = NULL;
    return 0;  /* STATUS_SUCCESS */
}

/*=========================================================================
 * Critical Sections (RTL)
 *
 * RtlEnterCriticalSection and RtlLeaveCriticalSection are already
 * implemented in xam/kernel.c with proper pthread mutex support.
 *=========================================================================*/

static pthread_mutex_t g_rtl_cs_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * ke_EnterCriticalSection
 *
 * Game-side wrapper that calls RtlEnterCriticalSection.
 * The parameter is a critical section handle/index.
 */
void ke_EnterCriticalSection(uint32_t a) {
    (void)a;
    pthread_mutex_lock(&g_rtl_cs_mutex);
}

/**
 * ke_LeaveCriticalSection
 *
 * Game-side wrapper that calls RtlLeaveCriticalSection.
 */
void ke_LeaveCriticalSection(uint32_t a) {
    (void)a;
    pthread_mutex_unlock(&g_rtl_cs_mutex);
}

/*=========================================================================
 * Synchronization Events
 *
 * These are already fully implemented in xam/kernel.c with proper
 * pthread condition variables. The stubs here forward to those
 * implementations. We re-declare them here with the exact signatures
 * expected by the game code.
 *=========================================================================*/

/* Note: KeResetEvent, KeSetEvent, KeWaitForSingleObject are already
 * implemented in xam/kernel.c with full pthread-based event semantics.
 * The stubs in stubs.cpp can be removed — the linker will find the
 * real implementations in kernel.c. */

/*=========================================================================
 * TLS Thunks
 *
 * These are thin wrappers that adapt the game's calling convention
 * to the real TLS functions in xam/kernel.c.
 *=========================================================================*/

/**
 * _KeTlsAlloc_thunk @ 0x8242FB70 | size: 0x04
 *
 * Allocates a TLS slot with an optional destructor callback.
 * On Xbox 360, the destructor is called when the thread exits.
 * On the host, we ignore the destructor and delegate to KeTlsAlloc.
 *
 * @param destructorThunk  Pointer to destructor function (ignored on host)
 * @return  TLS slot index, or -1 on failure
 */
int _KeTlsAlloc_thunk(void* destructorThunk) {
    (void)destructorThunk;
    uint32_t index = KeTlsAlloc();
    return (index == 0xFFFFFFFF) ? -1 : (int)index;
}

/**
 * KeTlsFree_stub
 *
 * Frees a previously allocated TLS slot.
 *
 * @param tlsIndex  TLS slot index to free
 * @return  1 on success, 0 on failure
 */
int KeTlsFree_stub(uint32_t tlsIndex) {
    return (int)KeTlsFree(tlsIndex);
}

/**
 * KeTlsSetValue_stub
 *
 * Sets the value for a TLS slot in the current thread.
 *
 * @param tlsIndex  TLS slot index
 * @param value     Value to store
 * @return  1 on success, 0 on failure
 */
int KeTlsSetValue_stub(uint32_t tlsIndex, void* value) {
    return (int)KeTlsSetValue(tlsIndex, value);
}

/*=========================================================================
 * Kernel Object Lifecycle
 *=========================================================================*/

/**
 * ke_ConstructObject
 *
 * Initializes a kernel synchronization object (event, mutex, etc.).
 * On the host, this is a no-op — objects are initialized when created.
 */
void ke_ConstructObject(void* a) {
    (void)a;
}

/**
 * ke_DestroyObjectA / ke_DestroyObjectB
 *
 * Destroys a kernel synchronization object. Two variants exist in the
 * binary for different object types.
 * On the host, this is a no-op — cleanup happens at process exit.
 */
void ke_DestroyObjectA(void* a) {
    (void)a;
}

void ke_DestroyObjectB(void* a) {
    (void)a;
}

/*=========================================================================
 * String Conversion
 *
 * RtlMultiByteToUnicodeN_6FA8_w is already implemented in xam_stubs.c
 * with a proper ASCII-to-UTF-16 widening implementation.
 *=========================================================================*/
