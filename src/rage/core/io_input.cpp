/**
 * io_input.cpp — io / io_Input polling primitives (proto-RAGE)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Thin input-state gate used by the per-frame player/swing pipelines.
 * Each call inspects a couple of boolean fields on the input singleton
 * and returns a single bit indicating whether the caller should act.
 *
 * Real-implementation lifts:
 *   io_Input_poll_9D68   @ 0x821C9D68 — compound-predicate poll
 *   io_ExecuteStateAction (aka io_9E30) @ 0x822F9E30 — SDA-gated wrapper
 *
 * Link-only shim:
 *   io_Input_poll        — parameterless callsite stub used by older shims.
 */

#include <cstdint>

// Forward-declare the opaque `io` handle type — the real declaration lives
// in src/game/misc/pong_misc.hpp but the full type is unused here (the
// `self` parameter is carried only for ABI compatibility with existing
// callers in pong_misc.cpp and the drill/spectator pipelines).
struct io;

// ─── External SDA resolvers ───────────────────────────────────────────────────
//
// io_9E30 loads its input-object pointer from the SDA (sda_r11 + -21720);
// that slot holds a pointer to the active input singleton. The game
// maintains two such pointers for split-screen / spectator contexts.
//
// We reuse the already-exported `g_input_obj_ptr` symbol from pong_misc —
// it aliases the same SDA cell.

extern "C" void* g_input_obj_ptr;

// Additional SDA-proximate scratch (sda_r11 + -21712 → +576/+578 bytes of
// boolean state). The original binary bundles both bytes into a single
// struct so we mirror that here.
namespace {
struct IoInputGate {
    uint8_t _pad0[576];     // +0x000
    uint8_t suspendFlag;    // +0x240 (576) — nonzero suppresses polling
    uint8_t _pad1[1];
    uint8_t readyFlag;      // +0x242 (578) — nonzero enables deep poll
};
// File-scope placeholder — the real SDA-backed gate pointer will be wired
// in when the globals pool lift lands. Callers hit the null guard below
// and return the suppressed-state result (1) until then.
IoInputGate* g_io_input_gate = nullptr;
}  // namespace

// ─── io_Input_poll_9D68 @ 0x821C9D68 ──────────────────────────────────────────
//
// Two-phase predicate:
//   Phase A: if (obj + 334) != 0  OR  (obj + 340) != 0  → continue.
//   Phase B: if (obj + 332) != 0                         → return 1.
// Otherwise returns 0.
//
// This matches the original's control flow: bytes 332/334 are the
// "pressed" and "held" single-bit gates; word 340 is the repeat counter.
int io_Input_poll_9D68(void* inputObj) {
    if (!inputObj) {
        return 0;
    }

    auto*    obj  = reinterpret_cast<uint8_t*>(inputObj);
    uint8_t  held = obj[334];
    uint32_t rep  = *reinterpret_cast<uint32_t*>(obj + 340);

    bool phaseA = (held != 0) || (rep != 0);
    if (!phaseA) {
        return 0;
    }

    uint8_t pressed = obj[332];
    return pressed ? 1 : 0;
}

// ─── io_ExecuteStateAction (io_9E30) @ 0x822F9E30 ─────────────────────────────
//
// Wrapper around io_Input_poll_9D68 gated by two SDA bytes:
//   suspendFlag  (nonzero) → immediately return 1 (suppressed; upstream
//                             treats this as "consumed").
//   readyFlag    (nonzero) → drill down into io_Input_poll_9D68 against
//                             the global input pointer; non-zero means
//                             "act".
//   neither      → return 0.
//
// The io-shaped `self` parameter is unused by the original body — the
// gate is entirely global. We keep the signature for ABI compatibility
// with existing callers.
uint8_t io_ExecuteStateAction(io* self) {
    (void)self;

    IoInputGate* gate = g_io_input_gate;
    if (!gate) {
        // Matches the SDA-null branch: the hardware gate hasn't been bound
        // yet, so treat the action as suppressed.
        return 1;
    }

    if (gate->suspendFlag != 0) {
        return 1;
    }

    if (gate->readyFlag == 0) {
        return 0;
    }

    int pollResult = io_Input_poll_9D68(g_input_obj_ptr);
    return pollResult ? 0 : 1;
}

// ─── Parameterless io_Input_poll (link-only shim) ─────────────────────────────
//
// A handful of legacy callers (largely in the spectator and drill
// pipelines) still reference the raw no-argument entry point; the
// original binary dispatches through the gate above, so we tie the two
// together here.

extern "C" void io_Input_poll(void) {
    (void)io_ExecuteStateAction(nullptr);
}
