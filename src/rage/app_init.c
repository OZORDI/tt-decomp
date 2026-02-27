/**
 * rage/app_init.c — RAGE engine game-object and application initialisation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers the grcSetup class: construction (rage_game_obj_init_CB60 / Init),
 * the five immediately-following vtable methods for subsystem startup,
 * shutdown, frame-time smoothing, and frame presentation.
 *
 * grcSetup struct layout (confirmed offsets):
 *   +0x00  void**    vtable
 *   +0x04  uint32_t  flags              (0xFFFFFFFF until configured)
 *   +0x08  uint64_t  m_renderTimestamp  (mftb at ctor; updated by UpdateRenderTime)
 *   +0x10  uint64_t  m_frameTimestamp2  (second mftb capture at ctor)
 *   +0x18  uint64_t  m_frameTimestamp3  (third mftb capture at ctor)
 *   +0x20  float     m_smoothRenderTime (EMA-filtered render delta, seconds)
 *   +0x28  uint64_t  m_frameTimestamp1  (mftb updated by UpdateFrameTime)
 *   +0x30  float     m_smoothFrameTime  (EMA-filtered frame delta, seconds)
 *   +0x1CC uint8_t   m_renderState      (0=idle, 1=pending, 2=submitted)
 *   +0x10  uint64_t  m_presentTimestamp (stamped at every PresentFrame call)
 *
 * Known globals:
 *   g_pGrcRenderer   @ 0x825EBC58 — active grcRenderer instance
 *   g_pGrcRenderer2  @ 0x825EBC3C — secondary renderer reference
 *   g_pGrcDevice     @ 0x82606404 — active hardware device object
 *   g_pGameLoop      @ 0x82606400 — game-loop singleton
 *   g_kTicksPerSec   @ (0x82027108 + 2204) — float: CPU ticks-per-second scale
 *   g_kFrameSnapThresh @ (0x82027108 + 0) — float: frame-time snap threshold (s)
 *   g_kFrameAlpha    @ (0x825C4930) — float: EMA alpha for frame smoothing
 *   g_kOneMinusAlpha @ 0x8202CFE8  — float: 1 - EMA alpha
 */

#include "rage/rage_system.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ---------------------------------------------------------------------------
// External / forward declarations
// ---------------------------------------------------------------------------
extern const struct grcNameRecord g_grcDefaultNameRecord;  // @ 0x824DC690
extern void*     g_grcSetup_vtable[];                      // @ 0x825DC668
extern struct    grcFlagsObj g_grcFlagsObj;                // @ 0x825D5A24
extern const float g_kZero;

// Timing / smoothing constants
extern const float g_kTicksPerSec;       // @ (0x82027108 + 2204) — ticks → seconds scale
extern const float g_kFrameSnapThresh;   // @ (0x82027108 +    0) — deviation that triggers snap
extern const float g_kFrameAlpha;        // @ 0x825C4930          — EMA alpha weight
extern const float g_kOneMinusAlpha;     // @ 0x8202CFE8          — 1.0 - g_kFrameAlpha

// Singleton availability guards (checked before allocating subsystems)
extern struct grcSingletonSlot g_grcSubsystem1;  // @ 0x825CA90C  (lis(-32163) + -22324)
extern struct grcSingletonSlot g_grcSubsystem2;  // @ 0x825CA8B8  (lis(-32163) + -22344)

// Global subsystem pointers written by Startup / cleared by Shutdown
extern void*  g_pGrcRenderer;   // @ 0x825EBC58 — grcRenderer*
extern void*  g_pGrcRenderer2;  // @ 0x825EBC3C — secondary reference
extern void*  g_pGrcDevice;     // @ 0x82606404 — hardware device object
extern void*  g_pGameLoop;      // @ 0x82606400 — game-loop singleton
extern uint32_t g_grcReadyFlag; // @ (lis(-32160) + 26172) — 1 when grc subsystem live

// Scene resource freed during shutdown
extern void*  g_pGrcScene;      // @ (lis(-32161) + -17888)
extern void*  g_pGrcSceneB;     // @ (lis(-32161) + -17884)

// Hardware device vtable (16-byte grcDevice objects get this stamped)
extern void*  g_grcDeviceVtable[];  // @ 0x824DC5C4 (lis(-32253) + 21764)

#include "rage/memory.h"

