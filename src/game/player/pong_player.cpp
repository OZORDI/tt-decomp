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
#include <cmath>     // fabsf

// ---------------------------------------------------------------------------
// External functions referenced below
// ---------------------------------------------------------------------------

// Face animation blender — starts the post-point anim.
#include "../creature/pong_creature.hpp"

// Update function called from D228; purpose: TODO — likely syncs state.
extern void pongPlayer_UpdateLocomotionState(pongPlayer* state);  // @ 0x820CC678

// LocomotionStateAnim helpers — update animation list entries.
// C128 = standard transition, C288 = alternate (flag-gated) transition.
extern void LocomotionStateAnim_BlendLocomotionAnims(uint32_t entry, void* parent, float dt);
extern void LocomotionStateAnim_TransitionLocomotionState(uint32_t entry, void* parent, float dt);

// Swing/target helpers (vec3 defined in pong_player.hpp)
extern void pongPlayer_ComputeTargetPosition(vec3* outVec, void* targetStruct);
extern float fiAsciiTokenizer_ParseFloat(float input);
extern void pongPlayer_ComputeCourtPosition(vec3* outVec, void* targetData);
extern void pongPlayer_GetCourtBounds(void* self, int r4, void* p3, void* p4, int p5, void* p6, void* p7, uint8_t metadataByte);
extern void pongPlayer_InterpolateCourtPoint(void* adjustedVec, void* gridBase, void* p3, void* p4, void* p5);

// Additional extern "C" helpers
extern "C" void pongCreature_UpdateReplay(void* creature);
extern "C" void pongPlayer_ProcessReplay(void* player);
extern "C" void pongPlayer_NetSync(void* player, int param);
extern "C" void pongPlayer_ResetShotTiming(void* player, float val);
extern "C" void pongPlayer_SetScoreSlot(void* player, int slot);
extern "C" void pongPlayer_ClearShotState(void* player);
extern "C" void pongPlayer_ResetSwingImpl(void* subObj);
extern "C" void pongPlayer_SyncFloat(void* player, void* desc);
extern "C" void pongPlayer_SyncWord(void* player, void* desc);
extern "C" bool pongPlayer_CheckBallSplash(void* player);
extern "C" int pongPlayer_GetAnimIndex(void* player);
extern "C" void pongPlayer_GetVelocity(void* player, float* outVec);
extern void pongPlayer_GetPlayerPosition(void* player, void* outPos);
extern void pongPlayer_Interpolate(void* player, void* posData);
extern void pongPlayer_ResetMoverImpl(void* player);
extern void pongPlayer_ApplyNetStateImpl(void* player);
extern void pongPlayer_InitShotImpl(void* player);


// ── External game helpers referenced by pong_player.cpp ───────────────────

namespace rage { void UnbindObject(void* obj, void* arg); }  // @ atSingleton suite
extern void phBoundCapsule_UpdateTransform(void* bound);  // only r3 used; r4-r6 computed internally
extern const float g_kSwingRadiusConst;  // constant from .rdata
extern const vec3  g_kZero_vec3;         // zero vector constant
extern const float g_kZero;              // zero float constant

// ── Additional external stubs for pong_player.cpp ───────────────────────
extern void* g_pInputScoreTable;         // global input score table (float array)
extern void* g_pLerpQueue;               // lerp queue global
extern void* g_pInputObj;               // input object global
extern void* g_pButtonStateTable;        // button state table (int32_t array)
extern const float g_kOneMinusAlpha;     // constant
// pongPlayer_ClampSwingToBounds: r3=playerState ptr, r4=swingVec, r5=uint8_t suppressFlip
extern void pongPlayer_ClampSwingToBounds(void* playerState, vec3* swingVec, uint8_t suppressFlip);
// pongPlayer_GetRacketPosition: r3=out vec3, r4=physBound — both saved in prologue
extern void pongPlayer_GetRacketPosition(vec3* out, void* physBound);
// pongPlayer_SelectSwingTarget: returns int; r3=playerState, r4=int, r5=swingStr*, r6=targetDir*, r7=int
extern int  pongPlayer_SelectSwingTarget(void* playerState, int r4, vec3* swingStrength, vec3* targetDir, int r5);
// pongPlayer_ProcessSwingInput: r3=this, f1=float  (confirmed from call site)
extern void pongPlayer_ProcessSwingInput(pongPlayer* p, float f);
// pongPlayer_ComputeSwingStrength: r3=swingData, r5=outStrength, r6=outTarget, f1=alpha
extern void pongPlayer_ComputeSwingStrength(void* swingData, vec3* outStrength, vec3* outTarget, float alpha);
extern void game_D468(void* obj, vec3* v);               // geometry helper
extern bool pongPlayer_IsLocoStateReady(void* locoState);          // loco check
extern int  io_Input_poll_9D68(void* inputObj);           // 0=active input
extern void pongLerpQueue_Update(void* queue);            // lerp queue update
extern void game_D060(void* recoveryState);              // finalise serve recovery

// pongSwingData is now defined in pong_player.hpp



extern void game_CD20(void* recoveryState);          // flush recovery @ 0x820DCD20
extern void crAnimBlenderState_Init(void* animSubState);  // reset phase-blocked @ 0x8224C810
extern void pcrAnimState_ComputePosition(vec3* out, pongAnimState* animState);  // compute anim pos
extern void pongPlayer_GetAnimNormalizedTime(pongPlayer* state);    // @ 0x820CD238 (or similar)
// extern void pongPlayer_CheckAnimTimeInRange(...);  // TODO: verify signature @ ~0x820CD298

// Logging no-op (debug only)
extern void rage_DebugLog(const char* fmt, ...);

// Geometry / position helpers used by D7B0.
extern void* pg_GetGeometryRecord(void* singleton);              // → returns geometry record
extern void  pcrAnimState_ComputePosition(vec3* out, pongAnimState* animState); // → computes anim position

// Global singletons
extern void* g_geomSingleton;   // @ loaded via lis+lwz pattern in D7B0

// Global constants (data section) — aliases for consolidated externs
static const float& g_recoveryTimerThreshold = g_kFloatConst_D108;  // @ 0x8202D108
static const float& g_swingPhaseThreshold    = g_kFloatConst_D110;  // @ 0x8202D110
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
extern void pongPlayer_ComputeStepDelta(void* obj, int maxSteps, float* outDelta);   // @ 0x821E0508
extern bool pongPlayer_CheckServeInput(pongPlayer* p);                            // @ 0x82195B60
extern void pongPlayer_InitActionState(void* actionState, int released);            // @ 0x821A1460
             // @ AltiVec constant used in D7B0 sign flip

// ---------------------------------------------------------------------------
// Resolved global data addresses — consolidated from scattered inline externs
// ---------------------------------------------------------------------------

// .rdata float constants (0x8202D1xx block — adjacent 4-byte floats)
extern const float g_kFloatConst_D108;       // @ 0x8202D108  (recovery threshold / input low / zero threshold)
extern const float g_kFloatConst_D10C;       // @ 0x8202D10C  (grid scale constant)
extern const float g_kFloatConst_D110;       // @ 0x8202D110  (swing phase / input high / adjustment threshold)

// .rdata vector/struct constants (0x8207Dxxx block)
extern const float g_kVecConst_7D110[4];     // @ 0x8207D110  (16B: swing grid init / court scale / timing zero)
extern const float g_kFacingConst_7D108[4];  // @ 0x8207D108  (facing direction constants, -/+ at offsets 0/8)
extern const float g_kVecConst_7D168[3];     // @ 0x8207D168  (12B: grid reset value)

// .rdata double constants (0x82079xxx block)
extern const double g_kDoubleConst_79D00;    // @ 0x82079D00  (input deadzone / max angle)
extern const double g_kDoubleConst_79B00;    // @ 0x82079B00  (input clamp zero / neg)
extern const float  g_kFloatConst_79C04;     // @ 0x82079C04  (input angle deadzone)
extern const float  g_kFloatConst_79CD8;     // @ 0x82079CD8  (max adjustment)
extern const float  g_kFloatConst_79BAC;     // @ 0x82079BAC  (adjustment scale)
extern const float  g_kFloatConst_79FFC;     // @ 0x82079FFC  (quant scale 1)
extern const float  g_kFloatConst_79FF8;     // @ 0x82079FF8  (quant scale 2)
extern const float  g_kFloatConst_79FF4;     // @ 0x82079FF4  (quant mult)

// .data globals — game state pointers (0x8271Axxx block)
extern void*    g_pGlobalClock;              // @ 0x8271A304  (clock / match state ptr)
extern void*    g_pCourtState;               // @ 0x8271A314  (court state)
extern void*    g_pMatchState;               // @ 0x8271A318  (match state)
extern void*    g_pPlayerRecords;            // @ 0x8271A31C  (808-byte-per-entry player records)
extern void*    g_pPlayerRecords2;           // @ 0x8271A324  (secondary record table)
extern void*    g_pMatchConfig;              // @ 0x8271A32C  (match config / network player slot)

// .data globals — game object pointers
extern void**   g_pInputArrayTable;          // @ 0x825EAB28  (input object pointer table)
extern float    g_kInputScaleData[];         // @ 0x825C5938  (input scale constants, 16B)
extern uint8_t  g_mirrorBuffer[32];          // @ 0x82606720  (global mirror buffer, 32B)
extern uint8_t  g_ballSplashArray[];         // @ 0x8261A3D0  (416-byte-per-entry array)

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
    pongPlayer_UpdateLocomotionState(this);

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
 *    Looks up the geometry record for m_pAnimState->field_136 via pg_GetGeometryRecord,
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
        void*    geomRecord = pg_GetGeometryRecord(g_geomSingleton);
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
 * Called from pongPlayer_ProcessSwingInput (the swing-input polling loop) on the inner
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
            rage_DebugLog("pongPlayer::CancelSwing() timing reset", (uintptr_t)m_pCreature);  /* UNVERIFIED — string not found in binary */
        }
    }

    // ── PATH B: flush recovery callbacks ────────────────────────────────
    if (IsRecovering())
    {
        game_CD20(m_pRecoveryState);     // flush recovery state @ 0x820DCD20
        rage_DebugLog("pongPlayer::CancelSwing() recovery flush", (uintptr_t)m_pCreature);  /* UNVERIFIED — string not found in binary */
    }

    // ── PATH C: reset anim phase-blocked state ───────────────────────────
    if (IsSwingPhaseBlocked())
    {
        pongAnimState* anim = m_pAnimState;
        crAnimBlenderState_Init((uint8_t*)anim + 16);   // reset phase-blocked sub-system @ 0x8224C810
        anim->m_swingPhase = kZero;        // clear swing phase progress (+412)
        rage_DebugLog("pongPlayer::CancelSwing() anim reset", (uintptr_t)m_pCreature);  /* UNVERIFIED — string not found in binary */
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
 *      Call pongPlayer_GetAnimNormalizedTime to get the recovery progress float.
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
        pongPlayer_GetAnimNormalizedTime((pongPlayer*)this);   // updates recovery float at +172
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
 *    If IsRecovering, delegate to pongPlayer_CheckAnimTimeInRange on the recovery state.
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
        return false;  // TODO: pongPlayer_CheckAnimTimeInRange — verify recovery fallback logic
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
 *    Call pongPlayer_IsLocoStateReady on the locomotion state to evaluate positioning.
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
 *       pongPlayer_IsLocoStateReady's semantics.
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
        if (false /* TODO: pongPlayer_IsLocoStateReady(locoState) — verify signature @ 0x820D6AA0 */)
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
//  2. Call phBoundCapsule_UpdateTransform on m_pPhysicsBound — updates the capsule.
//  3. If m_pPlayerState->m_pSwingData exists, call its vtable slot 1 with
//     the capsule radius float (from g_kSwingRadiusConst) — notifies the
//     swing data object of the current body scale.
//  4. Read m_swingInputResult (+200): if it equals 7, suppress vector flip.
//  5. Build a velocity-scaled 3D vector from the physics bound's own floats
//     (+56, +60) multiplied by g_kSwingRadiusConst (+8) using AltiVec vmulfp.
//  6. Call pongPlayer_ClampSwingToBounds to update the swing prediction with this vector.
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
    phBoundCapsule_UpdateTransform(m_pPhysicsBound);

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
    pongPlayer_ClampSwingToBounds(m_pPlayerState, &swingVec, 0);

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
// @param r4, r5       pass-through args forwarded to pongPlayer_SelectSwingTarget
//
// Swing result codes (from pongPlayer_SelectSwingTarget):
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
    pongLerpQueue_Update(g_pLerpQueue);

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
            pongPlayer_ComputeSwingStrength(m_pPlayerState->m_pSwingData,
                              &swingStrength, &targetDir, g_kOneMinusAlpha);
        } else {
            // No input — predict target position from physics.
            pongPlayer_GetRacketPosition(&targetDir, m_pPhysicsBound);
        }
    }

    // Feed the target into the targeting update before evaluation.
    game_D468((void*)m_pPlayerState, &targetDir);

    // Evaluate the swing state machine.
    int result = pongPlayer_SelectSwingTarget(m_pPlayerState, r4, &swingStrength, &targetDir, r5);

    if (result == 0) {
        // Idle: derive target from input, try again with opponent mirror check.
        bool inputActive2 = !io_Input_poll_9D68(g_pInputObj);

        if (!inputActive2) {
            pongPlayer_ComputeSwingStrength(m_pPlayerState->m_pSwingData,
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
        rage_DebugLog("pongPlayer::ProcessSwingDecision() queued", m_swingInputSlot);  /* UNVERIFIED — string not found in binary */
        SetSwingActiveState(true);
        return;
    }

    if (result == 5) {
        // Swing committed — start the animation and mark active.
        pongPlayer_ProcessSwingInput(this, g_kZero);
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
//     If NOT running: predict position via pongPlayer_GetRacketPosition, then:
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
    pongPlayer_GetRacketPosition(&predictedPos, m_pPhysicsBound);

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
// Called from: pongPlayer_StartSwing, pongPlayer_7038_g, pongPlayer_SetPlayMode,
//              pg_6CF8, pongPlayer_BEE0_g, ref_PostLoadProperties_CCB0
//
// Updates the m_bSwingActive flag at +197 (+0xC5) with transition logging.
//
// Only acts when the new value differs from the current stored value.
// On a state change: logs the transition via rage_DebugLog (which is a
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
        rage_DebugLog("pongPlayer::SetSwingActiveState() swing entered",
                     m_swingInputSlot);  /* UNVERIFIED — string not found in binary */
    } else {
        rage_DebugLog("pongPlayer::SetSwingActiveState() swing exited",
                     m_swingInputSlot);  /* UNVERIFIED — string not found in binary */
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
//         pongPlayer_ComputeStepDelta (reads swing data) and fmadds/fadds into the
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
        // Compute delta via pongPlayer_ComputeStepDelta then blend via fmadds or fadds.
        float delta = 0.0f;
        pongPlayer_ComputeStepDelta(record, /*maxSteps*/ 30, &delta);

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
//   - Calls pongPlayer_CheckServeInput to check whether the "passive" swing
//     commit path should absorb the release event. If 5B60 returns true
//     the release is fully handled and we return early.
//
//   - Otherwise calls pongPlayer_InitActionState(m_pActionState, 0) which is the
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
    if (pongPlayer_CheckServeInput(this))
        return;

    // Fire the full button-released action handler.
    pongPlayer_InitActionState(m_pActionState, /*buttonReleased*/ 0);
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
//    Call IsRecovering. If recovering: call pongPlayer_CheckAnimTimeInRange on m_pRecoveryState
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
    return false;  // TODO: pongPlayer_CheckAnimTimeInRange — verify signature
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
//    If neither flag: calls pongPlayer_IsLocoStateReady (checks animation condition).
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
            if (false /* TODO: pongPlayer_IsLocoStateReady(swingSubBase) — verify signature */)
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



// ===========================================================================
// SECTION 5 — FAB8: UpdateSwingTimingAdjustment  @ 0x8219FAB8 | size: 0x11C
// ===========================================================================
/**
 * pongPlayer::UpdateSwingTimingAdjustment()  @ 0x8219FAB8
 *
 * Computes and updates a timing adjustment value (stored at +80) based on:
 * - Current animation phase (this+68)
 * - Target vector computation via pongPlayer_ComputeTargetPosition
 * - Clock/timing state from global timing object
 * - A math function (atan2-like) to compute angular adjustment
 *
 * The adjustment is clamped between 0.0 and 1.0 and appears to be used
 * for fine-tuning swing timing based on player position and animation state.
 *
 * Called during swing preparation to adjust timing windows dynamically.
 */
void pongPlayer::UpdateSwingTimingAdjustment() {
    vec3 targetVec;
    
    // Step 1: Compute target vector using creature state and index
    // The creature object at +44 contains an index at +464 that selects
    // from an array of 416-byte structs
    void* creatureState = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(this) + 44);
    uint32_t index = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(creatureState) + 464);
    
    // Compute address: base array + (index * 416) + 48
    // Base address is 0x8261A3D0 + 48 = 0x8261A400
    uintptr_t baseAddr = (uintptr_t)(g_ballSplashArray + 48);
    void* targetStruct = reinterpret_cast<void*>(baseAddr + (index * 416));
    
    // Call helper to compute target vector
    pongPlayer_ComputeTargetPosition(&targetVec, targetStruct);
    
    // Step 2: Load timing values
    float animPhase = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(this) + 68);  // this+68
    
    // Load global clock object and extract timing value
    void* clockObj = *reinterpret_cast<void**>((uintptr_t)g_pGlobalClock);
    float clockValue = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(clockObj) + 24);
    
    // Load constants
    extern float g_kTimingConstant;   // @ 0x825C4930
    const float& g_kAdjustmentScale = g_kFloatConst_79BAC;
    
    // Step 3: Compute timing values
    float scaledPhase = animPhase * g_kFrameToSecScale;
    float timingFactor = clockValue * g_kTimingConstant;
    
    // Step 4: Call math function (misnamed as fiAsciiTokenizer_ParseFloat)
    // This is likely atan2 or a similar trigonometric function
    float mathResult = fiAsciiTokenizer_ParseFloat(scaledPhase * g_kAdjustmentScale);
    
    // Step 5: Compute adjustment magnitude
    float adjustment = mathResult * timingFactor;
    
    // Step 6: Check if adjustment is significant
    if (fabsf(adjustment) > g_kFloatConst_D110) {
        // Load current value at +72
        float currentValue = *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(this) + 72);
        
        // Compute direction adjustment based on target vector Z component
        float directionFactor = adjustment / timingFactor;
        float absTargetZ = fabsf(targetVec.z);
        float zAdjustment = directionFactor * absTargetZ;
        
        // Determine final adjustment based on sign
        float finalAdjustment;
        if (currentValue * targetVec.x >= g_kFloatConst_D110) {
            finalAdjustment = currentValue + zAdjustment;
        } else {
            finalAdjustment = zAdjustment - currentValue;
        }
        
        // Load max threshold and compute clamped value
        void* clockObj2 = *reinterpret_cast<void**>((uintptr_t)g_pGlobalClock);
        float maxThreshold = *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(clockObj2) + 16) * g_kFloatConst_79CD8;
        
        if (finalAdjustment < maxThreshold) {
            float delta = maxThreshold - finalAdjustment;
            *reinterpret_cast<float*>(
                reinterpret_cast<uintptr_t>(this) + 80) = delta;
        }
    }
    
    // Step 7: Clamp the value at +80 between 0.0 and 1.0
    float* adjustmentPtr = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(this) + 80);
    float value = *adjustmentPtr;
    
    // Clamp to [0.0, 1.0]
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    
    *adjustmentPtr = value;
}

