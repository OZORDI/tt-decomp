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
// pongGameState — Base game state class (HSM pattern)
// @ vtable 0x82071AA4
// ────────────────────────────────────────────────────────────────────────────

struct pongGameState {
    void** vtable;  // +0x00
    
    // Virtual methods (HSM state interface)
    virtual ~pongGameState() = default;
    virtual void Init() {}                           // [10] — initialize state resources
    virtual void OnEnter(int prevStateIdx) {}        // [11] — called on state entry
    virtual void OnExit(int nextStateIdx) {}         // [12] — called on state exit
    virtual const char* GetName();                   // [13] — returns state name string
    virtual void ProcessInput() {}                   // [14] — per-frame input processing
};

/**
 * pongGameState::GetName
 * @ 0x823D5318 | size: 0xc
 * 
 * Returns a string constant identifier for this state.
 * Used for debugging/logging state names.
 */
const char* pongGameState::GetName() {
    // Returns string constant @ 0x82071844 (.rdata, 14 bytes)
    // Likely "pongGameState" or similar state name
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
    uint8_t  _pad_08[0x0C];       // +0x08..+0x13
    uint8_t  m_bMenuAccept;       // +0x15 — set=1 on msg 2140 accept path
    uint8_t  m_bMenuBack;         // +0x16 — set=1 on msg 2140 cancel/back path
    uint8_t  m_bMenuCancel;       // +0x17 — set=1 on msg 2141 quit-confirm
};
static_assert(sizeof(pongGameStateData) >= 0x18, "pongGameStateData layout");

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
    uint8_t  _pad_00[8];          // +0x00..+0x07
    int32_t  m_iSessionState;     // +0x08 — 3 = in-session menu, 5 = leaving
    uint8_t  _pad_0C[0x0C];
    uint8_t  m_bSuspended;        // +0x18 — nonzero: swallow menu-navigation events
};
static_assert(sizeof(pongInputObj) >= 0x19, "pongInputObj layout");

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
    if (msgId == 0x85D) {  // 2141 — sets input field +0x30=5, touches event +0x16/+0x17
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x80D) {  // 2061 — toggles event +0x1F flag; float compare via atStringHash "f"
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x86D) {  // 2157 — reads atStringHash "i", stores r6 into *(event+0x10)
        // TODO: case body — see agent a57c057
        return 0;
    }

    // ── 0x20xx block (msgGame: gameplay / match) ──────────────────────────
    if (msgId == 0x2001) {  // 8193 — sets input +0x30=2, calls sub_823D7858(event)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x2007) {  // 8199 — sets *(this+0x1D)=1 (paired with 0x200A/0x200E/0x200F)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x200A) {  // 8202 — clears *(this+0x1D)=0
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x200E) {  // 8206 — falls through to 0x2007-style set (see loc_823D6248)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x200F) {  // 8207 — falls through to 0x200E (see loc_823D622C)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x2019) {  // 8217 — msgNet::kJoinFromHudRequested (sets event+0x1E, observer notify)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x201B) {  // 8219 — input state fork, atStringHash "i", NotifyObservers w/ 0xA0/0x2/0x6
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x2027) {  // 8231 — msgUI::kPostNetTourneyUIEnd, sets *(event+0x16)=1
        // TODO: case body — see agent a57c057
        return 0;
    }

    // ── 0x38xx block (msgNet: network / tourney) ──────────────────────────
    if (msgId == 0x381C) {  // 14364 — bare early-return stub (no body in original)
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x382D) {  // 14381 — sets *(event+0x15)=1
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x3831) {  // 14385 — msgNet::kTourneyNextMatchReady, sets *(event+0x15)=1
        // TODO: case body — see agent a57c057
        return 0;
    }
    if (msgId == 0x3837) {  // 14391 — checks atStringHash "i" != 0, sets *(event+0x15)=1
        // TODO: case body — see agent a57c057
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
