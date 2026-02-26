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
const char* g_error_unhandled_event = "Unhandled event in attract state";  // @ 0x8205E110

// Error message for character type mismatch
const char* g_error_type_mismatch = "Character type mismatch in view data";  // @ 0x8203F2E0

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
