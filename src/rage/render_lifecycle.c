/**
 * render_lifecycle.c - Rendering lifecycle management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Functions for initializing, configuring, and managing the rendering
 * subsystem lifecycle, including render target setup and fiber management.
 */

#include <stdint.h>
#include <string.h>

/* ── Forward declarations ─────────────────────────────────────────────────── */

/* Root game object pointer @ 0x825EB988 */
extern void* g_pRootGameObj;

/* Screen dimension globals @ 0x825C9014 / 0x825C9018 */
extern uint32_t g_nScreenWidth;
extern uint32_t g_nScreenHeight;

/* Non-widescreen flag @ 0x825C9021 */
extern uint8_t g_bNonWidescreen;

/* Render config globals */
extern uint32_t g_renderStateFlags;      /* lbl_825EBB1C @ 0x825EBB1C */
extern uint8_t g_renderConfigData[];     /* lbl_825EB9E4 @ 0x825EB9E4 */

/* Display mode configuration @ 0x825C9030 */
extern uint32_t g_displayModeConfig;

/* Aspect ratio scale constants */
extern const float k_aspectRatioScale;   /* lbl_8202D108 @ 0x8202D108 */

/* Helper functions */
extern void rage_GetExecutableName(const char* pKey, uint32_t* pOut);
extern void _crt_tls_fiber_setup(void);
extern void VdQueryVideoMode(void* pMode);
extern void memcpy(void* dst, const void* src, uint32_t size);

/* Render target management */
extern void* grcRenderTarget_CreatePrimary(void* vtable, uint32_t flags, uint32_t param1,
                                     uint32_t param2, uint32_t param3, uint32_t format,
                                     uint32_t param5, uint32_t param6, uint32_t param7);
extern void* grcRenderTarget_CreateSecondary(void* vtable, uint32_t flags, uint32_t format,
                                     uint32_t width, uint32_t param4);
extern void grcRenderTarget_Free(void* pTarget);
extern void grcRenderTarget_Configure(void* pObj, uint32_t index, uint32_t param);
extern void grcRenderTarget_InitState(void* pObj, uint32_t param);
extern void grcRenderPipeline_Configure(void* pObj, void* pConfig);

/* Video mode structure (Xbox 360 SDK) */
typedef struct VideoMode {
    uint32_t displayWidth;      /* +0x00 */
    uint32_t displayHeight;     /* +0x04 */
    uint32_t isInterlaced;      /* +0x08 */
    uint32_t isWidescreen;      /* +0x0C */
    uint32_t isHiDef;           /* +0x10 */
    float    refreshRate;       /* +0x14 */
    uint32_t videoStandard;     /* +0x18 */
    uint8_t  _reserved[100];    /* +0x1C..0x7F */
} VideoMode;

/* Display mode constants */
#define DISPLAY_MODE_STANDARD_262  0x28200106  /* 10280 << 16 | 262 */
#define DISPLAY_MODE_STANDARD_310  0x28200136  /* 10280 << 16 | 310 */
#define DISPLAY_MODE_STANDARD_390  0x28200186  /* 10280 << 16 | 390 */
#define DISPLAY_MODE_STANDARD_438  0x282001B6  /* 10280 << 16 | 438 */
#define DISPLAY_MODE_HIDEF_262     0x18200106  /* 6184 << 16 | 262 */
#define DISPLAY_MODE_HIDEF_310     0x18200136  /* 6184 << 16 | 310 */
#define DISPLAY_MODE_HIDEF_407     0x1A200197  /* 6690 << 16 | 407 */
#define DISPLAY_MODE_HIDEF_447     0x1A2001BF  /* 6690 << 16 | 447 */


/* ═══════════════════════════════════════════════════════════════════════════
 * InitializeRenderConfig @ 0x8214F400 | size: 0x248
 * 
 * Initializes global rendering configuration structures.
 * Sets up screen dimensions, display modes, and render timing.
 * ═══════════════════════════════════════════════════════════════════════════ */
