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

// ── Join Events ──

/**
 * EvtJoinSucceeded @ 0x82072CF4
 * Notification that join operation succeeded.
 */
class EvtJoinSucceeded : public snNotifyBase {
public:
    virtual ~EvtJoinSucceeded() = default;
};

/**
 * EvtJoinFailed @ 0x82072D08
 * Notification that join operation failed.
 */
class EvtJoinFailed : public snNotifyBase {
public:
    virtual ~EvtJoinFailed() = default;
};

/**
 * EvtLocalJoinSucceeded @ 0x82072D1C
 * Notification that local player join succeeded.
 */
class EvtLocalJoinSucceeded : public snNotifyBase {
public:
    virtual ~EvtLocalJoinSucceeded() = default;
};

/**
 * EvtLocalJoinFailed @ 0x82072D30
 * Notification that local player join failed.
 */
class EvtLocalJoinFailed : public snNotifyBase {
public:
    virtual ~EvtLocalJoinFailed() = default;
};

/**
 * EvtRemoteJoinSucceeded @ 0x82072D44
 * Notification that remote player join succeeded.
 */
class EvtRemoteJoinSucceeded : public snNotifyBase {
public:
    virtual ~EvtRemoteJoinSucceeded() = default;
};

/**
 * EvtRemoteJoinFailed @ 0x82072D58
 * Notification that remote player join failed.
 */
class EvtRemoteJoinFailed : public snNotifyBase {
public:
    virtual ~EvtRemoteJoinFailed() = default;
};

/**
 * EvtRequestJoinSucceeded @ 0x82072D6C
 * Notification that join request succeeded.
 */
class EvtRequestJoinSucceeded : public snNotifyBase {
public:
    virtual ~EvtRequestJoinSucceeded() = default;
};

/**
 * EvtRequestJoinFailed @ 0x82072D80
 * Notification that join request failed.
 */
class EvtRequestJoinFailed : public snNotifyBase {
public:
    virtual ~EvtRequestJoinFailed() = default;
};

/**
 * EvtAcceptJoinRequestSucceeded @ 0x82072D94
 * Notification that accepting join request succeeded.
 */
class EvtAcceptJoinRequestSucceeded : public snNotifyBase {
public:
    virtual ~EvtAcceptJoinRequestSucceeded() = default;
};

/**
 * EvtAcceptJoinRequestFailed @ 0x82072DA8
 * Notification that accepting join request failed.
 */
class EvtAcceptJoinRequestFailed : public snNotifyBase {
public:
    virtual ~EvtAcceptJoinRequestFailed() = default;
};

/**
 * EvtSessionJoinSucceeded @ 0x82072DBC
 * Notification that session join succeeded.
 */
class EvtSessionJoinSucceeded : public snNotifyBase {
public:
    virtual ~EvtSessionJoinSucceeded() = default;
};

/**
 * EvtSessionJoinFailed @ 0x82072DD0
 * Notification that session join failed.
 */
class EvtSessionJoinFailed : public snNotifyBase {
public:
    virtual ~EvtSessionJoinFailed() = default;
};

/**
 * EvtJoin @ 0x82072DE4
 * Notification that a join operation is occurring.
 */
class EvtJoin : public snNotifyBase {
public:
    virtual ~EvtJoin() = default;
};

/**
 * EvtRemoteJoin @ 0x82072DF8
 * Notification that a remote player is joining.
 */
class EvtRemoteJoin : public snNotifyBase {
public:
    virtual ~EvtRemoteJoin() = default;
};

/**
 * EvtJoinRequest @ 0x82072E0C
 * Notification of a join request.
 */
class EvtJoinRequest : public snNotifyBase {
public:
    virtual ~EvtJoinRequest() = default;
};

/**
 * EvtJoinReply @ 0x82072E20
 * Notification of a join reply.
 */
class EvtJoinReply : public snNotifyBase {
public:
    virtual ~EvtJoinReply() = default;
};

/**
 * EvtJoinAccepted @ 0x82074388
 * Notification that join was accepted.
 */
class EvtJoinAccepted : public snNotifyBase {
public:
    virtual ~EvtJoinAccepted() = default;
};

/**
 * EvtJoinDenied @ 0x8207439C
 * Notification that join was denied.
 */
class EvtJoinDenied : public snNotifyBase {
public:
    virtual ~EvtJoinDenied() = default;
};

