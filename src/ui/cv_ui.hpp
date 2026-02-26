/**
 * cv_ui.hpp — NURBS Curve class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * "cv" = Control Vertex — these classes implement NURBS (Non-Uniform Rational
 * B-Spline) curve evaluation used by the game's animation and camera systems.
 *
 * Hierarchy:
 *   cvCurve              — abstract base, owns knot vector + control points
 *   cvCurveNurbs         — NURBS specialisation of cvCurve (no new virtuals)
 *   cvCurveNurbsWM       — NURBS "With Manipulation": weighted evaluation,
 *                          tangent computation, and full B-spline solve
 *
 * Multiple vtables per class (4–5) indicate virtual-base or template family
 * instantiation — the standard rage engine MI pattern.
 *
 * Sources: rtti_vtable_map | vtable_layout_map
 *          | structured_pass5_final recomp (recomp.9.cpp)
 */
#pragma once
#include <stdint.h>

// ─────────────────────────────────────────────────────────────────────────────
// cvCurve  [5 vtables @ 0x8204ED54, 0x8204EDA4, 0x8204EE90, 0x8204EEE0, 0x82058A84]
// ─────────────────────────────────────────────────────────────────────────────
// Field layout inferred from cvCurveNurbsWM recomp (pass5_final recomp.9.cpp):
//   +0x08  m_pControlPoints  — float[n][30] control point array (120 bytes/entry)
//   +0x10  m_nControlPoints  — uint32 number of control points
//   +0x18  m_pKnotVector     — float* knot value array
//   +0x3C  m_pKnotSpanData   — void* auxiliary span data
//   +0x58  (unidentified pointer)
class cvCurve {
public:
    virtual ~cvCurve();           // [0] @ 0x82230460
    virtual void ScalarDtor();    // [1] @ 0x8222ECE8  (scalar delete thunk)
};

// ─────────────────────────────────────────────────────────────────────────────
// cvCurveNurbs  [4 vtables @ 0x8204ED2C, 0x8204ED7C, 0x8204EE68, 0x8204EEB8]
// Inherits from cvCurve (inferred from naming and address adjacency).
// No additional virtual overrides found in vtable_layout_map.
// ─────────────────────────────────────────────────────────────────────────────
class cvCurveNurbs : public cvCurve {
public:
};

// ─────────────────────────────────────────────────────────────────────────────
// cvCurveNurbsWM  [4 vtables @ 0x8204ECDC, 0x8204ED04, 0x8204EE18, 0x8204EE40]
// Inherits from cvCurveNurbs (inferred).
// "WM" = "With Manipulation" — adds tangent evaluation and B-spline solve.
//
// Additional fields (appended after cvCurve layout):
//   +0x1C  m_basisA   — float  first blended basis weight  (evaluation scratch)
//   +0x20  m_basisB   — float  second blended basis weight
//   +0x24  m_basisC   — float  third blended basis weight
//   +0x28  m_basisD   — float  fourth blended basis weight
//   +0x2C  m_tangentX — float  output tangent X component
//   +0x30  m_tangentY — float  output tangent Y component
//   +0x34  m_tangentZ — float  output tangent Z component
//   +0x38  m_tangentW — float  output tangent W (homogeneous weight)
//   +0x40  m_tFinal   — float  final clamped/normalised parameter value
// ─────────────────────────────────────────────────────────────────────────────
class cvCurveNurbsWM : public cvCurveNurbs {
public:
    virtual ~cvCurveNurbsWM();    // [0] @ 0x8222EC48
    virtual void ScalarDtor();    // [1] @ 0x8222EE48  (scalar delete thunk)

    /**
     * Evaluate @ [2] 0x8222F058
     * De Boor B-spline evaluation at parameter t.  Locates the knot span,
     * blends the surrounding control points, and writes position into pOut.
     * Also computes tangent if pTangentOut != nullptr.
     * Calls helpers cvCurveNurbsWM_25F8 / _26F0 / _1E00 / _1EA0.
     */
    virtual void Evaluate(void* pOut, float t, void* pTangentOut);  // [2]

    // TODO: vfn_3 @ 0x8222F390 — purpose unclear from pseudocode; likely
    //       a derivative or arc-length parameterisation helper.
    virtual void vfn_3();         // [3] @ 0x8222F390

    // TODO: vfn_4 @ 0x8222F640 — possibly second derivative or fit query.
    virtual void vfn_4();         // [4] @ 0x8222F640

    // TODO: vfn_5 @ 0x82236B88 — possibly Solve/Fit from scattered points.
    virtual void vfn_5();         // [5] @ 0x82236B88

    // slot 6 not overridden in this class (base provides it or it is pure).

    // TODO: vfn_7 @ 0x8222F8A0 — purpose unknown; no pseudocode available.
    virtual void vfn_7();         // [7] @ 0x8222F8A0
};

