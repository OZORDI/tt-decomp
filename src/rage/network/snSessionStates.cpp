/**
 * snSessionStates.cpp — Session Network State Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of hierarchical state machines for active session management.
 * Handles presence changes, joining, leaving, and other session state transitions.
 */

#include <stdint.h>

namespace rage {

// Forward declarations
class SinglesNetworkClient;

// External utility functions
extern void util_D988(void* context, void* session, void* connectionList);  // @ 0x823ED988
extern void util_DA90(void* context, void* session, void* connectionList);  // @ 0x823EDA90
extern void util_F850(void* parent, void* childState);                     // @ 0x823EF850 — AttachChildState

// ────────────────────────────────────────────────────────────────────────────
// snSession_snRoot_snChangingPresence State Machine
// ────────────────────────────────────────────────────────────────────────────

/**
 * snSession_snRoot_snChangingPresence::GetStateName @ 0x823E3040 | size: 0x50
 *
 * Manages network connections during a session presence change.
 * 
 * This function is part of the snChangingPresence state which handles transitions
 * in session presence (e.g., "in menu" -> "in game", "playing" -> "paused").
 * 
 * Process:
 *   1. Call vfn_11 on this object (returns a context/state object)
 *   2. Get the network client's connection list from m_pNetworkClient
 *   3. Associate connections with the session (util_D988)
 *   4. Process pending connection state changes (util_DA90)
 *
 * The connection list is located at offset +5780 (0x1694) from the network
 * client base pointer.
 *
 * @note This is vtable slot 13 in the snChangingPresence state machine
 */
void snSession_snRoot_snChangingPresence_vfn_13(void* thisPtr) {
    void* session = thisPtr;
    
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);
    
    SinglesNetworkClient* networkClient = *(SinglesNetworkClient**)((char*)session + 20);
    void* connectionList = (char*)networkClient + 5780;
    
