/**
 * rage_math.cpp — RAGE N-Dimensional Linear Algebra Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements rage::VectorNI (integer vector), rage::VectorND (double vector),
 * and rage::MatrixMND (M×N double matrix) — general-purpose linear algebra
 * types used by the CM (Creature Movement) system for IK solving, animation
 * blending weights, and physics constraint computation.
 *
 * These are dynamically-sized containers: dimension is stored as int16 at +4,
 * data is heap-allocated via the RAGE allocator at +8.
 */

#include "rage_math.hpp"
#include <cstring>

extern "C" {
    void  sysMemAllocator_InitMainThread(void);  // @ 0x820C0038
    void  rage_free(void* ptr);                  // @ 0x820C00C0
}

// Forward: RAGE heap allocator vtable slot 1 (Allocate)
static void* rage_HeapAlloc(uint32_t size, uint32_t alignment) {
    sysMemAllocator_InitMainThread();
    extern void* g_pAllocatorBase;
    void** base = (void**)g_pAllocatorBase;
    void* allocator = base[1];
    if (!allocator) return nullptr;

    // Call vtable slot 1: Allocate(size, alignment)
    typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
    void** vtable = *(void***)allocator;
    return ((AllocFunc)vtable[1])(allocator, size, alignment);
}

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// VectorNI — N-dimensional integer vector
// ─────────────────────────────────────────────────────────────────────────────

/**
 * VectorNI::VectorNI @ 0x8241DDC8 | size: 0xC8
 *
 * Constructs an N-dimensional integer vector. Allocates dimension * 4 bytes
 * from the RAGE heap (16-byte aligned) and zeroes all elements.
 *
 * @param dimension  Number of elements (int16)
 */
VectorNI::VectorNI(int16_t dimension) {
    m_dimension = dimension;
    m_pData = nullptr;

    // Compute allocation size: dimension * sizeof(int32_t)
    int32_t dim = (int32_t)dimension;
    uint32_t allocSize = (dim << 2) & 0xFFFFFFFC;  // dim * 4, aligned

    // Overflow check: if dim > 0x3FFFFFFF, set allocSize to -1 (will fail alloc)
    if ((uint32_t)dim > 0x3FFFFFFF) {
        allocSize = 0xFFFFFFFF;
    }

    m_pData = (int32_t*)rage_HeapAlloc(allocSize, 16);

    // Zero-initialize all elements
    if (dim > 0) {
        for (int16_t i = 0; i < m_dimension; i++) {
            m_pData[i] = 0;
        }
    }
}

/**
 * VectorNI::~VectorNI @ 0x8241DD60 | size: 0x68
 *
 * Destructor — frees the data array if allocated.
 */
