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
extern void snSession_InitChildState(void* machine, void* childState);  // @ 0x823EF850 — init child state with parent context
extern void NotifyHandler_3D80_g(void* notifyList, void* callback);     // @ 0x823B3D80 — unregister notification handler
extern bool xam_E6C0_g(void* xamContext);                               // @ 0x8236E6C0 — XAM session start
extern bool xam_EAB8_g(void* xamContext);                               // @ 0x8236EAB8 — XAM session delete
extern bool xam_DD68_g(void* xamContext, void* session);                // @ 0x8236DD68 — XAM session register
extern void snSession_95D8_fw(void* state, void* event);                // @ 0x823E95D8 — forward event to session (start variant)
extern void snSession_9530_fw(void* state, void* event);                // @ 0x823E9530 — forward event to session (apply variant)

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

// Global state type identifiers
extern uint32_t g_stateType_CreatingHost;       // @ 0x825D1888
extern uint32_t g_stateType_CreatingGuest;      // @ 0x825D1894
extern uint32_t g_stateType_CreatingOffline;    // @ 0x825D187C
extern uint32_t g_stateType_StartingSession;    // @ 0x825D1AA4
extern uint32_t g_stateType_DestroyingSession;  // @ 0x825D1A8C

// Session configuration data blocks
extern uint8_t g_offlineSessionConfig[];        // @ 0x825E51B0 — offline session config (36 bytes)
extern uint8_t g_sessionType[];                 // @ 0x825E5448 — session type descriptor (28 bytes)
extern uint8_t g_applyConfigData[];             // @ 0x825E57FC — apply config data (40 bytes)

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
    // @ 0x82074160 — "CreateMachine"
    extern const char g_str_snCreateMachine[];
    return g_str_snCreateMachine;
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
 * snCreateMachine::InitializeChildStates @ 0x823DEBC0 | size: 0x78
 *
 * Initializes all child HSM states by linking them to this machine's
 * session context and network client. Each child state at a fixed offset
 * is registered via snSession_InitChildState.
 */
void snCreateMachine::InitializeChildStates() {
    // Offsets for all 7 child HSM states within snCreateMachine
    // CreatingHost(+84), CreatingGuest(+108), CreatingOffline(+132),
    // RequestingConfig(+156), ApplyingConfig(+204), StartingSession(+228),
    // DestroyingSession(+252)
    static const int kChildStateOffsets[] = { 84, 108, 132, 156, 204, 228, 252 };

    for (int i = 0; i < 7; i++) {
        hsmState* childState = reinterpret_cast<hsmState*>(
            reinterpret_cast<uint8_t*>(this) + kChildStateOffsets[i]
        );
        snSession_InitChildState(this, childState);
    }
}

/**
 * snCreateMachine::ShutdownChildStates @ 0x823DEC38 | size: 0x130
 *
 * Shuts down all child HSM states. For each child state that has a
 * non-null parent reference (offset +8), clears the parent and calls
 * the virtual shutdown method (vtable slot 4).
 */
void snCreateMachine::ShutdownChildStates() {
    // Same 7 child state offsets as InitializeChildStates
    static const int kChildStateOffsets[] = { 84, 108, 132, 156, 204, 228, 252 };

    for (int i = 0; i < 7; i++) {
        hsmState* childState = reinterpret_cast<hsmState*>(
            reinterpret_cast<uint8_t*>(this) + kChildStateOffsets[i]
        );
        // Check if child has a parent reference (offset +8 in the child state)
        hsmState* parentRef = *reinterpret_cast<hsmState**>(
            reinterpret_cast<uint8_t*>(childState) + 8
        );
        if (parentRef != nullptr) {
            // Clear the parent reference
            *reinterpret_cast<hsmState**>(
                reinterpret_cast<uint8_t*>(childState) + 8
            ) = nullptr;
            // Call virtual shutdown (slot 4)
            childState->Shutdown();
        }
    }
}

/**
 * snCreateMachine::UnregisterNotifyHandler @ 0x823F27E8 | size: 0x40
 *
 * Unregisters the notification handler from the network client's
 * notify list, then clears the local callback pointer.
 */
