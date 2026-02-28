/**
 * rage/ui_context.c — UI Context Registration System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements the UI context registration system used by credits, dialog,
 * leaderboard, and other UI screens. Also includes debug overlay rendering
 * and render mode configuration.
 */

#include "rage/ui_context.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Forward declarations for external functions */
extern void rage::GetFactory(void* pFactory);
extern void* rageObjectFactory_Build(void* pFactory, void* pStackBuf, 
                                          uint32_t bufSize, const char* typeName,
                                          uint32_t flags, const char* nameStr);
extern void* rage_LoadTexture(const char* path, uint32_t* pWidth, uint32_t* pHeight);
extern void ke_8190(void* pDest, uint32_t width, uint32_t height);
extern void game_9EA0(void* pPageGroup, void* pTexInfo);
extern void ke_8220(void* pTexInfo);
extern void* SinglesNetworkClient_B2A8_g(void* pContext);
extern void* xe_EC88(uint32_t size);
extern void* game_8D08(void* pPageGroup, uint32_t field1, uint32_t field2,
                       uint32_t field3, uint32_t field4, uint32_t field5);
extern void SinglesNetworkClient_B320_g(void* pContext);
extern void nop_8240E6D0(const char* fmt, ...);
extern int _snprintf(char* buf, size_t size, const char* fmt, ...);

/* External globals */
extern void* g_pUIFactory;              /* @ 0x825D0080 */
extern void* g_pRenderDevice;           /* @ 0x825BF938 */
extern uint8_t g_renderFlags;           /* @ 0x826063DB */

/**
 * rage_RegisterUIContext @ 0x822EADF8 | size: 0x1CC
 * 
 * Original symbol: rage_ADF8
 * 
 * Registers a UI context with the UI management system. This is called by
 * pongCreditsContext, pongDialogContext, and other UI contexts during their
 * initialization to set up their page groups and register with the category
 * manager.
 */
void rage_RegisterUIContext(void* pContext, uint32_t categoryId, const char* nameStr) {
    uint8_t* ctx = (uint8_t*)pContext;
    void* pConfigData;
    void* pPageGroup;
    const char* displayName;
    uint16_t nameLen;
    char stackBuf[192];
    char pathBuf[256];
    uint32_t texWidth = 0;
    uint32_t texHeight = 0;
    void* pTexture;
    void* pCategoryEntry;
    uint8_t* entry;
    uint8_t* cfg;
    uint16_t field1, field2, field3, field4;
    uint32_t* pFactory;
    uint32_t refCount;
    
    /* Load config data and page group from context */
    pConfigData = *(void**)(ctx + 0x40);
    pPageGroup = *(void**)(ctx + 0x58);
    
    /* Determine display name */
    displayName = "";
    if (pConfigData != NULL) {
        nameLen = *(uint16_t*)((uint8_t*)pConfigData + 4);
        if (nameLen > 0) {
            displayName = (const char*)*(void**)pConfigData;
        }
    }
    if (nameStr != NULL && displayName[0] == '\0') {
        displayName = nameStr;
    }
    
    /* Create page group if it doesn't exist */
    if (pPageGroup == NULL) {
        /* Get UI factory singleton */
        rage::GetFactory(g_pUIFactory);
        
        /* Build page group via factory */
        _snprintf(pathBuf, sizeof(pathBuf), "ui/layouts/%s.xml", displayName);
        
        pPageGroup = rageObjectFactory_Build(
            g_pUIFactory,
            stackBuf,
            sizeof(stackBuf),
            "UIPageGroup",
            0,
            displayName
        );
        
        /* Load texture dimensions */
        pTexture = rage_LoadTexture(pathBuf, &texWidth, &texHeight);
        
        if (pPageGroup != NULL) {
            /* Initialize texture info structure on stack */
            uint8_t texInfo[16];
            ke_8190(texInfo, texWidth, texHeight);
            
            /* Bind texture to page group */
            game_9EA0(pPageGroup, texInfo);
            
            /* Clean up texture info */
            ke_8220(texInfo);
        }
        
        /* Store page group in context */
        *(void**)(ctx + 0x58) = pPageGroup;
        
        /* Decrement factory reference count */
        pFactory = (uint32_t*)g_pUIFactory;
        refCount = *(uint32_t*)(pFactory + 0x600);
        *(uint32_t*)(pFactory + 0x600) = refCount - 1;
    }
    
    /* Allocate and initialize category entry */
    pCategoryEntry = *(void**)(ctx + 0x5C);
    if (pCategoryEntry == NULL && pPageGroup != NULL) {
        /* Check if we should create category entry */
        uint8_t shouldCreate = (uint8_t)SinglesNetworkClient_B2A8_g(pContext);
        
        if (shouldCreate != 0) {
            /* Allocate 172-byte category entry */
            pCategoryEntry = xe_EC88(172);
            
            if (pCategoryEntry != NULL) {
                /* Initialize category entry with config data fields */
                entry = (uint8_t*)pCategoryEntry;
                cfg = (uint8_t*)pConfigData;
                
                field1 = *(uint16_t*)(cfg + 8);
                field2 = *(uint16_t*)(cfg + 16);
                field3 = *(uint16_t*)(cfg + 12);
                field4 = *(uint16_t*)(cfg + 24);
                
                /* Create category entry via game_8D08 */
                game_8D08(pPageGroup, field1, field2, field3, field4, categoryId);
                
                *(void**)(ctx + 0x5C) = pCategoryEntry;
            }
        }
        
        /* Notify context of completion */
        SinglesNetworkClient_B320_g(pContext);
    }
    
    /* Log registration failure if needed */
    if (pPageGroup == NULL) {
        nop_8240E6D0("Failed to register UI context: %s", displayName);
    }
}

