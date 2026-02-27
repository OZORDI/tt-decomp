/**
 * rage/render_loop.c — gameLoop per-frame phase methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers the complete set of per-frame gameLoop phase functions that live in
 * the binary range 0x82229760–0x82229F3C:
 *
 *   gameLoop_StepFrame_9760  @ 0x82229760 — thunk → gmLogic::StepFrame
 *   gameLoop_Update_9768     @ 0x82229768 — per-frame logic update
 *   gameLoop_Tick_97D8       @ 0x822297D8 — physics & AI tick
 *   gameLoop_PreRender_9938  @ 0x82229938 — pre-render state-machine dispatch
 *   gameLoop_BeginRender_9950@ 0x82229950 — begins render, times the pass
 *   gameLoop_Render_9A58     @ 0x82229A58 — main scene render dispatch
 *   gameLoop_EndRender_9AF8  @ 0x82229AF8 — finishes render, flips flags
 *   gameLoop_PostRender_9BA0 @ 0x82229BA0 — FPS overlay, aspect-ratio adjust
 *   gameLoop_Present_9F30    @ 0x82229F30 — marks frame as presented
 *
 * The gameLoop object layout (offsets confirmed from scaffolds):
 *   +0x000  vtable*
 *   +0x018  hsmContext  m_stateMachine  (embedded — NOT a pointer)
 *   +0x038  float  m_fFrameTimeA
 *   +0x03C  float  m_fFrameTimeB
 *   +0x040  float  m_fFrameTimeC
 *   +0x044  float  m_fFrameTimeD
 *   +0x1E9  uint8  m_bUpdateEnabled    (bool)
 *   +0x1EA  uint8  m_bWidescreen       (bool)
 *   +0x1EB  uint8  m_bPostRenderOn     (bool)
 *   +0x1EC  uint8  m_bFullscreenMode   (bool)
 *   +0x1EF  uint8  m_bSkipRender       (bool)
 *   +0x1F0  int32  m_nPendingUpdates
 *   +0x1F4  float  m_fTargetFPS
 *   +0x1FC  uint32 m_fiberHandle
 *   +0x208  uint32 m_nScreenWidth
 *   +0x20C  float  m_fLastSceneTime
 *   +0x210  uint8  m_bFramePresented   (bool)
 *
 * Note: several functions dispatch via the hsmContext embedded at +0x18.
 * Slot numbers in comments are relative to *that object's* vtable.
 */

#include "rage/rage_system.hpp"
#include <stdint.h>
#include <stdio.h>   /* _snprintf */

/* ── Forward declarations ─────────────────────────────────────────────────── */

/* Physics world singleton pointer @ 0x8271A358 */
extern void* g_pPhysicsWorld;

/* Root game-object pointer @ 0x825EB988 */
extern void* g_pRootGameObj;

/* HUD overlay object pointer (8-byte descriptor) @ 0x825EBAA0 */
extern void* g_pHudOverlay;

/* Camera singleton pointer @ 0x825EBC3C */
extern void* g_pCamera;

/* Render timing struct A — 92 bytes, +64 = render-pass enum, +76 = dirty flag.
 * lbl_825EBBD8 */
extern uint8_t g_renderTimingA[92];

/* Render timing struct B — 88 bytes, +64 = render-pass enum, +76 = dirty flag.
 * lbl_825EBB80 */
extern uint8_t g_renderTimingB[88];

/* Render state flags bitmask — bit 16/19 = widescreen toggles.
 * lbl_825EBB1C */
extern uint32_t g_renderStateFlags;

/* GRC render config block — 88 bytes, +64 = render-pass enum.
 * lbl_825C8F58 */
extern uint8_t g_grcRenderConfig[88];

/* 64-bit timebase start timestamp saved at BeginRender.
 * lbl_82619BB0 */
extern uint64_t g_renderBeginTimestamp;

/* Per-frame render time in seconds (float), SDA offset +25948.
 * lbl_8260655C */
extern float g_renderFrameTime;

/* Restart-pending flag — set by some external system when a game restart
 * is requested; cleared once acknowledged in gameLoop_Tick.
 * lbl_825EB9ED */
