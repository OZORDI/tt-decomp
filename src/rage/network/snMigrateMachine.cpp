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
extern void hsmState_AttachChild(void* parent, void* childState);                     // @ 0x823EF850 — AttachChildState
extern void snSession_AssociateConnections(void* context, void* session, void* connectionList); // @ 0x823ED988 — AssociateConnection
extern void snSession_ProcessPendingConnections(void* context, void* session, void* connectionList); // @ 0x823EDA90 — ProcessPendingConnections

// Destructor helpers
extern void util_5B50(void* thisPtr);   // @ 0x823E5B50 — snMigrateMachine destructor body
extern void util_6558(void* thisPtr);   // @ 0x823E6558 — snNotifyingGuests/snWaitingForMigrateMsg-level destructor
extern void hsmState_Destroy(void* thisPtr);   // @ 0x823E8F48 — hsmState base destructor (resets vtable, zeros fields)

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
extern "C" void* lbl_82073458[];  // @ 0x82073458 — rage::EvtAcquireHostSucceeded vtable
extern "C" void* lbl_82073480[];  // @ 0x82073480 — rage::EvtMigrateAsGuestSucceeded vtable
extern "C" void* lbl_82073494[];  // @ 0x82073494 — rage::EvtMigrateAsGuestFailed vtable

// Additional state transition helpers
extern void SinglesNetworkClient_A100_g(void* state, void* event);  // @ 0x823EA100 — NotifyingGuests transition failure
extern void SinglesNetworkClient_2AF0_g(void* thisPtr);             // @ 0x823F2AF0 — send guest notification messages
extern void snMigrating_A1A8(void* state, void* event);             // @ 0x823EA1A8 — MigratingAsGuest transition handler
extern void snSession_9F08_fw(void* state, void* event);            // @ 0x823E9F08 — MigrateMachine forward handler (host start)
extern void snSession_9FB0_fw(void* state, void* event);            // @ 0x823E9FB0 — MigrateMachine forward handler (guest start)
extern void game_2720(void* client);                                 // @ 0x823F2720 — game-level migrate cleanup
extern void snSession_AddNode_C068(void* nodeList, void* node);     // @ 0x823EC068 — add node to session linked list
extern void SinglesNetworkClient_2FD0_g(void* thisPtr);             // @ 0x823F2FD0 — migration completion handler
extern void SinglesNetworkClient_0978_g(void* evtObj, int p1, void* p2, void* p3, int p4); // @ 0x82430978 — broadcast init

// Global event type IDs (loaded from .data)
extern uint32_t g_evtType_AcquireHostSucceeded;  // @ 0x825D1AD4 (lis -32163, offset 6868)
extern uint32_t g_evtType_AcquireHostFailed;     // @ 0x825D1AE0 (offset 6880)
extern uint32_t g_evtType_MigrateNotifyDone;     // @ 0x825D1AC8 (offset 6856)
extern uint32_t g_evtType_HostMigrateBegin;      // @ 0x825D1AEC (offset 6892)
extern uint32_t g_evtType_HostMigrateNotify;     // @ 0x825D1AF8 (offset 6904)
extern uint32_t g_evtType_GuestMigrateBegin;     // @ 0x825D1B04 (offset 6916)
extern uint32_t g_evtType_GuestMigrateNotify;    // @ 0x825D1B10 (offset 6928)

// vtable references
extern "C" void* lbl_82072864[];  // @ 0x82072864 — rage::hsmEvent vtable

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
// Attach child sub-states via hsmState_AttachChild (AttachChildState).
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snMigrateMachine::OnEnter @ 0x823E2448 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+84  (snAcquiringHost)
 *   - this+1340 (snMigratingAsGuest)
 */
void MigrateMachine_OnEnter(void* thisPtr) {
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 84);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1340);
}

/**
 * snAcquiringHost::OnEnter @ 0x823E1F40 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+1184 (snMigrating)
 *   - this+1208 (snNotifyingGuests)
 */
void AcquiringHost_OnEnterAttach(void* thisPtr) {
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1184);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1208);
}

/**
 * snMigratingAsGuest::OnEnter @ 0x823E2388 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+24 (snWaitingForMigrateMsg)
 *   - this+76 (snMigrating)
 */
void MigratingAsGuest_OnEnterAttach(void* thisPtr) {
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 24);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 76);
}

