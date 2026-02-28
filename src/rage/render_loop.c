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
 *   +0x208  float  m_fRefreshRate
 *   +0x20C  float  m_fLastSceneTime
 *   +0x210  uint8  m_bFramePresented   (bool)
 *
 * Note: several functions dispatch via the hsmContext embedded at +0x18.
 * Slot numbers in comments are relative to *that object's* vtable.
 */

#include "rage/rage_system.hpp"
#include <stdint.h>
#include "rage/grc_setup_types.h"  /* __mftb() */
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

/* ── XVIDEO_MODE layout (Xbox 360 SDK) ────────────────────────────────────── */
typedef struct XVideoMode {
    uint32_t  dwDisplayWidth;   /* +0x00 */
    uint32_t  dwDisplayHeight;  /* +0x04 */
    uint32_t  fIsInterlaced;    /* +0x08 (BOOL) */
    uint32_t  fIsWideScreen;    /* +0x0C (BOOL) */
    uint32_t  fIsHiDef;         /* +0x10 (BOOL) */
    float     fRefreshRate;     /* +0x14 */
    uint32_t  dwVideoStandard;  /* +0x18 */
    uint8_t   _pad[100];        /* +0x1C..0x7F — rest of 128-byte struct */
} XVideoMode;

#include "rage/game_loop_types.h"



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
        uint8_t bActive = ((IsActiveFn)((void**)(*(void**)pObj))[1])(pObj);
        if (bActive) {
            typedef void (*ObjFn)(void*);
            ((ObjFn)((void**)(*(void**)pObj))[7])(pObj);
        }
    }

    /* Tick the standalone ball object if registered. */
    void* pBall = *(void**)(pWorldBytes + 60);
    if (pBall != NULL) {
        typedef void (*ObjFn)(void*);
        ((ObjFn)((void**)(*(void**)pBall))[7])(pBall);
    }

    /* Physics material manager post-tick. */
    phMaterialMgrImpl_AE20_p46(pWorld);

    /* Current HSM state's Tick (slot 4). */
    uint8_t* pSmBytes = (uint8_t*)&pLoop->m_stateMachine;
    int32_t  curIdx   = *(int32_t*)(pSmBytes + 12);
    void**   pStates  = *(void***)(pSmBytes + 8);
    void*    pState   = pStates[curIdx];
    typedef void (*StateTickFn)(void*);
    ((StateTickFn)((void**)(*(void**)pState))[4])(pState);

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
    uint64_t tBegin = __mftb();
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
        uint8_t bVis = ((IsVisibleFn)((void**)(*(void**)pObj))[1])(pObj);
        if (bVis) {
            typedef void (*ObjFn)(void*);
            ((ObjFn)((void**)(*(void**)pObj))[8])(pObj);
        }
    }

    /* Ball object BeginRender. */
    void* pBall = *(void**)(pWorldBytes + 60);
    if (pBall != NULL) {
        typedef void (*ObjFn)(void*);
        ((ObjFn)((void**)(*(void**)pBall))[8])(pBall);
    }

    /* Post-BeginRender virtual on gameLoop (slot 24). */
    ((VFn)pLoop->vtable[24])(pLoop);

    /* Record elapsed render time in seconds. */
    uint64_t tEnd = __mftb();
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
        snprintf(fpsBuf, sizeof(fpsBuf), k_fpsFmtStr,
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


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_init_C2D0 @ 0x822BC2D0 | size: 0x108
 *
 * Constructor — initialises a gameLoop object to default state.
 *   1. Set parent vtable (lbl_8204E9AC), zero base fields.
 *   2. Initialise hsmContext at +24 via rage_game_obj_init_CB60,
 *      set its vtable to lbl_8204E984.
 *   3. Load default FPS from rdata (lbl_8202D108) → m_fTargetFPS, m_fDefaultFPS.
 *   4. Load initial scene-time from rdata (lbl_8202D110) → m_fLastSceneTime,
 *      m_fUnknown544.
 *   5. Load initial refresh rate float from rdata (lbl_825C4948) → m_fRefreshRate.
 *   6. Zero all flags 488–495, set m_bFullscreenMode=1, m_bFramePresented=1.
 *   7. Overwrite vtable with derived vtable (lbl_82059154).
 *   8. Zero all state fields 532–584, clear flags 576–582.
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Parent / derived vtables for gameLoop (rdata labels). */
extern void* gameLoop_vtable_parent;   /* lbl_8204E9AC */
extern void* gameLoop_vtable_derived;  /* lbl_82059154 */
extern void* hsmContext_vtable_init;   /* lbl_8204E984 */

/* Embedded state-machine initialiser @ 0x8215CB60 */
extern void rage_game_obj_init_CB60(hsmContext* pHsm);

/* Float constants in .rdata */
extern const float k_defaultFPS;       /* lbl_8202D108 */
extern const float k_initSceneTime;    /* lbl_8202D110 */
extern const float k_initRefreshRate;  /* lbl_825C4948 */

void gameLoop_init_C2D0(gameLoop* pLoop)
{
    /* Set parent vtable first (overwritten with derived at end). */
    pLoop->vtable = &gameLoop_vtable_parent;

    /* Zero base fields at +4..+8, set +12 and +16 to -1. */
    pLoop->_unk004 = 0;
    pLoop->_unk008 = 0;
    pLoop->_unk00C = -1;
    pLoop->_unk010 = -1;

    /* Initialise embedded hsmContext, then set its vtable. */
    rage_game_obj_init_CB60(&pLoop->m_stateMachine);
    pLoop->m_stateMachine.vtable = &hsmContext_vtable_init;

    /* Store initial refresh rate float. */
    pLoop->m_fRefreshRate = k_initRefreshRate;

    /* Zero all boolean flags 488–495. */
    pLoop->m_bUnknown488    = 0;
    pLoop->m_bUpdateEnabled = 0;
    pLoop->m_bWidescreen    = 0;
    pLoop->m_bPostRenderOn  = 0;
    pLoop->_pad2[0]         = 0;  /* +493 */
    pLoop->_pad2[1]         = 0;  /* +494 */
    pLoop->m_bSkipRender    = 0;

    /* Load float constants from rdata. */
    pLoop->m_fTargetFPS     = k_defaultFPS;
    pLoop->m_fDefaultFPS    = k_defaultFPS;
    pLoop->m_fLastSceneTime = k_initSceneTime;
    pLoop->m_fUnknown544    = k_initSceneTime;

    /* Integer fields. */
    pLoop->m_nPendingUpdates = 0;
    pLoop->m_nUnknown504     = -1;
    pLoop->m_fiberHandle     = 0;
    pLoop->m_nDisplayModeIdx = -1;

    /* Set m_bFullscreenMode and m_bFramePresented to 1. */
    pLoop->m_bFullscreenMode = 1;
    pLoop->m_bFramePresented = 1;

    /* Zero state fields 532–568 and clear flags 576–584. */
    pLoop->m_nDisplayStateA = 0;
    pLoop->m_nDisplayStateB = 0;
    pLoop->m_nConfigParam540 = 0;
    pLoop->m_nUnknown548    = 0;
    pLoop->m_nUnknown552    = 0;
    pLoop->m_nUnknown556    = 0;
    pLoop->m_nUnknown560    = 0;
    pLoop->m_nUnknown564    = 0;
    pLoop->m_bUnknown568    = 0;
    pLoop->m_bClearColor    = 0;
    pLoop->m_bClearDepth    = 0;
    pLoop->m_bClearStencil  = 0;
    pLoop->m_bClear3        = 0;
    pLoop->m_bClear4        = 0;
    pLoop->m_bClear5        = 0;
    pLoop->m_bClear6        = 0;
    pLoop->m_nUnknown584    = 0;

    /* Overwrite vtable with derived. */
    pLoop->vtable = &gameLoop_vtable_derived;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Shutdown_94B8 @ 0x822294B8 | size: 0x17C
 *
 * Teardown — destroys subsystems in reverse init order:
 *   1. Destroy display device via VCALL slot 2 on g_display_obj_ptr.
 *   2. Destroy audio object at g_audio_obj_ptr (SDA+25444):
 *        if refcount non-zero → VCALL slot 0 with arg 3,
 *        else → rage_free.
 *   3. Null SDA audio globals (+25444, +25436), load +25448,
 *      null +25440, free loaded +25448 ptr, null +25448.
 *   5. audSystem_shutdown_29C0 — full audio system teardown.
 *   6. Destroy physics world at lbl_8271A358 via VCALL slot 0.
 *   7. Shutdown hsmContext at +24 via VCALL slot 6.
 *   8. Conditional grcDevice shutdown (FA58 or alt 40D0).
 *   9. Destroy net system at lbl_8271A374 → netSystem_shutdown + free.
 *  10. Walk g_threadpool_head linked list calling rage_threadpool_cleanup.
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Display device singleton pointer @ 0x826066E4 */
extern void* g_display_obj_ptr;

/* Audio globals (SDA offsets). */
extern void* g_audio_obj_ptr;       /* SDA+25444 @ 0x82606364 */
extern void* g_audio_unknown_25436; /* SDA+25436 @ 0x8260635C */
extern void* g_audio_unknown_25440; /* SDA+25440 @ 0x82606360 */
extern void* g_audio_config_ptr;    /* SDA+25448 @ 0x82606368 */

/* Audio system shutdown @ 0x822229C0 */
extern void audSystem_shutdown_29C0(gameLoop* pLoop);

/* GRC device shutdown variants. */
extern void grcDevice_shutdown_FA58(void);
extern void grcDevice_shutdown_alt_40D0(void);

/* GRC device shutdown condition flag @ 0x825CA8B8 (check +4 offset). */
extern uint8_t g_grcDeviceState[8];  /* lbl_825CA8B8 */

/* Net system singleton pointer @ 0x8271A374 */
extern void* g_pNetSystem;

/* Net system shutdown @ 0x8234B510 */
extern void netSystem_shutdown_B510(void* pNetSys);

/* Thread pool linked list head @ 0x825EBCBC */
extern void* g_threadpool_head;

/* Thread pool cleanup per-node @ 0x82176878 */
extern void rage_threadpool_cleanup_6878(void* pNode);

/* Heap free @ 0x820C00C0 */
extern void rage_free(void* ptr);

void gameLoop_Shutdown_94B8(gameLoop* pLoop)
{
    /* 1. Destroy display device (VCALL slot 2 — no null check in scaffold). */
    {
        typedef void (*DestroyFn)(void*);
        void** vt = *(void***)g_display_obj_ptr;
        ((DestroyFn)vt[2])(g_display_obj_ptr);
    }

    /* 2. Destroy audio object. */
    void* pAudio = g_audio_obj_ptr;
    if (pAudio != NULL) {
        /* The audio object pointer is at SDA+25444, but the actual object
         * base is 4 bytes before it (the ptr points to +4 of the object).
         * Check refcount at base-4. */
        uint8_t* pAudioBase = (uint8_t*)pAudio - 4;
        int32_t refCount = *(int32_t*)pAudioBase;
        if (refCount != 0) {
            /* Virtual destructor (slot 0) with flags=3. */
            typedef void (*DtorFn)(void*, int32_t);
            void** vt = *(void***)pAudio;
            ((DtorFn)vt[0])(pAudio, 3);
        } else {
            rage_free(pAudioBase);
        }
    }

    /* 3. Null audio globals (order matches scaffold: 25444, 25436, load 25448,
     *    null 25440, free loaded ptr, null 25448). */
    g_audio_obj_ptr       = NULL;  /* SDA+25444 */
    g_audio_unknown_25436 = NULL;  /* SDA+25436 */
    {
        void* pCfgToFree = g_audio_config_ptr;  /* load SDA+25448 before nulling 25440 */
        g_audio_unknown_25440 = NULL;            /* SDA+25440 */
        rage_free(pCfgToFree);
    }
    g_audio_config_ptr = NULL;  /* SDA+25448 */

    /* 5. Full audio system shutdown. */
    audSystem_shutdown_29C0(pLoop);

    /* 6. Destroy physics world if present (VCALL slot 0 with arg 1). */
    if (g_pPhysicsWorld != NULL) {
        typedef void (*DtorFn)(void*, int32_t);
        void** vt = *(void***)g_pPhysicsWorld;
        ((DtorFn)vt[0])(g_pPhysicsWorld, 1);
        g_pPhysicsWorld = NULL;
    }

    /* 7. Shutdown hsmContext at +24 (VCALL slot 6). */
    {
        hsmContext* pSm = &pLoop->m_stateMachine;
        typedef void (*SmShutdownFn)(hsmContext*);
        ((SmShutdownFn)pSm->vtable[6])(pSm);
    }

    /* 8. Conditional grcDevice shutdown. */
    {
        uint32_t devState = *(uint32_t*)(g_grcDeviceState + 4);
        if (devState == 0) {
            grcDevice_shutdown_FA58();
        } else {
            grcDevice_shutdown_alt_40D0();
        }
    }

    /* 9. Destroy net system at lbl_8271A374. */
    {
        void* pNet = g_pNetSystem;
        if (pNet != NULL) {
            netSystem_shutdown_B510(pNet);
            rage_free(pNet);
        }
    }

    /* 10. Walk thread pool linked list and clean up each node.
     *     Each node has a next-pointer at +20. */
    {
        void* pNode = g_threadpool_head;
        g_pNetSystem = NULL;
        while (pNode != NULL) {
            rage_threadpool_cleanup_6878(pNode);
            pNode = *(void**)((uint8_t*)pNode + 20);
        }
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * gameLoop_Init_8F30 @ 0x82228F30 | size: 0x588
 *
 * Post-construction initialisation — called once after the constructor with
 * a config struct pointer.  Performs extensive engine subsystem setup:
 *
 *   1. Set up TLS fiber, store handle at +508.
 *   2. Copy display states from config (+8, +12) to gameLoop (+532, +536).
 *   3. Initialise hsmContext with display modes via fiAsciiTokenizer_CE30_w.
 *   4. Validate target FPS from config (+16) against bounds, store at +500.
 *   5. Allocate net system singleton (24 bytes) → lbl_8271A374.
 *   6. Set default screen 1280×720, apply exe-name overrides.
 *   7. Query XGetVideoMode for hi-def / widescreen detection.
 *   8. Compute truncated refresh rate → m_fRefreshRate (+520).
 *   9. Allocate pongPostEffects → g_pPostEffects (SDA+25556).
 *  10. Create display device via rage_obj_factory → grcDisplay (84 bytes).
 *  11. Display-mode matching loop → m_nDisplayModeIdx (+512).
 *  12. Allocate physics world container (64 bytes) → g_pPhysicsWorld (0x8271A358).
 *  13. Call audSystem_configure_2930 with config.
 *  14. Store config+20 to m_nUnknown504 (+504) on success.
 *
 * Config struct layout (passed in r4):
 *   +0x00  uint32_t  nAllocCount    — allocation hint
 *   +0x08  uint32_t  nDisplayStateA — stored at gameLoop+532
 *   +0x0C  uint32_t  nDisplayStateB — stored at gameLoop+536
 *   +0x10  float     fTargetFPS     — validated and stored at +500
 *   +0x14  uint32_t  nUnknown20     — stored at +504 on success
 *   +0x1C  uint32_t  pConfigPath    — optional config file path
 *   +0x20  uint32_t  nUnknown32     — passed to xe_5BB0
 * ═══════════════════════════════════════════════════════════════════════════ */

/* TLS fiber setup (CRT) @ 0x82566B78 */
extern uint32_t _crt_tls_fiber_setup(void);

/* hsmContext display-mode init @ 0x8215CE30 */
extern void fiAsciiTokenizer_CE30_w(hsmContext* pHsm, uint32_t stateA, uint32_t stateB);

/* FPS bound constants in .rdata. */
extern const float k_fpsLowerBound;     /* lbl_8202D110 (same as k_initSceneTime) */
extern const float k_fpsUpperBound;     /* lbl_82079BE0 */

/* Subsystem init helpers. */
extern void rage_SetRenderMode(int32_t nFPS, int32_t flag);  /* @ 0x8214F9A8 */
extern void xe_main_thread_init_0038(void);
extern void rage_get_exe_name_6628(const char* pKey, uint32_t* pOut);
extern void rage_CEF0(hsmContext* pHsm);

/* Net system / singleton init @ 0x8234B618 */
extern void atSingleton_B618_h(void);

/* Allocator — accessed via global allocator pointer chain.
 * The pattern is: lwz r3, 0(r13); lwzx r3, r25, r24; VCALL slot 1.
 * Slot 1 = allocate(size, alignment). */
extern void* g_pAllocator;  /* SDA+0 @ 0x82600000 — points to allocator table */

/* pongPostEffects constructor @ 0x8213F160 */
extern void* pongPostEffects_ctor_F160(void* pMem);

/* Post-effects singleton @ 0x826063D4 */
extern void* g_pPostEffects;

/* Render config check for post-render enable @ 0x825CB720 (+4). */
extern uint8_t g_postRenderConfig[8]; /* lbl_825CB720 */

/* Object factory creation @ 0x822E3040 / finalise @ 0x822E3B38 */
extern void* rage_obj_factory_create_3040(void* pFactory, void* pVtable,
                                          void* pParam, int bFlag1, int bFlag2);
extern void rage_obj_finalize_3B38(void* pObj);

/* Warning-log no-op @ 0x8240E6D0 */
extern void nop_8240E6D0(const char* pFmt, ...);

/* Display device creation @ 0x82243E20 */
extern void grcDisplay_create_3E20(void* pDisplay);

/* Display device vtable (rdata). */
extern void* grcDisplay_vtable;   /* lbl_8204EAEC */
extern void* grcDisplay_vtable2;  /* lbl_82027C50 — secondary vtable at +16 */

/* Screen dimension globals @ 0x825C9014 / 0x825C9018 */
extern uint32_t g_nScreenWidth;
extern uint32_t g_nScreenHeight;

/* Display mode table @ 0x825CB75C — array of 12-byte entries. */
extern uint8_t g_displayModeTable[];  /* lbl_825CB75C */

/* Display mode table base @ 0x825CB75C - 8 (entries start at +8). */
extern uint8_t g_displayModeBase[];   /* lbl_825CB75C */

/* Tolerance constant for display-mode matching @ 0x8202D168 */
extern const float k_displayModeTolerance;

/* Aspect ratio floats for hi-def vs standard displays. */
extern const float k_aspectHiDef;      /* lbl_82079F9C */
extern const float k_aspectStandard;   /* lbl_82079BD0 */

/* Refresh rate offset constant @ 0x8202D10C */
extern const float k_refreshRateOffset; /* lbl_8202D10C */

/* Non-widescreen flag @ 0x825C9021 */
extern uint8_t g_bNonWidescreen;

/* Misc init helpers. */
/* Rendering lifecycle management functions */
extern void InitializeRenderConfig(void);           /* rage_F400 @ 0x8214F400 */
extern void SetupRenderFiber(void* pObj);           /* rage_AD98 @ 0x8235AD98 */
extern void CleanupRenderTargets(void* pObj);       /* rage_66F0 @ 0x823666F0 */
extern int  ConfigureRenderTargets(void* pObj, void* pCfg); /* rage_6530 @ 0x82366530 */
extern void xe_5BB0(uint32_t val);

/* Init-complete flag @ 0x825EE296 */
extern uint8_t g_bInitComplete;

/* Zero global @ 0x825EB188 */
extern uint32_t g_initStateFlag;

/* Object factory base @ 0x825D0080 */
extern uint8_t g_objFactory[];

/* Factory create params (rdata). */
extern void* k_factoryParam1;     /* lbl_8203AB50 */
extern void* k_factoryVtable;     /* lbl_8204E8CC */

/* Physics world container vtable @ 0x8204EADC */
extern void* physicsWorld_vtable;

/* Callback setup for config file. */
extern void nt_0420(void* pPath, const char* pStr);

/* Callback function pointer @ 0x82228C68 */
extern void rage_RenderDebugOverlay(void);  /* @ 0x82228C68 */

/* Callback store location @ 0x825C2AD8 */
extern void* g_configCallback;

/* Audio system configure @ 0x82222930 */
extern void audSystem_configure_2930(gameLoop* pLoop, void* pConfig);

/* Non-widescreen init config @ 0x825EB900. */
extern uint8_t g_nonWsConfig[];

void gameLoop_Init_8F30(gameLoop* pLoop, void* pConfig)
{
    uint8_t* pCfg = (uint8_t*)pConfig;
    hsmContext* pSm = &pLoop->m_stateMachine;

    /* 1. Set up TLS fiber, store handle. */
    pLoop->m_fiberHandle = _crt_tls_fiber_setup();

    /* 2. Copy display states from config. */
    pLoop->m_nDisplayStateB = *(uint32_t*)(pCfg + 12);
    pLoop->m_nDisplayStateA = *(uint32_t*)(pCfg + 8);

    /* 3. Initialise hsmContext with display modes. */
    fiAsciiTokenizer_CE30_w(pSm, *(uint32_t*)(pCfg + 8), *(uint32_t*)(pCfg + 12));

    /* 4. Validate target FPS from config. */
    float fTargetFPS = *(float*)(pCfg + 16);
    if (fTargetFPS < k_fpsLowerBound || fTargetFPS > k_fpsUpperBound) {
        /* FPS out of valid range — log and use default. */
        nop_8240E6D0("Invalid FPS %f", (double)fTargetFPS);
        fTargetFPS = k_defaultFPS;
    }
    pLoop->m_fTargetFPS = fTargetFPS;

    /* Convert FPS to integer for rage_SetRenderMode. */
    {
        int32_t nFPS = (int32_t)fTargetFPS;
        rage_SetRenderMode(nFPS, 1);
    }

    /* 5. Allocate net system singleton (24 bytes). */
    xe_main_thread_init_0038();
    {
        /* Allocator chain: SDA+0 → table → entry at +4 → VCALL slot 1. */
        typedef void* (*AllocFn)(void*, int32_t, int32_t);
        void* allocObj = *(void**)((uint8_t*)g_pAllocator + 4);
        void** vt = *(void***)allocObj;
        void* pNetMem = ((AllocFn)vt[1])(allocObj, 24, 16);

        if (pNetMem != NULL) {
            /* Zero-init the 24-byte net system object. */
            uint8_t* pNet = (uint8_t*)pNetMem;
            *(uint16_t*)(pNet + 6)  = 0;
            *(uint16_t*)(pNet + 4)  = 0;
            *(uint32_t*)(pNet + 0)  = 0;
            *(uint16_t*)(pNet + 14) = 0;
            *(uint16_t*)(pNet + 12) = 0;
            *(uint32_t*)(pNet + 8)  = 0;
            *(uint16_t*)(pNet + 22) = 0;
            *(uint16_t*)(pNet + 20) = 0;
            *(uint32_t*)(pNet + 16) = 0;
            atSingleton_B618_h();
            g_pNetSystem = pNetMem;
        } else {
            g_pNetSystem = NULL;
        }
    }

    /* 6. Set default screen 1280×720 and apply exe-name overrides. */
    {
        uint32_t width  = 1280;
        uint32_t height = 720;
        rage_get_exe_name_6628("ScreenWidth", &width);
        rage_get_exe_name_6628("ScreenHeight", &height);
        g_nScreenWidth  = width;
        g_nScreenHeight = height;

        /* 7. Query XGetVideoMode, compute truncated refresh rate. */
        XVideoMode vm;
        XGetVideoMode(&vm);

        /* refreshRate + offset → truncate to int → back to float (stfs). */
        float fComputed = vm.fRefreshRate + k_refreshRateOffset;
        int32_t nTruncated = (int32_t)fComputed;
        pLoop->m_fRefreshRate = (float)nTruncated;

        /* Dispatch hsmContext init completion. */
        rage_CEF0(pSm);
        typedef void (*SmInitFn)(hsmContext*);
        ((SmInitFn)pSm->vtable[1])(pSm);

        /* 7b. Check widescreen mode (XVideoMode+12 = fIsWideScreen). */
        XGetVideoMode(&vm);
        uint8_t bIsWidescreen = (vm.fIsWideScreen != 0) ? 1 : 0;

        if (!bIsWidescreen) {
            /* Non-widescreen path: set flag, configure render. */
            g_bNonWidescreen = 0;
            InitializeRenderConfig();
            void* pRoot = g_pRootGameObj;
            SetupRenderFiber(pRoot);
            CleanupRenderTargets(pRoot);
            if (ConfigureRenderTargets(pRoot, g_nonWsConfig) == 0) {
                CleanupRenderTargets(pRoot);
            }
        }
    }

    /* Store config+32 value and clear init state flag. */
    g_initStateFlag = 0;
    xe_5BB0(*(uint32_t*)(pCfg + 32));

    /* 9. Allocate pongPostEffects (512 bytes, align 16). */
    xe_main_thread_init_0038();
    {
        typedef void* (*AllocFn)(void*, int32_t, int32_t);
        void* allocObj = *(void**)((uint8_t*)g_pAllocator + 4);
        void** vt = *(void***)allocObj;
        void* pFxMem = ((AllocFn)vt[1])(allocObj, 512, 16);

        if (pFxMem != NULL) {
            g_pPostEffects = pongPostEffects_ctor_F160(pFxMem);
        } else {
            g_pPostEffects = NULL;
        }
    }

    /* Enable post-render if render config flag is set. */
    {
        uint32_t cfgFlag = *(uint32_t*)(g_postRenderConfig + 4);
        if (cfgFlag != 0) {
            pLoop->m_bPostRenderOn = 1;
        }
    }

    /* 10. Create display device via object factory. */
    {
        void* pCreated = rage_obj_factory_create_3040(
            g_objFactory + 128,    /* factory instance */
            &k_factoryVtable,      /* vtable param */
            &k_factoryParam1,      /* creation param */
            1, 1);
        if (pCreated != NULL) {
            rage_obj_finalize_3B38(pCreated);
        } else {
            nop_8240E6D0("Failed to create display device");
        }
    }

    /* 11. Display-mode matching loop.
     * Walk display mode table (12-byte entries at g_displayModeBase+8),
     * find entry whose float at entry+0 is within tolerance of the
     * target aspect ratio. */
    {
        XVideoMode vm;
        XGetVideoMode(&vm);
        uint8_t bWidescreen = (vm.fIsWideScreen != 0) ? 1 : 0;
        float fTargetAspect = bWidescreen ? k_aspectHiDef : k_aspectStandard;

        int32_t matchIdx = -1;
        uint8_t* pEntries = g_displayModeBase + 8;
        int i;
        for (i = 0; i < 4; ++i) {
            float fEntry = *(float*)(pEntries + i * 12);
            float fDiff  = fTargetAspect - fEntry;
            if (fDiff < 0.0f) fDiff = -fDiff;
            if (fDiff < k_displayModeTolerance) {
                /* Match found — compute display mode value.
                 * index = i*3, value = table[index] */
                int32_t idx3 = i + i * 2;  /* i * 3 */
                matchIdx = *(int32_t*)(g_displayModeBase + idx3 * 4);
                break;
            }
        }

        pLoop->m_nDisplayModeIdx = matchIdx;

        /* Dispatch gameLoop vtable slot 31 (post-display-mode-init). */
        typedef void (*VFn)(gameLoop*);
        ((VFn)pLoop->vtable[31])(pLoop);
    }

    /* Set init-complete flag. */
    g_bInitComplete = 1;

    /* 12. Allocate display device object (84 bytes, align 16). */
    xe_main_thread_init_0038();
    {
        typedef void* (*AllocFn)(void*, int32_t, int32_t);
        void* allocObj = *(void**)((uint8_t*)g_pAllocator + 4);
        void** vt = *(void***)allocObj;
        void* pDispMem = ((AllocFn)vt[1])(allocObj, 84, 16);

        if (pDispMem != NULL) {
            uint8_t* pDisp = (uint8_t*)pDispMem;
            /* Set vtable. */
            *(void**)(pDisp + 0) = &grcDisplay_vtable;
            *(uint32_t*)(pDisp + 4) = 0;
            /* Initialise grcDisplay subsystem at +8. */
            grcDisplay_create_3E20(pDisp + 8);
            /* Zero display mode fields. */
            *(uint16_t*)(pDisp + 74) = 0;
            *(uint16_t*)(pDisp + 72) = 0;
            *(uint32_t*)(pDisp + 68) = 0;
            *(uint32_t*)(pDisp + 76) = 0;
            *(uint16_t*)(pDisp + 80) = 0;
            *(uint16_t*)(pDisp + 82) = 0;
            g_display_obj_ptr = pDispMem;
        } else {
            g_display_obj_ptr = NULL;
        }

        /* Set secondary vtable at +16 of display object. */
        if (g_display_obj_ptr != NULL) {
            *(void**)((uint8_t*)g_display_obj_ptr + 16) = &grcDisplay_vtable2;
        }
    }

    /* 13. Config file callback setup (optional). */
    {
        void* pConfigPath = *(void**)(pCfg + 28);
        if (pConfigPath != NULL) {
            pLoop->m_nConfigParam540 = *(uint32_t*)(pCfg + 28);
            g_configCallback = (void*)rage_RenderDebugOverlay;
            nt_0420(pConfigPath, "GameConfig");
        }
    }

    /* 14. Allocate physics world container (64 bytes, align 16). */
    xe_main_thread_init_0038();
    {
        typedef void* (*AllocFn)(void*, int32_t, int32_t);
        void* allocObj = *(void**)((uint8_t*)g_pAllocator + 4);
        void** vt = *(void***)allocObj;
        void* pAcMem = ((AllocFn)vt[1])(allocObj, 64, 16);

        if (pAcMem != NULL) {
            uint8_t* pAc = (uint8_t*)pAcMem;
            /* Initialise physics world container fields. */
            *(uint8_t*)(pAc + 4)   = 0;
            *(uint32_t*)(pAc + 8)  = 0;
            *(uint8_t*)(pAc + 12)  = 0;
            *(uint32_t*)(pAc + 16) = 0;
            *(uint8_t*)(pAc + 20)  = 0;
            *(uint32_t*)(pAc + 24) = 0;
            *(uint8_t*)(pAc + 28)  = 0;
            *(uint32_t*)(pAc + 32) = 0;
            *(uint32_t*)(pAc + 36) = 0;
            *(uint32_t*)(pAc + 40) = 0;
            *(uint32_t*)(pAc + 44) = 0;
            *(int32_t*)(pAc + 48)  = -1;
            *(uint32_t*)(pAc + 52) = 0;
            *(uint32_t*)(pAc + 56) = 0;
            *(uint32_t*)(pAc + 60) = 0;
            *(void**)(pAc + 0)     = &physicsWorld_vtable;
            g_pPhysicsWorld = pAcMem;
        } else {
            g_pPhysicsWorld = NULL;
        }
    }

    /* Store allocation count from config+0. */
    {
        uint8_t* pAc = (uint8_t*)g_pPhysicsWorld;
        uint32_t nAllocCount = *(uint32_t*)(pCfg + 0);

        /* Clamp allocation count: if > 0x3FFFFFFF, set alloc size to -1. */
        uint32_t nAllocSize = nAllocCount << 2;  /* * 4 */
        *(uint32_t*)(pAc + 40) = nAllocCount;
        if (nAllocCount > 0x3FFFFFFF) {
            nAllocSize = (uint32_t)-1;
        }

        /* Allocate backing buffer for physics world container. */
        xe_main_thread_init_0038();
        {
            typedef void* (*AllocFn)(void*, int32_t, int32_t);
            void* allocObj2 = *(void**)((uint8_t*)g_pAllocator + 4);
            void** vt2 = *(void***)allocObj2;
            void* pBuf = ((AllocFn)vt2[1])(allocObj2, (int32_t)nAllocSize, 16);
            *(void**)(pAc + 44)    = pBuf;
            *(uint32_t*)(pAc + 52) = 0;
            *(uint32_t*)(pAc + 56) = 0;
            *(uint32_t*)(pAc + 60) = 0;
            *(int32_t*)(pAc + 48)  = -1;
            *(uint32_t*)(pAc + 36) = 0;
        }
    }

    /* 15. Configure audio system. */
    audSystem_configure_2930(pLoop, pConfig);

    /* 16. Store config+20 to m_nUnknown504 on vtable slot 6 success. */
    {
        uint32_t nVal = *(uint32_t*)(pCfg + 20);
        typedef uint8_t (*VFn6)(gameLoop*, uint32_t);
        uint8_t bOk = ((VFn6)pLoop->vtable[6])(pLoop, nVal);
        if (bOk) {
            pLoop->m_nUnknown504 = (int32_t)nVal;
        }
    }
}
