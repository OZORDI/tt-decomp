/**
 * spectator_states.cpp — gmLogicSpectator HSM virtual methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * gmLogicSpectator is the client-side HSM that drives viewing of a live
 * online match when the player is not one of the two active competitors.
 * It mirrors the structure of gmLogicSinglesMatch but with a trimmed-down
 * state family (19 states: stateQuit, stateMatchEnded, statePostGame,
 * statePostPointCS, stateReplay, statePostPoint, stateScoring,
 * stateReturnHit, stateAwaitReturnHit, stateServeBounce, stateServeHit,
 * stateServeToss, statePreServe, statePreGameSync, statePreGame,
 * statePreMatch, stateInit, statePreInit, stateBase) and no tournament /
 * save-dialog branches because spectators do not own the match outcome.
 *
 * Anchor strings (see match_rtti_strings.cpp):
 *   @0x8206B760 "Replay manager still isn't recording when entering
 *                gmLogicSpectator::stateServeToss"
 *   @0x8206B7C0 "gmLogicSpectator::stateReplay - replay buffer is not
 *                done recording; skipping replay"
 *   @0x8206B818 "gmLogicSpectator::stateReplay::onEnterState"
 *   @0x8206B848 "MRB- host cxn was reset or someone dropped. Forcing
 *                exit from  gmLogicSpectator::statePostPointCS()"
 *
 * Field layout (gmLogicSpectator — inferred from vfn_3 / vfn_4 / vfn_8 /
 * vfn_11 / vfn_13 / vfn_17 / vfn_20):
 *   +0x00  vtable
 *   +0x04  stateCount                  (initialised to 18 by vfn_8)
 *   +0x08  stateTable*                 (array of state-entry pointers)
 *   +0x0C  currentStateIdx             (index into stateTable)
 *   +0x18  activeState*                (see vfn_12 / vfn_13)
 *   +0x1C  g_stateCookie               (u32, =-1 after vfn_3 init)
 *   +0x20  flags[7]                    (u8 flags 32..38)
 *   +0x28  f_countdown                 (float; vfn_11 adds 84(-32163,…))
 *   +0x2C  transitionPending           (int; non-zero enables polling)
 *   +0x30  currentSubState*            (vfn_17/vfn_20 temp pointer)
 *   +0x34  replayModeFlag              (byte; gates replay buffer queries)
 *
 * Corresponding static-recomp scaffolds live at 0x8238Cxxx / 0x8238D660 /
 * 0x8238D990 / 0x8238D550 in tt-decomp_recomp.18.cpp.
 */

#include <cstdint>
#include <cstddef>

// ────────────────────────────────────────────────────────────────────────────
// Externals referenced by this translation unit
// ────────────────────────────────────────────────────────────────────────────

// Parent-class virtuals (gmLogic base class, see pong_match.cpp).
extern "C" void gmLogic_vfn_3(void* self);          // @ 0x82222a78 — base ctor
extern "C" void gmLogic_vfn_4(void* self);          // @ 0x82222b20 — base dtor

// Render/world singleton accessor byproducts (see misc layer).
extern "C" int  pg_10E0_g(void);                    // @ 0x821010e0 — ready check
extern "C" void pg_0CB8(void);                      // @ 0x82100cb8 — reset helper

// HSM sibling helper shared with stateServeBounce @ 0x8238D9A8.
extern "C" void stateServeBounce_vfn_11_D9A8_1(void* self);

// atSingleton access used by vfn_12 transition dispatcher.
extern "C" void atSingleton_CF10_g(void* ctx);      // @ 0x8238cf10

// SDA-relative globals derived from the lis r11,-32161 block at 0x8258xxxx.
//   -21720 ≙ g_render_obj_ptr  (0x825EAB28)
//   -21716 ≙ g_render_obj_ptr2 (0x825EAB2C)
// Consumers at +12/+13/+102/+144.
extern "C" uint8_t* g_render_obj_ptr;               // @ 0x825EAB28
extern "C" uint8_t* g_render_obj_ptr2;              // @ 0x825EAB2C

// f32 constant 0.0 used throughout the HSM (lis r11,-32253; -12016 off).
extern "C" const float g_fZero;                     // @ 0x825D1150

// ────────────────────────────────────────────────────────────────────────────
// Inline helpers
// ────────────────────────────────────────────────────────────────────────────

