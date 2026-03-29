/**
 * rage/ui_context.c — UI Context Registration System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the UI context registration system used by credits, dialog,
 * leaderboard, and other UI screens. Also includes render mode configuration.
 *
 * Functions:
 *   rage_RegisterUIContext  — registers a UI context with page groups + categories
 *   rage_RenderDebugOverlay — debug FPS overlay (stub in this build)
 *   rage_SetRenderMode      — configures render pipeline and FPS target
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ─────────────────────────────────────────────────────────────────────────────
 * External functions
 * ───────────────────────────────────────────────────────────────────────────── */

/* RAGE UI factory singleton accessor — retrieves the global UI factory. */
extern void rage_GetUIFactory(void* pFactory);

/* Factory build — creates a UI object from the factory using a type name
 * and stack buffer for construction.
 * @ uses UIPageGroup type string internally */
extern void* rageObjectFactory_Build(void* pFactory, void* pStackBuf,
                                     uint32_t bufSize, const char* typeName,
                                     uint32_t flags, const char* nameStr);

/* Texture loader — loads a texture from an XML layout path and fills
 * in width/height dimensions. */
extern void* rage_LoadTexture(const char* path, uint32_t* pWidth,
                               uint32_t* pHeight);

/* grcTextureInfo initialization — constructs a 16-byte texture info
 * struct on the stack with the given dimensions.
 * @ 0x82188190 | size: 0x90 */
extern void grcTextureInfo_Init(void* pTexInfo, uint32_t width, uint32_t height);

/* UIPageGroup texture binding — binds a loaded texture to a page group.
 * @ 0x823F9EA0 | size: 0x10C */
extern void UIPageGroup_BindTexture(void* pPageGroup, void* pTexInfo);

/* grcTextureInfo cleanup — releases any resources held by the texture info.
 * @ 0x82188220 | size: 0x60 */
extern void grcTextureInfo_Cleanup(void* pTexInfo);

/* UI context category query — returns non-zero if the context should
 * create a category entry for the category manager.
 * @ 0x822EB2A8 | size: 0x78 */
extern void* uiContext_ShouldCreateCategory(void* pContext);

/* RAGE heap allocator. */
extern void* rage_alloc(uint32_t size);

/* Category entry creation — creates and initializes a 172-byte category
 * entry from config data fields and a page group.
 * @ 0x823F8D08 | size: 0x2E8 */
extern void* uiCategoryEntry_Create(void* pPageGroup, uint32_t field1,
                                     uint32_t field2, uint32_t field3,
                                     uint32_t field4, uint32_t categoryId);

/* UI context registration notification — notifies the context that
 * registration is complete.
 * @ 0x822EB320 | size: 0x60 */
extern void uiContext_NotifyRegistered(void* pContext);

/* Debug logging (no-op in release builds). */
extern void rage_DebugLog(const char* fmt, ...);

/* CRT snprintf. */
extern int _snprintf(char* buf, size_t size, const char* fmt, ...);

/* ─────────────────────────────────────────────────────────────────────────────
 * External globals
 * ───────────────────────────────────────────────────────────────────────────── */

/* RAGE UI factory singleton (1544 bytes). */
extern void* g_pUIFactory;              /* @ 0x825D0080 */

/* Render device pointer (inside large render device data block). */
extern void* g_pRenderDevice;           /* @ 0x825BF938 */

/* Render flags byte — controls various render mode options. */
extern uint8_t g_renderFlags;           /* @ 0x826063DB */


/* ═══════════════════════════════════════════════════════════════════════════════
 *  rage_RegisterUIContext
 *
 *  @ 0x822EADF8 | size: 0x1CC (460 bytes)
 *
 *  Registers a UI context with the UI management system.  Called by
 *  pongCreditsContext, pongDialogContext, and other UI contexts during
 *  their initialization to set up page groups and register with the
 *  category manager.
 *
 *  UI Context layout:
 *    +0x40  m_pConfigData   — configuration data pointer
 *    +0x58  m_pPageGroup    — UI page group (created if NULL)
 *    +0x5C  m_pCategoryEntry — category manager entry (allocated if NULL)
 *
 *  Config data layout:
 *    +0x00  m_pDisplayName  — pointer to display name string
 *    +0x04  m_nNameLength   — uint16, length of display name
 *    +0x08  config field 1 (uint16)
 *    +0x0C  config field 3 (uint16)
 *    +0x10  config field 2 (uint16)
 *    +0x18  config field 4 (uint16)
 * ═══════════════════════════════════════════════════════════════════════════════ */
