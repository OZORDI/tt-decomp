/**
 * pcr_anim_blenders.cpp — Pong Creature Animation Blenders
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements three blender classes that drive creature animation during
 * specific gameplay phases:
 *
 *   pcrFaceAnimBlender    — facial expression blending after a point is scored
 *   pcrEmoteBlender       — emote / reaction animations (celebration, frustration)
 *   pcrPostPointBlender   — full body post-point animation sequence controller
 *
 * ─── pcrFaceAnimBlender layout ────────────────────────────────────────────
 *   m_pCreatureState  (+0x84, 132)  — pcrCreatureState* for this creature
 *   m_pAnimObj        (+0x90, 144)  — crAnimDof-family object driving the body clip
 *
 * Relevant pcrCreatureState fields (accessed from StartPostPoint):
 *   +0x04  bool                m_bFaceAnimEnabled  — guards face-anim path
 *   +0x44  void*               m_pFaceAnimHandle   — active face-anim handle
 *   +0xB0  phFaceMaterialTable m_faceMaterial      — hash table for face materials
 *
 * ─── pcrEmoteBlender layout ───────────────────────────────────────────────
 *   m_pPlayer         (+0x04)   — owning pongPlayer
 *   m_pSessionInfo    (+0x5C)   — per-session state
 *   m_emoteIndex      (+0x180)  — which emote anim is active (stored at +0x100*4+384)
 *   m_state           (+0x58)   — playback state uint16 (1 = active)
 *   m_pCurrentClip    (+0x54)   — pointer to active crAnimClip
 *   m_blendWeightA    (+0x50)   — blend weight A (in sub-struct at +16+80)
 *   m_blendWeightB    (+0x54)   — blend weight B (in sub-struct at +16+84)
 *   m_targetBlend     (+0x64)   — target blend weight (at +100)
 *   m_bShouldStop     (+0x5A)   — uint16: stop after current cycle
 *   m_animSubStruct   (+0x10)   — inline locomotion-blender sub-object
 *
 * ─── pcrPostPointBlender layout ───────────────────────────────────────────
 *   m_pPlayer         (+0x04)   — owning pongPlayer
 *   m_animSubStruct   (+0x10)   — inline crAnimBlender sub-object
 *   m_pSessionInfo    (+0x5C)   — per-session state
 *   m_pCurrentClip    (+0x54)   — active clip pointer (in blender sub-struct)
 *   m_blendWeight     (+0x70)   — blend weight for this layer (at sub+80)
 *   m_bForced         (+0x1A8)  — byte: blender was force-started (at +424)
 *   m_currentState    (+0x58)   — uint16 state word (at +88)
 *   m_pActiveClipObj  (+0x54)   — active clip (at +84)
 *   m_clipArray       (+0x190)  — pointer array of crAnimClip* (at +400)
 *   m_clipRefCounts   (+0x1A0)  — per-slot play counts (at +416)
 *   m_activeClipIdx   (+0x1AC)  — current clip index; -1 = none (at +428)
 *   m_pCelebString    (+0x1B0)  — atString fields (at +432/416/408 area)
 *   m_bComplete       (+0x1A8)  — byte at +424: sequence is done
 */

#include "../game/pong_creature.hpp"
#include "../physics/ph_physics.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Externals
// ─────────────────────────────────────────────────────────────────────────────

// crAnimDof / crAnimBlend base class destructor.
// @ 0x820C1188
extern void pcrAnimBlender_Initialize(void* obj);

// RAGE heap deallocator.
// @ 0x820C00C0
extern void rage_free(void* ptr);

// Hash-table lookup in pcrCreatureState::m_faceMaterial.
// Returns a pointer to the matched entry's value cell, or null on miss.
// @ 0x8234C208
extern void* phMaterialMgrImpl_C208_g(void* table, const void* key);

// No-op in release; debug would fire a material-bind notification.
// @ 0x8240E6D0
extern void nop_8240E6D0(const void*, const void*, uint32_t);