// Subsystem calls
void  rage_InitMainThreadHeap(void);             // thread/heap init @ 0x82186038
void* sysMemAllocator_Alloc(size_t sz, size_t align, int flags);  // vtable slot 1
void  phWorld_Construct(void* mem);               // physics world ctor @ 0x8215DE00
void  grcDevice_SubInit(void);                    // grc device sub-init @ 0x821513A0
void* rage_BlockAlloc(void* target, int flags, int sz, int align, int maxSz); // block alloc @ 0x821586E8
void  datRef_Release(void* ptr);                  // decrement refcount / release @ 0x820C2E18
void  atSingleton_Remove(void* mgr);              // singleton remove @ 0x82151C70
void  rage_DebugLog(const char* name, int id);    // debug/log nop @ 0x8240E6D0
void  grcDevice_WaitFence(void);                  // grc pre-present fence @ 0x8214FC30
void  grcDevice_Present(uint32_t flags, int width, int height, float scale, bool flip, int a6); // present @ 0x8214F088


// ===========================================================================
// grcSetup::Init  (rage_game_obj_init_CB60) @ 0x8215CB60 | size: 0xB8
//
// In-place constructor. Stamps the vtable, sets all-ones flags, captures
// three consecutive timebase readings as creation timestamps, resets
// child/count/name fields, and records instance-ID seeds.
// ===========================================================================
void grcSetup_Init(struct grcSetup* self)
{
    self->vtable = g_grcSetup_vtable;
    self->flags  = 0xFFFFFFFF;

    self->m_frameTimestamp3 = __mftb();
    self->m_frameTimestamp2 = __mftb();
    self->m_renderTimestamp = __mftb();

    self->m_bActive      = 0;
    self->m_bVisible     = 0;
    self->m_bLodFlag     = 0;
    self->m_objectCount  = 1;
    self->m_pChildLink   = NULL;

    self->nameField0 = g_grcDefaultNameRecord.field0;
    self->nameField1 = g_grcDefaultNameRecord.field1;
    self->nameField2 = g_grcDefaultNameRecord.field2;
    self->nameField3 = g_grcDefaultNameRecord.field3;

    self->m_bHasFlags = (g_grcFlagsObj.pFlags != NULL) ? 1 : 0;

    self->m_boundX = g_kZero;
    self->m_boundY = g_kZero;
    self->m_boundZ = g_kZero;

    self->m_instanceId  = g_grcInstanceIdSeed;
    self->m_instanceId2 = g_grcInstanceIdSeed;
}


// ===========================================================================
// grcSetup::Startup  @ 0x8215CC18 | size: 0x148
// vtable slot 1  (NOTE: pass5 labels this "scalar_destructor" — the code
// unambiguously performs construction/startup, not destruction.)
//
// Creates the hardware device and physics subsystems, then kicks off the
// grc renderer and allocates the two frame-timing buffer globals.
//
//  1. Check whether grc subsystem 1 is live; set g_grcReadyFlag accordingly.
//  2. Check whether grc subsystem 2 is live:
//       a. Yes → allocate a 16-byte grcDevice shell, stamp g_grcDeviceVtable,
//                 store in g_pGrcDevice.
//       b. No  → allocate a 76-byte physics block, construct via ph_ctor_DE00,
//                 store in g_pGrcDevice.
//  3. Call grc_13A0 to complete grc device initialisation.
//  4. Allocate the 8-byte frame-buffer block; store pointer into both
//     g_pGrcRenderer and g_pGrcRenderer2.
// ===========================================================================
void grcSetup_Startup(void)
{
    // ── Step 1: check grc subsystem 1 availability ──────────────────────
    bool sub1Live = (g_grcSubsystem1.ptr != NULL);
    if (sub1Live)
        g_grcReadyFlag = 1;

    // ── Step 2: create device or physics object ─────────────────────────
    bool sub2Live = (g_grcSubsystem2.ptr != NULL);

    if (sub2Live) {
        // Small path: allocate a lightweight grcDevice shell (16 bytes, align 4).
        rage_InitMainThreadHeap();
        void* dev = sysMemAllocator_Alloc(/*size*/ 16, /*align*/ 4, /*flags*/ 4);
        if (dev) {
            *(void**)dev = g_grcDeviceVtable;   // stamp vtable
            g_pGrcDevice = dev;
        } else {
            g_pGrcDevice = NULL;
        }
    } else {
        // Large path: allocate a full physics-world object (76 bytes, align 4).
        rage_InitMainThreadHeap();
        void* phys = sysMemAllocator_Alloc(/*size*/ 76, /*align*/ 4, /*flags*/ 4);
        if (phys) {
            phWorld_Construct(phys);
            g_pGrcDevice = phys;
        } else {
            g_pGrcDevice = NULL;
        }
    }

    // ── Step 3: initialise grc renderer subsystem ───────────────────────
    grcDevice_SubInit();

    // ── Step 4: allocate 8-byte frame-timing buffer; store in both refs ─
    // xe_86E8(target=subsystem_base, flags=1, size=8, align=8, maxSz=127)
    void* frameBuf = rage_BlockAlloc(NULL, /*flags*/ 1, /*sz*/ 8, /*align*/ 8, /*max*/ 127);
    g_pGrcRenderer  = frameBuf;
    g_pGrcRenderer2 = frameBuf;
}


