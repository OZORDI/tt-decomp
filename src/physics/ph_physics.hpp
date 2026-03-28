/**
 * ph_physics.hpp — Physics system class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 43 classes from the RAGE physics subsystem (ph* prefix).
 * All vtable addresses verified against rtti_vtable_map.json.
 *
 * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map
 *          | debug_string_field_map | rtti_class_hierarchy
 *
 * Naming conventions (RAGE engine):
 *   - Members prefixed with m_
 *   - Pointers:  m_p*
 *   - Counts:    m_n*
 *   - Flags:     m_b* (bool) or m_flags
 *   - Virtual method slots that have not yet been named retain vfn_N comments.
 */
#pragma once
#include <stdint.h>
#include <stddef.h>

// ── phDemoObject  [vtable @ 0x82056C3C] ──────────────────────────────────────
struct phDemoObject {
    void**      vtable;           // +0x00

    virtual void Render();        // [2] @ 0x8227CF50
};

// ── phDemoWorld  [vtable @ 0x82056E40] ──────────────────────────────────────
struct phDemoWorld {
    void**      vtable;           // +0x00

    virtual ~phDemoWorld();                   // [0] @ 0x8227E920
    virtual void ScalarDtor(int flags);       // [1] @ 0x8227E810
    virtual void vfn_2();                     // [2] @ 0x8227E548  — TODO: name
    virtual void vfn_3();                     // [3] @ 0x8227E5C8
    virtual void vfn_5();                     // [5] @ 0x8227F378
    virtual void vfn_6();                     // [6] @ 0x8227E468
    virtual void vfn_7();                     // [7] @ 0x8227EF30
    virtual void vfn_14();                    // [14] @ 0x8247E518
    virtual void vfn_17();                    // [17] @ 0x8247E4C0
    virtual void vfn_18();                    // [18] @ 0x8247E790
    virtual void vfn_19();                    // [19] @ 0x8247E5F8
    virtual void vfn_20();                    // [20] @ 0x8247E260
    virtual void vfn_21();                    // [21] @ 0x8247EAF8
    virtual void vfn_26();                    // [26] @ 0x8247EB10
    virtual void vfn_33();                    // [33] @ 0x8247E988
    virtual void vfn_34();                    // [34] @ 0x8247E9A0
    virtual void vfn_35();                    // [35] @ 0x8247EB00
    virtual void vfn_36();                    // [36] @ 0x8247EA18
    virtual void vfn_37();                    // [37] @ 0x8247EB18
    virtual void vfn_40();                    // [40] @ 0x8247EB20
    virtual void vfn_42();                    // [42] @ 0x8247F600
};

// ── phMaterialMgrImpl  [vtable @ 0x8204EAEC] ─────────────────────────────────
struct phMaterialMgrImpl {
    void**      vtable;           // +0x00

    virtual ~phMaterialMgrImpl();             // [0] @ 0x8222A7E8
    virtual void ScalarDtor(int flags);       // [1] @ 0x8222A040
    virtual void vfn_2();                     // [2] @ 0x8222A460
    virtual void vfn_3();                     // [3] @ 0x8222A4A8
    virtual void vfn_4();                     // [4] @ 0x8222A4B8
    virtual void vfn_8();                     // [8] @ 0x8222A550
    virtual void vfn_9();                     // [9] @ 0x8222A5B8
    virtual void vfn_10();                    // [10] @ 0x8222A5C0
    virtual void vfn_11();                    // [11] @ 0x8222A640
    virtual void vfn_12();                    // [12] @ 0x8222A670
};

// ── phUpdateObject  [vtable @ 0x82027AC0] ────────────────────────────────────
//
// Base class for objects that participate in per-frame physics updates.
// Maintains a double-buffered transform array indexed by the global frame
// counter (g_phFrameIndex @ 0x825C4898).
//
// Struct layout (confirmed from scaffold analysis):
//   +0x00  vtable pointer
//   +0x04  unknown/padding
//   +0x10  inline 4x4 local-to-world matrix (4×16 bytes = 64 bytes)
//   +0x14  m_pData          — ptr to external data block; NULL = inactive
//   +0x18  inline matrix row 1 (part of local transform)
//   +0x50  inline matrix end / additional state
//   +0x60  m_pStateOwner    — ptr to a state-owning object (offset 96)
//   +0x6C  m_pState         — ptr to linked state struct (offset 108)
//   +0x80  m_pFrameBuffer   — double-buffered frame array base (offset 128)
//   +0x85  m_bActive        — activity flag (offset 133)
struct phUpdateObject {
    void**      vtable;           // +0x00

    uint8_t     _pad0x04[16];
    void*       m_pData;          // +0x14  Non-NULL when object has active state
    uint8_t     _pad0x18[104];
    void*       m_pFrameBuffer;   // +0x80  Base of double-buffered per-frame array
    uint8_t     _pad0x84[1];
    uint8_t     m_bActive;        // +0x85  Object is active / enabled

    virtual ~phUpdateObject();                // [0] @ 0x8227DA08
    virtual void ScalarDtor(int flags);       // [1] @ 0x8227D960
    virtual void Update();                    // [2] @ 0x820C41E8  — per-frame update
    virtual void vfn_3();                     // [3] @ 0x8227D5B0  — AddToWorld / step
    virtual void vfn_4();                     // [4] @ 0x8227D768
    virtual void vfn_5();                     // [5] @ 0x8227D898
    virtual void Cleanup(void* frameData, uint32_t flags);  // [6] @ 0x8227DA40
};

namespace rage {

// ── rage::clothController  [vtable @ 0x82071C40] ─────────────────────────────
struct clothController {
    void**      vtable;           // +0x00

    virtual ~clothController();   // [0] @ 0x823D86C0
};

// ── rage::environmentCloth  [vtable @ 0x82071C34] ────────────────────────────
struct environmentCloth {
    void**      vtable;           // +0x00

    virtual ~environmentCloth();  // [0] @ 0x823D7CE0
};

// ── rage::phArchetype  [vtable @ 0x82059B4C] ─────────────────────────────────
// Confirmed methods: Load
struct phArchetype {
    void**      vtable;           // +0x00

    virtual ~phArchetype();                   // [0] @ 0x822C9918
    virtual void vfn_2();                     // [2] @ 0x822C94E0
    virtual void vfn_3();                     // [3] @ 0x822C9968
    virtual void vfn_4();                     // [4] @ 0x822C9A28
    virtual void vfn_5();                     // [5] @ 0x822C9C18
    virtual void vfn_6();                     // [6] @ 0x822C9CD0
    virtual void vfn_9();                     // [9] @ 0x822C9500
    virtual void vfn_11();                    // [11] @ 0x822C9510

    void Load();  // debug string: "phArchetype::Load() -- .phys file %s does not exist"
};

// ── rage::phArchetypeBase  [vtable @ 0x82059B40] ─────────────────────────────
struct phArchetypeBase {
    void**      vtable;           // +0x00

    virtual ~phArchetypeBase();   // [0] @ 0x822C9498
};

// ── rage::phArchetypeDamp  [vtable @ 0x82059BEC] ─────────────────────────────
struct phArchetypeDamp {
    void**      vtable;           // +0x00

    virtual void vfn_2();         // [2] @ 0x822C9600
    virtual void vfn_3();         // [3] @ 0x822CA3D8
    virtual void vfn_4();         // [4] @ 0x822CA438
    virtual void vfn_5();         // [5] @ 0x822CA680
};

// ── rage::phArchetypeMgr  [vtable @ 0x82059F18] ──────────────────────────────
struct phArchetypeMgr {
    void**      vtable;           // +0x00
};

// ── rage::phArchetypePhys  [vtable @ 0x82059B9C] ─────────────────────────────
struct phArchetypePhys {
    void**      vtable;           // +0x00

    virtual void vfn_2();         // [2] @ 0x822C9558
    virtual void vfn_3();         // [3] @ 0x822C9F38
    virtual void vfn_4();         // [4] @ 0x822C9F90
    virtual void vfn_5();         // [5] @ 0x822CA288
    virtual void vfn_9();         // [9] @ 0x82380920
    virtual void vfn_10();        // [10] @ 0x822C95F8
    virtual void vfn_11();        // [11] @ 0x822CA278
    virtual void vfn_13();        // [13] @ 0x822C9568
    virtual void vfn_14();        // [14] @ 0x822CA238
    virtual void vfn_15();        // [15] @ 0x822C95F0
};

// ── rage::phArticulatedCollider  [vtable @ 0x8204FD74] ───────────────────────
struct phArticulatedCollider {
    void**      vtable;           // +0x00

    // ── field access clusters ──
    uint32_t    m_state;          // +0x0004  R:61  — primary state word
    uint32_t    field_0x0008;     // +0x0008  R:10
    uint32_t    field_0x000c;     // +0x000c  R:2
    uint32_t    field_0x0010;     // +0x0010  R:8
    uint32_t    field_0x0014;     // +0x0014  R:4
    uint32_t    field_0x0018;     // +0x0018  R:3
    uint32_t    field_0x0020;     // +0x0020  R:3
    uint32_t    field_0x0024;     // +0x0024  R:3
    uint32_t    field_0x0028;     // +0x0028  R:3 W:1
    uint8_t     _pad0x002c[8];
    uint32_t    field_0x0034;     // +0x0034  W:1
    uint8_t     _pad0x0038[44];
    uint32_t    field_0x0064;     // +0x0064  R:3
    uint32_t    field_0x0068;     // +0x0068  R:1
    uint8_t     _pad0x006c[56];
    uint32_t    field_0x00a4;     // +0x00a4  R:1
    uint8_t     _pad0x00a8[72];
    uint32_t    field_0x00f0;     // +0x00f0  R:1 W:1
    uint32_t    field_0x00f4;     // +0x00f4  R:1 W:1
    uint8_t     _pad0x00f8[184];
    uint32_t    field_0x01b0;     // +0x01b0  R:1
    uint32_t    field_0x01b4;     // +0x01b4  R:1
    uint8_t     _pad0x01b8[24];
    union {
        uint32_t    m_nActiveJoints;  // +0x01d0  R:50  — joint count (most-read field)
        uint32_t*   m_pJointArray;    // +0x01d0  — same field, used as pointer
    };
    union {
        uint8_t     m_bJointsActive;  // +0x01d4  W:3   — joints active flag
        uint8_t     m_bJointDirtyFlag; // +0x01d4  — alias
    };
    union {
        uint32_t    m_nJointCount;    // +0x01d8  R:8 W:2  — joint count
        void*       m_pJointStatePtr; // +0x01d8  — alias (used as pointer)
        void*       m_pJointProcessor;// +0x01d8  — alias (joint processor)
    };
    uint32_t    field_0x01dc;     // +0x01dc  R:8
    uint16_t    field_0x01e2;     // +0x01e2  R:1
    uint32_t    field_0x01e4;     // +0x01e4  R:8
    uint32_t    field_0x01e8;     // +0x01e8  R:3
    uint16_t    field_0x01ea;     // +0x01ea  R:1
    uint32_t    field_0x01ec;     // +0x01ec  R:5
    uint16_t    field_0x01f2;     // +0x01f2  R:1
    uint32_t    field_0x01f4;     // +0x01f4  R:4
    uint16_t    field_0x01fa;     // +0x01fa  R:1
    uint32_t    field_0x01fc;     // +0x01fc  R:5
    uint32_t    field_0x0200;     // +0x0200  R:1
    uint16_t    field_0x0202;     // +0x0202  R:1
    uint8_t     _pad0x0204[94];
    uint32_t    field_0x0264;     // +0x0264  R:1
    uint8_t     _pad0x0268[156];
    uint32_t    field_0x0304;     // +0x0304  R:1
    uint16_t    field_0x030a;     // +0x030a  R:1
    uint32_t    field_0x030c;     // +0x030c  R:1
    uint16_t    field_0x0312;     // +0x0312  R:1
    uint8_t     _pad0x0314[9606];
    uint32_t    field_0x289c;     // +0x289c  R:1
    uint8_t     _pad0x28a0[1048];
    uint32_t    m_nComponents;    // +0x2cb8  R:14 W:6 — component count
    uint32_t    m_nActiveComponents; // +0x2cbc  R:12 W:3
    uint32_t    field_0x2cc4;     // +0x2cc4  R:1 W:1
    uint32_t    field_0x2ccc;     // +0x2ccc  R:1 W:1
    uint32_t    field_0x2cd0;     // +0x2cd0  R:1 W:1
    uint32_t    field_0x2cd4;     // +0x2cd4  R:1 W:1
    uint32_t    field_0x2cd8;     // +0x2cd8  W:1
    uint8_t     _pad0x2cdc[20];
    uint32_t    field_0x2cf0;     // +0x2cf0  R:1 W:1
    uint8_t     _pad0x2cf4[36];
    uint32_t    field_0x2d18;     // +0x2d18  W:1
    uint32_t    field_0x2d1c;     // +0x2d1c  R:4 W:4
    uint32_t    field_0x2d20;     // +0x2d20  R:1 W:1
    uint32_t    field_0x2d24;     // +0x2d24  R:1 W:1
    uint32_t    field_0x2d28;     // +0x2d28  R:1 W:1
    uint32_t    field_0x2d2c;     // +0x2d2c  R:1 W:1
    uint8_t     _pad0x2d30[13];
    uint8_t     field_0x2d3d;     // +0x2d3d  W:1
    uint8_t     field_0x2d3e;     // +0x2d3e  W:1
    uint8_t     field_0x2d3f;     // +0x2d3f  W:1
    uint8_t     field_0x2d41;     // +0x2d41  W:1
    uint8_t     field_0x2d42;     // +0x2d42  W:1
    uint8_t     field_0x2d43;     // +0x2d43  W:1
    uint32_t    field_0x2d44;     // +0x2d44  R:1 W:1
    uint8_t     _pad0x2d48[36];
    uint32_t    m_nJoints;        // +0x2d6c  R:16 W:8 — joint array count
    uint32_t    field_0x2d70;     // +0x2d70  R:10 W:8
    uint32_t    field_0x2d74;     // +0x2d74  R:22 W:11
    uint32_t    field_0x2d78;     // +0x2d78  W:8
    uint32_t    field_0x2d7c;     // +0x2d7c  R:8 W:4
    uint32_t    field_0x2d80;     // +0x2d80  R:6 W:3
    uint8_t     field_0x2d82;     // +0x2d82  W:1
    uint32_t    field_0x2d84;     // +0x2d84  R:3 W:2
    uint32_t    field_0x2d88;     // +0x2d88  R:10 W:8
    uint32_t    field_0x2d8c;     // +0x2d8c  R:1 W:1
    uint8_t     _pad0x2d90[8];
    uint32_t    field_0x2d98;     // +0x2d98  W:8
    uint32_t    field_0x2d9c;     // +0x2d9c  W:8
    uint32_t    field_0x2da0;     // +0x2da0  W:8
    uint16_t    field_0x2da4;     // +0x2da4  W:1
    uint16_t    field_0x2da6;     // +0x2da6  W:1
    uint16_t    field_0x2da8;     // +0x2da8  W:1
    uint16_t    field_0x2daa;     // +0x2daa  W:1
    uint8_t     _pad0x2dac[10];
    uint32_t    field_0x2db8;     // +0x2db8  R:2 W:1
    uint32_t    field_0x2dbc;     // +0x2dbc  W:1
    uint32_t    field_0x2dc0;     // +0x2dc0  W:1
    uint8_t     _pad0x2dc4[60];
    uint32_t    field_0x2e00;     // +0x2e00  R:2 W:1
    uint32_t    field_0x2e04;     // +0x2e04  W:1
    uint32_t    field_0x2e08;     // +0x2e08  W:1
    uint32_t    field_0x2e0c;     // +0x2e0c  W:1
    uint32_t    field_0x2e10;     // +0x2e10  W:1
    uint8_t     _pad0x2e14[44];
    uint32_t    field_0x2e40;     // +0x2e40  W:1
    uint8_t     _pad0x2e44[76];
    uint32_t    field_0x2e90;     // +0x2e90  R:2 W:1
    uint32_t    field_0x2e94;     // +0x2e94  R:1 W:1
    uint32_t    field_0x2e98;     // +0x2e98  R:1 W:1
    uint32_t    field_0x2e9c;     // +0x2e9c  R:1 W:1
    uint8_t     _pad0x2ea0[16];
    uint32_t    field_0x2eb0;     // +0x2eb0  W:1
    uint32_t    field_0x2eb4;     // +0x2eb4  W:1
    uint32_t    field_0x2eb8;     // +0x2eb8  W:1
    uint32_t    field_0x2ebc;     // +0x2ebc  W:1
    uint32_t    field_0x2ec0;     // +0x2ec0  W:1
    uint8_t     _pad0x2ec4[16];
    uint32_t    field_0x2ed4;     // +0x2ed4  W:1
    uint32_t    field_0x2ed8;     // +0x2ed8  W:1
    uint32_t    field_0x2edc;     // +0x2edc  W:1
    uint8_t     _pad0x2ee0[124];
    uint32_t    field_0x2f5c;     // +0x2f5c  W:1
    uint32_t    field_0x2f60;     // +0x2f60  W:1
    uint32_t    field_0x2f64;     // +0x2f64  W:1
    uint32_t    field_0x2f68;     // +0x2f68  W:1
    uint32_t    field_0x2f6c;     // +0x2f6c  W:1
    uint32_t    field_0x2f70;     // +0x2f70  W:1
    uint32_t    field_0x2f74;     // +0x2f74  W:1
    uint32_t    field_0x2f78;     // +0x2f78  W:1
    uint32_t    field_0x2f7c;     // +0x2f7c  W:1
    uint8_t     _pad0x2f80[376];
    uint32_t    field_0x30f8;     // +0x30f8  R:2
    uint32_t    field_0x30fc;     // +0x30fc  R:2
    uint32_t    field_0x3100;     // +0x3100  R:2
    uint32_t    field_0x3104;     // +0x3104  R:2
    uint8_t     _pad0x3108[1160];
    uint32_t    field_0x3590;     // +0x3590  R:1 W:1

