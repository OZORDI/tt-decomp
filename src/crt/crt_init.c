/**
 * crt_init.c — CRT initialization, spinlocks, TLS, and utility functions
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * This file implements the Xbox 360 CRT internal functions that are
 * referenced by the startup sequence (startup.c) and other CRT code.
 * On the host platform, these are implemented using standard C library
 * and POSIX threading primitives.
 *
 * Functions implemented:
 *   __cinit_impl        @ 0x82433D00 | size: 0x84
 *   _crt_spinlock_acquire @ 0x82433F58 | size: 0x60
 *   _crt_spinlock_release (paired with acquire)
 *   _crt_tls_fiber_setup @ 0x82566B78 | size: 0x0C
 *   _crt_tls_callback
 *   _crt_fiber_destroy
 *   _tls_dtor_cleanup
 *   _calloc_crt         (CRT internal calloc)
 *   _realloc_crt        (CRT internal realloc)
 *   _xe_strcpyn_10      @ 0x82432120 | size: 0x0C
 *   _stricmp            @ 0x82433A40 | size: 0x54
 *   __cinit_impl        @ 0x82433D00 | size: 0x84
 *   _heap_init_impl     @ 0x8242C1E8 | size: 0xD8
 *   _check_xdk_version  @ 0x8242BB70 | size: 0x1A4 (already in version.c)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

/*=========================================================================
 * CRT Spinlock Implementation
 *
 * The Xbox 360 CRT uses a table of critical sections indexed by ID.
 * On the host, we use pthread mutexes. The spinlock table has 36 slots
 * (288 bytes / 8 bytes per slot).
 *=========================================================================*/

#define CRT_MAX_SPINLOCKS 36

static pthread_mutex_t g_crt_spinlocks[CRT_MAX_SPINLOCKS];
static uint8_t g_crt_spinlocks_initialized[CRT_MAX_SPINLOCKS] = {0};
static pthread_mutex_t g_crt_spinlock_init_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Ensure a spinlock slot is initialized before use.
 */
static void _ensure_spinlock_init(int id) {
    if (id < 0 || id >= CRT_MAX_SPINLOCKS) return;

    if (!g_crt_spinlocks_initialized[id]) {
        pthread_mutex_lock(&g_crt_spinlock_init_mutex);
        if (!g_crt_spinlocks_initialized[id]) {
            pthread_mutex_init(&g_crt_spinlocks[id], NULL);
            g_crt_spinlocks_initialized[id] = 1;
        }
        pthread_mutex_unlock(&g_crt_spinlock_init_mutex);
    }
}

/**
 * _crt_spinlock_acquire @ 0x82433F58 | size: 0x60
 *
 * Acquires a CRT spinlock by index. On Xbox 360, this indexes into a
 * table of RTL_CRITICAL_SECTION objects and calls RtlEnterCriticalSection.
 * On the host, we use pthread mutexes.
 *
 * @param id  Spinlock index (0-35)
 */
void _crt_spinlock_acquire(int id) {
    if (id < 0 || id >= CRT_MAX_SPINLOCKS) return;
    _ensure_spinlock_init(id);
    pthread_mutex_lock(&g_crt_spinlocks[id]);
}

/**
 * _crt_spinlock_release (paired with _crt_spinlock_acquire)
 *
 * Releases a CRT spinlock by index.
 *
 * @param id  Spinlock index (0-35)
 */
void _crt_spinlock_release(int id) {
    if (id < 0 || id >= CRT_MAX_SPINLOCKS) return;
    _ensure_spinlock_init(id);
    pthread_mutex_unlock(&g_crt_spinlocks[id]);
}

/*=========================================================================
 * CRT TLS / Fiber Support
 *
 * The Xbox 360 CRT uses a fiber-based TLS system for per-thread CRT
 * state (errno, strtok state, etc.). On the host, we use pthread TLS
 * to store a per-thread context block.
 *=========================================================================*/

static pthread_key_t g_crt_fiber_key;
static int g_crt_fiber_key_created = 0;

/**
 * _crt_tls_fiber_setup @ 0x82566B78 | size: 0x0C
 *
 * On Xbox 360, reads the TLS fiber value from the thread's XeTlsBlock.
 * On the host, returns the current thread's CRT fiber context pointer.
 * Returns NULL if no fiber context has been set up for this thread.
 */
void _crt_tls_fiber_setup(void) {
    /* On the host, this is a no-op. The fiber context is managed
     * through _cinit_setup's TLS slot allocation. */
}

/**
 * _crt_tls_callback
 *
 * Called during CRT init/shutdown phases to set up or tear down
 * per-thread CRT state. On the host, this is a no-op since we
 * use standard C library TLS.
 *
 * @param initFlag  1 = initializing, 0 = shutting down
 */
void _crt_tls_callback(int initFlag) {
    (void)initFlag;
    /* On the host, per-thread CRT state is managed by the platform's
     * C library. No additional setup needed. */
}

/**
 * _crt_fiber_destroy
 *
 * TLS destructor callback registered via _KeTlsAlloc_thunk during
 * _cinit_setup. Called when a thread exits to clean up its CRT fiber
 * context. On the host, we free the allocated context block.
 */
