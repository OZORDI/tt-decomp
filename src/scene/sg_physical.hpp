#pragma once
/**
 * sg_physical.hpp — sgPhysicalObject physics-integrated scene node
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 2 vtables — multiple/template inheritance
 * Inherits from: sgRMDrawable (primary), and a secondary render-entry base at +0x60
 */

#include "sg_drawable.hpp"

// Forward declare — full definition in physics/ph_physics.hpp (rage namespace)
namespace rage { struct phInst; }
using rage::phInst;

// Forward declarations for physics types
struct phBound;

// ---------------------------------------------------------------------------
// sgPhysicalObject  —  scene node with integrated physics simulation
//
// Layout (confirmed from recomp of 0x823D9208, 0x823D98C0):
//   inherits sgRMDrawable fields (+0x00..+0x57)
//   +0x58  uint32_t  field_0x0058       — unknown (padding/flags between base and render entry)
//   +0x5C  uint32_t  field_0x005C       — unknown
//   +0x60  void**    m_renderEntry_vtable — secondary vtable for render entry (2nd RTTI @ 0x8204F408)
//   +0x64  float     m_fRenderDistance  — distance to camera (written during visibility check)
//   +0x68  uint32_t  m_nVisibility      — visibility flags from cull check
//   +0x6C  uint32_t  field_0x006C       — unknown (padding)
//   +0x70  uint8_t   m_bIsHidden        — suppresses cleanup call when invisible
//   +0x71  uint8_t   _pad71
//   +0x72  uint8_t   m_bIsArticulated   — true = ragdoll/skeleton physics, false = rigid body
//   +0x73  uint8_t   _pad73
//   +0x74  void*     m_pPhysicsType     — physics type descriptor
//   +0x78  uint32_t  m_nSecondaryData   — copied from phys type +4 on articulated creation
//   +0x7C  phInst*   m_pPhysicsInstance — owning physics instance
// ---------------------------------------------------------------------------
struct sgPhysicalObject : sgRMDrawable {
    uint32_t field_0x0058;         // +0x58  TODO: type unknown
    uint32_t field_0x005C;         // +0x5C  TODO: type unknown
    void**   m_pRenderEntryVtable; // +0x60  secondary vtable (render entry)
    float    m_fRenderDistance;    // +0x64  camera distance for LOD
    uint32_t m_nVisibility;        // +0x68  visibility/cull result
    uint32_t field_0x006C;         // +0x6C  TODO: type unknown
    uint8_t  m_bIsHidden;          // +0x70  suppresses cleanup when invisible
    uint8_t  _pad71;               // +0x71
    uint8_t  m_bIsArticulated;     // +0x72  articulated (ragdoll) flag
    uint8_t  _pad73;               // +0x73
    void*    m_pPhysicsType;       // +0x74  physics type descriptor
    uint32_t m_nSecondaryData;     // +0x78  secondary physics data
    phInst*  m_pPhysicsInstance;   // +0x7C  owning physics instance

    // slot 29 — Transform() / visibility update @ 0x823D9208 (size 0x130)
    //   1. Bail if no physics instance or no bound.
    //   2. Pick transform source: articulation > blended pose > base.
    //   3. Virtual visibility/culling check (phBound slot 9).
    //   4. If visible: store distance + flags, add to draw bucket.
    //   5. If invisible and not m_bIsHidden: call render-entry cleanup (slot 1).
    virtual void Transform();

    // slot 33 — PhysicsUpdate() @ 0x823D9368 (size 0x14)
    //   Tail-calls phInst virtual slot 5 (Update).
    virtual void PhysicsUpdate();

    // slot 39 — AdvancedPhysics() @ 0x823D98C0 (size 0x7C)
    //   Creates physics instance: articulated (ragdoll) or simple (rigid body).
    virtual void AdvancedPhysics();
};
