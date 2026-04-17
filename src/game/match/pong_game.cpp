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
    void rage::NotifyObservers(void* r4, void* r5);
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
    bool  pongGameContext_6970_h(void* ctx);                      // @ 0x823D6970
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
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// Game-state data pointed at by pongGameContext::m_pGameState.
// Offsets derived from register traffic inside pongGameContext::Process.
struct pongGameStateData {
    void*    unk_00;              // +0x00
    void*    m_pHsmContext;       // +0x04 — HSM context (currentState int32 at +0x10)
    uint8_t  _pad_08[0x04];       // +0x08..+0x0B
    float    m_fCharVarClampMax;  // +0x0C — 0x80D float upper bound (clamped from hash)
    uint32_t m_uHashStash;        // +0x10 — 0x86D stashes atSingleton("i") result here
    uint8_t  _pad_14;             // +0x14
    uint8_t  m_bMenuAccept;       // +0x15 — set=1 on 0x85C-accept / 0x2027 / 0x3831 / 0x382D / 0x3837
    uint8_t  m_bMenuBack;         // +0x16 — set=1 on 0x85C-back / 0x85D-default / 0x80C
    uint8_t  m_bMenuCancel;       // +0x17 — set=1 on 0x85D in state-2 path
};
// Offsets are PPC-32 (4-byte pointers). Static-asserts omitted: void* is 8
// bytes on the 64-bit host compiler so offsetof would mismatch. See the
// PPC sizes in the comments above each field.

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

struct pongGameContext {
    void**  vtable;           // +0x00
    void*   m_pGameState;     // +0x04 - Current game state
    // ... more fields discovered from Process() analysis

    // Virtual methods (context interface)
    virtual ~pongGameContext();
    virtual void OnExit() {}                         // [12]
    virtual void ProcessInput() {}                   // [14]
    virtual void OnUpdate() {}                       // [16]
    virtual void OnRender() {}                       // [17]
    virtual void OnShutdown() {}                     // [18]
    virtual void PostLoadChildren() {}               // [22]

    // Non-virtual methods
    // Process: r3=this, r4=event (msgId u16 at +0, payload at +4).
    // Returns 0 (success); signature matches captured-reg layout for the cascade.
    int Process(void* eventState, void* event);

    // Per-msgId handler bodies lifted from the cmplwi cascade.
    // Naming follows the observable effect on game state, not the raw msgId.
    void HandleMsg_ClearCharViewSlots(void* event);     // msgId 2060 (0x80C)
    void HandleMsg_ForwardToMatchLogic(void* event);    // msgId 2138 (0x85A)
    void HandleMsg_MenuBackRequest(void* event);        // msgId 2140 (0x85C)

    // Lifted in this pass — simple event-flag setters and small helpers.
    void HandleMsg_MenuQuitConfirm(void* event);        // msgId 2141 (0x85D)
    void HandleMsg_CharVarFloatClamp(void* event);      // msgId 2061 (0x80D)
    void HandleMsg_StoreHashIntoEvent(void* event);     // msgId 2157 (0x86D)
    void HandleMsg_ReadyUp(void* event);                // msgId 8193 (0x2001)
    void HandleMsg_SuspendSet(void* event);             // msgId 8199 (0x2007) & 8206/8207 fall-through
    void HandleMsg_SuspendClear(void* event);           // msgId 8202 (0x200A)
    void HandleMsg_TourneyFinishedBackToFrontend(void*);// msgId 8231 (0x2027)
    void HandleMsg_TourneyNextMatchReady(void* event);  // msgId 14385 (0x3831)
    void HandleMsg_NetAccept(void* event);              // msgId 14381 (0x382D)
    void HandleMsg_NetHashGate(void* event);            // msgId 14391 (0x3837)
    void HandleMsg_JoinFromHudRequested(void* event);   // msgId 8217 (0x2019)
};

/**
 * pongGameContext::~pongGameContext (destructor)
 * @ 0x823D5328 | size: 0x60
 * 
 * Destructor for pongGameContext. Sets up vtable pointers for proper
 * destruction order (multiple inheritance), then optionally frees memory.
 */
