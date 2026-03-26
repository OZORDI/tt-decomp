/**
 * rage_math.hpp — RAGE N-Dimensional Linear Algebra Types
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Dynamically-sized math containers used by the CM system for IK,
 * animation weights, and physics constraints.
 *
 * Classes:
 *   rage::VectorNI  — N-dimensional int32 vector   [vtable @ 0x82078F48]
 *   rage::VectorND  — N-dimensional double vector   [vtable @ 0x82078F54]
 *   rage::MatrixMND — M×N double matrix (row-major) [vtable @ 0x82078F3C]
 */
#pragma once
#include <cstdint>

namespace rage {

// ── rage::VectorNI  [vtable @ 0x82078F48] ───────────────────────────
/**
 * N-dimensional integer vector. Elements are int32_t, heap-allocated.
 *
 * Layout:
 *   +0x00: vtable
 *   +0x04: m_dimension (int16_t)
 *   +0x06: (padding)
 *   +0x08: m_pData (int32_t*)
 */
struct VectorNI {
    int16_t   m_dimension;    // +0x04 — number of elements
    int16_t   _pad06;         // +0x06
    int32_t*  m_pData;        // +0x08 — heap-allocated element array

    VectorNI(int16_t dimension);             // @ 0x8241DDC8
    virtual ~VectorNI();                     // [0] @ 0x8241DD60
};

// ── rage::VectorND  [vtable @ 0x82078F54] ───────────────────────────
/**
 * N-dimensional double-precision vector. Elements are double, heap-allocated.
 *
 * Layout:
 *   +0x00: vtable
 *   +0x04: m_dimension (int16_t)
 *   +0x06: (padding)
 *   +0x08: m_pData (double*)
 */
struct VectorND {
    int16_t   m_dimension;    // +0x04 — number of elements
    int16_t   _pad06;         // +0x06
    double*   m_pData;        // +0x08 — heap-allocated element array

    VectorND(int16_t dimension);             // @ 0x8241DEF8
    virtual ~VectorND();                     // [0] @ 0x8241DE90
};

// ── rage::MatrixMND  [vtable @ 0x82078F3C] ──────────────────────────
/**
 * M×N double-precision matrix. Row-major with row pointer indirection.
 *
 * Layout:
 *   +0x00: vtable
 *   +0x04: m_rows (int16_t)
 *   +0x06: m_cols (int16_t)
 *   +0x08: m_pRowPtrs (double**) — array of M pointers, each → N doubles
 *
 * Data is stored contiguously: m_pRowPtrs[0] points to the base,
 * subsequent rows are at stride = m_cols * sizeof(double).
 */
struct MatrixMND {
    int16_t   m_rows;         // +0x04 — number of rows
    int16_t   m_cols;         // +0x06 — number of columns
    double**  m_pRowPtrs;     // +0x08 — row pointer array

    MatrixMND(int16_t rows, int16_t cols = 1);  // @ 0x8241E118
    virtual ~MatrixMND();                        // [0] @ 0x8241E0A0

    /**
     * MultiplyRow @ 0x8241DFC8 | size: 0xD8
     * Dot product of a matrix row with a vector, 4x unrolled.
     */
    void MultiplyRow(VectorND* result, int rowIdx, int lastCol,
                     const MatrixMND* source, double bias) const;
};

} // namespace rage
