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
extern void hsmEvent_Init(void* event);  // @ 0x823DDA08 — init HSM event

namespace rage {

// Forward declarations
class SinglesNetworkClient;

// External HSM utility functions
extern void hsmState_AttachChild(void* parent, void* childState);                     // @ 0x823EF850 — AttachChildState
extern void snSession_AssociateConnections(void* context, void* session, void* connectionList); // @ 0x823ED988 — AssociateConnection
extern void snSession_ProcessPendingConnections(void* context, void* session, void* connectionList); // @ 0x823EDA90 — ProcessPendingConnections

// Destructor helpers
extern void snMigrateMachine_DestroyBody(void* thisPtr);   // @ 0x823E5B50 — snMigrateMachine destructor body
extern void hsmNotifyState_Destroy(void* thisPtr);   // @ 0x823E6558 — snNotifyingGuests/snWaitingForMigrateMsg-level destructor
extern void hsmState_Destroy(void* thisPtr);   // @ 0x823E8F48 — hsmState base destructor (resets vtable, zeros fields)

// Tail-call targets for snAcquiringHost OnEnter/OnExit
extern void snAcquiringHost_OnEnterBody(void* thisPtr);  // @ 0x823F2828 — snAcquiringHost::OnEnter body
extern void snAcquiringHost_OnExitBody(void* thisPtr);    // @ 0x823F2A08 — snAcquiringHost::OnExit body

// snMigrateMachine::OnEnter helpers
extern void snSession_RegisterNotifyHandler(void* notifyList, int eventType, void* param); // @ 0x82371BF8 — register notify handler
extern void rlLinkedList_Insert(void* notifyList, void* handler);                     // @ 0x823DD170 — linked list insert

// snAcquiringHost::snMigrating::OnEnter helpers
extern uint8_t xamSession_QueryStatus(void* sessionHandle, int param1, int param2);    // @ 0x8236E430 — XAM session query
extern void snAcquiringHost_Migrating_QueueTransition(void* state, void* event);                     // @ 0x823EA058 — state transition handler

// snMigratingAsGuest::snWaitingForMigrateMsg::OnEnter helpers
extern void snSession_QueryConfigStatus(void* session);                     // @ 0x823DA940 — query config status
extern void hsmState_SetTimeout(void* state, int timeout);                            // @ 0x823ED4F8 — set max transitions/timeout

// Callback thunks (function pointers stored into notify handler slots)
extern "C" void thunk_snMigrateNotifyCallback(void);  // @ 0x823F3190 — migrate notify callback thunk
extern "C" void thunk_snWaitingForMigrateMsgCallback(void);  // @ 0x823F31A0 — waiting-for-migrate-msg notify callback thunk

// Named vtable pointers (RTTI-verified)
extern void* g_&g_vtbl_EvtAcquireHostFailed;      // @ 0x8207346C — rage::EvtAcquireHostFailed
extern void* g_&g_vtbl_EvtAcquireHostSucceeded;    // @ 0x82073458 — rage::EvtAcquireHostSucceeded
extern void* g_&g_vtbl_EvtMigrateAsGuestSucceeded; // @ 0x82073480 — rage::EvtMigrateAsGuestSucceeded
extern void* g_&g_vtbl_EvtMigrateAsGuestFailed;    // @ 0x82073494 — rage::EvtMigrateAsGuestFailed

// Additional state transition helpers
extern void snNotifyingGuests_TransitionFailed(void* state, void* event);  // @ 0x823EA100 — NotifyingGuests transition failure
extern void snNotifyingGuests_SendMessages(void* thisPtr);             // @ 0x823F2AF0 — send guest notification messages
extern void snMigratingAsGuest_QueueTransition(void* state, void* event);             // @ 0x823EA1A8 — MigratingAsGuest transition handler
extern void snSession_ForwardHostMigrate(void* state, void* event);            // @ 0x823E9F08 — MigrateMachine forward handler (host start)
extern void snSession_ForwardGuestMigrate(void* state, void* event);            // @ 0x823E9FB0 — MigrateMachine forward handler (guest start)
extern void snMigrateMachine_ProcessMigrateEvent(void* client);                                 // @ 0x823F2720 — game-level migrate cleanup
extern void snSession_AddNode(void* nodeList, void* node);     // @ 0x823EC068 — add node to session linked list
extern void snMigrateAsGuest_OnCompleted(void* thisPtr);             // @ 0x823F2FD0 — migration completion handler
extern void rlEvent_Init(void* evtObj, int p1, void* p2, void* p3, int p4); // @ 0x82430978 — broadcast init

// Global event type IDs (loaded from .data)
extern uint32_t g_evtType_AcquireHostSucceeded;  // @ 0x825D1AD4 (lis -32163, offset 6868)
extern uint32_t g_evtType_AcquireHostFailed;     // @ 0x825D1AE0 (offset 6880)
extern uint32_t g_evtType_MigrateNotifyDone;     // @ 0x825D1AC8 (offset 6856)
extern uint32_t g_evtType_HostMigrateBegin;      // @ 0x825D1AEC (offset 6892)
extern uint32_t g_evtType_HostMigrateNotify;     // @ 0x825D1AF8 (offset 6904)
extern uint32_t g_evtType_GuestMigrateBegin;     // @ 0x825D1B04 (offset 6916)
extern uint32_t g_evtType_GuestMigrateNotify;    // @ 0x825D1B10 (offset 6928)

// vtable references
extern void* g_&g_vtbl_hsmEvent;                   // @ 0x82072864 — rage::hsmEvent base

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
 * Tail-calls snAcquiringHost_OnEnterBody to handle host acquisition entry.
 */
void snAcquiringHost_OnEnter(void* thisPtr) {
    snAcquiringHost_OnEnterBody(thisPtr);
}

/**
 * snAcquiringHost::OnExit @ 0x823E1C60 | size: 0x4 | vfn_15
 * Tail-calls snAcquiringHost_OnExitBody to handle host acquisition exit.
 */
void snAcquiringHost_OnExit(void* thisPtr) {
    snAcquiringHost_OnExitBody(thisPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// OnEnter Handlers (vfn_5, 60B each)
// Attach child sub-states via hsmState_AttachChild (AttachChildState).
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snMigrateMachine::OnEnter @ 0x823E2448 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+0x54  (snAcquiringHost)
 *   - this+0x53C (snMigratingAsGuest)
 */
void MigrateMachine_OnEnter(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);
    hsmState_AttachChild(thisPtr, self + 0x54);   // m_acquiringHost
    hsmState_AttachChild(thisPtr, self + 0x53C);  // m_migratingAsGuest
}

/**
 * snAcquiringHost::OnEnter @ 0x823E1F40 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+0x4A0 (snMigrating)
 *   - this+0x4B8 (snNotifyingGuests)
 */
void AcquiringHost_OnEnterAttach(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);
    hsmState_AttachChild(thisPtr, self + 0x4A0);  // m_migrating
    hsmState_AttachChild(thisPtr, self + 0x4B8);  // m_notifyingGuests
}

/**
 * snMigratingAsGuest::OnEnter @ 0x823E2388 | size: 0x3C | vfn_5
 *
 * Attaches two child sub-states:
 *   - this+0x18 (snWaitingForMigrateMsg)
 *   - this+0x4C (snMigrating)
 */
void MigratingAsGuest_OnEnterAttach(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);
    hsmState_AttachChild(thisPtr, self + 0x18);  // m_waitingForMigrateMsg
    hsmState_AttachChild(thisPtr, self + 0x4C);  // m_migrating
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
 * using the connection list at this+0x4A0 (m_migrating).
 */
void snAcquiringHost_OnEvent(void* thisPtr) {
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    char* self = static_cast<char*>(thisPtr);
    void* connectionList = self + 0x4A0;  // m_migrating

    snSession_AssociateConnections(context, thisPtr, connectionList);
    snSession_ProcessPendingConnections(context, thisPtr, connectionList);
}

/**
 * snMigratingAsGuest::OnEvent @ 0x823E2690 | size: 0x4C | vfn_13
 *
 * Processes network connections during guest migration.
 * Gets context via vfn_11, then associates and processes connections
 * using the connection list at this+0x18 (m_waitingForMigrateMsg).
 */
void snMigratingAsGuest_OnEvent(void* thisPtr) {
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    char* self = static_cast<char*>(thisPtr);
    void* connectionList = self + 0x18;  // m_waitingForMigrateMsg

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
 * Destroys child snNotifyingGuests at this+0x4B8 via hsmNotifyState_Destroy,
 * then destroys snMigrating at this+0x4A0 and self via hsmState_Destroy.
 */
void snAcquiringHost_Destructor(void* thisPtr, uint32_t flags) {
    char* self = static_cast<char*>(thisPtr);
    hsmNotifyState_Destroy(self + 0x4B8);  // m_notifyingGuests
    hsmState_Destroy(self + 0x4A0);        // m_migrating
    hsmState_Destroy(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snMigratingAsGuest::~snMigratingAsGuest @ 0x823E64F0 | size: 0x64 | vfn_0
 *
 * Destructor for the MigratingAsGuest state.
 * Destroys child snMigrating at this+0x4C via hsmState_Destroy,
 * then destroys snWaitingForMigrateMsg at this+0x18 via hsmNotifyState_Destroy,
 * then self via hsmState_Destroy.
 */
void snMigratingAsGuest_Destructor(void* thisPtr, uint32_t flags) {
    char* self = static_cast<char*>(thisPtr);
    hsmState_Destroy(self + 0x4C);         // m_migrating
    hsmNotifyState_Destroy(self + 0x18);   // m_waitingForMigrateMsg
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
 * Calls hsmNotifyState_Destroy to clean up notification handlers, then resets.
 */
void snAcquiringHost_NotifyingGuests_Destructor(void* thisPtr, uint32_t flags) {
    hsmNotifyState_Destroy(thisPtr);
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
    char* self = static_cast<char*>(thisPtr);
    void* session = *(void**)(self + 0x10);  // m_pSession

    // Register migration event notification (type 21)
    snSession_RegisterNotifyHandler((char*)session + 0x6C, 21, nullptr);

    // Set up notify handler: store self-reference and callback thunk
    *(void**)(self + 0x1C) = thisPtr;                             // m_notifyOwner
    *(void**)(self + 0x20) = (void*)thunk_snMigrateNotifyCallback; // m_notifyCallback

    // Insert handler into session's notify list at session+0xE8
    void* notifyList = (char*)session + 0xE8;  // m_notifyHandlerList
    rlLinkedList_Insert(notifyList, self + 0x18);  // m_notifyHandler
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
    char* self = static_cast<char*>(thisPtr);

    // Set up notify handler
    *(void**)(self + 0x1C) = thisPtr;                                     // m_notifyOwner
    *(void**)(self + 0x20) = (void*)thunk_snWaitingForMigrateMsgCallback; // m_notifyCallback

    // Clear bit 7 of flags byte at this+0x30
    uint8_t* flags = reinterpret_cast<uint8_t*>(self + 0x30);
    *flags = *flags & 0x7F;

    // Get session handle from parent: this+0x10 -> session, session+0xA4 -> handle
    void* session = *(void**)(self + 0x10);  // m_pSession
    void* sessionHandle = *(void**)((char*)session + 0xA4);

    // Query session config status
    snSession_QueryConfigStatus(sessionHandle);

    // Set 10-second timeout (10000ms)
    hsmState_SetTimeout(thisPtr, 10000);
}

// ─────────────────────────────────────────────────────────────────────────────
// snAcquiringHost::snMigrating::OnEnter
// @ 0x823E1C78 | size: 0x6C | vfn_14
//
// Checks XAM session status. If the session query fails (returns 0),
// creates an EvtAcquireHostFailed event and transitions the state machine.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_Migrating_OnEnter(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);
    // Get session handle: this+0x14 -> parent, parent+0x10 -> session, session+0xE8 -> xamContext
    void* parent = *(void**)(self + 0x14);  // m_pParent
    void* session = *(void**)((char*)parent + 0x10);  // parent->m_pSession
    void* sessionHandle = (char*)session + 0xE8;  // session->m_xamContext

    // Query XAM session status
    uint8_t result = xamSession_QueryStatus(sessionHandle, 0, 0);

    if (result == 0) {
        // Session query failed — create EvtAcquireHostFailed event on stack
        void* event[1];
        ::hsmEvent_Init(event);
        event[0] = &g_vtbl_EvtAcquireHostFailed;  // Set EvtAcquireHostFailed vtable

        // Transition state machine with failure event
        snAcquiringHost_Migrating_QueueTransition(thisPtr, event);
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
    char* self = static_cast<char*>(thisPtr);

    // Get current event via vfn_10
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);

    // Get session broadcast info
    rlEvent_Init(eventObj, 0, nullptr, nullptr, 0);

    // Copy 9 dwords (36 bytes) from result+12 into self+48
    uint32_t* src = (uint32_t*)((char*)stateCtx + 0x0C);  // re-read after call
    // Actually the scaffold shows copying from the result (r3+12) into self+48
    // The scaffold loops 9 iterations copying 4 bytes each
    char* resultPtr = (char*)stateCtx;  // vfn_10 result
    uint32_t* dstSlots = (uint32_t*)(self + 0x30);  // m_migrationData
    uint32_t* srcSlots = (uint32_t*)((char*)resultPtr + 12);
    // NOTE: This is from the original r3 return value of vfn_10
    // The scaffold copies 9 dwords from result+12 to self+48

    // Compare 6-byte MAC address at self+0x3A vs session address
    void* parent = *(void**)(self + 0x14);  // m_pParent
    void* session = *(void**)((char*)parent + 0x10);  // parent->m_pSession
    void* sessionData = *(void**)((char*)session + 0xEC8);  // session+0xEC8 (3784)
    char* sessionAddr = (char*)sessionData + 0x32;  // sessionData+0x32 (50)
    char* selfAddr = self + 0x3A;  // m_macAddress

    // Compare 6 bytes (byte-by-byte loop)
    bool match = true;
    for (int i = 0; i < 6; i++) {
        if (sessionAddr[i] != selfAddr[i]) {
            match = false;
            break;
        }
    }

    // Select child state based on MAC address match
    void* connectionList;
    if (match) {
        connectionList = (void*)(self + 0x54);   // m_acquiringHost
    } else {
        connectionList = (void*)(self + 0x53C);  // m_migratingAsGuest
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
// transitions via snAcquiringHost_Migrating_QueueTransition.
// Otherwise marks event as unhandled.
// ─────────────────────────────────────────────────────────────────────────────

void snAcquiringHost_Migrating_OnEvent(void* thisPtr, void* event, bool* handled) {
    char* self = static_cast<char*>(thisPtr);
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostSucceeded) {
        // Host acquired — process connections at parent+1292
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 0x14);  // m_pParent
        void* connectionList = (char*)parent + 0x50C;  // m_connectionList

        snSession_AssociateConnections(context, thisPtr, connectionList);
        snSession_ProcessPendingConnections(context, thisPtr, connectionList);
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 0x0C);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostFailed) {
        // Host acquisition failed — transition with failure event
        void* evt[1];
        ::hsmEvent_Init(evt);
        evt[0] = &g_vtbl_EvtAcquireHostFailed;  // EvtAcquireHostFailed vtable
        snAcquiringHost_Migrating_QueueTransition(thisPtr, evt);
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
    char* self = static_cast<char*>(thisPtr);

    // Set up notify handler: store self-reference and callback thunk
    *(void**)(self + 0x1C) = thisPtr;                                     // m_notifyOwner
    *(void**)(self + 0x20) = (void*)thunk_snWaitingForMigrateMsgCallback; // m_notifyCallback

    // Query session config status
    void* session = *(void**)(self + 0x10);  // m_pSession
    void* sessionHandle = *(void**)((char*)session + 0xA4);  // session->m_sessionHandle
    snSession_QueryConfigStatus(sessionHandle);

    // Increment notification attempt counter at parent+0x4F0
    void* parent = *(void**)(self + 0x14);  // m_pParent
    uint32_t* counter = (uint32_t*)((char*)parent + 0x4F0);  // m_notifyCounter
    uint32_t count = *counter;
    *counter = count + 1;

    if (count >= 10) {
        // Too many attempts — create failure event and transition
        void* evt[1];
        ::hsmEvent_Init(evt);
        evt[0] = &g_vtbl_EvtAcquireHostSucceeded;  // EvtAcquireHostSucceeded vtable (used as NotifyFailed marker)
        snNotifyingGuests_TransitionFailed(thisPtr, evt);
        return;
    }

    // Send guest notification messages
    snNotifyingGuests_SendMessages(thisPtr);

    // Set 500ms timeout
    hsmState_SetTimeout(thisPtr, 500);
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
    char* self = static_cast<char*>(thisPtr);
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == 0) {
        // Timeout — process connections at parent+1292
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 0x14);  // m_pParent
        void* connectionList = (char*)parent + 0x50C;  // m_connectionList

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
 *   +0x4A0 (snMigrating)
 *   +0x4B8 (snNotifyingGuests)
 */
void snAcquiringHost_OnExit(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);

    // Cancel notifier at this+0x4A0 (m_migrating)
    char* n1 = self + 0x4A0;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
        cancelFn(n1);
    }

    // Cancel notifier at this+0x4B8 (m_notifyingGuests)
    char* n2 = self + 0x4B8;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
        cancelFn(n2);
    }
}

/**
 * snMigratingAsGuest::OnExit @ 0x823E23C8 | size: 0x7C | vfn_6
 *
 * Cancels pending notifications on two child states:
 *   +0x18 (snWaitingForMigrateMsg)
 *   +0x4C (snMigrating)
 */
void MigratingAsGuest_OnExit(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);