    virtual ~phArticulatedCollider();                 // [0] @ 0x8224E580
    virtual void ScalarDtor(int flags);               // [1] @ 0x8224E6D8
    virtual void ResetForces();                       // [2] @ 0x8224E720  — zero force vecs, call ResetVelocity
    virtual void InitFromArchetype();                 // [3] @ 0x8224E1D0  — init collider from archetype
    virtual void ResetVelocity();                     // [4] @ 0x8224E7A0  — reset velocity state
    virtual void Step();                              // [5] @ 0x8224E7C8  — advance simulation step
    virtual void UpdateBounds();                      // [6] @ 0x8224EA28  — update bound geometry
    virtual void ApplyGravity();                      // [7] @ 0x8224EA70  — apply gravity to joints
    virtual void IntegrateVelocities();               // [8] @ 0x8224EC58  — integrate joint velocities
    virtual void PreCollide();                        // [9] @ 0x8224EBC8  — pre-collision setup
    virtual void GetTransform();                      // [10] @ 0x822CB250 — get world transform matrix
    virtual void SetTransform();                      // [11] @ 0x822CB360 — set world transform matrix
    virtual void GetLinearVelocity();                 // [12] @ 0x8224FF70
    virtual void SetLinearVelocity();                 // [13] @ 0x822500E8
    virtual void GetAngularVelocity();                // [14] @ 0x82250170
    virtual void SetAngularVelocity();                // [15] @ 0x822501F8
    virtual void ApplyImpulse();                      // [17] @ 0x822505B0
    virtual void ApplyForce();                        // [18] @ 0x82250288
    virtual void ApplyTorque();                       // [19] @ 0x82250370
    virtual void ApplyForceAtPoint();                 // [20] @ 0x82250440
    virtual void GetInverseInertiaWorld();            // [21] @ 0x822CBF30
    virtual void GetMass();                           // [22] @ 0x8224F220
    virtual void GetInverseMass();                    // [23] @ 0x8224F228
    virtual void ComputeInertia();                    // [24] @ 0x822CC700
    virtual void CastRay();                           // [25] @ 0x8224FD50
    virtual void CollideDispatch();                   // [27] @ 0x8224FD58
    virtual void CollideVsBound();                    // [28] @ 0x8224FDA8
    virtual void CollideVsGeometry();                 // [29] @ 0x8224FE40
    virtual void CollideVsQuadtree();                 // [30] @ 0x8224FED8
    virtual void CollideVsOTGrid();                   // [31] @ 0x8224FA98
    virtual void CollideVsRibbon();                   // [32] @ 0x8224FB90
    virtual void ComputeSupportDistance();            // [33] @ 0x8224FD30
    virtual void CopyFrom();                          // [34] @ 0x8224F990
    virtual void Clone();                             // [35] @ 0x8224FC60
    virtual void GetPosition();                       // [36] @ 0x8224EFA0
    virtual void SetPosition();                       // [37] @ 0x8224EFC8
    virtual void GetOrientation();                    // [38] @ 0x8224EFE0
    virtual void SetOrientation();                    // [39] @ 0x822504A8
    virtual void GetBound();                          // [40] @ 0x8224F870
    virtual void SetBound();                          // [41] @ 0x8224F8C8
    virtual void GetArchetype();                      // [42] @ 0x8224F910
    virtual void SetArchetype();                      // [43] @ 0x8224F948
    virtual void GetPhysicsData();                    // [46] @ 0x8224E248 — returns ptr at +472
    virtual void ApplyImpulseAtJoint();               // [47] @ 0x82250528
    virtual void ApplyForceAtJoint();                 // [48] @ 0x82250620
    virtual void ApplyTorqueAtJoint();                // [49] @ 0x82250710
    virtual void ApplyLinearImpulseAtJoint();         // [50] @ 0x82250578
    virtual void ApplyAngularImpulseAtJoint();        // [51] @ 0x822505E8
    virtual void GetJointLinearVelocity();            // [52] @ 0x82250818
    virtual void GetJointAngularVelocity();           // [53] @ 0x82250870
    virtual void GetJointForce();                     // [54] @ 0x82250770
    virtual void GetJointTorque();                    // [55] @ 0x822508A8
    virtual void GetJointMassAtIndex();               // [56] @ 0x8224E250 — float from array +508
    virtual void SetJointLinearVelocity();            // [57] @ 0x822508E0
    virtual void SetJointAngularVelocity();           // [58] @ 0x82250910
    virtual void GetJointCount();                     // [59] @ 0x8224F218
    virtual void GetJointInertiaAtIndex();            // [60] @ 0x8224E260 — float from array +492
    virtual void GetJointDampingAtIndex();            // [61] @ 0x8224E270 — float from array +500
    virtual void GetJointFrictionAtIndex();           // [62] @ 0x8224E280 — float from array
    virtual void GetJointTransform();                 // [63] @ 0x8224E2D8

    // Non-virtual methods
    void* GetActiveJointsPointer();
    void DelegateToJointProcessor();
    void DelegateToCapsuleHandler();
    void DelegateToColliderHandler();
    float GetJointMass(int jointIndex);
    float GetJointInertia(int jointIndex);
    float GetJointDamping(int jointIndex);
    void Update();
    void AddForceToJoint(const float* forceVector);
    void ApplyImpulse(void* param1, void* param2, void* param3);
    void ScalarDestructor();
    void ResetActiveJoints();
    void ApplyScaledGravity(float scale);
    void AccumulateScaledForceX(float scale);
    void AccumulateScaledForceY(float scale);
    void AccumulateScaledForceZ(float scale);
    void SaveAndClearJointForces();
    void NormalizeVector(const float* src, float* dst);
    void SetTimestep(float timestep);
    void SetVelocityScaled(const float* velocity);
    void SetAngularVelocity(const float* angularVelocity);
    void UpdatePostIntegrate();
    void CopyJointTransformMatrix(float* outMatrix, int boneIndex);
    void CopyJointVelocityMatrix(float* outMatrix, int boneIndex);
    void CopyJointRotationMatrix(float* outMatrix, int boneIndex);
    void ApplyDeltaPositionToJoint(int param, const float* targetPos, const float* altTarget, int solverParam);
    void GetJointBackupForce(float* outVec, phArticulatedCollider* srcCollider, int boneIndex);
    void AccumulateScaledJointForces(float* outVec, float scale);
    void GetJointOrientationVector(float* outVec, int boneIndex);
    void ApplyDeltaToJointForce(const float* targetPos, int boneIndex);
    void ResetJointConstraintState(int boneIndex);
    void DispatchJointSlot12(int jointIndex, float* outVec);
    void DispatchJointSlot7(int jointIndex);
    void DelegateToBoundCapsule();
    void DispatchJointSlot5(int jointIndex);
    float GetJointStiffness(int jointIndex);
    void AccumulateForceAtJoint(const float* forceVec, int boneIndex);
    void AccumulateTorqueAtRootJoint(const float* forceVec);
    void ComputeJointCrossProduct(float* outVec, const float* targetPos, phArticulatedCollider* otherCollider, int boneIndex);
    // Bitfield accessors
    uint32_t GetSolverParam_0(); uint32_t GetSolverParam_3(); uint32_t GetSolverParam_11();
    uint32_t GetSolverParam_14(); uint32_t GetSolverParam_17(); uint32_t GetSolverParam_21();
    uint32_t GetRotAxis_0(); uint32_t GetRotAxis_3(); uint32_t GetRotAxis_6();
    uint32_t GetRotAxis_9(); uint32_t GetRotAxis_12(); uint32_t GetRotAxis_15();
    uint32_t GetRotAxis_18(); uint32_t GetRotAxis_21(); uint32_t GetRotAxis_24();
    uint32_t GetRotAxis_27(); uint32_t GetRotAxis_30();
    uint32_t GetConstraint_0(); uint32_t GetConstraint_4(); uint32_t GetConstraint_8();
    uint32_t GetConstraint_12(); uint32_t GetConstraint_16(); uint32_t GetConstraint_20();
    uint32_t GetConstraint_24(); uint32_t GetConstraint_28();
    uint32_t GetFlag7C_0(); uint32_t GetFlag7C_1(); uint32_t GetFlag7C_4();
    uint32_t GetFlag7C_4b(); uint32_t GetFlag7C_5(); uint32_t GetFlag7C_Byte();
    uint32_t GetActive_0(); uint32_t GetActive_1(); uint32_t GetActive_2();
    uint32_t GetActive_2b(); uint32_t GetActive_3(); uint32_t GetActive_5();
    uint32_t GetDofFlag_0(); uint32_t GetLimitType_0(); uint32_t GetSolverFlag_18();
    uint32_t GetCBCField_23();
    uint32_t GetIndexedByteFlag_0(int index); uint32_t GetIndexedByteFlag_1(int index);
    uint32_t GetIndexedByteFlag_2(int index);
    uint32_t GetIndexedField(int index, int shift, int mask);
    uint32_t GetBoneField_5778(int index); uint32_t GetBoneField_5B90(int index);
    uint32_t GetBoneField_5BE0(int index); uint32_t GetBoneField_5C30(int index);
    uint32_t GetBoneField_5C88(int index); uint32_t GetBoneField_5CE0(int index);
    uint32_t GetBoneField_5D38(int index); uint32_t GetBoneField_5D90(int index);
    uint32_t GetBoneFieldNeg_5DF0(int index);
    bool IsBoneFieldNonZero(int index); bool IsElementActive(int index);
    float GetInterpolatedLimit(int index); float GetScaledFloat();
    // Setters
    void SetActive_2b(uint32_t value); void SetActive_3(uint32_t value);
    void SetActive_5(uint32_t value); void SetActive_11648(uint32_t value);
    void SetActiveByte_11650(uint8_t value);
    void SetConstraint_0b(uint32_t value); void SetConstraint_4b(uint32_t value);
    void SetConstraint_8b(uint32_t value); void SetConstraint_12b(uint32_t value);
    void SetConstraint_28b(uint32_t value); void SetConstraint_11628(uint32_t value);
    void SetCBCField_23(uint32_t value);
    void SetDofFlag_0(uint32_t value); void SetDofU16_11840(uint32_t value);
    void SetFlag7C_4b(uint32_t value); void SetFlag7C_24(uint32_t value);
    void SetFlags_11644(uint32_t value);
    void SetFourConsecutive(uint32_t value);
    void SetJointParam_0(uint32_t value); void SetJointParam_28(uint32_t value);
    void SetLimitFloat_11708(float value); void SetLimitFloat_11712(float value);
    void SetLimitType_0(uint32_t value);
    void SetMiscField_11652(uint32_t value); void SetField_11544(uint32_t value);
    void SetRangeFloat_11780(float value); void SetRangeFloat_11784(float value);
    void SetRangeFloat_11788(float value); void SetRangeFloat_11792(float value);
    void SetRotAxis_2(uint32_t value); void SetRotAxis_8(uint32_t value);
    void SetRotAxis_14(uint32_t value); void SetRotAxis_20(uint32_t value);
    void SetRotAxis_26(uint32_t value);
    void SetSolverParam_11636(uint32_t value); void SetSolverParam_15(uint32_t value);
    void SetSolverParam_21(uint32_t value);
    void ClearJointForces();
    void DispatchConstraintSolver(void* param1, void* param2);
    void AccumulateJointForce(int jointParam, const float* forceVec);
    void PostIntegrate();
    void Reset();
    void ClearForces();
    void UpdateChildJoints();
    void ApplyScaledForce(float scaleFactor);
    void SetVelocityAndSync(const float* velocityVec);
    void SyncAfterBaseUpdate();
    void Shutdown();
    void ApplyScaledMatrixRowX(float scale);
    void ApplyScaledMatrixRowY(float scale);
    void ApplyScaledMatrixRowZ(float scale);
    void SetupJointConstraints();
    void DispatchJointUpdate(int index);
    void DispatchJointSolve(int index);
};

// ── rage::phBound  [vtable @ 0x82057EF4] ─────────────────────────────────────
// Confirmed methods: Load_v110
struct phBound {
    void**      vtable;           // +0x00

    virtual void SetType(uint8_t type);          // [1] @ 0x8228CE18
    virtual uint8_t GetType();                    // [2] @ 0x8228CE20
    virtual void CalcAABB();                       // [3] @ 0x8228D0E8  — transform AABB by matrix
    virtual void SetCentroidOffset();              // [6] @ 0x8228D178  — store centroid, set flag
    virtual void GetInertia();                     // [9] @ 0x8228CE38  — write inertia to output
    virtual void DebugDraw();                      // [14] @ 0x8228D2F0 — draw via display object
    virtual void CastRay();                        // [25] @ 0x8228D450 — ray/bound intersection
    virtual void CollideDispatch();                // [27] @ 0x8228D978 — dispatch by collision type
    virtual float GetMargin(void*, bool useOuter); // [33] @ 0x8228D9F8
    virtual void CopyFrom();                       // [34] @ 0x8228DA18 — copy bound from source
    virtual void Clone();                          // [35] @ 0x8228DB08 — allocate and copy bound
    virtual void LoadFromStream();                 // [37] @ 0x8228D438 — load bound version 110

