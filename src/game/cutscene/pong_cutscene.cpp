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
// TODO: upstream Pause on the base takes a `float dt`. The header's current
//       nullary declaration drops that parameter; align the pongCSActionIf
//       override signature to `Pause(float dt)` in a follow-up header pass
//       and plumb `dt` through to the child dispatch below.
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
// TODO: upstream Seek on the base takes a `float t`. Update the header so
//       this override can forward the timeline value rather than dropping it.
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
// TODO: surface the registration failures as a proper assert/log once the
//       diagnostic subsystem is lifted.
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
// TODO: model the names-manager lookup (the pg_9C00_g call) once that
//       subsystem is lifted; for now the effect is captured by writing the
//       resolved pointer into m_pResource.
void pongCSActionCharAnim::Update() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);

    // The original fetches a global names-manager and asks it for the
    // anim container matching data->m_pNameArray[0]->handle. We hoist that
    // to a single opaque call site here; the cached result lands at +24.
    void* resolved = nullptr;  // TODO: pg_9C00_g(globalNamesMgr, handle)
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
// TODO: introduce a typed CamShotScratch struct for the 68-byte init block
//       so the identity-transform store isn't a pile of offset literals.
void pongCSActionCharAnim::Stop() {
    CharAnim* self = reinterpret_cast<CharAnim*>(this);
    CharAnimData* data = static_cast<CharAnimData*>(self->m_pData);

    // Random draw. Args: (namesArray, seed, picker-mask).
    void* picked = nullptr;  // TODO: gdCSCharAnimNames::FindRandAnimData(
                             //           data->m_pNameArray,
                             //           data->m_nNameCount,
                             //           data->m_nSeed);
    if (!picked) {
        self->m_bFinished = 1;
        return;
    }

    // If the action is gated (byte at data+28), resolve a concrete sub-anim
    // via pongCSActionCharAnim_CF30_wrh and (through a slot-20 predicate)
    // check that its bundle is live before promoting it into m_pActiveAnim.
    const uint8_t gated = *(reinterpret_cast<uint8_t*>(data) + 28);
    if (gated) {
        // TODO: lift pongCSActionCharAnim_CF30_wrh and slot-20 predicate.
        (void)picked;
    }

    // Install shot on the global cam-shot manager and seed scratch record
    // with identity transform + elapsed==0.0f.
    // TODO: camShotMgr_5768_g(globalCamMgr, pickedShotId);
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
    void* camMgrSlot = nullptr;  // TODO: global cam-mgr "active shot" slot
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
    // TODO: lift the "pick gated vs ungated handle" branch + camShotMgr_5768_g
    //       call. The store pattern matches CamAnim::Pause exactly.
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
    // We capture the shape as a TODO since the caller isn't lifted yet.
    uint16_t msgId = 0;  // TODO: *(uint16_t*)msgRecord
    const bool isGeneric     = (msgId == 0x1010);
    const bool isPerPlayer   = (msgId == 0x580F);
    if (!isGeneric && !isPerPlayer) {
        return;
    }
    // TODO: if (isPerPlayer && SinglesNetworkClient_GetPlayerID(true) !=
    //                        data->m_nOwnerPlayerId) return;
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
    void* camMgrSlot = nullptr;  // TODO: global cam-mgr "active shot" slot
    const bool playing = camMgrSlot ? pongCSActionCamAnim_IsCamShotPlaying(camMgrSlot)
                                    : true;
    const bool settled = camMgrSlot ? pongCSActionCamAnim_IsCamShotSettled(camMgrSlot)
                                    : true;
    if (!(playing && settled)) {
        return;
    }

    // When the cam layer reports the previous shot as both playing and
    // settled, advance to the next sub-anim and re-arm camShotMgr_5768_g.
    // TODO(liftteam): wire up the cam-mgr accessors and shot-launch helper.
    ++self->m_nSubIndex;
}


