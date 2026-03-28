/**
 * graphics/grc_render.cpp — grcDevice GPU command submission
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the two functions that form the graphics device's per-frame
 * command submission interface:
 *
 *   grcDevice_beginScene  @ 0x82305E78 (392 bytes)
 *     — Gates scene start on multiple render-state conditions, then
 *       calls grcDevice_beginGPUPass to begin the GPU pass and dispatches BeginScene
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

#if defined(TT_PLATFORM_PC)
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <stdio.h>

/* ── SDL2 / OpenGL window state (module-local) ────────────────────────────── */
static SDL_Window*   s_pWindow    = NULL;
static SDL_GLContext  s_glContext  = NULL;
static int            s_bSDLInited = 0;

/**
 * sdl_InitWindowAndGL — one-shot SDL2 + OpenGL 3.3 context creation.
 * Called on the first invocation of grcDevice_beginScene.
 */
static void sdl_InitWindowAndGL(void)
{
    if (s_bSDLInited) return;
    s_bSDLInited = 1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    s_pWindow = SDL_CreateWindow(
        "Rockstar Table Tennis",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!s_pWindow) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    s_glContext = SDL_GL_CreateContext(s_pWindow);
    if (!s_glContext) {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "glewInit failed: %s\n", glewGetErrorString(err));
        return;
    }

    SDL_GL_SetSwapInterval(1);  /* vsync */
    glViewport(0, 0, 1280, 720);
    fprintf(stdout, "[grc] SDL2 window + OpenGL 3.3 context created (1280x720)\n");
}

/**
 * sdl_PollAndSwap — pump SDL events, swap buffers.
 * Called from grcDevice_Present (in stubs_remaining.c, which is C).
 */
extern "C" void sdl_PollAndSwap(void)
{
    if (!s_pWindow) return;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            SDL_DestroyWindow(s_pWindow);
            s_pWindow = NULL;
            SDL_Quit();
            exit(0);
        }
    }

    SDL_GL_SwapWindow(s_pWindow);
}

#endif /* TT_PLATFORM_PC */

/* ── External dependencies ────────────────────────────────────────────────── */

/* pg subsystem — begin GPU pass for this device @ 0x82305D50 */
extern void grcDevice_beginGPUPass(void* pDevice);

/* pgStreamer — cancel profiling bracket (mode=-1) @ 0x8242C3B8 */
extern void pgStreamer_cancelBracket(void* pStream, int32_t mode);