    // debug string: "phBound::Load_v110 - not defined for this bound type (%d)"
    void Load_v110();
};

// ── rage::phBoundBox  [vtable @ 0x82058B8C] ──────────────────────────────────
// Confirmed methods: FindImpactsToBox
// Field hints: m_maxImpacts:int
struct phBoundBox {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:10 W:1
    uint8_t     field_0x0005;     // +0x0005  R:2 W:1
    uint8_t     field_0x0006;     // +0x0006  W:2
    uint32_t    field_0x0008;     // +0x0008  R:11 W:2
    uint32_t    field_0x000c;     // +0x000c  W:1
    uint32_t    field_0x0010;     // +0x0010  R:1
    uint32_t    field_0x0014;     // +0x0014  R:1
    uint32_t    field_0x0018;     // +0x0018  R:1
    uint32_t    field_0x0020;     // +0x0020  R:1
    uint32_t    field_0x0024;     // +0x0024  R:1
    uint32_t    field_0x0028;     // +0x0028  R:1
    uint8_t     _pad0x002c[52];
    uint16_t    field_0x0060;     // +0x0060  W:1
    uint8_t     _pad0x0062[12];
    uint32_t    field_0x0070;     // +0x0070  R:3 W:1
    uint32_t    field_0x0078;     // +0x0078  R:2 W:1
    uint8_t     _pad0x007c[24];
    uint32_t    field_0x0094;     // +0x0094  R:2
    uint32_t    field_0x0098;     // +0x0098  R:2
    uint32_t    field_0x00a0;     // +0x00a0  R:1
    uint32_t    field_0x00a4;     // +0x00a4  R:1
    uint32_t    field_0x00a8;     // +0x00a8  R:1
    uint8_t     _pad0x00ac[324];
    uint32_t    m_maxImpacts;     // +0x01f0  R:4 W:2 — max collision impacts
    uint8_t     _pad0x01f4[22944];
    uint32_t    field_0x5b94;     // +0x5b94  W:2

    virtual void SetCentroidOffset();     // [6] @ 0x822A77E0
    virtual void TranslateAndDispatch(); // [7] @ 0x822A7898
    virtual void GetVolume();            // [8] @ 0x822A6A10
    virtual void GetSupportPoint();      // [9] @ 0x822A7AA0
    virtual void UpdateBound();          // [11] @ 0x822A69E0
    virtual void GetMaterialIndex();     // [12] @ 0x822A6A00
    virtual void SetMaterialIndex();     // [13] @ 0x822A6A08
    virtual void ContainsPoint();        // [18] @ 0x822AACF0
    virtual void TestSegment();          // [19] @ 0x822AAEB0
    virtual void TestMovingSphere();     // [20] @ 0x822AB018
    virtual void TestAABBOverlap();      // [21] @ 0x822AB2E0
    virtual void TestSweep();            // [22] @ 0x822ABA68
    virtual void CollideVsBound();       // [28] @ 0x822A7B10
    virtual void ComputeSupportDistance(); // [33] @ 0x822A7A00
    virtual void CopyFrom();             // [34] @ 0x822B99F0
    virtual void Deserialize();          // [36] @ 0x822A75D0
    virtual void RecalcBounds();         // [37] @ 0x822A78D0

    // debug string: "phBoundBox::FindImpactsToBox() -- MAX_NUM_IMPACTS %d exceeded"
    void FindImpactsToBox();
};

// ── rage::phBoundCapsule  [vtable @ 0x82058AAC] ───────────────────────────────
struct phBoundCapsule {
    void**      vtable;           // +0x00

    uint8_t     field_0x0001;     // +0x0001  R:1
    uint32_t    m_nType;          // +0x0004  R:72 W:20  — bound type / flags
    uint8_t     m_bHasOffset;     // +0x0005  R:5 W:1  — set when center has non-zero offset
    uint8_t     field_0x0006;     // +0x0006  W:2
    uint8_t     m_bHasTransform;  // +0x0007  R:1  — checked before applying world transform
    uint64_t    field_0x0008;     // +0x0008  R:59 W:21  — packed pair
    uint32_t    field_0x000c;     // +0x000c  R:20 W:10
    uint32_t    field_0x0010;     // +0x0010  R:14 W:8
    uint8_t     field_0x0011;     // +0x0011  W:3
    uint8_t     field_0x0012;     // +0x0012  R:1 W:2
    uint8_t     field_0x0013;     // +0x0013  R:2
    uint32_t    field_0x0014;     // +0x0014  R:24 W:6
    uint32_t    field_0x0018;     // +0x0018  R:20 W:8
    uint8_t     field_0x0019;     // +0x0019  R:1
    uint32_t    field_0x001c;     // +0x001c  R:20 W:6
    uint16_t    field_0x001e;     // +0x001e  W:1
    uint32_t    field_0x0020;     // +0x0020  R:13 W:5
    uint16_t    field_0x0022;     // +0x0022  R:27 W:2
    uint32_t    field_0x0024;     // +0x0024  R:14 W:13
    uint32_t    field_0x0028;     // +0x0028  R:19 W:21
    uint8_t     field_0x002a;     // +0x002a  W:1
    uint32_t    field_0x002c;     // +0x002c  R:13 W:3
    uint32_t    m_vCenterX;       // +0x0030  R:15 W:4  — center position X
    uint32_t    m_vCenterY;       // +0x0034  R:10 W:3  — center position Y
    uint32_t    m_vCenterZ;       // +0x0038  R:7 W:3   — center position Z
    uint8_t     field_0x0039;     // +0x0039  R:1 W:1
    uint32_t    field_0x003c;     // +0x003c  R:13 W:4
    uint32_t    field_0x0040;     // +0x0040  R:9 W:4
    uint32_t    field_0x0044;     // +0x0044  R:3 W:2
    uint32_t    field_0x0048;     // +0x0048  W:2
    uint32_t    field_0x004c;     // +0x004c  W:2
    uint32_t    field_0x0050;     // +0x0050  R:8 W:3
    uint32_t    field_0x0054;     // +0x0054  R:2 W:3
    uint32_t    field_0x0058;     // +0x0058  R:3 W:3
    uint32_t    field_0x005c;     // +0x005c  R:1 W:3
    uint32_t    field_0x0060;     // +0x0060  R:7 W:5
    uint32_t    field_0x0064;     // +0x0064  R:2 W:2
    uint32_t    field_0x0068;     // +0x0068  R:2 W:3
    uint32_t    field_0x006c;     // +0x006c  R:1 W:4
    uint16_t    field_0x006e;     // +0x006e  R:7 W:2
    uint32_t    m_pVertices;      // +0x0070  R:17 W:2  — vertex array ptr (or halfHeight for capsule)
    uint32_t    field_0x0074;     // +0x0074  R:7 W:3
    uint32_t    field_0x0078;     // +0x0078  R:3 W:2
    uint32_t    field_0x007c;     // +0x007c  R:1 W:1
    uint32_t    m_fRadius;        // +0x0080  R:12 W:1  — capsule radius (float as uint32)
    uint32_t    field_0x0084;     // +0x0084  R:2 W:1
    uint32_t    field_0x0088;     // +0x0088  R:1
    uint32_t    field_0x008c;     // +0x008c  R:7 W:1
    uint32_t    field_0x0090;     // +0x0090  R:8 W:1
    uint32_t    m_nVertexCount;   // +0x0094  R:2 W:1  — number of vertices
    uint32_t    field_0x0098;     // +0x0098  R:4 W:1
    uint16_t    field_0x009a;     // +0x009a  W:1
    uint32_t    field_0x009c;     // +0x009c  R:2 W:2
    uint8_t     field_0x009d;     // +0x009d  R:1
    uint8_t     field_0x009e;     // +0x009e  W:3
    uint32_t    m_pMaterials;     // +0x00a0  R:2 W:1  — pointer to material array
    uint32_t    m_pCurrentMaterial; // +0x00a4  R:3 W:1  — current material pointer
    uint64_t    m_nMaterialCount; // +0x00a8  R:2 W:1  — material count (uint8 low byte)
    uint32_t    field_0x00ac;     // +0x00ac  R:2
    uint64_t    field_0x00b0;     // +0x00b0  R:2 W:2
    uint32_t    field_0x00b4;     // +0x00b4  R:1
    uint64_t    field_0x00b8;     // +0x00b8  R:1 W:2
    uint32_t    m_nMaterialIndex; // +0x00c0  R:8 W:4  — active material index
    uint8_t     field_0x00c8;     // +0x00c8  W:1
    uint8_t     field_0x00c9;     // +0x00c9  W:1
    uint16_t    field_0x00ca;     // +0x00ca  W:1
    uint32_t    field_0x00cc;     // +0x00cc  R:1 W:1
    uint32_t    field_0x00d0;     // +0x00d0  W:1
    uint16_t    field_0x00d2;     // +0x00d2  W:1
    uint32_t    field_0x00d4;     // +0x00d4  W:5
    uint32_t    field_0x00d8;     // +0x00d8  R:1 W:5
    uint32_t    field_0x00dc;     // +0x00dc  W:4
    uint8_t     field_0x00dd;     // +0x00dd  W:1
    uint8_t     field_0x00de;     // +0x00de  W:1
    uint32_t    field_0x00e0;     // +0x00e0  W:2
    uint32_t    field_0x00e4;     // +0x00e4  R:3 W:8
    uint8_t     field_0x00e5;     // +0x00e5  W:1
    uint32_t    field_0x00e8;     // +0x00e8  W:3
    uint32_t    field_0x00ec;     // +0x00ec  W:4
    uint8_t     field_0x00ed;     // +0x00ed  W:1
    uint8_t     field_0x00ee;     // +0x00ee  W:1
    uint32_t    field_0x00f0;     // +0x00f0  W:4
    uint32_t    field_0x00f4;     // +0x00f4  R:6 W:4
    uint32_t    field_0x00f8;     // +0x00f8  W:3
    uint32_t    field_0x00fc;     // +0x00fc  R:3 W:3
    uint32_t    m_vAxisTop;       // +0x0100  R:15 W:2  — capsule axis top endpoint
    uint32_t    field_0x0104;     // +0x0104  R:1 W:2
    uint32_t    field_0x0108;     // +0x0108  R:1 W:2
    uint32_t    field_0x010c;     // +0x010c  R:5 W:2
    uint32_t    field_0x0110;     // +0x0110  R:1 W:3
    uint32_t    field_0x0114;     // +0x0114  R:2 W:4
    uint32_t    field_0x0118;     // +0x0118  R:2 W:3
    uint32_t    field_0x011c;     // +0x011c  W:2
    uint32_t    field_0x0120;     // +0x0120  W:5
    uint32_t    field_0x0124;     // +0x0124  W:3
    uint32_t    field_0x0128;     // +0x0128  R:1 W:4
    uint8_t     field_0x0129;     // +0x0129  W:1
    uint32_t    field_0x012c;     // +0x012c  R:1 W:4
    uint32_t    field_0x0130;     // +0x0130  W:1
    uint32_t    field_0x0134;     // +0x0134  W:1
    uint32_t    field_0x0138;     // +0x0138  W:1
    uint32_t    field_0x013c;     // +0x013c  W:1
    uint32_t    field_0x0140;     // +0x0140  R:12 W:1
    uint32_t    field_0x0144;     // +0x0144  R:3 W:1
    uint32_t    field_0x0148;     // +0x0148  R:2 W:1
    uint32_t    field_0x014c;     // +0x014c  R:1 W:1
    uint32_t    field_0x0150;     // +0x0150  R:1 W:1
    uint32_t    field_0x0154;     // +0x0154  R:3 W:1
    uint32_t    field_0x0158;     // +0x0158  R:2 W:1
    uint32_t    field_0x015c;     // +0x015c  R:3 W:1
    uint32_t    field_0x0160;     // +0x0160  W:1
    uint32_t    field_0x0164;     // +0x0164  R:1 W:1
    uint32_t    field_0x0168;     // +0x0168  R:1 W:1
    uint32_t    field_0x016c;     // +0x016c  R:1 W:1
    uint32_t    field_0x0170;     // +0x0170  R:1 W:1
    uint32_t    field_0x0174;     // +0x0174  W:2
    uint32_t    field_0x0178;     // +0x0178  W:1
    uint32_t    field_0x017c;     // +0x017c  W:1
    uint32_t    field_0x0184;     // +0x0184  R:2 W:3
    uint32_t    field_0x0188;     // +0x0188  R:1 W:3
    uint32_t    field_0x018c;     // +0x018c  W:18
    uint32_t    field_0x0190;     // +0x0190  R:1 W:1
    uint32_t    field_0x0194;     // +0x0194  R:1 W:1
    uint32_t    field_0x0198;     // +0x0198  W:5
    uint32_t    field_0x019c;     // +0x019c  W:1
    uint32_t    field_0x01a0;     // +0x01a0  W:1
    uint32_t    field_0x01a4;     // +0x01a4  W:1
    uint32_t    field_0x01a8;     // +0x01a8  W:1
    uint32_t    field_0x01ac;     // +0x01ac  W:1
    uint32_t    field_0x01b0;     // +0x01b0  W:1
    uint32_t    field_0x01b4;     // +0x01b4  W:1
    uint32_t    field_0x01b8;     // +0x01b8  W:2
    uint32_t    field_0x01bc;     // +0x01bc  R:2 W:3
    uint32_t    field_0x01c0;     // +0x01c0  R:4 W:3
    uint32_t    field_0x01c4;     // +0x01c4  R:1 W:6
    uint32_t    field_0x01c8;     // +0x01c8  R:17 W:6
    uint32_t    field_0x01cc;     // +0x01cc  R:4 W:3
    uint32_t    field_0x01d0;     // +0x01d0  R:7 W:4
    uint32_t    field_0x01d4;     // +0x01d4  R:4 W:4
    uint32_t    field_0x01d8;     // +0x01d8  W:4
    uint32_t    field_0x01dc;     // +0x01dc  W:4
    uint32_t    field_0x01e0;     // +0x01e0  W:1
    uint32_t    field_0x01e4;     // +0x01e4  W:1
    uint32_t    field_0x01e8;     // +0x01e8  W:6
    uint32_t    field_0x01ec;     // +0x01ec  W:1
    uint32_t    field_0x01f0;     // +0x01f0  R:2 W:3
    uint32_t    field_0x01f4;     // +0x01f4  W:2
    uint32_t    field_0x01f8;     // +0x01f8  W:2
    uint32_t    field_0x01fc;     // +0x01fc  W:2
    uint32_t    field_0x0200;     // +0x0200  R:1 W:2
    uint32_t    field_0x0204;     // +0x0204  R:1 W:3
    uint32_t    field_0x0208;     // +0x0208  R:1 W:1
    uint32_t    field_0x020c;     // +0x020c  W:1
    uint8_t     _pad0x0210[12];
    uint32_t    field_0x021c;     // +0x021c  W:1
    uint32_t    field_0x0220;     // +0x0220  W:1
    uint32_t    field_0x0224;     // +0x0224  W:1
    uint32_t    field_0x0228;     // +0x0228  W:1
    uint32_t    field_0x022c;     // +0x022c  W:1
    uint32_t    field_0x0230;     // +0x0230  W:1
    uint32_t    field_0x0234;     // +0x0234  W:1
    uint32_t    field_0x0238;     // +0x0238  W:1
    uint32_t    field_0x023c;     // +0x023c  W:1
    uint32_t    field_0x0240;     // +0x0240  W:1
    uint16_t    field_0x0244;     // +0x0244  R:2 W:1
    uint32_t    field_0x0248;     // +0x0248  R:1 W:1
    uint32_t    field_0x024c;     // +0x024c  W:2
    uint32_t    field_0x0254;     // +0x0254  W:1
    uint32_t    field_0x0258;     // +0x0258  W:2
    uint32_t    field_0x025c;     // +0x025c  W:2
    uint32_t    field_0x0260;     // +0x0260  W:1
    uint32_t    field_0x0264;     // +0x0264  W:1
    uint32_t    field_0x0268;     // +0x0268  W:1
    uint32_t    field_0x026c;     // +0x026c  W:1
    uint32_t    field_0x0270;     // +0x0270  W:2
    uint8_t     _pad0x0274[28];
    uint32_t    field_0x0290;     // +0x0290  W:1
    uint8_t     _pad0x0294[32];
    uint32_t    field_0x02b4;     // +0x02b4  W:1
    uint32_t    field_0x02b8;     // +0x02b8  W:1
    uint8_t     _pad0x02bc[20];
    uint32_t    field_0x02d0;     // +0x02d0  R:12 W:1
    uint32_t    field_0x02d4;     // +0x02d4  R:12 W:1
    uint8_t     _pad0x02d8[12];
    uint32_t    field_0x02e4;     // +0x02e4  W:1
    uint32_t    field_0x02e8;     // +0x02e8  W:2
    uint8_t     _pad0x02ec[36];
    uint32_t    field_0x0310;     // +0x0310  W:1
    uint8_t     _pad0x0314[28];
    uint8_t     field_0x0330;     // +0x0330  W:1
    uint8_t     field_0x0331;     // +0x0331  W:1
    uint8_t     field_0x0332;     // +0x0332  W:1
    uint8_t     field_0x0333;     // +0x0333  W:1
    uint32_t    field_0x0334;     // +0x0334  W:2
    uint8_t     field_0x0335;     // +0x0335  W:1
    uint32_t    field_0x0338;     // +0x0338  R:3
    uint32_t    field_0x033c;     // +0x033c  W:1
    uint32_t    field_0x0340;     // +0x0340  R:1 W:1
    uint32_t    field_0x0344;     // +0x0344  R:1 W:1
    uint32_t    field_0x0348;     // +0x0348  W:1
    uint8_t     _pad0x034c[24];
    uint32_t    field_0x0364;     // +0x0364  R:1
    uint32_t    field_0x0368;     // +0x0368  R:1
    uint32_t    field_0x036c;     // +0x036c  R:1
    uint32_t    field_0x0370;     // +0x0370  R:1
    uint32_t    field_0x0374;     // +0x0374  R:1
    uint32_t    field_0x0378;     // +0x0378  R:1
    uint32_t    field_0x037c;     // +0x037c  R:1
    uint32_t    field_0x0380;     // +0x0380  R:1
    uint8_t     _pad0x0384[190];
    uint8_t     field_0x0442;     // +0x0442  W:2
    uint8_t     _pad0x0443[26];
    uint32_t    field_0x0460;     // +0x0460  R:1
    uint8_t     _pad0x0464[5012];
    uint8_t     field_0x17f8;     // +0x17f8  W:2
    uint8_t     _pad0x17f9[3464];
    uint32_t    field_0x2584;     // +0x2584  R:2
    uint8_t     _pad0x2588[32];
    uint32_t    field_0x25a8;     // +0x25a8  R:2
    uint8_t     _pad0x25ac[260];
    uint32_t    field_0x26b0;     // +0x26b0  R:1
    uint8_t     _pad0x26b4[2480];
    uint32_t    field_0x3064;     // +0x3064  W:1
    uint32_t    field_0x306c;     // +0x306c  W:1
    uint8_t     _pad0x3070[36];
    uint32_t    field_0x3094;     // +0x3094  W:1
    uint8_t     _pad0x3098[16796];
    uint32_t    field_0x7234;     // +0x7234  R:1

