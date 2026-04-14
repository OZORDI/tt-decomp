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
#include <cstring>   // memset
#include <cmath>     // fabsf

// rage memory — declare only what we need to avoid memset redecl conflict
extern void rage_free(void* ptr);

// ---------------------------------------------------------------------------
// External functions referenced below
// ---------------------------------------------------------------------------

// Face animation blender — starts the post-point anim.
// (pcrCreature forward-declared in pong_player.hpp)

// Update function called from D228; purpose: TODO — likely syncs state.
extern void pongPlayer_UpdateLocomotionState(pongPlayer* state);  // @ 0x820CC678

// LocomotionStateAnim helpers — update animation list entries.
// C128 = standard transition, C288 = alternate (flag-gated) transition.
extern void LocomotionStateAnim_BlendLocomotionAnims(uint32_t entry, void* parent, float dt);
extern void LocomotionStateAnim_TransitionLocomotionState(uint32_t entry, void* parent, float dt);


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
extern void pongPlayer_GetAnimNormalizedTime(pongPlayer* state);    // @ 0x820CD238 (or similar)
// extern void pongPlayer_CheckAnimTimeInRange(...);  // TODO: verify signature @ ~0x820CD298

// Logging no-op (debug only)
extern void rage_debugLog(const char* fmt, ...);

// Geometry / position helpers used by D7B0.
extern void* pg_GetGeometryRecord(void* singleton);              // → returns geometry record

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
extern void pongPlayer_ComputeStepDelta(void* obj, int maxSteps, float* outDelta);   // @ 0x821E0508
extern bool pongPlayer_CheckServeInput(pongPlayer* p);                            // @ 0x82195B60
extern void pongPlayer_InitActionState(void* actionState, int released);            // @ 0x821A1460

// ---------------------------------------------------------------------------
// Resolved global data addresses (from binary analysis)
// ---------------------------------------------------------------------------

// .rdata float constants (adjacent 4-byte floats in 0x8202D1xx)
extern const float g_kFloatConst_D108;       // @ 0x8202D108
extern const float g_kFloatConst_D10C;       // @ 0x8202D10C
extern const float g_kFloatConst_D110;       // @ 0x8202D110

// .rdata vector constants (0x8207Dxxx)
extern const float g_kVecConst_7D110[4];     // @ 0x8207D110  (16B)
extern const float g_kVecConst_7D168[3];     // @ 0x8207D168  (12B)

// .rdata scalar constants (0x82079xxx)
extern const double g_kDoubleConst_79D00;    // @ 0x82079D00  (8B)  upper clamp bound
extern const double g_kDoubleConst_79B00;    // @ 0x82079B00  (8B)  lower clamp bound (zero as double)
extern const double g_kDoubleConst_79EC8;    // @ 0x82079EC8  (8B)  select-zero double (0.0)
extern const float  g_kFloatConst_79C04;     // @ 0x82079C04          epsilon/deadzone
extern const float  g_kFloatConst_79CD8;     // @ 0x82079CD8
extern const float  g_kFloatConst_79BAC;     // @ 0x82079BAC
extern const float  g_kFloatConst_79FFC;     // @ 0x82079FFC
extern const float  g_kFloatConst_79FF8;     // @ 0x82079FF8
extern const float  g_kFloatConst_79FF4;     // @ 0x82079FF4

// .data globals — game state pointers
extern void*    g_pGlobalClock;              // @ 0x8271A304
extern void*    g_pCourtState;               // @ 0x8271A314
extern void*    g_pMatchState;               // @ 0x8271A318
extern void*    g_pPlayerRecords;            // @ 0x8271A31C
extern void*    g_pPlayerRecords2;           // @ 0x8271A324
extern void*    g_pMatchConfig;              // @ 0x8271A32C

// .data globals — game objects
extern void**   g_pInputArrayTable;          // @ 0x825EAB28
extern uint8_t  g_mirrorBuffer[32];          // @ 0x82606720
extern uint8_t  g_ballSplashArray[];         // @ 0x8261A3D0  (416-byte-per-entry)

// ── External functions for batch 10 implementations ──────────────────────
extern void  nop_8240E6D0(const char* fmt, ...);                                        // debug no-op @ 0x8240E6D0
extern void  pongPlayer_Deactivate(void* entry);                                         // @ 0x8218BDC0
extern void  pongPlayer_5C00_g(void* playerPtr, int32_t param);                          // @ 0x82195C00
extern bool  pongPlayer_99B0_g(float* valuePtr, float startVal, float ratio, float step);// @ 0x820C99B0
extern float pongPlayer_0508_g(void* obj, int channel, float inputVal);                  // @ 0x821E0508

// ── External globals for batch 10 implementations ────────────────────────
extern void*       g_pCourtGeomSingleton;     // @ 0x8251A304 — court geometry singleton
extern float       g_kCourtHeightOffset;      // @ 0x825C8078 — court height offset constant
extern const float g_kFloatConst_D168;        // @ 0x8202D168 — grid reset float constant
extern float       g_swingSlotGridDefault;    // @ 0x825CAF7C — writable grid default
extern const float g_kPositiveSideSign;       // @ 0x825C5938 — positive court side sign
extern const float g_kTransitionStep;         // @ 0x825CAEC0 — transition blend step size
extern const float g_kSwingTargetConst;       // @ 0x825C80FC — swing target input constant
extern void*       g_pPlayerSlotLookup;       // @ 0x8251A32C — player slot lookup global

// Forward declarations for functions implemented at end of file
void pongPlayer_E640_g(void* gridSubObj);  // ClearSwingSlotGrid @ 0x8219E640
void pongPlayer_3628_g(pongPlayer* p, uint8_t flip);  // SetServeSideFlip @ 0x82193628
void pongPlayer_E590_g(float* lerpFactors, uint8_t flip, float* outPos,
                       float* boundsA, float* boundsB, float* offset);  // InterpolateGridPosition @ 0x8219E590
bool pongPlayer_6050_p46(int32_t swingType);  // IsSwingType @ 0x821D6050
void pongPlayer_0F10_g(void* stateObj, float* outSign, float deadZone);  // GetCourtSideSign @ 0x821A0F10
void pongPlayer_76E8_g(void* swingHistory);  // InitSwingHistorySlots @ 0x821A76E8
void pongPlayer_AE10_g(void* slotsBase, int32_t slotId, int32_t column);  // RemoveSlotEntry @ 0x8218AE10
void pongPlayer_4438_fw(void* transObj);  // UpdateTransitionBlend @ 0x82384438
void pongPlayer_D6C8_fw(void* animTransObj);  // ResetAnimTransitionState @ 0x8239D6C8
void pongPlayer_06A0_g(float* outVec, pongPlayer* player);  // ComputeSwingTargetDelta @ 0x821906A0