// Animation blender control functions
extern void crAnimBlenderState_Init(void* obj);  // @ 0x8224C810  reset/clear
extern void util_C930(void* obj);                // @ 0x8224C930  query active clip
extern void util_CDF0(void* blender,
                      const void* clipDesc,
                      uint32_t flags);           // @ 0x8224CDF0  start clip
extern void SetPageGroupVisible(void* obj);              // @ 0x8224CFE0  GetCurrentClip
extern void PostPageGroupMessage(uint32_t msg,
                    uint8_t  flags,
                    uint32_t, uint32_t,
                    uint32_t, uint32_t);         // @ 0x8225E6E0  send message
extern void pcrAnimBlender_ApplyClipWeights(void* player,
                                             uint32_t clipRef,
                                             float weightA,
                                             float weightB);  // @ 0x820CF420

// pongPlayer helper: checks sequence completion and notifies the player.
// Returns true if the player acknowledged and transitioned state.
// @ 0x820CDE98
extern bool pongPlayer_DE98_g(void* player);

// Selects the next clip index to play from the post-point clip array.
// Returns -1 if no valid clip is found.
// @ 0x820DE040
extern int32_t pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender* self);

// atSingleton helpers for sub-struct setup.
extern void rage::InitializeSingleton(void* blenderSubStruct);  // @ 0x821A8588
extern void rage::UnregisterSingleton(void* descriptor);        // @ 0x820C29E0
extern void atArray_Clear(void* obj);                        // @ 0x821A8070

// ─────────────────────────────────────────────────────────────────────────────
// Constants
// ─────────────────────────────────────────────────────────────────────────────

// Initial blend phase constant (~0.0f) used when activating the body clip.
// @ 0x8202D110  (.rdata, 4 bytes)
extern const float kFaceAnimStartPhase;

// SDA global: dirty flag cleared when a new face-anim handle is installed so
// the renderer picks up the change on the next frame.
// @ 0x82606324  (SDA r2-relative offset 25380)
extern uint8_t g_faceAnimDirty;

// Singleton re-entry guard for the material manager.
// If [+4] is non-null, a material operation is already in flight.
// @ 0x825C9EB0  (.data, 20 bytes)
extern uint8_t g_matMgrGuard[20];


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  pcrFaceAnimBlender                                                      ║
// ╚══════════════════════════════════════════════════════════════════════════╝

/**
 * pcrFaceAnimBlender::~pcrFaceAnimBlender  [vtable slot 0]
 * @ 0x820C0C50 | size: 0x50
 *
 * Tears down the crAnimDof parent class.  Memory freeing is handled by the
 * caller (delete expression in lifted C++; raw flag check in the recomp).
 */
pcrFaceAnimBlender::~pcrFaceAnimBlender()
{
    pcrAnimBlender_Initialize(this);
}

/**
 * pcrFaceAnimBlender::StartPostPoint
 * @ 0x820CC490 | size: 0x114
 *
 * Called by pongPlayer when a point is scored.  Activates the creature's
 * post-point body animation, then — if face animation is enabled for this
 * creature — looks up the matching face material and installs it as the
 * active face-anim handle.
 *
 * Returns the activation token from the body-anim vtable call.  The low byte
 * is non-zero on success.  Most callers ignore this value.
 */
