/**
 * dialog_strings.cpp — Original binary string constants for the dialog system
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All values verified byte-for-byte from orig/434C4803/default_base.bin.
 * Covers pongDialogState, UnlockDialog/SaveDialog handlers, DialogMenu,
 * and dialog data loading ($/tune/dialogs).
 */

// ────────────────────────────────────────────────────────────────────────────
// Dialog state / event names (state-machine tags)
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_dialog_unlockDialog        = "UnlockDialog";                                // @ 0x82030C78
const char* g_str_dialog_saveDialog          = "SaveDialog";                                  // @ 0x82030C88
const char* g_str_dialog_dialogMenu          = "DialogMenu";                                  // @ 0x8204E33C
const char* g_str_dialog_exitDialog          = "ExitDialog";                                  // @ 0x82050900
const char* g_str_dialog_enterDialog         = "EnterDialog";                                 // @ 0x8205090C
const char* g_str_dialog_terminateDialog     = "TerminateDialog";                             // @ 0x820517D0
const char* g_str_dialog_terminateAllDialogs = "TerminateAllDialogs";                         // @ 0x820517E0
const char* g_str_dialog_Dialog              = "Dialog";                                      // @ 0x8205AFB0
const char* g_str_dialog_pongDialogState     = "pongDialogState";                             // @ 0x8205F230

// ────────────────────────────────────────────────────────────────────────────
// Dialog ID / field names + formatters
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_dialog_dialogID            = "DialogID";                                    // @ 0x820421C8
const char* g_str_dialog_fmt_dialog          = "dialog_%d";                                   // @ 0x82042330
const char* g_str_dialog_fmt_dialogTitle     = "dialog_title_%d";                             // @ 0x8205B0AC

// ────────────────────────────────────────────────────────────────────────────
// Dialog data loading ($/tune/dialogs)
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_dialog_loadingData         = "Loading dialog data...";                      // @ 0x8205AFB8
const char* g_str_dialog_tuneDialogsPath     = "$/tune/dialogs";                              // @ 0x8205AFD0
const char* g_str_dialog_fmt_dialogsFile     = "dialogs_%s";                                  // @ 0x8205AFE0
const char* g_str_dialog_loadDone            = "Dialog data load done...";                    // @ 0x8205B018
const char* g_str_dialog_loadingBox          = "Loading dialog box...";                       // @ 0x8205F240
const char* g_str_dialog_boxLoaded           = "Dialog box loaded.";                          // @ 0x8205F258

// ────────────────────────────────────────────────────────────────────────────
// Dialog menu tracing
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_dialog_fmt_dialogMenuTrace = "       dialog menu: %d";                      // @ 0x8204E53C

// ────────────────────────────────────────────────────────────────────────────
// Dialog errors / warnings
// ────────────────────────────────────────────────────────────────────────────
const char* g_str_dialog_invalidID_reviewTip = "Invalid Dialog ID %d, for review only tip!";  // @ 0x820432D0
const char* g_str_dialog_invalidUnlockMsg    = "Invalid unlock dialog message found: %d for item: %s"; // @ 0x8204CC8C
const char* g_str_dialog_invalidLineCount    = "Invalid line count %d, for dialog %d [%s]";   // @ 0x8205AFEC
const char* g_str_dialog_noDialogsToShow     = "Tried to enter dialog state, but there are no dialogs types to show!"; // @ 0x8205B038
const char* g_str_dialog_createNewMatchAbort = "CreateNewMatch Dialog aborted?";              // @ 0x8205D61C
const char* g_str_dialog_enterFromInvalid    = "Entering dialog state from invalid state %s (%d)"; // @ 0x8205F26C
const char* g_str_dialog_exitToInvalid       = "Exiting dialog state to invalid state %s (%d)";    // @ 0x8205F2A0
const char* g_str_dialog_invalidDialogID     = "Invalid dialog ID[%d]";                       // @ 0x82075E78
const char* g_str_dialog_invalidLines        = "Invalid Lines[%d] for dialog[%d]";            // @ 0x82075E90