void rage_RegisterUIContext(void* pContext, uint32_t categoryId,
                             const char* nameStr)
{
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
    uint8_t* cfg;
    uint16_t field1, field2, field3, field4;
    uint32_t* pFactory;
    uint32_t refCount;

    /* Load config data and page group from context */
    pConfigData = *(void**)(ctx + 0x40);
    pPageGroup = *(void**)(ctx + 0x58);

    /* Determine display name — prefer config data, fall back to nameStr */
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

    /* Create page group if it doesn't exist yet */
    if (pPageGroup == NULL) {
        /* Get UI factory singleton */
        rage_GetUIFactory(g_pUIFactory);

        /* Build XML layout path */
        _snprintf(pathBuf, sizeof(pathBuf), "ui/layouts/%s.xml", displayName);

        /* Build page group via factory */
        pPageGroup = rageObjectFactory_Build(
            g_pUIFactory,
            stackBuf,
            sizeof(stackBuf),
            "UIPageGroup",
            0,
            displayName
        );

        /* Load texture and bind to page group */
        pTexture = rage_LoadTexture(pathBuf, &texWidth, &texHeight);

        if (pPageGroup != NULL) {
            uint8_t texInfo[16];
            grcTextureInfo_Init(texInfo, texWidth, texHeight);
            UIPageGroup_BindTexture(pPageGroup, texInfo);
            grcTextureInfo_Cleanup(texInfo);
        }

        /* Store page group in context */
        *(void**)(ctx + 0x58) = pPageGroup;

        /* Decrement factory reference count */
        pFactory = (uint32_t*)g_pUIFactory;
        refCount = *(uint32_t*)(pFactory + 0x600);
        *(uint32_t*)(pFactory + 0x600) = refCount - 1;
    }

    /* Allocate and initialize category entry if needed */
    pCategoryEntry = *(void**)(ctx + 0x5C);
    if (pCategoryEntry == NULL && pPageGroup != NULL) {
        uint8_t shouldCreate = (uint8_t)(uintptr_t)uiContext_ShouldCreateCategory(pContext);

        if (shouldCreate != 0) {
            /* Allocate 172-byte category entry */
            pCategoryEntry = rage_alloc(172);

            if (pCategoryEntry != NULL) {
                cfg = (uint8_t*)pConfigData;

                field1 = *(uint16_t*)(cfg + 8);
                field2 = *(uint16_t*)(cfg + 16);
                field3 = *(uint16_t*)(cfg + 12);
                field4 = *(uint16_t*)(cfg + 24);

                uiCategoryEntry_Create(pPageGroup, field1, field2,
                                        field3, field4, categoryId);

                *(void**)(ctx + 0x5C) = pCategoryEntry;
            }
        }

        /* Notify context of registration completion */
        uiContext_NotifyRegistered(pContext);
    }

    /* Log registration failure */
    if (pPageGroup == NULL) {
        rage_DebugLog("Failed to register UI context: %s", displayName);
    }
}


/* ═══════════════════════════════════════════════════════════════════════════════
 *  rage_RenderDebugOverlay
 *
 *  @ 0x82228C68 | size: 0x194 (404 bytes)
 *
 *  Renders debug FPS overlay on screen.  Set as a config callback during
 *  initialization.  The full implementation renders debug text twice with
 *  different blend modes for visibility — requires HUD flash system.
 * ═══════════════════════════════════════════════════════════════════════════════ */
void rage_RenderDebugOverlay(void)
{
    /* TODO: Full implementation requires HUD flash system integration.
     * The original renders debug text twice with different blend modes
     * for drop-shadow / visibility. */
    rage_DebugLog("rage_RenderDebugOverlay called");
}


/* ═══════════════════════════════════════════════════════════════════════════════
 *  rage_SetRenderMode
 *
 *  @ 0x8214F9A8 | size: 0xB0 (176 bytes)
 *
 *  Configures the render mode and FPS target.  Called during gameLoop
 *  initialization to set up the rendering pipeline.
 *
 *  Sequence:
 *    1. Check if already initialized (flag at g_pRenderDeviceState+4)
 *    2. Store target FPS and render flags
 *    3. Initialize grcDevice render pipeline:
 *       BeginRenderConfig → InitializeRenderConfig → SetupRenderFiber →
 *       CleanupRenderTargets → ConfigureRenderTargets → FinalizeRenderSetup
 * ═══════════════════════════════════════════════════════════════════════════════ */
void rage_SetRenderMode(int32_t targetFPS, int32_t flags)
{
    extern void* g_pRenderDeviceState;  /* @ 0x825C9A90 */
    extern int32_t g_nTargetFPS;        /* @ 0x825D903C */
    extern void* g_pRenderConfig;       /* @ 0x825EB900 */

    extern void grcDevice_BeginRenderConfig(void);
    extern void grcDevice_InitializeRenderConfig(void);
    extern void grcDevice_SetupRenderFiber(void* pDevice);
    extern void grcDevice_CleanupRenderTargets(void* pDevice);
    extern int  grcDevice_ConfigureRenderTargets(void* pDevice, void* pConfig);
    extern void grcDevice_FinalizeRenderSetup(void);

    uint32_t* pState;
    uint32_t initFlag;
    int result;

    /* Check if already initialized */
    pState = (uint32_t*)g_pRenderDeviceState;
    initFlag = *(uint32_t*)(pState + 1);

    if (initFlag != 0) {
        return;  /* Already initialized */
    }

    /* Store target FPS and render flags */
    g_nTargetFPS = targetFPS;
    g_renderFlags = (uint8_t)flags;

    /* Initialize grcDevice render pipeline if device present */
    if (g_pRenderDevice != NULL) {
        grcDevice_BeginRenderConfig();
        grcDevice_InitializeRenderConfig();
        grcDevice_SetupRenderFiber(g_pRenderDevice);
        grcDevice_CleanupRenderTargets(g_pRenderDevice);

        result = grcDevice_ConfigureRenderTargets(g_pRenderDevice,
                                                   g_pRenderConfig);

        if (result == 0) {
            /* Configuration failed — clean up */
            grcDevice_CleanupRenderTargets(g_pRenderDevice);
        }

        grcDevice_FinalizeRenderSetup();
    }
}
