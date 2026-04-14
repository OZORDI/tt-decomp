// pong_logos_strings.cpp
//
// String literals referenced by the Logos screen, Tournament UI, and related
// HUD/dialog subsystems. Addresses are the original .rdata VAs from the Xbox
// 360 XEX build; names follow the g_str_* convention.
//
// Source: xex_excavation_tt/debug_strings.txt

namespace {

// --- Logos state (pongLogosState) ---

// 0x8205C54C - misspelled in the original binary; preserved verbatim.
const char* const g_str_sm_logo_isvisble               = "sm_logo_isvisble";

// 0x8205E478
const char* const g_str_pongLogosState                 = "pongLogosState";

// 0x8205E488
const char* const g_str_loading_logos_screen           = "Loading logos screen...";

// 0x8205E4A0
const char* const g_str_logos_screen_loaded            = "Logos screen loaded";

// 0x8205E4B4
const char* const g_str_entered_logos_invalid_state    = "entered logos state from invalid state %s (%d)";

// 0x8205E4E4
const char* const g_str_exiting_logos_invalid_state    = "exiting logos state to invalid state %s (%d)";

// 0x8205FB60 - logo asset / frontend element id
const char* const g_str_TableTennisLogo                = "TableTennisLogo";

// --- Tournament / match dialogs & warnings ---

// 0x82060550
const char* const g_str_stxt_restart_match_warning     = "stxt_restart_match_warning";

// 0x82050900 - shared with dialog category; single definition here.
const char* const g_str_ExitDialog                     = "ExitDialog";

// 0x8205031C
const char* const g_str_TwoMinuteWarning               = "TwoMinuteWarning";

// 0x82051E04
const char* const g_str_TimerWarning                   = "TimerWarning";

// 0x8203C69C - HUD event/widget id
const char* const g_str_HUD_TIMER_WARNING              = "HUD:TIMER_WARNING";

// 0x820647B4 - leaderboard debug formatter
const char* const g_str_hudLeaderboard_debug_level     = "hudLeaderboard: Debug Level %d";

// 0x8206EDF4
const char* const g_str_TwoMinuteWarningMessage        = "TwoMinuteWarningMessage";

// 0x82070F0C
const char* const g_str_introWaitWarningLimit          = "introWaitWarningLimit";

// --- Debug / AI subsystem ids ---

// 0x82050AE4
const char* const g_str_SwingDebugModeExit             = "SwingDebugModeExit";

// 0x82041BE0
const char* const g_str_gdaiComeBackLogic              = "gdaiComeBackLogic";

// 0x820418FC
const char* const g_str_gdaiMeterLogic                 = "gdaiMeterLogic";

} // namespace
