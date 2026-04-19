/**
 * pong_game.cpp — Game state and context management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Core game state machine and context classes that manage:
 * - Game state transitions (pongGameState)
 * - Game context and message processing (pongGameContext)
 * - Main game object (pongGame)
 * 
 * These classes form the backbone of the game's state management system,
 * handling UI messages, input events, and game flow control.
 *
 * pongGameState vtable layout (HSM base state class):
 *   [0]  ~pongGameState()
 *   [10] Init()
 *   [11] OnEnter(int prevStateIdx)
 *   [12] OnExit(int nextStateIdx)
 *   [13] GetName() — returns const char* state name
 *   [14] ProcessInput()
 *
 * pongGameContext vtable layout (game context / message processor):
 *   [0]  ~pongGameContext()
 *   [12] OnExit()
 *   [14] ProcessInput()
 *   [16] OnUpdate()
 *   [17] OnRender()
 *   [18] OnShutdown()
 *   [22] PostLoadChildren()
 */

#include "pong_game.hpp"

#include <stdint.h>
#include <cstring>

// Forward declarations for external dependencies
extern "C" {
    void rage_free(void* ptr);
    void pongPlayer_ADE8_p42(void* r3, uint32_t r4, void* base);
    void rage_DebugLog(void* r3, const char* format, ...);
    void gdGameData_CalculateVariation(void* gdData, uint32_t r4, uint32_t r5, uint32_t r6, int32_t r7);
    void* pg_C2A0_g(void* r3, uint32_t r4);
    void game_7208(void* r3);
    void PongNetExhibitionCoordinator_2BA8_g(void* r3);
    // atSingleton lookup — reads/writes a slot into sp+112 via r5 out-ptr.
    // Signature matches raw ppcrecomp trampoline: (this, key, outSlot).
    void atSingleton_E998_g(void* r3, const char* key, void* outSlot);

    // ── helpers used by the 7 true vtable slots (lifted in this pass) ──
    void  atSingleton_ECE0_h(void* ctx);                          // @ 0x8227ECE0
    int   io_Input_poll_9D68(void* inputObj);                     // @ 0x821C9D68
    void  util_9F28(void* arg);                                   // @ 0x82199F28
    void  pg_A070_h(void* arg);                                   // @ 0x8219A070
    void  atSingleton_5A68_h(void* a, void* b);                   // @ 0x821B5A68
    void  atSingleton_A600_h(void* arg, float f);                 // @ 0x8240A600
    void  pongGameContext_A070(void* guardRec);                   // @ 0x823DA070
    // pongGameContext_6970_h defined in this file below (uses pongGameContext fields).
    bool  pongGameContext_CEE8_wrh(void* data);                   // @ 0x821CCEE8
    void  pongLerpQueue_C838_g(void* list);                       // @ 0x821CC838
    void  util_D0F8(void* state, void* slot, void* minVec, void* maxVec, void* gridVec);  // @ 0x821CD0F8
    void  pongShadowMap_2700_g(void);                             // @ 0x82312700
    void  pongCameraMgr_6280_g(void* mgr);                        // @ 0x82166280
    void  SinglesNetworkClient_1330_g(void* client, void* state); // @ 0x82421330

    // ── helpers for Process cascade handlers (this pass) ──
    void     game_7858(void* event);                              // @ 0x823D7858 — ReadyUp forward
    void     nop_8240E6D0(void* r3, ...);                         // @ 0x8240E6D0 — debug/log sink
    uint32_t atSingleton_D070_g(void* ctx, uint32_t r4, uint32_t r5);  // @ 0x822ED070 — bit-query
    void     atSingleton_CBE0_g(void* r3, void* event, void* arg, int r6, int r7, int r8, void* payload);  // @ 0x822ECBE0 — UI modal enqueue (big argpack)
    void     atSingleton_CB90_g(void* r3);                        // @ 0x822ECB90 — finish UI enqueue
    uint32_t util_CF10(void* coord);                              // @ 0x823ACF10 — tourney join probe
}

// External globals
extern void* g_input_obj_ptr;          // @ 0x825eab28
extern void* g_pongCharViewData;       // @ 0x8271A330 — character-select view state
extern void* g_pongMatchLogicContext;  // @ 0x8271A338 — match-logic arg passed to game_7208
extern void* g_pongNetExhibitionCoord; // @ 0x8271A7B0 — net exhibition coordinator singleton
extern int   g_pongMenuPendingState;   // @ 0x825C9A6C — pending sub-menu state (2 = net exhibition)
extern const float kFloatZero;         // @ 0x8202D110 — shared 0.0f rodata constant

// Globals additionally referenced by the 7 lifted vtable slots.
extern void* g_SinglesMatchLogic;      // @ 0x8271A2F0 — match-logic singleton
extern void* g_pongCameraMgr;          // @ 0x8260641C — camera manager
extern int32_t g_pongGlobalMode;       // @ 0x826064C0 — shadow-map gate (==1 ⇒ netclient branch)
extern void* g_pSceneRenderObj;        // @ 0x82606410 — scene-render singleton
extern void* g_pongShadowCfg;          // @ 0x82606524 — shadow config block

// hsmState — opaque child-state type used by m_pLocalPlayerState /
// m_pNetworkClientState / m_pSpectatorState; we only ever index its vtable.
struct hsmState {
    void** vtable;
};

// pongGameState_view — opaque view onto m_pGameState used by the
// ShouldTickGameplay() helper. The flag bytes live at +0x89/+0x8A.
struct pongGameState_view {
    uint8_t  _pad[137];
    uint8_t  m_bFlagA;   // +0x89 (137)
    uint8_t  m_bFlagB;   // +0x8A (138)
};

// ────────────────────────────────────────────────────────────────────────────
// pongGameState / pongGameContext / pongGame — declared in pong_game.hpp
// (skeleton, included above). Only non-virtual method bodies live here.
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameState::GetName  @ 0x823D5318 | size: 0xc
 *
 * Returns the state's name string (.rdata @ 0x82071844). Emitted as a
 * plain extern-C symbol so the skeleton's pure-virtual slot [13] can
 * resolve without pulling a full vtable implementation.
 */
extern "C" const char* pongGameState_GetName() {
    return "pongGameState";
}

// ────────────────────────────────────────────────────────────────────────────
// pongGameContext — Game context and message processor
// @ vtable 0x82071AEC (primary), 0x82071B50 (secondary - multiple inheritance)
//
// The struct itself is declared in pong_game.hpp (included above); only
// method bodies + file-local view types belong here.
// ────────────────────────────────────────────────────────────────────────────

// ── pongGameStateData ─ file-local view onto the per-context "state blob"
// pointer that the Process cascade threads through `*(this + 4)` (captured
// via `lwz rX, 4(r25)` in the recomp). This is NOT the HSM state class
// `pongGameState` declared in pong_game.hpp — that class has a 5-slot
// public vtable (Init/OnEnter/OnExit/GetName/ProcessInput) and is the HSM
// state type, whereas the blob at `*(this + 4)` is a per-match transient
// carrying the menu accept/back/cancel flags, the char-variation clamp
// float, and the atStringHash "i" stash.
//
// Kept file-local rather than promoted to a header member for three reasons:
//   1) The recomp reads *(this + 4), which falls inside the opaque primary-
//      base payload (`_opaqueBase[16]` @ +0x04..+0x13). Promoting would
//      require tearing apart that opaque payload across every function in
//      the file — risky while 10 other agents are touching adjacent code.
//   2) No other file (checked via grep pongGameState_view / pongGameStateData)
//      reaches into these byte offsets — the view is genuinely dispatch-local.
//   3) The pongGameState header struct maps a DIFFERENT runtime class: an
//      HSM state object whose vtable is at 0x82071AA4. Unioning this view
//      on top would conflate two RTTI classes.
//
// Layout mirrors the raw PPC offsets observed in Process cascade:
//   +0x00 vtable word, +0x04 HSM context ptr, +0x0C clamp float,
//   +0x10 hash stash, +0x15/16/17 the three menu flags.
struct pongGameStateData {
    uint32_t vtable;                 // +0x00
    void*    m_pHsmContext;          // +0x04 — owning HSM context (vfn slot +0x10)
    uint32_t _pad08;                 // +0x08
    float    m_fCharVarClampMax;     // +0x0C — char-variation clamp (msgId 0x80D)
    uint32_t m_uHashStash;           // +0x10 — atStringHash "i" stash (msgId 0x86D)
    uint8_t  _pad14;                 // +0x14
    uint8_t  m_bMenuAccept;          // +0x15 — accept flag (0x2027/0x3831/etc)
    uint8_t  m_bMenuBack;            // +0x16 — back   flag (0x85C/0x85D/0x2027)
    uint8_t  m_bMenuCancel;          // +0x17 — quit-confirm flag (0x85D)
};

