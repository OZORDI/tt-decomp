/**
 * match_rtti_strings.cpp — RTTI / typeinfo name strings and debug log strings
 *                          for gmLogicSinglesMatch, gmLogicSwingPractice, and
 *                          gmLogicSpectator HSMs.
 *
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Addresses verified from orig/434C4803/default_base.bin via
 * xex_excavation_tt/debug_strings.txt.
 *
 * RTTI / typeinfo raw mangled names (.?AV...@@) live in .rdata and are the
 * operands of the per-class typeinfo objects used by MSVC __RTDynamicCast
 * and dynamic type queries. They are emitted here so the string refs are
 * visible to the static-recomp indexer alongside the regular log strings.
 *
 * This file is coordinated with pt3 (other gmLogic* subsystems); keep each
 * section self-contained so sections can be merged without overlap.
 */

// ────────────────────────────────────────────────────────────────────────────
// Section 0 — gmLogicSinglesMatch
// ────────────────────────────────────────────────────────────────────────────

// RTTI / typeinfo decorated names (per-state + class)
const char* k_rtti_gmLogicSinglesMatch_statePostTourney                = ".?AVstatePostTourney@gmLogicSinglesMatch@@";                // @ 0x825D3994
const char* k_rtti_gmLogicSinglesMatch_statePostMatch                  = ".?AVstatePostMatch@gmLogicSinglesMatch@@";                  // @ 0x825D39C8
const char* k_rtti_gmLogicSinglesMatch_stateSaveDialog                 = ".?AVstateSaveDialog@gmLogicSinglesMatch@@";                 // @ 0x825D39FC
const char* k_rtti_gmLogicSinglesMatch_stateUnlockDialog               = ".?AVstateUnlockDialog@gmLogicSinglesMatch@@";               // @ 0x825D3A30
const char* k_rtti_gmLogicSinglesMatch_statePostTourneyMovie           = ".?AVstatePostTourneyMovie@gmLogicSinglesMatch@@";           // @ 0x825D3A64
const char* k_rtti_gmLogicSinglesMatch_statePostPostGame               = ".?AVstatePostPostGame@gmLogicSinglesMatch@@";               // @ 0x825D3A9C
const char* k_rtti_gmLogicSinglesMatch_statePostGame                   = ".?AVstatePostGame@gmLogicSinglesMatch@@";                   // @ 0x825D3AD0
const char* k_rtti_gmLogicSinglesMatch_statePostPointCS                = ".?AVstatePostPointCS@gmLogicSinglesMatch@@";                // @ 0x825D3B00
const char* k_rtti_gmLogicSinglesMatch_stateReplay                     = ".?AVstateReplay@gmLogicSinglesMatch@@";                     // @ 0x825D3B34
const char* k_rtti_gmLogicSinglesMatch_stateReplayNegotiation          = ".?AVstateReplayNegotiation@gmLogicSinglesMatch@@";          // @ 0x825D3B64
const char* k_rtti_gmLogicSinglesMatch_stateReplayNegotiationPreSync   = ".?AVstateReplayNegotiationPreSync@gmLogicSinglesMatch@@";   // @ 0x825D3BA0
const char* k_rtti_gmLogicSinglesMatch_statePostPoint                  = ".?AVstatePostPoint@gmLogicSinglesMatch@@";                  // @ 0x825D3BE0
const char* k_rtti_gmLogicSinglesMatch_statePostPointScoringSync       = ".?AVstatePostPointScoringSync@gmLogicSinglesMatch@@";       // @ 0x825D3C14
const char* k_rtti_gmLogicSinglesMatch_statePointScoringNet            = ".?AVstatePointScoringNet@gmLogicSinglesMatch@@";            // @ 0x825D3C50
const char* k_rtti_gmLogicSinglesMatch_statePointScoring               = ".?AVstatePointScoring@gmLogicSinglesMatch@@";               // @ 0x825D3C88
const char* k_rtti_gmLogicSinglesMatch_statePointScoringSync           = ".?AVstatePointScoringSync@gmLogicSinglesMatch@@";           // @ 0x825D3CBC
const char* k_rtti_gmLogicSinglesMatch_stateHelpTips                   = ".?AVstateHelpTips@gmLogicSinglesMatch@@";                   // @ 0x825D3CF4
const char* k_rtti_gmLogicSinglesMatch_stateReturnHit                  = ".?AVstateReturnHit@gmLogicSinglesMatch@@";                  // @ 0x825D3D24
const char* k_rtti_gmLogicSinglesMatch_stateAwaitReturnHit             = ".?AVstateAwaitReturnHit@gmLogicSinglesMatch@@";             // @ 0x825D3D58
const char* k_rtti_gmLogicSinglesMatch_stateAwaitServeReturnHit        = ".?AVstateAwaitServeReturnHit@gmLogicSinglesMatch@@";        // @ 0x825D3D90
const char* k_rtti_gmLogicSinglesMatch_stateRallyBase                  = ".?AVstateRallyBase@gmLogicSinglesMatch@@";                  // @ 0x825D3DCC
const char* k_rtti_gmLogicSinglesMatch_stateLet                        = ".?AVstateLet@gmLogicSinglesMatch@@";                        // @ 0x825D3E00
const char* k_rtti_gmLogicSinglesMatch_stateHitNetServe                = ".?AVstateHitNetServe@gmLogicSinglesMatch@@";                // @ 0x825D3E2C
const char* k_rtti_gmLogicSinglesMatch_stateServeBounce                = ".?AVstateServeBounce@gmLogicSinglesMatch@@";                // @ 0x825D3E60
const char* k_rtti_gmLogicSinglesMatch_stateServeHit                   = ".?AVstateServeHit@gmLogicSinglesMatch@@";                   // @ 0x825D3E94
const char* k_rtti_gmLogicSinglesMatch_stateServeToss                  = ".?AVstateServeToss@gmLogicSinglesMatch@@";                  // @ 0x825D3EC4
const char* k_rtti_gmLogicSinglesMatch_statePreServe                   = ".?AVstatePreServe@gmLogicSinglesMatch@@";                   // @ 0x825D3EF8
const char* k_rtti_gmLogicSinglesMatch_statePreServeSync               = ".?AVstatePreServeSync@gmLogicSinglesMatch@@";               // @ 0x825D3F28
const char* k_rtti_gmLogicSinglesMatch_statePreServeSpectatorSyncState = ".?AVstatePreServeSpectatorSyncState@gmLogicSinglesMatch@@"; // @ 0x825D3F60
const char* k_rtti_gmLogicSinglesMatch_statePrePreServe                = ".?AVstatePrePreServe@gmLogicSinglesMatch@@";                // @ 0x825D3FA4
const char* k_rtti_gmLogicSinglesMatch_stateAnticipationCam            = ".?AVstateAnticipationCam@gmLogicSinglesMatch@@";            // @ 0x825D3FD8
const char* k_rtti_gmLogicSinglesMatch_statePreGame                    = ".?AVstatePreGame@gmLogicSinglesMatch@@";                    // @ 0x825D4010
const char* k_rtti_gmLogicSinglesMatch_statePreMatch                   = ".?AVstatePreMatch@gmLogicSinglesMatch@@";                   // @ 0x825D4040
const char* k_rtti_gmLogicSinglesMatch_statePreMatchSync               = ".?AVstatePreMatchSync@gmLogicSinglesMatch@@";               // @ 0x825D4070
const char* k_rtti_gmLogicSinglesMatch_stateInit                       = ".?AVstateInit@gmLogicSinglesMatch@@";                       // @ 0x825D40A4
const char* k_rtti_gmLogicSinglesMatch_stateCutScene                   = ".?AVstateCutScene@gmLogicSinglesMatch@@";                   // @ 0x825D40D0
const char* k_rtti_gmLogicSinglesMatch_stateBase                       = ".?AVstateBase@gmLogicSinglesMatch@@";                       // @ 0x825D4100
const char* k_rtti_gmLogicSinglesMatch                                 = ".?AVgmLogicSinglesMatch@@";                                 // @ 0x825D412C