// Swing/target helpers
extern void pongPlayer_ComputeTargetPosition(vec3* outVec, void* targetStruct);
extern float fiAsciiTokenizer_ParseFloat(float input);
extern void pongPlayer_ComputeCourtPosition(vec3* outVec, void* targetData);
extern void pongPlayer_GetCourtBounds(void* self, int r4, void* p3, void* p4, int p5, void* p6, void* p7, uint8_t metadataByte);
extern void pongPlayer_InterpolateCourtPoint(void* adjustedVec, void* gridBase, void* p3, void* p4, void* p5);


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
        uint8_t postPointFlag = m_pCreatureState2->m_postPointFlag;  // +0x1A8
        if (postPointFlag) {
            // TODO: Get pcrFaceAnimBlender instance and call StartPostPoint
        }
    }

    // Step 2: sync state.
    pongPlayer_UpdateLocomotionState(this);

    // Step 3: range-check m_animTimer against [g_kZero, g_recoveryTimerThreshold].
    float animTime = m_animTimer;
    bool inRange = (animTime >= g_kZero) && (animTime <= g_recoveryTimerThreshold);

    // Also check bit 0 of flags word via parent pointer.
    bool useAltAnim = false;
    if (m_pParent) {
        uint32_t* parentBase = static_cast<uint32_t*>(m_pParent);
        uint32_t flagWord = parentBase[5];   // parent+20 (flags word)
        useAltAnim = (flagWord & 0x1) != 0;
    }

    // Select update function.
    using AnimUpdateFn = void(*)(uint32_t, void*, float);
    AnimUpdateFn updateFn = (inRange && !useAltAnim)
        ? LocomotionStateAnim_BlendLocomotionAnims
        : LocomotionStateAnim_TransitionLocomotionState;

    // Step 4: iterate animation list.
    // In-range: use m_pGameState (+0x50); out-of-range: use m_pLocoState2 (+0x20).
    // TODO: these offsets suggest the lists are embedded in those sub-objects.
    // Animation list sub-object: +8 = entry array ptr, +12 = entry count
    // +0x50 is m_pGameState per header layout (not a typed field on pongPlayer yet).
    void* listObj = inRange ? *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(this) + 0x50)
                            : m_pLocoState2;
    uint32_t* listWords = static_cast<uint32_t*>(listObj);
    uint32_t* entries   = reinterpret_cast<uint32_t*>(listWords[2]);  // +8
    uint16_t  count     = static_cast<uint16_t>(listWords[3]);        // +12

    for (int i = 0; i < (int)count; ++i) {
        updateFn(entries[i], m_pParent, g_recoveryTimerThreshold);
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
        uint32_t animSubOffset = state->m_pAnimState->m_animSubOffset;  // +0x88

        // Read the 16-byte target vector from the timing state (+0x50).
        vec3 target = timing->m_swingTargetVec;

        // Mirror check: if handedness flags don't match, flip the vector sign.
        // geomRecord+44 → pointer; byte at ptr+260 is the handedness flag.
        // TODO: verify exact pointer arithmetic — see vsubfp128 at 0x820CD868.
        void** geomWords = static_cast<void**>(geomRecord);
        uint8_t* geomSubPtr = static_cast<uint8_t*>(geomWords[11]);  // +44 / 4
        uint8_t geomFlag = geomSubPtr[260];
        uint8_t animFlag = static_cast<uint8_t*>(g_geomSingleton)[animSubOffset + 64];
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
        *out = state->m_pRecoveryState->m_recoveryVec;  // +0x70 (112)
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
            m_pTimingState->m_swingTargetVec.x = kZero;  // +80
            m_pTimingState->m_swingTargetVec.y = kZero;  // +84
            g_swingCountFlag            = 0;           // stw r10,25408(r9) @ lis(-32160)
            m_pTimingState->m_swingTargetVec.z = kZero;  // +88
            m_pTimingState->m_bComplete = 0;           // byte +141
            rage_debugLog("pongPlayer::CancelSwing() timing reset", (uintptr_t)m_pCreature);
        }
    }

    // ── PATH B: flush recovery callbacks ────────────────────────────────
    if (IsRecovering())
    {
        game_CD20(m_pRecoveryState);     // flush recovery state @ 0x820DCD20
        rage_debugLog("pongPlayer::CancelSwing() recovery flush", (uintptr_t)m_pCreature);
    }

    // ── PATH C: reset anim phase-blocked state ───────────────────────────
    if (IsSwingPhaseBlocked())
    {
        pongAnimState* anim = m_pAnimState;
        // pongAnimState::m_pAnimSubState lives at +16 — pass its address.
        crAnimBlenderState_Init(&anim->m_pAnimSubState);   // reset phase-blocked sub-system @ 0x8224C810
        anim->m_swingPhase = kZero;        // clear swing phase progress (+412)
        rage_debugLog("pongPlayer::CancelSwing() anim reset", (uintptr_t)m_pCreature);
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
            // frameEntry layout: +16 = velocityRecord (ptr), +56 = frameId (int32)
            // velocityRecord: +12 = velFloat (float)
            uint32_t frameIdx   = (uint32_t)m_pAnimState->m_animProgress;  // +96 (index into array)
            void**   frameArray = (void**)m_pAnimState->m_pFrameArray;      // +4
            void*    frame      = frameArray[frameIdx];                     // array[idx]
            uint32_t* frameWords = static_cast<uint32_t*>(frame);
            void*    velocRec   = reinterpret_cast<void*>(frameWords[4]);   // +16 / 4

            float velFloat = static_cast<float*>(velocRec)[3];             // +12 / 4

            float fraction;
            if (velFloat > g_kZero) {
                int32_t frameId = static_cast<int32_t>(frameWords[14]);    // +56 / 4
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
        if (m_pRecoveryState->m_bForceBlock)
            return false;

        // Recovery state also needs a valid follow-up action to compare against.
        if (!m_pRecoveryState->m_pFollowAction)
            return false;

        // Evaluate recovery progress via helper.
        pongPlayer_GetAnimNormalizedTime((pongPlayer*)this);
        float recoveryVal = m_pRecoveryState->m_recoveryTimer;

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
        // The locomotion sub-state lives via m_pSwingSubState->m_pSubObject (+0x20).
        uint8_t* locoState = static_cast<uint8_t*>(m_pSwingSubState->m_pSubObject);
        uint8_t  flags     = m_pSwingSubState->m_locoFlags;  // +0x138 (312)

        // Bit 3: player already committed to swing zone.
        if (flags & 0x8)
            return true;

        // Bit 2: alternate commitment flag.
        if (flags & 0x4)
            return true;

        // Check whether the player has reached the return position spatially.
        if (false /* TODO: pongPlayer_IsLocoStateReady(locoState) */)
            return true;

        // Compute normalised arrival fraction from frame count and body speed.
        uint16_t frameCount = m_pSwingSubState->m_locoFrameCount;  // +0x134 (308)
        float    speed      = static_cast<float*>(m_pSwingSubState->m_pBodyData)[3];  // bodyData+12 / 4 = speed

        float fraction;
        if (speed > g_kZero) {
            fraction = (float)(int16_t)frameCount * g_kSwingScaleFactor / speed;
        } else {
            fraction = g_kZero;
        }

        float currentTime = m_pTimingState->m_currentTime;
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
    {
        typedef void (*BodyUpdateFn)(void*, void*);
        void** bodyVtbl = *static_cast<void***>(m_pPhysicsBody);
        reinterpret_cast<BodyUpdateFn>(bodyVtbl[3])(m_pPhysicsBody,
                                     static_cast<uint8_t*>(m_pPhysicsBody) + 16);
    }

    // ── 2. Capsule shape update ───────────────────────────────────────────
    phBoundCapsule_UpdateTransform(m_pPhysicsBound);

    // ── 3. Notify swing data of current capsule radius ────────────────────
    pongSwingData* swingData = m_pPlayerState ? m_pPlayerState->m_pSwingData : nullptr;
    if (swingData) {
        float radius = g_kSwingRadiusConst;  // @ 0x825CAEB8 + 8
        typedef void (*SwingDataNotifyFn)(pongSwingData*, float);
        void** sdVtbl = *reinterpret_cast<void***>(swingData);
        reinterpret_cast<SwingDataNotifyFn>(sdVtbl[1])(swingData, radius);
    }

    // ── 4. Flip suppression check ─────────────────────────────────────────
    // Result code 7 means the swing has already "flipped" — suppress further flip.
    bool suppressFlip = (m_swingInputResult == 7);
    (void)suppressFlip;  // used downstream in B208 call via f1 arg

    // ── 5. Build velocity-scaled swing vector ─────────────────────────────
    // phBoundCapsule: +0x38 (56) = directionX, +0x3C (60) = directionY
    float* boundBase = reinterpret_cast<float*>(m_pPhysicsBound);
    float vx = boundBase[14];  // +56 / 4 = directionX
    float vy = boundBase[15];  // +60 / 4 = directionY
    float scale = g_kSwingRadiusConst;
    vec3 swingVec = { vx * scale, vy * scale, 0.0f };

    // ── 6. Update swing prediction ────────────────────────────────────────
    pongPlayer_ClampSwingToBounds(m_pPlayerState, &swingVec, 0);

    // ── 7. Write power reading into input score table ─────────────────────
    // phBoundCapsule +0x48 (72) = current power value
    float* bodyBase = reinterpret_cast<float*>(m_pPhysicsBody);
    float power = bodyBase[18];  // +72 / 4 = power
    static_cast<float*>(g_pInputScoreTable)[m_swingInputSlot] = power;


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
            // Copy target vec to pongSwingData +352 (16-byte store).
            // TODO: add m_targetDir field to pongSwingData struct definition.
            *reinterpret_cast<vec3*>(reinterpret_cast<uint8_t*>(sd) + 352) = targetDir;

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
        rage_debugLog("pongPlayer::ProcessSwingDecision() queued", m_swingInputSlot);
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
    int32_t stateCode  = rstate->m_stateCode;      // +0x1C (28)
    int32_t serverFlag = rstate->m_serverFlag;      // +0x20 (32)
    // m_pFollowAction at +0x24 doubles as subState int32 here
    int32_t subState   = static_cast<int32_t>(reinterpret_cast<intptr_t>(rstate->m_pFollowAction));  // +0x24 (36)

    // Qualify: stateCode == 0 OR subState == 3, AND serverFlag == 1.
    bool slotOk = (stateCode == 0) ? (subState == 3) : false;
    bool serverOk = (serverFlag == 1);
    if (!slotOk || !serverOk)
        return;

    // Copy 16-byte target vectors into the recovery slot area.
    rstate->m_vec0 = *pSlotA;   // +0x50 (80)
    rstate->m_vec1 = *pSlotB;   // +0x60 (96)

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

    void* followAction = rstate->m_pFollowAction;  // +0x24 (36)
    if (!followAction)
        return false;

    // Read the action's velocity record.
    // followAction layout: +20 = frameCount (int32), +36 = velRecord (ptr)
    // velRecord layout: +12 = speedFloat (float)
    float recoveryProgress = rstate->m_recoveryTimer;  // +0xAC (172)
    uint32_t* actionWords = static_cast<uint32_t*>(followAction);
    void*  velRecord   = reinterpret_cast<void*>(actionWords[9]);   // +36 / 4
    float* velFloats   = static_cast<float*>(velRecord);
    float  velFloat    = velFloats[3];                              // +12 / 4
    float normThreshold = g_kZero;

    if (velFloat > g_kZero) {
        int32_t frameCount = static_cast<int32_t>(actionWords[5]); // +20 / 4
        normThreshold = (float)frameCount * g_kSwingScaleFactor / velFloat;
    }

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
// On a state change: logs the transition via rage_debugLog (which is a
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
        rage_debugLog("pongPlayer::SetSwingActiveState() swing entered",
                     m_swingInputSlot);
    } else {
        rage_debugLog("pongPlayer::SetSwingActiveState() swing exited",
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

    // phBoundCapsule +0x40 (64): bit 0 = "swing capable" flag
    uint8_t boundFlags = reinterpret_cast<uint8_t*>(opp->m_pPhysicsBound)[64];
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
    // g_pClockObj → ptr → +12 = frame state int
    uint32_t* clockPtr = reinterpret_cast<uint32_t*>(g_pClockObj);
    int32_t clockState = reinterpret_cast<int32_t*>(clockPtr[0])[3];  // deref + offset 12/4
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
    // Each row in g_pPlayerDataTable is 808 bytes; the per-player record begins at +12 within the row.
    // TODO: define PlayerDataRecord struct (808 bytes; +784 = button state block start).
    uint8_t* record     = reinterpret_cast<uint8_t*>(g_pPlayerDataTable) + (slotIdx * 808) + 12;

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

        // Score accumulator @ g_pButtonStateTable + 0x8CEC (36076).
        // TODO: typed accessor once ButtonStateTable layout is characterised.
        float& scoref = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(g_pButtonStateTable) + 0x8CEC);

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
            float swingProgress = m_pSwingSubState->m_swingProgress;  // +0xA0
            if (swingProgress <= g_kZero)
                goto path_b;

            // Retrieve the velocity record and timing state.
            void*  velRecord = m_pSwingSubState->m_pVelRecord;  // +0x30
            float  speedFloat = static_cast<float*>(velRecord)[3];  // velRecord+12 / 4 = speed
            int16_t frameCount = m_pSwingSubState->m_swingSubFrame;  // +0x154

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

        float swingPhase = m_pAnimState->m_swingPhase;  // +0x19C
        if (swingPhase <= g_kZero)
            return false;

        // Compute normalised fraction from anim frame array entry.
        // frameEntry layout: +16 = velRecord (ptr), +56 = frameDuration (int32)
        // velRecord layout: +12 = speedFloat (float)
        int32_t  frameIdx    = m_pAnimState->m_frameIndex;       // +0x194
        void**   frameArray  = static_cast<void**>(m_pAnimState->m_pFrameArray);  // +0x04
        void*    frameEntry  = frameArray[frameIdx];
        uint32_t* frameWords = static_cast<uint32_t*>(frameEntry);
        void*    velRecord   = reinterpret_cast<void*>(frameWords[4]);   // +16 / 4
        float    speedFloat  = static_cast<float*>(velRecord)[3];        // +12 / 4
        int32_t  frameDuration = static_cast<int32_t>(frameWords[14]);   // +56 / 4

        float apexFraction = g_kZero;
        if (speedFloat > g_kZero) {
            apexFraction = (float)frameDuration * g_kFrameToSecScale / speedFloat;
        }

        float animPos = m_pAnimState->m_animProgress;  // +0x60
        return animPos < apexFraction;
    }

    // ── PATH C: in recovery ───────────────────────────────────────────────
    if (IsRecovering()) {
        if (!m_pRecoveryState)
            return false;

        // If force-blocked, the recovery apex is trivially reachable.
        if (m_pRecoveryState->m_bForceBlock)
            return false;  // TODO: verify — force-block=true may mean inverted

        if (!m_pRecoveryState->m_pFollowAction)
            return false;

        float progress = m_pRecoveryState->m_recoveryTimer;  // +0xAC
        return progress > threshold;
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
            float peakTime = m_pTimingState->m_peakTime;  // +0x2C (44)
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
            if (!m_pSwingSubState)
                return false;

            // Sub-object via m_pSubObject (+0x20)
            uint8_t* swingSubBase = static_cast<uint8_t*>(m_pSwingSubState->m_pSubObject);
            if (!swingSubBase)
                return false;

            // Check position flag bits.
            uint8_t flags = m_pSwingSubState->m_locoFlags;  // +0x138 (312)
            if (flags & 0x8)
                return true;  // flag A: already in position
            if (flags & 0x4)
                return true;  // flag B: in return position

            // Ask the animation system if we've cleared the swing gate.
            if (false /* TODO: pongPlayer_IsLocoStateReady(swingSubBase) */)
                return true;

            // Compute normalised timing fraction.
            float   speedFloat = static_cast<float*>(m_pSwingSubState->m_pBodyData)[3];  // bodyData+12 / 4
            int16_t frameCount = m_pSwingSubState->m_locoFrameCount;  // +0x134 (308)

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
    // Locomotion state +464 = ball splash index (selects from 416-byte entries)
    uint32_t* locoWords = static_cast<uint32_t*>(this->m_pLocomotionState);
    uint32_t index = locoWords[116];  // +464 / 4

    // Base array g_ballSplashArray (416-byte entries), offset by +48 per entry
    void* targetStruct = static_cast<void*>(g_ballSplashArray + 48 + (index * 416));
    
    // Call helper to compute target vector
    pongPlayer_ComputeTargetPosition(&targetVec, targetStruct);
    
    // Step 2: Load timing values
    float animPhase = this->m_swingPhaseInput;  // this+68

    // Global clock object: +24 = timing value (float)
    float* clockFloats = static_cast<float*>(g_pGlobalClock);
    float clockValue = clockFloats[6];  // +24 / 4
    
    // Load constants
    // g_kFrameToSecScale already declared at file scope (non-const)
    extern const float g_kTimingConstant;   // @ 0x825C4930
    // Step 3: Compute timing values
    float scaledPhase = animPhase * g_kFrameToSecScale;
    float timingFactor = clockValue * g_kTimingConstant;

    // Step 4: Call math function
    float mathResult = fiAsciiTokenizer_ParseFloat(scaledPhase * g_kFloatConst_79BAC);  // was g_kAdjustmentScale @ 0x82079BAC
    
    // Step 5: Compute adjustment magnitude
    float adjustment = mathResult * timingFactor;
    
    // Step 6: Check if adjustment is significant
    if (fabsf(adjustment) > g_kFloatConst_D110) {  // was g_kAdjustmentThreshold @ 0x8202D110
        // Load current value at +72
        float currentValue = this->m_swingDirectionAdj;
        
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
        // Global clock: +16 = max threshold base (float)
        float maxThreshold = clockFloats[4] * g_kFloatConst_79CD8;  // +16 / 4
        
        if (finalAdjustment < maxThreshold) {
            float delta = maxThreshold - finalAdjustment;
            this->m_swingTimingClamp = delta;
        }
    }
    
    // Step 7: Clamp the value at +80 between 0.0 and 1.0
    float* adjustmentPtr = &this->m_swingTimingClamp;
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
    // Load the two indices that determine which swing target to use.
    // These are at high offsets (+948, +952) in the pongPlayer struct — past the
    // confirmed layout. Access via byte pointer until struct is extended.
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    uint32_t index1 = *reinterpret_cast<uint32_t*>(self + 948);
    uint32_t index2 = *reinterpret_cast<uint32_t*>(self + 952);

    // Compute the array offset: ((index1 * 9 + index2 * 3) * 32) + 976
    uint32_t arrayOffset = ((index1 * 9 + index2 * 3) * 32) + 976;

    // Get pointer to the swing target data within this object.
    vec3* swingTarget = reinterpret_cast<vec3*>(self + arrayOffset);

    vec3 targetDelta;
    pongPlayer_ComputeCourtPosition(&targetDelta, swingTarget);

    // Load position offset vector at +880, store result at +832.
    vec3* currentOffset  = reinterpret_cast<vec3*>(self + 880);
    vec3* resultPosition = reinterpret_cast<vec3*>(self + 832);

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
    const float* constants = &g_kFloatConst_D10C;  // was (const float*)0x8202D10C
    const float scale = constants[0];      // Base scale factor
    const float zero = constants[1];       // Zero constant
    const float halfScale = 0.5f;          // Normalization factor
    const float epsilon = 0.01f;           // Small offset for grid adjustments
    
    // Initialize base state via helper function
    pongPlayer_GetCourtBounds(this, r4, nullptr, nullptr, 1, nullptr, nullptr, metadataByte);
    
    // Grid storage base pointers
    uint8_t* gridBase = (uint8_t*)this + 864;      // +0x360
    uint8_t* cellPtr = (uint8_t*)this + 984;       // +0x3D8 (first cell)
    uint8_t* mirrorBase = g_mirrorBuffer;  // was (uint8_t*)0x82606720
    
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
    // Load player's input slot index from locomotion state +464
    int32_t* locoInts = static_cast<int32_t*>(m_pLocomotionState);
    int32_t inputSlot = locoInts[116];  // +464 / 4

    // Load global input data table base
    extern uint8_t* g_pPlayerInputTable;  // @ lis(-32158) + -23600
    extern int32_t  g_currentInputSlot;   // @ lis(-32160) + 25976

    // Calculate offset into input table: inputSlot * 416 bytes per entry
    uint8_t* inputData = g_pPlayerInputTable + (inputSlot * 416);

    // Load current position vector from inputData+240
    vec3 currentPos;
    memcpy(&currentPos, inputData + 240, sizeof(vec3));

    vec3 tempVec1;
    pongPlayer_ComputeTargetPosition(&tempVec1, inputData);

    // Scale input by player-specific factors
    // this+36 and this+40 are within the pongPlayer struct pad region
    float* selfFloats = reinterpret_cast<float*>(this);
    float scaleX = selfFloats[9];   // +36 / 4
    float scaleY = selfFloats[10];  // +40 / 4
    
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
    
    // Store result to this+48 (16-byte aligned, overlaps m_pLocoState2 region)
    memcpy(&this->m_pLocoState2, &finalVec, sizeof(vec3));
    
    // Load array pointer and compute index
    int32_t arrayIndex = inputSlot + 17;  // g_pInputArrayTable declared at file scope @ 0x825EAB28
    void* arrayEntry = g_pInputArrayTable[arrayIndex];
    
    // Input array entry: +56 = threshold float
    float threshold = static_cast<float*>(arrayEntry)[14];  // +56 / 4
    
    // Compute scaled threshold check
    float scaledThreshold = threshold * deltaX;
    
    // Initialize output value
    int32_t outputValue = 0;

    // Check if input magnitude is significant
    if (fabs(deltaX) >= g_kFloatConst_D108) {  // was g_kZeroThreshold @ 0x8202D108
        // Quantize the input to integer range
        // This complex section converts float input to clamped integer
        
        // Determine sign and apply scaling
        float absInput = fabs(deltaX);
        
        float quantScale = (deltaY <= g_kFloatConst_D110) ?  // was g_kInputThresholdHigh @ 0x8202D110
                          g_kFloatConst_79FFC : g_kFloatConst_79FF8;  // was g_kQuantScale1/2 @ 0x82079FFC/FF8
        
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
            int32_t stateValue = reinterpret_cast<int32_t*>(inputData)[39];  // +156 / 4
            
            if (stateValue == 3 && outputValue >= 0) {
                outputValue = 0;
            }
        }
    }
    
    // Check locomotion state byte at +6626
    uint8_t bodyState = reinterpret_cast<uint8_t*>(m_pLocomotionState)[6626];
    
    if (bodyState != 0) {
        // Apply tighter clamping: [-4, 31]
        if (outputValue < -4) {
            outputValue = -4;
        } else if (outputValue > 31) {
            outputValue = 31;
        }
    }
    
    // Store final result to inputData+48
    reinterpret_cast<int32_t*>(inputData)[12] = outputValue;  // +48 / 4
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
                // Recovery state +0x20 (32): reinterpreted as float (same offset as m_serverFlag)
                // TODO: this offset may actually be a float field, not m_serverFlag
                float posX = reinterpret_cast<float*>(m_pRecoveryState)[8];  // +32 / 4
                
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
    if (m_courtSide != side) {
        // Log the change (debug only)
        const char* sideStr = side ? "right" : "left";
        rage_debugLog("pongPlayer::SetPlayerSide() - setting to %s", sideStr);
        
        // Update the side
        m_courtSide = side;
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
        float secondaryThreshold = m_pTimingState->m_peakTime;  // +0x2C (44, labeled as +36 in scaffold)
        
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
    float current = m_lerpValue;  // field at +0
    
    if (current < target) {
        // Lerp upwards
        current += rate * deltaTime;
        if (current > target) {
            current = target;
        }
    } else if (current > target) {
        // Lerp downwards
        current -= rate * deltaTime;
        if (current < target) {
            current = target;
        }
    }
    
    m_lerpValue = current;
    
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


// ===========================================================================
// SECTION 20 — Process/gameplay helper functions (92-216B)
// ===========================================================================

// ── Additional externs for Section 20 ─────────────────────────────────────
extern void pongPlayer_EF38_g(void* self, uint8_t r9, uint8_t r10, uint32_t r6,
                               uint32_t r3, float f3, float f2, float f1);
extern void pongPlayer_F2A0_g(void* self, void* subObj);
extern void pongPlayer_SetPlayMode(pongPlayer* p, int param);
extern void pongMover_CalcInitMatrix(void* out, void* inst);
extern void pongPlayer_SetupShotTarget(void* target, float f1, float f2);
extern void pg_E6E0(int code, uint8_t mask, int p3, int p4);
extern void pongPlayer_9108_g(void* target, void* syncData);

// g_pMatchState @ 0x8271A318 and g_pMatchConfig @ 0x8271A32C declared at file scope
extern float g_kSpeedTable[];     // @ 0x825D7600


/**
 * pongPlayer::IsMatchSlotValid  @ 0x8218EDB0 | size: 0x5C (92 bytes)
 *
 * Checks whether the player's current match slot index is valid and has an
 * active entry. Reads the slot index from field +464, indexes into two
 * separate lookup tables (one for slot validation, one for entry existence),
 * and returns true only if both checks pass (slot == 1 in the first table,
 * and entry != -1 in the second table).
 *
 * Called via vtable slot 1.
 */
bool pongPlayer::IsMatchSlotValid() const {  // pongPlayer_vfn_1 @ 0x8218EDB0
    uint32_t slotIndex = m_slotIndex;  // +464

    // Check slot validity: table[(slotIndex + 14)] must be 1
    uint32_t* validationTable = g_pPlayerSlotTable;
    bool slotValid = (validationTable[slotIndex + 14] == 1);

    // Check entry existence: table2[(slotIndex + 1)] must not be -1
    int32_t* entryTable = reinterpret_cast<int32_t*>(g_pPlayerDataTable);
    int32_t entry = entryTable[slotIndex + 1];

    if (!slotValid) {
        return false;
    }
    if (entry == -1) {
        return false;
    }
    return true;
}


/**
 * pongPlayer::UpdateSwingTrajectory  @ 0x82192CD0 | size: 0xB4 (180 bytes)
 *
 * Called from the main update virtual (vfn_2) to compute and apply the
 * swing trajectory for the current frame:
 *   1. Look up the geometry record via pg_GetGeometryRecord.
 *   2. Check the shot type in a 416-byte-per-entry table: must be 3.
 *   3. Gather the current state (slot, handedness, shot parameters).
 *   4. Call pongPlayer_EF38_g to compute the trajectory arc.
 *   5. Call pongPlayer_F2A0_g to submit the result to the animation system.
 */
void pongPlayer::UpdateSwingTrajectory() {  // pongPlayer_2CD0_g @ 0x82192CD0
    uint32_t slotIndex = m_slotIndex;  // +464
    void* geomBase = g_pPlayerSlotTable;

    void* geomRecord = pg_GetGeometryRecord(geomBase);

    // Check shot type in 416-byte-per-entry table at offset 204
    uint8_t* tableBase = reinterpret_cast<uint8_t*>(g_pPlayerDataTable) - 23600;
    uint32_t* shotEntry = reinterpret_cast<uint32_t*>(tableBase + 204 + slotIndex * 416);
    bool isShotType3 = (*shotEntry == 3);

    // Load geometry speed data from record (+40 -> +156)
    uint8_t* geomRecordBytes = static_cast<uint8_t*>(geomRecord);
    void* geomData = *reinterpret_cast<void**>(geomRecordBytes + 40);
    float speedParam = *reinterpret_cast<float*>(
        static_cast<uint8_t*>(geomData) + 156);

    uint32_t playerState = m_playerFlags;  // +448
    float swingStrengthX = m_swingStrengthX;  // +6320
    float swingStrengthY = m_swingStrengthY;  // +6324
    void* shotState = m_pShotState;  // +492

    // Compute trajectory arc
    pongPlayer_EF38_g(shotState, static_cast<uint8_t>(isShotType3),
                      static_cast<uint8_t>(*(static_cast<uint8_t*>(geomBase) + slotIndex + 64)),
                      playerState, m_slotIndex,
                      speedParam, swingStrengthY, swingStrengthX);

    // Submit to animation sub-object at slot (index+17) in table
    uint32_t* animTable = g_pPlayerSlotTable;
    uint32_t animEntry = animTable[m_slotIndex + 17];
    void* animSubObj = reinterpret_cast<void*>(animEntry + 48);
    pongPlayer_F2A0_g(shotState, animSubObj);
}


/**
 * pongPlayer::UpdateCreatureSlotAnims  @ 0x821987B0 | size: 0x90 (144 bytes)
 *
 * Iterates over 4 creature sub-object slots (at this+6148, stride 4).
 * For each valid (non-null) entry with positive anim count:
 *   - If resetFlag is set, calls vtable slot 2 to reset the creature.
 *   - Otherwise, copies the speed from sub-object (+15400->+24) into
 *     blend fields (+15392, +15396).
 * Clears field +6316 at the start.
 *
 * Called from pongPlayer_Process and pongPlayer_BEE0_g.
 */
void pongPlayer::UpdateCreatureSlotAnims(uint8_t resetFlag) {  // pongPlayer_87B0_g @ 0x821987B0
    m_creatureSlotCounter = 0;  // +6316

    for (int i = 0; i < 4; ++i) {
        void* slotObj = m_creatureSlots[i];  // +6148, array of 4 pointers
        if (!slotObj) {
            continue;
        }

        uint8_t* slotBytes = static_cast<uint8_t*>(slotObj);
        int32_t animCount = *reinterpret_cast<int32_t*>(slotBytes + 20);
        if (animCount <= 0) {
            continue;
        }

        if (resetFlag) {
            // Call virtual slot 2 to reset creature animation
            void** vt = *reinterpret_cast<void***>(slotObj);
            reinterpret_cast<void(*)(void*)>(vt[2])(slotObj);
        } else {
            // Copy speed from sub-object's target data into blend fields
            void* targetData = *reinterpret_cast<void**>(slotBytes + 15400);
            float targetSpeed = *reinterpret_cast<float*>(
                static_cast<uint8_t*>(targetData) + 24);
            float currentAnim = *reinterpret_cast<float*>(slotBytes + 46184);

            *reinterpret_cast<float*>(slotBytes + 15392) = targetSpeed;
            *reinterpret_cast<float*>(slotBytes + 15396) = currentAnim;
        }
    }
}


/**
 * pongPlayer::InitMovementAndContact  @ 0x82195D50 | size: 0x9C (156 bytes)
 *
 * Called from pongPlayer_Process. Initializes movement state and checks
 * for ball contact readiness:
 *   1. Calls pongPlayer_SetPlayMode(0) to reset movement state.
 *   2. If slot is null in validation table, or IsInputActiveAndReady
 *      returns true, computes the initial position matrix and sets up
 *      the contact zone parameters.
 */
void pongPlayer::InitMovementAndContact() {  // pongPlayer_5D50_g @ 0x82195D50
    pongPlayer_SetPlayMode(this, 0);

    uint32_t slotIndex = m_slotIndex;  // +464
    uint32_t* validationTable = g_pPlayerSlotTable;
    uint32_t slotEntry = validationTable[slotIndex + 14];

    bool needsInit = (slotEntry == 0);

    if (!needsInit) {
        if (!pongPlayer_CheckServeInput(this)) {
            return;
        }
    }

    // Compute initial position matrix from creature instance (+452 -> +168)
    void* creatureState = m_pCreatureState;  // +452
    void* creatureInst = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(static_cast<void*>(creatureState)) + 168);

    float localMatrix[16];
    pongMover_CalcInitMatrix(localMatrix, creatureInst);

    // Set up contact zone with scale constants
    extern const float g_kContactScale1;  // @ 0x825C8A50
    extern const float g_kContactScale2;  // @ 0x82028080
    void* shotState = m_pShotState;  // +496
    pongPlayer_SetupShotTarget(shotState, g_kContactScale1, g_kContactScale2);
}


/**
 * pongPlayer::ResetSwingSlotEntries  @ 0x821A6080 | size: 0x8C (140 bytes)
 *
 * Zeroes out all 10 swing slot entries (80-byte stride). Each entry:
 *   - flags word at +4: cleared to 0
 *   - byte at +76: cleared to 0
 *   - byte at +77: set to 1 (enabled)
 *   - counter at +84: cleared to 0
 * Plus two trailing dword clears at +800 and +804.
 *
 * Called from game_5FC8 during match init.
 */
void pongPlayer::ResetSwingSlotEntries(void* slotsBase) {  // pongPlayer_6080_p33 @ 0x821A6080
    uint8_t* base = reinterpret_cast<uint8_t*>(slotsBase);

    for (int i = 0; i < 10; ++i) {
        uint8_t* entry = base + i * 80;
        *reinterpret_cast<uint32_t*>(entry + 4)  = 0;
        *(entry + 76) = 0;
        *(entry + 77) = 1;
        *reinterpret_cast<uint32_t*>(entry + 84) = 0;
    }

    *reinterpret_cast<uint32_t*>(base + 800) = 0;
    *reinterpret_cast<uint32_t*>(base + 804) = 0;
}


/**
 * pongPlayer::ComputeShotSpeedForType  @ 0x821D6100 | size: 0xA8 (168 bytes)
 *
 * Computes shot speed based on shot type (+156) and input power (f1).
 * Input is clamped to [0, 1], then used to lerp between base and max
 * speed from a lookup table. Result stored at this+148.
 *
 * Shot types 8, 9, 10 all fall through to the same computation.
 * Called from pongPlayer_SetupServe and pongPlayer_0780_g.
 */
void pongPlayer::ComputeShotSpeedForType(float inputPower) {  // pongPlayer_6100_g @ 0x821D6100
    extern const float g_kShotSpeedClampZero;  // @ 0x82079D30
    extern const double g_kShotSpeedClampMax;  // @ 0x82079CE8

    // Clamp to [0, max] via fsel pattern
    float clamped = inputPower - g_kShotSpeedClampZero;
    float safeVal = (clamped >= 0.0f) ? inputPower
                                      : static_cast<float>(g_kShotSpeedClampMax);
    float excess = safeVal - static_cast<float>(g_kShotSpeedClampMax);
    float finalVal = (excess >= 0.0f) ? static_cast<float>(g_kShotSpeedClampMax)
                                      : safeVal;

    float absVal = fabsf(finalVal);

    // Lerp between base speed and max speed
    float baseSpeed = g_kSpeedTable[1];  // @ 0x825D7604
    float maxSpeed  = g_kSpeedTable[0];  // @ 0x825D7600

    float range = maxSpeed - baseSpeed;
    m_shotSpeed = range * absVal + baseSpeed;  // +148
}


/**
 * pongPlayer::NotifySlotChangeAndSync  @ 0x821C9870 | size: 0xA4 (164 bytes)
 *
 * When a slot's dirty byte is clear, sends a page-group notification
 * (code 4130 with slot mask | 64) and marks it dirty. Then synchronises
 * via pongPlayer_9108_g (SyncByteField).
 *
 * Used for network synchronisation of player slot state changes.
 */
void pongPlayer::NotifySlotChangeAndSync(void* slotsBase, uint32_t slotIndex,
                                          uint32_t columnIndex) {
    // pongPlayer_9870_g @ 0x821C9870
    uint8_t* base = reinterpret_cast<uint8_t*>(slotsBase);

    // Look up speed value from table indexed by columnIndex
    extern float g_kSlotSpeedLookup[];  // @ 0x825C76CC
    float speedVal = g_kSlotSpeedLookup[columnIndex];

    // Store speed at slot offset (+61 base, *4 stride)
    uint32_t storeOffset = (slotIndex + 61) * 4;
    *reinterpret_cast<float*>(base + storeOffset) = speedVal;

    // Compute entry stride: slot*3 + column -> *3 final
    uint32_t entryIdx = slotIndex * 3 + columnIndex;
    uint32_t finalIdx = entryIdx * 3;

    uint8_t* dirtyAddr = base + finalIdx * 4 + 172;
    uint8_t dirtyFlag = *dirtyAddr;

    if (dirtyFlag == 0) {
        uint8_t slotMask = static_cast<uint8_t>(1 << slotIndex) | 64;
        pg_E6E0(4130, slotMask, 1, static_cast<int>(columnIndex));

        *(base + slotIndex + 288) = 1;
    }

    // Sync byte field
    uint8_t syncBuf[4];
    syncBuf[0] = 1;
    pongPlayer_9108_g(dirtyAddr, syncBuf);
}


/**
 * pongPlayer::UpdateServeSpeed  @ 0x8218CD48 | size: 0xB8 (184 bytes)
 *
 * Called from pongPlayer_Process. Determines the serve speed based on
 * the current match rally state:
 *   1. If rally index is out of range, uses a default speed constant.
 *   2. Otherwise, XORs handedness flags to pick left/right speed
 *      column (+24 or +28) from the rally speed table.
 *   3. Stores result in creature state's speed field (+208).
 */
void pongPlayer::UpdateServeSpeed() {  // pongPlayer_CD48_g @ 0x8218CD48
    uint8_t* matchBytes = static_cast<uint8_t*>(g_pMatchState);
    int32_t rallyIndex = *reinterpret_cast<int32_t*>(matchBytes + 28);

    bool indexValid = false;
    if (rallyIndex >= 0) {
        int32_t maxRallies = *reinterpret_cast<int32_t*>(matchBytes + 8);
        if (rallyIndex < maxRallies - 1) {
            indexValid = true;
        }
    }

    uint8_t* creatureBytes = reinterpret_cast<uint8_t*>(m_pCreatureState);

    if (!indexValid) {
        *reinterpret_cast<float*>(creatureBytes + 208) = g_kFloatConst_D108;
        return;
    }

    uint8_t* configBytes = static_cast<uint8_t*>(g_pMatchConfig);
    uint8_t configHandedness = configBytes[6];

    if (configHandedness == 0) {
        extern const float g_kDefaultServeSpeed;
        *reinterpret_cast<float*>(creatureBytes + 208) = g_kDefaultServeSpeed;
        return;
    }

    uint8_t configSide = configBytes[5];
    uint32_t slotIndex = m_slotIndex;  // +464

    // Look up rally entry
    uint32_t nextRally = rallyIndex + 1;
    uint32_t* rallyTable = *reinterpret_cast<uint32_t**>(matchBytes + 12);
    void* rallyEntry = reinterpret_cast<void*>(rallyTable[nextRally]);

    // Get speed table from rally entry (+8 -> +40)
    uint8_t* rallyBytes = static_cast<uint8_t*>(rallyEntry);
    void* rallySubObj = *reinterpret_cast<void**>(rallyBytes + 8);
    void* speedData = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(rallySubObj) + 40);

    uint8_t playerTableSide = *(static_cast<uint8_t*>(
        static_cast<void*>(g_pPlayerSlotTable)) + 64);

    uint32_t sideXor = configSide ^ playerTableSide;

    // Pick forehand (+24) or backhand (+28) speed
    uint8_t* speedBytes = static_cast<uint8_t*>(speedData);
    int speedOffset = (sideXor == slotIndex) ? 24 : 28;
    float speed = *reinterpret_cast<float*>(speedBytes + speedOffset);
    *reinterpret_cast<float*>(creatureBytes + 208) = speed;
}


