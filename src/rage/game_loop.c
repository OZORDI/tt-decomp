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

// RAGE kernel thread registration.
// r3=func, r4=param, r5=stack_size, r6=base_priority_delta,
// r7=name_fmt_str, r8=start_suspended, r9=processor_affinity
// Returns the kernel thread handle (or 0/-1 on failure).
extern uint32_t rage_thread_register_7FD0(void* func, void* param,
                                          uint32_t stackSize,
                                          uint32_t basePriorityDelta,
                                          void* nameFmt,
                                          uint32_t startSuspended,
                                          uint32_t processorAffinity); // @ 0x82187FD0


// "Assert / log" function called when the subsystem init guard fails
// (lock count is zero — double-init or premature call).
extern void nop_8240E6D0(const char* msg);  // @ 0x8240E6D0

// File-device layer
extern struct fiDevice* fiDevice_GetDevice(const char* path,
                                           uint32_t openForWrite); // @ 0x822E12A0

// Globals

// g_semRender, g_semUpdate, g_semIO — three XAM kernel semaphore handles
// Stored at r31+26104/26108/26112 where r31 = lis(-32160) = 0x82600000
extern uint32_t g_semRender;   /* @ 0x826065F8 */
extern uint32_t g_semUpdate;   /* @ 0x826065FC */
extern uint32_t g_semIO;       /* @ 0x82606600 */

// g_mainThreadHandle — kernel handle for the RAGE main-loop scheduler thread
extern uint32_t g_mainThreadHandle;  /* @ 0x825D0078 */

// atSingleton_2598_w — RAGE main-loop scheduler thread entry function
// Registered by rage_subsystem_init as the primary game-loop thread.
extern void atSingleton_2598_w(void* gameObjPtr);  /* @ 0x822E2598 */

// Subsystem init debug strings (.rdata, release no-ops via nop_8240E6D0)
extern const char k_subsysInitErrFmt[];   /* @ 0x8205A30C */
extern const char k_rageThreadNameFmt[];  /* @ 0x8205A340 — debug thread name */


// ===========================================================================
// rage_subsystem_init  @ 0x822E2430 | size: 0xDC
//
// Initialises the three RAGE scheduler semaphores and registers the RAGE
// main-loop scheduler thread (atSingleton_2598_w @ 0x822E2598).
//
// Step 1: Guard — aborts (with debug log) if g_game_obj_ptr is null.
//         The root game object must be created before subsystems can init.
//
// Step 2: Allocate three XAM kernel semaphores via xam_6C88_g:
//           g_semRender (0x826065F8): type=0, init=1, max=0, flags=0
//           g_semUpdate (0x826065FC): type=0, init=1, max=0, flags=0
//           g_semIO     (0x82606600): type=0, init=1, max=1, flags=0
//         NOTE: max=0 for render/update (binary semaphores), max=1 for IO.
//
// Step 3: Register the RAGE main-loop scheduler thread:
//           func   = atSingleton_2598_w (game-loop scheduler)
//           param  = g_game_obj_ptr     (startup arg: root game object)
//           stack  = 0x18000 (98304 bytes)
//           basePriorityDelta = 8  → KeSetBasePriorityThread(h, 8-8=0)
//           nameFmt = k_rageThreadNameFmt (debug name, no-op in release)
//           startSuspended = 1   → thread starts suspended
//           processorAffinity = 0 → run on processor 0
//         Handle stored to g_mainThreadHandle @ 0x825D0078.
//
// Key globals (all at lis(-32160)+offset, base=0x82600000):
//   g_game_obj_ptr @ 0x82606604  — root game object pointer (guard + thread param)
//   g_semRender    @ 0x826065F8  — render semaphore handle
//   g_semUpdate    @ 0x826065FC  — update semaphore handle
//   g_semIO        @ 0x82606600  — IO semaphore handle
// ===========================================================================
void rage_subsystem_init(void) {
    // Guard: root game object must exist.
    if (g_game_obj_ptr == NULL) {
        nop_8240E6D0(k_subsysInitErrFmt);
        return;
    }

    // Allocate render semaphore: binary (max=0), starts signalled (init=1).
    g_semRender = xam_6C88_g(0, 1, 0, 0);

    // Allocate update semaphore: binary (max=0), starts signalled (init=1).
    g_semUpdate = xam_6C88_g(0, 1, 0, 0);

    // Allocate IO semaphore: counting (max=1), starts signalled (init=1).
    g_semIO = xam_6C88_g(0, 1, 1, 0);

    // Register the RAGE main-loop scheduler thread.
    g_mainThreadHandle = rage_thread_register_7FD0(
        (void*)atSingleton_2598_w, /* r3: thread entry function             */
        (void*)g_game_obj_ptr,     /* r4: startup param = root game object  */
        98304u,                    /* r5: stack size = 0x18000              */
        8u,                        /* r6: base priority delta (8-8=0)       */
        (void*)k_rageThreadNameFmt,/* r7: debug thread name fmt             */
        1u,                        /* r8: start suspended                   */
        0u                         /* r9: processor affinity = CPU 0        */
    );
}