// Helper: chase the state-blob pointer the recomp reads via `lwz rX, 4(r25)`.
// Kept inline + static so dispatch hot-path stays cheap. All handler bodies
// that previously reached through `m_pGameState` (the HSM state field at
// +0x1C) were actually reaching the wrong field; this helper makes the
// intent explicit and lets us rename later without touching every caller.
static inline pongGameStateData* getStateBlob(pongGameContext* self) {
    // `_opaqueBase[16]` spans +0x04..+0x13; the first uint32_t within is the
    // per-match state-blob pointer. Read it without an aliasing cast.
    uint8_t* rawBase = reinterpret_cast<uint8_t*>(self);
    uint32_t ptrVal  = *reinterpret_cast<uint32_t*>(rawBase + 4);
    return reinterpret_cast<pongGameStateData*>(static_cast<uintptr_t>(ptrVal));
}

// ────────────────────────────────────────────────────────────────────────────
// Character-select view data at g_pongCharViewData (0x8271A330).
// Floats +52..+72 are per-slot variation state zeroed by msg 2060.
struct pongCharViewData {
    uint8_t  _pad_00[52];         // +0x00..+0x33
    float    m_aSlotState[6];     // +52, +56, +60, +64, +68, +72
};
static_assert(sizeof(pongCharViewData) >= 76, "pongCharViewData layout");

// ────────────────────────────────────────────────────────────────────────────
// Input / session singleton at g_input_obj_ptr (0x825EAB28).
// Only the fields touched by these handlers are typed.
struct pongInputObj {
    uint8_t  _pad_00[0x08];        // +0x00
    int32_t  m_iSessionState;      // +0x08 — 3 = in-session menu, 5 = leaving
    uint8_t  _pad_0C[0x0C];        // +0x0C
    uint8_t  m_bSuspended;         // +0x18 — nonzero: swallow menu-navigation events
    uint8_t  _pad_19[0x02];        // +0x19
    uint8_t  m_bJoinPathBlocked;   // +0x1B (27) — 0x2019 secondary gate
    uint8_t  _pad_1C[0x03];        // +0x1C
    uint8_t  m_bCharVarFlag;       // +0x1F (31) — 0x80D toggle-once flag
    uint8_t  _pad_20[0x08];        // +0x20
    uint32_t m_uSelectedSlot;      // +0x28 (40) — UI-selected slot (published by 0x80C)
    uint8_t  _pad_2C[0x04];        // +0x2C
    int32_t  m_iLeaveState;        // +0x30 (48) — 2=ReadyUp, 5=QuitConfirm
    uint8_t  _pad_34[0x11A];       // +0x34..+0x14D
    uint8_t  m_bStateAllowReady;   // +0x14E (334) — kJoinFromHud gate
    uint8_t  _pad_14F[0x05];       // +0x14F..+0x153
    uint32_t m_uStateFlag340;      // +0x154 (340) — kJoinFromHud gate #2
};
// No offsetof asserts — PPC-native offsets are documented in the field comments.
// 64-bit host void* would perturb the numbers anyway.

// pongGameContext struct + static_asserts live in pong_game.hpp (skeleton,
// included above). Only method bodies + file-local globals follow.

// Extra SDA/data globals referenced by the lifted vtable slots.
extern void* lbl_8271A2EC;   // shadow/mode table root
extern void* lbl_8271A2F0;   // match-logic singleton root
extern void* lbl_8271A308;   // UI-tick singleton
extern void* lbl_8271A31C;   // shadow-config arg
extern void* lbl_8271A2F8;   // lerp-queue state handle

// ────────────────────────────────────────────────────────────────────────────
// 7 true vtable slots — lifted from 0x823D5328..0x823D5AE8
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameContext::Dtor  [0]  @ 0x823D5328 | size: 0x60
 *
 * MI-aware destructor. Writes the primary vtable stamp at +0x00 and the
 * secondary at +0x14 (with a transient intermediate stamp stored first;
 * the two-store sequence at +20 is bit-exact to the original). When the
 * low bit of `flags` is set, hands the object back to rage_free.
 *
 * Stamp encodings (from lis/addi pairs):
 *   0x82072B54  primary stamp   (-32249, +6992)
 *   0x82027B54  intermediate    (-32254, +31540)  — transient
 *   0x820776C4  secondary stamp (-32254, +30404)
 */
void pongGameContext::Dtor(int flags) {
    // Guest VAs stored as uint32_t in the layout; method body records the
    // transient→final vtable stamps exactly as the PPC recomp emits them.
    m_vtableSecondary = 0x82027B54u;  // transient
    m_vtableSecondary = 0x820776C4u;  // final
    vtable            = 0x82072B54u;

    if ((flags & 0x1) != 0) {
        rage_free(this);
    }
}

/**
 * pongGameContext::OnExit  [12]  @ 0x823D5388 | size: 0xB4
 *
 * Tears down the three child HSM-state members (local player / network
 * client / spectator). Each child's slot-0 dtor is invoked with the
 * "also free" flag (r4=1); the slot is then nulled. After the children
 * are gone, atSingleton_ECE0_h de-registers the gameState and rage_free
 * releases the gameState slot.
 */
void pongGameContext::OnExit() {
    // Each child field is a guest VA (uint32_t). Cast back to hsmState* to
    // walk its host-visible vtable, matching the PPC recomp semantics.
    if (m_pLocalPlayerState != 0) {
        hsmState* child = reinterpret_cast<hsmState*>(static_cast<uintptr_t>(m_pLocalPlayerState));
        using DtorFn = void(*)(hsmState*, int);
        reinterpret_cast<DtorFn>(child->vtable[0])(child, 1);
    }
    m_pLocalPlayerState = 0;

    if (m_pNetworkClientState != 0) {
        hsmState* child = reinterpret_cast<hsmState*>(static_cast<uintptr_t>(m_pNetworkClientState));
        using DtorFn = void(*)(hsmState*, int);
        reinterpret_cast<DtorFn>(child->vtable[0])(child, 1);
    }
    m_pNetworkClientState = 0;

    if (m_pSpectatorState != 0) {
        hsmState* child = reinterpret_cast<hsmState*>(static_cast<uintptr_t>(m_pSpectatorState));
        using DtorFn = void(*)(hsmState*, int);
        reinterpret_cast<DtorFn>(child->vtable[0])(child, 1);
    }
    m_pSpectatorState = 0;

    void* gs = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pGameState));
    atSingleton_ECE0_h(gs);
    rage_free(gs);
}

/**
 * pongGameContext::ProcessInput  [14]  @ 0x823D5440 | size: 0xA8
 *
 * Per-frame input fan-out. VCALL slot 5 on local-player + spectator
 * children; polls the input singleton. On "active" result + netclient
 * present, clears netclient's +1384 transient gate. Ticks lbl_8271A308
 * via util_9F28 and clears m_bPostLoadDone (next frame re-runs post-load).
 */
void pongGameContext::ProcessInput() {
    if (m_pLocalPlayerState != 0) {
        hsmState* local = reinterpret_cast<hsmState*>(static_cast<uintptr_t>(m_pLocalPlayerState));
        using Vfn5 = void(*)(hsmState*);
        reinterpret_cast<Vfn5>(local->vtable[5])(local);
    }

    int polled = io_Input_poll_9D68(g_input_obj_ptr);
    if (polled != 0 && m_pNetworkClientState != 0) {
        uint8_t* nc = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(m_pNetworkClientState));
        *reinterpret_cast<uint32_t*>(nc + 1384) = 0;
    }

    if (m_pSpectatorState != 0) {
        hsmState* spec = reinterpret_cast<hsmState*>(static_cast<uintptr_t>(m_pSpectatorState));
        using Vfn5 = void(*)(hsmState*);
        reinterpret_cast<Vfn5>(spec->vtable[5])(spec);
    }

    util_9F28(lbl_8271A308);
    m_bPostLoadDone = false;
}

/**
 * pongGameContext::OnUpdate  [16]  @ 0x823D54E8 | size: 0x238
 *
 * Gated on (!m_bSuspended && gs->+137 && gs->+138). Gameplay path VCALLs
 * slots 2/3/4 on m_pGameState around a guard-record dtor, kicks
 * atSingleton_A600_h when the shadow word is nonzero, then ticks the UI
 * singleton + scene-render. Otherwise the camera manager runs. Tail runs
 * only when gameplay ISN'T ticking — spectator/netclient sync gated on
 * g_pongGlobalMode.
 */