void* pcrFaceAnimBlender::StartPostPoint()
{
    // ── Activate the body-animation clip (vtable slot 5 = "Activate") ──────
    //
    // The crAnimDof-family vtable[5] call signature:
    //   Activate(this, mode=0, phase)
    //   mode  = 0   → start-fresh / reset
    //   phase = kFaceAnimStartPhase (~0.0f)
    //   returns a handle token; low byte non-zero = clip found and started.

    struct IAnimObj {
        void**   vtable;
        // ... (interior fields omitted)
        void**   m_pClipArray;    // +0x190 (400) — array of clip-entry pointers
        int32_t  m_activeClipIdx; // +0x1AC (428) — index of current clip, -1 = none
    };

    IAnimObj* animObj = static_cast<IAnimObj*>(m_pAnimObj);
    typedef void* (*ActivateFn)(IAnimObj*, int32_t, float);
    void* activationToken =
        reinterpret_cast<ActivateFn>(animObj->vtable[5])(animObj, 0, kFaceAnimStartPhase);

    // Bail out early if the clip was not found or could not be started.
    if ((reinterpret_cast<uintptr_t>(activationToken) & 0xFF) == 0)
        return activationToken;

    // ── Validate creature state ───────────────────────────────────────────
    pcrCreatureState* cs = (pcrCreatureState*)m_pCreatureState;
    if (!cs)
        return activationToken;

    // ── Resolve the active clip's data pointer ────────────────────────────
    //
    // The body-anim object maintains an array of clip entries.
    // Each entry carries a sub-object at +16 that holds the clip data at +32.
    // We need this pointer to key into the face-material hash table below.
    void* clipData = nullptr;
    int32_t clipIdx = animObj->m_activeClipIdx;
    if (clipIdx >= 0)
    {
        void*  clipEntry  = animObj->m_pClipArray[clipIdx];
        void*  clipSubObj = *reinterpret_cast<void**>(
                                static_cast<char*>(clipEntry) + 16);
        clipData          = *reinterpret_cast<void**>(
                                static_cast<char*>(clipSubObj) + 32);
    }

    // ── Per-creature face-anim enable guard ───────────────────────────────
    if (!cs->m_bFaceAnimEnabled)
        return activationToken;

    // ── Singleton re-entry guard ──────────────────────────────────────────
    //
    // If the guard pointer at g_matMgrGuard+4 is already set, a material
    // operation is currently in flight — skip to avoid re-entrancy.
    void* guardPtr = *reinterpret_cast<void**>(g_matMgrGuard + 4);
    if (guardPtr != nullptr)
        return activationToken;

    // ── Look up the face material for this clip ───────────────────────────
    //
    // phMaterialMgrImpl_C208_g performs a hash-table lookup inside the
    // creature's face-material registry (an inline struct at cs+0xB0).
    // Returns a pointer to the matched value cell, or null on miss.
    void* materialResult = phMaterialMgrImpl_C208_g(&cs->m_faceMaterial, clipData);
    if (!materialResult)
        return activationToken;

    // ── Retire the old face-anim handle ──────────────────────────────────
    void* oldHandle = cs->m_pFaceAnimHandle;
    if (oldHandle)
    {
        // Reset the outgoing handle's elapsed-time field back to start phase.
        *reinterpret_cast<float*>(static_cast<char*>(oldHandle) + 0x14) =
            kFaceAnimStartPhase;

        // Clear the renderer's dirty flag so it picks up the handle switch.
        g_faceAnimDirty = 0;
    }

    // ── Install the new face-anim handle ─────────────────────────────────
    //
    // materialResult points at the value cell inside the hash entry.
    // Dereference once to obtain the handle object pointer.
    void* newHandle = *reinterpret_cast<void**>(materialResult);
    cs->m_pFaceAnimHandle = newHandle;

    // NOTE: the release build optimised away a material-bind notification call
    // here (0x8240E6D0 is a blr stub).  Debug builds would have fired it.
    // nop_8240E6D0(blendTable, newHandle, cs->m_emoteIndex);

    return activationToken;
}


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  pcrEmoteBlender                                                         ║
// ╚══════════════════════════════════════════════════════════════════════════╝

/**
 * pcrEmoteBlender::~pcrEmoteBlender  [vtable slot 0]
 * @ 0x820CB878 | size: 0x40
 *
 * Restores the base-class vtable pair (primary + secondary) then optionally
 * frees the allocation.
 */
pcrEmoteBlender::~pcrEmoteBlender()
{
    // Restore base-class vtables (primary @ +0, secondary @ +16).
    // Both are reset to the base pongBlender vtables before the object is torn down.
    // (Handled implicitly by the C++ runtime in lifted code.)
}

/**
 * pcrEmoteBlender::SetPlayer  [vtable slot 1]
 * @ 0x820DB508 | size: ~0x10
 *
 * Stores the owning player pointer and copies the session info reference
 * from the player's state block.
 */
