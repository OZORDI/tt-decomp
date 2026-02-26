/**
 * session_events_extended.hpp — Additional RAGE Session Event Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Extended event notification classes for multiplayer session management.
 * These 47 additional event classes complete the session event system.
 *
 * Base class: snNotifyBase (defined in session_events.hpp)
 * All event classes inherit from snNotifyBase and provide type-safe
 * event notifications for session state changes.
 */
#pragma once

#include "session_events.hpp"

namespace rage {

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
 * Notification of join operation.
 */
class EvtJoin : public snNotifyBase {
public:
    virtual ~EvtJoin() = default;
};

/**
 * EvtRemoteJoin @ 0x82072DF8
 * Notification of remote player join.
 */
class EvtRemoteJoin : public snNotifyBase {
public:
    virtual ~EvtRemoteJoin() = default;
};

/**
 * EvtJoinRequest @ 0x82072E0C
 * Notification of join request.
 */
class EvtJoinRequest : public snNotifyBase {
public:
    virtual ~EvtJoinRequest() = default;
};

/**
 * EvtJoinReply @ 0x82072E20
 * Notification of join reply.
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
 * Notification of leave operation.
 */
class EvtLeave : public snNotifyBase {
public:
    virtual ~EvtLeave() = default;
};

/**
 * EvtRemoteLeave @ 0x820731C0
 * Notification of remote player leave.
 */
class EvtRemoteLeave : public snNotifyBase {
public:
    virtual ~EvtRemoteLeave() = default;
};

/**
 * EvtLeaveRequest @ 0x820731D4
 * Notification of leave request.
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

// ── Migration Events ──

/**
 * EvtMigrateSession @ 0x82073444
 * Notification to migrate session.
 */
class EvtMigrateSession : public snNotifyBase {
public:
    virtual ~EvtMigrateSession() = default;
};

/**
 * EvtAcquireHostSucceeded @ 0x82073458
 * Notification that acquiring host role succeeded.
 */
class EvtAcquireHostSucceeded : public snNotifyBase {
public:
    virtual ~EvtAcquireHostSucceeded() = default;
};

/**
 * EvtAcquireHostFailed @ 0x8207346C
 * Notification that acquiring host role failed.
 */
class EvtAcquireHostFailed : public snNotifyBase {
public:
    virtual ~EvtAcquireHostFailed() = default;
};

/**
 * EvtMigrateAsGuestSucceeded @ 0x82073480
 * Notification that migrating as guest succeeded.
 */
class EvtMigrateAsGuestSucceeded : public snNotifyBase {
public:
    virtual ~EvtMigrateAsGuestSucceeded() = default;
};

/**
 * EvtMigrateAsGuestFailed @ 0x82073494
 * Notification that migrating as guest failed.
 */
class EvtMigrateAsGuestFailed : public snNotifyBase {
public:
    virtual ~EvtMigrateAsGuestFailed() = default;
};

/**
 * EvtDoGuestMigrate @ 0x82074540
 * Notification to perform guest migration.
 */
class EvtDoGuestMigrate : public snNotifyBase {
public:
    virtual ~EvtDoGuestMigrate() = default;
};

/**
 * EvtSessionMigrateSucceeded @ 0x82074554
 * Notification that session migration succeeded.
 */
class EvtSessionMigrateSucceeded : public snNotifyBase {
public:
    virtual ~EvtSessionMigrateSucceeded() = default;
};

/**
 * EvtSessionMigrateFailed @ 0x82074568
 * Notification that session migration failed.
 */
class EvtSessionMigrateFailed : public snNotifyBase {
public:
    virtual ~EvtSessionMigrateFailed() = default;
};

// ── Arbitration Events ──

/**
 * EvtArbGuestRegister @ 0x820733E4
 * Notification to register guest with arbitration.
 */
class EvtArbGuestRegister : public snNotifyBase {
public:
    virtual ~EvtArbGuestRegister() = default;
};

/**
 * EvtArbRegisterSucceeded @ 0x820746BC
 * Notification that arbitration registration succeeded.
 */
class EvtArbRegisterSucceeded : public snNotifyBase {
public:
    virtual ~EvtArbRegisterSucceeded() = default;
};

/**
 * EvtArbRegisterFailed @ 0x820746D0
 * Notification that arbitration registration failed.
 */
class EvtArbRegisterFailed : public snNotifyBase {
public:
    virtual ~EvtArbRegisterFailed() = default;
};

// ── Configuration Events ──

/**
 * EvtReceivedConfig @ 0x820744E4
 * Notification that configuration was received.
 */
class EvtReceivedConfig : public snNotifyBase {
public:
    virtual ~EvtReceivedConfig() = default;
};

} // namespace rage
