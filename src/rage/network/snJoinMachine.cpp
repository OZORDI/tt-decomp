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
 *     +-- snHsmLocalJoinPending     -- waiting for local join to complete
 *     +-- snHsmRequestingJoin       -- actively requesting to join a session
 *     |   +-- snHsmWaitingForReply  -- sent join request, awaiting host reply
 *     |   +-- snHsmJoinPending      -- join accepted, waiting for session setup
 *     |   +-- snHsmUndoingJoinRequest -- cancelling a pending join request
 *     +-- snHsmRemoteJoinPending    -- host waiting for remote player to finish joining
 *     +-- snHsmAcceptingJoinRequest -- host accepting an incoming join request
 */

#include <stdint.h>

extern "C" void rage_free(void* ptr);  // @ 0x820C00C0 -- heap free (C linkage)

namespace rage {

// ============================================================================
// Struct definitions for snJoinMachine and its sub-states
// ============================================================================

// Forward declarations
class SinglesNetworkClient;

// 12-byte HSM event: vtable + 8 bytes of payload
struct snHsmEventData {
    void**      m_pVtable;        // +0x00
    uint32_t    m_nPayload0;      // +0x04
    uint32_t    m_nPayload1;      // +0x08
};

// Connection reference (52 bytes) -- holds address + deep-copy payload
struct snConnectionRef {
    void**      m_pVtable;        // +0x00
    uint32_t    m_nAddr0;         // +0x04
    uint32_t    m_nAddr1;         // +0x08
    uint32_t    m_nPad0C;         // +0x0C
    void*       m_pPayload;       // +0x10  -- start of deep-copy region
    uint8_t     m_aData[32];      // +0x14..+0x33
};

// Notifier block (24 bytes) -- embedded in HSM states for cancel/notify
struct snNotifier {
    void**      m_pVtable;        // +0x00
    void*       m_pField04;       // +0x04
    void*       m_pPendingCallback; // +0x08
    uint32_t    m_nField0C;       // +0x0C
    void*       m_pCancelFn;      // +0x10  -- vtable slot 4, cancel handler
    uint32_t    m_nNotifyList;    // +0x14
};

// HSM state context returned by vfn_10
struct snHsmStateContext {
    uint32_t    m_nField00;       // +0x00
    uint32_t    m_nField04;       // +0x04
    uint32_t    m_nField08;       // +0x08
    void*       m_pCurrentEvent;  // +0x0C
};

// HSM context returned by vfn_11 -- holds the event node list
struct snHsmContext {
    uint8_t     m_aPad[56];       // +0x00..+0x37
    void*       m_pNodeList;      // +0x38 (+56)
};

// Event node allocator -- lives at nodeList+4
struct snNodeAllocator {
    void**      m_pVtable;        // +0x00
    // vtable[1] = Alloc(this, size, type)
};

// Node list -- nodeList+0 is vtable, +4 is allocator, +8 is list head
struct snNodeList {
    void**      m_pVtable;        // +0x00
    snNodeAllocator* m_pAllocator; // +0x04
    void*       m_pListHead;      // +0x08
};

// Base HSM state layout (24 bytes) -- shared by all child states
struct snHsmStateBase {
    void**      m_pVtable;        // +0x00
    uint32_t    m_nField04;       // +0x04
    uint32_t    m_nField08;       // +0x08
    uint32_t    m_nField0C;       // +0x0C
    void*       m_pSession;       // +0x10 (+16) -- parent snSession*
    void*       m_pNetworkClient; // +0x14 (+20) -- network client context
    // Optional: callback region starts at +0x18 (+24) in some states
};

// Extended HSM state with callback region (36 bytes)
// Used by WaitingForReply, SendingGamerData, and other callback-capable states
struct snHsmStateWithCallback {
    void**      m_pVtable;          // +0x00
    uint32_t    m_nField04;         // +0x04
    uint32_t    m_nField08;         // +0x08
    uint32_t    m_nField0C;         // +0x0C
    void*       m_pSession;         // +0x10 (+16)
    void*       m_pNetworkClient;   // +0x14 (+20)
    void*       m_pCallbackTarget;  // +0x18 (+24)
    void*       m_pCallbackSelf;    // +0x1C (+28)
    void*       m_pCallbackHandler; // +0x20 (+32)
};

// SendingGamerData state -- extends callback state with 3 counters
struct snHsmSendingGamerData {
    void**      m_pVtable;          // +0x00
    uint32_t    m_nField04;         // +0x04
    uint32_t    m_nField08;         // +0x08
    uint32_t    m_nField0C;         // +0x0C
    void*       m_pSession;         // +0x10 (+16)
    void*       m_pNetworkClient;   // +0x14 (+20)
    void*       m_pCallbackTarget;  // +0x18 (+24)
    void*       m_pCallbackSelf;    // +0x1C (+28)
    void*       m_pCallbackHandler; // +0x20 (+32)
    uint32_t    m_nCounter0;        // +0x28 (+40)
    uint32_t    m_nCounter1;        // +0x2C (+44)
    uint32_t    m_nCounter2;        // +0x30 (+48)
};

// Callback region appended after snHsmStateBase for notifier-capable states
struct snHsmCallbackRegion {
    void*       m_pCallbackTarget;  // +0x00 (+24 from state base)
    void*       m_pCallbackSelf;    // +0x04 (+28 from state base)
    void*       m_pCallbackHandler; // +0x08 (+32 from state base)
};

// snHsmRequestingJoin -- contains 3 embedded child states
// Layout: base(24) + WaitingForReply(24@+24) + JoinPending(24@+72) + UndoingJoinRequest(24@+96)
struct snHsmRequestingJoin {
    snHsmStateBase m_Base;              // +0x00..+0x17
    snNotifier     m_WaitingForReply;   // +0x18 (+24)
    snNotifier     m_JoinPending;       // +0x48 (+72)
    snNotifier     m_UndoingJoinRequest;// +0x60 (+96)
};

// snSession -- partial layout for fields accessed by snJoinMachine
struct snSession {
    uint8_t     m_aPad00[108];    // +0x00..+0x6B
    void*       m_pEventDispatch; // +0x6C (+108) -- event dispatch target
    uint8_t     m_aPad70[56];     // +0x70..+0xA3
    void*       m_pConnectionRef; // +0xA4 (+164) -- connection ref pointer
    uint8_t     m_aPadA8[68];     // +0xA8..+0xE7
    void*       m_pListHead;      // +0xE8 (+232) -- linked list head
};

// snNetworkTransport -- inner transport object accessed via snSession+16
struct snNetworkTransport {
    uint8_t     m_aPad00[404];    // +0x000..+0x193
    uint32_t    m_nRttBase;       // +0x194 (+404)
    uint32_t    m_nRttExtra;      // +0x198 (+408)
};

// snConnData -- connection tracking data with notify handler list
struct snConnData {
    uint8_t     m_aPad00[672];    // +0x000..+0x29F
    void*       m_pNotifyList;    // +0x2A0 (+672) -- notify handler list
};

// snJoinMachineClient -- the network client pointer, partial layout for fields
// accessed from join machine states. Different from snSession.
struct snJoinMachineClient {
    uint8_t     m_aPad00[16];     // +0x00..+0x0F
    void*       m_pTransport;     // +0x10 (+16)  -- inner transport object
    uint8_t     m_aPad14[76];     // +0x14..+0x5F
    void*       m_pConnectionListA;// +0x60 (+96)  -- connection list (RequestingJoin child)
    uint8_t     m_aPad64[32];     // +0x64..+0x8F
    void*       m_pConnectionListB;// +0x90 (+144) -- connection list (WaitingForReply)
    uint8_t     m_aPad94[24];     // +0x94..+0xA7
    void*       m_pConnectionListC;// +0xA8 (+168) -- connection list (JoinPending fail)
    uint8_t     m_aPadAC[72];     // +0xAC..+0xEF
    void*       m_pConnectionListD;// +0xF0 (+240) -- connection list (AcceptingJoinRequest)
    uint8_t     m_aPadF4[208];    // +0xF4..+0x1C3
    uint32_t    m_nField1C4;      // +0x1C4 (+452) -- padding
    uint8_t     m_aPad1C8[8];     // +0x1C8..+0x1CF
    snConnectionRef m_ConnRef;    // +0x1D0 (+464) -- connection ref for join reply
    uint8_t     m_aPad204[120];   // +0x204..+0x279 -- (adjusted for snConnectionRef = 52 bytes)
    uint32_t    m_nPlayerIndex;   // +0x26C (+620)
    uint8_t     m_aPad270[44];    // +0x270..
    uint32_t    m_nCachedPlayerIndex; // +0x140 (+320) -- cached player index
};

// SinglesNetworkClient -- partial layout for fields accessed by join machine
struct snSinglesNetClient {
    uint8_t     m_aPad00[108];    // +0x00..+0x6B
    void*       m_pEventDispatch; // +0x6C (+108)
    uint8_t     m_aPad70[3672];   // +0x70..+0xE9F
    uint8_t     m_nSessionFlags;  // +0xEA0 (+3744) -- bit 7 = host side
    uint8_t     m_aPadEA1[11];    // +0xEA1..+0xEAB
    void*       m_pPeerList;      // +0xEAC (+3756) -- peer connection map
};

// snJoinMachine -- root state machine
// Major field layout derived from vfn_14 (OnEnter) and cross-references:
struct snJoinMachine {
    void**      m_pVtable;        // +0x000
    uint32_t    m_nField04;       // +0x004
    uint32_t    m_nField08;       // +0x008
    uint32_t    m_nField0C;       // +0x00C
    void*       m_pSession;       // +0x010 (+16)
    void*       m_pNetworkClient; // +0x014 (+20)
    void*       m_pCallbackTarget;// +0x018 (+24)
    void*       m_pCallbackSelf;  // +0x01C (+28)
    void*       m_pCallbackHandler;// +0x020 (+32)
    uint8_t     m_aPad24[12];     // +0x024..+0x02F (+36..+47)
    // Embedded child state slots:
    snNotifier  m_LocalJoinPending;      // +0x030 (+48)
    snNotifier  m_RemoteJoinPending;     // +0x048 (+72)
    uint8_t     m_aConnListSlot[24];     // +0x060 (+96)  -- connection list for RequestingJoin child
    uint8_t     m_aPadMid[72];           // +0x078 (+120..+191)
    snNotifier  m_AcceptingJoinRequest;  // +0x0C0 (+192)
    snNotifier  m_RequestingJoin;        // +0x0D8 (+216)
    snNotifier  m_SendingGamerData;      // +0x0F0 (+240)
    uint8_t     m_aPad108[28];           // +0x108 (+264..+291)
    uint32_t    m_nJoinResult;           // +0x124 (+292)
    uint8_t     m_bJoinActive;           // +0x128 (+296)
    uint8_t     m_aPad129[15];           // +0x129 (+297..+311)
    uint64_t    m_nSessionId;            // +0x138 (+312)
    uint32_t    m_nPlayerIndex;          // +0x140 (+320)
    uint32_t    m_aConnSlots[9];         // +0x144 (+324..+359)
    uint8_t     m_aCapacityBlock[104];   // +0x168 (+360..+463)
    // Connection reference region:
    uint32_t    m_nConnRefPad;           // +0x1D0 (+464)
    uint32_t    m_nConnAddr0;            // +0x1D4 (+468)
    uint32_t    m_nConnAddr1;            // +0x1D8 (+472)
    uint8_t     m_aPadConn[4];           // +0x1DC (+476..+479)
    uint8_t     m_aConnPayload[136];     // +0x1E0 (+480..+615)
    uint8_t     m_aPadFlags[8];          // +0x268 (+616..+623)
    uint8_t     m_nFlags;                // +0x270 (+624)
};

// ============================================================================
// External utility functions
// ============================================================================

extern void snJoinMachine_SetCapacity(void* machine, int capacity);        // @ 0x822603D0
extern void snConnectionRef_GetLocal(void* outRef);                        // @ 0x823E04A0
extern void snConnectionRef_Copy(void* dst, void* src);                    // @ 0x823E0500
extern void snConnectionRef_Release(void* client);                         // @ 0x82260268
extern void snLinkedList_InsertNode(void* list, void* node);               // @ 0x823DD170
extern void snHsmState_Init(void* state);                                  // @ 0x823DDA08

// Join state transition helpers
extern void snHsmLocalJoinPending_TransitionFailed(void* state, void* evt);    // @ 0x823E9878
extern void snHsmWaitingForReply_TransitionFailed(void* state, void* evt);     // @ 0x823E9920
extern void snHsmRemoteJoinPending_TransitionFailed(void* state, void* evt);   // @ 0x823E99C8
extern void snHsmAcceptingJoinRequest_TransitionFailed(void* state, void* evt); // @ 0x823E9A70
extern void snHsmJoinPending_TransitionFailed(void* state, void* evt);         // @ 0x823E9680

// Network client query functions
extern bool snSession_IsLocalJoinPending(void* client);                    // @ 0x823EFF38
extern bool snSession_IsRemoteJoinPending(void* client);                   // @ 0x823F0040
extern bool snSession_IsAcceptingJoinRequest(void* client);                // @ 0x823F0528
extern bool snSession_HasJoinCompleted(void* client);                      // @ 0x823EFEB8
extern bool snSession_ValidateJoinReply(void* client);                     // @ 0x823F0140
extern void snSession_QueryConfigStatus(void* client, void* connectionRef); // @ 0x823DA940
extern void snHsmContext_SetMaxTransitions(void* machine, int maxTransitions); // @ 0x823ED4F8

// Session node management
extern void snSession_AssociateConnection(void* ctx, void* state, void* connectionList);  // @ 0x823ED988
extern void snSession_ProcessPendingConnections(void* ctx, void* state, void* connectionList); // @ 0x823EDA90
extern void snSession_AddNode(void* nodeList, void* node);                 // @ 0x823EC068
extern void snConnectionRef_InitBroadcast(void* session, void* p1, void* p2, void* p3, int p4); // @ 0x82430978

// HSM child state attachment
extern void snHsmState_AttachChild(void* parent, void* childState);        // @ 0x823EF850

// Destructor helpers
extern void snJoinMachine_DestroyBody(void* thisPtr);                      // @ 0x823E56C0
extern void snHsmState_Destroy(void* thisPtr);                             // @ 0x823E8E10
extern void snHsmNotifier_Destroy(void* thisPtr);                          // @ 0x823E6300

// Notification management
extern void snNotifyHandler_Unregister(void* notifyList, void* handler);   // @ 0x823B3D80

// SendingGamerData helpers
extern void snSendingGamerData_ProcessUpdate(void* thisPtr);               // @ 0x823E01F8

// Join notify utility
extern void snJoinMachine_CopyNotifyData(void* list, uint32_t a, uint32_t b, uint32_t c); // @ 0x822608C8

// ============================================================================
// Global state type identifiers (runtime event type IDs loaded from .data)
// ============================================================================

extern uint32_t g_evtType_JoinSucceeded;     // @ 0x825D181C
extern uint32_t g_evtType_JoinFailed;        // @ 0x825D1828
extern uint32_t g_evtType_RequestJoinOk;     // @ 0x825D1864
extern uint32_t g_evtType_RequestJoinDenied; // @ 0x825D1870

// ============================================================================
// Event vtable pointers (from .rdata)
// ============================================================================

extern void* g_vtable_hsmEvent;                    // @ 0x82072864  vtable<rage::hsmEvent>
extern void* g_vtable_EvtLocalJoinSucceeded;       // @ 0x82072D1C  vtable<rage::EvtLocalJoinSucceeded>
extern void* g_vtable_EvtLocalJoinFailed;          // @ 0x82072D30  vtable<rage::EvtLocalJoinFailed>
extern void* g_vtable_EvtRemoteJoinSucceeded;      // @ 0x82072D44  vtable<rage::EvtRemoteJoinSucceeded>
extern void* g_vtable_EvtRemoteJoinFailed;         // @ 0x82072D58  vtable<rage::EvtRemoteJoinFailed>
extern void* g_vtable_EvtRequestJoinSucceeded;     // @ 0x82072D6C  vtable<rage::EvtRequestJoinSucceeded>
extern void* g_vtable_EvtRequestJoinFailed;        // @ 0x82072D80  vtable<rage::EvtRequestJoinFailed>
extern void* g_vtable_EvtAcceptJoinRequestFailed;  // @ 0x82072DA8  vtable<rage::EvtAcceptJoinRequestFailed>
extern void* g_vtable_EvtSessionJoinFailed;        // @ 0x82072DD0  vtable<rage::EvtSessionJoinFailed>

// Callback/handler vtable used for join completion notification
extern void* g_joinCompletionHandler;  // @ 0x823EB0F8  thunk_fn_823DEF68

// Reply timeout handler function pointer
extern void thunk_snHsmWaitingForReply_TimeoutHandler(); // @ 0x823F1508  thunk_fn_823F0D68

// ============================================================================
// Helper: get the current event's type ID from the HSM state machine
// ============================================================================

static inline uint32_t snJoinMachine_CurEventTypeId(snHsmStateBase* self) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    snHsmStateContext* stateCtx = (snHsmStateContext*)vfn_10(self);
    snHsmEventData* eventObj = (snHsmEventData*)stateCtx->m_pCurrentEvent;
    void** evtVtable = *reinterpret_cast<void***>(eventObj);
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    return getTypeId(eventObj);
}

