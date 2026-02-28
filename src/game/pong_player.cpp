/**
 * pongPlayer — Player state query functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Covers the small boolean state-query methods (D550–D6B8) that gate
 * swing/action initiation, plus the vector-computation helper D7B0 that
 * resolves the "current target position" for a pending swing.
 *
 * Struct field names come from offset-frequency clustering and debug strings.
 */

#include "pong_player.hpp"
#include "rage/memory.h"
#include <cstring>   // memset

// ---------------------------------------------------------------------------
// External functions referenced below
// ---------------------------------------------------------------------------

// Face animation blender — starts the post-point anim.
#include "pong_creature.hpp"

// Update function called from D228; purpose: TODO — likely syncs state.
extern void pongPlayer_C678_g(pongPlayer* state);  // @ 0x820CC678

// LocomotionStateAnim helpers — update animation list entries.
// C128 = standard transition, C288 = alternate (flag-gated) transition.
extern void LocomotionStateAnim_BlendLocomotionAnims(uint32_t entry, void* parent, float dt);
extern void LocomotionStateAnim_TransitionLocomotionState(uint32_t entry, void* parent, float dt);


// ── External game helpers referenced by pong_player.cpp ───────────────────

namespace rage { void UnbindObject(void* obj, void* arg); }  // @ atSingleton suite
extern void phBoundCapsule_A080_g(void* bound);  // only r3 used; r4-r6 computed internally
extern const float g_kSwingRadiusConst;  // constant from .rdata
extern const vec3  g_kZero_vec3;         // zero vector constant
extern const float g_kZero;              // zero float constant

// ── Additional external stubs for pong_player.cpp ───────────────────────
extern void* g_pInputScoreTable;         // global input score table (float array)
extern void* g_pLerpQueue;               // lerp queue global
extern void* g_pInputObj;               // input object global
extern void* g_pButtonStateTable;        // button state table (int32_t array)
extern const float g_kOneMinusAlpha;     // constant
// pongPlayer_B208_g: r3=playerState ptr, r4=swingVec, r5=uint8_t suppressFlip
extern void pongPlayer_B208_g(void* playerState, vec3* swingVec, uint8_t suppressFlip);
// pongPlayer_AB48_g: r3=out vec3, r4=physBound — both saved in prologue
extern void pongPlayer_AB48_g(vec3* out, void* physBound);
// pongPlayer_BF18_g: returns int; r3=playerState, r4=int, r5=swingStr*, r6=targetDir*, r7=int
extern int  pongPlayer_BF18_g(void* playerState, int r4, vec3* swingStrength, vec3* targetDir, int r5);
// pongPlayer_73E8_g: r3=this, f1=float  (confirmed from call site)
extern void pongPlayer_73E8_g(pongPlayer* p, float f);
// pongPlayer_42E0_g: r3=swingData, r5=outStrength, r6=outTarget, f1=alpha
extern void pongPlayer_42E0_g(void* swingData, vec3* outStrength, vec3* outTarget, float alpha);
extern void game_D468(void* obj, vec3* v);               // geometry helper
extern bool pongPlayer_6AA0_g(void* locoState);          // loco check
extern int  io_Input_poll_9D68(void* inputObj);           // 0=active input
extern void pongLerpQueue_3410_g(void* queue);            // lerp queue update
extern void game_D060(void* recoveryState);              // finalise serve recovery

struct pongSwingData {
    uint32_t vtable_ptr;  // +0   (32-bit game ptr kept as uint32 for layout)
    uint8_t  _pad1[337];  // +4..+340
    uint8_t  _pad341;     // +341
    uint8_t  m_bTriggered;   // +342  byte flag — 1 = swing triggered
    uint8_t  _pad343;     // +343
    float    m_swingStrength; // +344  swing power
    uint8_t  _pad348[4];  // +348..+351
    float    m_swingVec[3]; // +352  target vec3
};



extern void game_CD20(void* recoveryState);          // flush recovery @ 0x820DCD20
extern void crAnimBlenderState_Init(void* animSubState);  // reset phase-blocked @ 0x8224C810
extern void pcrAnimState_ComputePosition(vec3* out, pongAnimState* animState);  // compute anim pos
extern void pongPlayer_D238_g(pongPlayer* state);    // @ 0x820CD238 (or similar)
// extern void pongPlayer_D298_2hr(...);  // TODO: verify signature @ ~0x820CD298

// Logging no-op (debug only)
extern void nop_8240E6D0(const char* fmt, ...);

// Geometry / position helpers used by D7B0.
extern void* pg_9C00_g(void* singleton);              // → returns geometry record
extern void  pcrAnimState_ComputePosition(vec3* out, pongAnimState* animState); // → computes anim position

// Global singletons
extern void* g_geomSingleton;   // @ loaded via lis+lwz pattern in D7B0

// Global constants (data section)
extern const float g_recoveryTimerThreshold;   // @ 0x8202D108
extern const float g_swingPhaseThreshold;       // @ 0x8202D110
extern const vec3  g_hitVectorFlip;
// ── Additional globals for CheckButtonInput / OnButtonReleased / IsSwingApexReached ──
extern uint32_t  g_pClockObj;                    // clock object pointer @ SDA area
extern uint32_t* g_pPlayerSlotTable;             // per-slot index table
extern uint8_t*  g_pPlayerDataTable;             // 808-byte-per-entry player records
extern uint32_t  g_pPlayerDataTable_slot;        // active slot index mirror
extern float     g_kPowerBlend;                  // power blend constant @ 0x825C2EA0
extern float     g_kFrameToSecScale;             // frames-to-seconds   @ 0x8202D100

extern bool CheckButtonPressed(void* record);                              // pg_FFF8_g @ 0x8225FFF8
extern void PostPageGroupMessage(void* rec, int code, int mask, int a3, int opActive); // pg_E6E0
extern void pongPlayer_0508_g(void* obj, int maxSteps, float* outDelta);   // @ 0x821E0508
extern bool pongPlayer_5B60_gen(pongPlayer* p);                            // @ 0x82195B60
extern void pongPlayer_1460_g(void* actionState, int released);            // @ 0x821A1460
             // @ AltiVec constant used in D7B0 sign flip


// ===========================================================================
// SECTION 1 — State query methods
// ===========================================================================

/**
 * pongPlayerState::IsSwingTimerActive()  @ 0x820CD550 | size: 0x48
 *
 * Returns true while the swing timing sub-state's elapsed time has not yet
 * reached its target duration (i.e. the countdown is still running).
 */
bool pongPlayer::IsSwingTimerActive() const {
    if (!m_pTimingState) {
        return false;
    }
    return m_pTimingState->m_currentTime < m_pTimingState->m_targetTime;
}

/**
 * pongPlayerState::IsRecovering()  @ 0x820CD598 | size: 0x58
 *
 * Returns true when the player is locked in a post-swing recovery phase.
 * Either a force-block flag is set, or the recovery timer has not yet
 * counted up to the global threshold.
 */
bool pongPlayer::IsRecovering() const {
    if (!m_pRecoveryState) {
        return false;
    }
    if (m_pRecoveryState->m_bForceBlock) {
        return true;
    }
    return m_pRecoveryState->m_recoveryTimer < g_recoveryTimerThreshold;
}

/**
 * pongPlayerState::IsSwingPhaseBlocked()  @ 0x820CD5F0 | size: 0x70
 *
 * Returns true when the anim phase has exceeded the threshold, or when bit 0
 * of the anim state's flags word is set.  Returns false only when both the
 * phase is within range AND the flag is clear.
 *
 * In the caller (pongPlayer_69A0_g) a false return skips the swing path,
 * so this function acts as a "phase window open" gate.
 *
 * TODO: bso (NaN/SO) path around 0x820CD610 is not handled by scaffold —
 *       NaN most likely follows the "phase > threshold" (true) branch.
 */
bool pongPlayer::IsSwingPhaseBlocked() const {
    if (!m_pAnimState) {
        return false;
    }
    if (m_pAnimState->m_animPhase > g_swingPhaseThreshold) {
        return true;
    }
    return (m_pAnimState->m_flags & 0x1) != 0;
}

/**
 * pongPlayerState::IsCreatureStateReady()  @ 0x820CD660 | size: 0x54
 *
 * Compound gate: requires the state to be active (m_bActive != 0) AND the
 * creature-state sub-object to return true from its virtual IsReady (slot 7).
 */
bool pongPlayer::IsCreatureStateReady() const {
    if (!m_bActive) {
        return false;
    }
    return m_pCreatureState->IsReady();   // vtable slot 7
}

/**
 * pongPlayerState::IsCreatureState2Active()  @ 0x820CD6B8 | size: 0x80
 *
 * Checks the secondary creature-state object (m_pCreatureState2) via two
 * virtual queries: IsActive (slot 6) then IsReady (slot 7).  Both must pass.
 */
bool pongPlayer::IsCreatureState2Active() const {
    if (!m_pCreatureState2) {
        return false;
    }
    if (!m_pCreatureState2->IsActive()) {  // vtable slot 6
        return false;
    }
    return m_pCreatureState2->IsReady();   // vtable slot 7
}