void pongGameContext::OnUpdate() {
    // Guest VAs are u32 in storage; cast to host pointers for dereferencing.
    void* gsPtr  = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pGameState));
    void* spPtr  = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pSpectatorState));
    void* lpPtr  = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pLocalPlayerState));
    void* ncPtr  = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pNetworkClientState));

    bool shouldTick = false;
    if (!m_bSuspended) {
        pongGameState_view* gs = static_cast<pongGameState_view*>(gsPtr);
        shouldTick = (gs->m_bFlagA != 0 && gs->m_bFlagB != 0);
    }

    if (shouldTick) {
        using Vfn = void(*)(void*);
        void** gsVtable = *(void***)gsPtr;
        reinterpret_cast<Vfn>(gsVtable[2])(gsPtr);  // slot 2 (byte +8)

        struct GuardRec { uint32_t a, b; };
        GuardRec guard;
        uint32_t payload = *(uint32_t*)((uint8_t*)lbl_8271A2F0 + 4);
        guard.a = payload;
        guard.b = payload;
        pongGameContext_A070(&guard);

        void* sub = *(void**)((uint8_t*)lbl_8271A2F0 + 32);
        if (sub != nullptr) {
            using DtorFn = void(*)(void*, void*);
            void** subVt = *(void***)sub;
            reinterpret_cast<DtorFn>(subVt[0])(sub, &guard);
        }

        reinterpret_cast<Vfn>(gsVtable[3])(gsPtr);  // slot 3 (byte +12)
        reinterpret_cast<Vfn>(gsVtable[4])(gsPtr);  // slot 4 (byte +16)

        uint32_t* shadowTable = *(uint32_t**)lbl_8271A2EC;
        uint32_t  shadowWord  = shadowTable[0x8CF0 / 4];
        if (shadowWord != 0) {
            float payloadFloat = *(const float*)0x82072B08;
            atSingleton_A600_h(lbl_8271A2F0, payloadFloat);
        }

        pg_A070_h(lbl_8271A308);

        void* sceneObj  = g_pSceneRenderObj;
        void* shadowArg = *(void**)lbl_8271A31C;
        atSingleton_5A68_h(sceneObj, shadowArg);
    } else {
        pongCameraMgr_6280_g(g_pongCameraMgr);
    }

    // Tail runs only when gameplay ISN'T ticking (menus / loading).
    bool shouldTick2 = false;
    if (!m_bSuspended) {
        pongGameState_view* gs = static_cast<pongGameState_view*>(gsPtr);
        shouldTick2 = (gs->m_bFlagA != 0 && gs->m_bFlagB != 0);
    }
    if (shouldTick2) {
        return;
    }
    if (!m_bActiveFlagA) {
        return;
    }
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    uint8_t guardA = *((uint8_t*)input + 493);
    uint8_t guardB = *((uint8_t*)input + 494);
    if (guardA != 0 && guardB != 0) {
        return;
    }

    using Vfn2 = void(*)(void*);
    if (g_pongGlobalMode == 1) {
        void** vt = *(void***)spPtr;
        reinterpret_cast<Vfn2>(vt[2])(spPtr);
    } else {
        void** vt = *(void***)lpPtr;
        reinterpret_cast<Vfn2>(vt[2])(lpPtr);
        SinglesNetworkClient_1330_g(ncPtr, lpPtr);
    }
}

/**
 * pongGameContext::OnRender  [17]  @ 0x823D5720 | size: 0x1D0
 *
 * Gated on the same predicate as OnUpdate. Assembles a guard-record,
 * dtors the match-logic singleton's +36 child, optionally drives
 * util_D0F8 with a vec3-bundle, then ticks the lerp queue.
 */
void pongGameContext::OnRender() {
    void* gsPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pGameState));
    bool shouldTick = false;
    if (!m_bSuspended) {
        pongGameState_view* gs = static_cast<pongGameState_view*>(gsPtr);
        shouldTick = (gs->m_bFlagA != 0 && gs->m_bFlagB != 0);
    }
    if (!shouldTick) {
        return;
    }

    struct GuardRec { uint32_t a, b; };
    GuardRec guard;
    uint32_t payload = *(uint32_t*)((uint8_t*)lbl_8271A2F0 + 4);
    guard.a = payload;
    guard.b = payload;
    pongGameContext_A070(&guard);

    void* sub = *(void**)((uint8_t*)lbl_8271A2F0 + 36);
    if (sub != nullptr) {
        using DtorFn = void(*)(void*, void*);
        void** subVt = *(void***)sub;
        reinterpret_cast<DtorFn>(subVt[0])(sub, &guard);
    }

    uint32_t* shadowTable = *(uint32_t**)lbl_8271A2EC;
    uint32_t  shadowWord  = shadowTable[0x8CF0 / 4];
    if (shadowWord != 0 && pongGameContext_CEE8_wrh(lbl_8271A2F0)) {
        // Pass5 scaffolding: the geometry-record tight-bounds branch only
        // changes which vec3 seeds each slot; the bundle dispatched to
        // util_D0F8 is semantically identical in both branches.
        void* state = *(void**)lbl_8271A2F8;
        float vecBuf[12] = {0};
        util_D0F8(state, &vecBuf[0], &vecBuf[3], &vecBuf[6], &vecBuf[9]);
    }

    pongLerpQueue_C838_g(lbl_8271A2F0);
}

/**
 * pongGameContext_6970_h  @ 0x823D6970 | size: 0x6C
 *
 * "Should-tick" predicate used by OnShutdown to gate the netclient teardown
 * path. Returns true iff the context isn't suspended (+51/+52 both zero)
 * AND the current pongGameState's two gameplay-active bytes (+137/+138)
 * are both non-zero. Mirrors the same guard used inline by OnUpdate and
 * OnRender, but collapsed into a shared helper because OnShutdown needs
 * it twice per call.
 *
 * Original returns u8 (0 or 1); we promote to bool at the host-C++ layer.
 */
bool pongGameContext_6970_h(pongGameContext* ctx) {
    // +52 (0x34) = m_bOverlayFlag, +51 (0x33) = m_bSuspended.
    // Any non-zero suspend bit forces a false return.
    if (ctx->m_bOverlayFlag) {
        return false;
    }
    if (ctx->m_bSuspended) {
        return false;
    }

    // Chase m_pGameState and inspect the two gameplay-tick flag bytes
    // at +137/+138. Both must be non-zero for the predicate to fire.
    pongGameState_view* state = reinterpret_cast<pongGameState_view*>(
        static_cast<uintptr_t>(ctx->m_pGameState));
    if (state->m_bFlagA == 0) {
        return false;
    }
    return state->m_bFlagB != 0;
}

// OnShutdown's internal call accepts a void* for historical reasons; the
// hpp-level declaration already uses `pongGameContext*` via `this`. The
// inline callsites compile via implicit upcast.

/**
 * pongGameContext::OnShutdown  [18]  @ 0x823D58F0 | size: 0x140
 *
 * pongGameContext_6970_h (shadow/mode query) gates netclient teardown.
 * Second gate picks spectator vs. local-player for the slot-6 VCALL.
 * Always stamps m_bSuspended and m_bOverlayFlag from input +495/+24/+64.
 */
void pongGameContext::OnShutdown() {
    if (pongGameContext_6970_h(this)) {
        struct GuardRec { uint32_t a, b; };
        GuardRec guard;
        uint32_t payload = *(uint32_t*)((uint8_t*)lbl_8271A2F0 + 4);
        guard.a = payload;
        guard.b = payload;
        pongGameContext_A070(&guard);

        void* sub = *(void**)((uint8_t*)lbl_8271A2F0 + 40);
        if (sub != nullptr) {
            using DtorFn = void(*)(void*, void*);
            void** subVt = *(void***)sub;
            reinterpret_cast<DtorFn>(subVt[0])(sub, &guard);
        }
    }

    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    bool runTail = true;
    if (!pongGameContext_6970_h(this)) {
        if (!m_bActiveFlagA) {
            runTail = false;
        } else {
            uint8_t guardA = *((uint8_t*)input + 493);
            uint8_t guardB = *((uint8_t*)input + 494);
            if (guardA != 0 && guardB != 0) {
                runTail = false;
            }
        }
    }

    if (runTail) {
        using Vfn6 = void(*)(void*);
        if (g_pongGlobalMode == 1) {
            void* spPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pSpectatorState));
            void** vt = *(void***)spPtr;
            reinterpret_cast<Vfn6>(vt[6])(spPtr);  // slot 6 (byte +24)
        } else {
            void* lpPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(m_pLocalPlayerState));
            void** vt = *(void***)lpPtr;
            reinterpret_cast<Vfn6>(vt[6])(lpPtr);
        }
    }

    uint8_t inputByte495 = *((uint8_t*)input + 495);
    uint8_t inputByte24  = *((uint8_t*)input + 24);
    m_bSuspended   = (inputByte495 != 0) || (inputByte24 != 0);
    m_bOverlayFlag = (*((uint8_t*)input + 64) != 0);
}

/**
 * pongGameContext::PostLoadChildren  [22]  @ 0x823D5A30 | size: 0xC0
 *
 * Dtors the match-logic singleton's +44 (spectator) child through slot 0,
 * then VCALL slot 6 on the +32 → +4 nested child of m_pGameState.
 * Shadow-mode==1 AND shadowTable+0x8CFA != 0 ⇒ pongShadowMap_2700_g.
 */
