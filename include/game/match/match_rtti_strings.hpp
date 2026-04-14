/**
 * match_rtti_strings.hpp — RTTI typeinfo + gameplay log strings for
 *                          gmLogicSinglesMatch (and siblings). Matches the
 *                          `const char*` convention used in
 *                          src/game/match/match_rtti_strings.cpp.
 *
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The `.?AV...@@` strings are MSVC-mangled typeinfo type descriptor names
 * emitted by the compiler for every polymorphic class. They back
 * RTTI / dynamic_cast lookups. All addresses verified from
 * orig/434C4803/default_base.bin.
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch — RTTI type descriptor strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* k_rtti_gmLogicSinglesMatch_statePostTourney;                // @ 0x825D3994
extern const char* k_rtti_gmLogicSinglesMatch_statePostMatch;                  // @ 0x825D39C8
extern const char* k_rtti_gmLogicSinglesMatch_stateSaveDialog;                 // @ 0x825D39FC
extern const char* k_rtti_gmLogicSinglesMatch_stateUnlockDialog;               // @ 0x825D3A30
extern const char* k_rtti_gmLogicSinglesMatch_statePostTourneyMovie;           // @ 0x825D3A64
extern const char* k_rtti_gmLogicSinglesMatch_statePostPostGame;               // @ 0x825D3A9C
extern const char* k_rtti_gmLogicSinglesMatch_statePostGame;                   // @ 0x825D3AD0
extern const char* k_rtti_gmLogicSinglesMatch_statePostPointCS;                // @ 0x825D3B00
extern const char* k_rtti_gmLogicSinglesMatch_stateReplay;                     // @ 0x825D3B34
extern const char* k_rtti_gmLogicSinglesMatch_stateReplayNegotiation;          // @ 0x825D3B64
extern const char* k_rtti_gmLogicSinglesMatch_stateReplayNegotiationPreSync;   // @ 0x825D3BA0
extern const char* k_rtti_gmLogicSinglesMatch_statePostPoint;                  // @ 0x825D3BE0
extern const char* k_rtti_gmLogicSinglesMatch_statePostPointScoringSync;       // @ 0x825D3C14
extern const char* k_rtti_gmLogicSinglesMatch_statePointScoringNet;            // @ 0x825D3C50
extern const char* k_rtti_gmLogicSinglesMatch_statePointScoring;               // @ 0x825D3C88
extern const char* k_rtti_gmLogicSinglesMatch_statePointScoringSync;           // @ 0x825D3CBC
extern const char* k_rtti_gmLogicSinglesMatch_stateHelpTips;                   // @ 0x825D3CF4
extern const char* k_rtti_gmLogicSinglesMatch_stateReturnHit;                  // @ 0x825D3D24
extern const char* k_rtti_gmLogicSinglesMatch_stateAwaitReturnHit;             // @ 0x825D3D58
extern const char* k_rtti_gmLogicSinglesMatch_stateAwaitServeReturnHit;        // @ 0x825D3D90
extern const char* k_rtti_gmLogicSinglesMatch_stateRallyBase;                  // @ 0x825D3DCC
extern const char* k_rtti_gmLogicSinglesMatch_stateLet;                        // @ 0x825D3E00
extern const char* k_rtti_gmLogicSinglesMatch_stateHitNetServe;                // @ 0x825D3E2C
extern const char* k_rtti_gmLogicSinglesMatch_stateServeBounce;                // @ 0x825D3E60
extern const char* k_rtti_gmLogicSinglesMatch_stateServeHit;                   // @ 0x825D3E94
extern const char* k_rtti_gmLogicSinglesMatch_stateServeToss;                  // @ 0x825D3EC4
extern const char* k_rtti_gmLogicSinglesMatch_statePreServe;                   // @ 0x825D3EF8
extern const char* k_rtti_gmLogicSinglesMatch_statePreServeSync;               // @ 0x825D3F28
extern const char* k_rtti_gmLogicSinglesMatch_statePreServeSpectatorSyncState; // @ 0x825D3F60
extern const char* k_rtti_gmLogicSinglesMatch_statePrePreServe;                // @ 0x825D3FA4
extern const char* k_rtti_gmLogicSinglesMatch_stateAnticipationCam;            // @ 0x825D3FD8
extern const char* k_rtti_gmLogicSinglesMatch_statePreGame;                    // @ 0x825D4010
extern const char* k_rtti_gmLogicSinglesMatch_statePreMatch;                   // @ 0x825D4040
extern const char* k_rtti_gmLogicSinglesMatch_statePreMatchSync;               // @ 0x825D4070
extern const char* k_rtti_gmLogicSinglesMatch_stateInit;                       // @ 0x825D40A4
extern const char* k_rtti_gmLogicSinglesMatch_stateCutScene;                   // @ 0x825D40D0
extern const char* k_rtti_gmLogicSinglesMatch_stateBase;                       // @ 0x825D4100
extern const char* k_rtti_gmLogicSinglesMatch;                                 // @ 0x825D412C

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSinglesMatch — gameplay log / debug strings
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gmLogicSinglesMatch_midGameUnknownPPG;  // @ 0x82030780
extern const char* g_str_gmLogicSinglesMatch_nextStateWas;       // @ 0x8203085C
extern const char* g_str_gmLogicSinglesMatch_stateReplayOnEnter; // @ 0x82030BA0