// ===========================================================================
// SECTION 2 — D228: UpdateAnimationState  @ 0x820CD228 | size: 0x18C
// ===========================================================================
/**
 * pongPlayerState::UpdateAnimationState()  @ 0x820CD228
 *
 * Called from pongPlayer_69A0_g to synchronise the player's locomotion
 * animation list with the current state.  Specifically:
 *
 *  1. If the secondary creature state exists and its flag at +424 is set,
 *     start the post-point face animation.
 *  2. Call C678 (state sync helper).
 *  3. Evaluate whether the float field at +0xB0 (animation time) lies within
 *     the range [g_kZero, g_recoveryTimerThreshold].  This selects between
 *     two LocomotionStateAnim update functions (C128 vs C288).  An additional
 *     flag from the parent player's flags word (+4 → +20) can override to C288.
 *  4. Walk the animation list at this+32 (or this+80 in the alternate path)
 *     and call the selected update function on each entry, passing the stored
 *     float value and the parent pointer.
 *
 * TODO: the alternate list path (this+80) triggered by f0 >= g_kZero is not
 *       fully understood — the two paths are structurally identical except for
 *       the list base pointer.  Verify at 0x820CD294 / 0x820CD330.
 */
void pongPlayer::UpdateAnimationState() {
    // Step 1: optional post-point face animation.
    if (m_pCreatureState2) {
        // byte at creatureState2+424: some "post point" trigger flag
        uint8_t postPointFlag = *reinterpret_cast<uint8_t*>(
            reinterpret_cast<uintptr_t>(m_pCreatureState2) + 424);
        if (postPointFlag) {
            // TODO: Get pcrFaceAnimBlender instance and call StartPostPoint
            // The blender should be accessible through the creature state
            // pcrFaceAnimBlender* blender = m_pCreatureState2->GetFaceAnimBlender();
            // if (blender) {
            //     blender->StartPostPoint();
            // }
        }
    }

    // Step 2: sync state.
    pongPlayer_C678_g(this);

    // Step 3: range-check m_animTimer against [g_kZero, g_recoveryTimerThreshold].
    float animTime = m_animTimer;
    bool inRange = (animTime >= g_kZero) && (animTime <= g_recoveryTimerThreshold);

    // Also check bit 0 of flags word via parent pointer (this+28 → flags+20).
    // TODO: verify exact chain — scaffold shows lwz r29,28(r31); lwz r8,4(r29); bit 0 of +20
    bool useAltAnim = false;
    if (m_pParent) {
        uint32_t* flagsBase = *reinterpret_cast<uint32_t**>(
            reinterpret_cast<uintptr_t>(this) + 0x1C);
        uint32_t flagWord = flagsBase[20 / 4];   // +20
        useAltAnim = (flagWord & 0x1) != 0;
    }

    // Select update function.
    using AnimUpdateFn = void(*)(uint32_t, void*, float);
    AnimUpdateFn updateFn = (inRange && !useAltAnim)
        ? LocomotionStateAnim_BlendLocomotionAnims    // @ 0x8224C128
        : LocomotionStateAnim_TransitionLocomotionState;   // @ 0x8224C288

    // Step 4: iterate animation list.
    // List is at this+32 (in-range path) or this+80 (out-of-range path).
    // TODO: verify which base is correct for each branch.
    uintptr_t listBase = reinterpret_cast<uintptr_t>(this) + (inRange ? 0x50 : 0x20);
    uint16_t  count    = *reinterpret_cast<uint16_t*>(listBase + 12);
    uint32_t* entries  = *reinterpret_cast<uint32_t**>(listBase + 8);

    void* parent = reinterpret_cast<void*>(m_pParent);

    for (int i = 0; i < (int)count; ++i) {
        updateFn(entries[i], parent, g_recoveryTimerThreshold);
        // TODO: f1 = var_f31 (the "upper" range threshold) — double-check dt arg
    }
}


// ===========================================================================
// SECTION 3 — D7B0: GetSwingTargetVector  @ 0x820CD7B0 | size: 0x154
// ===========================================================================
/**
 * pongPlayerState::GetSwingTargetVector(vec3* out, pongPlayerState* state)
 *   @ 0x820CD7B0
 *
 * Fills *out with the 3D position that the current swing is aimed at.
 * There are three priority-ordered paths:
 *
 *  PATH A — Swing timer running (m_pTimingState->currentTime < targetTime):
 *    Looks up the geometry record for m_pAnimState->field_136 via pg_9C00_g,
 *    reads a pre-baked vector from the timing state (+80, 16-byte aligned),
 *    optionally flips its sign if a "mirror" flag doesn't match the geometry
 *    record's handedness, then stores the result.
 *
 *  PATH B — Swing phase open (IsSwingPhaseBlocked returns true):
 *    Computes the position analytically via pcrAnimState_ComputePosition(out, m_pAnimState).
 *
 *  PATH C — In recovery (IsRecovering returns true):
 *    Copies the 16-byte vector stored at m_pRecoveryState+0x70 (112) directly.
 *
 *  Fallback: all paths fail → *out is left as the zero-initialised value.
 *
 * TODO: the "mirror" xor check (byte at geomRecord[44][260] vs (base+offset)[64])
 *       needs cross-referencing with the actual geometry struct layout.
 */
void pongPlayer::GetSwingTargetVector(vec3* out, pongPlayer* state) const {
    // Zero-initialise output (matches the stfs f0 × 3 prologue in the scaffold).
    memset(out, 0, sizeof(vec3));

    pongTimingState* timing = state->m_pTimingState;

    // PATH A: timer is still counting down.
    if (timing && timing->m_currentTime < timing->m_targetTime) {
        // Retrieve geometry base via singleton lookup.
        void*    geomRecord = pg_9C00_g(g_geomSingleton);
        // TODO: var_r30 = state->m_pAnimState field at +136 — identify field name.
        uint32_t animSubOffset = *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uintptr_t>(state->m_pAnimState) + 136);

        // Read the 16-byte target vector from the timing state's aligned slot (+80).
        vec3 target;
        memcpy(&target,
               reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(timing) + 80),
               sizeof(vec3));

        // Mirror check: if handedness flags don't match, flip the vector sign.
        // byte at geomRecord+44→+260 vs byte at (animSubOffset + geomBase)[64]
        // TODO: verify exact pointer arithmetic — see vsubfp128 at 0x820CD868.
        uint8_t geomFlag = *reinterpret_cast<uint8_t*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(
                    reinterpret_cast<uintptr_t>(geomRecord) + 44))
            + 260);
        uint8_t animFlag = *reinterpret_cast<uint8_t*>(
            animSubOffset + reinterpret_cast<uintptr_t>(g_geomSingleton) + 64);
        if (geomFlag != animFlag) {
            target.x = g_hitVectorFlip.x - target.x;
            target.y = g_hitVectorFlip.y - target.y;
            target.z = g_hitVectorFlip.z - target.z;
        }

        *out = target;
        return;
    }

    // PATH B: swing phase window is open (IsSwingPhaseBlocked returns true here).
    if (state->IsSwingPhaseBlocked()) {
        pcrAnimState_ComputePosition(out, state->m_pAnimState);
        return;
    }

    // PATH C: player is in recovery — use the stored recovery vector.
    if (state->IsRecovering()) {
        memcpy(out,
               reinterpret_cast<void*>(
                   reinterpret_cast<uintptr_t>(state->m_pRecoveryState) + 112),
               sizeof(vec3));
    }
    // else: output stays zeroed.
}



// ===========================================================================
// SECTION 4 — C3C8: CanAcceptSwingInput  @ 0x820CC3C8 | size: 0xC8
// ===========================================================================
/**
 * pongPlayer::CanAcceptSwingInput()  @ 0x820CC3C8
 *
 * Returns true when the player's swing subsystem is in a fully neutral state
 * and is ready to register a new swing command.
 *
 * All five conditions must hold simultaneously:
 *   1. Swing phase is NOT blocked (IsSwingPhaseBlocked == false)
 *   2. Swing timer is NOT running: timing state exists AND
 *      currentTime >= targetTime (NaN treated as expired — see header note)
 *   3. NOT in recovery (IsRecovering == false)
 *   4. Secondary creature state is NOT active (IsCreatureState2Active == false)
 *   5. Primary creature state is NOT in "ready" state (IsCreatureStateReady == false)
 *
 * Condition 5 is counterintuitive: "ready" here means the creature state
 * machine is in a post-action "ready-for-next" state, which means it is still
 * completing the previous action's resolution. Returning false for "ready"
 * prevents double-queuing.
 *
 * Called from pongPlayer_73E8_g (the swing-input polling loop) on the inner
 * pongPlayerState object (this = pongPlayer+0xBC).
 */
