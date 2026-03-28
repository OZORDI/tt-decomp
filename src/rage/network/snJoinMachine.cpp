/**
 * snJoinMachine.cpp — Session Network Join Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the hierarchical state machine for network join protocol.
 * Handles local joins, remote joins, join requests, and join acceptance
 * across multiple nested HSM states.
 *
 * State hierarchy:
 *   snJoinMachine (root)
 *     ├── snHsmLocalJoinPending     — waiting for local join to complete
 *     ├── snHsmRequestingJoin       — actively requesting to join a session
 *     │   ├── snHsmWaitingForReply  — sent join request, awaiting host reply
 *     │   ├── snHsmJoinPending      — join accepted, waiting for session setup
 *     │   └── snHsmUndoingJoinRequest — cancelling a pending join request
 *     ├── snHsmRemoteJoinPending    — host waiting for remote player to finish joining
 *     └── snHsmAcceptingJoinRequest — host accepting an incoming join request
 */

#include <stdint.h>

extern "C" void rage_free(void* ptr);  // @ 0x820C00C0 — heap free (C linkage)

namespace rage {

// Forward declarations
class SinglesNetworkClient;

// ────────────────────────────────────────────────────────────────────────────
// External utility functions
// ────────────────────────────────────────────────────────────────────────────

extern void snJoinMachine_SetCapacity(void* machine, int capacity);        // @ 0x822603D0 — sets max player/connection capacity
extern void snConnectionRef_GetLocal(void* outRef);                        // @ 0x823E04A0 — retrieves local connection reference
extern void snConnectionRef_Copy(void* dst, void* src);                    // @ 0x823E0500 — deep-copies a connection reference struct
extern void snConnectionRef_Release(void* client);                         // @ 0x82260268 — decrement refcount, cleanup, zero struct
extern void snLinkedList_InsertNode(void* list, void* node);               // @ 0x823DD170 — doubly-linked list insert
extern void snHsmState_Init(void* state);                                  // @ 0x823DDA08 — init HSM state, set vtables

// Join state transition helpers (each allocates + links a child state node)
extern void snHsmLocalJoinPending_TransitionFailed(void* state, void* evt);    // @ 0x823E9878
extern void snHsmWaitingForReply_TransitionFailed(void* state, void* evt);     // @ 0x823E9920
extern void snHsmRemoteJoinPending_TransitionFailed(void* state, void* evt);   // @ 0x823E99C8
extern void snHsmAcceptingJoinRequest_TransitionFailed(void* state, void* evt); // @ 0x823E9A70
extern void snHsmJoinPending_TransitionFailed(void* state, void* evt);         // @ 0x823E9680 — forward to session failure handler

// Network client query functions
extern bool snSession_IsLocalJoinPending(void* client);                    // @ 0x823EFF38 — checks if a local join is in progress
extern bool snSession_IsRemoteJoinPending(void* client);                   // @ 0x823F0040 — checks if a remote join is in progress
extern bool snSession_IsAcceptingJoinRequest(void* client);                // @ 0x823F0528 — checks if host is accepting join
extern bool snSession_HasJoinCompleted(void* client);                      // @ 0x823EFEB8 — checks if join handshake is complete
extern bool snSession_ValidateJoinReply(void* client);                     // @ 0x823F0140 — validates incoming join reply message
extern void snSession_QueryConfigStatus(void* client, void* connectionRef); // @ 0x823DA940 — query session config from host
extern void snHsmContext_SetMaxTransitions(void* machine, int maxTransitions); // @ 0x823ED4F8 — sets max transition count

// Session node management
extern void snSession_AssociateConnection(void* ctx, void* state, void* connectionList);  // @ 0x823ED988
extern void snSession_ProcessPendingConnections(void* ctx, void* state, void* connectionList); // @ 0x823EDA90
extern void snSession_AddNode(void* nodeList, void* node);                 // @ 0x823EC068 — adds a node to the session's linked list
extern void snConnectionRef_InitBroadcast(void* session, void* p1, void* p2, void* p3, int p4); // @ 0x82430978

// HSM child state attachment
extern void snHsmState_AttachChild(void* parent, void* childState);        // @ 0x823EF850

// Destructor helpers
extern void snJoinMachine_DestroyBody(void* thisPtr);                      // @ 0x823E56C0 — root join machine destructor body
extern void snHsmState_Destroy(void* thisPtr);                             // @ 0x823E8E10 — hsmState base destructor (resets vtable, zeros)
extern void snHsmNotifier_Destroy(void* thisPtr);                          // @ 0x823E6300 — notifier-level destructor (cancel pending + base dtor)

// Notification management
extern void snNotifyHandler_Unregister(void* notifyList, void* handler);   // @ 0x823B3D80 — unregister notification handler

// SendingGamerData helpers
extern void snSendingGamerData_ProcessUpdate(void* thisPtr);               // @ 0x823E01F8 — gamer data exchange handler

// Join notify utility
extern void snJoinMachine_SetCapacity_03D0(void* list, int32_t value);     // @ 0x822603D0 — also used for join notify init
extern void snJoinMachine_CopyNotifyData(void* list, uint32_t a, uint32_t b, uint32_t c); // @ 0x822608C8

// ────────────────────────────────────────────────────────────────────────────
// Global state type identifiers (runtime event type IDs loaded from .data)
// ────────────────────────────────────────────────────────────────────────────

// These globals hold event type hash values used to identify HSM events.
// Loaded via lis r11,-32163 (0x825D0000) + offset patterns.
extern uint32_t g_evtType_JoinSucceeded;     // @ 0x825D181C (offset 6172)
extern uint32_t g_evtType_JoinFailed;        // @ 0x825D1828 (offset 6184)
extern uint32_t g_evtType_RequestJoinOk;     // @ 0x825D1864 (offset 6244) — join reply accepted
extern uint32_t g_evtType_RequestJoinDenied; // @ 0x825D1870 (offset 6256) — join reply denied

// ────────────────────────────────────────────────────────────────────────────
// Event vtable pointers (from .rdata, used to construct stack events)
// ────────────────────────────────────────────────────────────────────────────

// vtable<rage::hsmEvent>                    @ 0x82072864
extern void* g_vtable_hsmEvent;
// vtable<rage::EvtLocalJoinSucceeded>       @ 0x82072D1C
extern void* g_vtable_EvtLocalJoinSucceeded;
// vtable<rage::EvtLocalJoinFailed>          @ 0x82072D30
extern void* g_vtable_EvtLocalJoinFailed;
// vtable<rage::EvtRemoteJoinSucceeded>      @ 0x82072D44
extern void* g_vtable_EvtRemoteJoinSucceeded;
// vtable<rage::EvtRemoteJoinFailed>         @ 0x82072D58
extern void* g_vtable_EvtRemoteJoinFailed;
// vtable<rage::EvtRequestJoinSucceeded>     @ 0x82072D6C
extern void* g_vtable_EvtRequestJoinSucceeded;
// vtable<rage::EvtRequestJoinFailed>        @ 0x82072D80
extern void* g_vtable_EvtRequestJoinFailed;
// vtable<rage::EvtAcceptJoinRequestFailed>  @ 0x82072DA8
extern void* g_vtable_EvtAcceptJoinRequestFailed;
// vtable<rage::EvtSessionJoinFailed>        @ 0x82072DD0
extern void* g_vtable_EvtSessionJoinFailed;

// Callback/handler vtable used for join completion notification
// @ 0x823EB0F8 — thunk for join completion callback
extern void* g_joinCompletionHandler;

// ────────────────────────────────────────────────────────────────────────────
// snJoinMachine::OnEnter @ 0x823DF060 | size: 0xD4 | vfn_14
//
// Initializes the join machine when entering the join state.
// - Clears session fields (join status, flags, connection list at +324)
// - Resets capacity via snJoinMachine_SetCapacity
// - Copies the local connection reference into the machine at +464
// - Releases the temporary connection reference
// - Clears bit 7 of flags byte at +624
// - Sets up the join completion callback at +24
// - Inserts the machine into the session's linked list at session+232
// ────────────────────────────────────────────────────────────────────────────
void snJoinMachine_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;