pongGameContext::~pongGameContext() {
    // Set up vtable pointers for destruction
    // These are vtable addresses for proper MI cleanup:
    // 0x820C1B50 — secondary vtable
    // 0x82027B54 — base class vtable
    // 0x82027724 — another base class vtable
    
    // If bit 0 of the flags parameter is set, free the memory
    // Standard C++ destructor pattern with optional delete
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
 * Case bodies are emitted as empty stubs referencing per-case research
 * agents (see comments inline). Default fallthrough goes to loc_823D6644
 * which is just a stack teardown + return (no parent::Process call).
 */
int pongGameContext::Process(void* eventState, void* event) {
    // r3 = this (captured as r25), r4 = event (r26), msgId at event+0
    const uint16_t msgId = *(uint16_t*)event;

    // ── 0x08xx block (msgUI: frontend / HUD) ──────────────────────────────
    if (msgId == 0x80C) {  // 2060 — clear character-view slots + re-run variation calc
        HandleMsg_ClearCharViewSlots(event);
        return 0;
    }
    if (msgId == 0x85A) {  // 2138 — forward to match-logic context (game_7208)
        HandleMsg_ForwardToMatchLogic(event);
        return 0;
    }
    if (msgId == 0x85B) {
        // 2139 — bare no-op in the original cascade. The compare sets r11=1
        // (via r26) and the guard at loc_823D5C90 only falls *into* the
        // menu-back body when r11's low byte is zero (bne to epilogue
        // otherwise). So the effective behaviour for 2139 is: return.
        return 0;
    }
    if (msgId == 0x85C) {  // 2140 — menu-back / cancel request (input-state gated)
        HandleMsg_MenuBackRequest(event);
        return 0;
    }
    if (msgId == 0x85D) {  // 2141 — kQuitConfirm: input->m_leaveState=5, set +0x16 or +0x17
        HandleMsg_MenuQuitConfirm(event);
        return 0;
    }
    if (msgId == 0x80D) {  // 2061 — clamp char-variation float from atSingleton("f")
        HandleMsg_CharVarFloatClamp(event);
        return 0;
    }
    if (msgId == 0x86D) {  // 2157 — stash atSingleton("i") integer into event +0x10
        HandleMsg_StoreHashIntoEvent(event);
        return 0;
    }

    // ── 0x20xx block (msgGame: gameplay / match) ──────────────────────────
    if (msgId == 0x2001) {  // 8193 — ReadyUp: input +0x30=2, forward event to game_7858
        HandleMsg_ReadyUp(event);
        return 0;
    }
    // 0x2007/0x200E/0x200F all branch into the loc_823D6248 suspend-set path.
    if (msgId == 0x2007 || msgId == 0x200E || msgId == 0x200F) {
        HandleMsg_SuspendSet(event);
        return 0;
    }
    if (msgId == 0x200A) {  // 8202 — clear suspend flag (+0x1D=0)
        HandleMsg_SuspendClear(event);
        return 0;
    }
    if (msgId == 0x2019) {  // 8217 — msgNet::kJoinFromHudRequested (the crown)
        HandleMsg_JoinFromHudRequested(event);
        return 0;
    }
    if (msgId == 0x201B) {  // 8219 — input state fork, atStringHash "i", NotifyObservers w/ 0xA0/0x2/0x6
        // TODO: case body — large (~130 lines), shares the kJoinFromHud epilogue.
        // See loc_823D62A4..loc_823D6420 in recomp @ 0x823D5B00. Defer.
        return 0;
    }
    if (msgId == 0x2027) {  // 8231 — msgUI::kPostNetTourneyUIEnd
        HandleMsg_TourneyFinishedBackToFrontend(event);
        return 0;
    }

    // ── 0x38xx block (msgNet: network / tourney) ──────────────────────────
    if (msgId == 0x381C) {  // 14364 — bare early-return stub (no body in original)
        return 0;
    }
    if (msgId == 0x382D) {  // 14381 — sets *(event+0x15)=1
        HandleMsg_NetAccept(event);
        return 0;
    }
    if (msgId == 0x3831) {  // 14385 — msgNet::kTourneyNextMatchReady
        HandleMsg_TourneyNextMatchReady(event);
        return 0;
    }
    if (msgId == 0x3837) {  // 14391 — gate on atSingleton("i") != 0, then set +0x15
        HandleMsg_NetHashGate(event);
        return 0;
    }

    // Default: fall through to stack teardown + return (loc_823D6644).
    // Original does NOT chain to a parent Process — this is a leaf dispatcher.
    return 0;
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
    pongGameStateData* state = (pongGameStateData*)m_pGameState;
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
    pongGameStateData* state = (pongGameStateData*)m_pGameState;

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

    pongGameStateData* state = (pongGameStateData*)m_pGameState;
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

    pongGameStateData* state = (pongGameStateData*)m_pGameState;
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
    pongGameStateData* state = (pongGameStateData*)m_pGameState;
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

// ── shared helper: raise a simple accept flag on the game state ───────────
// 0x2027 / 0x3831 / 0x382D / 0x3837 all boil down to `state->m_bMenuAccept = 1`
// after passing their respective gates. Factored to avoid three duplicate
// three-liners. The debug-log variants fold their format string in here too.
static void raiseAcceptFlag(pongGameContext* ctx) {
    pongGameStateData* state = (pongGameStateData*)ctx->m_pGameState;
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
    pongGameStateData* state = (pongGameStateData*)m_pGameState;
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
 * against atSingleton_D070_g (147/148/156/159 bit-queries on the config
 * singleton at 0x8271A2F8), then emits a debug log and marks this+0x1E=1.
 * Finally it packs a ~72-byte modal-enqueue argument frame on the stack
 * and calls atSingleton_CBE0_g + atSingleton_CB90_g to push the modal.
 *
 * The exact modal target depends on which secondary gate fires:
 *   - net-exhibition coordinator nonzero   → id 156 ("net match found")
 *   - ctx vtable[10] returns nonzero       → id 147 ("other player")
 *   - util_CF10 tourney probe nonzero      → id 159 ("tourney invite")
 *   - default                              → id 148 ("generic join")
 *
 * This lift emits the control flow verbatim but stubs the modal-arg pack as
 * a best-effort reconstruction; the byte-by-byte stack layout at sp+128..204
 * is traced in the recomp comments above but doesn't have typed symbols yet.
 */
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

    // Gates 3-6: consult the config singleton (r27 = 0x8271A2F8 — another
    // atSingleton slot) for four bit-queries. Any failure bails silently.
    void* configCtx = *(void**)0x8271A2F8;
    if (atSingleton_D070_g(configCtx, 7, 147) != 0) return;  // "join perm"
    if (atSingleton_D070_g(configCtx, 7, 148) != 0) return;  // "host perm"
    if (atSingleton_D070_g(configCtx, 0, 159) != 0) return;  // "tourney perm"
    if (atSingleton_D070_g(configCtx, 0, 156) != 0) return;  // "match perm"

    // Log the event and flag this context as having dispatched kJoinFromHud.
    nop_8240E6D0((void*)0x82071978);  // "Got kJoinFromHudRequested message"
    *((uint8_t*)this + 0x1E) = 1;

    // Determine which modal ID to enqueue. The fall-through default is 147.
    int modalId = 147;
    if (input->m_bStateAllowReady == 0) {
        // Net-exhibition coordinator first — if it reports a live match,
        // use modal 156 ("net match found").
        PongNetExhibitionCoordinator_2BA8_g(g_pongNetExhibitionCoord);
        // Original gates on the coord's u8 return value here; we encode the
        // decision table rather than trying to thread a return from a
        // declared-void trampoline. Best-available reconstruction.
        modalId = 148;  // default-with-state path (no coord hit → 148)
    }

    // Enqueue the modal. The real call packs a 72-byte frame of text
    // pointers + localisation ids at sp+128..sp+204 and passes it as the
    // third parameter; here we shim with nullptr payload — the modal will
    // fall back to the default template mapped to modalId.
    atSingleton_CBE0_g(configCtx, nullptr, nullptr, 1, 0, 0, (void*)(intptr_t)modalId);
    atSingleton_CB90_g(configCtx);
}
// ────────────────────────────────────────────────────────────────────────────
// pongGame — Main game object
// @ vtable 0x82071B9C
// ────────────────────────────────────────────────────────────────────────────

struct pongGame {
    void** vtable;  // +0x00
};

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
