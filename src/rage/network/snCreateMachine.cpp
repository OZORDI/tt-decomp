/**
 * snCreateMachine.cpp — Session Network Create Machine Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of hierarchical state machine for session creation.
 * Manages the complex flow of creating, configuring, and starting network sessions.
 */

#include "rage/snCreateMachine.hpp"
#include "rage/memory.h"
#include <cstring>

namespace rage {

// Forward declarations for HSM / session utility functions
extern void snSession_AssociateConnection(void* ctx, void* base);       // @ 0x823ED988 — "associating connection:%d with session"
extern void snSession_ProcessPendingConnections(void* ctx, void* base); // @ 0x823EDA90
extern void snHsmState_Init(void* ctx, void* base);                     // @ 0x823DDA08 — init HSM state, set vtables
extern void snSession_AddChildNode(void* ctx, void* base);              // @ 0x823E93D0 — allocates + links child node into session list
extern void snHsmContext_SetMaxTransitions(void* ctx, void* base);      // @ 0x823ED4F8 — sets max transition count + flag
extern void snSession_DestroyState(void* ctx, void* base);              // @ 0x823E53A8 — tears down notify handlers
extern void snLinkedList_InsertNode(void* ctx, void* base);             // @ 0x823DD170 — doubly-linked list insert
extern void snSession_GetSessionHandle(void* ctx, void* base);          // @ 0x82416650 — returns session handle

// External functions
extern void rage_free(void* ptr);                                       // @ 0x820C00C0 — canonical heap free (see src/crt/heap.c)
extern void snSession_QueryConfigStatus(void* ctx, void* base);         // @ 0x823DA940
extern void snSession_RetrieveConfig(void* ctx, void* base);            // @ 0x823DC2B0
extern void snSession_ProcessJoinRequest(void* ctx, void* base);        // @ 0x823F2178
extern void snConnectionRef_InitBroadcast(void* ctx, void* base);       // @ 0x82430978
extern void rage_DebugLogNop(void* ctx, void* base);                    // @ 0x8240E6D0 — 4-byte no-op (stripped debug logging)
extern void snSession_AddNode(void* ctx, void* base);                   // @ 0x823EC068
extern void xamSession_Create(void* ctx, void* base);                   // @ 0x8236D900 — XAM session creation wrapper
extern void xamSession_Modify(void* ctx, void* base);                   // @ 0x8236D758 — XAM session modification wrapper

// Global state type identifiers (from analysis @ 0x825D187C, 0x825D1888, 0x825D1894)
extern uint32_t g_stateType_CreatingHost;
extern uint32_t g_stateType_CreatingGuest;
extern uint32_t g_stateType_CreatingOffline;

// ────────────────────────────────────────────────────────────────────────────
// snCreateMachine Base Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snCreateMachine::~snCreateMachine @ 0x823E5CC0 | size: 0x50
 * 
 * Destructor - cleans up state machine resources.
 * Calls base cleanup and optionally frees memory.
 */
snCreateMachine::~snCreateMachine() {
    // Call base state cleanup
    snSession_DestroyState(this, nullptr);
    
    // Note: Memory freeing handled by caller based on flags
    // If delete flag is set, rage_free is called
}

/**
 * snCreateMachine::GetStateName @ 0x823DDA60 | size: 0xC
 * 
 * Returns the state machine name string.
 */
const char* snCreateMachine::GetStateName() const {
    // Returns address 0x82072160 - "snCreateMachine" string
    return reinterpret_cast<const char*>(0x82072160);
}

/**
 * snCreateMachine::OnUpdate @ 0x823DDA70 | size: 0x3C
 * 
 * Called each frame to update the state machine.
 * Initializes state context and clears session data.
 */
void snCreateMachine::OnUpdate() {
    // Clear state context pointers
    m_pCurrentHsmState = nullptr;
    m_pStateVtable = nullptr;
    
    // Clear session data
    m_sessionId = 0;
    m_sessionHandle = 0;
    m_field_120 = 0;
    m_retryCount = 0;
    
    // Clear flags (preserve upper bits)
    m_flags = m_flags & 0x7F;
    
    // Initialize state context with network client
    void* stateContext = m_pStateContext;
    if (m_pNetworkClient) {
        // Call network client initialization
        snLinkedList_InsertNode(this, nullptr);
    }
}

/**
 * snCreateMachine::OnEnter @ 0x823DDAB0 | size: 0x194
 * 
 * Called when entering the create machine state.
 * Determines which sub-state to transition to based on current session state.
 */
void snCreateMachine::OnEnter() {
    // Get current state from state machine
    hsmState* currentState = GetCurrentState();
    if (!currentState) return;
    
    // Get session from current state
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );
    if (!session) return;
    