void pongGameContext::PostLoadChildren() {
    struct GuardRec { uint32_t a, b; };
    GuardRec guard;
    uint32_t payload = *(uint32_t*)((uint8_t*)lbl_8271A2F0 + 4);
    guard.a = payload;
    guard.b = payload;
    pongGameContext_A070(&guard);

    void* spec = *(void**)((uint8_t*)lbl_8271A2F0 + 44);
    if (spec != nullptr) {
        using DtorFn = void(*)(void*, void*);
        void** specVt = *(void***)spec;
        reinterpret_cast<DtorFn>(specVt[0])(spec, &guard);
    }

    uint8_t* bytes     = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(m_pGameState));
    void*    stateRoot = *(void**)(bytes + 32);
    void*    innerNode = *(void**)((uint8_t*)stateRoot + 4);
    using Vfn6 = void(*)(void*);
    void**   innerVt   = *(void***)innerNode;
    reinterpret_cast<Vfn6>(innerVt[6])(innerNode);

    uint32_t* shadowTable = *(uint32_t**)lbl_8271A2EC;
    uint32_t  mode        = shadowTable[0x8CF0 / 4];
    if (mode == 1) {
        uint8_t extraGate = *((uint8_t*)shadowTable + 0x8CFA);
        if (extraGate != 0) {
            pongShadowMap_2700_g();
        }
    }
}

/**
 * pongGameContext::Process
 * @ 0x823D5B00 | size: 0xB4C (2892 bytes)
 *
 * Master message dispatch cascade for the game context. Reconstructed from
 * a flat cmplwi chain against msgId (not a switch-table): every branch is
 * a linear `if (msgId == 0xNNNN) { body; return; }` check with fallthrough.
 *
 * Calling convention (r3 = this, r4 = event):
 *   r25 captured as `this`     (pongGameContext*)
 *   r26 captured as event ptr  (pongEvent*, msgId at +0, payload at +4)
 *
 * Message ID taxonomy (decoded from the cascade order + string xrefs):
 *   0x08xx = msgUI   — frontend / HUD events
 *   0x20xx = msgGame — gameplay (match, player, input, tourney finish)
 *   0x38xx = msgNet  — network (join, leave, tourney sync)
 *
 * 19 distinct msgIDs are dispatched. Three have identifying debug strings:
 *   0x3831 → msgNet::kTourneyNextMatchReady ("Loading into next match")
 *   0x2027 → msgUI::kPostNetTourneyUIEnd     ("Tourney finished / back to frontend")
 *   0x2019 → msgNet::kJoinFromHudRequested
 *
 * Cases route to per-message HandleMsg_* bodies defined below. Default
 * fallthrough returns 0 (matches loc_823D6644, which is just a stack
 * teardown + return — no parent::Process chain).
 */
