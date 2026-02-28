/**
 * globals.c — Global Variables Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <stddef.h>
#include "globals.h"

// Graphics System
void* g_grcDevice = (void*)0x8271A81C;

// UI System
void* g_uiManager = (void*)0x8271A358;

// Game State
void* g_gameObject = (void*)0x82606604;

// Audio System
void* g_audioObject = NULL;
void* g_audioConfig = NULL;

// Display System
void* g_displayObject = NULL;

// Asset System
void* g_cachedAssetPath = NULL;

/* grcDevice instances — the two device pointers used by the rendering pipeline */
void* g_pGrcDevice_beginScene = (void*)0x8271A81C;  /* full device for beginScene */
void* g_pGrcDevice_clear      = (void*)0x8271A320;  /* compact clear-queue device */

/* rage::cmSampleCamActions global pointer (gate in beginScene) */
void* g_pCamActionSys = NULL;           /* @ 0x8271A344, set at runtime */

/* Camera actions flags gate word (inside lbl_825D07CC + 4) */
uint32_t g_pCamActionsFlags = 0;        /* @ 0x825D07D0 */

/* Default viewport rectangle used to reset grcDeviceClear state */
const uint8_t g_defaultViewportRect[16] = { 0 };  /* @ 0x8261A0C0 */

// Template Registry System
// @ 0x82607C40 (SDA r13+31808)
struct TemplateRegistry g_templateRegistry = {0};

// Matrix Comparison System
// Reference identity matrix @ 0x825CB800
const float g_identityMatrixRef[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

// SIMD comparison mask @ 0x82619BE0
// This mask is applied during matrix comparisons to ignore certain components
const uint32_t g_vectorComparisonMask[4] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

// UI Input System
// @ SDA+25804 (0x826064CC) - UI input enable flag
uint8_t g_uiInputFlag = 0;
