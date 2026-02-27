/**
 * rage/game_loop.c — Subsystem and audio initialisation for the game loop
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers:
 *   rage_subsystem_init_2430  — Allocates Xbox thread semaphores and registers
 *                               the main game thread with the RAGE scheduler.
 *   audSystem_init_1ED0       — Walks a null-terminated list of audio-device
 *                               path strings, builds full file-system paths, and
 *                               opens each device via fiDevice_GetDevice.
 */

#include "rage/rage_system.hpp"
#include "audio_system.hpp"
#include <stdint.h>
#include <stdio.h>   /* _snprintf */

// ---------------------------------------------------------------------------
// External declarations
// ---------------------------------------------------------------------------

// Xbox thread / synchronisation primitives (XAM layer)
// xam_6C88_g — Creates a synchronisation object (semaphore/event).
//   r3=type?, r4=initialCount, r5=maxCount, r6=flags → returns handle
extern uint32_t xam_6C88_g(uint32_t type, uint32_t initCount,
                            uint32_t maxCount, uint32_t flags); // @ 0x82566C88

// RAGE main-thread registration.
// Registers a named thread with the given stack size and priority flags.
extern uint32_t rage_thread_register_7FD0(void* nameStr, uint32_t stackSize,
                                          uint32_t flags, uint32_t priority,
                                          void* param, uint32_t paramSize,
                                          uint32_t affinity, uint8_t suspended); // @ 0x82187FD0

// "Assert / log" function called when the subsystem init guard fails
// (lock count is zero — double-init or premature call).
extern void nop_8240E6D0(const char* msg);  // @ 0x8240E6D0

// File-device layer
extern struct fiDevice* fiDevice_GetDevice(const char* path,
                                           uint32_t openForWrite); // @ 0x822E12A0

// Globals
// g_lockCount — nesting counter protecting subsystem re-init @ (lis(-32160)+7760)
extern uint32_t g_lockCount;  // @ 0x82066530 (approx)

// g_semRender, g_semUpdate, g_semIO — three XAM semaphore handles
extern uint32_t g_semRender;   // @ 0x820665F8
extern uint32_t g_semUpdate;   // @ 0x820665FC
extern uint32_t g_semIO;       // @ 0x82066600

// g_mainThreadHandle — handle returned by rage_thread_register
extern uint32_t g_mainThreadHandle;  // @ 0x825D0078

// Audio device path list — null-terminated array of (char*) pointers
// @ 0x825D1D40  (addi r25, lis(-32163), 7488)
extern const char* g_audDevicePaths[];  // @ 0x825D1D40

// Base audio path prefix string  @ 0x8204DAA4
extern const char g_audBasePath[];   // @ 0x8204DAA4

// Audio path format strings
extern const char g_audPathFmt[];    // @ 0x82027434  — e.g. "%s%s"
extern const char g_audPathFmt2[];   // @ 0x8204DBFC  — alternate format

// Thread name string for the RAGE main thread @ (lis(-32210)+9624)
extern const char g_mainThreadName[];  // @ 0x822DA988 (approx)

// Subsystem guard address  @ (lis(-32160)+26116)
extern uint32_t g_subsystemInitGuard;  // @ 0x82066804 (approx)


