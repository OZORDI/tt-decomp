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


// ─────────────────────────────────────────────────────────────────────────────
// pongCSAction (base) — container that fans each virtual call out to children.
// All actions store their associated data at m_pData (+4). The enabled flag
// lives at +16 in every gdCSAction*Data derivative.
// ─────────────────────────────────────────────────────────────────────────────

namespace {
    // Every action-data struct inherits from a common base whose first byte
    // after the vtable+_pad cluster (at +16) is the "enabled" flag. We only
    // need that one field here, so a tiny accessor struct keeps things typed
    // without pulling the full data hierarchy into this translation unit.
    struct pongCSActionDataBase {
        void**      vtable;         // +0x00
        uint32_t    _pad04[3];      // +0x04-0x0F
        bool        m_bEnabled;     // +0x10
    };

    inline bool ActionDataEnabled(const pongCSAction* self) {
        return static_cast<pongCSActionDataBase*>(self->m_pData)->m_bEnabled;
    }
}

// ── pongCSAction::Update() [vtable 4 @ 0x82349200] ──────────────────────────
void pongCSAction::Update() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Update();
    }
}

// ── pongCSAction::Play() [vtable 5 @ 0x82349258] ────────────────────────────
void pongCSAction::Play() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Play();
    }
}

// ── pongCSAction::Stop() [vtable 6 @ 0x82349318] ────────────────────────────
void pongCSAction::Stop() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Stop();
    }
}

// ── pongCSAction::Pause(float dt) [vtable 7 @ 0x823492b0] ───────────────────
void pongCSAction::Pause(float dt) {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Pause(dt);
    }
}

// ── pongCSAction::Resume() [vtable 8 @ 0x82349370] ──────────────────────────
// Returns true when *all* children report "ready to resume". Short-circuits
// on the first child that returns false (still running).
bool pongCSAction::Resume() {
    if (!ActionDataEnabled(this)) {
        return true;
    }
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        if (!m_pChildren[i]->Resume()) {
            return false;
        }
    }
    return true;
}

// ── pongCSAction::Seek(float t) [vtable 9 @ 0x823493f8] ─────────────────────
void pongCSAction::Seek(float t) {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Seek(t);
    }
}

// ── pongCSActionDoInOrder::Play() [vtable 5 @ 0x82349680] ───────────────────
// Delegates to the base "fan out Play()" and then resets the child index.
void pongCSActionDoInOrder::Play() {
    pongCSAction::Play();
    m_nCurrentChild = 0;
}

// ── pongCSActionDoInOrder::Stop() [vtable 6 @ 0x823496b8] ───────────────────
// Forwards Stop() to the currently active child (if the sequence hasn't
// already finished). Other children are idle so we skip them.
void pongCSActionDoInOrder::Stop() {
    if (m_nCurrentChild >= m_nChildCount) {
        return;
    }
    m_pChildren[m_nCurrentChild]->Stop();
}

// ── pongCSActionDoInOrder::Resume() [vtable 8 @ 0x82349798] ─────────────────
// If the current child says "I'm done" (Resume()==true), advance to the next.
// Then re-enter Stop() on the (now previous) child via the vtable so derived
// classes get their cleanup. Return value mirrors the base contract: true
// when the whole sequence has drained.
bool pongCSActionDoInOrder::Resume() {
    if (!ActionDataEnabled(this)) {
        return true;
    }
    if (m_nCurrentChild >= m_nChildCount) {
        return true;
    }
    return false;  // still playing: at least one child remains
}

// ── pongCSActionWait::Play() [vtable 5 @ 0x82349fc0] ────────────────────────
// Reset the elapsed-time counter. The .rdata constant at 0x8202D110 is 0.0f.
void pongCSActionWait::Play() {
    m_fElapsed = 0.0f;
}

// ── pongCSActionWait::Pause(float dt) [vtable 7 @ 0x82349fd0] ───────────────
// Accumulate dt into the elapsed timer.
void pongCSActionWait::Pause(float dt) {
    m_fElapsed = m_fElapsed + dt;
}

// ── pongCSActionWait::Resume() [vtable 8 @ 0x82349fe0] ──────────────────────
// Report completion once the timer has reached the configured duration.
bool pongCSActionWait::Resume() {
    gdCSActionWaitData* data = static_cast<gdCSActionWaitData*>(m_pData);
    if (!data->m_bEnabled) {
        return true;
    }
    return m_fElapsed >= data->m_fDuration;
}