// Debug log strings
const char* g_str_gmLogicSinglesMatch_midGameUnknownPPG  = "gmLogicSinglesMatch::MidGame(): Unknown PointsPerGame enum value = %d"; // @ 0x82030780
const char* g_str_gmLogicSinglesMatch_nextStateWas       = "gmLogicSinglesMatch's next state was %s, is %s";                        // @ 0x8203085C
const char* g_str_gmLogicSinglesMatch_stateReplayOnEnter = "gmLogicSinglesMatch::stateReplay::onEnterState";                        // @ 0x82030BA0

// ────────────────────────────────────────────────────────────────────────────
// Section A — gmLogicSwingPractice
// ────────────────────────────────────────────────────────────────────────────

// RTTI / typeinfo decorated names
const char* k_rtti_gmLogicSwingPractice_statePostPoint       = ".?AVstatePostPoint@gmLogicSwingPractice@@";        // @ 0x825D3778
const char* k_rtti_gmLogicSwingPractice_stateReturnHit       = ".?AVstateReturnHit@gmLogicSwingPractice@@";        // @ 0x825D37AC
const char* k_rtti_gmLogicSwingPractice_stateAwaitReturnHit  = ".?AVstateAwaitReturnHit@gmLogicSwingPractice@@";   // @ 0x825D37E0
const char* k_rtti_gmLogicSwingPractice_stateShoot           = ".?AVstateShoot@gmLogicSwingPractice@@";            // @ 0x825D3818
const char* k_rtti_gmLogicSwingPractice_stateAim             = ".?AVstateAim@gmLogicSwingPractice@@";              // @ 0x825D3848
const char* k_rtti_gmLogicSwingPractice_stateMainMenu        = ".?AVstateMainMenu@gmLogicSwingPractice@@";         // @ 0x825D3874
const char* k_rtti_gmLogicSwingPractice_stateInit            = ".?AVstateInit@gmLogicSwingPractice@@";             // @ 0x825D38A8
const char* k_rtti_gmLogicSwingPractice_statePreInit         = ".?AVstatePreInit@gmLogicSwingPractice@@";          // @ 0x825D38D8
const char* k_rtti_gmLogicSwingPractice_stateBase            = ".?AVstateBase@gmLogicSwingPractice@@";             // @ 0x825D3908
const char* k_rtti_gmLogicSwingPractice                      = ".?AVgmLogicSwingPractice@@";                       // @ 0x825D3938

