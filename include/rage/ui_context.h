/**
 * rage/ui_context.h — UI Context Registration System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rage_RegisterUIContext @ 0x822EADF8 | size: 0x1CC
 * 
 * Registers a UI context (credits, dialog, leaderboard, etc.) with the
 * UI management system. Creates a UI page group, binds it to the context,
 * and registers it with the appropriate category manager.
 * 
 * @param pContext    Pointer to the UI context object (e.g., pongCreditsContext)
 * @param categoryId  Category ID (202=credits, 204=dialog, etc.)
 * @param nameStr     Display name string for the context
 */
void rage_RegisterUIContext(void* pContext, uint32_t categoryId, const char* nameStr);

/**
 * rage_RenderDebugOverlay @ 0x82228C68 | size: 0x194
 * 
 * Renders debug FPS overlay text on screen. Called as a config callback
 * during development builds. Renders the overlay twice with different
 * blend modes for visibility.
 */
void rage_RenderDebugOverlay(void);

/**
 * rage_SetRenderMode @ 0x8214F9A8 | size: 0xB0
 * 
 * Configures the render mode and FPS target. Sets up render device
 * parameters and initializes the rendering pipeline.
 * 
 * @param targetFPS  Target frames per second (typically 30 or 60)
 * @param flags      Render mode flags
 */
void rage_SetRenderMode(int32_t targetFPS, int32_t flags);

#ifdef __cplusplus
}
#endif
