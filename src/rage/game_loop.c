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
/* audio_system.hpp removed: C file needs no C++ headers */
#include <stdint.h>
#include <string.h>
extern void*    g_game_obj_ptr;
extern void     xe_main_thread_init_0038(void);

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
extern uint32_t rage_RegisterThread(void* func, void* param,
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
    g_mainThreadHandle = rage_RegisterThread(
        (void*)atSingleton_2598_w, /* r3: thread entry function             */
        (void*)g_game_obj_ptr,     /* r4: startup param = root game object  */
        98304u,                    /* r5: stack size = 0x18000              */
        8u,                        /* r6: base priority delta (8-8=0)       */
        (void*)k_rageThreadNameFmt,/* r7: debug thread name fmt             */
        1u,                        /* r8: start suspended                   */
        0u                         /* r9: processor affinity = CPU 0        */
    );
}

// ===========================================================================
// audSystem_init_1ED0  @ 0x82221ED0 | size: 0x18C
//
// Opens the audio render-output device ("RN_OUT") then mounts each component
// path in the audio-device table.
//
// The audio-device table (g_audPathTable @ 0x825D1D40, 96 bytes) is an array
// of 12 eight-byte records:
//
//   struct AudPathRecord {
//       const char* m_pPath;   // +0  — path string; NULL ends iteration
//       uint32_t    m_fHasMore;// +4  — non-null means at least one more entry
//   };
//
// For each non-empty entry:
//   1. Build fullPath:     snprintf(buf, 256, g_audPathFmt,  "RN_OUT", entry)
//   2. Build strippedPath: snprintf(buf, 256, g_audPathFmt2, "RN_OUT", entry)
//   3. pos = strlen(strippedPath)   ← prefix length
//   4. Inner loop — walk fullPath from pos, split on '\\':
//        a. Copy chars fullPath[pos..pos+n] to strippedPath[pos..pos+n]
//           until '\\' or '\0' is found.
//        b. Null-terminate strippedPath[pos+n].
//        c. device->vtable[10](device, strippedPath)  ← mount this component.
//        d. Restore strippedPath[pos+n] = '\\'.
//        e. pos += n + 1 (skip the backslash).
//        f. Repeat until fullPath[pos] == '\0'.
//
// Key globals:
//   g_audPathTable   @ 0x825D1D40  — 12-entry AudPathRecord array
//   k_audBasePath    @ 0x8204DAA4  — "RN_OUT"  (14 bytes incl. null)
//   k_audPathFmt     @ 0x82027434  — format1: "%s%s" or similar (6 bytes)
//   k_audPathFmt2    @ 0x8204DBFC  — format2: "NameChant" label (4 bytes)
// ===========================================================================

// Entry in the audio-device path table (8 bytes each, 12 entries @ 0x825D1D40).
typedef struct AudPathRecord {
    const char* m_pPath;    /* +0 — path string pointer; NULL = end of list */
    uint32_t    m_fHasMore; /* +4 — non-null means successor entry exists   */
} AudPathRecord;

extern AudPathRecord g_audPathTable[12]; /* @ 0x825D1D40 */
extern const char    k_audBasePath[];    /* "RN_OUT"     @ 0x8204DAA4 */
extern const char    k_audPathFmt[];     /* path format1 @ 0x82027434 */
extern const char    k_audPathFmt2[];    /* path format2 @ 0x8204DBFC */

// Virtual-call helper: fiDevice::Mount (vtable slot 10, byte offset 40)
typedef int (*fiDevice_MountFn)(struct fiDevice*, const char*);
#define fiDevice_Mount(dev, path) \
    ((fiDevice_MountFn)(*(void***)(dev))[10])((dev), (path))