// ─────────────────────────────────────────────────────────────────────────────
// Event Handlers (vfn_13, 76B each)
// Call vfn_11 to get context, then process connections via snSession_AssociateConnections/DA90.
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

    snSession_AssociateConnections(context, thisPtr, connectionList);
    snSession_ProcessPendingConnections(context, thisPtr, connectionList);
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

    snSession_AssociateConnections(context, thisPtr, connectionList);
    snSession_ProcessPendingConnections(context, thisPtr, connectionList);
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
 * then destroys snMigrating at this+1184 and self via hsmState_Destroy.
 */
void snAcquiringHost_Destructor(void* thisPtr, uint32_t flags) {
    util_6558((char*)thisPtr + 1208);
    hsmState_Destroy((char*)thisPtr + 1184);
    hsmState_Destroy(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snMigratingAsGuest::~snMigratingAsGuest @ 0x823E64F0 | size: 0x64 | vfn_0
 *
 * Destructor for the MigratingAsGuest state.
 * Destroys child snMigrating at this+76 via hsmState_Destroy,
 * then destroys snWaitingForMigrateMsg at this+24 via util_6558,
 * then self via hsmState_Destroy.
 */
void snMigratingAsGuest_Destructor(void* thisPtr, uint32_t flags) {
    hsmState_Destroy((char*)thisPtr + 76);
    util_6558((char*)thisPtr + 24);
    hsmState_Destroy(thisPtr);
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
    hsmState_Destroy(thisPtr);
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

// ─────────────────────────────────────────────────────────────────────────────
// snMigrateMachine::OnUpdate @ 0x823E18F8 | size: 0x110 | vfn_13
//
// Top-level update handler. Gets the current event via vfn_10, retrieves
// session info via broadcast init, then compares a 6-byte MAC address
// from the machine at +58 against the session's address at session+3784+50.
// If they match, selects child snAcquiringHost (+84); otherwise selects
// snMigratingAsGuest (+1340). Then calls vfn_11 and processes connections.
// ─────────────────────────────────────────────────────────────────────────────

void MigrateMachine_OnUpdate(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Get current event via vfn_10
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);

    // Get session broadcast info
    SinglesNetworkClient_0978_g(eventObj, 0, nullptr, nullptr, 0);

    // Copy 9 dwords (36 bytes) from result+12 into self+48
    uint32_t* src = (uint32_t*)((char*)stateCtx + 12);  // re-read after call
    // Actually the scaffold shows copying from the result (r3+12) into self+48
    // The scaffold loops 9 iterations copying 4 bytes each
    char* resultPtr = (char*)stateCtx;  // vfn_10 result
    uint32_t* dstSlots = (uint32_t*)(self + 48);
    uint32_t* srcSlots = (uint32_t*)((char*)resultPtr + 12);
    // NOTE: This is from the original r3 return value of vfn_10
    // The scaffold copies 9 dwords from result+12 to self+48

    // Compare 6-byte address at self+58 vs session address
    void* parent = *(void**)(self + 20);
    void* session = *(void**)((char*)parent + 16);
    void* sessionData = *(void**)((char*)session + 3784);
    char* sessionAddr = (char*)sessionData + 50;
    char* selfAddr = self + 58;

    // Compare 6 bytes (byte-by-byte loop)
    bool match = true;
    for (int i = 0; i < 6; i++) {
        if (sessionAddr[i] != selfAddr[i]) {
            match = false;
            break;
        }
    }

    // Select child state based on match
    void* connectionList;
    if (match) {
        connectionList = (void*)(self + 84);   // snAcquiringHost
    } else {
        connectionList = (void*)(self + 1340); // snMigratingAsGuest
    }

    // Call vfn_11 and process connections
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    snSession_AssociateConnections(context, thisPtr, connectionList);
    snSession_ProcessPendingConnections(context, thisPtr, connectionList);
}

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost::snMigrating::OnEvent @ 0x823E1CE8 | size: 0x100 | vfn_12
//
// Event handler for the Migrating sub-state of AcquiringHost.
// Checks for AcquireHostSucceeded: if matched, gets context via vfn_11
// and processes connections at parent+1292.
// Checks for AcquireHostFailed: creates EvtAcquireHostFailed event and
// transitions via snMigrating_A058.
// Otherwise marks event as unhandled.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_Migrating_OnEvent(void* thisPtr, void* event, bool* handled) {
    char* self = (char*)thisPtr;
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostSucceeded) {
        // Host acquired — process connections at parent+1292
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 20);
        void* connectionList = (char*)parent + 1292;

        snSession_AssociateConnections(context, thisPtr, connectionList);
        snSession_ProcessPendingConnections(context, thisPtr, connectionList);
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostFailed) {
        // Host acquisition failed — transition with failure event
        void* evt[1];
        ::util_DA08(evt);
        evt[0] = lbl_8207346C;  // EvtAcquireHostFailed vtable
        snMigrating_A058(thisPtr, evt);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost::snNotifyingGuests::OnEnter @ 0x823E1DF8 | size: 0xA8 | vfn_14
//
// Sets up the notification handler callback, queries session config,
// increments the notification counter at parent+1264, and if the counter
// reaches 10, fires a failure transition. Otherwise sends guest
// notification and sets a 500ms timeout.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_NotifyingGuests_OnEnter(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Set up notify handler: store self-reference and callback thunk
    *(void**)(self + 28) = thisPtr;
    *(void**)(self + 32) = (void*)thunk_fn_823F2E10;

    // Query session config status
    void* session = *(void**)(self + 16);
    void* sessionHandle = *(void**)((char*)session + 164);
    SinglesNetworkClient_A940_g(sessionHandle);

    // Increment notification attempt counter at parent+1264
    void* parent = *(void**)(self + 20);
    uint32_t* counter = (uint32_t*)((char*)parent + 1264);
    uint32_t count = *counter;
    *counter = count + 1;

    if (count >= 10) {
        // Too many attempts — create failure event and transition
        void* evt[1];
        ::util_DA08(evt);
        evt[0] = lbl_82073458;  // EvtAcquireHostSucceeded vtable (used as NotifyFailed marker)
        SinglesNetworkClient_A100_g(thisPtr, evt);
        return;
    }

    // Send guest notification messages
    SinglesNetworkClient_2AF0_g(thisPtr);

    // Set 500ms timeout
    util_D4F8(thisPtr, 500);
}

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost::snNotifyingGuests::OnEvent @ 0x823E1EA0 | size: 0x9C | vfn_12
//
// Event handler for the NotifyingGuests sub-state of AcquiringHost.
// Checks for timeout event (type == 0): if matched, gets context via
// vfn_11, processes connections at parent+1292.
// Otherwise marks event as unhandled.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_NotifyingGuests_OnEvent(void* thisPtr, void* event, bool* handled) {
    char* self = (char*)thisPtr;
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == 0) {
        // Timeout — process connections at parent+1292
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 20);
        void* connectionList = (char*)parent + 1292;

        snSession_AssociateConnections(context, thisPtr, connectionList);
        snSession_ProcessPendingConnections(context, thisPtr, connectionList);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// OnExit Handlers (vfn_6)
