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


/* ── External dependencies for leave request state machine ────────────────── */

/* Network client state check functions */
extern uint8_t SinglesNetworkClient_19E0_g(void* pClient);  /* @ 0x823F19E0 */
extern uint8_t util_1550(void* pClient);                     /* @ 0x823F1550 */

/* Event initialization */
extern void util_DA08(void* pEvent);  /* @ 0x823DDA08 */

/* State transition handler */
extern void snHsmAcceptingLeaveRequest_9E60(
    void* pState,
    void* pEvent);  /* @ 0x823E9E60 */

/* Vtable for EvtAcceptLeaveRequestFailed event @ 0x82073170 */
extern void* g_vtable_EvtAcceptLeaveRequestFailed;  /* @ 0x82073170 */


namespace rage {
namespace snLeaveMachine {

/* ═══════════════════════════════════════════════════════════════════════════
 * snHsmAcceptingLeaveRequest::OnEnter @ 0x823E1530 | size: 0x70 (112 bytes)
 *
 * Vtable slot 14 - Handles leave request acceptance state logic.
 *
 * This function checks the network client's state to determine if a leave
 * request can proceed. If the client is not in the correct state or fails
 * validation, it creates an EvtAcceptLeaveRequestFailed event and transitions
 * the state machine.
 *
 * State machine layout (partial):
 *   +0x14 (20)   m_pNetworkClient  - Pointer to SinglesNetworkClient
 *
 * Algorithm:
 *   1. Check if network client is ready (SinglesNetworkClient_19E0_g)
 *   2. If ready, check if client passes validation (util_1550)
 *   3. If both checks pass, return (stay in current state)
 *   4. Otherwise:
 *      a. Create EvtAcceptLeaveRequestFailed event on stack
 *      b. Initialize event with vtable
 *      c. Call state transition handler with event
 * ═══════════════════════════════════════════════════════════════════════════ */
void snHsmAcceptingLeaveRequest::OnEnter()
{
    /* Load network client pointer from state machine */
    void* pNetworkClient = *(void**)((uint8_t*)this + 20);
    
    /* Check if network client is ready for leave request */
    uint8_t isReady = SinglesNetworkClient_19E0_g(pNetworkClient);
    
    if (isReady) {
        /* Client is ready - perform additional validation */
        uint8_t isValid = util_1550(pNetworkClient);
        
        if (isValid) {
            /* Both checks passed - stay in current state */
            return;
        }
    }
    
    /* ── Client not ready or validation failed - create failure event ────── */
    
    /* Create event object on stack (vtable pointer only) */
    struct EvtAcceptLeaveRequestFailed {
        void* vtable;
    } event;
    
    /* Initialize event structure */
    util_DA08(&event);
    
    /* Set vtable for EvtAcceptLeaveRequestFailed */
    event.vtable = &g_vtable_EvtAcceptLeaveRequestFailed;
    
    /* Transition state machine with failure event */
    snHsmAcceptingLeaveRequest_9E60(this, &event);
}

} // namespace snLeaveMachine
} // namespace rage