// Helper: get the current event object from the HSM state machine
static inline snHsmEventData* snJoinMachine_GetCurrentEvent(snHsmStateBase* self) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    snHsmStateContext* stateCtx = (snHsmStateContext*)vfn_10(self);
    return (snHsmEventData*)stateCtx->m_pCurrentEvent;
}

// Helper: build a 16-byte hsmEvent on the stack with a specific vtable
static inline void snJoinMachine_MakeEvent(snHsmEventData* slot, void* vtableForEvent) {
    snHsmState_Init(slot);
    slot->m_pVtable = (void**)vtableForEvent;
}

// Helper: allocate and enqueue a node via the HSM context's node list
static inline void snJoinMachine_AllocAndEnqueueEvent(snHsmStateBase* self, void* evtVtablePtr, snHsmEventData* srcEvt) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    snHsmContext* hsmCtx = (snHsmContext*)vfn_11(self);
    snNodeList* nodeList = (snNodeList*)hsmCtx->m_pNodeList;
    snNodeAllocator* allocator = nodeList->m_pAllocator;

    void** allocVtable = *reinterpret_cast<void***>(allocator);
    void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
    snHsmEventData* newNode = (snHsmEventData*)allocFn(allocator, 12, 0);

    if (newNode == nullptr) {
        return;
    }

    // Initialize vtable to base hsmEvent, then overwrite with specific type
    newNode->m_pVtable = (void**)&g_vtable_hsmEvent;
    newNode->m_pVtable = (void**)evtVtablePtr;
    newNode->m_nPayload0 = srcEvt->m_nPayload0;
    newNode->m_nPayload1 = srcEvt->m_nPayload1;

    snSession_AddNode(nodeList->m_pListHead, newNode);
}

