/**
 * globals.c — Global Variables Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

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
