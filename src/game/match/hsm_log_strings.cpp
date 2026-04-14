/**
 * hsm_log_strings.cpp — HSM (Hierarchical State Machine) log/debug string constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Log / warning / state-transition strings used by the gmLogic* HSM and by
 * the rage action system.  These are runtime LOG strings, distinct from the
 * RTTI class-name strings handled by match_rtti_strings.cpp.
 *
 * Addresses verified byte-for-byte from orig/434C4803/default_base.bin via
 * xex_excavation_tt/debug_strings.txt.
 *
 * Note: ExitDialog / EnterDialog / TerminateDialog / TerminateAllDialogs
 * (0x82050900, 0x8205090C, 0x820517D0, 0x820517E0) already live in
 * src/game/dialog_strings.cpp and are not redefined here.
 */

// ────────────────────────────────────────────────────────────────────────────
// HSM state-transition log strings (gmLogic* onEnterState debug tags)
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_hsm_gmLogicSinglesMatch_stateReplay_onEnterState =
    "gmLogicSinglesMatch::stateReplay::onEnterState";                             // @ 0x82030BA0
const char* g_str_hsm_gmLogicSwingPractice_stateAim_onEnterState   =
    "gmLogicSwingPractice::stateAim::onEnterState";                               // @ 0x820303B8

// HSM state transition printf format (SetNextLogic / state swap trace)
const char* g_str_hsm_fmt_gmLogicSinglesMatch_nextState =
    "gmLogicSinglesMatch's next state was %s, is %s";                             // @ 0x8203085C

// ────────────────────────────────────────────────────────────────────────────
// gmLogic event / method names (HSM message tags)
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_hsm_setGameLogicType   = "SetGameLogicType";                    // @ 0x820507EC
const char* g_str_hsm_setNextLogic       = "SetNextLogic";                        // @ 0x820502B0

// ────────────────────────────────────────────────────────────────────────────
// HSM debug channel + stack warnings
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_hsm_debugChannel       = "hsmDebug";                            // @ 0x82073E90
const char* g_str_hsm_warn_popEmptyStack =
    "Warning: Attempting to pop object from the stack but nothing is on the stack"; // @ 0x820750E8

// ────────────────────────────────────────────────────────────────────────────
// gmLogicNode class tag (runtime string companion to the RTTI name)
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_hsm_gmLogicNode        = "gmLogicNode";                         // @ 0x820300DC

// ────────────────────────────────────────────────────────────────────────────
// rage action-system warning format strings
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_hsm_warn_actionFallback =
    "Warning: Can't run action '%s'. Using '%s' as a fallback";                   // @ 0x820521A0
const char* g_str_hsm_warn_actionFail     =
    "Warning: Can't run action '%s'";                                             // @ 0x820521DC
