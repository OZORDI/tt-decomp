/**
 * graphics/grc_render.cpp — grcDevice GPU command submission
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the two functions that form the graphics device's per-frame
 * command submission interface:
 *
 *   grcDevice_beginScene_5E78  @ 0x82305E78 (392 bytes)
 *     — Gates scene start on multiple render-state conditions, then
 *       calls pg_5D50_g to begin the GPU pass and dispatches BeginScene
 *       + channel-flag calls to the attached grcRenderTargetXenon.
 *
 *   grcDevice_clear_9290  @ 0x82379290 (400 bytes)
 *     — Advances the render-command ring: releases the old current entry
 *       (vtable[5]), promotes the queued next entry, copies the device's
 *       viewport rectangles into it and executes it (vtable[2]), then
 *       resets the queue to defaults.
 *
 * The grcDevice instance used for beginScene lives at 0x8271A81C and has a
 * broader layout covering render targets and state flags.  The instance used
 * for the clear operation at 0x8271A320 is compact (64 bytes) and manages
 * only the ring buffer and viewport data.
 *
 * Global dependencies (all are extern; definitions in globals.c):
 *   g_loop_obj_ptr     @ 0x825EAB30 — gameLoop singleton
 *   g_pCamActionSys    @ 0x8271A344 — rage::cmSampleCamActions ptr (gate)
 *   g_pCamActionsFlags @ 0x825D07D0 — additional gate word (inside lbl_825D07CC)
 */

#include "graphics/grc_render.hpp"
#include "rage/rage_system.hpp"
#include <stdint.h>
#include <string.h>   /* memcpy */

/* ── External dependencies ────────────────────────────────────────────────── */

/* pg subsystem — begin GPU pass for this device @ 0x82305D50 */
extern void pg_5D50_g(void* pDevice);

/* pgStreamer — cancel profiling bracket (mode=-1) @ 0x8242C3B8 */
extern void pg_C3B8_g(void* pStream, int32_t mode);

/* pgStreamer — open profiling bracket (mode=1) @ 0x82566DC0 */
extern void pg_6DC0_g(void* pStream, int32_t mode);

/* gameLoop singleton pointer (gameLoop struct defined in render_loop.c) */
extern void* g_loop_obj_ptr;   /* @ 0x825EAB30 */


/*
 * g_pCamActionSys — pointer to the rage::cmSampleCamActions global instance.
 * When this pointer is non-null, the object at +24 is checked for activity;
 * a non-zero byte there indicates an active camera sample is in progress and
 * beginScene should gate on device flags.
 * SDA: lis(-32142) + (-23740) = 0x8271A344.
 */
extern void* g_pCamActionSys;     /* @ 0x8271A344 */

/*
 * g_pCamActionsFlags — secondary gate word inside lbl_825D07CC.
 * Loaded from lbl_825D07CC + 4 = 0x825D07D0.
 * Non-zero means skip beginScene entirely.
 */
extern uint32_t g_pCamActionsFlags;   /* @ 0x825D07D0 */

/*
 * g_defaultViewportRect — 16-byte default viewport rectangle.
 * Stored at lbl_8261A0C0 (.data, 16 bytes).  Copied back into both
 * viewport rect slots of grcDeviceClear after each flush.
 */
extern const uint8_t g_defaultViewportRect[16];  /* @ 0x8261A0C0 */

/* ── grcDeviceClear — compact clear-queue device (0x8271A320) ────────────── */

/*
 * Layout of the compact grcDeviceClear instance used for buffer-clear
 * command submission.  The ring buffer holds pointers to GPU command
 * entries whose vtable[5] releases the entry and vtable[2] executes it.
 *
 *   +0x00..+0x0F  m_ringBuf[4]      — up to 4 GPU command entry pointers
 *   +0x10 (16)    m_nCurrentIdx     — index into m_ringBuf of live entry, -1=none
 *   +0x14 (20)    m_nNextIdx        — index of next queued entry, -1=empty
 *   +0x18 (24)    m_viewportRect1   — float[4]: first viewport rectangle
 *   +0x28 (40)    m_viewportRect2   — float[4]: second viewport rectangle
 *   +0x38 (56)    m_bPendingWork    — uint8: 1 if a clear is pending
 */
typedef struct {
    void*    m_ringBuf[4];      /* +0x00 */
    int32_t  m_nCurrentIdx;     /* +0x10 — -1 = no live entry */
    int32_t  m_nNextIdx;        /* +0x14 — -1 = queue empty */
    float    m_viewportRect1[4];/* +0x18 */
    float    m_viewportRect2[4];/* +0x28 */
    uint8_t  m_bPendingWork;    /* +0x38 */
} grcDeviceClear;

/* ── grcDeviceBeginScene — full device instance (0x8271A81C) ─────────────── */