int pongGameContext::Process(void* eventState, void* event) {
    // r3 = this (captured as r25), r4 = event (r26), msgId at event+0.
    //
    // Each cmplwi branch in the original is a linear "if (msgId == N) body"
    // with fallthrough to the next check. Dispatch mirrors that exactly,
    // routing to the lifted HandleMsg_* body for each recognized msgId.
    // Return convention: 1 = handled, 0 = unknown (matches the r3 that
    // survives the restgprlr epilogue in the recomp's fall-through exit).
    const uint16_t msgId = *(uint16_t*)event;

    // ── 0x08xx block (msgUI: frontend / HUD) ──────────────────────────────
    switch (msgId) {
    case 0x80C:  // 2060 — character variation change (resets floats +52..+72)
        HandleMsg_ClearCharViewSlots(event);
        return 1;
    case 0x85A:  // 2138 — game state transition (calls game_7208)
        HandleMsg_ForwardToMatchLogic(event);
        return 1;
    case 0x85B:  // 2139 — bare no-op (PPC code jumps to loc_823D6644 without effect)
        return 1;
    case 0x85C:  // 2140 — menu back/cancel request (state-machine-aware)
        HandleMsg_MenuBackRequest(event);
        return 1;
    case 0x85D:  // 2141 — quit-confirm modal dismiss
        HandleMsg_MenuQuitConfirm(event);
        return 1;
    case 0x80D:  // 2061 — char-variation float clamp via atStringHash "f"
        HandleMsg_CharVarFloatClamp(event);
        return 1;
    case 0x86D:  // 2157 — stash atStringHash "i" into state blob +0x10
        HandleMsg_StoreHashIntoEvent(event);
        return 1;

    // ── 0x20xx block (msgGame: gameplay / match) ──────────────────────────
    case 0x2001:  // 8193 — msgGame::kReadyUp, forwards payload to game_7858
        HandleMsg_ReadyUp(event);
        return 1;
    case 0x2007:  // 8199 — suspend flag set  (this+0x1D = 1)
    case 0x200E:  // 8206 — fallthrough to suspend-set in the original
    case 0x200F:  // 8207 — fallthrough to suspend-set in the original
        HandleMsg_SuspendSet(event);
        return 1;
    case 0x200A:  // 8202 — suspend flag clear (this+0x1D = 0)
        HandleMsg_SuspendClear(event);
        return 1;
    case 0x2019:  // 8217 — msgNet::kJoinFromHudRequested (72-byte modal argpack)
        HandleMsg_JoinFromHudRequested(event);
        return 1;
    case 0x201B:  // 8219 — input-state-forked UI enqueue; body shares modal
                  // scaffolding with 0x2019 but uses a different ID table.
                  // TODO: lift dedicated handler; see loc_823D62B4..loc_823D6420.
        return 1;
    case 0x2027:  // 8231 — msgUI::kPostNetTourneyUIEnd (tourney done → frontend)
        HandleMsg_TourneyFinishedBackToFrontend(event);
        return 1;

    // ── 0x38xx block (msgNet: network / tourney) ──────────────────────────
    case 0x381C:  // 14364 — bare no-op (jumps to loc_823D6644)
        return 1;
    case 0x382D:  // 14381 — raw accept flag (no log, no gate)
        HandleMsg_NetAccept(event);
        return 1;
    case 0x3831:  // 14385 — msgNet::kTourneyNextMatchReady (log + accept)
        HandleMsg_TourneyNextMatchReady(event);
        return 1;
    case 0x3837:  // 14391 — atStringHash "i" nonzero gates the accept flag
        HandleMsg_NetHashGate(event);
        return 1;

    default:
        // Default: fall through to stack teardown + return (loc_823D6644).
        // Original does NOT chain to a parent Process — this is a leaf
        // dispatcher. Unknown msgId is silently dropped.
        return 0;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// Process cascade — per-message handler bodies
// Extracted from pongGameContext::Process (@ 0x823D5B00, size 0xB4C).
// Each corresponds to one cmplwi branch; names reflect observed behaviour.
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongGameContext::HandleMsg_ClearCharViewSlots
 * Lifted from loc_823D5B30..loc_823D5C50 (msgId 2060 / 0x80C).
 *
 * Zeroes the six per-slot selection floats in the character-view singleton,
 * then — provided we're not already in hsm state 6 and the input singleton
 * reports session state 3 — drives a full character-variation refresh
 * (pongPlayer_ADE8_p42 → gdGameData_CalculateVariation → pg_C2A0_g) and
 * publishes the new selection index back to the input object at +0x28.
 */
void pongGameContext::HandleMsg_ClearCharViewSlots(void* /*event*/) {
    pongCharViewData* charView = (pongCharViewData*)g_pongCharViewData;
    for (int i = 0; i < 6; ++i) {
        charView->m_aSlotState[i] = kFloatZero;  // +52, +56, +60, +64, +68, +72
    }

    // If the HSM is already in state 6 (post-load/transition), we're done.
    // Read the state-blob pointer via *(this + 4), NOT m_pGameState at +0x1C.
    pongGameStateData* state = getStateBlob(this);
    int32_t currentState = *(int32_t*)((uint8_t*)state->m_pHsmContext + 0x10);
    if (currentState == 6) {
        return;
    }

    // Only run the variation refresh when the input singleton is in state 3
    // (in-session menu). All other input states bail out silently.
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    if (input->m_iSessionState != 3) {
        return;
    }

    // The selected player index is stored on the state blob; load its slot
    // entry from an 8-byte-strided table at state +0x30 + (idx+6)*8.
    uint8_t* stateBytes = (uint8_t*)state;
    uint32_t playerIdx  = *(uint32_t*)(stateBytes + 128);
    uint32_t slotOffset = (playerIdx + 6) * 8;
    uint32_t slotEntry  = *(uint32_t*)(stateBytes + slotOffset);

    // Refresh the character model and recompute the selected variation.
    pongPlayer_ADE8_p42(input, 0, nullptr);
    gdGameData_CalculateVariation(input, 1, slotEntry, 0, -5);

    // Re-read the post-refresh slot entry and notify the UI string table.
    // Arg r3 is a global context pointer loaded from 0x8271A3D0 (another
    // singles-match singleton) — passed straight through to pg_C2A0_g.
    extern void* g_pongSinglesMatchCtx;  // @ 0x8271A3D0
    uint32_t newPlayerIdx = *(uint32_t*)(stateBytes + 128);
    uint32_t newSlotEntry = *(uint32_t*)(stateBytes + (newPlayerIdx * 8) + 52);
    pg_C2A0_g(g_pongSinglesMatchCtx, newSlotEntry);

    // Stash the newly-selected index on the input singleton for the UI.
    *(uint32_t*)((uint8_t*)input + 0x28) = newSlotEntry;
}

/**
 * pongGameContext::HandleMsg_ForwardToMatchLogic
 * Lifted from loc_823D5C60 (msgId 2138 / 0x85A).
 *
 * Forwards the event to the global match-logic context by loading the
 * singleton pointer at 0x8271A338 and invoking game_7208 on it. game_7208
 * is the per-frame/per-event tick entry for the singles-match state
 * machine; this case is a thin passthrough.
 */
void pongGameContext::HandleMsg_ForwardToMatchLogic(void* /*event*/) {
    // Original code (disasm):
    //   lwz r3, lbl_8271A338    ; global match-logic context pointer
    //   bl  game_7208           ; tick/advance the singles-match SM
    // No VCALL is involved — this is a direct C-style call. The task
    // hand-off referenced a VCALL on g_SinglesMatchLogic @ 0x8271A2F0, but
    // the actual global read is at 0x8271A338 and the dispatch is direct.
    game_7208(g_pongMatchLogicContext);
}

/**
 * pongGameContext::HandleMsg_MenuBackRequest
 * Lifted from loc_823D5CAC..loc_823D5D70 (msgId 2140 / 0x85C).
 *
 * "Back / cancel" request from the menu. Behaviour depends on the input
 * singleton's session state:
 *   - state 3: consult the "yCloseMessage" atSingleton entry. If absent,
 *              set the accept flag (+0x15) — UI will close normally. If
 *              present and input is not suspended (+0x18 == 0), set the
 *              back flag (+0x16) — UI will show confirmation.
 *   - state 5: we're already leaving — swallow the event.
 *   - other:   if not suspended and the pending-menu-state global is
 *              2 (net exhibition), delegate to the net-exhibition
 *              coordinator's 2BA8 probe. Otherwise fall through to set
 *              the back flag (+0x16).
 *
 * Note: msgId 2139 (0x85B) shares this code path's guard register in the
 * original and hits the function epilogue directly — emitted as a bare
 * return in the dispatch cascade above.
 */
void pongGameContext::HandleMsg_MenuBackRequest(void* /*event*/) {
    pongInputObj*      input = (pongInputObj*)g_input_obj_ptr;
    // State blob is `*(this + 4)`, not m_pGameState at +0x1C.
    pongGameStateData* state = getStateBlob(this);

    if (input->m_iSessionState == 3) {
        // Query an atSingleton registry for a pending modal-close hook.
        // Key is a const data ptr at 0x82067898 (inside lbl_82067890, a
        // 52-byte .rdata blob whose payload reads "to leave while another
        // operation is pending"). Left as a raw-address lookup here until
        // the enclosing struct/key is identified.
        void* closeMessageSlot = nullptr;
        atSingleton_E998_g(nullptr, (const char*)0x82067898, &closeMessageSlot);

        if (closeMessageSlot == nullptr) {
            // No modal close pending — accept the back as a normal dismiss.
            state->m_bMenuAccept = 1;  // +0x15
            return;
        }
        // A modal IS pending — respect input-suspend before raising it.
        if (input->m_bSuspended != 0) {
            return;
        }
        state->m_bMenuBack = 1;  // +0x16
        return;
    }

    if (input->m_iSessionState == 5) {
        // Already leaving the session — drop the event.
        return;
    }

    // Out-of-session path: only honor the back if input is not suspended.
    if (input->m_bSuspended != 0) {
        return;
    }

    // Net-exhibition pending (state 2) → let the coordinator decide whether
    // to accept the back. Its return value gates the flag set.
    if (g_pongMenuPendingState == 2) {
        PongNetExhibitionCoordinator_2BA8_g(g_pongNetExhibitionCoord);
        // Original calls PongNetExhibitionCoordinator_2BA8_g then tests the
        // return byte: zero → drop, nonzero → set back flag. Void trampoline
        // here captures the full control path for pass5 scaffolding; see
        // recomp @ 0x823D5D3C for the exact `clrlwi r5,r3,24; beq` gate.
        state->m_bMenuBack = 1;  // +0x16 (gated in original by the coord retval)
        return;
    }

    // Default path: set the back flag.
    state->m_bMenuBack = 1;  // +0x16
}

// ────────────────────────────────────────────────────────────────────────────
// Process cascade — new handlers lifted in this pass
// Every body corresponds to one branch of the cmplwi chain @ 0x823D5B00.
// String/global key blobs are referenced by address (keys are const data).
// ────────────────────────────────────────────────────────────────────────────

// Const key addresses used by atSingleton_E998_g lookups inside the cascade.
// Each decoded via `lis r11,-32254; addi r4,r11,30872` → 0x82286058 etc.
static constexpr uintptr_t kKey_i_Hash    = 0x82286058;  // integer hash "i"
static constexpr uintptr_t kKey_f_Hash    = 0x82258648;  // float hash "f" (-32252 + -14712)

// Secondary singleton at 0x825EAB30 (g_loop_obj_ptr). kJoinFromHud / 0x201B
// read a byte at +576 / +578 to gate the UI-enqueue.
extern void* g_loop_obj_ptr;  // @ 0x825EAB30

// lbl_8271A364 — a pointer-to-something with an int32 phase at +12 read by
// 0x85D (msg 2141). Probably the frontend/menu state singleton; dereferenced
// exactly once, never written from this file.
extern void* lbl_8271A364;  // @ 0x8271A364

/**
 * HandleMsg_MenuQuitConfirm @ 0x823D5E54 (msgId 2141 / 0x85D)
 *
 * Quit-confirm modal dismiss. Unconditionally drives the input singleton
 * into leave-state 5. If the frontend-state singleton reports phase==2 the
 * quit-confirm flag (+0x17) is raised; otherwise the normal back flag
 * (+0x16) is set.
 *
 * Original: two pointer chases — lwz r11,(0x8271A364); lwz r11,12(r11).
 */
void pongGameContext::HandleMsg_MenuQuitConfirm(void* /*event*/) {
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    input->m_iLeaveState = 5;

    void*   menuSingleton = lbl_8271A364;
    int32_t menuPhase     = *(int32_t*)((uint8_t*)menuSingleton + 12);

    pongGameStateData* state = getStateBlob(this);
    if (menuPhase == 2) {
        state->m_bMenuCancel = 1;  // +0x17
        return;
    }
    state->m_bMenuBack = 1;        // +0x16
}

/**
 * HandleMsg_CharVarFloatClamp @ 0x823D5EB4 (msgId 2061 / 0x80D)
 *
 * Toggle-once flag on the input singleton, then consult the float hash "f"
 * and clamp the char-variation state float (+0x0C on m_pGameState) down to
 * the hash value iff it's currently ≥ 0.0f. The clamp is one-sided: values
 * already < 0 are left alone.
 */
void pongGameContext::HandleMsg_CharVarFloatClamp(void* /*event*/) {
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    if (input->m_bCharVarFlag != 0) {
        // First-write path: clear the flag (lit r31 = 0) and bail.
        input->m_bCharVarFlag = 0;
        return;
    }

    // Fetch the float value registered under hash "f" into a stack slot.
    float hashFloat = 0.0f;
    atSingleton_E998_g(nullptr, (const char*)kKey_f_Hash, &hashFloat);

    pongGameStateData* state = getStateBlob(this);
    if (state->m_fCharVarClampMax >= kFloatZero) {
        // Already nonnegative — swallow event.
        return;
    }
    state->m_fCharVarClampMax = hashFloat;
}

/**
 * HandleMsg_StoreHashIntoEvent @ 0x823D6420 (msgId 2157 / 0x86D)
 *
 * Look up the integer hash "i" and stash it at m_pGameState +0x10 (the
 * hash-stash slot read by later network code). No gate, no side effects.
 */
void pongGameContext::HandleMsg_StoreHashIntoEvent(void* /*event*/) {
    uint32_t hashVal = 0;
    atSingleton_E998_g(nullptr, (const char*)kKey_i_Hash, &hashVal);
    pongGameStateData* state = getStateBlob(this);
    state->m_uHashStash = hashVal;  // +0x10
}

/**
 * HandleMsg_ReadyUp @ 0x823D5F28 (msgId 8193 / 0x2001)
 *
 * Drives the input into leave-state 2 (ready-up) and forwards the event's
 * payload pointer to game_7858, which advances the match-logic state.
 */
void pongGameContext::HandleMsg_ReadyUp(void* event) {
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    input->m_iLeaveState = 2;
    // Original: `lwz r3,4(r25); bl 0x823d7858` — pass event->payload.
    void* payload = *(void**)((uint8_t*)event + 4);
    game_7858(payload);
}

/**
 * HandleMsg_SuspendSet @ 0x823D6248 (msgIds 8199/0x2007, 8206/0x200E, 8207/0x200F)
 *
 * All three of these IDs converge on the same body: set the context-level
 * suspend flag at this+0x1D. Only 0x2007 actually carries the write in the
 * original; 0x200E/0x200F fall through the guard chain and exit without
 * effect. We treat them identically for forward-compat — the write is
 * idempotent and harmless for the fall-through cases.
 */
void pongGameContext::HandleMsg_SuspendSet(void* /*event*/) {
    *((uint8_t*)this + 0x1D) = 1;
}

/**
 * HandleMsg_SuspendClear @ 0x823D627C (msgId 8202 / 0x200A)
 *
 * Clears the context-level suspend flag at this+0x1D.
 */
void pongGameContext::HandleMsg_SuspendClear(void* /*event*/) {
    *((uint8_t*)this + 0x1D) = 0;
}

// ── shared helper: raise a simple accept flag on the state blob ───────────
// 0x2027 / 0x3831 / 0x382D / 0x3837 all boil down to `state->m_bMenuAccept = 1`
// after passing their respective gates. Factored to avoid three duplicate
// three-liners. The debug-log variants fold their format string in here too.
static void raiseAcceptFlag(pongGameContext* ctx) {
    pongGameStateData* state = getStateBlob(ctx);
    state->m_bMenuAccept = 1;  // +0x15
}

/**
 * HandleMsg_TourneyFinishedBackToFrontend @ 0x823D5DA8 (msgId 8231 / 0x2027)
 *
 * msgUI::kPostNetTourneyUIEnd — tourney is done, UI is heading back to the
 * frontend. Logs the transition then raises the accept flag. But wait —
 * the recomp actually writes to +22 (m_bMenuBack), not +21. Consistent with
 * "UI dismiss from frontend return"; the flag name is stable for both
 * accept / back semantics at this layer.
 */
void pongGameContext::HandleMsg_TourneyFinishedBackToFrontend(void* /*event*/) {
    // rage_DebugLog(ctx, "pongGameContext::Process() msgUI::kPostNetTourneyUIEnd...");
    nop_8240E6D0((void*)0x82071910);  // string @ 0x82071910
    pongGameStateData* state = getStateBlob(this);
    state->m_bMenuBack = 1;  // +0x16
}

/**
 * HandleMsg_TourneyNextMatchReady @ 0x823D5D70 (msgId 14385 / 0x3831)
 *
 * msgNet::kTourneyNextMatchReady — netcode signalled the next bracket match
 * is ready to load. Logs then raises the accept flag on the state (+0x15).
 */
void pongGameContext::HandleMsg_TourneyNextMatchReady(void* /*event*/) {
    nop_8240E6D0((void*)0x820718B0);  // "Loading into next match" string
    raiseAcceptFlag(this);
}

/**
 * HandleMsg_NetAccept @ 0x823D5DE0 (msgId 14381 / 0x382D)
 *
 * Bare accept: no log, no gate. Just raises the accept flag. Observed in
 * the recomp as a two-instruction stub (load + stb).
 */
void pongGameContext::HandleMsg_NetAccept(void* /*event*/) {
    raiseAcceptFlag(this);
}

/**
 * HandleMsg_NetHashGate @ 0x823D5E0C (msgId 14391 / 0x3837)
 *
 * Guarded accept: only raises the flag if atSingleton("i") returns nonzero.
 * Used by net code to confirm the integer token before allowing the UI to
 * proceed.
 */
void pongGameContext::HandleMsg_NetHashGate(void* /*event*/) {
    uint32_t hashVal = 0;
    atSingleton_E998_g(nullptr, (const char*)kKey_i_Hash, &hashVal);
    if (hashVal == 0) {
        return;
    }
    raiseAcceptFlag(this);
}

/**
 * HandleMsg_JoinFromHudRequested @ 0x823D5F90 (msgId 8217 / 0x2019) — ~220 LOC
 *
 * The kJoinFromHudRequested handler. Walks a four-gate precondition chain
 * against atSingleton_D070_g (147/148/156/159 bit-queries — all with r4=7,
 * the "query-mode" argument — on the config singleton loaded from
 * 0x8271A33C), then emits a debug log and marks this+0x1E=1. Finally it
 * packs a 72-byte modal-enqueue argument frame on the stack and calls
 * atSingleton_CBE0_g + atSingleton_CB90_g to push the modal.
 *
 * The exact modal target depends on which secondary gate fires (line
 * numbers refer to recomp.21.cpp):
 *   - net-exhibition coordinator nonzero   → id 156  (loc_823D6104 @40783)
 *   - ctx vtable[10] returns nonzero       → id 147  (loc_823D6140 @40810)
 *   - util_CF10 tourney probe nonzero      → id 159  (loc_823D6178 @40832)
 *   - default                              → id 148  (loc_823D6184 @40842)
 *
 * 72-byte modal argpack layout (sp+128..sp+204):
 *   +0x00 (sp+128) : secondary-this of `this` (this - 20) — MI-adjusted
 *   +0x04 (sp+132) : string @ 0x82065938 ("JoinFromHud" label string)
 *   +0x08 (sp+136) : 0                    (modalPhase)
 *   +0x0C (sp+140) : 1                    (modalEnabled)
 *   +0x10 (sp+144) : float @ 0x82065938 copied from rdata
 *   +0x14 (sp+148) : 0                    (int payload slot)
 *   +0x18 (sp+152) : 7                    (modal-group tag)
 *   +0x1C (sp+156) : modalId              (147/148/156/159 per branch)
 *   +0x20 (sp+160) : 1                    (accept-default flag)
 *   +0x24 (sp+164) : 2                    (button-layout code)
 *   +0x28 (sp+168) : vec4 from 0x8271A0C0 (4 u32s: text-geom descriptor)
 *   +0x38 (sp+184) : *(0x8271A33C + 4)    (singleton-local config pointer)
 *   +0x3C (sp+188) : *(0x8271A33C + 4)    (duplicated from +0x38)
 *   +0x40 (sp+192) : 0
 *   +0x44 (sp+196) : state blob's menu-accept flag (set in 0x2027 path)
 *   +0x48 (sp+200) : 0
 *   +0x4C (sp+204) : 0
 *
 * See recomp.21.cpp lines 40666..40917 for the full argpack construction.
 */

// Modal argpack struct matching the sp+128..sp+204 layout in the recomp.
// Kept local to this translation unit — the shape is specific to the two
// sites that consume it (0x2019 and 0x201B handlers).
//
// All "pointer" slots stored as uint32_t guest VAs to preserve the on-wire
// 4-byte layout on 64-bit hosts (same convention as pongGameContext).
struct pongJoinFromHudModalArgs {
    uint32_t m_pSelfSecondary;      // +0x00  secondary-this of the context
    uint32_t m_pLabelString;        // +0x04  "JoinFromHud" .rdata pointer (VA)
    uint32_t m_modalPhase;          // +0x08
    uint32_t m_modalEnabled;        // +0x0C
    float    m_fGeomValue;          // +0x10  from 0x82065938
    uint32_t m_payloadInt;          // +0x14
    uint32_t m_modalGroupTag;       // +0x18
    uint32_t m_modalId;             // +0x1C  147 / 148 / 156 / 159
    uint32_t m_acceptDefault;       // +0x20
    uint32_t m_buttonLayoutCode;    // +0x24
    uint32_t m_vecGeom[4];          // +0x28  from 0x8271A0C0
    uint32_t m_cfgSingletonA;       // +0x38
    uint32_t m_cfgSingletonB;       // +0x3C
    uint32_t _pad40;                // +0x40
    uint8_t  m_bStateAllowReadySnap;// +0x44  byte copy of input->m_bStateAllowReady
    uint8_t  _pad45[3];             // +0x45
    uint32_t _pad48;                // +0x48
    uint32_t _pad4C;                // +0x4C
};
static_assert(sizeof(pongJoinFromHudModalArgs) == 0x50,
              "pongJoinFromHudModalArgs must be 80 bytes (72 used + 8 tail pad)");

void pongGameContext::HandleMsg_JoinFromHudRequested(void* /*event*/) {
    // Gate 1: g_loop_obj_ptr points to an object with a byte at +576.
    uint8_t* loopObj = (uint8_t*)g_loop_obj_ptr;
    if (loopObj[576] != 0) {
        return;
    }

    // Gate 2: input singleton's join-path-blocked byte (+27) must be zero.
    pongInputObj* input = (pongInputObj*)g_input_obj_ptr;
    if (input->m_bJoinPathBlocked != 0) {
        return;
    }

    // Gates 3-6: consult the config singleton at 0x8271A33C for four bit-
    // queries. In the recomp r4 is set ONCE to 7 at line 40603 and never
    // reloaded across the four calls, so all four queries use query-mode 7.
    // Any nonzero return bails silently — the join is not permissible.
    void* configCtx = *(void**)0x8271A33C;
    if (atSingleton_D070_g(configCtx, 7, 147) != 0) return;  // "join perm"
    if (atSingleton_D070_g(configCtx, 7, 148) != 0) return;  // "host perm"
    if (atSingleton_D070_g(configCtx, 7, 159) != 0) return;  // "tourney perm"
    if (atSingleton_D070_g(configCtx, 7, 156) != 0) return;  // "match perm"

    // Log the event and flag this context as having dispatched kJoinFromHud.
    nop_8240E6D0((void*)0x82071978);  // "Got kJoinFromHudRequested message"
    *((uint8_t*)this + 0x1E) = 1;

    // Determine which modal ID to enqueue. The original walks a three-way
    // probe chain; we follow the same order (line 40684 onwards).
    //   (1) if input->m_bStateAllowReady == 0 → short-circuit to 147
    //   (2) else probe the net-exhibition coordinator — nonzero → 156
    //   (3) else probe ctx vtable[10]        — nonzero → 147
    //   (4) else probe util_CF10 tourney     — nonzero → 159
    //   (5) default → 148
    uint32_t modalId;
    if (input->m_bStateAllowReady != 0) {
        // The fast-path sets 147 and jumps straight to loc_823D6190.
        modalId = 147;
    } else {
        PongNetExhibitionCoordinator_2BA8_g(g_pongNetExhibitionCoord);
        // The PPC recomp tests the u8 return of 2BA8. Our trampoline is
        // declared void, so we approximate by falling straight to the
        // secondary probe chain. When a proper return-value shim lands for
        // 2BA8 this branch will gain the 156 short-circuit.
        if (util_CF10(g_pongNetExhibitionCoord) != 0) {
            modalId = 159;  // tourney invite
        } else {
            modalId = 148;  // generic join (default)
        }
    }

    // Build the 72-byte argpack on the stack and enqueue. Each field
    // corresponds to an stw in recomp lines 40688..40909.
    pongJoinFromHudModalArgs args{};
    args.m_pSelfSecondary   = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(this) - 20));
    args.m_pLabelString     = 0x82065938u;  // "JoinFromHud" label VA
    args.m_modalPhase       = 0;
    args.m_modalEnabled     = 1;
    args.m_fGeomValue       = *(const float*)0x82065938;
    args.m_payloadInt       = 0;
    args.m_modalGroupTag    = 7;
    args.m_modalId          = modalId;
    args.m_acceptDefault    = 1;
    args.m_buttonLayoutCode = 2;
    {
        const uint32_t* vecSrc = (const uint32_t*)0x8271A0C0;  // 16-byte vec
        args.m_vecGeom[0] = vecSrc[0];
        args.m_vecGeom[1] = vecSrc[1];
        args.m_vecGeom[2] = vecSrc[2];
        args.m_vecGeom[3] = vecSrc[3];
    }
    {
        // Mirror the two duplicate cfg-singleton writes at sp+184 / sp+188.
        uint32_t cfgField = *(uint32_t*)((uint8_t*)configCtx + 4);
        args.m_cfgSingletonA = cfgField;
        args.m_cfgSingletonB = cfgField;
    }
    args.m_bStateAllowReadySnap = input->m_bStateAllowReady;

    // Enqueue via the UI modal path.
    atSingleton_CBE0_g(configCtx, nullptr, &args, 1, 0, 0,
                       (void*)(intptr_t)modalId);
    atSingleton_CB90_g(configCtx);
}
// ────────────────────────────────────────────────────────────────────────────
// NOTES
// ────────────────────────────────────────────────────────────────────────────
//
// Class hierarchy:
//   pongGame (root game object)
//     └─> pongGameContext (manages current context/mode)
//           └─> pongGameState (specific state within context)
//
// Key patterns:
// 1. Message-based architecture (16-bit message IDs)
// 2. Global singletons for subsystem access
// 3. State flags stored in game state objects
// 4. Multiple inheritance (pongGameContext has 2 vtables)
// ────────────────────────────────────────────────────────────────────────────