/**
 * pongPlayer::InitializeReplaySnapshot  @ 0x820D9420 | size: 0xD4 (212 bytes)
 *
 * Captures the initial replay snapshot. Called from pongPlayer_83C8_g
 * during replay recording setup:
 *   1. Zero-initialises vec3 buffers, calls vtable slot 4 for position.
 *   2. Copies transform vector from singleton+64 into output+16.
 *   3. Stores base speed at output+0, clears bytes +8..+10.
 */
void pongPlayer::InitializeReplaySnapshot(void* outSnapshot) {
    // pongPlayer_9420_wrh @ 0x820D9420
    uint8_t* out = reinterpret_cast<uint8_t*>(outSnapshot);

    void** singletonPtr = reinterpret_cast<void**>(g_pMatchState);
    void* singleton = *singletonPtr;

    float zero = g_kFloatConst_D110;  // was g_kReplayZero @ 0x8202D110

    float posVec[4]  = { zero, zero, zero, 0.0f };
    float zeroVec[4] = { zero, zero, zero, 0.0f };

    // Call vtable slot 4 for player position
    typedef void (*GetPosFn)(void*, float*);
    void** vtable = *reinterpret_cast<void***>(singleton);
    GetPosFn getPos = reinterpret_cast<GetPosFn>(vtable[4]);
    getPos(singleton, posVec);

    // Read base speed from singleton (+48)
    uint8_t* singletonBytes = static_cast<uint8_t*>(singleton);
    float baseSpeed = *reinterpret_cast<float*>(singletonBytes + 48);

    *reinterpret_cast<float*>(out + 0) = baseSpeed;

    // Copy transform (singleton+64 -> output+16)
    memcpy(out + 16, singletonBytes + 64, 16);

    memcpy(out + 32, posVec, 16);
    memcpy(out + 48, zeroVec, 16);

    out[8]  = 0;
    out[9]  = 0;
    out[10] = 0;
}


/**
 * pongPlayer::ClampMovementToCourtBounds  @ 0x8219D558 | size: 0xD8 (216 bytes)
 *
 * Clamps X and Z movement deltas to court boundaries using the player's
 * court half-widths (+896 for X, +904 for Z). Uses fsel-style clamping.
 *
 * Called from pongPlayer_D8E8_g during movement processing.
 */
void pongPlayer::ClampMovementToCourtBounds(float* delta) {
    // pongPlayer_D558_wrh @ 0x8219D558
    // Constants: g_kFloatConst_D110=g_kFloatConst_D110 @ 0x8202D110
    extern const double g_kCourtMaxDelta;  // @ 0x82079D78
    extern const double g_kCourtNegMax;    // @ 0x82079B80
    extern const float g_kCourtMinDelta;   // @ 0x82079D68

    float maxDelta = static_cast<float>(g_kCourtMaxDelta);
    float negMax   = static_cast<float>(g_kCourtNegMax);

    // Clamp X component
    if (delta[0] > g_kFloatConst_D110) {
        float adj = delta[0] - m_courtHalfWidthX;  // +896
        if (adj >= 0.0f) {
            float clamped = (adj >= 0.0f) ? adj : negMax;
            delta[0] = (clamped - maxDelta >= 0.0f) ? maxDelta : clamped;
        }
    }
    if (delta[0] < g_kFloatConst_D110) {
        float adj = m_courtHalfWidthX + delta[0];
        if (adj - g_kCourtMinDelta >= 0.0f) {
            delta[0] = (adj >= 0.0f) ? negMax : adj;
        }
    }

    // Clamp Z component
    if (delta[2] > g_kFloatConst_D110) {
        float adj = delta[2] - m_courtHalfWidthZ;  // +904
        if (adj >= 0.0f) {
            float clamped = (adj >= 0.0f) ? adj : negMax;
            delta[2] = (clamped - maxDelta >= 0.0f) ? maxDelta : clamped;
        }
    }
    if (delta[2] < g_kFloatConst_D110) {
        float adj = m_courtHalfWidthZ + delta[2];
        if (adj - g_kCourtMinDelta >= 0.0f) {
            delta[2] = (adj >= 0.0f) ? negMax : adj;
        }
    }
}


// ===========================================================================
// SECTION 26 — SaveDrawData  @ 0x8218E860 | size: 0x30 (48 bytes)
// ===========================================================================
/**
 * pongPlayer::SaveDrawData()
 *
 * Delegates draw-data serialisation to the render sub-object.
 * If the player is not active (byte +36 == 0), calls the debug no-op
 * and returns early.  Otherwise loads the render object pointer at +444,
 * advances by 16, and calls vtable slot 1 (SaveDrawData) on it.
 */
void pongPlayer::SaveDrawData() {  // pongPlayer_SaveDrawData @ 0x8218E860
    if (!m_bVisible) {  // byte +36
        rage_debugLog("pongPlayer::SaveDrawData() not visible");  // debug logging no-op
        return;
    }
    void* renderObj = this->m_pDrawData;
    uint8_t* renderBytes = static_cast<uint8_t*>(renderObj);
    void* renderSub = renderBytes + 16;
    // vtable slot 1 call on renderSub
    void** vt = *reinterpret_cast<void***>(renderSub);
    reinterpret_cast<void(*)(void*)>(vt[1])(renderSub);
}


// ===========================================================================
// SECTION 27 — CompareTypeNames  @ 0x820CF0B8 | size: 0x38 (56 bytes)
// ===========================================================================
/**
 * pongPlayer::CompareTypeNames(void* a, void* b)
 *
 * Byte-by-byte string comparison of two RTTI type name strings.
 * Each parameter is a pointer to an object; the function dereferences
 * the first word (vtable/type ptr), then dereferences again to get
 * the actual string pointer, and compares the strings character by
 * character.  Returns the difference of the first mismatched byte
 * pair, or 0 if identical up to the null terminator.
 *
 * Equivalent to: strcmp(**(char***)a, **(char***)b)
 */
int pongPlayer::CompareTypeNames(void* a, void* b) {  // pongPlayer_F0B8_p46 @ 0x820CF0B8
    const char* strA = **reinterpret_cast<const char***>(a);
    const char* strB = **reinterpret_cast<const char***>(b);

    while (true) {
        int diff = static_cast<int>(static_cast<uint8_t>(*strA)) -
                   static_cast<int>(static_cast<uint8_t>(*strB));
        if (*strA == '\0') {
            return diff;
        }
        ++strA;
        ++strB;
        if (diff != 0) {
            return diff;
        }
    }
}


// ===========================================================================
// SECTION 28 — IsLocomotionReady  @ 0x82192578 | size: 0x50 (80 bytes)
// ===========================================================================
/**
 * pongPlayer::IsLocomotionReady()
 *
 * Checks whether the player's locomotion sub-system is in a ready state.
 * Traverses: this+452 -> +188 -> +120 to get a locomotion object.
 * If null, returns false.  Otherwise calls pongPlayer_IsLocoStateReady on that
 * object+32 and returns true if it succeeds, false otherwise.
 */
bool pongPlayer::IsLocomotionReady() const {  // pongPlayer_2578_g @ 0x82192578
    // TODO: +452 is m_pOpponent but this chain accesses sub-fields at +188/+120 —
    //       likely should be a different typed field (m_pCreatureState or similar)
    uint8_t* sub1 = reinterpret_cast<uint8_t*>(this->m_pOpponent);
    void* sub2 = *reinterpret_cast<void**>(sub1 + 188);
    void* locoObj = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(sub2) + 120);
    if (!locoObj) {
        return false;
    }
    void* locoState = static_cast<uint8_t*>(locoObj) + 32;
    return pongPlayer_IsLocoStateReady(locoState);
}


// ===========================================================================
// SECTION 29 — ResetServePosition  @ 0x821936D8 | size: 0x60 (96 bytes)
// ===========================================================================
/**
 * pongPlayer::ResetServePosition()
 *
 * Resets the player to the serve position by:
 *  1. Checking a handedness flag via the geometry singleton (byte at
 *     field+464+global_offset+64), converting to a "flip" boolean.
 *  2. Calling pongPlayer_3628_g(this, flipFlag) to set serve-side.
 *  3. Calling game_C690(this) to compute the reset position.
 *  4. Calling pongMover_Reset on the mover sub-object at (this+452)->+168
 *     with the position result.
 */
// pongPlayer_3628_g (SetServeSideFlip) — implemented at end of file
extern void* game_C690(pongPlayer* player);
extern void pongMover_Reset(void* mover, void* position);
extern uint32_t g_kGeomOffset;  // global loaded via lis+lwz

void pongPlayer::ResetServePosition() {  // pongPlayer_36D8_g @ 0x821936D8
    uint32_t fieldOffset = static_cast<uint32_t>(this->m_inputSlotIdx);
    uint8_t* geomBase = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(fieldOffset + g_kGeomOffset));
    uint8_t handFlag = geomBase[64];
    // cntlzw + rlwinm: convert 0 -> 1, nonzero -> 0
    uint8_t flip = (handFlag == 0) ? 1 : 0;

    pongPlayer_3628_g(this, flip);

    void* posResult = game_C690(this);

    // TODO: +452 is m_pOpponent but accesses +168 sub-field — likely wrong typed field
    uint8_t* sub = reinterpret_cast<uint8_t*>(this->m_pOpponent);
    void* mover = *reinterpret_cast<void**>(sub + 168);
    pongMover_Reset(mover, posResult);
}


// ===========================================================================
// SECTION 30 — GetFrameIndexDelta  @ 0x821A7A48 | size: 0x58 (88 bytes)
// ===========================================================================
/**
 * pongPlayer::GetFrameIndexDelta(void* frameData)
 *
 * Computes a frame index delta from two 16-bit fields at frameData+4 and +6.
 * If both are -1 (0xFFFF), returns 0 (no frames).
 * Otherwise computes (field4 - field6 + 1), wrapping by +20 if field4 < field6.
 */
int pongPlayer::GetFrameIndexDelta(void* frameData) {  // pongPlayer_7A48_g @ 0x821A7A48
    int16_t field4 = *reinterpret_cast<int16_t*>(
        reinterpret_cast<uintptr_t>(frameData) + 4);
    uint16_t field6 = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<uintptr_t>(frameData) + 6);

    if (field4 == -1 && field6 == 0xFFFF) {
        return 0;
    }

    int16_t end = static_cast<int16_t>(field6);
    int diff = static_cast<int>(field4) - static_cast<int>(end);
    if (field4 >= end) {
        return diff + 1;
    }
    return diff + 21;
}


// ===========================================================================
// SECTION 31 — ComputePositionWithOffsets  @ 0x821D62A8 | size: 0x60 (96 bytes)
// ===========================================================================
/**
 * pongPlayer::ComputePositionWithOffsets(void* inputData)
 *
 * Computes a combined position vector by calling pongPlayer_ComputeTargetPosition to get
 * a base target vector, then adding the offset vectors at this+224 (vec4)
 * and this+240 (vec4), storing the result at this+32.
 *
 * result = 6470_g(self, inputData) + vec4@(self+224) + vec4@(self+240)
 */
void pongPlayer::ComputePositionWithOffsets(void* inputData) {  // pongPlayer_62A8_g @ 0x821D62A8
    float tempVec[4];
    pongPlayer_ComputeTargetPosition(reinterpret_cast<vec3*>(tempVec), inputData);

    // TODO: offsets +224, +240, +32 are beyond confirmed pongPlayer layout —
    //       this function may operate on a different sub-object type
    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    float* offset1 = reinterpret_cast<float*>(base + 224);
    float* offset2 = reinterpret_cast<float*>(base + 240);
    float* dest    = reinterpret_cast<float*>(base + 32);

    // vaddfp: result = temp + offset1 + offset2
    dest[0] = tempVec[0] + offset1[0] + offset2[0];
    dest[1] = tempVec[1] + offset1[1] + offset2[1];
    dest[2] = tempVec[2] + offset1[2] + offset2[2];
    dest[3] = tempVec[3] + offset1[3] + offset2[3];
}


// ===========================================================================
// SECTION 32 — ComputePositionWithOffset  @ 0x821D6308 | size: 0x54 (84 bytes)
// ===========================================================================
/**
 * pongPlayer::ComputePositionWithOffset()
 *
 * Similar to ComputePositionWithOffsets but only adds one offset vector
 * (at this+224).  Calls pongPlayer_ComputeTargetPosition with r5=0 (no input data),
 * then adds vec4@(this+224), storing the result at this+32.
 *
 * result = 6470_g(self, NULL) + vec4@(self+224)
 */
void pongPlayer::ComputePositionWithOffset() {  // pongPlayer_6308_g @ 0x821D6308
    float tempVec[4];
    pongPlayer_ComputeTargetPosition(reinterpret_cast<vec3*>(tempVec), nullptr);

    // TODO: offsets +224, +32 are beyond confirmed pongPlayer layout
    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    float* offset = reinterpret_cast<float*>(base + 224);
    float* dest   = reinterpret_cast<float*>(base + 32);

    // vaddfp: result = temp + offset
    dest[0] = tempVec[0] + offset[0];
    dest[1] = tempVec[1] + offset[1];
    dest[2] = tempVec[2] + offset[2];
    dest[3] = tempVec[3] + offset[3];
}


// ===========================================================================
// SECTION 33 — CheckHandednessDifference  @ 0x821E04B0 | size: 0x54 (84 bytes)
// ===========================================================================
/**
 * pongPlayer::CheckHandednessDifference()
 *
 * Calls pongPlayer_GetInputStepIndex to get an index, uses it to look up a 4-byte
 * entry in the table at this+796.  XORs byte[0] with byte[3] of that entry;
 * if the XOR result > 127, returns true (significant difference), else false.
 */
extern int pongPlayer_GetInputStepIndex(pongPlayer* p);

bool pongPlayer::CheckHandednessDifference() {  // pongPlayer_04B0_g @ 0x821E04B0
    int index = pongPlayer_GetInputStepIndex(this);

    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    uint8_t* tableBase = *reinterpret_cast<uint8_t**>(base + 796);
    uint8_t* entry = tableBase + (index * 4);

    uint8_t byte3 = entry[3];
    uint8_t byte0 = entry[0];
    uint8_t diff = byte3 ^ byte0;

    return diff > 127;
}


// (GetStateObjectByIndex already defined above in SECTION 20)

// ===========================================================================
// SECTION 35 — SyncByteField  @ 0x82199108 | size: 0x7C (124 bytes)
// ===========================================================================
/**
 * pongPlayer::SyncByteField(void* target, void* source)
 *
 * Compares byte[0] of target with byte[0] of source.  If different:
 *   - If field +8 (callback pointer) is non-null, invokes the callback
 *     with field +4 as the argument (marks the field dirty for network sync).
 *   - Copies the new byte value from source to target.
 */
void pongPlayer::SyncByteField(void* target, void* source) {  // pongPlayer_9108_g @ 0x82199108
    uint8_t* tgt = static_cast<uint8_t*>(target);
    uint8_t oldVal = tgt[0];
    uint8_t newVal = *static_cast<uint8_t*>(source);

    if (oldVal != newVal) {
        void* callback = *reinterpret_cast<void**>(tgt + 8);
        if (callback) {
            void* arg = *reinterpret_cast<void**>(tgt + 4);
            reinterpret_cast<void(*)(void*)>(callback)(arg);
        }
        tgt[0] = newVal;
    }
}


