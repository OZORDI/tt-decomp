/**
 * graphics/grc_device.cpp — Graphics Device Lifecycle Management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Core graphics device functions:
 *   - grcDevice_beginScene @ 0x82305E78 — Begin rendering frame
 *   - grcDevice_shutdown   @ 0x8214FA58 — Primary shutdown sequence
 *   - grcDevice_shutdownAlt @ 0x821540D0 — Alternate shutdown path
 */

#include <cstdint>
#include <cstddef>

/* ── External Dependencies ────────────────────────────────────────────────── */

// Global singleton pointers
extern void* g_loop_obj_ptr;           // @ 0x825EAB30 — gameLoop singleton
extern void* g_pCamActionSys;          // @ 0x8271A344 — Camera action system
extern uint32_t g_pCamActionsFlags;    // @ 0x825D07D0 — Camera action flags
extern void* g_pHudOverlay;            // @ 0x825EBAA0 — HUD overlay object

// Graphics subsystem functions
extern void CreatePageGroup(void* pDevice);           // @ 0x82305D50 — Begin GPU pass
extern void GetPageGroupState(void* pStream, int32_t mode);  // @ 0x8242C3B8 — Cancel profiling
extern void RenderPageGroup(void* pStream, int32_t mode);  // @ 0x82566DC0 — Open profiling

// Memory management
extern void rage_free(void* ptr);      // @ 0x820C00C0

// Device management functions
extern void rage_5BF8(void* pObj);          // @ 0x82445BF8 — Destroy render object
extern void rage_56A8(void* pCache);        // @ 0x824456A8 — Destroy texture cache
extern void grcSafeRelease(void* pDevice); // @ 0x82151068 — Cleanup internal
extern void rage_F338(void);                // @ 0x8214F338 — Shutdown graphics core
extern void rage_6CA0(void* pTarget);       // @ 0x82366CA0 — Cleanup render target
extern void _locale_register(void* pLocale, uint32_t flags);  // @ 0x820C02D0
extern void thunk_DbgBreakPoint(void);      // @ 0x82566B70
extern void rage_7400(void* pDevice, int32_t arg);  // @ 0x82357400

// Forward declarations
void grcDevice_shutdownAlt(void);

/* ── Structure Definitions ────────────────────────────────────────────────── */

typedef struct {
    uint8_t _pad0[493];
    uint8_t m_bRenderPaused;    // +493 (0x1ED)
    uint8_t m_bRenderBlocked;   // +494 (0x1EE)
    uint8_t m_bSkipRender;      // +495 (0x1EF)
    uint8_t _pad1[60];
    void*   m_pStreamObj;       // +556 (0x22C) — Profiling stream object
} gameLoop;

typedef struct {
    uint8_t  _pad0[100];
    void*    m_pRenderTarget;    // +100 (0x64) — grcRenderTargetXenon*
    uint8_t  _pad1[7];
    uint8_t  m_bColorChannel;    // +108 (0x6C)
    uint8_t  m_bDepthChannel;    // +109 (0x6D)
    uint8_t  _pad2[2];
    uint8_t  m_bDeviceReady;     // +112 (0x70)
    uint8_t  m_bBeginAllowed;    // +113 (0x71)
} grcDeviceBeginScene;

typedef struct {
    uint8_t  _pad0[4];
    uint8_t  m_bActive;          // +4
    uint8_t  _pad1[51];
    void*    m_pStream;          // +56
} pgStreamObj;

typedef struct {
    void**   vtable;             // +0
    uint8_t  _pad[4];
    uint16_t m_refCount;         // +6
} RenderObject;