void pcrEmoteBlender::SetPlayer(void* pPlayer, void* pSessionBlock)
{
    m_pPlayer      = pPlayer;
    // Session info pointer lives at pSessionBlock+4.
    m_pSessionInfo = *reinterpret_cast<void**>(
                         static_cast<char*>(pSessionBlock) + 4);
}

/**
 * pcrEmoteBlender::SetEmoteIndex  [vtable slot 2]
 * @ 0x820CB8D0 | size: 0x10
 *
 * Stores the emote animation index used when selecting the clip to play.
 */
void pcrEmoteBlender::SetEmoteIndex(uint32_t index)
{
    m_emoteIndex = index;  // +0x180 (384)
}

/**
 * pcrEmoteBlender::Reset  [vtable slot 3]
 * @ 0x820DB518 | size: ~0x10
 *
 * Clears the locomotion-blender sub-object back to its initial state.
 */
void pcrEmoteBlender::Reset()
{
    crAnimBlenderState_Init(&m_animSubStruct);  // sub-struct at +0x10
}

/**
 * pcrEmoteBlender::Update  [vtable slot 4]
 * @ 0x820DB520 | size: 0xCC
 *
 * Per-frame update.  Checks whether the emote is active and if the current
 * clip has finished, then notifies the player and sends a UI message.
 */
void pcrEmoteBlender::Update()
{
    // Query the locomotion blender's current active state.
    bool wasActive = (reinterpret_cast<bool(*)(pcrEmoteBlender*)>(
        reinterpret_cast<void**>(vtable)[6]))(this);  // vfn_6 = IsActive

    util_C930(&m_animSubStruct);  // polls the blender sub-struct for state

    // Re-query active state after the poll.
    bool isNowActive = (reinterpret_cast<bool(*)(pcrEmoteBlender*)>(
        reinterpret_cast<void**>(vtable)[6]))(this);

    // Query whether we should loop: vtable[10] = ShouldLoop.
    void* loopDesc;
    (reinterpret_cast<void(*)(pcrEmoteBlender*, void*)>(
        reinterpret_cast<void**>(vtable)[10]))(this, &loopDesc);

    // If the blender sub-struct shows exactly one active entry (state == 1)
    // and we have a valid clip pointer, push blend weights to the player.
    if (m_state == 1 && m_pCurrentClip)
    {
        pcrAnimBlender_ApplyClipWeights(m_pPlayer,
                                         *reinterpret_cast<uint32_t*>(
                                             static_cast<char*>(m_pCurrentClip) + 32),
                                         m_blendWeightA,
                                         m_blendWeightB);

        // If the transition from active → inactive just occurred and we have
        // a valid emote index, send the completion message to the UI.
        if (!wasActive && isNowActive)
        {
            // (wasActive/isNowActive polarity: active here means "clip running")
        }
        else if (wasActive && !isNowActive)
        {
            uint32_t bit = (uint32_t(1) << m_emoteIndex);
            PostPageGroupMessage(0x100F,            // message id 4111
                    uint8_t(bit | 64), // channel flags
                    0, 0, 0, 0);
        }
    }
}

/**
 * pcrEmoteBlender::StartEmote  [vtable slot 5]
 * @ 0x820DB620 | size: 0xB8
 *
 * Begins playing the emote animation described by clipDesc.  Normalises the
 * blend-in duration (f1) clamping it to [0, 1], stores the resulting weight
 * pair in the sub-struct, and optionally fires vtable[10] if the duration is
 * non-zero.
 */
