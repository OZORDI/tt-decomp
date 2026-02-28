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
    struct TemplateRegistryEntry entries[8192];  // Array of template entries (8 bytes each)
    uint32_t count;                              // Number of active templates (at offset +0x10000)
};

extern struct TemplateRegistry g_templateRegistry;  // @ 0x82607C40 (SDA r13+31808)

// ────────────────────────────────────────────────────────────────────────────
// Matrix Comparison System
// ────────────────────────────────────────────────────────────────────────────

// Reference identity matrix for transform comparisons
// @ 0x825CB800 (64 bytes)
extern const float g_identityMatrixRef[16];

// SIMD comparison mask for matrix identity checks
// @ 0x82619BE0 (16 bytes)
extern const uint32_t g_vectorComparisonMask[4];

// ────────────────────────────────────────────────────────────────────────────
// UI Input System
// ────────────────────────────────────────────────────────────────────────────

// UI input enable flag
// @ SDA+25804 (0x826064CC)
extern uint8_t g_uiInputFlag;

#ifdef __cplusplus
}
#endif

// ────────────────────────────────────────────────────────────────────────────
// AI Data System Type Identifiers
// ────────────────────────────────────────────────────────────────────────────

// Global AI data type identifiers used for validation in gdai* classes
// These are used by the RAGE serialization system for type safety
extern uint32_t* g_aiTypeId_1;  // @ 0x825C60B4
extern uint32_t* g_aiTypeId_2;  // @ 0x825C803C
extern uint32_t* g_aiTypeId_3;  // @ 0x825C8038

// ────────────────────────────────────────────────────────────────────────────
// Training Drill Tip Data Validation
// ────────────────────────────────────────────────────────────────────────────

// Shot type validation globals for noSoftShotsTipData
// Used by PostLoadProperties @ 0x821F2678 to validate allowed shot types
extern uint32_t g_shotType1_825C5F50;  // @ 0x825C5F50
extern uint32_t g_shotType2_825C803C;  // @ 0x825C803C
extern uint32_t g_shotType3_825C8038;  // @ 0x825C8038
