/**
 * session_finder.hpp — RAGE Session Finder System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Session discovery system for finding multiplayer sessions.
 * Implements hierarchical state machine (HSM) for searching via system link
 * and Xbox Live online matchmaking.
 *
 * Architecture:
 * - snSessionFinder: Main session finder controller
 * - slfRoot: Root HSM state
 *   - slfSearching: Active search state
 *     - slfFindingSyslink: System link discovery
 *       - slfBroadcasting: Broadcasting for local sessions
 *     - slfFindingOnline: Xbox Live matchmaking
 *   - slfFinal: Search complete state
 *
 * Event notifications:
 * - snNotifySyslinkRequest: Request to search via system link
 * - snNotifyFoundSyslink: System link session found
 * - snNotifyFoundOnline: Online session found
 */
#pragma once

#include <stdint.h>

namespace rage {

// Forward declarations
class snNotifyBase;
class hsmState;

// ── Session Finder Event Notifications ──

/**
 * snNotifySyslinkRequest @ 0x82074100
 * Notification requesting system link session search.
 * Contains two embedded SinglesNetworkClient instances.
 */
class snNotifySyslinkRequest : public snNotifyBase {
public:
    virtual ~snNotifySyslinkRequest();  // @ 0x823EE1F0
    
    // Structure layout (size >= 112 bytes)
    uint8_t m_client1[104];                // +0x04 to +0x6B: First client
    uint8_t m_client2[104];                // +0x6C to +0xD3: Second client
};

/**
 * snNotifyFoundSyslink @ 0x8207410C
 * Notification that a system link session was found.
 * Contains one embedded SinglesNetworkClient instance.
 */
class snNotifyFoundSyslink : public snNotifyBase {
public:
    virtual ~snNotifyFoundSyslink();  // @ 0x823EE258
    
    // Structure layout
    uint8_t m_client[104];                 // +0x04 to +0x6B: Client data
};

/**
 * snNotifyFoundOnline @ 0x82074118
 * Notification that an online session was found.
 */
class snNotifyFoundOnline : public snNotifyBase {
public:
    virtual ~snNotifyFoundOnline() = default;
};

// ── Session Finder State Machine ──

/**
 * snSessionFinder @ 0x82074124
 * Main session finder controller.
 * Manages hierarchical state machine for session discovery.
 * 
 * Structure layout (size >= 105 bytes):
 * +0x00: vtable pointer
 * +0x04: param2 (from constructor)
 * +0x08: state root pointer (points to +0x3C)
 * +0x10: field (initialized to 0)
 * +0x14: param1 (from constructor)
 * +0x18: field (initialized to 0)
 * +0x24: field (initialized to 0)
 * +0x28: param1 copy
 * +0x3C: embedded state root (slfRoot)
 * +0x4C: field (initialized to 0)
 * +0x50: field (initialized to 0)
 * +0x54: field (initialized to 0)
 * +0x58: field (initialized to 0)
 * +0x68: flags byte (bit 7 cleared during init)
 */
class snSessionFinder {
public:
    // Constructor @ 0x823ED720
    // Parameters are swapped: param1 goes to +0x14, param2 goes to +0x04
    snSessionFinder(void* param1, void* param2);
    
    virtual ~snSessionFinder();  // @ 0x823EED30
    
    // State machine root
    class slfRoot;
    
protected:
    void* m_vtable;                        // +0x00
    void* m_pParam2;                       // +0x04 (constructor param2)
    void* m_pStateRoot;                    // +0x08 (points to +0x3C)
    uint32_t m_field_10;                   // +0x10
    void* m_pParam1;                       // +0x14 (constructor param1)
    uint32_t m_field_18;                   // +0x18
    uint32_t m_field_1C;                   // +0x1C
    uint32_t m_field_20;                   // +0x20
    uint32_t m_field_24;                   // +0x24
    void* m_pParam1Copy;                   // +0x28 (copy of param1)
    
