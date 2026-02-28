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
 *   +0x1F0  void*               m_pActionState       passed to pongPlayer_1460_g in Update
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
// pongTimingSubState: swing timing sub-state
struct pongTimingSubState {
    void*       vtable;           // +0x00
    uint8_t     _pad04[156];      // +0x04..+0x9F placeholder
    float       m_swingProgress;  // +0xA0 (160) — used by CanAcceptSwingInput
    uint8_t     _padA4[4];        // +0xA4
    uint32_t    m_frameId;        // +0xA8 — frame counter / animation frame reference
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
    uint8_t     _pad0[12];
    float       m_targetX;       // +0x0C  zero-filled by CancelSwing
    float       m_targetY;       // +0x10
    float       m_targetZ;       // +0x14
    uint8_t     _pad1[4];
    float       m_currentTime;   // +0x1C  elapsed swing time
    float       m_currentTime2;  // +0x20
    float       m_peakX;         // +0x24  position component / velocity data
    float       m_peakY;         // +0x28  confirmed: compared vs normalised fraction
    float       m_peakTime;      // +0x2C
    float       m_targetTime;    // +0x30  full swing duration
    uint8_t     _pad2[24];
    float       m_vel0;          // +0x40
    float       m_vel1;          // +0x44
    float       m_vel2;          // +0x48
    uint8_t     _pad3[4];
    float       m_vel3;          // +0x50
    float       m_vel4;          // +0x54
    float       m_vel5;          // +0x58
    uint8_t     _pad4[4];
    uint8_t     m_bComplete;     // +0x8D  cleared by CancelSwing
};

// ── Sub-struct: pongRecoveryState ────────────────────────────────────────
struct pongRecoveryState {
    uint8_t     _pad[44];
    uint8_t     m_bForceBlock;   // +0x2C
    uint8_t     _pad2[3];
    void*       m_pFollowAction; // +0x24 ? — checked for NULL in IsSwingApexReached
    uint8_t     _padN[64];
    float       m_vec0[4];       // +0x50  recovery target vector slot A (copied in SetupRecoverySlots)
    float       m_vec1[4];       // +0x60  recovery target vector slot B
    float       m_swingPhase;    // somewhere; TODO: verify exact offset
    float       m_recoveryTimer;    // +0xAC (172) — compared vs g_recoveryTimerThreshold in D598
};

// ── Sub-struct: pongAnimState ────────────────────────────────────────────
struct pongAnimState {
    void*       vtable;          // +0x00
    void*       m_pFrameArray;   // +0x04  frame-data array pointer
    float       _pad1[22];
    float       m_animProgress;  // +0x60  (96 bytes from base)
    uint8_t     _pad2[312];
    float       m_swingPhase;    // +0x19C (412 bytes) — confirmed from D908, 73E8
    uint32_t    m_flags;         // bit 0 = IsSwingPhaseBlocked secondary
    float       m_animPhase;     // compared vs g_swingPhaseThreshold
};

// ── Sub-struct: pongCreatureState ────────────────────────────────────────
struct pongCreatureState {
    void**      vtable;
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
    uint8_t             _pad3[12];
    uint8_t             m_team;              // +0x04C
    uint8_t             _pad4[3];
    void*               m_pGameState;        // +0x050
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
    uint8_t             _pad11[468];
    pongPlayer*         m_pOpponent;         // +0x1C4 (+452) R:66 W:0 — set once at init
    uint32_t            _unk_0x1C8;          // +0x1C8
    uint32_t            _unk_0x1CC;          // +0x1CC
    int32_t             m_inputSlotIdx;      // +0x1D0 (+464) — player's button/input slot index
                                             //   MOST ACCESSED (R:112); per-frame input polling
    uint8_t             _pad12[20];
    void*               _unk_0x1E8;          // +0x1E8
    uint32_t            _unk_0x1EC;          // +0x1EC
    void*               m_pActionState;      // +0x1F0 (+496) — passed to pongPlayer_1460_g

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
};

// ── Inner heap state: pongPlayerState ────────────────────────────────────
// Same layout as pongPlayer; used for the heap-allocated state sub-object.
// TODO: confirm whether this is a separate type or inheritance.
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
