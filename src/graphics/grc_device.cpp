/**
 * graphics/grc_device.cpp — Graphics Device Lifecycle Management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Core graphics device functions:
 *   - grcDevice_shutdown    @ 0x8214FA58 — Primary shutdown sequence
 *   - grcDevice_shutdownAlt @ 0x821540D0 — Alternate shutdown path
 *
 * Note: grcDevice_beginScene lives in grc_render.cpp alongside grcDevice_clear.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── External Dependencies ────────────────────────────────────────────────── */

/* Global singleton pointers */
extern void* g_pHudOverlay;            /* @ 0x825EBAA0 — HUD overlay object */

/* Memory management */
extern void sysMemAllocator_Free(void* ptr);             /* @ 0x820C00C0 — sysMemFree */
extern void rage_free(void* ptr);                        /* @ 0x820C00C0 — rage memory free */

/* Device management functions */
extern void grmShaderGroup_destroy(void* pObj);    /* @ 0x82445BF8 — Destroy shader group (refcounted) */
extern void grcTextureDictionary_destroy(void* pCache); /* @ 0x824456A8 — Destroy texture dictionary */
extern void grcDevice_cleanupInternal(void* pDevice);   /* @ 0x82151068 — Cleanup internal device struct */
extern void grcDevice_releaseSharedResources(void);      /* @ 0x8214F338 — Release shared grc resources */
extern void grcDeviceXenon_teardown(void* pTarget);      /* @ 0x82366CA0 — Full GPU teardown (VdShutdownEngines) */
extern void _locale_register(void* pLocale, uint32_t flags);  /* @ 0x820C02D0 */
extern void thunk_DbgBreakPoint(void);             /* @ 0x82566B70 */
extern void grcDevice_setRenderTarget(void* pDevice, int32_t arg); /* @ 0x82357400 — Set active render target */

/* Forward declaration — defined below */
void grcDevice_shutdownAlt(void);

/* ── Global device-state pointers (extern, defined in globals) ───────────── */

extern uint32_t g_grcDeviceState[100];       /* @ 0x825EBB8C — device state flags array */
extern void*    g_pGrcDeviceObj;             /* @ 0x825EBB94 — primary device object pointer */
extern void*    g_pGrcDeviceAlt;             /* @ 0x825EBB9C — alternate device object pointer */
extern void*    g_pGrcTextureCache;          /* @ 0x825EBB40 — texture cache pointer */
extern void*    g_grcRenderObjects[2];       /* @ 0x82617E60 — render object pair */
extern void*    g_grcRefCountedObjs[4];      /* @ 0x826063E8 — refcounted render objects (2 pairs) */

/* ── Structure Definitions ────────────────────────────────────────────────── */

typedef struct {
    void**   vtable;             /* +0 */
    uint8_t  _pad[4];
    uint16_t m_refCount;         /* +6 */
} RenderObject;


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
    /* 1. Clear device state flag at index 99 (+396). */
    g_grcDeviceState[99] = 0;

    /* Check and clear first state flag; reset render target if it was set. */
    if (g_grcDeviceState[0] != 0) {
        g_grcDeviceState[0] = 0;
        grcDevice_setRenderTarget(g_pGrcDeviceObj, 0);
    }

    /* 2. Destroy shader group and free associated texture data. */
    if (g_grcRenderObjects[0] != NULL) {
        grmShaderGroup_destroy(g_grcRenderObjects[0]);
        g_grcRenderObjects[0] = NULL;
    }

    if (g_grcRenderObjects[1] != NULL) {
        sysMemAllocator_Free(g_grcRenderObjects[1]);
        g_grcRenderObjects[1] = NULL;
    }

    /* 3. Call alternate shutdown path (HUD overlay cleanup). */
    grcDevice_shutdownAlt();

    /* 4. Destroy texture cache. */
    if (g_pGrcTextureCache != NULL) {
        grcTextureDictionary_destroy(g_pGrcTextureCache);
        g_pGrcTextureCache = NULL;
    }

    /* 5. Clean up internal device structures.
     *    These are two adjacent device structs at 0x825EBB98 and 0x825EBBA0. */
    grcDevice_cleanupInternal((void*)((uint8_t*)&g_grcDeviceState[3]));   /* +0x0C from base */
    grcDevice_cleanupInternal((void*)((uint8_t*)&g_grcDeviceState[5]));   /* +0x14 from base */

    /* 6. Release shared graphics resources (global shader/texture refs). */
    grcDevice_releaseSharedResources();

    /* 7. Release refcounted render objects (2 pairs of 2). */
    for (int i = 0; i < 4; i++) {
        RenderObject* pObj = (RenderObject*)g_grcRefCountedObjs[i];
        if (pObj != NULL) {
            pObj->m_refCount--;

            if (pObj->m_refCount == 0) {
                /* Call destructor (vtable slot 0) with flags=1. */
                typedef void (*DtorFn)(void*, int);
                ((DtorFn)pObj->vtable[0])(pObj, 1);
            } else {
                thunk_DbgBreakPoint();  /* Unexpected refcount */
            }

            g_grcRefCountedObjs[i] = NULL;
        }
    }

    /* 8. Release the primary device render target with reference counting. */
    if (g_pGrcDeviceObj != NULL) {
        uint32_t* pTargetData = (uint32_t*)g_pGrcDeviceObj;
        uint32_t refCount = pTargetData[3];  /* +12 */

        if (refCount == 1) {
            grcDeviceXenon_teardown(g_pGrcDeviceObj);

            /* Free locale data (allocation header at -4). */
            void* pLocale = *(void**)((uint8_t*)g_pGrcDeviceObj - 4);
            _locale_register(pLocale, 0x24800000);  /* lis r4, 9344 */

            g_pGrcDeviceObj = NULL;
        } else {
            pTargetData[3] = refCount - 1;

            if (refCount - 1 == 0) {
                thunk_DbgBreakPoint();
            }

            g_pGrcDeviceObj = NULL;
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
    void* pDevice = g_pGrcDeviceAlt;

    if (pDevice != NULL) {
        /* If this device is also the HUD overlay, clear that reference. */
        if (g_pHudOverlay == pDevice) {
            g_pHudOverlay = NULL;
        }

        /* Free the device memory. */
        rage_free(pDevice);
    }

    /* Clear device pointer. */
    g_pGrcDeviceAlt = NULL;
}
