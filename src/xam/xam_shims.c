/**
 * xam_shims.c — Xbox 360 XAM and network API shims for host platform
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * These functions are Xbox 360 XAM (Xbox Application Manager) imports
 * that have no implementation in the game binary. They are implemented
 * here as cross-platform shims.
 *
 * Functions implemented:
 *   NetDll_WSACleanup       — network cleanup (no-op)
 *   NetDll_WSAStartup       — network init (no-op)
 *   NetDll_XNetStartup      — XNet init (no-op)
 *   XGetVideoMode           — video mode query (returns 720p)
 *   XamInputGetState        — controller input (→ xam/input.c)
 *   XamInputSetState        — controller vibration (→ xam/input.c)
 *   XamShowGamerCardUIForXUID — Xbox Live UI (no-op)
 *   XamLoaderTerminateTitle — terminate process (→ xam/xam_stubs.c)
 *   xam_CreateEvent         — create kernel event object
 *   xam_CreateEvent_c       — C-linkage wrapper
 *   xam_singleton_init_8D60 — singleton initializer (→ xam/static_init.c)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*=========================================================================
 * Network DLL Functions
 *
 * The Xbox 360 network stack (NetDll_*) wraps Winsock-like APIs.
 * For the decomp, networking is not needed for menu boot, so these
 * are no-ops that return success.
 *=========================================================================*/

/**
 * NetDll_WSACleanup
 *
 * Cleans up the Winsock subsystem. No-op on host.
 * @return  0 (success)
 */
int NetDll_WSACleanup(void) {
    return 0;
}

/**
 * NetDll_WSAStartup
 *
 * Initializes the Winsock subsystem with the requested version.
 * No-op on host — BSD sockets are always available.
 *
 * @param version  Requested Winsock version (e.g., 0x0202 for 2.2)
 * @param data     Pointer to WSADATA output structure
 * @return  0 (success)
 */
int NetDll_WSAStartup(uint16_t version, void* data) {
    (void)version;
    if (data) memset(data, 0, 400);  /* Zero out WSADATA */
    return 0;
}

/**
 * NetDll_XNetStartup
 *
 * Initializes the Xbox Network (XNet) subsystem.
 * No-op on host.
 *
 * @param params  XNet startup parameters (ignored)
 * @return  0 (success)
 */
int NetDll_XNetStartup(void* params) {
    (void)params;
    return 0;
}

/*=========================================================================
 * Video Mode
 *=========================================================================*/

/**
 * XGetVideoMode
 *
 * Queries the current video display mode. On Xbox 360, this returns
 * the active display resolution and flags.
 * On the host, we return a standard 720p mode.
 *
 * The VIDEO_MODE structure layout (from Xbox 360 SDK):
 *   +0x00  uint32_t  dwDisplayWidth
 *   +0x04  uint32_t  dwDisplayHeight
 *   +0x08  uint32_t  fIsInterlaced
 *   +0x0C  uint32_t  fIsWideScreen
 *   +0x10  uint32_t  fIsHiDef
 *   +0x14  float     RefreshRate
 *   +0x18  uint32_t  VideoStandard
 *
 * @param mode  Pointer to VIDEO_MODE structure to fill
 */
void XGetVideoMode(void* mode) {
    if (!mode) return;

    uint32_t* m = (uint32_t*)mode;
    m[0] = 1280;   /* dwDisplayWidth */
    m[1] = 720;    /* dwDisplayHeight */
    m[2] = 0;      /* fIsInterlaced = false */
    m[3] = 1;      /* fIsWideScreen = true */
    m[4] = 1;      /* fIsHiDef = true */

    /* RefreshRate as float */
    float refreshRate = 60.0f;
    memcpy(&m[5], &refreshRate, sizeof(float));

    m[6] = 1;      /* VideoStandard = NTSC */
}

/*=========================================================================
 * XAM Input (forwarded to xam/input.c)
 *
 * XamInputGetState and XamInputSetState are already implemented in
 * xam/input.c with full SDL2 gamepad support. The stubs in stubs.cpp
 * can be removed — the linker will find the real implementations.
 *=========================================================================*/

/* Note: XamInputGetState, XamInputSetState, XamShowGamerCardUIForXUID
 * are already implemented in xam/input.c. No need to re-implement here. */

/*=========================================================================
 * XAM Event Creation
 *=========================================================================*/

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int32_t         signaled;
    int32_t         manual_reset;
} HostKernelEvent;

/**
 * xam_CreateEvent
 *
 * Creates a kernel event object. On Xbox 360, this calls
 * NtCreateEvent through the XAM layer.
 * On the host, we allocate a pthread-based event.
 *
 * @param a  Object attributes (ignored)
 * @param b  Manual reset flag (1 = manual, 0 = auto)
 * @param c  Initial state (1 = signaled, 0 = nonsignaled)
 * @param d  Reserved
 * @return   Handle to the event object (cast to uint32_t)
 */
uint32_t xam_CreateEvent(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    (void)a;
    (void)d;

    HostKernelEvent* event = (HostKernelEvent*)calloc(1, sizeof(HostKernelEvent));
    if (!event) return 0;

    pthread_mutex_init(&event->mutex, NULL);
    pthread_cond_init(&event->cond, NULL);
    event->manual_reset = (b != 0) ? 1 : 0;
    event->signaled = (c != 0) ? 1 : 0;

    return (uint32_t)(uintptr_t)event;
}

/**
 * xam_CreateEvent_c
 *
 * C-linkage wrapper for xam_CreateEvent, called from generated code
 * that doesn't pass parameters (uses defaults).
 */
void xam_CreateEvent_c(void) {
    /* Create a default auto-reset, non-signaled event */
    xam_CreateEvent(0, 0, 0, 0);
}

/*=========================================================================
 * XAM Singleton Init (forwarded to xam/static_init.c)
 *
 * xam_singleton_init_8D60 is already implemented in xam/static_init.c
 * as xam_GetInitSingleton. The stubs.cpp version can be removed.
 *=========================================================================*/

/* Note: xam_singleton_init_8D60 is implemented in xam/static_init.c */

/*=========================================================================
 * xam_singleton_init_8D60 — alias for xam_GetInitSingleton
 *
 * The game code references this function by its address-based name.
 * The real implementation is xam_GetInitSingleton in static_init.c.
 *=========================================================================*/

typedef struct xam_init_singleton xam_init_singleton;
extern xam_init_singleton* xam_GetInitSingleton(void);

void* xam_singleton_init_8D60(void* a) {
    (void)a;
    return (void*)xam_GetInitSingleton();
}