    virtual void CalcAABB();              // [3] @ 0x822A30C8  — extends base with capsule extents
    virtual void TranslateAndDispatch(); // [7] @ 0x8233AB50
    virtual void GetVolume();            // [8] @ 0x822A2DB0
    virtual void GetSupportPoint();      // [9] @ 0x822A3258
    virtual void UpdateBound();          // [11] @ 0x822A2DE0
    virtual void GetMaterialIndex();     // [12] @ 0x8256FBD0
    virtual void SetMaterialIndex();     // [13] @ 0x822A2E00
    virtual void ContainsPoint();        // [18] @ 0x822A32B8 — test point inside capsule
    virtual void TestSegment();          // [19] @ 0x822A3490 — segment vs capsule
    virtual void TestMovingSphere();     // [20] @ 0x822A3648 — moving sphere vs capsule
    virtual void TestAABBOverlap();      // [21] @ 0x822A3948 — AABB overlap test
    virtual void TestSweep();            // [22] @ 0x822A3DB0 — sweep with contact gen
    virtual void GetClosestPoint();      // [23] @ 0x822A3C70 — closest point on capsule
    virtual void TestSphereOverlap();    // [24] @ 0x822A3B10 — sphere overlap test
    virtual void CollideVsBound();       // [28] @ 0x822A48D8 — capsule vs bound collision
    virtual void CollideVsCapsule();     // [29] @ 0x822A4DC8 — capsule vs capsule collision
    virtual void ComputeSupportDistance(); // [33] @ 0x822A3268
    virtual void CopyFrom();             // [34] @ 0x822A2F28 — copy capsule from source
    virtual void Deserialize();          // [36] @ 0x822A6828 — read data from stream
    virtual void RecalcBounds();         // [37] @ 0x822A2FE8 — recompute AABB extents

    // Non-virtual methods
    int32_t ComputeFixedPointDotProduct();  // @ 0x824C35C8
    float GetRadius() const;
    void ScaleRadius(float scale);
    void ComputeBounds(float scale, float* outMin, float* outMax);
    float ComputeExtent(float param1, float param2) const;
    void InitializeAxisAlignedX(float halfLength, float* outMatrix);
    void InitializeAxisAlignedY(float halfLength, float* outMatrix);
    void InitializeAxisAlignedZ(float halfLength, float* outMatrix);
    void TransformByDirection(float halfLength, const float* direction, float* outMatrix);
    void InitializeFromDirection(float halfLength, const float* direction, float* outMatrix);
    void ApplyTransform(const float* transform, const float* inPoints);
    void SetMaterialIndex(uint32_t index);
    uint32_t GetMaterialIndex() const;
    float GetVolume() const;
    void GetSupportPoint(void* direction, void* outPoint);
    void CopyFrom(const phBoundCapsule* source);
    int CheckValueSign() const;
    int CheckScaledValueSign() const;
    void SetupCapsuleFromState();
    float SumFloatArray(int count) const;
    void RotateVectors();
    void RotateVectorsAlt();
    int NormalizeVector2D(float* outVec) const;
    void LookupTableValues(void* outValues);
    void SetActiveState(int stateIndex);
    void DispatchCapsuleFromCamera(void* param1, void* param2);
    int32_t TestPointContainment(const float* point);
    void InitializeCapsule(float halfHeight, float radius);
    uint32_t GetPolygonCount() const;
    void SetPolygonCount(uint32_t count);
    void GetExtents(float* outExtents, const void* params);
    void RenderDebug();
    float CalculateVolume() const;
    void SetCentroidOffset(const float* offset);
    float GetBoundingDistance(const float* direction, uint8_t includeCenter) const;
    void ValidateAndSetExtents(const float* matrix);
    void GetJointLimitsForAxis(uint32_t jointIndex, uint32_t axis, float* outMin, float* outMax);
};

// ── rage::phBoundComposite  [vtable @ 0x82057FD4] ────────────────────────────
struct phBoundComposite {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:32
    uint8_t     field_0x0007;     // +0x0007  R:1
    uint32_t    m_nChildren;      // +0x0008  R:15 W:15  — child bound count
    uint8_t     field_0x000d;     // +0x000d  R:30  — packed flags (heavily read)
    uint32_t    field_0x0014;     // +0x0014  R:30
    uint32_t    field_0x0018;     // +0x0018  R:30
    uint32_t    field_0x001c;     // +0x001c  R:15 W:15
    uint32_t    field_0x0024;     // +0x0024  R:15 W:15
    uint32_t    field_0x0028;     // +0x0028  R:15
    uint8_t     _pad0x002c[8];
    uint32_t    field_0x0034;     // +0x0034  R:7 W:11
    uint8_t     _pad0x0038[40];
    uint16_t    field_0x0060;     // +0x0060  W:1
    uint8_t     _pad0x0062[12];
    uint32_t    field_0x0070;     // +0x0070  R:13 W:2
    uint32_t    field_0x0074;     // +0x0074  R:2 W:2
    uint32_t    field_0x0078;     // +0x0078  R:2 W:2
    uint32_t    field_0x007c;     // +0x007c  R:1 W:2
    uint16_t    field_0x0080;     // +0x0080  R:2 W:2
    uint16_t    m_nMaxChildren;   // +0x0082  R:13 W:1  — allocated capacity

    virtual ~phBoundComposite();            // [0] @ 0x8228DF00
    virtual void CalcAABB();                // [3] @ 0x8228E1E0
    virtual void GetVolume();               // [8] @ 0x8228EE00
    virtual void GetSupportPoint();         // [9] @ 0x8228EE78
    virtual void* GetFirstChildCentroid();  // [10] @ 0x82290808
    virtual void UpdateChildBounds();       // [11] @ 0x822906F0
    virtual void RebuildChildBounds();      // [12] @ 0x82290788
    virtual void SetAllMaterials();         // [13] @ 0x82290830
    virtual void DebugDraw();               // [14] @ 0x82290898
    virtual void SelectMaterialForRender(); // [15] @ 0x82290988
    virtual void GetMaterialCount();        // [16] @ 0x82290918
    virtual void ContainsPointComposite();  // [17] @ 0x8228F668
    virtual void ContainsPoint();           // [18] @ 0x8228F910
    virtual void TestSegment();             // [19] @ 0x8228FB60
    virtual void TestMovingSphere();        // [20] @ 0x8228FDE0
    virtual void TestAABBOverlap();         // [21] @ 0x822901C0
    virtual void TestSweep();               // [22] @ 0x82290428
    virtual void CollideDispatch();         // [27] @ 0x822909F0
    virtual void CalculateExtents();        // [28] @ 0x8228DDA0
    virtual void CopyFrom();               // [34] @ 0x8228F6C0
    virtual void Clone();                   // [35] @ 0x8228F870
    virtual void Deserialize();             // [36] @ 0x8228E2B0
    virtual void SetDefaultFlags();         // [37] @ 0x8228E820
    virtual void GetChildMaterialIndex();   // [38] @ 0x82290708
    virtual void SetChildMaterialIndex();   // [39] @ 0x822907A0
};

// ── rage::phBoundGeomCullable  [vtable @ 0x82058E7C] ─────────────────────────
struct phBoundGeomCullable {
    void**      vtable;           // +0x00

    virtual ~phBoundGeomCullable();  // [0] @ 0x822BB2D8
};

// ── rage::phBoundGeometry  [vtable @ 0x82058494] ─────────────────────────────
// Confirmed methods: Load_v110
struct phBoundGeometry {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:31 W:3
    uint8_t     field_0x0005;     // +0x0005  R:1 W:2
    uint8_t     field_0x0007;     // +0x0007  R:1
    uint32_t    m_nFaces;         // +0x0008  R:17 W:15  — face/polygon count
    uint32_t    field_0x000c;     // +0x000c  W:1
    uint8_t     field_0x000d;     // +0x000d  R:30  — packed flags
    uint8_t     field_0x000f;     // +0x000f  R:1
    uint16_t    field_0x0010;     // +0x0010  R:1
    uint16_t    field_0x0012;     // +0x0012  R:1
    uint32_t    field_0x0014;     // +0x0014  R:32 W:2
    uint16_t    field_0x0016;     // +0x0016  R:1
    uint32_t    field_0x0018;     // +0x0018  R:31 W:2
    uint32_t    field_0x001c;     // +0x001c  R:15 W:15
    uint32_t    field_0x0024;     // +0x0024  R:16 W:17
    uint32_t    field_0x0028;     // +0x0028  R:16 W:2
    uint8_t     _pad0x002c[52];
    uint16_t    field_0x0060;     // +0x0060  W:1
    uint8_t     _pad0x0062[12];
    uint32_t    m_pVertices;      // +0x0070  R:20 W:2  — pointer to vertex array
    uint32_t    field_0x0074;     // +0x0074  R:2 W:1
    uint32_t    field_0x0078;     // +0x0078  R:11 W:2
    uint32_t    field_0x007c;     // +0x007c  R:4 W:1
    uint8_t     field_0x0080;     // +0x0080  R:2 W:1
    uint8_t     field_0x0081;     // +0x0081  W:1
    uint8_t     field_0x0082;     // +0x0082  W:1
    uint8_t     field_0x0083;     // +0x0083  W:1
    uint32_t    field_0x0084;     // +0x0084  R:1 W:1
    uint32_t    field_0x0088;     // +0x0088  R:1 W:1
    uint32_t    field_0x008c;     // +0x008c  W:1
    uint32_t    field_0x0090;     // +0x0090  W:1
    uint32_t    m_nVertexCount;   // +0x0094  R:12 W:1  — number of vertices
    uint32_t    field_0x0098;     // +0x0098  R:11 W:1
    uint8_t     field_0x009c;     // +0x009c  W:1
    uint8_t     field_0x009d;     // +0x009d  W:1
    uint8_t     field_0x009e;     // +0x009e  W:1
    uint8_t     field_0x009f;     // +0x009f  W:1
    uint32_t    m_pMaterials;     // +0x00a0  R:5 W:2  — pointer to material array
    uint32_t    m_pCurrentMaterial; // +0x00a4  R:2 W:2  — current material ptr
    uint8_t     m_nMaterialCount; // +0x00a8  R:7 W:1  — number of materials
    uint8_t     field_0x00a9;     // +0x00a9  W:1
    uint8_t     field_0x00aa;     // +0x00aa  W:1
    uint8_t     field_0x00ab;     // +0x00ab  W:1
    uint8_t     field_0x00ac;     // +0x00ac  W:1
    uint8_t     field_0x00ad;     // +0x00ad  W:1
    uint8_t     field_0x00ae;     // +0x00ae  W:1
    uint8_t     field_0x00af;     // +0x00af  W:1
    uint32_t    field_0x00b0;     // +0x00b0  R:5
    uint32_t    field_0x00b8;     // +0x00b8  R:4
    uint16_t    field_0x00bc;     // +0x00bc  R:2
    uint32_t    field_0x00c0;     // +0x00c0  W:1
    uint32_t    field_0x00c4;     // +0x00c4  R:4 W:2
    uint8_t     _pad0x00c8[16856];
    uint32_t    field_0x42a0;     // +0x42a0  R:1
    uint32_t    field_0x42a4;     // +0x42a4  R:1