void InitializeRenderConfig(void) {
    uint8_t* pConfigData = g_renderConfigData;
    uint32_t screenWidth = 1280;
    uint32_t screenHeight = 720;
    
    /* Clear configuration data (124 bytes) */
    for (int i = 0; i < 31; i++) {
        ((uint32_t*)pConfigData)[i] = 0;
    }
    
    /* Query screen dimensions from executable name overrides */
    rage_GetExecutableName("ScreenWidth", &screenWidth);
    rage_GetExecutableName("ScreenHeight", &screenHeight);
    
    g_nScreenWidth = screenWidth;
    g_nScreenHeight = screenHeight;
    
    /* Store dimensions in config */
    ((uint32_t*)pConfigData)[0] = screenWidth;
    ((uint32_t*)pConfigData)[1] = screenHeight;
    
    /* Determine display mode based on hi-def capability */
    uint32_t displayMode;
    if (g_displayModeConfig != 0) {
        displayMode = DISPLAY_MODE_HIDEF_407;
    } else {
        displayMode = DISPLAY_MODE_STANDARD_390;
    }
    ((uint32_t*)pConfigData)[2] = displayMode;
    
    /* Configure render state flags */
    uint32_t renderFlags = g_renderStateFlags;
    if (g_displayModeConfig != 0) {
        renderFlags = DISPLAY_MODE_HIDEF_407;
        g_renderStateFlags = renderFlags;
    }
    
    /* Set up aspect ratio and display parameters */
    ((uint32_t*)pConfigData)[3] = 1;  /* Enable flag */
    ((uint32_t*)pConfigData)[4] = 0;  /* Reserved */
    
    /* Configure widescreen mode */
    uint32_t isWidescreen = g_bNonWidescreen ? 0 : 1;
    ((uint32_t*)pConfigData)[5] = isWidescreen;
    ((uint32_t*)pConfigData)[6] = renderFlags;
    ((uint32_t*)pConfigData)[7] = 0;  /* Reserved */
    
    /* Set display mode constant */
    ((uint32_t*)pConfigData)[8] = DISPLAY_MODE_STANDARD_262;
    ((uint32_t*)pConfigData)[9] = 1;  /* Enable post-processing */
    ((uint32_t*)pConfigData)[10] = 1; /* Enable HDR */
    
    /* Calculate aspect ratio scale */
    float aspectScale = (float)screenHeight / (float)screenWidth;
    aspectScale *= k_aspectRatioScale;
    
    /* Store computed values */
    ((float*)pConfigData)[11] = aspectScale;
}


/* ═══════════════════════════════════════════════════════════════════════════
 * SetupRenderFiber @ 0x8235AD98 | size: 0x88
 * 
 * Sets up the rendering fiber/thread context for async rendering.
 * ═══════════════════════════════════════════════════════════════════════════ */
void SetupRenderFiber(void* pRootGameObj) {
    uint8_t* pRoot = (uint8_t*)pRootGameObj;
    
    /* Read current vtable pointer */
    uint32_t currentVtable = *(uint32_t*)(pRoot + 0);
    uint32_t targetVtable = *(uint32_t*)(pRoot + 8);
    
    /* Check if vtable needs updating */
    if (currentVtable > targetVtable) {
        /* Advance vtable pointer (implementation detail) */
        /* This appears to be a state machine advancement */
        currentVtable += 4;  /* Move to next vtable entry */
    }
    
    /* Store render mode constants */
    *(uint32_t*)(pRoot + 4) = 1480;   /* Render mode ID */
    *(uint32_t*)(pRoot + 8) = 0x20000; /* Render flags */
    *(uint32_t*)(pRoot + 0) = currentVtable;
    
    /* Read fiber handle from offset +10396 */
    uint32_t fiberHandle = *(uint32_t*)(pRoot + 10396);
    
    /* Initialize TLS fiber */
    _crt_tls_fiber_setup();
    
    /* If fiber handle exists, configure it */
    if (fiberHandle != 0) {
        /* Configure fiber context (implementation specific) */
        /* This would call into fiber management subsystem */
    }
    
    /* Wait for render ready state at offset +10492 */
    uint32_t renderState;
    do {
        renderState = *(uint32_t*)(pRoot + 10492);
    } while (renderState != 0);
}


/* ═══════════════════════════════════════════════════════════════════════════
 * CleanupRenderTargets @ 0x823666F0 | size: 0x94
 * 
 * Cleans up and resets all render targets.
 * ═══════════════════════════════════════════════════════════════════════════ */
void CleanupRenderTargets(void* pRootGameObj) {
    uint8_t* pRoot = (uint8_t*)pRootGameObj;
    
    /* Check if vtable is initialized */
    uint32_t vtable = *(uint32_t*)(pRoot + 0);
    if (vtable != 0) {
        /* Initialize render target state */
        grcRenderTarget_InitState(pRootGameObj, 0);
        
        /* Configure all 4 render target slots */
        for (int i = 0; i < 4; i++) {
            grcRenderTarget_Configure(pRootGameObj, i, 0);
        }
        
        /* Setup render fiber for async operations */
        SetupRenderFiber(pRootGameObj);
    }
    
    /* Free render target at offset +13976 */
    void* pTarget1 = *(void**)(pRoot + 13976);
    if (pTarget1 != NULL) {
        grcRenderTarget_Free(pTarget1);
        *(void**)(pRoot + 13976) = NULL;
    }
    
    /* Free render target at offset +13980 */
    void* pTarget2 = *(void**)(pRoot + 13980);
    if (pTarget2 != NULL) {
        grcRenderTarget_Free(pTarget2);
        *(void**)(pRoot + 13980) = NULL;
    }
    
    /* Free render target at offset +13972 */
    void* pTarget3 = *(void**)(pRoot + 13972);
    if (pTarget3 != NULL) {
        grcRenderTarget_Free(pTarget3);
        *(void**)(pRoot + 13972) = NULL;
    }
}


