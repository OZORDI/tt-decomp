/**
 * gd_cutscene_strings.hpp — Original binary string constants for cutscene system
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All addresses verified from orig/434C4803/default_base.bin.
 * These strings exist in the binary's .rdata section at the listed addresses.
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// gdCSCharAnimData PostLoadProperties validation strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSCharAnimData_missingFileName;    // @ 0x820789E0
extern const char* g_str_gdCSCharAnimData_negTimeOffset;      // @ 0x82078A28
extern const char* g_str_gdCSCharAnimData_negWeight;          // @ 0x82078A78
extern const char* g_str_gdCSCharAnimData_invalidEmote;       // @ 0x82078AC0

// ────────────────────────────────────────────────────────────────────────────
// gdCSCharAnimNames strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSCharAnimNames_noAnimData;        // @ 0x82078C80
extern const char* g_str_gdCSCharAnimData_boneName;           // @ 0x8203F2E0

// ────────────────────────────────────────────────────────────────────────────
// gdCSCamAnimShotName PostLoadProperties validation strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSCamAnimShotName_negWeight;       // @ 0x82078E38
extern const char* g_str_gdCSCamAnimShotName_missingShotName; // @ 0x82078E80
extern const char* g_str_gdCSCamAnimShotName_negTimeOffset;   // @ 0x82078EC8
extern const char* g_str_gdCSCamAnimShotName_shotNotFound;    // @ 0x82078F18

// ────────────────────────────────────────────────────────────────────────────
// gdCSNameData / gdCutSceneData strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSNameData_missingFileName;        // @ 0x820790F0
extern const char* g_str_gdCSNameData_unknownTrigger;         // @ 0x82079138
extern const char* g_str_gdCutSceneData_unknownSkip;          // @ 0x820791A0

// ────────────────────────────────────────────────────────────────────────────
// gdCSActionIfData PostLoadProperties validation strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSActionIfData_unknownCondition;   // @ 0x82079208
extern const char* g_str_gdCSActionIfData_needsPlayerId;      // @ 0x82079258
extern const char* g_str_gdCSActionIfData_needsPercent;       // @ 0x820792B0
extern const char* g_str_gdCSActionIfData_percentExceeds;     // @ 0x82079308
extern const char* g_str_gdCSActionIfData_needsGameType;      // @ 0x82079358
extern const char* g_str_gdCSActionIfData_invalidGameType;    // @ 0x820793B0
extern const char* g_str_gdCSActionIfData_needsArenaType;     // @ 0x82079408
extern const char* g_str_gdCSActionIfData_invalidArenaType;   // @ 0x82079470

// ────────────────────────────────────────────────────────────────────────────
// gdCSAction*Data PostLoadProperties validation strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSActionCamAnimData_missingFileName;         // @ 0x820794D8
extern const char* g_str_gdCSActionCharAnimData_missingPlayerId;        // @ 0x82079550
extern const char* g_str_gdCSActionCharAnimData_missingFileName;        // @ 0x820795A0
extern const char* g_str_gdCSActionCharVisibleData_missingPlayerId;     // @ 0x820795F8
extern const char* g_str_gdCSActionPlayAudioData_missingAudioType;      // @ 0x82079668
extern const char* g_str_gdCSActionPlayAudioData_missingAudioEvent;     // @ 0x820796C0
extern const char* g_str_gdCSActionPlayAudioData_unknownAudioType;      // @ 0x82079730
extern const char* g_str_gdCSActionPlayAudioData_unknownAudioEventId;   // @ 0x82079790

// ────────────────────────────────────────────────────────────────────────────
// gdCSActionAmb*Data PostLoadProperties validation strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdCSActionLvlAnimData_missingAnimName;         // @ 0x820797E0
extern const char* g_str_gdCSActionLvlAmbAnimData_missingAmbName;       // @ 0x82079840
extern const char* g_str_gdCSActionLvlAmbAnimData_missingAnimName;      // @ 0x82079890
extern const char* g_str_gdCSActionCharAmbAnimData_missingPlayerId;     // @ 0x820798E0
extern const char* g_str_gdCSActionCharAmbAnimData_missingAmbName;      // @ 0x82079930
extern const char* g_str_gdCSActionCharAmbAnimData_missingAnimName;     // @ 0x82079980