void pcrEmoteBlender::StartEmote(const void* clipDesc, uint32_t flags, float blendDuration)
{
    bool looping = (*reinterpret_cast<const uint32_t*>(
                       static_cast<const char*>(clipDesc) + 4) & 1) != 0;

    util_CDF0(&m_animSubStruct, clipDesc, looping ? 1u : 0u);

    // Compute normalised blend weight from the duration parameter.
    float duration = *reinterpret_cast<const float*>(
                         static_cast<const char*>(clipDesc) + 12);
    float weight   = (duration > kFaceAnimStartPhase)
                     ? blendDuration / duration
                     : 0.0f;

    // Clamp to [0, 1].
    weight = (weight > 1.0f) ? 1.0f : weight;

    m_blendWeightA = weight;  // +0x10+80  (sub-struct relative)
    m_blendWeightB = weight;  // +0x10+84
    m_targetBlend  = 0.0f;    // reset target at +100

    // If there is a positive duration, notify the locomotion blender.
    if (blendDuration > kFaceAnimStartPhase)
    {
        (reinterpret_cast<void(*)(pcrEmoteBlender*, void*)>(
            reinterpret_cast<void**>(vtable)[10]))(this, const_cast<void*>(
                static_cast<const void*>(&flags)));
    }
}

/**
 * pcrEmoteBlender::IsFinished  [vtable slot 7]
 * @ 0x820DB5F8 | size: 0x20
 *
 * Returns true if the emote clip has completed (m_state == 1 and the clip
 * pointer is non-null); false otherwise.
 */
bool pcrEmoteBlender::IsFinished() const
{
    if (m_state == 1 && m_pCurrentClip)
        return true;
    return false;
}

/**
 * pcrEmoteBlender::IsActive  [vtable slot 6]
 * @ 0x820CB8D8 | size: 0x30
 *
 * Returns true if the blender sub-struct's active flag (bit 0 of m_bShouldStop
 * at +90) is set.  Uses cntlzw/rlwinm to convert the bit to a bool.
 */
bool pcrEmoteBlender::IsActive() const
{
    return (m_bShouldStop & 1) != 0;
}

/**
 * pcrEmoteBlender::GetCurrentClip  [vtable slot 8]
 * @ 0x820CB908 | size: ~0x10
 *
 * Returns the current clip pointer from the locomotion-blender sub-struct.
 */
void* pcrEmoteBlender::GetCurrentClip()
{
    SetPageGroupVisible(&m_animSubStruct);  // fills the sub-struct's active clip field
    return m_pCurrentClip;
}

/**
 * pcrEmoteBlender::GetBlendWeights  [vtable slot 9]
 * @ 0x820DB6E8 | size: 0x40
 *
 * Writes the current blend weight pair into the output parameters if the
 * emote is active (m_state == 1 and clip is valid), otherwise writes zeros.
 */
bool pcrEmoteBlender::GetBlendWeights(float* outWeightA, float* outWeightB) const
{
    if (m_state == 1 && m_pCurrentClip)
    {
        *outWeightA = m_blendWeightA * *reinterpret_cast<const float*>(
                          static_cast<const char*>(m_pCurrentClip) + 12);
        *outWeightB = *reinterpret_cast<const float*>(
                          static_cast<const char*>(m_pCurrentClip) + 12);
        return true;
    }
    *outWeightA = 0.0f;
    *outWeightB = 0.0f;
    return false;
}

/**
 * pcrEmoteBlender::ComputeBlend  [vtable slot 10]
 * @ 0x820DB728 | size: ~0xA0
 *
 * Computes and stores the blend-weight vector into the output buffer based on
 * the current state.  If active (m_state == 1, clip valid), uses the stored
 * blend weights; otherwise writes a zero vector.
 */
void pcrEmoteBlender::ComputeBlend(void* outBlendVec)
{
    // Zero the output vector (16-byte aligned AltiVec slot).
    float* out = static_cast<float*>(outBlendVec);
    out[0] = out[1] = out[2] = 0.0f;
    out[3] = 1.0f;  // w component (loaded from kFaceAnimStartPhase constant block)

    if (m_state == 1 && m_pCurrentClip)
    {
        // Drive the locomotion blender sub-struct.
        SetPageGroupVisible(&m_animSubStruct);
        // TODO: full AltiVec SIMD blend computation — see recomp vfn_10 for detail.
    }
}


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  pcrPostPointBlender                                                     ║
// ╚══════════════════════════════════════════════════════════════════════════╝