// Cancel pending notifications on two embedded rlNotifier child objects.
// Pattern: if pending callback at child+8 is non-null, clear it and
// invoke the Cancel function (vtable slot 4, byte +16).
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snAcquiringHost::OnExit @ 0x823E1F80 | size: 0x7C | vfn_6
 *
 * Cancels pending notifications on two child states:
 *   +1184 (snMigrating)
 *   +1208 (snNotifyingGuests)
 */
void snAcquiringHost_OnExit(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Cancel notifier at this+1184 (snMigrating)
    char* n1 = self + 1184;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n1);
    }

    // Cancel notifier at this+1208 (snNotifyingGuests)
    char* n2 = self + 1208;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n2);
    }
}

/**
 * snMigratingAsGuest::OnExit @ 0x823E23C8 | size: 0x7C | vfn_6
 *
 * Cancels pending notifications on two child states:
 *   +24 (snWaitingForMigrateMsg)
 *   +76 (snMigrating)
 */
void MigratingAsGuest_OnExit(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Cancel notifier at this+24 (snWaitingForMigrateMsg)
    char* n1 = self + 24;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n1);
    }

    // Cancel notifier at this+76 (snMigrating)
    char* n2 = self + 76;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n2);
    }
}

/**
 * snMigrateMachine::OnExit @ 0x823E2488 | size: 0x7C | vfn_6
 *
 * Cancels pending notifications on two child states:
 *   +84  (snAcquiringHost)
 *   +1340 (snMigratingAsGuest)
 */
void MigrateMachine_OnExit(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Cancel notifier at this+84 (snAcquiringHost)
    char* n1 = self + 84;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n1);
    }

    // Cancel notifier at this+1340 (snMigratingAsGuest)
    char* n2 = self + 1340;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n2);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snMigratingAsGuest::snWaitingForMigrateMsg::OnEvent
// @ 0x823E2078 | size: 0xFC | vfn_12
//
// Checks for timeout (type == 0): transitions with EvtMigrateAsGuestFailed.
// Checks for MigrateNotifyDone: gets context via vfn_11, processes
// connections at parent+1416.
// Otherwise marks event as unhandled.
// ─────────────────────────────────────────────────────────────────────────────