// ===========================================================================
// rage_subsystem_init  @ 0x822E2430 | size: 0xDC
//
// Initialises the core Xbox subsystems needed by the RAGE game loop:
//
//  1. Guard check — if g_subsystemInitGuard is zero, logs an error and
//     returns early (prevents spurious re-init from unrelated threads).
//
//  2. Allocates three XAM semaphores (render, update, IO) with initial count
//     0 and max count 1, storing their handles in the corresponding globals.
//
//  3. Registers the RAGE main thread with the Xbox scheduler via
//     rage_thread_register, specifying a 64 KB stack, a specific affinity
//     mask (0x18000), and priority 8.  Saves the returned thread handle to
//     g_mainThreadHandle.
// ===========================================================================
void rage_subsystem_init(void) {
    // Guard: abort if subsystem has not been marked as ready.
    if (g_subsystemInitGuard == 0) {
        nop_8240E6D0("subsystem init called before guard set");
        return;
    }

    // Allocate render semaphore (type=0, init=1, max=1, flags=0).
    g_semRender = xam_6C88_g(0, 1, 1, 0);

    // Allocate update semaphore (type=0, init=1, max=1, flags=0).
    g_semUpdate = xam_6C88_g(0, 1, 1, 0);

    // Allocate IO semaphore (type=0, init=1, max=1, flags=1 — signalled?).
    g_semIO = xam_6C88_g(0, 1, 1, 1);

    // Register main RAGE thread.
    //  stack    = 0x18000 (98304 bytes)
    //  priority = 8
    //  affinity = g_subsystemInitGuard (hardware thread bitmap)
    //  suspended = 1 (starts suspended)
    g_mainThreadHandle = rage_thread_register_7FD0(
        (void*)g_mainThreadName,
        (uint32_t)g_audDevicePaths,  // TODO: verify — r4 = addi(r11) at call site
        98304,                        // 0x18000
        8,
        (void*)g_subsystemInitGuard,
        0,
        1,
        0
    );
    // TODO: the exact argument mapping for rage_thread_register_7FD0 needs
    //       cross-checking once that function is lifted (@ 0x82187FD0).
}


// ===========================================================================
// audSystem_init  @ 0x82221ED0 | size: 0x18C
//
// Scans the global audio-device path list (g_audDevicePaths) and opens each
// device through the RAGE file-system layer.
//
// Algorithm:
//   For each non-null entry in g_audDevicePaths:
//     1. Measure the path string's length.
//     2. If non-empty, build two formatted paths with _snprintf:
//          fullPath[256]:   g_audPathFmt  % g_audBasePath + entryStr
//          strippedPath[256]: g_audPathFmt2 % g_audBasePath + entryStr
//     3. Walk strippedPath from the end to find the last non-backslash
//        segment, copying it into the end of fullPath (replacing backslash-
//        delimited components one character at a time).
//     4. Null-terminate the assembled path and open it via
//        fiDevice_GetDevice(fullPath, 1 /*open*/).
//     5. Append a backslash separator (ASCII 92) and the stripped component.
//
// TODO: the inner path-stripping loop (0x82221FB8–0x82222040) is complex.
//       The current implementation is a faithful structural translation but
//       the exact semantics of the "replace last component" pattern need
//       verification against additional callers.
// ===========================================================================
void audSystem_init(void) {
    struct fiDevice* device = fiDevice_GetDevice(g_audBasePath, 1);

    // Walk null-terminated list of device sub-paths.
    for (int i = 0; g_audDevicePaths[i] != NULL; ++i) {
        const char* entry = g_audDevicePaths[i];

        // Measure length.
        int len = 0;
        while (entry[len]) len++;

        if (len == 0) {
            continue;
        }

        // Build full path and stripped path into local buffers.
        char fullPath[256];
        char strippedPath[256];
        _snprintf(fullPath,    sizeof(fullPath),    g_audPathFmt,  g_audBasePath, entry);
        _snprintf(strippedPath, sizeof(strippedPath), g_audPathFmt2, g_audBasePath, entry);

        // Measure stripped path.
        int slen = 0;
        while (strippedPath[slen]) slen++;

        // Walk from the end of fullPath, replacing with characters from
        // strippedPath until we hit a backslash or the start.
        int pos = slen;
        while (pos < (int)sizeof(fullPath)) {
            char c = fullPath[pos];
            if (c == '\\') {
                // Stop at backslash boundary.
                break;
            }
            if (c == '\0') {
                break;
            }
            // Copy character into strippedPath at corresponding offset.
            strippedPath[pos - slen] = c;
            pos++;
        }

        // Null-terminate and open.
        strippedPath[pos - slen] = '\0';

        // Re-open device with assembled path.
        device = fiDevice_GetDevice(strippedPath, 1);

        // Append separator (ASCII 92 = '\\') to the stripped path slot.
        strippedPath[pos - slen] = (char)92;  // '\\'

        // Step through remaining characters.
        // TODO: the scaffold continues iterating at loc_82221FB8 after the
        //       open call — this outer loop may need an additional pass.
        // @ 0x82222040 — verify loop-back condition.
    }
}


