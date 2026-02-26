/**
 * snCreateMachine.hpp — Session Network Create Machine State Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Hierarchical state machine for managing session creation flow.
 * Handles host/guest/offline creation, config negotiation, and session startup.
 *
 * State hierarchy:
 *   snCreateMachine (base)
 *   ├── snHsmCreatingHost      - Creating as host
 *   ├── snHsmCreatingGuest     - Creating as guest
 *   ├── snHsmCreatingOffline   - Creating offline session
 *   ├── snHsmRequestingConfig  - Requesting session configuration
 *   ├── snHsmApplyingConfig    - Applying received configuration
 *   ├── snHsmStartingSession   - Starting the session
 *   └── snHsmDestroyingSession - Tearing down the session
 */

#pragma once
#include <stdint.h>
#include "rage/hsmState.hpp"

namespace rage {

// Forward declarations
class SinglesNetworkClient;
class snSession;

/**
 * snCreateMachine @ vtable 0x82072AB4
 * 
 * Base state machine for session creation.
 * Manages the lifecycle of creating and configuring a network session.
 */
class snCreateMachine : public hsmState {
public:
    // ── Lifecycle ──
    virtual ~snCreateMachine();                                    // [0] @ 0x823E5CC0
    
    // ── State Machine Interface ──
    virtual const char* GetStateName() const;                      // [2] @ 0x823DDA60
    virtual void OnEnter();                                        // [13] @ 0x823DDAB0
    virtual void OnUpdate();                                       // [14] @ 0x823DDA70
    
    // ── Session Management ──
    virtual void InitializeSession();                              // [5] @ 0x823DEBC0
    virtual void UpdateSessionState();                             // [6] @ 0x823DEC38
    virtual void HandleStateTransition();                          // [15] @ 0x823F27E8
    
    // ── State Access ──
    virtual hsmState* GetCurrentState();                           // [10] (unnamed)
    virtual hsmState* GetStateByType(uint32_t stateType);         // [11] (unnamed)
    
protected:
    // ── Members ──
    SinglesNetworkClient* m_pNetworkClient;  // +0x14 (20)
    void*                 m_pStateContext;   // +0x18 (24)
    hsmState*             m_pCurrentHsmState; // +0x1C (28)
    void*                 m_pStateVtable;    // +0x20 (32)
    
    // State-specific data at different offsets:
    uint8_t               m_stateData[268];  // +0x24 to +0x12C (36-300)
    // +0x54 (84):  CreatingHost state
    // +0x6C (108): CreatingGuest state  
    // +0x84 (132): CreatingOffline state
    
    uint32_t              m_sessionId;       // +0x114 (276)
    uint64_t              m_sessionHandle;   // +0x118 (280)
    uint64_t              m_field_120;       // +0x120 (288)
    uint32_t              m_retryCount;      // +0x128 (296)
    uint8_t               m_flags;           // +0x12C (300)
};

/**
 * snCreateMachine::snHsmCreatingHost @ vtable 0x82072AFC
 * 
 * State for creating a session as the host.
 * Handles host-specific initialization and network setup.
 */
class snHsmCreatingHost : public hsmState {
public:
    virtual ~snHsmCreatingHost();                                  // [0] @ 0x823E5FC8
    virtual const char* GetStateName() const;                      // [2] @ 0x823DDC48
    virtual void OnUpdate();                                       // [14] @ 0x823DDC58
    virtual void OnExit();                                         // [15] @ 0x823DDD58
};

/**
 * snCreateMachine::snHsmCreatingGuest @ vtable 0x82072B44
 * 
 * State for creating a session as a guest.
 * Handles guest-specific initialization and joining logic.
 */
class snHsmCreatingGuest : public hsmState {
public:
    virtual ~snHsmCreatingGuest();                                 // [0] (shares destructor)
    virtual const char* GetStateName() const;                      // [2] @ 0x823DDDF8
    virtual void OnTick();                                         // [12] @ 0x823DDFF0
    virtual void OnUpdate();                                       // [14] @ 0x823DDE08
};

/**
 * snCreateMachine::snHsmCreatingOffline @ vtable 0x82072B8C
 * 
 * State for creating an offline session.
 * Handles local-only session setup without network.
 */
class snHsmCreatingOffline : public hsmState {
public:
    virtual ~snHsmCreatingOffline();                               // [0] (shares destructor)
    virtual const char* GetStateName() const;                      // [2] @ 0x823DE098
    virtual void OnUpdate();                                       // [14] @ 0x823DE0A8
};

/**
 * snCreateMachine::snHsmRequestingConfig @ vtable 0x82072BD4
 * 
 * State for requesting session configuration from host.
 * Polls for config availability and handles timeouts.
 */
class snHsmRequestingConfig : public hsmState {
public:
    virtual ~snHsmRequestingConfig();                              // [0] @ 0x823E6018
    virtual const char* GetStateName() const;                      // [2] @ 0x823DE228
    virtual void OnTick();                                         // [12] @ 0x823DE450
    virtual void OnUpdate();                                       // [14] @ 0x823DE238
    virtual void OnExit();                                         // [15] @ 0x823DE408
};

/**
 * snCreateMachine::snHsmApplyingConfig @ vtable 0x82072C1C
 * 
 * State for applying received session configuration.
 * Validates and applies config data to local session.
 */
class snHsmApplyingConfig : public hsmState {
public:
    virtual ~snHsmApplyingConfig();                                // [0] (shares destructor)
    virtual const char* GetStateName() const;                      // [2] @ 0x823DE5E0
    virtual void OnTick();                                         // [12] @ 0x823DE7E8
    virtual void OnUpdate();                                       // [14] @ 0x823DE5F0
};

/**
 * snCreateMachine::snHsmStartingSession @ vtable 0x82072C64
 * 
 * State for starting the configured session.
 * Finalizes setup and transitions to active session state.
 */
class snHsmStartingSession : public hsmState {
public:
    virtual ~snHsmStartingSession();                               // [0] (shares destructor)
    virtual const char* GetStateName() const;                      // [2] @ 0x823DE908
    virtual void OnTick();                                         // [12] @ 0x823DEA18
    virtual void OnUpdate();                                       // [14] @ 0x823DE918
};

/**
 * snCreateMachine::snHsmDestroyingSession @ vtable 0x82072CAC
 * 
 * State for destroying/cleaning up a session.
 * Handles graceful shutdown and resource cleanup.
 */
class snHsmDestroyingSession : public hsmState {
public:
    virtual ~snHsmDestroyingSession();                             // [0] (shares destructor)
    virtual const char* GetStateName() const;                      // [2] @ 0x823DEAC0
    virtual void OnUpdate();                                       // [14] @ 0x823DEAD0
};

} // namespace rage