extern uint8_t g_bRestartPending;

/* Ticks-to-seconds conversion constant (float rdata).
 * Loaded from lbl_8202D108+100 in BeginRender. */
extern const float g_ticksToSeconds;

/* FPS display format string (rdata). */
extern const char k_fpsFmtStr[];

/* Standard aspect scale constant (rdata). */
extern const float k_standardAspectScale;

/* Network client per-frame tick @ 0x8221EFB8 */
extern void SinglesNetworkClient_EFB8_g(void);

/* Physics world addref wrapper @ 0x8222AB48 */
extern void atSingleton_AB48_fw(void* pObj);

/* Physics material manager per-frame update @ 0x8222AE20 */
extern void phMaterialMgrImpl_AE20_p46(void* pObj);

/* HSM set-next-state @ 0x82222800 */
extern void hsmContext_SetNextState_2800(void* pHsm, int32_t stateIdx);

/* Xbox 360 video-mode query @ 0x82585F0C */
extern void XGetVideoMode(void* pMode);

/* gmLogic step-frame dispatch @ 0x82222A78 */
extern void gmLogic_vfn_3(void* pLogic);

/* ── XVIDEO_MODE layout (Xbox 360 SDK, fields used here only) ─────────────── */
typedef struct XVideoMode {
    uint32_t  dwDisplayWidth;   /* +0 */
    uint32_t  dwDisplayHeight;  /* +4 */
    uint32_t  fIsHiDef;         /* +8  (BOOL) */
    uint8_t   _pad[116];        /* rest of 128-byte struct */
} XVideoMode;

/* ── hsmContext (embedded state machine at gameLoop+0x18) ─────────────────── */
typedef struct hsmContext {
    void** vtable;   /* +0 */
    /* remainder varies; we only need to pass this* for dispatch */
} hsmContext;

/* ── gameLoop struct (partial layout) ────────────────────────────────────── */
typedef struct gameLoop {
    void**      vtable;             /* +0x000 */
    uint8_t     _pad0[20];
    hsmContext  m_stateMachine;     /* +0x018 embedded (24 bytes min) */
    float       m_fFrameTimeA;      /* +0x038 */
    float       m_fFrameTimeB;      /* +0x03C */
    float       m_fFrameTimeC;      /* +0x040 */
    float       m_fFrameTimeD;      /* +0x044 */
    uint8_t     _pad1[437];
    uint8_t     m_bUpdateEnabled;   /* +0x1E9 (489) */
    uint8_t     m_bWidescreen;      /* +0x1EA (490) */
    uint8_t     m_bPostRenderOn;    /* +0x1EB (491) */
    uint8_t     m_bFullscreenMode;  /* +0x1EC (492) */
    uint8_t     _pad2[2];
    uint8_t     m_bSkipRender;      /* +0x1EF (495) */
    int32_t     m_nPendingUpdates;  /* +0x1F0 (496) */
    float       m_fTargetFPS;       /* +0x1F4 (500) */
    uint8_t     _pad3[3];
    uint32_t    m_fiberHandle;      /* +0x1FC (508) */
    uint8_t     _pad4[11];
    uint32_t    m_nScreenWidth;     /* +0x208 (520) */
    float       m_fLastSceneTime;   /* +0x20C (524) */
    uint8_t     m_bFramePresented;  /* +0x210 (528) */
    uint8_t     _pad5[47];
    uint8_t     m_bClearColor;      /* +0x240 (576) — clear colour buffer this frame */
    uint8_t     m_bClearDepth;      /* +0x241 (577) — clear depth buffer this frame */
    uint8_t     m_bClearStencil;    /* +0x242 (578) — clear stencil buffer this frame */
} gameLoop;



/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_StepFrame_9760 @ 0x82229760 | size: 0x4
 *
 * Thunk — tail-calls gmLogic::StepFrame (vtable slot 3 of the global gmLogic
 * singleton).  The function is only 4 bytes: a single unconditional branch to
 * gmLogic_vfn_3 @ 0x82222A78.
 *
 * gmLogic_vfn_3 iterates child nodes registered in the gmLogic singleton,
 * invokes vtable slot 3 on each, then resets m_nLastFrameIndex to -1 and
 * clears the child array.
 * ═══════════════════════════════════════════════════════════════════════════ */
