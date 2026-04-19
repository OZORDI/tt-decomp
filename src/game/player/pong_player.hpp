/**
 * pong_player.hpp — pongPlayer class declaration (reconstructed)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Reconstructed from:
 *   - Offset-frequency clustering (279 methods, 52k symbols)
 *   - Debug string field hints (Activate/Draw/Render/Process/Teleport/StartSwing/Deactivate)
 *   - Lifted method bodies (D550 through 7C78, Destroy/Reset/Update)
 *   - Caller names: pongPlayer_Update1, pongPlayer_StartSwing, pongPlayer_StartServe,
 *                   pongPlayerShaderParams_UpdateParams, ServeStartedMessage_5728
 *   - RAGE engine naming conventions (m_ prefix, PascalCase methods)
 *
 * ── Class hierarchy ───────────────────────────────────────────────────
 *
 *   atSingleton              (RAGE singleton base — ID, pool, ownership)
 *       └── pongPlayer       (table-tennis player: physics, input, swing AI)
 *
 * ── Architecture notes ────────────────────────────────────────────────
 *
 *  pongPlayer is a moderately large object (>500 bytes confirmed).  It acts
 *  as the outer shell for everything about a single player:
 *
 *   [outer shell: pongPlayer]
 *     ├── m_pPhysicsBody    +0x0A4  physics transform/pose
 *     ├── m_pPhysicsBound   +0x0A8  phBoundCapsule for collision
 *     ├── m_pPlayerState    +0x0BC  heap-alloc'd inner state (swing, anim, recovery)
 *     ├── m_swingInputResult +0x0C8  result code from pongPlayer_BF18 swing evaluator
 *     ├── m_swingInputSlot  +0x0CC  index into the global per-player input score table
 *     ├── m_bSwingActive    +0x0C5  bool: is the player currently in a swing?
 *     ├── m_inputSlotIdx    +0x1D0  player's button/input slot index (0-based)
 *     ├── m_pOpponent       +0x1C4  pongPlayer* opponent reference (read-only after init)
 *     └── m_pActionState    +0x1F0  some action state object
 *
 *  The "inner state" at m_pPlayerState (+0x0BC) stores all mutable per-frame
 *  swing data: timing, recovery, anim phase, creature state references.
 *  Most of the 0x820Cxxxx functions operate on m_pPlayerState, not pongPlayer directly.
 *
 * ── CONFIRMED FIELD OFFSETS (pongPlayer) ──────────────────────────────
 *
 *   +0x000  void**              vtable
 *   +0x004  uint32_t            m_flags
 *   +0x008  pcrCreature*        m_pCreature          debug: "creature already activated"
 *   +0x00C  ?
 *   +0x010  uint32_t*           m_pBody              (ptr; float at +12 = movement speed)
 *   +0x014  uint8_t             m_bActive            bool
 *   +0x01C  void*               m_pParent            parent/owner (flags at +20 checked)
 *   +0x02C  void*               m_pLocomotionState   R:29 W:4
 *   +0x030  void*               m_pLocoState2        R:49 W:3
 *   +0x03C  void*               m_pAnimList          R:20 W:6
 *   +0x04C  uint8_t             m_team               team/side byte, freq-written
 *   +0x050  void*               m_pGameState         R:34 W:5
 *   +0x078  pongTimingSubState* m_pSwingSubState
 *   +0x07C  pongTimingState*    m_pTimingState
 *   +0x080  pongRecoveryState*  m_pRecoveryState
 *   +0x084  ?
 *   +0x088  pongAnimState*      m_pAnimState
 *   +0x08C  pongCreatureState*  m_pCreatureState2    vtable slot6=IsActive, slot7=IsReady
 *   +0x090  ?
 *   +0x094  pongSwingData*      m_pSwingData         byte@+341, float@+344, vec3@+352
 *   +0x0A4  void*               m_pPhysicsBody       vtable slot3 called in 5FB8
 *   +0x0A8  void*               m_pPhysicsBound      phBoundCapsule, floats at +56,+60,+64,+72
 *   +0x0BC  pongPlayerState*    m_pPlayerState       heap-alloc'd inner state (R:large)
 *   +0x0C4  ?
 *   +0x0C5  uint8_t             m_bSwingActive       bool; written by SetSwingActiveState
 *   +0x0C8  int32_t             m_swingInputResult   result code from BF18 evaluator
 *   +0x0CC  int32_t             m_swingInputSlot     index into global score float array
 *   +0x0E6  uint8_t             m_bSwingCommit1      byte at +230 checked in 5890
 *   +0x0E7  uint8_t             m_bSwingCommit2      byte at +231 checked in 5890
 *   +0x1C4  pongPlayer*         m_pOpponent          opponent player ref (R:66 W:0 — set at init)
 *   +0x1CC  ?                                        R:60 W:2
 *   +0x1D0  int32_t             m_inputSlotIdx       player's button/input slot index
 *                                                    (used in vtable slot 7, 9; also as +464)
 *   +0x1EC  ?                                        R:23 W:0
 *   +0x1F0  void*               m_pActionState       passed to pongPlayer_InitActionState in Update
 *
 *   NOTE: +0x1D0 above is the MOST ACCESSED field (R:112 W:2). At first I called this
 *         m_pMatch, but Reset and Update use it as an integer input slot index.
 *         Revising to m_inputSlotIdx. The two writes are initial assignment.
 *         The 112 reads are the per-frame input polling.
 *
 * ── Global tables / constants ─────────────────────────────────────────
 *
 *   g_pInputObj          @ 0x825EAB28   — input system singleton pointer
 *   g_pLerpQueue         @ lis(-32142) + -23816 — pongLerpQueue* singleton
 *   g_pPlayerDataTable   @ lis(-32142) + -23780 — per-player 808-byte records
 *   g_pButtonStateTable  @ lis(-32142) + -23828 — button state bitfield table
 *   g_kSwingRadiusConst  @ 0x825CAEB8 + 8       — float: capsule radius or scale
 *   g_pInputScoreTable   @ lis(-32161) + -21696  — float array[inputSlot]
 *
 * ── Notes ─────────────────────────────────────────────────────────────
 *
 *   The `bns` PPC float branch idiom translates as `!(x < y)` throughout.
 *   Under IEEE 754, NaN operands make `<` return false, so `!(NaN < y) = true`.
 *   Uninitialised timer floats (NaN) are treated as "expired" — intentional.
 */