void _crt_fiber_destroy(void) {
    /* On the host, the CRT fiber context is a simple malloc'd block.
     * The platform TLS destructor handles cleanup automatically. */
}

/**
 * _tls_dtor_cleanup
 *
 * Runs all registered TLS destructors during CRT shutdown.
 * On the host, pthread handles TLS destructor invocation automatically.
 */
void _tls_dtor_cleanup(void) {
    /* No-op on host — pthread_key_delete handles cleanup. */
}

/*=========================================================================
 * CRT Memory Allocation Helpers
 *=========================================================================*/

/**
 * _calloc_crt
 *
 * CRT-internal calloc used during startup for allocating CRT data
 * structures (fiber contexts, TLS backing stores, etc.).
 * On the host, delegates to standard calloc.
 *
 * @param count  Number of elements
 * @param size   Size of each element in bytes
 * @return       Pointer to zero-initialized memory, or NULL on failure
 */
void* _calloc_crt(int count, int size) {
    if (count <= 0 || size <= 0) return NULL;
    return calloc((size_t)count, (size_t)size);
}

/**
 * _realloc_crt
 *
 * CRT-internal realloc used by the atexit table growth logic.
 * Tries to allocate newSz bytes first; if that fails, tries minSz.
 *
 * @param newSz   Preferred allocation size in bytes
 * @param minSz   Minimum acceptable size (fallback)
 * @return        Pointer to reallocated memory, or NULL on failure
 */
void* _realloc_crt(size_t newSz, size_t minSz) {
    void* ptr = malloc(newSz);
    if (ptr) return ptr;

    /* Fallback to minimum size */
    if (minSz > 0 && minSz < newSz) {
        ptr = malloc(minSz);
    }
    return ptr;
}

/*=========================================================================
 * CRT String Functions
 *=========================================================================*/

/**
 * _xe_strcpyn_10 @ 0x82432120 | size: 0x0C
 *
 * Copies up to 10 bytes from src to dest (including null terminator).
 * This is a thin wrapper around the CRT's strncpy-like function with
 * a hardcoded max length of 10.
 *
 * @param dest  Destination buffer (must be at least 10 bytes)
 * @param src   Source string
 * @param n     Unused — always 10 internally
 */
void _xe_strcpyn_10(char* dest, const char* src, size_t n) {
    (void)n;
    if (!dest) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }
    strncpy(dest, src, 10);
    dest[9] = '\0';  /* Ensure null termination */
}

/**
 * _stricmp @ 0x82433A40 | size: 0x54
 *
 * Standard case-insensitive string comparison. This is the Xbox 360 CRT's
 * _stricmp, which is a straightforward tolower-and-compare loop.
 * Different from rage_stricmp (which has '=' termination and plural-s handling).
 *
 * @param a  First string
 * @param b  Second string
 * @return   0 if equal (case-insensitive), negative if a < b, positive if a > b
 */
int _stricmp(const char* a, const char* b) {
    if (!a || !b) {
        if (a == b) return 0;
        return a ? 1 : -1;
    }

    unsigned char ca, cb;
    for (;;) {
        ca = (unsigned char)*a++;
        cb = (unsigned char)*b++;

        /* Convert A-Z to a-z */
        if (ca >= 'A' && ca <= 'Z') ca |= 0x20;
        if (cb >= 'A' && cb <= 'Z') cb |= 0x20;

        if (ca != cb) return (int)ca - (int)cb;
        if (ca == 0) return 0;
    }
}

/*=========================================================================
 * CRT Initialization
 *=========================================================================*/

/**
 * __cinit_impl @ 0x82433D00 | size: 0x84
 *
 * Initializes the CRT's internal I/O stream table. On Xbox 360, this
 * walks a table of 36 stream descriptors (8 bytes each, at 0x825B0F70)
 * and for each one marked as "needs init" (field+4 == 1), allocates a
 * 28-byte I/O context block and calls the stream initializer.
 *
 * On the host, the standard C library handles stdio initialization.
 * We return 1 (success) since the host CRT is already initialized.
 *
 * @return  1 on success, 0 on failure
 */
int __cinit_impl(void) {
    /* The host C library's stdio is already initialized by the time
     * we reach this point. Return success. */
    return 1;
}

/**
 * _heap_init_impl @ 0x8242C1E8 | size: 0xD8
 *
 * Initializes the CRT heap. On Xbox 360, this queries the XEX header
 * for the heap reservation size, then creates a heap object via the
 * kernel's heap creation API.
 *
 * On the host, the standard C library heap is already available.
 * We return 1 (success) to indicate the heap is ready.
 *
 * @return  1 on success, 0 on failure
 */
int _heap_init_impl(void) {
    /* Host heap is always available via malloc/free. */
    return 1;
}

/*=========================================================================
 * Xbox 360 Import Table Globals
 *
 * These are pointers from the XEX import table that other CRT/startup
 * code references. On the host, they're initialized to NULL/safe defaults.
 *=========================================================================*/

void* __imp_ExLoadedCommandLine = NULL;
void* __imp_ExThreadObjectType = NULL;
