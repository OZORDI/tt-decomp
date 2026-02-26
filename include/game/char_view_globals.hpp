/**
 * char_view_globals.hpp — Character View System Global Variables
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Global variables, constants, and data objects used by the character view system.
 * Extracted from assembly analysis of charView functions.
 */

#pragma once

#include <stdint.h>

// ────────────────────────────────────────────────────────────────────────────
// Thread-Local Storage
// ────────────────────────────────────────────────────────────────────────────

// TLS base pointer (r13 register on PowerPC)
// Contains pointers to thread-local allocators and other per-thread data
extern void** g_tls_base;  // @ 0x82600000

// ────────────────────────────────────────────────────────────────────────────
// UI System Globals (Attract Mode / Menu System)
// ────────────────────────────────────────────────────────────────────────────

// Global UI state counter - tracks number of active UI objects
// @ 0x825F3864 (lbl_825F3864)
extern uint32_t g_ui_state_counter;

// UI object types array [256] - stores type IDs for each UI slot
// @ 0x825F3868 (lbl_825F3868)
extern uint32_t g_ui_object_types[256];

// UI object pointers array [256] - stores pointers to UI objects
// @ 0x825F3968 (lbl_825F3968)
extern void* g_ui_object_ptrs[256];

// UI object flags array [64] - stores flags for UI objects
// @ 0x825F3A68 (lbl_825F3A68)
extern uint8_t g_ui_object_flags[64];

// Global UI state management array
// @ 0x8271A348 (lbl_8271A348)
extern uint32_t* g_global_ui_state;

// Global UI context pointer
// @ 0x8271A81C (lbl_8271A81C)
extern void* g_ui_context;

// ────────────────────────────────────────────────────────────────────────────
// Game Data Manager
// ────────────────────────────────────────────────────────────────────────────

// Pointer to game data manager singleton
// Contains character data, counts, and other game configuration
// @ 0x825CA174 (used in LoadViewData)
extern void* g_game_data_manager;

// ────────────────────────────────────────────────────────────────────────────
// Character Type IDs
// ────────────────────────────────────────────────────────────────────────────

// Character type filter ID used in view data loading
// @ 0x825C2BC0 (lbl_825C2BC0)
extern uint32_t g_character_type_id;

// Additional character type IDs for type checking
// @ 0x825C803C
extern uint32_t g_character_type_id_2;

// @ 0x825C8038
extern uint32_t g_character_type_id_3;

// ────────────────────────────────────────────────────────────────────────────
// Default Constants
// ────────────────────────────────────────────────────────────────────────────

// Default float value 0.0f for initialization
// @ 0x825CAF94
extern const float g_default_float_zero;

// Default float value (non-zero)
// @ 0x825CAF88
extern const float g_default_float_value;

// Default float value (another variant)
// @ 0x825CAF90
extern const float g_default_float_value_2;

// ────────────────────────────────────────────────────────────────────────────
// Error Message Strings
// ────────────────────────────────────────────────────────────────────────────

// Error message for unhandled events in attract state
// @ 0x8205E110
extern const char* g_error_unhandled_event;

// Error message for character type mismatch
// @ 0x8203F2E0 (lbl_8203F2E0) - "Character type mismatch in view data"
extern const char* g_error_type_mismatch;

// ────────────────────────────────────────────────────────────────────────────
// VTable Addresses
// ────────────────────────────────────────────────────────────────────────────

// pongAttractContext primary vtable
// @ 0x8205E0F4 (lbl_8205E0F4) - RTTI: pongAttractContext
extern void* g_vtable_pong_attract_context;

// pongAttractContext secondary vtable (multiple inheritance)
// @ 0x8205E108
extern void* g_vtable_pong_attract_context_2;

// charViewCS vtable
// @ 0x8203F454
extern void* g_vtable_char_view_cs;

// pongAttractState vtable
// @ 0x8205E0AC
extern void* g_vtable_pong_attract_state;

// pongCharViewState vtable
// @ 0x8205F174
extern void* g_vtable_pong_char_view_state;

// pongCharViewContext primary vtable
// @ 0x8205F1BC
extern void* g_vtable_pong_char_view_context;

// pongCharViewContext secondary vtable
// @ 0x8205F1D4
extern void* g_vtable_pong_char_view_context_2;

// pongCharViewContext tertiary vtable
// @ 0x8205F7B4
extern void* g_vtable_pong_char_view_context_3;
