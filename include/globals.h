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

// ────────────────────────────────────────────────────────────────────────────
// Template Registry System
// ────────────────────────────────────────────────────────────────────────────

// Template registry structure @ 0x82607C40 (SDA r13+31808)
// 65616 bytes total: array base + count at offset +0x10000
struct TemplateRegistryEntry {
    uint32_t field1;  // Template data field 1
    uint32_t field2;  // Template data field 2
};

struct TemplateRegistry {
    TemplateRegistryEntry entries[8192];  // Array of template entries (8 bytes each)
    uint32_t count;                       // Number of active templates (at offset +0x10000)
};

extern TemplateRegistry g_templateRegistry;  // @ 0x82607C40 (SDA r13+31808)

// ────────────────────────────────────────────────────────────────────────────
// Matrix Comparison System
// ────────────────────────────────────────────────────────────────────────────

// Reference identity matrix for transform comparisons
// @ 0x825CB800 (64 bytes)
extern const float g_identityMatrixRef[16];

// SIMD comparison mask for matrix identity checks
// @ 0x82619BE0 (16 bytes)
extern const uint32_t g_vectorComparisonMask[4];