bool pongPlayer::CanAcceptSwingInput() const
{
    // Gate 1: swing phase must not be blocked (anim hasn't passed the window).
    if (IsSwingPhaseBlocked())
        return false;

    // Gate 2: timing state — timer must be expired (currentTime >= targetTime).
    // An absent timing state is treated as expired (timer never started).
    if (m_pTimingState) {
        // `!(currentTime < targetTime)` is used instead of `currentTime >= targetTime`
        // so that NaN inputs (uninitialised timer) return true (expired), matching
        // the PPC `blt/bns` idiom — see header notes.
        bool timerRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);
        if (timerRunning)
            return false;
    }

    // Gate 3: recovery window must have elapsed.
    if (IsRecovering())
        return false;

    // Gate 4: secondary creature state must not be in mid-action.
    if (IsCreatureState2Active())
        return false;

    // Gate 5: primary creature state must NOT be in the "ready" sentinel state.
    // "Ready" = state machine just completed something and is waiting; return
    // false to prevent immediate re-entry into the swing path.
    if (IsCreatureStateReady())
        return false;

    return true;
}


// ===========================================================================
// SECTION 5 — D908: CancelSwing  @ 0x820CD908 | size: 0x150
// ===========================================================================
/**
 * pongPlayer::CancelSwing()  @ 0x820CD908
 *
 * Aborts the currently-pending swing and resets all associated state.
 * Safe to call even when no swing is active (guards protect all paths).
 *
 * Three independent cleanup paths (each only runs if the relevant subsystem
 * is currently active):
 *
 *  PATH A — Timing state active (timer running):
 *    Zero-fills the timing state's position/velocity data (12 floats across
 *    three vector slots), restores currentTime fields to g_kZero, clears the
 *    global "swing in flight" flag, and clears the m_bComplete byte.
 *    Debug-logs the creature pointer.
 *
 *  PATH B — In recovery (IsRecovering):
 *    Calls game_CD20 on the recovery state to flush queued recovery callbacks,
 *    then debug-logs the creature pointer.
 *
 *  PATH C — Swing phase blocked (IsSwingPhaseBlocked):
 *    Loads the anim state, calls crAnimBlenderState_Init at anim+16 (resets the anim
 *    "phase blocked" sub-system), zeroes the swing-phase float (m_swingPhase
 *    at +412), then debug-logs the creature pointer.
 *
 * All three paths are independent — multiple can fire in one call if several
 * subsystems are simultaneously active.
 */
void pongPlayer::CancelSwing()
{
    const float kZero = g_kZero;

    // ── PATH A: abort active swing timer ────────────────────────────────
    if (m_pTimingState)
    {
        bool timerRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);

        if (timerRunning) {
            // Zero the three vector slots and velocity fields.
            m_pTimingState->m_targetX   = kZero;
            m_pTimingState->m_targetY   = kZero;
            m_pTimingState->m_targetZ   = kZero;
            m_pTimingState->m_peakX     = kZero;
            m_pTimingState->m_peakY     = kZero;
            m_pTimingState->m_currentTime  = g_kZero; // the "start" float (g_kZero+8 in const block)
            m_pTimingState->m_currentTime2 = g_kZero;
            g_swingInFlightFlag          = 0;          // global @ lis(-32160)+25404
            m_pTimingState->m_vel0      = kZero;
            m_pTimingState->m_vel1      = kZero;
            m_pTimingState->m_vel2      = kZero;
            m_pTimingState->m_vel3      = kZero;       // +64,+68,+72
            m_pTimingState->m_vel4      = kZero;       // +80,+84
            g_swingCountFlag            = 0;           // stw r10,25408(r9) @ lis(-32160)
            m_pTimingState->m_vel5      = kZero;       // +88
            m_pTimingState->m_bComplete = 0;           // byte +141
            nop_8240E6D0("pongPlayer::CancelSwing() timing reset", (uintptr_t)m_pCreature);
        }
    }

    // ── PATH B: flush recovery callbacks ────────────────────────────────
    if (IsRecovering())
    {
        game_CD20(m_pRecoveryState);     // flush recovery state @ 0x820DCD20
        nop_8240E6D0("pongPlayer::CancelSwing() recovery flush", (uintptr_t)m_pCreature);
    }

    // ── PATH C: reset anim phase-blocked state ───────────────────────────
    if (IsSwingPhaseBlocked())
    {
        pongAnimState* anim = m_pAnimState;
        crAnimBlenderState_Init((uint8_t*)anim + 16);   // reset phase-blocked sub-system @ 0x8224C810
        anim->m_swingPhase = kZero;        // clear swing phase progress (+412)
        nop_8240E6D0("pongPlayer::CancelSwing() anim reset", (uintptr_t)m_pCreature);
    }
}


// ===========================================================================
// SECTION 6 — DA58: IsSwingApexReached  @ 0x820CDA58 | size: 0x27C
// ===========================================================================
/**
 * pongPlayer::IsSwingApexReached(float threshold)  @ 0x820CDA58
 *
 * Complex multi-path gate that determines whether the active swing has
 * progressed past a caller-supplied strength threshold. Returns true when
 * the swing should be registered as having "connected".
 *
 * The function encapsulates four priority-ordered evaluation paths:
 *
 *  PATH 0 — Active flag + creature state (prerequisite):
 *    If m_bActive is set, verifies that the creature state (m_pCreatureState2,
 *    slot 7 = IsReady) confirms the player is active. If either check fails
 *    the result is false and the remaining paths are skipped.
 *
 *    If the prerequisite passes → continue to PATH 1.
 *    If the prerequisite fails  → evaluate PATHS 2/3 without PATH 1's result.
 *
 *  PATH 1 — Creature state 2 blocking:
 *    If IsCreatureState2Active, return false immediately.
 *
 *  PATH 2 — Swing timer path:
 *    If IsSwingTimerActive: read m_pSwingSubState->m_swingProgress (+160).
 *    If progress > 0 ("swing has launched"), compute a normalised fraction:
 *       fraction = float(frameId at +340) * g_kSwingScaleFactor / swingProgress
 *    Compare m_pTimingState->m_peakX (+28 = recorded target position component)
 *    against this fraction. Return result of comparison.
 *
 *  PATH 3 — Swing phase path (IsSwingPhaseBlocked):
 *    Read anim state m_swingPhase (+412) against g_kZero.
 *    If phase > 0, look up the current frame from the anim array
 *    (m_pAnimState->m_pFrameArray[m_animProgress * 4 → slot][+16 → velocity record][+12 → float]).
 *    Compute normalised fraction same as PATH 2.
 *    Return comparison of m_swingPhase vs fraction.
 *
 *  PATH 4 — Recovery path:
 *    If IsRecovering: check recovery state byte at +44 (m_bForceBlock).
 *    If force-blocked → false. Otherwise:
 *      Call pongPlayer_D238_g to get the recovery progress float.
 *      Compare recovery float at +172 against caller's threshold (f1 arg).
 *      Return true if recovery is complete past threshold.
 *
 * @param threshold  Caller-supplied strength threshold (passed in f1).
 *                   Used in PATH 4 only.
 */
bool pongPlayer::IsSwingApexReached(float threshold) const
{
    // ── PATH 0: active + creature state prerequisite ────────────────────
    bool activeAndReady = false;
    if (m_bActive) {
        if (m_pCreatureState2) {
            activeAndReady = m_pCreatureState2->IsReady();  // vtable slot 7
        }
    }

    if (!activeAndReady) {
        // ── PATH 1: creature state 2 block ──────────────────────────────
        if (IsCreatureState2Active())
            return false;

        // ── PATH 2: swing timer ──────────────────────────────────────────
        if (IsSwingTimerActive()) {
            if (!m_pSwingSubState)
                goto path3;

            float swingProgress = m_pSwingSubState->m_swingProgress;  // +160
            if (swingProgress <= g_kZero)
                goto path3;

            // Normalise frame count to [0, 1] range relative to total swing duration.
            float fraction;
            if (swingProgress > g_kZero) {
                int16_t frameId = (int16_t)m_pSwingSubState->m_frameId;  // +340
                fraction = (float)frameId * g_kSwingScaleFactor / swingProgress;
            } else {
                fraction = g_kZero;
            }

            float currentPos = m_pTimingState->m_peakX;  // +28 = recorded position component
            return currentPos > fraction;
        }

    path3:
        // ── PATH 3: swing phase blocked ──────────────────────────────────
        if (IsSwingPhaseBlocked()) {
            float swingPhase = m_pAnimState->m_swingPhase;  // +412
            if (swingPhase <= g_kZero)
                goto path4;

            // Look up the current anim frame's velocity record.
            uint32_t frameIdx   = (uint32_t)m_pAnimState->m_animProgress;  // +96 (index into array)
            void**   frameArray = (void**)m_pAnimState->m_pFrameArray;      // +4
            void*    frame      = frameArray[frameIdx];                     // array[idx]
            void*    velocRec   = *(void**)((uint8_t*)frame + 16);          // frame->velocityRecord

            float velFloat = *(float*)((uint8_t*)velocRec + 12);

            float fraction;
            if (velFloat > g_kZero) {
                int32_t frameId = *(int32_t*)((uint8_t*)frame + 56);       // frame->frameId (int)
                fraction = (float)frameId * g_kSwingScaleFactor / velFloat;
            } else {
                fraction = g_kZero;
            }

            // Return true if current phase >= fraction (past the apex).
            // NaN-safe: `!(phase < fraction)` matches bns idiom.
            return !(swingPhase < fraction);
        }

    path4:
        // ── PATH 4: recovery ──────────────────────────────────────────────
        if (!IsRecovering())
            return false;

        // Force-block flag prevents any connection during mandatory recovery.
        if (*(uint8_t*)((uint8_t*)m_pRecoveryState + 44))   // m_bForceBlock
            return false;

        // Recovery state also needs a valid follow-up action to compare against.
        if (!*(void**)((uint8_t*)m_pRecoveryState + 36))     // m_pFollowAction
            return false;

        // Evaluate recovery progress via helper.
        pongPlayer_D238_g((pongPlayer*)this);   // updates recovery float at +172
        float recoveryVal = *(float*)((uint8_t*)m_pRecoveryState + 172);

        // NaN-safe comparison — matches bns idiom.
        return !(recoveryVal < threshold);  // true if recovery ≥ threshold
    }

    return false;
}


