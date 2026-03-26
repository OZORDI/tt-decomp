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

} // namespace rage