/* pgStreamer — open profiling bracket (mode=1) @ 0x82566DC0 */
extern void pgStreamer_openBracket(void* pStream, int32_t mode);

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
 * Only the fields referenced by grcDevice_beginScene are listed here.
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
 * grcDevice_beginScene @ 0x82305E78 | size: 0x188 (392 bytes)
 *
 * Gates GPU scene start on a chain of conditions, then calls grcDevice_beginGPUPass to
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
 *        call grcDevice_beginGPUPass(this) to begin the GPU pass.
 *   2. If !m_bDeviceReady: return.
 *   3. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        cancel the profiling bracket (pgStreamer_cancelBracket(m_pStreamObj[56], -1)).
 *   4. If m_pRenderTarget != NULL:
 *        vtable[11](pRT, m_bColorChannel)
 *        vtable[12](pRT, m_bDepthChannel)
 *        vtable[8](pRT)
 *   5. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        open profiling bracket (pgStreamer_openBracket(m_pStreamObj[56], 1)).
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_beginScene(grcDeviceBeginScene* pDevice)
{
#if defined(TT_PLATFORM_PC)
    /* First call: create SDL2 window + OpenGL context.
     * Subsequent calls: just ensure the GL context is current. */
    sdl_InitWindowAndGL();
    if (s_pWindow && s_glContext) {
        SDL_GL_MakeCurrent(s_pWindow, s_glContext);
    }
#endif

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
        /* No active stream — call grcDevice_beginGPUPass to begin the GPU pass now. */
        grcDevice_beginGPUPass(pDevice);
        /* Re-read gameLoop; grcDevice_beginGPUPass may update global state. */
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
        pgStreamer_cancelBracket(pStream, -1);
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
        pgStreamer_openBracket(pStream, 1);
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
#if defined(TT_PLATFORM_PC)
    /* Issue an OpenGL clear regardless of the Xbox ring-buffer state. */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

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


/* ═══════════════════════════════════════════════════════════════════════════
 * grcCommandBuffer_initShaderConstants @ 0x8235CB48 | size: 0x23c (572 bytes)
 *
 * Builds GPU command packets for shader constant initialization and GPU state
 * setup. This function constructs three command packets in the GPU command
 * buffer and sets various GPU state flags.
 *
 * The function:
 *   1. Ensures buffer space is available (calls grcCommandBuffer_flushDirtyState if needed)
 *   2. Builds first packet: shader constant upload (108 bytes of data)
 *   3. Builds second packet: another shader constant (36 bytes + 12 bytes)
 *   4. Calls grcCommandBuffer_finalizePacket to finalize packet construction
 *   5. Builds third packet: GPU register writes for state setup
 *   6. Manipulates 64-bit GPU state flags at device+16
 *   7. Calls grcCommandBuffer_submitViewport to submit the commands
 *
 * Command packet format:
 *   +0: Command word (PM4 packet type and size)
 *   +4: Parameter 1
 *   +8: Parameter 2
 *   +12: Data payload (variable size)
 *
 * GPU registers written:
 *   0x2200 (8704), 0x2201 (8705), 0x2202 (8706), 0x2203 (8707)
 *   0x2104 (8452), 0x2208 (8712)
 * ═══════════════════════════════════════════════════════════════════════════ */

/* External function declarations */
extern void grcCommandBuffer_flushDirtyState(void* pDevice);
extern void* grcCommandBuffer_expand(void* pDevice);
extern void* grcCommandBuffer_finalizePacket(void* pDevice);
extern void grcCommandBuffer_submitViewport(void* pDevice);

/* Shader constant data table @ 0x8202C328 (.rdata, 440 bytes) */
extern const uint8_t g_shaderConstantData[440];

void grcCommandBuffer_initShaderConstants(void* pDevice) {
    uint8_t* device = (uint8_t*)pDevice;
    
    /* Ensure buffer has space - calls grcCommandBuffer_flushDirtyState to expand if needed */
    grcCommandBuffer_flushDirtyState(pDevice);
    
    /* Load current write pointer and buffer end */
    uintptr_t writePtr = *(uintptr_t*)(device + 0);
    uintptr_t bufferEnd = *(uintptr_t*)(device + 8);

    /* Check if we need to expand buffer */
    if (writePtr > bufferEnd) {
        writePtr = (uintptr_t)grcCommandBuffer_expand(pDevice);
    }

    /* ── Build first packet: shader constant upload (108 bytes data) ────── */

    uint32_t* pkt1 = (uint32_t*)(writePtr + 4);
    pkt1[0] = 0xC01C2B00;  /* PM4 command: shader constant load */
    pkt1[1] = 0;           /* Parameter 1 */
    pkt1[2] = 27;          /* Parameter 2 */
    
    /* Copy 108 bytes of shader constant data from table+40 */
    memcpy(&pkt1[3], &g_shaderConstantData[40], 108);
    
    /* Update write pointer (12 header + 108 data = 120 bytes) */
    writePtr = (uintptr_t)&pkt1[3] + 108;

    /* Check buffer space again */
    if (writePtr > bufferEnd) {
        writePtr = (uintptr_t)grcCommandBuffer_expand(pDevice);
    }

    /* ── Build second packet: another shader constant (36 bytes + extras) ── */

    uint32_t* pkt2 = (uint32_t*)(writePtr + 4);
    pkt2[0] = 0xC00A2B00;  /* PM4 command */
    pkt2[1] = 1;           /* Parameter 1 */
    pkt2[2] = 9;           /* Parameter 2 */
    
    /* Copy 36 bytes of shader constant data from table start */
    memcpy(&pkt2[3], &g_shaderConstantData[0], 36);
    
    /* Add three additional 32-bit values after the data */
    uint32_t* extra = (uint32_t*)((uint8_t*)&pkt2[3] + 36);
    extra[0] = 0x00012180;  /* lis 1, ori 8576 */
    extra[1] = 0x10010001;  /* lis 4097, ori 1 */
    extra[2] = 0;
    
    /* Update write pointer */
    writePtr = (uintptr_t)&extra[3];
    *(uintptr_t*)(device + 0) = writePtr;

    /* Check buffer space */
    if (writePtr > bufferEnd) {
        writePtr = (uintptr_t)grcCommandBuffer_expand(pDevice);
    }

    /* ── Finalize packet construction ────────────────────────────────────── */

    writePtr = (uintptr_t)grcCommandBuffer_finalizePacket(pDevice);

    /* ── Build third packet: GPU register writes ─────────────────────────── */

    uint32_t* pkt3 = (uint32_t*)(writePtr + 4);
    pkt3[0] = 8704;   /* Register 0x2200 */
    pkt3[1] = 0;
    pkt3[2] = 8707;   /* Register 0x2203 */
    pkt3[3] = 0;
    
    /* Manipulate 64-bit GPU state flags at device+16 */
    uint64_t* flags = (uint64_t*)(device + 16);
    
    /* Set various GPU state bits using 64-bit OR operations */
    flags[1] |= 0x0000200000000000ULL;  /* bit 45 (oris 8192 on upper 32 bits) */
    flags[2] |= 0x0000000000000080ULL;  /* bit 7 (ori 128) */
    flags[2] |= 0x0000000000000100ULL;  /* bit 8 (ori 256) */
    flags[0] |= 0x8000000000000000ULL;  /* bit 63 (rldicr 1,63,63) */
    flags[1] |= 0x0000002000000000ULL;  /* bit 37 (rldicr 1,37,63) */
    flags[1] |= 0x0000000400000000ULL;  /* bit 34 (rldicr 1,34,63) */
    flags[1] |= 0x0800000000000000ULL;  /* bit 59 (rldicr 1,59,63) */
    
    /* Continue building packet */
    pkt3[4] = 8712;   /* Register 0x2208 */
    pkt3[5] = 4;
    pkt3[6] = 8452;   /* Register 0x2104 */
    pkt3[7] = 0;
    pkt3[8] = 8706;   /* Register 0x2202 */
    pkt3[9] = 0;
    pkt3[10] = 8705;  /* Register 0x2201 */
    pkt3[11] = 0x00010001;  /* lis 1, ori 1 */
    
    /* Set more GPU state bits */
    flags[1] |= 0x0000000800000000ULL;  /* bit 35 (rldicr 1,35,63) */
    flags[1] |= 0x0000001000000000ULL;  /* bit 36 (rldicr 1,36,63) */
    
    /* Update write pointer */
    writePtr = (uintptr_t)&pkt3[12];
    *(uintptr_t*)(device + 0) = writePtr;
    
    /* ── Submit commands ──────────────────────────────────────────────────── */
    
    grcCommandBuffer_submitViewport(pDevice);
}


/* ── External dependencies for texture factory ────────────────────────────── */

/* Debug log function (no-op) @ 0x8240E6D0 */
extern void nop_8240E6D0(const char* fmt, ...);

/* Texture processing functions */
extern void grcTextureXenon_initRaw(void* pTexture, void* pDevice);  /* @ 0x8215FD68 */
extern void grcTextureXenon_initCompressed(void* pTexture, void* pDevice);  /* @ 0x8215DC00 */

/* Debug format string for invalid texture types @ 0x82035300 */
extern const char g_invalidTextureTypeMsg[];  /* @ 0x82035300 */


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureFactoryXenon_vfn_10 @ 0x8215FCB0 | size: 0xb8 (184 bytes)
 *
 * Processes an array of texture pointers, adjusting their addresses based on
 * device offset calculations and dispatching to appropriate handlers based on
 * texture type.
 *
 * This function is part of the texture factory's vtable (slot 10) and handles
 * texture relocation and initialization during resource loading.
 *
 * Parameters:
 *   pThis       - grcTextureFactoryXenon instance (r3, unused in this function)
 *   pDevice     - Device/context structure with offset tables (r4)
 *   ppTextures  - Array of texture pointers to process (r5)
 *   count       - Number of textures in the array (r6)
 *
 * Device structure layout (partial):
 *   +0x04 (4)    m_baseAddress     - Base address for offset calculations
 *   +0x4C (76)   m_strideSize      - Size of each offset table entry
 *   +0x08+ (8+)  m_offsetTable[]   - Array of offset values indexed by (index+2)*4
 *
 * Texture structure layout (partial):
 *   +0x00 (0)    m_pData           - Pointer to texture data (adjusted by this function)
 *   +0x04 (4)    m_type            - Texture type: 0=raw, 1=skip, 2=compressed, 3+=invalid
 *
 * Algorithm:
 *   For each texture in the array:
 *     1. If texture data pointer is non-null:
 *        a. Calculate offset index: (dataPtr - baseAddress) / strideSize
 *        b. Look up offset value in device table at index (offsetIndex + 2)
 *        c. Adjust texture data pointer: dataPtr += offsetTable[offsetIndex + 2]
 *     2. Process texture based on type:
 *        - Type 0: Call grcTextureXenon_initRaw (raw texture processing)
 *        - Type 1: Skip (already processed)
 *        - Type 2: Call grcTextureXenon_initCompressed (compressed texture processing)
 *        - Type 3+: Log error via nop_8240E6D0 (invalid type)
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcTextureFactoryXenon_vfn_10(
    void* pThis,
    void* pDevice,
    void** ppTextures,
    int count)
{
    (void)pThis;  /* Unused in this function */
    
    if (count <= 0) {
        return;
    }
    
    uint8_t* device = (uint8_t*)pDevice;
    
    /* Load device offset calculation parameters */
    uintptr_t baseAddress = *(uintptr_t*)(device + 4);
    uintptr_t strideSize = *(uintptr_t*)(device + 76);
    
    /* Process each texture in the array */
    for (int i = 0; i < count; i++) {
        void** pTexture = (void**)ppTextures[i];
        
        if (pTexture == NULL) {
            continue;
        }
        
        /* ── Step 1: Adjust texture data pointer if non-null ────────────── */
        
        uintptr_t dataPtr = (uintptr_t)pTexture[0];

        if (dataPtr != 0) {
            /* Calculate offset index from data pointer */
            uintptr_t offsetFromBase = dataPtr - baseAddress;
            uintptr_t offsetIndex = offsetFromBase / strideSize;

            /* Look up offset value in device table at (offsetIndex + 2) */
            uintptr_t tableIndex = (offsetIndex + 2) * 4;  /* *4 for pointer array indexing */
            uintptr_t offsetValue = *(uintptr_t*)(device + tableIndex);

            /* Adjust the texture data pointer */
            pTexture[0] = (void*)(dataPtr + offsetValue);
        }
        
        /* ── Step 2: Process texture based on type ──────────────────────── */
        
        uint8_t* textureBytes = (uint8_t*)pTexture[0];
        
        if (textureBytes == NULL) {
            continue;
        }
        
        uint8_t textureType = textureBytes[4];
        
        if (textureType == 0) {
            /* Type 0: Raw texture - call grcTextureXenon_initRaw */
            grcTextureXenon_initRaw(pTexture[0], pDevice);
        }
        else if (textureType == 1) {
            /* Type 1: Already processed - skip */
            continue;
        }
        else if (textureType == 2) {
            /* Type 2: Compressed texture - call grcTextureXenon_initCompressed */
            grcTextureXenon_initCompressed(pTexture[0], pDevice);
        }
        else {
            /* Type 3+: Invalid texture type - log error */
            nop_8240E6D0(g_invalidTextureTypeMsg);
        }
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureReferenceBase vtable accessors
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * grcTextureReferenceBase_vfn_4 @ 0x8215D6A8 | size: 0x8 (8 bytes)
 * vtable slot 4 — GetHandle(): returns m_pInternalData (+0x0C).
 */
void* grcTextureReferenceBase_vfn_4(void* pThis)
{
    uint8_t* self = (uint8_t*)pThis;
    return *(void**)(self + 12);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcRenderTargetXenon vtable accessors — dimension / filter / LOD queries
 *
 * Field layout (from scaffolds):
 *   +0x14 (20)  uint16_t  m_nWidth
 *   +0x16 (22)  uint16_t  m_nHeight
 *   +0x1C (28)  uint8_t   m_filterMode0
 *   +0x1D (29)  uint8_t   m_filterMode1
 *   +0x1E (30)  uint8_t   m_addressMode   (clamped to max 2)
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * grcRenderTargetXenon_vfn_8 @ 0x8215DD58 | size: 0x8 (8 bytes)
 * vtable slot 8 — GetWidth(): returns uint16 at +20.
 */
uint16_t grcRenderTargetXenon_vfn_8(void* pThis)
{
    uint8_t* self = (uint8_t*)pThis;
    return *(uint16_t*)(self + 20);
}

/*
 * grcRenderTargetXenon_vfn_9 @ 0x8215DD60 | size: 0x8 (8 bytes)
 * vtable slot 9 — GetHeight(): returns uint16 at +22.
 */
uint16_t grcRenderTargetXenon_vfn_9(void* pThis)
{
    uint8_t* self = (uint8_t*)pThis;
    return *(uint16_t*)(self + 22);
}

/*
 * grcRenderTargetXenon_vfn_13 @ 0x8215DD68 | size: 0xC (12 bytes)
 * vtable slot 13 — SetFilterMode(a, b): stores two uint8 at +28, +29.
 */
void grcRenderTargetXenon_vfn_13(void* pThis, uint8_t a, uint8_t b)
{
    uint8_t* self = (uint8_t*)pThis;
    self[28] = a;
    self[29] = b;
}

/*
 * grcRenderTargetXenon_vfn_14 @ 0x8215DD78 | size: 0x14 (20 bytes)
 * vtable slot 14 — GetFilterMode(pA, pB): reads two uint8 from +28, +29.
 */
void grcRenderTargetXenon_vfn_14(void* pThis, uint8_t* pA, uint8_t* pB)
{
    uint8_t* self = (uint8_t*)pThis;
    *pA = self[28];
    *pB = self[29];
}

/*
 * grcRenderTargetXenon_vfn_15 @ 0x8215DDB0 | size: 0x8 (8 bytes)
 * vtable slot 15 — GetAddressMode(): returns uint8 at +30.
 */
uint8_t grcRenderTargetXenon_vfn_15(void* pThis)
{
    uint8_t* self = (uint8_t*)pThis;
    return self[30];
}

/*
 * grcRenderTargetXenon_vfn_16 @ 0x8215DD90 | size: 0x1C (28 bytes)
 * vtable slot 16 — SetAddressMode(mode): stores mode at +30, clamped to max 2.
 */
void grcRenderTargetXenon_vfn_16(void* pThis, uint8_t mode)
{
    uint8_t* self = (uint8_t*)pThis;
    if (mode >= 2) {
        mode = 2;
    }
    self[30] = mode;
}

/*
 * grcRenderTargetXenon_vfn_18 @ 0x82151250 | size: 0x18 (24 bytes)
 * vtable slot 18 — GetLODParams(pLodDist, pLodLevel):
 *   Writes a default float constant (from .rdata @ 0x8202D110) to *pLodDist,
 *   and 0 to *pLodLevel.
 *   Render targets have no LOD support, so this returns defaults.
 */
extern const float g_grcRenderTargetDefaultLOD;  /* @ 0x8202D110 (.rdata, 4 bytes) */

void grcRenderTargetXenon_vfn_18(void* pThis, float* pLodDist, uint32_t* pLodLevel)
{
    (void)pThis;
    *pLodDist = g_grcRenderTargetDefaultLOD;
    *pLodLevel = 0;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureXenon vtable accessors — filter / LOD / mip queries
 *
 * Field layout (from scaffolds):
 *   +0x18 (24)  uint8_t   m_filterMode0
 *   +0x19 (25)  uint8_t   m_filterMode1
 *   +0x1B (27)  uint8_t   m_mipLevels
 *   +0x1C (28)  float     m_lodDistance
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * grcTextureXenon_vfn_13 @ 0x8215DDB8 | size: 0xC (12 bytes)
 * vtable slot 13 — SetFilterMode(a, b): stores two uint8 at +24, +25.
 */
void grcTextureXenon_vfn_13(void* pThis, uint8_t a, uint8_t b)
{
    uint8_t* self = (uint8_t*)pThis;
    self[24] = a;
    self[25] = b;
}

/*
 * grcTextureXenon_vfn_14 @ 0x8215DDC8 | size: 0x14 (20 bytes)
 * vtable slot 14 — GetFilterMode(pA, pB): reads two uint8 from +24, +25.
 */
void grcTextureXenon_vfn_14(void* pThis, uint8_t* pA, uint8_t* pB)
{
    uint8_t* self = (uint8_t*)pThis;
    *pA = self[24];
    *pB = self[25];
}

/*
 * grcTextureXenon_vfn_15 @ 0x8215DDE8 | size: 0x8 (8 bytes)
 * vtable slot 15 — GetMipLevels(): returns uint8 at +27.
 */
uint8_t grcTextureXenon_vfn_15(void* pThis)
{
    uint8_t* self = (uint8_t*)pThis;
    return self[27];
}

/*
 * grcTextureXenon_vfn_16 @ 0x8215DDE0 | size: 0x8 (8 bytes)
 * vtable slot 16 — SetMipLevels(n): stores uint8 at +27.
 */
void grcTextureXenon_vfn_16(void* pThis, uint8_t n)
{
    uint8_t* self = (uint8_t*)pThis;
    self[27] = n;
}

/*
 * grcTextureXenon_vfn_18 @ 0x8215DDF0 | size: 0xC (12 bytes)
 * vtable slot 18 — GetLODDistance(pOut): copies float from +28 to *pOut.
 */
void grcTextureXenon_vfn_18(void* pThis, float* pOut)
{
    uint8_t* self = (uint8_t*)pThis;
    *pOut = *(float*)(self + 28);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureString vtable accessor
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * grcTextureString_vfn_14 @ 0x8215FE70 | size: 0x10 (16 bytes)
 * vtable slot 14 — GetFilterMode(pA, pB): string textures have no filter,
 *   so both outputs are zeroed.
 */
void grcTextureString_vfn_14(void* pThis, uint8_t* pA, uint8_t* pB)
{
    (void)pThis;
    *pB = 0;
    *pA = 0;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureFactoryString vtable accessor
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * grcTextureFactoryString_vfn_3 @ 0x82160038 | size: 0xC (12 bytes)
 * vtable slot 3 — GetSingleton(): returns the global texture factory string
 *   instance pointer stored at lbl_82606400.
 */
extern void* g_pGrcTextureFactoryString;  /* @ 0x82606400 (.data, 4 bytes) */

void* grcTextureFactoryString_vfn_3(void)
{
    return g_pGrcTextureFactoryString;
}
