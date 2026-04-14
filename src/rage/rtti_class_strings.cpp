/**
 * rtti_class_strings.cpp — Centralized RTTI / typeinfo mangled class-name
 *                          strings for miscellaneous (non-HSM, non-subsystem-
 *                          specific) classes.
 *
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Addresses verified from orig/434C4803/default_base.bin via
 * xex_excavation_tt/debug_strings.txt.
 *
 * Each string is a raw MSVC-mangled typeinfo name (.?AV...@@) that lives in
 * .rdata and is the operand of a per-class std::type_info object used by
 * __RTDynamicCast and dynamic_cast<>. Emitting them here keeps the string
 * refs visible to the static-recomp indexer for classes whose implementation
 * is scattered or not yet lifted.
 *
 * Grouped by subsystem for readability; all entries use the k_rtti_ prefix.
 */

// ────────────────────────────────────────────────────────────────────────────
// Gameplay logic / AI
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_plrSwingLogic           = ".?AVplrSwingLogic@@";                        // @ 0x825D6FD4
const char* k_rtti_gdaiComeBackLogic       = ".?AVgdaiComeBackLogic@@";                    // @ 0x825D820C
const char* k_rtti_gdaiMeterLogic          = ".?AVgdaiMeterLogic@@";                       // @ 0x825D82DC

// ────────────────────────────────────────────────────────────────────────────
// Logical clip data manager (gd cutscene/clip subsystem)
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_gdLogicalClipDataMgr              = ".?AVgdLogicalClipDataMgr@@";                    // @ 0x825D7848
const char* k_rtti_atSingleton_gdLogicalClipDataMgr  = ".?AV?$atSingleton@VgdLogicalClipDataMgr@@@@";   // @ 0x825D786C

// ────────────────────────────────────────────────────────────────────────────
// Dialog manager
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_dialogManager                     = ".?AVdialogManager@@";                           // @ 0x825DE5A4
const char* k_rtti_atSingleton_dialogManager         = ".?AV?$atSingleton@VdialogManager@@@@";          // @ 0x825DE5C0
const char* k_rtti_dialogData                        = ".?AVdialogData@@";                              // @ 0x825E5C40

// ────────────────────────────────────────────────────────────────────────────
// Logic network client
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_LogicNetworkClient                = ".?AVLogicNetworkClient@@";                      // @ 0x825DEE24

// ────────────────────────────────────────────────────────────────────────────
// Front-end: logos / dialog context+state (pong UI flow)
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_pongLogosContext                  = ".?AVpongLogosContext@@";                        // @ 0x825DEF9C
const char* k_rtti_pongLogosState                    = ".?AVpongLogosState@@";                          // @ 0x825DEFBC
const char* k_rtti_pongDialogContext                 = ".?AVpongDialogContext@@";                       // @ 0x825DF174
const char* k_rtti_pongDialogState                   = ".?AVpongDialogState@@";                         // @ 0x825DF194

// ────────────────────────────────────────────────────────────────────────────
// HUD screens
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_hudLogosScreen                    = ".?AVhudLogosScreen@@";                          // @ 0x825DF368
const char* k_rtti_hudDialog                         = ".?AVhudDialog@@";                               // @ 0x825DF504

// ────────────────────────────────────────────────────────────────────────────
// Network messages
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_pongNetMessageHolder_TwoMinuteWarning =
    ".?AV?$pongNetMessageHolder@VTwoMinuteWarningMessage@@$01@@";                                       // @ 0x825E15B8
const char* k_rtti_TwoMinuteWarningMessage           = ".?AVTwoMinuteWarningMessage@@";                 // @ 0x825E2854

// ────────────────────────────────────────────────────────────────────────────
// Scene graph / debug draw
// ────────────────────────────────────────────────────────────────────────────

const char* k_rtti_sgTraverseDrawDebug               = ".?AVsgTraverseDrawDebug@@";                     // @ 0x825E38DC
