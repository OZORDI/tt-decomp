/**
 * gd_logic_strings.cpp — Game logic / loading / storage debug string constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All string values verified byte-for-byte from orig/434C4803/default_base.bin.
 *
 * Covers:
 *   - gmLogic / gmLogicNode class-name strings (RTTI + identifier)
 *   - Logical clip data loader status messages
 *   - gdai*Logic node type identifiers
 *   - Storage / loading / logging debug tokens
 */

// --------------------------------------------------------------------------
// gmLogic hierarchy — class/RTTI identifiers
// --------------------------------------------------------------------------

// 0x820300DC — plain class-name token used by gmLogicNode
const char* const g_str_gmLogicNode                  = "gmLogicNode";

// 0x825D3744 — RTTI type descriptor for class gmLogicNode
const char* const g_str_rtti_gmLogicNode             = ".?AVgmLogicNode@@";

// 0x825D3760 — RTTI type descriptor for class gmLogic
const char* const g_str_rtti_gmLogic                 = ".?AVgmLogic@@";

// 0x825D395C — RTTI type descriptor for gmLogic::stateBase (nested state class)
const char* const g_str_rtti_gmLogic_stateBase       = ".?AVstateBase@gmLogic@@";

// --------------------------------------------------------------------------
// Logical clip data loader — debug progress messages
// --------------------------------------------------------------------------

// 0x82049610
const char* const g_str_logicalClipData_loading     = "loading logical clip data...";

// 0x8204963C
const char* const g_str_logicalClipData_loadDone    = "logical clip data load done.";

// --------------------------------------------------------------------------
// gdai*Logic — AI logic node type identifiers
// --------------------------------------------------------------------------

// 0x82041BE0 — "come back" AI logic node (recovery/return behaviour)
const char* const g_str_gdaiComeBackLogic            = "gdaiComeBackLogic";

// 0x820418FC — meter-driven AI logic node
const char* const g_str_gdaiMeterLogic               = "gdaiMeterLogic";

// --------------------------------------------------------------------------
// Storage / intro / logging subsystem tokens
// --------------------------------------------------------------------------

// 0x82069EE8 — storage-package debug category tag
const char* const g_str_stpDebug                     = "stpDebug";

// 0x82070F0C — intro-sequence wait-warning limit property name
const char* const g_str_introWaitWarningLimit        = "introWaitWarningLimit";

// 0x82074E54 — logging-enabled property (intro/boot context)
const char* const g_str_loggingEnabled_intro         = "loggingEnabled";

// 0x8203CD00 — logging-enabled property (core/global context)
const char* const g_str_loggingEnabled_core          = "loggingEnabled";

// 0x8204E82C — log-open command / hook token
const char* const g_str_logopen                      = "logopen";