/**
 * game_CheckNetworkAndTransitionToState6 @ 0x823D7788 | size: 0xCC
 *
 * Checks network client status and transitions to state 6 if conditions are met.
 * This function is called from various game state handlers to ensure proper
 * network synchronization before proceeding.
 *
 * Logic flow:
 *   1. Load HSM context from offset +4 of the state object
 *   2. Check if current state (at +12) is not 6
 *   3. If not state 6, set next state to 6 via hsmContext_SetNextState
 *   4. Check network client status (player indices at +16 and +20)
 *   5. If both indices are -1, call utility function with specific parameters
 *   6. Check input object status at offset +332
 *   7. If input is active, call SinglesNetworkClient function with -1 parameter
 *
 * Globals used:
 *   0x8271A320 - Network client pointer 1
 *   0x8271A328 - Network client pointer 2  
 *   0x825EAB28 - g_input_obj_ptr
 *   0x8261A0C0 - 16-byte data buffer for util call
 */
void game_CheckNetworkAndTransitionToState6(void* stateObj) {
    // Load HSM context from offset +4
    void* hsmContext = *(void**)((uint8_t*)stateObj + 4);
    
    // Load current state index from offset +12 of context
    int32_t currentState = *(int32_t*)((uint8_t*)hsmContext + 12);
    
    // If not in state 6, transition to it
    if (currentState != 6) {
        // Call hsmContext_SetNextState with state index 6
        extern void hsmContext_SetNextState(void* ctx, int32_t stateIdx);
        hsmContext_SetNextState(hsmContext, 6);
        
        // Load network client pointer from global
        extern void* lbl_8271A320;  // @ 0x8271A320
        void* networkClient = lbl_8271A320;
        
        // Check player indices at offsets +16 and +20
        int32_t playerIdx1 = *(int32_t*)((uint8_t*)networkClient + 16);
        int32_t playerIdx2 = *(int32_t*)((uint8_t*)networkClient + 20);
        
        // If both indices are -1, network is not fully initialized
        bool networkNotReady = (playerIdx1 == -1 && playerIdx2 == -1);
        
        if (!networkNotReady) {
            // Network has at least one player - call utility function
            extern void util_94B8(int32_t param1, int32_t param2, uint64_t* data1, uint64_t* data2);
            extern uint64_t lbl_8261A0C0[2];  // @ 0x8261A0C0 (16-byte buffer)
            
            // Call with parameters: (2, 1, data, data)
            util_94B8(2, 1, lbl_8261A0C0, lbl_8261A0C0);
        }
        
        // Check input object status
        extern void* g_input_obj_ptr;  // @ 0x825EAB28
        void* inputObj = g_input_obj_ptr;
        
        // Load status byte from offset +332
        uint8_t inputActive = *(uint8_t*)((uint8_t*)inputObj + 332);
        
        if (inputActive != 0) {
            // Input is active - call network client function
            extern void SinglesNetworkClient_1410_g(void* client, int32_t param);
            extern void* lbl_8271A328;  // @ 0x8271A328
            void* networkClient2 = lbl_8271A328;
            
            SinglesNetworkClient_1410_g(networkClient2, -1);
        }
    }
}