/* gmLogic global singleton pointer — gmLogic_vfn_3 uses this directly. */
extern void* g_pGmLogic;

void gameLoop_StepFrame(gameLoop* pLoop)
{
    (void)pLoop;
    /* The assembly is just "b gmLogic_vfn_3" — the pLoop argument is unused;
     * gmLogic_vfn_3 reads from g_pGmLogic internally. */
    gmLogic_vfn_3(g_pGmLogic);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Update_9768 @ 0x82229768 | size: 0x70
 *
 * Per-frame logic update:
 *   1. Update m_bUpdateEnabled from m_nPendingUpdates (>0 → 1, else 0).
 *   2. Dispatch Update (slot 2) to the embedded hsmContext.
 *   3. Tick the network client (SinglesNetworkClient_EFB8_g).
 *   4. Dispatch post-update virtual (slot 16 on gameLoop).
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_Update(gameLoop* pLoop)
{
    pLoop->m_bUpdateEnabled = (pLoop->m_nPendingUpdates > 0) ? 1 : 0;

    /* State machine Update — hsmContext vtable slot 2. */
    typedef void (*SmUpdateFn)(hsmContext*);
    ((SmUpdateFn)pLoop->m_stateMachine.vtable[2])(&pLoop->m_stateMachine);

    /* Per-frame network tick (Xbox Live messages, lobby). */
    SinglesNetworkClient_EFB8_g();

    /* Game-specific post-update (slot 16 on gameLoop). */
    typedef void (*PostUpdateFn)(gameLoop*);
    ((PostUpdateFn)pLoop->vtable[16])(pLoop);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Tick_97D8 @ 0x822297D8 | size: 0x158
 *
 * Physics + AI tick:
 *   1. Dispatch slot 7  (pre-tick) on gameLoop.
 *   2. Dispatch slot 11 (mid-tick) on gameLoop.
 *   3. Dispatch slot 19 on gameLoop.
 *   4. Acquire physics world via atSingleton_AB48_fw(g_pPhysicsWorld).
 *   5. For each physics object [0..m_nObjects] (world +44/+48 ptr/count):
 *        • slot 1 (IsActive) — if true, call slot 7 (Tick) on that object.
 *   6. Tick standalone ball object (world +60) if non-null.
 *   7. phMaterialMgrImpl_AE20_p46(world) — material manager post-tick.
 *   8. Get current HSM state (m_stateMachine.m_pStates[m_nCurState]) and
 *      call its slot 4 (state Tick).
 *   9. Dispatch slot 12 (post-tick) on gameLoop.
 *  10. If m_bSkipRender && g_bRestartPending:
 *        clear g_bRestartPending, keep m_bSkipRender=1,
 *        call hsmContext_SetNextState(state_machine, 1).
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_Tick(gameLoop* pLoop)
{
    /* Pre-tick virtuals on gameLoop. */
    typedef void (*VFn)(gameLoop*);
    ((VFn)pLoop->vtable[7])(pLoop);
    ((VFn)pLoop->vtable[11])(pLoop);
    ((VFn)pLoop->vtable[19])(pLoop);

    /* Acquire physics world reference. */
    void* pWorld = g_pPhysicsWorld;
    atSingleton_AB48_fw(pWorld);

    /* Iterate physics objects. */
    uint8_t* pWorldBytes = (uint8_t*)pWorld;
    int32_t  nObjects    = *(int32_t*)(pWorldBytes + 48);
    void**   pObjects    = *(void***)(pWorldBytes + 44);

    for (int i = 0; i <= nObjects; ++i) {
        void* pObj = pObjects[i];
        typedef uint8_t (*IsActiveFn)(void*);
        uint8_t bActive = ((IsActiveFn)(*(void**)pObj)[1])(pObj);
        if (bActive) {
            typedef void (*ObjFn)(void*);
            ((ObjFn)(*(void**)pObj)[7])(pObj);
        }
    }

    /* Tick the standalone ball object if registered. */
    void* pBall = *(void**)(pWorldBytes + 60);
    if (pBall != NULL) {
        typedef void (*ObjFn)(void*);
        ((ObjFn)(*(void**)pBall)[7])(pBall);
    }

    /* Physics material manager post-tick. */
    phMaterialMgrImpl_AE20_p46(pWorld);

    /* Current HSM state's Tick (slot 4). */
    uint8_t* pSmBytes = (uint8_t*)&pLoop->m_stateMachine;
    int32_t  curIdx   = *(int32_t*)(pSmBytes + 12);
    void**   pStates  = *(void***)(pSmBytes + 8);
    void*    pState   = pStates[curIdx];
    typedef void (*StateTickFn)(void*);
    ((StateTickFn)(*(void**)pState)[4])(pState);

    /* Post-tick virtual on gameLoop. */
    ((VFn)pLoop->vtable[12])(pLoop);

    /* Restart-pending acknowledgement. */
    if (pLoop->m_bSkipRender && g_bRestartPending) {
        g_bRestartPending = 0;
        pLoop->m_bSkipRender = 1;
        hsmContext_SetNextState_2800(&pLoop->m_stateMachine, 1);
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_PreRender_9938 @ 0x82229938 | size: 0x18
 *
 * Dispatches PreRender (slot 3) to the embedded hsmContext at gameLoop+0x18.
 * The entire function body is: addi r3,r3,24; indirect branch through slot 3.
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_PreRender(gameLoop* pLoop)
{
    hsmContext* pSm = &pLoop->m_stateMachine;
    typedef void (*PreRenderFn)(hsmContext*);
    ((PreRenderFn)pSm->vtable[3])(pSm);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_BeginRender_9950 @ 0x82229950 | size: 0x150
 *
 * Begins a render pass:
 *   1. Snapshot timebase into g_renderBeginTimestamp.
 *   2. Dispatch slot 22 (BeginRender) on gameLoop.
 *   3. For each physics object [0..m_nObjects]: IsVisible (slot 1) →
 *      if visible, BeginRender (slot 8).
 *   4. BeginRender (slot 8) the standalone ball if present.
 *   5. Dispatch slot 24 on gameLoop.
 *   6. g_renderFrameTime = (float)(tEnd - tBegin) * g_ticksToSeconds.
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_BeginRender(gameLoop* pLoop)
{
    /* Snapshot start timebase. */
    uint64_t tBegin;
    __asm__ volatile ("mftb %0" : "=r"(tBegin));
    g_renderBeginTimestamp = tBegin;

    /* BeginRender virtual on gameLoop (slot 22). */
    typedef void (*VFn)(gameLoop*);
    ((VFn)pLoop->vtable[22])(pLoop);

    /* Iterate physics objects for visibility + BeginRender. */
    void* pWorld = g_pPhysicsWorld;
    uint8_t* pWorldBytes = (uint8_t*)pWorld;
    int32_t  nObjects    = *(int32_t*)(pWorldBytes + 48);
    void**   pObjects    = *(void***)(pWorldBytes + 44);

    for (int i = 0; i <= nObjects; ++i) {
        void* pObj = pObjects[i];
        typedef uint8_t (*IsVisibleFn)(void*);
        uint8_t bVis = ((IsVisibleFn)(*(void**)pObj)[1])(pObj);
        if (bVis) {
            typedef void (*ObjFn)(void*);
            ((ObjFn)(*(void**)pObj)[8])(pObj);
        }
    }

    /* Ball object BeginRender. */
    void* pBall = *(void**)(pWorldBytes + 60);
    if (pBall != NULL) {
        typedef void (*ObjFn)(void*);
        ((ObjFn)(*(void**)pBall)[8])(pBall);
    }

    /* Post-BeginRender virtual on gameLoop (slot 24). */
    ((VFn)pLoop->vtable[24])(pLoop);

    /* Record elapsed render time in seconds. */
    uint64_t tEnd;
    __asm__ volatile ("mftb %0" : "=r"(tEnd));
    g_renderFrameTime = (float)((double)(tEnd - tBegin) * (double)g_ticksToSeconds);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Render_9A58 @ 0x82229A58 | size: 0xA0
 *
 * Main scene render:
 *   1. bForceRender = (arg bForce != 0 && m_bFullscreenMode != 0) ? 1 : 0.
 *   2. Dispatch Render (slot 4) on the hsmContext, passing bForceRender.
 *   3. Load g_renderDeltaTime float.
 *   4. Call g_pRootGameObj->m_pfnSceneRender(+444)(pRoot, dt).
 *   5. Call g_pRootGameObj->m_pfnPostSceneRender(+448)(pRoot, dt).
 *
 * The function pointers at +444 and +448 in the root game object are set
 * during Init and point to the game-specific scene and post-scene render
 * callbacks (e.g. pongGame::Render and pongGame::PostRender).
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Per-frame render delta-time (48-byte struct; first float = dt).
 * lbl_825C9C8C */
extern uint8_t g_renderDeltaTime[48];

void gameLoop_Render(gameLoop* pLoop, uint8_t bForce)
{
    /* Evaluate force-render flag. */
    uint8_t bForceRender = (bForce && pLoop->m_bFullscreenMode) ? 1 : 0;

    /* State machine Render (hsmContext slot 4). */
    hsmContext* pSm = &pLoop->m_stateMachine;
    typedef void (*SmRenderFn)(hsmContext*, uint8_t);
    ((SmRenderFn)pSm->vtable[4])(pSm, bForceRender);

    /* Current frame's render delta-time (first float in the struct). */
    float dt = *(float*)g_renderDeltaTime;

    /* Call root game object's scene-render and post-scene-render callbacks. */
    uint8_t* pRoot = (uint8_t*)g_pRootGameObj;
    typedef void (*SceneFn)(void*, float);
    ((SceneFn)(*(SceneFn*)(pRoot + 444)))(g_pRootGameObj, dt);
    ((SceneFn)(*(SceneFn*)(pRoot + 448)))(g_pRootGameObj, dt);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_EndRender_9AF8 @ 0x82229AF8 | size: 0xA8
 *
 * Finalises a render pass:
 *   1. mode = m_bWidescreen ? 1 : 2.
 *   2. Write mode to g_grcRenderConfig[+64], g_renderTimingA[+64].
 *   3. Compare mode with g_renderTimingB[+64]:
 *        equal   → clear bit 16 in g_renderStateFlags
 *        unequal → set  bit 19 in g_renderStateFlags  (oris rX,rX,8)
 *   4. Dispatch slot 26 on gameLoop.
 *   5. Dispatch slot 28 on gameLoop.
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_EndRender(gameLoop* pLoop)
{
    uint32_t mode = pLoop->m_bWidescreen ? 1u : 2u;

    /* Write render mode to GRC config blocks. */
    *(uint32_t*)(g_grcRenderConfig + 64) = mode;
    *(uint32_t*)(g_renderTimingA   + 64) = mode;

    /* Read previous mode from B block for comparison. */
    uint32_t prevMode = *(uint32_t*)(g_renderTimingB + 64);

    /* Update widescreen bit. */
    uint32_t flags = g_renderStateFlags;
    if (mode == prevMode) {
        flags &= ~(1u << 16);   /* rlwinm clears bit 16 */
    } else {
        flags |= (1u << 19);    /* oris rX,rX,8 sets bit 19 */
    }
    g_renderStateFlags = flags;

    /* EndRender virtuals. */
    typedef void (*VFn)(gameLoop*);
    ((VFn)pLoop->vtable[26])(pLoop);
    ((VFn)pLoop->vtable[28])(pLoop);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_PostRender_9BA0 @ 0x82229BA0 | size: 0x35C
 *
 * Post-render: FPS overlay, aspect-ratio management.
 * Only executed when m_bPostRenderOn (+491) is set.
 *
 * Key operations:
 *   A. FPS overlay — formats an FPS string from render timing data and
 *      passes it to the HUD overlay object (g_pHudOverlay).
 *   B. Aspect ratio — polls XGetVideoMode and updates the camera's
 *      m_fAspectRatio (g_pCamera[+36]) based on hi-def vs standard output.
 *   C. Dirty-flag processing — resets g_renderTimingA/B dirty flags and
 *      updates g_renderStateFlags accordingly.
 *
 * NOTE: The inner aspect-ratio clamping loop at 0x82229D54–0x82229F08 is
 * summarised here. See scaffold at tt-decomp_recomp.9.cpp for the full
 * float-comparison branch tree.
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_PostRender(gameLoop* pLoop)
{
    if (!pLoop->m_bPostRenderOn) {
        return;
    }

    /* ── A. FPS overlay ──────────────────────────────────────────────────── */
    {
        char fpsBuf[128];

        /* Compute frame timings. */
        float ticksPerFrame = *(float*)(g_renderTimingA + 100);
        float frameTimeSec  = pLoop->m_fFrameTimeA + g_renderFrameTime;

        /* HUD projected dimensions (pixels, loaded as floats). */
        float projW = 0.0f, projH = 0.0f;
        if (g_pHudOverlay != NULL) {
            uint8_t* pHud = (uint8_t*)g_pHudOverlay;
            projW = (float)(*(int32_t*)(pHud + 8));
            projH = (float)(*(int32_t*)(pHud + 12));
        }

        float frameMs = frameTimeSec - pLoop->m_fLastSceneTime;
        float divisor = (pLoop->m_fFrameTimeA > 0.0f)
                            ? ticksPerFrame / pLoop->m_fFrameTimeA
                            : 0.0f;
        _snprintf(fpsBuf, sizeof(fpsBuf), k_fpsFmtStr,
                  (double)frameMs, (double)divisor,
                  (double)pLoop->m_fLastSceneTime,
                  (int32_t)projW, (int32_t)projH);

        /* TODO @ 0x82229CD4: pass fpsBuf to HUD rendering system.
         * Assembly calls into HUD object at g_pHudOverlay[+816] etc. */
        (void)fpsBuf;
    }

    /* ── C. Dirty-flag processing ────────────────────────────────────────── */
    {
        uint32_t flags = g_renderStateFlags;

        if (*(uint32_t*)(g_renderTimingA + 76)) {
            *(uint32_t*)(g_renderTimingA + 76) = 0;
            flags |= (1u << 19);   /* oris: set bit 19 */
        }

        if (*(uint32_t*)(g_renderTimingB + 76)) {
            *(uint32_t*)(g_renderTimingB + 76) = 0;
            flags &= ~(1u << 19);  /* rlwinm: clear bit 19 */
        }

        g_renderStateFlags = flags;
    }

    /* ── B. Aspect-ratio management ─────────────────────────────────────── */
    {
        XVideoMode vm;
        XGetVideoMode(&vm);

        /* If not hi-def, scale camera aspect ratio from standard resolution. */
        if (!vm.fIsHiDef && g_pCamera != NULL) {
            uint8_t* pCam = (uint8_t*)g_pCamera;
            float camAR   = *(float*)(pCam + 36);
            camAR = camAR * k_standardAspectScale;
            *(float*)(pCam + 36) = camAR;
        }

        /* Second query for aspect-ratio divisor.
         * TODO @ 0x82229D54: full aspect-ratio clamping branch tree (~100 lines).
         * Summary: compares m_fFrameTimeC/D against computed resolution divisors
         * and adjusts g_pCamera[+40] (FOV/aspect clamp). */
        XGetVideoMode(&vm);
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Present_9F30 @ 0x82229F30 | size: 0x10
 *
 * Clears m_bFramePresented (+528) to 0, marking that this frame has been
 * committed to the GPU.  Called once per game-loop iteration, after the
 * frame buffer has been swapped / presented.
 * ═══════════════════════════════════════════════════════════════════════════ */
void gameLoop_Present(gameLoop* pLoop)
{
    pLoop->m_bFramePresented = 0;
}
