/**
 * rage/scene_render.c — RAGE scene render entry points
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers the two functions that form the top of the rendering call chain,
 * from the game loop's Render phase down to the first GPU draw call:
 *
 *   rage_render_default_08D8  @ 0x822C08D8 (576 bytes)
 *     — THE FIRST DRAW CALL.  Builds the per-frame clear-flags word,
 *       updates the render-command ring buffer, calls grcDevice::beginScene
 *       and grcDevice::clear, then polls input and invokes the page manager.
 *
 *   rage_render_scene_0B18    @ 0x822C0B18 (304 bytes)
 *     — Scene render controller.  Wraps rage_render_default_08D8 with an
 *       optional pg profiling pass and a post-render stream flush.
 *
 * Ring-buffer layout (per-frame, double-buffered):
 *   g_renderCmdRingBufPtr points to a flat array of uint32_t.
 *   The slot for a given frame is at (frameWriteIdx + 24) * 4 bytes.
 *   g_renderCmdRingCounter wraps at 2 (tracks which of two slots is live).
 *
 * The gameLoop render flags accessed here (m_bClearColor/Depth/Stencil at
 * +0x240-0x242) extend beyond the struct documented in render_loop.c and
 * are populated once per frame by the game-state machine before this
 * function is called.
 */

#include "rage/rage_system.hpp"
#include <stdint.h>
#include "rage/grc_setup_types.h"  /* __mftb() */

/* ── Forward declarations ─────────────────────────────────────────────────── */

/* Graphics device — beginScene @ 0x82305E78 */
extern void    grcDevice_beginScene(void* pDevice);

/* Graphics device — clear @ 0x82379290 */
extern void    grcDevice_clear(void* pDevice);

/* Input system — poll controller state @ 0x821C9D68 */
extern uint8_t io_Input_poll(void* pInput);

/* Page manager — per-frame notification @ 0x823CEDE0 */
extern void    pg_EDE0_gen(void* pPageObj, char flag);

/* Assert / debug log — no-op in release @ 0x8240E6D0 */
extern void    nop_8240E6D0(const char* msg);

/* pg_C3B8_g — pgStreamer start/cancel profiling stamp @ 0x8242C3B8 */
extern void    pg_C3B8_g(void* pPgObj, int32_t mode);

/* pg_6C40_g — pgStreamer finalize stream @ 0x82566C40 */
extern void    pg_6C40_g(void* pStream);

/* gameLoop struct defined in render_loop.c / rage_system.hpp */
#include "rage/game_loop_types.h"

/* ── Globals ──────────────────────────────────────────────────────────────── */

/*
 * g_loop_obj_ptr — pointer to the current gameLoop singleton.
 * lwz r3, -21712(lis(-32161)) = *(0x825EAB30)
 */
extern gameLoop* g_loop_obj_ptr;             /* @ 0x825EAB30 */

/*
 * g_phFrameIndex — physics/render frame parity index (0 or 1).
 * SDA+26076 = 0x826065DC.  Shared with physics frame-buffer indexing.
 */
extern uint32_t g_phFrameIndex;              /* @ 0x826065DC */

/*
 * g_phFrameCounter — secondary frame parity counter.
 * lis(-32164)+18584 = 0x825C4898.
 */
extern uint32_t g_phFrameCounter;            /* @ 0x825C4898 */

/*
 * g_renderCmdRingCounter — live render-command ring slot (wraps at 2).
 * SDA+26348 = 0x826066EC.
 */
extern uint32_t g_renderCmdRingCounter;      /* @ 0x826066EC */

/*
 * g_renderCmdRingCounterSaved — snapshot of g_renderCmdRingCounter taken
 * each frame before incrementing.  SDA+26344 = 0x826066E8.
 */
extern uint32_t g_renderCmdRingCounterSaved; /* @ 0x826066E8 */

/*
 * g_renderCmdRingBufPtr — pointer to the render-command ring buffer array.
 * SDA+25844 = 0x826064F4.
 */
extern uint32_t* g_renderCmdRingBufPtr;      /* @ 0x826064F4 */

/*
 * g_renderClearRingIdx — running modulo index into the clear-command ring.
 * SDA+25416 = 0x82606348.
 */
extern uint32_t g_renderClearRingIdx;        /* @ 0x82606348 */

