/**
 * cv_ui.cpp — NURBS Curve Implementation (cvCurve / cvCurveNurbsWM)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * "cv" = Control Vertex — implements NURBS (Non-Uniform Rational B-Spline)
 * curve evaluation used by the animation and camera systems.
 *
 * Functions implemented:
 *   cvCurve:
 *     - Destructor (vfn_0)          — frees control points + self
 *     - ScalarDtor (vfn_1)          — destructor with scalar-delete pattern
 *     - RelocatePointers (ctor_0AE0) — resource relocation for datResource loading
 *   cvCurveNurbsWM:
 *     - Destructor (vfn_0)          — derived destructor
 *     - EvaluateAtZero (EBE0_w)     — evaluate at t=0 convenience wrapper
 *     - AccumulateParameter (vfn_5) — advance curve parameter with clamping/wrapping
 *     - Rebuild (vfn_7)             — tail-call to knot vector rebuild
 */

#include "cv_ui.hpp"
#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// RAGE heap deallocator.
// @ 0x820C00C0
extern "C" void rage_free(void* ptr);

// Knot vector rebuild — recomputes the internal B-spline knot data after
// control point modifications.  4292 bytes — the core NURBS setup function.
// @ 0x82230D38 | size: 0x10C4
extern void cvCurve_RebuildKnotVector(void* curve);

// ─────────────────────────────────────────────────────────────────────────────
// Vtable pointers (from .rdata)
// ─────────────────────────────────────────────────────────────────────────────

// cvCurve primary vtable
// @ 0x8204ED54
extern void* g_cvCurve_vtable[];

// cvCurveNurbs primary vtable (set during relocation as second phase)
// @ 0x8204EDA4
extern void* g_cvCurveNurbs_vtable[];

// cvCurveNurbsWM vtable
// @ 0x8204ECDC
extern void* g_cvCurveNurbsWM_vtable[];

// ─────────────────────────────────────────────────────────────────────────────
// Float constants (from .rdata)
// ─────────────────────────────────────────────────────────────────────────────

// Epsilon threshold for arc-length comparison (small positive float).
// @ 0x82079B10
extern float g_cvCurve_arcLengthEpsilon;

// Default parameter step — used when arcLength <= epsilon.
// @ 0x82078078
extern float g_cvCurve_defaultStep;

// Maximum parameter value — upper clamp bound for AccumulateParameter.
// @ 0x82079B08
extern float g_cvCurve_maxParam;

// Zero constant (0.0f).
// @ 0x82079B10 (same as epsilon — reused in clamp logic)


// ═══════════════════════════════════════════════════════════════════════════════
//  cvCurve — Base Class
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cvCurve::~cvCurve  [vtable slot 0]
 * @ 0x82230460 | size: 0x6C
 *
 * Destructor.  Sets vtable to cvCurve base vtable.  If the owned-flag
 * at +14 is non-zero, frees the control point array at +8.
 * If flags & 1, frees self (scalar-delete pattern).
 *
 * Layout used:
 *   +0x00  vtable pointer
 *   +0x08  m_pControlPoints (float*) — control point data
 *   +0x0E  m_bOwnsControlPoints (uint16) — ownership flag
 */
cvCurve::~cvCurve()
{
    // Set vtable to cvCurve base
    *reinterpret_cast<void**>(this) = g_cvCurve_vtable;

    // Free control point array if owned
    uint16_t ownsCP = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<char*>(this) + 14);
    if (ownsCP != 0) {
        void* controlPoints = *reinterpret_cast<void**>(
            reinterpret_cast<char*>(this) + 8);
        rage_free(controlPoints);
    }
}

/**
 * cvCurve::RelocatePointers (resource relocation)
 * @ 0x82230AE0 | size: 0x258
 *
 * Called during datResource loading to relocate serialized pointers.
 * Uses the datResourceMap context (r4) where:
 *   +4    = base address of the resource in memory
 *   +76   = segment size/stride for relocation table lookup
 *
 * Relocates pointer fields at offsets:
 *   +8 (control points), +20 (knot vector), +24 (knot span data),
 *   +60, +64, +68, +72, +76, +80, +84, +88 (various auxiliary data)
 *
 * Sets two vtables during relocation:
 *   First phase:  cvCurve vtable (0x8204ED54)
 *   Second phase: cvCurveNurbs vtable (0x8204EDA4 — 40 bytes later)
 *
 * This is a datResource::Place-style function for virtual-base MI objects.
 */
