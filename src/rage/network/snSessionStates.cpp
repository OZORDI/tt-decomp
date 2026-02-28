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
 * snSession_snRoot_snChangingPresence::vfn_13 @ 0x823E3040 | size: 0x50
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

} // namespace rage