    virtual ~phBoundGeometry();               // [0] @ 0x82291008
    virtual void CalcAABB();                  // [3] @ 0x822B00D0
    virtual void SetCentroidOffset();         // [6] @ 0x82293D88 — CheckBoundsAndUpdate
    virtual void TranslateAndDispatch();      // [7] @ 0x82293E10 — UpdateBounds
    virtual void GetVolume();                 // [8] @ 0x822B9468
    virtual void GetSupportPoint();           // [9] @ 0x822B9918
    virtual void GetMaterialCount();          // [10] @ 0x82290F60
    virtual void UpdateBound();               // [11] @ 0x822912A8
    virtual void GetMaterialAtIndex();        // [12] @ 0x82290F68
    virtual void SetAllMaterials();           // [13] @ 0x82293D50
    virtual void DebugDraw();                 // [14] @ 0x82293BE8 — RenderDebugGeometry
    virtual void SelectMaterialForRender();   // [15] @ 0x82293CF8
    virtual void GetMaterialCountAlt();       // [16] @ 0x82293C48
    virtual void ContainsPoint();             // [18] @ 0x822B9AD8
    virtual void TestSegment();               // [19] @ 0x822B9D40
    virtual void TestMovingSphere();          // [20] @ 0x822B9DE8
    virtual void TestAABBOverlap();           // [21] @ 0x822BA048
    virtual void TestSweep();                 // [22] @ 0x822BA0B0
    virtual void CollideVsBound();            // [28] @ 0x822B08A8
    virtual void CollideVsGeometry();         // [29] @ 0x822B0F10
    virtual void CollideVsQuadtree();         // [30] @ 0x822AD8C8
    virtual void CollideVsOTGrid();           // [31] @ 0x822B29B0
    virtual void ComputeSupportDistance();    // [33] @ 0x822B0818
    virtual void CopyFrom();                  // [34] @ 0x822941D0
    virtual void Deserialize();               // [36] @ 0x82291738
    virtual void RecalcBounds();              // [37] @ 0x82293E98
    virtual void GetTriangleData();           // [38] @ 0x822B94D0
    virtual void TestSweepTriangles();        // [39] @ 0x822BA530
    virtual void IsConvex();                  // [40] @ 0x82291260

    // debug string: "phBoundGeometry::Load_v110(%s) -- 'centroid:' not supported"
    void Load_v110();

    // Non-virtual methods
    void* GetDisplayObject();
    uint8_t GetMaterialCount() const;
    void* GetMaterialAtIndex(int index) const;
    void SetAllMaterials(void* material);
    void RenderDebugGeometry();
    void SelectMaterialForRendering();
    void CheckBoundsAndUpdate(const float* point);
    void UpdateBounds(const float* offset);
    void Destructor(int shouldFree);
    bool CallVTableSlot37();
};

// ── rage::phBoundOTGrid  [vtable @ 0x82058854] ───────────────────────────────
//
// Octree-based spatial partitioning grid for collision detection.
// Divides 3D space into a hierarchical grid of cells, each containing
// collision geometry. Supports dimension reduction (3D -> 2D -> 1D) based
// on which extents are zero.
struct phBoundOTGrid {
    void**      vtable;           // +0x00

    uint8_t     _pad0x04[112];
    float       m_gridScaleFactor;    // +0x74 (116) - World-to-grid scale factor
    int32_t     m_gridOffsetY;        // +0x78 (120) - Grid Y offset
    uint8_t     _pad0x7c[4];
    int32_t     m_gridDimensionY;     // +0x80 (128) - Grid Y dimension
    int32_t     m_gridOffsetX;        // +0x84 (132) - Grid X offset
    uint8_t     _pad0x88[4];
    int32_t     m_gridDimensionX;     // +0x8C (140) - Grid X dimension
    void*       m_pGridData;          // +0x90 (144) - Pointer to grid cell array

    uint8_t     _pad0x94[15144];
    void*       m_pGridMinX;      // +0x3BBC (15292) - Min X grid cell
    void*       m_pGridMinY;      // +0x3BC0 (15296) - Min Y grid cell
    void*       m_pGridMinZ;      // +0x3BC4 (15300) - Min Z grid cell
    void*       m_pGridMaxX;      // +0x3BC8 (15304) - Max X grid cell
    void*       m_pGridMaxY;      // +0x3BCC (15308) - Max Y grid cell
    void*       m_pGridMaxZ;      // +0x3BD0 (15312) - Max Z grid cell

    virtual ~phBoundOTGrid();                         // [0] @ 0x8229B550
    virtual void GetCellAtIndex();                    // [11] @ 0x8229D448
    virtual void* QueryCellState();                   // [12] @ 0x8229D488
    virtual void RenderCell(int cellIndex);           // [14] @ 0x8229D4B8
    virtual void UpdateCell(int cellIndex);           // [15] @ 0x8229D548
    virtual void* ProcessCell(int cellIndex);         // [16] @ 0x8229D508
    virtual bool IsCellValid(int cellIndex);          // [17] @ 0x8229D598
    virtual void TestSegment();                        // [19] @ 0x8229C888
    virtual void TestAABBOverlap();                    // [21] @ 0x8229CE38
    virtual void TestSweep();                          // [22] @ 0x8229D120
    virtual void CastRay();                            // [25] @ 0x8229CA78
    virtual void CollideVsBound();                     // [28] @ 0x8229BCB0
    virtual void CollideVsGeometry();                  // [29] @ 0x8229BF98
    virtual void CollideVsOTGrid();                    // [31] @ 0x8229C280
    virtual void Deserialize();                        // [36] @ 0x8229B8D0
    virtual void RecalcBounds();                       // [37] @ 0x8229B860
    virtual void GetTriangleData();                    // [38] @ 0x8229B5B8
    virtual void TestSweepTriangles();                 // [39] @ 0x8229B7A0

    // Non-virtual methods
    int SetupCollisionGrid(void* gridMinX, void* gridMinY, void* gridMinZ,
                          float extentX, float extentY, float extentZ,
                          void* gridMaxX, void* gridMaxY, void* gridMaxZ,
                          float minX, float minY, float minZ,
                          float maxX, float maxY, float maxZ);  // @ 0x82508098

    // Grid coordinate conversion
    double RoundToNearestInt(double value);                     // @ 0x82431910
    int WorldPositionToGridIndex(const float* position);        // @ 0x8229B418

    // Collision detection helpers (called by SetupCollisionGrid)
    int CollideLineX(float minX, float maxX, float z);
    int CollidePlaneXZ(float minX, float maxX, float minZ, float maxZ);
    int CollidePlaneXY(float minX, float maxX, float y);
    int Collide3D(float minX, float maxX, float y, float minZ, float maxZ);
    int Collide3DYExtent(float minX, float maxX, float minY, float maxY);
    int Collide3DFull(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
    int CollideCapsule(float minX, float maxX, float minY, float maxY, 
                      float minZ, float maxZ, float extentY, float extentZ);
};

// ── rage::phBoundOctree  [vtable @ 0x8205872C] ───────────────────────────────
struct phBoundOctree {
    void**      vtable;           // +0x00

    virtual ~phBoundOctree();     // [0] @ 0x8229B6F8
    virtual void TestSegment();    // [19] @ 0x82298DB8
    virtual void TestAABBOverlap();// [21] @ 0x82298DC0
    virtual void TestSweep();      // [22] @ 0x82298E10
    virtual void CastRay();        // [25] @ 0x82298F18
    virtual void CollideVsBound(); // [28] @ 0x82297660
    virtual void CollideVsGeometry(); // [29] @ 0x82297790
    virtual void Deserialize();    // [36] @ 0x822973D0
};

// ── rage::phBoundPolyhedron  [vtable @ 0x82058DD4] ───────────────────────────
struct phBoundPolyhedron {
    void**      vtable;           // +0x00
};

// ── rage::phBoundQuadtree  [vtable @ 0x82058C34] ─────────────────────────────
struct phBoundQuadtree {
    void**      vtable;           // +0x00

    virtual ~phBoundQuadtree();   // [0] @ 0x822AD0F0
    virtual void TestSegment();    // [19] @ 0x822AED40
    virtual void TestAABBOverlap();// [21] @ 0x822AEDB0
    virtual void TestSweep();      // [22] @ 0x822AEE00
    virtual void CastRay();        // [25] @ 0x822AED78
    virtual void CollideVsBound(); // [28] @ 0x822AD668
    virtual void CollideVsGeometry(); // [29] @ 0x822AD798
    virtual void CollideVsOTGrid();// [31] @ 0x822AD8D8
    virtual void Deserialize();    // [36] @ 0x822AD358
};

// ── rage::phBoundRibbon  [vtable @ 0x820589BC] ───────────────────────────────
// Physics bound for ribbon/strip geometry (likely used for cloth or flexible objects)
// Total size: 168 bytes (0xA8)
struct phBoundRibbon {
    void**      vtable;           // +0x00
    uint32_t    field_0x04;       // +0x04
    uint32_t    field_0x08;       // +0x08
    uint32_t    field_0x0C;       // +0x0C
    uint32_t    field_0x10;       // +0x10
    uint8_t     _pad0x14[12];     // +0x14
    
    // SIMD vectors for geometry/physics data
    float       m_vec1[4];        // +0x20 (32)  - 16-byte aligned vector
    float       m_vec2[4];        // +0x30 (48)
    float       m_vec3[4];        // +0x40 (64)
    
    // Float parameters
    float       m_float1;         // +0x50 (80)
    float       m_float2;         // +0x54 (84)
    float       m_float3;         // +0x58 (88)
    uint8_t     m_flags;          // +0x5C (92)
    uint8_t     _pad0x5D[3];      // +0x5D
    
    // Additional SIMD vectors
    float       m_vec4[4];        // +0x60 (96)
    float       m_vec5[4];        // +0x70 (112)
    float       m_vec6[4];        // +0x80 (128)
    float       m_vec7[4];        // +0x90 (144)
    
    // Trailing fields
    uint32_t    field_0xA0;       // +0xA0 (160)
    uint32_t    field_0xA4;       // +0xA4 (164)

    virtual ~phBoundRibbon();     // [0] @ 0x8229D970
    virtual void UpdateBound();    // [11] @ 0x8229D6A0
    virtual void GetMaterialIndex(); // [12] @ 0x8229D6C0
    virtual void SetMaterialIndex(); // [13] @ 0x8229D6C8
    virtual void TestSegment();    // [19] @ 0x822A2300
    virtual void TestMovingSphere(); // [20] @ 0x822A1E48
    virtual void TestAABBOverlap();// [21] @ 0x822A2420
    virtual void TestSweep();      // [22] @ 0x822A25B0
    virtual void CollideVsBound(); // [28] @ 0x822A09E0
    virtual void CollideVsGeometry(); // [29] @ 0x822A0BF0
    virtual void CollideVsOTGrid();// [31] @ 0x822A1198
    virtual void Deserialize();    // [36] @ 0x822A06D0
    virtual void RecalcBounds();   // [37] @ 0x8229DDF8
    
    // @ 0x82294AB8 | size: 0xD4
    void CopyFrom(const phBoundRibbon* source);
};

// ── rage::phBoundSphere  [vtable @ 0x82058584] ───────────────────────────────
struct phBoundSphere {
    void**      vtable;           // +0x00

    virtual void SetCentroidOffset();     // [6] @ 0x8233AB08
    virtual void TranslateAndDispatch(); // [7] @ 0x82295548
    virtual void GetVolume();            // [8] @ 0x822954A8
    virtual void GetSupportPoint();      // [9] @ 0x82295678
    virtual void UpdateBound();          // [11] @ 0x82295478
    virtual void GetMaterialIndex();     // [12] @ 0x82295498
    virtual void SetMaterialIndex();     // [13] @ 0x822954A0
    virtual void DebugDraw();            // [14] @ 0x8233A8F8
    virtual void ContainsPoint();        // [18] @ 0x82295C70
    virtual void TestSegment();          // [19] @ 0x82295DC8
    virtual void TestMovingSphere();     // [20] @ 0x82295FA0
    virtual void TestAABBOverlap();      // [21] @ 0x82296300
    virtual void TestSweep();            // [22] @ 0x82296438
    virtual void GetClosestPoint();      // [23] @ 0x82296B38
    virtual void TestSphereOverlap();    // [24] @ 0x82296A10
    virtual void CollideVsBound();       // [28] @ 0x82295898
    virtual void ComputeSupportDistance(); // [33] @ 0x82295860
    virtual void Deserialize();          // [36] @ 0x822956A8
    virtual void RecalcBounds();         // [37] @ 0x82295598

    // Non-virtual methods
    void DispatchMaterialAlloc();
    void SetMaterialIndex(uint32_t index);
    float ComputeVolume();
    void ComputeInertiaTensor(float* outTensor, const void* source, float density);
    float GetProjectedExtent(const float* direction, uint8_t flags);
    void RenderDebug();
    void SetPositionAndUpdateFlags(const float* position);
};

// ── rage::phBoundSurface  [vtable @ 0x82058CE4] ──────────────────────────────
struct phBoundSurface {
    void**      vtable;           // +0x00

    virtual void vfn_38();        // [38] @ 0x822AFE88
};

// ── rage::phBoundTaperedCapsule  [vtable @ 0x82064FBC] ───────────────────────
struct phBoundTaperedCapsule {
    void**      vtable;           // +0x00

    virtual ~phBoundTaperedCapsule();  // [0] @ 0x820C9168
    virtual void vfn_3();              // [3] @ 0x8233AB98
    virtual void vfn_8();              // [8] @ 0x8233A898
    virtual void vfn_9();              // [9] @ 0x8233AD38
    virtual void vfn_11();             // [11] @ 0x8233A8C8
    virtual void vfn_12();             // [12] @ 0x8233A8E8
    virtual void vfn_13();             // [13] @ 0x8233A8F0
    virtual void vfn_18();             // [18] @ 0x8233B020
    virtual void vfn_19();             // [19] @ 0x8233B1F8
    virtual void vfn_20();             // [20] @ 0x8233ADA8
    virtual void vfn_21();             // [21] @ 0x8233B6F8
    virtual void vfn_22();             // [22] @ 0x8233BC28
    virtual void vfn_23();             // [23] @ 0x8233BA48
    virtual void vfn_24();             // [24] @ 0x8233B8B8
    virtual void vfn_28();             // [28] @ 0x8233C760
    virtual void vfn_29();             // [29] @ 0x8233CC50
    virtual void vfn_33();             // [33] @ 0x8233AD48
    virtual void vfn_36();             // [36] @ 0x8233E688
    virtual void vfn_37();             // [37] @ 0x8233AA00
};

// ── rage::phClothVerletBehavior  [vtable @ 0x82079010] ───────────────────────
struct phClothVerletBehavior {
    void**      vtable;           // +0x00

    virtual ~phClothVerletBehavior();             // [0] @ 0x82421F20
    virtual void ScalarDtor(int flags);           // [1] @ 0x823D7C50
    virtual void vfn_5();                         // [5] @ 0x82421FD8
    virtual void vfn_6();                         // [6] @ 0x82422160
};

// ── rage::phClothVerletInst  [vtable @ 0x82071BAC] ───────────────────────────
struct phClothVerletInst {
    void**      vtable;           // +0x00