// ===========================================================================
// grcSetup::Shutdown  (grcSetup_vfn_6_CD60_1) @ 0x8215CD60 | size: 0xCC
// vtable slot 6
//
// Tears down all subsystems created by Startup, in reverse order:
//
//  1. If g_pGrcRenderer is set: release its internal resource via rage_2E18,
//     free its backing buffer (+24), then free the renderer struct itself.
//     Clear g_pGrcRenderer.
//  2. Release and clear g_pGrcScene via rage_2E18.
//  3. Release and clear g_pGrcSceneB; call atSingleton_1C70_fw to deregister
//     the scene singleton.
//  4. Release g_pGameLoop via rage_2E18.
//  5. If g_pGrcDevice is set: call its vtable slot 0 destructor.
//     Clear g_pGrcDevice.
// ===========================================================================
void grcSetup_Shutdown(void)
{
    // ── 1. Tear down renderer ────────────────────────────────────────────
    if (g_pGrcRenderer) {
        // Release the renderer's internal resource (refcount decrement / GPU sync)
        void* rendererResource = *(void**)g_pGrcRenderer;
        if (rendererResource)
            datRef_Release(rendererResource);

        // Free the backing allocation stored at renderer+24
        void* rendererBacking = *(void**)((uint8_t*)g_pGrcRenderer + 24);
        rage_free(rendererBacking);
        rage_free(g_pGrcRenderer);
        g_pGrcRenderer = NULL;
    }

    // ── 2. Release scene resource A ─────────────────────────────────────
    datRef_Release(g_pGrcScene);
    if (g_pGrcScene) {
        // Log/debug: report scene name (id at +6)
        uint16_t sceneId = *(uint16_t*)((uint8_t*)g_pGrcScene + 6);
        rage_DebugLog("grcSetup_Shutdown scene", sceneId);
    }
    g_pGrcScene = NULL;

    // ── 3. Release scene resource B + deregister singleton ──────────────
    datRef_Release(g_pGrcSceneB);
    g_pGrcSceneB = NULL;
    atSingleton_Remove(NULL);  // unregister grc scene singleton

    // ── 4. Release game-loop singleton ───────────────────────────────────
    datRef_Release(g_pGameLoop);

    // ── 5. Destroy device object via vtable destructor ───────────────────
    if (g_pGrcDevice) {
        // Call vtable slot 0 (destructor) with flag=1 (delete after dtor)
        typedef void (*VtableDtor)(void*, int);
        void** vtbl = *(void***)g_pGrcDevice;
        ((VtableDtor)vtbl[0])(g_pGrcDevice, 1);
        // NOTE: g_pGrcDevice is intentionally left pointing to the
        // (now-freed) block; callers must not use it after Shutdown.
    }
}


// ===========================================================================
// grcSetup::UpdateFrameTime  (grcSetup_vfn_2_CFD0_1) @ 0x8215CFD0 | size: 0x8C
// vtable slot 2
//
// Computes the elapsed time since the last frame and updates the
// exponentially-smoothed frame delta (m_smoothFrameTime, +40 / 0x28).
//
// Two-mode update:
//   SNAP mode  (|new - smooth| > g_kFrameSnapThresh):
//     Directly replace m_smoothFrameTime with the raw new delta,
//     and refresh both timestamp slots (+24 and +8) with fresh mftb reads.
//     Use case: first frame, or a very long hitch (e.g. loading).
//
//   SMOOTH mode (small deviation):
//     Apply exponential moving average:
//       smooth = alpha * old_smooth + (1 - alpha) * raw_delta
//     Update the frame-timing timestamps (+24 and +8).
//
// Reads:
//   +24 (m_frameTimestamp1) — last UpdateFrameTime timestamp
//   +40 (m_smoothFrameTime) — current smoothed frame time
// Writes:
//   +40 → new smoothed value
//   +24, +8 → two fresh mftb captures
// ===========================================================================
void grcSetup_UpdateFrameTime(struct grcSetup* self)
{
    uint64_t lastTick    = self->m_frameTimestamp1;   // +24
    uint64_t nowTick     = __mftb();
    float    oldSmooth   = self->m_smoothFrameTime;   // +40
    float    elapsedSec  = (float)(nowTick - lastTick) * g_kTicksPerSec;

    float deviation = elapsedSec - oldSmooth;
    if (deviation < 0.0f) deviation = -deviation;   // fabs

    if (deviation > g_kFrameSnapThresh) {
        // Large jump — snap directly to the raw measurement.
        self->m_smoothFrameTime  = elapsedSec;        // +40
        self->m_frameTimestamp1  = __mftb();           // +24
        self->m_renderTimestamp  = __mftb();           // +8
        return;
    }

    // Small drift — apply EMA: new = alpha*old + (1-alpha)*raw
    float smoothed = oldSmooth * g_kFrameAlpha + elapsedSec * g_kOneMinusAlpha;
    self->m_smoothFrameTime = smoothed;               // +40
    self->m_frameTimestamp1 = __mftb();               // +24
    self->m_renderTimestamp = __mftb();               // +8
}


