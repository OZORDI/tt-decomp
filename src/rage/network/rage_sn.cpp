/**
 * rage_sn.cpp — RAGE Session/Networking System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Hierarchical State Machine (HSM) and session management classes.
 */

#include "rage_sn.hpp"
#include <cstring>

namespace rage {

// External vtable references
extern "C" void* lbl_82073F08[];  // rage::hsmContext vtable
extern "C" void* lbl_82073F7C[];  // rage::hsmContext::Top vtable

/**
 * hsmContext::hsmContext @ 0x823ED570 | size: 0x90
 *
 * Constructor for the Hierarchical State Machine context.
 * Initializes the context with its vtable, zeros all fields,
 * sets up the embedded Top state, and clears initialization flags.
 */
hsmContext::hsmContext() {
    // Set main vtable
    this->vtable = lbl_82073F08;
    
    // Zero main fields
    this->field_04 = nullptr;
    this->field_08 = nullptr;
    this->field_0C = nullptr;
    
    // Zero embedded Top state structure
    this->topState.field_00 = nullptr;
    this->topState.field_04 = nullptr;
    this->topState.field_08 = nullptr;
    this->topState.field_0C = nullptr;
    this->topState.field_10 = nullptr;
    this->topState.field_14 = nullptr;
    this->topState.field_18 = nullptr;
    this->topState.field_1C = nullptr;
    this->topState.field_20 = nullptr;
    this->topState.field_24 = nullptr;
    
    // Set pointer to embedded Top state
    this->pTopState = &this->topState;
    
    // Set Top vtable
    this->topVtable = lbl_82073F7C;
    
    // Zero remaining fields
    this->field_40 = nullptr;
    this->field_44 = nullptr;
    this->field_48 = nullptr;
    this->field_4C = nullptr;
    this->field_50 = nullptr;
    this->field_54 = nullptr;
    this->field_58 = nullptr;
    this->field_5C = nullptr;
    this->field_60 = nullptr;
    this->field_64 = nullptr;
    
    // Clear bit 7 of flags (clrlwi r10,r11,25 -> keep lower 7 bits)
    this->flags &= 0x7F;
}

} // namespace rage

// ─────────────────────────────────────────────────────────────────────────────
// snMigrateMachine deleting destructor @ 0x823E5F28
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations
extern "C" void util_5B50(void* obj);           // @ 0x823E5B50 - Base class destructor
extern "C" void rage_free_00C0(void* ptr);      // @ 0x820C00C0 - Free memory

/**
 * snMigrateMachine_vfn_0 @ 0x823E5F28 | size: 0x50
 *
 * Deleting destructor for rage::snMigrateMachine (vtable slot 0).
 * Calls the base class destructor to clean up internal state,
 * then conditionally frees the object's memory if the delete flag is set.
 *
 * @param obj Pointer to the snMigrateMachine instance
 * @param flags Destruction flags (bit 0: free memory if set)
 */
extern "C" void snMigrateMachine_vfn_0(void* obj, int flags) {
    // Call base class destructor to clean up internal state
    util_5B50(obj);
    
    // If delete flag is set, free the object's memory
    if (flags & 0x1) {
        rage_free_00C0(obj);
    }
}