#pragma once

#include <stdint.h>

// Forward declarations
struct pcrCreature;
struct pongTimingState;
// pongTimingSubState: swing timing sub-state (large struct, ~342+ bytes)
struct pongTimingSubState {
    void*       vtable;           // +0x00
    uint8_t     _pad04[12];       // +0x04..+0x0F
    void*       m_pBodyData;      // +0x10  body data (float at +12 = speed)
    uint8_t     _pad14[12];       // +0x14..+0x1F
    void*       m_pSubObject;     // +0x20 (32)  sub-object pointer (loco sub-state)
    uint8_t     _pad24[12];       // +0x24..+0x2F
    void*       m_pVelRecord;     // +0x30 (48)  velocity record pointer
    uint8_t     _pad34[108];      // +0x34..+0x9F
    float       m_swingProgress;  // +0xA0 (160) — cumulative swing progress
    uint8_t     _padA4[4];        // +0xA4..+0xA7
    uint32_t    m_frameId;        // +0xA8 (168) — frame counter / animation frame reference
    uint8_t     _padAC[136];      // +0xAC..+0x133
    uint16_t    m_locoFrameCount; // +0x134 (308) — frame count for normalised fraction
    uint8_t     _pad136[2];       // +0x136..+0x137
    uint8_t     m_locoFlags;      // +0x138 (312) — loco flags (bit 3=0x8, bit 2=0x4)
    uint8_t     _pad139[27];      // +0x139..+0x153
    int16_t     m_swingSubFrame;  // +0x154 (340) — frame ID for apex fraction computation
};
struct pongRecoveryState;
struct pongAnimState;
struct pongCreatureState;
struct pongSwingData;
struct pongPlayerState;
struct vec3 {
    float x, y, z;
    float _pad;  // 16-byte aligned on PPC AltiVec
};


