/**
 * pcr_anim_blenders.cpp — Pong Creature Facial Animation Blender
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Manages facial animation blending for player creatures during gameplay events.
 * After a point is scored, StartPostPoint() kicks off a facial expression sequence:
 * it activates a body animation clip, resolves the corresponding face material from
 * a per-creature hash table, and links the result into the creature's animation state.
 *
 * Key relationships:
 *   pcrFaceAnimBlender owns a pointer to a crAnim-family object (m_pAnimObj, +0x90)
 *   that drives the body clip timeline, and a pointer to the creature's shared state
 *   block (m_pCreatureState, +0x84) which holds the face material registry and the
 *   currently active face-anim handle.
 */

#include "../anim/pcr_anim_blenders.hpp"
#include "../game/pong_creature.hpp"
#include "../physics/ph_material_mgr.hpp"
#include "rage/memory.h"

// Parent-class destructor — rage_1188 = crAnimDof or similar base
extern void rage_1188(void* obj);

// Material-manager singleton guard: if the pointer at +4 is non-null,
// a face-anim material operation is already in flight; skip re-entry.
// @ 0x825C9EB0  (.data, 20 bytes)
extern uint8_t lbl_825C9EB0[20];

// One-byte dirty flag cleared whenever we push a new face-anim handle.
// @ 0x82606324  (.data, 1 byte, SDA r2-relative offset 25380)
extern uint8_t g_faceAnimDirty;

// Initial blend phase constant loaded from .rdata and passed to the body-anim
// vtable call as well as written into an outgoing handle's timer field.
// Raw value: 0x92b75a4c (~-1.16e-27 — functionally 0.0 for timing purposes)
// @ 0x8202D110  (.rdata, 4 bytes)
extern const float kFaceAnimStartPhase;

// ----------------------------------------------------------------------------
// pcrFaceAnimBlender destructor
// @ 0x820C0C50 | size: 0x50
//
// Standard RAGE two-phase destructor.  The hidden bool parameter (bit 0 of the
// original r4) tells us whether the backing memory should be freed — this is the
// pattern used throughout the engine to support both stack-allocated and
// heap-allocated instances without code duplication.
// ----------------------------------------------------------------------------
pcrFaceAnimBlender::~pcrFaceAnimBlender()
{
    // Tear down the parent class (crAnimDof or similar base).
    // After this call, the vtable still points here, but base members are gone.
    rage_1188(this);

    // Memory ownership is tracked at the call site via the flags word.
    // When the object was new'd, bit 0 is set and the runtime calls
    // rage_free here to release the allocation.
    // (In lifted C++ the delete expression handles this automatically.)
}

// ----------------------------------------------------------------------------
// pcrFaceAnimBlender::StartPostPoint
// @ 0x820CC490 | size: 0x114
//
// Called by pongPlayer when a point is scored.  Activates the creature's
// post-point body animation and, if face animation is enabled on this creature,
// looks up the matching face material in the creature-state registry and wires
// it up as the active face-anim handle.
//
// The function returns the raw activation token from the body-anim vtable call
// (low byte non-zero = successfully started).  Callers (pongPlayer_Process,
// pongPlayer_D228_g) typically ignore this return value.
// ----------------------------------------------------------------------------
void* pcrFaceAnimBlender::StartPostPoint()
{
    // --- Step 1: kick off the body animation ---
    //
    // Vtable slot 5 on the animation object is the "activate" entry point.
    // Arguments (PPC calling convention):
    //   r3 = m_pAnimObj (implicit this of the anim object)
    //   r4 = 0          (reset / "start fresh" mode)
    //   r5 = stack buf  (output parameter block, 64 bytes on the caller's frame)
    //   f1 = kFaceAnimStartPhase   (initial blend phase, ~0.0)
    //
    // The low byte of the returned handle indicates whether the clip was found
    // and activation succeeded.
    struct AnimObj {
        void** vtable;
        // ... other fields omitted ...
        void**   m_pClipArray;       // +0x190 (400)  — pointer array of clip entries
        int32_t  m_activeClipIndex;  // +0x1AC (428)  — current clip, -1 = none
    };

    AnimObj* animObj = m_pAnimObj;
    void*    activationHandle = animObj->vtable[5](animObj, 0, kFaceAnimStartPhase);

    // If activation failed (low byte == 0), nothing more to do.
    if ((reinterpret_cast<uintptr_t>(activationHandle) & 0xFF) == 0)
        return activationHandle;

    // --- Step 2: validate creature state ---
    pcrCreatureState* cs = m_pCreatureState;
    if (!cs)
        return activationHandle;

    // --- Step 3: resolve the active clip's animation data pointer ---
    //
    // The body-anim object tracks an array of clip entries.  Each entry has a
    // sub-object at +16 that in turn holds the actual clip data pointer at +32.
    // We need that pointer to look up the matching face material below.
    void* clipData = nullptr;
    int32_t clipIndex = animObj->m_activeClipIndex;
    if (clipIndex >= 0)
    {
        // clipArray[clipIndex]->field_0x10->field_0x20
        void*  clipEntry  = animObj->m_pClipArray[clipIndex];
        void*  clipSubObj = *reinterpret_cast<void**>(static_cast<char*>(clipEntry) + 16);
        clipData          = *reinterpret_cast<void**>(static_cast<char*>(clipSubObj) + 32);
    }

    // --- Step 4: check per-creature face-anim enable flag ---
    if (!cs->m_bFaceAnimEnabled)
        return activationHandle;

    // --- Step 5: singleton guard ---
    //
    // The pointer at lbl_825C9EB0+4 acts as a re-entry / "in-flight" guard.
    // If it is already set, a material operation is currently active and we
    // should not initiate another one.
    void* guardPtr = *reinterpret_cast<void**>(lbl_825C9EB0 + 4);
    if (guardPtr != nullptr)
        return activationHandle;

    // --- Step 6: look up the face material for this clip ---
    //
    // phMaterialMgrImpl_C208_g is a hash-table lookup inside the creature's
    // face-material registry (an inline struct at creature_state+0xB0).
    // It returns a pointer to the matched entry's value field, or null on miss.
    void* materialResult = phMaterialMgrImpl_C208_g(&cs->m_faceMaterial, clipData);
    if (!materialResult)
        return activationHandle;

    // --- Step 7: retire the old handle if present ---
    //
    // Before switching to the new handle we reset the old one's elapsed-time
    // field and clear the global dirty flag so the renderer picks up the change.
    void* oldHandle = cs->m_pFaceAnimHandle;
    if (oldHandle)
    {
        // Reset the outgoing handle's timer back to the start phase.
        *reinterpret_cast<float*>(static_cast<char*>(oldHandle) + 0x14) = kFaceAnimStartPhase;

        // Clear the global "face-anim dirty" flag (SDA global @ 0x82606324).
        g_faceAnimDirty = 0;
    }

    // --- Step 8: install the new handle ---
    //
    // materialResult points to the value cell inside the hash entry.
    // Dereference once to get the actual handle object pointer.
    void* newHandle = *reinterpret_cast<void**>(materialResult);
    cs->m_pFaceAnimHandle = newHandle;

    // NOTE: a follow-up call here (nop_8240E6D0) was stripped to a no-op in
    // the release build.  In debug it likely triggered a material-bind
    // notification using the face-anim blend table at lbl_82027684.
    // @ 0x820CC590 — bl 0x8240E6D0 (blr)

    return activationHandle;
}