void audSystem_init(void)
{
    // Open the base audio output device ("RN_OUT").
    struct fiDevice* device = fiDevice_GetDevice(k_audBasePath, 1);

    // First-entry guard: if the first record has no successor, bail.
    if (g_audPathTable[0].m_fHasMore == 0)
        return;

    // Walk each path record in the table.
    for (int i = 0; g_audPathTable[i].m_pPath != NULL; ++i) {
        const char* entry = g_audPathTable[i].m_pPath;

        // Measure path length; skip empty entries.
        int elen = 0;
        while (entry[elen]) elen++;
        if (elen == 0)
            continue;

        // Build the two formatted path buffers on the stack.
        char strippedPath[256];   /* sp+80  in original frame  */
        char fullPath[256];       /* sp+336 in original frame  */
        snprintf(fullPath,     sizeof(fullPath),     k_audPathFmt,  k_audBasePath, entry);
        snprintf(strippedPath, sizeof(strippedPath), k_audPathFmt2, k_audBasePath, entry);

        // pos starts at end of the prefix (strippedPath's filled length).
        int pos = 0;
        while (strippedPath[pos]) pos++;

        // Inner loop: split fullPath on '\\' from position pos onward,
        // mounting each component via the device vtable.
        // Assembly: loc_82221FB8 — loc_82222040.
        for (;;) {
            // Copy chars from fullPath[pos..] into strippedPath[pos..]
            // until we hit '\\' or '\0'.
            int start = pos;
            for (;;) {
                char c = fullPath[pos];
                if (c == '\\' || c == '\0')
                    break;
                strippedPath[pos] = c;   /* stbx r10, r8, r11 (r8 = sp+80-sp+336 = -256) */
                pos++;
            }

            // Null-terminate and mount this component path.
            strippedPath[pos] = '\0';
            fiDevice_Mount(device, strippedPath);

            // Restore the backslash separator and advance past it.
            strippedPath[pos] = '\\';
            char next = fullPath[pos];   /* lbzx r7, r31, r8 — char AT pos */
            pos++;                        /* addi r31, r31, 1  */

            // Stop when we've consumed the full path.
            if (next == '\0')
                break;
        }

        // Advance to next record and check the "has more" flag.
        // (The loop condition re-evaluates g_audPathTable[i].m_pPath.)
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
        rage_RegisterThread(
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



// ===========================================================================
// fiStreamBuf — RAGE file-stream ring buffer
//
// The three functions fiStreamBuf_OpenAll, rage_obj_bind_3828, and
// fiStreamBuf_Close (along with the helper rage_obj_close_3BA8) all
// operate on the same ring-buffer object that mediates async file reads via
// the fiDevice virtual-device abstraction.
//
// Struct layout (offsets confirmed from scaffolds):
//   +0x00  void*    vtable     — virtual dispatch (fiDevice-compatible layout)
//   +0x04  uint32_t flags      — open-mode flags passed to virtual Open
//   +0x08  uint8_t* pBuffer    — backing store (allocated by device Open)
//   +0x0C  int32_t  writePos   — absolute stream position of buffer end
//   +0x10  int32_t  readPos    — start of unconsumed data within pBuffer
//   +0x14  int32_t  endPos     — exclusive end of buffered data (0 = empty)
//   +0x18  int32_t  capacity   — maximum fetch size (buffer size limit)
//
// The device-enumeration factory object (first arg to fiStreamBuf_OpenAll)
// has two relevant fields:
//   +0x600 (1536)  const char*  m_pBasePath  — base directory path
//   +0x604 (1540)  int32_t      m_nDevices   — number of devices to open
// ===========================================================================

// ---------------------------------------------------------------------------
// Supporting path utility — fiPath_RemoveParentDir @ 0x822E2A20 | size: 0x12C
//
// Removes one occurrence of a "../" (or "..\") component from a path string
// in-place.  Called in a loop by fiPath_Build until all ../ are gone.
//
// Algorithm:
//   1. Find the first '.' character at or after pPath (using ph_21B0 with
//      delim=46 i.e. '.').
//   2. Verify it is preceded by '/' or '\' and followed by "./" or ".\".
//   3. Walk backward from the found '..' to find the parent-directory boundary.
//   4. Collapse the "<parent>/../" sequence: memmove to remove it.
//   Returns: 1 if a collapse was performed, 0 if not.
// ---------------------------------------------------------------------------
extern void* ph_21B0(const char* s, int delim);  /* strchr-like @ 0x824321B0 */

static int fiPath_RemoveParentDir(char* pPath)
{
    // Find first '.' in the path
    char* pDot = (char*)ph_21B0(pPath, 46 /* '.' */);
    if (pDot == NULL || (uintptr_t)pDot <= (uintptr_t)pPath) {
        return 0;
    }

    // Verify the character before pDot is a directory separator
    char cBefore = pDot[-1];
    if (cBefore != '/' && cBefore != '\\') {
        return 0;
    }

    // Verify character after pDot is '.' (i.e. we have "..")
    if (pDot[1] != 46 /* '.' */) {
        return 0;
    }

    // Verify the character after ".." is a separator
    char cAfter = pDot[2];
    if (cAfter != '/' && cAfter != '\\') {
        return 0;
    }

    // Walk backward past the parent directory component to find its separator
    char* pParentSep = pDot - 2;
    char* pLimit     = pPath + 2;
    while (pParentSep > pLimit) {
        char c = pParentSep[-1];
        if (c == '/' || c == '\\') {
            break;
        }
        --pParentSep;
    }

    // Measure the tail (everything after the "../")
    char*   pTail    = pDot + 3;
    size_t  tailLen  = 0;
    while (pTail[tailLen]) { ++tailLen; }

    // Collapse: move tail over the "<parent>/../" region
    memmove(pParentSep - 1, pTail, tailLen + 1);
    return 1;
}


// ---------------------------------------------------------------------------
// fiPath_Build @ 0x822E2B50 | size: 0x1E0
//
// Constructs a canonicalized file path into pOutBuf (max outLen chars) by
// combining pBasePath and pRelPath:
//   1. If pRelPath is an absolute path (starts with '/' or '\', or has a
//      drive letter e.g. "C:"), copy it verbatim and skip the base path.
//   2. Otherwise: if pBase is set on pFactory (offset +1536), try to prepend
//      the factory base path.  If that base path is also absolute, use it
//      alone; otherwise combine base + relPath.
//   3. After assembly, call fiPath_RemoveParentDir in a loop to eliminate
//      all "../" components.
//   4. Finally call fiDeviceMemory_2830(pFactory, pOutBuf, relPath, basePath,
//      devIndex) — registers the opened device with the factory.
//
// Parameters:
//   pFactory    (r3) — device-enumeration factory object
//   pOutBuf     (r4) — output buffer (PATH_MAX = 256 bytes)
//   outLen      (r5) — output buffer length (always 256 at call site)
//   pRelPath    (r6) — relative or absolute file path to open
//   devIndex    (r7) — device index within the factory's list
//   entryIdx    (r8) — entry index (passed through to fiDeviceMemory_2830)
// ---------------------------------------------------------------------------
extern void fiDeviceMemory_2830(void* pFactory, char* pOutBuf,
                                const char* pRelPath, const char* pBasePath,
                                int devIndex);  /* @ 0x822E2830 */
extern void rage_strcat_bounded(void* pDst, const char* pSrc, size_t n,
                      const char* pFmt);  /* @ 0x820F0888 — formatted strncpy */

static void fiPath_Build(void* pFactory, char* pOutBuf, int outLen,
                         const char* pRelPath, int devIndex, int entryIdx)
{
    uint8_t* pFact = (uint8_t*)pFactory;

    // Determine if pRelPath is an absolute path (starts with '/' or '\',
    // or contains a ':' drive-letter separator).
    int bAbsRelPath = 0;
    if (pRelPath != NULL) {
        char c = pRelPath[0];
        if (c == '/' || c == '\\') {
            bAbsRelPath = 1;
        } else {
            // Check for drive letter (contains ':')
            if ((char*)ph_21B0(pRelPath, 58 /* ':' */) != NULL) {
                bAbsRelPath = 1;
            }
        }
    }

    // Build slot base address within factory: slot = entryIdx * 128
    int slotBase = *(int32_t*)(pFact + 1540);  // reuse as temp; real slot:
    (void)slotBase;

    int   factBase = *(int32_t*)(pFact + 1536); // base path index
    char* pSrcBase = (factBase != 0)
                        ? (char*)(pFact + 3 + (uint32_t)((factBase + 3) & ~3))
                        : NULL;

    // Copy relative portion into pOutBuf (up to outLen-1 chars)
    {
        int  remaining = outLen - 1;
        char* pDst     = pOutBuf;
        if (pRelPath != NULL && !bAbsRelPath) {
            // Prepend base path from factory if available and non-absolute
            const char* pBase = pSrcBase;
            if (pBase != NULL) {
                while (remaining > 0 && *pBase) {
                    *pDst++ = *pBase++;
                    --remaining;
                }
            }
        }
        // Append relative path
        if (pRelPath != NULL) {
            while (remaining > 0 && *pRelPath) {
                *pDst++ = *pRelPath++;
                --remaining;
            }
        }
        *pDst = '\0';
    }

    // Normalize: collapse all "../" sequences
    while (fiPath_RemoveParentDir(pOutBuf)) { /* loop */ }

    // Register with factory
    fiDeviceMemory_2830(pFactory, pOutBuf, pRelPath, pSrcBase, devIndex);
}


// ---------------------------------------------------------------------------
// fiStreamBuf struct definition
// ---------------------------------------------------------------------------
typedef struct {
    void*       vtable;    // +0x00 virtual dispatch
    uint32_t    flags;     // +0x04 open-mode flags
    uint8_t*    pBuffer;   // +0x08 ring buffer backing store
    int32_t     writePos;  // +0x0C absolute stream write offset
    int32_t     readPos;   // +0x10 read cursor within pBuffer
    int32_t     endPos;    // +0x14 exclusive end of buffered data (0 = empty)
    int32_t     capacity;  // +0x18 maximum capacity (buffer size)
} fiStreamBuf;


// ---------------------------------------------------------------------------
// fiStreamBuf_FetchChunk @ 0x822E3BA8 | size: 0xC0
//
// Fetches the next chunk of data from the underlying device into pBuf:
//
//   Case A (endPos != 0, data queued for commit):
//     If endPos != readPos (partial buffer used):
//       call vtable[5](this, flags, pBuffer+readPos, 0, endPos-readPos)
//       — "write commit" for the buffered window
//     Else (entire ring used up to capacity):
//       call vtable[4](this, flags, pBuffer)
//       — "flush full buffer"
//
//   Case B (endPos == 0, buffer empty but readPos != 0):
//     call vtable[4](this, flags, pBuffer)
//     — "re-use empty buffer from readPos"
//
//   After either case:
//     Clear endPos = 0, readPos = 0.
//     Advance writePos += old endPos.
//     Call vtable[8](this, flags) — "notify write complete".
//
//   Returns: 0 on success, -1 on failure (propagated from virtual calls).
// ---------------------------------------------------------------------------
static int fiStreamBuf_FetchChunk(fiStreamBuf* pBuf)
{
    if (pBuf->endPos != 0) {
        int32_t oldEnd  = pBuf->endPos;
        int32_t oldRead = pBuf->readPos;

        if (oldEnd != oldRead) {
            // Partial buffer: commit [readPos .. endPos)
            typedef int (*CommitFn)(void*, uint32_t, uint8_t*, int, int);
            int r = ((CommitFn)((void**)pBuf->vtable)[5])(
                        pBuf, pBuf->flags,
                        pBuf->pBuffer + oldRead,
                        0,
                        oldEnd - oldRead);
            if (r != 0) {
                return -1;
            }
        } else {
            // Full buffer: flush from base
            typedef int (*FlushFn)(void*, uint32_t, uint8_t*);
            int r = ((FlushFn)((void**)pBuf->vtable)[4])(
                        pBuf, pBuf->flags, pBuf->pBuffer);
            if (r != 0) {
                return -1;
            }
        }

        // Reset buffer window and advance absolute write position
        pBuf->readPos  = 0;
        pBuf->endPos   = 0;
        pBuf->writePos += oldEnd;

    } else if (pBuf->readPos != 0) {
        // Empty but offset: re-use from base
        typedef int (*FlushFn)(void*, uint32_t, uint8_t*);
        int r = ((FlushFn)((void**)pBuf->vtable)[4])(
                    pBuf, pBuf->flags, pBuf->pBuffer);
        if (r != 0) {
            return -1;
        }

        int32_t oldRead = pBuf->readPos;
        pBuf->readPos  = 0;
        pBuf->endPos   = 0;
        pBuf->writePos += oldRead;
    } else {
        // Nothing to flush
        return 0;
    }

    // Notify write complete (vtable slot 8)
    typedef void (*NotifyFn)(void*, uint32_t);
    ((NotifyFn)((void**)pBuf->vtable)[8])(pBuf, pBuf->flags);
    return 0;
}


// ---------------------------------------------------------------------------
// rage_obj_bind_3828 / fiStreamBuf_Read @ 0x822E3828 | size: 0x184
// Reads `size` bytes from the ring buffer into pDst.
//
// Algorithm:
//   1. If endPos == 0 && readPos != 0 (buffer consumed, more to fetch):
//      call FetchChunk; if it fails return -1.
//   2. Compute available = endPos - readPos.
//   3. If size > available:
//        a. If there is partial buffered data (endPos != readPos):
//             Copy buffered data to pDst; advance pDst, size, and
//             note how many bytes were pre-copied (r28).
//        b. Call vtable[3](this, flags, pDst, size, capacity)
//             — "request more data" virtual; fills pDst up to capacity.
//           If result < 0: reset readPos/endPos to 0; return -1.
//        c. Store new endPos = result; reset readPos = 0.
//        d. Return accumulated (r26 + size_requested).
//   4. If size <= available:
//        Clamp size to min(size, endPos - readPos).
//        Copy pBuffer[readPos..readPos+size] to pDst.
//        readPos += size.
//        Return r26 + size.
//
// Parameters:
//   pBuf  (r3) — fiStreamBuf*
//   pDst  (r4) — destination buffer
//   size  (r5) — bytes to read
//   (r6, r7 used internally)
//   r7    (r7) — passed to vtable[3] as 5th arg
// ---------------------------------------------------------------------------
int fiStreamBuf_Read(fiStreamBuf* pBuf, uint8_t* pDst, int32_t size, int r6, int r7)
{
    int32_t accumulated = 0;

    // If endPos is exhausted but readPos is advanced, we need more data.
    // NOTE: condition is (endPos == 0 && readPos != 0), NOT readPos == 0.
    // A fresh buffer at position 0,0 needs no fetch — the device call below
    // handles first-fetch.  Only a buffer where we consumed data (readPos>0)
    // but then wrapped back to endPos=0 needs an explicit FetchChunk here.
    if (pBuf->endPos == 0 && pBuf->readPos != 0) {
        int rc = fiStreamBuf_FetchChunk(pBuf);
        if (rc < 0) {
            return -1;
        }
    }


    int32_t available = pBuf->endPos - pBuf->readPos;

    if (size > available) {
        int32_t preCopied = 0;

        // Pre-copy whatever is currently buffered into pDst
        if (pBuf->endPos != pBuf->readPos) {
            int32_t partialLen = pBuf->endPos - pBuf->readPos;
            memcpy(pDst, pBuf->pBuffer + pBuf->readPos, (size_t)partialLen);
            pBuf->readPos  = 0;  // consumed
            pBuf->endPos   = 0;
            pBuf->writePos += partialLen;
            pDst           += partialLen;
            size           -= partialLen;
            preCopied       = partialLen;
            accumulated    += partialLen;
        }

        // Request more data from device through vtable slot 3
        typedef int (*RequestFn)(void*, uint32_t, uint8_t*, int32_t, int32_t);
        int32_t got = ((RequestFn)((void**)pBuf->vtable)[3])(
                          pBuf, pBuf->flags, pDst, size, pBuf->capacity);

        if (got < 0) {
            pBuf->readPos = 0;
            pBuf->endPos  = 0;
            return -1;
        }

        pBuf->readPos = 0;
        pBuf->endPos  = got;

        return accumulated + preCopied;

    } else {
        // Sufficient data in buffer: clamp and copy
        int32_t clampedSize = (size < available) ? size : available;
        memcpy(pDst, pBuf->pBuffer + pBuf->readPos, (size_t)clampedSize);
        pBuf->readPos += clampedSize;
        return accumulated + clampedSize;
    }
}

// ---------------------------------------------------------------------------
// fiStreamBuf_Close / fiStreamBuf_Close @ 0x822E3B38 | size: 0x70
//
// Closes the stream buffer:
//   1. If endPos == 0 and readPos != 0: call FetchChunk (flush pending data).
//      NOTE: assembly does NOT check FetchChunk's return value here and
//      does NOT return early — it proceeds to vtable Close unconditionally.
//   2. Call vtable[6](this, flags) — virtual Close (byte offset 24 in vtable).
//   3. Set flags = -1, vtable = NULL (marks the object as destroyed).
//   Returns: 0.
// ---------------------------------------------------------------------------
int fiStreamBuf_Close(fiStreamBuf* pBuf)
{
    // Flush trailing data if the buffer window is empty but stream advanced.
    // The assembly calls FetchChunk but ignores its return value — no early exit.
    if (pBuf->endPos == 0 && pBuf->readPos != 0) {
        fiStreamBuf_FetchChunk(pBuf);   /* result intentionally discarded */
    }

    // Virtual Close — vtable slot 6 (byte offset 24)
    typedef void (*CloseFn)(void*, uint32_t);
    ((CloseFn)((void**)pBuf->vtable)[6])(pBuf, pBuf->flags);

    // Invalidate the object so double-close is detectable
    pBuf->flags  = (uint32_t)-1;
    pBuf->vtable = NULL;
    return 0;
}



// ---------------------------------------------------------------------------
// fiStreamBuf_OpenAll / fiStreamBuf_OpenAll @ 0x822E3040 | size: 0x78
//
// Device-enumeration loop: for each of the factory's m_nDevices entries,
// builds the full path and opens a fiStream.
//
// Parameters:
//   pFactory (r3) — factory/enumerator object
//   pRelPath (r4) — base relative path (e.g. "game.rpf")
//   pAux     (r5) — auxiliary data pointer passed to fiPath_Build
//   (r6 unused)
//   pExtra   (r7) — extra parameter forwarded to fiStream_Open
//
// For each device i (0..m_nDevices-1):
//   1. Build path: fiPath_Build(pFactory, stackBuf, 256, pRelPath, pAux, i)
//   2. Open stream: fiStream_Open(stackBuf, pExtra)
//   3. If stream is opened (returns non-null handle), stop early.
//      Assembly: r3 is tested at the top of each loop iteration;
//      any non-zero return from fiStream_Open causes an immediate exit.
// ---------------------------------------------------------------------------
extern uint32_t fiStream_Open(char* pPath, void* pExtra);  /* @ 0x822E3518 */

void fiStreamBuf_OpenAll(void* pFactory, const char* pRelPath,
                         void* pAux, void* pUnused, void* pExtra)
{
    uint8_t* pFact    = (uint8_t*)pFactory;
    int32_t  nDevices = *(int32_t*)(pFact + 1540);

    char    pathBuf[256];
    uint32_t result = 0;

    for (int i = 0; i < nDevices; ++i) {
        // Early-exit: a previous iteration successfully opened a stream
        if (result != 0)
            break;

        // Build the full path for device i
        fiPath_Build(pFactory, pathBuf, 256, pRelPath, i, (int)(uintptr_t)pAux);

        // Open the stream; capture result — non-null means success, stops loop
        result = fiStream_Open(pathBuf, pExtra);
    }
}