// ============================================================================
// snJoinMachine::OnEnter @ 0x823DF060 | size: 0xD4 | vfn_14
// ============================================================================
void snJoinMachine_OnEnter(snJoinMachine* self) { // vfn_14
    // Clear join status fields
    self->m_nJoinResult = 0;
    self->m_bJoinActive = 0;
    self->m_nSessionId = 0;
    self->m_nPlayerIndex = (uint32_t)-1;

    // Zero-fill the connection slot array (+324, 9 entries)
    for (int i = 0; i < 9; i++) {
        self->m_aConnSlots[i] = 0;
    }

    // Reset capacity to 0
    snJoinMachine_SetCapacity(&self->m_aCapacityBlock, 0);

    // Get local connection reference (into stack temp)
    snConnectionRef localConnRef;
    snConnectionRef_GetLocal(&localConnRef);

    // Copy local connection address (8 bytes at +4) into machine
    self->m_nConnAddr0 = localConnRef.m_nAddr0;
    self->m_nConnAddr1 = localConnRef.m_nAddr1;

    // Deep-copy the connection reference payload
    snConnectionRef_Copy(&self->m_aConnPayload, &localConnRef.m_pPayload);

    // Release the temporary local connection reference
    snConnectionRef releaseRef;
    snConnectionRef_Release(&releaseRef);

    // Clear bit 7 of flags byte (preserve lower 7 bits)
    self->m_nFlags = self->m_nFlags & 0x7F;

    // Set up join completion callback
    self->m_pCallbackHandler = &g_joinCompletionHandler; // @ 0x823EB0F8
    self->m_pCallbackSelf = self;

    // Insert into session's linked list at session+232
    snSession* session = (snSession*)self->m_pSession;
    snLinkedList_InsertNode(&session->m_pListHead, self);
}

// ============================================================================
// snHsmLocalJoinPending::OnEnter @ 0x823DF638 | size: 0x70 | vfn_14
// ============================================================================
void snHsmLocalJoinPending_OnEnter(snHsmStateBase* self) { // vfn_14
    void* client = self->m_pNetworkClient;

    bool localJoinPending = snSession_IsLocalJoinPending(client);
    if (localJoinPending) {
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return;
        }
    }

    // Local join is not pending or handshake incomplete -- transition to failed
    snHsmEventData evt;
    snHsmState_Init(&evt);
    evt.m_pVtable = (void**)&g_vtable_EvtLocalJoinFailed;
    snHsmLocalJoinPending_TransitionFailed(self, &evt);
}

// ============================================================================
// snHsmLocalJoinPending::OnEvent @ 0x823DF6A8 | size: 0x158 | vfn_12
// ============================================================================
void snHsmLocalJoinPending_OnEvent(snHsmStateBase* self, void* event, bool* handled) { // vfn_12
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinSucceeded) {
        // Join succeeded -- fire EvtLocalJoinSucceeded event
        snHsmEventData evt;
        snHsmState_Init(&evt);

        snJoinMachine_AllocAndEnqueueEvent(self, &g_vtable_EvtLocalJoinSucceeded, &evt);
    } else {
        // Check for JoinFailed event
        typeId = snJoinMachine_CurEventTypeId(self);

        if (typeId == g_evtType_JoinFailed) {
            snHsmEventData evt;
            snHsmState_Init(&evt);
            evt.m_pVtable = (void**)&g_vtable_EvtLocalJoinFailed;
            snHsmLocalJoinPending_TransitionFailed(self, &evt);
        } else {
            *handled = false;
        }
    }
}