    // Embedded state root at +0x3C
    uint8_t m_stateRootData[0x2C];         // +0x3C to +0x67
    uint8_t m_flags;                       // +0x68 (bit 7 cleared)
};

/**
 * slfRoot @ 0x82074134
 * Root state of session finder HSM.
 * Contains embedded sub-states at fixed offsets.
 */
class snSessionFinder::slfRoot : public hsmState {
public:
    virtual ~slfRoot();  // @ 0x823EEE88
    virtual void vfn_2();  // @ 0x823EE2B8 - State entry
    virtual void vfn_5();  // @ 0x823EEA40 - State update
    virtual void vfn_6();  // @ 0x823EEA80 - State event handler
    virtual void vfn_10();  // Unnamed - Get state info
    virtual void vfn_11();  // Unnamed - Get sub-state
    virtual void vfn_12();  // @ 0x823EE2C8 - Main state processing
    
    // Nested states
    class slfSearching;
    class slfFinal;
    
protected:
    // Embedded sub-states at fixed offsets
    uint8_t m_stateSearching[72];          // +0x30 (offset 48)
    uint8_t m_stateOnline[72];             // +0x78 (offset 120)
    uint8_t m_stateFinal[72];              // +0x90 (offset 144)
};

/**
 * slfSearching @ 0x8207417C
 * Active search state - coordinates system link and online searches.
 * Manages flags at offset +0x10D20 (68896) in parent structure.
 */
class snSessionFinder::slfRoot::slfSearching : public hsmState {
public:
    virtual ~slfSearching();  // @ 0x823EEEF8
    virtual void vfn_2();  // @ 0x823EE450 - State entry
    virtual void vfn_5();  // @ 0x823EE910 - State update
    virtual void vfn_6();  // @ 0x823EE950 - State event handler
    virtual void vfn_14();  // @ 0x823EE460 - Initialize search
    virtual void vfn_15();  // @ 0x823EE4A8 - Cleanup search
    
    // Nested states
    class slfFindingSyslink;
    class slfFindingOnline;
    
protected:
    void* m_pNetworkState;                 // +0x10 - Points to large network object
};

/**
 * slfFindingSyslink @ 0x820741C4
 * System link discovery state - searches for local network sessions.
 * Accesses network state fields at offsets +0x914, +0x918, +0x91C, +0x928.
 */
class snSessionFinder::slfRoot::slfSearching::slfFindingSyslink : public hsmState {
public:
    virtual ~slfFindingSyslink();  // @ 0x823EEF50
    virtual void vfn_2();  // @ 0x823EE4C8 - State entry
    virtual void vfn_14();  // @ 0x823EE4D8 - System link search logic
    
    // Nested state: Broadcasting
    class slfBroadcasting;
};

/**
 * slfBroadcasting @ 0x8207420C
 * Broadcasting state - actively broadcasts search requests on local network.
 */
class snSessionFinder::slfRoot::slfSearching::slfFindingSyslink::slfBroadcasting : public hsmState {
public:
    virtual ~slfBroadcasting();  // @ 0x823EEB00
    virtual void vfn_2();  // @ 0x823EE590 - State entry
    virtual void vfn_12();  // @ 0x823EE658 - Broadcast processing
    virtual void vfn_14();  // @ 0x823EE5A0 - Start broadcasting
    virtual void vfn_15();  // @ 0x823EE610 - Stop broadcasting
};

/**
 * slfFindingOnline @ 0x82074254
 * Online matchmaking state - searches Xbox Live for sessions.
 */
class snSessionFinder::slfRoot::slfSearching::slfFindingOnline : public hsmState {
public:
    virtual ~slfFindingOnline() = default;
    virtual void vfn_2();  // @ 0x823EE748 - State entry
    virtual void vfn_14();  // @ 0x823EE758 - Online search logic
    virtual void vfn_15();  // @ 0x823EE8D0 - Cleanup online search
};

/**
 * slfFinal @ 0x8207429C
 * Final state - search complete.
 */
class snSessionFinder::slfRoot::slfFinal : public hsmState {
public:
    virtual ~slfFinal() = default;
    virtual void vfn_2();  // @ 0x823EE9D0 - State entry
    virtual void vfn_14();  // @ 0x823EE9E0 - Final processing
};

} // namespace rage
