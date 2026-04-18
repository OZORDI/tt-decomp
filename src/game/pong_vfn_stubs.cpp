/**
 * pong_vfn_stubs.cpp — Small virtual-method stubs (FLIRT batch 00)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Scope
 * -----
 * This file holds the small (1-2 instruction) virtual-method slot bodies
 * that FLIRT batch 00 surfaced. Every function in this file was flagged
 * by IDA's FLIRT pass as one of:
 *   ?OLIST_GetObjSize@XGRAPHICS@@...
 *   ?IsAlloc@IRInst@XGRAPHICS@@...
 *   XGComparePixelShaders[_NN]
 *   ?OperationInputs@IRTrinary@XGRAPHICS@@...
 *   ?OperationInputs@IRBinary@XGRAPHICS@@...
 *   ?hasMoreData@SocketXBOX@meq@@...
 *   ?FA_GetMode@XGRAPHICS@@...
 *
 * Triage via `get_function_recomp` on every address proved each body was
 * genuine game code (proto-RAGE vtable thunks returning small constants or
 * trivial accessor values) that FLIRT had mis-matched against an XGRAPHICS
 * or D3DXShader prologue signature. The RTTI class name, pass5_final
 * symbol, and call graph all contradict the FLIRT label — so the entries
 * below are lifted under their true class-vfn identity.
 *
 * All are tiny enough to be inlinable leaf functions that the optimizer
 * will fold into callers once whole-program LTO is in place.
 *
 * Batch-00 address range: 0x820c4be8 .. 0x82187858
 */

#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations for sibling vtable slots that the entries below
// chain into. None of these are lifted yet; we only need their extern "C"
// signature so the link step can resolve the call.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    // fragInst base class — rage::fragInst vtable slot 30 (744 bytes,
    // @ 0x82120120, not yet lifted).
    void fragInst_vfn_30(void* self);
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofVector3::vfn_3   @ 0x820C4BE8 | size: 0x8
// vtable slot 3 — GetDofCount() for a Vector3 animation DOF. The
// fixed constant '32' is the per-DOF keyframe-slot budget that the
// animation streaming code pre-allocates for vector DOFs.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t crAnimDofVector3_vfn_3(void* /*pThis*/)
{
    return 32;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCreatureInst::vfn_30   @ 0x820C8F80 | size: 0x24
// vtable slot 30 — Called during fragment instantiation. Chains into the
// base-class rage::fragInst::vfn_30 for the real work, then swallows the
// return value and signals "no extra action required" by returning 0.
// The pattern appears across most pong-game pongCreatureInst derivatives.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t pongCreatureInst_vfn_30(void* pThis)
{
    fragInst_vfn_30(pThis);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// grmModelFactory::vfn_1   @ 0x820F3D10 | size: 0x18
// vtable slot 1 [scalar_destructor] — Forwarder to rage_1028 (the
// rage::grmModelFactory real constructor/initializer at 0x820F1028). The
// shim just slides r4..r8 into r3..r7 and tail-calls through; this pattern
// is the classic MSVC thiscall→stdcall thunk for a factory that isn't
// the first base in the vtable-layout flattening.
//
// We express the tail-call as an extern to rage_1028 because 0x820F1028 is
// a 696-byte singleton helper that is still in the pass5_final scaffold
// (src/rage/rage_system.cpp only references it by name in a comment).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_1028(void* objectList, void* typeInfo, void* flags,
                          void* pAlloc, void* pParent);

extern "C" void grmModelFactory_vfn_1(void* /*pThis*/, void* objectList,
                                      void* typeInfo, void* flags,
                                      void* pAlloc, void* pParent)
{
    // Drop 'this' (pass5_final shows the translator copies r4..r8 over
    // r3..r7 and branches — 'this' is never observed).
    rage_1028(objectList, typeInfo, flags, pAlloc, pParent);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiAsciiTokenizer::vfn_0   @ 0x8210CAF8 | size: 0x8
// vtable slot 0 — GetTokenKind()/GetType() returning 0 ("raw text" / default
// kind). The tokenizer's default handler yields a zero kind-tag so the
// parser treats unclassified input as literal.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t fiAsciiTokenizer_vfn_0(void* /*pThis*/)
{
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofQuaternion::vfn_20   @ 0x8210CBD0 | size: 0x8
// vtable slot 20 — IsQuaternionDof()/GetCategory(): Quaternion DOFs report
// category=1 (vs. Vector3's 0 and Scalar's 2) in the animation blender's
// DOF-kind switch.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t crAnimDofQuaternion_vfn_20(void* /*pThis*/)
{
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongTrainingDrill::vfn_23   @ 0x8210CBD8 | size: 0x8
// vtable slot 23 — getter at object offset +0x10. In the pongTrainingDrill
// layout, +0x10 is the "current drill record pointer" (see
// src/game/drill/pong_drill.hpp m_pActiveDrill-style field).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void* pongTrainingDrill_vfn_23(void* pThis)
{
    return *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(pThis) + 0x10);
}

// ─────────────────────────────────────────────────────────────────────────────
// xmlNodeStructServeType::vfn_6   @ 0x8210CC08 | size: 0x8
// vtable slot 6 — GetNodeKind() for serve-type XML nodes. Returns 2
// (xmlNodeStruct enum kServeTypeElement).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t xmlNodeStructServeType_vfn_6(void* /*pThis*/)
{
    return 2;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCreatureInst::vfn_1   @ 0x8210CC40 | size: 0x8
// vtable slot 1 — GetRTTIKind() returning 3 (pongCreatureInst in the
// rage::datBase RTTI-class registry).
// ─────────────────────────────────────────────────────────────────────────────
extern "C" int32_t pongCreatureInst_vfn_1(void* /*pThis*/)
{
    return 3;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongTrainingDrill::vfn_2   @ 0x8210CDA8 | size: 0x10
// vtable slot 2 — indirect dispatcher: forwards to vtable slot 3 of the
// same object (byte offset +12 in the vtable). Pattern produced when a
// subclass pins vfn_2 to be an alias for vfn_3 at link time.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void pongTrainingDrill_vfn_2(void* pThis)
{
    typedef void (*VfnSlot3)(void* self);
    void** vtable = *reinterpret_cast<void***>(pThis);
    VfnSlot3 slot3 = reinterpret_cast<VfnSlot3>(vtable[3]);
    slot3(pThis);
}

// ─────────────────────────────────────────────────────────────────────────────
// audControl3d::vfn_10   @ 0x82160760 | size: 0x8
// vtable slot 10 — getter at object offset +0x08. In the audControl3d
// layout this is the m_pSoundSource back-pointer that the 3D audio
// positioning code uses when the spatialiser wants to look up the owning
// emitter during per-frame pan/attenuation recomputation.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void* audControl3d_vfn_10(void* pThis)
{
    return *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(pThis) + 0x08);
}