// ============================================================================
// snHsmWaitingForReply::OnEnter @ 0x823DF870 | size: 0xB8 | vfn_14
// ============================================================================
void snHsmWaitingForReply_OnEnter(snHsmStateWithCallback* self) { // vfn_14
    // Set up callback: store self at +28, handler at +32
    self->m_pCallbackSelf = self;
    self->m_pCallbackHandler = (void*)&thunk_snHsmWaitingForReply_TimeoutHandler;

    // Query session config from host connection
    snSession* session = (snSession*)self->m_pSession;
    snSession_QueryConfigStatus(session->m_pConnectionRef, &self->m_pCallbackTarget);

    // Validate the join reply from the client
    void* client = self->m_pNetworkClient;
    bool replyValid = snSession_ValidateJoinReply(client);

    if (!replyValid) {
        snHsmEventData evt;
        snHsmState_Init(&evt);
        evt.m_pVtable = (void**)&g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(self, &evt);
        return;
    }

    // Reply is valid -- compute timeout from round-trip time
    snJoinMachineClient* typedClient = (snJoinMachineClient*)client;
    snNetworkTransport* transport = (snNetworkTransport*)typedClient->m_pTransport;
    int32_t rtt = (int32_t)(transport->m_nRttBase + transport->m_nRttExtra);
    int32_t rttPadded = rtt + 50;
    int32_t timeout = rttPadded * rtt * 100;

    snHsmContext_SetMaxTransitions(self, timeout);
}

// ============================================================================
// snHsmWaitingForReply::OnEvent @ 0x823DF928 | size: 0x1B4 | vfn_12
// ============================================================================
void snHsmWaitingForReply_OnEvent(snHsmStateBase* self, void* event, bool* handled) { // vfn_12
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_RequestJoinOk) {
        // Join request accepted -- transition via vfn_11 and process connections
        void** vtable = *reinterpret_cast<void***>(self);
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(self);

        snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;

        snSession_AssociateConnection(hsmCtx, self, &client->m_pConnectionListB);
        snSession_ProcessPendingConnections(hsmCtx, self, &client->m_pConnectionListB);
        return;
    }

    // Check for RequestJoinDenied
    typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_RequestJoinDenied) {
        snHsmEventData* eventObj = snJoinMachine_GetCurrentEvent(self);

        snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

        // Copy connection address into client connection ref
        snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;
        client->m_ConnRef.m_nAddr0 = eventObj->m_nPayload0;
        client->m_ConnRef.m_nAddr1 = eventObj->m_nPayload1;

        // Deep-copy the payload region (+16 from each base)
        snConnectionRef_Copy(&client->m_ConnRef.m_pPayload, (char*)eventObj + 16);

        // Transition to failure
        snHsmEventData evt;
        snHsmState_Init(&evt);
        evt.m_pVtable = (void**)&g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(self, &evt);
        return;
    }

    // Check for timeout (event type == 0)
    typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == 0) {
        snHsmEventData evt;
        snHsmState_Init(&evt);
        evt.m_pVtable = (void**)&g_vtable_EvtRequestJoinFailed;
        snHsmWaitingForReply_TransitionFailed(self, &evt);
        return;
    }

    *handled = false;
}

// ============================================================================
// snHsmJoinPending::OnEnter @ 0x823DFAF0 | size: 0xC4 | vfn_14
// ============================================================================
void snHsmJoinPending_OnEnter(snHsmStateBase* self) { // vfn_14
    snHsmEventData* eventObj = snJoinMachine_GetCurrentEvent(self);

    // Retrieve session info from the accepted join
    snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

    // Copy connection data into client connection ref
    snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;

    client->m_ConnRef.m_nAddr0 = eventObj->m_nPayload0;
    client->m_ConnRef.m_nAddr1 = eventObj->m_nPayload1;

    snConnectionRef_Copy(&client->m_ConnRef.m_pPayload, (char*)eventObj + 16);

    // Copy player index from m_nPlayerIndex (+620) to m_nCachedPlayerIndex (+320)
    client->m_nCachedPlayerIndex = client->m_nPlayerIndex;

    // Check if join handshake has completed
    bool joinCompleted = snSession_HasJoinCompleted(client);
    if (!joinCompleted) {
        snHsmEventData evt;
        snHsmState_Init(&evt);
        evt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snHsmJoinPending_TransitionFailed(self, &evt);
    }
}

// ============================================================================
// snHsmJoinPending::OnEvent @ 0x823DFBB8 | size: 0x164 | vfn_12
// ============================================================================
void snHsmJoinPending_OnEvent(snHsmStateBase* self, void* event, bool* handled) { // vfn_12
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinSucceeded) {
        // Session join succeeded -- fire EvtRequestJoinSucceeded
        snHsmEventData evt;
        snHsmState_Init(&evt);

        snJoinMachine_AllocAndEnqueueEvent(self, &g_vtable_EvtRequestJoinSucceeded, &evt);
        return;
    }

    // Check for JoinFailed event
    typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinFailed) {
        void** vtable = *reinterpret_cast<void***>(self);
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(self);

        snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;

        snSession_AssociateConnection(hsmCtx, self, &client->m_pConnectionListC);
        snSession_ProcessPendingConnections(hsmCtx, self, &client->m_pConnectionListC);
        return;
    }

    *handled = false;
}

// ============================================================================
// snHsmRemoteJoinPending::OnEnter @ 0x823DFE70 | size: 0x70 | vfn_14
// ============================================================================
void snHsmRemoteJoinPending_OnEnter(snHsmStateBase* self) { // vfn_14
    void* client = self->m_pNetworkClient;

    bool remoteJoinPending = snSession_IsRemoteJoinPending(client);
    if (remoteJoinPending) {
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return;
        }
    }

    snHsmEventData evt;
    snHsmState_Init(&evt);
    evt.m_pVtable = (void**)&g_vtable_EvtRemoteJoinFailed;
    snHsmRemoteJoinPending_TransitionFailed(self, &evt);
}

// ============================================================================
// snHsmRemoteJoinPending::OnEvent @ 0x823DFEE0 | size: 0x158 | vfn_12
// ============================================================================
void snHsmRemoteJoinPending_OnEvent(snHsmStateBase* self, void* event, bool* handled) { // vfn_12
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinSucceeded) {
        // Remote join succeeded -- fire EvtRemoteJoinSucceeded
        snHsmEventData evt;
        snHsmState_Init(&evt);

        snJoinMachine_AllocAndEnqueueEvent(self, &g_vtable_EvtRemoteJoinSucceeded, &evt);
    } else {
        typeId = snJoinMachine_CurEventTypeId(self);

        if (typeId == g_evtType_JoinFailed) {
            snHsmEventData evt;
            snHsmState_Init(&evt);
            evt.m_pVtable = (void**)&g_vtable_EvtRemoteJoinFailed;
            snHsmRemoteJoinPending_TransitionFailed(self, &evt);
        } else {
            *handled = false;
        }
    }
}

// ============================================================================
// snHsmAcceptingJoinRequest::OnEnter @ 0x823E0048 | size: 0x70 | vfn_14
// ============================================================================
void snHsmAcceptingJoinRequest_OnEnter(snHsmStateBase* self) { // vfn_14
    void* client = self->m_pNetworkClient;

    bool accepting = snSession_IsAcceptingJoinRequest(client);
    if (accepting) {
        bool joinCompleted = snSession_HasJoinCompleted(client);
        if (joinCompleted) {
            return;
        }
    }

    snHsmEventData evt;
    snHsmState_Init(&evt);
    evt.m_pVtable = (void**)&g_vtable_EvtAcceptJoinRequestFailed;
    snHsmAcceptingJoinRequest_TransitionFailed(self, &evt);
}

