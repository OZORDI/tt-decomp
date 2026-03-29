/**
 * cv_ui.hpp — NURBS Curve Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * "cv" = Control Vertex — implements NURBS (Non-Uniform Rational B-Spline)
 * curve evaluation used by the animation and camera systems.
 *
 * Hierarchy:
 *   cvCurve          — abstract base, owns knot vector + control points
 *   cvCurveNurbs     — NURBS specialization (no new virtuals)
 *   cvCurveNurbsWM   — "With Manipulation": weighted evaluation,
 *                       tangent computation, parameter accumulation
 *
 * Multiple vtables per class (4-5) indicate virtual-base or template
 * family instantiation — standard RAGE engine MI pattern.
 */
#pragma once
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// cvCurve  [5 vtables @ 0x8204ED54, 0x8204EDA4, 0x8204EE90, 0x8204EEE0, 0x82058A84]
// ─────────────────────────────────────────────────────────────────────────────
//
// Field layout (from cvCurveNurbsWM scaffold analysis):
//   +0x05  m_bWrapAround     — uint8, if set: clamp parameter to 0.0 on overflow
//   +0x08  m_pControlPoints  — float[n][30] control point array (120 bytes/entry)
//   +0x10  m_nControlPoints  — uint32, number of control points
//   +0x18  m_pKnotVector     — float* knot value array
//   +0x3C  m_pKnotSpanData   — void* auxiliary span data
//   +0x58  (unidentified pointer)
class cvCurve {
public:
    virtual ~cvCurve();                   // [0] @ 0x82230460
    virtual void ScalarDtor();            // [1] @ 0x8222ECE8
};

// Resource relocation constructor — relocates serialized pointers
// during datResource loading.
void cvCurve_RelocatePointers(cvCurve* self, void* resourceMap);

// ─────────────────────────────────────────────────────────────────────────────
// cvCurveNurbs  [4 vtables @ 0x8204ED2C, 0x8204ED7C, 0x8204EE68, 0x8204EEB8]
// ─────────────────────────────────────────────────────────────────────────────
class cvCurveNurbs : public cvCurve {
};

// ─────────────────────────────────────────────────────────────────────────────
// cvCurveNurbsWM  [4 vtables @ 0x8204ECDC, 0x8204ED04, 0x8204EE18, 0x8204EE40]
// "With Manipulation" — weighted NURBS evaluation + tangent + parameter control
//
// Additional fields (appended after cvCurve layout):
//   +0x1C  m_basisA    — float, first blended basis weight (scratch)
//   +0x20  m_basisB    — float, second blended basis weight
//   +0x24  m_basisC    — float, third blended basis weight
//   +0x28  m_basisD    — float, fourth blended basis weight
//   +0x2C  m_tangentX  — float, output tangent X
//   +0x30  m_tangentY  — float, output tangent Y
//   +0x34  m_tangentZ  — float, output tangent Z
//   +0x38  m_tangentW  — float, output tangent W (homogeneous)
//   +0x40  m_tFinal    — float, final clamped parameter value
// ─────────────────────────────────────────────────────────────────────────────
class cvCurveNurbsWM : public cvCurveNurbs {
public:
    virtual ~cvCurveNurbsWM();            // [0] @ 0x8222EC48
    virtual void ScalarDtor();            // [1] @ 0x8222EE48

    // Convenience: zeroes output, evaluates at t=0
    cvCurveNurbsWM* EvaluateAtZero(void* outBuffer);  // @ 0x8222EBE0

    /**
     * Evaluate @ 0x8222F058 | size: 0x338  [vtable slot 2]
     *
     * De Boor B-spline evaluation at parameter t. Locates the knot span,
     * blends surrounding control points, writes position into pOut.
     * Computes tangent if pTangentOut != nullptr.
     */
    virtual void Evaluate(void* pOut, float t, void* pTangentOut);

    /**
     * SetupRenderState @ 0x8222F390 | size: 0x2AC  [vtable slot 3]
     *
     * Configures GPU render state flags (blend mode, alpha test, depth
     * write, cull mode, etc.) based on the current render pass index.
     * Despite living in the cvCurve vtable, this is render pipeline
     * configuration — likely a virtual-base slot inherited from a
     * render-state mixin class.
     */
    virtual void SetupRenderState();

    /**
     * UpdateRenderState @ 0x8222F640 | size: 0x260  [vtable slot 4]
     *
     * Secondary render state update — applies pass-specific overrides
     * for texture stages, sampler state, and shader constants.
     */
    virtual void UpdateRenderState();

    /**
     * AccumulateParameter @ 0x82236B88 | size: 0x68  [vtable slot 5]
     *
     * Advances the curve parameter: if arcLength > epsilon, adds
     * (delta / arcLength) to the accumulator; otherwise adds a default
     * step. Clamps result to [0, maxParam], wrapping if m_bWrapAround.
     *
     * @param delta       Arc-length delta to advance
     * @param arcLength   Total arc length for normalization
     * @param pAccum      Pointer to float accumulator (read/write)
     */
    virtual void AccumulateParameter(float delta, float arcLength, float* pAccum);

    // Slot 6: not overridden in this class

    /**
     * Rebuild @ 0x8222F8A0 | size: 0x4  [vtable slot 7]
     *
     * Tail-calls the knot vector rebuild function. Recomputes internal
     * B-spline data after control point modifications.
     */
    virtual void Rebuild();
};