    // Cancel notifier at this+0x18 (m_waitingForMigrateMsg)
    char* n1 = self + 0x18;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
        cancelFn(n1);
    }

    // Cancel notifier at this+0x4C (m_migrating)
    char* n2 = self + 0x4C;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
        cancelFn(n2);
    }
}

/**
 * snMigrateMachine::OnExit @ 0x823E2488 | size: 0x7C | vfn_6
 *
 * Cancels pending notifications on two child states:
 *   +0x54  (snAcquiringHost)
 *   +0x53C (snMigratingAsGuest)
 */
void MigrateMachine_OnExit(void* thisPtr) {
    char* self = static_cast<char*>(thisPtr);

    // Cancel notifier at this+0x54 (m_acquiringHost)
    char* n1 = self + 0x54;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
        cancelFn(n1);
    }

    // Cancel notifier at this+0x53C (m_migratingAsGuest)
    char* n2 = self + 0x53C;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 0x10);  // vtable slot 4: Cancel
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
    char* self = static_cast<char*>(thisPtr);
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == 0) {
        // Timeout — transition with failure event
        void* evt[1];
        ::hsmEvent_Init(evt);
        evt[0] = &g_vtbl_EvtMigrateAsGuestFailed;  // EvtMigrateAsGuestFailed vtable
        snMigratingAsGuest_QueueTransition(thisPtr, evt);
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 0x0C);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_MigrateNotifyDone) {
        // Migrate notification received — process connections at parent+1416
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* context = vfn_11(thisPtr);

        void* parent = *(void**)(self + 0x14);  // m_pParent
        void* connectionList = (char*)parent + 0x588;  // m_guestConnectionList

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
    char* self = static_cast<char*>(thisPtr);

    // Get current event via vfn_10
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);  // m_pChildState

    // Get session info via broadcast init
    rlEvent_Init(eventObj, 0, nullptr, nullptr, 0);

    // Get session handle and query XAM
    void* parent = *(void**)(self + 0x14);  // m_pParent
    void* session = *(void**)((char*)parent + 0x10);  // parent->m_pSession
    void* sessionHandle = (char*)session + 0xE8;  // session->m_xamContext

    // Copy data from result into session context
    uint64_t data = *(uint64_t*)((char*)stateCtx + 0x18);  // ld r5,24(r3)

    uint8_t result = xamSession_QueryStatus(sessionHandle, 0, 0);

    if (result == 0) {
        // XAM query failed — transition with failure event
        void* evt[1];
        ::hsmEvent_Init(evt);
        evt[0] = &g_vtbl_EvtMigrateAsGuestFailed;  // EvtMigrateAsGuestFailed vtable
        snMigratingAsGuest_QueueTransition(thisPtr, evt);
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
    char* self = static_cast<char*>(thisPtr);
    *handled = true;

    // Get current event type
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 0x0C);
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostSucceeded) {
        // Migration succeeded — call completion handler
        snMigrateAsGuest_OnCompleted(thisPtr);

        // Create EvtMigrateAsGuestSucceeded event
        void* evt[1];
        ::hsmEvent_Init(evt);
        void* evtSucceededVtable = &g_vtbl_EvtMigrateAsGuestSucceeded;  // EvtMigrateAsGuestSucceeded

        // Get context via vfn_11
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        evt[0] = evtSucceededVtable;
        void* hsmCtx = vfn_11(thisPtr);

        // Get session node list from context+56
        void* nodeList = *(void**)((char*)hsmCtx + 0x38);  // hsmCtx->m_eventQueue
        void* allocator = *(void**)((char*)nodeList + 0x04);  // nodeList->m_allocator
        void** allocVtable = *(void***)allocator;
        void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
        void* newNode = allocFn(allocator, 12, 0);

        if (newNode == nullptr) {
            return;
        }

        // Initialize node with event data
        *(void**)newNode = &g_vtbl_hsmEvent;  // hsmEvent vtable
        *(void**)newNode = evtSucceededVtable;
        uint32_t evtData0 = *((uint32_t*)evt + 1);
        uint32_t evtData1 = *((uint32_t*)evt + 2);
        uint64_t combined = ((uint64_t)evtData0 << 32) | evtData1;
        *(uint64_t*)((char*)newNode + 0x04) = combined;  // event payload

        snSession_AddNode((char*)nodeList + 0x08, newNode);  // nodeList->m_head
        return;
    }

    // Re-get event type for second check
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 0x0C);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_AcquireHostFailed) {
        // Migration failed — transition with failure event
        void* evt[1];
        ::hsmEvent_Init(evt);
        evt[0] = &g_vtbl_EvtMigrateAsGuestFailed;  // EvtMigrateAsGuestFailed vtable
        snMigratingAsGuest_QueueTransition(thisPtr, evt);
        return;
    }

    // Unhandled event
    *handled = false;
}

} // namespace snMigrateMachine_States

} // namespace rage