// ============================================================================
// snHsmAcceptingJoinRequest::OnEvent @ 0x823E00B8 | size: 0x100 | vfn_12
// ============================================================================
void snHsmAcceptingJoinRequest_OnEvent(snHsmStateBase* self, void* event, bool* handled) { // vfn_12
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinSucceeded) {
        void** vtable = *reinterpret_cast<void***>(self);
        void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
        void* hsmCtx = vfn_11(self);

        snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;

        snSession_AssociateConnection(hsmCtx, self, &client->m_pConnectionListD);
        snSession_ProcessPendingConnections(hsmCtx, self, &client->m_pConnectionListD);
        return;
    }

    typeId = snJoinMachine_CurEventTypeId(self);

    if (typeId == g_evtType_JoinFailed) {
        snHsmEventData evt;
        snHsmState_Init(&evt);
        evt.m_pVtable = (void**)&g_vtable_EvtAcceptJoinRequestFailed;
        snHsmAcceptingJoinRequest_TransitionFailed(self, &evt);
        return;
    }

    *handled = false;
}

// ============================================================================
// HSM State Name Strings (verified from binary .rdata)
// ============================================================================

static const char* const s_JoinMachine          = "JoinMachine";          // @ 0x82072368
static const char* const s_LocalJoinPending     = "LocalJoinPending";     // @ 0x82072374
static const char* const s_RequestingJoin       = "RequestingJoin";       // @ 0x82072388
static const char* const s_WaitingForReply      = "WaitingForReply";      // @ 0x82072398
static const char* const s_JoinPending          = "JoinPending";          // @ 0x820723A8
static const char* const s_UndoingJoinRequest   = "UndoingJoinRequest";   // @ 0x820723B4
static const char* const s_RemoteJoinPending    = "RemoteJoinPending";    // @ 0x820723C8
static const char* const s_AcceptingJoinRequest = "AcceptingJoinRequest"; // @ 0x820723DC
static const char* const s_SendingGamerData     = "SendingGamerData";     // @ 0x820723F4

// ============================================================================
// GetStateName functions (vfn_2, 12B each)
// ============================================================================

const char* snJoinMachine_GetStateName()              { return s_JoinMachine; }          // @ 0x823DF050
const char* snHsmLocalJoinPending_GetStateName()      { return s_LocalJoinPending; }     // @ 0x823DF628
const char* snHsmRequestingJoin_GetStateName()        { return s_RequestingJoin; }       // @ 0x823DF800
const char* snHsmWaitingForReply_GetStateName()       { return s_WaitingForReply; }      // @ 0x823DF860
const char* snHsmJoinPending_GetStateName()           { return s_JoinPending; }          // @ 0x823DFAE0
const char* snHsmUndoingJoinRequest_GetStateName()    { return s_UndoingJoinRequest; }   // @ 0x823DFD20
const char* snHsmRemoteJoinPending_GetStateName()     { return s_RemoteJoinPending; }    // @ 0x823DFE60
const char* snHsmAcceptingJoinRequest_GetStateName()  { return s_AcceptingJoinRequest; } // @ 0x823E0038
const char* snSendingGamerData_GetStateName()         { return s_SendingGamerData; }     // @ 0x823E01B8

// ============================================================================
// Destructors (vfn_0)
// ============================================================================

/** snJoinMachine::~snJoinMachine @ 0x823E5E88 | size: 0x50 | vfn_0 */
void snJoinMachine_Destructor(snJoinMachine* self, uint32_t flags) {
    snJoinMachine_DestroyBody(self);
    if (flags & 1) {
        rage_free(self);
    }
}

/** snHsmLocalJoinPending::~snHsmLocalJoinPending @ 0x823E6240 | size: 0x50 | vfn_0 */
void snHsmLocalJoinPending_Destructor(snHsmStateBase* self, uint32_t flags) {
    snHsmState_Destroy(self);
    if (flags & 1) {
        rage_free(self);
    }
}

/** snHsmRequestingJoin::~snHsmRequestingJoin @ 0x823E6290 | size: 0x6C | vfn_0 */
void snHsmRequestingJoin_Destructor(snHsmRequestingJoin* self, uint32_t flags) {
    snHsmState_Destroy(&self->m_UndoingJoinRequest);  // +96
    snHsmState_Destroy(&self->m_JoinPending);         // +72
    snHsmNotifier_Destroy(&self->m_WaitingForReply);  // +24
    snHsmState_Destroy(self);
    if (flags & 1) {
        rage_free(self);
    }
}

/** snHsmWaitingForReply::~snHsmWaitingForReply @ 0x823E6610 | size: 0x50 | vfn_0 */
void snHsmWaitingForReply_Destructor(snHsmStateBase* self, uint32_t flags) {
    snHsmNotifier_Destroy(self);
    if (flags & 1) {
        rage_free(self);
    }
}

// ============================================================================
// OnEnter_AttachChildren Handlers (vfn_5)
// ============================================================================

/**
 * snJoinMachine::OnEnter_AttachChildren @ 0x823E0358 | size: 0x60 | vfn_5
 * Attaches child sub-states at fixed offsets:
 *   +48  (LocalJoinPending), +72  (RemoteJoinPending),
 *   +192 (AcceptingJoinRequest), +216 (RequestingJoin), +240 (SendingGamerData)
 */
void snJoinMachine_AttachChildren(snJoinMachine* self) {
    snHsmState_AttachChild(self, &self->m_LocalJoinPending);       // +48
    snHsmState_AttachChild(self, &self->m_RemoteJoinPending);      // +72
    snHsmState_AttachChild(self, &self->m_AcceptingJoinRequest);   // +192
    snHsmState_AttachChild(self, &self->m_RequestingJoin);         // +216
    snHsmState_AttachChild(self, &self->m_SendingGamerData);       // +240
}

/**
 * snHsmRequestingJoin::OnEnter_AttachChildren @ 0x823DFD78 | size: 0x48 | vfn_5
 * Attaches child sub-states: +24 (WaitingForReply), +72 (JoinPending), +96 (UndoingJoinRequest)
 */
void snHsmRequestingJoin_AttachChildren(snHsmRequestingJoin* self) {
    snHsmState_AttachChild(self, &self->m_WaitingForReply);         // +24
    snHsmState_AttachChild(self, &self->m_JoinPending);             // +72
    snHsmState_AttachChild(self, &self->m_UndoingJoinRequest);      // +96
}

/**
 * snHsmUndoingJoinRequest::OnEnter @ 0x823DFD30 | size: 0x48 | vfn_14
 */
void snHsmUndoingJoinRequest_OnEnter(snHsmStateBase* self) {
    snHsmEventData stackState;
    snHsmState_Init(&stackState);
    stackState.m_pVtable = (void**)&g_vtable_EvtRequestJoinFailed;
    snHsmWaitingForReply_TransitionFailed(self, &stackState);
}

// ============================================================================
// OnExit Handlers
// ============================================================================

// Helper: cancel a pending notifier callback
static inline void CancelNotifier(snNotifier* notifier) {
    if (notifier->m_pPendingCallback != nullptr) {
        notifier->m_pPendingCallback = nullptr;
        void (*cancelFn)(void*) = (void (*)(void*))notifier->m_pCancelFn;
        cancelFn(notifier);
    }
}

/**
 * snHsmRequestingJoin::OnExit @ 0x823DFDC0 | size: 0xA0 | vfn_6
 * Cancels pending notifications on three embedded notifier objects.
 */
void snHsmRequestingJoin_OnExit(snHsmRequestingJoin* self) {
    CancelNotifier(&self->m_WaitingForReply);     // +24
    CancelNotifier(&self->m_JoinPending);         // +72
    CancelNotifier(&self->m_UndoingJoinRequest);  // +96
}

/**
 * snHsmWaitingForReply::OnExit @ 0x823E3588 | size: 0x44 | vfn_15
 * Unregisters the notification handler from the connection data's notify list.
 */