// ===========================================================================
// SECTION 36 — SyncFloatField  @ 0x8219CF10 | size: 0x7C (124 bytes)
// ===========================================================================
/**
 * pongPlayer::SyncFloatField(void* target, void* source)
 *
 * Compares float[0] of target with float[0] of source.  If different:
 *   - If field +8 (callback pointer) is non-null, invokes the callback
 *     with field +4 as the argument (marks the field dirty for network sync).
 *   - Copies the new float value from source to target.
 */
void pongPlayer::SyncFloatField(void* target, void* source) {  // pongPlayer_CF10_g @ 0x8219CF10
    uint8_t* tgt = static_cast<uint8_t*>(target);
    float oldVal = *reinterpret_cast<float*>(tgt);
    float newVal = *reinterpret_cast<float*>(source);

    if (oldVal != newVal) {
        void* callback = *reinterpret_cast<void**>(tgt + 8);
        if (callback) {
            void* arg = *reinterpret_cast<void**>(tgt + 4);
            reinterpret_cast<void(*)(void*)>(callback)(arg);
        }
        *reinterpret_cast<float*>(tgt) = newVal;
    }
}


// ===========================================================================
// SECTION 37 — UpdateDirtyFlags  @ 0x82199E48 | size: 0x5C (92 bytes)
// ===========================================================================
/**
 * pongPlayer::UpdateDirtyFlags(void* obj, uint8_t forceReset)
 *
 * Manages three byte flags at obj+40, obj+41, and obj+42 that control
 * dirty-state tracking for network synchronisation.
 *
 * If forceReset is non-zero: clears +40 and +42, sets +41 to 1.
 * If forceReset is zero:
 *   - If +40 is already set, or +41 is already set: already dirty, return.
 *   - If both +40 and +41 are clear: sets +41 to 1 (mark as needing sync).
 */
void pongPlayer::UpdateDirtyFlags(void* obj, uint8_t forceReset) {  // pongPlayer_9E48_g @ 0x82199E48
    uint8_t* base = static_cast<uint8_t*>(obj);
    uint8_t* flag40 = base + 40;
    uint8_t* flag41 = base + 41;
    uint8_t* flag42 = base + 42;

    if (forceReset) {
        *flag40 = 0;
        *flag42 = 0;
        *flag41 = 1;
        return;
    }

    if (*flag40 != 0 || *flag41 != 0) {
        // Already dirty
        return;
    }

    // Neither flag set -- mark as needing sync
    *flag41 = 1;
}


// ===========================================================================
// SECTION 38 — MarkNetDirtyPosition  @ 0x82198AC0 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyPosition()
 *
 * If network dirty tracking is enabled (m_bNetDirtyEnabled != 0),
 * sets bit 0 of the network dirty flags word.  This marks the
 * player's position data as needing replication.
 *
 * Called from pongPlayer_Process to flag position changes.
 */
void pongPlayer::MarkNetDirtyPosition() {  // pongPlayer_8AC0_p33 @ 0x82198AC0
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x1;
}


// ===========================================================================
// SECTION 39 — MarkNetDirtyRotation  @ 0x82198AE0 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyRotation()
 *
 * If network dirty tracking is enabled, sets bit 1 of the network
 * dirty flags word.  Marks the player's rotation data for replication.
 */
void pongPlayer::MarkNetDirtyRotation() {  // pongPlayer_8AE0_p33 @ 0x82198AE0
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x2;
}


// ===========================================================================
// SECTION 40 — SyncFieldWithCallback  @ 0x82199EA8 | size: 0x7C (124 bytes)
// ===========================================================================
/**
 * pongPlayer::SyncFieldWithCallback(void* target, void* source)
 *
 * Compares the uint32_t value at target+0 with source+0.  If they differ
 * and a callback pointer is stored at target+8, the callback is invoked
 * with the argument from target+4.  The new value from source is then
 * written to target+0.
 *
 * Used by the network sync system to update fields and fire change
 * notifications (e.g. animation state transitions, score changes).
 */
void pongPlayer::SyncFieldWithCallback(void* target, void* source) {  // pongPlayer_9EA8_g @ 0x82199EA8
    uint8_t* tgt = static_cast<uint8_t*>(target);
    uint32_t* targetVal = reinterpret_cast<uint32_t*>(tgt);
    uint32_t* sourceVal = static_cast<uint32_t*>(source);

    if (*targetVal == *sourceVal)
        return;

    // Check for a registered callback at target+8
    void* callback = *reinterpret_cast<void**>(tgt + 8);
    if (callback) {
        void* arg = *reinterpret_cast<void**>(tgt + 4);
        reinterpret_cast<void(*)(void*)>(callback)(arg);
    }

    // Store the new value
    *targetVal = *sourceVal;
}


// ===========================================================================
// SECTION 41 — ResetShotSyncFields  @ 0x82199310 | size: 0x7C (124 bytes)
// ===========================================================================
/**
 * pongPlayer::ResetShotSyncFields(void* shotState, uint8_t clearNetFlags)
 *
 * Resets the float-typed sync fields within a shot tracking sub-object.
 * Zeroes out two float sync entries (at shotState+8 and shotState+20)
 * via SyncFloatField, stores 0.0 at shotState+32, clears byte flag
 * at shotState+36, then calls SyncFieldWithCallback on the sub-object
 * at shotState+44 passing the clearNetFlags argument.
 *
 * Called from ResetShotTrackingState and the main shot processing loop.
 */
void pongPlayer::ResetShotSyncFields(void* shotState, uint8_t clearNetFlags) {  // pongPlayer_9310_g @ 0x82199310
    extern const float g_kZeroFloat;  // @ 0x82079D10 (0.0f)

    float zero = g_kZeroFloat;

    uint8_t* shot = static_cast<uint8_t*>(shotState);

    // Reset first float sync field at shotState+8
    float syncBuf = zero;
    SyncFloatField(shot + 8, &syncBuf);

    // Reset second float sync field at shotState+20
    syncBuf = zero;
    SyncFloatField(shot + 20, &syncBuf);

    // Clear float at shotState+32 (accumulated timing)
    *reinterpret_cast<float*>(shot + 32) = zero;

    // Clear byte flag at shotState+36
    shot[36] = 0;

    // Sync the sub-field at shotState+44 with the clearNetFlags argument
    uint32_t flagVal = clearNetFlags;
    SyncFieldWithCallback(shot + 44, &flagVal);
}


// ===========================================================================
// SECTION 42 — ResetShotTrackingState  @ 0x821992A0 | size: 0x70 (112 bytes)
// ===========================================================================
/**
 * pongPlayer::ResetShotTrackingState()
 *
 * Full reset of the player's shot tracking subsystem.  Called from
 * pongPlayer_Process when transitioning to an idle state:
 *
 *   1. Calls ResetShotSyncFields to zero the float sync entries.
 *   2. Calls ResetButtonSlotTrackingData twice (with indices 0 and 1) to reset
 *      per-player-slot shot data using the global button state table.
 *   3. Clears the shot result dword (+72), and three byte flags at
 *      +69, +76, +77 (shot active, pending, committed).
 */
void pongPlayer::ResetShotTrackingState() {  // pongPlayer_92A0_g @ 0x821992A0
    // Reset all shot sync fields
    uint8_t clearFlags = 0;
    ResetShotSyncFields(this, clearFlags);

    // Reset shot data for both player slots
    ResetButtonSlotTrackingData(reinterpret_cast<void*>(g_pButtonStateTable), 0);
    ResetButtonSlotTrackingData(reinterpret_cast<void*>(g_pButtonStateTable), 1);

    // Clear shot result and tracking flags
    uint8_t* selfBase = reinterpret_cast<uint8_t*>(this);
    *reinterpret_cast<uint32_t*>(&this->m_swingDirectionAdj) = 0;
    selfBase[69] = 0;
    this->m_team = 0;
    selfBase[77] = 0;
}


// ===========================================================================
// SECTION 43 — ClearSwingTrajectoryData  @ 0x8219E748 | size: 0x68 (104 bytes)
// ===========================================================================
/**
 * pongPlayer::ClearSwingTrajectoryData(void* trajectoryBlock)
 *
 * Zeroes out the swing trajectory data structure:
 *   - 4 x 16-byte vector slots at +32..+95 (via AltiVec vxor + stvx loop)
 *   - Floats at +4 and +8 set to 0.0
 *   - 16-byte vector at +16 zeroed
 *   - Global 16-byte vector at a fixed address zeroed
 *   - Bytes at +0 and +1 cleared
 *
 * This is a full initialisation of the trajectory prediction buffer
 * used during swing animation playback.
 */
void pongPlayer::ClearSwingTrajectoryData(void* trajectoryBlock) {  // pongPlayer_E748_p33 @ 0x8219E748
    uint8_t* base = reinterpret_cast<uint8_t*>(trajectoryBlock);

    // Zero 4 x 16-byte vector slots at +32..+95
    memset(base + 32, 0, 64);

    // Zero floats at +4 and +8
    *reinterpret_cast<float*>(base + 4) = 0.0f;
    *reinterpret_cast<float*>(base + 8) = 0.0f;

    // Zero 16-byte vector at +16
    memset(base + 16, 0, 16);

    // Zero global trajectory vector
    extern uint8_t g_swingTrajectoryGlobal[];  // @ 0x825C6720
    memset(g_swingTrajectoryGlobal, 0, 16);

    // Clear byte flags at +0 and +1
    base[1] = 0;
    base[0] = 0;
}


// ===========================================================================
// SECTION 44 — ResetSwingTrackingState  @ 0x8219FA40 | size: 0x74 (116 bytes)
// ===========================================================================
/**
 * pongPlayer::ResetSwingTrackingState()
 *
 * Resets the player's per-swing tracking state to default values.
 * Called from FinalizeServeSetup and pongPlayer_InitActionState (button release
 * handler) when a swing/serve is finalised or cancelled.
 *
 *   1. Zeroes floats at +4, +8 (position deltas)
 *   2. Clears byte flags at +84, +86, +85 (swing phase, type, active)
 *   3. Zeroes floats at +64, +72, +76, +80 (swing vectors/power)
 *   4. Calls ClearSwingSlotGrid on two grid sub-objects at +96 and +2800
 *   5. Clears byte flag at +5513 (swing committed)
 */
// pongPlayer_E640_g (ClearSwingSlotGrid) — implemented at end of file

void pongPlayer::ResetSwingTrackingState() {  // pongPlayer_FA40_g @ 0x8219FA40
    uint8_t* base = reinterpret_cast<uint8_t*>(this);

    // Zero position delta floats
    *reinterpret_cast<float*>(base + 4) = 0.0f;
    *reinterpret_cast<float*>(base + 8) = 0.0f;

    // Clear swing phase flags
    base[84] = 0;
    base[86] = 0;
    base[85] = 0;

    // Zero swing vector/power floats
    *reinterpret_cast<float*>(base + 64) = 0.0f;
    *reinterpret_cast<float*>(base + 72) = 0.0f;
    *reinterpret_cast<float*>(base + 76) = 0.0f;
    *reinterpret_cast<float*>(base + 80) = 0.0f;

    // Clear both swing slot grids
    pongPlayer_E640_g(base + 96);
    pongPlayer_E640_g(base + 2800);

    // Clear swing committed flag
    base[5513] = 0;
}


// ===========================================================================
// SECTION 45 — FinalizeServeSetup  @ 0x8219F9C0 | size: 0x80 (128 bytes)
// ===========================================================================
/**
 * pongPlayer::FinalizeServeSetup()
 *
 * Completes the serve setup phase by copying shot data to the global
 * character table, resetting swing tracking, and marking the player
 * as ready for the next rally point.
 *
 * Called from SetupServe, pongPlayer_2D88_g (serve confirm), and
 * pongPlayer_0780_g (serve input handler).
 *
 *   1. If the player's character slot index (at m_pParent+464) is valid
 *      (!= -1), copies shot data into the character table via
 *      pongPlayer_5F70_g.
 *   2. Stores the player's shot type byte (+5504) into the character
 *      table at the corresponding slot.
 *   3. Clears the serve pending flag (m_pParent+6380).
 *   4. Resets swing tracking state via ResetSwingTrackingState.
 *   5. Clears byte at +5512 and sets +5504 to 1 (default shot type).
 *   6. Calls ref_fi_FCD8 to finalise the serve FSM transition.
 */
extern void pongPlayer_5F70_g(void* charSlotEntry);  // copy shot data to char table
extern void ref_fi_FCD8(void* self);                  // finalise serve FSM

void pongPlayer::FinalizeServeSetup() {  // pongPlayer_F9C0_g @ 0x8219F9C0
    // g_ballSplashArray declared at file scope @ 0x8261A3D0
    uint8_t* base = reinterpret_cast<uint8_t*>(this);

    // Read parent object pointer (this+44) and get character slot index
    uint8_t* parent = *reinterpret_cast<uint8_t**>(base + 44);
    int32_t charSlot = *reinterpret_cast<int32_t*>(parent + 464);

    // If character slot is valid, copy shot data to the character table
    if (charSlot != -1) {
        void* charEntry = g_ballSplashArray + 48 + charSlot * 416;
        pongPlayer_5F70_g(charEntry);
    }

    // Store shot type byte into character table at the slot's offset +264
    uint8_t shotType = base[5504];
    g_ballSplashArray[264 + charSlot * 416] = shotType;

    // Clear serve pending flag on parent
    parent[6380] = 0;

    // Reset swing tracking state
    ResetSwingTrackingState();

    // Clear byte at +5512, set shot type to 1 (default)
    base[5512] = 0;
    base[5504] = 1;

    // Finalise serve FSM transition
    ref_fi_FCD8(this);
}


// ===========================================================================
// SECTION 46 — LookupElementByFloatKey  @ 0x821EB1E0 | size: 0x90 (144 bytes)
// ===========================================================================
/**
 * pongPlayer::LookupElementByFloatKey(void* table, float key)
 *
 * Given a table structure with:
 *   +24 : pointer to an array of uint32_t element pointers
 *   +28 : uint16_t element count
 *
 * Returns the element pointer at the index corresponding to `key`,
 * clamped to [0, count-1].  If key >= count or is negative/NaN,
 * returns the first element (index 0).
 *
 * Called from 32+ animation/physics lookup functions that index into
 * per-frame data arrays using a normalised float time value.
 */
void* pongPlayer::LookupElementByFloatKey(void* table, float key) {  // pongPlayer_B1E0_g @ 0x821EB1E0
    uint8_t* tbl = static_cast<uint8_t*>(table);
    uint16_t count = *reinterpret_cast<uint16_t*>(tbl + 28);
    uint32_t* elements = *reinterpret_cast<uint32_t**>(tbl + 24);

    float countF = static_cast<float>(static_cast<int32_t>(count));

    // Check if key is within [0, count)
    if (key + 0.5f <= countF && !(key < 0.0f)) {
        // Clamp key to [0, count-1] and convert to index
        float clamped = key;
        if (clamped < 0.0f)
            clamped = 0.0f;
        float maxVal = static_cast<float>(count);
        if (clamped >= maxVal)
            clamped = maxVal;
        int32_t index = static_cast<int32_t>(clamped);

        return reinterpret_cast<void*>(elements[index]);
    }

    // Fallback: return first element
    return reinterpret_cast<void*>(elements[0]);
}


// ===========================================================================
// SECTION 47 — RegisterDebugDrawEntries  @ 0x8218BB38 | size: 0xA4 (164 bytes)
// ===========================================================================
/**
 * pongPlayer::RegisterDebugDrawEntries()
 *
 * Registers the player's debug draw objects with the global debug
 * render system.  Called from pongPlayer_Activate and pongPlayer_B6F0_g
 * (initialisation path).
 *
 * Adds up to 3 entries to the global debug draw table:
 *   1. Skeleton debug draw object (at this+6208, offset +4), type 1031, priority 128
 *   2. If m_pParent field +248 is non-null: that object, type 15, priority 128
 *   3. Player base object (this+4), type 1703, priority 128
 *
 * The global table has three parallel arrays (pointer, type, priority)
 * indexed by a shared counter at a fixed SDA address.
 */
void pongPlayer::RegisterDebugDrawEntries() {  // pongPlayer_BB38_g @ 0x8218BB38
    extern uint32_t  g_debugDrawCount;        // @ SDA+14436
    extern uint32_t  g_debugDrawPtrs[];       // @ SDA+14440  (pointer array)
    extern uint32_t  g_debugDrawTypes[];      // @ SDA+14696  (type ID array)
    extern uint8_t   g_debugDrawPriority[];   // @ SDA+14952  (priority array)

    uint8_t* base = reinterpret_cast<uint8_t*>(this);

    // Get skeleton debug draw object (this+6208 -> +4)
    void* skelDebugObj = *reinterpret_cast<void**>(base + 6208);
    void* skelEntry = skelDebugObj ? reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(skelDebugObj) + 4) : nullptr;

    uint32_t idx = g_debugDrawCount;

    // Entry 1: skeleton debug draw
    g_debugDrawPtrs[idx]     = reinterpret_cast<uintptr_t>(skelEntry);
    g_debugDrawTypes[idx]    = 1031;
    g_debugDrawPriority[idx] = 128;
    idx++;

    // Entry 2: optional sub-object from m_pParent field +248
    void* parentState = *reinterpret_cast<void**>(base + 452);
    void* subObj = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(parentState) + 248);
    if (subObj) {
        g_debugDrawPtrs[idx]     = reinterpret_cast<uintptr_t>(subObj);
        g_debugDrawTypes[idx]    = 15;
        g_debugDrawPriority[idx] = 128;
        idx++;
    }

    // Entry 3: player base object (this+4)
    g_debugDrawPtrs[idx]     = reinterpret_cast<uintptr_t>(base + 4);
    g_debugDrawTypes[idx]    = 1703;
    g_debugDrawPriority[idx] = 128;
    idx++;

    g_debugDrawCount = idx;
}


// ===========================================================================
// SECTION 38b — MarkNetDirtyVelocity  @ 0x82198B00 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyVelocity()
 *
 * If network dirty tracking is enabled (m_bNetDirtyEnabled != 0),
 * sets bit 2 of the network dirty flags word.  Marks the player's
 * velocity data as needing replication.
 */
void pongPlayer::MarkNetDirtyVelocity() {  // pongPlayer_8B00_p33 @ 0x82198B00
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x4;
}


// ===========================================================================
// SECTION 38c — MarkNetDirtyAnimation  @ 0x82198B20 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyAnimation()
 *
 * If network dirty tracking is enabled, sets bit 3 of the network
 * dirty flags word.  Marks the player's animation state for replication.
 */
void pongPlayer::MarkNetDirtyAnimation() {  // pongPlayer_8B20_p33 @ 0x82198B20
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x8;
}


// ===========================================================================
// SECTION 38d — MarkNetDirtyShotState  @ 0x82198B40 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyShotState()
 *
 * If network dirty tracking is enabled, sets bit 4 of the network
 * dirty flags word.  Marks the player's shot/swing state for replication.
 */
void pongPlayer::MarkNetDirtyShotState() {  // pongPlayer_8B40_p33 @ 0x82198B40
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x10;
}


// ===========================================================================
// VTABLE THUNKS — MarkNetDirty redirects
// ===========================================================================
// Seven adjacent 16-byte vtable forwarding thunks @ 0x82199188-91F4.
// Each is a tail call (lis/addi/mtctr/bctr) redirecting a secondary
// vtable slot to the corresponding MarkNetDirty implementation.
// ===========================================================================

/**
 * Vtable thunk → MarkNetDirtyPosition  @ 0x82199188 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8AC0_p33 @ 0x82198AC0.
 */
void pongPlayer::VtMarkNetDirtyPosition() {  // pongPlayer_9188_2hr @ 0x82199188
    MarkNetDirtyPosition();
}

/**
 * Vtable thunk → MarkNetDirtyRotation  @ 0x82199198 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8AE0_p33 @ 0x82198AE0.
 */
void pongPlayer::VtMarkNetDirtyRotation() {  // pongPlayer_9198_2h @ 0x82199198
    MarkNetDirtyRotation();
}

/**
 * Vtable thunk → MarkNetDirtyVelocity  @ 0x821991A8 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8B00_p33 @ 0x82198B00.
 */
void pongPlayer::VtMarkNetDirtyVelocity() {  // pongPlayer_91A8_2h @ 0x821991A8
    MarkNetDirtyVelocity();
}