// ── Sub-struct: pongTimingState ──────────────────────────────────────────
struct pongTimingState {
    uint8_t     _pad0[12];       // +0x00..+0x0B
    float       m_targetX;       // +0x0C  zero-filled by CancelSwing
    float       m_targetY;       // +0x10
    float       m_targetZ;       // +0x14
    uint8_t     _pad1[4];        // +0x18..+0x1B
    float       m_currentTime;   // +0x1C (28) elapsed swing time
    float       m_currentTime2;  // +0x20 (32)
    float       m_peakX;         // +0x24 (36) position component / velocity data
    float       m_peakY;         // +0x28 (40) confirmed: compared vs normalised fraction
    float       m_peakTime;      // +0x2C (44)
    float       m_targetTime;    // +0x30 (48) full swing duration
    uint8_t     _pad2[12];       // +0x34..+0x3F
    float       m_vel0;          // +0x40 (64)
    float       m_vel1;          // +0x44 (68)
    float       m_vel2;          // +0x48 (72)
    uint8_t     _pad3[4];        // +0x4C..+0x4F
    vec3        m_swingTargetVec;// +0x50 (80) 16-byte aligned swing target vector
    uint8_t     _pad4[45];       // +0x60..+0x8C
    uint8_t     m_bComplete;     // +0x8D (141) cleared by CancelSwing
};

// ── Sub-struct: pongRecoveryState ────────────────────────────────────────
struct pongRecoveryState {
    uint8_t     _pad00[28];      // +0x00..+0x1B
    int32_t     m_stateCode;     // +0x1C (28)  state code (0=idle)
    int32_t     m_serverFlag;    // +0x20 (32)  server flag (1=serving)
    void*       m_pFollowAction; // +0x24 (36)  follow-up action pointer
    uint8_t     _pad28[4];       // +0x28..+0x2B
    uint8_t     m_bForceBlock;   // +0x2C (44)  force-block flag
    uint8_t     _pad2D[35];      // +0x2D..+0x4F
    vec3        m_vec0;          // +0x50 (80)  recovery target vector slot A
    vec3        m_vec1;          // +0x60 (96)  recovery target vector slot B
    vec3        m_recoveryVec;   // +0x70 (112) stored recovery position vector
    uint8_t     _pad80[44];      // +0x80..+0xAB
    float       m_recoveryTimer; // +0xAC (172) — compared vs g_recoveryTimerThreshold
};

// ── Sub-struct: pongAnimState ────────────────────────────────────────────
struct pongAnimState {
    void*       vtable;          // +0x00
    void*       m_pFrameArray;   // +0x04  frame-data array pointer
    uint8_t     _pad08[8];       // +0x08..+0x0F
    void*       m_pAnimSubState; // +0x10 (16)  anim blender sub-state (crAnimBlenderState)
    uint8_t     _pad14[76];      // +0x14..+0x5F
    float       m_animProgress;  // +0x60 (96)  current frame index (cast to int for array lookup)
    uint8_t     _pad64[36];      // +0x64..+0x87
    uint32_t    m_animSubOffset; // +0x88 (136) geometry sub-offset / lookup key
    uint8_t     _pad8C[264];     // +0x8C..+0x193
    int32_t     m_frameIndex;    // +0x194 (404) frame index for apex fraction computation
    uint8_t     _pad198[4];      // +0x198..+0x19B
    float       m_swingPhase;    // +0x19C (412) — confirmed from D908, 73E8
    uint32_t    m_flags;         // +0x1A0 (416) bit 0 = IsSwingPhaseBlocked secondary
    float       m_animPhase;     // +0x1A4 (420) compared vs g_swingPhaseThreshold
};