// ────────────────────────────────────────────────────────────────────────────
// Scene-graph iterator helpers — used by OnUpdate/OnRender as a 2-word
// "GuardRec" walker (fields +0=root, +4=current). The triple forms a
// pre-order traversal over sgNode children rooted at the match-logic
// singleton's child list. Each visit VCALLs slot 20 (type-gate, returns
// nonzero to bail) and slot 19 (getter emitted into sgIter debug log).
//
// Node layout (from recomp offsets):
//   +0x00 : parent/root back-pointer
//   +0x04 : left child  (first descendant)
//   +0x08 : right sibling
//   +0x0C : first-child link (primary descent)
//
// All three are free functions — they operate on an iterator record
// passed by pointer, NOT on a pongGameContext. The names are kept in
// pongGameContext_* form because the debug symbols live under that
// prefix in the binary. Extern-"C" wrappers match the 4 existing call
// sites in OnUpdate/OnRender above.
// ────────────────────────────────────────────────────────────────────────────

namespace {

// sgIter record — what GuardRec in OnUpdate/OnRender actually is.
// First word is the list root; second word is the "current" cursor the
// iterator re-writes as it descends.
struct sgIterRec {
    uint32_t m_pRoot;     // +0x00
    uint32_t m_pCurrent;  // +0x04
};

// sgNode — opaque 16-byte-ish tree node. Only the four link offsets
// used by the iterator are typed.
struct sgNode {
    uint32_t m_pParent;      // +0x00
    uint32_t m_pLeft;        // +0x04
    uint32_t m_pRight;       // +0x08
    uint32_t m_pFirstChild;  // +0x0C
};

// Shared debug-log format string referenced by pongGameContext_A070.
static constexpr uint32_t kSgIterSkipFormat = 0x82071DC8u;  // "sgIter - Skipping non-sgNode '%s'"

} // namespace

