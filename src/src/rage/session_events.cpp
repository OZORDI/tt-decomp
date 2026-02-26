/**
 * session_events.cpp — RAGE Session Event Notification Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implementation of session event notification classes.
 * These are lightweight notification objects used by the RAGE networking
 * layer to signal session state changes.
 *
 * Most event classes have trivial implementations - they exist primarily
 * for type safety and polymorphic dispatch through the snNotifyBase interface.
 *
 * Event Categories:
 * - Session Creation: Host/guest creation, success/failure
 * - Session Start/End: Starting and ending sessions
 * - Session Destroy: Cleanup and destruction
 * - Presence: Xbox Live presence changes
 * - Migration: Host migration events
 * - Connection: Connection lifecycle events
 * - Configuration: Session config changes
 * - Join: Player join operations (local/remote/request/accept)
 * - Leave: Player leave operations (local/remote/request/accept)
 */

#include "rage/session_events.hpp"

namespace rage {

/**
 * snNotifyBase::~snNotifyBase @ 0x823DD8E0 | size: 0x48
 * 
 * Base destructor for session notification events.
 * Sets vtable pointer and optionally frees memory if delete flag is set.
 * 
 * Original assembly shows this is a standard virtual destructor that:
 * 1. Sets vtable to snNotifyBase vtable (0x82072A1C)
 * 2. Checks delete flag (bit 0 of second parameter)
 * 3. Calls rage_free_00C0 if flag is set
 */
snNotifyBase::~snNotifyBase() {
    // Vtable is automatically set by compiler
    // Memory management handled by RAGE allocator
}

// ── Implementation Notes ──
//
// All derived event classes use compiler-generated default destructors.
// The vtable entries for these classes are located in .rdata section:
//
// Join Events (0x82072CF4 - 0x8207439C):
//   - EvtJoinSucceeded, EvtJoinFailed
//   - EvtLocalJoinSucceeded, EvtLocalJoinFailed
//   - EvtRemoteJoinSucceeded, EvtRemoteJoinFailed
//   - EvtRequestJoinSucceeded, EvtRequestJoinFailed
//   - EvtAcceptJoinRequestSucceeded, EvtAcceptJoinRequestFailed
//   - EvtSessionJoinSucceeded, EvtSessionJoinFailed
//   - EvtJoin, EvtRemoteJoin, EvtJoinRequest, EvtJoinReply
//   - EvtJoinAccepted, EvtJoinDenied
//
// Leave Events (0x820730BC - 0x82074454):
//   - EvtLeaveSucceeded, EvtLeaveFailed
//   - EvtLocalLeaveSucceeded, EvtLocalLeaveFailed
//   - EvtRemoteLeaveSucceeded, EvtRemoteLeaveFailed
//   - EvtRequestLeaveSucceeded, EvtRequestLeaveFailed
//   - EvtAcceptLeaveRequestSucceeded, EvtAcceptLeaveRequestFailed
//   - EvtSessionLeaveSucceeded, EvtSessionLeaveFailed
//   - EvtLeave, EvtRemoteLeave, EvtLeaveRequest
//   - EvtLeaveAccepted, EvtLeaveDenied
//
// Each vtable is 20 bytes (0x14) containing:
//   +0x00: Pointer to destructor
//   +0x04: RTTI type descriptor
//   +0x08-0x13: Additional vtable data
//
// These events are used throughout the networking layer to notify
// game code of session state changes. The type system ensures that
// handlers can distinguish between different event types at compile time.

} // namespace rage
