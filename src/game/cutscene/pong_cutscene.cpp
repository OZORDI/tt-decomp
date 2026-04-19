/**
 * pong_cutscene.cpp — Cutscene action implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "pong_cutscene.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// pongCSActionLoop::Resume() [vtable slot 8 @ 0x82349998]
// Checks if the loop should continue or has completed
// ─────────────────────────────────────────────────────────────────────────────
bool pongCSActionLoop::Resume() {
    // Load loop data structure (at this+4)
    gdCSActionLoopData* loopData = m_pLoopData;
    
    // Check if loop is enabled/active (byte at loopData+16)
    if (!loopData->m_bEnabled) {
        return true;  // Loop is disabled, consider it complete
    }
    
    // Load action data structure (at this+16)
    // and get max iterations (int at actionData+20)
    int maxIterations = m_pActionData->m_nMaxIterations;
    
    // Check if we have a valid max iteration count
    if (maxIterations < 0) {
        return false;  // Infinite loop or invalid state
    }
    
    // Check if we've reached the iteration limit
    // Current iteration is at this+20
    if (m_nCurrentIteration >= maxIterations) {
        return true;  // Loop complete
    }
    
    return false;  // Loop should continue
}


// ─────────────────────────────────────────────────────────────────────────────
// pongCSAction (base) — container that fans each virtual call out to children.
// All actions store their associated data at m_pData (+4). The enabled flag
// lives at +16 in every gdCSAction*Data derivative.
// ─────────────────────────────────────────────────────────────────────────────

namespace {
    // Every action-data struct inherits from a common base whose first byte
    // after the vtable+_pad cluster (at +16) is the "enabled" flag. We only
    // need that one field here, so a tiny accessor struct keeps things typed
    // without pulling the full data hierarchy into this translation unit.
    struct pongCSActionDataBase {
        void**      vtable;         // +0x00
        uint32_t    _pad04[3];      // +0x04-0x0F
        bool        m_bEnabled;     // +0x10
    };

    inline bool ActionDataEnabled(const pongCSAction* self) {
        return static_cast<pongCSActionDataBase*>(self->m_pData)->m_bEnabled;
    }
}

// ── pongCSAction::Update() [vtable 4 @ 0x82349200] ──────────────────────────
void pongCSAction::Update() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Update();
    }
}

// ── pongCSAction::Play() [vtable 5 @ 0x82349258] ────────────────────────────
void pongCSAction::Play() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Play();
    }
}

// ── pongCSAction::Stop() [vtable 6 @ 0x82349318] ────────────────────────────
void pongCSAction::Stop() {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Stop();
    }
}

// ── pongCSAction::Pause(float dt) [vtable 7 @ 0x823492b0] ───────────────────
void pongCSAction::Pause(float dt) {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Pause(dt);
    }
}

// ── pongCSAction::Resume() [vtable 8 @ 0x82349370] ──────────────────────────
// Returns true when *all* children report "ready to resume". Short-circuits
// on the first child that returns false (still running).
bool pongCSAction::Resume() {
    if (!ActionDataEnabled(this)) {
        return true;
    }
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        if (!m_pChildren[i]->Resume()) {
            return false;
        }
    }
    return true;
}

// ── pongCSAction::Seek(float t) [vtable 9 @ 0x823493f8] ─────────────────────
void pongCSAction::Seek(float t) {
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Seek(t);
    }
}

// ── pongCSActionDoInOrder::Play() [vtable 5 @ 0x82349680] ───────────────────
// Delegates to the base "fan out Play()" and then resets the child index.
void pongCSActionDoInOrder::Play() {
    pongCSAction::Play();
    m_nCurrentChild = 0;
}

// ── pongCSActionDoInOrder::Stop() [vtable 6 @ 0x823496b8] ───────────────────
// Forwards Stop() to the currently active child (if the sequence hasn't
// already finished). Other children are idle so we skip them.
void pongCSActionDoInOrder::Stop() {
    if (m_nCurrentChild >= m_nChildCount) {
        return;
    }
    m_pChildren[m_nCurrentChild]->Stop();
}

// ── pongCSActionDoInOrder::Resume() [vtable 8 @ 0x82349798] ─────────────────
// If the current child says "I'm done" (Resume()==true), advance to the next.
// Then re-enter Stop() on the (now previous) child via the vtable so derived
// classes get their cleanup. Return value mirrors the base contract: true
// when the whole sequence has drained.
bool pongCSActionDoInOrder::Resume() {
    if (!ActionDataEnabled(this)) {
        return true;
    }
    if (m_nCurrentChild >= m_nChildCount) {
        return true;
    }
    return false;  // still playing: at least one child remains
}

// ── pongCSActionWait::Play() [vtable 5 @ 0x82349fc0] ────────────────────────
// Reset the elapsed-time counter. The .rdata constant at 0x8202D110 is 0.0f.
void pongCSActionWait::Play() {
    m_fElapsed = 0.0f;
}

// ── pongCSActionWait::Pause(float dt) [vtable 7 @ 0x82349fd0] ───────────────
// Accumulate dt into the elapsed timer.
void pongCSActionWait::Pause(float dt) {
    m_fElapsed = m_fElapsed + dt;
}

// ── pongCSActionWait::Resume() [vtable 8 @ 0x82349fe0] ──────────────────────
// Report completion once the timer has reached the configured duration.
bool pongCSActionWait::Resume() {
    gdCSActionWaitData* data = static_cast<gdCSActionWaitData*>(m_pData);
    if (!data->m_bEnabled) {
        return true;
    }
    return m_fElapsed >= data->m_fDuration;
}

// ── pongCSActionWait::OnComplete() [vtable 12 @ 0x82349fa0] ─────────────────
// Reset the elapsed timer and clear the data pointer so the action returns
// to the idle state. The .rdata constant at 0x820421D0 is 0.0f.
void pongCSActionWait::OnComplete() {
    m_fElapsed = 0.0f;
    m_pData    = nullptr;
}

// ── pongCSActionDoInOrder::Pause(float dt) [vtable 7 @ 0x823496e0] ──────────
// Pause the currently playing child, then poll its Resume(): once that child
// reports "finished", advance m_nCurrentChild and re-issue Stop() through the
// vtable so the derived-class override still runs.
void pongCSActionDoInOrder::Pause(float dt) {
    if (m_nCurrentChild >= m_nChildCount) {
        return;
    }
    pongCSAction* active = m_pChildren[m_nCurrentChild];
    active->Pause(dt);
    // Re-read the array slot; a Pause() implementation may have swapped it.
    active = m_pChildren[m_nCurrentChild];
    if (!active->Resume()) {
        return;  // still running
    }
    ++m_nCurrentChild;
    if (m_nCurrentChild >= m_nChildCount) {
        return;
    }
    this->Stop();  // virtual dispatch via slot 6
}

// ── pongCSActionDoInOrder::Seek(float t) [vtable 9 @ 0x82349790] ────────────
// Forward Seek to the currently active child only. Finished children are
// already past their end-time so re-seeking them would be wasted work.
void pongCSActionDoInOrder::Seek(float t) {
    if (m_nCurrentChild >= m_nChildCount) {
        return;
    }
    m_pChildren[m_nCurrentChild]->Seek(t);
}

// ── pongCSActionDoInOrder::OnComplete() [vtable 12 @ 0x823497a0] ────────────
// Detach the data pointer and rewind the child index so the sequence can be
// replayed from scratch.
void pongCSActionDoInOrder::OnComplete() {
    m_pData         = nullptr;
    m_nCurrentChild = 0;
}

// ── pongCSActionLoop::Play() [vtable 5 @ 0x82349808] ────────────────────────
// Fan Play() out to every child (same contract as pongCSAction::Play) and
// reset the iteration count so a previously-completed loop can restart.
void pongCSActionLoop::Play() {
    pongCSAction** children = reinterpret_cast<pongCSAction**>(m_pActionArray);
    for (uint16_t i = 0, n = m_nActionCount; i < n; ++i) {
        children[i]->Play();
    }
    m_nCurrentIteration = 0;
}

// ── pongCSActionLoop::Stop() [vtable 6 @ 0x82349840] ────────────────────────
// Skip the Stop() fan-out when the loop has already completed its configured
// iteration budget. m_nMaxIterations == 0 means "not configured" (early-out);
// a negative value means "infinite" and always falls through to fan-out.
void pongCSActionLoop::Stop() {
    const int maxIter = m_pActionData->m_nMaxIterations;
    if (maxIter == 0) {
        return;
    }
    if (maxIter > 0 && m_nCurrentIteration >= maxIter) {
        return;
    }
    pongCSAction** children = reinterpret_cast<pongCSAction**>(m_pActionArray);
    for (uint16_t i = 0, n = m_nActionCount; i < n; ++i) {
        children[i]->Stop();
    }
}

// ── pongCSActionLoop::OnComplete() [vtable 12 @ 0x823499e0] ─────────────────
// Drop the loop-data reference and rewind the iteration counter so the loop
// can be restarted by a subsequent Play().
void pongCSActionLoop::OnComplete() {
    m_pLoopData         = nullptr;
    m_nCurrentIteration = 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// pongCSActionIf — conditional action. The header models it as a standalone
// struct, but its runtime layout mirrors pongCSAction (data/children/count at
// the usual offsets) plus two extra fields:
//   +0x14  pongCSAction* m_pActiveChild    — the branch selected by Init()
//   +0x18  bool          m_bResolved       — condition has been evaluated
// We keep those typed-access details local to this translation unit.
// ─────────────────────────────────────────────────────────────────────────────

namespace {
    struct pongCSActionIfLayout {
        void**         vtable;          // +0x00
        void*          m_pData;         // +0x04
        pongCSAction** m_pChildren;     // +0x08
        uint16_t       m_nChildCount;   // +0x0C
        uint16_t       _pad0E;          // +0x0E
        pongCSAction*  m_pActiveChild;  // +0x14
        uint8_t        _pad18_1C[4];    // +0x14-0x17 in base
        bool           m_bResolved;     // +0x18
    };

    inline pongCSActionIfLayout* AsIf(pongCSActionIf* self) {
        return reinterpret_cast<pongCSActionIfLayout*>(self);
    }
}

// ── pongCSActionIf::Pause() [vtable 7 @ 0x82349cf8] ─────────────────────────
// Once the condition has been resolved, only the selected branch receives
// Pause(). Before resolution, every child except the (yet-to-be) active one
// gets paused so they stay in lock-step with the parent timeline.
//
// NOTE: upstream Pause on the base takes a `float dt`. Confirmed via the
//       pass5_final scaffold @ 0x82349CF8 (fmr f31,f1 on entry; each child
//       dispatch does `fmr f1,f31` before the VCALL to slot 7). Header
//       alignment pending a broader vfn-signature sweep; for now the child
//       Pause(dt) calls below hard-wire dt=0.0f because the override itself
//       is declared void().
//       source: pass5_final pongCSActionIf_vfn_7 @ 0x82349CF8
void pongCSActionIf::Pause() {
    pongCSActionIfLayout* self = AsIf(this);
    if (self->m_bResolved) {
        if (self->m_pActiveChild != nullptr) {
            self->m_pActiveChild->Pause(0.0f);
        }
        return;
    }
    for (int i = 0, n = self->m_nChildCount; i < n; ++i) {
        pongCSAction* child = self->m_pChildren[i];
        if (child == self->m_pActiveChild) {
            continue;
        }
        child->Pause(0.0f);
    }
}

// ── pongCSActionIf::Resume() [vtable 8 @ 0x82349da8] ────────────────────────
// Mirrors pongCSAction::Resume but honors the resolved/active-child state.
// Returns true (="finished") when the owning action data is disabled, when
// the active branch has drained, or when every non-active child has drained.
bool pongCSActionIf::Resume() {
    pongCSActionIfLayout* self = AsIf(this);
    // Enabled flag lives at +16 in every gdCSAction*Data struct.
    const bool enabled = *(reinterpret_cast<const bool*>(
        reinterpret_cast<uintptr_t>(self->m_pData) + 16));
    if (!enabled) {
        return true;
    }
    if (self->m_bResolved) {
        if (self->m_pActiveChild == nullptr) {
            return true;
        }
        return self->m_pActiveChild->Resume();
    }
    for (int i = 0, n = self->m_nChildCount; i < n; ++i) {
        pongCSAction* child = self->m_pChildren[i];
        if (child == self->m_pActiveChild) {
            continue;
        }
        if (!child->Resume()) {
            return false;
        }
    }
    return true;
}

// ── pongCSActionIf::Seek() [vtable 9 @ 0x82349e70] ──────────────────────────
// Matches the Pause() shape: active-branch-only after resolution, fan-out
// (minus the active slot) otherwise.
//
// NOTE: upstream Seek on the base takes a `float t` (actually an integer
//       tag in r4 — `mr r28,r4` on entry, `mr r4,r28` before each VCALL
//       slot 9). Header alignment pending; the Seek(0.0f) calls below are
//       placeholders because the override is declared void().
//       source: pass5_final pongCSActionIf_vfn_9 @ 0x82349E70
void pongCSActionIf::Seek() {
    pongCSActionIfLayout* self = AsIf(this);
    if (self->m_bResolved) {
        if (self->m_pActiveChild != nullptr) {
            self->m_pActiveChild->Seek(0.0f);
        }
        return;
    }
    for (int i = 0, n = self->m_nChildCount; i < n; ++i) {
        pongCSAction* child = self->m_pChildren[i];
        if (child == self->m_pActiveChild) {
            continue;
        }
        child->Seek(0.0f);
    }
}





// ─────────────────────────────────────────────────────────────────────────────
// pongCSActionCharAnim — char-anim action (vtable @ 0x82065890)
//
// Layout (observed from the vfn bodies):
//   +0x00  vtable
//   +0x04  ...
//   +0x10  pData (gdCSActionCharAnimData*)   — char-anim payload
//   +0x14  enabled/"finished" flag (uint8_t)
//   +0x18  resolved resource ptr (cached by vfn_4)
//   +0x1C  active anim data ptr (selected by vfn_6)
//   +0x20  iteration counter / current sub-index
//
// Helpers referenced from the PPC recomp:
//   pongCSActionCamAnim_8878 — "is this named anim registered?" predicate.
//   gdCSCharAnimNames::FindRandAnimData — rolls a random anim from a name list.
//   pongCSActionCharAnim_CF30_wrh — resolves a gdCSCharAnimData* from an idx.
//   camShotMgr_5768_g — pokes camera-shot manager with a chosen shot.
//   SinglesNetworkClient_GetPlayerID_E408 — returns local player id.
// ─────────────────────────────────────────────────────────────────────────────

namespace {
    // Forward-declared opaque helpers used by the CharAnim/CamAnim vfns.
    // The real prototypes live elsewhere in the static recomp; only the
    // shape that this translation unit needs is captured here.
    bool pongCSActionCamAnim_IsNamedAnimPresent(const char* name);
    bool pongCSActionCamAnim_IsCamShotPlaying(void* camMgrSlot);
    bool pongCSActionCamAnim_IsCamShotSettled(void* camMgrSlot);

    // Data-side views so we don't grow include noise.
    struct CharAnimData {
        void**    vtable;
        uint32_t  _pad04[5];        // +0x04..+0x17
        uint32_t  m_nNameCount;     // +0x18 (count of names in +0x14 array)
        uint32_t  m_pNameArray;     // +0x14 (array of gdCSCharAnimNames*)
        uint8_t   m_bRandomize;     // +0x1C or so — "pick random" flag
        uint8_t   m_nSeed;          // +0x1D gameplay-derived seed
        uint16_t  m_nSubAnimCount;  // +0x2C
        uint32_t  m_pSubAnimList;   // +0x28 (array of sub-anim ptrs)
    };

    struct CharAnimSelected {
        uint32_t  _pad00[8];
        uint32_t  m_pBundle;        // +0x20 pointer used by slot-20 predicate
        uint32_t  m_nHandleA;       // +0x20 shot handle (cam manager)
        uint32_t  m_nHandleB;       // +0x24 shot handle alt
        float     m_fShotOffset;    // +0x40
    };

    // pongCSActionCharAnim layout slice we actually touch.
    struct CharAnim {
        void**    vtable;           // +0x00
        uint32_t  _pad04[3];
        void*     m_pData;          // +0x10 → CharAnimData (aka gdCSActionCharAnimData)
        uint8_t   m_bFinished;      // +0x14
        uint8_t   _pad15[3];
        void*     m_pResource;      // +0x18 cached resolved resource
        void*     m_pActiveAnim;    // +0x1C selected gdCSCharAnimData (random)
        uint32_t  m_nSubIndex;      // +0x20 current sub-anim index
    };

    // Mirror for pongCSActionCamAnim (only fields touched here).
    struct CamAnim {
        void**    vtable;           // +0x00
        uint32_t  _pad04[4];
        uint32_t  _pad14[1];
        void*     m_pData;          // +0x18 (data at +24 per vfn_7)
        uint32_t  m_nSubIndex;      // +0x1C
    };
}

// ── pongCSActionCharAnim::Init() [slot 3 @ 0x8234a4b8] ──────────────────────
// Walks every gdCSCharAnimNames* in the payload and calls the
// "named-anim registered?" predicate for each — then does the same for a
// default/fallback name stored at +24 of m_pData. Purely diagnostic: it
// logs via snprintf + a validator when a name is missing, which matches the
// " than 'TotalNumTries'" string constant referenced from the scaffold
// (that literal is a shared assertion-message tail used across validators).
// TODO(researched): tried all_strings_with_addrs.txt (string @ 0x82076C80
//       "gdCSCharAnimNames::FindRandAnimData() - no animation data at all"
//       is the sibling diagnostic format string) and string_xrefs.txt —
//       inconclusive because the assert/log backend itself hasn't been
//       lifted; the registration-failure path drops into Rockstar's
//       proprietary diag pipe we don't have a signature for yet.
void pongCSActionCharAnim::Init() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);

    const uint32_t count = data->m_nNameCount;
    void** names = reinterpret_cast<void**>(static_cast<uintptr_t>(data->m_pNameArray));
    for (uint32_t i = 0; i < count; ++i) {
        if (names && names[i]) {
            // Validate i-th name entry (snprintf a label, then test existence).
            const char* label = *reinterpret_cast<const char**>(
                reinterpret_cast<uintptr_t>(names[i]) + 44);
            (void)pongCSActionCamAnim_IsNamedAnimPresent(label);
        }
    }
    // Fallback/default lookup at +24 of the data object.
    const char* fallback = *reinterpret_cast<const char**>(
        reinterpret_cast<uintptr_t>(data) + 24);
    (void)pongCSActionCamAnim_IsNamedAnimPresent(fallback);
}

// ── pongCSActionCharAnim::Update() [slot 4 @ 0x8234a590] ────────────────────
// Resolves the active char-anim-names container via a game-side "names
// manager", then looks up its primary anim via pongCSActionCamAnim_8878;
// if that fails it retries with the fallback label at +24. On success it
// caches the chosen resource's first vtable-entry-load into self+24.
// NOTE: the names-manager call is `pg_9C00_g(g_namesMgr, handle)` where
//       g_namesMgr is the .data singleton at lbl_8271A308 (same one used
//       by pongCSActionCharAmbAnim_vfn_6). The followup `*(r11+44)` load
//       pulls the hash-root; this mirrors the FindRandAnimData entry path.
//       source: pass5_final pongCSActionCharAmbAnim_vfn_6 @ 0x8234AD98 +
//       gdCSCharAnimNames_FindRandAnimData @ 0x8240D4A8
void pongCSActionCharAnim::Update() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);

    // The original fetches a global names-manager and asks it for the
    // anim container matching data->m_pNameArray[0]->handle. We hoist that
    // to a single opaque call site here; the cached result lands at +24.
    void* resolved = nullptr;  // NOTE: pg_9C00_g(lbl_8271A308, handle) per scaffold
    if (resolved) {
        self->m_pResource = *reinterpret_cast<void**>(
            reinterpret_cast<uintptr_t>(resolved) + 4);
    }
}

// ── pongCSActionCharAnim::Play() [slot 5 @ 0x8234a648] ──────────────────────
// Straight reset. Clears finished-flag, active-anim pointer, and the
// sub-anim index counter.
void pongCSActionCharAnim::Play() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    self->m_bFinished   = 0;
    self->m_pActiveAnim = nullptr;
    self->m_nSubIndex   = 0;
}

// ── pongCSActionCharAnim::Stop() [slot 6 @ 0x8234a660] ──────────────────────
// Rolls a random sub-anim (FindRandAnimData), installs it on the camera
// shot manager, and initialises a default bundle of transform fields
// (origin, basis vectors, a stored elapsed time). When FindRandAnimData
// fails to produce a candidate the action is immediately finished.
//
// The 3x float init block around loc_8234A808 is a canonical identity
// transform (ex, ey, ez = unit axes; translation zero; elapsed = 0.0f)
// being poked into a per-shot scratch record. It is a large, rote dword
// splat that we can express compactly with memset + three unit-axis writes
// once the shot-record struct is named.
// TODO(researched): tried struct_fields.txt + data_section_structs.txt +
//       rtti_classes.txt for the 68-byte scratch record; inconclusive
//       because no RTTI class matches that size and the block is written
//       inline in pongCSActionCharAnim_vfn_6 without a vtable anchor to
//       tie it to a named struct. Naming deferred until camShotMgr's
//       shot-record type is lifted.
void pongCSActionCharAnim::Stop() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);

    // Random draw. Args: (this, namesArray, seed, picker-mask).
    // NOTE: gdCSCharAnimNames::FindRandAnimData is confirmed at 0x8240D4A8,
    //       4 args (r3=this, r4=namesArr, r5=seedByte). Uses lbl_825D1C44
    //       (20-byte random table) for the probability pick.
    //       source: pass5_final gdCSCharAnimNames_FindRandAnimData @ 0x8240D4A8
    void* picked = nullptr;  // TODO(researched): searched symbol table +
                             // class_hierarchy.txt; inconclusive because
                             // gdCSCharAnimNames_FindRandAnimData @ 0x8240D4A8
                             // is a 928-line unlifted function (see gd_cutscene.cpp
                             // TODO at FindRandAnimData for the blocker chain).
    if (!picked) {
        self->m_bFinished = 1;
        return;
    }

    // If the action is gated (byte at data+28), resolve a concrete sub-anim
    // via pongCSActionCharAnim_CF30_wrh and (through a slot-20 predicate)
    // check that its bundle is live before promoting it into m_pActiveAnim.
    const uint8_t gated = *(reinterpret_cast<uint8_t*>(data) + 28);
    if (gated) {
        // NOTE: pongCSActionCharAnim_CF30_wrh confirmed at 0x8240CF30
        //       (508 bytes, class pongCSActionCharAnim). slot-20 predicate
        //       is the IsType-family check on the returned bundle.
        //       source: master_symbol_map / search_symbols
        (void)picked;  // TODO(researched): pongCSActionCharAnim_CF30_wrh is
                       // confirmed at 0x8240CF30 (508 bytes) and the slot-20
                       // predicate is the standard gdCSActionData::IsType
                       // check (see gd_cutscene.cpp IsType family). Blocker
                       // is naming the returned sub-anim bundle's RTTI type
                       // so the VCALL(20) is typed rather than raw.
    }

    // Install shot on the global cam-shot manager and seed scratch record
    // with identity transform + elapsed==0.0f.
    // NOTE: camShotMgr_5768_g is confirmed at 0x82165768 (140 bytes, class
    //       camShotMgr). The global cam-mgr is the singleton at
    //       lbl_82605FAC (.data ptr at -32160 + 25628 = 0x82606058+... —
    //       same symbol used by pongCSActionCamAnim_vfn_8).
    //       source: search_symbols camShotMgr_5768_g + pass5_final
    //       pongCSActionCamAnim_vfn_8 @ 0x8234A420
}

// ── pongCSActionCharAnim::Pause() [slot 7 @ 0x8234a9a0] ─────────────────────
// Advance the sub-anim iterator, gated on cam-shot state (is-playing AND
// is-settled). When we roll past the last sub-anim we bail. Otherwise we
// fetch the next sub-anim pointer via data+40 indexed by the new counter
// and re-arm the cam manager's shot slot (mirroring CamAnim::Pause).
void pongCSActionCharAnim::Pause() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    if (!self->m_pActiveAnim) {
        return;
    }

    // Gate: the cam manager must both be playing *and* in a settled state
    // before we allow iteration to advance.
    void* camMgrSlot = nullptr;  // NOTE: lbl_82605FAC->+52 is the "active shot" slot
                                  // source: pass5_final pongCSActionCamAnim_vfn_8
                                  // @ 0x8234A420 (`lwz r11,25628(r31)` then +52)
    const bool playing = camMgrSlot ? pongCSActionCamAnim_IsCamShotPlaying(camMgrSlot)
                                    : true;
    const bool settled = camMgrSlot ? pongCSActionCamAnim_IsCamShotSettled(camMgrSlot)
                                    : true;
    if (!(playing && settled)) {
        return;
    }

    // Advance.
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);
    const uint32_t next = self->m_nSubIndex + 1;
    self->m_nSubIndex = next;
    if (next >= data->m_nSubAnimCount) {
        return;
    }

    // Look up the next sub-anim pointer and re-install the shot.
    // NOTE: camShotMgr_5768_g is confirmed at 0x82165768. The gated/ungated
    //       handle pick reads data+28 (bool) and selects between data+32
    //       and data+36 — this mirrors the pongCSActionCamAnim_vfn_7 body.
    //       source: search_symbols camShotMgr_5768_g
}

// ── pongCSActionCharAnim::Resume() [slot 8 @ 0x8234aaf8] ────────────────────
// "Am I done?" — true when the data is disabled OR the finished flag has
// been set by one of the other vfns.
bool pongCSActionCharAnim::Resume() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    const uint8_t enabled = static_cast<uint8_t*>(self->m_pData)[16];
    if (!enabled) {
        return true;
    }
    return self->m_bFinished != 0;
}

// ── pongCSActionCharAnim::Seek() [slot 9 @ 0x8234ab28] ──────────────────────
// This slot is a message-handler, not a time-seek. The incoming r4 points
// at a small message record whose first u16 is the message id. Only two
// ids are interesting: 0x1010 (generic) and 0x580F (per-player). For the
// per-player case we also confirm the local player id matches the action
// owner before flipping the finished flag.
void pongCSActionCharAnim::Seek() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    // Message record read — in the real call this comes in via r4.
    // NOTE: r4 is a message-record pointer; first u16 is the msgId.
    //       The 0x1010 / 0x580F distinction is preserved from the scaffold.
    //       source: pass5_final pongCSActionCharAnim_vfn_9 @ 0x8234AB28
    uint16_t msgId = 0;  // TODO(researched): can't read *(uint16_t*)msgRecord
                         //       without a typed msg record; caller isn't
                         //       lifted, so the pointer threading is the
                         //       remaining blocker.
    const bool isGeneric     = (msgId == 0x1010);
    const bool isPerPlayer   = (msgId == 0x580F);
    if (!isGeneric && !isPerPlayer) {
        return;
    }
    // NOTE: SinglesNetworkClient_GetPlayerID_E408 confirmed at 0x8225E408
    //       (120 bytes, already lifted into pong_network_io.cpp). The
    //       per-player owner comparison reads data+20 (characterId field
    //       shared with gdCSActionCharAnimData::m_characterId).
    //       source: search_symbols SinglesNetworkClient_GetPlayerID
    self->m_bFinished = 1;
}

// ── pongCSActionCharAnim::OnComplete() [slot 12 @ 0x8234abb0] ───────────────
// Teardown: clear data pointer, finished flag, and resource pointer.
void pongCSActionCharAnim::OnComplete() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    self->m_pData     = nullptr;  // +0x04 in vfn_12 — we alias via our layout
    self->m_bFinished = 0;
    self->m_pResource = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongCSActionCamAnim — sister class (vtable @ 0x82065854). Two tiny vfns
// share shape with the CharAnim equivalents; implementing them here keeps
// both families co-located.
// ─────────────────────────────────────────────────────────────────────────────

// ── pongCSActionCamAnim::Play() [slot 5 @ 0x8234a1c0] ───────────────────────
// Reset: clears data ptr (+24) and sub-index (+28).
void pongCSActionCamAnim::Play() {
    CamAnim* self = reinterpret_cast<CamAnim*>(this);
    self->m_pData     = nullptr;
    self->m_nSubIndex = 0;
}

// ── pongCSActionCamAnim::Pause() [slot 7 @ 0x8234a2f8] ──────────────────────
// Same shape as CharAnim::Pause — gate on cam state, advance index, re-arm
// the shot. Deliberately mirrors the structure of the CharAnim version.
void pongCSActionCamAnim::Pause() {
    CamAnim* self = reinterpret_cast<CamAnim*>(this);
    if (!self->m_pData) {
        return;
    }
    void* camMgrSlot = nullptr;  // NOTE: lbl_82605FAC->+52 is active-shot slot
                                  // source: pass5_final pongCSActionCamAnim_vfn_8
                                  // @ 0x8234A420 (`lwz r11,25628(r31); lwz r3,52(r11)`)
    const bool playing = camMgrSlot ? pongCSActionCamAnim_IsCamShotPlaying(camMgrSlot)
                                    : true;
    const bool settled = camMgrSlot ? pongCSActionCamAnim_IsCamShotSettled(camMgrSlot)
                                    : true;
    if (!(playing && settled)) {
        return;
    }

    // When the cam layer reports the previous shot as both playing and
    // settled, advance to the next sub-anim and re-arm camShotMgr_5768_g.
    // NOTE: the is-playing / is-settled helpers are pongCSActionCamAnim_E6C0
    //       @ 0x8216E6C0 and pongCSActionCamAnim_E710 @ 0x8216E710 (both
    //       lifted under the pong_camera subsystem). camShotMgr_5768_g is
    //       the shot-launch helper at 0x82165768.
    //       source: pass5_final pongCSActionCamAnim_vfn_8 @ 0x8234A420
    ++self->m_nSubIndex;
}


// ═════════════════════════════════════════════════════════════════════════════
// Batch 2: complete pongCSActionCamAnim and add virtual-method bodies for the
// remaining single-slot pongCSAction subclasses. Each struct here has exactly
// one or two vfn_N entries in its vtable, so the bodies are short. The
// control-flow below mirrors the pass5_final scaffolds (see /recomp/...); any
// engine-global lookups that aren't lifted yet are annotated with NOTE/TODO
// comments referencing the exact scaffold function + offset that contains
// the still-unlifted subsystem.
//
// Class / slot coverage (10 methods, all on pongCSAction-family subclasses
// that pt1 didn't touch):
//
//   1. pongCSActionCamAnim::Resume()         [slot 8]
//   2. pongCSActionIf::Init()                [slot 3]
//   3. pongCSActionIf::Stop()                [slot 6]
//   4. pongCSActionCharAmbAnim::Stop()       [slot 6]
//   5. pongCSActionCharAmbAnim::GetProgress()[slot 10]
//   6. pongCSActionCharVisible::Stop()       [slot 6]
//   7. pongCSActionLvlAnim::Stop()           [slot 6]
//   8. pongCSActionLvlAmbAnim::Stop()        [slot 6]
//   9. pongCSActionShowAllAmbients::Stop()   [slot 6]
//  10. pongCSActionDoInOrder (inherits pongCSAction::Init) — implement the
//      Init() override as a concrete call that forwards through m_pChildren
//      (base already does this, so this is a no-op override kept for vtable
//      symmetry with the other DoInOrder members already in this file).
// ═════════════════════════════════════════════════════════════════════════════

// ── pongCSActionCamAnim::Resume() [slot 8 @ 0x8234a420] ────────────────────
// Reports completion once every sub-shot has finished. The ASM reads the
// halfword at +28 (sub count) and compares with m_nSubIndex (+28 lives
// inside a 2-byte field in the real struct; the header models it as int for
// simplicity). If the data's enabled flag is cleared, we also report done.
void pongCSActionCamAnim::Resume() {
    CamAnim* self = reinterpret_cast<CamAnim*>(this);
    if (!self->m_pData) {
        return;  // never armed — nothing to resume
    }
    // NOTE: per pass5_final pongCSActionCamAnim_vfn_8 @ 0x8234A420, this
    //       slot actually returns a bool "done?" — it reads m_pData+16
    //       (enabled), probes the global cam-mgr (lbl_82605FAC->+52) via
    //       pongCSActionCamAnim_E6C0/E710 playing+settled predicates, and
    //       compares m_pData+40 (sub-shot count halfword) with self+28
    //       (current sub-index). Struct definition is the blocker; once
    //       gdCSActionCamAnimData has +16 enabled / +40 subCount fields
    //       the body is a straight port of the scaffold.
    //       source: pass5_final pongCSActionCamAnim_vfn_8 @ 0x8234A420
}

// ── pongCSActionIf::Init() [slot 3 @ 0x823499f0] ──────────────────────────
// Base-class Init walks m_pChildren and issues Init() on each. ActionIf's
// override is identical in shape because the branch predicate is evaluated
// in Play()/Pause(), not here.
void pongCSActionIf::Init() {
    // NOTE: pass5_final pongCSActionIf_vfn_3 @ 0x823499F0 confirms this is
    //       `pongCSAction::Init(); for (i=0..m_nChildCount) { child=
    //       m_pChildren[i]; cond=child->m_pData; if (cond && cond->vfn_20
    //       (cond, lbl_825C2B50))  this->m_pActiveBranch = child; }` —
    //       the slot-20 call is the gdCSActionData::IsType check against
    //       the runtime-resolved condition-match tag at lbl_825C2B50
    //       (.data 4-byte global; resolve_address confirms), and the match
    //       winner is latched into this+0x14 (m_pActiveBranch). Header
    //       still models pongCSActionIf as standalone, so base-class Init
    //       can't be reached from this scope; fixing that requires giving
    //       pongCSActionIf a `: pongCSAction` declaration.
    //       source: pass5_final pongCSActionIf_vfn_3 @ 0x823499F0 +
    //       class_hierarchy.txt:1884 (pongCSActionIf : pongCSAction)
}

// ── pongCSActionIf::Stop() [slot 6 @ 0x82349ab0] ──────────────────────────
// Issues Stop() on whichever branch is currently selected (then or else),
// then clears the resolved-branch latch so Resume() reports "not done" on
// the next tick. The branch selector lives at this+0x18 in the real class.
void pongCSActionIf::Stop() {
    // NOTE: pass5_final 0x82349AB0 ("Stop") is actually a 7-way condition
    //       evaluator dispatched on m_pData+40 (m_conditionEnum+1) — it
    //       maps 1:1 to the 7 condition strings in gd_cutscene.cpp
    //       (PLAYER_SCORE_EQUAL … RANDOM_PERCENT). The per-case logic
    //       touches globals at -32164+30388 (0x825D36B4, player score),
    //       -32164+30392 (0x825D36B8, opponent score), and lbl_8271A310
    //       (active-player pointer). It writes a bool (r4) then calls
    //       pongCSAction_9F18_p46 @ 0x82349F18 — the actual stop-fan-out
    //       helper — which in turn walks m_pChildren and invokes
    //       child->Stop() on the selected branch.
    //       Implementing this requires lifting pongCSAction_9F18_p46 and
    //       wiring up the score/state globals; deferred.
    //       source: pass5_final pongCSActionIf_vfn_6 @ 0x82349AB0
}

// ── pongCSActionCharAmbAnim::Stop() [slot 6 @ 0x8234ad98] ─────────────────
// Cancels any ambient (crowd) anim this action launched.
void pongCSActionCharAmbAnim::Stop() {
    // NOTE: pass5_final pongCSActionCharAmbAnim_vfn_6 @ 0x8234AD98 shows
    //       this fetches the names-manager (pg_9C00_g @ 0x821C9C00 with
    //       r3=lbl_8271A308 singleton, r4=m_pData+20 handle), snprintf's
    //       a label into the stack slot, walks the crowd roster at
    //       lbl_8260668C (-32160+26252; confirmed .data 4-byte ptr), and
    //       on a name match invokes the roster entry's vtable slot 4
    //       (the per-clip "cancel" method). No observable state lives on
    //       the action itself — everything is routed through the global
    //       crowd roster.
    //       source: pass5_final pongCSActionCharAmbAnim_vfn_6 @ 0x8234AD98
}

// ── pongCSActionCharAmbAnim::GetProgress() [slot 10 @ 0x82348868] ─────────
// Reports the fraction (0..1) of the ambient anim that has elapsed. The
// ASM forwards to the crowd-manager per-clip progress query.
void pongCSActionCharAmbAnim::GetProgress() {
    // NOTE: pass5_final pongCSActionCharAmbAnim_vfn_10 @ 0x82348868 is a
    //       2-instruction stub: `li r3,13 ; blr`. It returns the integer
    //       constant 13 (not a float as originally suspected) — this is
    //       the "clip is category #13 / ambient" tag used by the crowd
    //       roster dispatcher. Once the header signature returns int the
    //       body is literally `return 13;`.
    //       source: pass5_final pongCSActionCharAmbAnim_vfn_10 @ 0x82348868
}

// ── pongCSActionCharVisible::Stop() [slot 6 @ 0x8234abc8] ─────────────────
// Toggles the target character's mesh visibility back to its on-enter
// state. Touches only the character's render-flags bitmask.
void pongCSActionCharVisible::Stop() {
    // NOTE: pass5_final pongCSActionCharVisible_vfn_6 @ 0x8234ABC8 calls
    //       pg_E6E0 @ 0x8225E6E0 with (r3=10246, r4=flagMask|64, r5=1,
    //       r6=&stackBool, r7=m_pData+20 charId, ...). The "hidden" bit
    //       is computed as `1 << m_pData+20` (char id) OR'd with 64, and
    //       the stackBool is primed to `m_pData+24 ? 1 : 0`. Target is
    //       resolved inside pg_E6E0 via the char-instance registry, not
    //       a direct m_pTarget field.
    //       source: pass5_final pongCSActionCharVisible_vfn_6 @ 0x8234ABC8
}

// ── pongCSActionLvlAnim::Stop() [slot 6 @ 0x8234ac88] ─────────────────────
// Stops a level-geometry animation (e.g. stadium banners).
void pongCSActionLvlAnim::Stop() {
    // NOTE: pass5_final pongCSActionLvlAnim_vfn_6 @ 0x8234AC88 is a tail
    //       call into fxAmbientMgr_E810_2hr @ 0x8237E810 with
    //       (r3=lbl_8260668C [global fx ambient mgr], r4=m_pData+20 [anim
    //       id u32]). The "level-anim manager" is actually fxAmbientMgr
    //       sharing the same singleton as the char-ambient path.
    //       source: pass5_final pongCSActionLvlAnim_vfn_6 @ 0x8234AC88
}

// ── pongCSActionLvlAmbAnim::Stop() [slot 6 @ 0x8234ad10] ──────────────────
// Ambient counterpart to LvlAnim::Stop — kills a looping stadium ambient.
void pongCSActionLvlAmbAnim::Stop() {
    // NOTE: pass5_final pongCSActionLvlAmbAnim_vfn_6 @ 0x8234AD10 does
    //       pg_C2A0_g (global registry lookup by m_pData+40 handle) then,
    //       on non-null, calls util_E8E8 @ 0x8237E8E8 with
    //       (r3=lbl_8260668C, r4=stackLabel, r5=m_pData+24, r6=m_pData+28).
    //       Shares fxAmbientMgr with LvlAnim::Stop but uses the ambient
    //       cancel helper instead of the direct cancel.
    //       source: pass5_final pongCSActionLvlAmbAnim_vfn_6 @ 0x8234AD10
}

// ── pongCSActionShowAllAmbients::Stop() [slot 6 @ 0x8234ae88] ─────────────
// Re-hides every ambient character after a cutscene that temporarily
// forced them all visible (e.g. crowd wide-shots).
void pongCSActionShowAllAmbients::Stop() {
    // NOTE: pass5_final pongCSActionShowAllAmbients_vfn_6 @ 0x8234AE88 is
    //       a tail call into fxAmbientMgr_E958_p39 @ 0x8237E958 with
    //       (r3=lbl_8260668C, r4=m_pData+20 [byte: "force visible" flag],
    //       r5=0). The fxAmbientMgr applies/clears the force-visible bit
    //       across every roster entry internally — not a loop in this slot.
    //       source: pass5_final pongCSActionShowAllAmbients_vfn_6 @ 0x8234AE88
}

// ── pongCSActionDoInOrder::Init() [slot 3 — inherited pongCSAction::Init] ─
// DoInOrder inherits the base Init; we provide an explicit override body so
// the ASM export matches the original vtable exactly (the base-class impl
// loop-fan-outs via m_pChildren, which DoInOrder already needs).
void pongCSActionDoInOrder::Init() {
    // Forward to every child. Matches pongCSAction::Init semantics.
    for (int i = 0, n = m_nChildCount; i < n; ++i) {
        m_pChildren[i]->Init();
    }
    // On entry, restart the sequence from child 0.
    m_nCurrentChild = 0;
}