// ===========================================================================
// SECTION 7 — DCD8: IsBeforeSwingPeak  @ 0x820CDCD8 | size: 0xE8
// ===========================================================================
/**
 * pongPlayer::IsBeforeSwingPeak()  @ 0x820CDCD8
 *
 * Returns true when a swing is active AND the timing has not yet reached
 * the swing apex (peak time).
 *
 * Two-phase check:
 *
 *  PHASE 1 — Timer active gate:
 *    m_pTimingState must exist AND currentTime < targetTime.
 *    If the timer is not running, proceed to phase 2 (recovery path).
 *
 *  PHASE 2a — Pre-peak check (timer IS running):
 *    Re-read currentTime and compare against peakTime (+36).
 *    Returns true if peakTime > currentTime (haven't hit the apex yet).
 *    Returns false if peakTime <= currentTime (at or past apex).
 *
 *  PHASE 2b — Recovery fallback (timer NOT running):
 *    If IsRecovering, delegate to pongPlayer_D298_2hr on the recovery state.
 *    Otherwise return false.
 *
 * Called from pongPlayer_5800_h (the per-frame swing state machine tick).
 * Used to gate the "swing ascending" phase of animation — actions are
 * allowed before the peak, suppressed after.
 */
bool pongPlayer::IsBeforeSwingPeak() const
{
    // ── PHASE 1: is the swing timer running? ────────────────────────────
    bool timerRunning = false;
    if (m_pTimingState) {
        timerRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);
    }

    if (timerRunning) {
        // ── PHASE 2a: pre-peak test ──────────────────────────────────────
        // First re-confirm timer is still running (redundant but matches asm).
        bool stillRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);

        if (stillRunning) {
            // True if we are before the peak: peakTime > currentTime.
            float currentTime = m_pTimingState->m_currentTime;   // +28
            float peakTime    = m_pTimingState->m_peakTime;       // +36
            return peakTime > currentTime;
        }
        return false;
    }

    // ── PHASE 2b: recovery fallback ─────────────────────────────────────
    if (IsRecovering()) {
        return false;  // TODO: pongPlayer_D298_2hr — verify recovery fallback logic
    }

    return false;
}


// ===========================================================================
// SECTION 8 — DDC0: IsSwingSystemIdle  @ 0x820CDDC0 | size: 0xD4
// ===========================================================================
/**
 * pongPlayer::IsSwingSystemIdle()  @ 0x820CDDC0
 *
 * Returns true when the entire swing subsystem is completely at rest and
 * no residual state is blocking the next action.
 *
 * This is a stricter version of CanAcceptSwingInput(): it requires ALL
 * subsystems to be inactive simultaneously, with NO exceptions.
 *
 * Returns false (not idle) if ANY of:
 *   - IsSwingPhaseBlocked()         — anim phase window still open
 *   - Swing timer running           — m_pTimingState current < target
 *   - IsCreatureStateReady()        — creature in post-action "ready" state
 *   - IsCreatureState2Active()      — secondary creature state is active
 *   - IsRecovering()                — recovery timer still counting
 *
 * Returns true only when ALL return false.
 *
 * Used as a pre-condition for serve/reset operations where the player
 * must be fully neutral before any new sequence begins.
 */
bool pongPlayer::IsSwingSystemIdle() const
{
    // Any blocked phase → not idle.
    if (IsSwingPhaseBlocked())
        return false;

    // Timer still running → not idle.
    if (m_pTimingState) {
        bool timerRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);
        if (timerRunning)
            return false;
    }

    // Creature state in post-action resolution → not idle.
    if (IsCreatureStateReady())
        return false;

    // Secondary creature state still active → not idle.
    if (IsCreatureState2Active())
        return false;

    // Still in recovery window → not idle.
    if (IsRecovering())
        return false;

    return true;
}


// ===========================================================================
// SECTION 9 — DE98: IsInReturnPosition  @ 0x820CDE98 | size: 0x168
// ===========================================================================
/**
 * pongPlayer::IsInReturnPosition()  @ 0x820CDE98
 *
 * Determines whether the player is correctly positioned and oriented to
 * execute a return stroke. This combines timer state, locomotion flags,
 * and a distance/angle check against the target position.
 *
 * Called from pcrPostPointBlender_vfn_5, which controls the "post point"
 * blending animation — suggesting this gate fires between rallies when
 * deciding if the player has reached their return stance.
 *
 * Two main branches:
 *
 *  BRANCH A — Swing timer running (currentTime < targetTime):
 *    Read locomotion sub-state flags at `(*m_pSwingSubState + 32) + 312`.
 *    Check bit 3 (0x8): if set → return true (player is in swing-committed zone).
 *    Check bit 2 (0x4): if set → return true.
 *    Call pongPlayer_6AA0_g on the locomotion state to evaluate positioning.
 *    If not positioned, compute the angular distance:
 *      - Load m_pBody->speed (float at +12 relative to m_playerIndex ptr)
 *      - Load frame count from locomotion state uint16 at +308
 *      - If speed > 0: fraction = float(frameCount) * g_kSwingScaleFactor / speed
 *      - Compare m_pTimingState->m_currentTime against fraction
 *      - Return !(currentTime < fraction) — true when timer has passed the threshold
 *
 *  BRANCH B — Swing phase blocked OR in recovery (fallback):
 *    Check IsSwingPhaseBlocked → if blocked, also check IsRecovering.
 *    Return true if IsRecovering (the player is in a valid return-ready state).
 *
 * TODO: The "angular distance" computation (speed * scale / denominator) may
 *       actually be a normalised time, not an angle. Confirm against
 *       pongPlayer_6AA0_g's semantics.
 */
bool pongPlayer::IsInReturnPosition() const
{
    // ── BRANCH A: swing timer active ────────────────────────────────────
    bool timerRunning = false;
    if (m_pTimingState) {
        timerRunning = !(m_pTimingState->m_currentTime < m_pTimingState->m_targetTime);
    }

    if (timerRunning) {
        // The locomotion sub-state lives at m_pSwingSubState + 32.
        uint8_t* locoState = (uint8_t*)m_pSwingSubState + 32;
        uint8_t  flags     = locoState[312];

        // Bit 3: player already committed to swing zone → ready.
        if (flags & 0x8)
            return true;

        // Bit 2: alternate commitment flag → ready.
        if (flags & 0x4)
            return true;

        // Check whether the player has reached the return position spatially.
        if (false /* TODO: pongPlayer_6AA0_g(locoState) — verify signature @ 0x820D6AA0 */)
            return true;

        // Compute normalised arrival fraction from frame count and body speed.
        uint16_t frameCount = *(uint16_t*)(locoState + 308);
        void*    bodyPtr    = *(void**)((uint8_t*)this + 16);         // m_pBody / m_pPlayerIndex
        float    speed      = *(float*)((uint8_t*)bodyPtr + 12);      // speed float

        float fraction;
        if (speed > g_kZero) {
            fraction = (float)(int16_t)frameCount * g_kSwingScaleFactor / speed;
        } else {
            fraction = g_kZero;
        }

        float currentTime = m_pTimingState->m_currentTime;  // +28
        // True when elapsed time has reached or passed the arrival fraction.
        return !(currentTime < fraction);
    }

    // ── BRANCH B: phase/recovery fallback ───────────────────────────────
    if (!IsSwingPhaseBlocked())
        return false;

    return IsRecovering();
}



// ===========================================================================
// SECTION 10 — pongPlayer::~pongPlayer  (vfn_0) @ 0x8218B3C0 | size: 0x50
// vtable slot 0 [destructor]
//
// Standard RAGE scalar destructor.  Runs the atSingleton base destructor
// chain via rage::UnbindObject (which in turn calls each registered sub-
// system's Deactivate/Destroy), then conditionally frees the object itself
// if the `flags` argument has bit 0 set (the C++ "delete after dtor" ABI
// convention used throughout this codebase).
//
// @param flags  Bit 0 set → free this pointer via rage_free after dtor.
// ===========================================================================
pongPlayer::~pongPlayer()
{
    rage::UnbindObject(this, 0);  // base class dtor chain — does NOT free this
}