/**
 * Vtable thunk → MarkNetDirtyAnimation  @ 0x821991B8 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8B20_p33 @ 0x82198B20.
 */
void pongPlayer::VtMarkNetDirtyAnimation() {  // pongPlayer_91B8_2h @ 0x821991B8
    MarkNetDirtyAnimation();
}

/**
 * Vtable thunk → MarkNetDirtyShotState  @ 0x821991C8 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8B40_p33 @ 0x82198B40.
 */
void pongPlayer::VtMarkNetDirtyShotState() {  // pongPlayer_91C8_2h @ 0x821991C8
    MarkNetDirtyShotState();
}

/**
 * Vtable thunk → MarkNetDirtyStamina  @ 0x821991D8 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8B60_p33 @ 0x82198B60.
 * Target inlined: m_netDirtyFlags |= 0x20 (bit 5).
 */
void pongPlayer::VtMarkNetDirtyStamina() {  // pongPlayer_91D8_2h @ 0x821991D8
    MarkNetDirtyStamina();
}

/**
 * Vtable thunk → MarkNetDirtyScore  @ 0x821991E8 | size: 0x10 (16 bytes)
 * Tail call redirect to pongPlayer_8B80_p33 @ 0x82198B80.
 * Target inlined: m_netDirtyFlags |= 0x40 (bit 6).
 */
void pongPlayer::VtMarkNetDirtyScore() {  // pongPlayer_91E8_2h @ 0x821991E8
    MarkNetDirtyScore();
}


// ===========================================================================
// SECTION — MarkNetDirtyStamina  @ 0x82198B60 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyStamina()
 *
 * If network dirty tracking is enabled (m_bNetDirtyEnabled != 0),
 * sets bit 5 (0x20) of the network dirty flags word.  This marks the
 * player's stamina data as needing replication.
 *
 * Same pattern as MarkNetDirtyPosition (0x1) and MarkNetDirtyRotation (0x2).
 */
void pongPlayer::MarkNetDirtyStamina() {  // pongPlayer_8B60_p33 @ 0x82198B60
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x20;
}


// ===========================================================================
// SECTION — MarkNetDirtyScore  @ 0x82198B80 | size: 0x1C (28 bytes)
// ===========================================================================
/**
 * pongPlayer::MarkNetDirtyScore()
 *
 * If network dirty tracking is enabled (m_bNetDirtyEnabled != 0),
 * sets bit 6 (0x40) of the network dirty flags word.  This marks the
 * player's score data as needing replication.
 */
void pongPlayer::MarkNetDirtyScore() {  // pongPlayer_8B80_p33 @ 0x82198B80
    if (!m_bNetDirtyEnabled)
        return;
    m_netDirtyFlags |= 0x40;
}


// ===========================================================================
// SECTION — Update  @ 0x8218CF08 | size: 0x28 (40 bytes)
// ===========================================================================
/**
 * pongPlayer::Update()
 *
 * Top-level update entry point.  Checks if the player is initialised
 * (pointer at +0x2C non-null, via big-endian MSB byte check), then
 * dispatches to the virtual Update1 method (vtable slot 4, byte offset +16).
 *
 * If the player is not initialised, calls the debug log no-op and returns.
 */
void pongPlayer::Update() {  // pongPlayer_Update @ 0x8218CF08
    if (!m_pLocomotionState) {
        rage_debugLog("pongPlayer::Update() - player not initialized");
        return;
    }
    // Virtual dispatch to Update1 (vtable slot 4, byte offset +16)
    typedef void (*UpdateFn)(pongPlayer*);
    UpdateFn fn = reinterpret_cast<UpdateFn>(vtable[4]);
    fn(this);
}


// ===========================================================================
// SECTION — UpdateReplay  @ 0x8218E178 | size: 0x64 (100 bytes)
// ===========================================================================
/**
 * pongPlayer::UpdateReplay()
 *
 * Called during replay playback to update the player's state.
 * Checks if the player is initialised (same +0x2C null check as Update).
 * Then calls pongCreature_59C0_g on the opponent creature, followed by
 * pongPlayer_4C08_g on self to synchronise replay state.
 */
extern void pongCreature_59C0_g(void* creature);    // @ 0x820C59C0
extern void pongPlayer_4C08_g(pongPlayer* player);  // @ 0x82194C08

void pongPlayer::UpdateReplay() {  // pongPlayer_UpdateReplay @ 0x8218E178
    if (!m_pLocomotionState) {
        rage_debugLog("pongPlayer::UpdateReplay() - player is not active");
        return;
    }
    pongCreature_59C0_g(m_pOpponent);
    pongPlayer_4C08_g(this);
}


// ===========================================================================
// SECTION — pongPlayer_0C58_g  @ 0x821A0C58 | size: 0x50 (80 bytes)
// ===========================================================================
/**
 * pongPlayer_0C58_g()
 *
 * Checks whether the ball splash 2D effect for the current player's input
 * slot is active.  Loads the current input slot index from the SDA global,
 * indexes into g_ballSplashArray (416 bytes per entry, offset 48 from base),
 * and calls fxBallSplash2D_5998_g to query the active state.
 *
 * Returns: 1 if the splash effect is active (fxBallSplash2D returns 1),
 *          0 otherwise.
 */
extern int32_t  g_currentInputSlot;                              // @ SDA+25976 (0x82606578)
extern int      fxBallSplash2D_5998_g(void* splashEntry);       // @ 0x821D5998

uint8_t pongPlayer_0C58_g() {  // @ 0x821A0C58
    int32_t slot = g_currentInputSlot;
    uint8_t* entry = g_ballSplashArray + 48 + slot * 416;
    return (fxBallSplash2D_5998_g(entry) == 1) ? 1 : 0;
}


// ===========================================================================
// SECTION — ResetSwingDisplayValues  @ 0x821A7038 | size: 0x54 (84 bytes)
// ===========================================================================
/**
 * pongPlayer::ResetSwingDisplayValues()
 *
 * Resets the swing display sub-state at (this+5404) via pongPlayer_76E8_g,
 * then initialises three float fields at +5600, +5604, +5608 with
 * constant values from .rdata.
 */
extern void pongPlayer_76E8_g(void* swingDisplay);   // @ 0x821A76E8
extern const float g_kSwingDisplay0;                 // @ 0x82079AD4 [.rdata]
extern const float g_kSwingDisplay1;                 // @ 0x82079AD8 [.rdata]
extern const float g_kSwingDisplay2;                 // @ 0x82085938 [.rdata]

void pongPlayer::ResetSwingDisplayValues() {  // pongPlayer_7038 @ 0x821A7038
    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    pongPlayer_76E8_g(base + 5404);
    *reinterpret_cast<float*>(base + 5600) = g_kSwingDisplay0;
    *reinterpret_cast<float*>(base + 5604) = g_kSwingDisplay1;
    *reinterpret_cast<float*>(base + 5608) = g_kSwingDisplay2;
}


// ===========================================================================
// SECTION — pongPlayer_D238_g  @ 0x820DD238 | size: 0x60 (96 bytes)
// ===========================================================================
/**
 * pongPlayer_D238_g(void* self)
 *
 * Computes the normalised animation time / recovery progress for the
 * player's recovery state.  Follows a pointer chain at self+36 to reach
 * the inner timing data:
 *   - If the sub-object at +36 is NULL, returns g_recoveryTimerThreshold.
 *   - If the total duration (sub->inner+12) is ≤ g_swingPhaseThreshold,
 *     returns g_swingPhaseThreshold.
 *   - Otherwise returns: (float)(frameCount @ sub+32) * g_kFrameScale
 *                         / (totalDuration @ sub->inner+12)
 */
extern const float g_kFrameScale;    // @ 0x82079BC8 [.rdata]

float pongPlayer_D238_g(void* self) {  // @ 0x820DD238
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    void* subObj = *reinterpret_cast<void**>(base + 36);

    if (!subObj) {
        return g_recoveryTimerThreshold;  // @ 0x8202D108
    }

    uint8_t* sub = reinterpret_cast<uint8_t*>(subObj);
    void* innerObj = *reinterpret_cast<void**>(sub + 36);
    float totalDuration = *reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(innerObj) + 12);

    if (totalDuration <= g_swingPhaseThreshold) {  // @ 0x8202D110
        return g_swingPhaseThreshold;
    }

    int32_t frameCount = *reinterpret_cast<int32_t*>(sub + 32);
    return (float)frameCount * g_kFrameScale / totalDuration;
}


// ===========================================================================
// SECTION — pongPlayer_1510_g  @ 0x82391510 | size: 0x5C (92 bytes)
// ===========================================================================
/**
 * pongPlayer_1510_g(void* self)
 *
 * Processes a network client wrapper object.  Loads the inner object from
 * self+12, calls its virtual Process method (vtable slot 2) which returns
 * a float, stores that result at inner+12, then calls
 * SinglesNetworkClient_BE30_g(inner+16, 50) to update the network client.
 *
 * Returns: pointer to the network client sub-object (inner+16).
 */
extern void SinglesNetworkClient_BE30_g(void* client, int param);  // @ 0x8239BE30

void* pongPlayer_1510_g(void* self) {  // @ 0x82391510
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    void* innerObj = *reinterpret_cast<void**>(base + 12);
    uint8_t* inner = reinterpret_cast<uint8_t*>(innerObj);

    // Call vtable slot 2 (Process) on inner object — returns float
    void** innerVtable = *reinterpret_cast<void***>(inner);
    typedef float (*ProcessFn)(void*);
    float result = reinterpret_cast<ProcessFn>(innerVtable[2])(innerObj);

    // Store process result to inner+12
    *reinterpret_cast<float*>(inner + 12) = result;

    // Update network client at inner+16 with frame count param 50
    void* netClient = reinterpret_cast<void*>(inner + 16);
    SinglesNetworkClient_BE30_g(netClient, 50);

    return netClient;
}


// ===========================================================================
// SECTION — pongPlayer_5170_w  @ 0x82285170 | size: 0x5C (92 bytes)
// ===========================================================================
/**
 * pongPlayer_5170_w(void* boundsObj)
 *
 * 2D bounds check on a physics/collision object.  Returns 1 if the float
 * values at offsets +36 and +32 are both within their respective valid
 * ranges; returns 0 otherwise (including for NaN values).
 *
 * Conditions for returning 1:
 *   - val36 ∈ [g_kBoundsLower36, g_kBoundsUpper36)
 *   - val32 ∈ [g_kBoundsLower32, g_kBoundsUpper32)
 */
extern const float g_kBoundsUpper36;   // @ 0x82079B24 [.rdata]
extern const float g_kBoundsLower36;   // @ 0x8202C4E4 [.rdata]
extern const float g_kBoundsLower32;   // @ 0x82079B20 [.rdata]
extern const float g_kBoundsUpper32;   // @ 0x8202CFF4 [.rdata]

int pongPlayer_5170_w(void* boundsObj) {  // @ 0x82285170
    uint8_t* base = reinterpret_cast<uint8_t*>(boundsObj);
    float val36 = *reinterpret_cast<float*>(base + 36);
    float val32 = *reinterpret_cast<float*>(base + 32);

    // Check val36 is in range [lower, upper)
    if (val36 >= g_kBoundsUpper36)
        return 0;
    if (!(val36 >= g_kBoundsLower36))  // NaN-safe: !(NaN >= x) is true
        return 0;

    // Check val32 is in range [lower, upper)
    if (!(val32 >= g_kBoundsLower32))  // NaN-safe
        return 0;
    if (val32 < g_kBoundsUpper32)
        return 1;

    return 0;
}


// ===========================================================================
// SECTION — pongPlayer_AED0_p39  @ 0x8218AED0 | size: 0x6C (108 bytes)
// ===========================================================================
/**
 * pongPlayer::FindRegisteredListener(void* unused, void* target)
 *
 * Searches the registered listener array (count at this+144, pointer array
 * at this+152) for a matching target pointer.  Logs a debug message
 * indicating whether the target was found or not (via the target's name
 * field at target+4).
 *
 * This is a network reconciliation function (p39 suffix) used to validate
 * listener registration state.
 *
 * Note: first explicit argument (r4) is unused in the function body.
 *       Target pointer arrives in r5 (second explicit argument).
 */
void pongPlayer::FindRegisteredListener(void* unused, void* target) {
    // pongPlayer_AED0_p39 @ 0x8218AED0
    (void)unused;
    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    int32_t count = *reinterpret_cast<int32_t*>(base + 144);
    uint32_t* array = *reinterpret_cast<uint32_t**>(base + 152);
    const char* targetName = *reinterpret_cast<const char**>(
        reinterpret_cast<uint8_t*>(target) + 4);

    int32_t foundIdx = -1;

    if (count > 0) {
        for (int32_t i = 0; i < count; i++) {
            if (foundIdx != -1) {
                // Found on previous iteration — log and return
                rage_debugLog("pongPlayer::FindRegisteredListener() found %s", targetName);
                return;
            }
            if (array[i] == reinterpret_cast<uintptr_t>(target)) {
                foundIdx = i;
            }
        }

        if (foundIdx != -1) {
            rage_debugLog("pongPlayer::FindRegisteredListener() found %s", targetName);
            return;
        }
    }

    // Not found in array
    rage_debugLog("pongPlayer::FindRegisteredListener() not found %s", targetName);
}


// ===========================================================================
// BATCH — 9 pongPlayer functions (112–148 bytes)
// Translated from PPC recompilation scaffolds
// ===========================================================================

// ── New external function declarations for this batch ───────────────────
// Court bounds + interpolation (called by pongPlayer_E7B0_g)
extern void  pongPlayer_FD20_g(void* a, int b, void* c, void* d,
                               int e, void* f, void* g, unsigned char h); // @ 0x821EFD20
// pongPlayer_E590_g — forward declared at top of file with correct 6-param signature
// Clock/config query — returns timing/state data from match config
extern void* pongPlayer_7CC0_g(void* matchConfig);                     // @ 0x821E7CC0
// Player record entry setup — configures a slot in the player data table
extern void  pongPlayer_05A8_g(void* entry, uint8_t metaByte,
                               void* clockData);                       // @ 0x821E05A8
// Normalize sub-struct extent vector (float param = threshold via f1)
extern void  pongPlayer_4EA0_g(void* sub, float normalizeVal);         // @ 0x821D4EA0
// Compute interpolated position into output buffer
extern void  pongPlayer_5010_g(void* sub, void* output,
                               void* parent, int flag);                // @ 0x821D5010

// ── New external global data for this batch ─────────────────────────────
extern uint8_t    g_replayStateData[];   // lbl_825CB3C0 @ 0x825CB3C0 (20 bytes, .data)
extern const float g_kPercentScale;      // flt_8202D118 @ 0x8202D118 (100.0f, .rdata)


// ===========================================================================
// SEC 48 — pongPlayer_9C60_g  @ 0x821C9C60 | size: 0x70 (112 bytes)
// ===========================================================================
/**
 * Registers a value into a player record slot.
 *
 * Queries match config for a clock/timing value via pongPlayer_7CC0_g,
 * reads a metadata byte from the caller's data region, looks up a slot
 * index from the player records table at [index+1], and — if valid
 * (0–3) — stores the value at entry+20 and calls pongPlayer_05A8_g
 * to configure the record.
 *
 * @param base   Pointer to caller's data region (byte at base+index+64)
 * @param index  Index into the slot table and byte-data offset
 * @param value  Value to store at the record entry's offset +20
 */
extern "C" void pongPlayer_9C60_g(void* base, int index, uint32_t value) {
    // Get timing state from match config
    void* clockData = pongPlayer_7CC0_g(g_pMatchConfig);

    // Read metadata byte from caller's data region
    uint8_t metaByte = *reinterpret_cast<uint8_t*>(
        reinterpret_cast<uintptr_t>(base) + index + 64);

    // Look up slot index from player records table
    uint32_t* slotTable = reinterpret_cast<uint32_t*>(g_pPlayerRecords);
    int32_t slotIdx = static_cast<int32_t>(slotTable[index + 1]);

    if (slotIdx >= 0 && slotIdx < 4) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(g_pPlayerRecords)
                         + slotIdx * 808;
        *reinterpret_cast<uint32_t*>(entry + 20) = value;
        pongPlayer_05A8_g(entry + 12, metaByte, clockData);
    }
}


// ===========================================================================
// SEC 49 — pongPlayer_E7B0_g  @ 0x8219E7B0 | size: 0x7C (124 bytes)
// ===========================================================================
/**
 * Computes a swing target vector from court bounds.
 *
 * Input struct layout:
 *   +0:  uint8_t courtType     +1: uint8_t courtSubType
 *   +4:  float   posX          +8: float   posY
 *   +16: vec3    result (output — written by this function)
 *
 * Steps:
 *  1. Calls pongPlayer_FD20_g to resolve two court boundary vectors.
 *  2. Packs (posX, posY) into a temp and calls pongPlayer_E590_g
 *     to interpolate a position within those bounds.
 *  3. Returns pointer to the result vec3 at input+16.
 */
extern "C" void* pongPlayer_E7B0_g(void* inputStruct) {
    uint8_t* input = reinterpret_cast<uint8_t*>(inputStruct);
    uint8_t courtType    = input[0];
    uint8_t courtSubType = input[1];
    float posX = *reinterpret_cast<float*>(input + 4);
    float posY = *reinterpret_cast<float*>(input + 8);

    // Resolve court boundary vectors for the given types
    vec3 boundsA{}, boundsB{};
    pongPlayer_FD20_g(
        g_pGlobalClock, courtType, &boundsA, &boundsB, courtSubType,
        reinterpret_cast<void*>(static_cast<uintptr_t>(1)), nullptr, 0);

    // Pack current position for interpolation
    float tempPos[2] = { posX, posY };

    // Interpolate within bounds → write result at input+16
    void* result = input + 16;
    pongPlayer_E590_g(
        tempPos,
        courtType,
        reinterpret_cast<float*>(result), &boundsA.x, &boundsB.x, nullptr);

    return result;
}


// ===========================================================================
// SEC 50 — pongPlayer_8E88_g  @ 0x821C8E88 | size: 0x78 (120 bytes)
// ===========================================================================
/**
 * Returns a float value from the player state based on status flags.
 *
 * If the player is in an active state (flag byte at +334 set, or
 * counter at +340 non-zero), returns the alt value at offset +268.
 * Otherwise checks a global replay state flag (lbl_825CB3C0 +4);
 * if set → +268, else → base value at +264.
 */
extern "C" float pongPlayer_8E88_g(void* self) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    bool isActive = (base[334] != 0)
                 || (*reinterpret_cast<uint32_t*>(base + 340) != 0);

    if (isActive) {
        return *reinterpret_cast<float*>(base + 268);
    }

    // Check global replay state flag at +4
    uint32_t replayFlag = *reinterpret_cast<uint32_t*>(g_replayStateData + 4);
    if (replayFlag != 0) {
        return *reinterpret_cast<float*>(base + 268);
    }

    return *reinterpret_cast<float*>(base + 264);
}


// ===========================================================================
// SEC 51 — pongPlayer_4510_wrh  @ 0x82384510 | size: 0x78 (120 bytes)
// ===========================================================================
/**
 * Resets an array container sub-object.
 *
 * Sub-object layout:
 *   +4:  pointer to array of object pointers
 *   +8:  uint16_t element count
 *   +48: float (cleared to 0.0)   +52: byte (cleared)
 *   +56: float (cleared to 0.0)   +60: byte (cleared)
 *   +62: byte (cleared at end)
 *
 * Calls vtable slot 1 (reset/clear) on each element in the array,
 * then clears all timing/state fields.
 */
extern "C" void pongPlayer_4510_wrh(void* self) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Clear timing floats and state bytes
    *reinterpret_cast<float*>(base + 56) = 0.0f;
    *reinterpret_cast<float*>(base + 48) = 0.0f;
    base[60] = 0;
    base[52] = 0;

    int16_t count = static_cast<int16_t>(
        *reinterpret_cast<uint16_t*>(base + 8));

    if (count > 0) {
        uint32_t* array = *reinterpret_cast<uint32_t**>(base + 4);
        for (int16_t i = 0; i < count; i++) {
            // Call vtable slot 1 (reset) on each element
            void* obj = reinterpret_cast<void*>(
                static_cast<uintptr_t>(array[i]));
            void** vtable = *reinterpret_cast<void***>(obj);
            reinterpret_cast<void(*)(void*)>(vtable[1])(obj);

            // Reload count (may be modified by callback)
            count = static_cast<int16_t>(
                *reinterpret_cast<uint16_t*>(base + 8));
        }
    }

    base[62] = 0;
}