// ── Sub-struct: pongCreatureState ────────────────────────────────────────
struct pongCreatureState {
    void**      vtable;              // +0x00
    uint8_t     _pad04[420];         // +0x04..+0x1A7
    uint8_t     m_postPointFlag;     // +0x1A8 (424)  post-point face animation trigger
    bool IsActive() const { return ((bool(*)(const pongCreatureState*))vtable[6])(this); }
    bool IsReady()  const { return ((bool(*)(const pongCreatureState*))vtable[7])(this); }
};


// ── Outer class: pongPlayer ───────────────────────────────────────────────
struct pongPlayer {
    // ── atSingleton base ────────────────────────────────────────────────
    void**              vtable;              // +0x000
    uint32_t            m_flags;             // +0x004
    pcrCreature*        m_pCreature;         // +0x008
    uint32_t            _unk_0x00C;          // +0x00C
    uint32_t*           m_pBody;             // +0x010  body/transform (float at +12 = speed)
    uint8_t             m_bActive;           // +0x014
    uint8_t             _pad0[7];
    void*               m_pParent;           // +0x01C
    uint8_t             _pad1[12];
    void*               m_pLocomotionState;  // +0x02C
    void*               m_pLocoState2;       // +0x030
    uint8_t             _pad2[12];
    void*               m_pAnimList;         // +0x03C
    uint8_t             _pad3[4];            // +0x040
    float               m_swingPhaseInput;   // +0x044 (+68) animation phase for swing timing
    float               m_swingDirectionAdj; // +0x048 (+72) directional adjustment value
    uint8_t             m_team;              // +0x04C
    uint8_t             _pad4[3];
    float               m_swingTimingClamp;  // +0x050 (+80) swing timing adjustment [0,1]
    uint8_t             _pad5[32];
    pongTimingSubState* m_pSwingSubState;    // +0x078
    pongTimingState*    m_pTimingState;      // +0x07C
    pongRecoveryState*  m_pRecoveryState;    // +0x080
    uint32_t            _unk_0x084;          // +0x084
    pongAnimState*      m_pAnimState;        // +0x088
    pongCreatureState*  m_pCreatureState;    // +0x08C  (IsCreatureStateReady, D660)
    pongCreatureState*  m_pCreatureState2;   // +0x090  (IsCreatureState2Active, D6B8)
    pongSwingData*      m_pSwingData;        // +0x094  byte@+341, float@+344, vec3@+352
    uint8_t             _pad6[12];
    void*               m_pPhysicsBody;      // +0x0A4  vtable slot3 = physics pose update
    void*               m_pPhysicsBound;     // +0x0A8  phBoundCapsule; floats +56,+60,+64,+72
    uint8_t             _pad7_AC[4];         // +0x0AC
    float               m_animTimer;         // +0x0B0  compared vs g_recoveryTimerThreshold in D228
    uint8_t             _pad7_B4[8];         // +0x0B4
    pongPlayerState*    m_pPlayerState;      // +0x0BC
    uint8_t             _pad8[8];
    uint8_t             m_bSwingActive;      // +0x0C5  logged by SetSwingActiveState
    uint8_t             _pad9[2];
    int32_t             m_swingInputResult;  // +0x0C8  result code: 0=idle,2=restart,3-4=log,5=swing
    int32_t             m_swingInputSlot;    // +0x0CC  index into g_pInputScoreTable[]
    uint8_t             _pad10[249];
    uint8_t             m_bSwingCommit1;     // +0x0E6 (+230) — checked in 5890_g
    uint8_t             m_bSwingCommit2;     // +0x0E7 (+231)
    uint8_t             _pad11[212];         // +0x0E8..+0x1BB
    void*               m_pDrawData;         // +0x1BC (+444) draw/render data object
    uint8_t             _pad11b[4];          // +0x1C0..+0x1C3
    pongPlayer*         m_pOpponent;         // +0x1C4 (+452) R:66 W:0 — set once at init
    uint32_t            _unk_0x1C8;          // +0x1C8
    uint32_t            _unk_0x1CC;          // +0x1CC
    int32_t             m_inputSlotIdx;      // +0x1D0 (+464) — player's button/input slot index
                                             //   MOST ACCESSED (R:112); per-frame input polling
    uint8_t             _pad12[20];
    void*               _unk_0x1E8;          // +0x1E8
    uint32_t            _unk_0x1EC;          // +0x1EC
    void*               m_pActionState;      // +0x1F0 (+496) — passed to pongPlayer_InitActionState