void snCreateMachine::UnregisterNotifyHandler() {
    // Get the network client from the parent machine context
    SinglesNetworkClient* client = m_pNetworkClient;
    // Access the notify handler list at client+240
    void* notifyList = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(client) + 240
    );
    // The notification callback is at this+24
    void* notifyCallback = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(this) + 24
    );
    NotifyHandler_3D80_g(notifyList, notifyCallback);
    // Clear the callback data pointer at this+44 (24+20)
    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 44
    ) = 0;
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
    // @ 0x82074170 — "CreatingHost"
    extern const char g_str_snHsmCreatingHost[];
    return g_str_snHsmCreatingHost;
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
 * Called when exiting the host creation state.
 * Clears the parent machine's current state context pointers.
 */
void snHsmCreatingHost::OnExit() {
    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );
    machine->m_pCurrentHsmState = nullptr;
    machine->m_pStateVtable = nullptr;
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
    // @ 0x82074180 — "CreatingGuest"
    extern const char g_str_snHsmCreatingGuest[];
    return g_str_snHsmCreatingGuest;
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
 * Tick handler for guest creation state.
 * Checks if the current session state type matches CreatingOffline (type at 0x825D1A74).
 * If so, transitions to the offline creation sub-state at parent+156.
 * Otherwise, sets the handled flag to false.
 */
void snHsmCreatingGuest::OnTick(bool* handled) {
    *handled = true;

    // Get current state via virtual call (slot 10)
    hsmState* currentState = GetCurrentState();
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );

    // Get session state type via virtual call (slot 1)
    uint32_t stateType = *reinterpret_cast<uint32_t*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(session)) + 4
    );

    // Compare with CreatingOffline state type @ 0x825D1A74
    if (stateType == g_stateType_CreatingOffline) {
        // Get parent session and transition to the offline state
        hsmState* nextState = GetStateByType(g_stateType_CreatingOffline);
        snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
        );
        // Offline sub-state is at parent+156
        void* offlineState = reinterpret_cast<void*>(
            reinterpret_cast<uint8_t*>(machine) + 156
        );
        snSession_AssociateConnection(nextState, offlineState);
        snSession_ProcessPendingConnections(this, offlineState);
        return;
    }

    *handled = false;
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
    // @ 0x82074190 — "CreatingOffline"
    extern const char g_str_snHsmCreatingOffline[];
    return g_str_snHsmCreatingOffline;
}

/**
 * snHsmCreatingOffline::OnUpdate @ 0x823DE0A8 | size: 0xF4
 *
 * Creates an offline session by calling the connection reference initializer
 * twice — once for the parent machine and once for the parent's own parent.
 * Stores session ID and handle, then attempts XAM session registration.
 * On failure, raises an EvtCreateFailed event.
 * Finally, sets the parent machine's state context to point to this state.
 */
void snHsmCreatingOffline::OnUpdate() {
    // Get current state to access session
    hsmState* currentState = GetCurrentState();
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );

    // Initialize connection for offline session
    snConnectionRef_InitBroadcast(session, g_sessionType, g_offlineSessionConfig, 0);

    // Store session ID from result at offset +12
    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );
    uint32_t sessionId = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(session) + 12
    );
    machine->m_sessionId = sessionId;

    // Get session handle
    uint64_t sessionHandle = 0;
    snSession_GetSessionHandle(&sessionHandle, nullptr);
    machine->m_sessionHandle = sessionHandle;

    // Get parent machine's parent and do second init
    snCreateMachine* parentMachine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(machine) + 20)
    );
    hsmState* parentCurrentState = parentMachine->GetCurrentState();
    void* parentSession = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(parentCurrentState) + 12)
    );
    snConnectionRef_InitBroadcast(parentSession, g_sessionType, g_offlineSessionConfig, 0);

    // Register with XAM session subsystem
    void* networkClient = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(parentMachine) + 16)
    );
    void* xamContext = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(networkClient) + 232
    );
    bool success = xam_DD68_g(xamContext, parentSession);
    if (!success) {
        // Raise EvtCreateFailed event
        hsmEvent failEvent;
        snHsmState_Init(&failEvent, nullptr);
        // Set vtable to EvtCreateFailed @ 0x82072A28
        failEvent.vtable = reinterpret_cast<void**>(0x82072A28);
        snSession_AddChildNode(this, &failEvent);
    }

    // Set parent machine state context to point to this state
    // thunk_fn_823DE1A0 @ 0x823EB0D0
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = reinterpret_cast<void*>(0x823EB0D0);
}