// ===========================================================================
// grcDevice_init  @ 0x820F8A00 | size: 0xFC
//
// Allocates and initializes the grcDevice command-pool free list, then
// starts the render thread if it hasn't been registered yet.
//
// Memory layout (globals at 0x825EB26C):
//   g_grcCmdPool.capacity    @ 0x825EB26C — 256 (pool size)
//   g_grcCmdPool.freeHead    @ 0x825EB270 — head index of free list (0)
//   g_grcCmdPool.pMemory     @ 0x825EB274 — pointer to the 40960-byte pool
//
// Pool structure:
//   256 entries, each 160 bytes apart, forming a singly-linked free list.
//   Each node layout:
//     [+0]  uint32  index           — node's own index (0–255)
//     [+4]  uint32  state           — 0xFFFFFFFF (unused/free sentinel)
//     [+16] uint32  nextFreeIndex   — next node in free list (256 = end)
//
// Render thread registration (guarded by g_grcInitGate):
//   Thread function : pg_8250_g @ 0x820F8250
//   Stack size      : 65536 bytes (0x10000)
//   Priority        : 6
//   Parameter       : pool memory pointer
// ===========================================================================

// Pool descriptor (layout straddles 0x825EB26C–0x825EB274)
typedef struct grcCmdPool {
    uint32_t capacity;   /* +0  @ 0x825EB26C — always 256 */
    uint32_t freeHead;   /* +4  @ 0x825EB270 — index of first free node */
    void*    pMemory;    /* +8  @ 0x825EB274 — backing allocation */
} grcCmdPool;

extern grcCmdPool g_grcCmdPool;  /* @ 0x825EB26C */

// Init gate: non-null m_pName means render thread already registered.
typedef struct {
    uint32_t m_vftable;
    uint8_t* m_pName;
} grcInitGate;
extern grcInitGate g_grcInitGate;  /* @ 0x825CA074 */

// Render thread entry function (pg_8250_g @ 0x820F8250)
extern void pg_8250_g(void* pCmdPool);

void grcDevice_init(void) {
    g_grcCmdPool.capacity = 256;

    // Ensure main thread context is ready before allocating.
    xe_main_thread_init_0038();

    // Allocate the command pool: 256 nodes * 160 bytes = 40960 (0xA000) bytes,
    // 16-byte aligned, via the RAGE memory allocator (VCALL slot 1).
    extern uint32_t* g_sda_base;  /* SDA r13 base */
    uint32_t*  allocCtx  = (uint32_t*)(uintptr_t)g_sda_base[0];
    void*      allocObj  = (void*)(uintptr_t)allocCtx[1];  /* [allocCtx+4] */
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn    allocFn   = ((AllocFn*)*(void**)allocObj)[1]; /* vtable slot 1 */
    void*      pool      = allocFn(allocObj, 0xA000u, 16u);
    g_grcCmdPool.pMemory = pool;

    // Initialise free list: 256 nodes, each spaced 160 bytes apart.
    uint8_t* base = (uint8_t*)pool;
    for (int i = 0; i < 256; ++i) {
        uint8_t* node = base + i * 160;
        *(uint32_t*)(node +  0) = (uint32_t)i;           /* own index     */
        *(uint32_t*)(node +  4) = 0xFFFFFFFFu;           /* free sentinel */
        *(uint32_t*)(node + 16) = (uint32_t)(i + 1);     /* next (256 = end) */
    }

    g_grcCmdPool.freeHead = 0;

    // Only register the render thread if the init gate is not yet set.
    if (g_grcInitGate.m_pName == NULL) {
        rage_thread_register_7FD0(
            (void*)pg_8250_g,  /* thread entry function      */
            pool,              /* parameter: command pool    */
            65536u,            /* stack size (0x10000)       */
            6u,                /* priority / affinity flags  */
            (void*)"%s",       /* thread name format string  */
            1u,                /* flags                      */
            0u                 /* reserved                   */
        );
    }
}