    // Get state type identifier
    uint32_t stateType = 0;
    // Virtual call to get state type (slot 1)
    // stateType = session->GetStateType();
    
    // Check if we're creating as host
    if (stateType == g_stateType_CreatingHost) {
        hsmState* hostState = GetStateByType(g_stateType_CreatingHost);
        if (hostState) {
            // Transition to CreatingHost state at offset +84
            void* hostStateData = reinterpret_cast<void*>(
                reinterpret_cast<uint8_t*>(this) + 84
            );
            snSession_AssociateConnection(this, hostStateData);
            snSession_ProcessPendingConnections(this, hostStateData);
        }
        return;
    }
    
    // Check if we're creating as guest
    if (stateType == g_stateType_CreatingGuest) {
        hsmState* guestState = GetStateByType(g_stateType_CreatingGuest);
        if (guestState) {
            // Transition to CreatingGuest state at offset +108
            void* guestStateData = reinterpret_cast<void*>(
                reinterpret_cast<uint8_t*>(this) + 108
            );
            snSession_AssociateConnection(this, guestStateData);
            snSession_ProcessPendingConnections(this, guestStateData);
        }
        return;
    }
    
    // Check if we're creating offline
    if (stateType == g_stateType_CreatingOffline) {
        hsmState* offlineState = GetStateByType(g_stateType_CreatingOffline);
        if (offlineState) {
            // Transition to CreatingOffline state at offset +132
            void* offlineStateData = reinterpret_cast<void*>(
                reinterpret_cast<uint8_t*>(this) + 132
            );
            snSession_AssociateConnection(this, offlineStateData);
            snSession_ProcessPendingConnections(this, offlineStateData);
        }
        return;
    }
    
    // Unknown state type - create error notification
    snHsmState_Init(this, nullptr);
    snSession_AddChildNode(this, nullptr);
}

/**
 * snCreateMachine::InitializeSession @ 0x823DEBC0 | size: 0x78
 * 
 * Initializes session resources.
 */
void snCreateMachine::InitializeSession() {
    // TODO: Implementation
}

/**
 * snCreateMachine::UpdateSessionState @ 0x823DEC38 | size: 0x130
 * 
 * Updates session state based on current conditions.
 */
void snCreateMachine::UpdateSessionState() {
    // TODO: Implementation
}

/**
 * snCreateMachine::HandleStateTransition @ 0x823F27E8 | size: 0x40
 * 
 * Handles transitions between states.
 */
