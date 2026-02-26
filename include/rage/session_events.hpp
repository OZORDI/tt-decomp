/**
 * session_events.hpp — RAGE Session Event Notification Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Event notification system for multiplayer session management.
 * These classes represent various session lifecycle events (create, destroy,
 * join, leave, migrate, etc.) used by the RAGE networking layer.
 *
 * Base class: snNotifyBase
 * All event classes inherit from snNotifyBase and provide type-safe
 * event notifications for session state changes.
 */
#pragma once

#include <stdint.h>

namespace rage {

/**
 * snNotifyBase @ 0x82072A1C
 * Base class for all session notification events.
 * Provides virtual destructor and common event interface.
 */
class snNotifyBase {
public:
    virtual ~snNotifyBase();  // @ 0x823DD8E0
    
protected:
    // Constructor sets up vtable
    snNotifyBase() = default;
};

// ── Session Creation Events ──

/**
 * EvtHostCreate @ 0x820729A4
 * Notification that host is creating a session.
 */
class EvtHostCreate : public snNotifyBase {
public:
    virtual ~EvtHostCreate() = default;
};

/**
 * EvtGuestCreate @ 0x820729B8
 * Notification that guest is creating a session.
 */
class EvtGuestCreate : public snNotifyBase {
public:
    virtual ~EvtGuestCreate() = default;
};

/**
 * EvtCreateSucceeded @ 0x82072AA0
 * Notification that session creation succeeded.
 */
class EvtCreateSucceeded : public snNotifyBase {
public:
    virtual ~EvtCreateSucceeded() = default;
};

/**
 * EvtCreateFailed @ 0x82072A28
 * Notification that session creation failed.
 */
class EvtCreateFailed : public snNotifyBase {
public:
    virtual ~EvtCreateFailed() = default;
};

/**
 * EvtGuestCreateSucceeded @ 0x82072A3C
 * Notification that guest session creation succeeded.
 */
class EvtGuestCreateSucceeded : public snNotifyBase {
public:
    virtual ~EvtGuestCreateSucceeded() = default;
};

// ── Session Start Events ──

/**
 * EvtStart @ 0x820728F0
 * Notification that session is starting.
 */
class EvtStart : public snNotifyBase {
public:
    virtual ~EvtStart() = default;
};

/**
 * EvtStartSucceeded @ 0x82072904
 * Notification that session start succeeded.
 */
class EvtStartSucceeded : public snNotifyBase {
public:
    virtual ~EvtStartSucceeded() = default;
};

/**
 * EvtStartFailed @ 0x82072918
 * Notification that session start failed.
 */
class EvtStartFailed : public snNotifyBase {
public:
    virtual ~EvtStartFailed() = default;
};

/**
 * EvtStartInProgressSession @ 0x82072A78
 * Notification that an in-progress session is starting.
 */
class EvtStartInProgressSession : public snNotifyBase {
public:
    virtual ~EvtStartInProgressSession() = default;
};

/**
 * EvtStartSessionFailed @ 0x82072A8C
 * Notification that session start failed.
 */
class EvtStartSessionFailed : public snNotifyBase {
public:
    virtual ~EvtStartSessionFailed() = default;
};

// ── Session End Events ──

/**
 * EvtEnd @ 0x8207292C
 * Notification that session is ending.
 */
class EvtEnd : public snNotifyBase {
public:
    virtual ~EvtEnd() = default;
};

/**
 * EvtEndSucceeded @ 0x82072940
 * Notification that session end succeeded.
 */
class EvtEndSucceeded : public snNotifyBase {
public:
    virtual ~EvtEndSucceeded() = default;
};

/**
 * EvtEndFailed @ 0x82072954
 * Notification that session end failed.
 */
class EvtEndFailed : public snNotifyBase {
public:
    virtual ~EvtEndFailed() = default;
};

// ── Session Destroy Events ──

/**
 * EvtDestroy @ 0x82072968
 * Notification that session is being destroyed.
 */
class EvtDestroy : public snNotifyBase {
public:
    virtual ~EvtDestroy() = default;
};

/**
 * EvtDestroySucceeded @ 0x8207297C
 * Notification that session destruction succeeded.
 */
class EvtDestroySucceeded : public snNotifyBase {
public:
    virtual ~EvtDestroySucceeded() = default;
};

/**
 * EvtDestroyFailed @ 0x82072990
 * Notification that session destruction failed.
 */
class EvtDestroyFailed : public snNotifyBase {
public:
    virtual ~EvtDestroyFailed() = default;
};

// ── Presence Change Events ──

/**
 * EvtChangePresence @ 0x820729CC
 * Notification that presence is changing.
 */
class EvtChangePresence : public snNotifyBase {
public:
    virtual ~EvtChangePresence() = default;
};

/**
 * EvtHostChangePresenceSucceeded @ 0x8207288C
 * Notification that host presence change succeeded.
 */
class EvtHostChangePresenceSucceeded : public snNotifyBase {
public:
    virtual ~EvtHostChangePresenceSucceeded() = default;
};

/**
 * EvtChangePresenceSucceeded @ 0x820728A0
 * Notification that presence change succeeded.
 */
class EvtChangePresenceSucceeded : public snNotifyBase {
public:
    virtual ~EvtChangePresenceSucceeded() = default;
};

/**
 * EvtChangePresenceFailed @ 0x820728B4
 * Notification that presence change failed.
 */
class EvtChangePresenceFailed : public snNotifyBase {
public:
    virtual ~EvtChangePresenceFailed() = default;
};

// ── Migration Events ──

/**
 * EvtMigrateSucceeded @ 0x820728C8
 * Notification that session migration succeeded.
 */
class EvtMigrateSucceeded : public snNotifyBase {
public:
    virtual ~EvtMigrateSucceeded() = default;
};

/**
 * EvtMigrateFailed @ 0x820728DC
 * Notification that session migration failed.
 */
class EvtMigrateFailed : public snNotifyBase {
public:
    virtual ~EvtMigrateFailed() = default;
};

// ── Connection Events ──

/**
 * EvtKillConnection @ 0x820729E0
 * Notification that a connection is being killed.
 */
class EvtKillConnection : public snNotifyBase {
public:
    virtual ~EvtKillConnection() = default;
};

/**
 * EvtDroppedConnection @ 0x820729F4
 * Notification that a connection was dropped.
 */
class EvtDroppedConnection : public snNotifyBase {
public:
    virtual ~EvtDroppedConnection() = default;
};

/**
 * EvtClosedConnection @ 0x82072A08
 * Notification that a connection was closed.
 */
class EvtClosedConnection : public snNotifyBase {
public:
    virtual ~EvtClosedConnection() = default;
};

// ── Configuration Events ──

/**
 * EvtRequestConfigFailed @ 0x82072A50
 * Notification that config request failed.
 */
class EvtRequestConfigFailed : public snNotifyBase {
public:
    virtual ~EvtRequestConfigFailed() = default;
};

/**
 * EvtApplyConfigFailed @ 0x82072A64
 * Notification that config application failed.
 */
class EvtApplyConfigFailed : public snNotifyBase {
public:
    virtual ~EvtApplyConfigFailed() = default;
};

} // namespace rage
