/**
 * snSessionStates.cpp — Session Network State Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of hierarchical state machines for active session management.
 * Handles presence changes, joining, leaving, and other session state transitions.
 */

#include <stdint.h>

extern "C" void rage_free(void* ptr);  // @ 0x820C00C0 — heap free (C linkage)

namespace rage {

// Forward declarations
class SinglesNetworkClient;

// External utility functions
extern void snSession_AssociateConnections(void* context, void* session, void* connectionList);  // @ 0x823ED988
extern void snSession_ProcessPendingConnections(void* context, void* session, void* connectionList);  // @ 0x823EDA90
extern void hsmState_AttachChild(void* parent, void* childState);                     // @ 0x823EF850 — AttachChildState

// HSM state destructor helpers (each tears down a specific state level)
extern void snSession_4EB0_w(void* thisPtr);   // @ 0x823E4EB0 — snRoot destructor body
extern void snSession_5118_w(void* thisPtr);   // @ 0x823E5118 — snActive destructor body
extern void snSession_9010_gen(void* thisPtr); // @ 0x823E9010 — hsmState base destructor (resets vtable, zeros fields)
extern void snSession_5E20_gen(void* thisPtr); // @ 0x823E5E20 — rlNotifier destructor (cancel pending + base dtor)
extern void util_6178(void* thisPtr);          // @ 0x823E6178 — ArbGuestRegistering-level destructor
extern void util_5C58(void* thisPtr);          // @ 0x823E5C58 — WaitingForReplies-level destructor
extern void util_53A8(void* thisPtr);          // @ 0x823E53A8 — Creating-level destructor (notify handlers)
extern void NotifyHandler_3D80_g(void* notifyList, void* handler); // @ 0x823B3D80 — unregister notification

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
 *   3. Associate connections with the session (snSession_AssociateConnections)
 *   4. Process pending connection state changes (snSession_ProcessPendingConnections)
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
    
    snSession_AssociateConnections(context, session, connectionList);
    snSession_ProcessPendingConnections(context, session, connectionList);
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
 * Detaches the child state at this+24 via hsmState_AttachChild (AttachChildState).
 */
void snIdle_OnExit(void* thisPtr) {
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 24);
}

/**
 * snStarting::OnExit @ 0x823E44E0 | size: 0x3C | vfn_5
 *
 * Exit handler for the Active::Starting state.
 * Detaches two child states at this+24 and this+1412 via hsmState_AttachChild.
 */
void snStarting_OnExit(void* thisPtr) {
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 24);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1412);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Destructors (vfn_0)
// All follow the pattern: call state-level cleanup helper, then conditionally
// free memory if the lowest bit of the flags parameter is set.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snRoot::~snRoot @ 0x823E4F70 | size: 0x50 | vfn_0
 *
 * Destructor for the root session state machine.
 * Calls snSession_4EB0_w to tear down all child states (snChangingPresence,
 * snActive, snCreating, snDormant, etc.), then conditionally frees memory.
 */
void snRoot_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E4F70
    snSession_4EB0_w(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snActive::~snActive @ 0x823E5448 | size: 0x50 | vfn_0
 *
 * Destructor for the Active state. Tears down all Active sub-states
 * (Idle, Starting, InProgress, Ending) via snSession_5118_w.
 */
void snActive_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5448
    snSession_5118_w(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snDormant::~snDormant @ 0x823E5190 | size: 0x50 | vfn_0
 *
 * Destructor for the Dormant state. Calls hsmState base destructor
 * to reset vtable and zero out fields.
 */
void snDormant_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5190
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snCreating::~snCreating @ 0x823E51E0 | size: 0x5C | vfn_0
 *
 * Destructor for the Creating state. Destroys the embedded create machine
 * sub-state at this+24 via util_53A8, then calls hsmState base destructor.
 */
void snCreating_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E51E0
    util_53A8((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snDestroying::~snDestroying @ 0x823E61F0 | size: 0x50 | vfn_0
 *
 * Destructor for the Destroying state. Calls snSession_5E20_gen to cancel
 * any pending rlNotifier callbacks, then resets via hsmState base destructor.
 */
void snDestroying_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E61F0
    snSession_5E20_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snActive::snIdle::~snIdle @ 0x823E5D78 | size: 0x50 | vfn_0
 *
 * Destructor for the Idle sub-state within Active. Destroys the embedded
 * ArbGuestRegistering child at this+48 via util_6178, then tears down
 * the intermediate state at this+24 and this itself via hsmState base dtor.
 */
void snIdle_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5D78
    util_6178((char*)thisPtr + 48);
    snSession_9010_gen((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snActive::snStarting::~snStarting @ 0x823E5DC8 | size: 0x58 | vfn_0
 *
 * Destructor for the Starting sub-state within Active. Destroys the embedded
 * StartingSession notifier at this+1412 via snSession_5E20_gen, destroys the
 * ArbHostRegistering child at this+48 via util_6178, then tears down
 * the intermediate state at this+24 and this itself.
 */
void snStarting_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5DC8
    snSession_5E20_gen((char*)thisPtr + 1412);
    util_6178((char*)thisPtr + 48);
    snSession_9010_gen((char*)thisPtr + 24);
    snSession_9010_gen(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

/**
 * snChangingPresence::snWaitingForReplies::~snWaitingForReplies @ 0x823E5F78 | size: 0x50 | vfn_0
 *
 * Destructor for the WaitingForReplies sub-state. Calls util_5C58 to
 * clean up the embedded connection notification handler, then resets.
 */
void snWaitingForReplies_Destructor(void* thisPtr, uint32_t flags) { // @ 0x823E5F78
    util_5C58(thisPtr);
    if (flags & 1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnEnter Handler (vfn_5)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snActive::OnEnter @ 0x823E48C0 | size: 0x54 | vfn_5
 *
 * Entry handler for the Active state. Attaches four child sub-states
 * at fixed offsets within the Active state structure:
 *   - this+24  (Idle / first child)
 *   - this+1436 (second child)
 *   - this+1460 (third child)
 *   - this+2920 (fourth child)
 * Each is attached via hsmState_AttachChild (AttachChildState) which sets up parent
 * pointers and calls the child's vfn_5 (OnEnter).
 */
void snActive_OnEnter(void* thisPtr) { // @ 0x823E48C0
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 24);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1436);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 1460);
    hsmState_AttachChild(thisPtr, (char*)thisPtr + 2920);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM OnExit Handler (vfn_15)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * snDestroying::OnExit @ 0x823E4728 | size: 0x50 | vfn_15
 *
 * Exit handler for the Destroying state. Decrements the join reference
 * count at session+9672, then unregisters the notification handler at
 * session+240 for the embedded notifier at this+24. Finally clears the
 * pending notification pointer at this+44.
 */
void snDestroying_OnExit(void* thisPtr) { // @ 0x823E4728
    void* session = *(void**)((char*)thisPtr + 16);
    char* notifier = (char*)thisPtr + 24;

    // Decrement join reference count
    (*(int32_t*)((char*)session + 9672))--;

    // Unregister notification handler
    NotifyHandler_3D80_g((char*)session + 240, notifier);

    // Clear pending notification
    *(uint32_t*)(notifier + 20) = 0;
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

} // namespace snSession_States

} // namespace rage