    virtual ~phClothVerletInst(); // [0] @ 0x823D7C90
};

// ── rage::phCollider  [vtable @ 0x82059C3C] ──────────────────────────────────
struct phCollider {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:1 W:1
    uint32_t    field_0x0008;     // +0x0008  R:1 W:1
    uint32_t    field_0x000c;     // +0x000c  W:1
    uint32_t    field_0x0010;     // +0x0010  R:5 W:1
    uint8_t     field_0x0014;     // +0x0014  W:1
    uint8_t     _pad0x0015[75];
    uint32_t    field_0x0064;     // +0x0064  R:1
    uint32_t    field_0x0068;     // +0x0068  R:4
    uint32_t    field_0x0070;     // +0x0070  R:1
    uint32_t    field_0x0074;     // +0x0074  R:1
    uint32_t    field_0x0078;     // +0x0078  R:1
    uint32_t    field_0x0080;     // +0x0080  R:1
    uint32_t    field_0x0084;     // +0x0084  R:1
    uint32_t    field_0x0088;     // +0x0088  R:1
    uint8_t     _pad0x008c[292];
    uint32_t    field_0x01b0;     // +0x01b0  R:1 W:1
    uint32_t    field_0x01b4;     // +0x01b4  W:1

    virtual ~phCollider();                        // [0] @ 0x822C1998
    virtual void ScalarDtor(int flags);           // [1] @ 0x822CABE0
    virtual void vfn_2();                         // [2] @ 0x822CAD28
    virtual void vfn_4();                         // [4] @ 0x822CAD78
    virtual void vfn_5();                         // [5] @ 0x822CAF38
    virtual void vfn_6();                         // [6] @ 0x822CB070
    virtual void vfn_7();                         // [7] @ 0x822CB740
    virtual void vfn_8();                         // [8] @ 0x822CB3B8
    virtual void vfn_9();                         // [9] @ 0x822CB450
    virtual void vfn_13();                        // [13] @ 0x822CB8B0
    virtual void vfn_14();                        // [14] @ 0x822CB988
    virtual void vfn_15();                        // [15] @ 0x822CBA30
    virtual void vfn_16();                        // [16] @ 0x822CB998
    virtual void vfn_18();                        // [18] @ 0x822CBC68
    virtual void vfn_19();                        // [19] @ 0x822CBCB8
    virtual void vfn_20();                        // [20] @ 0x822CBE38
    virtual void vfn_22();                        // [22] @ 0x822CC080
    virtual void vfn_23();                        // [23] @ 0x822CC128
    virtual void vfn_25();                        // [25] @ 0x822CC708
    virtual void vfn_26();                        // [26] @ 0x822CC7B8
    virtual void vfn_28();                        // [28] @ 0x822CC710
    virtual void vfn_29();                        // [29] @ 0x822CC748
    virtual void vfn_30();                        // [30] @ 0x822CC780
    virtual void vfn_31();                        // [31] @ 0x822CCC48
    virtual void vfn_32();                        // [32] @ 0x822CCCB0
    virtual void vfn_33();                        // [33] @ 0x822CCCC8
    virtual void vfn_34();                        // [34] @ 0x822CC880
    virtual void vfn_35();                        // [35] @ 0x822CCB88
    virtual void vfn_36();                        // [36] @ 0x822CC978
    virtual void vfn_37();                        // [37] @ 0x822CCAB0
    virtual void vfn_38();                        // [38] @ 0x822CCBA0
    virtual void vfn_39();                        // [39] @ 0x822CA7D8
    virtual void vfn_40();                        // [40] @ 0x822CCCE0
    virtual void vfn_41();                        // [41] @ 0x822CCD18
    virtual void vfn_42();                        // [42] @ 0x822CCD28
    virtual void vfn_43();                        // [43] @ 0x822CCDE0
};

// ── rage::phConstrainedCollider  [vtable @ 0x82059DAC] ───────────────────────
struct phConstrainedCollider {
    void**      vtable;           // +0x00

    virtual ~phConstrainedCollider();             // [0] @ 0x822DA918
    virtual void vfn_5();                         // [5] @ 0x822DCB08
    virtual void vfn_6();                         // [6] @ 0x822DCD78
    virtual void vfn_8();                         // [8] @ 0x822DCE28
    virtual void vfn_11();                        // [11] @ 0x822DCDB8
    virtual void vfn_15();                        // [15] @ 0x822DC258
    virtual void vfn_19();                        // [19] @ 0x822DBBE8
    virtual void vfn_20();                        // [20] @ 0x822DBDB8
    virtual void vfn_21();                        // [21] @ 0x822DBED8
    virtual void vfn_22();                        // [22] @ 0x822DCE68
    virtual void vfn_36();                        // [36] @ 0x822DCF90
};

// ── rage::phInst  [vtable @ 0x8205991C] ──────────────────────────────────────
//
// Core physics instance. Holds transform, bound data, and simulation state.
// The struct is very large (~0x506C bytes) reflecting full articulated-body state.
struct phInst {
    void**      vtable;           // +0x00

    uint8_t     field_0x0001;     // +0x0001  R:3
    uint32_t    m_pBound;         // +0x0004  R:28 W:21  — pointer to phBound
    uint8_t     field_0x0005;     // +0x0005  R:1
    uint64_t    m_pDestructor;    // +0x0008  R:52 W:15  — destructor callback
    uint8_t     field_0x0009;     // +0x0009  R:1
    uint32_t    m_nRefCount;      // +0x000c  R:19 W:11  — atomic reference count
    uint64_t    m_CriticalSection; // +0x0010  R:12 W:8  — mutex for Lock/Unlock
    uint32_t    m_pBoundData;     // +0x0014  R:7 W:5   — secondary bound data ptr
    uint8_t     field_0x0015;     // +0x0015  W:3
    uint64_t    m_pDirtyFlags;    // +0x0018  R:10 W:6  — dirty flag tracking
    uint8_t     field_0x0019;     // +0x0019  R:1
    uint16_t    field_0x001a;     // +0x001a  R:1
    uint32_t    field_0x001c;     // +0x001c  R:4 W:3
    uint32_t    m_nFlags;         // +0x0020  R:7 W:4   — packed flags (physics layer bits 6-9)
    uint32_t    m_pBoundResource; // +0x0024  R:8 W:3   — page-aligned bound resource ptr
    uint16_t    field_0x0026;     // +0x0026  R:1
    uint64_t    field_0x0028;     // +0x0028  R:3 W:2
    uint16_t    field_0x002a;     // +0x002a  R:1
    uint32_t    field_0x002c;     // +0x002c  R:12 W:10
    uint64_t    field_0x0030;     // +0x0030  R:4 W:11
    uint32_t    field_0x0034;     // +0x0034  R:3 W:5
    uint64_t    m_pOwner;         // +0x0038  R:37 W:8  — pointer to owner/parent object
    uint8_t     field_0x003b;     // +0x003b  R:1
    uint32_t    field_0x003c;     // +0x003c  R:8 W:7
    uint16_t    field_0x003e;     // +0x003e  R:5 W:4
    uint64_t    m_pArchetype;     // +0x0040  R:7 W:10  — archetype (shared phys properties)
    uint32_t    m_nDataIndex;     // +0x0044  R:8 W:7   — index into physics data table
    uint8_t     field_0x0045;     // +0x0045  R:2 W:1
    uint64_t    m_pSimulator;     // +0x0048  R:12 W:12 — simulator / physics world ptr
    uint32_t    m_pBoundShape;    // +0x004c  R:10 W:4  — bound shape for SetBoundScale
    uint64_t    field_0x0050;     // +0x0050  R:4 W:6
    uint32_t    field_0x0054;     // +0x0054  R:1
    uint64_t    field_0x0058;     // +0x0058  R:7 W:3
    uint32_t    field_0x0060;     // +0x0060  R:9 W:2
    uint32_t    field_0x0064;     // +0x0064  R:5 W:2
    uint32_t    field_0x0068;     // +0x0068  R:5 W:5
    uint32_t    field_0x006c;     // +0x006c  R:2 W:2
    uint32_t    field_0x0070;     // +0x0070  R:1 W:2
    uint32_t    field_0x0074;     // +0x0074  R:5 W:6
    uint32_t    field_0x0078;     // +0x0078  R:8 W:3
    uint32_t    field_0x007c;     // +0x007c  R:4 W:3
    uint32_t    field_0x0080;     // +0x0080  R:3 W:1
    uint8_t     _pad0x0084[12];
    uint8_t     field_0x0090;     // +0x0090  W:1
    uint8_t     _pad0x0091[83];
    uint16_t    field_0x00e4;     // +0x00e4  R:2
    uint8_t     _pad0x00e6[34];
    uint32_t    field_0x0108;     // +0x0108  R:1
    uint32_t    field_0x010c;     // +0x010c  R:1
    uint32_t    field_0x0110;     // +0x0110  R:1
    uint32_t    field_0x0114;     // +0x0114  R:1
    uint64_t    field_0x0118;     // +0x0118  R:2
    uint32_t    m_nCollisionGroup; // +0x011c  R:2 W:2  — collision group ID
    uint8_t     _pad0x0120[8];
    uint64_t    field_0x0128;     // +0x0128  W:1
    uint32_t    field_0x0130;     // +0x0130  W:1
    uint8_t     field_0x0134;     // +0x0134  W:1
    uint32_t    field_0x0138;     // +0x0138  W:1
    uint8_t     _pad0x013c[8];
    uint32_t    field_0x0144;     // +0x0144  R:2 W:3
    uint32_t    field_0x0148;     // +0x0148  R:2 W:3
    uint32_t    field_0x014c;     // +0x014c  W:1
    uint32_t    field_0x0150;     // +0x0150  W:2
    uint32_t    field_0x0154;     // +0x0154  W:2
    uint32_t    field_0x0158;     // +0x0158  W:2
    uint32_t    field_0x015c;     // +0x015c  W:2
    uint32_t    field_0x0160;     // +0x0160  W:2
    uint32_t    field_0x0164;     // +0x0164  W:2
    uint32_t    field_0x0168;     // +0x0168  W:2
    uint32_t    field_0x016c;     // +0x016c  W:2
    uint32_t    field_0x0170;     // +0x0170  R:2 W:1
    uint32_t    field_0x0174;     // +0x0174  R:6 W:3
    uint8_t     _pad0x0178[16];
    uint32_t    m_pTransform;     // +0x0188  R:2 W:8  — pointer to transform matrix
    uint64_t    field_0x0190;     // +0x0190  W:2
    uint64_t    field_0x0198;     // +0x0198  W:1
    uint32_t    field_0x01a0;     // +0x01a0  W:2
    uint32_t    field_0x01a4;     // +0x01a4  W:2
    uint32_t    field_0x01a8;     // +0x01a8  R:1
    uint32_t    field_0x01b0;     // +0x01b0  R:1
    uint8_t     _pad0x01b4[8];
    uint32_t    m_nCollisionMask; // +0x01bc  R:1 W:2  — collision filter bitmask
    uint32_t    m_nUserData;      // +0x01c0  R:1 W:2  — user-defined data word
    uint8_t     _pad0x01c4[40];
    uint32_t    field_0x01ec;     // +0x01ec  R:1
    uint16_t    field_0x01f0;     // +0x01f0  R:1
    uint16_t    field_0x01f2;     // +0x01f2  R:1
    uint8_t     field_0x01f4;     // +0x01f4  R:2
    uint8_t     _pad0x01f5[15];
    uint8_t     field_0x0204;     // +0x0204  R:1
    uint16_t    field_0x0206;     // +0x0206  R:2
    uint16_t    field_0x0208;     // +0x0208  W:1
    uint8_t     field_0x020c;     // +0x020c  W:1
    uint8_t     field_0x020d;     // +0x020d  R:2
    uint8_t     _pad0x020e[14];
    uint32_t    field_0x021c;     // +0x021c  R:2 W:2
    uint16_t    field_0x0220;     // +0x0220  R:2 W:1
    uint32_t    field_0x0224;     // +0x0224  R:2
    uint8_t     _pad0x0228[916];
    uint32_t    field_0x05bc;     // +0x05bc  R:2 W:1
    uint32_t    field_0x05c0;     // +0x05c0  W:3
    uint32_t    field_0x05c4;     // +0x05c4  R:5 W:3
    uint32_t    field_0x05c8;     // +0x05c8  R:3 W:4
    uint32_t    field_0x05cc;     // +0x05cc  R:1 W:2
    uint8_t     _pad0x05d0[8896];
    uint32_t    field_0x2890;     // +0x2890  R:2
    uint32_t    field_0x2894;     // +0x2894  R:2
    uint8_t     _pad0x2898[41];
    uint8_t     field_0x28c1;     // +0x28c1  R:1
    uint8_t     field_0x28c2;     // +0x28c2  R:1 W:1
    uint8_t     _pad0x28c3[9];
    uint32_t    field_0x28cc;     // +0x28cc  R:1 W:1
    uint8_t     _pad0x28d0[2496];
    uint32_t    field_0x3290;     // +0x3290  R:1 W:1
    uint8_t     _pad0x3294[588];
    uint32_t    field_0x34e0;     // +0x34e0  R:3
    uint32_t    field_0x34e4;     // +0x34e4  R:3
    uint8_t     _pad0x34e8[452];
    uint32_t    field_0x36ac;     // +0x36ac  R:1
    uint32_t    field_0x36b0;     // +0x36b0  R:1
    uint32_t    field_0x36b4;     // +0x36b4  R:1
    uint32_t    field_0x36b8;     // +0x36b8  R:2
    uint32_t    field_0x36bc;     // +0x36bc  R:1
    uint32_t    field_0x36c0;     // +0x36c0  R:1 W:3
    uint32_t    field_0x36c4;     // +0x36c4  R:4 W:1
    uint32_t    field_0x36c8;     // +0x36c8  R:2 W:2
    uint32_t    field_0x36cc;     // +0x36cc  R:1
    uint8_t     _pad0x36d0[6416];
    uint32_t    field_0x4fe0;     // +0x4fe0  R:4
    uint8_t     _pad0x4fe4[136];
    uint32_t    field_0x506c;     // +0x506c  R:2