/*
 * g_renderClearRingSaved — snapshot of g_renderClearRingIdx.
 * SDA+25420 = 0x8260634C.
 */
extern uint32_t g_renderClearRingSaved;      /* @ 0x8260634C */

/*
 * g_renderClearRingDivisor — modulo divisor for g_renderClearRingIdx.
 * @ 0x825C9A6C.
 */
extern uint32_t g_renderClearRingDivisor;    /* @ 0x825C9A6C */

/*
 * g_renderCmdBufBase — base pointer to the GPU command buffer.
 * The clear-flag bytes for this frame are stored at byte offsets
 * 0x189C0 (stencil flag) and 0x189C1 (stencil availability).
 * SDA+25468 = 0x8260637C.
 */
extern uint8_t* g_renderCmdBufBase;          /* @ 0x8260637C */

/*
 * g_pRenderSetupObj — render-state object; vtable slot 32 sets up the
 * clear colour prior to grcDevice::clear.
 * SDA+25648 = 0x82606430.
 */
extern void*    g_pRenderSetupObj;           /* @ 0x82606430 */

/*
 * g_pGrcDevice_beginScene — grcDevice instance used for beginScene.
 * @ 0x8271A81C.
 */
extern void*    g_pGrcDevice_beginScene;     /* @ 0x8271A81C */

/*
 * g_pGrcDevice_clear — grcDevice instance used for clear.
 * @ 0x8271A320.
 */
extern void*    g_pGrcDevice_clear;          /* @ 0x8271A320 */

/*
 * g_pStencilMgrObj — stencil manager; vtable slot 34 returns a bool
 * indicating stencil-buffer availability for the current frame.
 * SDA+25968 = 0x82606570.
 */
extern void*    g_pStencilMgrObj;            /* @ 0x82606570 */

/*
 * g_pSecondaryDevice — secondary device pointer guarding the input-poll
 * and pg_EDE0_gen path.  NULL means skip that path entirely.
 * @ 0x8271A328.
 */
extern void*    g_pSecondaryDevice;          /* @ 0x8271A328 */

/* Input system singleton pointer @ 0x825EAB28 */
extern void*    g_input_obj_ptr;             /* @ 0x825EAB28 */

/*
 * g_frameRenderCounter — monotonically increasing render-frame counter.
 * Incremented at the end of each rage_render_default call.
 * SDA+26080 = 0x826065E0.
 */
extern uint32_t g_frameRenderCounter;        /* @ 0x826065E0 */

/*
 * g_bProfilingActive — per-render-pass profiling gate byte.
 * Set before entering rage_render_scene; cleared after rage_render_default.
 * lis(-32164)+16639 = 0x825C40FF.
 */
extern uint8_t  g_bProfilingActive;          /* @ 0x825C40FF */

/* Debug/profiling log strings (no-op in release) */
extern const char k_sceneRenderErr[];        /* "onsumed!" @ 0x82059530 */
extern const char k_sceneRenderTimingFmt[];  /* "f, z:%f"  @ 0x82059564 */

/* Ticks-to-seconds conversion constants for profiling timing */
extern const float g_renderTicksToSecondsA; /* @ 0x8202D9A4 */
extern const float g_renderTicksToSecondsB; /* @ 0x82030000 */