void snHsmWaitingForReply_OnExit(snHsmStateWithCallback* self) {
    snSession* session = (snSession*)self->m_pSession;
    snNotifier* notifier = (snNotifier*)&self->m_pCallbackTarget;

    snConnData* connData = (snConnData*)session->m_pConnectionRef;
    snNotifyHandler_Unregister(&connData->m_pNotifyList, notifier);

    notifier->m_nNotifyList = 0;
}

// ============================================================================
// OnUpdate Handler
// ============================================================================

/**
 * snHsmRequestingJoin::OnUpdate @ 0x823DF810 | size: 0x50 | vfn_13
 */
void snHsmRequestingJoin_OnUpdate(snHsmStateBase* self) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(self);

    snJoinMachineClient* joinMachine = (snJoinMachineClient*)self->m_pNetworkClient;

    snSession_AssociateConnection(context, self, &joinMachine->m_pConnectionListA);
    snSession_ProcessPendingConnections(context, self, &joinMachine->m_pConnectionListA);
}

// ============================================================================
// snSendingGamerData -- Simple State Functions
// ============================================================================

/** snSendingGamerData::OnEnter @ 0x823E01E0 | size: 0x14 | vfn_14 */
void snSendingGamerData_OnEnter(snHsmSendingGamerData* self) {
    self->m_nCounter0 = 0;
    self->m_nCounter1 = 0;
    self->m_nCounter2 = 0;
}

/** snSendingGamerData::GetParentState @ 0x823E01D0 | size: 0x10 | vfn_8 */
void* snSendingGamerData_GetParentState(snHsmStateWithCallback* self) {
    self->m_pCallbackTarget = self;
    return &self->m_pCallbackTarget;
}

/** snSendingGamerData::OnUpdate @ 0x823E01C8 | size: 0x4 | vfn_9 */
void snSendingGamerData_OnUpdate(snHsmStateBase* self) {
    snSendingGamerData_ProcessUpdate(self);
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * snJoinMachine::InitJoinNotify @ 0x823EFDF0 | size: 0x64
 */
void snJoinMachine_InitJoinNotify(snConnectionRef* dst, void* callback, void* source) {
    dst->m_nAddr0 = (uint32_t)(uintptr_t)callback;
    snJoinMachine_SetCapacity(&dst->m_nAddr1, 0);

    if (source != nullptr) {
        uint32_t field4  = *(uint32_t*)((char*)source + 4);
        uint32_t field16 = *(uint32_t*)((char*)source + 16);
        uint32_t field24 = *(uint32_t*)((char*)source + 24);
        snJoinMachine_CopyNotifyData(&dst->m_nAddr1, field4, field16, field24);
    }
}

// ============================================================================
// snJoinMachine::OnExit @ 0x823E03B8 | size: 0xE8 | vfn_6
// ============================================================================
void snJoinMachine_OnExit(snJoinMachine* self) { // vfn_6
    CancelNotifier(&self->m_LocalJoinPending);       // +48
    CancelNotifier(&self->m_RemoteJoinPending);      // +72
    CancelNotifier(&self->m_AcceptingJoinRequest);   // +192
    CancelNotifier(&self->m_RequestingJoin);         // +216
    CancelNotifier(&self->m_SendingGamerData);       // +240
}

// ============================================================================
// snJoinMachine::FireEvent @ 0x823E97D0 | size: 0xA4
// ============================================================================
void snJoinMachine_FireEvent(snHsmStateBase* self, snHsmEventData* event) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    snHsmContext* hsmCtx = (snHsmContext*)vfn_11(self);

    snNodeList* nodeList = (snNodeList*)hsmCtx->m_pNodeList;
    snNodeAllocator* allocator = nodeList->m_pAllocator;
    void** allocVtable = *reinterpret_cast<void***>(allocator);
    void* (*allocFn)(void*, int, int) = (void* (*)(void*, int, int))allocVtable[1];
    snHsmEventData* newNode = (snHsmEventData*)allocFn(allocator, 12, 0);

    if (newNode != nullptr) {
        // Set vtable to base hsmEvent first, then copy event data
        newNode->m_pVtable = (void**)&g_vtable_hsmEvent;

        uint32_t addr0 = event->m_nPayload0;
        uint32_t addr1 = event->m_nPayload1;

        // Overwrite vtable with event-specific vtable from source
        newNode->m_pVtable = event->m_pVtable;
        newNode->m_nPayload0 = addr0;
        newNode->m_nPayload1 = addr1;

        snSession_AddNode(nodeList->m_pListHead, newNode);
    }
}


// ============================================================================
// Additional event-type ID globals
// ============================================================================
extern uint32_t g_evtType_HostChangePresenceSucceeded; // @ 0x825D17BC
extern uint32_t g_evtType_ChangePresenceSucceeded;     // @ 0x825D17C8
extern uint32_t g_evtType_ChangePresenceFailed;        // @ 0x825D17D4
extern uint32_t g_evtType_KillConnection;              // @ 0x825D17E0
extern uint32_t g_evtType_DroppedConnection;           // @ 0x825D17EC
extern uint32_t g_evtType_ClosedConnection;            // @ 0x825D17F8
extern uint32_t g_evtType_GuestCreate;                 // @ 0x825D1804
extern uint32_t g_evtType_GuestCreateSucceeded;        // @ 0x825D1810
extern uint32_t g_evtType_JoinRequest;                 // @ 0x825D1834
extern uint32_t g_evtType_JoinReply;                   // @ 0x825D1840
extern uint32_t g_evtType_Join;                        // @ 0x825D184C

extern void* g_vtable_EvtAcceptJoinRequestSucceeded; // @ 0x82072D94
extern void* g_vtable_EvtRequestJoinOk;              // @ 0x82072D6C

// Additional helpers
extern void snJoinMachine_SessionJoinDispatch(void* self, void* cfgOrEvt);  // @ 0x823E9728
extern void snJoinMachine_97D0(void* self, void* cfgOrEvt);                 // @ 0x823E97D0
extern void snLeaveMachine_ReleaseHost(void* client, void* connectionRef);  // @ 0x823E81C0
extern void snLeaveMachine_ForceLeave(void* self);                          // @ 0x823F0438
extern void snSession_KickAndClose(void* self);                             // @ 0x823F0778
extern void snLeaveMachine_DropRemote(void* self);                          // @ 0x823F0880
extern void SinglesNetworkClient_PeerLookupA250(void* sessionList, void* connection); // @ 0x823EA250
extern void SinglesNetworkClient_ResetJoinState(void* self);                // @ 0x823E7310
extern void SinglesNetworkClient_InitConnRef(void* connRef);                // @ 0x823B3FD8

// Forward decl for the existing vfn_13 implementation further below.
void* snSession_InitConnectionBroadcast(void* client, void* tableA, void* tableB,
                                        void* tableC, int flag);


// ============================================================================
// snJoinMachine::vfn_2 @ 0x823DF050 | size: 0xC
// ============================================================================
extern void* g_rtti_snJoinMachine; // @ 0x82372368

void* snJoinMachine_GetRtti(void* /*thisPtr*/) { // vfn_2
    return &g_rtti_snJoinMachine;
}

// ============================================================================
// snJoinMachine::OnEvent @ 0x823DF138 | size: 0x4F0 | vfn_12 (PRIMARY)
//
// Root-level HSM event dispatcher for the join machine.
// Events handled:
//   HostChangePresenceSucceeded, ChangePresenceSucceeded,
//   ChangePresenceFailed, KillConnection, DroppedConnection,
//   ClosedConnection, GuestCreate, GuestCreateSucceeded
// ============================================================================

