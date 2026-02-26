/**
 * hsmState.cpp — Hierarchical State Machine Base Classes Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "rage/hsmState.hpp"
#include <cstring>

namespace rage {

/**
 * hsmStateBase::Reset @ 0x823ED388 | size: 0x14
 * 
 * Base reset implementation - does nothing in base class.
 */
void hsmStateBase::Reset() {
    // Base implementation is empty
    // Derived classes override to provide specific reset behavior
}

} // namespace rage

// ────────────────────────────────────────────────────────────────────────────
// hsmState Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * hsmState::GetStateName @ 0x823ED280 | size: 0xC
 * 
 * Returns the state name string.
 */
const char* hsmState::GetStateName() const {
    // Returns address 0x82073E9C - a default state name string
    return (const char*)0x82073E9C;
}

/**
 * hsmState::GetFullStatePath @ 0x823ED290 | size: 0xF8
 * 
 * Builds full hierarchical state path by recursing up parent chain.
 * Format: "ParentState/ChildState/ThisState"
 */
char* hsmState::GetFullStatePath(char* buffer, uint32_t bufferSize) const {
    uint32_t currentLen = 0;
    
    // If we have a parent state, get its path first
    if (m_pParentState) {
        m_pParentState->GetFullStatePath(buffer, bufferSize);
        
        // Calculate current length
        currentLen = (uint32_t)strlen(buffer);
    }
    
    // Get our state name
    const char* stateName = GetStateName();
    
    // Calculate remaining space
    uint32_t remaining = bufferSize - currentLen;
    
    if (currentLen < bufferSize) {
        // Append separator and our name
        const char* separator = "/";  // @ 0x8207EC9C
        
        // Copy our name into the buffer
        char* dest = buffer + currentLen;
        uint32_t nameLen = (uint32_t)strlen(stateName);
        
        if (nameLen < remaining) {
            // Copy character by character
            for (uint32_t i = 0; i < nameLen && i < remaining - 1; i++) {
                dest[i] = stateName[i];
                if (stateName[i] == '\0') break;
            }
            dest[nameLen < remaining ? nameLen : remaining - 1] = '\0';
        }
    }
    
    return buffer;
}

/**
 * hsmState::Reset @ 0x823E8D58 | size: 0x50
 * 
 * Clears all state fields.
 * Calls OnExit first (vtable slot 6) as a cleanup hook.
 */
void hsmState::Reset() {
    // Call OnExit as cleanup hook
    OnExit();
    
    // Clear all state fields
    m_pManager = nullptr;
    m_pParentState = nullptr;
    m_pChildState = nullptr;
    m_field_10 = 0;
    m_field_14 = 0;
}

