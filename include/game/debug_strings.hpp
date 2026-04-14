/**
 * debug_strings.hpp — Debug cheat / console-variable string constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All addresses verified from orig/434C4803/default_base.bin.
 * These strings back the engine's debug console variables (console-var-style
 * toggles read at runtime to enable on-screen debug overlays, IK probes,
 * sound-system instrumentation, network tracing, etc.).
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// Character / head / IK debug cvars
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_debugFace;                  // @ 0x8202762C — "debugface"
extern const char* g_str_debugHeadIK;                // @ 0x820285FC — "debugheadik"
extern const char* g_str_debugHeadXY;                // @ 0x82028608 — "debugheadxy"
extern const char* g_str_debugHeadPosition;          // @ 0x820384AC — "DebugHeadPosition"
extern const char* g_str_debugHeadOrientation;       // @ 0x820384C0 — "DebugHeadOrientation"

// ────────────────────────────────────────────────────────────────────────────
// Rendering / shaders / overlays
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_debugShaders;               // @ 0x820348D4 — "debugshaders"
extern const char* g_str_drawDebugPrompt;            // @ 0x82056CC4 — "Draw Debug:       ?"
extern const char* g_str_debugFlash;                 // @ 0x8205AE20 — "debugflash"
extern const char* g_str_debugText;                  // @ 0x8205C80C — "debug_text"
extern const char* g_str_debugBoards;                // @ 0x8206479C — "debugboards"
extern const char* g_str_debugCrowd;                 // @ 0x8206B528 — "debugcrowd"

// ────────────────────────────────────────────────────────────────────────────
// Camera / shot debug
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_shotPrintDebug;             // @ 0x82035F2C — "shotprintdebug"
extern const char* g_str_camPrintDebug;              // @ 0x82035F3C — "camprintdebug"
extern const char* g_str_camMachDebug;               // @ 0x820522D0 — "cammachdebug"

// ────────────────────────────────────────────────────────────────────────────
// Debug pad / input
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_debugPadState;              // @ 0x82037288 — "DebugPadState"
extern const char* g_str_debugPad3State;             // @ 0x82037298 — "DebugPad3State"
extern const char* g_str_debugPad3Movestick;         // @ 0x820372A8 — "DebugPad3Movestick"
extern const char* g_str_debugPad3Lookstick;         // @ 0x820372BC — "DebugPad3Lookstick"

// ────────────────────────────────────────────────────────────────────────────
// Sound-system debug
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_debugSndSys;                // @ 0x8203AC44 — "debugsndsys"
extern const char* g_str_debugSnd;                   // @ 0x8203AC50 — "debugsnd"

// ────────────────────────────────────────────────────────────────────────────
// Subsystem-tagged debug channel names (rage:: subsystems)
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_rlDebug;                    // @ 0x82068658 — "rlDebug"
extern const char* g_str_stpDebug;                   // @ 0x82069EE8 — "stpDebug"
extern const char* g_str_hsmDebug;                   // @ 0x82073E90 — "hsmDebug"
extern const char* g_str_nocsDebug;                  // @ 0x820770D4 — "nocsdebug"
extern const char* g_str_netDebug;                   // @ 0x82078D04 — "netDebug"
