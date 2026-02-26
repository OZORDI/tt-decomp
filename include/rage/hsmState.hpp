/**
 * hsmState.hpp — Hierarchical State Machine Base Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE engine hierarchical state machine (HSM) framework.
 * Provides base classes for state management with event handling,
 * transitions, and hierarchical state composition.
 */

#pragma once

#include <stdint.h>

namespace rage {

// ────────────────────────────────────────────────────────────────────────────
// hsmStateBase — Base State Machine State
// @ vtable 0x82073F1C
// ────────────────────────────────────────────────────────────────────────────

/**
 * Base class for hierarchical state machine states.
 * Provides minimal interface for state lifecycle management.
 */
class hsmStateBase {
public:
    /**
     * Virtual destructor
     */
    virtual ~hsmStateBase() = default;
    
    /**
     * GetStateName @ slot 2
     * Returns the name of this state as a string
     */
    virtual const char* GetStateName() const = 0;
    
    /**
     * GetFullStatePath @ slot 3
     * Builds full hierarchical state path into buffer
     */
    virtual char* GetFullStatePath(char* buffer, uint32_t bufferSize) const = 0;
    
    /**
     * Reset @ slot 4 | @ 0x823ED388
     * Clears state fields - resets state to initial condition
     */
    virtual void Reset();

protected:
    void** m_vtable;              // +0x00
};

} // namespace rage

// ────────────────────────────────────────────────────────────────────────────
// hsmState — Extended State Machine State
// @ vtable 0x8206E70C
// Inherits from: rage::hsmStateBase
// ────────────────────────────────────────────────────────────────────────────

/**
 * Extended state machine state with parent/child relationships
 * and event handling capabilities.
 * 
 * Structure:
 *   +0x00: vtable pointer
 *   +0x04: m_pManager - state manager/context
 *   +0x08: m_pParentState - parent state in hierarchy
 *   +0x0C: m_pChildState - active child state
 *   +0x10: m_field_10 - unknown field
 *   +0x14: m_field_14 - unknown field
 */
class hsmState : public rage::hsmStateBase {
public:
    /**
     * GetStateName @ 0x823ED280 | size: 0xC
     * Returns state name string (address 0x82073E9C)
     */
    virtual const char* GetStateName() const override;
    
    /**
     * GetFullStatePath @ 0x823ED290 | size: 0xF8
     * Builds full hierarchical path by recursing up parent chain
     */
    virtual char* GetFullStatePath(char* buffer, uint32_t bufferSize) const override;
    
    /**
     * Reset @ 0x823E8D58 | size: 0x50
     * Clears all state fields (+4, +8, +12, +16, +20)
     * Calls vfn_6 first (cleanup hook)
     */
    virtual void Reset() override;
    
    /**
     * OnEnter @ slot 5
     * Called when entering this state
     */
    virtual void OnEnter() = 0;
    
    /**
     * OnExit @ slot 6
     * Called when leaving this state
     */
    virtual void OnExit() = 0;
    
    /**
     * OnEvent @ slot 7
     * Handle state machine events
     */
    virtual void OnEvent(int32_t eventType) = 0;
    
    /**
     * Update @ slot 8
     * Frame update callback
     */
    virtual void Update(float deltaTime) = 0;

protected:
    void** m_vtable;              // +0x00
    void* m_pManager;             // +0x04 - State manager/context
    hsmState* m_pParentState;     // +0x08 - Parent state in hierarchy
    hsmState* m_pChildState;      // +0x0C - Active child state
    uint32_t m_field_10;          // +0x10
    uint32_t m_field_14;          // +0x14
};