/* ═══════════════════════════════════════════════════════════════════════════
 * grcDevice_beginScene @ 0x82305E78 | size: 0x188 (392 bytes)
 *
 * Begins a new rendering frame with multiple gate checks:
 *   1. Skip-render and camera-action gates
 *   2. Device readiness and render-pause checks
 *   3. Camera action override check
 *   4. GPU pass initialization
 *   5. Render target channel setup
 *   6. Profiling bracket management
 *
 * This function ensures rendering only proceeds when all subsystems are ready
 * and no blocking conditions are active.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_beginScene(grcDeviceBeginScene* pDevice)
{
    gameLoop* pLoop = (gameLoop*)g_loop_obj_ptr;
    
    /* ── Gate A: Skip-render and camera-action check ─────────────────────── */
    bool bGateFired = false;
    
    if (pLoop->m_bSkipRender == 0) {
        // Not in skip-render mode — check if camera action is active
        if (g_pCamActionSys != nullptr) {
            uint8_t* pCamSys = (uint8_t*)g_pCamActionSys;
            uint8_t camActive = pCamSys[24];
            bGateFired = (camActive != 0);
        }
    } else {
        bGateFired = true;
    }
    
    /* ── Gate B: Device flags and render-pause checks ────────────────────── */
    if (bGateFired) {
        if (!pDevice->m_bBeginAllowed) {
            return;
        }
        
        if (pLoop->m_bRenderPaused || pLoop->m_bRenderBlocked) {
            return;
        }
    }
    
    /* ── Gate C: Camera action override check ────────────────────────────── */
    if (g_pCamActionsFlags != 0) {
        return;
    }
    
    /* ── GPU Pass Initialization ─────────────────────────────────────────── */
    pLoop = (gameLoop*)g_loop_obj_ptr;  // Reload after potential mutations
    pgStreamObj* pStreamObj = (pgStreamObj*)pLoop->m_pStreamObj;
    
    bool streamActive = (pStreamObj != nullptr && pStreamObj->m_bActive != 0);
    
    if (!streamActive) {
        // No active profiling stream — begin GPU pass now
        CreatePageGroup(pDevice);
        pLoop = (gameLoop*)g_loop_obj_ptr;  // Reload after GPU call
    }
    
    /* ── Device Readiness Check ──────────────────────────────────────────── */
    if (!pDevice->m_bDeviceReady) {
        return;
    }
    
    /* ── Profiling Bracket: Cancel Before Render Target ─────────────────── */
    pStreamObj = (pgStreamObj*)pLoop->m_pStreamObj;
    if (pStreamObj != nullptr && pStreamObj->m_bActive != 0) {
        GetPageGroupState(pStreamObj->m_pStream, -1);
        pLoop = (gameLoop*)g_loop_obj_ptr;
    }
    
    /* ── Render Target Channel Setup ─────────────────────────────────────── */
    void* pRenderTarget = pDevice->m_pRenderTarget;
    if (pRenderTarget != nullptr) {
        void** vtable = *(void***)pRenderTarget;
        
        // Set color channel clear flag (vtable slot 11)
        typedef void (*SetChannelFn)(void*, uint8_t);
        ((SetChannelFn)vtable[11])(pRenderTarget, pDevice->m_bColorChannel);
        
        // Set depth channel clear flag (vtable slot 12)
        ((SetChannelFn)vtable[12])(pRenderTarget, pDevice->m_bDepthChannel);
        
        // Finalize BeginScene on render target (vtable slot 8)
        typedef void (*FinalizeFn)(void*);
        ((FinalizeFn)vtable[8])(pRenderTarget);
        
        pLoop = (gameLoop*)g_loop_obj_ptr;
    }
    
    /* ── Profiling Bracket: Open After Render Target ────────────────────── */
    pStreamObj = (pgStreamObj*)pLoop->m_pStreamObj;
    if (pStreamObj != nullptr && pStreamObj->m_bActive != 0) {
        RenderPageGroup(pStreamObj->m_pStream, 1);
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcDevice_shutdown @ 0x8214FA58 | size: 0x1D4 (468 bytes)
 *
 * Primary graphics device shutdown sequence:
 *   1. Clear device state flags
 *   2. Destroy render objects with reference counting
 *   3. Free texture cache
 *   4. Call alternate shutdown path
 *   5. Destroy texture cache object
 *   6. Clean up internal device structures
 *   7. Shutdown graphics core
 *   8. Release render target with reference counting
 *
 * This is the main teardown path called during normal game shutdown.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_shutdown(void)
{
    // Global addresses
    uint32_t* pDeviceState = (uint32_t*)0x825EBB8C;      // lbl_825EBB8C
    RenderObject** pRenderObjects = (RenderObject**)0x82617E60;  // lbl_82617E60
    void** ppDevice = (void**)0x825EBB94;                // lbl_825EBB94
    void** ppTextureCache = (void**)0x825EBB40;          // lbl_825EBB40
    
    /* 1. Clear device state flag at +396 */
    pDeviceState[99] = 0;
    
    /* Check and clear first state flag */
    if (pDeviceState[0] != 0) {
        pDeviceState[0] = 0;
        rage_7400(*ppDevice, 0);
    }
    
    /* 2. Destroy render objects array */
    if (pRenderObjects[0] != nullptr) {
        rage_5BF8(pRenderObjects[0]);
        pRenderObjects[0] = nullptr;
    }
    
    if (pRenderObjects[1] != nullptr) {
        rage_free(pRenderObjects[1]);
        pRenderObjects[1] = nullptr;
    }
    
    /* 3. Call alternate shutdown path */
    grcDevice_shutdownAlt();
    
    /* 4. Destroy texture cache */
    if (*ppTextureCache != nullptr) {
        rage_56A8(*ppTextureCache);
        *ppTextureCache = nullptr;
    }
    
    /* 5. Clean up internal device structures */
    grcSafeRelease((void*)0x825EBB98);  // lbl_825EBB98
    grcSafeRelease((void*)0x825EBBA0);  // lbl_825EBBA0
    
    /* 6. Shutdown graphics core */
    rage_F338();
    
    /* 7. Release render objects with reference counting */
    for (int i = 0; i < 2; i++) {
        uint32_t baseOffset = 0x826063E8 + (i * 8);  // lbl_826063F0 - 8
        
        // Process first object in pair
        RenderObject** ppObj1 = (RenderObject**)baseOffset;
        if (*ppObj1 != nullptr) {
            RenderObject* pObj = *ppObj1;
            pObj->m_refCount--;
            
            if (pObj->m_refCount == 0) {
                // Call destructor (vtable slot 0) with flags=1
                typedef void (*DtorFn)(void*, int);
                ((DtorFn)pObj->vtable[0])(pObj, 1);
            } else {
                thunk_DbgBreakPoint();  // Unexpected refcount
            }
            
            *ppObj1 = nullptr;
        }
        
        // Process second object in pair
        RenderObject** ppObj2 = (RenderObject**)(baseOffset + 8);
        if (*ppObj2 != nullptr) {
            RenderObject* pObj = *ppObj2;
            pObj->m_refCount--;
            
            if (pObj->m_refCount == 0) {
                typedef void (*DtorFn)(void*, int);
                ((DtorFn)pObj->vtable[0])(pObj, 1);
            } else {
                thunk_DbgBreakPoint();
            }
            
            *ppObj2 = nullptr;
        }
    }
    
    /* 8. Release render target with reference counting */
    if (*ppDevice != nullptr) {
        uint32_t* pTargetData = (uint32_t*)*ppDevice;
        uint32_t refCount = pTargetData[3];  // +12
        
        if (refCount == 1) {
            rage_6CA0(*ppDevice);
            
            // Free locale data
            void* pLocale = *(void**)((uint8_t*)*ppDevice - 4);
            _locale_register(pLocale, 0x24800000);  // lis r4, 9344
            
            *ppDevice = nullptr;
        } else {
            pTargetData[3] = refCount - 1;
            
            if (refCount - 1 == 0) {
                thunk_DbgBreakPoint();
            }
            
            *ppDevice = nullptr;
        }
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * grcDevice_shutdownAlt @ 0x821540D0 | size: 0x58 (88 bytes)
 *
 * Alternate shutdown path that handles HUD overlay cleanup:
 *   1. Check if HUD overlay pointer matches device pointer
 *   2. Clear HUD overlay pointer if they match
 *   3. Free the device memory
 *   4. Clear device pointer
 *
 * This is called as part of the main shutdown sequence and handles the
 * specific case where the device is also registered as the HUD overlay.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grcDevice_shutdownAlt(void)
{
    void** ppDevice = (void**)0x825EBB9C;  // lbl_825EBB9C
    
    void* pDevice = *ppDevice;
    
    if (pDevice != nullptr) {
        // Check if this device is also the HUD overlay
        if (g_pHudOverlay == pDevice) {
            g_pHudOverlay = nullptr;
        }
        
        // Free the device memory
        rage_free(pDevice);
    }
    
    // Clear device pointer
    *ppDevice = nullptr;
}


// ─────────────────────────────────────────────────────────────────────────────
// grcRenderer::Init @ 0x82100B78 | size: 0x13C (316 bytes)
//
// Initializes the grcRenderer object. Sets the vtable pointer, zeroes all
// render state fields, and initializes 5 embedded render-pass descriptors
// with their respective vtable pointers and default values.
//
// Layout:
//   +0:   vtable (0x8203008C)
//   +4:   render state block (108 bytes, zeroed)
//     +4..+36:  8 state dwords (zeroed)
//     +24:      invalidation handle (-1)
//     +100..104: 3 bool flags (zeroed)
//     +108:     secondary handle (-1)
//     +109:     enable flag (zeroed)
//   +112: render config block
//     +112: config flag (zeroed)
//     +116: config float A (0.0f)
//     +120: config float B (0.0f)
//   +128: 5 embedded render-pass descriptors (32 bytes each)
//     Each pass: vtable, float(0.0), zeroes, zero count, zero handle
//     Pass 0 vtable: 0x82030CB8
//     Pass 1-4 vtable: 0x8203F258
//   +288: final state byte (zeroed)
//   +292-300: 3 trailing dwords (0, 0, -1)
// ─────────────────────────────────────────────────────────────────────────────
void grcRenderer_Init(void* renderer) {
    char* r = (char*)renderer;

    // Set main vtable
    *(void**)r = (void*)0x8203008C;

    // Zero render state block (+4 to +36)
    for (int i = 0; i < 8; i++) {
        *(uint32_t*)(r + 4 + i * 4) = 0;
    }
    *(int32_t*)(r + 4 + 16) = -1;  // invalidation handle at +20 from base (+4+16)

    // Zero byte flags at +100..+102 from state base (+4+96..+4+98)
    *(uint8_t*)(r + 4 + 96) = 0;
    *(uint8_t*)(r + 4 + 97) = 0;
    *(uint8_t*)(r + 4 + 98) = 0;
    *(int32_t*)(r + 4 + 100) = -1;  // secondary handle
    *(uint8_t*)(r + 4 + 104) = 0;   // enable flag

    // Zero second state array (+36 to +68, 8 dwords)
    for (int i = 0; i < 8; i++) {
        *(uint32_t*)(r + 4 + 32 + i * 4) = 0;
    }

    // Zero third state array (+68 to +100, 8 dwords)
    for (int i = 0; i < 8; i++) {
        *(uint32_t*)(r + 4 + 64 + i * 4) = 0;
    }

    // Render config block at +112
    *(uint8_t*)(r + 112) = 0;           // config flag
    *(uint32_t*)(r + 116) = 0;          // padding
    *(float*)(r + 120) = 0.0f;          // config float A
    *(float*)(r + 124) = 0.0f;          // config float B

    // Render pass 0 at +128 (vtable = 0x82030CB8)
    *(void**)(r + 128) = (void*)0x82030CB8;  // pass vtable
    *(float*)(r + 132) = 0.0f;               // blend weight
    *(uint32_t*)(r + 136) = 0;               // state
    *(uint32_t*)(r + 144) = 0;               // count
    *(uint16_t*)(r + 152) = 0;               // flags

    // Render passes 1-4 at +156, +188, +220, +252 (vtable = 0x8203F258)
    for (int pass = 0; pass < 4; pass++) {
        int base = 156 + pass * 32;
        *(void**)(r + base) = (void*)0x8203F258;
        *(float*)(r + base + 4) = 0.0f;
        *(uint32_t*)(r + base + 8) = 0;
        *(uint32_t*)(r + base + 16) = 0;
        *(uint16_t*)(r + base + 24) = 0;
        *(uint8_t*)(r + base + 28) = 0;
    }

    // Trailing state
    *(uint8_t*)(r + 288) = 0;
    *(uint32_t*)(r + 292) = 0;
    *(uint32_t*)(r + 296) = 0;
    *(int32_t*)(r + 300) = -1;
}