// ===========================================================================
// SEC 52 — pongPlayer_7F98_g  @ 0x821A7F98 | size: 0x84 (132 bytes)
// ===========================================================================
/**
 * Computes a completion percentage from timing sub-state fields.
 *
 * Sub-struct layout (not full pongPlayer):
 *   +4:  float currentTime (near path)
 *   +8:  float totalTime   (near path)
 *   +12: float currentTime (far path)
 *   +16: float totalTime   (far path)
 *   +28: uint8_t directionFlag
 *
 * When directionFlag != 0 (near path):
 *   Returns ((totalTime - currentTime) / totalTime) * 100.0f
 *
 * When directionFlag == 0 (far path):
 *   Returns (1.0f - ((totalTime - currentTime) / totalTime)) * 100.0f
 *
 * Returns 0.0f if any denominator is zero or negative.
 */
extern "C" float pongPlayer_7F98_g(void* self) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint8_t directionFlag = base[28];

    const float threshold = g_swingPhaseThreshold;  // 0.0f @ 0x8202D110
    const float scale     = g_kPercentScale;        // 100.0f @ 0x8202D118
    const float oneConst  = g_recoveryTimerThreshold; // 1.0f @ 0x8202D108

    if (directionFlag != 0) {
        // Near path: use fields at +4 and +8
        float current = *reinterpret_cast<float*>(base + 4);
        if (current > threshold) {
            float total = *reinterpret_cast<float*>(base + 8);
            if (total > threshold) {
                float ratio = (total - current) / total;
                return ratio * scale;
            }
        }
        // Flag set but times invalid → return 0.0f
        return threshold;
    }

    // Far path (directionFlag == 0): use fields at +12 and +16
    float current = *reinterpret_cast<float*>(base + 12);
    if (current <= threshold) return threshold;

    float total = *reinterpret_cast<float*>(base + 16);
    if (total <= threshold) return threshold;

    float ratio = (total - current) / total;
    float adjusted = oneConst - ratio;  // effectively currentTime / totalTime
    return adjusted * scale;
}


// ===========================================================================
// SEC 53 — pongPlayer_6360_g  @ 0x821D6360 | size: 0x84 (132 bytes)
// ===========================================================================
/**
 * Sets a court-position sub-struct from a boundary rectangle.
 *
 * Takes a struct (at self) with a 16-byte sub-struct at offset +64,
 * and a boundary rectangle (at bounds) with layout:
 *   +0: float x0   +4: float x1   +8: float z0   +12: float z1
 *
 * Computes:
 *   sub[0] = z1 - z0  (depth extent)
 *   sub[4] = x0 - x1  (width extent)
 *   sub[8] = 0.0f      (threshold constant)
 *
 * Then normalizes via pongPlayer_4EA0_g and computes an interpolated
 * position into sub+16 via pongPlayer_5010_g.
 */
extern "C" void pongPlayer_6360_g(void* self, void* bounds) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    float* rect   = reinterpret_cast<float*>(bounds);

    float x0 = rect[0];
    float x1 = rect[1];
    float z0 = rect[2];
    float z1 = rect[3];

    float widthDiff = x0 - x1;
    float depthDiff = z1 - z0;

    // Sub-struct at this + 64
    uint8_t* sub = base + 64;
    *reinterpret_cast<float*>(sub + 0) = depthDiff;
    *reinterpret_cast<float*>(sub + 8) = g_swingPhaseThreshold;  // 0.0f
    *reinterpret_cast<float*>(sub + 4) = widthDiff;

    // Normalize extent vector
    pongPlayer_4EA0_g(sub, g_swingPhaseThreshold);

    // Compute interpolated position → output at sub+16
    pongPlayer_5010_g(sub, sub + 16, self, 1);
}


// ===========================================================================
// SEC 54 — pongPlayer_62C0_g  @ 0x821962C0 | size: 0x88 (136 bytes)
// ===========================================================================
/**
 * Resets opponent animation/physics state flags.
 *
 * If a pending-clear flag at this+6537 is set:
 *   - Clears the flag and several timing floats at +6512/+6516/+6572
 *   - Sets bit 0 of the opponent's physicsBound flags at +64
 *   - Clears secondary flag at +6637
 *
 * Then, if the opponent exists:
 *   - If resetFlag is true:  zeros 16 bytes at physicsBound+208 and
 *     sets physicsBound+134 = 1 (enable anim tracking)
 *   - If resetFlag is false: clears physicsBound+134 = 0 (disable)
 */
extern "C" void pongPlayer_62C0_g(pongPlayer* self, uint8_t resetFlag) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    if (base[6537] != 0) {
        pongPlayer* opponent = self->m_pOpponent;
        base[6537] = 0;

        // Clear timing/interpolation floats
        *reinterpret_cast<float*>(base + 6512) = 0.0f;
        *reinterpret_cast<float*>(base + 6516) = 0.0f;
        *reinterpret_cast<float*>(base + 6572) = 0.0f;

        // Set bit 0 on opponent's physicsBound flags at +64
        uint8_t* physBound = reinterpret_cast<uint8_t*>(
            opponent->m_pPhysicsBound);
        physBound[64] |= 1;

        base[6637] = 0;
    }

    pongPlayer* opponent = self->m_pOpponent;
    if (opponent == nullptr) return;

    uint8_t* physBound = reinterpret_cast<uint8_t*>(
        opponent->m_pPhysicsBound);

    if (resetFlag != 0) {
        // Zero 16-byte animation vector at physicsBound+208
        memset(physBound + 208, 0, 16);
        physBound[134] = 1;
    } else {
        physBound[134] = 0;
    }
}


// ===========================================================================
// SEC 55 — pongPlayer_63E8_g  @ 0x821D63E8 | size: 0x88 (136 bytes)
// ===========================================================================
/**
 * Accumulates court-position extents from a boundary rectangle.
 *
 * Same as pongPlayer_6360_g but ADDS the computed deltas to the
 * existing sub-struct values instead of overwriting them:
 *   sub[0] += depthDiff  (z1 - z0)
 *   sub[4] += widthDiff  (x0 - x1)
 *
 * Then normalizes and recomputes interpolated position.
 */
extern "C" void pongPlayer_63E8_g(void* self, void* bounds) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    float* rect   = reinterpret_cast<float*>(bounds);

    float x0 = rect[0];
    float x1 = rect[1];
    float z0 = rect[2];
    float z1 = rect[3];

    float widthDiff = x0 - x1;
    float depthDiff = z1 - z0;

    // Sub-struct at this + 64
    uint8_t* sub = base + 64;

    // Accumulate deltas into existing values
    float oldDepth = *reinterpret_cast<float*>(sub + 0);
    float oldWidth = *reinterpret_cast<float*>(sub + 4);
    *reinterpret_cast<float*>(sub + 4) = oldWidth + widthDiff;
    *reinterpret_cast<float*>(sub + 0) = oldDepth + depthDiff;

    // Normalize extent vector
    pongPlayer_4EA0_g(sub, g_swingPhaseThreshold);

    // Compute interpolated position → output at sub+16
    pongPlayer_5010_g(sub, sub + 16, self, 1);
}


// ===========================================================================
// SEC 56 — pongPlayer_5800_h  @ 0x82195800 | size: 0x90 (144 bytes)
// ===========================================================================
/**
 * Checks whether the player can accept swing input this frame.
 *
 * Three-stage check:
 *  1. If the global input table entry has both flags at +212 and +213
 *     set → return false (input blocked by system).
 *  2. If the player's action state at +460 is 2, 3, or 4 (serve
 *     receive / active rally states) → return true.
 *  3. Otherwise, delegates to the opponent's player state
 *     IsBeforeSwingPeak() check.
 *
 * @return true if the player can initiate a swing, false otherwise.
 */
extern "C" bool pongPlayer_5800_h(pongPlayer* self) {
    // Look up entry in g_ballSplashArray (416-byte stride, +48 header)
    int32_t slotIdx = self->m_inputSlotIdx;
    uint8_t* entry = g_ballSplashArray + 48
                   + static_cast<uint32_t>(slotIdx) * 416;

    // Check if both input channel flags are set → blocked
    if (entry[213] != 0 && entry[212] != 0) {
        return false;
    }

    // Check action state (offset +460 = 0x1CC from this)
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int32_t actionState = *reinterpret_cast<int32_t*>(base + 460);
    if (actionState == 4 || actionState == 2 || actionState == 3) {
        return true;
    }

    // Fall through: check opponent's swing peak state
    return self->m_pOpponent->m_pPlayerState->IsBeforeSwingPeak();
}

// NOTE: pongPlayer_9CD0_g @ 0x821C9CD0 (0x94 bytes) is already
// implemented in src/game/player/pong_player_swing.cpp as
// GetCreatureBoneMatrix(), with an extern "C" wrapper.


// ===========================================================================
// ===========================================================================
//  BATCH 10 — Medium pongPlayer functions (148–220 bytes)
// ===========================================================================
// ===========================================================================


// ===========================================================================
// ClearSwingSlotGrid  @ 0x8219E640 | size: 0x94 (148 bytes)
// ===========================================================================
/**
 * Resets a swing slot grid sub-object to default state.
 *
 * Zeros 6 × 16-byte vector slots, resets two default float fields,
 * clears associated flags/counters, and writes a reset constant to
 * a global default value.
 *
 * @param gridSubObj  Pointer to swing grid sub-object
 *                    (pongPlayer base + 96 or + 2800)
 */
void pongPlayer_E640_g(void* gridSubObj) {  // ClearSwingSlotGrid @ 0x8219E640
    uint8_t* base = static_cast<uint8_t*>(gridSubObj);

    // Zero 6 × 16-byte vector slots
    memset(base + 832, 0, 16);
    memset(base + 864, 0, 16);
    memset(base + 880, 0, 16);
    memset(base + 896, 0, 16);
    memset(base + 912, 0, 16);
    memset(base + 928, 0, 16);

    // Reset float fields to constant default
    *reinterpret_cast<float*>(base + 848) = g_kFloatConst_D110;
    *reinterpret_cast<float*>(base + 852) = g_kFloatConst_D110;

    // Clear flags and counter
    base[956] = 0;
    base[968] = 0;
    *reinterpret_cast<uint32_t*>(base + 972) = 0;

    // Write reset constant to global
    g_swingSlotGridDefault = g_kFloatConst_D168;
}


// ===========================================================================
// InterpolateGridPosition  @ 0x8219E590 | size: 0xB0 (176 bytes)
// ===========================================================================
/**
 * Computes an interpolated court grid position.
 *
 * Lerps X/Z between two boundary corners based on a flip flag,
 * sets Y from the court geometry singleton, and adds an offset vector.
 *
 * @param lerpFactors  2 floats: X lerp at [0], Z lerp at [1]
 * @param flip         0 = lerp A->B, nonzero = lerp B->A
 * @param outPos       Output vec3 (writes X at +0, Y at +4, Z at +8)
 * @param boundsA      Corner A (reads X at +0, Z at +8)
 * @param boundsB      Corner B (reads X at +0, Z at +8)
 * @param offset       Offset vec3 (added to final result)
 */
void pongPlayer_E590_g(float* lerpFactors, uint8_t flip, float* outPos,
                       float* boundsA, float* boundsB,
                       float* offset) {  // InterpolateGridPosition @ 0x8219E590
    // Load court height from global singleton (+20 bytes from base)
    float* courtSingleton = static_cast<float*>(g_pCourtGeomSingleton);
    float courtHeight = courtSingleton[5];

    // Y = court height + offset constant
    outPos[1] = courtHeight + g_kCourtHeightOffset;

    if (flip) {
        // Lerp from boundsB toward boundsA
        float rangeX = boundsA[0] - boundsB[0];
        outPos[0] = boundsB[0] + lerpFactors[0] * rangeX;

        float rangeZ = boundsA[2] - boundsB[2];
        outPos[2] = boundsB[2] + lerpFactors[1] * rangeZ;
    } else {
        // Lerp from boundsA toward boundsB
        float rangeX = boundsB[0] - boundsA[0];
        outPos[0] = boundsA[0] + lerpFactors[0] * rangeX;

        float rangeZ = boundsB[2] - boundsA[2];
        outPos[2] = boundsA[2] + lerpFactors[1] * rangeZ;
    }

    // Add offset vector (PPC vaddfp: component-wise float add)
    outPos[0] += offset[0];
    outPos[1] += offset[1];
    outPos[2] += offset[2];
}


// ===========================================================================
// SetServeSideFlip  @ 0x82193628 | size: 0xAC (172 bytes)
// ===========================================================================
/**
 * Stores the serve-side flip flag to geometry tables and propagates
 * to the opponent's record if an active opponent slot exists.
 *
 * Called from ResetServePosition to configure serve direction.
 *
 * @param player  pongPlayer whose serve side is being set
 * @param flip    0 or 1 — serve-side flip flag
 */
void pongPlayer_3628_g(pongPlayer* player, uint8_t flip) {  // SetServeSideFlip @ 0x82193628
    int32_t slotIdx = player->m_inputSlotIdx;

    // Store flip to geometry base: g_pInputObj[slotIdx] + 64
    uint8_t* geomBase = static_cast<uint8_t*>(g_pInputObj) + slotIdx;
    geomBase[64] = flip;

    // Store flip to court data table: g_ballSplashArray + 262 + slotIdx * 416
    g_ballSplashArray[262 + slotIdx * 416] = flip;

    // Check if opponent slot is active (word at g_pInputObj[(slotIdx+14)*4])
    uint32_t* inputSlots = reinterpret_cast<uint32_t*>(g_pInputObj);
    bool opponentActive = (inputSlots[slotIdx + 14] == 1);

    if (opponentActive) {
        // Look up opponent entry from player data table
        uint32_t* dataEntries = reinterpret_cast<uint32_t*>(g_pPlayerDataTable);
        uint32_t opponentEntry = dataEntries[slotIdx + 1];

        // Get player handedness from slot lookup
        void* handedness = pongPlayer_7CC0_g(g_pPlayerSlotLookup);

        // Compute opponent record base: table + entry*808 + 12
        void* recordBase = g_pPlayerDataTable + opponentEntry * 808 + 12;

        // Apply serve-side flip to opponent record
        pongPlayer_05A8_g(recordBase, flip, handedness);
    }
}


// ===========================================================================
// IsSwingType  @ 0x821D6050 | size: 0xB0 (176 bytes)
// ===========================================================================
/**
 * Returns true if the given type ID is a recognized swing type.
 *
 * Forehand: 2, 3, 10, 11 | Power: 8, 16 | Topspin: 5, 13 | Slice: 7, 15
 *
 * Pure function — no memory access, no side effects.
 */
bool pongPlayer_6050_p46(int32_t swingType) {  // IsSwingType @ 0x821D6050
    switch (swingType) {
        case 2: case 3: case 10: case 11:   // forehand group
        case 8: case 16:                     // power group
        case 5: case 13:                     // topspin group
        case 7: case 15:                     // slice group
            return true;
        default:
            return false;
    }
}


// ===========================================================================
// GetCourtSideSign  @ 0x821A0F10 | size: 0xB8 (184 bytes)
// ===========================================================================
/**
 * Determines which side of the court a player is on based on
 * the X position of a linked position sub-object.
 *
 * @param stateObj  Object with parent pongPlayer* at +48
 * @param outSign   Output: receives a positive or negative sign constant
 * @param deadZone  Absolute threshold below which no side is determined
 */
void pongPlayer_0F10_g(void* stateObj, float* outSign,
                       float deadZone) {  // GetCourtSideSign @ 0x821A0F10
    uint8_t* base = static_cast<uint8_t*>(stateObj);
    uint8_t* parent = *reinterpret_cast<uint8_t**>(base + 48);
    int32_t slotIdx = *reinterpret_cast<int32_t*>(parent + 464);

    // Select position sub-object based on slot index
    void* posObj;
    if (slotIdx == 0) {
        posObj = *reinterpret_cast<void**>(parent + 72);
    } else {
        posObj = *reinterpret_cast<void**>(parent + 68);
    }

    // Read X position from sub-object position vector at +48
    float posX = *reinterpret_cast<float*>(
        static_cast<uint8_t*>(posObj) + 48);

    // Dead-zone check: return early if position is too close to center
    if (fabsf(posX) < deadZone) return;

    // Determine court side sign from position X
    if (posX >= g_kFloatConst_D110) {
        *outSign = g_kPositiveSideSign;   // positive side @ 0x825C5938
    } else {
        *outSign = g_kFloatConst_D108;    // negative side @ 0x8202D108
    }
}


// ===========================================================================
// InitSwingHistorySlots  @ 0x821A76E8 | size: 0xBC (188 bytes)
// ===========================================================================
/**
 * Initializes a swing history tracking structure.
 *
 * Layout: +4/+6 = int16 slot indices (-1), +8 = int16 count (0),
 * +12..+23 = 3 header floats, +24..+103 = 20 weight floats,
 * +108..+187 = 20 value floats (from source max at +192).
 *
 * @param swingHistory  Pointer to swing history structure
 */
void pongPlayer_76E8_g(void* swingHistory) {  // InitSwingHistorySlots @ 0x821A76E8
    uint8_t* base = static_cast<uint8_t*>(swingHistory);
    float defaultVal = g_kFloatConst_D110;
    float maxVal = *reinterpret_cast<float*>(base + 192);

    // Reset slot indices and count
    *reinterpret_cast<int16_t*>(base + 4) = -1;
    *reinterpret_cast<int16_t*>(base + 6) = -1;
    *reinterpret_cast<int16_t*>(base + 8) = 0;

    // Reset header floats
    *reinterpret_cast<float*>(base + 12) = defaultVal;
    *reinterpret_cast<float*>(base + 16) = defaultVal;
    *reinterpret_cast<float*>(base + 20) = defaultVal;

    // Reset weight and value arrays (2 groups of 10 entries each)
    float* weights = reinterpret_cast<float*>(base + 24);
    float* values  = reinterpret_cast<float*>(base + 108);
    for (int i = 0; i < 20; i++) {
        weights[i] = defaultVal;
        values[i]  = maxVal;
    }
}


// ===========================================================================
// RemoveSlotEntry  @ 0x8218AE10 | size: 0xBC (188 bytes)
// ===========================================================================
/**
 * Deactivates and removes an entry from a slot grid.
 *
 * Computes the cell from (entryId, column) using a 24-byte stride,
 * deactivates the entry if active, calls its vtable[0] reset, clears
 * the back-reference, and zeros the cell metadata.
 *
 * @param slotsBase  Base pointer of the slot collection
 * @param slotId     Slot ID (indexes into entry table at +33 words)
 * @param column     Column index within the 2D grid
 */
void pongPlayer_AE10_g(void* slotsBase, int32_t slotId,
                       int32_t column) {  // RemoveSlotEntry @ 0x8218AE10
    uint8_t* base = static_cast<uint8_t*>(slotsBase);

    // Look up entry ID from slot table
    uint32_t entryId = *reinterpret_cast<uint32_t*>(
        base + (slotId + 33) * 4);

    // Compute cell byte offset: (entryId*2 + column) * 24
    uint32_t cellIndex = entryId * 2 + column;
    uint32_t byteOffset = cellIndex * 3 * 8;  // 24-byte cell stride
    uint8_t* cellPtr = base + byteOffset;

    // Get entry object pointer from cell+40
    void* entry = *reinterpret_cast<void**>(cellPtr + 40);

    // Debug checkpoint (no-op in release)
    nop_8240E6D0("RemoveSlotEntry enter");

    // Deactivate entry if active (byte at entry+44)
    uint8_t* entryBytes = static_cast<uint8_t*>(entry);
    if (entryBytes[44]) {
        pongPlayer_Deactivate(entry);
    }

    // Call entry's vtable[0](entry, 0) — virtual reset
    void** vtable = *reinterpret_cast<void***>(entry);
    typedef void (*VtableSlot0Fn)(void*, int);
    reinterpret_cast<VtableSlot0Fn>(vtable[0])(entry, 0);

    // Debug checkpoint (no-op in release)
    nop_8240E6D0("RemoveSlotEntry done");

    // Clear back-reference if it matches this entry
    void* currentEntry = *reinterpret_cast<void**>(cellPtr + 40);
    void** backRef = reinterpret_cast<void**>(
        base + (entryId + 29) * 4);
    if (*backRef == currentEntry) {
        *backRef = nullptr;
    }

    // Clear cell metadata
    cellPtr[32] = 0;
    cellPtr[33] = 0;
    *reinterpret_cast<int32_t*>(cellPtr + 24) = -1;
    *reinterpret_cast<int32_t*>(cellPtr + 28) = -1;
    *reinterpret_cast<void**>(cellPtr + 40) = nullptr;
}


