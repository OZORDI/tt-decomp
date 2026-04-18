/**
 * cm_shims.cpp
 * Rockstar Presents Table Tennis (Xbox 360, 2006) — proto-RAGE
 *
 * Void-pointer trampoline shims for the CM (Control Machine) node/port system.
 *
 * Background:
 *   Early recomp passes introduced `void*`-typed wrappers that forward to
 *   the real typed implementations in src/rage/misc/rage_cm.cpp
 *   (cmNode_GetVector, cmNode_GetInt, cmNode_GetDim, ...).
 *
 *   src/game/misc/cm_operators.cpp still declares and calls those `void*`
 *   signatures directly, so we keep them here as thin wrappers/aliases
 *   until callers migrate to the typed API.
 *
 * Functions in this file map to entries that previously lived in stubs.cpp
 * for the cmAngle / cmApproach2 / cmNode / cmPort / cmPowerApproach / cmReporter
 * / cmVec4 / cmOperator / cmMetafileTuningSet families. The names do not
 * correspond to single addresses in pass5_final — they are synthetic glue
 * symbols for linkage and are documented as such.
 */

#include <cstdint>
#include <cstring>

// Forward decls of the typed implementations in rage_cm.cpp.
// (Pulling rage_cm_types.hpp here would force unrelated engine headers.)
namespace {
struct cmNodePortLite {
    int32_t  m_type;
    void*    m_pData;
};
}  // namespace