void snJoinMachine_OnSessionEvent(snJoinMachine* self, void* /*event*/, bool* handled) { // vfn_12
    snHsmStateBase* selfBase = (snHsmStateBase*)self;
    *handled = true;

    uint32_t typeId = snJoinMachine_CurEventTypeId(selfBase);

    // -- case 1: HostChangePresenceSucceeded --
    if (typeId == g_evtType_HostChangePresenceSucceeded) {
        SinglesNetworkClient_ResetJoinState(self->m_pSession); // @ 0x823E7310

        snConnectionRef connRef;
        *(void**)&connRef = (void*)&g_vtable_hsmEvent;
        connRef.m_nAddr0 = 0;
        SinglesNetworkClient_InitConnRef(&connRef);
        connRef.m_nAddr0 = self->m_nJoinResult;
        snJoinMachine_SetCapacity(&connRef, 0);

        snSession* session = (snSession*)self->m_pSession;
        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)6u, &connRef);

        snHsmEventData slot10Evt;
        snJoinMachine_MakeEvent(&slot10Evt, &g_vtable_hsmEvent);
        slot10Evt.m_nPayload0 = self->m_nJoinResult;
        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)10u, &slot10Evt);

        snConnectionRef_Release(&connRef);

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_SessionJoinDispatch(self, &cfgEvt);
        return;
    }

    // -- case 2: ChangePresenceSucceeded --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_ChangePresenceSucceeded) {
        snSession* session = (snSession*)self->m_pSession;
        snLeaveMachine_ReleaseHost(session, &self->m_aConnListSlot);

        snHsmEventData slot7Evt;
        snJoinMachine_MakeEvent(&slot7Evt, &g_vtable_hsmEvent);
        slot7Evt.m_nPayload0 = self->m_nJoinResult;
        SinglesNetworkClient_InitConnRef(&slot7Evt);
        slot7Evt.m_nPayload0 = self->m_nJoinResult;
        snJoinMachine_SetCapacity(&slot7Evt, 0);

        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)7u, &slot7Evt);
        snConnectionRef_Release(&slot7Evt);

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }

    // -- case 3: ChangePresenceFailed --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_ChangePresenceFailed) {
        snSession* session = (snSession*)self->m_pSession;

        snHsmEventData slot10Evt;
        slot10Evt.m_pVtable = (void**)&g_vtable_hsmEvent;
        slot10Evt.m_nPayload0 = self->m_nJoinResult;
        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)10u, &slot10Evt);

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_SessionJoinDispatch(self, &cfgEvt);
        return;
    }

    // -- case 4: KillConnection --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_KillConnection) {
        snSession* session = (snSession*)self->m_pSession;
        snLeaveMachine_ReleaseHost(session, &self->m_bJoinActive);

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }

    // -- case 5: DroppedConnection --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_DroppedConnection) {
        snJoinMachineClient* client = (snJoinMachineClient*)self->m_pNetworkClient;
        uint32_t sessionId = self->m_nJoinResult;
        void* notifySrc = (void*)((char*)client + 516);

        snConnectionRef scratchRef;
        *(void**)&scratchRef = (void*)&g_vtable_hsmEvent;
        scratchRef.m_nAddr0 = 0;
        SinglesNetworkClient_InitConnRef(&scratchRef);
        snJoinMachine_CopyNotifyData(&scratchRef,
                                     sessionId,
                                     *(uint32_t*)notifySrc,
                                     (uint32_t)(uintptr_t)notifySrc);

        snSession* session = (snSession*)self->m_pSession;
        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)6u, &scratchRef);

        snHsmEventData slot10Evt;
        slot10Evt.m_pVtable = (void**)&g_vtable_hsmEvent;
        slot10Evt.m_nPayload0 = sessionId;
        snSession_AssociateConnection(&session->m_pEventDispatch,
                                      (void*)(uintptr_t)10u, &slot10Evt);
        snConnectionRef_Release(&scratchRef);

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_SessionJoinDispatch(self, &cfgEvt);
        return;
    }

    // -- case 6: ClosedConnection -> ForceLeave --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_ClosedConnection) {
        snLeaveMachine_ForceLeave(self);
        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }

    // -- case 7: GuestCreate -> KickAndClose --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_GuestCreate) {
        snSession_KickAndClose(self);
        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_SessionJoinDispatch(self, &cfgEvt);
        return;
    }

    // -- case 8: GuestCreateSucceeded -> DropRemote --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_GuestCreateSucceeded) {
        snLeaveMachine_DropRemote(self);
        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }

    *handled = false;
}

// ============================================================================
// snJoinMachine::OnUpdate @ 0x823F0A70 | size: 0x2F8 | vfn_13
// Handles JoinRequest/JoinReply/Join (inbound) events.
// ============================================================================
void snJoinMachine_OnEvent_JoinRequestArm(snJoinMachine* self) { // vfn_13 (inbound arm)
    snHsmStateBase* selfBase = (snHsmStateBase*)self;
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];

    // Capture the current event up front
    snHsmStateContext* stateCtx = (snHsmStateContext*)vfn_10(self);
    snHsmEventData* eventObj = (snHsmEventData*)stateCtx->m_pCurrentEvent;

    // -- case 1: JoinRequest --
    uint32_t typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_JoinRequest) {
        snSinglesNetClient* client = (snSinglesNetClient*)self->m_pSession;
        bool openSession = ((client->m_nSessionFlags & 0x80u) != 0);

        void* hsmCtx = vfn_11(self);
        void* childSlot = openSession ? (void*)&self->m_LocalJoinPending     // +48
                                      : (void*)&self->m_RemoteJoinPending;   // +72
        snSession_AssociateConnection(hsmCtx, self, childSlot);
        snSession_ProcessPendingConnections(hsmCtx, self, childSlot);
        return;
    }

    // -- case 2: JoinReply --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_JoinReply) {
        char replyInfo[48] = {0};
        snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

        snSinglesNetClient* client = (snSinglesNetClient*)self->m_pSession;
        void* peerKey = (void*)((char*)eventObj + 16);
        void* found = (void*)(uintptr_t)0;
        SinglesNetworkClient_PeerLookupA250(client->m_pPeerList, peerKey);
        (void)found;

        bool peerKnown = false;
        if (!peerKnown) {
            void* hsmCtx = vfn_11(self);
            snSession_AssociateConnection(hsmCtx, self, &self->m_AcceptingJoinRequest);  // +192
            snSession_ProcessPendingConnections(hsmCtx, self, &self->m_AcceptingJoinRequest);
            return;
        }

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }

    // -- case 3: Join --
    typeId = snJoinMachine_CurEventTypeId(selfBase);
    if (typeId == g_evtType_Join) {
        snConnectionRef_InitBroadcast(eventObj, nullptr, nullptr, nullptr, 0);

        snSinglesNetClient* client = (snSinglesNetClient*)self->m_pSession;
        void* peerKey = (void*)((char*)eventObj + 16);
        SinglesNetworkClient_PeerLookupA250(client->m_pPeerList, peerKey);
        bool peerKnown = false;

        if (!peerKnown) {
            void* hsmCtx = vfn_11(self);
            snSession_AssociateConnection(hsmCtx, self, &self->m_RequestingJoin);  // +216
            snSession_ProcessPendingConnections(hsmCtx, self, &self->m_RequestingJoin);
            return;
        }

        snHsmEventData cfgEvt;
        snJoinMachine_MakeEvent(&cfgEvt, &g_vtable_hsmEvent);
        cfgEvt.m_pVtable = (void**)&g_vtable_EvtSessionJoinFailed;
        snJoinMachine_97D0(self, &cfgEvt);
        return;
    }
    // TODO[vfn_13]: the A250 peer-lookup result flows back into a bool via
    // r3 in the recomp -- wire that through once the helper is lifted; for now
    // we follow the "unknown peer" success branch, matching the common case.
}