/* ═══════════════════════════════════════════════════════════════════════════
 * rage_render_default  @ 0x822C08D8 | size: 0x240 (576 bytes)
 *
 * THE FIRST DRAW CALL — invoked from gameLoop_Render via the hsmContext
 * state-machine dispatch on every rendered frame.
 *
 * 1. Build clear-flags from the gameLoop singleton's per-frame booleans:
 *      bit 0 = m_bClearStencil (+0x242)
 *      bit 1 = m_bClearColor   (+0x240)
 *      bit 2 = m_bClearDepth   (+0x241)
 *
 * 2. Frame ring-buffer update (double-buffered GPU synchronisation):
 *    a. Snapshot & increment g_renderCmdRingCounter (wraps at 2).
 *    b. Ring slot = (g_phFrameIndex + 24) * 4; write clearFlags there.
 *    c. Toggle g_phFrameIndex and g_phFrameCounter (0 -> 1 -> 0 ...).
 *
 * 3. Clear-ring counter: increment g_renderClearRingIdx modulo divisor.
 *
 * 4. If m_bClearColor: call vtable slot 32 on g_pRenderSetupObj to set
 *    the GPU clear colour.
 *
 * 5. If m_bClearColor || m_bClearStencil: call grcDevice::beginScene.
 *
 * 6. Always call grcDevice::clear (honours the clearFlags bitmask).
 *
 * 7. Write m_bClearStencil to GPU cmd buf at byte 0x189C0.
 *    If stencil is active: query g_pStencilMgrObj slot 34 for availability;
 *    write result to GPU cmd buf byte 0x189C1.
 *
 * 8. If g_pSecondaryDevice != NULL:
 *    a. Poll input; if no input ready, increment counter and return early.
 *    b. Read device flag; if zero, increment counter and return early.
 *    c. Notify page manager: pg_EDE0_gen(subObj + 96, flag).
 *
 * 9. Increment g_frameRenderCounter.
 * ═══════════════════════════════════════════════════════════════════════════ */