    // Clear join status fields
    *(uint32_t*)(self + 292) = 0;   // join result
    *(uint8_t*)(self + 296) = 0;    // join active flag
    *(uint64_t*)(self + 312) = 0;   // join session ID
    *(uint32_t*)(self + 320) = (uint32_t)-1; // join player index (none)

    // Zero-fill the connection slot array at +324 (9 entries x 4 bytes)
    uint32_t* connSlots = (uint32_t*)(self + 324);
    for (int i = 0; i < 9; i++) {
        connSlots[i] = 0;
    }

    // Reset capacity to 0
    snJoinMachine_SetCapacity((void*)(self + 360), 0);

    // Get local connection reference (into stack temp)
    char localConnRef[52]; // stack frame at sp+80
    snConnectionRef_GetLocal(localConnRef);

    // Copy local connection address (8 bytes at +4) into machine at +468
    *(uint32_t*)(self + 468) = *(uint32_t*)(localConnRef + 4);
    *(uint32_t*)(self + 472) = *(uint32_t*)(localConnRef + 8);

    // Deep-copy the connection reference payload (+16 onward)
    snConnectionRef_Copy((void*)(self + 480), (void*)(localConnRef + 16));

    // Release the temporary local connection reference
    char releaseRef[52]; // stack frame at sp+132
    snConnectionRef_Release(releaseRef);

    // Clear bit 7 of flags byte at +624 (preserve lower 7 bits)
    *(uint8_t*)(self + 624) = *(uint8_t*)(self + 624) & 0x7F;

    // Set up join completion callback at +24
    // Store vtable for hsmEvent at sp+80 (used as event context)
    // Set handler pointer at +32 to joinCompletionHandler
    *(void**)(self + 32) = &g_joinCompletionHandler; // @ 0x823EB0F8
    *(void**)(self + 28) = thisPtr;                   // self-reference

    // Insert into session's linked list at session+232
    void* session = *(void**)(self + 16);
    snLinkedList_InsertNode((char*)session + 232, thisPtr);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmLocalJoinPending::OnEnter @ 0x823DF638 | size: 0x70 | vfn_14
//
// Called when entering the LocalJoinPending state.
// Checks if a local join is still pending and the join handshake has
// completed. If both conditions fail, transitions to the failure state
// by firing an EvtLocalJoinFailed event.
// ────────────────────────────────────────────────────────────────────────────
void snHsmLocalJoinPending_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;
    void* client = *(void**)(self + 20);  // m_pNetworkClient

    // Check if local join is still in progress
    bool localJoinPending = snSession_IsLocalJoinPending(client);
    if (localJoinPending) {
        // Also check if join handshake has completed
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return; // Join is pending and handshake done — stay in state
        }
    }

    // Local join is not pending or handshake incomplete — transition to failed
    char evt[32]; // stack event at sp+80
    snHsmState_Init(evt);
    *(void**)evt = &g_vtable_EvtLocalJoinFailed; // set vtable to EvtLocalJoinFailed
    snHsmLocalJoinPending_TransitionFailed(thisPtr, evt);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmLocalJoinPending::OnEvent @ 0x823DF6A8 | size: 0x158 | vfn_12
