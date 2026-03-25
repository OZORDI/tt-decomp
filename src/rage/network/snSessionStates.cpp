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

// External utility functions (from snCreateMachine.cpp)
extern void util_D988(void* context, void* session, void* connectionList);  // @ 0x823ED988
extern void util_DA90(void* context, void* session, void* connectionList);  // @ 0x823EDA90

// ────────────────────────────────────────────────────────────────────────────
// snSession_snRoot_snChangingPresence State Machine
// ────────────────────────────────────────────────────────────────────────────

/**
 * snSession_snRoot_snChangingPresence::GetStateName @ 0x823E3040 | size: 0x50
 *
 * Manages network connections during a session presence change.
 * 
 * This function is part of the snChangingPresence state which handles transitions
 * in session presence (e.g., "in menu" → "in game", "playing" → "paused").
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
    // Save 'this' pointer
    void* session = thisPtr;
    
    // Call vfn_11 on this object - returns a context object used by utility functions
    // This is a virtual call through the vtable at slot 11
    void** vtable = *(void***)thisPtr;
    void* (*vfn_11)(void*) = (void* (*)(void*))vtable[11];
    void* context = vfn_11(thisPtr);
    
    // Get the network client pointer from offset +20 (0x14)
    // This is m_pNetworkClient in the state machine structure
    SinglesNetworkClient* networkClient = *(SinglesNetworkClient**)((char*)session + 20);
    
    // Calculate connection list address
    // The connection list is at offset +5780 (0x1694) from the network client
    void* connectionList = (char*)networkClient + 5780;
    
    // Associate connections with this session during presence change
    // util_D988 @ 0x823ED988 - manages connection association and state updates
    util_D988(context, session, connectionList);
    
    // Process pending connection state changes for the presence transition
    // util_DA90 @ 0x823EDA90 - finalizes connection state and handles cleanup
    util_DA90(context, session, connectionList);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Names (13 GetName functions, 12B each)
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

const char* snRoot_GetName()              { return s_snRoot; }              // vfn_2
const char* snJoining_GetName()           { return s_snJoining; }           // vfn_2
const char* snLeaving_GetName()           { return s_snLeaving; }           // vfn_2
const char* snDormant_GetName()           { return s_snDormant; }           // vfn_2
const char* snCreating_GetName()          { return s_snCreating; }          // vfn_2
const char* snDestroying_GetName()        { return s_snDestroying; }        // vfn_2
const char* snChangingPresence_GetName()  { return s_snChangingPresence; }  // vfn_2
const char* snChanging_GetName()          { return s_snChanging; }          // vfn_2
const char* snWaitingForReplies_GetName() { return s_snWaitingForReplies; } // vfn_2
const char* snActive_GetName()            { return s_snActive; }            // vfn_2
const char* snArbGuestReg_GetName()       { return s_snArbGuestReg; }       // vfn_2
const char* snInProgress_GetName()        { return s_snInProgress; }        // vfn_2
const char* snStarting_GetName()          { return s_snStarting; }          // vfn_2
const char* snArbHostReg_GetName()        { return s_snArbHostReg; }        // vfn_2
const char* snEnding_GetName()            { return s_snEnding; }            // vfn_2

// ─────────────────────────────────────────────────────────────────────────────
// snSession HSM State Utilities
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
 * Walks a linked list following +20 pointers to find the last node.
 */
void* snSession_FindListTail(void* head) {
    char* node = (char*)head;
    while (node) {
        void* next = *(void**)((char*)node + 20);
        if (!next) return node;
        node = (char*)next;
    }
    return nullptr;
}

/**
 * snSession::snDormant_OnEntry @ snDormant_vfn_14 | size: 0x10
 * Clears the field at session+3784 to 0 on entering dormant state.
 */
void snDormant_OnEntry(void* session) {
    *(uint32_t*)((char*)session + 3784) = 0;
}

/**
 * snSession::snJoining_IncrementRef @ snJoining_vfn_14 | size: 0x14
 * Increments the join reference count at session+9672.
 */
void snJoining_IncrementRef(void* session) {
    (*(int32_t*)((char*)session + 9672))++;
}

/**
 * snSession::snJoining_DecrementRef @ snJoining_vfn_15 | size: 0x14
 * Decrements the join reference count at session+9672.
 */
void snJoining_DecrementRef(void* session) {
    (*(int32_t*)((char*)session + 9672))--;
}

} // namespace snSession_States



} // namespace rage
