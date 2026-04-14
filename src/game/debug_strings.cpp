/**
 * debug_strings.cpp — Debug cheat / console-variable string constants
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Definitions for debug_strings.hpp. Addresses verified from
 * orig/434C4803/default_base.bin via xex_excavation_tt/debug_strings.txt.
 */
#include "game/debug_strings.hpp"

// Character / head / IK debug cvars
const char* g_str_debugFace                  = "debugface";              // @ 0x8202762C
const char* g_str_debugHeadIK                = "debugheadik";            // @ 0x820285FC
const char* g_str_debugHeadXY                = "debugheadxy";            // @ 0x82028608
const char* g_str_debugHeadPosition          = "DebugHeadPosition";      // @ 0x820384AC
const char* g_str_debugHeadOrientation       = "DebugHeadOrientation";   // @ 0x820384C0

// Rendering / shaders / overlays
const char* g_str_debugShaders               = "debugshaders";           // @ 0x820348D4
const char* g_str_drawDebugPrompt            = "Draw Debug:       ?";    // @ 0x82056CC4
const char* g_str_debugFlash                 = "debugflash";             // @ 0x8205AE20
const char* g_str_debugText                  = "debug_text";             // @ 0x8205C80C
const char* g_str_debugBoards                = "debugboards";            // @ 0x8206479C
const char* g_str_debugCrowd                 = "debugcrowd";             // @ 0x8206B528

// Camera / shot debug
const char* g_str_shotPrintDebug             = "shotprintdebug";         // @ 0x82035F2C
const char* g_str_camPrintDebug              = "camprintdebug";          // @ 0x82035F3C
const char* g_str_camMachDebug               = "cammachdebug";           // @ 0x820522D0

// Debug pad / input
const char* g_str_debugPadState              = "DebugPadState";          // @ 0x82037288
const char* g_str_debugPad3State             = "DebugPad3State";         // @ 0x82037298
const char* g_str_debugPad3Movestick         = "DebugPad3Movestick";     // @ 0x820372A8
const char* g_str_debugPad3Lookstick         = "DebugPad3Lookstick";     // @ 0x820372BC

// Sound-system debug
const char* g_str_debugSndSys                = "debugsndsys";            // @ 0x8203AC44
const char* g_str_debugSnd                   = "debugsnd";               // @ 0x8203AC50

// Subsystem-tagged debug channel names
const char* g_str_rlDebug                    = "rlDebug";                // @ 0x82068658
const char* g_str_stpDebug                   = "stpDebug";               // @ 0x82069EE8
const char* g_str_hsmDebug                   = "hsmDebug";               // @ 0x82073E90
const char* g_str_nocsDebug                  = "nocsdebug";              // @ 0x820770D4
const char* g_str_netDebug                   = "netDebug";               // @ 0x82078D04