    // ── Additional reconstructed fields ─────────────────────────────────
    // These fields are accessed by later-lifted functions. Placed here to
    // compile; exact struct offsets TBD (past end of confirmed layout).
    uint8_t             m_courtSide;         // court side byte
    float               m_lerpValue;         // lerp interpolation state
    int32_t             m_slotIndex;         // match slot index
    uint32_t            m_playerFlags;       // player state flags
    float               m_swingStrengthX;    // swing strength X component
    float               m_swingStrengthY;    // swing strength Y component
    void*               m_pShotState;        // shot state sub-object
    uint32_t            m_creatureSlotCounter; // creature slot anim counter
    void**              m_creatureSlots;     // creature slot array (4 pointers)
    float               m_shotSpeed;         // computed shot speed
    float               m_courtHalfWidthX;   // court half-width X
    float               m_courtHalfWidthZ;   // court half-width Z
    uint8_t             m_bVisible;          // draw visibility flag
    uint8_t             m_bNetDirtyEnabled;  // network dirty tracking enabled
    uint32_t            m_netDirtyFlags;     // network dirty bitfield

    // ── Vtable methods (confirmed slots) ───────────────────────────────
    virtual ~pongPlayer();                               // slot 0
    virtual void ScalarDtor(int flags);                  // slot 1
    virtual void Process(float dt);                      // slot 2
    virtual void CheckButtonInput();                     // slot 7 — @ 0x8218EE10
    virtual void OnButtonReleased();                     // slot 9 — @ 0x821935B8

    // ── State query methods (0x820Cxxxx block) ─────────────────────────
    bool IsSwingTimerActive()     const;  // @ 0x820CD550
    bool IsRecovering()           const;  // @ 0x820CD598
    bool IsSwingPhaseBlocked()    const;  // @ 0x820CD5F0
    bool IsCreatureStateReady()   const;  // @ 0x820CD660
    bool IsCreatureState2Active() const;  // @ 0x820CD6B8
    bool CanAcceptSwingInput()    const;  // @ 0x820CC3C8
    bool IsBeforeSwingPeak()      const;  // @ 0x820CDCD8
    bool IsSwingApexReached(float threshold) const;  // @ 0x820CDA58
    bool IsSwingSystemIdle()       const;  // @ 0x820CDC98
    bool IsInReturnPosition()     const;                 // @ 0x820CDE98
    bool IsActionComplete()       const;                 // @ 0x820C7890
    bool CheckOpponentSwingApex() const;                 // @ 0x820C5890 (approx)


    // ── Swing execution ─────────────────────────────────────────────────
    void CancelSwing();                                   // @ 0x820CD908
    void ProcessSwingDecision(int r4, int r5,
                              bool cancelFirst,
                              const vec3* pTargetVec,
                              int r7);                   // @ 0x820C7038
    void SetupRecoverySlots(const vec3* pSlotA,
                            const vec3* pSlotB);          // @ 0x820C7248
    void SetSwingActiveState(bool active);                // @ 0x820C7C78

    // ── Per-frame update ────────────────────────────────────────────────
    void UpdatePhysicsAndSwingInput();                    // @ 0x820C5FB8
    void UpdateAnimationState();                          // @ 0x820CD228
    void GetSwingTargetVector(vec3* out,
                              pongPlayer* state) const;   // @ 0x820CD7B0
    void ProcessInputVector(float x, float y, float z,
                            uint8_t flags);               // @ 0x821A0050