// ===========================================================================
// UpdateTransitionBlend  @ 0x82384438 | size: 0xD8 (216 bytes)
// ===========================================================================
/**
 * Updates an animation transition blend state object.
 *
 * Checks completion flag, compares current vs previous section,
 * and either interpolates the transition value via pongPlayer_99B0_g
 * or directly snaps it based on the target distance.
 *
 * @param transObj  Transition blend state object
 *   +48: float current value, +52: byte current section,
 *   +56: float target, +60: byte previous section, +61: byte complete
 */
void pongPlayer_4438_fw(void* transObj) {  // UpdateTransitionBlend @ 0x82384438
    uint8_t* base = static_cast<uint8_t*>(transObj);

    // Already complete — nothing to do
    if (base[61]) return;

    uint8_t currentSection = base[52];
    uint8_t previousSection = base[60];

    float lowerBound = g_kFloatConst_D110;   // @ 0x8202D110
    float upperBound = g_kFloatConst_D108;   // @ 0x8202D108

    // If sections match and value is within transition bounds, skip
    if (currentSection == previousSection) {
        float value = *reinterpret_cast<float*>(base + 48);
        if (value <= lowerBound) return;
        if (value >= upperBound) return;
    }

    float target = *reinterpret_cast<float*>(base + 56);

    if (target > lowerBound) {
        // Interpolated transition using pongPlayer_99B0_g
        float startValue = (currentSection != 0) ? upperBound : lowerBound;
        float ratio = upperBound / target;

        bool updated = pongPlayer_99B0_g(
            reinterpret_cast<float*>(base + 48),
            startValue, ratio, g_kTransitionStep);

        if (!updated) return;

        // Sync: copy current section to previous
        base[60] = base[52];
    } else {
        // Direct update: snap to target
        base[60] = currentSection;
        float snapValue = (currentSection != 0) ? upperBound : lowerBound;
        *reinterpret_cast<float*>(base + 48) = snapValue;
    }
}


// ===========================================================================
// ResetAnimTransitionState  @ 0x8239D6C8 | size: 0xDC (220 bytes)
// ===========================================================================
/**
 * Clears all animation transition tracking data.
 *
 * Zeros two 80-byte data blocks, three 16-byte vectors, and a flag
 * byte. If a reset-pending flag is set, triggers a state change via
 * pongPlayer_5C00_g and clears an animation visibility bit.
 *
 * @param animTransObj  Animation transition state object
 *   +4: pongPlayer*, +112: data block A, +196: data block B,
 *   +288/304/320: vectors, +336: flag, +352: reset-pending
 */
void pongPlayer_D6C8_fw(void* animTransObj) {  // ResetAnimTransitionState @ 0x8239D6C8
    uint8_t* base = static_cast<uint8_t*>(animTransObj);

    // Zero first data block: 20 words (80 bytes) at +112
    memset(base + 112, 0, 80);

    // Zero second data block: 20 words (80 bytes) at +196
    memset(base + 196, 0, 80);

    // Zero three 16-byte vectors at +288, +304, +320
    memset(base + 288, 0, 48);

    // Clear flag byte at +336
    base[336] = 0;

    // If reset-pending flag is set
    if (base[352]) {
        // Trigger animation state change (mode 3)
        void* playerPtr = *reinterpret_cast<void**>(base + 4);
        pongPlayer_5C00_g(playerPtr, 3);

        // Clear animation visibility bit (bit 5) on nested sub-object
        uint8_t* playerBytes = *reinterpret_cast<uint8_t**>(base + 4);  // reload after call
        uint8_t* opponentObj = *reinterpret_cast<uint8_t**>(playerBytes + 452);
        uint8_t* animObj = *reinterpret_cast<uint8_t**>(opponentObj + 168);
        uint8_t* flagByte = animObj + 64;
        *flagByte &= 0xDF;  // clear bit 5

        // Clear the reset-pending flag
        base[352] = 0;
    }
}


// ===========================================================================
// ComputeSwingTargetDelta  @ 0x821906A0 | size: 0xDC (220 bytes)
// ===========================================================================
/**
 * Computes a swing target direction vector from the opponent's
 * player data record.
 *
 * Calls pongPlayer_0508_g twice (channels 31 and 30) to get swing
 * deltas. Negates both, then checks handedness — if the stored
 * handedness matches the computed one, negates again (cancelling out).
 *
 * @param outVec   Output: 2 floats ([0]=X, [1]=Y direction)
 * @param player   pongPlayer whose opponent record is queried
 */
void pongPlayer_06A0_g(float* outVec,
                       pongPlayer* player) {  // ComputeSwingTargetDelta @ 0x821906A0
    float inputConst = g_kSwingTargetConst;   // @ 0x825C80FC
    int32_t slotIdx = player->m_inputSlotIdx;

    // Look up opponent record from player data table
    uint32_t* dataEntries = reinterpret_cast<uint32_t*>(g_pPlayerDataTable);
    uint32_t opponentEntry = dataEntries[slotIdx + 1];
    void* recordBase = g_pPlayerDataTable + opponentEntry * 808 + 12;

    // Get swing deltas for channels 31 and 30
    float delta1 = pongPlayer_0508_g(recordBase, 31, inputConst);
    float delta2 = pongPlayer_0508_g(recordBase, 30, inputConst);

    // Store negated deltas
    outVec[1] = -delta1;
    outVec[0] = -delta2;

    // Get computed handedness from global slot lookup
    void* computedHandedness = pongPlayer_7CC0_g(g_pPlayerSlotLookup);

    // Read stored handedness from geometry table
    uint8_t* geomBase = static_cast<uint8_t*>(g_pInputObj) + slotIdx;
    uint8_t storedHandedness = geomBase[64];

    // If handedness matches (XOR == 0): negate again → original direction
    if ((storedHandedness ^ static_cast<uint8_t>(reinterpret_cast<uintptr_t>(computedHandedness) & 0xFF)) == 0) {
        outVec[0] = -outVec[0];
        outVec[1] = -outVec[1];
    }
}

// ===========================================================================
// BATCH 10 — Medium pongPlayer functions (136-248 bytes)
// ===========================================================================

// ---------------------------------------------------------------------------
// External forward declarations for batch 10
// ---------------------------------------------------------------------------
extern void* pg_9C00_g(void* inputObj, void* creature);
extern void phBoundCapsule_0880_g(void* capsule, void* outMatrix, uint32_t param);
extern float pongPlayer_C7A8_g(pongPlayer* player);
extern void pongPlayer_B088_2hr(void* timingData, float dir, float power);
extern void pongPlayer_E2C8_g(void* obj, int a, int b, int c, int d);
extern void pongPlayer_E008_g(void* obj, int a, int b, int c);
extern bool pongPlayer_D6B8_IsCreatureReady(void* characterSub);
extern void ServeStartedMessage_CFE8_2h(void* msgObj, void* targetData, int param);
extern void PlayerMovementMessage_D258_2hr(void* msgObj, float posX, float posZ);
extern void pongPlayer_B208_g(void* state, vec3* swingVec, uint8_t suppressFlip);
extern void pg_E6E0_msg(int messageId, int flags, int count, void* data);
extern "C" void pongPlayer_9CD0_g(void*, int, void*, void*);

extern uint32_t g_rngState[2];              // RNG state (lo, hi) @ 0x825CA2C8
extern const float g_kFloatConst_79E68;     // random scale factor @ 0x82079E68
extern const float g_kFloatConst_5C5938;    // height ratio threshold @ 0x825C5938
extern const float g_kFloatConst_79BB8;     // frame-to-duration scale @ 0x82079BB8
extern const double g_kDoubleConst_79D10;   // floor constant @ 0x82079D10
extern const double g_kDoubleConst_79B08;   // ceiling constant @ 0x82079B08


// ===========================================================================
// 1. UpdateOpponentBoundTransform
// ===========================================================================
/**
 * pongPlayer::UpdateOpponentBoundTransform()
 * @ 0x82194C08 | size: 0xA8
 *
 * Gets the opponent's physics bound capsule via the creature lookup chain,
 * computes a transform matrix, and copies the 4x16-byte result into
 * m_pLocoState2 at offset +32.
 */
void pongPlayer::UpdateOpponentBoundTransform() {  // pongPlayer_4C08_g @ 0x82194C08
    if (!m_pOpponent) return;

    // Get opponent's character sub-object at +188
    uint8_t* opponentBytes = reinterpret_cast<uint8_t*>(m_pOpponent);
    uint8_t* characterSub = *reinterpret_cast<uint8_t**>(opponentBytes + 188);

    // Look up creature in the physics system
    void* creature = *reinterpret_cast<void**>(characterSub + 8);
    void* lookupResult = pg_9C00_g(
        reinterpret_cast<void*>(g_pInputArrayTable), creature);

    // Navigate chain: result->+44->+324 to get bound capsule
    uint8_t* chainEntry = *reinterpret_cast<uint8_t**>(
        static_cast<uint8_t*>(lookupResult) + 44);
    void* boundCapsule = *reinterpret_cast<void**>(chainEntry + 324);

    // Compute transform matrix from bound capsule
    vec3 transformMatrix[4];  // 4x16 = 64 bytes
    uint32_t meshParam = *reinterpret_cast<uint32_t*>(characterSub + 28);
    phBoundCapsule_0880_g(boundCapsule, transformMatrix, meshParam);

    // Copy matrix to locomotion state data at +32
    uint8_t* dest = reinterpret_cast<uint8_t*>(m_pLocoState2) + 32;
    memcpy(dest, transformMatrix, sizeof(vec3) * 4);
}


// ===========================================================================
// 2. UpdateOpponentSwingTiming
// ===========================================================================
/**
 * pongPlayer::UpdateOpponentSwingTiming()
 * @ 0x82193738 | size: 0xA8
 *
 * If the input button at slot (inputSlotIdx + 14) is pressed, computes a
 * swing timing value via pongPlayer_C7A8_g and stores it (plus its square)
 * in the opponent's timing sub-object at +72/+76, then forwards to the
 * timing computation handler.
 */
void pongPlayer::UpdateOpponentSwingTiming(float swingDirection,
                                           float swingPower) {  // pongPlayer_3738 @ 0x82193738
    // Check button state at input slot + 14
    uint32_t* inputTable = reinterpret_cast<uint32_t*>(g_pInputArrayTable);
    uint32_t buttonState = inputTable[m_inputSlotIdx + 14];

    if (buttonState != 1) return;

    // Compute swing timing value
    float timingValue = pongPlayer_C7A8_g(this);

    // Get opponent's timing sub-object at +168
    uint8_t* opponentBytes = reinterpret_cast<uint8_t*>(m_pOpponent);
    uint8_t* timingSub = *reinterpret_cast<uint8_t**>(opponentBytes + 168);

    // Store timing value and its square
    *reinterpret_cast<float*>(timingSub + 72) = timingValue;
    *reinterpret_cast<float*>(timingSub + 76) = timingValue * timingValue;

    // Forward to timing computation handler
    pongPlayer_B088_2hr(timingSub, swingDirection, swingPower);
}


// ===========================================================================
// 3. ResetButtonSlotTrackingData (static)
// ===========================================================================
/**
 * pongPlayer::ResetButtonSlotTrackingData()
 * @ 0x821C9918 | size: 0xF8
 *
 * Resets per-player-slot shot tracking data.  Each slot is 36 bytes in
 * the button state table; within each slot at +172, three 12-byte tracking
 * entries store button state flags.  If any flags are raised, posts a
 * message event before clearing all entries via SyncByteField.
 */
void pongPlayer::ResetButtonSlotTrackingData(
    void* buttonStateBase, int slotIndex) {  // pongPlayer_9918_g @ 0x821C9918
    uint8_t* base = reinterpret_cast<uint8_t*>(buttonStateBase);
    uint8_t* slotData = base + slotIndex * 36;
    uint8_t* trackBase = slotData + 172;

    uint8_t flag0 = trackBase[0];
    uint8_t flag1 = trackBase[12];   // slotData + 184
    uint8_t flag2 = trackBase[24];   // slotData + 196

    // Check if any tracking flags are raised
    if (flag0 || flag1 || flag2) {
        // Determine which slot flag triggered (0, 1, or 2)
        int slotResult = 0;
        if (!flag0) {
            slotResult = flag1 ? 1 : (flag2 ? 2 : 0);
        }

        // Post tracking reset event (pg_E6E0 @ 0x8225E6E0)
        uint8_t bitMask = static_cast<uint8_t>((1 << slotIndex) & 0xFF) | 64;
        pg_E6E0_msg(4131, static_cast<int>(bitMask), 1, &slotResult);
    }

    // Clear float at computed offset (slotIndex + 61) * 4
    *reinterpret_cast<float*>(base + (slotIndex + 61) * 4) = 0.0f;

    // Clear 3 tracking entries (12 bytes apart) via SyncByteField
    uint8_t clearByte = 0;
    for (int i = 0; i < 3; i++) {
        pongPlayer_9108_g(trackBase + i * 12, &clearByte);
    }

    // Clear float at computed offset (slotIndex + 63) * 4
    *reinterpret_cast<float*>(base + (slotIndex + 63) * 4) = 0.0f;
}


// ===========================================================================
// 4. ComputeClampedPositions (free function)
// ===========================================================================
/**
 * pongPlayer_ComputeClampedPositions()
 * @ 0x821729F8 | size: 0xEC
 *
 * Computes two clamped normalized values from creature bone data.
 * Looks up bone matrices for the given creature index, determines the
 * mirroring state, then normalizes Z-position and height ratio relative
 * to the bone range.  Both outputs are clamped to [floor, ceiling] using
 * double-precision .rdata constants.
 */
void pongPlayer_ComputeClampedPositions(int creatureIndex,
                                        float* outA,
                                        float* outB) {  // pongPlayer_29F8_fw @ 0x821729F8
    // Get creature bone matrices
    vec3 bonePos1, bonePos2;
    pongPlayer_9CD0_g(reinterpret_cast<void*>(g_pInputArrayTable),
                      creatureIndex, &bonePos1, &bonePos2);

    // Check mirror flag at inputTable base + creatureIndex + 64
    uint8_t isMirrored = *(reinterpret_cast<uint8_t*>(g_pInputArrayTable)
                           + creatureIndex + 64);

    // Select range bounds based on mirror state
    float rangeHi, rangeLo, widthDenom;
    if (isMirrored) {
        rangeHi    = bonePos2.z;
        rangeLo    = bonePos1.z;
        widthDenom = bonePos2.x;
    } else {
        rangeHi    = bonePos1.z;
        rangeLo    = bonePos2.z;
        widthDenom = bonePos1.x;
    }

    // Get reference data from input table entry
    uint32_t* inputTable = reinterpret_cast<uint32_t*>(g_pInputArrayTable);
    uint8_t* refObj = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(inputTable[creatureIndex + 17]));
    float refZ      = *reinterpret_cast<float*>(refObj + 56);
    float refHeight = *reinterpret_cast<float*>(refObj + 48);

    // Compute normalized Z position and height ratio
    float range = rangeHi - rangeLo;
    float normalizedPos = (refZ - rangeLo) / range;
    float heightRatio = refHeight / widthDenom;

    // Clamp normalizedPos to floor (fsel: >= 0 ? value : floor)
    double posVal = (normalizedPos >= 0.0f)
        ? static_cast<double>(normalizedPos) : g_kDoubleConst_79B00;

    // Clamp heightRatio: threshold-based floor
    double heightVal = ((heightRatio - g_kFloatConst_5C5938) >= 0.0f)
        ? static_cast<double>(heightRatio) : g_kDoubleConst_79D10;

    // Clamp both to ceiling
    double ceiling = g_kDoubleConst_79B08;
    *outB = static_cast<float>((posVal - ceiling >= 0.0) ? ceiling : posVal);
    *outA = static_cast<float>((heightVal - ceiling >= 0.0) ? ceiling : heightVal);
}


// ===========================================================================
// 5. RandomizeShotTargetData (free function)
// ===========================================================================
/**
 * pongPlayer_RandomizeShotTargetData()
 * @ 0x821A6A98 | size: 0xE4
 *
 * Randomizes two position fields in a shot target data structure using
 * a 64-bit linear congruential RNG.  The LCG multiplier is 0x5CCCFAA7.
 * Each randomized value is: random_mantissa * range * scale + base.
 * Also resets associated tracking bytes and flags.
 */
void pongPlayer_RandomizeShotTargetData(void* shotData) {  // pongPlayer_6A98_g @ 0x821A6A98
    uint8_t* base = reinterpret_cast<uint8_t*>(shotData);
    const uint32_t kRngMultiplier = 0x5CCCFAA7u;
    float scale = g_kFloatConst_79E68;

    // Load struct parameters for first randomization
    float range1 = *reinterpret_cast<float*>(base + 16);
    float base1  = *reinterpret_cast<float*>(base + 8);
    float target = *reinterpret_cast<float*>(base + 20);

    // Clear flags and copy target float
    *(base + 56) = 0;
    *reinterpret_cast<float*>(base + 24) = target;
    *(base + 57) = 0;

    // Generate first random value (LCG step)
    uint64_t rngVal = static_cast<uint64_t>(g_rngState[0]) * kRngMultiplier
                    + g_rngState[1];
    g_rngState[0] = static_cast<uint32_t>(rngVal);
    g_rngState[1] = static_cast<uint32_t>(rngVal >> 32);
    uint32_t rand1 = g_rngState[0] & 0x7FFFFF;

    // Compute randomized position: random * range * scale + base
    float randFloat1 = static_cast<float>(static_cast<int64_t>(rand1));
    *reinterpret_cast<float*>(base + 4) = randFloat1 * range1 * scale + base1;

    // Zero tracking floats
    *reinterpret_cast<float*>(base + 52) = 0.0f;
    *reinterpret_cast<float*>(base + 48) = 0.0f;

    // Generate second random value (LCG step)
    rngVal = static_cast<uint64_t>(g_rngState[0]) * kRngMultiplier
           + g_rngState[1];
    g_rngState[0] = static_cast<uint32_t>(rngVal);
    g_rngState[1] = static_cast<uint32_t>(rngVal >> 32);
    uint32_t rand2 = g_rngState[0] & 0x7FFFFF;

    // Load second set of parameters and set flags
    *reinterpret_cast<float*>(base + 72) = 0.0f;
    float range2 = *reinterpret_cast<float*>(base + 64);
    *(base + 76) = 0;
    float base2  = *reinterpret_cast<float*>(base + 60);
    *(base + 77) = 1;

    // Compute second randomized position
    float randFloat2 = static_cast<float>(static_cast<int64_t>(rand2));
    *reinterpret_cast<float*>(base + 68) = randFloat2 * range2 * scale + base2;
}


// ===========================================================================
// 6. InitShotTrackingGrid (free function)
// ===========================================================================
/**
 * pongPlayer_InitShotTrackingGrid()
 * @ 0x8219DF38 | size: 0xCC
 *
 * Initializes a shot tracking grid structure.  Stores mode/side bytes,
 * calls ClearSwingSlotGrid, zeros flag bytes in two 4x48 tables,
 * calls grid setup functions, then resets status fields.
 */
void pongPlayer_InitShotTrackingGrid(void* gridData, uint8_t mode,
                                     uint8_t side) {  // pongPlayer_DF38_g @ 0x8219DF38
    uint8_t* base = reinterpret_cast<uint8_t*>(gridData);

    // Initialize mode/side and clear swing progress
    *reinterpret_cast<float*>(base + 960) = 0.0f;
    *(base + 944) = side;
    *(base + 945) = mode;

    // Clear swing slot grid
    pongPlayer_E640_g(gridData);

    // Zero flag bytes in first table (4 entries, stride 48, at +32)
    for (int i = 0; i < 4; i++) {
        *(base + 32 + i * 48) = 0;
    }

    // Zero flag bytes in second table (4 entries, stride 48, at +224)
    for (int i = 0; i < 4; i++) {
        *(base + 224 + i * 48) = 0;
    }

    // Initialize grid entries with mode parameters
    pongPlayer_E2C8_g(gridData, 3, 3, static_cast<int>(mode), 1);
    pongPlayer_E008_g(gridData, 3, 3, static_cast<int>(mode));

    // Reset status fields
    *(base + 956) = 0;
    *reinterpret_cast<uint32_t*>(base + 972) = 0;
    *(base + 968) = 0;
    *(base + 964) = 0;
    *reinterpret_cast<uint32_t*>(base + 948) = 1;
    *reinterpret_cast<uint32_t*>(base + 952) = 1;
    *(base + 965) = 0;
    *(base + 966) = 0;
    *(base + 967) = 0;
}