namespace {

// Field accessors — keep the layout comment above the single source of truth.
inline void*&   fActiveState   (void* s)       { return *reinterpret_cast<void**  >(reinterpret_cast<uint8_t*>(s) + 0x2C); }
inline uint8_t& fReplayModeFlag(void* s)       { return *(reinterpret_cast<uint8_t*>(s) + 0x34); }

// vtable helpers — HSM sub-state entries look like { vptr, rttiPtr, owner, stateId }.
using FnThisVoid = void(*)(void*);
using FnEnter    = void(*)(void*);
using FnLeave    = void(*)(void*);

inline void VCallSlot(void* obj, size_t slotByteOffset) {
    if (!obj) return;
    void** vt = *reinterpret_cast<void***>(obj);
    auto fn = reinterpret_cast<FnThisVoid>(vt[slotByteOffset / sizeof(void*)]);
    fn(obj);
}

} // anonymous namespace

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::ctor                                    (vtable slot 3)
//   @ 0x8238C860 — size 0x88
//
// Chains to gmLogic::vfn_3 (base ctor), then resets the render-obj singleton's
// cookie triple (+292/+296/+300 = -1), calls pg_0CB8 once for effect, and
// zero-initialises the 7-byte flag block at +32..+38 plus the countdown at
// +40 (f32 0.0) and the cookie at +28 (-1, u32).
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_3_impl(void* self)
{
    gmLogic_vfn_3(self);

    uint8_t* robj = g_render_obj_ptr2;            // lwz r3,-21716(r11)
    if (robj) {
        *reinterpret_cast<int32_t*>(robj + 292) = -1;
        *reinterpret_cast<int32_t*>(robj + 296) = -1;
        *reinterpret_cast<int32_t*>(robj + 300) = -1;
    }

    pg_0CB8();

    uint8_t* s = reinterpret_cast<uint8_t*>(self);
    *reinterpret_cast<int32_t*>(s + 0x1C) = -1;   // cookie
    s[0x20] = 0xFF;                               // primary flag = 255
    s[0x21] = 0;
    s[0x22] = 0;
    s[0x23] = 0;
    s[0x24] = 0;
    s[0x25] = 0;
    s[0x26] = 0;
    s[0x27] = 0xFF;                               // byte flag @+39 = -1
    *reinterpret_cast<float*>(s + 0x28) = g_fZero; // f32 0.0
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::dtor                                    (vtable slot 4)
//   @ 0x8238C1E0 — size 0x50
//
// If an active sub-state is attached at +44, forward vfn_2 (leave / dispose)
// on it, then chain to base class vfn_4.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_4_impl(void* self)
{
    void* sub = fActiveState(self);
    if (sub) {
        VCallSlot(sub, 8);                        // vtable slot 2 -> byte +8
    }
    gmLogic_vfn_4(self);
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::Tick                                    (vtable slot 11)
//   @ 0x8238C0E8 — size 0xB4
//
// Advances the countdown at +48 by 0.054 (the SDA f32 at -32163,84). Stores
// the new value back to +48 and mirrors it into the render singleton at +144.
// If the render-obj's +12 gating flag is set we ask pg_10E0_g whether the
// world is ready:
//   - if ready and the HSM's +56 latch is zero → set the render-obj +13 latch
//     to 0 (frozen) then return.
//   - otherwise → set +13 to 1 (running).
// When +12 is clear we always clear +13.
//
// Net effect: this is the per-frame pacing tick that decides whether the
// render thread should animate or freeze while the spectator HSM waits for
// asset streaming / net-sync.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_11_impl(void* self)
{
    uint8_t* s   = reinterpret_cast<uint8_t*>(self);
    float    dt  = 0.054f;                        // SDA f32 @ -32163,84
    float&   cd  = *reinterpret_cast<float*>(s + 48);
    cd += dt;

    uint8_t* robj = g_render_obj_ptr;
    uint8_t  gate = robj[12];
    *reinterpret_cast<float*>(robj + 144) = cd;

    if (gate == 0) {
        robj[13] = 0;
        return;
    }

    // Primary branch: gate active.
    uint8_t* robj2 = g_render_obj_ptr2;           // same struct as +144 source
    if (robj2[102] != 0) {
        robj[13] = 1;
        return;
    }

    // World not yet spun up; poll readiness.
    if (pg_10E0_g() != 0) {
        robj[13] = 1;
        return;
    }

    // Ready-to-advance: require the HSM transition latch at +56 to be nonzero
    // before we flip into running. Otherwise we freeze for this frame.
    int32_t latch = *reinterpret_cast<int32_t*>(s + 56);
    robj[13] = (latch != 0) ? 1 : 0;
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::TransitionIfReady                       (vtable slot 12)
//   @ 0x8238C1A0 — size 0x40
//
// If the +56 transition latch is nonzero AND currentStateIdx (+12) is in the
// open interval (2, 11) — i.e. an in-game rally state — tail-call the shared
// transition dispatcher with the HSM-at-+24 pointer.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_12_impl(void* self)
{
    uint8_t* s = reinterpret_cast<uint8_t*>(self);
    int32_t  latch = *reinterpret_cast<int32_t*>(s + 56);
    if (latch == 0) return;

    int32_t idx = *reinterpret_cast<int32_t*>(s + 12);
    if (idx < 2 || idx >= 11) return;

    void* dispatchArg = *reinterpret_cast<void**>(s + 24);
    atSingleton_CF10_g(dispatchArg);
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::CanAcceptStateId                        (vtable slot 13)
//   @ 0x8238C958 — size 0x94
//
// Query: "may the HSM transition into stateId `targetIdx`?"
//   - Fails if targetIdx ≥ stateCount (+4).
//   - If +56 latch is set and targetIdx == 12 (stateReplay), succeeds.
//   - Else if targetIdx == 11 (statePostPoint), succeeds.
//   - Else if currentStateIdx is in [0, 17], defer to that sub-state's
//     slot-15 handler (byte +60) for the answer.
//   - Else returns (targetIdx == 1) — the generic "can always start init".
// ────────────────────────────────────────────────────────────────────────────
extern "C" int32_t gmLogicSpectator_vfn_13_impl(void* self, int32_t targetIdx)
{
    uint8_t* s = reinterpret_cast<uint8_t*>(self);
    int32_t  stateCount = *reinterpret_cast<int32_t*>(s + 4);
    if (targetIdx >= stateCount) return 0;

    int32_t latch = *reinterpret_cast<int32_t*>(s + 56);
    if (latch != 0 && targetIdx == 12) return 1;
    if (targetIdx == 11) return 1;

    uint32_t curIdx = *reinterpret_cast<uint32_t*>(s + 12);
    if (curIdx <= 17) {
        void** table   = *reinterpret_cast<void***>(s + 8);
        void*  subState = table[curIdx];
        if (subState) {
            void** vt = *reinterpret_cast<void***>(subState);
            using Fn = int32_t(*)(void*, int32_t);
            return reinterpret_cast<Fn>(vt[15])(subState, targetIdx);
        }
    }
    return targetIdx == 1;
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::LeaveAndReleaseSubstate                 (vtable slot 17)
//   @ 0x8238C8E8 — size 0x6C
//
// Reads the current sub-state from stateTable[currentStateIdx] and dispatches
// its slot-14 "OnLeave" (byte +56). Then, if an ancillary sub-state exists at
// +44, calls its slot-3 dispose (byte +12) and clears the pointer.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_17_impl(void* self)
{
    uint8_t* s = reinterpret_cast<uint8_t*>(self);
    uint32_t idx   = *reinterpret_cast<uint32_t*>(s + 12);
    void**   table = *reinterpret_cast<void***>(s + 8);
    void*    sub   = table[idx];
    if (sub) {
        void** vt = *reinterpret_cast<void***>(sub);
        using Fn = void(*)(void*);
        reinterpret_cast<Fn>(vt[14])(sub);
    }

    void*& anc = fActiveState(self);
    if (anc) {
        void** vt = *reinterpret_cast<void***>(anc);
        using Fn = void(*)(void*);
        reinterpret_cast<Fn>(vt[3])(anc);
        anc = nullptr;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::ReleaseAncillary                        (vtable slot 20)
//   @ 0x8238D990 — size 0x18
//
// Null-out the ancillary sub-state pointer at +44 without disposing; used by
// state-transition edges that hand ownership elsewhere.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void gmLogicSpectator_vfn_20_impl(void* self)
{
    void*& anc = fActiveState(self);
    if (anc) anc = nullptr;
}

// ────────────────────────────────────────────────────────────────────────────
// gmLogicSpectator::HsmEntry_WriteHandler                   (p46 helper)
//   @ 0x8238D550 — size 0x3C
//
// Copies 16 bytes (offset +0..+15) from the source HSM descriptor in r4
// into the destination HSM entry at (r3 + 28), then tail-calls into the
// rage::hsmContext store-fixup helper pg_E6E0 (@ 0x8225E6E0) with
// r3=14376, r4=64, r5=0, r6=0.  Used internally by the HSM sub-state
// constructor path (see vfn_8) to install a state descriptor.
// ────────────────────────────────────────────────────────────────────────────
extern "C" void pg_E6E0(void* dst, void* src, int32_t a, int32_t b, int32_t c, int32_t d);

extern "C" void gmLogicSpectator_D550_p46_impl(void* self, const void* src)
{
    uint8_t* dst = reinterpret_cast<uint8_t*>(self) + 28;
    const uint32_t* s32 = reinterpret_cast<const uint32_t*>(src);
    reinterpret_cast<uint32_t*>(dst)[0] = s32[0];
    reinterpret_cast<uint32_t*>(dst)[1] = s32[1];
    reinterpret_cast<uint32_t*>(dst)[2] = s32[2];
    reinterpret_cast<uint32_t*>(dst)[3] = s32[3];

    // Tail-call with fixed argument block used for type-14376 HSM entries.
    pg_E6E0(reinterpret_cast<void*>(14376), nullptr, 64, 0, 0, 0);
}
