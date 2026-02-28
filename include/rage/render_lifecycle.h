/**
 * render_lifecycle.h - Rendering lifecycle management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Functions for initializing, configuring, and managing the rendering
 * subsystem lifecycle, including render target setup and fiber management.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * InitializeRenderConfig @ 0x8214F400 | size: 0x248
 * 
 * Initializes global rendering configuration structures.
 * Sets up screen dimensions, display modes, and render timing.
 * 
 * Operations:
 * - Sets default screen resolution (1280x720)
 * - Configures display modes based on widescreen/standard
 * - Initializes render timing structures
 * - Handles non-widescreen configuration overrides
 * 
 * Original symbol: rage_F400
 */
void InitializeRenderConfig(void);

/**
 * SetupRenderFiber @ 0x8235AD98 | size: 0x88
 * 
 * Sets up the rendering fiber/thread context for async rendering.
 * 
 * Parameters:
 *   pRootGameObj - Pointer to root game object
 * 
 * Operations:
 * - Initializes TLS fiber for rendering thread
 * - Waits for render-ready state
 * - Configures async rendering context
 * 
 * Original symbol: rage_AD98
 */
void SetupRenderFiber(void* pRootGameObj);

/**
 * CleanupRenderTargets @ 0x823666F0 | size: 0x94
 * 
 * Cleans up and resets all render targets.
 * 
 * Parameters:
 *   pRootGameObj - Pointer to root game object
 * 
 * Operations:
 * - Frees existing render target resources (3 targets at offsets +13972, +13976, +13980)
 * - Resets render state to default
 * - Calls SetupRenderFiber to reinitialize
 * 
 * Original symbol: rage_66F0
 */
void CleanupRenderTargets(void* pRootGameObj);

/**
 * ConfigureRenderTargets @ 0x82366530 | size: 0x1BC
 * 
 * Configures render targets for the current video mode.
 * 
 * Parameters:
 *   pRootGameObj - Pointer to root game object
 *   pConfig      - Pointer to render configuration structure (124 bytes)
 * 
 * Returns:
 *   1 on success, 0 on failure
 * 
 * Operations:
 * - Queries current video mode (resolution, aspect ratio)
 * - Creates appropriate render targets based on configuration
 * - Handles widescreen vs standard aspect ratios
 * - Stores configuration in root game object at offset +13736
 * 
 * Configuration structure layout:
 *   +0x00  vtable pointer
 *   +0x04  flags
 *   +0x08  format flags
 *   +0x10  width
 *   +0x24  create tertiary flag
 *   +0x28  format
 *   +0x38  create secondary flag
 *   +0x3C  create primary flag
 *   +0x40  display mode
 *   +0x44  additional params
 *   +0x60  pipeline config (28 bytes)
 * 
 * Original symbol: rage_6530
 */
int ConfigureRenderTargets(void* pRootGameObj, void* pConfig);

#ifdef __cplusplus
}
#endif