// ── Leave Events ──

/**
 * EvtLeaveSucceeded @ 0x820730BC
 * Notification that leave operation succeeded.
 */
class EvtLeaveSucceeded : public snNotifyBase {
public:
    virtual ~EvtLeaveSucceeded() = default;
};

/**
 * EvtLeaveFailed @ 0x820730D0
 * Notification that leave operation failed.
 */
class EvtLeaveFailed : public snNotifyBase {
public:
    virtual ~EvtLeaveFailed() = default;
};

/**
 * EvtLocalLeaveSucceeded @ 0x820730E4
 * Notification that local player leave succeeded.
 */
class EvtLocalLeaveSucceeded : public snNotifyBase {
public:
    virtual ~EvtLocalLeaveSucceeded() = default;
};

/**
 * EvtLocalLeaveFailed @ 0x820730F8
 * Notification that local player leave failed.
 */
class EvtLocalLeaveFailed : public snNotifyBase {
public:
    virtual ~EvtLocalLeaveFailed() = default;
};

/**
 * EvtRemoteLeaveSucceeded @ 0x8207310C
 * Notification that remote player leave succeeded.
 */
class EvtRemoteLeaveSucceeded : public snNotifyBase {
public:
    virtual ~EvtRemoteLeaveSucceeded() = default;
};

/**
 * EvtRemoteLeaveFailed @ 0x82073120
 * Notification that remote player leave failed.
 */
class EvtRemoteLeaveFailed : public snNotifyBase {
public:
    virtual ~EvtRemoteLeaveFailed() = default;
};

/**
 * EvtRequestLeaveSucceeded @ 0x82073134
 * Notification that leave request succeeded.
 */
class EvtRequestLeaveSucceeded : public snNotifyBase {
public:
    virtual ~EvtRequestLeaveSucceeded() = default;
};

/**
 * EvtRequestLeaveFailed @ 0x82073148
 * Notification that leave request failed.
 */
class EvtRequestLeaveFailed : public snNotifyBase {
public:
    virtual ~EvtRequestLeaveFailed() = default;
};

/**
 * EvtAcceptLeaveRequestSucceeded @ 0x8207315C
 * Notification that accepting leave request succeeded.
 */
class EvtAcceptLeaveRequestSucceeded : public snNotifyBase {
public:
    virtual ~EvtAcceptLeaveRequestSucceeded() = default;
};

/**
 * EvtAcceptLeaveRequestFailed @ 0x82073170
 * Notification that accepting leave request failed.
 */
class EvtAcceptLeaveRequestFailed : public snNotifyBase {
public:
    virtual ~EvtAcceptLeaveRequestFailed() = default;
};

/**
 * EvtSessionLeaveSucceeded @ 0x82073184
 * Notification that session leave succeeded.
 */
class EvtSessionLeaveSucceeded : public snNotifyBase {
public:
    virtual ~EvtSessionLeaveSucceeded() = default;
};

/**
 * EvtSessionLeaveFailed @ 0x82073198
 * Notification that session leave failed.
 */
class EvtSessionLeaveFailed : public snNotifyBase {
public:
    virtual ~EvtSessionLeaveFailed() = default;
};

/**
 * EvtLeave @ 0x820731AC
 * Notification that a leave operation is occurring.
 */
class EvtLeave : public snNotifyBase {
public:
    virtual ~EvtLeave() = default;
};

/**
 * EvtRemoteLeave @ 0x820731C0
 * Notification that a remote player is leaving.
 */
class EvtRemoteLeave : public snNotifyBase {
public:
    virtual ~EvtRemoteLeave() = default;
};

/**
 * EvtLeaveRequest @ 0x820731D4
 * Notification of a leave request.
 */
class EvtLeaveRequest : public snNotifyBase {
public:
    virtual ~EvtLeaveRequest() = default;
};

/**
 * EvtLeaveAccepted @ 0x82074440
 * Notification that leave was accepted.
 */
class EvtLeaveAccepted : public snNotifyBase {
public:
    virtual ~EvtLeaveAccepted() = default;
};

/**
 * EvtLeaveDenied @ 0x82074454
 * Notification that leave was denied.
 */
class EvtLeaveDenied : public snNotifyBase {
public:
    virtual ~EvtLeaveDenied() = default;
};

} // namespace rage
