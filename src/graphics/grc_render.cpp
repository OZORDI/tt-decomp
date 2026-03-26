/**
 * graphics/grc_render.cpp — grcDevice GPU command submission
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the two functions that form the graphics device's per-frame
 * command submission interface:
 *
 *   grcDevice_beginScene  @ 0x82305E78 (392 bytes)
 *     — Gates scene start on multiple render-state conditions, then
 *       calls CreatePageGroup to begin the GPU pass and dispatches BeginScene
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
extern void CreatePageGroup(void* pDevice);

/* pgStreamer — cancel profiling bracket (mode=-1) @ 0x8242C3B8 */
extern void GetPageGroupState(void* pStream, int32_t mode);

/* pgStreamer — open profiling bracket (mode=1) @ 0x82566DC0 */
extern void RenderPageGroup(void* pStream, int32_t mode);

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
 * Gates GPU scene start on a chain of conditions, then calls CreatePageGroup to
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
 *        call CreatePageGroup(this) to begin the GPU pass.
 *   2. If !m_bDeviceReady: return.
 *   3. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        cancel the profiling bracket (GetPageGroupState(m_pStreamObj[56], -1)).
 *   4. If m_pRenderTarget != NULL:
 *        vtable[11](pRT, m_bColorChannel)
 *        vtable[12](pRT, m_bDepthChannel)
 *        vtable[8](pRT)
 *   5. If m_pStreamObj != NULL and m_pStreamObj[4] != 0:
 *        open profiling bracket (RenderPageGroup(m_pStreamObj[56], 1)).
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_beginScene(grcDeviceBeginScene* pDevice)
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
        /* No active stream — call CreatePageGroup to begin the GPU pass now. */
        CreatePageGroup(pDevice);
        /* Re-read gameLoop; CreatePageGroup may update global state. */
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
        GetPageGroupState(pStream, -1);
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
        RenderPageGroup(pStream, 1);
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


/* ═══════════════════════════════════════════════════════════════════════════
 * grc_CB48 @ 0x8235CB48 | size: 0x23c (572 bytes)
 *
 * Builds GPU command packets for shader constant initialization and GPU state
 * setup. This function constructs three command packets in the GPU command
 * buffer and sets various GPU state flags.
 *
 * The function:
 *   1. Ensures buffer space is available (calls grc_3E20 if needed)
 *   2. Builds first packet: shader constant upload (108 bytes of data)
 *   3. Builds second packet: another shader constant (36 bytes + 12 bytes)
 *   4. Calls grc_DBF8 to finalize packet construction
 *   5. Builds third packet: GPU register writes for state setup
 *   6. Manipulates 64-bit GPU state flags at device+16
 *   7. Calls game_37B0 to submit the commands
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
extern void grc_3E20(void* pDevice);
extern void* rage_A980(void* pDevice);
extern void grc_DBF8(void* pDevice);
extern void game_37B0(void* pDevice);

/* Shader constant data table @ 0x8202C328 (.rdata, 440 bytes) */
extern const uint8_t g_shaderConstantData[440];