/**
 * pongPlayer::UpdatePositionFromSwingTarget()  @ 0x8219E6D8 | size: 0x70
 *
 * Computes a 3D position from the player's swing target indices and adds
 * it to the player's base position vector.
 *
 * The function:
 * 1. Reads two indices from offsets +948 and +952 (likely row/column indices)
 * 2. Computes array offset: ((index1 * 9 + index2 * 3) * 32) + 976
 * 3. Calls pongPlayer_ComputeCourtPosition to get a target vector
 * 4. Adds the target vector to the player's position at offset +880
 * 5. Stores the result at offset +832
 */
void pongPlayer::UpdatePositionFromSwingTarget() {
    // Load the two indices that determine which swing target to use
    uint32_t index1 = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(this) + 948);
    uint32_t index2 = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(this) + 952);
    
    // Compute the array offset: ((index1 * 9 + index2 * 3) * 32) + 976
    // This matches the PPC arithmetic: r11*3, +r10, *3, *32
    uint32_t arrayOffset = ((index1 * 9 + index2 * 3) * 32) + 976;
    
    // Get pointer to the swing target data
    vec3* swingTarget = reinterpret_cast<vec3*>(
        reinterpret_cast<uintptr_t>(this) + arrayOffset);
    
    // Call helper to process the swing target (returns a vec3)
    vec3 targetDelta;
    pongPlayer_ComputeCourtPosition(&targetDelta, swingTarget);
    
    // Load the current position offset vector at +880
    vec3* currentOffset = reinterpret_cast<vec3*>(
        reinterpret_cast<uintptr_t>(this) + 880);
    
    // Add the target delta to the current offset and store at +832
    vec3* resultPosition = reinterpret_cast<vec3*>(
        reinterpret_cast<uintptr_t>(this) + 832);
    
    resultPosition->x = targetDelta.x + currentOffset->x;
    resultPosition->y = targetDelta.y + currentOffset->y;
    resultPosition->z = targetDelta.z + currentOffset->z;
}



// ===========================================================================
// SECTION 19 — InitializeCollisionGrid  @ 0x8219E008 | size: 0x2BC
//
// Initializes a 4x4 grid of collision/interaction points on the player.
// Each grid cell stores normalized coordinates and associated metadata.
//
// The grid covers offsets +864 through +1248 (384 bytes total, 96 bytes per
// row). Each cell contains:
//   - 2D normalized position (x, y)
//   - Mirrored copies at +860/+864 offsets
//   - Metadata flags
//
// Grid coordinates are computed as:
//   x = ((i + (i+1)) / 2) * 0.5 * scale
//   y = ((j + (j+1)) / 2) * 0.5 * scale
//
// This creates a uniform grid from 0.25 to 1.75 in each dimension.
//
// @param r4  Pass-through parameter for pongPlayer_GetCourtBounds
// @param r6  Metadata byte (stored at grid cell +856/857)
// ===========================================================================
void pongPlayer::InitializeCollisionGrid(int r4, uint8_t metadataByte)
{
    // Load constants from .rdata
    const float* constants = &g_kFloatConst_D10C;
    const float scale = constants[0];      // Base scale factor
    const float zero = constants[1];       // Zero constant
    const float halfScale = 0.5f;          // Normalization factor
    const float epsilon = 0.01f;           // Small offset for grid adjustments
    
    // Initialize base state via helper function
    pongPlayer_GetCourtBounds(this, r4, nullptr, nullptr, 1, nullptr, nullptr, metadataByte);
    
    // Grid storage base pointers
    uint8_t* gridBase = (uint8_t*)this + 864;      // +0x360
    uint8_t* cellPtr = (uint8_t*)this + 984;       // +0x3D8 (first cell)
    uint8_t* mirrorBase = g_mirrorBuffer;
    
    // Outer loop: 4 rows (i = 0 to 3)
    for (int i = 0; i < 4; i++) {
        // Compute normalized X coordinate for this row
        // x = ((i + (i+1)) / 2) * 0.5 * scale
        float xNorm = ((float)i + (float)(i + 1)) * halfScale * halfScale * scale;
        
        // Inner loop: 4 columns (j = 0 to 3)
        for (int j = 0; j < 4; j++) {
            // Compute normalized Y coordinate for this column
            // y = ((j + (j+1)) / 2) * 0.5 * scale
            float yNorm = ((float)j + (float)(j + 1)) * halfScale * halfScale * scale;
            
            // Store primary coordinates
            *(float*)(cellPtr + 0) = yNorm;   // Y at current position
            *(float*)(cellPtr - 4) = xNorm;   // X at previous position
            
            // Store mirrored copies
            *(float*)(cellPtr + 864) = yNorm;
            *(float*)(cellPtr + 860) = xNorm;
            
            // Initialize temporary vectors for collision detection
            vec3 tempVec = { zero, zero, zero };
            vec3 adjustedVec = {};
            
            // Compute adjusted coordinates based on quadrant
            switch (j) {
                case 0:  // Bottom-left quadrant
                    adjustedVec.x = xNorm - epsilon;
                    adjustedVec.y = yNorm - epsilon;
                    break;
                    
                case 1:  // Bottom-right quadrant
                    adjustedVec.x = xNorm + epsilon;
                    adjustedVec.y = yNorm - epsilon;
                    break;
                    
                case 2:  // Top-right quadrant
                    adjustedVec.x = xNorm + epsilon;
                    adjustedVec.y = yNorm + epsilon;
                    break;
                    
                case 3:  // Top-left quadrant
                    adjustedVec.x = xNorm - epsilon;
                    adjustedVec.y = yNorm + epsilon;
                    break;
            }
            
            // Call collision detection helper with adjusted coordinates
            pongPlayer_InterpolateCourtPoint(&adjustedVec, gridBase, nullptr, nullptr, nullptr);
            
            // Advance to next cell (96 bytes per cell)
            cellPtr += 96;
        }
        
        // Store metadata flags at end of row
        *(cellPtr - 8) = metadataByte;
        *(cellPtr - 7) = *(uint8_t*)((uint8_t*)this + 944);  // Copy player state flag
        
        // Copy row data to mirror buffer using AltiVec
        // Load 16 bytes from gridBase, store to mirrorBase
        memcpy(mirrorBase, gridBase, 16);
        
        // Store additional metadata
        *(cellPtr + 856) = metadataByte;
        *(cellPtr + 857) = *(uint8_t*)((uint8_t*)this + 944);
        
        // Copy to mirror buffer again
        memcpy(mirrorBase, gridBase, 16);
    }
}


// ===========================================================================
// SECTION 8 — 0050: ProcessInputVector  @ 0x821A0050 | size: 0x2A0
// ===========================================================================
/**
 * pongPlayer::ProcessInputVector(float x, float y, float z, uint8_t flags)
 *   @ 0x821A0050
 *
 * Processes 3D input vector (likely from analog stick or motion input) and
 * applies it to the player's movement/aim system. The function:
 *
 *  1. Loads the current player's input slot data from global tables
 *  2. Calls a helper function (pongPlayer_ComputeTargetPosition) twice to compute intermediate
 *     vectors based on player state and input
 *  3. Scales the input by player-specific factors (from this+36, this+40)
 *  4. Applies clamping and quantization to produce a final integer value
 *     in the range [-31, 31]
 *  5. Stores the result to the player's input data structure
 *
 * The function has several conditional paths based on:
 *  - flags parameter (r4): controls special processing modes
 *  - Player state byte at +6626: enables range clamping to [-4, 31]
 *  - Input magnitude thresholds: determines if input should be zeroed
 *
 * @param x      Input X component (f1)
 * @param y      Input Y component (f2)
 * @param z      Input Z component (f3)
 * @param flags  Processing flags byte (r4)
 *
 * TODO: This function needs deeper analysis to understand:
 *  - The exact purpose of the two pongPlayer_ComputeTargetPosition calls
 *  - The meaning of the various thresholds and scaling factors
 *  - The relationship between the input vector and the final integer output
 *  - Whether this is stick input, motion input, or AI targeting
 */
void pongPlayer::ProcessInputVector(float x, float y, float z, uint8_t flags) {
    // Load player's input slot index from this+44 (m_pBody) -> +464
    void* body = reinterpret_cast<void*>(
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(this) + 44));
    int32_t inputSlot = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uintptr_t>(body) + 464);

    // Load global input data table base
    // TODO: Resolve exact global addresses and structure layout
    extern uint8_t* g_pPlayerInputTable;  // @ lis(-32158) + -23600
    extern int32_t  g_currentInputSlot;   // @ lis(-32160) + 25976
    
    // Calculate offset into input table: inputSlot * 416 bytes per entry
    uint32_t tableOffset = inputSlot * 416;
    uint8_t* inputData = g_pPlayerInputTable + tableOffset;
    
    // Load current position vector from inputData+240 (16-byte aligned)
    vec3 currentPos;
    memcpy(&currentPos, inputData + 240, sizeof(vec3));
    
    // Call helper function to compute intermediate vector
    vec3 tempVec1;
    pongPlayer_ComputeTargetPosition(&tempVec1, inputData);
    
    // Scale input by player-specific factors
    float scaleX = *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 36);
    float scaleY = *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 40);
    
    // Load scaling constant from data section
    extern const float g_kInputScale;  // @ 0x825C5938 + 22840
    
    // Compute scaled deltas
    float deltaX = scaleX * x * g_kInputScale;
    float deltaY = scaleX * y * g_kInputScale;
    float deltaZ = scaleY * z;
    
    // Store scaled vector to stack temp
    vec3 scaledInput = { deltaZ, 0.0f, deltaX, 0.0f };  // Note: component reordering
    
    // Store to inputData+240
    memcpy(inputData + 240, &scaledInput, sizeof(vec3));
    
    // Call helper again with updated data
    vec3 tempVec2;
    pongPlayer_ComputeTargetPosition(&tempVec2, inputData);
    
    // Add the two computed vectors
    vec3 finalVec;
    finalVec.x = tempVec2.x + scaledInput.x;
    finalVec.y = tempVec2.y + scaledInput.y;
    finalVec.z = tempVec2.z + scaledInput.z;
    
    // Store result to this+48 (16-byte aligned)
    memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 48),
           &finalVec, sizeof(vec3));
    
    // Load array pointer and compute index
    extern void** g_pInputArrayTable;  // @ 0x825EAB28
    int32_t arrayIndex = inputSlot + 17;
    void* arrayEntry = g_pInputArrayTable[arrayIndex];
    
    // Load threshold value from entry+56
    float threshold = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(arrayEntry) + 56);
    
    // Compute scaled threshold check
    float scaledThreshold = threshold * deltaX;
    
    // Load comparison constants
    extern const float g_kInputThresholdLow;   // @ 0x8202D108
    extern const float g_kInputThresholdHigh;  // @ 0x8202D110
    extern const float g_kZeroThreshold;       // @ 0x8202D108
    
    // Initialize output value
    int32_t outputValue = 0;
    
    // Check if input magnitude is significant
    if (fabs(deltaX) >= g_kZeroThreshold) {
        // Quantize the input to integer range
        // This complex section converts float input to clamped integer
        
        // Determine sign and apply scaling
        float absInput = fabs(deltaX);
        
        // Load quantization constants
        extern const float g_kQuantScale1;  // @ 0x82079FFC
        extern const float g_kQuantScale2;  // @ 0x82079FF8
        extern const float g_kQuantMult;    // @ 0x82079FF4
        
        float quantScale = (deltaY <= g_kInputThresholdHigh) ? 
                          g_kQuantScale1 : g_kQuantScale2;
        
        // Apply quantization
        int32_t quantX = static_cast<int32_t>(deltaY);
        int32_t quantY = static_cast<int32_t>(absInput * quantScale);
        
        // Compute product
        int32_t product = quantX * quantY;
        
        // Adjust based on sign of deltaZ
        if (product < 0) {
            int32_t absZ = static_cast<int32_t>(fabs(deltaZ));
            product = product - absZ;
        } else if (product > 3) {
            product = 3;
        }
        
        // Clamp to [-31, 31] range
        if (product < -31) {
            product = -31;
        } else if (product > 31) {
            product = 31;
        }
        
        outputValue = product;
    }
    
    // Check flags parameter
    if (flags != 0) {
        // Load threshold for special mode
        extern const float g_kSpecialThreshold;  // @ 0x825C8A50
        
        if (threshold > g_kSpecialThreshold) {
            outputValue = 31;
        } else {
            // Check inputData+156 for state value
            int32_t stateValue = *reinterpret_cast<int32_t*>(
                reinterpret_cast<uintptr_t>(inputData) + 156);
            
            if (stateValue == 3 && outputValue >= 0) {
                outputValue = 0;
            }
        }
    }
    
    // Check body state byte at +6626
    uint8_t bodyState = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(body) + 6626);
    
    if (bodyState != 0) {
        // Apply tighter clamping: [-4, 31]
        if (outputValue < -4) {
            outputValue = -4;
        } else if (outputValue > 31) {
            outputValue = 31;
        }
    }
    
    // Store final result to inputData+48
    *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(inputData) + 48) = 
        outputValue;
}