// ===========================================================================
// grcSetup::UpdateRenderTime  (grcSetup_vfn_3_D060_1) @ 0x8215D060 | size: 0x6C
// vtable slot 3
//
// Identical logic to UpdateFrameTime but operates on the render-thread
// timing pair: m_renderTimestamp (+8) and m_smoothRenderTime (+32).
//
// Unlike UpdateFrameTime, a snap does NOT update the timestamp — it only
// stores the raw value. The timestamp is never written back here; the
// render-thread caller controls it directly.
//
// Reads:
//   +8  (m_renderTimestamp) — last render-thread mftb
//   +32 (m_smoothRenderTime) — current smoothed render delta
// Writes:
//   +32 → new smoothed value  (either snapped or EMA)
// ===========================================================================
void grcSetup_UpdateRenderTime(struct grcSetup* self)
{
    uint64_t lastTick   = self->m_renderTimestamp;    // +8
    uint64_t nowTick    = __mftb();
    float    oldSmooth  = self->m_smoothRenderTime;   // +32
    float    elapsedSec = (float)(nowTick - lastTick) * g_kTicksPerSec;

    float deviation = elapsedSec - oldSmooth;
    if (deviation < 0.0f) deviation = -deviation;    // fabs

    if (deviation > g_kFrameSnapThresh) {
        // Snap — direct replacement, no timestamp writeback.
        self->m_smoothRenderTime = elapsedSec;        // +32
        return;
    }

    // EMA: smooth = alpha*old + (1-alpha)*new
    float smoothed = oldSmooth * g_kFrameAlpha + elapsedSec * g_kOneMinusAlpha;
    self->m_smoothRenderTime = smoothed;              // +32
}


// ===========================================================================
// grcSetup::PresentFrame  (grcSetup_vfn_4) @ 0x8215D0D0 | size: 0x88
// vtable slot 4
//
// Submits the current frame for display.  Manages a three-state render
// state machine stored at self->m_renderState (+460 / 0x1CC):
//
//   0 = idle    — no frame in flight
//   1 = pending — grc_F088 has been called, waiting for acknowledgement
//   2 = submitted — frame already committed this tick, do not re-submit
//
// Parameters:
//   @param self     grcSetup instance
//   @param doFlip   non-zero to request a buffer flip / present
//
// Behaviour:
//   - Always calls rage_FC30 first (GPU command-buffer pre-present fence).
//   - If doFlip && state == 2: frame already committed, just stamp timestamp.
//   - If doFlip && state != 2: call grc_F088 to submit with present flags.
//       - If state transitions to 1 (pending): mark as submitted (state = 2).
//   - Always stamps m_presentTimestamp (+16) with a fresh mftb.
// ===========================================================================
void grcSetup_PresentFrame(struct grcSetup* self, bool doFlip)
{
    grcDevice_WaitFence();   // GPU fence / pre-present sync

    if (doFlip) {
        if (self->m_renderState == 2) {
            // Already submitted this tick — just record the timestamp and bail.
            self->m_presentTimestamp = __mftb();   // +16
            return;
        }

        // Submit: flags=self->flags, width=1, height=1, scale=0.0f, flip=true, a6=0
        grcDevice_Present(self->flags, /*width*/ 1, /*height*/ 1, /*scale*/ 0.0f, /*flip*/ true, /*a6*/ 0);

        if (self->m_renderState == 1) {
            // Submission acknowledged — advance state to "submitted"
            self->m_renderState = 2;
        }
    }

    self->m_presentTimestamp = __mftb();   // +16
}