/**
 * pcrPostPointBlender::~pcrPostPointBlender  [vtable slot 0]
 * @ 0x820DDCB8 | size: ~0xA0
 *
 * Frees the clip-data sub-struct (at +436), the ref-count array (at +432),
 * and the three atString buffers for the celebratory clip names
 * (at +416/+408/+400), then tears down the two base-class vtable chains
 * and optionally frees the allocation.
 */
pcrPostPointBlender::~pcrPostPointBlender()
{
    // Destroy the clip-data sub-struct at +436 (atArray_Clear handles teardown).
    atArray_Clear(&m_animSubStruct2);  // sub-struct 2 at +436

    // Free the ref-count pointer at +432 if present.
    rage_free(m_pRefCountArray);

    // Free atString buffers conditionally on their length fields.
    if (m_celebNameCap)   rage_free((void*)m_pCelebNameA);  // +416 if +422 != 0
    if (m_celebNameBCap)  rage_free((void*)m_pCelebNameB);  // +408 if +414 != 0
    if (m_celebNameCCap)  rage_free((void*)m_pCelebNameC);  // +400 if +406 != 0

    // The base-class destructors restore the vtable pair and, if the
    // free-flag bit is set in r4, call rage_free on the object itself.
    // (Handled by delete in lifted C++.)
}

/**
 * pcrPostPointBlender::Init  [vtable slot 1]
 * @ 0x820DDD78 | size: ~0x80
 *
 * Initialises the blender with the owning player pointer and session info.
 * Sets up both animation sub-structs by allocating descriptors via
 * rage::InitializeSingleton / rage::UnregisterSingleton.
 */
void pcrPostPointBlender::Init(void* pPlayer, void* pSessionBlock)
{
    m_pPlayer = pPlayer;
    // Session info pointer at pSessionBlock+4.
    m_pSessionInfo = *reinterpret_cast<void**>(
                         static_cast<char*>(pSessionBlock) + 4);

    // Initialise first animation sub-struct (at +444).
    rage::InitializeSingleton(&m_animSubStruct);  // at +444; allocates 16-byte block
    // rage::UnregisterSingleton resolves the descriptor string for sub-struct 1.
    // ... (two-step alloc pattern mirrored from recomp vfn_1 body)
    // TODO: confirm exact descriptor string addresses from SDA map.
}

/**
 * pcrPostPointBlender::ResetState  [vtable slot 3]
 * @ 0x820DDE28 | size: ~0x30
 *
 * Resets the blender to an inactive state: clears m_bForced, sets the active
 * clip index to -1 (0xFFFFFFFF), and resets the locomotion sub-struct.
 */
void pcrPostPointBlender::ResetState()
{
    m_bForced      = 0;       // +424
    m_activeClipIdx = -1;     // +428  (stored as 0xFFFFFFFF)
    crAnimBlenderState_Init(&m_animSubStruct);  // sub-struct at +16
}

/**
 * pcrPostPointBlender::IsPlaying  [vtable slot 4]
 * @ 0x820DDE48 | size: ~0x10
 *
 * Returns true if m_bForced (+424) is non-zero.
 */
bool pcrPostPointBlender::IsPlaying() const
{
    return m_bForced != 0;  // +424
}

/**
 * pcrPostPointBlender::Update  [vtable slot 5]
 * @ 0x820DDE60 | size: ~0x140
 *
 * Per-frame update for the post-point blender.  Reads the current ball-speed
 * value from the player's physics state, determines the animation direction
 * (forehand vs. backhand) and checks whether the current clip has ended.
 * When a clip ends, selects the next clip from the library and notifies the
 * player via pongPlayer_DE98_g, then sends a UI completion message.
 */