/* ═══════════════════════════════════════════════════════════════════════════
 * ConfigureRenderTargets @ 0x82366530 | size: 0x1BC
 * 
 * Configures render targets for the current video mode.
 * Returns 1 on success, 0 on failure.
 * ═══════════════════════════════════════════════════════════════════════════ */
int ConfigureRenderTargets(void* pRootGameObj, void* pConfig) {
    uint8_t* pRoot = (uint8_t*)pRootGameObj;
    uint8_t* pCfg = (uint8_t*)pConfig;
    VideoMode videoMode;
    
    /* Read configuration parameters */
    uint32_t vtable = *(uint32_t*)(pCfg + 0);
    uint32_t flags = *(uint32_t*)(pCfg + 4);
    uint32_t formatFlags = *(uint32_t*)(pCfg + 8);
    uint32_t width = *(uint32_t*)(pCfg + 16);
    uint32_t format = *(uint32_t*)(pCfg + 40);
    uint32_t displayMode = *(uint32_t*)(pCfg + 64);
    
    /* Determine target display mode based on format flags */
    uint32_t targetMode = DISPLAY_MODE_STANDARD_262;
    
    if (displayMode == 0) {
        /* Use format flags to determine mode */
        targetMode = formatFlags & 0x3F;  /* Extract lower 6 bits */
    } else if (displayMode == DISPLAY_MODE_STANDARD_390) {
        targetMode = DISPLAY_MODE_STANDARD_262;
    } else if (displayMode == DISPLAY_MODE_STANDARD_438) {
        targetMode = DISPLAY_MODE_STANDARD_310;
    }
    
    /* Query current video mode */
    VdQueryVideoMode(&videoMode);
    
    /* Store video mode info in root game object */
    *(uint32_t*)(pRoot + 20456) = videoMode.displayWidth;
    *(uint32_t*)(pRoot + 20460) = videoMode.displayHeight;
    *(uint32_t*)(pRoot + 20464) = videoMode.displayWidth;
    *(uint32_t*)(pRoot + 20468) = *(uint32_t*)(pCfg + 68);
    
    /* Create primary render target if needed */
    uint32_t createPrimary = *(uint32_t*)(pCfg + 60);
    if (createPrimary == 0) {
        void* pTarget = grcRenderTarget_CreatePrimary(
            (void*)vtable, flags, 1, 1, 0, targetMode, 0, 3, 0
        );
        
        if (pTarget == NULL) {
            return 0;  /* Failure */
        }
        
        *(void**)(pRoot + 13976) = pTarget;
    }
    
    /* Create secondary render target if needed */
    uint32_t createSecondary = *(uint32_t*)(pCfg + 56);
    if (createSecondary == 0) {
        void* pTarget = grcRenderTarget_CreateSecondary(
            (void*)vtable, flags, formatFlags, width, 0
        );
        
        if (pTarget == NULL) {
            return 0;  /* Failure */
        }
        
        *(void**)(pRoot + 13980) = pTarget;
        grcRenderTarget_Configure(pRootGameObj, 0, 0);
    }
    
    /* Create tertiary render target if needed */
    uint32_t createTertiary = *(uint32_t*)(pCfg + 36);
    if (createTertiary != 0) {
        void* pTarget = grcRenderTarget_CreateSecondary(
            (void*)vtable, flags, format, width, 0
        );
        
        if (pTarget == NULL) {
            return 0;  /* Failure */
        }
        
        *(void**)(pRoot + 13972) = pTarget;
        grcRenderTarget_InitState(pRootGameObj, 0);
    }
    
    /* Copy configuration to root game object at offset +13736 */
    memcpy(pRoot + 13736, pConfig, 124);
    
    /* Adjust display mode for hi-def */
    if (targetMode == DISPLAY_MODE_HIDEF_310) {
        targetMode = DISPLAY_MODE_STANDARD_310;
    } else if (targetMode == DISPLAY_MODE_HIDEF_262) {
        targetMode = DISPLAY_MODE_STANDARD_262;
    }
    
    /* Store final display mode */
    *(uint32_t*)(pRoot + 13800) = targetMode;
    
    /* Configure render pipeline */
    grcRenderPipeline_Configure(pRootGameObj, pCfg + 96);
    
    /* Set render ready flag */
    uint8_t renderFlags = *(uint8_t*)(pRoot + 10433);
    renderFlags |= 0x20;  /* Set bit 5 */
    *(uint8_t*)(pRoot + 10433) = renderFlags;
    
    return 1;  /* Success */
}