void pongPlayer::ScalarDtor(int flags)
{
    rage::UnbindObject(this, 0);  // base class dtor chain
    if (flags & 1)
        rage_free(this);     // ABI-level "delete" — free the allocation
}


// ===========================================================================
// SECTION 11 — UpdatePhysicsAndSwingInput  @ 0x820C5FB8 | size: 0x10C
// Called from: pongPlayer_Update1
//
// Per-frame synchronisation of physics state and swing input targeting.
// Runs as part of the main update loop before the swing decision is made.
//
// Operations, in order:
//  1. Call vtable slot 3 on m_pPhysicsBody with (body+16) as argument —
//     updates the physics pose / bounding box for the current frame.
//  2. Call phBoundCapsule_A080_g on m_pPhysicsBound — updates the capsule.
//  3. If m_pPlayerState->m_pSwingData exists, call its vtable slot 1 with
//     the capsule radius float (from g_kSwingRadiusConst) — notifies the
//     swing data object of the current body scale.
//  4. Read m_swingInputResult (+200): if it equals 7, suppress vector flip.
//  5. Build a velocity-scaled 3D vector from the physics bound's own floats
//     (+56, +60) multiplied by g_kSwingRadiusConst (+8) using AltiVec vmulfp.
//  6. Call pongPlayer_B208_g to update the swing prediction with this vector.
//  7. Store m_pPhysicsBody float at +72 into g_pInputScoreTable[m_swingInputSlot].
//     This writes the current "power" reading into the input score array so
//     the swing evaluator can pick it up.
// ===========================================================================
void pongPlayer::UpdatePhysicsAndSwingInput()
{
    // ── 1. Physics body pose update (vtable slot 3) ──────────────────────
    typedef void (*BodyUpdateFn)(void*, void*);
    void** bodyVtbl = *(void***)m_pPhysicsBody;
    ((BodyUpdateFn)bodyVtbl[3])(m_pPhysicsBody,
                                 (uint8_t*)m_pPhysicsBody + 16);

    // ── 2. Capsule shape update ───────────────────────────────────────────
    phBoundCapsule_A080_g(m_pPhysicsBound);

    // ── 3. Notify swing data of current capsule radius ────────────────────
    pongSwingData* swingData = m_pPlayerState ? m_pPlayerState->m_pSwingData : nullptr;
    if (swingData) {
        float radius = g_kSwingRadiusConst;  // @ 0x825CAEB8 + 8
        typedef void (*SwingDataNotifyFn)(pongSwingData*, float);
        void** sdVtbl = *(void***)swingData;
        ((SwingDataNotifyFn)sdVtbl[1])(swingData, radius);
    }

    // ── 4. Flip suppression check ─────────────────────────────────────────
    // Result code 7 means the swing has already "flipped" — suppress further flip.
    bool suppressFlip = (m_swingInputResult == 7);
    (void)suppressFlip;  // used downstream in B208 call via f1 arg

    // ── 5. Build velocity-scaled swing vector ─────────────────────────────
    // Read two floats from the physics bound (+56 = vx, +60 = vy as xy of direction),
    // scale by the radius constant (splat into vector, multiply).
    float vx = *(float*)((uint8_t*)m_pPhysicsBound + 56);
    float vy = *(float*)((uint8_t*)m_pPhysicsBound + 60);
    float scale = g_kSwingRadiusConst;
    vec3 swingVec = { vx * scale, vy * scale, 0.0f };

    // ── 6. Update swing prediction ────────────────────────────────────────
    // r3=m_pPlayerState, r4=swingVec, r5=0 (no flip suppression) per recomp call site
    pongPlayer_B208_g(m_pPlayerState, &swingVec, 0);

    // ── 7. Write power reading into input score table ─────────────────────
    // m_pPhysicsBody float at +72 is the current "power" value for this player.
    float power = *(float*)((uint8_t*)m_pPhysicsBody + 72);
    ((float*)g_pInputScoreTable)[m_swingInputSlot] = power;


}  // end UpdatePhysicsAndSwingInput



// SECTION 12 — ProcessSwingDecision  @ 0x820C7038 | size: 0x210
// Called from: pongPlayer_StartSwing, ref_PostLoadProperties_CCB0
//
// The main per-frame swing state-machine driver.  Given a target direction
// and various flags, evaluates the current swing status and dispatches to
// the appropriate outcome path.
//
// @param cancelFirst  r6 — if true, cancel any in-progress swing before
//                     starting the new evaluation (used on new ball serve)
// @param pTargetVec   r7 — optional 16-byte aligned vector giving the
//                     explicit target direction; if NULL, the direction is
//                     read from input (poll → D42E0) or predicted (AB48)
// @param r4, r5       pass-through args forwarded to pongPlayer_BF18_g
//
// Swing result codes (from pongPlayer_BF18_g):
//   0 = idle               — no swing in progress, nothing to do
//   1 = (unused/internal)
//   2 = restart            — re-run the decision loop; reset swing active flag
//   3 = queued (log only)  — log the swing queue event, reset flag
//   4 = queued (log only)  — same as 3
//   5 = swing started      — player has committed to a swing; call 73E8 to
//                            start the anim and set swing active flag
//   other = no-op
//
// After handling the result, if it leads to a "reset" path (0, 2, 3, 4),
// clears m_bSwingActive and calls SetSwingActiveState(false).
// ===========================================================================
void pongPlayer::ProcessSwingDecision(int r4, int r5,
                                      bool cancelFirst,
                                      const vec3* pTargetVec,
                                      int r7)
{
    // Flush any stale lerp-queue entries before evaluating (global queue tick).
    pongLerpQueue_3410_g(g_pLerpQueue);

    // Optionally cancel the previous swing before setting up the new one.
    if (cancelFirst)
        m_pPlayerState->CancelSwing();

    // Determine the target direction vector.
    vec3 targetDir = {};    // zero-init
    vec3 swingStrength = {};

    if (pTargetVec) {
        // Explicit target provided (e.g. from network message or serve setup).
        targetDir = *pTargetVec;
    } else {
        // Derive target from input.
        bool inputActive = !io_Input_poll_9D68(g_pInputObj);   // 0 = no input

        if (!inputActive) {
            // Input button is down — read the current input vector.
            pongPlayer_42E0_g(m_pPlayerState->m_pSwingData,
                              &swingStrength, &targetDir, g_kOneMinusAlpha);
        } else {
            // No input — predict target position from physics.
            pongPlayer_AB48_g(&targetDir, m_pPhysicsBound);
        }
    }

    // Feed the target into the targeting update before evaluation.
    game_D468((void*)m_pPlayerState, &targetDir);

    // Evaluate the swing state machine.
    int result = pongPlayer_BF18_g(m_pPlayerState, r4, &swingStrength, &targetDir, r5);

    if (result == 0) {
        // Idle: derive target from input, try again with opponent mirror check.
        bool inputActive2 = !io_Input_poll_9D68(g_pInputObj);

        if (!inputActive2) {
            pongPlayer_42E0_g(m_pPlayerState->m_pSwingData,
                              &swingStrength, &targetDir, g_kOneMinusAlpha);

            // Mark swing triggered on swing data; clear swing active flag.
            pongSwingData* sd = m_pPlayerState->m_pSwingData;
            sd->m_swingStrength  = swingStrength.x;  // float at +344
            sd->m_bTriggered     = 1;                // byte at +342
            // Copy target vec to swing data +352 (16-byte store).
            *(vec3*)((uint8_t*)sd + 352) = targetDir;

            m_bSwingActive = 0;
            SetSwingActiveState(false);
            return;
        } else {
            // Propagate "triggered" flag to the opponent's matching swing data.
            pongPlayer* partner = m_pOpponent;
            if (partner) {
                pongSwingData* partnerSD = partner->m_pPlayerState
                                               ? partner->m_pPlayerState->m_pSwingData
                                               : nullptr;
                if (partnerSD)
                    partnerSD->m_bTriggered = 1;  // byte at +341
            }

            m_bSwingActive = 0;
            SetSwingActiveState(false);
            return;
        }
    }

    if (result == 4 || result == 3) {
        // Queued events — log the swing queue and reset active flag.
        nop_8240E6D0("pongPlayer::ProcessSwingDecision() queued", m_swingInputSlot);
        SetSwingActiveState(true);
        return;
    }

    if (result == 5) {
        // Swing committed — start the animation and mark active.
        pongPlayer_73E8_g(this, g_kZero);
        return;
    }

    if (result == 2) {
        // Restart — reset flag with explicit false, no further action.
        m_bSwingActive = 0;
        SetSwingActiveState(false);
    }
    // result == other: no-op
}