void pcrPostPointBlender::Update()
{
    // The owning player's state block is at m_pPlayer (stored at +4).
    // Physics/action state is at m_pPlayer->field_168->field_70.
    void* playerStateBlock =
        *reinterpret_cast<void**>(static_cast<char*>(m_pPlayer) + 168);
    void* actionState =
        *reinterpret_cast<void**>(static_cast<char*>(playerStateBlock) + 112);

    // f0 = speed/direction value at actionState+48.
    float speed = *reinterpret_cast<float*>(static_cast<char*>(actionState) + 48);

    // A global player-side flag at SDA + (-21720) controls direction negation.
    // If the flag at playerStateBlock+0xCC+64 is set, negate speed.
    bool  bMirror = (*reinterpret_cast<uint8_t*>(
                         static_cast<char*>(actionState) + 204 + 64)) != 0;
    if (bMirror) speed = -speed;

    // Check against zero threshold.
    if (speed > 0.0f)
    {
        // Speed is positive — check via pongPlayer_DE98_g whether the
        // current sequence has ended and the player is ready to transition.
        if (pongPlayer_DE98_g(
                *reinterpret_cast<void**>(static_cast<char*>(m_pPlayer) + 188)))
        {
            // Try to select the next clip.
            m_activeClipIdx = pcrPostPointBlender_GetActiveClipIndex(this);  // @ +428
            if (m_activeClipIdx >= 0)
            {
                void*  clipEntry = reinterpret_cast<void**>(m_clipArray)[m_activeClipIdx];
                void*  clipObj   = *reinterpret_cast<void**>(
                                       static_cast<char*>(clipEntry) + 16);

                // Start the selected clip in the locomotion sub-struct.
                util_CDF0(&m_animSubStruct, clipObj, 0);

                if (clipObj)
                {
                    // Notify via the (stripped) face-anim blend table.
                    // nop_8240E6D0 is a blr in release.

                    // Send post-point completion message to the UI.
                    uint32_t bit = (uint32_t(1) << m_emoteIndex);
                    PostPageGroupMessage(0x100F,           // msg 4111
                            uint8_t(bit | 64),
                            0, 0, 0, 0);

                    // If the active clip index is still negative after
                    // reselection, mark as complete.
                    m_activeClipIdx = pcrPostPointBlender_GetActiveClipIndex(this);
                    if (m_activeClipIdx < 0)
                    {
                        m_bForced = !m_bForced ? 0 : 1;  // cntlzw/rlwinm NOT idiom
                        return;
                    }

                    // Increment the per-slot play count.
                    m_clipRefCounts[m_activeClipIdx]++;
                }
            }
            else
            {
                m_activeClipIdx = -1;
            }
        }
    }

    // Reflect final NOT-of-m_bForced into m_bComplete.
    m_bForced = m_bForced ? 0 : 1;
}

/**
 * pcrPostPointBlender::IsActive  [vtable slot 6]
 * @ 0x820DDFF8 | size: ~0x40
 *
 * Returns true if the blender is in a playing state.
 * Checks m_bForced (+424) first; if clear, also inspects the locomotion
 * sub-struct's state word (bit 0 of m_bShouldStop at +90) and whether a
 * current clip is loaded.
 */
bool pcrPostPointBlender::IsActive() const
{
    if (m_bForced != 0) return false;

    // Check locomotion sub-struct active flag (bit 0 of m_bShouldStop at +90).
    bool subActive = (m_bShouldStop & 1) != 0;
    if (!subActive)        return false;
    if (!m_pActiveClipObj) return false;
    return true;
}

/**
 * pcrPostPointBlender::IsFinished  [vtable slot 7]
 * @ 0x820DDFB0 | size: ~0x50
 *
 * Returns true if the blender is not playing and the locomotion sub-struct
 * is not active.
 */
bool pcrPostPointBlender::IsFinished() const
{
    if (m_bForced != 0) return false;

    // Mirrors pcrEmoteBlender::IsActive but for the post-point sub-struct.
    bool subActive = (*reinterpret_cast<const uint16_t*>(
                          static_cast<const char*>(static_cast<const void*>(this))
                          + 90) & 1) != 0;

    // If sub-struct not active, also check the primary clip object.
    if (!subActive) return false;
    return m_pActiveClipObj != nullptr;
}