// Debug log strings
const char* g_str_gmLogicSwingPractice_stateAim_onEnter      = "gmLogicSwingPractice::stateAim::onEnterState";     // @ 0x820303B8

// ────────────────────────────────────────────────────────────────────────────
// Section B — gmLogicSpectator
// ────────────────────────────────────────────────────────────────────────────

// RTTI / typeinfo decorated name (class object). Per-state typeinfo strings
// for gmLogicSpectator live at lines 209-228 of debug_strings.txt; the class
// itself is emitted here and the per-state entries follow when merged.
const char* k_rtti_gmLogicSpectator                          = ".?AVgmLogicSpectator@@";                           // @ 0x825D6FB4

// Debug log strings
const char* g_str_gmLogicSpectator_replayNotRecording_serveToss =
    "Replay manager still isn't recording when entering gmLogicSpectator::stateServeToss";                         // @ 0x8206B760

const char* g_str_gmLogicSpectator_stateReplay_bufferNotDone =
    "gmLogicSpectator::stateReplay - replay buffer is not done recording; skipping replay";                        // @ 0x8206B7C0

const char* g_str_gmLogicSpectator_stateReplay_onEnter       =
    "gmLogicSpectator::stateReplay::onEnterState";                                                                  // @ 0x8206B818

const char* g_str_gmLogicSpectator_postPointCS_hostDropped   =
    "MRB- host cxn was reset or someone dropped. Forcing exit from  gmLogicSpectator::statePostPointCS()";         // @ 0x8206B848