// ===========================================================================
// SECTION 13 — SetupRecoverySlots  @ 0x820C7248 | size: 0x19C
// Called from: pongPlayer_StartServe, ServeStartedMessage_5728
//
// Configures the recovery system with two target "slot" vectors — the
// positions the player should move through during a serve recovery arc.
//
// @param pSlotA  First 16-byte target vector (serve aim or return start)
// @param pSlotB  Second 16-byte target vector (serve aim or return end)
//
// Logic:
//  1. Check if the timing state is running (currentTime < targetTime).
//     If so, check whether m_swingInputSlot (+14 in some lookup table)
//     maps to button state == 2 (held). If it does, cancel swing.
//
//  2. Re-check if timer is running.
//     If NOT running: predict position via pongPlayer_AB48_g, then:
//       - Read swing data result codes (+28, +36, +32) from m_pPlayerState
//       - Verify the match conditions (state == 0 and sub-state == 3,
//         and server flag == 1)
//       - Copy pSlotA (16 bytes) to m_pPlayerState->m_pRecoveryState+80
//       - Copy pSlotB (16 bytes) to m_pPlayerState->m_pRecoveryState+96
//       - Call game_D060 to finalise the serve recovery setup.
// ===========================================================================
void pongPlayer::SetupRecoverySlots(const vec3* pSlotA, const vec3* pSlotB)
{
    pongPlayerState* ps = m_pPlayerState;

    // ── Step 1: cancel swing if timer running and button is "held" ───────
    if (ps->m_pTimingState) {
        bool timerRunning = !(ps->m_pTimingState->m_currentTime
                               < ps->m_pTimingState->m_targetTime);
        if (timerRunning) {
            // Check if the player's input slot (at offset +14 in table) is "held" (== 2).
            uint32_t inputEntry = ((int32_t*)g_pButtonStateTable)[m_inputSlotIdx + 14];
            if (inputEntry == 2)
                ps->CancelSwing();
        }
    }

    // ── Step 2: if timer no longer running, set up serve recovery slots ──
    if (ps->m_pTimingState) {
        bool timerRunning2 = !(ps->m_pTimingState->m_currentTime
                                < ps->m_pTimingState->m_targetTime);
        if (timerRunning2)
            return;  // timer still active after cancel attempt — abort
    }

    // Predict current position into a local buffer.
    vec3 predictedPos = {};
    pongPlayer_AB48_g(&predictedPos, m_pPhysicsBound);

    // Read swing data state codes from recovery state.
    pongRecoveryState* rstate = ps->m_pRecoveryState;
    int32_t stateCode  = *(int32_t*)((uint8_t*)rstate + 28);
    int32_t subState   = *(int32_t*)((uint8_t*)rstate + 36);
    int32_t serverFlag = *(int32_t*)((uint8_t*)rstate + 32);

    // Qualify: stateCode == 0 OR subState == 3, AND serverFlag == 1.
    bool slotOk = (stateCode == 0) ? (subState == 3) : false;
    bool serverOk = (serverFlag == 1);
    if (!slotOk || !serverOk)
        return;

    // Copy 16-byte target vectors into the recovery slot area.
    // Slot A → recovery state + 80 (m_vec0)
    // Slot B → recovery state + 96 (m_vec1)
    // The "128(r10)" base = m_pPlayerState->m_pRecoveryState + 80/96.
    // Confirmed: the lwz r11, 128(r10) + addi r11, r11, 80/96 pattern.
    uint8_t* slotBase = (uint8_t*)ps->m_pRecoveryState;  // TODO: +128 offset may be indirection
    *(vec3*)(slotBase + 80) = *pSlotA;
    *(vec3*)(slotBase + 96) = *pSlotB;

    // Finalise serve recovery state.
    game_D060(ps->m_pRecoveryState);
}


// ===========================================================================
// SECTION 14 — IsActionComplete  @ 0x820C7890 | size: 0x100
// Called from: pongPlayerShaderParams_UpdateParams, pongPlayer_D818_g
//
// Returns true when the player's swing or serve action is fully complete
// and the system is in a stable "done" state.
//
// Fast-path: if m_swingInputResult is 2 or 3 → immediately return true.
// These codes are used during replays and certain match-end states where the
// player is treated as unconditionally "complete".
//
// Slow-path: check IsRecovering on the inner state.  If not recovering,
// return false. If recovering, drill into the recovery state's follow-action
// chain: read the action's velocity record and compare the current recovery
// progress float against a normalised threshold. Return true if recovery
// progress < threshold (i.e. still in the early phase = "action just started"
// = caller treats it as "complete enough").
//
// The double call to IsRecovering mirrors the original assembly exactly:
// the first call gates the fast inner path, the second re-confirms before
// the velocity computation.
// ===========================================================================
bool pongPlayer::IsActionComplete() const
{
    // Fast-path: replay/match-end codes.
    if (m_swingInputResult == 3 || m_swingInputResult == 2)
        return true;

    // Slow-path: recovery state checks.
    pongPlayerState* ps = m_pPlayerState;
    if (!ps->IsRecovering())
        return false;

    // Re-confirm recovery is still active.
    if (!ps->IsRecovering())
        return false;

    // Check if recovery has a valid follow-action and compute normalised progress.
    pongRecoveryState* rstate = ps->m_pRecoveryState;
    if (!rstate)
        return false;

    void* followAction = *(void**)((uint8_t*)rstate + 36);  // m_pFollowAction
    if (!followAction)
        return false;

    // Read the action's velocity record.
    float recoveryProgress = *(float*)((uint8_t*)rstate + 172);  // m_recoveryProgress
    void* velRecord        = *(void**)((uint8_t*)followAction + 36);  // followAction->velRecord
    float velFloat         = *(float*)((uint8_t*)velRecord + 12);      // velRecord->m_float
    float normThreshold    = g_kZero;

    if (velFloat > g_kZero) {
        // Compute threshold: int frameCount / velFloat (normalised frame fraction).
        int32_t frameCount = *(int32_t*)((uint8_t*)followAction + 20);
        normThreshold = (float)frameCount * g_kSwingScaleFactor / velFloat;
    }

    // Return true if recovery progress is still below the normalised threshold.
    return recoveryProgress < normThreshold;
}


// ===========================================================================
// SECTION 15 — SetSwingActiveState  @ 0x820C7C78 | size: 0x6C
// Called from: pongPlayer_StartSwing, pongPlayer_7038_g, pongPlayer_5C00_g,
//              pg_6CF8, pongPlayer_BEE0_g, ref_PostLoadProperties_CCB0
//
// Updates the m_bSwingActive flag at +197 (+0xC5) with transition logging.
//
// Only acts when the new value differs from the current stored value.
// On a state change: logs the transition via nop_8240E6D0 (which is a
// debug trampoline — always a no-op in release builds, but leaves the string
// literals as breadcrumbs in the binary), then stores the new value.
//
// @param active  true  → player is entering the swing window ("swing entered")
//                false → player is leaving the swing window ("swing exited")
// ===========================================================================
void pongPlayer::SetSwingActiveState(bool active)
{
    if (m_bSwingActive == (uint8_t)active)
        return;  // already in the requested state — no-op

    if (active) {
        nop_8240E6D0("pongPlayer::SetSwingActiveState() swing entered",
                     m_swingInputSlot);
    } else {
        nop_8240E6D0("pongPlayer::SetSwingActiveState() swing exited",
                     m_swingInputSlot);
    }

    m_bSwingActive = (uint8_t)active;
}


// ===========================================================================
// SECTION 16 — pongPlayer_5890_g  @ 0x82195890 | size: 0x88
//
// Checks whether a player (resolved via m_pOpponent at +452) has reached
// the apex of their swing AND certain commit flags permit the action.
//
// Despite the pongPlayer prefix this function's `this` is called on the
// outer pongPlayer; it reads `this->m_pOpponent` to get the target player.
//
// Returns true iff ALL hold:
//   1. m_pOpponent->m_pPhysicsBound byte at +64 has bit 0 set ("swing capable")
//   2. m_pOpponent->m_pPlayerState->IsSwingApexReached() is true
//   3. m_pOpponent->m_bSwingCommit1 (+230) == 0  (not already committed)
//   4. m_pOpponent->m_bSwingCommit2 (+231) != 0  (secondary commit flag set)
//
// Used to determine whether the opponent's swing has reached the apex at
// a point where the current player can react to it.
// ===========================================================================
bool pongPlayer::CheckOpponentSwingApex() const
{
    pongPlayer* opp = m_pOpponent;
    if (!opp)
        return false;

    // Bit 0 of the physics bound byte at +64 — "swing capable" flag.
    uint8_t boundFlags = *(uint8_t*)((uint8_t*)opp->m_pPhysicsBound + 64);
    if (!(boundFlags & 0x1))
        return false;

    // Has the opponent's swing reached its apex?
    if (!opp->m_pPlayerState->IsSwingApexReached(0.0f))
        return false;

    // Commit flags: must not be committed yet, but secondary flag must be set.
    if (opp->m_bSwingCommit1)
        return false;
    if (!opp->m_bSwingCommit2)
        return false;

    return true;
}


