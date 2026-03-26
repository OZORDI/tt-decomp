/**
 * snMigrateMachine.cpp — Session Migration State Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of hierarchical state machines for session host migration.
 * Handles the complex flow of migrating session ownership between players,
 * including both host-side (AcquiringHost) and guest-side (MigratingAsGuest)
 * state machines.
 */

#include <stdint.h>

extern "C" void rage_free(void* ptr);  // @ 0x820C00C0 — heap free

// Global-scope extern helpers (defined outside namespace rage in stubs)
extern void util_DA08(void* event);  // @ 0x823DDA08 — init HSM event

namespace rage {

// Forward declarations
class SinglesNetworkClient;

// External HSM utility functions
extern void util_F850(void* parent, void* childState);                     // @ 0x823EF850 — AttachChildState
extern void util_D988(void* context, void* session, void* connectionList); // @ 0x823ED988 — AssociateConnection
extern void util_DA90(void* context, void* session, void* connectionList); // @ 0x823EDA90 — ProcessPendingConnections

// Destructor helpers
extern void util_5B50(void* thisPtr);   // @ 0x823E5B50 — snMigrateMachine destructor body
extern void util_6558(void* thisPtr);   // @ 0x823E6558 — snNotifyingGuests/snWaitingForMigrateMsg-level destructor
extern void util_8F48(void* thisPtr);   // @ 0x823E8F48 — hsmState base destructor (resets vtable, zeros fields)

// Tail-call targets for snAcquiringHost OnEnter/OnExit
extern void SinglesNetworkClient_2828_p45(void* thisPtr);  // @ 0x823F2828 — snAcquiringHost::OnEnter body
extern void SinglesNetworkClient_2A08_w(void* thisPtr);    // @ 0x823F2A08 — snAcquiringHost::OnExit body

// snMigrateMachine::OnEnter helpers
extern void snSession_1BF8_g(void* notifyList, int eventType, void* param); // @ 0x82371BF8 — register notify handler
extern void util_D170(void* notifyList, void* handler);                     // @ 0x823DD170 — linked list insert

// snAcquiringHost::snMigrating::OnEnter helpers
extern uint8_t xam_E430_g(void* sessionHandle, int param1, int param2);    // @ 0x8236E430 — XAM session query
extern void snMigrating_A058(void* state, void* event);                     // @ 0x823EA058 — state transition handler

// snMigratingAsGuest::snWaitingForMigrateMsg::OnEnter helpers
extern void SinglesNetworkClient_A940_g(void* session);                     // @ 0x823DA940 — query config status
extern void util_D4F8(void* state, int timeout);                            // @ 0x823ED4F8 — set max transitions/timeout

// Callback thunks (function pointers stored into notify handler slots)
extern "C" void thunk_fn_823F25E8(void);  // @ 0x823F3190 — migrate notify callback thunk
extern "C" void thunk_fn_823F2E10(void);  // @ 0x823F31A0 — waiting-for-migrate-msg notify callback thunk

// Event vtable
extern "C" void* lbl_8207346C[];  // @ 0x8207346C — rage::EvtAcquireHostFailed vtable

// ────────────────────────────────────────────────────────────────────────────
// State Name Strings (from .rdata)
// ────────────────────────────────────────────────────────────────────────────

static const char* const s_MigrateMachine        = "MigrateMachine";        // @ 0x82074660
static const char* const s_AcquiringHost         = "AcquiringHost";         // @ 0x82074670
static const char* const s_Migrating             = "Migrating";             // @ 0x82074680
static const char* const s_NotifyingGuests       = "NotifyingGuests";       // @ 0x8207468C
static const char* const s_MigratingAsGuest      = "MigratingAsGuest";      // @ 0x8207469C
static const char* const s_snWaitingForMigrateMsg = "snWaitingForMigrateMsg"; // @ 0x820746B0
static const char* const s_snMigrating           = "snMigrating";           // @ 0x820746C8

namespace snMigrateMachine_States {

// ─────────────────────────────────────────────────────────────────────────────
// GetStateName Functions (vfn_2, 12B each)
// All return a const char* state display name from .rdata.
// ─────────────────────────────────────────────────────────────────────────────

const char* MigrateMachine_GetName()                          { return s_MigrateMachine; }        // @ 0x823E18E8
const char* AcquiringHost_GetName()                           { return s_AcquiringHost; }          // @ 0x823E1BF8
const char* AcquiringHost_Migrating_GetName()                 { return s_Migrating; }              // @ 0x823E1C68
const char* AcquiringHost_NotifyingGuests_GetName()           { return s_NotifyingGuests; }        // @ 0x823E1DE8
const char* MigratingAsGuest_GetName()                        { return s_MigratingAsGuest; }       // @ 0x823E2000
const char* MigratingAsGuest_WaitingForMigrateMsg_GetName()   { return s_snWaitingForMigrateMsg; } // @ 0x823E2010
const char* MigratingAsGuest_Migrating_GetName()              { return s_snMigrating; }            // @ 0x823E2178

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost Tail-Call Thunks (vfn_14 / vfn_15, 4B each)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snAcquiringHost::OnEnter @ 0x823E1C58 | size: 0x4 | vfn_14
 * Tail-calls SinglesNetworkClient_2828_p45 to handle host acquisition entry.
 */
void snAcquiringHost_OnEnter(void* thisPtr) {
    SinglesNetworkClient_2828_p45(thisPtr);
}

/**
 * snAcquiringHost::OnExit @ 0x823E1C60 | size: 0x4 | vfn_15
 * Tail-calls SinglesNetworkClient_2A08_w to handle host acquisition exit.
 */
void snAcquiringHost_OnExit(void* thisPtr) {
    SinglesNetworkClient_2A08_w(thisPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// OnEnter Handlers (vfn_5, 60B each)
// Attach child sub-states via util_F850 (AttachChildState).
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snMigrateMachine::OnEnter @ 0x823E2448 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+84  (snAcquiringHost)
 *   - this+1340 (snMigratingAsGuest)
 */
void MigrateMachine_OnEnter(void* thisPtr) {
    util_F850(thisPtr, (char*)thisPtr + 84);
    util_F850(thisPtr, (char*)thisPtr + 1340);
}

/**
 * snAcquiringHost::OnEnter @ 0x823E1F40 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+1184 (snMigrating)
 *   - this+1208 (snNotifyingGuests)
 */
void AcquiringHost_OnEnterAttach(void* thisPtr) {
    util_F850(thisPtr, (char*)thisPtr + 1184);
    util_F850(thisPtr, (char*)thisPtr + 1208);
}

/**
 * snMigratingAsGuest::OnEnter @ 0x823E2388 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+24 (snWaitingForMigrateMsg)
 *   - this+76 (snMigrating)
 */
void MigratingAsGuest_OnEnterAttach(void* thisPtr) {
    util_F850(thisPtr, (char*)thisPtr + 24);
    util_F850(thisPtr, (char*)thisPtr + 76);
}

// ─────────────────────────────────────────────────────────────────────────────
// Event Handlers (vfn_13, 76B each)
// Call vfn_11 to get context, then process connections via util_D988/DA90.
// Identical pattern to snSession_snRoot_snChangingPresence_vfn_13.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snAcquiringHost::OnEvent @ 0x823E1C08 | size: 0x4C | vfn_13
 *
 * Processes network connections during host acquisition.
 * Gets context via vfn_11, then associates and processes connections
 * using the connection list at this+1184.
 */
void snAcquiringHost_OnEvent(void* thisPtr) {
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    void* connectionList = (char*)thisPtr + 1184;

    util_D988(context, thisPtr, connectionList);
    util_DA90(context, thisPtr, connectionList);
}

/**
 * snMigratingAsGuest::OnEvent @ 0x823E2690 | size: 0x4C | vfn_13
 *
 * Processes network connections during guest migration.
 * Gets context via vfn_11, then associates and processes connections
 * using the connection list at this+24.
 */
void snMigratingAsGuest_OnEvent(void* thisPtr) {
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    void* connectionList = (char*)thisPtr + 24;

    util_D988(context, thisPtr, connectionList);
    util_DA90(context, thisPtr, connectionList);
}

// ─────────────────────────────────────────────────────────────────────────────
// Destructors (vfn_0)
// All follow: call state-level cleanup helper(s), then conditionally free.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snAcquiringHost::~snAcquiringHost @ 0x823E6488 | size: 0x64 | vfn_0
 *
 * Destructor for the AcquiringHost state.
 * Destroys child snNotifyingGuests at this+1208 via util_6558,
 * then destroys snMigrating at this+1184 and self via util_8F48.
 */
void snAcquiringHost_Destructor(void* thisPtr, uint32_t flags) {
    util_6558((char*)thisPtr + 1208);
    util_8F48((char*)thisPtr + 1184);
    util_8F48(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snMigratingAsGuest::~snMigratingAsGuest @ 0x823E64F0 | size: 0x64 | vfn_0
 *
 * Destructor for the MigratingAsGuest state.
 * Destroys child snMigrating at this+76 via util_8F48,
 * then destroys snWaitingForMigrateMsg at this+24 via util_6558,
 * then self via util_8F48.
 */
void snMigratingAsGuest_Destructor(void* thisPtr, uint32_t flags) {
    util_8F48((char*)thisPtr + 76);
    util_6558((char*)thisPtr + 24);
    util_8F48(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snAcquiringHost::snMigrating::~snMigrating @ 0x823E66B0 | size: 0x50 | vfn_0
 *
 * Destructor for the Migrating sub-state within AcquiringHost.
 * Calls hsmState base destructor to reset vtable and zero fields.
 */
void snAcquiringHost_Migrating_Destructor(void* thisPtr, uint32_t flags) {
    util_8F48(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snAcquiringHost::snNotifyingGuests::~snNotifyingGuests @ 0x823E6700 | size: 0x50 | vfn_0
 *
 * Destructor for the NotifyingGuests sub-state within AcquiringHost.
 * Calls util_6558 to clean up notification handlers, then resets.
 */
void snAcquiringHost_NotifyingGuests_Destructor(void* thisPtr, uint32_t flags) {
    util_6558(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snMigrateMachine::OnEnter @ 0x823F2788 | size: 0x5C | vfn_14
//
// Entry handler for the top-level MigrateMachine state.
// Registers a notification handler for migration events, sets up the
// callback thunk, and inserts the handler into the session notify list.
// ─────────────────────────────────────────────────────────────────────────────

void MigrateMachine_OnEnterFull(void* thisPtr) {
    void* session = *(void**)((char*)thisPtr + 16);

    // Register migration event notification (type 21)
    snSession_1BF8_g((char*)session + 108, 21, nullptr);

    // Set up notify handler: store self-reference and callback thunk
    *(void**)((char*)thisPtr + 28) = thisPtr;                   // this+28 = owner ptr
    *(void**)((char*)thisPtr + 32) = (void*)thunk_fn_823F25E8;        // this+32 = callback thunk

    // Insert handler into session's notify list at session+232
    void* notifyList = (char*)session + 232;
    // Passing handler at this+24
    util_D170(notifyList, (char*)thisPtr + 24);
}

// ─────────────────────────────────────────────────────────────────────────────
// snMigratingAsGuest::snWaitingForMigrateMsg::OnEnter
// @ 0x823E2020 | size: 0x58 | vfn_14
//
// Sets up the waiting state: installs callback thunk, clears bit 7 of
// flags at this+48, queries session config status, and sets a 10-second
// timeout for the state machine.
// ─────────────────────────────────────────────────────────────────────────────

void snWaitingForMigrateMsg_OnEnter(void* thisPtr) {
    // Set up notify handler
    *(void**)((char*)thisPtr + 28) = thisPtr;                   // this+28 = owner ptr
    *(void**)((char*)thisPtr + 32) = (void*)thunk_fn_823F2E10;        // this+32 = callback thunk

    // Clear bit 7 of flags byte at this+48
    uint8_t* flags = (uint8_t*)((char*)thisPtr + 48);
    *flags = *flags & 0x7F;

    // Get session handle from parent: this+16 -> session, session+164 -> handle
    void* session = *(void**)((char*)thisPtr + 16);
    void* sessionHandle = *(void**)((char*)session + 164);

    // Query session config status
    SinglesNetworkClient_A940_g(sessionHandle);

    // Set 10-second timeout (10000ms)
    util_D4F8(thisPtr, 10000);
}

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost::snMigrating::OnEnter
// @ 0x823E1C78 | size: 0x6C | vfn_14
//
// Checks XAM session status. If the session query fails (returns 0),
// creates an EvtAcquireHostFailed event and transitions the state machine.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_Migrating_OnEnter(void* thisPtr) {
    // Get session handle: this+20 -> parent, parent+16 -> session, session+232 -> handle
    void* parent = *(void**)((char*)thisPtr + 20);
    void* session = *(void**)((char*)parent + 16);
    void* sessionHandle = (char*)session + 232;

    // Query XAM session status
    uint8_t result = xam_E430_g(sessionHandle, 0, 0);

    if (result == 0) {
        // Session query failed — create EvtAcquireHostFailed event on stack
        void* event[1];
        ::util_DA08(event);
        event[0] = lbl_8207346C;  // Set EvtAcquireHostFailed vtable

        // Transition state machine with failure event
        snMigrating_A058(thisPtr, event);
    }
}

} // namespace snMigrateMachine_States

} // namespace rage