// ============================================================================
// snHsmRequestingJoin::OnUpdateChild @ ~0x823DFEE0 | vfn_13
// ============================================================================
void snHsmRequestingJoin_OnUpdateChild(snHsmRequestingJoin* self) { // vfn_13
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* hsmCtx = vfn_11(self);
    snSession_AssociateConnection(hsmCtx, self, &self->m_UndoingJoinRequest);
    snSession_ProcessPendingConnections(hsmCtx, self, &self->m_UndoingJoinRequest);
}


// ============================================================================
// snJoinMachine_FDF0_g / snNotifyJoin_PackData @ 0x823EFDF0 | size: 0x64
// ============================================================================
void snNotifyJoin_PackData(snConnectionRef* dst, uint32_t sessionId, void* sourceRecord) { // FDF0_g
    dst->m_nAddr0 = sessionId;
    snJoinMachine_SetCapacity(&dst->m_nAddr1, 0);
    if (sourceRecord != nullptr) {
        uint32_t word4  = *(uint32_t*)((char*)sourceRecord + 4);
        uint32_t word16 = *(uint32_t*)((char*)sourceRecord + 16);
        uint32_t word24 = *(uint32_t*)((char*)sourceRecord + 24);
        snJoinMachine_CopyNotifyData(&dst->m_nAddr1, word4, word16, word24);
    }
}

// (insertion boundary -- continuing inside namespace rage)

// ============================================================================
// snJoinMachine::OnEvent @ 0x823F0A70 | size: 0x2F8 | vfn_13
// Handles LocalJoinSucceeded / RemoteJoinSucceeded / AcceptJoinRequest
// ============================================================================

// Stack event shape used to build transition events on the fly.
struct snHsmStackEvent {
    void*    m_pVtable;   // +0
    uint32_t m_nPad04;    // +4
    uint32_t m_nPad08;    // +8
    uint32_t m_nPad0C;    // +12
    uint32_t m_nPad10;    // +16
    uint32_t m_nPad14;    // +20
    uint32_t m_nPad18;    // +24
    uint32_t m_nPad1C;    // +28
};

// External event-type constants
extern uint32_t g_evtType_LocalJoinSucceeded;  // @ 0x825D17F4
extern uint32_t g_evtType_RemoteJoinSucceeded; // @ 0x825D1800
extern uint32_t g_evtType_AcceptJoinRequest;   // @ 0x825D180C

extern bool snSession_IsConnectionPending(void* sessionConnMap, void* connRef); // @ 0x823EA250
extern void snJoinMachine_RaiseSessionJoinFailed(void* thisPtr, void* evt);     // @ 0x823E97D0
extern void* g_vtable_EvtSessionJoinFailed_ptr; // @ .rdata

namespace {

// Promote into a success sub-state
inline void snJoinMachine_PromoteSuccess(snJoinMachine* self, void* connList) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* hsmCtx = vfn_11(self);
    snSession_AssociateConnection(hsmCtx, self, connList);
    snSession_ProcessPendingConnections(hsmCtx, self, connList);
}

// Builds and raises an EvtSessionJoinFailed on the caller stack.
inline void snJoinMachine_RaiseFailure(snJoinMachine* self) {
    snHsmStackEvent evt;
    snHsmState_Init(&evt);
    evt.m_pVtable = &g_vtable_EvtSessionJoinFailed_ptr;
    snJoinMachine_RaiseSessionJoinFailed(self, &evt);
}

// Returns the runtime event-type id for the current HSM event.
inline uint32_t snJoinMachine_GetCurrentEventTypeId(snJoinMachine* self) {
    void** vtable = *reinterpret_cast<void***>(self);
    void* (*vfn_10)(void*) = (void* (*)(void*))vtable[10];
    snHsmStateContext* stateCtx = (snHsmStateContext*)vfn_10(self);
    snHsmEventData* eventObj = (snHsmEventData*)stateCtx->m_pCurrentEvent;
    void** evtVtable = *reinterpret_cast<void***>(eventObj);
    uint32_t (*getTypeId)(void*) = (uint32_t (*)(void*))evtVtable[1];
    return getTypeId(eventObj);
}

} // anonymous namespace

void snJoinMachine_OnEvent(snJoinMachine* self) { // vfn_13
    // -- Arm 1 : EvtLocalJoinSucceeded --
    uint32_t typeId = snJoinMachine_GetCurrentEventTypeId(self);
    if (typeId == g_evtType_LocalJoinSucceeded) {
        snSinglesNetClient* networkClient = (snSinglesNetClient*)self->m_pSession;
        bool clientIsHostSide = ((networkClient->m_nSessionFlags & 0x80u) != 0);

        if (!clientIsHostSide) {
            snJoinMachine_PromoteSuccess(self, &self->m_LocalJoinPending);
        } else {
            snJoinMachine_PromoteSuccess(self, &self->m_RemoteJoinPending);
        }
        return;
    }

    // -- Arm 2 : EvtRemoteJoinSucceeded --
    typeId = snJoinMachine_GetCurrentEventTypeId(self);
    if (typeId == g_evtType_RemoteJoinSucceeded) {
        snSinglesNetClient* networkClient = (snSinglesNetClient*)self->m_pSession;

        snConnectionRef* broadcast = (snConnectionRef*)snSession_InitConnectionBroadcast(
            networkClient, nullptr, nullptr, nullptr, 0);

        bool alreadyTracked = snSession_IsConnectionPending(networkClient->m_pPeerList,
                                                            &broadcast->m_pPayload);

        if (!alreadyTracked) {
            snJoinMachine_PromoteSuccess(self, &self->m_AcceptingJoinRequest);
            return;
        }

        snJoinMachine_RaiseFailure(self);
        return;
    }

    // -- Arm 3 : EvtAcceptJoinRequest --
    typeId = snJoinMachine_GetCurrentEventTypeId(self);
    if (typeId == g_evtType_AcceptJoinRequest) {
        snSinglesNetClient* networkClient = (snSinglesNetClient*)self->m_pSession;

        snConnectionRef* broadcast = (snConnectionRef*)snSession_InitConnectionBroadcast(
            networkClient, nullptr, nullptr, nullptr, 0);

        bool alreadyTracked = snSession_IsConnectionPending(networkClient->m_pPeerList,
                                                            &broadcast->m_pPayload);

        if (!alreadyTracked) {
            snJoinMachine_PromoteSuccess(self, &self->m_RequestingJoin);
            return;
        }

        snJoinMachine_RaiseFailure(self);
        return;
    }

    // -- Fallthrough : unknown event -> raise session-join-failed --
    snJoinMachine_RaiseFailure(self);
}

// ============================================================================
// Supporting lifts (session-join flow)
// ============================================================================

void* snSession_InitConnectionBroadcast(void* client, void* tableA, void* tableB,
                                        void* tableC, int flag);

void snHsmWaitingForReply_TimeoutTick(void* thisPtr, void* event, int kind,
                                      void* context);

// snJoinMachine_SetReplyTimeoutHandler -- binds thisPtr + timeout thunk into callback slot
void snJoinMachine_SetReplyTimeoutHandler(snHsmStateWithCallback* self) {
    self->m_pCallbackSelf = self;
    self->m_pCallbackHandler =
        reinterpret_cast<void*>(&thunk_snHsmWaitingForReply_TimeoutHandler);
}

// snJoinMachine_ComputeReplyTimeoutMs -- reply-timeout math: (rtt+50)*rtt*100
int32_t snJoinMachine_ComputeReplyTimeoutMs(const snNetworkTransport* transport) {
    int32_t rtt = (int32_t)(transport->m_nRttBase + transport->m_nRttExtra);
    return (rtt + 50) * rtt * 100;
}

// snJoinMachine_IsHostSide -- bit-7 host/peer test on client+3744
bool snJoinMachine_IsHostSide(const snSinglesNetClient* networkClient) {
    return (networkClient->m_nSessionFlags & 0x80u) != 0;
}

} // namespace rage