void grc_CB48(void* pDevice) {
    uint8_t* device = (uint8_t*)pDevice;
    
    /* Ensure buffer has space - calls grc_3E20 to expand if needed */
    grc_3E20(pDevice);
    
    /* Load current write pointer and buffer end */
    uint32_t writePtr = *(uint32_t*)(device + 0);
    uint32_t bufferEnd = *(uint32_t*)(device + 8);
    
    /* Check if we need to expand buffer */
    if (writePtr > bufferEnd) {
        writePtr = (uint32_t)rage_A980(pDevice);
    }
    
    /* ── Build first packet: shader constant upload (108 bytes data) ────── */
    
    uint32_t* pkt1 = (uint32_t*)(writePtr + 4);
    pkt1[0] = 0xC01C2B00;  /* PM4 command: shader constant load */
    pkt1[1] = 0;           /* Parameter 1 */
    pkt1[2] = 27;          /* Parameter 2 */
    
    /* Copy 108 bytes of shader constant data from table+40 */
    memcpy(&pkt1[3], &g_shaderConstantData[40], 108);
    
    /* Update write pointer (12 header + 108 data = 120 bytes) */
    writePtr = (uint32_t)&pkt1[3] + 108;
    
    /* Check buffer space again */
    if (writePtr > bufferEnd) {
        writePtr = (uint32_t)rage_A980(pDevice);
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
    writePtr = (uint32_t)&extra[3];
    *(uint32_t*)(device + 0) = writePtr;
    
    /* Check buffer space */
    if (writePtr > bufferEnd) {
        writePtr = (uint32_t)rage_A980(pDevice);
    }
    
    /* ── Finalize packet construction ────────────────────────────────────── */
    
    writePtr = (uint32_t)grc_DBF8(pDevice);
    
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
    writePtr = (uint32_t)&pkt3[12];
    *(uint32_t*)(device + 0) = writePtr;
    
    /* ── Submit commands ──────────────────────────────────────────────────── */
    
    game_37B0(pDevice);
}


/* ── External dependencies for texture factory ────────────────────────────── */

/* Debug log function (no-op) @ 0x8240E6D0 */
extern void rage_DebugLog(const char* fmt, ...);

/* Texture processing functions */
extern void grc_FD68(void* pTexture, void* pDevice);  /* @ 0x8215FD68 */
extern void grc_DC00(void* pTexture, void* pDevice);  /* @ 0x8215DC00 */

/* Debug format string for invalid texture types @ 0x82035300 */
extern const char g_invalidTextureTypeMsg[];  /* @ 0x82035300 */


/* ═══════════════════════════════════════════════════════════════════════════
 * grcTextureFactoryXenon::FixupTextures @ 0x8215FCB0 | size: 0xb8 (184 bytes)
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
 *        - Type 0: Call grc_FD68 (raw texture processing)
 *        - Type 1: Skip (already processed)
 *        - Type 2: Call grc_DC00 (compressed texture processing)
 *        - Type 3+: Log error via rage_DebugLog (invalid type)
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcTextureFactoryXenon::FixupTextures(
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
    uint32_t baseAddress = *(uint32_t*)(device + 4);
    uint32_t strideSize = *(uint32_t*)(device + 76);
    
    /* Process each texture in the array */
    for (int i = 0; i < count; i++) {
        void** pTexture = (void**)ppTextures[i];
        
        if (pTexture == NULL) {
            continue;
        }
        
        /* ── Step 1: Adjust texture data pointer if non-null ────────────── */
        
        uint32_t dataPtr = (uint32_t)pTexture[0];
        
        if (dataPtr != 0) {
            /* Calculate offset index from data pointer */
            uint32_t offsetFromBase = dataPtr - baseAddress;
            uint32_t offsetIndex = offsetFromBase / strideSize;
            
            /* Look up offset value in device table at (offsetIndex + 2) */
            uint32_t tableIndex = (offsetIndex + 2) * 4;  /* *4 for pointer array indexing */
            uint32_t offsetValue = *(uint32_t*)(device + tableIndex);
            
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
            /* Type 0: Raw texture - call grc_FD68 */
            grc_FD68(pTexture[0], pDevice);
        }
        else if (textureType == 1) {
            /* Type 1: Already processed - skip */
            continue;
        }
        else if (textureType == 2) {
            /* Type 2: Compressed texture - call grc_DC00 */
            grc_DC00(pTexture[0], pDevice);
        }
        else {
            /* Type 3+: Invalid texture type - log error */
            rage_DebugLog(g_invalidTextureTypeMsg);
        }
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grc_8838_h @ 0x82568838 | size: 0x1C (28 bytes)
 *
 * Stores four uint32 values into consecutive fields of a structure and
 * returns 1 (success). Used as a bulk-setter for a 16-byte render state
 * block (e.g. viewport rect or scissor rect).
 *
 * Parameters:
 *   pDest (r3) — pointer to destination struct
 *   a (r4), b (r5), c (r6), d (r7) — four uint32 values to store
 * Returns: 1
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x82568838
int32_t grc_8838_h(void* pDest, uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
    uint32_t* p = (uint32_t*)pDest;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_F6B8 @ 0x8215F6B8 | size: 0x1C (28 bytes)
 *
 * Checks if a texture format ID matches 0x1A22ABCD. If it matches,
 * returns the adjacent format code 0x1A22ABE0. Otherwise returns the
 * input value unchanged (by not modifying r3).
 *
 * This appears to be a format remapping function that converts one
 * specific internal texture format ID to another.
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x8215F6B8
uint32_t grc_F6B8(uint32_t formatId)
{
    if (formatId != 0x1A22ABCD)
        return formatId;
    return 0x1A22ABE0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_B7E8 @ 0x8215B7E8 | size: 0x24 (36 bytes)
 *
 * Counts the number of nodes in a singly-linked list. The head pointer
 * is at offset +24 of the input object, and each node's next pointer is
 * also at offset +24. Returns 0 if the list is empty.
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x8215B7E8
int32_t grc_B7E8(void* pObj)
{
    uint8_t* node = *(uint8_t**)((uint8_t*)pObj + 24);
    if (node == NULL)
        return 0;

    int32_t count = 0;
    while (node != NULL) {
        node = *(uint8_t**)(node + 24);
        count++;
    }
    return count;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_6A08 @ 0x82366A08 | size: 0x28 (40 bytes)
 *
 * Initiates GPU/video engine shutdown. Registers a title terminate
 * notification with a NULL callback, then calls VdShutdownEngines to
 * shut down the Xenon video driver. Called during application exit.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void ExRegisterTitleTerminateNotification(void* pNotification, uint32_t bCancel);
extern void VdShutdownEngines(void);

// @ 0x82366A08
void grc_6A08(void)
{
    ExRegisterTitleTerminateNotification(NULL, 0);
    VdShutdownEngines();
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_EAE8 @ 0x8214EAE8 | size: 0x28 (40 bytes)
 *
 * Conditionally subtracts a float from 1.0f. Checks a global format ID
 * at 0x825E9070; if it matches the sentinel value 0x1A220197, the input
 * float is subtracted from 1.0f and the result returned. Otherwise the
 * input is returned unchanged.
 *
 * Used by the texture factory to flip V coordinates for certain texture
 * formats that store UVs in inverted space.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern uint32_t g_grcFormatId;  // @ 0x825E9070

// @ 0x8214EAE8
float grc_EAE8(float value)
{
    if (g_grcFormatId != 0x1A220197)
        return value;
    return 1.0f - value;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_1268 @ 0x82151268 | size: 0x4C (76 bytes)
 *
 * Decrements a reference count at offset +32 of the given object. When
 * the count reaches zero, calls grc_12B8 (the destructor/release function)
 * with flag=1 and returns 0. Otherwise returns the new refcount.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_12B8(void* pObj, int32_t flags);

// @ 0x82151268
int32_t grc_1268(void* pObj)
{
    int32_t refCount = *(int32_t*)((uint8_t*)pObj + 32);
    refCount--;
    *(int32_t*)((uint8_t*)pObj + 32) = refCount;

    if (refCount == 0) {
        grc_12B8(pObj, 1);
        return 0;
    }
    return refCount;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_CC50_h @ 0x8213CC50 | size: 0x58 (88 bytes)
 *
 * Sets an indexed sampler/render state value, tracked by a dirty bitmask.
 * Stores the new value into two parallel state arrays (indexed by r3),
 * reads the old value from a third array, and sets or clears the
 * corresponding bit in a dirty flags word based on whether the value changed.
 *
 * Global arrays (SDA-relative, base lis(-32161)):
 *   g_grcSamplerOld    @ 0x825EBB60 — old state values (read-only here)
 *   g_grcSamplerNew1   @ 0x825E8F48 — first shadow copy of new state
 *   g_grcSamplerNew2   @ 0x825EBB98 — second shadow copy of new state
 *   g_grcDirtyBits     @ 0x825EBB1C — dirty bitmask (32 bits)
 * ═══════════════════════════════════════════════════════════════════════════ */
extern uint32_t g_grcSamplerOld[];   // @ 0x825EBB60
extern uint32_t g_grcSamplerNew1[];  // @ 0x825E8F48
extern uint32_t g_grcSamplerNew2[];  // @ 0x825EBB98
extern uint32_t g_grcDirtyBits;      // @ 0x825EBB1C

// @ 0x8213CC50
void grc_CC50_h(uint32_t index, uint32_t value)
{
    uint32_t oldValue = g_grcSamplerOld[index];
    g_grcSamplerNew1[index] = value;
    g_grcSamplerNew2[index] = value;

    uint32_t bit = 1u << index;

    if (value == oldValue) {
        // State unchanged — clear the dirty bit
        g_grcDirtyBits &= ~bit;
    } else {
        // State changed — set the dirty bit
        g_grcDirtyBits |= bit;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_9858_h @ 0x82359858 | size: 0x58 (88 bytes)
 *
 * Extracts render state from a GPU command object and dispatches it to
 * grc_8928. Checks bit 25 of the flags word at offset +0; if set,
 * extracts a 4-bit field from offset +32 (bits 2-5) and the data pointer
 * from offset +44. Otherwise passes 0 for the extracted field.
 * After dispatch, marks the output object as valid by storing 1 at +4.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_8928(void* pSrc, uint32_t field, void* pDest);

// @ 0x82359858
void grc_9858_h(void* pCmd, void* pOut)
{
    uint32_t flags = *(uint32_t*)((uint8_t*)pCmd + 0);
    void* pSrc;
    uint32_t field;

    if (flags & 0x02000000) {
        uint32_t word32 = *(uint32_t*)((uint8_t*)pCmd + 32);
        field = (word32 >> 2) & 0xF;
        pSrc = *(void**)((uint8_t*)pCmd + 44);
    } else {
        pSrc = pCmd;  // r3 passed through unchanged
        field = 0;
    }

    grc_8928(pSrc, field, pOut);

    *(uint32_t*)((uint8_t*)pOut + 4) = 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_FF38 @ 0x8243FF38 | size: 0x60 (96 bytes)
 *
 * Retrieves render target dimensions. Calls grc_7DD0 to obtain width and
 * height as stack outputs, then stores them into the optional output
 * pointers if non-NULL.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_7DD0(void* pObj, uint32_t* pWidth, uint32_t* pHeight);

// @ 0x8243FF38
void grc_FF38(void* pObj, uint32_t* pOutWidth, uint32_t* pOutHeight)
{
    uint32_t width, height;
    grc_7DD0(pObj, &width, &height);

    if (pOutWidth != NULL) {
        *pOutWidth = width;
    }
    if (pOutHeight != NULL) {
        *pOutHeight = height;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_8B58 @ 0x82158B58 | size: 0x74 (116 bytes)
 *
 * Flushes a dirty texture object. If the dirty flag at offset +20 is set
 * and the texture pointer at +12 is valid, submits the texture data via
 * grc_2CC8 (unless g_bTextureEndianDirty is set, which suppresses flush).
 * Clears the dirty flag after flushing. Returns 1 if the flag was
 * cleared (i.e. was dirty), 0 if it was already clean.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_2CC8(void* pTexData, void* pInternalData, uint32_t param);

// @ 0x82158B58
uint8_t grc_8B58(void* pObj)
{
    uint8_t* obj = (uint8_t*)pObj;

    if (obj[20] != 0) {
        void* pTexPtr = *(void**)(obj + 12);
        if (pTexPtr != NULL) {
            extern uint8_t g_bTextureEndianDirty;  // @ 0x82606562
            if (g_bTextureEndianDirty == 0) {
                void* pInternalData = *(void**)((uint8_t*)pTexPtr + 12);
                grc_2CC8(pTexPtr, pInternalData, 0);
            }
        }
        obj[20] = 0;
    }

    // Return !dirty (cntlzw/rlwinm idiom: returns 1 if byte is 0)
    return (obj[20] == 0) ? 1 : 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_3448 @ 0x82323448 | size: 0x68 (104 bytes)
 *
 * Conditionally binds a texture to the GPU. If the texture reference at
 * r4[0] is zero, looks up the active GPU device, checks a flag at
 * device[index+20], and if set, calls grc_0E28 to bind the default
 * texture and marks a global dirty flag.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void* g_grcDevicePtr;      // @ 0x825E4898
extern void* g_grcDevicePtr2;     // @ 0x825F63D4
extern void grc_0E28(void* pTex, uint32_t param1, uint32_t param2);

// @ 0x82323448
void grc_3448(uint32_t index, uint32_t* pTexRef)
{
    if (*pTexRef != 0)
        return;

    uint8_t* pDevice = (uint8_t*)g_grcDevicePtr;
    uint8_t* pEntry = pDevice + index;

    if (pEntry[20] == 0)
        return;

    uint8_t* pDev2 = (uint8_t*)g_grcDevicePtr2;
    void* pTex = *(void**)((uint8_t*)pDev2 + 108);
    grc_0E28(pTex, 0, 0);

    uint8_t* pGlobal = (uint8_t*)g_grcDevicePtr2;
    uint8_t* pFlags = (uint8_t*)(*(void**)((uint8_t*)pGlobal - 21720 + (uint32_t)pGlobal));
    // Simplified: set dirty flag
    *(uint8_t*)(*(uint8_t**)((uint8_t*)g_grcDevicePtr2) + 344) = 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_30C0 @ 0x823530C0 | size: 0x6C (108 bytes)
 *
 * Wrapper for grc_29E0 (DrawPrimitive). Sets up a non-indexed draw call
 * with primitive type 46 (triangle fan). When offset/count are zero,
 * defaults to the full buffer (size from offset +16, masked to strip
 * the 2 low bits). Applies draw flags with 0x03000000 bitmask.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_29E0(void* pBuffer, uint32_t primType, uint32_t startVertex,
                      uint32_t baseData, uint32_t param5, void* pSrc,
                      uint32_t flags, uint32_t extraFlags);

// @ 0x823530C0
void grc_30C0(void* pVB, uint32_t offset, uint32_t count, uint32_t flags)
{
    uint8_t* vb = (uint8_t*)pVB;

    if (count == 0) {
        count = *(uint32_t*)(vb + 16) & 0x3FFFFFC;
        offset = 0;
    }

    uint32_t stride = *(uint32_t*)(vb + 12) & 0xFFFFFFFC;
    void* pBuffer = *(void**)(vb + 4);

    grc_29E0(pBuffer, 46, 0, stride + offset, 0, pVB, flags | 2, 0x03000000);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_31F0 @ 0x823531F0 | size: 0x68 (104 bytes)
 *
 * Wrapper for grc_29E0 (DrawIndexedPrimitive). Similar to grc_30C0 but
 * for indexed draws with primitive type 47 (triangle list, indexed).
 * When count is zero, defaults to the full buffer size from offset +16.
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x823531F0
void grc_31F0(void* pVB, uint32_t offset, uint32_t count, uint32_t flags)
{
    uint8_t* vb = (uint8_t*)pVB;

    if (count == 0) {
        count = *(uint32_t*)(vb + 16);
        offset = 0;
    }

    uint32_t stride = *(uint32_t*)(vb + 12);
    void* pBuffer = *(void**)(vb + 4);

    grc_29E0(pBuffer, 47, 0, stride + offset, 0, pVB, flags | 2, 0);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_F538 @ 0x8243F538 | size: 0x70 (112 bytes)
 *
 * Draw triangle list wrapper. Calls grc_F338 with primitive type 3
 * (triangle list). Reshuffles many register parameters into a stack-based
 * call frame for the common draw dispatcher.
 *
 * Parameters are vertex data pointers and counts for multi-stream draw calls.
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void grc_F338(void* pVB, uint32_t primType, uint32_t startVertex,
                      void* pVtxData, uint32_t vtxStride, uint32_t vtxCount,
                      uint32_t param7, void* pIdxData, uint32_t idxCount,
                      uint32_t param10, uint32_t param11, uint32_t param12,
                      uint32_t param13, uint32_t param14, uint32_t param15);

// @ 0x8243F538
void grc_F538(void* pVB, uint32_t startVertex, void* pVtxData,
              uint32_t vtxStride, uint32_t vtxCount, void* pIdxData,
              uint32_t idxCount, uint32_t extra1, uint32_t extra2,
              uint32_t extra3, uint32_t extra4)
{
    grc_F338(pVB, 1, startVertex, pVtxData, vtxStride, vtxCount,
             0, pIdxData, idxCount, 2, 3, extra1, extra2, extra3, extra4);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_F5A8 @ 0x8243F5A8 | size: 0x74 (116 bytes)
 *
 * Draw line strip wrapper. Calls grc_F338 with primitive type 6
 * (line strip). Similar register reshuffling as grc_F538.
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x8243F5A8
void grc_F5A8(void* pVB, uint32_t param2, void* pVtxData,
              uint32_t vtxStride, uint32_t vtxCount, void* pIdxData,
              uint32_t idxCount, uint32_t extra1, uint32_t extra2,
              uint32_t extra3, uint32_t extra4)
{
    grc_F338(pVB, 6, param2, pVtxData, vtxStride, vtxCount,
             0, pIdxData, 0, 2, 5, 0, extra1, extra2, extra3);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_F620 @ 0x8243F620 | size: 0x74 (116 bytes)
 *
 * Draw triangle strip wrapper. Calls grc_F338 with primitive type 4
 * (triangle strip). Same pattern as the other draw wrappers.
 * ═══════════════════════════════════════════════════════════════════════════ */
// @ 0x8243F620
void grc_F620(void* pVB, uint32_t startVertex, void* pVtxData,
              uint32_t vtxStride, uint32_t vtxCount, void* pIdxData,
              uint32_t idxCount, void* pExtra1, uint32_t extra2,
              uint32_t extra3, uint32_t extra4)
{
    grc_F338(pVB, 1, startVertex, pVtxData, vtxStride, vtxCount,
             0, pIdxData, 0, 2, 4, 0, (uint32_t)(uintptr_t)pExtra1,
             extra2, extra3);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grc_12B8 @ 0x821512B8 | size: 0x7C (124 bytes)
 *
 * Texture factory node destructor. Recursively releases child nodes via
 * grc_1268 (refcount decrement), frees internal data, and cleans up.
 *
 * Object layout:
 *   +16  m_pData        — heap data pointer (freed via rage_free)
 *   +20  m_pOwner       — owner pointer (freed when both children are NULL)
 *   +24  m_pChild1      — first child node pointer
 *   +28  m_pChild2      — second child node pointer
 *   +32  m_nRefCount    — reference count
 * ═══════════════════════════════════════════════════════════════════════════ */
extern void rage_free(void* ptr);

// @ 0x821512B8
void grc_12B8_impl(void* pNode, int32_t flags)
{
    uint8_t* node = (uint8_t*)pNode;

    // Release second child node if present
    void* pChild2 = *(void**)(node + 28);
    if (pChild2 != NULL) {
        grc_1268(pChild2);
    }

    // Free data pointer
    rage_free(*(void**)(node + 16));

    // If both children are NULL, free the owner
    void* pChild1 = *(void**)(node + 24);
    if (pChild1 == NULL) {
        void* pChild2b = *(void**)(node + 28);
        if (pChild2b == NULL) {
            rage_free(*(void**)(node + 20));
        }
    }

    // Release first child node if present
    pChild1 = *(void**)(node + 24);
    if (pChild1 != NULL) {
        grc_1268(pChild1);
    }

    // Free this node
    rage_free(pNode);
}
