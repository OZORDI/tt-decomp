#pragma once
/**
 * sg_physical.hpp — sgPhysicalObject physics-integrated scene node
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 2 vtables — multiple/template inheritance
 * Inherits from: sgRMDrawable (primary), and at least one physics base
 */

#include "sg_drawable.hpp"

// Forward declare — full definition in physics/ph_physics.hpp (rage namespace)
namespace rage { struct phInst; }
using rage::phInst;

// ---------------------------------------------------------------------------
// sgPhysicalObject  —  scene node with integrated physics simulation
//
// Layout (partially confirmed from recomp of 0x823D9208):
//   inherits sgRMDrawable fields (+0x00..+0x57)
//   +0x58  phInst*  m_pPhysicsInstance  — owning physics instance
//   +0x5C  bool     m_isHidden          — suppresses cleanup call when invisible
// ---------------------------------------------------------------------------
struct sgPhysicalObject : sgRMDrawable {
    phInst*  m_pPhysicsInstance;  // +0x58
    bool     m_isHidden;          // +0x5C

    // slot 29 — Transform() / visibility update @ 0x823D9208 (size 0x130)
    //   1. Bail if no physics instance.
    //   2. Retrieve collision bound and transform from phInst.
    //   3. Virtual visibility/culling check (slot 9).
    //   4. If visible → add to draw bucket via pongDrawBucket_AddEntry.
    //   5. If invisible and not m_isHidden → call cleanup (slot 1).
    virtual void Transform();

    // slot 33 — PhysicsUpdate() @ 0x823D9368 (size 0x14)
    virtual void PhysicsUpdate();

    // slot 39 — AdvancedPhysics() @ 0x823D98C0 (size 0x7C)
    //   Handles ragdoll, articulation, and other complex physics.
    virtual void AdvancedPhysics();
};