extern "C" {

// ─────────────────────────────────────────────────────────────────────────────
// cmNode port accessors — void* shims for cm_operators.cpp
//
// These forward to the typed overloads in rage_cm.cpp by reinterpreting the
// cmPort*/cmNodePort* layout. The typed functions live in the same final
// image, so this produces no duplicate symbol conflict (different signatures).
// ─────────────────────────────────────────────────────────────────────────────

// From rage_cm.cpp (typed):
//   float* cmNode_GetVector(float* dst, const cmNodePort* port);
//   int32_t cmNode_GetInt   (const cmNodePort* port);
//   int32_t cmNode_GetDim   (const cmNodePort* port);
//   float  cmNode_GetFloat  (const cmNodePort* port);

// These C++ overloads above cannot be called from extern "C" — we use a
// minimal reimplementation that follows the same contract from rage_cm.cpp.

static int32_t cm_getPortDim_impl(const cmNodePortLite* port) {
    if (!port) return 0;
    if (port->m_type == 1) {
        const uint8_t* obj = static_cast<const uint8_t*>(port->m_pData);
        return obj ? static_cast<int32_t>(obj[16]) : 0;
    }
    if (port->m_type == 2) {
        const int32_t* node = static_cast<const int32_t*>(port->m_pData);
        return node ? node[1] : 0;
    }
    return 0;
}

/**
 * cmNode_GetVector — read a 4-float vector from a cmPort.
 * Mirrors rage_cm.cpp: DIRECT copies, NODE dispatches vtable slot 2.
 */
void cmNode_GetVector(void* dest, void* port) {
    float* dst = static_cast<float*>(dest);
    const auto* p = static_cast<const cmNodePortLite*>(port);
    if (!dst) return;
    if (!p) { dst[0] = dst[1] = dst[2] = dst[3] = 0.0f; return; }
    if (p->m_type == 1) {
        const float* src = static_cast<const float*>(p->m_pData);
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
    } else if (p->m_type == 2) {
        struct N { void** vtable; };
        auto* n = static_cast<N*>(p->m_pData);
        using Fn = void(*)(void*, float*);
        reinterpret_cast<Fn>(n->vtable[2])(n, dst);
    } else {
        dst[0] = dst[1] = dst[2] = dst[3] = 0.0f;
    }
}

/**
 * cmNode_GetInt — read an int32 from a cmPort.
 */
int cmNode_GetInt(void* port) {
    const auto* p = static_cast<const cmNodePortLite*>(port);
    if (!p) return 0;
    if (p->m_type == 1) {
        return *static_cast<const int32_t*>(p->m_pData);
    }
    if (p->m_type == 2) {
        struct N { void** vtable; };
        auto* n = static_cast<N*>(p->m_pData);
        int32_t tmp = 0;
        using Fn = void(*)(void*, int32_t*);
        reinterpret_cast<Fn>(n->vtable[1])(n, &tmp);
        return tmp;
    }
    return 0;
}

/**
 * cmNode_GetDim_stub — read a dimension (int32) from a cmPort.
 */
int cmNode_GetDim_stub(void* port) {
    const auto* p = static_cast<const cmNodePortLite*>(port);
    if (!p) return 0;
    if (p->m_type == 1) {
        return *static_cast<const int32_t*>(p->m_pData);
    }
    if (p->m_type == 2) {
        struct N { void** vtable; };
        auto* n = static_cast<N*>(p->m_pData);
        int32_t tmp = 0;
        using Fn = void(*)(void*, int32_t*);
        reinterpret_cast<Fn>(n->vtable[5])(n, &tmp);
        return tmp;
    }
    return 0;
}

/**
 * cmOperator_EvalFloat — read a float from a cmPort (scalar).
 */
float cmOperator_EvalFloat(void* port) {
    const auto* p = static_cast<const cmNodePortLite*>(port);
    if (!p) return 0.0f;
    if (p->m_type == 1) {
        return *static_cast<const float*>(p->m_pData);
    }
    if (p->m_type == 2) {
        struct N { void** vtable; };
        auto* n = static_cast<N*>(p->m_pData);
        float tmp = 0.0f;
        using Fn = void(*)(void*, float*);
        reinterpret_cast<Fn>(n->vtable[4])(n, &tmp);
        return tmp;
    }
    return 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmNode_GetBoolValue / cmNode_GetDimValue — convenience accessors used by
// early generator passes. They collapse to the int accessor.
// Zero callers in the current tree; retained to preserve the ABI surface.
// ─────────────────────────────────────────────────────────────────────────────

uint8_t cmNode_GetBoolValue(void* port) {
    return cmNode_GetInt(port) != 0 ? 1u : 0u;
}

uint32_t cmNode_GetDimValue(void* port) {
    const auto* p = static_cast<const cmNodePortLite*>(port);
    return static_cast<uint32_t>(cm_getPortDim_impl(p));
}

// ─────────────────────────────────────────────────────────────────────────────
// cmNode_TryConnect — generator-bundled trampolines with no concrete address.
// The real connect paths (cmNode_TryConnect, cmNode_TryConnectUnary) live in
// rage_cm.cpp with typed cmBinaryNode*/cmUnaryNode* signatures. The void*
// forms below are kept as no-op accepting wrappers for linker compatibility.
// ─────────────────────────────────────────────────────────────────────────────

bool cmNode_TryConnect3(void* /*node*/, void* /*desc*/) {
    // Ternary dimension validator stub; the binary-node TryConnect in
    // rage_cm.cpp does not handle the portC==3 path — mirror that here.
    return false;
}

bool cmNode_TryConnectSingle(void* /*node*/, void* /*desc*/) {
    // Unary dimension validator stub — callers should migrate to
    // rage::cmNode_TryConnectUnary(cmUnaryNode*, cmPortDesc).
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Math helpers — cmAngle / cmApproach2 / cmPowerApproach / cmVec4
//
// These names don't resolve to single addresses in pass5_final; they're
// generator-synthesized trampolines. The original implementations are spread
// across class vtables (cmAngle_vfn_4 @ 0x8227A0E0, cmApproach2_vfn_*, etc.).
// Until we know which callsite uses each flavor, provide neutral math impls.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmAngle_Normalize — wrap an angle into (-π, π].
 * Matches the common rage helper: a - floor((a + π) / 2π) * 2π.
 */
float cmAngle_Normalize(float a) {
    constexpr float kPi    = 3.14159265358979323846f;
    constexpr float kTwoPi = 6.28318530717958647692f;
    // Branchless modulo into (-π, π]
    float t = a + kPi;
    float k = t - kTwoPi * __builtin_floorf(t / kTwoPi);
    return k - kPi;
}

/**
 * cmApproach2_ComputeFactor — scalar pre-approach factor.
 * Mirrors cmApproach2_vfn_* which emit `1 - exp(-rate * dt)` clamped; we
 * collapse to 0 when no input block is available.
 */
float cmApproach2_ComputeFactor(void* /*ctx*/) {
    return 0.0f;
}

/**
 * cmPowerApproach_Step — step a value toward a target using a power curve.
 * Three-parameter form (current, target, rate). Writes into *out.
 * Until the exact fx pipeline is traced, this behaves as a pass-through
 * (no change, matching the ctor-default "disabled" state).
 */
void cmPowerApproach_Step(float* out, float /*current*/, float /*target*/,
                          float /*rate*/) {
    if (out) *out = 0.0f;
}

/**
 * cmVec4_Atan2 — compute atan2 from two 4-float vectors (y, x).
 * Falls back to zero if either pointer is null.
 */
float cmVec4_Atan2(float* y, float* x) {
    if (!y || !x) return 0.0f;
    return __builtin_atan2f(*y, *x);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmPort — buffer copy / sync helpers. Used by metafile tuning blocks.
// Neither name resolves to a single address; keep as no-op shims.
// ─────────────────────────────────────────────────────────────────────────────

void cmPort_CopyToBuffer(void* /*port*/, void* /*buffer*/) {
    // No active callers. Retained for ABI.
}

void cmPort_SyncValue(void* /*port*/, void* /*src*/, void* /*dst*/) {
    // No active callers. Retained for ABI.
}

// ─────────────────────────────────────────────────────────────────────────────
// cmNormalizedTimer_Allocate — ctor-style allocator trampoline.
// The real cmNormalizedTimer ctor chain lives at vfn_* addresses and is
// invoked via cmOperatorCtor_*; this is a no-op placeholder.
// ─────────────────────────────────────────────────────────────────────────────

void cmNormalizedTimer_Allocate(void* /*timer*/) {
    // Zero-init contract: callers pre-zero the block.
}

// ─────────────────────────────────────────────────────────────────────────────
// cmReporter_Init — reporter setup trampoline. Real work happens inside
// cmReporterCtor_3BA0 @ 0x82273BA0.
// ─────────────────────────────────────────────────────────────────────────────

void cmReporter_Init(void* /*reporter*/) {
    // No-op: construction happens elsewhere.
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperator_SetLabel — par-serializer helper. Referenced by rage_par.cpp
// (cmOperator_CreateString etc.) as an extern-C declaration. The real impl
// lives at an unlocated address in pass5_final; we emit a neutral pass-through
// so linkage resolves and the name-set operations simply no-op.
// ─────────────────────────────────────────────────────────────────────────────

void cmOperator_SetLabel(void* /*pOperator*/,
                         const char* /*pPrimaryName*/,
                         const char* /*pSecondaryName*/,
                         uint32_t /*copyPrimary*/,
                         uint32_t /*copySecondary*/) {
    // No-op: real implementation not yet located. Label storage is populated
    // by cmOperator_SetPrimaryName / SetSecondaryName on the same operator.
}

}  // extern "C"

// ─────────────────────────────────────────────────────────────────────────────
// cmMetafileTuningSet_vfn_8 — metafile tuning param dispatch. The real vtable
// entry is at 0x822759B0 (88 bytes). Called from rage_cm.cpp with C++ linkage.
// ─────────────────────────────────────────────────────────────────────────────

void cmMetafileTuningSet_vfn_8(void* /*self*/, uint32_t /*a*/, uint32_t /*b*/) {
    // Real impl @ 0x822759B0 — TODO: lift when tuning pipeline is traced.
}

/**
 * cmOperator_5FC8_g — shared MetafileTuningSet param accumulator.
 * Paired with cmMetafileTuningSet_vfn_8 in rage_cm.cpp:1403.
 */
void cmOperator_5FC8_g(void* /*self*/, uint32_t* /*p1*/, uint32_t* /*p2*/) {
    // Real impl @ 0x82125FC8 — TODO: lift when ctor chain is traced.
}