/**
 * rage_RenderDebugOverlay @ 0x82228C68 | size: 0x194
 * 
 * Original symbol: rage_8C68
 * 
 * Renders debug FPS overlay on screen. This is set as a config callback
 * (g_configCallback) during initialization and is called to render
 * performance statistics.
 */
void rage_RenderDebugOverlay(void) {
    /* This function renders debug text twice with different blend modes
     * for better visibility. The actual implementation calls into the
     * HUD flash system and text rendering pipeline. */
    
    /* TODO: Full implementation requires HUD flash system integration */
    nop_8240E6D0("rage_RenderDebugOverlay called");
}

/**
 * rage_SetRenderMode @ 0x8214F9A8 | size: 0xB0
 * 
 * Original symbol: rage_F9A8
 * 
 * Configures the render mode and FPS target. Called during gameLoop
 * initialization to set up the rendering pipeline.
 */
void rage_SetRenderMode(int32_t targetFPS, int32_t flags) {
    extern void* g_pRenderDeviceState;  /* @ 0x825C9A90 */
    extern int32_t g_nTargetFPS;        /* @ 0x825D903C */
    extern void grcDevice_BeginRenderConfig(void);
    extern void grcDevice_InitializeRenderConfig(void);
    extern void grcDevice_SetupRenderFiber(void* pDevice);
    extern void grcDevice_CleanupRenderTargets(void* pDevice);
    extern int grcDevice_ConfigureRenderTargets(void* pDevice, void* pConfig);
    extern void grcDevice_FinalizeRenderSetup(void);
    
    uint32_t* pState;
    uint32_t initFlag;
    int result;
    extern void* g_pRenderConfig;  /* @ 0x825EB900 */
    
    /* Check if already initialized */
    pState = (uint32_t*)g_pRenderDeviceState;
    initFlag = *(uint32_t*)(pState + 1);
    
    if (initFlag != 0) {
        /* Already initialized - early return */
        return;
    }
    
    /* Store target FPS */
    g_nTargetFPS = targetFPS;
    
    /* Store render flags */
    g_renderFlags = (uint8_t)flags;
    
    /* Initialize render device if present */
    if (g_pRenderDevice != NULL) {
        grcDevice_BeginRenderConfig();
        grcDevice_InitializeRenderConfig();
        grcDevice_SetupRenderFiber(g_pRenderDevice);
        grcDevice_CleanupRenderTargets(g_pRenderDevice);
        
        /* Configure render targets */
        result = grcDevice_ConfigureRenderTargets(g_pRenderDevice, g_pRenderConfig);
        
        if (result == 0) {
            /* Configuration failed - clean up */
            grcDevice_CleanupRenderTargets(g_pRenderDevice);
        }
        
        grcDevice_FinalizeRenderSetup();
    }
}