    virtual ~phInst();                            // [0] @ 0x82346400
    virtual void vfn_9();                         // [9] @ 0x8248D7C8
    virtual void vfn_10();                        // [10] @ 0x824829E0
    virtual void vfn_11();                        // [11] @ 0x82484018
    virtual void vfn_12();                        // [12] @ 0x82482A40
    virtual void vfn_13();                        // [13] @ 0x82494160
    virtual void vfn_14();                        // [14] @ 0x82482A90
    virtual void vfn_15();                        // [15] @ 0x8248D888
    virtual void vfn_16();                        // [16] @ 0x8248B8C8
    virtual void vfn_17();                        // [17] @ 0x8248D840
    virtual void vfn_18();                        // [18] @ 0x824840A0
    virtual void vfn_19();                        // [19] @ 0x8248B8D0
    virtual void vfn_20();                        // [20] @ 0x82484268
    virtual void vfn_21();                        // [21] @ 0x82483EB0
    virtual void vfn_22();                        // [22] @ 0x82483E30
    virtual void vfn_23();                        // [23] @ 0x82483FD8
    virtual void vfn_24();                        // [24] @ 0x82484650
    virtual void vfn_25();                        // [25] @ 0x82483F28
    virtual void vfn_32();                        // [32] @ 0x8248D758
    virtual void vfn_34();                        // [34] @ 0x8248D868
    virtual void vfn_35();                        // [35] @ 0x8248D848
    virtual void vfn_38();                        // [38] @ 0x8248D808
    virtual void vfn_39();                        // [39] @ 0x8248D818
    virtual void vfn_40();                        // [40] @ 0x8248D880
    virtual void vfn_42();                        // [42] @ 0x8248D860
    virtual void vfn_44();                        // [44] @ 0x8248D810
    virtual void vfn_45();                        // [45] @ 0x8248D820
    virtual void vfn_46();                        // [46] @ 0x8248E078
    virtual void vfn_48();                        // [48] @ 0x8247E2D0
    virtual void vfn_58();                        // [58] @ 0x8248D9C8
    virtual void vfn_59();                        // [59] @ 0x8248DA68
    virtual void vfn_60();                        // [60] @ 0x8248DA88
    virtual void vfn_61();                        // [61] @ 0x8248DAA8
    virtual void vfn_62();                        // [62] @ 0x8248DAC8
    virtual void vfn_63();                        // [63] @ 0x8248DAE8
    virtual void vfn_64();                        // [64] @ 0x82487FD8
    virtual void vfn_65();                        // [65] @ 0x82488028
    virtual void vfn_66();                        // [66] @ 0x8248DB48
    virtual void vfn_67();                        // [67] @ 0x8248DB68
    virtual void vfn_68();                        // [68] @ 0x8248DB88
    virtual void vfn_69();                        // [69] @ 0x8248DBA8
    virtual void vfn_71();                        // [71] @ 0x82488480
    virtual void vfn_74();                        // [74] @ 0x82487A20
    virtual void vfn_75();                        // [75] @ 0x824879A0
    virtual void vfn_76();                        // [76] @ 0x82487890
    virtual void vfn_77();                        // [77] @ 0x82487840
    virtual void vfn_78();                        // [78] @ 0x82487AA8
    virtual void vfn_79();                        // [79] @ 0x82487CE0
    virtual void vfn_80();                        // [80] @ 0x82487E60

    // Non-virtual methods
    void* GetBoundPtr();
    void* GetField14();
    void SetField9(void* val);
    void SetField11(void* val);
    int GetStaticSize();
    int StoreSize(int* outParam);
    void ForwardSlot23(void* arg);
    void ForwardSlot12();
    void AdjustorSlot1();
    void AdjustorSlot2(void* arg);
    void AdjustorSlot5();
    void AdjustorSlot6(void* arg);
    int GetPhysicsLayer();
    int GetU16Delta();
    int ComputeDataOffset();
    int AddRef();
    int Release();
    void Lock();
    void Unlock();
    void ShiftArgsAndCall(void* a, void* b, void* c);
    void LoadGlobalAndCall();
    void InitVtableAndCleanup();
    void SetIndexedFlag(int index);
    void ClearIndexedFlag(int index);
    void ConditionalStore(int flag, uint32_t val1, uint32_t val2);
    void ZeroFieldRange();
    void ZeroFieldRanges();
    void ClearSubStatePtr();
    int StoreSizeConstant(uint32_t* outParam);
    void DispatchTransformA(void* arg1, void* arg2);
    void InitTripleVtable();
    void InitWithParam(void* param);
    int GetSubObjectPtr(void** outPtr);
    void SetPhysicsLayer(void* arg);
    void ConditionalBroadcast(void* msg, bool includeData);
    void ReadMMIOAndStore();
    void SetUserData(uint32_t val);
    uint32_t GetUserData();
    void SetCollisionGroup(uint32_t val);
    uint32_t GetCollisionGroup();
    void SetCollisionMask(uint32_t val);
    uint32_t GetCollisionMask();
    uint32_t GetErrorCode();
    void CallVfn12ThenInit();
    void AtomicDecrementAndCallback();
    void SetCollisionFlags(uint32_t flags);
    uint32_t GetCollisionFlags();
    void SetContactFilter(uint32_t filter);
    uint32_t GetContactFilter();
    void SetContactCallback(uint32_t callback);
    uint32_t GetContactCallback();
    uint64_t GetArchetypeData();
    uint32_t GetSupportedFlags();
    void CopyCurrentTransform(void* src, uint32_t srcLen);
    void CopyLastTransform(void* src, uint32_t srcLen);
};

// ── rage::phInstStatic ───────────────────────────────────────────────────────
struct phInstStatic {
    void**      vtable;           // +0x00

    void ClearMotionState();
    void ClearAllMotion();
};

// ── rage::phInstBehavior  [vtable @ 0x82065080] ──────────────────────────────
struct phInstBehavior {
    void**      vtable;           // +0x00

    virtual ~phInstBehavior();    // [0] @ 0x82382560
};

// ── rage::phJoint  [vtable @ 0x8204FE9C] ─────────────────────────────────────
struct phJoint {
    void**      vtable;           // +0x00

    virtual void vfn_17();        // [17] @ 0x8225B430
};

// ── rage::phJoint1Dof  [vtable @ 0x8203315C] ─────────────────────────────────
struct phJoint1Dof {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:5
    uint32_t    field_0x0008;     // +0x0008  R:1
    uint32_t    field_0x0010;     // +0x0010  R:5
    uint32_t    field_0x0014;     // +0x0014  R:1
    uint32_t    m_stiffness;      // +0x0018  R:11 W:1  — joint stiffness
    uint32_t    m_damping;        // +0x001c  R:22 W:1  — joint damping (most-read field)
    uint8_t     _pad0x0020[544];
    uint32_t    field_0x0240;     // +0x0240  R:1
    uint32_t    field_0x0248;     // +0x0248  R:1
    uint32_t    field_0x0250;     // +0x0250  R:1
    uint32_t    field_0x0258;     // +0x0258  R:1
    uint32_t    field_0x0260;     // +0x0260  R:1
    uint32_t    field_0x0268;     // +0x0268  R:1
    uint8_t     _pad0x0270[36];
    uint32_t    field_0x0290;     // +0x0290  R:1
    uint32_t    field_0x0298;     // +0x0298  R:2
    uint32_t    field_0x02a0;     // +0x02a0  R:1
    uint32_t    field_0x02a8;     // +0x02a8  R:2
    uint32_t    field_0x02b0;     // +0x02b0  R:1
    uint32_t    field_0x02b8;     // +0x02b8  R:2
    uint8_t     _pad0x02c0[20];
    uint32_t    field_0x02d0;     // +0x02d0  R:2 W:1
    uint32_t    field_0x02d4;     // +0x02d4  R:2 W:1
    uint32_t    field_0x02d8;     // +0x02d8  R:2 W:1
    uint32_t    field_0x02dc;     // +0x02dc  R:2 W:1
    uint32_t    field_0x02e0;     // +0x02e0  R:3 W:1
    uint32_t    field_0x02e4;     // +0x02e4  R:2 W:3
    uint32_t    field_0x02e8;     // +0x02e8  R:8 W:3
    uint8_t     _pad0x02ec[132];
    uint32_t    field_0x0370;     // +0x0370  R:1 W:1
    uint32_t    field_0x0374;     // +0x0374  R:5 W:1
    uint32_t    field_0x0378;     // +0x0378  R:5 W:1
    uint32_t    field_0x037c;     // +0x037c  W:2

    virtual ~phJoint1Dof();                       // [0] @ 0x82259FF8
    virtual void ScalarDtor(int flags);           // [1] @ 0x82259AB0
    virtual void vfn_2();                         // [2] @ 0x82126EA0
    virtual void vfn_3();                         // [3] @ 0x82259910
    virtual void vfn_4();                         // [4] @ 0x82259958
    virtual void vfn_5();                         // [5] @ 0x822599C8
    virtual void vfn_6();                         // [6] @ 0x82259A40
    virtual void vfn_7();                         // [7] @ 0x82259B78
    virtual void vfn_8();                         // [8] @ 0x82259BE8
    virtual void vfn_9();                         // [9] @ 0x82259AB8
    virtual void vfn_10();                        // [10] @ 0x82259B30
    virtual void vfn_11();                        // [11] @ 0x82259C50
    virtual void vfn_12();                        // [12] @ 0x82259DF8
    virtual void vfn_13();                        // [13] @ 0x82259428
    virtual void vfn_14();                        // [14] @ 0x822597E0
    virtual void* GetMotorData();                  // [15] @ 0x82126CF8
    virtual void vfn_16();                        // [16] @ 0x82255318
    virtual void vfn_17();                        // [17] @ 0x8225AB78
    virtual void vfn_18();                        // [18] @ 0x82259E58
    virtual void vfn_19();                        // [19] @ 0x8225A4B0
    virtual void vfn_20();                        // [20] @ 0x8225A348
    virtual void vfn_21();                        // [21] @ 0x8225A540
    virtual void vfn_22();                        // [22] @ 0x8225A590
    virtual void vfn_23();                        // [23] @ 0x8225A758
    virtual void vfn_24();                        // [24] @ 0x8225A5E0
    virtual void vfn_25();                        // [25] @ 0x8225A6A0
    virtual void vfn_26();                        // [26] @ 0x8225A800
    virtual void vfn_27();                        // [27] @ 0x8225AA18
    virtual void vfn_28();                        // [28] @ 0x8225A8B8
    virtual void vfn_29();                        // [29] @ 0x8225A960
    virtual void vfn_30();                        // [30] @ 0x8225AAC0
    virtual void vfn_31();                        // [31] @ 0x8225A310
    virtual void vfn_32();                        // [32] @ 0x82126CD0
    virtual void vfn_33();                        // [33] @ 0x82126C80
};

// ── rage::phJoint3Dof  [vtable @ 0x820331EC] ─────────────────────────────────
struct phJoint3Dof {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:16 W:11
    uint16_t    field_0x0006;     // +0x0006  R:4 W:8
    uint32_t    field_0x0008;     // +0x0008  R:17 W:6
    uint32_t    field_0x000c;     // +0x000c  R:8 W:3
    uint8_t     field_0x000d;     // +0x000d  W:1
    uint16_t    field_0x000e;     // +0x000e  W:1
    uint32_t    field_0x0010;     // +0x0010  R:3 W:6
    uint32_t    field_0x0014;     // +0x0014  R:5 W:6
    uint16_t    field_0x0016;     // +0x0016  W:1
    uint32_t    m_angularLimits;  // +0x0018  R:14 W:3  — angular constraint limits
    uint32_t    m_angularDamping; // +0x001c  R:28 W:1  — most-read; angular damping
    uint16_t    field_0x001e;     // +0x001e  W:1
    uint32_t    field_0x0020;     // +0x0020  R:2 W:4
    uint32_t    field_0x0024;     // +0x0024  R:8 W:3
    uint16_t    field_0x0026;     // +0x0026  W:1
    uint32_t    field_0x0028;     // +0x0028  R:2 W:5
    uint16_t    field_0x002c;     // +0x002c  W:1
    uint16_t    field_0x002e;     // +0x002e  W:1
    uint32_t    field_0x0030;     // +0x0030  W:2
    uint16_t    field_0x0034;     // +0x0034  W:1
    uint16_t    field_0x0036;     // +0x0036  W:1
    uint32_t    field_0x0038;     // +0x0038  W:1
    uint32_t    field_0x003c;     // +0x003c  R:1 W:1
    uint16_t    field_0x003e;     // +0x003e  W:1
    uint32_t    field_0x0040;     // +0x0040  R:2 W:1
    uint16_t    field_0x0044;     // +0x0044  W:1
    uint16_t    field_0x0046;     // +0x0046  W:1
    uint8_t     _pad0x0048[22];
    uint32_t    field_0x0060;     // +0x0060  R:7
    uint32_t    field_0x0064;     // +0x0064  R:5
    uint8_t     _pad0x0068[8];
    uint32_t    field_0x0070;     // +0x0070  R:1
    uint8_t     _pad0x0074[8];
    uint32_t    field_0x007c;     // +0x007c  W:1
    uint32_t    field_0x0080;     // +0x0080  R:4 W:1
    uint32_t    field_0x0088;     // +0x0088  R:1
    uint32_t    field_0x008c;     // +0x008c  W:1
    uint32_t    field_0x0090;     // +0x0090  W:1
    uint32_t    field_0x0094;     // +0x0094  W:1
    uint32_t    field_0x0098;     // +0x0098  W:1
    uint32_t    field_0x009c;     // +0x009c  W:1
    uint32_t    field_0x00a0;     // +0x00a0  W:1
    uint32_t    field_0x00a4;     // +0x00a4  W:1
    uint32_t    field_0x00a8;     // +0x00a8  W:1
    uint32_t    field_0x00ac;     // +0x00ac  W:1
    uint32_t    field_0x00b0;     // +0x00b0  W:1
    uint8_t     _pad0x00b4[64];
    uint32_t    field_0x00f4;     // +0x00f4  W:1
    uint32_t    field_0x00f8;     // +0x00f8  W:1
    uint32_t    field_0x00fc;     // +0x00fc  W:1
    uint32_t    field_0x0100;     // +0x0100  W:1
    uint32_t    field_0x0104;     // +0x0104  W:1
    uint32_t    field_0x0108;     // +0x0108  W:1
    uint8_t     _pad0x010c[8];
    uint32_t    field_0x0114;     // +0x0114  W:1
    uint32_t    field_0x0118;     // +0x0118  W:1
    uint32_t    field_0x011c;     // +0x011c  W:1
    uint32_t    field_0x0120;     // +0x0120  W:1
    uint32_t    field_0x0124;     // +0x0124  W:1
    uint32_t    field_0x0128;     // +0x0128  W:1
    uint8_t     _pad0x012c[420];
    uint32_t    field_0x02d0;     // +0x02d0  W:1
    uint32_t    field_0x02d4;     // +0x02d4  W:1
    uint32_t    field_0x02d8;     // +0x02d8  W:1
    uint32_t    field_0x02dc;     // +0x02dc  W:1
    uint32_t    field_0x02e0;     // +0x02e0  R:2 W:1
    uint32_t    field_0x02e4;     // +0x02e4  R:1 W:1
    uint8_t     field_0x02e8;     // +0x02e8  W:1
    uint8_t     field_0x02e9;     // +0x02e9  R:2 W:1
    uint8_t     field_0x02ea;     // +0x02ea  R:4 W:3  — heavily-read byte flag
    uint8_t     field_0x02eb;     // +0x02eb  R:1 W:4
    uint32_t    field_0x02ec;     // +0x02ec  R:2
    uint32_t    field_0x02f0;     // +0x02f0  R:2
    uint32_t    field_0x02f4;     // +0x02f4  R:3 W:2
    uint32_t    field_0x02f8;     // +0x02f8  R:4 W:2
    uint32_t    field_0x02fc;     // +0x02fc  R:2 W:2
    uint32_t    field_0x0300;     // +0x0300  R:3 W:4
    uint32_t    field_0x0304;     // +0x0304  R:3
    uint32_t    field_0x0308;     // +0x0308  R:5
    uint8_t     _pad0x030c[148];
    uint32_t    field_0x03a0;     // +0x03a0  R:10
    uint8_t     _pad0x03a4[12];
    uint32_t    field_0x03b0;     // +0x03b0  R:1
    uint32_t    field_0x03b4;     // +0x03b4  R:1
    uint8_t     _pad0x03b8[8];
    uint32_t    field_0x03c0;     // +0x03c0  R:1
    uint32_t    field_0x03c4;     // +0x03c4  R:1
    uint8_t     _pad0x03c8[8];
    uint32_t    field_0x03d0;     // +0x03d0  R:1
    uint32_t    field_0x03d4;     // +0x03d4  R:1
    uint8_t     _pad0x03d8[380];
    uint32_t    field_0x0554;     // +0x0554  R:1
    uint8_t     _pad0x0558[16];
    uint32_t    field_0x0568;     // +0x0568  R:1
    uint8_t     _pad0x056c[276];
    uint32_t    m_nConstraints;   // +0x0680  R:6 W:1  — constraint count
    uint8_t     _pad0x0684[8728];
    uint32_t    field_0x289c;     // +0x289c  R:1
    uint8_t     _pad0x28a0[1108];
    uint32_t    field_0x2cf4;     // +0x2cf4  R:1