void cvCurve_RelocatePointers(cvCurve* self, void* resourceMap)
{
    char* obj = reinterpret_cast<char*>(self);
    char* resMap = reinterpret_cast<char*>(resourceMap);

    // Set first vtable (cvCurve)
    *reinterpret_cast<void**>(obj) = g_cvCurve_vtable;

    // Helper: for each pointer field, if non-null, compute the relocated
    // address using the resource map's base and segment table.
    // relocPtr = segmentTable[((ptr - base) / stride) + 2] + ptr
    //
    // The offsets relocated are: 8, 20, 24, 60, 64, 68, 72, 76, 80, 84, 88
    static const int offsets[] = {8, 20, 24, 60, 64, 68, 72, 76, 80, 84, 88};

    uint32_t base = *reinterpret_cast<uint32_t*>(resMap + 4);
    uint32_t stride = *reinterpret_cast<uint32_t*>(resMap + 76);

    for (int i = 0; i < 11; i++) {
        int off = offsets[i];
        uint32_t ptr = *reinterpret_cast<uint32_t*>(obj + off);

        if (ptr != 0) {
            uint32_t relIdx = (ptr - base) / stride + 2;
            uint32_t segBase = *reinterpret_cast<uint32_t*>(
                resMap + relIdx * 4);
            *reinterpret_cast<uint32_t*>(obj + off) = segBase + ptr;
        }

        // After relocating offset +8, set the second vtable (cvCurveNurbs)
        if (i == 0) {
            *reinterpret_cast<void**>(obj) = g_cvCurveNurbs_vtable;
        }
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  cvCurveNurbsWM — "With Manipulation" NURBS Specialization
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cvCurveNurbsWM::~cvCurveNurbsWM  [vtable slot 0]
 * @ 0x8222EC48 | size: 0x50
 *
 * Derived destructor — chains to cvCurve base destructor.
 */
cvCurveNurbsWM::~cvCurveNurbsWM()
{
    // Base class destructor called automatically by C++
}

/**
 * cvCurveNurbsWM::EvaluateAtZero
 * @ 0x8222EBE0 | size: 0x64
 *
 * Convenience wrapper: zeroes a 120-byte (30 × float) output buffer,
 * then evaluates the curve at parameter t = 0.0 with no tangent output.
 *
 * @param outBuffer   Result buffer (120 bytes = one control point worth of data)
 * @return this
 */
cvCurveNurbsWM* cvCurveNurbsWM::EvaluateAtZero(void* outBuffer)
{
    // Zero 30 uint32s (120 bytes) — one control point entry
    uint32_t* dst = static_cast<uint32_t*>(outBuffer);
    for (int i = 0; i < 30; i++) {
        dst[i] = 0;
    }

    // Evaluate at t=0.0 with no tangent output
    this->Evaluate(outBuffer, 0.0f, nullptr);
    return this;
}

/**
 * cvCurveNurbsWM::AccumulateParameter  [vtable slot 5]
 * @ 0x82236B88 | size: 0x68
 *
 * Advances the curve parameter accumulator.
 *
 * If arcLength > epsilon:
 *   *pAccum += (delta / arcLength)
 * Else:
 *   *pAccum += defaultStep (constant from .rdata)
 *
 * After accumulation, if *pAccum > maxParam:
 *   If m_bWrapAround (+5): clamp to 0.0
 *   Else: clamp to maxParam
 */
void cvCurveNurbsWM::AccumulateParameter(float delta, float arcLength,
                                          float* pAccum)
{
    if (arcLength > g_cvCurve_arcLengthEpsilon) {
        // Normal case: advance by normalized delta
        *pAccum += delta / arcLength;
    } else {
        // Fallback: advance by constant default step
        *pAccum += g_cvCurve_defaultStep;
    }

    // Clamp check
    if (*pAccum > g_cvCurve_maxParam) {
        uint8_t wrapAround = *reinterpret_cast<uint8_t*>(
            reinterpret_cast<char*>(this) + 5);
        if (wrapAround != 0) {
            *pAccum = 0.0f;  // Wrap to start
        } else {
            *pAccum = g_cvCurve_maxParam;  // Clamp to end
        }
    }
}

/**
 * cvCurveNurbsWM::Rebuild  [vtable slot 7]
 * @ 0x8222F8A0 | size: 0x04
 *
 * Tail-calls cvCurve_RebuildKnotVector to recompute the internal
 * B-spline knot data after control point modifications.
 */
void cvCurveNurbsWM::Rebuild()
{
    cvCurve_RebuildKnotVector(this);
}
