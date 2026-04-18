/**
 * char_view_globals.cpp — Character View System Global Variables Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Definitions for global variables used by the character view system.
 * These map to specific addresses in the original Xbox 360 binary.
 */

#include "game/char_view_globals.hpp"

// ────────────────────────────────────────────────────────────────────────────
// Thread-Local Storage
// ────────────────────────────────────────────────────────────────────────────

// TLS base pointer (r13 register on PowerPC)
void** g_tls_base = (void**)0x82600000;

// ────────────────────────────────────────────────────────────────────────────
// UI System Globals (Attract Mode / Menu System)
// ────────────────────────────────────────────────────────────────────────────

// Global UI state counter
uint32_t g_ui_state_counter = 0;  // @ 0x825F3864

// UI object types array [256]
uint32_t g_ui_object_types[256] = {0};  // @ 0x825F3868

// UI object pointers array [256]
void* g_ui_object_ptrs[256] = {nullptr};  // @ 0x825F3968

// UI object flags array [64]
uint8_t g_ui_object_flags[64] = {0};  // @ 0x825F3A68

// Global UI state management array
uint32_t* g_global_ui_state = (uint32_t*)0x8271A348;

// Global UI context pointer
void* g_ui_context = (void*)0x8271A81C;

// ────────────────────────────────────────────────────────────────────────────
// Game Data Manager
// ────────────────────────────────────────────────────────────────────────────

// Pointer to game data manager singleton
void* g_game_data_manager = nullptr;  // @ 0x825CA174

// ────────────────────────────────────────────────────────────────────────────
// Character Type IDs
// ────────────────────────────────────────────────────────────────────────────

// Character type filter ID
uint32_t g_character_type_id = 0;  // @ 0x825C2BC0

// Additional character type IDs
uint32_t g_character_type_id_2 = 0;  // @ 0x825C803C
uint32_t g_character_type_id_3 = 0;  // @ 0x825C8038

// ────────────────────────────────────────────────────────────────────────────
// Default Constants
// ────────────────────────────────────────────────────────────────────────────

// Default float values for initialization
const float g_default_float_zero = 0.0f;  // @ 0x825CAF94
const float g_default_float_value = 0.0f;  // @ 0x825CAF88
const float g_default_float_value_2 = 0.0f;  // @ 0x825CAF90

// ────────────────────────────────────────────────────────────────────────────
// Error Message Strings
// ────────────────────────────────────────────────────────────────────────────

// Error message for unhandled events
// TODO: unresolved — claimed @ 0x8205E110 but that address lies inside the
// pongAttractContext vtable (lbl_8205E0F4, size 0x64), not a string region.
// Referenced by char_view.cpp:165 (rage_debugLog call); left as "" until the
// real format string is located in the binary.
const char* g_error_unhandled_event = "";

// Error message for character type mismatch
const char* g_error_type_mismatch = "unknown node type found in %s node: %s";  // @ 0x8203F2E0

// ────────────────────────────────────────────────────────────────────────────
// VTable Addresses
// ────────────────────────────────────────────────────────────────────────────

// pongAttractContext vtables
void* g_vtable_pong_attract_context = (void*)0x8205E0F4;
void* g_vtable_pong_attract_context_2 = (void*)0x8205E108;

// charViewCS vtable
void* g_vtable_char_view_cs = (void*)0x8203F454;

// pongAttractState vtable
void* g_vtable_pong_attract_state = (void*)0x8205E0AC;

// pongCharViewState vtable
void* g_vtable_pong_char_view_state = (void*)0x8205F174;

// pongCharViewContext vtables (multiple inheritance)
void* g_vtable_pong_char_view_context = (void*)0x8205F1BC;
void* g_vtable_pong_char_view_context_2 = (void*)0x8205F1D4;
void* g_vtable_pong_char_view_context_3 = (void*)0x8205F7B4;

// ────────────────────────────────────────────────────────────────────────────
// Character View State Globals
// ────────────────────────────────────────────────────────────────────────────

// Pointer to active character view data
void* g_charViewData_ptr = nullptr;  // @ 0x8271A2F0

// Selection counters
uint32_t g_charViewSelectCounter = 0;  // @ 0x82606490
uint32_t g_charViewSelectFlag = 0;     // @ 0x82606494

// Selected character index (-1 = none)
int32_t g_selectedCharacterIndex = -1;  // @ 0x825C76B8

// Character view data type ID
uint32_t g_charViewTypeId = 0;  // @ 0x825C2BC4

// Scene render object pointers
void* g_sceneRenderObj = nullptr;   // @ 0x82606430
void* g_sceneRenderObj2 = nullptr;  // @ 0x8260641C

// Game loop object pointer
void* g_loop_obj_ptr = nullptr;  // @ 0x825EAB30

// ────────────────────────────────────────────────────────────────────────────
// Error / Debug Strings
// ────────────────────────────────────────────────────────────────────────────

const char* g_error_attract_exit = "exiting attract state to invalid state %s (%d)";    // @ 0x8205E064
const char* g_error_charview_enter = "Entering character viewer state from invalid state %s (%d)"; // @ 0x8205F0F8
const char* g_error_charview_exit = "Exiting character viewer state to invalid state %s (%d)";   // @ 0x8205F134

// ────────────────────────────────────────────────────────────────────────────
// State Name Strings
// ────────────────────────────────────────────────────────────────────────────

const char* g_stateName_attract = "pongAttractState";  // @ 0x8205E094
const char* g_stateName_charView = "pongCharViewState"; // @ 0x8205EF5C