//
// Tick handler for LocalJoinPending state.
// Checks incoming events against known join event types:
// - JoinSucceeded: fires EvtLocalJoinSucceeded, allocates node, adds to
//   session list
// - JoinFailed: fires EvtLocalJoinFailed, transitions to failed state
// If neither matches, marks event as unhandled.
// ────────────────────────────────────────────────────────────────────────────
void snHsmLocalJoinPending_OnEvent(void* thisPtr, void* event, bool* handled) { // vfn_12
    char* self = (char*)thisPtr;
    *handled = true;

    // Get current event from state machine: vfn_10 -> get event payload
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);

    // Get event type ID via vtable slot 1
    void** evtVtable = *(void***)eventObj;
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    uint32_t typeId = getTypeId(eventObj);

    if (typeId == g_evtType_JoinSucceeded) {
        // Join succeeded — fire EvtLocalJoinSucceeded event
        char evt[32];
        snHsmState_Init(evt);

        void* evtLocalJoinSucceededVtable = &g_vtable_EvtLocalJoinSucceeded;

        // Call vfn_11 to get the HSM context for state transitions
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        // Get session node list from HSM context (+56)
        void* nodeList = *(void**)((char*)hsmCtx + 56);

        // Allocate a new node (12 bytes, type 0) via session's allocator (+4)
        void* allocator = *(void**)((char*)nodeList + 4);
        void** allocVtable = *(void***)allocator;
        void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
        void* newNode = allocFn(allocator, 12, 0);

        if (newNode == nullptr) {
            return;
        }

        // Initialize the new node: set vtable to hsmEvent, copy event payload
        *(void**)newNode = &g_vtable_hsmEvent;
        // Copy 8 bytes of event data from stack event
        *(void**)newNode = evtLocalJoinSucceededVtable;
        uint32_t evtData0 = *(uint32_t*)(evt + 4);
        uint32_t evtData1 = *(uint32_t*)(evt + 8);
        uint64_t combined = ((uint64_t)evtData0 << 32) | evtData1;
        *(uint64_t*)((char*)newNode + 4) = combined;

        // Add node to session list
        snSession_AddNode((char*)nodeList + 8, newNode);
    } else {
        // Check for JoinFailed event
        stateCtx = vfn_10(thisPtr);
        eventObj = *(void**)((char*)stateCtx + 12);
        evtVtable = *(void***)eventObj;
        getTypeId = (uint32_t (*)(void*))evtVtable[1];
        typeId = getTypeId(eventObj);

        if (typeId == g_evtType_JoinFailed) {
            // Join failed — transition to failure state
            char evt[16];
            snHsmState_Init(evt);
            *(void**)evt = &g_vtable_EvtLocalJoinFailed;
            snHsmLocalJoinPending_TransitionFailed(thisPtr, evt);
        } else {
            // Unhandled event
            *handled = false;
        }
    }
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmWaitingForReply::OnEnter @ 0x823DF870 | size: 0xB8 | vfn_14
//
// Called when entering the WaitingForReply state (sub-state of
// snHsmRequestingJoin). Sets up the join completion callback at +24,
// queries session config from the host via the connection reference at
// m_pNetworkClient+164, then validates the join reply.
//
// If validation fails, transitions to the failure state with
// EvtRequestJoinFailed. If validation succeeds, computes a timeout
// from the connection's round-trip time: timeout = (rtt + 50) * rtt * 100.
// ────────────────────────────────────────────────────────────────────────────
void snHsmWaitingForReply_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;

    // Set up callback at +24: store self at +28, handler at +32
    *(void**)(self + 28) = thisPtr;
    *(void**)(self + 32) = (void*)0x823F1508; // thunk_fn_823F0D68 — reply timeout handler

    // Query session config from host connection
    void* networkClient = *(void**)(self + 16);
    void* connectionRef = (void*)(*(uint32_t*)((char*)networkClient + 164));
    snSession_QueryConfigStatus(connectionRef, (void*)(self + 24));

    // Validate the join reply from the client
    void* client = *(void**)(self + 20);
    bool replyValid = snSession_ValidateJoinReply(client);

    if (!replyValid) {
        // No valid reply yet — transition to failure state
        char evt[32];
        snHsmState_Init(evt);
        *(void**)evt = &g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(thisPtr, evt);
        return;
    }

    // Reply is valid — compute timeout from round-trip time
    // Get connection info from client: client -> +16 -> +404 and +408
    void* clientInner = *(void**)((char*)client + 16);
    uint32_t rttBase = *(uint32_t*)((char*)clientInner + 404);
    uint32_t rttExtra = *(uint32_t*)((char*)clientInner + 408);
    int32_t rtt = (int32_t)(rttBase + rttExtra);
    int32_t rttPadded = rtt + 50;
    int32_t timeout = rttPadded * rtt * 100;

    snHsmContext_SetMaxTransitions(thisPtr, timeout);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmWaitingForReply::OnEvent @ 0x823DF928 | size: 0x1B4 | vfn_12
