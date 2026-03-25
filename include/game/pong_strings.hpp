/**
 * pong_strings.hpp — Original binary string constants for gameplay systems
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All addresses verified from orig/434C4803/default_base.bin.
 * Strings marked "INVENTED" do NOT exist in the binary and should be
 * replaced with the actual binary string when the correct address is found.
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// pongMover / pongCreature
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_pongMover_calcInitMatrix;            // @ 0x8202A088

// ────────────────────────────────────────────────────────────────────────────
// pongTrainingDrill
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_pongDrill_tooManySuccesses;          // @ 0x82033970

// ────────────────────────────────────────────────────────────────────────────
// gdVibEvent
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdVibEvent_effectNotFound;           // @ 0x8204C0B4

// ────────────────────────────────────────────────────────────────────────────
// Game data loading
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_needToPreloadTemplate;               // @ 0x820311B0

// ────────────────────────────────────────────────────────────────────────────
// SWF / Scaleform
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_swf_unsupportedMethod;               // @ 0x82077054

// ────────────────────────────────────────────────────────────────────────────
// Audio
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_audVoiceSfx_formatChar;              // @ 0x820357AC — "char_%l_%s"
extern const char* g_str_audVoiceSfx_formatSuffix;            // @ 0x820357B3 — "_%s"
extern const char* g_str_audVoiceSfx_formatLevel;             // @ 0x820357B0 — "_%l"
extern const char* g_str_audVoiceSfx_ankle;                   // @ 0x8203581C — "ankle"