// ────────────────────────────────────────────────────────────────────────────
// snHsmRequestingConfig Implementation
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// snHsmRequestingConfig Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * snHsmRequestingConfig::~snHsmRequestingConfig @ 0x823E6018 | size: 0x50
 * 
 * Destructor for config request state.
 * Calls base cleanup and optionally frees memory based on flags.
 */
snHsmRequestingConfig::~snHsmRequestingConfig() {
    // Call base state cleanup (NotifyHandler_5D10 @ 0x823E5D10)
    // This handles cleanup of notification handlers and state resources
    // TODO: Implement when NotifyHandler_5D10 is available
}

/**
 * snHsmRequestingConfig::GetStateName @ 0x823DE228 | size: 0xC
 * 
 * Returns the state name string for debugging and logging.
 */
const char* snHsmRequestingConfig::GetStateName() const {
    // @ 0x820741A0 (corrected) — "RequestingConfig"
    extern const char g_str_snHsmRequestingConfig[];
    return g_str_snHsmRequestingConfig;
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
void snHsmRequestingConfig::OnTick() {
    // TODO: Full implementation
    // 
    // Assembly flow:
    // 1. Set transition flag to true (stb r11,0(r30))
    // 2. Get parent state machine (this pointer in r31)
    // 3. Call vfn_10 to get current state (VCALL slot 10, byte offset +40)
    // 4. Get session from state at offset +12
    // 5. Call session->GetStateType() (vfn_1, byte offset +4)
    // 6. Load state type globals from .data section:
    //    - g_stateType_CreatingHost @ 0x825D1888 (offset 6808 from base -32163)
    //    - g_stateType_CreatingGuest @ 0x825D1894 (offset 6784 from base -32163)
    //    - g_stateType_CreatingOffline @ 0x825D187C
    // 7. Compare state type and transition to appropriate state:
    //    - If CreatingHost: call vfn_11, get state data at +204, call snSession_AssociateConnections and snSession_ProcessPendingConnections
    //    - If CreatingGuest: call vfn_11, get state data at +252, call snSession_AssociateConnections and snSession_ProcessPendingConnections
    //    - If offline (type 0): call vfn_11, get state data at +156, call snSession_AssociateConnections and snSession_ProcessPendingConnections
    //    - Otherwise: set transition flag to false
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
    // TODO: Full implementation
    //
    // Assembly flow:
    // 1. Get parent state machine from offset +20
    // 2. Store state context pointers at offsets +28 and +32
    //    - Vtable pointer at 0x82031AF8 (calculated from lis -32193, addi -20264)
    // 3. Get network client from parent at offset +20, then +16
    // 4. Query config status: SinglesNetworkClient_A940_g @ 0x823DA940
    // 5. Retrieve config: SinglesNetworkClient_C2B0_g @ 0x823DC2B0
    // 6. Check config availability:
    //    - Load config data at offset +84, check if >= 0
    //    - If ready, transition to next state
    // 7. Check retry count at offset +296:
    //    - If >= 20: timeout, create error event
    //    - If >= 10: timeout, create error event
    // 8. Process join requests: SinglesNetworkClient_2178_g @ 0x823F2178
    // 9. If timeout, create EvtRequestConfigFailed event:
    //    - Call util_DA08 @ 0x823DDA08 to initialize event state
    //    - Set vtable to 0x82072A50 (rage::EvtRequestConfigFailed)
    //    - Call vfn_11 to get session
    //    - Allocate event with vfn_1 (slot 1, size 12)
    //    - Store event data and call snSession_AddNode_C068 @ 0x823EC068
    // 10. Otherwise, increment retry count and call util_D4F8 @ 0x823ED4F8 with timeout 500
}

/**
 * snHsmRequestingConfig::OnExit @ 0x823DE408 | size: 0x48
 * 
 * Called when exiting config request state.
 * Cleans up notification handlers and resets state.
 */
void snHsmRequestingConfig::OnExit() {
    // TODO: Full implementation
    //
    // Assembly flow:
    // 1. Get parent state machine from offset +20
    // 2. Get network client from parent at offset +20, then +16
    // 3. Get notification handler at offset +164 from client, then +672
    // 4. Call NotifyHandler_3D80_g @ 0x823B3D80 to unregister notifications
    //    - Pass state context at offset +24 (this + 24)
    // 5. Clear state context pointer at offset +44 (24 + 20)
    //    - Store 0 at that location
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
    // @ 0x820741E4 — "ApplyingConfig"
    extern const char g_str_snHsmApplyingConfig[];
    return g_str_snHsmApplyingConfig;
}

/**
 * snHsmApplyingConfig::OnUpdate @ 0x823DE5F0 | size: 0xD4
 *
 * Applies session configuration by initializing the connection reference
 * with session config data, storing the session handle, updating session
 * flags, and calling the XAM session modification wrapper.
 * On failure, raises an EvtApplyConfigFailed event.
 * Sets the parent machine's state context to ApplyingConfig thunk.
 */
void snHsmApplyingConfig::OnUpdate() {
    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );

    // Get current state and session from parent machine
    hsmState* currentState = machine->GetCurrentState();
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );

    // Initialize connection with session config data
    snConnectionRef_InitBroadcast(session, g_sessionType, g_applyConfigData, 0);

    // Store session handle from result
    uint64_t sessionHandle = *reinterpret_cast<uint64_t*>(
        reinterpret_cast<uint8_t*>(session) + 24
    );
    snCreateMachine* parentMachine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(machine) + 20)
    );
    parentMachine->m_sessionHandle = sessionHandle;

    // Update session flags with public/private bit from config
    uint8_t configFlag = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uint8_t*>(session) + 312
    );
    uint8_t currentFlags = parentMachine->m_flags;
    // Insert configFlag bit 0 into flags bit 7 (rlwimi r6,r7,7,0,24)
    parentMachine->m_flags = (currentFlags & 0x7F) | ((configFlag & 1) << 7);

    // Call XAM session modification
    void* networkClient = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(parentMachine) + 16)
    );
    void* xamContext = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(networkClient) + 232
    );
    bool success = xamSession_Modify(xamContext, session);
    if (!success) {
        // Raise EvtApplyConfigFailed event
        hsmEvent failEvent;
        snHsmState_Init(&failEvent, nullptr);
        failEvent.vtable = reinterpret_cast<void**>(0x82072A64);  // EvtApplyConfigFailed
        snSession_9530_fw(this, &failEvent);
    }

    // Set parent machine state context
    // thunk_fn_823DE6C8 @ 0x823EB0E0
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = reinterpret_cast<void*>(0x823EB0E0);
}