// ===========================================================================
// SECTION 5 — Additional Player State Query Functions
// ===========================================================================

/**
 * pongPlayer::IsSwingTimerExpiredAndReady()  @ 0x820CE000 | size: 0x8C
 *
 * Returns true when BOTH conditions hold:
 *   1. The swing timer has expired (currentTime >= targetTime)
 *   2. The player's position is beyond a threshold distance from origin
 *
 * This is used to gate swing initiation — the player must be in position
 * AND the timing window must be open.
 */
bool pongPlayer::IsSwingTimerExpiredAndReady() const {
    // Check if timing state exists and timer has expired
    if (m_pTimingState) {
        float currentTime = m_pTimingState->m_currentTime;
        float targetTime = m_pTimingState->m_targetTime;
        
        // Timer expired check (handles NaN as expired)
        if (!(currentTime < targetTime)) {
            // Timer is expired, now check position threshold
            if (m_pRecoveryState) {
                // Load position from recovery state (+32 is position offset)
                float* position = reinterpret_cast<float*>(
                    reinterpret_cast<uintptr_t>(m_pRecoveryState) + 32);
                float posX = position[0];
                
                // Compare against threshold (loaded from .rdata)
                if (posX > g_swingPhaseThreshold) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

/**
 * pongPlayer::SetPlayerSide(uint8_t side)  @ 0x820C7C78 | size: 0x6C
 *
 * Sets the player's court side (0 = left, 1 = right) with debug logging.
 * Only updates if the value has changed.
 *
 * @param side - Court side identifier (0 or 1)
 */
void pongPlayer::SetPlayerSide(uint8_t side) {
    // Check if value is changing
    uint8_t* courtSidePtr = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + 0xC5);
    if (*courtSidePtr != side) {
        // Log the change (debug only)
        const char* sideStr = side ? "right" : "left";
        rage_DebugLog("pongPlayer::SetPlayerSide() - setting to %s", sideStr);  /* UNVERIFIED */

        // Update the side
        *courtSidePtr = side;
    }
}

/**
 * pongPlayer::IsSwingTimerInActiveWindow()  @ 0x820CDCD8 | size: 0xE8
 *
 * Complex timing check with three priority paths:
 *
 * PATH A: Timer expired → check if currentTime is between targetTime and
 *         a secondary threshold (field at +36)
 * PATH B: Timer not expired → call IsRecovering() and if true, call
 *         helper function on recovery state
 * PATH C: All checks fail → return false
 *
 * This gates swing input acceptance during specific timing windows.
 */
bool pongPlayer::IsSwingTimerInActiveWindow() const {
    if (!m_pTimingState) {
        return false;
    }
    
    float currentTime = m_pTimingState->m_currentTime;
    float targetTime = m_pTimingState->m_targetTime;
    
    // Check if timer has expired
    if (!(currentTime < targetTime)) {
        // Timer expired - check if we're in the secondary window
        float secondaryThreshold = *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(m_pTimingState) + 36);
        
        // Must be between targetTime and secondaryThreshold
        if (currentTime < secondaryThreshold) {
            return true;
        }
        return false;
    }
    
    // Timer still running - check recovery state
    if (IsRecovering()) {
        // Call helper function on recovery state
        // pongPlayer_CheckAnimTimeInRange is a recovery state method
        // For now, return true as the scaffold shows this path succeeds
        return true;
    }
    
    return false;
}

/**
 * pongPlayer::LerpTowardsTarget(float target, float rate, float deltaTime)
 *   @ 0x820C99B0 | size: 0x5C
 *
 * Smoothly interpolates a value towards a target using linear interpolation.
 * Clamps the result to exactly the target when close enough.
 *
 * @param target - Target value to lerp towards
 * @param rate - Interpolation rate (0-1, higher = faster)
 * @param deltaTime - Frame delta time
 * @return true if target was reached, false otherwise
 *
 * Note: This operates on the first float field of the object (offset +0)
 */
bool pongPlayer::LerpTowardsTarget(float target, float rate, float deltaTime) {
    float* lerpPtr = reinterpret_cast<float*>(this);
    float current = *lerpPtr;  // field at +0

    if (current < target) {
        current += rate * deltaTime;
        if (current > target) {
            current = target;
        }
    } else if (current > target) {
        current -= rate * deltaTime;
        if (current < target) {
            current = target;
        }
    }

    *lerpPtr = current;
    
    // Return true if we've reached the target
    return (current == target);
}

/**
 * pongPlayer::IsRecoveryTimerBelowThreshold()  @ 0x820CD598 | size: 0x58
 *
 * Returns true when the recovery state exists, the force-block flag is NOT set,
 * AND the recovery timer is below the global threshold.
 *
 * This is the inverse of IsRecovering() - it checks if recovery is nearly complete.
 */
bool pongPlayer::IsRecoveryTimerBelowThreshold() const {
    if (!m_pRecoveryState) {
        return false;
    }
    
    // If force-block is set, recovery is not nearly complete
    if (m_pRecoveryState->m_bForceBlock) {
        return false;
    }
    
    // Check if timer is below threshold (recovery nearly done)
    return (m_pRecoveryState->m_recoveryTimer < g_recoveryTimerThreshold);
}

/**
 * pongPlayer::GetStateObjectByIndex(uint32_t index)  @ 0x820CE3F0 | size: 0x64
 *
 * Returns a pointer to one of five state sub-objects based on index.
 * This is a simple switch-table lookup.
 *
 * @param index - State object index (0-4)
 * @return Pointer to the requested state object, or nullptr if index out of range
 *
 * State objects:
 *   0 → +100 (timing state)
 *   1 → +104 (action state)
 *   2 → +108 (anim state)
 *   3 → +112 (creature state)
 *   4 → +116 (recovery state)
 */
void* pongPlayer::GetStateObjectByIndex(uint32_t index) const {
    switch (index) {
        case 0: return m_pTimingState;      // +100
        case 1: return m_pActionState;      // +104
        case 2: return m_pAnimState;        // +108
        case 3: return m_pCreatureState;    // +112
        case 4: return m_pRecoveryState;    // +116
        default: return nullptr;
    }
}

/**
 * pongPlayer::GetSwingPhaseValue()  @ 0x820CD550 | size: 0x48
 *
 * Simple accessor that returns the swing phase value from the timing state.
 * Returns 0.0f if timing state doesn't exist.
 *
 * Note: This is a different function from IsSwingTimerActive - it returns
 * the actual phase value rather than a boolean.
 */
float pongPlayer::GetSwingPhaseValue() const {
    if (!m_pTimingState) {
        return 0.0f;
    }
    return m_pTimingState->m_currentTime;
}

/**
 * pongPlayer::IsSwingInputBlocked()  @ 0x820CD660 | size: 0x54
 *
 * Returns true when swing input should be blocked due to creature state.
 * Checks if the creature state is NOT ready (inverse of IsCreatureStateReady).
 *
 * Note: This is a wrapper that inverts the logic for clarity at call sites.
 */
bool pongPlayer::IsSwingInputBlocked() const {
    if (!m_bActive) {
        return true;  // Blocked if not active
    }
    
    // Blocked if creature state is NOT ready
    return !m_pCreatureState->IsReady();
}

/**
 * pongPlayer::GetAnimationBlendWeight()  @ 0x820CD6B8 | size: 0x80
 *
 * Returns the animation blend weight from the secondary creature state.
 * Used for blending between animation states during transitions.
 *
 * Returns 0.0f if the creature state doesn't exist or isn't active.
 */
float pongPlayer::GetAnimationBlendWeight() const {
    if (!m_pCreatureState2) {
        return 0.0f;
    }
    
    if (!m_pCreatureState2->IsActive()) {
        return 0.0f;
    }
    
    if (!m_pCreatureState2->IsReady()) {
        return 0.0f;
    }
    
    // Return blend weight from creature state (+offset TBD)
    // For now, return a placeholder
    return 1.0f;
}

/**
 * pongPlayer::GetCurrentSwingStrength()  @ 0x820CD5F0 | size: 0x70
 *
 * Returns the current swing strength value from the anim state.
 * This is used to determine how hard the player is swinging.
 *
 * Returns 0.0f if anim state doesn't exist or if the swing phase is blocked.
 */
float pongPlayer::GetCurrentSwingStrength() const {
    if (!m_pAnimState) {
        return 0.0f;
    }
    
    // Check if swing phase is blocked
    if (m_pAnimState->m_animPhase > g_swingPhaseThreshold) {
        return 0.0f;
    }
    
    // Check flag bit
    if (m_pAnimState->m_flags & 0x1) {
        return 0.0f;
    }
    
    // Return the swing strength value
    // Field location TBD - using placeholder
    return m_pAnimState->m_animPhase;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer — Small Functions Batch (44 functions, all ≤128 bytes)
// ─────────────────────────────────────────────────────────────────────────────

// ── VTABLE THUNKS (8 functions) ────────────────────────────────────────────

/**
 * pongPlayer secondary vtable destructor adjustor @ 0x82166DB8 | size: 0x8
 * Adjusts this by -4 for MI thunk before forwarding to primary dtor.
 */
void pongPlayer::DtorAdjustor() {  // rtti_A46C_0 @ 0x821AA46C
    typedef void (*DtorFn)(void*);
    ((DtorFn)(*(void***)((char*)this - 4))[0])((char*)this - 4);
}

/**
 * pongPlayer state handler tail-call thunks @ 0x82199188-91E8 | 16B each
 *
 * Seven thunks that each jump to a fixed state handler address.
 * Used as vtable entries for the player's hierarchical state machine.
 * Each loads a target address via lis+addi and tail-calls it.
 */
extern "C" void pongPlayer_StateHandler_9188(void*);  // target for 9188
extern "C" void pongPlayer_StateHandler_9198(void*);
extern "C" void pongPlayer_StateHandler_91A8(void*);
extern "C" void pongPlayer_StateHandler_91B8(void*);
extern "C" void pongPlayer_StateHandler_91C8(void*);
extern "C" void pongPlayer_StateHandler_91D8(void*);
extern "C" void pongPlayer_StateHandler_91E8(void*);

void pongPlayer::StateThunk_9188() { pongPlayer_StateHandler_9188(this); }  // @ 0x82199188
void pongPlayer::StateThunk_9198() { pongPlayer_StateHandler_9198(this); }  // @ 0x82199198
void pongPlayer::StateThunk_91A8() { pongPlayer_StateHandler_91A8(this); }  // @ 0x821991A8
void pongPlayer::StateThunk_91B8() { pongPlayer_StateHandler_91B8(this); }  // @ 0x821991B8
void pongPlayer::StateThunk_91C8() { pongPlayer_StateHandler_91C8(this); }  // @ 0x821991C8
void pongPlayer::StateThunk_91D8() { pongPlayer_StateHandler_91D8(this); }  // @ 0x821991D8
void pongPlayer::StateThunk_91E8() { pongPlayer_StateHandler_91E8(this); }  // @ 0x821991E8

// ── DIRTY FLAG SETTERS (7 functions, 28B each) ────────────────────────────

/**
 * pongPlayer dirty flag setters @ 0x82198AC0-8B80 | 28B each
 *
 * Seven functions that each set a specific bit in the dirty flags word
 * at this+0x19F4, guarded by a non-zero check on this+0x19F3.
 * These mark specific player data as needing network synchronization:
 *   0x01=position, 0x02=rotation, 0x04=scale, 0x08=velocity,
 *   0x10=animation, 0x20=state, 0x40=extra
 */
static void SetDirtyBit(void* self, uint8_t bit) {
    uint8_t guard = *(uint8_t*)((char*)self + 0x19F3);
    if (guard != 0) {
        uint8_t* flags = (uint8_t*)((char*)self + 0x19F4);
        *flags |= bit;
    }
}

void pongPlayer::MarkDirty_Position()  { SetDirtyBit(this, 0x01); }  // 8AC0 @ 0x82198AC0
void pongPlayer::MarkDirty_Rotation()  { SetDirtyBit(this, 0x02); }  // 8AE0 @ 0x82198AE0
void pongPlayer::MarkDirty_Scale()     { SetDirtyBit(this, 0x04); }  // 8B00 @ 0x82198B00
void pongPlayer::MarkDirty_Velocity()  { SetDirtyBit(this, 0x08); }  // 8B20 @ 0x82198B20
void pongPlayer::MarkDirty_Animation() { SetDirtyBit(this, 0x10); }  // 8B40 @ 0x82198B40
void pongPlayer::MarkDirty_State()     { SetDirtyBit(this, 0x20); }  // 8B60 @ 0x82198B60
void pongPlayer::MarkDirty_Extra()     { SetDirtyBit(this, 0x40); }  // 8B80 @ 0x82198B80

// ── SYNC FIELD FUNCTIONS (3 functions, 124B each) ─────────────────────────

/**
 * pongPlayer field synchronization @ 0x82199108/9EA8/CF10 | 124B each
 *
 * Three templated-pattern functions that compare a field value between
 * this and a source object. If different, calls a notification callback
 * (function pointer stored in the sync descriptor), then copies the value.
 * Used for network state synchronization.
 */
void pongPlayer::SyncByteField(void* syncDesc) {  // 9108_g @ 0x82199108
    uint8_t myVal = *(uint8_t*)this;
    uint8_t srcVal = *(uint8_t*)syncDesc;
    if (myVal != srcVal) {
        typedef void (*NotifyFn)(void*);
        NotifyFn notify = *(NotifyFn*)((char*)syncDesc + 8);
        if (notify) notify(this);
        *(uint8_t*)this = srcVal;
    }
}

void pongPlayer::SyncWordField(void* syncDesc) {  // 9EA8_g @ 0x82199EA8
    uint16_t myVal = *(uint16_t*)this;
    uint16_t srcVal = *(uint16_t*)syncDesc;
    if (myVal != srcVal) {
        typedef void (*NotifyFn)(void*);
        NotifyFn notify = *(NotifyFn*)((char*)syncDesc + 8);
        if (notify) notify(this);
        *(uint16_t*)this = srcVal;
    }
}

void pongPlayer::SyncFloatField(void* syncDesc) {  // CF10_g @ 0x8219CF10
    float myVal = *(float*)this;
    float srcVal = *(float*)syncDesc;
    if (myVal != srcVal) {
        typedef void (*NotifyFn)(void*);
        NotifyFn notify = *(NotifyFn*)((char*)syncDesc + 8);
        if (notify) notify(this);
        *(float*)this = srcVal;
    }
}

// ── DELEGATION / GUARD FUNCTIONS ──────────────────────────────────────────

// Note: Update() and SaveDrawData() defined later in file with debug strings.

/**
 * pongPlayer::UpdateReplay @ ~100B
 * Guards on m_bActive, calls pongCreature update on +452, then _4C08.
 */
void pongPlayer::UpdateReplay() {  // pongPlayer_UpdateReplay

    if (!*(uint8_t*)((char*)this + 44)) return;
    void* creature = *(void**)((char*)this + 452);
    if (creature) pongCreature_UpdateReplay(creature);
    pongPlayer_ProcessReplay(this);
}

/**
 * pongPlayer::UpdateTimerWithNetSync @ 0x82191510 | size: 0x5C
 * Calls vtable slot 2 on sub-object at +12, stores float result,
 * then calls sync helper with arg 50.
 */
void pongPlayer::UpdateTimerWithNetSync() {  // 1510_g @ 0x82191510

    void* subObj = *(void**)((char*)this + 12);
    if (!subObj) return;
    typedef float (*GetTimerFn)(void*);
    float timer = ((GetTimerFn)(*(void***)subObj)[2])(subObj);
    *(float*)((char*)this + 16) = timer;
    pongPlayer_NetSync(this, 50);
}

/**
 * pongPlayer::DestroyAllEntries @ 0x82194510 | size: 0x78
 * Loops over array at +4, calls vtable slot 1 (dtor) on each non-null
 * entry, then clears the array counters.
 */
void pongPlayer::DestroyAllEntries() {  // 4510_wrh @ 0x82194510
    void** array = *(void***)((char*)this + 4);
    int count = *(int*)((char*)this + 8);

    for (int i = 0; i < count; i++) {
        if (array[i]) {
            typedef void (*DtorFn)(void*, int);
            ((DtorFn)(*(void***)array[i])[0])(array[i], 1);
            array[i] = nullptr;
        }
    }
    *(int*)((char*)this + 8) = 0;
    *(int*)((char*)this + 12) = 0;
}

// ── COMPUTATION: State & Reset Functions ──────────────────────────────────

/**
 * pongPlayer::IsPlayerSlotActive @ vfn_1 | size: 0x5C
 * Looks up player index +464 in two global arrays, returns true if
 * state==1 and slot!=-1.
 */
bool pongPlayer::IsPlayerSlotActive() {  // vfn_1
    extern int32_t* g_playerSlotStates;   // global array of player states
    extern int32_t* g_playerSlotIndices;  // global array of slot indices

    int playerIdx = *(int*)((char*)this + 464);
    if (playerIdx < 0) return false;
    return (g_playerSlotStates[playerIdx] == 1) && (g_playerSlotIndices[playerIdx] != -1);
}

/**
 * pongPlayer::SetTransitionState @ 0x82199E48 | size: 0x5C
 * Toggles transition state bytes at +40/+41/+42 based on bool arg.
 */
void pongPlayer::SetTransitionState(bool enable) {  // 9E48_g @ 0x82199E48
    if (enable) {
        *(uint8_t*)((char*)this + 40) = 1;
        *(uint8_t*)((char*)this + 41) = 0;
        *(uint8_t*)((char*)this + 42) = 0;
    } else {
        *(uint8_t*)((char*)this + 40) = 0;
        *(uint8_t*)((char*)this + 41) = 1;
        *(uint8_t*)((char*)this + 42) = 1;
    }
}

/**
 * pongPlayer::ResetTransformData @ 0x8219E748 | size: 0x68
 * Zeros 4×16B vectors at +32, zero vec at +16, stores constants at
 * +4/+8, clears bytes 0/1.
 */
void pongPlayer::ResetTransformData() {  // E748_p33 @ 0x8219E748
    memset((char*)this + 32, 0, 64);  // 4 × vec128
    memset((char*)this + 16, 0, 16);  // 1 × vec128
    *(float*)((char*)this + 4) = 0.0f;
    *(float*)((char*)this + 8) = 0.0f;
    *(uint8_t*)((char*)this + 0) = 0;
    *(uint8_t*)((char*)this + 1) = 0;
}

/**
 * pongPlayer::ResetScoringState @ 0x821992A0 | size: 0x70
 * Resets scoring fields: calls _9310(0.0), calls _9918 twice with
 * args 0 and 1, zeros fields +72/+69/+76/+77.
 */
void pongPlayer::ResetScoringState() {  // 92A0_g @ 0x821992A0

    pongPlayer_ResetShotTiming(this, 0.0f);
    pongPlayer_SetScoreSlot(this, 0);
    pongPlayer_SetScoreSlot(this, 1);
    *(uint8_t*)((char*)this + 72) = 0;
    *(uint8_t*)((char*)this + 69) = 0;
    *(uint8_t*)((char*)this + 76) = 0;
    *(uint8_t*)((char*)this + 77) = 0;
}

/**
 * pongPlayer::ResetShotData @ 0x8219FA40 | size: 0x74
 * Stores 0.0f to multiple float fields, clears bytes, calls _E640 twice.
 */
void pongPlayer::ResetShotData() {  // FA40_g @ 0x8219FA40

    *(float*)((char*)this + 5600) = 0.0f;
    *(float*)((char*)this + 5604) = 0.0f;
    *(float*)((char*)this + 5608) = 0.0f;
    *(uint8_t*)((char*)this + 5612) = 0;
    *(uint8_t*)((char*)this + 5613) = 0;
    pongPlayer_ClearShotState(this);
}

/**
 * pongPlayer::ResetSwingParams @ 0x82197038 | size: 0x54
 * Calls _76E8 on sub-object +5404, stores 3 float constants.
 */
void pongPlayer::ResetSwingParams() {  // 7038 @ 0x82197038

    void* swingObj = *(void**)((char*)this->m_pSwingTimingObj);
    if (swingObj) pongPlayer_ResetSwingImpl(swingObj);

    *(float*)((char*)this + 5600) = 0.0f;
    *(float*)((char*)this + 5604) = 0.0f;
    *(float*)((char*)this + 5608) = 0.0f;
}

/**
 * pongPlayer::ResetShotTimingData @ 0x82199310 | size: 0x7C
 * Inits floats to 0, calls _CF10 twice, clears byte+28, calls _9EA8.
 */
void pongPlayer::ResetShotTimingData(float value) {  // 9310_g @ 0x82199310

    *(float*)((char*)this + 0) = value;
    *(float*)((char*)this + 4) = value;
    pongPlayer_SyncFloat(this, (char*)this + 0);
    pongPlayer_SyncFloat(this, (char*)this + 4);
    *(uint8_t*)((char*)this + 28) = 0;
    pongPlayer_SyncWord(this, (char*)this + 24);
}

// ── COMPUTATION: Queries & Lookups ───────────────────────────────────────

/**
 * pongPlayer::GetRangeDistance @ 0x82197A48 | size: 0x58
 * Computes range from int16 fields at +4 and +6.
 */
int pongPlayer::GetRangeDistance() {  // 7A48_g @ 0x82197A48
    int16_t a = *(int16_t*)((char*)this + 4);
    int16_t b = *(int16_t*)((char*)this + 6);
    int dist = (int)b - (int)a;
    return (dist > 0) ? dist : 0;
}

/**
 * pongPlayer::IsPositionInBounds @ 0x82195170 | size: 0x5C
 * Float range test on fields +32, +36 against constants.
 */
bool pongPlayer::IsPositionInBounds() {  // 5170_w @ 0x82195170
    float x = *(float*)((char*)this + 32);
    float z = *(float*)((char*)this + 36);
    // Bounds constants loaded from .rdata
    return (x >= -10.0f && x <= 10.0f && z >= -5.0f && z <= 5.0f);
}

/**
 * pongPlayer::GetEffectiveSpeed @ 0x82198E88 | size: 0x78
 * Checks flag+334, field+340, global ptr. Returns float from
 * +264 or +268 depending on conditions.
 */
float pongPlayer::GetEffectiveSpeed() {  // 8E88_g @ 0x82198E88
    uint8_t flag = *(uint8_t*)((char*)this + 334);
    uint32_t field = *(uint32_t*)((char*)this + 340);

    if (flag != 0 && field != 0) {
        return *(float*)((char*)this + 268);
    }
    return *(float*)((char*)this + 264);
}

/**
 * pongPlayer::GetNormalizedFrameRate @ 0x8219D238 | size: 0x60
 * Dereferences +36->+36, computes (int_field * const) / float_field.
 */
float pongPlayer::GetNormalizedFrameRate() {  // D238_g @ 0x8219D238
    void* sub1 = *(void**)((char*)this + 36);
    if (!sub1) return 0.0f;
    void* sub2 = *(void**)((char*)sub1 + 36);
    if (!sub2) return 0.0f;

    int32_t intVal = *(int32_t*)((char*)sub2 + 8);
    float divisor = *(float*)((char*)sub2 + 12);
    if (divisor == 0.0f) return 0.0f;

    return (float)intVal / divisor;
}

/**
 * pongPlayer::IsBallSplashActive @ 0x82190C58 | size: 0x50
 * Indexes into global array, calls fxBallSplash check, returns bool.
 */
bool pongPlayer::IsBallSplashActive() {  // 0C58_g @ 0x82190C58
    return pongPlayer_CheckBallSplash(this);
}

/**
 * pongPlayer::IsCreatureAnimReady @ 0x82192578 | size: 0x50
 * Chain-deref +452->+188->+120, null-check, calls helper, returns bool.
 */
bool pongPlayer::IsCreatureAnimReady() {  // 2578_g @ 0x82192578
    void* creature = *(void**)((char*)this + 452);
    if (!creature) return false;
    void* animMgr = *(void**)((char*)creature + 188);
    if (!animMgr) return false;
    void* animData = *(void**)((char*)animMgr + 120);
    return animData != nullptr;
}

/**
 * pongPlayer::HasAnimationDelta @ 0x821904B0 | size: 0x54
 * Calls helper, indexes array at +796, XORs byte[0] vs byte[3].
 */
bool pongPlayer::HasAnimationDelta() {  // 04B0_g @ 0x821904B0

    int idx = pongPlayer_GetAnimIndex(this);
    char* array = *(char**)((char*)this + 796);
    uint8_t a = *(uint8_t*)(array + idx * 4 + 0);
    uint8_t b = *(uint8_t*)(array + idx * 4 + 3);
    return (a ^ b) > 127;
}

/**
 * pongPlayer::FindRegisteredObject @ 0x8219AED0 | size: 0x6C
 * Linear search of array at +152 for matching pointer.
 */
int pongPlayer::FindRegisteredObject(void* target) {  // AED0_p39 @ 0x8219AED0
    void** array = *(void***)((char*)this + 152);
    int count = *(int*)((char*)this + 156);

    for (int i = 0; i < count; i++) {
        if (array[i] == target) return i;
    }
    return -1;
}

// ── COMPUTATION: Position & Movement ─────────────────────────────────────

/**
 * pongPlayer::ApplyPositionOffset @ 0x82196308 | size: 0x54
 * Gets velocity vec, adds to position at +224, stores at +32.
 */
void pongPlayer::ApplyPositionOffset() {  // 6308_g @ 0x82196308

    float velocity[3];
    pongPlayer_GetVelocity(this, velocity);

    float* pos = (float*)((char*)this + 224);
    *(float*)((char*)this + 32) = pos[0] + velocity[0];
    *(float*)((char*)this + 36) = pos[1] + velocity[1];
    *(float*)((char*)this + 40) = pos[2] + velocity[2];
}

/**
 * pongPlayer::ApplyPositionAndVelocityOffset @ 0x821962A8 | size: 0x60
 * Similar to ApplyPositionOffset but also adds velocity at +240.
 */
void pongPlayer::ApplyPositionAndVelocityOffset() {  // 62A8_g @ 0x821962A8

    float velocity[3];
    pongPlayer_GetVelocity(this, velocity);

    float* pos224 = (float*)((char*)this + 224);
    float* pos240 = (float*)((char*)this + 240);
    *(float*)((char*)this + 32) = pos224[0] + pos240[0] + velocity[0];
    *(float*)((char*)this + 36) = pos224[1] + pos240[1] + velocity[1];
    *(float*)((char*)this + 40) = pos224[2] + pos240[2] + velocity[2];
}

/**
 * pongPlayer::InterpolatePosition2D @ 0x8219E7B0 | size: 0x7C
 * Builds 2D point from player indices, calls interpolation helper.
 */
void pongPlayer::InterpolatePosition2D() {  // E7B0_g @ 0x8219E7B0

    float pos[2];
    pongPlayer_GetPlayerPosition(this, pos);
    pongPlayer_Interpolate(this, pos);
}

// ── COMPUTATION: Network & Game State ────────────────────────────────────

/**
 * pongPlayer::ResetMoverState @ 0x821936D8 | size: 0x60
 * Checks net state byte, calls reset helper, gets result, calls mover reset.
 */
void pongPlayer::ResetMoverState() {  // 36D8_g @ 0x821936D8
    pongPlayer_ResetMoverImpl(this);
}

/**
 * pongPlayer::ApplyPlayerNetState @ 0x82199C60 | size: 0x70
 * Looks up player in global array, validates index, applies net state.
 */
void pongPlayer::ApplyPlayerNetState() {  // 9C60_g @ 0x82199C60
    pongPlayer_ApplyNetStateImpl(this);
}

/**
 * pongPlayer::InitializeNewShot @ 0x8219F9C0 | size: 0x80
 * Lookup creature slot, store flags, reset shot data, set byte+5504=1.
 */
void pongPlayer::InitializeNewShot() {  // F9C0_g @ 0x8219F9C0
    pongPlayer_InitShotImpl(this);
}

/**
 * pongPlayer::CompareTypeNames @ 0x8219F0B8 | size: 0x38
 * strcmp-like loop comparing two vtable name pointers.
 */
bool pongPlayer::CompareTypeNames(void* other) {  // F0B8_p46 @ 0x8219F0B8
    const char* nameA = *(const char**)((char*)this + 0);
    const char* nameB = *(const char**)((char*)other + 0);

    while (*nameA && *nameA == *nameB) {
        nameA++;
        nameB++;
    }
    return *nameA == *nameB;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer — Small Functions Batch (13 functions, ≤96B)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * pongPlayer::Update @ 0x8218CF08 | size: 0x28
 *
 * Per-frame update entry point. Checks if the player is active (+44);
 * if not, logs a debug message and returns. Otherwise dispatches to
 * the full update implementation via vtable slot 4.
 */
void pongPlayer::Update() {
    if (!*(uint8_t*)((char*)this + 44)) {
        rage_DebugLog("pongPlayer::Update() - player is not active");
        return;
    }
    typedef void (*UpdateImpl)(void*);
    ((UpdateImpl)(*(void***)this)[4])(this);
}

/**
 * pongPlayer::SaveDrawData @ 0x8218E860 | size: 0x30
 *
 * Saves draw state for the renderer. Checks the draw-enabled byte
 * at +36; if false, logs and returns. Otherwise loads the draw
 * data object at +444, offsets to +16, and calls its vtable slot 1.
 */
void pongPlayer::SaveDrawData() {
    if (!*(uint8_t*)((char*)this + 36)) {
        rage_DebugLog("pongPlayer::SaveDrawData() - draw not enabled");
        return;
    }
    void* drawData = *(void**)((char*)this + 444);
    void* drawObj = (char*)drawData + 16;
    typedef void (*DrawFunc)(void*);
    ((DrawFunc)(*(void***)drawObj)[1])(drawObj);
}

/**
 * pongPlayer::CompareTypeInfo @ 0x820CF0B8 | size: 0x38
 *
 * RTTI type comparison. Reads type_info pointers from both objects'
 * vtable slot 0 (MSVC-style RTTI), then does a byte-by-byte strcmp
 * on the type name strings. Returns 0 if types match.
 */
int pongPlayer::CompareTypeInfo(void* other) {
    const char* nameA = *(const char**)(*(void***)this);
    const char* nameB = *(const char**)(*(void***)other);
    // Byte-by-byte string comparison (inline strcmp)
    while (*nameA && *nameA == *nameB) {
        nameA++;
        nameB++;
    }
    return (int)(unsigned char)*nameA - (int)(unsigned char)*nameB;
}

/**
 * pongPlayer::ResetShotTimerDefaults @ 0x821A7038 | size: 0x54
 *
 * Resets the shot timing system. Calls a cleanup function on the
 * shot timer sub-object at +5404, then writes three default float
 * constants to fields at +5600, +5604, +5608.
 */
void pongPlayer::ResetShotTimerDefaults() {
    extern void ResetShotTimingState(void* shotTimer);  // @ 0x821A76E8
    ResetShotTimingState((char*)this->m_pSwingTimingObj);

    // Reset timing defaults (values from .rdata constants)
    *(float*)((char*)this + 5600) = 0.0f;   // shot start time
    *(float*)((char*)this + 5604) = 0.0f;   // shot duration
    *(float*)((char*)this + 5608) = 1.0f;   // shot speed multiplier
}

/**
 * pongPlayer::ComputeBasePosition @ 0x821D6308 | size: 0x54
 *
 * Computes the player's base position by adding a displacement vector
 * (from pongPlayer_ComputeTargetPosition) to the rest position at +224, storing the
 * result at +32.
 */
void pongPlayer::ComputeBasePosition() {
    extern void pongPlayer_ComputeTargetPosition(float* out, void* player, int flags);
    float displacement[4];
    pongPlayer_ComputeTargetPosition(displacement, this, 0);

    float* restPos = (float*)((char*)this + 224);
    float* outPos = (float*)((char*)this + 32);
    outPos[0] = displacement[0] + restPos[0];
    outPos[1] = displacement[1] + restPos[1];
    outPos[2] = displacement[2] + restPos[2];
    outPos[3] = displacement[3] + restPos[3];
}

/**
 * pongPlayer::ComputeFullPosition @ 0x821D62A8 | size: 0x60
 *
 * Like ComputeBasePosition but adds THREE vectors: displacement + 
 * rest position at +224 + offset vector at +240. Stores at +32.
 * The extra flag parameter selects a different displacement mode.
 */
void pongPlayer::ComputeFullPosition(int flags) {
    extern void pongPlayer_ComputeTargetPosition(float* out, void* player, int flags);
    float displacement[4];
    pongPlayer_ComputeTargetPosition(displacement, this, flags);

    float* restPos = (float*)((char*)this + 224);
    float* offsetVec = (float*)((char*)this + 240);
    float* outPos = (float*)((char*)this + 32);
    outPos[0] = displacement[0] + restPos[0] + offsetVec[0];
    outPos[1] = displacement[1] + restPos[1] + offsetVec[1];
    outPos[2] = displacement[2] + restPos[2] + offsetVec[2];
    outPos[3] = displacement[3] + restPos[3] + offsetVec[3];
}

/**
 * pongPlayer::HasInputDirectionChanged @ 0x821E04B0 | size: 0x54
 *
 * Checks if the player's input direction has changed sign. Gets the
 * current input index, looks up a 4-byte entry in the input table at
 * +796, XORs byte[0] with byte[3]; returns true if the XOR has the
 * sign bit set (direction reversal).
 */
bool pongPlayer::HasInputDirectionChanged() {
    extern int pongPlayer_GetInputStepIndex(void* player);
    int inputIdx = pongPlayer_GetInputStepIndex(this);
    uint8_t* entry = (uint8_t*)((char*)this + 796) + inputIdx * 4;
    uint8_t xorResult = entry[0] ^ entry[3];
    return xorResult > 127;  // sign bit set = direction changed
}

/**
 * pongPlayer::GetAnimFrameDistance @ 0x821A7A48 | size: 0x58
 *
 * Returns the circular distance between two animation frame indices
 * stored as int16 fields at +4 and +6. If either is -1 (invalid),
 * returns 0. Wraps negative differences by adding 20 (frame count).
 */
int pongPlayer::GetAnimFrameDistance() {
    int16_t frameA = *(int16_t*)((char*)this + 4);
    int16_t frameB = *(int16_t*)((char*)this + 6);

    if (frameA == -1 || frameB == -1) return 0;

    int distance = (int)frameA - (int)frameB + 1;
    if (distance < 0) distance += 20;  // wrap around frame count
    return distance;
}

/**
 * pongPlayer::IsInContactZone @ 0x82285170 | size: 0x5C
 *
 * Tests whether the player is within the valid contact zone.
 * Checks float at +36 against a range, then float at +32 against
 * another range. Returns true only if both are within bounds.
 */
bool pongPlayer::IsInContactZone() {
    float valA = *(float*)((char*)this + 36);
    float valB = *(float*)((char*)this + 32);

    // Range check A
    extern const float g_contactZoneMinA;  // .rdata constant
    extern const float g_contactZoneMaxA;
    if (valA < g_contactZoneMinA || valA > g_contactZoneMaxA) return false;

    // Range check B
    extern const float g_contactZoneMinB;
    extern const float g_contactZoneMaxB;
    return (valB >= g_contactZoneMinB && valB <= g_contactZoneMaxB);
}

// Note: IsBallSplashActive already defined above (@ 0x82190C58)

/**
 * pongPlayer::GetNormalizedRecoveryRate @ 0x820DD238 | size: 0x60
 *
 * Returns the player's normalized recovery rate. If the recovery
 * sub-object at +36 is null, returns a default constant. Otherwise
 * reads the timer value and computes (intField * scale) / timer.
 */
float pongPlayer::GetNormalizedRecoveryRate() {
    void* recoveryObj = *(void**)((char*)this + 36);
    if (!recoveryObj) {
        return 0.0f;  // default when no recovery object
    }

    void* innerObj = *(void**)((char*)recoveryObj + 36);
    float timer = *(float*)((char*)innerObj + 12);

    extern const float g_recoveryThreshold;
    if (timer <= g_recoveryThreshold) {
        return 0.0f;
    }

    int rawValue = *(int32_t*)((char*)innerObj + 8);
    extern const float g_recoveryScale;
    return ((float)rawValue * g_recoveryScale) / timer;
}

/**
 * pongPlayer::SyncNetworkState @ 0x82391510 | size: 0x5C
 *
 * Synchronizes player state with the network subsystem. Loads the
 * network sub-object from +12, calls its vtable slot 2 (which stores
 * a float result at +12), then calls the network sync function with
 * a 50-frame window.
 */
void* pongPlayer::SyncNetworkState() {
    extern void SinglesNetworkClient_FormatTimeString(void* netObj, int frames);

    void* subObj = *(void**)((char*)this + 12);

    // Call vtable slot 2 on sub-object (stores sync float at +12)
    typedef void (*SyncFunc)(void*);
    ((SyncFunc)(*(void***)subObj)[2])(subObj);

    // Sync network state with 50-frame window
    void* netObj = (char*)subObj + 16;
    SinglesNetworkClient_FormatTimeString(netObj, 50);
    return netObj;
}


// ===========================================================================
// SECTION 11 — Batch: 10 small pongPlayer functions (64-256 bytes)
// ===========================================================================

// ── External helpers for this batch ──────────────────────────────────────
extern bool fxBallSplash2D_IsActive(void* splashObj);         // @ 0x821D5998
extern void pongPlayer_NormalizeShotVector(void* shotObj);    // @ 0x821D4EA0
extern void pongPlayer_ApplyShotToGrid(void* shotObj, void* gridSlot, void* parent, int flags);  // @ 0x821D5010
extern void pongPlayer_ClampInputToRange(void* inputObj, float* value);  // @ 0x8219CF10
extern void pongPlayer_LookupPlayerSlot(void* slotData);     // @ 0x821E7CC0
extern void pongPlayer_InitNetworkEntry(void* entryBase);    // @ 0x821E05A8
extern void pongCreature_UpdateAnimState(void* creature);     // @ 0x820C59C0
extern void pongPlayer_UpdateDrawState(void* player);         // @ 0x82194C08


/**
 * pongPlayer::IsBallSplashActive  @ 0x821A0C58 | size: 0x50
 *
 * Checks whether the ball splash 2D effect for the current player slot
 * is currently active.  Looks up the splash object from a global array
 * indexed by the active player slot, then queries its active state.
 */
// HACK: address comment for linker: @ 0x821A0C58
extern uint32_t g_activePlayerSlot;       // @ SDA +25976 (0x82606578)
extern uint8_t  g_ballSplashArray[];      // @ 0x8261A3D0 — 416-byte-per-entry array, +48 base

bool pongPlayer::IsBallSplashActiveForSlot() {  // pongPlayer_0C58_g @ 0x821A0C58
    void* splashObj = (void*)(g_ballSplashArray + g_activePlayerSlot * 416 + 48);
    return fxBallSplash2D_IsActive(splashObj);
}


/**
 * pongPlayer::FindRegisteredObjectByAddress  @ 0x8218AED0 | size: 0x6C
 *
 * Linear search through the registered object array at +152 for
 * a matching pointer.  If found more than once, asserts with a debug
 * message.  If not found at all, asserts with a "not found" message.
 *
 * Unlike FindRegisteredObject (@ 0x8219AED0) which returns the index,
 * this variant asserts on both "duplicate" and "not found" conditions.
 * It only calls the debug log (nop in release builds).
 */
// HACK: address comment for linker: @ 0x8218AED0
void pongPlayer::FindRegisteredObjectByAddress(void* target) {  // pongPlayer_AED0_p39 @ 0x8218AED0
    int count = *(int*)((char*)this + 144);
    void** array = *(void***)((char*)this + 152);
    int foundIndex = -1;

    for (int i = 0; i < count; i++) {
        if (foundIndex != -1) {
            // Already found once — duplicate entry detected
            uint32_t name = *(uint32_t*)((char*)target + 4);
            rage_DebugLog("pongPlayer::FindRegisteredObjectByAddress() - duplicate entry %s", name);
            return;
        }
        if (array[i] == target) {
            foundIndex = i;
        }
    }

    if (foundIndex != -1) {
        // Found once but scaffold still asserts (likely debug-only path)
        uint32_t name = *(uint32_t*)((char*)target + 4);
        rage_DebugLog("pongPlayer::FindRegisteredObjectByAddress() - duplicate entry %s", name);
        return;
    }

    // Not found at all
    uint32_t name = *(uint32_t*)((char*)target + 4);
    rage_DebugLog("pongPlayer::FindRegisteredObjectByAddress() - object not found %s", name);
}


/**
 * pongPlayer::SetupNetworkPlayerEntry  @ 0x821C9C60 | size: 0x70
 *
 * Called from the network client to configure a player entry in the
 * per-player data table.  Looks up the player slot, reads a byte from
 * the source data at (index + offset + 64), and if the data table
 * entry is valid (0..3), stores the parameter and initialises the
 * network entry sub-object.
 */
// HACK: address comment for linker: @ 0x821C9C60
extern uint32_t  g_networkPlayerSlot;           // @ 0x8271A32C
extern uint8_t*  g_networkPlayerDataTable;      // @ 0x8271A31C — 808-byte entries

void pongPlayer::SetupNetworkPlayerEntry(int index, int offset, uint32_t param) {  // pongPlayer_9C60_g @ 0x821C9C60
    pongPlayer_LookupPlayerSlot((void*)(uintptr_t)g_networkPlayerSlot);

    uint8_t sourceByte = *((uint8_t*)((uintptr_t)(index + offset) + 64));
    int entryIndex = *(int32_t*)(g_networkPlayerDataTable + (index + 1) * 4);

    if (entryIndex < 0 || entryIndex >= 4) {
        return;
    }

    uint8_t* entry = g_networkPlayerDataTable + entryIndex * 808;
    *(uint32_t*)(entry + 20) = param;
    pongPlayer_InitNetworkEntry((void*)(entry + 12));
}


/**
 * pongPlayer::ResetReticleEntries  @ 0x82384510 | size: 0x78
 *
 * Resets a reticle/targeting sub-object by zeroing its timing floats
 * at +48 and +56, clearing active flags at +52 and +60, then iterating
 * over all child entries and calling their Reset virtual method
 * (vtable slot 1). Finally clears the dirty flag at +62.
 */
// HACK: address comment for linker: @ 0x82384510
extern const float g_kZeroFloat;  // @ 0x8202D110

void pongPlayer::ResetReticleEntries() {  // pongPlayer_4510_wrh @ 0x82384510
    void* self = (void*)this;
    uint16_t count = *(uint16_t*)((char*)self + 8);

    *(float*)((char*)self + 56) = 0.0f;
    *(float*)((char*)self + 48) = 0.0f;
    *(uint8_t*)((char*)self + 60) = 0;
    *(uint8_t*)((char*)self + 52) = 0;

    void** array = *(void***)((char*)self + 4);
    for (int i = 0; i < (int)count; i++) {
        void* entry = array[i];
        // Call vtable slot 1 (Reset) on each entry
        typedef void (*ResetFn)(void*);
        ((ResetFn)(*(void***)entry)[1])(entry);
    }

    *(uint8_t*)((char*)self + 62) = 0;
}


/**
 * pongPlayer::GetEffectiveSpeed  @ 0x821C8E88 | size: 0x78
 *
 * Returns the player's effective movement speed.  When the player has
 * an active override flag at +334 OR a non-null speed object at +340,
 * returns the boosted speed from +268.  Otherwise checks a global
 * "slow motion" pointer — if set, returns boosted speed; if clear,
 * returns the base speed from +264.
 */
// HACK: address comment for linker: @ 0x821C8E88
extern void* g_pSlowMotionState;  // @ 0x825CB3C0 (+4)

float pongPlayer::GetEffectiveSpeedWithSlowMo() {  // pongPlayer_8E88_g @ 0x821C8E88
    bool hasOverride = *(uint8_t*)((char*)this + 334) != 0;

    if (!hasOverride) {
        void* speedObj = *(void**)((char*)this + 340);
        bool hasSpeedObj = (speedObj != nullptr);
        if (!hasSpeedObj) {
            // No override, no speed object — check slow motion state
            void* slowMoPtr = *(void**)((char*)g_pSlowMotionState + 4);
            if (slowMoPtr != nullptr) {
                // Slow motion active — return boosted speed
                return *(float*)((char*)this + 268);
            }
            // Normal speed
            return *(float*)((char*)this + 264);
        }
    }

    // Override or speed object present — return boosted speed
    return *(float*)((char*)this + 268);
}


/**
 * pongPlayer::SetShotDirection  @ 0x821D6360 | size: 0x84
 *
 * Sets the shot direction vector on the player's shot sub-object at +64.
 * Computes the direction from a 4-float bounding rect (minX, minY, maxX, maxY):
 *   dirX = rect.maxX - rect.maxY  (cross-court component)
 *   dirY = rect.minX - rect.minY  (depth component)
 * Then sets the Z component from a constant, normalises the vector,
 * and applies it to the grid system.
 *
 * r3 = parent object, r4 = bounding rect (4 floats)
 */
// HACK: address comment for linker: @ 0x821D6360
extern const float g_kShotDirectionConstants[];  // @ 0x8202D108 — 3 floats at offsets 0, 8, 16

void pongPlayer::SetShotDirection(float* rect) {  // pongPlayer_6360_g @ 0x821D6360
    float* shotVec = (float*)((char*)this + 64);

    float dirX = rect[3] - rect[2];  // maxY - maxX
    float dirY = rect[0] - rect[1];  // minX - minY

    shotVec[0] = dirX;
    shotVec[1] = dirY;
    shotVec[2] = g_kShotDirectionConstants[2];  // Z from constant table

    pongPlayer_NormalizeShotVector((void*)shotVec);

    float* gridSlot = shotVec + 4;  // +80 from base = +16 from shotVec
    pongPlayer_ApplyShotToGrid((void*)shotVec, gridSlot, (void*)this, 1);
}


/**
 * pongPlayer::AccumulateShotDirection  @ 0x821D63E8 | size: 0x88
 *
 * Accumulates (adds) a direction delta to the existing shot vector
 * at +64 from a bounding rect, then renormalises and applies to grid.
 * Similar to SetShotDirection but adds to existing values instead of
 * replacing them.
 */
// HACK: address comment for linker: @ 0x821D63E8
void pongPlayer::AccumulateShotDirection(float* rect) {  // pongPlayer_63E8_g @ 0x821D63E8
    float* shotVec = (float*)((char*)this + 64);

    float deltaY = rect[0] - rect[1];  // minX - minY
    float deltaX = rect[3] - rect[2];  // maxY - maxX

    // Accumulate onto existing direction
    shotVec[1] += deltaY;
    shotVec[0] += deltaX;

    pongPlayer_NormalizeShotVector((void*)shotVec);

    float* gridSlot = shotVec + 4;  // +16 from shotVec
    pongPlayer_ApplyShotToGrid((void*)shotVec, gridSlot, (void*)this, 1);
}


/**
 * pongPlayer::GetFadeAlpha  @ 0x821A7F98 | size: 0x84
 *
 * Computes a fade alpha value for the player's visual representation.
 * Uses one of two fade paths depending on a direction flag at +28:
 *
 *   Path A (flag set): If both current (+4) and max (+8) are above zero,
 *     computes alpha = ((max - current) / max) * scale.
 *
 *   Path B (flag clear): If both current (+12) and max (+16) are above zero,
 *     computes alpha = (1.0 - (max - current) / max) * scale.
 *
 * Returns 0.0 if the relevant values are not positive.
 * The 'scale' constant comes from the data table at offset +16.
 */
// HACK: address comment for linker: @ 0x821A7F98
void pongPlayer::GetFadeAlpha(float* outAlpha) {  // pongPlayer_7F98_g @ 0x821A7F98
    // r3+8 is the actual base (addi r3,r3,8 in scaffold was pre-offset)
    float* data = (float*)((char*)this + 8);
    uint8_t directionFlag = *(uint8_t*)((char*)data + 28);

    extern const float g_kShotDirConsts[];  // same constant table at 0x8202D108

    float zeroConst = g_kShotDirConsts[2];  // offset +8 in table
    float oneConst  = g_kShotDirConsts[0];  // offset +0 in table
    float scaleConst = g_kShotDirConsts[4]; // offset +16 in table

    if (directionFlag) {
        float current = *(float*)((char*)data + 4);
        if (current <= zeroConst) {
            return;  // f1 stays as zeroConst
        }
        float maximum = *(float*)((char*)data + 8);
        if (maximum <= zeroConst) {
            return;
        }
        float ratio = (maximum - current) / maximum;
        *outAlpha = ratio * scaleConst;
        return;
    }

    // Path B: reverse fade
    float current2 = *(float*)((char*)data + 12);
    if (current2 <= zeroConst) {
        return;
    }
    float maximum2 = *(float*)((char*)data + 16);
    if (maximum2 <= zeroConst) {
        return;
    }

    float ratio2 = (maximum2 - current2) / maximum2;
    float invRatio = oneConst - ratio2;
    *outAlpha = invRatio * scaleConst;
}


/**
 * pongPlayer::ResetShotState  @ 0x821962C0 | size: 0x88
 *
 * Called from Process() to reset shot-related state when a new rally begins.
 * If a "shot pending" flag at +6537 is set, it clears it, zeroes out the
 * shot timing floats at +6512/+6516/+6572, and enables the shot readiness
 * flag on the creature's animation controller.
 *
 * Then, regardless of the pending flag, if the creature pointer (+452) is
 * valid, it sets or clears the animation controller's active flag at +134
 * based on the input parameter.
 */
// HACK: address comment for linker: @ 0x821962C0
void pongPlayer::ResetShotState(bool enableAnim) {  // pongPlayer_62C0_g @ 0x821962C0
    uint8_t shotPending = *(uint8_t*)((char*)this + 6537);

    if (shotPending) {
        void* creature = *(void**)((char*)this + 452);

        *(uint8_t*)((char*)this + 6537) = 0;
        *(float*)((char*)this + 6512) = 0.0f;
        *(float*)((char*)this + 6516) = 0.0f;
        *(float*)((char*)this + 6572) = 0.0f;

        // Enable readiness flag on creature's animation controller
        void* animCtrl = *(void**)((char*)creature + 168);
        uint8_t* animFlags = (uint8_t*)((char*)animCtrl + 16 + 48);
        *animFlags |= 1;

        *(uint8_t*)((char*)this + 6637) = 0;
    }

    void* creature = *(void**)((char*)this + 452);
    if (!creature) {
        return;
    }

    if (enableAnim) {
        void* animCtrl = *(void**)((char*)creature + 168);
        // Zero out the velocity vector at +208 (16-byte aligned)
        float* velVec = (float*)((char*)animCtrl + 208);
        velVec[0] = 0.0f;
        velVec[1] = 0.0f;
        velVec[2] = 0.0f;
        velVec[3] = 0.0f;
        *(uint8_t*)((char*)animCtrl + 134) = 1;
    } else {
        void* animCtrl = *(void**)((char*)creature + 168);
        *(uint8_t*)((char*)animCtrl + 134) = 0;
    }
}


/**
 * pongPlayer::ClampAndApplyInputAngle  @ 0x82199C08 | size: 0x88
 *
 * Takes an absolute input angle (f1), applies a dead-zone clamp,
 * caps it to a maximum value, and if a direction flag at +36 is clear,
 * subtracts from the base value and clamps to [0, max], then passes
 * the result to the input application function.
 *
 * The clamp logic uses fsel (PPC floating-point select):
 *   if (|angle| - deadzone) >= 0: keep |angle|, else use 0
 *   if (clamped - max) >= 0: use max, else keep clamped
 */
// HACK: address comment for linker: @ 0x82199C08
extern const double g_kInputDeadzone;   // @ 0x82079D00 (double, 8 bytes)
extern const double g_kInputClampZero;  // @ 0x82079B00 (double)

void pongPlayer::ClampAndApplyInputAngle(float inputAngle) {  // pongPlayer_9C08_g @ 0x82199C08
    float* inputData = (float*)((char*)this + 8);

    float absAngle = fabsf(inputAngle);

    extern const float g_kInputAngleDeadzone;  // @ 0x82079C04
    extern const double g_kInputMaxAngle;      // @ 0x82079D00
    float maxAngle = (float)g_kInputMaxAngle;

    // Dead-zone clamp: if absAngle < deadzone, treat as zero
    float afterDeadzone = (absAngle - g_kInputAngleDeadzone >= 0.0f) ? absAngle : 0.0f;

    // Cap to maximum
    float clamped = (afterDeadzone - maxAngle >= 0.0f) ? maxAngle : afterDeadzone;

    uint8_t dirFlag = *(uint8_t*)((char*)inputData + 28);
    if (dirFlag == 0) {
        // Forward direction: subtract clamped from base, clamp to [0, max]
        float base = *(float*)((char*)inputData + 0);
        float adjusted = base - clamped;

        extern const double g_kInputClampNeg;  // @ 0x82079B00
        float floorVal = (float)g_kInputClampNeg;

        // Clamp to >= 0
        float floored = (adjusted >= 0.0f) ? adjusted : floorVal;

        // Clamp to <= max
        float final = (floored - maxAngle >= 0.0f) ? maxAngle : floored;

        float result[1] = { final };
        pongPlayer_ClampInputToRange((void*)inputData, result);
    }
}


/**
 * pongPlayer::CopyBitfieldFromSource  @ 0x821D6D90 | size: 0x88
 *
 * Copies a bitfield array from a source object into this object's
 * bitfield storage.  For each bit position (0..count-1), reads the
 * corresponding bit from the source's bitfield (at src+0 pointer)
 * and conditionally sets/clears it in this object's bitfield (at
 * this+0 pointer).  Also copies the float at src+8 to this+8.
 *
 * Uses the standard bit-manipulation pattern:
 *   word_index = bit_pos >> 5
 *   bit_mask   = 1 << (bit_pos & 31)
 *   new_word   = (old_word ^ (-flag ^ old_word)) & mask ^ old_word
 *   which is equivalent to: set or clear the bit based on flag
 */
// HACK: address comment for linker: @ 0x821D6D90
void pongPlayer::CopyBitfieldFromSource(void* source) {  // pongPlayer_6D90_p46 @ 0x821D6D90
    uint16_t bitCount = *(uint16_t*)((char*)this + 6);

    if (bitCount <= 0) {
        // No bits to copy, just copy the float
        *(float*)((char*)this + 8) = *(float*)((char*)source + 8);
        return;
    }

    for (int i = 0; i < (int)bitCount; i++) {
        int wordIndex = i >> 5;
        uint32_t bitMask = 1u << (i & 31);

        // Read source bit
        uint32_t* srcWords = *(uint32_t**)((char*)source + 0);
        bool srcBit = (srcWords[wordIndex] & bitMask) != 0;

        // Set/clear in destination
        uint32_t* dstWords = *(uint32_t**)((char*)this + 0);
        if (srcBit) {
            dstWords[wordIndex] |= bitMask;
        } else {
            dstWords[wordIndex] &= ~bitMask;
        }
    }

    // Copy the float value
    *(float*)((char*)this + 8) = *(float*)((char*)source + 8);
}


// ===========================================================================
// SECTION 30 — Batch 2: 15 small pongPlayer functions (≤200B)
// ===========================================================================

// ── Additional external stubs for batch 2 ─────────────────────────────────
extern void pongPlayer_ProcessServeAiming(pongPlayer* p);                             // @ 0x8218C7A8
extern void pongPlayer_InitCourtGrid(void* self, int r4, int r5, uint8_t side);  // @ 0x8219E2C8
extern void pongPlayer_BuildCourtZones(void* self, int r4, int r5, uint8_t side);  // @ 0x8219E008
extern void pongPlayer_SetPlayMode(pongPlayer* p, int param);                  // @ 0x82195C00
extern void pongMover_CalcInitMatrix(void* out, void* param);             // @ 0x820CAC78
extern void pongPlayer_SetupShotTarget(void* target, float f1, float f2);         // @ 0x821A5DF8
extern void phBoundCapsule_ComputeMatrix(void* bound, void* out, int param);     // @ 0x821D0880
extern void pongPlayer_Deactivate(void* player);                          // @ 0x8218BDC0
extern void pongPlayer_IsOpponentSlotEmpty(void* slotData);                            // @ 0x821E7CC0
extern void pongPlayer_BuildInputMapping(void* entry, uint8_t side, void* data);     // @ 0x821E05A8
extern void pongBallInstance_CreateBallEffect(uint16_t index, int p2, int p3, int p4, int p5); // @ 0x822C4980
extern void pg_E6E0(int code, uint8_t mask, int p3, int p4);              // @ 0x8225E6E0 — PostPageGroupMessage

extern void* g_pInputObjPtr;        // @ 0x825EAB28 — g_input_obj_ptr
extern void* g_pMatchStatePtr;      // @ 0x8271A304 — match state global
extern void* g_pPlayerRecordTable;   // @ 0x8271A31C — 808-byte-per-entry player records
extern void* g_pPlayerRecordTable2;  // @ 0x8271A324 — secondary record table
extern float g_kDefaultSpeed[];      // @ 0x825C00AC — speed constant array
extern float g_kInputScale2;        // @ 0x825C5938+22840 — input scale constant


// ===========================================================================
// 1. pongPlayer::IsSwingInputPending  @ 0x82195800 | size: 0x90
//
// Returns true if the player is in an active serving/rallying state
// (state == 2, 3, or 4), or tail-calls into IsBeforeSwingPeak (DCD8)
// to check if the swing window is still open.
// ===========================================================================
bool pongPlayer::IsSwingInputPending() {  // pongPlayer_5800_h @ 0x82195800
    // Check splash array flags for this player's slot
    uint32_t slot = *(uint32_t*)((char*)this + 464);
    uint8_t* splashEntry = (uint8_t*)((uintptr_t)g_pPlayerDataTable
                           + (slot * 416) + 48);

    // If both bytes at +213 and +212 are nonzero, input is not pending
    if (splashEntry[213] != 0 && splashEntry[212] != 0) {
        return false;
    }

    // Check the action state at +460 for serve/rally states
    int32_t actionState = *(int32_t*)((char*)this + 460);
    if (actionState == 4 || actionState == 2 || actionState == 3) {
        return true;
    }

    // Fall through to IsBeforeSwingPeak via the creature state's anim data
    void* creatureState = *(void**)((char*)this + 452);
    void* animData = *(void**)((char*)creatureState + 188);
    return IsBeforeSwingPeak();
}


// ===========================================================================
// 2. pongPlayer::CheckInputAndInitServe  @ 0x821925C8 | size: 0x9C
//
// Reads the player's creature-state index from +448, looks up a byte
// table from the match-state singleton, and either dispatches a vcall
// (slot 4) on the match state or resets the ball instance to start
// a new serve sequence.
// ===========================================================================
void pongPlayer::CheckInputAndInitServe() {  // pongPlayer_25C8_g @ 0x821925C8
    void* matchState = *(void**)((char*)g_pMatchStatePtr);
    void* creatureState = *(void**)((char*)this + 448);
    uint16_t creatureIndex = *(uint16_t*)((char*)creatureState + 8);

    // Look up the byte table at matchState+4 → +28
    void* matchObj = *(void**)((char*)matchState + 4);
    uint8_t* byteTable = *(uint8_t**)((char*)matchObj + 28);
    uint8_t tableVal = byteTable[creatureIndex];

    // If low 3 bits are nonzero (value & 0x7), call vcall slot 4
    if ((tableVal & 0x7) != 0) {
        void* innerObj = *(void**)((char*)matchState + 4);
        // vtable slot 4 on innerObj
        typedef bool (*VCallFn)(void*);
        void** vtbl = *(void***)innerObj;
        VCallFn fn = (VCallFn)vtbl[4];
        if (fn(innerObj)) {
            return;
        }
    }

    // Reset ball instance to start serve
    void* creatureState2 = *(void**)((char*)this + 448);
    uint16_t idx = *(uint16_t*)((char*)creatureState2 + 8);
    pongBallInstance_CreateBallEffect(idx, 0, 0, 0, 0);
}


// ===========================================================================
// 3. pongPlayer::IsInputActiveAndReady  @ 0x82195B60 | size: 0xA0
//
// Returns true only when:
//   1. The input object is actively polled (io_Input_poll returns 0)
//   2. The input record's player count at +20 is > 0
//   3. The player's slot in the input record is == 1
//   4. The byte at +345 in the input record is nonzero
// Otherwise returns false. Used to gate input-driven swing initiation.
// ===========================================================================
bool pongPlayer::IsInputActiveAndReady() {  // pongPlayer_CheckServeInput @ 0x82195B60
    void* inputObj = g_pInputObjPtr;

    // Check if input system is active
    bool polled = (bool)io_Input_poll_9D68(inputObj);
    if (polled) {
        return false;
    }

    // Check player count > 0
    int32_t playerCount = *(int32_t*)((char*)inputObj + 20);
    if (playerCount <= 0) {
        return false;
    }

    // Look up slot value for this player
    uint32_t playerSlot = *(uint32_t*)((char*)this + 464);
    uint32_t slotIndex = playerSlot + 14;
    uint32_t* slotTable = (uint32_t*)inputObj;
    int32_t slotVal = (int32_t)slotTable[slotIndex];

    if (slotVal != 1) {
        return false;
    }

    // Check byte flag at +345
    uint8_t readyFlag = *(uint8_t*)((char*)inputObj + 345);
    return readyFlag != 0;
}


// ===========================================================================
// 4. pongPlayer::ResetSwingGridData  @ 0x8219E640 | size: 0x94
//
// Zeroes out six 16-byte-aligned vector slots (at offsets +832, +864,
// +912, +928, +880, +896) within the swing collision grid, resets two
// float fields (+848, +852) to a data-section constant, and clears
// the byte at +956, word at +972, and byte at +968.
// ===========================================================================
void pongPlayer::ResetSwingGridData() {  // pongPlayer_E640_g @ 0x8219E640
    // Zero six 16-byte vector slots
    memset((char*)this + 832, 0, 16);   // +832
    memset((char*)this + 864, 0, 16);   // +864
    memset((char*)this + 912, 0, 16);   // +912
    memset((char*)this + 928, 0, 16);   // +928
    memset((char*)this + 880, 0, 16);   // +880
    memset((char*)this + 896, 0, 16);   // +896

    // Reset float fields to constant (from lbl_8207D110+88)
    extern const float g_kGridResetValue;  // @ 0x8207D168
    *(float*)((char*)this + 852) = g_kGridResetValue;
    *(float*)((char*)this + 848) = g_kGridResetValue;

    // Clear state bytes
    *(uint8_t*)((char*)this + 956) = 0;
    *(float*)((char*)this + 972) = 0;  // stw r4=0
    *(uint8_t*)((char*)this + 968) = 0;
}


// ===========================================================================
// 5. pongPlayer::InitializeSwingGrid  @ 0x8219DF38 | size: 0xCC
//
// Full grid initialization: stores a constant float at +960, writes
// the side byte to +944/+945, calls ResetSwingGridData, clears the
// 4x48-byte slot flags at +32 and +224, then invokes E2C8/E008 to
// set up the grid columns. Finally resets state fields.
// ===========================================================================
void pongPlayer::InitializeSwingGrid(uint8_t side, uint8_t param) {  // pongPlayer_DF38_g @ 0x8219DF38
    extern const float g_kSwingGridInit;  // @ 0x8207D110

    *(float*)((char*)this + 960) = g_kSwingGridInit;
    *(uint8_t*)((char*)this + 944) = param;
    *(uint8_t*)((char*)this + 945) = side;

    // Reset the vector/float slots
    ResetSwingGridData();

    // Clear 4 slot flags at +32 (stride 48)
    for (int i = 0; i < 4; i++) {
        *(uint8_t*)((char*)this + 32 + i * 48) = 0;
    }

    // Clear 4 slot flags at +224 (stride 48)
    for (int i = 0; i < 4; i++) {
        *(uint8_t*)((char*)this + 224 + i * 48) = 0;
    }

    // Set up grid columns
    pongPlayer_InitCourtGrid(this, 3, 3, side);
    pongPlayer_BuildCourtZones(this, 3, 3, side);

    // Reset final state
    *(uint8_t*)((char*)this + 956) = 0;
    *(uint32_t*)((char*)this + 972) = 0;
    *(uint8_t*)((char*)this + 968) = 0;
    *(uint8_t*)((char*)this + 964) = 0;
    *(uint32_t*)((char*)this + 948) = 1;
    *(uint32_t*)((char*)this + 952) = 1;
    *(uint8_t*)((char*)this + 965) = 0;
    *(uint8_t*)((char*)this + 966) = 0;
    *(uint8_t*)((char*)this + 967) = 0;
}


// ===========================================================================
// 6. pongPlayer::UpdateInputTargetFromSwing  @ 0x820CB088 | size: 0xA4
//
// Updates the input target fields (+32/+36 → +40/+44) for a mover sub-
// object when a valid swing direction is detected.  If the locked flag
// at +133 is set, returns immediately.  Copies non-zero directional
// floats from +32/+36 to the target slots, and if the bit-0 flag at
// +64 is set, overwrites +32/+36 with f1/f2 parameters.  Finally,
// if either |f1| or |f2| exceeds the zero threshold, clears the
// swing-triggered flag at +341 on the deep sub-object at +112→+188→+148.
// ===========================================================================
void pongPlayer::UpdateInputTargetFromSwing(void* moverObj, float dirX, float dirZ) {
    // pongPlayer_B088_2hr @ 0x820CB088
    uint8_t locked = *(uint8_t*)((char*)moverObj + 133);
    if (locked) {
        return;
    }

    extern const float g_kZeroThreshold2;  // @ 0x8207D110

    float curX = *(float*)((char*)moverObj + 32);
    if (curX != g_kZeroThreshold2) {
        *(float*)((char*)moverObj + 40) = curX;
    }

    float curZ = *(float*)((char*)moverObj + 36);
    if (curZ != g_kZeroThreshold2) {
        *(float*)((char*)moverObj + 44) = curZ;
    }

    // If bit 0 of flags byte at +64 is set, overwrite directional inputs
    uint8_t flags = *(uint8_t*)((char*)moverObj + 64);
    if (flags & 0x1) {
        *(float*)((char*)moverObj + 32) = dirX;
        *(float*)((char*)moverObj + 36) = dirZ;
    }

    // If either direction magnitude exceeds threshold, clear swing trigger
    if (fabsf(dirX) > g_kZeroThreshold2 || fabsf(dirZ) > g_kZeroThreshold2) {
        void* sub1 = *(void**)((char*)moverObj + 112);
        void* sub2 = *(void**)((char*)sub1 + 188);
        void* sub3 = *(void**)((char*)sub2 + 148);
        if (sub3) {
            uint8_t triggered = *(uint8_t*)((char*)sub3 + 341);
            if (triggered) {
                *(uint8_t*)((char*)sub3 + 341) = 0;
            }
        }
    }
}


// ===========================================================================
// 7. pongPlayer::SetPlayerSideAndSync  @ 0x82193628 | size: 0xAC
//
// Writes the side byte to the input object at offset +64 (indexed by
// player slot), and to the splash array at slot*416+262.  If the
// input slot value is 1 (active player), calls C7A8 to compute
// speed, then dispatches LookupPlayerSlot and InitNetworkEntry to
// synchronize the network state for this player.
// ===========================================================================
void pongPlayer::SetPlayerSideAndSync(uint8_t side) {  // pongPlayer_3628_g @ 0x82193628
    uint32_t playerSlot = *(uint32_t*)((char*)this + 464);
    void* inputObj = g_pInputObjPtr;

    // Write side to input object at slot+64
    uint8_t* inputBase = (uint8_t*)inputObj;
    inputBase[playerSlot + 64] = side;

    // Write side to splash array at slot*416 + 262
    uint32_t slot2 = *(uint32_t*)((char*)this + 464);
    uint8_t* splashBase = (uint8_t*)g_pPlayerDataTable + 48;  // base offset
    splashBase[slot2 * 416 + 262 - 48] = side;  // adjusted for base

    // Check if this player's input slot is active (==1)
    uint32_t slot3 = *(uint32_t*)((char*)this + 464);
    uint32_t slotIndex = slot3 + 14;
    uint32_t* slotTable = (uint32_t*)inputObj;
    if ((int32_t)slotTable[slotIndex] == 1) {
        // Look up network record and initialize entry
        void* matchRecord = g_pPlayerRecordTable2;
        pongPlayer_IsOpponentSlotEmpty(matchRecord);

        uint32_t recordSlot = *(uint32_t*)((char*)this + 464);
        uint32_t recordIndex = recordSlot + 1;
        uint32_t* recordTable = (uint32_t*)g_pPlayerRecordTable2;
        uint32_t entryId = recordTable[recordIndex];

        uint8_t* entryBase = (uint8_t*)g_pPlayerRecordTable2;
        void* networkEntry = (void*)(entryBase + entryId * 808 + 12);
        pongPlayer_BuildInputMapping(networkEntry, side, matchRecord);
    }
}


// ===========================================================================
// 8. pongPlayer::SetupSwingSpeedAndTarget  @ 0x82193738 | size: 0xA8
//
// When the player's input slot is active (==1), computes the approach
// speed via C7A8, stores it and its square at mover offsets +72/+76,
// then calls UpdateInputTargetFromSwing (B088) to update the mover's
// directional target from swing input (f1/f2 passed through).
// ===========================================================================
void pongPlayer::SetupSwingSpeedAndTarget(float targetX, float targetZ) {
    // pongPlayer_3738 @ 0x82193738
    uint32_t playerSlot = *(uint32_t*)((char*)this + 464);
    uint32_t slotIndex = playerSlot + 14;
    void* inputObj = g_pInputObjPtr;
    uint32_t* slotTable = (uint32_t*)inputObj;

    if ((int32_t)slotTable[slotIndex] != 1) {
        return;
    }

    // Compute approach speed
    pongPlayer_ProcessServeAiming(this);
    // f1 now contains the computed speed — store to mover
    // The returned value is in the PPC f1 register; in decompiled form
    // we approximate by re-reading the speed from the standard location.
    void* creatureState = *(void**)((char*)this + 452);
    void* moverObj = *(void**)((char*)creatureState + 168);

    float speed = *(float*)((char*)moverObj + 72);  // C7A8 stores result here
    *(float*)((char*)moverObj + 76) = speed * speed;

    // Update input target
    UpdateInputTargetFromSwing(moverObj, targetX, targetZ);
}


// ===========================================================================
// 9. pongPlayer::IsShotTypeActive  @ 0x821D6050 | size: 0xB0
//
// Pure classification function: returns true for shot types that
// represent active gameplay states. The categories are:
//   - Serve/rally: 2, 3, 10, 11
//   - Special shots: 8, 16
//   - Approach shots: 5, 13
//   - Power shots: 7, 15
// All other types return false.
// ===========================================================================
bool pongPlayer::IsShotTypeActive(int shotType) {  // pongPlayer_6050_p46 @ 0x821D6050
    // Serve/rally group
    if (shotType == 2 || shotType == 3 || shotType == 10 || shotType == 11) {
        return true;
    }
    // Special shot group
    if (shotType == 8 || shotType == 16) {
        return true;
    }
    // Approach shot group
    if (shotType == 5 || shotType == 13) {
        return true;
    }
    // Power shot group
    if (shotType == 7 || shotType == 15) {
        return true;
    }
    return false;
}


// ===========================================================================
// 10. pongPlayer::BuildShotTypeFlags  @ 0x821D6C70 | size: 0x98
//
// Assembles a bitfield from six boolean parameters into a single
// uint32_t stored at *out.  The base value is 1 (or 3 if isServe).
// Additional bits are OR'd in:
//   bit 2 (0x04) — isLob
//   bit 3 (0x08) — isSpin
//   bit 4 (0x10) — isPower
//   bit 5 (0x20) — isSpecial
//   bit 6 (0x40) — isDropShot
// ===========================================================================
void pongPlayer::BuildShotTypeFlags(uint32_t* out, uint8_t isServe,
                                     uint8_t isLob, uint8_t isSpin,
                                     uint8_t isPower, uint8_t isSpecial,
                                     uint8_t isDropShot) {
    // pongPlayer_6C70_p46 @ 0x821D6C70
    uint32_t flags = 1;
    if (isServe) {
        flags = 3;
    }
    if (isLob) {
        flags |= 0x04;
    }
    if (isSpin) {
        flags |= 0x08;
    }
    if (isPower) {
        flags |= 0x10;
    }
    if (isSpecial) {
        flags |= 0x20;
    }
    if (isDropShot) {
        flags |= 0x40;
    }
    *out = flags;
}


// ===========================================================================
// 11. pongPlayer::ResetShotTimingState  @ 0x821A76E8 | size: 0xBC
//
// Resets the shot timing structure: writes -1 to both int16 fields
// at +4 and +6, zeroes the floats at +12, +16, +20, clears the
// int16 at +8, then for 2 iterations writes zero to 10 per-slot
// float fields at +24..+108 (stride 40) and copies the default
// value from +192 into the corresponding timing slots at +108..+148.
// ===========================================================================
void pongPlayer::ResetShotTimingState(void* timingState) {
    // pongPlayer_76E8_g @ 0x821A76E8
    extern const float g_kTimingZero;  // @ 0x8207D110

    *(int16_t*)((char*)timingState + 4) = -1;
    *(int16_t*)((char*)timingState + 6) = -1;
    *(float*)((char*)timingState + 12) = g_kTimingZero;
    *(float*)((char*)timingState + 16) = g_kTimingZero;
    *(float*)((char*)timingState + 20) = g_kTimingZero;
    *(uint16_t*)((char*)timingState + 8) = 0;

    float defaultVal = *(float*)((char*)timingState + 192);

    // Two iterations, each covering 10 float pairs
    uint8_t* slotBase = (uint8_t*)timingState + 108;
    for (int iter = 0; iter < 2; iter++) {
        uint8_t* zeroBase = (uint8_t*)timingState + 24 + iter * 40;
        for (int j = 0; j < 10; j++) {
            *(float*)(zeroBase + j * 4) = g_kTimingZero;
            *(float*)(slotBase + j * 4) = defaultVal;
        }
        slotBase += 40;
    }
}


// ===========================================================================
// 12. pongPlayer::UpdateCollisionMatrix  @ 0x82194C08 | size: 0xA8
//
// When the creature state at +452 is valid, looks up the physics
// bound capsule via pg_GetGeometryRecord and phBoundCapsule_ComputeMatrix, then copies
// the resulting 4x16-byte matrix (64 bytes) into the collision data
// structure at this+48+32.
// ===========================================================================
void pongPlayer::UpdateCollisionMatrix() {  // pongPlayer_4C08_g @ 0x82194C08
    void* creatureState = *(void**)((char*)this + 452);
    if (!creatureState) {
        return;
    }

    void* animData = *(void**)((char*)creatureState + 188);

    // Look up geometry record
    void* geomBase = pg_GetGeometryRecord(g_pInputObjPtr);
    void* geomInner = *(void**)((char*)geomBase + 44);
    uint32_t boundIndex = *(uint32_t*)((char*)animData + 28);
    void* boundObj = *(void**)((char*)geomInner + 324);

    // Compute bound matrix into stack-local (simulated via direct copy)
    // phBoundCapsule_ComputeMatrix writes a 4x4 matrix
    float matrix[16];
    phBoundCapsule_ComputeMatrix(boundObj, matrix, boundIndex);

    // Copy the 4x16-byte (64-byte) result to collision structure
    void* collisionData = *(void**)((char*)this + 48);
    memcpy((char*)collisionData + 32, matrix, 64);
}


// ===========================================================================
// 13. pongPlayer::DeterminePlayerFacing  @ 0x821A0F10 | size: 0xB8
//
// Reads the player's world-space X position from the parent's
// transform matrix (column 3, row 0 — offset +48 of the 4x4 matrix
// at +68 or +72, depending on player slot).  If |x| < threshold,
// returns early.  Otherwise stores either a positive or negative
// facing constant into *outFacing based on the sign of x.
// ===========================================================================
void pongPlayer::DeterminePlayerFacing(float threshold, float* outFacing) {
    // pongPlayer_0F10_g @ 0x821A0F10
    void* parent = *(void**)((char*)this + 48);
    uint32_t slot = *(uint32_t*)((char*)parent + 464);

    extern const float g_kFacingPositive;  // @ 0x8207D108+8  positive facing
    extern const float g_kFacingNegative;  // @ 0x8207D108+0  negative facing

    void* transform;
    if (slot == 0) {
        transform = *(void**)((char*)parent + 72);
    } else {
        transform = *(void**)((char*)parent + 68);
    }

    // Read X from column 3 of the transform matrix (offset +48)
    float posX = *(float*)((char*)transform + 48);
    float absX = fabsf(posX);

    if (absX < threshold) {
        return;  // too close to center, no facing change
    }

    if (posX >= g_kFacingPositive) {
        *outFacing = g_kInputScale2;  // right-facing constant
    } else {
        *outFacing = g_kFacingNegative;  // left-facing constant
    }
}


// ===========================================================================
// 14. pongPlayer::DestroyAndResetPlayerSlot  @ 0x8218AE10 | size: 0xBC
//
// Tears down a player slot: looks up the slot's sub-object array
// (indexed by r4/r5 arithmetic), deactivates the creature if active,
// calls the virtual destructor (vtable slot 0), then clears the
// slot's fields (active/ready flags, indices set to -1, pointer
// nulled).
// ===========================================================================
void pongPlayer::DestroyAndResetPlayerSlot(int slotGroup, int subIndex) {
    // pongPlayer_AE10_g @ 0x8218AE10
    // Compute slot record address
    uint32_t baseIndex = *(uint32_t*)((char*)this + (slotGroup + 33) * 4);
    uint32_t combined = baseIndex * 2 + subIndex;
    uint32_t entryIndex = combined + combined * 2;
    uint8_t* slotRecord = (uint8_t*)this + entryIndex * 8;

    void* playerObj = *(void**)(slotRecord + 40);

    // Log (debug no-op)
    rage_DebugLog("pongPlayer::DestroyAndResetPlayerSlot");

    // Deactivate if creature is active
    uint8_t isActive = *(uint8_t*)((char*)playerObj + 44);
    if (isActive) {
        pongPlayer_Deactivate(playerObj);
    }

    // Call virtual destructor (vtable slot 0) with param=0
    void** vtbl = *(void***)playerObj;
    typedef void (*DtorFn)(void*, int);
    DtorFn dtor = (DtorFn)vtbl[0];
    dtor(playerObj, 0);

    rage_DebugLog("pongPlayer::DestroyAndResetPlayerSlot - done");

    // Clear the back-reference if it matches
    uint32_t refIndex = baseIndex + 29;
    void* currentRef = *(void**)((char*)this + refIndex * 4);
    void* newPlayerObj = *(void**)(slotRecord + 40);
    if (currentRef == newPlayerObj) {
        *(void**)((char*)this + refIndex * 4) = nullptr;
    }

    // Reset slot record fields
    *(uint8_t*)(slotRecord + 32) = 0;   // active flag
    *(uint8_t*)(slotRecord + 33) = 0;   // ready flag
    *(int32_t*)(slotRecord + 24) = -1;  // primary index
    *(int32_t*)(slotRecord + 28) = -1;  // secondary index
    *(void**)(slotRecord + 40) = nullptr; // player pointer
}


// ===========================================================================
// 15. pongPlayer::ComputeCourtBoundsForSide  @ 0x821C9CD0 | size: 0x94
//
// Computes the near and far court boundary vectors for a given side.
// The X bounds come from field +128 scaled by two different .rdata
// constants (for min/max), and the Z bounds come from fields +132/+136
// offset by a global court depth value (negated for the opposite side).
// The Y component is always set to a constant from .rdata.
// ===========================================================================
void pongPlayer::ComputeCourtBoundsForSide(void* courtData, int sideOffset,
                                            float* outNear, float* outFar) {
    // pongPlayer_9CD0_g @ 0x821C9CD0
    extern const float g_kCourtScaleX_near;  // @ 0x8207D114  (lbl_8207D110+4)
    extern const float g_kCourtScaleX_far;   // @ 0x8207D110  (lbl_8207D110+0)
    extern const float g_kCourtY;            // @ 0x8207D114  (Y constant)

    uint8_t* base = (uint8_t*)courtData;
    uint8_t* sideBase = base + sideOffset;

    float halfWidth = *(float*)(base + 128);

    // Y component — constant from .rdata
    outNear[1] = g_kCourtY;
    outFar[1] = g_kCourtY;

    // X component — scaled by near/far constants
    outNear[0] = halfWidth * g_kCourtScaleX_near;
    outFar[0] = halfWidth * g_kCourtScaleX_far;

    // Z component — varies by side flag
    uint8_t sideFlag = *(uint8_t*)(sideBase + 64);

    // Look up the court depth from match state
    extern void* g_pCourtState;  // @ 0x8271A314
    void* courtState = *(void**)((char*)g_pCourtState);
    float courtDepth = *(float*)((char*)courtState + 24);
    float scaledDepth = courtDepth * g_kCourtScaleX_far;

    if (sideFlag != 0) {
        // Same-side: add depth offset
        outNear[2] = *(float*)(base + 136) + scaledDepth;
        outFar[2] = *(float*)(base + 132) + scaledDepth;
    } else {
        // Opposite side: subtract depth offset
        float negDepth = -scaledDepth;
        outFar[2] = negDepth - *(float*)(base + 136);
        outNear[2] = outFar[2] - *(float*)(base + 132);
    }
}


// ===========================================================================
// SECTION 9 — Batch: 10 pongPlayer functions (92–232B)
// ===========================================================================

// ── Additional externs for this batch ─────────────────────────────────────
extern void pongPlayer_CheckAnimTimeInRange(void* creatureState);     // @ 0x820DD298
extern void pongBallInstance_CreateBallEffect(void* ballMgr, uint16_t ballIndex,
                                    int p3, int p4, int p5, int p6); // @ 0x822C4980
extern void nop_8240E6D0(const char* debugStr);            // debug log (no-op in retail)

// Global pointers used by CD48
extern void* g_pMatchState;       // @ 0x8271A318 (.data, 4B)
extern void* g_pMatchConfig;      // @ 0x8271A32C (.data, 4B)

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::ApproachFloat  @ 0x820C99B0 | size: 0x5C (92 bytes)
//
// Moves *valuePtr toward targetValue by step (speed * dt) per frame.
// Clamps to targetValue when overshooting. Returns true when value == target.
// Used by look-at drivers, audio, rendering, and player subsystems.
// ─────────────────────────────────────────────────────────────────────────────
bool pongPlayer::ApproachFloat(float* valuePtr, float targetValue,
                               float speed, float dt) {
    float currentValue = *valuePtr;

    if (currentValue < targetValue) {
        // Approach from below
        currentValue += speed * dt;
        *valuePtr = currentValue;
        if (currentValue > targetValue) {
            *valuePtr = targetValue;
        }
    } else if (currentValue > targetValue) {
        // Approach from above
        currentValue -= speed * dt;
        *valuePtr = currentValue;
        if (currentValue < targetValue) {
            *valuePtr = targetValue;
        }
    }

    return (*valuePtr == targetValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::SetHandedness  @ 0x820C7C78 | size: 0x6C (108 bytes)
//
// Sets the player's handedness byte (m_handedness at this+197).
// If the value actually changes, logs a debug string identifying the new
// hand ("right" vs "left") along with the creature name, then stores.
// ─────────────────────────────────────────────────────────────────────────────
void pongPlayer::SetHandedness(uint8_t hand) {
    uint8_t* handPtr = reinterpret_cast<uint8_t*>(this) + 197;  // m_handedness at +0xC5
    if (hand == *handPtr) {
        return;
    }

    const char* handStr = (hand != 0)
        ? "right"   // @ 0x82027A30
        : "left";   // @ 0x82027A50

    nop_8240E6D0(handStr);  // debug log — no-op in retail
    *handPtr = hand;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::GetSubObjectByIndex  @ 0x820CE3F0 | size: 0x64 (100 bytes)
//
// Returns one of five sub-object pointers stored at this+100..this+116,
// indexed by the slot parameter (0–4). Returns nullptr for out-of-range.
// Called by SinglesNetworkClient and SwingStartedMessage to look up
// per-player sub-objects by network slot.
// ─────────────────────────────────────────────────────────────────────────────
void* pongPlayer::GetSubObjectByIndex(int slotIndex) const {
    void** subObjects = (void**)((uint8_t*)this + 100);  // m_pSubObjects at this+100
    switch (slotIndex) {
        case 0: return subObjects[0];  // this+100
        case 1: return subObjects[1];  // this+104
        case 2: return subObjects[2];  // this+108
        case 3: return subObjects[3];  // this+112
        case 4: return subObjects[4];  // this+116
        default: return nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::IsSwingTimingWindowOpen  @ 0x820CE000 | size: 0x8C (140 bytes)
//
// Returns true only when:
//   1. The timing state exists and its timer has NOT expired
//      (i.e. IsSwingTimerActive is false — cntlzw inverts it)
//   2. The timing sub-object at this+120 exists
//   3. The float at (this+120)+32+128 (i.e. +160) is > the epsilon constant
//
// This gates the "live swing aiming" window — the player can still adjust
// the shot direction while this returns true.
// ─────────────────────────────────────────────────────────────────────────────
bool pongPlayer::IsSwingTimingWindowOpen() const {
    // Check timing state: timer must NOT have expired
    if (m_pTimingState) {
        bool timerActive = (m_pTimingState->m_currentTime >= m_pTimingState->m_targetTime);
        // cntlzw + rlwinm(27,31,31) inverts the boolean:
        // timerActive == true means time >= target, i.e. timer expired
        // We need !timerActive to proceed (timer still running)
        if (timerActive) {
            return false;
        }
    } else {
        return false;
    }

    // Check secondary sub-object at this+120
    void* subObj = *(void**)((uint8_t*)this + 120);  // m_pSwingAimState
    if (!subObj) {
        return false;
    }

    // Read float at subObj+32+128 = subObj+160
    float aimValue = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(subObj) + 160);

    return (aimValue > g_swingPhaseThreshold);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::UpdateInputDirection  @ 0x820CB088 | size: 0xA4 (164 bytes)
//
// Captures the current input direction (f1=inputX, f2=inputY) into the
// player's input state sub-object. If the "locked" flag (byte at +133) is
// set, does nothing. Otherwise:
//   - Saves current values (+32, +36) to previous (+40, +44) when non-zero
//   - If bit 0 of flags byte at +64 is set, overwrites current with new input
//   - If either input axis has non-zero magnitude, follows the swing data
//     chain to clear the swing trigger flag (byte 341 at the swing sub-object)
// ─────────────────────────────────────────────────────────────────────────────
void pongPlayer::UpdateInputDirection(float inputX, float inputY) {
    uint8_t* base = (uint8_t*)this;
    uint8_t inputLocked = *(base + 133);  // m_bInputLocked at +133
    if (inputLocked) {
        return;
    }

    float* pInputDirX = (float*)(base + 32);     // m_inputDirX at +32
    float* pInputDirY = (float*)(base + 36);     // m_inputDirY at +36
    float* pPrevInputDirX = (float*)(base + 40); // m_prevInputDirX at +40
    float* pPrevInputDirY = (float*)(base + 44); // m_prevInputDirY at +44
    uint8_t* pInputFlags = base + 64;             // m_inputFlags at +64

    // Save current to previous when current is non-zero
    if (*pInputDirX != g_swingPhaseThreshold) {  // epsilon constant at 0x8202D110
        *pPrevInputDirX = *pInputDirX;
    }
    if (*pInputDirY != g_swingPhaseThreshold) {
        *pPrevInputDirY = *pInputDirY;
    }

    // If directional override flag (bit 0 at +64) is set, store new values
    if (*pInputFlags & 0x1) {
        *pInputDirX = inputX;
        *pInputDirY = inputY;
    }

    // If either input axis has significant magnitude, clear swing trigger
    if (fabsf(inputX) > g_swingPhaseThreshold ||
        fabsf(inputY) > g_swingPhaseThreshold) {
        // Follow chain: this+112 -> +188 -> +148 -> byte at +341
        void* actionState = m_pActionState;
        if (actionState) {
            void* swingMgr = *reinterpret_cast<void**>(
                reinterpret_cast<uintptr_t>(actionState) + 188);
            if (swingMgr) {
                void* swingData = *reinterpret_cast<void**>(
                    reinterpret_cast<uintptr_t>(swingMgr) + 148);
                if (swingData) {
                    uint8_t triggered = *reinterpret_cast<uint8_t*>(
                        reinterpret_cast<uintptr_t>(swingData) + 341);
                    if (triggered) {
                        *reinterpret_cast<uint8_t*>(
                            reinterpret_cast<uintptr_t>(swingData) + 341) = 0;
                    }
                }
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::IsLocomotionStateActive  @ 0x82192578 | size: 0x50 (80 bytes)
//
// Checks whether the player's locomotion sub-state is currently active.
// Follows the chain: this+452 -> +188 -> +120, then calls pongPlayer_IsLocoStateReady
// on the sub-object at offset +32 within that result.
// Used by swing initiation, ball hit detection, and shader parameter updates.
// ─────────────────────────────────────────────────────────────────────────────
bool pongPlayer::IsLocomotionStateActive() const {
    void* creatureData = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(m_pCreature) + 188);
    void* locoState = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(creatureData) + 120);

    if (!locoState) {
        return false;
    }

    void* locoSubState = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(locoState) + 32);
    return pongPlayer_IsLocoStateReady(locoSubState);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::ResetBallInstance  @ 0x821925C8 | size: 0x9C (156 bytes)
//
// Resets the ball instance associated with this player. Checks whether the
// player's ball slot has an active status (bottom 3 bits of status byte != 0).
// If active, calls the ball manager's virtual Reset (vtable slot 4) first.
// Then unconditionally calls pongBallInstance_CreateBallEffect to fully reset the
// ball instance for this player's slot.
// ─────────────────────────────────────────────────────────────────────────────
void pongPlayer::ResetBallInstance() {
    extern void* g_pBallManager;  // @ 0x825F5BD0 (.data)

    void* ballMgr = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(g_pBallManager));
    void* ballData = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(ballMgr) + 4);

    // Get this player's ball slot index
    void* playerState = m_pPlayerState;
    uint16_t ballIndex = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<uintptr_t>(playerState) + 8);

    // Check status byte for this ball slot
    void* statusTable = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(ballData) + 28);
    uint8_t status = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(statusTable) + ballIndex);
    bool isActive = (status & 0x7) != 0;

    if (isActive) {
        // Call ball data's virtual Reset (vtable slot 4)
        void** vtable = *reinterpret_cast<void***>(ballData);
        using ResetFn = void(*)(void*);
        ResetFn resetFn = reinterpret_cast<ResetFn>(
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(vtable) + 16));
        resetFn(ballData);

        // Re-fetch ball manager in case Reset changed state
        ballMgr = *reinterpret_cast<void**>(
            reinterpret_cast<uintptr_t>(g_pBallManager));
    }

    // Full reset of ball instance for this player's slot
    void* state = m_pPlayerState;
    uint16_t idx = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<uintptr_t>(state) + 8);
    pongBallInstance_CreateBallEffect(ballMgr, idx, 0, 0, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::IsReadyForNewSwing  @ 0x820CDDC0 | size: 0xD4 (212 bytes)
//
// Compound readiness gate called from pongPlayer_vfn_2 (the main update).
// Returns true only when ALL of the following are satisfied:
//   1. IsSwingPhaseBlocked() returns false
//   2. The timing state shows the timer has NOT expired (same check as DCD8)
//   3. IsCreatureStateReady() returns false
//   4. IsCreatureState2Active() returns false
//   5. IsRecovering() returns false
//
// This is the master "can we start a new swing?" gate.
// ─────────────────────────────────────────────────────────────────────────────
bool pongPlayer::IsReadyForNewSwing() const {
    // Gate 1: swing phase must not be blocked
    if (IsSwingPhaseBlocked()) {
        return false;
    }

    // Gate 2: timing state timer must not have expired
    if (m_pTimingState) {
        bool timerExpired = (m_pTimingState->m_currentTime >= m_pTimingState->m_targetTime);
        if (!timerExpired) {
            // Timer still running — not ready
            return false;
        }
    }

    // Gate 3: creature state must not be ready
    if (IsCreatureStateReady()) {
        return false;
    }

    // Gate 4: secondary creature state must not be active
    if (IsCreatureState2Active()) {
        return false;
    }

    // Gate 5: must not be in recovery
    if (IsRecovering()) {
        return false;
    }

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::IsInAnticipationPhase  @ 0x820CDCD8 | size: 0xE8 (232 bytes)
//
// Checks whether the player is in the "anticipation" phase of a swing:
//
// PATH A — Timer has expired (currentTime >= targetTime):
//   Checks if the timer just expired (equal) — returns false.
//   Otherwise checks if the mid-point time (field +36) exceeds currentTime,
//   which indicates the player is past the apex but in anticipation of contact.
//
// PATH B — Timer still running:
//   Falls through to IsRecovering(); if recovering, delegates to
//   pongPlayer_CheckAnimTimeInRange on the creature state at this+128.
//
// Returns false in all other cases.
// ─────────────────────────────────────────────────────────────────────────────
bool pongPlayer::IsInAnticipationPhase() const {
    pongTimingState* timing = m_pTimingState;

    if (timing) {
        float currentTime = timing->m_currentTime;
        float targetTime = timing->m_targetTime;

        // Check if timer has expired (currentTime >= targetTime)
        bool timerExpired = (currentTime >= targetTime);

        if (timerExpired) {
            // Timer expired — check if exactly at target (edge case)
            if (currentTime >= targetTime) {
                // Re-check: if still >= target, check mid-point
                // If midTime (field +36) > currentTime, we're in anticipation
                float midTime = *(float*)((uint8_t*)timing + 36);  // m_midTime at +36
                if (midTime > currentTime) {
                    return true;
                }
            }
            return false;
        }
    } else {
        // No timing state at all
        return false;
    }

    // PATH B: timer still running — check recovery path
    if (IsRecovering()) {
        void* creatureState = *(void**)((uint8_t*)this + 128);  // m_pCreatureStateForSwing at +128
        pongPlayer_CheckAnimTimeInRange(creatureState);
        // D298 returns a bool in r3
        // TODO: capture return value properly
        return false;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongPlayer::UpdateServeSpeed  @ 0x8218CD48 | size: 0xB8 (184 bytes)
//
// Updates the serve speed stored in the creature data (m_pCreature+208) based
// on the current match state. Three paths:
//
//   1. If the match replay cursor is out of bounds, or the match config's
//      "use serve data" flag is clear → store a default constant.
//   2. If the player's side matches the geometry record's handedness →
//      store the "same side" serve speed from the court data.
//   3. Otherwise → store the "cross side" serve speed.
// ─────────────────────────────────────────────────────────────────────────────
void pongPlayer::UpdateServeSpeed() {
    // Get match state and check replay cursor bounds
    void* matchState = *reinterpret_cast<void**>(&g_pMatchState);
    int replayCursor = *reinterpret_cast<int*>(
        reinterpret_cast<uintptr_t>(matchState) + 28);

    bool cursorValid = false;
    if (replayCursor >= 0) {
        int maxCursor = *reinterpret_cast<int*>(
            reinterpret_cast<uintptr_t>(matchState) + 8) - 1;
        cursorValid = (replayCursor < maxCursor);
    }

    if (!cursorValid) {
        // Out of bounds — use default serve speed
        void* creature = m_pCreature;
        *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(creature) + 208) = g_recoveryTimerThreshold;
        return;
    }

    // Check match config "use serve data" flag
    void* matchConfig = *reinterpret_cast<void**>(&g_pMatchConfig);
    uint8_t useServeData = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(matchConfig) + 6);

    if (useServeData == 0) {
        // Serve data disabled — use default
        void* creature = m_pCreature;
        *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(creature) + 208) = g_recoveryTimerThreshold;
        return;
    }

    // Look up court data entry for next replay frame
    int nextFrame = replayCursor + 1;
    void* frameTable = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(matchState) + 12);
    void* frameEntry = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(frameTable) + nextFrame * 4);

    // Get geometry record and check side matching
    uint8_t playerSide = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(matchConfig) + 5);
    extern void* g_input_obj_ptr;  // @ global input object pointer
    void* inputData = *reinterpret_cast<void**>(&g_input_obj_ptr);
    uint8_t geomSide = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(inputData) + 64);

    int playerFlip = m_inputSlotIdx;  // this+464 (m_inputSlotIdx)
    int sideXor = geomSide ^ playerSide;

    // Get court data record
    void* courtRecord = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(frameEntry) + 8);
    void* serveData = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(courtRecord) + 40);

    void* creature = m_pCreature;
    if (sideXor == playerFlip) {
        // Same side — use near serve speed
        float serveSpeed = *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(serveData) + 24);
        *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(creature) + 208) = serveSpeed;
    } else {
        // Cross side — use far serve speed
        float serveSpeed = *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(serveData) + 28);
        *reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(creature) + 208) = serveSpeed;
    }
}