/*
 * Layout of the broader grcDevice instance used for beginScene.
 * Only the fields referenced by grcDevice_beginScene_5E78 are listed here.
 *
 *   +0x64 (100)   m_pRenderTarget   — grcRenderTargetXenon*
 *   +0x6C (108)   m_bColorChannel   — uint8: passed to render target vtable[11]
 *   +0x6D (109)   m_bDepthChannel   — uint8: passed to render target vtable[12]
 *   +0x70 (112)   m_bDeviceReady    — uint8: must be 1 to proceed
 *   +0x71 (113)   m_bBeginAllowed   — uint8: must be 1 when skip-render gate fires
 */
typedef struct {
    uint8_t  _pad0[100];
    void*    m_pRenderTarget;    /* +0x64 */
    uint8_t  _pad1[7];
    uint8_t  m_bColorChannel;   /* +0x6C */
    uint8_t  m_bDepthChannel;   /* +0x6D */
    uint8_t  _pad2[2];
    uint8_t  m_bDeviceReady;    /* +0x70 */
    uint8_t  m_bBeginAllowed;   /* +0x71 */
} grcDeviceBeginScene;


/* ═══════════════════════════════════════════════════════════════════════════
 * grcDevice_beginScene_5E78 @ 0x82305E78 | size: 0x188 (392 bytes)
 *
 * Gates GPU scene start on a chain of conditions, then calls pg_5D50_g to
 * begin the actual GPU pass and dispatches channel-flag calls to the render
 * target vtable.
 *
 * Conditions (in order):
 *   A. If gameLoop->m_bSkipRender == 0 AND g_pCamActionSys[24] == 0 →
 *      skip the device-flag check entirely and proceed to gate B.
 *   B. Otherwise (skip-render active or cam-action active):
 *        — m_bBeginAllowed must be 1; else return.
 *        — If gameLoop->m_bRenderPaused OR gameLoop->m_bRenderBlocked → return.
 *   C. g_pCamActionsFlags must be 0; else return (camera action in flight).
 *
 * After all gates pass:
 *   1. If gameLoop->m_pStreamObj == NULL or m_pStreamObj[4] == 0:
 *        call pg_5D50_g(this) to begin the GPU pass.
 *   2. If !m_bDeviceReady: return.
 *   3. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        cancel the profiling bracket (pg_C3B8_g(m_pStreamObj[56], -1)).
 *   4. If m_pRenderTarget != NULL:
 *        vtable[11](pRT, m_bColorChannel)
 *        vtable[12](pRT, m_bDepthChannel)
 *        vtable[8](pRT)
 *   5. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        open profiling bracket (pg_6DC0_g(m_pStreamObj[56], 1)).
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_beginScene_5E78(grcDeviceBeginScene* pDevice)
{
/* ── Gate A: skip-render / camera-action check ───────────────────────── */

    /* Load the gameLoop singleton once; use uint8_t* for field access. */
    uint8_t* pLoop = (uint8_t*)g_loop_obj_ptr;

    uint8_t bGateFired;
    if (pLoop[495] == 0) {   /* gameLoop+0x1EF = m_bSkipRender */
        /* Not in skip-render — additionally check camera-action system. */
        void* pCamSys = g_pCamActionSys;
        uint8_t camActive = (pCamSys != NULL)
                            ? ((uint8_t*)pCamSys)[24]
                            : 0;
        bGateFired = (camActive != 0) ? 1 : 0;
    } else {
        bGateFired = 1;
    }


    if (bGateFired) {
        /* ── Gate B: device-flag and render-pause checks ──────────────────── */
        if (!pDevice->m_bBeginAllowed) {
            return;
        }
        /* Either gameLoop->m_bRenderPaused or m_bRenderBlocked blocks begin. */
        uint8_t paused  = ((uint8_t*)pLoop)[493];   /* +0x1ED m_bRenderPaused  */
        uint8_t blocked = ((uint8_t*)pLoop)[494];   /* +0x1EE m_bRenderBlocked */
        if (paused || blocked) {
            return;
        }
    }

    /* ── Gate C: no camera action currently overriding rendering ─────────── */
    if (g_pCamActionsFlags != 0) {
        return;
    }

    /* ── Streaming object gate ───────────────────────────────────────────── */

    /* Reload gameLoop (a prior VCALL might have side-effects on globals). */
    pLoop = (uint8_t*)g_loop_obj_ptr;

    /* m_pStreamObj is a pgStreamer / profiling stream at gameLoop+556. */
    void* pStreamObj = *(void**)((uint8_t*)pLoop + 556);   /* +0x22C */

    uint8_t streamActive = 0;
    if (pStreamObj != NULL) {
        streamActive = ((uint8_t*)pStreamObj)[4];
    }

    if (pStreamObj == NULL || streamActive == 0) {
        /* No active stream — call pg_5D50_g to begin the GPU pass now. */
        pg_5D50_g(pDevice);
        /* Re-read gameLoop; pg_5D50_g may update global state. */
        pLoop = (uint8_t*)g_loop_obj_ptr;
    }

    /* ── Device readiness check ──────────────────────────────────────────── */
    if (!pDevice->m_bDeviceReady) {
        return;
    }

    /* Reload the stream object pointer after potential mutation. */
    pStreamObj = *(void**)((uint8_t*)pLoop + 556);
    if (pStreamObj != NULL && ((uint8_t*)pStreamObj)[4] != 0) {
        /* An active profiling bracket is open — cancel it before proceeding. */
        void* pStream = *(void**)((uint8_t*)pStreamObj + 56);
        pg_C3B8_g(pStream, -1);
        /* Re-read gameLoop after the call. */
        pLoop = (uint8_t*)g_loop_obj_ptr;
    }

    /* ── Dispatch to render target ───────────────────────────────────────── */
    void* pRT = pDevice->m_pRenderTarget;
    if (pRT != NULL) {
        /* vtable[11]: set color-channel clear flag. */
        typedef void (*SlotBoolFn)(void*, uint8_t);
        ((SlotBoolFn)(*(void***)pRT)[11])(pRT, pDevice->m_bColorChannel);

        /* vtable[12]: set depth-channel clear flag. */
        ((SlotBoolFn)(*(void***)pRT)[12])(pRT, pDevice->m_bDepthChannel);

        /* vtable[8]: finalize BeginScene on the render target. */
        typedef void (*SlotVoidFn)(void*);
        ((SlotVoidFn)(*(void***)pRT)[8])(pRT);

        /* Re-read gameLoop after the virtual dispatch. */
        pLoop = (uint8_t*)g_loop_obj_ptr;
    }

    /* ── Profiling bracket: open after render-target dispatch ────────────── */
    pStreamObj = *(void**)((uint8_t*)pLoop + 556);
    if (pStreamObj != NULL && ((uint8_t*)pStreamObj)[4] != 0) {
        void* pStream = *(void**)((uint8_t*)pStreamObj + 56);
        pg_6DC0_g(pStream, 1);
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcDevice_clear_9290 @ 0x82379290 | size: 0x190 (400 bytes)
 *
 * Advances the render-command ring buffer by one slot, copying the device's
 * current viewport rectangles into the new entry and executing it.
 *
 * Algorithm:
 *   1. Guard: if m_bPendingWork == 0 → return immediately.
 *   2. If m_nCurrentIdx != -1: call m_ringBuf[m_nCurrentIdx]->vtable[5]()
 *      to release the previously-submitted entry.
 *   3. m_nCurrentIdx = m_nNextIdx (promote the queued slot).
 *   4. If the new m_nCurrentIdx != -1:
 *        a. entry = m_ringBuf[m_nCurrentIdx]
 *        b. Copy m_viewportRect1 → entry[+16], m_viewportRect2 → entry[+32].
 *        c. Call entry->vtable[2]() to execute the clear command.
 *   5. Reset: m_nNextIdx = -1; restore both viewport rects to
 *      g_defaultViewportRect; clear m_bPendingWork.
 *
 * The intermediate stack copies in the original assembly are an artefact of
 * the PowerPC calling convention (r5/r4 must hold destination pointers before
 * a bulk memory operation); they collapse cleanly to memcpy here.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_clear(grcDeviceClear* pDevice)
{
    /* 1. Guard: nothing to do if no clear is pending. */
    if (!pDevice->m_bPendingWork) {
        return;
    }

    /* 2. Release the previous live entry if one exists. */
    if (pDevice->m_nCurrentIdx != -1) {
        void* pEntry = pDevice->m_ringBuf[pDevice->m_nCurrentIdx];
        typedef void (*ReleaseFn)(void*);
        ((ReleaseFn)(*(void***)pEntry)[5])(pEntry);
    }

    /* 3. Promote the next queued entry to current. */
    pDevice->m_nCurrentIdx = pDevice->m_nNextIdx;

    /* 4. Execute the new entry if the queue was non-empty. */
    if (pDevice->m_nCurrentIdx != -1) {
        void* pEntry = pDevice->m_ringBuf[pDevice->m_nCurrentIdx];
        uint8_t* pEntryBytes = (uint8_t*)pEntry;

        /* 4b. Copy viewport rectangles into the entry's rect slots.
         *     Source: m_viewportRect1 → entry[+16], m_viewportRect2 → entry[+32].
         *     The original assembly does this via temporary stack buffers due
         *     to the PowerPC ABI; a simple memcpy is equivalent. */
        memcpy(pEntryBytes + 16, pDevice->m_viewportRect1, sizeof(pDevice->m_viewportRect1));
        memcpy(pEntryBytes + 32, pDevice->m_viewportRect2, sizeof(pDevice->m_viewportRect2));

        /* 4c. Execute: call vtable[2] on the entry. */
        typedef void (*ExecuteFn)(void*);
        ((ExecuteFn)(*(void***)pEntry)[2])(pEntry);
    }

    /* 5. Reset queue and viewport state for the next frame. */
    pDevice->m_nNextIdx = -1;
    memcpy(pDevice->m_viewportRect1, g_defaultViewportRect, 16);
    memcpy(pDevice->m_viewportRect2, g_defaultViewportRect, 16);
    pDevice->m_bPendingWork = 0;
}
