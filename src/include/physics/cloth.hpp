/**
 * cloth.hpp — Cloth Physics System Declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * RAGE engine cloth simulation using Verlet integration.
 * Handles character clothing, hair, and environmental cloth objects.
 *
 * Classes:
 *   phClothVerletInst     — Physics instance for a single Verlet cloth mesh
 *   phClothVerletBehavior — Behavior controller: manages simulation state,
 *                           binds cloth instances to creature skeletons
 *   environmentCloth      — Scene-level container for up to 6 cloth objects
 *   clothController       — High-level coordinator for cloth simulation
 *
 * All classes inherit from rage::datBase (vtable @ 0x820276C4).
 */

#pragma once

#include <cstdint>

// Forward declarations
class phBound;
class phInst;

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// VerletClothState — Inline state block for cloth instance tracking
//
// Two of these live at +0x10 and +0x18 inside phClothVerletBehavior.
// Each tracks a cloth instance/frame pair that the behavior is attached to.
//
// Layout (8 bytes):
//   +0x00  m_pDataArray   (uint32_t) — pointer to array of per-frame data entries
//   +0x04  m_entryCount   (uint16_t) — number of entries currently stored
//   +0x06  m_refCount     (uint16_t) — reference/generation counter (0 = inactive)
// ─────────────────────────────────────────────────────────────────────────────
struct VerletClothState {
    uint32_t     m_pDataArray;        // +0x00: pointer to entry array
    uint16_t     m_entryCount;        // +0x04: current entry count
    uint16_t     m_refCount;          // +0x06: ref/generation count (0 = unused)
};

// ─────────────────────────────────────────────────────────────────────────────
// phClothVerletInst  [vtable @ 0x82071BAC]
//
// Physics instance for a single Verlet-integrated cloth mesh.
// Inherits from rage::datBase.
// Only has a destructor in the vtable — the simulation logic is driven
// by the behavior controller.
// ─────────────────────────────────────────────────────────────────────────────
class phClothVerletInst {
public:
    virtual ~phClothVerletInst();  // [0] @ 0x823D7C90 | size: 0x50
};

// ─────────────────────────────────────────────────────────────────────────────
// phClothVerletBehavior  [vtable @ 0x82079010]
//
// Behavior controller that drives cloth simulation for one Verlet instance.
// Binds cloth instances to creature skeleton frames, copies transforms,
// and manages simulation state per-frame.
//
// Layout (32+ bytes):
//   +0x00  vtable pointer
//   +0x04  m_pPhysicsInst     — phInst* associated physics instance
//   +0x08  m_pClothBound      — phBound* cloth collision bound
//   +0x0C  m_bEnabled         — uint8_t: cloth simulation enabled flag
//   +0x10  m_stateA           — VerletClothState: instance tracking block A
//   +0x18  m_stateB           — VerletClothState: instance tracking block B
//   +0x14  m_halfword20       — uint16_t at +0x14 (within stateA, overlapping)
// ─────────────────────────────────────────────────────────────────────────────
class phClothVerletBehavior {
public:
    virtual ~phClothVerletBehavior();                  // [0] @ 0x82421F20 | size: 0x50
    virtual void Reset();                              // [1] @ 0x823D7C50 | size: 0x40
    // slots 2-4: inherited / unused
    virtual void UpdateSimulation();                   // [5] @ 0x82421FD8 | size: 0x188
    virtual void AttachToCreatureInst(void* physFrameTable);
                                                       // [6] @ 0x82422160 | size: 0x15C

    // ── Data members ──
    void*            m_pPhysicsInst;    // +0x04: owning physics instance
    void*            m_pClothBound;     // +0x08: cloth collision bound (phBound*)
    uint8_t          m_bEnabled;        // +0x0C: simulation enabled
    uint8_t          _pad0D[3];         // +0x0D: alignment padding
    VerletClothState m_stateA;          // +0x10: cloth tracking state A
    VerletClothState m_stateB;          // +0x18: cloth tracking state B
};

// ─────────────────────────────────────────────────────────────────────────────
// environmentCloth  [vtable @ 0x82071C34]
//
// Scene-level container for up to 6 cloth objects (flags, banners, nets).
// Inherits from rage::datBase.
// The destructor calls virtual delete on each non-null child.
// ─────────────────────────────────────────────────────────────────────────────
class environmentCloth {
public:
    virtual ~environmentCloth();  // [0] @ 0x823D7CE0 | size: 0x50

    // ── Data members ──
    void* m_pClothObjects[6];     // +0x04..+0x1C: child cloth object pointers
};

// ─────────────────────────────────────────────────────────────────────────────
// clothController  [vtable @ 0x82071C40]
//
// High-level controller that coordinates cloth simulation instances.
// Inherits from rage::datBase.
// ─────────────────────────────────────────────────────────────────────────────
class clothController {
public:
    virtual ~clothController();  // [0] @ 0x823D86C0 | size: 0x50
};

} // namespace rage