/**
 * snHsmApplyingConfig::OnTick @ 0x823DE7E8 | size: 0x11C
 *
 * Tick handler for the ApplyingConfig state.
 * Checks the session state type:
 *   - If StartingSession type (0x825D1AA4): transitions to StartingSession sub-state at parent+228.
 *   - If DestroyingSession type (0x825D1A8C): transitions to DestroyingSession sub-state at parent+252.
 *   - Otherwise: sets handled flag to false.
 */
void snHsmApplyingConfig::OnTick(bool* handled) {
    *handled = true;

    // Get current state and session type
    hsmState* currentState = GetCurrentState();
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );
    uint32_t stateType = session->GetStateType();

    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );

    // Check for StartingSession state type @ 0x825D1AA4
    if (stateType == g_stateType_StartingSession) {
        hsmState* nextState = GetStateByType(g_stateType_StartingSession);
        // StartingSession sub-state at parent+228
        void* startingState = reinterpret_cast<void*>(
            reinterpret_cast<uint8_t*>(machine) + 228
        );
        snSession_AssociateConnection(nextState, startingState);
        snSession_ProcessPendingConnections(this, startingState);
        return;
    }

    // Check for DestroyingSession state type @ 0x825D1A8C
    // Re-query state type (same pattern as scaffold - second VCALL)
    currentState = GetCurrentState();
    session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );
    stateType = session->GetStateType();

    if (stateType == g_stateType_DestroyingSession) {
        hsmState* nextState = GetStateByType(g_stateType_DestroyingSession);
        // DestroyingSession sub-state at parent+252
        void* destroyingState = reinterpret_cast<void*>(
            reinterpret_cast<uint8_t*>(machine) + 252
        );
        snSession_AssociateConnection(nextState, destroyingState);
        snSession_ProcessPendingConnections(this, destroyingState);
        return;
    }

    *handled = false;
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
    // @ 0x820741F4 — "StartingSession"
    extern const char g_str_snHsmStartingSession[];
    return g_str_snHsmStartingSession;
}