    util_D988(context, session, connectionList);
    util_DA90(context, session, connectionList);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Names (15 GetStateName functions, 12B each)
// All return a const char* from .rdata pointing to the state's display name.
// ─────────────────────────────────────────────────────────────────────────────

// State name string addresses (verified from binary)
static const char* const s_snRoot              = "snRoot";              // @ 0x820746D4
static const char* const s_snJoining           = "Joining";             // @ 0x820746DC
static const char* const s_snLeaving           = "Leaving";             // @ 0x820746E4
static const char* const s_snDormant           = "Dormant";             // @ 0x820746EC
static const char* const s_snCreating          = "Creating";            // @ 0x820746F4
static const char* const s_snDestroying        = "Destroying";          // @ 0x82074700
static const char* const s_snChangingPresence  = "ChangingPresence";    // @ 0x8207470C
static const char* const s_snChanging          = "Changing";            // @ 0x82074720
static const char* const s_snWaitingForReplies = "WaitingForReplies";   // @ 0x8207472C
static const char* const s_snActive            = "Active";              // @ 0x82074740
static const char* const s_snArbGuestReg       = "ArbGuestRegistering"; // @ 0x82074748
static const char* const s_snInProgress        = "InProgress";          // @ 0x8207475C
static const char* const s_snStarting          = "Starting";            // @ 0x82074768
static const char* const s_snArbHostReg        = "ArbHostRegistering";  // @ 0x82074774
static const char* const s_snEnding            = "Ending";              // @ 0x82074788

namespace snSession_States {

const char* snRoot_GetName()              { return s_snRoot; }              // @ 0x823E2580 vfn_2
const char* snJoining_GetName()           { return s_snJoining; }           // @ 0x823E2650 vfn_2
const char* snLeaving_GetName()           { return s_snLeaving; }           // @ 0x823E2800 vfn_2
const char* snDormant_GetName()           { return s_snDormant; }           // @ 0x823E2960 vfn_2
const char* snCreating_GetName()          { return s_snCreating; }          // @ 0x823E2B18 vfn_2
const char* snDestroying_GetName()        { return s_snDestroying; }        // @ 0x823E2D38 vfn_2
const char* snChangingPresence_GetName()  { return s_snChangingPresence; }  // @ 0x823E3030 vfn_2
const char* snChanging_GetName()          { return s_snChanging; }          // @ 0x823E3258 vfn_2
const char* snWaitingForReplies_GetName() { return s_snWaitingForReplies; } // @ 0x823E3520 vfn_2
const char* snActive_GetName()            { return s_snActive; }            // @ 0x823E3850 vfn_2
const char* snArbGuestReg_GetName()       { return s_snArbGuestReg; }       // @ 0x823E3DE0 vfn_2
const char* snInProgress_GetName()        { return s_snInProgress; }        // @ 0x823E3FB0 vfn_2
const char* snStarting_GetName()          { return s_snStarting; }          // @ 0x823E4068 vfn_2
const char* snArbHostReg_GetName()        { return s_snArbHostReg; }        // @ 0x823E4100 vfn_2
const char* snEnding_GetName()            { return s_snEnding; }            // @ 0x823E45A0 vfn_2

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnEnter / OnExit Handlers
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snJoining::OnEnter @ 0x823E2660 | size: 0x14 | vfn_14
 * Increments the join reference count at session+9672.
 */
void snJoining_OnEnter(void* thisPtr) {
    void* session = *(void**)((char*)thisPtr + 16);
    (*(int32_t*)((char*)session + 9672))++;
}

/**
 * snJoining::OnExit @ 0x823E2678 | size: 0x14 | vfn_15
 * Decrements the join reference count at session+9672.
 */
void snJoining_OnExit_DecrementRef(void* thisPtr) {
    void* session = *(void**)((char*)thisPtr + 16);
    (*(int32_t*)((char*)session + 9672))--;
}

/**
 * snJoining::OnExit @ 0x823E2930 | size: 0x2C | vfn_6
 *
 * Exit handler for the Joining state.  If the embedded rlNotifier at
 * this+24 still has a pending callback (+8 != NULL), clears the callback
 * pointer and invokes the notifier's Cancel function (vtable slot 4 of
 * the linked object at +0, i.e. vfn at byte offset +16).
 */
void snJoining_OnExit(void* thisPtr) {
    char* notifier = (char*)thisPtr + 24;
    void* pending = *(void**)(notifier + 8);
    if (pending == nullptr) return;

    void* linked = *(void**)(notifier + 0);
    *(void**)(notifier + 8) = nullptr;
    // Cancel the pending notification via vtable slot 4
    void (*cancelFn)(void*) = *(void (**)(void*))((char*)linked + 16);
    cancelFn(notifier);
}

/**
 * snDormant::OnEnter @ 0x823E2970 | size: 0x10 | vfn_14
 * Clears the field at session+3784 to 0 on entering dormant state.
 */
void snDormant_OnEnter(void* thisPtr) {
    void* session = *(void**)((char*)thisPtr + 16);
    *(uint32_t*)((char*)session + 3784) = 0;
}

/**
 * snIdle::OnExit @ 0x823EE740 | size: 0x8 | vfn_5
 *
 * Exit handler for the Active::Idle state.
 * Detaches the child state at this+24 via util_F850 (AttachChildState).
 */
void snIdle_OnExit(void* thisPtr) {
    util_F850(thisPtr, (char*)thisPtr + 24);
}

/**
 * snStarting::OnExit @ 0x823E44E0 | size: 0x3C | vfn_5
 *
 * Exit handler for the Active::Starting state.
 * Detaches two child states at this+24 and this+1412 via util_F850.
 */
void snStarting_OnExit(void* thisPtr) {
    util_F850(thisPtr, (char*)thisPtr + 24);
    util_F850(thisPtr, (char*)thisPtr + 1412);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession Utility Functions
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snSession::HasPendingJoins @ 0x823E71F0 | size: 0x1C
 * Returns true if the join reference count (session+9672) is > 0.
 */
bool snSession_HasPendingJoins(void* session) {
    int32_t joinRefCount = *(int32_t*)((char*)session + 9672);
    return joinRefCount > 0;
}

/**
 * snSession::FindListTail @ 0x823EA980 | size: 0x34
 * Walks a linked list (next pointer at +20) to find the last node.
 * Returns nullptr if the initial node pointer (at +0) is null.
 */
void* snSession_FindListTail(void* listHead) {
    void* node = *(void**)listHead;
    if (!node) return nullptr;

    while (*(void**)((char*)node + 20) != nullptr) {
        node = *(void**)((char*)node + 20);
    }
    return node;
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Destructors — Batch 2
// ─────────────────────────────────────────────────────────────────────────────

// External destructor helpers for Joining and Leaving states
extern void NotifyHandler_56C0_g(void* thisPtr);  // @ 0x823E56C0 — Joining rlNotifier dtor body
extern void util_5900(void* thisPtr);              // @ 0x823E5900 — Leaving-level destructor body

/**
 * snJoining::~snJoining @ 0x823E5498 | size: 0x5C | vfn_0
 *
 * Destructor for the Joining state. Tears down the embedded rlNotifier
 * at this+24 via NotifyHandler_56C0_g, then calls hsmState base destructor.
 */
void snJoining_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5498
    NotifyHandler_56C0_g((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snLeaving::~snLeaving @ 0x823E5788 | size: 0x5C | vfn_0
 *
 * Destructor for the Leaving state. Tears down the embedded leave machine
 * state at this+24 via util_5900, then calls hsmState base destructor.
 */
void snLeaving_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5788
    util_5900((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnEnter Handlers — Batch 2
// ─────────────────────────────────────────────────────────────────────────────

// External helpers for OnEnter handlers
extern void snLinkedList_InsertNode(void* listHead, void* node);            // @ 0x823DD170
extern bool snLeaveMachine_TryLeave(void* thisPtr);                         // @ 0x823E8558
extern void snSession_TransitionDestroying(void* thisPtr, void* evtState);  // @ 0x823EAA20
extern void snSession_TransitionChanging(void* thisPtr, void* evtState);    // @ 0x823EAB70
extern void snSession_TransitionStarting(void* thisPtr, void* evtState);    // @ 0x823EAC18
extern void snSession_TransitionEnding(void* thisPtr, void* evtState);      // @ 0x823EACC0
extern void snHsmContext_SetMaxTransitions(void* thisPtr, int32_t limit);    // @ 0x823ED4F8
extern void SinglesNetworkClient_RegisterNotify(void* networkClient);       // @ 0x823DA940
extern bool xamSession_StartSession(void* sessionNotify);                    // @ 0x8236E6C0
extern bool xamSession_EndSession(void* sessionNotify);                      // @ 0x8236E920

// Thunk function pointers used as notification callbacks
extern void thunk_fn_snSession_2D48();   // @ 0x823EB108 — Destroying notification callback
extern void thunk_fn_snSession_88F8();   // @ 0x823EB118 — WaitingForReplies notification callback
extern void thunk_fn_snSession_4110();   // @ 0x823EB120 — StartingSession notification callback
extern void thunk_fn_snSession_45B0();   // @ 0x823EB128 — Ending notification callback

/**
 * snRoot::OnEnter @ 0x823E49E0 | size: 0x84 | vfn_5
 *
 * Entry handler for the root session state. Attaches eight child sub-states
 * at fixed offsets within the root state structure via util_F850.
 *   - this+24    (Dormant)
 *   - this+48    (Joining)
 *   - this+376   (Leaving)
 *   - this+424   (Creating)
 *   - this+3392  (ChangingPresence)
 *   - this+4048  (Active)
 *   - this+4292  (Destroying)
 *   - this+5756  (Migrating)
 */
void snRoot_OnEnter(void* thisPtr) { // @ 0x823E49E0
    util_F850(thisPtr, (char*)thisPtr + 24);
    util_F850(thisPtr, (char*)thisPtr + 48);
    util_F850(thisPtr, (char*)thisPtr + 376);
    util_F850(thisPtr, (char*)thisPtr + 424);
    util_F850(thisPtr, (char*)thisPtr + 3392);
    util_F850(thisPtr, (char*)thisPtr + 4048);
    util_F850(thisPtr, (char*)thisPtr + 4292);
    util_F850(thisPtr, (char*)thisPtr + 5756);
}

/**
 * snJoining::OnEnter @ 0x823E3F50 | size: 0x5C | vfn_5
 *
 * Entry handler for the Joining state. Initializes the embedded child
 * state at this+24 by copying the session pointer from this+16 into
 * the child's fields, then calls the child's OnEnter (vfn_5) and sets
 * the child's parent pointer to this.
 */
void snJoining_OnEnterState(void* thisPtr) { // @ 0x823E3F50
    char* child = (char*)thisPtr + 24;
    void* session = *(void**)((char*)thisPtr + 16);

    // Initialize child state fields
    *(void**)(child + 20) = child;     // self-reference
    *(void**)(child + 4) = session;    // session pointer
    *(void**)(child + 16) = session;   // session pointer (parent context)

    // Call child's OnEnter via vtable slot 5
    void** vtable = *(void***)child;
    void (*childOnEnter)(void*) = (void (*)(void*))vtable[5];
    childOnEnter(child);

    // Set child's parent to this state
    *(void**)(child + 8) = thisPtr;
}

/**
 * snDestroying::OnEnter @ 0x823E2E30 | size: 0x8C | vfn_14
 *
 * Entry handler for the Destroying state. Increments join reference count,
 * sets up notification callback for destroy completion, registers with
 * the session's linked list, then initiates leave machine. If leave
 * fails immediately, creates an event to transition to Destroying.
 */
void snDestroying_OnEnter(void* thisPtr) { // @ 0x823E2E30
    char* notifier = (char*)thisPtr + 24;
    void* session = *(void**)((char*)thisPtr + 16);

    // Increment join reference count
    (*(int32_t*)((char*)session + 9672))++;

    // Set up notification callback (thunk -> snSession_2D48)
    *(void**)(notifier + 8) = (void*)&thunk_fn_snSession_2D48;
    *(void**)(notifier + 4) = thisPtr;

    // Register notifier in session's linked list at session+232
    snLinkedList_InsertNode((char*)session + 232, notifier);

    // Attempt to leave the session
    bool leaveResult = snLeaveMachine_TryLeave(thisPtr);
    if (!leaveResult) {
        // Leave failed immediately — create event and transition
        char evtState[24];
        hsmEvent_Init(evtState);
        *(void**)evtState = (void*)&thunk_fn_snSession_2D48; // vtable
        snSession_TransitionDestroying(thisPtr, evtState);
    }
}

/**
 * snWaitingForReplies::OnEnter @ 0x823E3530 | size: 0x58 | vfn_14
 *
 * Entry handler for the WaitingForReplies sub-state. Clears the reply
 * counter at this+48, sets the max transition count to 10000, sets up
 * the notification callback, and registers with the network client's
 * notification list.
 */
void snWaitingForReplies_OnEnter(void* thisPtr) { // @ 0x823E3530
    char* notifier = (char*)thisPtr + 24;

    // Clear reply counter
    *(uint32_t*)((char*)thisPtr + 48) = 0;

    // Set max transition count
    snHsmContext_SetMaxTransitions(thisPtr, 10000);

    // Set up notification callback (thunk -> SinglesNetworkClient_88F8)
    *(void**)(notifier + 4) = thisPtr;
    *(void**)(notifier + 8) = (void*)&thunk_fn_snSession_88F8;

    // Register with network client's notification list
    void* session = *(void**)((char*)thisPtr + 16);
    void* networkClient = *(void**)((char*)session + 164);
    SinglesNetworkClient_RegisterNotify(networkClient);
}

/**
 * snStartingSession::OnEnter @ 0x823E41F8 | size: 0x80 | vfn_14
 *
 * Entry handler for the StartingSession sub-state within Active::Starting.
 * Sets up notification callback, registers with session's linked list,
 * then calls xamSession_StartSession. If start returns immediately,
 * creates an event to transition.
 */
void snStartingSession_OnEnter(void* thisPtr) { // @ 0x823E41F8
    char* notifier = (char*)thisPtr + 24;
    void* session = *(void**)((char*)thisPtr + 16);

    // Set up notification callback (thunk -> snSession_4110)
    *(void**)(notifier + 4) = thisPtr;
    *(void**)(notifier + 8) = (void*)&thunk_fn_snSession_4110;

    // Register notifier in session's linked list at session+232
    snLinkedList_InsertNode((char*)session + 232, notifier);

    // Attempt to start session via XAM
    bool result = xamSession_StartSession((char*)session + 232);
    if (!result) {
        // Start completed immediately — create event and transition
        char evtState[24];
        hsmEvent_Init(evtState);
        *(void**)evtState = (void*)&thunk_fn_snSession_4110; // vtable
        snSession_TransitionStarting(thisPtr, evtState);
    }
}

/**
 * snEnding::OnEnter @ 0x823E4698 | size: 0x90 | vfn_14
 *
 * Entry handler for the Ending sub-state within Active. Increments
 * join reference count, sets up notification callback, registers with
 * session's linked list, then calls xamSession_EndSession. If end
 * returns immediately, creates an event to transition.
 */
void snEnding_OnEnter(void* thisPtr) { // @ 0x823E4698
    char* notifier = (char*)thisPtr + 24;
    void* session = *(void**)((char*)thisPtr + 16);

    // Increment join reference count
    (*(int32_t*)((char*)session + 9672))++;

    // Set up notification callback (thunk -> snSession_45B0)
    *(void**)(notifier + 8) = (void*)&thunk_fn_snSession_45B0;
    *(void**)(notifier + 4) = thisPtr;

    // Register notifier in session's linked list at session+232
    snLinkedList_InsertNode((char*)session + 232, notifier);

    // Attempt to end session via XAM
    bool result = xamSession_EndSession((char*)session + 232);
    if (!result) {
        // End completed immediately — create event and transition
        char evtState[24];
        hsmEvent_Init(evtState);
        *(void**)evtState = (void*)&thunk_fn_snSession_45B0; // vtable
        snSession_TransitionEnding(thisPtr, evtState);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnExit Handlers — Batch 2
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snActive::OnExit_Full @ 0x823E4918 | size: 0xC4 | vfn_6
 *
 * Exit handler for the Active state. Detaches four child sub-states at
 * this+24, this+1436, this+1460, and this+2920. Each child's pending
 * notification (at child+8) is checked; if non-null, cleared and
 * cancelled via its vtable slot 4 (Cancel).
 */
void snActive_OnExit_Full(void* thisPtr) { // @ 0x823E4918
    // Detach child at this+24
    char* child1 = (char*)thisPtr + 24;
    if (*(void**)(child1 + 8) != nullptr) {
        void** vtable1 = *(void***)child1;
        *(void**)(child1 + 8) = nullptr;
        void (*cancel1)(void*) = (void (*)(void*))vtable1[4];
        cancel1(child1);
    }

    // Detach child at this+1436
    char* child2 = (char*)thisPtr + 1436;
    if (*(void**)(child2 + 8) != nullptr) {
        void** vtable2 = *(void***)child2;
        *(void**)(child2 + 8) = nullptr;
        void (*cancel2)(void*) = (void (*)(void*))vtable2[4];
        cancel2(child2);
    }

    // Detach child at this+1460
    char* child3 = (char*)thisPtr + 1460;
    if (*(void**)(child3 + 8) != nullptr) {
        void** vtable3 = *(void***)child3;
        *(void**)(child3 + 8) = nullptr;
        void (*cancel3)(void*) = (void (*)(void*))vtable3[4];
        cancel3(child3);
    }

    // Detach child at this+2920
    char* child4 = (char*)thisPtr + 2920;
    if (*(void**)(child4 + 8) != nullptr) {
        void** vtable4 = *(void***)child4;
        *(void**)(child4 + 8) = nullptr;
        void (*cancel4)(void*) = (void (*)(void*))vtable4[4];
        cancel4(child4);
    }
}

/**
 * snStarting::OnExit_Full @ 0x823E4520 | size: 0x7C | vfn_6
 *
 * Exit handler for the Starting sub-state within Active. Cancels pending
 * notifications on two embedded notifiers at this+24 and this+1412.
 * If the notifier's pending pointer (+8) is non-null, clears it and
 * calls the notifier's Cancel function (vtable slot 4).
 */
void snStarting_OnExit_Full(void* thisPtr) { // @ 0x823E4520
    // Cancel notifier at this+24
    char* notifier1 = (char*)thisPtr + 24;
    if (*(void**)(notifier1 + 8) != nullptr) {
        void** vtable1 = *(void***)notifier1;
        *(void**)(notifier1 + 8) = nullptr;
        void (*cancel1)(void*) = (void (*)(void*))vtable1[4];
        cancel1(notifier1);
    }

    // Cancel notifier at this+1412
    char* notifier2 = (char*)thisPtr + 1412;
    if (*(void**)(notifier2 + 8) != nullptr) {
        void** vtable2 = *(void***)notifier2;
        *(void**)(notifier2 + 8) = nullptr;
        void (*cancel2)(void*) = (void (*)(void*))vtable2[4];
        cancel2(notifier2);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnTick / Event Handlers — Batch 2
// ─────────────────────────────────────────────────────────────────────────────

// Event type ID globals (RTTI descriptors loaded at runtime)
extern uint32_t g_evtType_SessionClosed;         // @ 0x825D1978 — used by snRoot
extern uint32_t g_evtType_SessionDeregistered;    // @ 0x825D1984 — used by snRoot
extern uint32_t g_evtType_PresenceChanged;        // @ 0x825D18AC — used by snChanging
extern uint32_t g_evtType_SessionStarted;         // @ 0x825D1924 — used by snInProgress
extern uint32_t g_evtType_SessionEnded;           // @ 0x825D1930 — used by snEnding (1st)
extern uint32_t g_evtType_SessionEndedAlt;        // @ 0x825D193C — used by snEnding (2nd)

// External session helpers
extern void snSession_HandleClose(void* thisPtr);                            // @ 0x823E8BB0
extern void SinglesNetworkClient_ProcessChange(void* thisPtr);               // @ 0x823E87F8
extern void snSession_NotifyStateChange(void* session, int32_t type, int32_t param); // @ 0x82371BF8

/**
 * snRoot::OnTick @ 0x823E2590 | size: 0xBC | vfn_12
 *
 * Tick handler for the root state. Checks the event queue for
 * SessionClosed or SessionDeregistered events. If either matches,
 * calls snSession_HandleClose to process session shutdown.
 * Otherwise sets the consumed flag to false.
 */
void snRoot_OnTick(void* thisPtr, void* event, uint8_t* consumed) { // @ 0x823E2590
    *consumed = 1;

    // Get event via vfn_10 (GetCurrentEvent)
    void** vtable = *(void***)thisPtr;
    void* (*getEvent)(void*) = (void* (*)(void*))vtable[10];
    void* evtObj = getEvent(thisPtr);

    // Get event type via vfn_1
    void* evtData = *(void**)((char*)evtObj + 12);
    void** evtVtable = *(void***)evtData;
    int32_t (*getType)(void*) = (int32_t (*)(void*))evtVtable[1];
    int32_t evtType = getType(evtData);

    // Check for SessionClosed event
    if (evtType == *(int32_t*)&g_evtType_SessionClosed) {
        snSession_HandleClose(thisPtr);
        return;
    }

    // Check for SessionDeregistered event (second check)
    vtable = *(void***)thisPtr;
    getEvent = (void* (*)(void*))vtable[10];
    evtObj = getEvent(thisPtr);
    evtData = *(void**)((char*)evtObj + 12);
    evtVtable = *(void***)evtData;
    getType = (int32_t (*)(void*))evtVtable[1];
    evtType = getType(evtData);

    if (evtType == *(int32_t*)&g_evtType_SessionDeregistered) {
        snSession_HandleClose(thisPtr);
        return;
    }

    // Event not consumed
    *consumed = 0;
}

/**
 * snChanging::OnTick @ 0x823E3470 | size: 0xAC | vfn_12
 *
 * Tick handler for the Changing sub-state within ChangingPresence.
 * Checks the event queue for PresenceChanged event. If matched,
 * calls vfn_11 to get context, then associates connections and
 * processes pending connection changes. Also calls
 * SinglesNetworkClient_ProcessChange to update presence.
 */
void snChanging_OnTick(void* thisPtr, void* event, uint8_t* consumed) { // @ 0x823E3470
    *consumed = 1;

    // Get event type
    void** vtable = *(void***)thisPtr;
    void* (*getEvent)(void*) = (void* (*)(void*))vtable[10];
    void* evtObj = getEvent(thisPtr);
    void* evtData = *(void**)((char*)evtObj + 12);
    void** evtVtable = *(void***)evtData;
    int32_t (*getType)(void*) = (int32_t (*)(void*))evtVtable[1];
    int32_t evtType = getType(evtData);

    if (evtType != *(int32_t*)&g_evtType_PresenceChanged) {
        *consumed = 0;
        return;
    }

    // Get context via vfn_11
    vtable = *(void***)thisPtr;
    void* (*getContext)(void*) = (void* (*)(void*))vtable[11];
    void* context = getContext(thisPtr);

    // Get network client's connection list at offset +5828
    SinglesNetworkClient* networkClient = *(SinglesNetworkClient**)((char*)thisPtr + 20);
    void* connectionList = (char*)networkClient + 5828;

    // Associate connections
    util_D988(context, thisPtr, connectionList);

    // Process presence change on network client
    SinglesNetworkClient_ProcessChange(thisPtr);

    // Process pending connections
    util_DA90(context, thisPtr, connectionList);
}

/**
 * snWaitingForReplies::OnTick @ 0x823E35D0 | size: 0x94 | vfn_12
 *
 * Tick handler for the WaitingForReplies sub-state. Gets the event via
 * vfn_10, checks its type via vfn_1. If the result is zero (no more
 * pending replies), creates a transition event. Otherwise clears the
 * consumed flag.
 */
void snWaitingForReplies_OnTick(void* thisPtr, void* event, uint8_t* consumed) { // @ 0x823E35D0
    *consumed = 1;

    // Get event type
    void** vtable = *(void***)thisPtr;
    void* (*getEvent)(void*) = (void* (*)(void*))vtable[10];
    void* evtObj = getEvent(thisPtr);
    void* evtData = *(void**)((char*)evtObj + 12);
    void** evtVtable = *(void***)evtData;
    int32_t (*getType)(void*) = (int32_t (*)(void*))evtVtable[1];
    int32_t result = getType(evtData);

    if (result == 0) {
        // All replies received — create transition event
        char evtState[24];
        hsmEvent_Init(evtState);
        *(void**)evtState = (void*)&thunk_fn_snSession_88F8; // vtable
        snSession_TransitionChanging(thisPtr, evtState);
    } else {
        *consumed = 0;
    }
}

/**
 * snInProgress::OnTick @ 0x823E3FC0 | size: 0xA4 | vfn_12
 *
 * Tick handler for the InProgress sub-state within Active. Checks the
 * event queue for SessionStarted event. If matched, calls vfn_11 to
 * get context, then associates connections at network client offset
 * +3344 and processes pending connections.
 */
void snInProgress_OnTick(void* thisPtr, void* event, uint8_t* consumed) { // @ 0x823E3FC0
    *consumed = 1;

    // Get event type
    void** vtable = *(void***)thisPtr;
    void* (*getEvent)(void*) = (void* (*)(void*))vtable[10];
    void* evtObj = getEvent(thisPtr);
    void* evtData = *(void**)((char*)evtObj + 12);
    void** evtVtable = *(void***)evtData;
    int32_t (*getType)(void*) = (int32_t (*)(void*))evtVtable[1];
    int32_t evtType = getType(evtData);

    if (evtType != *(int32_t*)&g_evtType_SessionStarted) {
        *consumed = 0;
        return;
    }

    // Get context via vfn_11
    vtable = *(void***)thisPtr;
    void* (*getContext)(void*) = (void* (*)(void*))vtable[11];
    void* context = getContext(thisPtr);

    // Get network client's connection list at offset +3344
    SinglesNetworkClient* networkClient = *(SinglesNetworkClient**)((char*)thisPtr + 20);
    void* connectionList = (char*)networkClient + 3344;

    // Associate and process connections
    util_D988(context, thisPtr, connectionList);
    util_DA90(context, thisPtr, connectionList);
}

/**
 * snStarting::OnUpdate @ 0x823E4078 | size: 0x88 | vfn_13
 *
 * Update handler for the Starting sub-state. Checks session flags at
 * session+3744 bits 3 and 7. If bit 3 is set and bit 7 is clear,
 * selects the child at this+24 (ArbHostRegistering). Otherwise selects
 * the child at this+1412 (StartingSession). Then calls vfn_11 to get
 * context, associates connections and processes pending connections.
 */
void snStarting_OnUpdate(void* thisPtr) { // @ 0x823E4078
    void* session = *(void**)((char*)thisPtr + 16);
    uint8_t flags = *(uint8_t*)((char*)session + 3744);

    void* context;
    char* selectedChild;

    if ((flags & 0x08) != 0 && (flags & 0x80) == 0) {
        // Bit 3 set, bit 7 clear — use ArbHostRegistering child
        void** vtable = *(void***)thisPtr;
        void* (*getContext)(void*) = (void* (*)(void*))vtable[11];
        context = getContext(thisPtr);
        selectedChild = (char*)thisPtr + 24;
    } else {
        // Use StartingSession child
        void** vtable = *(void***)thisPtr;
        void* (*getContext)(void*) = (void* (*)(void*))vtable[11];
        context = getContext(thisPtr);
        selectedChild = (char*)thisPtr + 1412;
    }

    // Associate and process connections using selected child
    util_D988(context, thisPtr, selectedChild);
    util_DA90(context, thisPtr, selectedChild);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Destructors — Batch 3
// ─────────────────────────────────────────────────────────────────────────────

// External destructor helpers
extern void snMigrateMachine_DestroyBody(void* thisPtr);  // @ 0x823E5B50 — snMigrating-level destructor body

/**
 * snMigrating::~snMigrating @ 0x823E5998 | size: 0x5C | vfn_0
 *
 * Destructor for the Migrating state. Destroys the embedded migrate machine
 * child at this+24 via snMigrateMachine_DestroyBody, then calls hsmState base destructor.
 * Conditionally frees memory if flags bit 0 is set.
 */
void snMigrating_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5998
    snMigrateMachine_DestroyBody((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snChangingPresence::~snChangingPresence @ 0x823E5BF0 | size: 0x64 | vfn_0
 *
 * Destructor for the ChangingPresence state. Destroys the embedded
 * WaitingForReplies child at this+72 via util_5C58, then destroys the
 * Changing notifier at this+24 via snSession_5E20_gen, then calls hsmState
 * base destructor. Conditionally frees memory.
 */
void snChangingPresence_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5BF0
    util_5C58((char*)thisPtr + 72);
    snSession_5E20_gen((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snArbGuestRegistering::~snArbGuestRegistering @ 0x823E6068 | size: 0x5C | vfn_0
 *
 * Destructor for the ArbGuestRegistering sub-state within Active::Idle.
 * Destroys the embedded child at this+24 via util_6178, then calls hsmState
 * base destructor. Conditionally frees memory.
 */
void snArbGuestRegistering_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E6068
    util_6178((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession Notification Callback Handlers — Batch 3
//
// These handle async operation results. Each checks the result code (r5),
// and on success creates an EvtCreateSucceeded event with session data,
// or on failure creates an EvtCreateFailed event. Both are dispatched
// via snSession_9478_fw or util_93D0.
// ─────────────────────────────────────────────────────────────────────────────

// External event dispatch helpers
extern void snSession_DispatchEventWithData(void* thisPtr, void* eventData);  // @ 0x823E9478
extern void snSession_DispatchSimpleEvent(void* thisPtr, void* eventData);    // @ 0x823E93D0
extern void snSession_DispatchStartEvent(void* thisPtr, void* eventData);     // @ 0x823E95D8

/**
 * snSession_DD70_w @ 0x823DDD70 | size: 0x84
 *
 * Notification callback for create-guest operations (result codes 2/3).
 * On result=2 (success): constructs EvtCreateSucceeded event with session
 * data from session+276/280, dispatches via snSession_DispatchEventWithData.
 * On result=3 (failure): constructs EvtCreateFailed event, dispatches via
 * snSession_DispatchSimpleEvent.
 */
void snSession_DD70_w(void* thisPtr, void* /*unused*/, int32_t resultCode) { // @ 0x823DDD70
    if (resultCode == 2) {
        // Success — build EvtCreateSucceeded with session data
        void* session = *(void**)((char*)thisPtr + 20);
        uint32_t sessionId = *(uint32_t*)((char*)session + 276);

        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(uint32_t*)(eventData + 12) = sessionId;
        // Copy 8 bytes of session handle from session+280
        uint64_t sessionHandle = *(uint64_t*)((char*)session + 280);
        *(uint64_t*)(eventData + 16) = sessionHandle;
        // Set vtable to EvtCreateSucceeded
        *(void**)eventData = (void*)0x82072AA0;

        snSession_DispatchEventWithData(thisPtr, eventData);
        return;
    }

    if (resultCode == 3) {
        // Failure — build EvtCreateFailed
        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(void**)eventData = (void*)0x82072A28;  // EvtCreateFailed vtable
        snSession_DispatchSimpleEvent(thisPtr, eventData);
    }
}

/**
 * snSession_E1A0_w @ 0x823DE1A0 | size: 0x84
 *
 * Notification callback for config-request operations (result codes 8/9).
 * Same pattern as DD70_w: on result=8 (success), creates EvtCreateSucceeded;
 * on result=9 (failure), creates EvtCreateFailed.
 */
void snSession_E1A0_w(void* thisPtr, void* /*unused*/, int32_t resultCode) { // @ 0x823DE1A0
    if (resultCode == 8) {
        void* session = *(void**)((char*)thisPtr + 20);
        uint32_t sessionId = *(uint32_t*)((char*)session + 276);

        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(uint32_t*)(eventData + 12) = sessionId;
        uint64_t sessionHandle = *(uint64_t*)((char*)session + 280);
        *(uint64_t*)(eventData + 16) = sessionHandle;
        *(void**)eventData = (void*)0x82072AA0;  // EvtCreateSucceeded vtable

        snSession_DispatchEventWithData(thisPtr, eventData);
        return;
    }

    if (resultCode == 9) {
        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(void**)eventData = (void*)0x82072A28;  // EvtCreateFailed vtable
        snSession_DispatchSimpleEvent(thisPtr, eventData);
    }
}

/**
 * snSession_E990 @ 0x823DE990 | size: 0x84
 *
 * Notification callback for start-session operations (result codes 10/11).
 * On result=10 (success), creates EvtCreateSucceeded with session data.
 * On result=11 (failure), creates EvtStartSessionFailed event and
 * dispatches via snSession_DispatchStartEvent.
 */
void snSession_E990(void* thisPtr, void* /*unused*/, int32_t resultCode) { // @ 0x823DE990
    if (resultCode == 10) {
        void* session = *(void**)((char*)thisPtr + 20);
        uint32_t sessionId = *(uint32_t*)((char*)session + 276);

        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(uint32_t*)(eventData + 12) = sessionId;
        uint64_t sessionHandle = *(uint64_t*)((char*)session + 280);
        *(uint64_t*)(eventData + 16) = sessionHandle;
        *(void**)eventData = (void*)0x82072AA0;  // EvtCreateSucceeded vtable

        snSession_DispatchEventWithData(thisPtr, eventData);
        return;
    }

    if (resultCode == 11) {
        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(void**)eventData = (void*)0x82072A8C;  // EvtStartSessionFailed vtable
        snSession_DispatchStartEvent(thisPtr, eventData);
    }
}

/**
 * snSession_EB48_w @ 0x823DEB48 | size: 0x74
 *
 * Notification callback for simple event pairs (result codes 14/15).
 * Both success (14) and failure (15) create an EvtCreateFailed event
 * and dispatch via snSession_DispatchSimpleEvent. Returns early if
 * neither code matches.
 */
void snSession_EB48_w(void* thisPtr, void* /*unused*/, int32_t resultCode) { // @ 0x823DEB48
    if (resultCode == 14) {
        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(void**)eventData = (void*)0x82072A28;  // EvtCreateFailed vtable
        snSession_DispatchSimpleEvent(thisPtr, eventData);
        return;
    }

    if (resultCode == 15) {
        uint8_t eventData[24];
        hsmEvent_Init(eventData);
        *(void**)eventData = (void*)0x82072A28;  // EvtCreateFailed vtable
        snSession_DispatchSimpleEvent(thisPtr, eventData);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession Event Queue and Utility Functions — Batch 3
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snSession::EnqueueDestroyEvent @ 0x823E6D68 | size: 0xAC
 *
 * Creates and enqueues an EvtDestroy event into the session's event queue.
 * Gets the event queue from context+56 (via vfn_11 on the session offset+212),
 * allocates a 24-byte event node via the queue's allocator (vfn_1 at +4),
 * copies the EvtDestroy data into it, and inserts it into the queue via
 * snSession_AddNode.
 *
 * Returns true if the event was successfully enqueued, false if allocation failed.
 */
bool snSession_EnqueueDestroyEvent(void* thisPtr) { // @ 0x823E6D68
    char* sessionOffset = (char*)thisPtr + 212;

    // Initialize event data on stack with EvtDestroy vtable
    uint8_t eventData[24];
    hsmEvent_Init(eventData);
    *(void**)eventData = (void*)0x82072968;  // EvtDestroy vtable

    // Get event queue via vfn_11 (session+212 has its own vtable)
    void** vtable = *(void***)sessionOffset;

    // Allocate event node: call allocator (vfn_1) with size=12, flags=0
    void* allocator = *(void**)((char*)sessionOffset + 4);
    void** allocVtable = *(void***)allocator;
    void* (*allocFn)(void*, int32_t, int32_t) = (void* (*)(void*, int32_t, int32_t))allocVtable[1];
    void* eventNode = allocFn(allocator, 12, 0);

    if (eventNode == nullptr) {
        return false;
    }

    // Set event node vtable to hsmEvent base
    *(void**)eventNode = (void*)0x82072864;

    // Copy event data fields into the node
    // Set EvtDestroy vtable on node
    *(void**)eventNode = (void*)0x82072968;

    // Copy fields from stack event data to allocated node
    uint32_t field1 = *(uint32_t*)(eventData + 4);
    uint32_t field2 = *(uint32_t*)(eventData + 8);
    *(uint32_t*)((char*)eventNode + 4) = field1;
    *(uint32_t*)((char*)eventNode + 8) = field2;

    // Insert into event queue
    void* eventQueue = (char*)sessionOffset + 8;  // queue at offset+8 from session+212
    snSession_AddNode(eventQueue, eventNode);

    return true;
}

/**
 * snSession::AddEventNode @ 0x823EC068 | size: 0x70
 *
 * Inserts a node into a doubly-linked list used as the session event queue.
 * The list structure has: +0 = head, +4 = tail, +8 = count.
 * Each node has: +4 = prev, +8 = next.
 *
 * If the list is empty, the node becomes both head and tail with null links.
 * If non-empty, the node is inserted after the current tail.
 * In both cases, the list count is incremented.
 */
void snSession_AddEventNode(void* list, void* node) { // @ 0x823EC068
    void* head = *(void**)list;

    if (head != nullptr) {
        // List is non-empty — insert after tail
        void* tail = *(void**)((char*)list + 4);

        // New node's next = tail's current next (should be null for tail)
        void* tailNext = *(void**)((char*)tail + 8);
        *(void**)((char*)node + 8) = tailNext;

        if (tailNext != nullptr) {
            // tailNext.prev = node
            *(void**)((char*)tailNext + 4) = node;
        } else {
            // node becomes the new tail
            *(void**)((char*)list + 4) = node;
        }

        // Link node after old tail
        *(void**)((char*)tail + 8) = node;
        *(void**)((char*)node + 4) = tail;
    } else {
        // List is empty — node becomes head and tail
        *(void**)list = node;
        *(void**)((char*)list + 4) = node;
        *(void**)((char*)node + 4) = nullptr;  // prev = null
        *(void**)((char*)node + 8) = nullptr;  // next = null  (offset stored at node+4+4)
    }

    // Increment count
    int32_t count = *(int32_t*)((char*)list + 8);
    *(int32_t*)((char*)list + 8) = count + 1;
}

/**
 * snSession::ResetHsmState @ 0x823E8EE0 | size: 0x64
 *
 * Resets an hsmState object to its base state. Sets the vtable to the
 * hsmState base vtable, calls the virtual destructor-like function at
 * vtable slot 6, then sets the vtable to hsmStateBase and zeros out all
 * fields (offsets 4, 8, 12, 16, 20).
 */
void snSession_ResetHsmState(void* thisPtr) { // @ 0x823E8EE0
    // Set vtable to hsmState
    *(void**)thisPtr = (void*)0x82073CAC;

    // Call vtable slot 6 (cleanup function) via indirect call
    void** vtable = *(void***)thisPtr;
    void (*cleanupFn)(void*) = (void (*)(void*))vtable[6];
    cleanupFn(thisPtr);

    // Reset to hsmStateBase vtable and zero all fields
    *(void**)thisPtr = (void*)0x82073F1C;
    *(uint32_t*)((char*)thisPtr + 16) = 0;
    *(uint32_t*)((char*)thisPtr + 20) = 0;
    *(uint32_t*)((char*)thisPtr + 4) = 0;
    *(uint32_t*)((char*)thisPtr + 8) = 0;
    *(uint32_t*)((char*)thisPtr + 12) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession Event Dispatch Forwarders — Batch 4 (8 functions, 164B each)
//
// All follow the same pattern:
//   1. Get HSM context via vfn_11, read event queue at context+56
//   2. Allocate a 12-byte event node from the queue's allocator (vfn_1)
//   3. If allocation succeeds, set up hsmEvent base vtable, copy 8 bytes
//      of event data from the caller's event struct (at +4), then set the
//      specific event subclass vtable, and enqueue via snSession_AddEventNode
// ─────────────────────────────────────────────────────────────────────────────

// Event vtable pointers (RTTI-verified)
extern void* g_vtbl_hsmEvent;                     // @ 0x82072864 — rage::hsmEvent base
extern void* g_vtbl_EvtApplyConfigFailed;          // @ 0x82072A64 — rage::EvtApplyConfigFailed
extern void* g_vtbl_EvtStartSessionFailed;         // @ 0x82072A8C — rage::EvtStartSessionFailed
extern void* g_vtbl_EvtSessionJoinFailed;          // @ 0x82072DD0 — rage::EvtSessionJoinFailed
extern void* g_vtbl_EvtJoinSucceeded;              // @ 0x82072CF4 — rage::EvtJoinSucceeded
extern void* g_vtbl_EvtLeaveSucceeded;             // @ 0x820730BC — rage::EvtLeaveSucceeded
extern void* g_vtbl_EvtMigrateSucceeded;           // @ 0x820728C8 — rage::EvtMigrateSucceeded
extern void* g_vtbl_EvtMigrateFailed;              // @ 0x820728DC — rage::EvtMigrateFailed
extern void* g_vtbl_EvtChangePresenceSucceeded;    // @ 0x820728A0 — rage::EvtChangePresenceSucceeded

/**
 * Helper: enqueues a typed HSM event into the session's event queue.
 * All 8 dispatch forwarders below share this exact logic, differing
 * only in the final event vtable written to the allocated node.
 */
static void snSession_EnqueueTypedEvent(void* thisPtr, void* eventData, void* eventVtbl) {
    // Get HSM context via vfn_11
    void** vtable = *(void***)thisPtr;
    void* (*getContext)(void*) = (void* (*)(void*))vtable[11];
    void* context = getContext(thisPtr);

    // Get event queue at context+56
    void* eventQueue = *(void**)((char*)context + 56);

    // Allocate 12-byte event node via queue allocator (vfn_1 at +4)
    void** queueVtbl = *(void***)((char*)eventQueue + 4);
    void* (*allocNode)(void*, uint32_t, uint32_t) =
        (void* (*)(void*, uint32_t, uint32_t))queueVtbl[1];
    void* node = allocNode((char*)eventQueue + 4, 12, 0);

    if (node == nullptr) return;

    // Set hsmEvent base vtable
    *(void**)node = &g_vtbl_hsmEvent;

    // Copy 8 bytes of event payload from eventData+4
    char* src = (char*)eventData + 4;
    char* dst = (char*)node + 4;
    *(uint32_t*)dst = *(uint32_t*)src;
    *(uint32_t*)(dst + 4) = *(uint32_t*)(src + 4);

    // Set specific event subclass vtable
    *(void**)node = eventVtbl;

    // Enqueue into event queue
    snSession_AddEventNode((char*)eventQueue + 8, node);
}

/**
 * snSession::EnqueueEvtApplyConfigFailed @ 0x823E9530 | size: 0xA4
 * Enqueues an EvtApplyConfigFailed event into the session event queue.
 */
void snSession_EnqueueEvtApplyConfigFailed(void* thisPtr, void* eventData) { // @ 0x823E9530
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtApplyConfigFailed);
}

/**
 * snSession::EnqueueEvtStartSessionFailed @ 0x823E95D8 | size: 0xA4
 * Enqueues an EvtStartSessionFailed event into the session event queue.
 */
void snSession_EnqueueEvtStartSessionFailed(void* thisPtr, void* eventData) { // @ 0x823E95D8
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtStartSessionFailed);
}

/**
 * snSession::EnqueueEvtSessionJoinFailed @ 0x823E9680 | size: 0xA4
 * Enqueues an EvtSessionJoinFailed event into the session event queue.
 */
void snSession_EnqueueEvtSessionJoinFailed(void* thisPtr, void* eventData) { // @ 0x823E9680
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtSessionJoinFailed);
}

/**
 * snSession::EnqueueEvtJoinSucceeded @ 0x823E9728 | size: 0xA4
 * Enqueues an EvtJoinSucceeded event into the session event queue.
 */
void snSession_EnqueueEvtJoinSucceeded(void* thisPtr, void* eventData) { // @ 0x823E9728
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtJoinSucceeded);
}

/**
 * snSession::EnqueueEvtLeaveSucceeded @ 0x823E9B18 | size: 0xA4
 * Enqueues an EvtLeaveSucceeded event into the session event queue.
 */
void snSession_EnqueueEvtLeaveSucceeded(void* thisPtr, void* eventData) { // @ 0x823E9B18
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtLeaveSucceeded);
}

/**
 * snSession::EnqueueEvtMigrateSucceeded @ 0x823E9F08 | size: 0xA4
 * Enqueues an EvtMigrateSucceeded event into the session event queue.
 */
void snSession_EnqueueEvtMigrateSucceeded(void* thisPtr, void* eventData) { // @ 0x823E9F08
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtMigrateSucceeded);
}

/**
 * snSession::EnqueueEvtMigrateFailed @ 0x823E9FB0 | size: 0xA4
 * Enqueues an EvtMigrateFailed event into the session event queue.
 */
void snSession_EnqueueEvtMigrateFailed(void* thisPtr, void* eventData) { // @ 0x823E9FB0
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtMigrateFailed);
}

/**
 * snSession::EnqueueEvtChangePresenceSucceeded @ 0x823EAAC8 | size: 0xA4
 * Enqueues an EvtChangePresenceSucceeded event into the session event queue.
 */
void snSession_EnqueueEvtChangePresenceSucceeded(void* thisPtr, void* eventData) { // @ 0x823EAAC8
    snSession_EnqueueTypedEvent(thisPtr, eventData, &g_vtbl_EvtChangePresenceSucceeded);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession Connection List Management — Batch 4 (2 functions)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snSession::InsertConnectionNode @ 0x823EDD10 | size: 0xA4
 *
 * Inserts a connection node into the session's connection list at offset +92.
 * Guards: returns if node is null or if the node's flags byte (at +12)
 * has bit 7 set (already queued). On insertion, clears bit 7 of the flags
 * byte and increments the list count at list+8.
 *
 * The list structure at this+92:
 *   +0: head pointer
 *   +4: tail pointer
 *   +8: count
 *
 * Each node (at r4):
 *   +4: prev pointer (in node-relative terms, node+4 = prev)
 *   +8: next pointer
 *   +12: flags byte (bit 7 = "dequeued" marker)
 */
void snSession_InsertConnectionNode(void* thisPtr, void* node) { // @ 0x823EDD10
    if (node == nullptr) return;

    // Check if bit 7 of flags is clear (meaning node is dequeued)
    uint8_t flags = *(uint8_t*)((char*)node + 12);
    if ((flags & 0x80) == 0) return;

    char* list = (char*)thisPtr + 92;
    void* head = *(void**)list;

    if (head != nullptr) {
        // List is non-empty — insert before the current tail's next
        void* tail = *(void**)(list + 4);
        void* tailNext = *(void**)((char*)tail + 8);

        // Link new node after tail
        *(void**)((char*)node + 8) = tailNext;
        if (tailNext != nullptr) {
            *(void**)((char*)tailNext + 4) = node;
        } else {
            *(void**)(list + 4) = node;
        }
        *(void**)((char*)tail + 8) = node;
        *(void**)((char*)node + 4) = tail;
    } else {
        // List is empty — node becomes both head and tail
        *(void**)list = node;
        *(void**)(list + 4) = node;
        *(void**)((char*)node + 4) = nullptr;  // node+8 = prev
        *(void**)((char*)node + 8) = nullptr;  // node+8 actually... node+4 = next field offset
    }

    // Increment count
    uint32_t count = *(uint32_t*)(list + 8);
    *(uint32_t*)(list + 8) = count + 1;

    // Clear bit 7 of flags (mark as queued)
    flags = *(uint8_t*)((char*)node + 12);
    *(uint8_t*)((char*)node + 12) = flags & 0x7F;
}

/**
 * snSession::RemoveConnectionNode @ 0x823EDDB8 | size: 0xA8
 *
 * Removes a connection node from the session's connection list at offset +92.
 * Guards: returns if node is null or if the node's flags byte (at +12)
 * has bit 7 clear (not queued / already removed). On removal, sets bit 7
 * of the flags byte and decrements the list count at list+8.
 */
void snSession_RemoveConnectionNode(void* thisPtr, void* node) { // @ 0x823EDDB8
    if (node == nullptr) return;

    // Check if bit 7 of flags is set (meaning node is queued)
    uint8_t flags = *(uint8_t*)((char*)node + 12);
    if ((flags & 0x80) != 0) return;

    char* nodeLinks = (char*)node + 4;  // prev/next links start at node+4
    char* list = (char*)thisPtr + 92;

    void* prev = *(void**)nodeLinks;       // node+4 = prev
    void* next = *(void**)(nodeLinks + 4); // node+8 = next

    if (prev == nullptr) {
        // Node is the head — update head to next
        *(void**)list = next;  // list+0 = head
        if (next != nullptr) {
            *(void**)((char*)next + 4) = nullptr;
        } else {
            *(void**)(list + 4) = nullptr;  // list+4 = tail
        }
    } else {
        // Node has a predecessor — unlink
        *(void**)((char*)prev + 8) = next;  // prev->next = node->next
        if (next != nullptr) {
            *(void**)((char*)next + 4) = prev;  // next->prev = node->prev
        } else {
            *(void**)(list + 4) = prev;  // update tail
        }
    }

    // Clear links
    *(void**)nodeLinks = nullptr;
    *(void**)(nodeLinks + 4) = nullptr;

    // Decrement count
    uint32_t count = *(uint32_t*)(list + 8);
    *(uint32_t*)(list + 8) = count - 1;

    // Set bit 7 of flags (mark as dequeued)
    flags = *(uint8_t*)((char*)node + 12);
    *(uint8_t*)((char*)node + 12) = flags | 0x80;
}

} // namespace snSession_States

} // namespace rage