VectorNI::~VectorNI() {
    if (m_pData) {
        rage_free(m_pData);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// VectorND — N-dimensional double vector
// ─────────────────────────────────────────────────────────────────────────────

/**
 * VectorND::VectorND @ 0x8241DEF8 | size: 0xCC
 *
 * Constructs an N-dimensional double-precision vector. Allocates
 * dimension * 8 bytes from the RAGE heap and zeroes all elements.
 *
 * @param dimension  Number of elements (int16)
 */
VectorND::VectorND(int16_t dimension) {
    m_dimension = dimension;
    m_pData = nullptr;

    // Compute allocation size: dimension * sizeof(double)
    int32_t dim = (int32_t)dimension;
    uint32_t allocSize = (dim << 3) & 0xFFFFFFF8;  // dim * 8, aligned

    // Overflow check: if dim > 0x1FFFFFFF, set allocSize to -1
    if ((uint32_t)dim > 0x1FFFFFFF) {
        allocSize = 0xFFFFFFFF;
    }

    m_pData = (double*)rage_HeapAlloc(allocSize, 16);

    // Zero-initialize all elements (0.0 as double = all zero bits)
    if (dim > 0) {
        for (int16_t i = 0; i < m_dimension; i++) {
            m_pData[i] = 0.0;
        }
    }
}

/**
 * VectorND::~VectorND @ 0x8241DE90 | size: 0x68
 *
 * Destructor — frees the data array if allocated.
 */
VectorND::~VectorND() {
    if (m_pData) {
        rage_free(m_pData);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MatrixMND — M×N double matrix (row-major, row pointers)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * MatrixMND::MatrixMND @ 0x8241E118 | size: 0x194
 *
 * Constructs an M×N double matrix. Memory layout:
 *   +0x04: m_rows (int16) = number of rows
 *   +0x06: m_cols (int16) = number of columns
 *   +0x08: m_pRowPtrs (double**) = array of M row pointers
 *
 * Each row pointer points into a contiguous block of N doubles.
 * The row pointer array is allocated first (M * 4 bytes), then the
 * data block (M * N * 8 bytes). Row pointers are set up so that
 * m_pRowPtrs[i] = dataBlock + i * (N * 8).
 *
 * All elements initialized to 0.0.
 *
 * @param rows  Number of rows (int16)
 * @param cols  Number of columns (int16, default 1)
 */
MatrixMND::MatrixMND(int16_t rows, int16_t cols) {
    m_rows = rows;
    m_cols = cols;
    m_pRowPtrs = nullptr;

    int32_t numRows = (int32_t)rows;
    int32_t numCols = (int32_t)cols;

    // Allocate row pointer array: rows * sizeof(double*)
    uint32_t rowPtrSize = (numRows << 2) & 0xFFFFFFFC;  // rows * 4
    if ((uint32_t)numRows > 0x3FFFFFFF) {
        rowPtrSize = 0xFFFFFFFF;
    }

    m_pRowPtrs = (double**)rage_HeapAlloc(rowPtrSize, 16);

    // Allocate data block: rows * cols * sizeof(double)
    int32_t totalElements = numCols * numRows;
    uint32_t dataSize;
    if ((uint32_t)totalElements > 0x1FFFFFFF) {
        dataSize = 0xFFFFFFFF;
    } else {
        dataSize = (totalElements << 3) & 0xFFFFFFF8;  // total * 8
    }

    double* dataBlock = (double*)rage_HeapAlloc(dataSize, 16);

    // Set up row pointers: each row starts at dataBlock + row * (cols * 8)
    m_pRowPtrs[0] = dataBlock;
    if (numRows > 1) {
        int32_t colStride = numCols * 8;  // bytes per row
        for (int32_t i = 1; i < numRows; i++) {
            m_pRowPtrs[i] = (double*)((char*)m_pRowPtrs[i - 1] + colStride);
        }
    }

    // Zero-initialize all elements
    for (int32_t row = 0; row < numRows; row++) {
        for (int32_t col = 0; col < numCols; col++) {
            m_pRowPtrs[row][col] = 0.0;
        }
    }
}

/**
 * MatrixMND::~MatrixMND @ 0x8241E0A0 | size: 0x74
 *
 * Destructor — frees the data block (via m_pRowPtrs[0]) and
 * the row pointer array.
 */
MatrixMND::~MatrixMND() {
    if (m_pRowPtrs) {
        // Free the contiguous data block (pointed to by first row)
        if (m_pRowPtrs[0]) {
            rage_free(m_pRowPtrs[0]);
        }
        // Free the row pointer array itself
        rage_free(m_pRowPtrs);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MatrixMND::MultiplyRow — Matrix-vector dot product (single row)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * MatrixMND::MultiplyRow @ 0x8241DFC8 | size: 0xD8
 *
 * Computes dot product of matrix row with a VectorND:
 *   result = sum(matrix[rowIdx][col][i] * vector[i], i=0..lastCol) + bias
 *
 * Uses 4x loop unrolling for performance on PPC: processes 4 elements
 * per iteration, then handles the remainder.
 *
 * @param result   Output VectorND to accumulate into (result.m_pData[0])
 * @param rowIdx   Which row of this matrix to use
 * @param lastCol  Last column index to include in the dot product
 * @param source   Source matrix providing the column data
 * @param bias     Initial accumulator value (0.0 for pure dot product)
 */
void MatrixMND::MultiplyRow(VectorND* result, int rowIdx, int lastCol,
                            const MatrixMND* source, double bias) const {
    double* matRow = source->m_pRowPtrs[0];   // source data base
    double* vecData = result->m_pData;         // output vector data
    int16_t srcCols = source->m_cols;
    int srcColStride = (int32_t)srcCols * 8;   // bytes between source columns

    // Pointer to start of the relevant column in source matrix
    double* srcPtr = (double*)((char*)matRow + rowIdx * 8);

    double accumulator = bias;
    int remaining = lastCol + 1;

    // 4x unrolled loop: process 4 elements per iteration
    if (remaining >= 4) {
        int unrolledCount = remaining / 4;
        for (int block = 0; block < unrolledCount; block++) {
            double v0 = vecData[0] * srcPtr[0];
            srcPtr = (double*)((char*)srcPtr + srcColStride);

            double v1 = vecData[1] * srcPtr[0];
            srcPtr = (double*)((char*)srcPtr + srcColStride);

            double v2 = vecData[2] * srcPtr[0];
            srcPtr = (double*)((char*)srcPtr + srcColStride);

            double v3 = vecData[3] * srcPtr[0];
            srcPtr = (double*)((char*)srcPtr + srcColStride);

            accumulator += v0 + v1 + v2 + v3;
            vecData += 4;
        }
        remaining -= unrolledCount * 4;
    }

    // Handle remainder (0-3 elements)
    for (int i = 0; i < remaining; i++) {
        accumulator += vecData[i] * srcPtr[0];
        srcPtr = (double*)((char*)srcPtr + srcColStride);
    }

    // Store result (returned in f1 register in original PPC)
    // The caller stores this into the appropriate output location
}

} // namespace rage