//
// Tick handler for WaitingForReply state.
// Checks incoming events:
// - RequestJoinOk: join accepted — call vfn_11 for context, get connection
//   list from client+144, associate and process connections
// - RequestJoinDenied: join denied — get session info via
//   snConnectionRef_InitBroadcast, copy connection data into client+464,
//   then transition to failure with EvtRequestJoinFailed
// - Event type 0 (timeout): transition to failure with EvtRequestJoinFailed
// If none match, marks event as unhandled.
// ────────────────────────────────────────────────────────────────────────────
void snHsmWaitingForReply_OnEvent(void* thisPtr, void* event, bool* handled) { // vfn_12
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

    if (typeId == g_evtType_RequestJoinOk) {
        // Join request accepted — transition via vfn_11 and process connections
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        // Get connection list from m_pNetworkClient+144
        void* client = *(void**)(self + 20);
        void* connectionList = (char*)client + 144;

        snSession_AssociateConnection(hsmCtx, thisPtr, connectionList);
        snSession_ProcessPendingConnections(hsmCtx, thisPtr, connectionList);
        return;
    }

    // Check for RequestJoinDenied
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_RequestJoinDenied) {
        // Join denied — retrieve session info and copy connection data
        stateCtx = vfn_10(thisPtr);
        eventObj = *(void**)((char*)stateCtx + 12);

        // Get denied session info via broadcast init
        // Uses addresses from .rdata for broadcast params
        snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

        // Copy connection address (8 bytes) from result into client+468
        void* client = *(void**)(self + 20);
        char* clientConn = (char*)client + 464;
        // Copy 8 bytes of address data
        uint32_t addr0 = *(uint32_t*)((char*)eventObj + 4);
        uint32_t addr1 = *(uint32_t*)((char*)eventObj + 8);
        *(uint32_t*)(clientConn + 4) = addr0;
        *(uint32_t*)(clientConn + 8) = addr1;

        // Deep-copy the rest
        snConnectionRef_Copy((char*)clientConn + 16, (char*)eventObj + 16);

        // Transition to failure
        char evt[32];
        snHsmState_Init(evt);
        *(void**)evt = &g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(thisPtr, evt);
        return;
    }

    // Check for timeout (event type == 0)
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == 0) {
        // Timeout — transition to failure
        char evt[16];
        snHsmState_Init(evt);
        *(void**)evt = &g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(thisPtr, evt);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmJoinPending::OnEnter @ 0x823DFAF0 | size: 0xC4 | vfn_14
//
// Called when entering the JoinPending state (join accepted, setting up
// session). Retrieves the session info via snConnectionRef_InitBroadcast
// with the accepted join data, copies the connection reference into
// the client at +464, copies the player index from client+620 into
// client+320, then checks if the join handshake has completed.
// If not complete, transitions to failure with EvtSessionJoinFailed.
// ────────────────────────────────────────────────────────────────────────────
void snHsmJoinPending_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;

    // Call vfn_10 to get current state context
    void** vtable = *(void***)thisPtr;
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* stateCtx = vfn_10(thisPtr);
    void* eventObj = *(void**)((char*)stateCtx + 12);

    // Retrieve session info from the accepted join
    snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

    // Copy connection data into client+464
    void* client = *(void**)(self + 20);
    char* clientConn = (char*)client + 464;

    uint32_t addr0 = *(uint32_t*)((char*)eventObj + 4);
    uint32_t addr1 = *(uint32_t*)((char*)eventObj + 8);
    *(uint32_t*)(clientConn + 4) = addr0;
    *(uint32_t*)(clientConn + 8) = addr1;

    snConnectionRef_Copy((char*)clientConn + 16, (char*)eventObj + 16);

    // Copy player index from client+620 to client+320
    uint32_t playerIndex = *(uint32_t*)((char*)client + 620);
    *(uint32_t*)((char*)client + 320) = playerIndex;

    // Check if join handshake has completed
    bool joinCompleted = snSession_HasJoinCompleted(client);
    if (!joinCompleted) {
        // Not complete — transition to session join failure
        char evt[32];
        snHsmState_Init(evt);
        *(void**)evt = &g_vtable_EvtSessionJoinFailed;
        snHsmJoinPending_TransitionFailed(thisPtr, evt);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmJoinPending::OnEvent @ 0x823DFBB8 | size: 0x164 | vfn_12
//
// Tick handler for JoinPending state.
// Checks incoming events:
// - JoinSucceeded: fires EvtRequestJoinSucceeded, allocates node,
//   adds to session list
// - JoinFailed: calls vfn_11 for context, gets connection list from
//   client+168, associates and processes connections
// If neither matches, marks event as unhandled.
// ────────────────────────────────────────────────────────────────────────────
void snHsmJoinPending_OnEvent(void* thisPtr, void* event, bool* handled) { // vfn_12
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

    if (typeId == g_evtType_JoinSucceeded) {
        // Session join succeeded — fire EvtRequestJoinSucceeded
        char evt[32];
        snHsmState_Init(evt);

        void* evtRequestJoinSucceededVtable = &g_vtable_EvtRequestJoinSucceeded;

        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        void* nodeList = *(void**)((char*)hsmCtx + 56);
        void* allocator = *(void**)((char*)nodeList + 4);
        void** allocVtable = *(void***)allocator;
        void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
        void* newNode = allocFn(allocator, 12, 0);

        if (newNode == nullptr) {
            return;
        }

        *(void**)newNode = &g_vtable_hsmEvent;
        *(void**)newNode = evtRequestJoinSucceededVtable;
        uint32_t evtData0 = *(uint32_t*)(evt + 4);
        uint32_t evtData1 = *(uint32_t*)(evt + 8);
        uint64_t combined = ((uint64_t)evtData0 << 32) | evtData1;
        *(uint64_t*)((char*)newNode + 4) = combined;

        snSession_AddNode((char*)nodeList + 8, newNode);
        return;
    }

    // Check for JoinFailed event
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_JoinFailed) {
        // Join failed — call vfn_11, get connection list from client+168
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        void* client = *(void**)(self + 20);
        void* connectionList = (char*)client + 168;

        snSession_AssociateConnection(hsmCtx, thisPtr, connectionList);
        snSession_ProcessPendingConnections(hsmCtx, thisPtr, connectionList);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmRemoteJoinPending::OnEnter @ 0x823DFE70 | size: 0x70 | vfn_14
//
// Called when entering the RemoteJoinPending state (host side).
// Checks if a remote join is still pending and the join handshake has
// completed. If either check fails, transitions to the failure state
// by firing an EvtRemoteJoinFailed event.
// ────────────────────────────────────────────────────────────────────────────
void snHsmRemoteJoinPending_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;
    void* client = *(void**)(self + 20);

    // Check if remote join is still in progress
    bool remoteJoinPending = snSession_IsRemoteJoinPending(client);
    if (remoteJoinPending) {
        // Also check if join handshake has completed
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return; // Remote join pending and handshake done — stay in state
        }
    }

    // Remote join not pending or handshake incomplete — transition to failed
    char evt[32];
    snHsmState_Init(evt);
    *(void**)evt = &g_vtable_EvtRemoteJoinFailed;
    snHsmRemoteJoinPending_TransitionFailed(thisPtr, evt);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmRemoteJoinPending::OnEvent @ 0x823DFEE0 | size: 0x158 | vfn_12
//
// Tick handler for RemoteJoinPending state (host side).
// Checks incoming events:
// - JoinSucceeded: fires EvtRemoteJoinSucceeded, allocates node,
//   adds to session list
// - JoinFailed: fires EvtRemoteJoinFailed, transitions to failed state
// If neither matches, marks event as unhandled.
// ────────────────────────────────────────────────────────────────────────────
void snHsmRemoteJoinPending_OnEvent(void* thisPtr, void* event, bool* handled) { // vfn_12
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

    if (typeId == g_evtType_JoinSucceeded) {
        // Remote join succeeded — fire EvtRemoteJoinSucceeded
        char evt[32];
        snHsmState_Init(evt);

        void* evtRemoteJoinSucceededVtable = &g_vtable_EvtRemoteJoinSucceeded;

        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        void* nodeList = *(void**)((char*)hsmCtx + 56);
        void* allocator = *(void**)((char*)nodeList + 4);
        void** allocVtable = *(void***)allocator;
        void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
        void* newNode = allocFn(allocator, 12, 0);

        if (newNode == nullptr) {
            return;
        }

        *(void**)newNode = &g_vtable_hsmEvent;
        *(void**)newNode = evtRemoteJoinSucceededVtable;
        uint32_t evtData0 = *(uint32_t*)(evt + 4);
        uint32_t evtData1 = *(uint32_t*)(evt + 8);
        uint64_t combined = ((uint64_t)evtData0 << 32) | evtData1;
        *(uint64_t*)((char*)newNode + 4) = combined;

        snSession_AddNode((char*)nodeList + 8, newNode);
    } else {
        // Check for JoinFailed event
        stateCtx = vfn_10(thisPtr);
        eventObj = *(void**)((char*)stateCtx + 12);
        evtVtable = *(void***)eventObj;
        getTypeId = (uint32_t (*)(void*))evtVtable[1];
        typeId = getTypeId(eventObj);

        if (typeId == g_evtType_JoinFailed) {
            // Remote join failed — transition to failure
            char evt[16];
            snHsmState_Init(evt);
            *(void**)evt = &g_vtable_EvtRemoteJoinFailed;
            snHsmRemoteJoinPending_TransitionFailed(thisPtr, evt);
        } else {
            // Unhandled event
            *handled = false;
        }
    }
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmAcceptingJoinRequest::OnEnter @ 0x823E0048 | size: 0x70 | vfn_14
//
// Called when entering the AcceptingJoinRequest state (host side).
// Checks if the host is still accepting join requests and the join
// handshake has completed. If either check fails, transitions to the
// failure state by firing an EvtAcceptJoinRequestFailed event.
// ────────────────────────────────────────────────────────────────────────────
void snHsmAcceptingJoinRequest_OnEnter(void* thisPtr) { // vfn_14
    char* self = (char*)thisPtr;
    void* client = *(void**)(self + 20);

    // Check if host is still accepting join requests
    bool accepting = snSession_IsAcceptingJoinRequest(client);
    if (accepting) {
        // Also check if join handshake has completed
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return; // Still accepting and handshake done — stay in state
        }
    }

    // Not accepting or handshake incomplete — transition to failed
    char evt[32];
    snHsmState_Init(evt);
    *(void**)evt = &g_vtable_EvtAcceptJoinRequestFailed;
    snHsmAcceptingJoinRequest_TransitionFailed(thisPtr, evt);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmAcceptingJoinRequest::OnEvent @ 0x823E00B8 | size: 0x100 | vfn_12
//
// Tick handler for AcceptingJoinRequest state (host side).
// Checks incoming events:
// - JoinSucceeded: calls vfn_11 for context, gets connection list from
//   client+240, associates and processes connections
// - JoinFailed: fires EvtAcceptJoinRequestFailed, transitions to failed
// If neither matches, marks event as unhandled.
// ────────────────────────────────────────────────────────────────────────────
void snHsmAcceptingJoinRequest_OnEvent(void* thisPtr, void* event, bool* handled) { // vfn_12
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

    if (typeId == g_evtType_JoinSucceeded) {
        // Accept succeeded — call vfn_11, get connection list from client+240
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(thisPtr);

        void* client = *(void**)(self + 20);
        void* connectionList = (char*)client + 240;

        snSession_AssociateConnection(hsmCtx, thisPtr, connectionList);
        snSession_ProcessPendingConnections(hsmCtx, thisPtr, connectionList);
        return;
    }

    // Check for JoinFailed event
    stateCtx = vfn_10(thisPtr);
    eventObj = *(void**)((char*)stateCtx + 12);
    evtVtable = *(void***)eventObj;
    getTypeId = (uint32_t (*)(void*))evtVtable[1];
    typeId = getTypeId(eventObj);

    if (typeId == g_evtType_JoinFailed) {
        // Accept failed — transition to failure
        char evt[32];
        snHsmState_Init(evt);
        *(void**)evt = &g_vtable_EvtAcceptJoinRequestFailed;
        snHsmAcceptingJoinRequest_TransitionFailed(thisPtr, evt);
        return;
    }

    // Unhandled event
    *handled = false;
}

// ────────────────────────────────────────────────────────────────────────────
// snJoinMachine HSM State Name Strings (verified from binary .rdata)
// ────────────────────────────────────────────────────────────────────────────

static const char* const s_JoinMachine          = "JoinMachine";          // @ 0x82074368
static const char* const s_LocalJoinPending     = "LocalJoinPending";     // @ 0x82074374
static const char* const s_RequestingJoin       = "RequestingJoin";       // @ 0x82074388
static const char* const s_WaitingForReply      = "WaitingForReply";      // @ 0x82074398
static const char* const s_JoinPending          = "JoinPending";          // @ 0x820743A8
static const char* const s_UndoingJoinRequest   = "UndoingJoinRequest";   // @ 0x820743B4
static const char* const s_RemoteJoinPending    = "RemoteJoinPending";    // @ 0x820743C8
static const char* const s_AcceptingJoinRequest = "AcceptingJoinRequest"; // @ 0x820743DC
static const char* const s_SendingGamerData     = "SendingGamerData";     // @ 0x820743F4

// ────────────────────────────────────────────────────────────────────────────
// GetStateName functions (vfn_2, 12B each)
// Each returns a const char* from .rdata for its HSM state display name.
// ────────────────────────────────────────────────────────────────────────────

const char* snJoinMachine_GetStateName()              { return s_JoinMachine; }          // @ 0x823DF050
const char* snHsmLocalJoinPending_GetStateName()      { return s_LocalJoinPending; }     // @ 0x823DF628
const char* snHsmRequestingJoin_GetStateName()        { return s_RequestingJoin; }       // @ 0x823DF800
const char* snHsmWaitingForReply_GetStateName()       { return s_WaitingForReply; }      // @ 0x823DF860
const char* snHsmJoinPending_GetStateName()           { return s_JoinPending; }          // @ 0x823DFAE0
const char* snHsmUndoingJoinRequest_GetStateName()    { return s_UndoingJoinRequest; }   // @ 0x823DFD20
const char* snHsmRemoteJoinPending_GetStateName()     { return s_RemoteJoinPending; }    // @ 0x823DFE60
const char* snHsmAcceptingJoinRequest_GetStateName()  { return s_AcceptingJoinRequest; } // @ 0x823E0038
const char* snSendingGamerData_GetStateName()         { return s_SendingGamerData; }     // @ 0x823E01B8

// ────────────────────────────────────────────────────────────────────────────
// Destructors (vfn_0)
// Pattern: call state-level cleanup helper, conditionally free if flags & 1.
// ────────────────────────────────────────────────────────────────────────────

/**
 * snJoinMachine::~snJoinMachine @ 0x823E5E88 | size: 0x50 | vfn_0
 *
 * Root destructor. Calls snJoinMachine_DestroyBody to tear down all child
 * states and notification handlers, then conditionally frees memory.
 */
void snJoinMachine_Destructor(void* thisPtr, uint32_t flags) {
    snJoinMachine_DestroyBody(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snHsmLocalJoinPending::~snHsmLocalJoinPending @ 0x823E6240 | size: 0x50 | vfn_0
 *
 * Destructor for the LocalJoinPending state. Calls hsmState base destructor
 * to reset vtable and zero out fields.
 */
void snHsmLocalJoinPending_Destructor(void* thisPtr, uint32_t flags) {
    snHsmState_Destroy(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snHsmRequestingJoin::~snHsmRequestingJoin @ 0x823E6290 | size: 0x6C | vfn_0
 *
 * Destructor for the RequestingJoin state. Tears down three embedded child
 * states at offsets +96 (UndoingJoinRequest), +72 (JoinPending), and
 * +24 (WaitingForReply notifier), then destroys this state itself.
 */
void snHsmRequestingJoin_Destructor(void* thisPtr, uint32_t flags) {
    snHsmState_Destroy((char*)thisPtr + 96);
    snHsmState_Destroy((char*)thisPtr + 72);
    snHsmNotifier_Destroy((char*)thisPtr + 24);
    snHsmState_Destroy(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snHsmWaitingForReply::~snHsmWaitingForReply @ 0x823E6610 | size: 0x50 | vfn_0
 *
 * Destructor for the WaitingForReply sub-state. Calls notifier-level
 * destructor to cancel pending notifications and clean up.
 */
void snHsmWaitingForReply_Destructor(void* thisPtr, uint32_t flags) {
    snHsmNotifier_Destroy(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// OnEnter Handlers (vfn_5)
// ────────────────────────────────────────────────────────────────────────────

/**
 * snJoinMachine::OnEnter_AttachChildren @ 0x823E0358 | size: 0x60 | vfn_5
 *
 * Attaches five child sub-states at fixed offsets within the JoinMachine:
 *   +48  (LocalJoinPending)
 *   +72  (RemoteJoinPending)
 *   +192 (AcceptingJoinRequest)
 *   +216 (RequestingJoin)
 *   +240 (SendingGamerData)
 */
void snJoinMachine_AttachChildren(void* thisPtr) {
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 48);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 72);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 192);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 216);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 240);
}

/**
 * snHsmRequestingJoin::OnEnter_AttachChildren @ 0x823DFD78 | size: 0x48 | vfn_5
 *
 * Attaches three child sub-states:
 *   +24 (WaitingForReply notifier)
 *   +72 (JoinPending)
 *   +96 (UndoingJoinRequest)
 */
void snHsmRequestingJoin_AttachChildren(void* thisPtr) {
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 24);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 72);
    snHsmState_AttachChild(thisPtr, (char*)thisPtr + 96);
}

/**
 * snHsmUndoingJoinRequest::OnEnter @ 0x823DFD30 | size: 0x48 | vfn_14
 *
 * Initializes a stack HSM state, sets the transition target vtable to
 * EvtRequestJoinFailed (@ 0x82072D80), then transitions to the
 * WaitingForReply failure state.
 */
void snHsmUndoingJoinRequest_OnEnter(void* thisPtr) {
    char stackState[32];
    snHsmState_Init(stackState);
    *(void**)stackState = &g_vtable_EvtRequestJoinFailed;
    snHsmWaitingForReply_TransitionFailed(thisPtr, stackState);
}

// ────────────────────────────────────────────────────────────────────────────
// OnExit Handlers
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmRequestingJoin::OnExit @ 0x823DFDC0 | size: 0xA0 | vfn_6
 *
 * Exit handler for RequestingJoin. Cancels pending notifications on three
 * embedded rlNotifier objects at offsets +24, +72, and +96.
 * For each: if the pending callback (+8) is non-null, clears it and invokes
 * the linked object's Cancel function (vtable slot 4, byte offset +16).
 */
void snHsmRequestingJoin_OnExit(void* thisPtr) {
    // Cancel notifier at this+24
    char* notifier1 = (char*)thisPtr + 24;
    if (*(void**)(notifier1 + 8) != nullptr) {
        void* linked1 = *(void**)notifier1;
        *(void**)(notifier1 + 8) = nullptr;
        void (*cancelFn1)(void*) = *(void (**)(void*))((char*)linked1 + 16);
        cancelFn1(notifier1);
    }

    // Cancel notifier at this+72
    char* notifier2 = (char*)thisPtr + 72;
    if (*(void**)(notifier2 + 8) != nullptr) {
        void* linked2 = *(void**)notifier2;
        *(void**)(notifier2 + 8) = nullptr;
        void (*cancelFn2)(void*) = *(void (**)(void*))((char*)linked2 + 16);
        cancelFn2(notifier2);
    }

    // Cancel notifier at this+96
    char* notifier3 = (char*)thisPtr + 96;
    if (*(void**)(notifier3 + 8) != nullptr) {
        void* linked3 = *(void**)notifier3;
        *(void**)(notifier3 + 8) = nullptr;
        void (*cancelFn3)(void*) = *(void (**)(void*))((char*)linked3 + 16);
        cancelFn3(notifier3);
    }
}

/**
 * snHsmWaitingForReply::OnExit @ 0x823E3588 | size: 0x44 | vfn_15
 *
 * Exit handler for WaitingForReply. Unregisters the notification handler
 * from the connection data's notification list at +672, then clears the
 * pending notification pointer at notifier+20.
 */
void snHsmWaitingForReply_OnExit(void* thisPtr) {
    void* session = *(void**)((char*)thisPtr + 16);
    char* notifier = (char*)thisPtr + 24;

    void* connData = *(void**)((char*)session + 164);
    snNotifyHandler_Unregister((char*)connData + 672, notifier);

    *(uint32_t*)(notifier + 20) = 0;
}

// ────────────────────────────────────────────────────────────────────────────
// OnUpdate Handler
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmRequestingJoin::OnUpdate @ 0x823DF810 | size: 0x50 | vfn_13
 *
 * Retrieves the session context via vfn_11, then associates and processes
 * pending connections using the join machine's connection list at
 * m_pNetworkClient+96.
 */
void snHsmRequestingJoin_OnUpdate(void* thisPtr) {
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);

    void* joinMachine = *(void**)((char*)thisPtr + 20);
    void* connectionList = (char*)joinMachine + 96;

    snSession_AssociateConnection(context, thisPtr, connectionList);
    snSession_ProcessPendingConnections(context, thisPtr, connectionList);
}

// ────────────────────────────────────────────────────────────────────────────
// snSendingGamerData — Simple State Functions
// ────────────────────────────────────────────────────────────────────────────

/**
 * snSendingGamerData::OnEnter @ 0x823E01E0 | size: 0x14 | vfn_14
 *
 * Clears three tracking fields at +40, +44, +48 on state entry.
 */
void snSendingGamerData_OnEnter(void* thisPtr) {
    *(uint32_t*)((char*)thisPtr + 40) = 0;
    *(uint32_t*)((char*)thisPtr + 44) = 0;
    *(uint32_t*)((char*)thisPtr + 48) = 0;
}

/**
 * snSendingGamerData::GetParentState @ 0x823E01D0 | size: 0x10 | vfn_8
 *
 * Stores `this` at this+24 and returns the address of this+24, linking
 * the SendingGamerData state back to its parent in the HSM hierarchy.
 */
void* snSendingGamerData_GetParentState(void* thisPtr) {
    *(void**)((char*)thisPtr + 24) = thisPtr;
    return (char*)thisPtr + 24;
}

/**
 * snSendingGamerData::OnUpdate @ 0x823E01C8 | size: 0x4 | vfn_9
 *
 * Tail-calls into snSendingGamerData_ProcessUpdate which handles the
 * actual gamer data exchange protocol.
 */
void snSendingGamerData_OnUpdate(void* thisPtr) {
    snSendingGamerData_ProcessUpdate(thisPtr);
}

// ────────────────────────────────────────────────────────────────────────────
// Utility Functions
// ────────────────────────────────────────────────────────────────────────────

/**
 * snJoinMachine::InitJoinNotify @ 0x823EFDF0 | size: 0x64
 *
 * Initializes a join notification entry. Stores the callback pointer at
 * this+4, initializes the join count list at this+8 via SetCapacity(0),
 * and if a source notification is provided, copies its data fields.
 */
void snJoinMachine_InitJoinNotify(void* thisPtr, void* callback, void* source) {
    char* self = (char*)thisPtr;

    *(void**)(self + 4) = callback;
    snJoinMachine_SetCapacity(self + 8, 0);

    if (source != nullptr) {
        uint32_t field16 = *(uint32_t*)((char*)source + 16);
        uint32_t field24 = *(uint32_t*)((char*)source + 24);
        uint32_t field4  = *(uint32_t*)((char*)source + 4);
        snJoinMachine_CopyNotifyData(self + 8, field4, field16, field24);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// snJoinMachine::OnExit @ 0x823E03B8 | size: 0xE8 | vfn_6
//
// Exit handler for the root JoinMachine state. Cancels pending
// notifications on five embedded rlNotifier objects at offsets
// +48, +72, +192, +216, and +240. For each: if the pending callback
// (+8) is non-null, clears it and invokes the linked Cancel function
// (vtable slot 4, byte offset +16).
// ────────────────────────────────────────────────────────────────────────────
void snJoinMachine_OnExit(void* thisPtr) { // vfn_6
    char* self = (char*)thisPtr;

    // Cancel notifier at this+48 (LocalJoinPending)
    char* n1 = self + 48;
    if (*(void**)(n1 + 8) != nullptr) {
        void* linked = *(void**)n1;
        *(void**)(n1 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n1);
    }

    // Cancel notifier at this+72 (RemoteJoinPending)
    char* n2 = self + 72;
    if (*(void**)(n2 + 8) != nullptr) {
        void* linked = *(void**)n2;
        *(void**)(n2 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n2);
    }

    // Cancel notifier at this+192 (AcceptingJoinRequest)
    char* n3 = self + 192;
    if (*(void**)(n3 + 8) != nullptr) {
        void* linked = *(void**)n3;
        *(void**)(n3 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n3);
    }

    // Cancel notifier at this+216 (RequestingJoin)
    char* n4 = self + 216;
    if (*(void**)(n4 + 8) != nullptr) {
        void* linked = *(void**)n4;
        *(void**)(n4 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n4);
    }

    // Cancel notifier at this+240 (SendingGamerData)
    char* n5 = self + 240;
    if (*(void**)(n5 + 8) != nullptr) {
        void* linked = *(void**)n5;
        *(void**)(n5 + 8) = nullptr;
        void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
        cancelFn(n5);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// snJoinMachine::FireEvent @ 0x823E97D0 | size: 0xA4
//
// Fires an event through the session's event node list. Calls vfn_11 to
// get the HSM context, retrieves the session node list from context+56,
// allocates a 12-byte event node, copies the event vtable and 8 bytes
// of payload, then adds the node to the session's linked list.
//
// @param thisPtr The snJoinMachine instance
// @param event   The event data to fire (vtable + 8 bytes payload at +4)
// ────────────────────────────────────────────────────────────────────────────
void snJoinMachine_FireEvent(void* thisPtr, void* event) {
    // Call vfn_11 to get the HSM context
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* hsmCtx = vfn_11(thisPtr);

    // Get session node list from context+56
    void* nodeList = *(void**)((char*)hsmCtx + 56);

    // Allocate a 12-byte event node (type 0) via session's allocator at +4
    void* allocator = *(void**)((char*)nodeList + 4);
    void** allocVtable = *(void***)allocator;
    void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
    void* newNode = allocFn(allocator, 12, 0);

    if (newNode != nullptr) {
        char* evtSrc = (char*)event;

        // Set vtable to hsmEvent, then copy source event data
        *(void**)newNode = &g_vtable_hsmEvent;

        // Copy 8 bytes of address from event+4 into node+4
        uint32_t addr0 = *(uint32_t*)(evtSrc + 4);
        uint32_t addr1 = *(uint32_t*)(evtSrc + 8);

        // Overwrite vtable with event-specific vtable from source
        void* evtVtable = *(void**)evtSrc;
        *(void**)newNode = evtVtable;

        *(uint32_t*)((char*)newNode + 4) = addr0;
        *(uint32_t*)((char*)newNode + 8) = addr1;

        // Add node to session list
        snSession_AddNode((char*)nodeList + 8, newNode);
    }
}

} // namespace rage
