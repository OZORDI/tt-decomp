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

} // namespace snSession_States

} // namespace rage