/**
 * snHsmStartingSession::OnUpdate @ 0x823DE918 | size: 0x78
 *
 * Finalizes session startup by calling the XAM session start function.
 * On failure, raises an EvtStartSessionFailed event.
 * Sets the parent machine's state context to the StartingSession thunk.
 */
void snHsmStartingSession::OnUpdate() {
    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );

    // Get network client's XAM context at offset +232
    void* networkClient = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(machine) + 16)
    );
    void* xamContext = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(networkClient) + 232
    );

    // Call XAM session start
    bool success = xam_E6C0_g(xamContext);
    if (!success) {
        // Raise EvtStartSessionFailed event
        hsmEvent failEvent;
        snHsmState_Init(&failEvent, nullptr);
        failEvent.vtable = reinterpret_cast<void**>(0x82072A8C);  // EvtStartSessionFailed
        snSession_95D8_fw(this, &failEvent);
    }

    // Set parent machine state context
    // thunk_fn_823DE990 @ 0x823EB0E8
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = reinterpret_cast<void*>(0x823EB0E8);
}

/**
 * snHsmStartingSession::OnTick @ 0x823DEA18 | size: 0xA4
 *
 * Tick handler for StartingSession state.
 * Checks if the session state type matches DestroyingSession (type at 0x825D1AB0).
 * If so, transitions to the DestroyingSession sub-state at parent+252.
 * Otherwise, sets handled to false.
 */
void snHsmStartingSession::OnTick(bool* handled) {
    *handled = true;

    // Get current state and session type
    hsmState* currentState = GetCurrentState();
    void* session = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(currentState) + 12)
    );
    uint32_t stateType = session->GetStateType();

    // Compare with DestroyingSession state type @ 0x825D1AB0
    if (stateType == g_stateType_DestroyingSession) {
        hsmState* nextState = GetStateByType(g_stateType_DestroyingSession);
        snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
        );
        // DestroyingSession sub-state at parent+252
        void* destroyingState = reinterpret_cast<void*>(
            reinterpret_cast<uint8_t*>(machine) + 252
        );
        snSession_AssociateConnection(nextState, destroyingState);
        snSession_ProcessPendingConnections(this, destroyingState);
        return;
    }

    *handled = false;
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
    // @ 0x82074204 — "DestroyingSession"
    extern const char g_str_snHsmDestroyingSession[];
    return g_str_snHsmDestroyingSession;
}

/**
 * snHsmDestroyingSession::OnUpdate @ 0x823DEAD0 | size: 0x78
 *
 * Initiates session destruction by calling the XAM session delete function.
 * On failure, raises an EvtCreateFailed event (reuses the create-failed
 * event type to signal teardown failure).
 * Sets the parent machine's state context to the DestroyingSession thunk.
 */
void snHsmDestroyingSession::OnUpdate() {
    snCreateMachine* machine = reinterpret_cast<snCreateMachine*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 20)
    );

    // Get network client's XAM context at offset +232
    void* networkClient = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(machine) + 16)
    );
    void* xamContext = reinterpret_cast<void*>(
        reinterpret_cast<uint8_t*>(networkClient) + 232
    );

    // Call XAM session delete
    bool success = xam_EAB8_g(xamContext);
    if (!success) {
        // Raise EvtCreateFailed event (reused for teardown failure)
        hsmEvent failEvent;
        snHsmState_Init(&failEvent, nullptr);
        failEvent.vtable = reinterpret_cast<void**>(0x82072A28);  // EvtCreateFailed
        snSession_AddChildNode(this, &failEvent);
    }

    // Set parent machine state context
    // thunk_fn_823DEB48 @ 0x823EB0F0
    machine->m_pCurrentHsmState = this;
    machine->m_pStateVtable = reinterpret_cast<void*>(0x823EB0F0);
}

} // namespace rage
