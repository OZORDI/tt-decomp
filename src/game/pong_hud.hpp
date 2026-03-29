/**
 * pong_hud.hpp — Game-level HUD class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Classes:
 *   pongHUDLive — Xbox Live HUD message sink (inherits msgMsgSink)
 *
 * Hierarchy (from xex_excavation_tt/class_hierarchy.txt):
 *   msgMsgSink
 *     └─ pongHUDLive          — Xbox Live presence / HUD notifications
 *     └─ pongLiveManager      — Live session management
 *     └─ pongLookAtController — Gaze tracking
 *     └─ pongPresence         — Profile presence state
 *     └─ pongGame             — Core game logic
 *     └─ pongFrontendLive     — Frontend Live integration
 *
 * Sources: rtti_vtable_map | xex_excavation_tt (class_hierarchy.txt,
 *          classes_with_methods.txt, vtables_with_addrs.txt)
 */
#pragma once
#include <stdint.h>

// ── pongHUDLive  [vtable @ 0x82078EB1] ──────────────────────────
//
// Lightweight msgMsgSink subclass for Xbox Live HUD notifications.
// Single vtable slot [0] = nullsub_1 (no-op destructor).
// RTTI: .?AVpongHUDLive@@ (type descriptor @ 0x825DA83C)
// No field accesses, no xrefs to vtable — likely instantiated
// indirectly or used as a marker/tag type for message routing.
//
// Parent class msgMsgSink (vtable @ 0x820278B8) has 1 virtual:
//   [0] sub_82430180  — message dispatch
//
struct pongHUDLive {
    void**      vtable;           // +0x00  (msgMsgSink vtable)
};