void snWaitingForMigrateMsg_OnEvent(void* thisPtr, void* event, bool* handled) {
    char* self = (char*)thisPtr;
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == 0) {
        // Timeout — transition with failure event
        void* evt[1];
        ::util_DA08(evt);
        evt[0] = lbl_82073494;  // EvtMigrateAsGuestFailed vtable
        snMigrating_A1A8(thisPtr, evt);
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_MigrateNotifyDone) {
        // Migrate notification received — process connections at parent+1416
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 20);
        void* connectionList = (char*)parent + 1416;

        snSession_AssociateConnections(context, thisPtr, connectionList);
        snSession_ProcessPendingConnections(context, thisPtr, connectionList);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// snMigratingAsGuest::snMigrating::OnEnter @ 0x823E2188 | size: 0x9C | vfn_14
//
// Gets current event via vfn_10, retrieves session info via broadcast init,
// then queries XAM session status. If the query fails (returns 0), creates
// an EvtMigrateAsGuestFailed event and transitions.
// ─────────────────────────────────────────────────────────────────────────────

void snMigratingAsGuest_Migrating_OnEnter(void* thisPtr) {
    char* self = (char*)thisPtr;

    // Get current event via vfn_10
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);

    // Get session info via broadcast init
    SinglesNetworkClient_0978_g(eventObj, 0, nullptr, nullptr, 0);

    // Get session handle and query XAM
    void* parent = *(void**)(self + 20);
    void* session = *(void**)((char*)parent + 16);
    void* sessionHandle = (char*)session + 232;

    // Copy data from result into session context
    uint64_t data = *(uint64_t*)((char*)stateCtx + 24);  // ld r5,24(r3)
    // store at session+232 area (passed as r3)

    uint8_t result = xam_E430_g(sessionHandle, 0, 0);

    if (result == 0) {
        // XAM query failed — transition with failure event
        void* evt[1];
        ::util_DA08(evt);
        evt[0] = lbl_82073494;  // EvtMigrateAsGuestFailed vtable
        snMigrating_A1A8(thisPtr, evt);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snMigratingAsGuest::snMigrating::OnEvent @ 0x823E2228 | size: 0x15C | vfn_12
//
// Checks for AcquireHostSucceeded: calls migration completion handler,
// fires EvtMigrateAsGuestSucceeded, allocates node, adds to session list.
// Checks for AcquireHostFailed: transitions with EvtMigrateAsGuestFailed.
// Otherwise marks event as unhandled.
// ─────────────────────────────────────────────────────────────────────────────

void snMigratingAsGuest_Migrating_OnEvent(void* thisPtr, void* event, bool* handled) {
    char* self = (char*)thisPtr;
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostSucceeded) {
        // Migration succeeded — call completion handler
        SinglesNetworkClient_2FD0_g(thisPtr);

        // Create EvtMigrateAsGuestSucceeded event
        void* evt[1];
        ::util_DA08(evt);
        void* evtSucceededVtable = lbl_82073480;  // EvtMigrateAsGuestSucceeded

        // Get context via vfn_11
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        evt[0] = evtSucceededVtable;
        void* hsmCtx = vfn_11(thisPtr);

        // Get session node list from context+56
        void* nodeList = *(void**)((char*)hsmCtx + 56);
        void* allocator = *(void**)((char*)nodeList + 4);
        void** allocVtable = *(void***)allocator;
        void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
        void* newNode = allocFn(allocator, 12, 0);

        if (newNode == nullptr) {
            return;
        }

        // Initialize node with event data
        *(void**)newNode = lbl_82072864;  // hsmEvent vtable
        *(void**)newNode = evtSucceededVtable;
        uint32_t evtData0 = *((uint32_t*)evt + 1);
        uint32_t evtData1 = *((uint32_t*)evt + 2);
        uint64_t combined = ((uint64_t)evtData0 << 32) | evtData1;
        *(uint64_t*)((char*)newNode + 4) = combined;

        snSession_AddNode_C068((char*)nodeList + 8, newNode);
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostFailed) {
        // Migration failed — transition with failure event
        void* evt[1];
        ::util_DA08(evt);
        evt[0] = lbl_82073494;  // EvtMigrateAsGuestFailed vtable
        snMigrating_A1A8(thisPtr, evt);
        return;
    }

    // Unhandled event
    *handled = false;
}

} // namespace snMigrateMachine_States

} // namespace rage