    virtual ~phJoint3Dof();                       // [0] @ 0x822551C8
    virtual void ScalarDtor(int flags);           // [1] @ 0x822526C8
    virtual void vfn_3();                         // [3] @ 0x822520C0
    virtual void vfn_4();                         // [4] @ 0x822522E0
    virtual void vfn_5();                         // [5] @ 0x82252440
    virtual void vfn_6();                         // [6] @ 0x82252508
    virtual void vfn_7();                         // [7] @ 0x82252790
    virtual void vfn_8();                         // [8] @ 0x82252928
    virtual void vfn_9();                         // [9] @ 0x82252AA8
    virtual void vfn_10();                        // [10] @ 0x82252B68
    virtual void vfn_11();                        // [11] @ 0x82252C10
    virtual void vfn_12();                        // [12] @ 0x82253358
    virtual void vfn_13();                        // [13] @ 0x82251200
    virtual void vfn_14();                        // [14] @ 0x82251268
    virtual void vfn_17();                        // [17] @ 0x82255340
    virtual void vfn_18();                        // [18] @ 0x82253460
    virtual void vfn_19();                        // [19] @ 0x82253BB8
    virtual void vfn_20();                        // [20] @ 0x822539A8
    virtual void vfn_21();                        // [21] @ 0x82253D38
    virtual void vfn_22();                        // [22] @ 0x82254150
    virtual void vfn_23();                        // [23] @ 0x82254B40
    virtual void vfn_24();                        // [24] @ 0x82254958
    virtual void vfn_25();                        // [25] @ 0x82254D28
    virtual void vfn_26();                        // [26] @ 0x82254D90
    virtual void vfn_27();                        // [27] @ 0x82254F78
    virtual void vfn_28();                        // [28] @ 0x82254DF8
    virtual void vfn_29();                        // [29] @ 0x822550F8
    virtual void vfn_30();                        // [30] @ 0x82255160
    virtual void vfn_31();                        // [31] @ 0x82253920

    // Non-virtual methods
    void SetDampingAndStiffness(float arg1, float arg2);
    void SetLimitAtIndex(int index, float val);
    void AccumulateForces(int index, float* outVec);
    void AccumulateTorques(int index, float* outVec);
    void GetLowerLimit(int index, float* outVec);
    void GetUpperLimit(int index, float* outVec);
    void ComputeJacobian();
    void SetLimits();
};

// ── rage::phLevelBase  [vtable @ 0x82059F24] ─────────────────────────────────
struct phLevelBase {
    void**      vtable;           // +0x00
};

// ── rage::phLevelNew  [vtable @ 0x82059894] ──────────────────────────────────
//
// Physics world / level. Manages collision pairs, object placement,
// and broadphase/narrowphase dispatch.
struct phLevelNew {
    void**      vtable;           // +0x00

    uint8_t     m_bInitialized;   // +0x0004  R:1
    uint32_t    field_0x0008;     // +0x0008  R:1
    uint32_t    field_0x000c;     // +0x000c  R:2
    uint8_t     field_0x000d;     // +0x000d  W:2
    uint16_t    m_nObjects;       // +0x000e  R:6  — object count
    uint16_t    field_0x0010;     // +0x0010  R:3
    uint16_t    m_nActivePairs;   // +0x0012  R:4 W:4
    uint16_t    field_0x0014;     // +0x0014  R:2 W:2
    uint16_t    m_nCollisionPairs;// +0x0016  R:4 W:4
    uint16_t    m_nProcessedPairs;// +0x0018  R:10 W:4  — most-read counter
    uint16_t    field_0x001a;     // +0x001a  R:1
    uint32_t    field_0x001c;     // +0x001c  R:14 W:1
    uint32_t    field_0x0020;     // +0x0020  R:4 W:1
    uint32_t    field_0x0024;     // +0x0024  R:3 W:1
    uint32_t    field_0x0028;     // +0x0028  R:8 W:1
    uint32_t    field_0x0030;     // +0x0030  R:5
    uint8_t     _pad0x0034[28];
    uint32_t    field_0x0050;     // +0x0050  R:2
    uint32_t    field_0x0054;     // +0x0054  R:1
    uint32_t    field_0x0058;     // +0x0058  R:2
    uint32_t    field_0x005c;     // +0x005c  R:21 W:2  — heavily read
    uint32_t    field_0x0060;     // +0x0060  R:6 W:2
    uint32_t    field_0x0064;     // +0x0064  R:5 W:2
    uint32_t    field_0x0068;     // +0x0068  R:7 W:2
    uint16_t    field_0x006c;     // +0x006c  R:4 W:1
    uint16_t    m_broadphaseCount;// +0x006e  R:2 W:3
    uint32_t    field_0x0070;     // +0x0070  R:23 W:2  — most-read field
    uint32_t    field_0x0074;     // +0x0074  R:4 W:2

    virtual ~phLevelNew();                        // [0] @ 0x822DE2E0
    virtual void vfn_2();                         // [2] @ 0x822C50D8
    virtual void vfn_3();                         // [3] @ 0x822C5390
    virtual void vfn_4();                         // [4] @ 0x822C5638
    virtual void vfn_5();                         // [5] @ 0x822C5528
    virtual void vfn_6();                         // [6] @ 0x822C5698
    virtual void vfn_7();                         // [7] @ 0x822C5798
    virtual void vfn_8();                         // [8] @ 0x822C58B0
    virtual void vfn_9();                         // [9] @ 0x822C5898
    virtual void vfn_10();                        // [10] @ 0x822C4D78
    virtual void vfn_11();                        // [11] @ 0x822C5998
    virtual void vfn_12();                        // [12] @ 0x822C4E08
    virtual void vfn_13();                        // [13] @ 0x822C60B0
    virtual void vfn_14();                        // [14] @ 0x822C4D68
    virtual void vfn_15();                        // [15] @ 0x822C61B8
    virtual void vfn_16();                        // [16] @ 0x822C61D0
    virtual void vfn_17();                        // [17] @ 0x822DE3B8
    virtual void vfn_18();                        // [18] @ 0x822C6208
    virtual void vfn_19();                        // [19] @ 0x822C60B8
    virtual void vfn_20();                        // [20] @ 0x822C6138
    virtual void vfn_21();                        // [21] @ 0x822C85F0
    virtual void vfn_22();                        // [22] @ 0x822C76E8
    virtual void vfn_23();                        // [23] @ 0x822C7A38
    virtual void vfn_24();                        // [24] @ 0x822C7E78
    virtual void vfn_25();                        // [25] @ 0x822C8880
    virtual void vfn_26();                        // [26] @ 0x822C5030
    virtual void vfn_27();                        // [27] @ 0x822C8B50
    virtual void vfn_28();                        // [28] @ 0x822C72D0
    virtual void vfn_29();                        // [29] @ 0x822C7350
};

// ── rage::phMaterialMgr  [vtable @ 0x8204EA3C] ───────────────────────────────
struct phMaterialMgr {
    void**      vtable;           // +0x00

    virtual ~phMaterialMgr();                     // [0] @ 0x82228BE8
    virtual void vfn_7();                         // [7] @ 0x82410900
    virtual void vfn_14();                        // [14] @ 0x824109A0
    virtual void vfn_15();                        // [15] @ 0x82410AF0
};

// ── rage::phMorphGeometry  [vtable @ 0x820650F4] ─────────────────────────────
struct phMorphGeometry {
    void**      vtable;           // +0x00

    virtual ~phMorphGeometry();   // [0] @ 0x82345A48
    virtual void vfn_36();        // [36] @ 0x82345D90
};

// ── rage::phRope  [vtable @ 0x820650B0] ──────────────────────────────────────
struct phRope {
    void**      vtable;           // +0x00

    virtual ~phRope();                            // [0] @ 0x82342A38
    virtual void ScalarDtor(int flags);           // [1] @ 0x823433A0
    virtual void vfn_3();                         // [3] @ 0x82342060
    virtual void vfn_5();                         // [5] @ 0x823435D8
    virtual void vfn_6();                         // [6] @ 0x823454A0
    virtual void vfn_8();                         // [8] @ 0x82344AF8
};

// ── rage::phSimulator  [vtable @ 0x8205976C] ─────────────────────────────────
// Confirmed methods: FindWeakestInst
// Field hints: m_maxBreakableComponents:int
struct phSimulator {
    void**      vtable;           // +0x00

    uint32_t    field_0x0004;     // +0x0004  R:10 W:3
    uint32_t    field_0x0008;     // +0x0008  R:7
    uint32_t    field_0x000c;     // +0x000c  R:5 W:2
    uint32_t    field_0x0010;     // +0x0010  R:4 W:2
    uint32_t    field_0x0014;     // +0x0014  R:3
    uint32_t    field_0x0018;     // +0x0018  R:4
    uint32_t    field_0x001c;     // +0x001c  R:8
    uint32_t    field_0x0020;     // +0x0020  R:18
    uint32_t    field_0x0024;     // +0x0024  R:30 W:1  — most-read
    uint32_t    field_0x0028;     // +0x0028  R:12
    uint16_t    field_0x002c;     // +0x002c  R:5
    uint32_t    field_0x0030;     // +0x0030  R:4
    uint16_t    field_0x0034;     // +0x0034  R:3
    uint8_t     _pad0x0036[40];
    uint32_t    field_0x0060;     // +0x0060  R:5
    uint32_t    field_0x0064;     // +0x0064  R:2
    uint8_t     _pad0x0068[24];
    uint32_t    field_0x0080;     // +0x0080  R:3
    uint8_t     _pad0x0084[332];
    uint32_t    field_0x01d0;     // +0x01d0  R:3
    uint8_t     _pad0x01d4[80];
    uint32_t    field_0x0224;     // +0x0224  R:4 W:3
    uint32_t    field_0x0228;     // +0x0228  R:1 W:3
    uint32_t    field_0x022c;     // +0x022c  R:5 W:3
    uint8_t     _pad0x0230[16504];
    uint32_t    m_maxBreakableComponents;  // +0x42a8  R:5 — max breakable component count
    uint32_t    field_0x42ac;     // +0x42ac  R:5
    uint32_t    field_0x42b0;     // +0x42b0  R:3
    uint32_t    field_0x42b4;     // +0x42b4  R:4

    virtual ~phSimulator();                       // [0] @ 0x822C14A0
    virtual void ScalarDtor(int flags);           // [1] @ 0x822C4C18
    virtual void vfn_2();                         // [2] @ 0x822C4228
    virtual void vfn_3();                         // [3] @ 0x822C1D18
    virtual void vfn_4();                         // [4] @ 0x822C1F00
    virtual void vfn_5();                         // [5] @ 0x822C4308
    virtual void vfn_6();                         // [6] @ 0x822C3128
    virtual void vfn_7();                         // [7] @ 0x822C40C0

    // debug string: "phSimulator::FindWeakestInst(): Maximum number of breakable components %d exceeded: %d"
    void FindWeakestInst();
};

// ── rage::phSleep  [vtable @ 0x8203327C] ─────────────────────────────────────
struct phSleep {
    void**      vtable;           // +0x00

    virtual ~phSleep();                           // [0] @ 0x822C1A50
    virtual void Activate();                       // [1] @ 0x822DD238
    virtual void vfn_2();                         // [2] @ 0x822DD248
};

// ── rage::phSpringAninmatedAttachment  [vtable @ 0x8206AD20] ─────────────────
// Note: name preserved from RTTI (typo: "Aninmated" is original)
struct phSpringAninmatedAttachment {
    void**      vtable;           // +0x00

    virtual void ScalarDtor(int flags);  // [1] @ 0x82382548
    virtual void vfn_5();                // [5] @ 0x82421B98
};

} // namespace rage

// ── phObject ─────────────────────────────────────────────────────────────────
// Generic physics object participating in physics updates.
struct phObject {
    void** vtable;            // +0x00
    uint8_t _pad0x04[0x44];   // +0x04
    void* m_pData;            // +0x30 (48)
    phObject* m_pChild;       // +0x34 (52)  — child object (Release'd in ReleaseViews)
    uint8_t _pad0x38[0x38];   // +0x38 (56)
    uint32_t m_field_112;     // +0x70 (112)
    uint32_t m_field_116;     // +0x74 (116)
    uint32_t m_field_120;     // +0x78 (120)
    uint8_t _pad0x7C[0x08];   // +0x7C (124)
    phObject* m_pParent;      // +0x84 (132)  — set to 'this' in ReleaseViews
    uint8_t _pad0x88[0x16C];  // +0x88 (136)
    uint32_t m_nViewCount;    // +0x1F4 (500) — zeroed in ReleaseViews
    uint32_t m_field_504;     // +0x1F8 (504)
    void* m_pResource;        // +0x1FC (508)
    uint64_t m_field_512;     // +0x200 (512)
    uint32_t m_field_520;     // +0x208 (520)
    uint32_t m_field_524;     // +0x20C (524)
    uint32_t m_field_528;     // +0x210 (528)

    virtual ~phObject();                               // [0]
    virtual void* vfn_1(void*);                        // [1]
    virtual void vfn_2();                              // [2]
    virtual void vfn_3();                              // [3]
    virtual void vfn_4();                              // [4]
    virtual void* vfn_5();                             // [5]
    virtual void* vfn_6();                             // [6]
    virtual void* vfn_7();                             // [7]
    virtual void* vfn_8();                             // [8]
    virtual void* vfn_9();                             // [9]
    virtual void* vfn_10();                            // [10]
    virtual void vfn_11(void*, void*, void*);          // [11]
    virtual void* vfn_12();                            // [12]
    virtual void vfn_13(void*);                        // [13]
    virtual int32_t vfn_14(int32_t);                   // [14]
    virtual int32_t vfn_15();                          // [15]
    virtual void* vfn_16();                            // [16]
    virtual void* vfn_17(void*);                       // [17]
    virtual void* vfn_18();                            // [18]
    virtual void vfn_19();                             // [19]
    virtual void vfn_20();                             // [20]
    virtual void* vfn_21();                            // [21]
    virtual void* vfn_22();                            // [22]
    virtual void* vfn_23();                            // [23]
    virtual void* vfn_24();                            // [24]
    virtual void* vfn_25();                            // [25]
    virtual void* vfn_26();                            // [26]
    virtual void* vfn_27();                            // [27]
    virtual void* vfn_28();                            // [28]
    virtual void* vfn_29();                            // [29]
    virtual void* vfn_30();                            // [30]
    virtual void* vfn_31();                            // [31]
    virtual void vfn_32();                             // [32]

    // Non-virtual methods
    void ResetState();
    void Initialize(void* a2, void* a3, void* a4);
    void* GetDescription();
    void SetDescription(void* a2);
    int32_t QueryInterface(int32_t);
    int32_t Release();
    void* CreateResource();
    void* CreateViews(phObject* param);
    void* CreateOutputViews();
    void ReleaseViews();
    void ReleaseResources();
};