    // ── Additional methods (lifted batches) ──────────────────────────────
    void UpdateSwingTimingAdjustment();
    void UpdatePositionFromSwingTarget();
    void InitializeCollisionGrid(int r4, uint8_t metadataByte);
    bool IsSwingTimerExpiredAndReady() const;
    void SetPlayerSide(uint8_t side);
    bool IsSwingTimerInActiveWindow() const;
    bool LerpTowardsTarget(float target, float rate, float deltaTime);
    bool IsRecoveryTimerBelowThreshold() const;
    void* GetStateObjectByIndex(uint32_t index) const;
    float GetSwingPhaseValue() const;
    bool IsSwingInputBlocked() const;
    float GetAnimationBlendWeight() const;
    float GetCurrentSwingStrength() const;
    bool IsMatchSlotValid() const;
    void UpdateSwingTrajectory();
    void UpdateCreatureSlotAnims(uint8_t resetFlag);
    void InitMovementAndContact();
    void ResetSwingSlotEntries(void* slotsBase);
    void ComputeShotSpeedForType(float inputPower);
    void NotifySlotChangeAndSync(void* slotsBase, uint32_t slotIndex,
                                 uint32_t columnIndex);
    void UpdateServeSpeed();
    void InitializeReplaySnapshot(void* outSnapshot);
    void ClampMovementToCourtBounds(float* delta);
    void SaveDrawData();
    int CompareTypeNames(void* a, void* b);
    bool IsLocomotionReady() const;
    void ResetServePosition();
    int GetFrameIndexDelta(void* frameData);
    void ComputePositionWithOffsets(void* inputData);
    void ComputePositionWithOffset();
    bool CheckHandednessDifference();
    void SyncByteField(void* target, void* source);
    void SyncFloatField(void* target, void* source);
    void UpdateDirtyFlags(void* obj, uint8_t forceReset);
    void MarkNetDirtyPosition();
    void MarkNetDirtyRotation();
    void MarkNetDirtyVelocity();
    void MarkNetDirtyAnimation();
    void MarkNetDirtyShotState();
    void MarkNetDirtyStamina();
    void MarkNetDirtyScore();
    void VtMarkNetDirtyPosition();
    void VtMarkNetDirtyRotation();
    void VtMarkNetDirtyVelocity();
    void VtMarkNetDirtyAnimation();
    void VtMarkNetDirtyShotState();
    void VtMarkNetDirtyStamina();
    void VtMarkNetDirtyScore();

    void SyncFieldWithCallback(void* target, void* source);
    void ResetShotSyncFields(void* shotState, uint8_t clearNetFlags);
    void ResetShotTrackingState();
    void ClearSwingTrajectoryData(void* trajectoryBlock);
    void ResetSwingTrackingState();
    void FinalizeServeSetup();
    void* LookupElementByFloatKey(void* table, float key);
    void RegisterDebugDrawEntries();

    // ── Additional methods (batch: pongPlayer small functions) ────────────
    void Update();                                           // @ 0x8218CF08
    void UpdateReplay();                                     // @ 0x8218E178
    void ResetSwingDisplayValues();                          // @ 0x821A7038
    void FindRegisteredListener(void* unused, void* target); // @ 0x8218AED0

    // ── Additional methods (batch 10: medium pongPlayer functions) ──────
    void UpdateOpponentBoundTransform();                      // @ 0x82194C08
    void UpdateOpponentSwingTiming(float swingDir, float swingPower); // @ 0x82193738
    static void ResetButtonSlotTrackingData(void* buttonStateBase, int slotIndex); // @ 0x821C9918
    void SendServeOrMovementMessage(vec3* targetVec, uint8_t hasDirectMovement); // @ 0x82195598
};

