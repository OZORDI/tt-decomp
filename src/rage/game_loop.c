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

#include "rage_system.hpp"
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