// ===========================================================================
// SECTION 17 — CheckButtonInput  (vfn_7) @ 0x8218EE10 | size: 0x1BC
// vtable slot 7
//
// Per-frame button-state handler.  Called every frame when the player is
// active. Skipped when the game clock is at frame 10 (pause/replay guard).
//
// Logic:
//  1. Read global clock (lis(-32161) + -21712 → +12). If it equals 10
//     (a special frame count used during replay scrubbing), skip entirely.
//
//  2. Check whether m_inputSlotIdx (+14 in g_pButtonStateTable) is in
//     "button pressed" state (value == 1).  If not pressed, bail.
//
//  3. If pressed, look up this player's 808-byte data record via the
//     global player data table (g_pPlayerDataTable), using the vtable
//     entry from slot 1 of our own vtable as the lookup key.
//
//  4. Call CheckButtonPressed on (record + 796) — checks whether this button press
//     is "new" (first frame down) vs already-held.
//       - If newly pressed: compute a bit mask `1 << m_inputSlotIdx` and
//         check whether the opponent's input slot (15 - m_inputSlotIdx)
//         is also active. Call PostPageGroupMessage to fire the "button down" event
//         with code 2125.
//       - If held (not new): accumulate a float into the record via
//         pongPlayer_0508_g (reads swing data) and fmadds/fadds into the
//         record's score field. This implements "power charging" while
//         the button is held.
//
// New confirmed fields:
//   +0x1D0 (464) = m_inputSlotIdx   — confirmed used as direct integer index
//   +0x1F0 (496) = m_pActionState   — confirmed from vfn_9
// ===========================================================================
void pongPlayer::CheckButtonInput()
{
    // Guard: skip during replay/pause frame.
    int32_t clockState = *(int32_t*)(*(uint32_t*)(g_pClockObj + 0) + 12);
    if (clockState == 10)
        return;

    // Check if this player's button is currently pressed (state == 1 = "down").
    int32_t buttonState = ((int32_t*)g_pButtonStateTable)[m_inputSlotIdx + 14];
    if (buttonState != 1)
        return;

    // Look up per-player 808-byte data record.
    // Key = vtable[1] of this object (the scalar dtor pointer, used as player ID).
    void*    vtableKey  = ((void**)*(void**)this)[1];
    uint32_t slotIdx    = g_pPlayerSlotTable[m_inputSlotIdx + 1];   // (+14 alternate index)
    uint8_t* record     = (uint8_t*)g_pPlayerDataTable + (slotIdx * 808) + 12;

    // Is this the first frame the button is down?
    bool isNewPress = CheckButtonPressed((void*)(record + 796));

    if (isNewPress) {
        // Check if opponent's slot is also active (mirrored slot = 15 - mySlot).
        int32_t mirrorSlot = 15 - m_inputSlotIdx;
        bool    opponentActive;
        if (((int32_t*)g_pButtonStateTable)[mirrorSlot] == 0) {
            // Verify against the saved slot in the player data table.
            opponentActive = (g_pPlayerDataTable_slot == m_inputSlotIdx);
        } else {
            opponentActive = true;
        }

        // Fire the "button down" event: mask = 1 << m_inputSlotIdx, code 2125.
        uint8_t buttonMask = (uint8_t)(1u << (m_inputSlotIdx & 31));
        PostPageGroupMessage(record, /*code*/ 2125, /*mask*/ buttonMask,
                /*unused*/ 0, /*opponentActive*/ (int)opponentActive);
    } else {
        // Button held: accumulate power into the record's score float.
        // Compute delta via pongPlayer_0508_g then blend via fmadds or fadds.
        float delta = 0.0f;
        pongPlayer_0508_g(record, /*maxSteps*/ 30, &delta);

        // Blend constant from global table: g_kPowerBlend @ 0x825C2EA0
        float blend = g_kPowerBlend;
        float threshold = g_kZero;

        float& scoref = *(float*)((uint8_t*)g_pButtonStateTable + 0x8CEC);  // offset 36076

        if (delta < threshold) {
            // Linear blend: scoref = delta * blend + threshold
            scoref = delta * blend + threshold;
        } else {
            // Additive: scoref = delta + threshold
            scoref = delta + threshold;
        }
    }
}


// ===========================================================================
// SECTION 18 — OnButtonReleased  (vfn_9) @ 0x821935B8 | size: 0x6C
// vtable slot 9
//
// Fires when the player's input button is released (goes from pressed to
// unpressed).  Checks that the button is currently unpressed (state == 0),
// then:
//
//   - Calls pongPlayer_5B60_gen to check whether the "passive" swing
//     commit path should absorb the release event. If 5B60 returns true
//     the release is fully handled and we return early.
//
//   - Otherwise calls pongPlayer_1460_g(m_pActionState, 0) which is the
//     main "button released" state-machine driver — dispatches into
//     the appropriate swing/serve cancel or confirm logic depending on
//     the current action state.
//
// The `!= 0` check after the input poll: the `io_Input_poll` pattern
// returns zero when the button is DOWN, so we check for non-zero (= not
// down = released/idle) before proceeding.
// ===========================================================================
void pongPlayer::OnButtonReleased()
{
    // Check if the button is currently unpressed (state == 0 = released/idle).
    int32_t buttonState = ((int32_t*)g_pButtonStateTable)[m_inputSlotIdx + 14];
    bool    notPressed  = (buttonState == 0);

    if (!notPressed)
        return;  // button still held — not a release event

    // Check whether the passive swing commit path absorbs this release.
    if (pongPlayer_5B60_gen(this))
        return;

    // Fire the full button-released action handler.
    pongPlayer_1460_g(m_pActionState, /*buttonReleased*/ 0);
}



// ===========================================================================
// SECTION 6 — DA58: IsSwingApexReached  @ 0x820CDA58 | size: 0x27C
// Called from: pongPlayer_5890_g (CheckOpponentSwingApex)
//
// Returns true when the player's swing has reached a critical apex threshold
// that the opponent can react to. This is a layered multi-system check:
//
//  GATE 0 (fast path): if m_bActive AND m_pCreatureState2->vtable[7]() is true
//    → skip all inner checks, always return false.  When the creature state is
//    "ready" the swing hasn't committed yet; no apex possible.
//
//  GATE 1: if IsCreatureState2Active → return false immediately.
//
//  PATH A — Swing timer running (IsSwingTimerActive):
//    Reads m_pSwingSubState[+160] (cumulative swing progress float).
//    If it is > g_kZero: compute a normalised apex fraction from the timing
//    state's frame count (int16 at +340) divided by the speed float (+48
//    of the velocity record at +48→+12).  Returns true if m_pTimingState's
//    currentTime (+28) exceeds that fraction.
//
//  PATH B — Swing phase blocked (IsSwingPhaseBlocked):
//    Reads m_pAnimState[+412] (m_swingPhase float) and the anim frame array.
//    Computes the same normalised fraction using the frame-count int at
//    m_pAnimState[+404], then compares m_pAnimState[+96] (current anim
//    position) against it.  Returns true if that position > fraction.
//
//  PATH C — Recovering (IsRecovering):
//    Reads m_pRecoveryState[+44] (force-block byte).
//    If NOT force-blocked: checks m_pRecoveryState[+36]→[+172] (progress
//    float) against a second normalised fraction from m_pRecoveryState[+36].
//    Returns true if progress > fraction.
//
//  Fallback: returns false.
//
// Python-verified: cntlzw(1)+rlwinm(27,31,31)=0, cntlzw(0)+rlwinm(27,31,31)=1
// i.e. the idiom is bool NOT — timer_running = (currentTime < targetTime).
// ===========================================================================
bool pongPlayerState::IsSwingApexReached(float threshold) const
{
    // GATE 0: if active AND creature state ready → apex impossible.
    if (m_bActive && m_pCreatureState2 && m_pCreatureState2->IsReady())
        return false;

    // GATE 1: creature state 2 still mid-action → apex not yet.
    if (IsCreatureState2Active())
        return false;

    // ── PATH A: swing timer running ─────────────────────────────────────
    if (IsSwingTimerActive()) {
        if (!m_pSwingSubState)
            goto path_b;

        {
            float swingProgress = *(float*)((uint8_t*)m_pSwingSubState + 160);
            if (swingProgress <= g_kZero)
                goto path_b;

            // Retrieve the velocity record and timing state.
            void*  velRecord = *(void**)((uint8_t*)m_pSwingSubState + 48);
            float  speedFloat = *(float*)((uint8_t*)velRecord + 12);
            int16_t frameCount = *(int16_t*)((uint8_t*)m_pSwingSubState + 340);

            float apexFraction = g_kZero;
            if (speedFloat > g_kZero) {
                apexFraction = (float)(int32_t)frameCount * g_kFrameToSecScale / speedFloat;
            }

            float currentTime = m_pTimingState->m_currentTime;
            return currentTime > apexFraction;
        }
    }

path_b:
    // ── PATH B: swing phase blocked ──────────────────────────────────────
    if (IsSwingPhaseBlocked()) {
        if (!m_pAnimState)
            return false;

        float swingPhase = *(float*)((uint8_t*)m_pAnimState + 412);
        if (swingPhase <= g_kZero)
            return false;  // NaN/zero treated as not-reached

        // Compute normalised fraction from anim frame array entry.
        int32_t  frameIdx    = *(int32_t*)((uint8_t*)m_pAnimState + 404);
        void**   frameArray  = *(void***)((uint8_t*)m_pAnimState + 4);
        void*    frameEntry  = frameArray[frameIdx];
        void*    velRecord   = *(void**)((uint8_t*)frameEntry + 16);
        float    speedFloat  = *(float*)((uint8_t*)velRecord + 12);
        int32_t  frameDuration = *(int32_t*)((uint8_t*)frameEntry + 56);

        float apexFraction = g_kZero;
        if (speedFloat > g_kZero) {
            apexFraction = (float)frameDuration * g_kFrameToSecScale / speedFloat;
        }

        float animPos = *(float*)((uint8_t*)m_pAnimState + 96);
        return animPos < apexFraction;  // blt: true if still before apex
    }

    // ── PATH C: in recovery ───────────────────────────────────────────────
    if (IsRecovering()) {
        if (!m_pRecoveryState)
            return false;

        // If force-blocked, the recovery apex is trivially reachable.
        uint8_t forceBlock = *(uint8_t*)((uint8_t*)m_pRecoveryState + 44);
        if (forceBlock)
            return false;  // TODO: verify — force-block=true may mean inverted

        void*  followRef    = *(void**)((uint8_t*)m_pRecoveryState + 36);
        if (!followRef)
            return false;

        float  progress     = *(float*)((uint8_t*)m_pRecoveryState + 172);
        float  progressThresh = threshold;  // passed in from caller
        return progress > progressThresh;
    }

    return false;
}