void snCreateMachine::HandleStateTransition() {
    // TODO: Implementation
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmCreatingHost Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmCreatingHost::~snHsmCreatingHost @ 0x823E5FC8 | size: 0x50
 * 
 * Destructor for host creation state.
 */
snHsmCreatingHost::~snHsmCreatingHost() {
    // Cleanup handled by base class
}

/**
 * snHsmCreatingHost::GetStateName @ 0x823DDC48 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmCreatingHost::GetStateName() const {
    // TODO: Return actual state name string
    return "snHsmCreatingHost";
}

/**
 * snHsmCreatingHost::OnUpdate @ 0x823DDC58 | size: 0x100
 * 
 * Updates host creation state.
 * Creates session as host and initiates network setup.
 */
void snHsmCreatingHost::OnUpdate() {
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(m_pParentState);
    if (!machine) return;
    
    // Get network client
    SinglesNetworkClient* client = machine->m_pNetworkClient;
    if (!client) return;
    
    // Create session parameters
    // Address 0x825E51F8 - host session config string
    const char* hostConfig = reinterpret_cast<const char*>(0x825E51F8);
    // Address 0x825E5448 - session type string
    const char* sessionType = reinterpret_cast<const char*>(0x825E5448);
    
    // Call network client to create session
    // SinglesNetworkClient_0978_g creates the session
    
    // Get session handle
    uint64_t sessionHandle = snSession_GetSessionHandle(nullptr, nullptr);
    
    // Store session data in machine
    // machine->m_sessionHandle = sessionHandle;
    
    // Set up state context
    // Address -20288 from base -32193 = state vtable
    void* stateVtable = reinterpret_cast<void*>(0x82031B10);
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = stateVtable;
}

/**
 * snHsmCreatingHost::OnExit @ 0x823DDD58 | size: 0x14
 * 
 * Called when exiting host creation state.
 */
void snHsmCreatingHost::OnExit() {
    // Cleanup handled by base
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmCreatingGuest Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmCreatingGuest::GetStateName @ 0x823DDDF8 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmCreatingGuest::GetStateName() const {
    return "snHsmCreatingGuest";
}

/**
 * snHsmCreatingGuest::OnUpdate @ 0x823DDE08 | size: 0x100
 * 
 * Updates guest creation state.
 * Joins existing session as guest.
 */
void snHsmCreatingGuest::OnUpdate() {
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(m_pParentState);
    if (!machine) return;
    
    // Get network client
    SinglesNetworkClient* client = machine->m_pNetworkClient;
    if (!client) return;
    
    // Guest session config at 0x825E51D4
    const char* guestConfig = reinterpret_cast<const char*>(0x825E51D4);
    const char* sessionType = reinterpret_cast<const char*>(0x825E5448);
    
    // Create session as guest
    // SinglesNetworkClient_0978_g handles guest join
    
    // Clear session handle (guest doesn't create)
    machine->m_sessionHandle = 0;
    
    // Set up state context
    void* stateVtable = reinterpret_cast<void*>(0x82031B08);
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = stateVtable;
}

/**
 * snHsmCreatingGuest::OnTick @ 0x823DDFF0 | size: 0xA4
 * 
 * Tick update for guest state.
 */
void snHsmCreatingGuest::OnTick() {
    // TODO: Implementation
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmCreatingOffline Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmCreatingOffline::GetStateName @ 0x823DE098 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmCreatingOffline::GetStateName() const {
    return "snHsmCreatingOffline";
}

/**
 * snHsmCreatingOffline::OnUpdate @ 0x823DE0A8 | size: 0xF4
 * 
 * Updates offline creation state.
 * Creates local-only session without network.
 */
void snHsmCreatingOffline::OnUpdate() {
    // Offline sessions don't need network setup
    // Just initialize local session state
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmRequestingConfig Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
/**
 * snHsmRequestingConfig::~snHsmRequestingConfig @ 0x823E6018 | size: 0x50
 * 
 * Destructor for config request state.
 * Calls base cleanup and optionally frees memory.
 */
snHsmRequestingConfig::~snHsmRequestingConfig() {
    // Call base state cleanup (NotifyHandler_5D10)
    // This handles cleanup of notification handlers and state resources
    
    // Note: Memory freeing is conditional based on flags
    // If bit 0 of the flags parameter is set, rage_free is called
    // This is handled by the caller passing the appropriate flags
}

/**
 * snHsmRequestingConfig::GetStateName @ 0x823DE228 | size: 0xC
 * 
 * Returns the state name string for debugging and logging.
 */
const char* snHsmRequestingConfig::GetStateName() const {
    // Returns string at 0x820721A0
    return "snHsmRequestingConfig";
}

/**
 * snHsmRequestingConfig::OnTick @ 0x823DE450 | size: 0x18C
 * 
 * Tick update for config request state.
 * Handles state transitions based on session type and configuration status.
 * 
 * This function checks the current session state type and transitions to
 * the appropriate next state (host, guest, or offline creation).
 */
void snHsmRequestingConfig::OnTick(bool* outTransitioned) {
    // Set initial transition flag
    *outTransitioned = true;
    
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(GetParentState());
    if (!machine) {
        *outTransitioned = false;
        return;
    }
    
    // Get current session from state machine
    snSession* session = machine->GetCurrentSession();
    if (!session) {
        *outTransitioned = false;
        return;
    }
    
    // Get session state type
    uint32_t stateType = session->GetStateType();
    
    // Check if we're creating as host (state type at 0x825D1888)
    extern uint32_t g_stateType_CreatingHost;  // @ 0x825D1888
    if (stateType == g_stateType_CreatingHost) {
        // Transition to host creation state
        hsmState* hostState = machine->GetStateByType(g_stateType_CreatingHost);
        if (hostState) {
            // Get state data at offset +204 from network client
            SinglesNetworkClient* client = machine->m_pNetworkClient;
            void* stateData = reinterpret_cast<uint8_t*>(client) + 204;
            
            // Associate connection and process pending connections
            machine->AssociateConnection(this, stateData);
            machine->ProcessPendingConnections(this, stateData);
        }
        return;
    }
    
    // Check if we're creating as guest (state type at 0x825D1894)
    extern uint32_t g_stateType_CreatingGuest;  // @ 0x825D1894
    if (stateType == g_stateType_CreatingGuest) {
        // Transition to guest creation state
        hsmState* guestState = machine->GetStateByType(g_stateType_CreatingGuest);
        if (guestState) {
            // Get state data at offset +252 from network client
            SinglesNetworkClient* client = machine->m_pNetworkClient;
            void* stateData = reinterpret_cast<uint8_t*>(client) + 252;
            
            // Associate connection and process pending connections
            machine->AssociateConnection(this, stateData);
            machine->ProcessPendingConnections(this, stateData);
        }
        return;
    }
    
    // Check if we're creating offline (state type at 0x825D187C)
    extern uint32_t g_stateType_CreatingOffline;  // @ 0x825D187C
    if (stateType == 0) {  // Offline has state type 0
        // Transition to offline creation state
        hsmState* offlineState = machine->GetStateByType(g_stateType_CreatingOffline);
        if (offlineState) {
            // Get state data at offset +156 from network client
            SinglesNetworkClient* client = machine->m_pNetworkClient;
            void* stateData = reinterpret_cast<uint8_t*>(client) + 156;
            
            // Associate connection and process pending connections
            machine->AssociateConnection(this, stateData);
            machine->ProcessPendingConnections(this, stateData);
        }
        return;
    }
    
    // Unknown state type - no transition
    *outTransitioned = false;
}

/**
 * snHsmRequestingConfig::OnUpdate @ 0x823DE238 | size: 0x1CC
 * 
 * Updates config request state.
 * Polls for configuration availability and handles timeouts.
 * 
 * This function repeatedly checks if the session configuration is ready,
 * and either transitions to the next state or handles timeout errors.
 */
void snHsmRequestingConfig::OnUpdate() {
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(m_pParentState);
    if (!machine) return;
    
    // Get network client
    SinglesNetworkClient* client = machine->m_pNetworkClient;
    if (!client) return;
    
    // Set up state context in parent machine
    // Vtable at 0x82031AF8 (calculated from -32193 + -20264)
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = reinterpret_cast<void*>(0x82031AF8);
    
    // Store this state in the state context at offset +28
    *reinterpret_cast<snHsmRequestingConfig**>(
        reinterpret_cast<uint8_t*>(this) + 28
    ) = this;
    
    // Check if configuration is available
    // Query config status from network client
    bool configReady = client->QueryConfigStatus();
    
    // If config is ready, retrieve it
    if (configReady) {
        void* configData = client->RetrieveConfig();
        if (configData) {
            // Config successfully retrieved
            int32_t configStatus = *reinterpret_cast<int32_t*>(
                reinterpret_cast<uint8_t*>(configData) + 84
            );
            
            if (configStatus >= 0) {
                // Config is valid - transition to next state
                return;
            }
        }
    }
    
    // Check if we should process join requests
    bool shouldProcessJoin = client->ProcessJoinRequest();
    
    if (!configReady && !shouldProcessJoin) {
        // Still waiting - check retry count for timeout
        if (machine->m_retryCount >= 20) {
            // Timeout after 20 retries - create error event
            CreateConfigFailedEvent();
            return;
        } else if (machine->m_retryCount >= 10) {
            // Timeout after 10 retries - create error event
            CreateConfigFailedEvent();
            return;
        }
    }
    
    // Not ready yet - increment retry count and set timeout
    machine->m_retryCount++;
    machine->SetMaxTransitions(500);  // 500ms timeout
}

/**
 * snHsmRequestingConfig::OnExit @ 0x823DE408 | size: 0x48
 * 
 * Called when exiting config request state.
 * Cleans up notification handlers and resets state.
 */
void snHsmRequestingConfig::OnExit() {
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(m_pParentState);
    if (!machine) return;
    
    // Get network client
    SinglesNetworkClient* client = machine->m_pNetworkClient;
    if (!client) return;
    
    // Get notification handler at offset +164 from client, then +672
    void* notifyHandler = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(client) + 164
        ) + 672
    );
    
    // Clear notification handler state
    // Call NotifyHandler_3D80_g to unregister this state's notifications
    
    // Clear the state context pointer at offset +44 (20 + 24)
    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 44
    ) = 0;
}

// Helper function to create config failed event
void snHsmRequestingConfig::CreateConfigFailedEvent() {
    // Initialize event state
    util_DA08(this, nullptr);
    
    // Create EvtRequestConfigFailed event
    // Vtable at 0x82072A50
    rage::EvtRequestConfigFailed* event = new rage::EvtRequestConfigFailed();
    
    // Get parent state machine
    snCreateMachine* machine = static_cast<snCreateMachine*>(m_pParentState);
    if (!machine) return;
    
    // Get session from machine
    snSession* session = machine->GetCurrentSession();
    if (!session) return;
    
    // Add event to session's event queue
    session->AddEvent(event);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmApplyingConfig Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmApplyingConfig::GetStateName @ 0x823DE5E0 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmApplyingConfig::GetStateName() const {
    return "snHsmApplyingConfig";
}

/**
 * snHsmApplyingConfig::OnUpdate @ 0x823DE5F0 | size: 0xD4
 * 
 * Updates config application state.
 * Applies received configuration to local session.
 */
void snHsmApplyingConfig::OnUpdate() {
    // TODO: Implementation
}

/**
 * snHsmApplyingConfig::OnTick @ 0x823DE7E8 | size: 0x11C
 * 
 * Tick update for config application state.
 */
void snHsmApplyingConfig::OnTick() {
    // TODO: Implementation
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmStartingSession Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmStartingSession::GetStateName @ 0x823DE908 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmStartingSession::GetStateName() const {
    return "snHsmStartingSession";
}

/**
 * snHsmStartingSession::OnUpdate @ 0x823DE918 | size: 0x78
 * 
 * Updates session start state.
 * Finalizes session setup and transitions to active state.
 */
void snHsmStartingSession::OnUpdate() {
    // TODO: Implementation
}

/**
 * snHsmStartingSession::OnTick @ 0x823DEA18 | size: 0xA4
 * 
 * Tick update for session start state.
 */
void snHsmStartingSession::OnTick() {
    // TODO: Implementation
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmDestroyingSession Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmDestroyingSession::GetStateName @ 0x823DEAC0 | size: 0xC
 * 
 * Returns state name string.
 */
const char* snHsmDestroyingSession::GetStateName() const {
    return "snHsmDestroyingSession";
}

/**
 * snHsmDestroyingSession::OnUpdate @ 0x823DEAD0 | size: 0x78
 * 
 * Updates session destruction state.
 * Handles graceful shutdown and resource cleanup.
 */
void snHsmDestroyingSession::OnUpdate() {
    // TODO: Implementation
}

} // namespace rage