void rage_render_default(void)
{
    /* 1. Load gameLoop singleton and build the clear-flags word. */
    gameLoop* pLoop = g_loop_obj_ptr;

    uint32_t clearFlags = 0;
    if (pLoop->m_bClearStencil) clearFlags |= 1u;
    if (pLoop->m_bClearDepth)   clearFlags |= 4u;
    if (pLoop->m_bClearColor)   clearFlags |= 2u;

    /* 2a. Snapshot the current frame parity indices. */
    uint32_t frameWriteIdx = g_phFrameIndex;
    uint32_t phFrameIdx    = g_phFrameCounter;

    /* Save old ring counter and increment (wrap at 2). */
    g_renderCmdRingCounterSaved = g_renderCmdRingCounter;
    uint32_t newRingCtr = g_renderCmdRingCounter + 1u;
    g_renderCmdRingCounter = newRingCtr;
    if (newRingCtr >= 2u) {
        g_renderCmdRingCounter = 0u;
    }

    /* 2b. Write clearFlags into the ring buffer at the current slot. */
    uint32_t ringSlot = (frameWriteIdx + 24u) * 4u;
    g_renderCmdRingBufPtr[ringSlot >> 2u] = clearFlags;

    /* 2c. Toggle both frame parity counters (0 <-> 1). */
    g_phFrameIndex   = (frameWriteIdx + 1u) % 2u;
    g_phFrameCounter = (phFrameIdx    + 1u) % 2u;

    /* 3. Modulo-increment the clear-ring index. */
    uint32_t clearIdx      = g_renderClearRingIdx;
    g_renderClearRingSaved = clearIdx;
    g_renderClearRingIdx   = (clearIdx + 1u) % g_renderClearRingDivisor;

    /* 4. Set up clear colour if colour buffer will be cleared. */
    if (pLoop->m_bClearColor) {
        void* pSetupObj = g_pRenderSetupObj;
        typedef void (*SetClearColorFn)(void*);
        ((SetClearColorFn)(*(void***)pSetupObj)[32])(pSetupObj);
        pLoop = g_loop_obj_ptr;   /* re-read; VCALL may have side-effects */
    }

    /* 5. Begin GPU scene if any colour or stencil clearing is needed. */
    if (pLoop->m_bClearColor || pLoop->m_bClearStencil) {
        grcDevice_beginScene(g_pGrcDevice_beginScene);
    }

    /* 6. Clear the GPU framebuffer (colour, depth, and/or stencil). */
    grcDevice_clear(g_pGrcDevice_clear);

    /* 7. Store per-frame flags into the GPU command buffer. */
    pLoop = g_loop_obj_ptr;
    g_renderCmdBufBase[0x189C0] = pLoop->m_bClearStencil;

    uint8_t stencilAvail = 0;
    if (pLoop->m_bClearStencil) {
        /* Query stencil manager for per-frame availability. */
        void* pStencil = g_pStencilMgrObj;
        typedef uint8_t (*QueryStencilFn)(void*);
        uint8_t stencilResult = ((QueryStencilFn)(*(void***)pStencil)[34])(pStencil);
        stencilAvail = (stencilResult != 0) ? 1u : 0u;
    }
    g_renderCmdBufBase[0x189C1] = stencilAvail;

    /* 8. Secondary device path: input poll + page manager notification. */
    if (g_pSecondaryDevice != NULL) {
        /* 8a. Poll controller input; skip the pg path if not ready. */
        uint8_t inputReady = io_Input_poll(g_input_obj_ptr);
        if (inputReady == 0) {
            g_frameRenderCounter++;
            return;
        }

        /* 8b. Read sub-device flag; skip the pg path if it is cleared. */
        uint8_t* pSubObj = *(uint8_t**)((uint8_t*)g_pSecondaryDevice + 20);
        uint8_t  subFlag = pSubObj[148];
        if (subFlag == 0) {
            g_frameRenderCounter++;
            return;
        }

        /* 8c. Notify the page manager for this completed frame. */
        pg_EDE0_gen(pSubObj + 96, (char)subFlag);
    }

    /* 9. Increment the frame-render counter. */
    g_frameRenderCounter++;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * rage_render_scene  @ 0x822C0B18 | size: 0x130 (304 bytes)
 *
 * Scene render controller — wraps rage_render_default with an optional
 * pgStreamer profiling pass and a post-render stream flush.
 *
 * Object layout (pThis):
 *   +0x00  vtable*
 *   +0x04  uint8_t  m_bInitialized   — must be 1; abort if 0
 *   +0x07  uint8_t  m_bSkipRender    — when 1, skip this frame entirely
 *   +0x14  void*    m_pStream        — stream handle for pg_6C40_g
 *   +0x18  void*    m_pPgObj         — pgStreamer object for profiling
 * ═══════════════════════════════════════════════════════════════════════════ */
typedef struct {
    void*    vtable;          /* +0x00 */
    uint8_t  m_bInitialized;  /* +0x04 — must be 1 before rendering */
    uint8_t  _pad0[2];
    uint8_t  m_bSkipRender;   /* +0x07 — skip this frame when set */
    uint8_t  _pad1[12];
    void*    m_pStream;       /* +0x14 — stream handle for pg_6C40_g */
    void*    m_pPgObj;        /* +0x18 — pgStreamer profiling object */
} rageSceneRenderCtx;

void rage_render_scene(rageSceneRenderCtx* pThis)
{
    /* 1. Initialization guard. */
    if (!pThis->m_bInitialized) {
        nop_8240E6D0(k_sceneRenderErr);
        return;
    }

    /* 2. Skip-render gate (set externally to pause rendering). */
    if (pThis->m_bSkipRender) {
        return;
    }

    /* 3. Capture render-start timestamp for optional profiling. */
    uint64_t tStart = __mftb();

    /* 4. pgStreamer profiling bracket.
     *    The assembly calls pg_C3B8_g twice: once with mode=0 (start) and,
     *    if the result indicates failure (cntlzw idiom returns 0), again
     *    with mode=-1 (cancel).  g_bProfilingActive is the shared gate. */
    if (!g_bProfilingActive) {
        pg_C3B8_g(pThis->m_pPgObj, 0);  /* attempt to start profiling */
        if (!g_bProfilingActive) {
            /* Profiling start failed or gate cleared — cancel the bracket. */
            pg_C3B8_g(pThis->m_pPgObj, -1);
        }
    }

    /* 5. If profiling is active: compute elapsed time and log it.
     *    Two back-to-back mftb reads capture start and current timebase. */
    if (g_bProfilingActive) {
        uint64_t tNow1 = __mftb();
        uint64_t tNow2 = __mftb();

        float elapsed_a = (float)(tNow1 - tStart) * g_renderTicksToSecondsB;
        float elapsed_b = (float)(tNow2 - tStart) * g_renderTicksToSecondsA;

        /* Log timing (no-op in release builds). */
        nop_8240E6D0(k_sceneRenderTimingFmt);
        (void)elapsed_a;
        (void)elapsed_b;
    }

    /* 6. ═══ FIRST DRAW CALL ═══
     *    Performs beginScene, clear, and page-manager notification. */
    rage_render_default();

    /* 7. Clear the profiling flag now that the GPU work is submitted. */
    if (g_bProfilingActive) {
        g_bProfilingActive = 0;
    }

    /* 8. Finalise / flush the render stream. */
    pg_6C40_g(pThis->m_pStream);
}
