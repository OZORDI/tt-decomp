/**
 * pong_cutscene.cpp — Cutscene action implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_cutscene.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// pongCSActionLoop::Resume() [vtable slot 8 @ 0x82349998]
// Checks if the loop should continue or has completed
// ─────────────────────────────────────────────────────────────────────────────
bool pongCSActionLoop::Resume() {
    // Load loop data structure (at this+4)
    gdCSActionLoopData* loopData = m_pLoopData;
    
    // Check if loop is enabled/active (byte at loopData+16)
    if (!loopData->m_bEnabled) {
        return true;  // Loop is disabled, consider it complete
    }
    
    // Load action data structure (at this+16)
    // and get max iterations (int at actionData+20)
    int maxIterations = m_pActionData->m_nMaxIterations;
    
    // Check if we have a valid max iteration count
    if (maxIterations < 0) {
        return false;  // Infinite loop or invalid state
    }
    
    // Check if we've reached the iteration limit
    // Current iteration is at this+20
    if (m_nCurrentIteration >= maxIterations) {
        return true;  // Loop complete
    }
    
    return false;  // Loop should continue
}