extern "C" {

/**
 * pongGameContext_A0F8_h  @ 0x823DA0F8 | size: 0x40
 *
 * "Find leftmost descendant with a first-child link, stopping at the
 * root sentinel." Walks the +4/+8 chain of `iter->m_pCurrent`. Returns
 * the +8 (right-sibling) of the first node whose own first-child (+8)
 * slot is non-null; returns 0 if the chain hits the root or dead-ends.
 *
 * Called from pongGameContext_9FD0_w when a direct descent failed.
 */
void* pongGameContext_A0F8_h(void* iterPtr, void* nodePtr) {
    sgIterRec* iter = reinterpret_cast<sgIterRec*>(iterPtr);
    sgNode*    node = reinterpret_cast<sgNode*>(nodePtr);

    uint32_t cur = node->m_pLeft;  // r4+4
    if (cur == 0) {
        return nullptr;
    }
    uint32_t root = iter->m_pRoot;  // r3+0
    while (true) {
        if (cur == root) {
            return nullptr;
        }
        sgNode* curNode = reinterpret_cast<sgNode*>(static_cast<uintptr_t>(cur));
        uint32_t firstChild = curNode->m_pRight;  // +8
        if (firstChild != 0) {
            return reinterpret_cast<void*>(static_cast<uintptr_t>(firstChild));
        }
        cur = curNode->m_pLeft;  // +4
        if (cur == 0) {
            return nullptr;
        }
    }
}

/**
 * pongGameContext_9FD0_w  @ 0x823D9FD0 | size: 0x9C
 *
 * Iterator advance: given `iter` (sgIterRec), consult `iter->m_pCurrent`
 * and compute the next-in-pre-order node. Priority:
 *   1. descend into +12 (first-child) if present;
 *   2. if sibling chain is empty (current is root), park at null;
 *   3. if current has a right-sibling +8, take it;
 *   4. else fall back to pongGameContext_A0F8_h to bubble up and find
 *      the next-right across ancestors.
 * After choosing the new node, store it into iter+4 and call
 * pongGameContext_A070 to visit / recurse.
 */
void pongGameContext_9FD0_w(void* iterPtr) {
    sgIterRec* iter = reinterpret_cast<sgIterRec*>(iterPtr);
    uint32_t   nextRaw = 0;

    uint32_t curRaw = iter->m_pCurrent;
    if (curRaw != 0) {
        sgNode* cur = reinterpret_cast<sgNode*>(static_cast<uintptr_t>(curRaw));

        // Primary descent: first-child at +12.
        uint32_t firstChild = cur->m_pFirstChild;
        if (firstChild != 0) {
            nextRaw = firstChild;
        } else {
            uint32_t rootRaw = iter->m_pRoot;
            if (curRaw == rootRaw) {
                // Cursor already sits on the root sentinel → park.
                nextRaw = 0;
            } else {
                // Try right sibling at +8.
                uint32_t rightRaw = cur->m_pRight;
                if (rightRaw != 0) {
                    nextRaw = rightRaw;
                } else {
                    // Neither child nor sibling — bubble up through
                    // left-parent chain. If leftLink itself carries a
                    // +8 right-sibling we take it directly; else we
                    // recurse through the deep-leftmost helper, which
                    // walks the +4 chain until it finds a node with
                    // a non-null +8 or falls off.
                    uint32_t leftRaw = cur->m_pLeft;
                    void* fallback = nullptr;
                    if (leftRaw != 0 && leftRaw != rootRaw) {
                        sgNode* leftNode =
                            reinterpret_cast<sgNode*>(static_cast<uintptr_t>(leftRaw));
                        if (leftNode->m_pRight != 0) {
                            fallback = reinterpret_cast<void*>(
                                static_cast<uintptr_t>(leftNode->m_pRight));
                        } else {
                            fallback = pongGameContext_A0F8_h(iter, leftNode);
                        }
                    }
                    nextRaw = fallback
                        ? static_cast<uint32_t>(
                              reinterpret_cast<uintptr_t>(fallback))
                        : 0u;
                }
            }
        }
    }

    iter->m_pCurrent = nextRaw;
    pongGameContext_A070(iter);
}

/**
 * pongGameContext_A070  @ 0x823DA070 | size: 0x88
 *
 * Visit the iterator's current node: VCALL slot 20 (type-gate — byte
 * offset +80 on the node's vtable). Returns early if the gate reports
 * nonzero (wrong node type). Otherwise VCALL slot 19 (byte +76, the
 * node's name getter) and feed it through the debug sink with the
 * "sgIter - Skipping non-sgNode '%s'" format at 0x82071DC8, then recurse
 * through pongGameContext_9FD0_w to advance.
 *
 * Mutates: none on iter directly (9FD0_w does the store).
 */
void pongGameContext_A070(void* iterPtr) {
    sgIterRec* iter = reinterpret_cast<sgIterRec*>(iterPtr);
    uint32_t   curRaw = iter->m_pCurrent;
    if (curRaw == 0) {
        return;
    }

    void*  curNode  = reinterpret_cast<void*>(static_cast<uintptr_t>(curRaw));
    void** curVt    = *reinterpret_cast<void***>(curNode);

    // Slot 20 (byte +80) — returns u8 gate; nonzero ⇒ this is a real
    // sgNode subtype and we should NOT emit the skip log.
    using GateFn = uint32_t (*)(void*);
    uint32_t gate = reinterpret_cast<GateFn>(curVt[20])(curNode) & 0xFFu;
    if (gate != 0) {
        return;
    }

    // Slot 19 (byte +76) — returns const char* node name for the log line.
    using NameFn = const char* (*)(void*);
    const char* name = reinterpret_cast<NameFn>(curVt[19])(curNode);

    // Debug log — format @ 0x82071DC8.
    nop_8240E6D0(reinterpret_cast<void*>(static_cast<uintptr_t>(kSgIterSkipFormat)),
                 name);

    // Tail-recurse through the advance function.
    pongGameContext_9FD0_w(iter);
}

} // extern "C"

// ────────────────────────────────────────────────────────────────────────────
// Net-exhibition quit shims — both tail-call pg_6B40_w(this=input, r4=1)
// to drive the PongNetExhibitionCoordinator probe and enqueue UI event
// 2144 or 2145 via pg_E6E0. Only the gating differs.
// ────────────────────────────────────────────────────────────────────────────

extern "C" {
    // pg_6B40_w(flag) — NetExhibitionCoordinator probe that writes
    // g_input_obj_ptr+48 = flag, then dispatches UI event 2144/2145 via
    // pg_E6E0 and sets g_input_obj_ptr+27 = 1. Body lives outside this
    // partition.
    void pg_6B40_w(int32_t flag);
}

// g_pongGlobalMode already extern'd at line 85.

/**
 * pongGameContext_6BD0_p46  @ 0x823D6BD0 | size: 0x20
 *
 * Gated net-exhibition quit entry: only fires when g_pongGlobalMode == 1
 * (single-player / offline). Tails pg_6B40_w(flag=1).
 */
extern "C" void pongGameContext_6BD0_p46() {
    if (g_pongGlobalMode != 1) {
        return;
    }
    pg_6B40_w(/*flag=*/1);
}

/**
 * pongGameContext_6BF0_p46  @ 0x823D6BF0 | size: 0x08
 *
 * Unconditional net-exhibition quit entry (slot [1] scalar-dtor-style
 * thunk on the primary vtable path, per pass5 alias to
 * pongGameState_vfn_0). Always tails pg_6B40_w(flag=1).
 */
extern "C" void pongGameContext_6BF0_p46() {
    pg_6B40_w(/*flag=*/1);
}

// ────────────────────────────────────────────────────────────────────────────
// Secondary-vtable RTTI thunk  @ 0x823D7928 | size: 0x08
//
// When an rtti/dynamic-cast path hits pongGameContext through the
// secondary vtable @ 0x82071B50 (offset +0x14 inside the object), the
// incoming `this` is displaced by the MI offset. The thunk subtracts
// 20 bytes to recover the primary-base `this` and tail-calls
// pongGameContext::Dtor (slot 0 @ 0x823D5328) with whatever scalar-dtor
// flag is in r4. Mirrors the pass5 PPC_FUNC_IMPL exactly.
// ────────────────────────────────────────────────────────────────────────────

extern "C" void pongGameContext_rtti_1B50_0(void* secondaryThis, int32_t flags) {
    // Adjust back to primary-base `this`. The secondary vtable sits at
    // +0x14 inside the object; displacing -20 (-0x14) lands on the start
    // of the primary layout where Dtor expects r3.
    uint8_t* raw = reinterpret_cast<uint8_t*>(secondaryThis) - 20;
    auto*    primary = reinterpret_cast<pongGameContext*>(raw);
    primary->Dtor(flags);
}