// ═════════════════════════════════════════════════════════════════════════════
// Batch 2: complete pongCSActionCamAnim and add virtual-method bodies for the
// remaining single-slot pongCSAction subclasses. Each struct here has exactly
// one or two vfn_N entries in its vtable, so the bodies are short. The
// control-flow below mirrors the pass5_final scaffolds (see /recomp/...); any
// engine-global lookups that aren't lifted yet are marked with explicit
// TODO(liftteam) comments.
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
    // TODO(liftteam): swap m_pData's raw bytes for a proper
    // gdCSActionCamAnimData* once that struct is defined; then honour the
    // enabled flag at +16 and the sub-shot count at +0x28 and re-arm the
    // cam shot via camShotMgr_5768_g.
}

// ── pongCSActionIf::Init() [slot 3 @ 0x823499f0] ──────────────────────────
// Base-class Init walks m_pChildren and issues Init() on each. ActionIf's
// override is identical in shape because the branch predicate is evaluated
// in Play()/Pause(), not here.
void pongCSActionIf::Init() {
    // TODO(liftteam): ActionIf stores its predicate at this+0x10. The Init
    // slot recursively initialises every then/else child exactly as the
    // base pongCSAction::Init does, so fall through to the standard fan-out.
    // Without a real base-class pointer (the header models ActionIf as
    // standalone) we rely on the action container holding its children
    // somewhere reachable — currently we have no way to reach them, so
    // leave the body empty with a TODO.
}

// ── pongCSActionIf::Stop() [slot 6 @ 0x82349ab0] ──────────────────────────
// Issues Stop() on whichever branch is currently selected (then or else),
// then clears the resolved-branch latch so Resume() reports "not done" on
// the next tick. The branch selector lives at this+0x18 in the real class.
void pongCSActionIf::Stop() {
    // TODO(liftteam): dispatch to m_pSelectedBranch->Stop() once the
    // ActionIf field layout is added to the header.
}

// ── pongCSActionCharAmbAnim::Stop() [slot 6 @ 0x8234ad98] ─────────────────
// Cancels any ambient (crowd) anim this action launched.
void pongCSActionCharAmbAnim::Stop() {
    // TODO(liftteam): the body calls the crowd-animation manager to drop
    // the ambient clip; no observable state lives on the action itself.
}

// ── pongCSActionCharAmbAnim::GetProgress() [slot 10 @ 0x82348868] ─────────
// Reports the fraction (0..1) of the ambient anim that has elapsed. The
// ASM forwards to the crowd-manager per-clip progress query.
void pongCSActionCharAmbAnim::GetProgress() {
    // TODO(liftteam): return the crowd-clip progress once the crowd
    // manager is lifted. Signature currently mirrors the header's
    // void-returning declaration; the real function returns a float in f1.
}

// ── pongCSActionCharVisible::Stop() [slot 6 @ 0x8234abc8] ─────────────────
// Toggles the target character's mesh visibility back to its on-enter
// state. Touches only the character's render-flags bitmask.
void pongCSActionCharVisible::Stop() {
    // TODO(liftteam): flip the "hidden" bit on m_pTarget->m_nRenderFlags
    // after the char-instance struct is lifted.
}

// ── pongCSActionLvlAnim::Stop() [slot 6 @ 0x8234ac88] ─────────────────────
// Stops a level-geometry animation (e.g. stadium banners).
void pongCSActionLvlAnim::Stop() {
    // TODO(liftteam): defer to lvlAnimMgr::Cancel once the level-anim
    // manager is lifted.
}

// ── pongCSActionLvlAmbAnim::Stop() [slot 6 @ 0x8234ad10] ──────────────────
// Ambient counterpart to LvlAnim::Stop — kills a looping stadium ambient.
void pongCSActionLvlAmbAnim::Stop() {
    // TODO(liftteam): defer to lvlAmbAnimMgr::Cancel.
}

// ── pongCSActionShowAllAmbients::Stop() [slot 6 @ 0x8234ae88] ─────────────
// Re-hides every ambient character after a cutscene that temporarily
// forced them all visible (e.g. crowd wide-shots).
void pongCSActionShowAllAmbients::Stop() {
    // TODO(liftteam): iterate the active-crowd roster and clear the
    // "force visible" bit on each entry.
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