// ===========================================================================
// SECTION 7 — DCD8: IsBeforeSwingPeak  @ 0x820CDCD8 | size: 0xE8
// Called from: pongPlayer_5800_h
//
// Returns true when either the swing timing is in its "before peak" window
// OR the player is recovering and a recovery transition check passes.
//
// Two independent paths:
//
//  PATH A — Timing state exists (m_pTimingState != nullptr):
//    Step 1: timer-NOT-running check (currentTime >= targetTime → timerRunning=false)
//            Python: cntlzw(1)+rlwinm(27,31,31)=0, cntlzw(0)+rlwinm(27,31,31)=1
//            so r5 = (currentTime < targetTime) = timerRunning
//    If timerRunning (r5 != 0):
//      Step 2: re-check currentTime < targetTime (redundant guard, as-per-asm)
//      If still running:
//        Check if m_peakTime (+36) > currentTime (+28) → true = "before peak"
//
//  PATH B — Timer not running (r5 == 0):
//    Call IsRecovering. If recovering: call pongPlayer_D298_2hr on m_pRecoveryState
//    and return its result.  If not recovering: return false.
//
// NOTE: m_peakTime at timing[+36] is the swing's peak point in time;
//       the function returns true when currentTime hasn't reached it yet.
// ===========================================================================
bool pongPlayerState::IsBeforeSwingPeak() const
{
    if (!m_pTimingState)
        goto path_b;

    {
        float currentTime = m_pTimingState->m_currentTime;  // +28
        float targetTime  = m_pTimingState->m_targetTime;   // +48

        // timerRunning = (currentTime < targetTime)
        // Python-verified: cntlzw+rlwinm idiom is bool NOT, so:
        // r11=1 when expired, r11=0 when running → NOT gives timerRunning
        bool timerRunning = (currentTime < targetTime);

        if (timerRunning) {
            // Re-check (the asm does two fcmpu/cntlzw passes — keep the guard).
            bool stillRunning = (currentTime < targetTime);
            if (!stillRunning)
                return false;

            // Are we before the peak time?
            float peakTime = *(float*)((uint8_t*)m_pTimingState + 36);  // m_peakTime
            return peakTime > currentTime;
        }
    }

path_b:
    // Timer expired or no timing state: check recovery transition.
    if (!IsRecovering())
        return false;

    // Delegate to the 2-handle recovery check.
    return false;  // TODO: pongPlayer_D298_2hr — verify signature
}


// ===========================================================================
// SECTION 8 — DDC0: IsSwingSystemIdle  @ 0x820CDDC0 | size: 0xD4
// Called from: pongPlayer_vfn_2 (the main Process() virtual)
//
// Returns true only when ALL swing subsystems are simultaneously idle/clear:
//   1. NOT swing-phase-blocked
//   2. Timing state: timer NOT running (currentTime >= targetTime), or absent
//   3. NOT creature-state-ready
//   4. NOT creature-state-2-active
//   5. NOT recovering
//
// Any single active subsystem causes an immediate return of false.
// This is the "may I accept a new serve/swing?" master gate used by Process().
//
// Note the asymmetry with CanAcceptSwingInput: that function has the same 5
// gates but returns true = "ready to swing".  This function returns true =
// "the whole system is at rest" — same semantics, slightly different call
// context.
// ===========================================================================
bool pongPlayerState::IsSwingSystemIdle() const
{
    // Gate 1: swing phase must be clear.
    if (IsSwingPhaseBlocked())
        return false;

    // Gate 2: timing state timer must be expired (or absent).
    if (m_pTimingState) {
        float currentTime = m_pTimingState->m_currentTime;  // +28
        float targetTime  = m_pTimingState->m_targetTime;   // +48
        // timerRunning = (currentTime < targetTime)
        bool timerRunning = (currentTime < targetTime);
        if (timerRunning)
            return false;
    }

    // Gate 3: primary creature state must not be "ready" (= mid-resolution).
    if (IsCreatureStateReady())
        return false;

    // Gate 4: secondary creature state must not be active.
    if (IsCreatureState2Active())
        return false;

    // Gate 5: recovery window must be clear.
    if (IsRecovering())
        return false;

    return true;
}


// ===========================================================================
// SECTION 9 — DE98: IsInReturnPosition  @ 0x820CDE98 | size: 0x168
// Called from: pcrPostPointBlender_vfn_5
//
// Returns true when the player is in the correct "return position" ready to
// receive the ball — used by the post-point blender to decide whether to
// start blending back to the ready stance.
//
// Two execution paths depending on whether the timing state is "running":
//
//  PATH A — Timing state running (currentTime < targetTime):
//    Reads a sub-object: swingSubBase = *(m_pSwingSubState + 32)
//    Checks two flag bits in swingSubBase[+312]:
//      - bit 0x8 (rlwinm ,0,28,28) → "in position flag A"
//      - bit 0x4 (rlwinm ,0,29,29) → "in position flag B"
//    If neither flag: calls pongPlayer_6AA0_g (checks animation condition).
//    If 6AA0 also false: computes a normalised fraction from the timing
//    state's velocity and frame count, compares m_pTimingState[+28] (currentTime)
//    against it. Returns true if currentTime >= fraction (i.e. past the point).
//
//  PATH B — Timer NOT running:
//    Returns true if IsSwingPhaseBlocked OR (NOT IsRecovering).
//    i.e. returns true when the swing anim is past its window OR there is no
//    recovery pending.
//
// Python-verified: bit masks 0x8=rlwinm(0,28,28), 0x4=rlwinm(0,29,29).
// ===========================================================================
bool pongPlayerState::IsInReturnPosition() const
{
    if (!m_pTimingState)
        goto path_b;

    {
        float currentTime = m_pTimingState->m_currentTime;  // timing[+28]
        float targetTime  = m_pTimingState->m_targetTime;   // timing[+48]
        bool  timerRunning = (currentTime < targetTime);

        if (timerRunning) {
            // Compute the sub-object base: *(m_pSwingSubState + 32)
            if (!m_pSwingSubState)
                return false;

            uint8_t* swingSubBase = *(uint8_t**)((uint8_t*)m_pSwingSubState + 32);
            if (!swingSubBase)
                return false;

            // Check position flag bits in byte at +312.
            // Python-verified: mask 0x8 = PPC bit 28, mask 0x4 = PPC bit 29.
            uint8_t flags = *(uint8_t*)(swingSubBase + 312);
            if (flags & 0x8)
                return true;  // flag A: already in position
            if (flags & 0x4)
                return true;  // flag B: in return position

            // Ask the animation system if we've cleared the swing gate.
            if (false /* TODO: pongPlayer_6AA0_g(swingSubBase) — verify signature */)
                return true;

            // Final: compare normalised timing fraction vs currentTime.
            // velocity record is at m_pSwingSubState[+16]
            void*   playerIndexObj = *(void**)((uint8_t*)swingSubBase + 16);
            float   speedFloat     = *(float*)((uint8_t*)playerIndexObj + 12);
            int16_t frameCount     = *(int16_t*)((uint8_t*)swingSubBase + 308);

            float fraction = g_kZero;
            if (speedFloat > g_kZero) {
                fraction = (float)(int32_t)frameCount * g_kFrameToSecScale / speedFloat;
            }

            // Return true if currentTime has reached the normalised fraction.
            return currentTime >= fraction;
        }
    }

path_b:
    // Timer not running — check anim and recovery state.
    if (IsSwingPhaseBlocked())
        return true;   // Anim past window → trivially in return position.

    if (IsRecovering())
        return false;  // Still in recovery → not yet in return position.

    return true;       // Default: no active system → in position.
}