// ── Inner heap state: pongPlayerState ────────────────────────────────────
// Same layout as pongPlayer; used for the heap-allocated state sub-object.
// NOTE: get_class_context reports pongPlayer has 3 vtables (multiple
//       inheritance or virtual-base) and rtti_classes.txt has no distinct
//       `pongPlayerState` RTTI entry — consistent with pongPlayerState being
//       a re-use of pongPlayer's layout at +0xBC, treated as inheritance
//       here for compile-time typing. The methods below are apex/ready-check
//       overrides that run against the inner state's own m_pTimingState etc.
struct pongPlayerState : pongPlayer {
    bool IsSwingApexReached(float threshold) const;  // @ 0x820CDA58 (pongPlayerState override)
    bool IsBeforeSwingPeak()                const;  // @ 0x820CDCD8 (pongPlayerState override)
    bool IsSwingSystemIdle()                const;  // @ 0x820CDC98 (pongPlayerState override)
    bool IsInReturnPosition()               const;  // @ 0x820CDE98 (pongPlayerState override)
};


// ── Global constants and singletons ──────────────────────────────────────
extern const float g_recoveryTimerThreshold; // @ 0x8202D108
extern const float g_swingPhaseThreshold;    // @ 0x8202D110
extern const float g_kZero;                  // @ 0x8202D10C
extern const float g_kSwingScaleFactor;      // @ 0x8204A3B8
extern const vec3  g_hitVectorFlip;
extern void*       g_geomSingleton;
extern uint32_t    g_swingInFlightFlag;      // @ lis(-32160)+25404
extern uint32_t    g_swingCountFlag;         // @ lis(-32160)+25408

// ── Batch: NetSyncFloat record helpers (0x82199Cxx family) ─────────────
// These operate on a NetSyncFloatRec — a sync-tracked float field used by the
// shot/stamina/score sub-records.  Layout (inferred from callers & SyncFloatField):
//   +0x00  float    m_currentValue      // current float value
//   +0x04  void*    m_callbackArg        // passed to m_onChange when value changes
//   +0x08  void*    m_onChange           // optional change callback (uint8_t *after all)
//   +0x18  uint8_t  m_updateInhibit      // if != 0 the value is never written by 9C08/9C90
// Note: callers pass the RECORD BASE as r3; the asm does `addi r3,r3,8` internally
// to land on m_currentValue before invoking SyncFloatField(this+8, &local).
extern void pongPlayer_DecrementSyncedFloatClamped(void* syncRec, float deltaMagnitude);        // 9C08_g
extern void pongPlayer_SetSyncedFloatClamped(void* syncRec);                                    // 9C90_g (uses m_currentValue)
extern void pongPlayer_AddToSyncedFloatClamped(void* syncRec, float delta, uint32_t bitSlot);   // 9AE0_g

// ── Batch 10 function declarations ──────────────────────────────────────
extern void pongPlayer_E640_g(void* gridSubObj);                     // ClearSwingSlotGrid @ 0x8219E640
extern void pongPlayer_E590_g(float* lerpFactors, uint8_t flip,
                               float* outPos, float* boundsA,
                               float* boundsB, float* offset);       // InterpolateGridPosition @ 0x8219E590
extern void pongPlayer_3628_g(pongPlayer* player, uint8_t flip);     // SetServeSideFlip @ 0x82193628
extern bool pongPlayer_6050_p46(int32_t swingType);                  // IsSwingType @ 0x821D6050
extern void pongPlayer_0F10_g(void* stateObj, float* outSign,
                               float deadZone);                       // GetCourtSideSign @ 0x821A0F10
extern void pongPlayer_76E8_g(void* swingHistory);                   // InitSwingHistorySlots @ 0x821A76E8
extern void pongPlayer_AE10_g(void* slotsBase, int32_t slotId,
                               int32_t column);                       // RemoveSlotEntry @ 0x8218AE10
extern void pongPlayer_4438_fw(void* transObj);                      // UpdateTransitionBlend @ 0x82384438
extern void pongPlayer_D6C8_fw(void* animTransObj);                  // ResetAnimTransitionState @ 0x8239D6C8
extern void pongPlayer_06A0_g(float* outVec, pongPlayer* player);   // ComputeSwingTargetDelta @ 0x821906A0