// ===========================================================================
// 7. TransformAndApplyVector (free function)
// ===========================================================================
/**
 * pongPlayer_TransformAndApplyVector()
 * @ 0x820CB130 | size: 0xD8
 *
 * Transforms an input vector by a 3x3 matrix looked up from the input
 * table via the state's slot index (+68), then passes the result to
 * pongPlayer_B208_g for application.
 */
void pongPlayer_TransformAndApplyVector(void* statePtr,
                                        const vec3* inputVec) {  // pongPlayer_B130_fw @ 0x820CB130
    uint8_t* stateBase = reinterpret_cast<uint8_t*>(statePtr);
    int32_t slotIdx = *reinterpret_cast<int32_t*>(stateBase + 68);

    // Look up transform matrix from input table
    uint32_t* inputTable = reinterpret_cast<uint32_t*>(g_pInputArrayTable);
    uint8_t* matObj = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(inputTable[slotIdx + 17]));

    // Extract 3 matrix rows (16 bytes each at +0, +16, +32)
    float* row0 = reinterpret_cast<float*>(matObj);
    float* row1 = reinterpret_cast<float*>(matObj + 16);
    float* row2 = reinterpret_cast<float*>(matObj + 32);

    float inX = inputVec->x;
    float inY = inputVec->y;
    float inZ = inputVec->z;

    // Matrix x vector (3x3 transform: row0*X + row2*Z + row1*Y)
    vec3 result;
    result.x = row0[0] * inX + row2[0] * inZ + row1[0] * inY;
    result.y = row0[1] * inX + row2[1] * inZ + row1[1] * inY;
    result.z = row0[2] * inX + row2[2] * inZ + row1[2] * inY;

    pongPlayer_B208_g(statePtr, &result, 0);
}


// ===========================================================================
// 8. SendServeOrMovementMessage
// ===========================================================================
/**
 * pongPlayer::SendServeOrMovementMessage()
 * @ 0x82195598 | size: 0xF4
 *
 * If the serve flag in the message handler (+500->+363) is set, modifies
 * the opponent's physics bound data (zeros two floats at +32/+36, sets
 * flag bit 0x10 at +64), then sends a serve started message.
 * Otherwise, if hasDirectMovement is set, sends a movement message
 * with the X and Z components of targetVec.
 */
void pongPlayer::SendServeOrMovementMessage(
    vec3* targetVec,
    uint8_t hasDirectMovement) {  // pongPlayer_5598_h @ 0x82195598
    // Get message handler at +500 (+0x1F4)
    uint8_t* selfBase = reinterpret_cast<uint8_t*>(this);
    void* msgHandler = *reinterpret_cast<void**>(selfBase + 500);

    uint8_t serveFlag = *(static_cast<uint8_t*>(msgHandler) + 363);

    if (serveFlag) {
        // Check opponent's player state creature readiness
        if (m_pOpponent->m_pPlayerState &&
            m_pOpponent->m_pPlayerState->IsCreatureState2Active()) {
            return;  // Opponent creature ready — skip serve message
        }

        // Modify opponent's physics bound data
        void* opponentPhysBound = m_pOpponent->m_pPhysicsBound;
        uint8_t* pbBase = reinterpret_cast<uint8_t*>(opponentPhysBound);

        // Zero two tracking floats and set flag bit
        *reinterpret_cast<float*>(pbBase + 32) = 0.0f;
        *reinterpret_cast<float*>(pbBase + 36) = 0.0f;
        *(pbBase + 64) |= 0x10;

        // Send serve started message
        ServeStartedMessage_CFE8_2h(msgHandler, targetVec, 0);
    } else {
        if (!hasDirectMovement) return;

        // Send movement message with position X and Z
        PlayerMovementMessage_D258_2hr(msgHandler, targetVec->x, targetVec->z);
    }
}


// ===========================================================================
// 9. IsWithinTimingWindow (free function)
// ===========================================================================
/**
 * pongPlayer_IsWithinTimingWindow()
 * @ 0x820DD298 | size: 0xEC
 *
 * Checks if the current animation progress (+172) falls within a timing
 * window defined by two normalized frame positions.  Returns true if
 * lowerBound <= currentProgress < upperBound, with bounds computed from
 * sub-object frame counts and duration.
 *
 * First gate: if the active flag (+44) is clear AND progress >= threshold,
 * returns false immediately (past timing window).
 */
bool pongPlayer_IsWithinTimingWindow(void* animState) {  // pongPlayer_D298_2hr @ 0x820DD298
    uint8_t* base = reinterpret_cast<uint8_t*>(animState);

    void* subObj = *reinterpret_cast<void**>(base + 36);
    if (!subObj) return false;

    uint8_t isActive = *(base + 44);
    float currentProgress = *reinterpret_cast<float*>(base + 172);

    // First gate: if inactive and progress >= threshold, not in window
    if (!isActive && currentProgress >= g_kFloatConst_D108) {
        return false;
    }

    uint8_t* subBase = reinterpret_cast<uint8_t*>(subObj);

    // Compute upper bound from frameCountHi (+24)
    void* subSubObj = *reinterpret_cast<void**>(subBase + 36);
    float duration = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(subSubObj) + 12);

    float upperBound;
    if (duration > 0.0f) {
        int32_t frameCountHi = *reinterpret_cast<int32_t*>(subBase + 24);
        upperBound = (static_cast<float>(frameCountHi) * g_kFloatConst_79BB8)
                   / duration;
    } else {
        upperBound = 0.0f;
    }

    if (upperBound <= currentProgress) return false;

    // Compute lower bound from frameCountLo (+20)
    subSubObj = *reinterpret_cast<void**>(subBase + 36);
    duration = *reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(subSubObj) + 12);

    float lowerBound;
    if (duration > 0.0f) {
        int32_t frameCountLo = *reinterpret_cast<int32_t*>(subBase + 20);
        lowerBound = (static_cast<float>(frameCountLo) * g_kFloatConst_79BB8)
                   / duration;
    } else {
        lowerBound = 0.0f;
    }

    // Within window: lowerBound <= currentProgress < upperBound
    return lowerBound <= currentProgress;
}


// ===========================================================================
// 10. CopyBitfieldState (free function)
// ===========================================================================
/**
 * pongPlayer_CopyBitfieldState()
 * @ 0x821D6D90 | size: 0x88
 *
 * Copies bitfield state from source to destination.  Each structure has:
 *   +0: uint32_t* data pointer (array of 32-bit words)
 *   +6: uint16_t numBits
 *   +8: float value
 *
 * Iterates over all bits, copying each from source to destination,
 * then copies the float value field.
 */
void pongPlayer_CopyBitfieldState(void* dest, void* source) {  // pongPlayer_6D90_p46 @ 0x821D6D90
    uint8_t* dstBase = reinterpret_cast<uint8_t*>(dest);
    uint8_t* srcBase = reinterpret_cast<uint8_t*>(source);

    int numBits = static_cast<int>(*reinterpret_cast<uint16_t*>(dstBase + 6));

    if (numBits > 0) {
        uint32_t* srcData = *reinterpret_cast<uint32_t**>(srcBase);
        uint32_t* dstData = *reinterpret_cast<uint32_t**>(dstBase);

        for (int i = 0; i < numBits; i++) {
            int wordIdx = i >> 5;
            uint32_t bitMask = 1u << (i & 0x1F);

            // Read source bit
            bool srcBit = (srcData[wordIdx] & bitMask) != 0;

            // Copy bit to destination word
            uint32_t dstWord = dstData[wordIdx];
            if (srcBit) {
                dstWord |= bitMask;
            } else {
                dstWord &= ~bitMask;
            }
            dstData[wordIdx] = dstWord;
        }
    }

    // Copy float value field
    *reinterpret_cast<float*>(dstBase + 8) =
        *reinterpret_cast<float*>(srcBase + 8);
}


// ===========================================================================
// SECTION 58 — NetSyncFloat record helpers (0x82199Cxx family)
// ===========================================================================
//
// These three helpers mutate a NetSyncFloatRec — a sync-tracked float field
// owned by shot/stamina/score sub-records.  The record layout (inferred from
// SyncFloatField (CF10_g) and ResetShotSyncFields (9310_g)) is:
//
//   +0x00  float    m_currentValue       // value replicated over the network
//   +0x04  void*    m_callbackArg        // passed to m_onChange if value changes
//   +0x08  void*    m_onChange           // optional change-notification callback
//   +0x18  uint8_t  m_updateInhibit      // non-zero => skip the sync update
//
// The three helpers share a common clamp pipeline:
//   1. Take |delta| (or direct value in 9C90).
//   2. If |delta| < g_kFloatConst_79C04 (epsilon) treat as 0.
//   3. Clamp the result to the upper bound g_kDoubleConst_79D00.
//   4. Combine with m_currentValue (subtract / overwrite / add).
//   5. Re-clamp to [0, upperBound] before calling SyncFloatField.
//
// Caller convention: r3 = record base, f1 = float delta (for 9C08/9AE0).
// The asm does `addi r3, r3, 8` internally so the SyncFloatField call
// receives (record+8) — matching the target layout used everywhere else.


// ---------------------------------------------------------------------------
// 9C08_g — DecrementSyncedFloatClamped  @ 0x82199C08 | size: 0x88 (136 bytes)
// ---------------------------------------------------------------------------
/**
 * Decrement a NetSyncFloatRec's value by |deltaMagnitude|, re-clamped to the
 * epsilon/upper-bound window, skipped entirely when m_updateInhibit is set.
 *
 * Called from pongPlayer_37E0_g, pongPlayer_3A68_g, and pongPlayer_Process
 * to bleed off a sync float (e.g. stamina tick) each frame.
 *
 * TODO: confirm the exact semantic of m_updateInhibit vs. the "use current
 *       value as baseline" branch — the pass5 asm treats the zero-inhibit
 *       path as the one that actually performs the SyncFloatField write.
 */
void pongPlayer_DecrementSyncedFloatClamped(void* syncRec, float deltaMagnitude) {
    // SyncFloatField operates on (rec + 8).  The first 0x18 bytes of the rec
    // form the tracked-field header; byte at +0x18 is the update inhibit flag.
    uint8_t* recBase  = reinterpret_cast<uint8_t*>(syncRec);
    uint8_t* syncSlot = recBase + 8;                         // target of SyncFloatField
    const uint8_t inhibit = syncSlot[0x1C];                  // recBase + 0x24 == syncSlot + 0x1C
                                                             // matches `lbz r11,28(r3+8)`

    // 1. Take absolute value.
    double v = deltaMagnitude < 0.0f ? -double(deltaMagnitude) : double(deltaMagnitude);

    // 2. Apply deadzone: if |delta| < epsilon → 0.
    const double epsilon = double(g_kFloatConst_79C04);
    if (v < epsilon) {
        v = double(g_kDoubleConst_79EC8);                    // 0.0 (select-zero double)
    }

    // 3. Clamp to upper bound.
    const double upperBound = g_kDoubleConst_79D00;
    if (v > upperBound) {
        v = upperBound;
    }

    if (inhibit == 0) {
        // 4. Combine: newValue = clamp01(currentValue − v) (but clamped to upperBound).
        float  oldValue = *reinterpret_cast<float*>(syncSlot);
        double combined = double(oldValue) - v;

        if (combined < 0.0) {
            combined = double(g_kDoubleConst_79EC8);         // floor at 0.0
        }
        if (combined > upperBound) {
            combined = upperBound;                           // re-clamp at ceiling
        }

        float newValue = float(combined);

        // 5. Commit through the standard change-notification path.
        //    Equivalent to pongPlayer::SyncFloatField(syncSlot, &newValue);
        //    — inlined here to avoid making it a static call.
        float* valPtr = reinterpret_cast<float*>(syncSlot);
        if (*valPtr != newValue) {
            void* callback = *reinterpret_cast<void**>(syncSlot + 8);
            if (callback) {
                void* cbArg = *reinterpret_cast<void**>(syncSlot + 4);
                reinterpret_cast<void(*)(void*)>(callback)(cbArg);
            }
            *valPtr = newValue;
        }
    }
    // else: inhibit != 0 → value is frozen this frame.
}


// ---------------------------------------------------------------------------
// 9C90_g — SetSyncedFloatClamped  @ 0x82199C90 | size: 0x1B8 (440 bytes)
// ---------------------------------------------------------------------------
/**
 * "Commit the pending value" variant of the net-sync record.  Rather than
 * taking a new value from an argument, it reads m_currentValue, converts it
 * to int (for flag lookup), and — depending on three state bytes at
 * +0x28/+0x29/+0x2A of the record — either pushes the current value to the
 * replicated slot, rolls the value back via a timer field at +0x3C/+0x40,
 * or clears the pending latch entirely.
 *
 * This is the counterpart of DecrementSyncedFloatClamped: when the record is
 * flagged as "value changed but not yet synced", this function chooses
 * whether to publish, re-queue, or discard the change.  The full control-flow
 * mirrors UpdateDirtyFlags (9E48_g) — three bytes forming a small state
 * machine (immediate / queued / previous).
 *
 * TODO: The full function body pivots on three byte flags at record+40/41/42
 *       plus two floats at +60 and +64 and a timer constant at lbl_8202D108.
 *       A faithful lift needs disambiguation of these offsets — deferred to
 *       a follow-up pass with ground-truth from the caller pongPlayer_Process.
 *       For now we stub with the correct signature so other lifts can refer
 *       to the function.
 */
void pongPlayer_SetSyncedFloatClamped(void* syncRec) {
    (void)syncRec;
    // TODO(9C90_g): flag-machine dispatch + float pin-to-bounds.
}


// ---------------------------------------------------------------------------
// 9AE0_g — AddToSyncedFloatClamped  @ 0x82199AE0 | size: 0x128 (296 bytes)
// ---------------------------------------------------------------------------
/**
 * Accumulator variant of the NetSyncFloat clamp helper.  Adds `delta` to the
 * record's current value, clamped to [0, upperBound] with the same epsilon
 * deadzone.  When the old value equals a sentinel (g_kFloatConst_79C04),
 * also raises bit (1 << bitSlot) in the record's flag word via pg_E480 —
 * this is the "first-touch since reset" notification path.
 *
 * Caller: pongPlayer_3A68_g passes bitSlot in r5 to select which bit of the
 * queued-update bitmask to OR in.
 *
 * TODO: confirm the pg_E480 signature (currently {rec_flags_base, slotId,
 *       bitmask, code=4129, stringPtr}) and the exact offset of the 64-byte
 *       field used as both input baseline and output accumulator — the +44
 *       and +64 offsets differ from pongPlayer proper, consistent with this
 *       being called on the shot sub-record, not the player itself.
 */
void pongPlayer_AddToSyncedFloatClamped(void* syncRec, float delta, uint32_t bitSlot) {
    (void)syncRec;
    (void)delta;
    (void)bitSlot;
    // TODO(9AE0_g): full lift pending shot-record field map.
}


// ===========================================================================
// SECTION 59 — Forward declarations for follow-up batch
// ===========================================================================
//
// The functions below are grouped with the 9Cxx NetSyncFloat family by their
// Process() call-graph neighbourhood but require additional struct reverse
// engineering before they can be safely lifted under the no-raw-offset rule.
// They are declared here so other translation units can reference them by
// name while the stub pass5 implementations remain in place.
//

/**
 * pongPlayer_9EC8_g @ 0x82189EC8 | size: 0x198 (408 bytes)
 *
 * Walks the two 4-byte player-record entries at this+0x74 and this+0x78
 * (matching the m_pPlayer[2] slot table), writes a single byte (team side)
 * into two deep sub-objects at +0x1C4+0xBC and +0x1C4+0x75, then polls the
 * global button-state singleton and, depending on the replay/network flag
 * at lbl_8207B6C0, either invokes xe_BB68 on the two vtable[160] helpers of
 * each player record or stores a fixed status code 2 into the button-state.
 *
 * TODO: The +0x1BC/+0x1A4/+0xA4 chains need confirmation — likely touch the
 *       creature state + character-config sub-object.  Function is too
 *       structurally dense to lift without first lifting 7CC0_g and 05A8_g.
 */
extern "C" void pongPlayer_9EC8_g(void* /*this*/);

/**
 * pongPlayer_AB48_g @ 0x820CAB48 | size: 0x130 (304 bytes)
 *
 * SIMD (vmx128) vector-math helper.  Reads a v3 at (arg1+0x70)+0xA4+148
 * through two vtable indirections, falls through to a scalar fallback that
 * calls game_AF18 (court-bounds sample), then blends the two via
 * pongPlayer_9CD0_g and clamps the output into out[0..3].
 *
 * TODO: depends on game_AF18 + 9CD0_g lifts; needs a proper vec4 typedef
 *       surfaced into the header before it can be written cleanly.
 */
extern "C" void pongPlayer_AB48_g(void* outVec, void* playerLike);

/**
 * pongPlayer_94F8_wrh @ 0x820D94F8 | size: 0xE4 (228 bytes)
 *
 * Iterates the 120-byte-per-entry global button-slot table (at g_pButtonStateTable
 * + 0x20), searching for the first active slot matching the current active
 * player.  Returns in f1 the float at byte offset 0 of the selected slot
 * (defaulting to g_kFloatConst_D110 if none found).
 *
 * The modular arithmetic `((v * 0x888D) >> 32 + v) >> 6` is a signed
 * divide-by-120 Hacker's-Delight reciprocal — it should be expressed as a
 * plain modulo in the lifted source once confirmed.
 */
extern "C" float pongPlayer_94F8_wrh();

/**
 * pongPlayer_DDC0_g @ 0x820CDDC0 | size: 0xD4 (212 bytes)
 *
 * Compound gate: returns true iff the anim phase is NOT blocked
 * (IsSwingPhaseBlocked == false) AND timing state exists with currentTime
 * already past targetTime AND neither creature state is active AND not
 * recovering.  This is the "ready to enter swing" test used by
 * pongPlayer_DE98_g (IsInReturnPosition) and pongPlayer_BF18_g.
 *
 * Fully lifted version is trivial — waiting on the companion DA58
 * IsSwingApexReached rewrite before landing to avoid churn in the
 * IsSwingPhaseBlocked / IsRecovering predicate chain.
 */
extern "C" bool pongPlayer_DDC0_g(pongPlayer* self);

/**
 * pongPlayer_D908_g @ 0x820CD908 | size: 0x150 (336 bytes) —
 *    CancelSwing full variant (already declared in header at hpp:303).
 *
 * Body performs: if IsSwingPhaseBlocked → reset anim state floats at
 * +12/+16/+20/+24/+28/+32/+36, clear two globals (g_swingInFlightFlag,
 * g_swingCountFlag), clear inner flag at +141, call nop_8240E6D0.
 * Then if IsRecovering → call game_CD20 on recovery state.  Finally if
 * IsSwingPhaseBlocked (again) → reset the m_pShotState->m_phase chain and
 * stamp float at +412.
 *
 * TODO: hit-cancel semantics need a second pass — defer.
 */
extern "C" void pongPlayer_D908_g(pongPlayer* self);

/**
 * pongPlayer_DCD8_g @ 0x820CDCD8 | size: 0xE8 (232 bytes) —
 *    IsBeforeSwingPeak (declared in hpp:294).
 *
 * Conjunction of four predicates: IsSwingPhaseBlocked, IsSwingTimerActive,
 * IsCreatureStateReady, IsRecovering — plus a sign check on
 * m_pAnimState->m_phase relative to g_swingPhaseThreshold.  Currently
 * implemented via forwarding; TODO merge the sub-predicate early-outs for
 * cleaner control flow.
 */

/**
 * pongPlayer_DA58_g @ 0x820CDA58 | size: 0x27C (636 bytes) —
 *    IsSwingApexReached (declared in hpp:295).
 *
 * Large time/threshold compare over m_pAnimState + m_pTimingState; already
 * stub-declared.  TODO: port the full body now that 9C08 is in place.
 */
