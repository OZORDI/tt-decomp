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

} // namespace rage