// ---------------------------------------------------------------------------
// Audio system globals (used by audSystem_init below)
// ---------------------------------------------------------------------------
extern const char* g_audDevicePaths[];  /* null-terminated list @ 0x825D1D40 */
extern const char  g_audBasePath[];     /* base path prefix      @ 0x8204DAA4 */
extern const char  g_audPathFmt[];      /* "%s%s" format         @ 0x82027434 */
extern const char  g_audPathFmt2[];     /* alternate fmt         @ 0x8204DBFC */




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
// pgStreamer_Init  @ 0x820F8A00 | size: 0xFC
//
// Initialises the pgStreamer asynchronous streaming pool and starts the
// pgStreamer background reader thread (pg_8250_g).
//
// This function lives in the pgStreamer compilation unit, immediately between
// pgStreamer_Close (0x820F88F8) and pgStreamer_Drain (0x820F8B00) in the
// binary layout.  The name "grcDevice_init_8A00" from the call-tree doc was
// a provisional label; the true subsystem is the pgStreamer asset streamer.
//
// Steps:
//  1. Stamp pool capacity (256) into g_pgStreamerPool.capacity.
//  2. Ensure the main-thread heap context is live (xe_main_thread_init_0038).
//  3. Allocate the backing pool via the RAGE sysMemAllocator vtable slot 1:
//       Alloc(size=0xA000, align=16)  →  256 × 160 bytes = 40 960 bytes.
//  4. Build a singly-linked free list across all 256 nodes:
//       node[+0]  = i             (self-index)
//       node[+4]  = 0xFFFFFFFF   (free sentinel)
//       node[+16] = i + 1        (next-free; 256 = end-of-list)
//  5. Reset freeHead = 0 (all nodes free, head at slot 0).
//  6. If g_pgThreadInitGate is unarmed, register pg_8250_g as a background
//     kernel thread:
//       stack  = 65 536 bytes (0x10000)
//       base-priority delta = 6  →  KeSetBasePriorityThread(h, 8-6=2)
//       processor = 0  →  XSetThreadProcessor(h, 0)
//
// Key globals:
//   g_pgStreamerPool.capacity @ 0x825EB26C  — written here (256)
//   g_pgStreamerPool.freeHead @ 0x825EB270  — written here (0)
//   g_pgStreamerPool.pMemory  @ 0x825EB274  — written here (alloc ptr)
//   g_pgQueue                @ 0x82607AA8  — 16-slot ring + semaphores (408 B)
//                                             (used by pg_8250_g via SDA)
//   g_pgThreadInitGate       @ 0x825CA074  — RTTI-gate; m_pName != NULL → skip
//   k_pgThreadNameFmt        @ 0x8202FB00  — "%s" thread name format
// ===========================================================================

// Three adjacent .data symbols form the pool descriptor:
//   lbl_825EB25C (20 B) — stats header (touched by pgStreamer_ProcessEntry)
//     +0x10 of that symbol = capacity field below
//   lbl_825EB270 (4 B)  — freeHead
//   lbl_825EB274 (8 B)  — pMemory + padding
// We model just the three fields this function writes:
typedef struct pgStreamerPool {
    uint32_t capacity;   /* @ 0x825EB26C — always 256 */
    uint32_t freeHead;   /* @ 0x825EB270 — head of free list */
    void*    pMemory;    /* @ 0x825EB274 — backing allocation */
} pgStreamerPool;

extern pgStreamerPool g_pgStreamerPool;  /* anchored at 0x825EB26C */

// Gate: non-null m_pName = thread already registered
typedef struct {
    uint32_t m_vftable;
    uint8_t* m_pName;
} pgThreadInitGate;
extern pgThreadInitGate g_pgThreadInitGate;  /* @ 0x825CA074 */

extern void           pg_8250_g(void);        /* pgStreamer reader thread @ 0x820F8250 */
extern const char     k_pgThreadNameFmt[];    /* "%s" @ 0x8202FB00 */

void pgStreamer_Init(void)
{
    g_pgStreamerPool.capacity = 256;

    // Ensure the main-thread allocator heap is ready.
    xe_main_thread_init_0038();

    // Allocate backing pool: 256 nodes × 160 bytes = 0xA000 bytes, 16-byte aligned.
    // VCALL: allocObj = *(g_sda_base[0] + 4);  call allocObj->vtable[1](size, align)
    extern uint32_t* g_sda_base;
    uint32_t* allocRoot = (uint32_t*)(uintptr_t)g_sda_base[0];
    void*     allocObj  = (void*)(uintptr_t)allocRoot[1];
    typedef void* (*AllocFn)(void*, uint32_t, uint32_t);
    AllocFn   allocFn   = ((AllocFn*)*(void**)allocObj)[1];
    void*     pool      = allocFn(allocObj, 0xA000u, 16u);
    g_pgStreamerPool.pMemory = pool;

    // Build free list: node stride = 160 bytes.
    uint8_t* base = (uint8_t*)pool;
    for (int i = 0; i < 256; ++i) {
        uint8_t* node = base + (uint32_t)i * 160u;
        *(uint32_t*)(node +  0) = (uint32_t)i;       /* own index (0-255)    */
        *(uint32_t*)(node +  4) = 0xFFFFFFFFu;       /* free sentinel        */
        *(uint32_t*)(node + 16) = (uint32_t)(i + 1); /* next (256 = end)     */
    }
    g_pgStreamerPool.freeHead = 0;

    // Register the reader thread if not already started.
    // r4 = 0 at the call site (result of gate-null clrlwi left 0 in r4).
    // The thread uses g_pgQueue (SDA global) rather than a startup param.
    if (g_pgThreadInitGate.m_pName == NULL) {
        rage_thread_register_7FD0(
            (void*)pg_8250_g,         /* r3: thread function                */
            NULL,                     /* r4: startup param (0 — uses SDA)  */
            65536u,                   /* r5: stack size = 0x10000           */
            6u,                       /* r6: base-priority delta (8-6 = 2) */
            (void*)k_pgThreadNameFmt, /* r7: "%s" name format               */
            1u,                       /* r8: start suspended = 1            */
            0u                        /* r9: processor affinity = 0         */
        );
    }
}

