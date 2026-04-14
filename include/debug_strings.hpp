/**
 * debug_strings.hpp — Master index for original-binary string constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * This is the single entry point for any TU that wants to reference a
 * debug / RTTI / log / format string pulled out of the original
 * orig/434C4803/default_base.bin image.  Rather than each consumer hunting
 * down the right per-subsystem header, #include this file and every
 * extern we have excavated is visible.
 *
 * ─── Naming convention ──────────────────────────────────────────────────────
 *   g_str_<subsystem>_<purpose>
 *       Runtime log / debug / format / identifier strings.
 *       Declared as `extern const char*` (pointer-to-const-char). Some
 *       TUs define them as `const char* const` — that still matches
 *       `extern const char*` at link time because the *pointee* is const
 *       either way; the top-level const is a TU-local property.
 *
 *   k_rtti_<class>[_<member>]
 *       MSVC-mangled RTTI typeinfo type-descriptor strings
 *       (the raw `.?AV...@@` operands stored in .rdata that back
 *       __RTDynamicCast / typeid()).
 *       Declared as `extern const char[]` so the address of the array
 *       matches the address of the string literal in the original binary.
 *
 *   k_str_<purpose>
 *       File-scope const char arrays that don't logically belong to a
 *       named subsystem class (e.g. GPU register block names in
 *       graphics/grc_debug_strings.cpp).  These are usually TU-internal
 *       and NOT re-declared here — include the owning TU's own header if
 *       one exists, or reference them via local extern declarations.
 *
 * ─── Source of truth ────────────────────────────────────────────────────────
 * Every address referenced from the headers below was verified
 * byte-for-byte against orig/434C4803/default_base.bin via the excavation
 * dumps under xex_excavation_tt/ (debug_strings.txt, assert_strings.txt,
 * format_strings.txt, all_strings_with_addrs.txt, …).
 *
 * ─── Layout ─────────────────────────────────────────────────────────────────
 * 1. Per-subsystem headers that already exist under include/ — simply
 *    included here.
 * 2. Forward declarations for subsystems whose strings currently live in
 *    a .cpp-only module with no dedicated header.  Consumers that want
 *    to reference one of those symbols can pick it up transitively from
 *    this master header.
 *
 * When a new per-subsystem strings module is added, prefer creating a
 * matching header under include/ and wiring it in via section 1 below
 * rather than extending section 2.
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// Section 1 — Per-subsystem string headers (preferred; always include here)
// ────────────────────────────────────────────────────────────────────────────
#include "game/debug_strings.hpp"            // debug / console-var cvars
#include "game/pong_strings.hpp"             // misc gameplay (pongMover, drills, …)
#include "game/gd_data_strings.hpp"          // gdRivalryData / gdTier / gdLadder*
#include "game/gd_cutscene_strings.hpp"      // gdCS* cutscene PostLoadProperties msgs
#include "game/match/match_rtti_strings.hpp" // gmLogicSinglesMatch RTTI + logs

// ────────────────────────────────────────────────────────────────────────────
// Section 2 — Forward declarations for header-less string modules
//
// These externs correspond to definitions in .cpp files that currently
// have no public header.  They are gathered here so a consumer that only
// needs one or two strings from these modules doesn't have to declare its
// own externs at every call site.
//
// If any of these groups grows substantially, lift it into its own
// include/<path>/<name>_strings.hpp and #include it in Section 1.
// ────────────────────────────────────────────────────────────────────────────

// ── src/game/dialog_strings.cpp ─────────────────────────────────────────────
// Dialog system (pongDialogState, UnlockDialog / SaveDialog, DialogMenu,
// dialog data loading under $/tune/dialogs).
extern const char* g_str_dialog_unlockDialog;           // @ 0x82030C78
extern const char* g_str_dialog_saveDialog;             // @ 0x82030C88
extern const char* g_str_dialog_dialogMenu;             // @ 0x8204E33C
extern const char* g_str_dialog_exitDialog;             // @ 0x82050900
extern const char* g_str_dialog_enterDialog;            // @ 0x8205090C
extern const char* g_str_dialog_terminateDialog;        // @ 0x820517D0
extern const char* g_str_dialog_terminateAllDialogs;    // @ 0x820517E0
extern const char* g_str_dialog_Dialog;                 // @ 0x8205AFB0
extern const char* g_str_dialog_pongDialogState;        // @ 0x8205F230
extern const char* g_str_dialog_dialogID;               // @ 0x820421C8
extern const char* g_str_dialog_fmt_dialog;             // @ 0x82042330
extern const char* g_str_dialog_fmt_dialogTitle;        // @ 0x8205B0AC
extern const char* g_str_dialog_loadingData;            // @ 0x8205AFB8
extern const char* g_str_dialog_tuneDialogsPath;        // @ 0x8205AFD0
extern const char* g_str_dialog_fmt_dialogsFile;        // @ 0x8205AFE0
extern const char* g_str_dialog_loadDone;               // @ 0x8205B018
extern const char* g_str_dialog_loadingBox;             // @ 0x8205F240
extern const char* g_str_dialog_boxLoaded;              // @ 0x8205F258
extern const char* g_str_dialog_fmt_dialogMenuTrace;    // @ 0x8204E53C
extern const char* g_str_dialog_invalidID_reviewTip;    // @ 0x820432D0
extern const char* g_str_dialog_invalidUnlockMsg;       // @ 0x8204CC8C
extern const char* g_str_dialog_invalidLineCount;       // @ 0x8205AFEC
extern const char* g_str_dialog_noDialogsToShow;        // @ 0x8205B038
extern const char* g_str_dialog_createNewMatchAbort;    // @ 0x8205D61C
extern const char* g_str_dialog_enterFromInvalid;       // @ 0x8205F26C
extern const char* g_str_dialog_exitToInvalid;          // @ 0x8205F2A0
extern const char* g_str_dialog_invalidDialogID;        // @ 0x82075E78
extern const char* g_str_dialog_invalidLines;           // @ 0x82075E90

// ── src/game/pong_logos_strings.cpp ─────────────────────────────────────────
// Logos screen, tournament UI, HUD warnings.  (Note the original binary
// misspells "sm_logo_isvisble" — preserved verbatim.)
extern const char* const g_str_sm_logo_isvisble;
extern const char* const g_str_pongLogosState;
extern const char* const g_str_loading_logos_screen;
extern const char* const g_str_logos_screen_loaded;
extern const char* const g_str_entered_logos_invalid_state;
extern const char* const g_str_exiting_logos_invalid_state;
extern const char* const g_str_TableTennisLogo;
extern const char* const g_str_stxt_restart_match_warning;
extern const char* const g_str_ExitDialog;
extern const char* const g_str_TwoMinuteWarning;
extern const char* const g_str_TimerWarning;
extern const char* const g_str_HUD_TIMER_WARNING;
extern const char* const g_str_hudLeaderboard_debug_level;
extern const char* const g_str_TwoMinuteWarningMessage;
extern const char* const g_str_introWaitWarningLimit;

// ── src/game/data/gd_logic_strings.cpp ──────────────────────────────────────
// gmLogic / gmLogicNode RTTI + identifier strings, logical clip data loader,
// gdai*Logic node type tags, storage / logging debug tokens.
extern const char* const g_str_gmLogicNode;
extern const char* const g_str_rtti_gmLogicNode;
extern const char* const g_str_rtti_gmLogic;
extern const char* const g_str_rtti_gmLogic_stateBase;
extern const char* const g_str_logicalClipData_loading;
extern const char* const g_str_logicalClipData_loadDone;
extern const char* const g_str_gdaiComeBackLogic;
extern const char* const g_str_gdaiMeterLogic;
extern const char* const g_str_stpDebug;
extern const char* const g_str_loggingEnabled_intro;
extern const char* const g_str_loggingEnabled_core;
extern const char* const g_str_logopen;

// ── src/game/match/hsm_log_strings.cpp ──────────────────────────────────────
// HSM (hierarchical state machine) log / warning / state-transition strings.
extern const char* g_str_hsm_gmLogicSinglesMatch_stateReplay_onEnterState;
extern const char* g_str_hsm_gmLogicSwingPractice_stateAim_onEnterState;
extern const char* g_str_hsm_fmt_gmLogicSinglesMatch_nextState;
extern const char* g_str_hsm_setGameLogicType;          // @ 0x820507EC
extern const char* g_str_hsm_setNextLogic;              // @ 0x820502B0
extern const char* g_str_hsm_debugChannel;              // @ 0x82073E90 (alias of g_str_hsmDebug)
extern const char* g_str_hsm_warn_popEmptyStack;
extern const char* g_str_hsm_gmLogicNode;               // @ 0x820300DC
extern const char* g_str_hsm_warn_actionFallback;
extern const char* g_str_hsm_warn_actionFail;

// ── src/game/network/xonline_strings.cpp ────────────────────────────────────
// Xbox Live XONLINE_E_* / XONLINE_S_* HRESULT name table. The individual
// name strings live in an anonymous namespace inside the TU; the only
// public entry point is the lookup helper (declared in the xam network
// headers rather than here).

// ── src/graphics/grc_debug_strings.cpp ──────────────────────────────────────
// Xenos GPU register block names and D3D hang-diagnostic messages. Scoped
// to namespace rage and defined with internal linkage-friendly const char
// arrays; consumers should reference them through the grc/D3D diagnostic
// helpers in the graphics subsystem rather than directly from here.
