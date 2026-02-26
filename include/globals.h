/**
 * globals.h — Global Variables and Constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Central header for all global variables, pointers, and constants
 * used throughout the game engine.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ────────────────────────────────────────────────────────────────────────────
// Graphics System Globals
// ────────────────────────────────────────────────────────────────────────────

// Global graphics device pointer (grcDevice singleton)
// @ 0x8271A81C (r2-22500)
extern void* g_grcDevice;

// ────────────────────────────────────────────────────────────────────────────
// UI System Globals
// ────────────────────────────────────────────────────────────────────────────

// Global UI manager pointer
// @ 0x8271A358 (r2-23720)
extern void* g_uiManager;

// ────────────────────────────────────────────────────────────────────────────
// Game State Globals
// ────────────────────────────────────────────────────────────────────────────

// Global game object pointer (main game state)
// @ 0x82606604 (r13+26116)
extern void* g_gameObject;

// ────────────────────────────────────────────────────────────────────────────
// Audio System Globals
// ────────────────────────────────────────────────────────────────────────────

// Global audio object pointer
// @ r13+25444
extern void* g_audioObject;

// Global audio config pointer
// @ r13+25448
extern void* g_audioConfig;

// ────────────────────────────────────────────────────────────────────────────
// Display System Globals
// ────────────────────────────────────────────────────────────────────────────

// Global display object pointer
// @ r13+26340
extern void* g_displayObject;

// ────────────────────────────────────────────────────────────────────────────
// Asset System Globals
// ────────────────────────────────────────────────────────────────────────────

// Cached asset path pointer
// @ r13+25836
extern void* g_cachedAssetPath;

#ifdef __cplusplus
}
#endif
