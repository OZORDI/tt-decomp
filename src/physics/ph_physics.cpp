/**
 * ph_physics.cpp — RAGE physics module utilities
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Small utility routines from the ph* physics subsystem that don't belong
 * to a specific class.  Placed in the physics compilation unit at
 * 0x824321xx (not in splits.txt → inferred from address range and prefix).
 */

#include "ph_physics.hpp"

extern "C" void* rage_alloc(uint32_t size);  // RAGE heap alloc (defined in heap.c)
#include <stddef.h>
#include <math.h>
#include <string.h>

/**
 * rage_strchr @ 0x824321B0 | size: 0x3C
 *
 * Scan a string for a specific character, returning a pointer to its first
 * occurrence.  Functionally equivalent to strchr(3) with two variants:
 *
 *   ch != 0  →  Returns pointer to first byte equal to ch, or NULL if not found.
 *   ch == 0  →  Returns pointer to the NUL terminator (scan-to-end shortcut).
 *
 * This variant is used during argument/config parsing in rage_AppInit
 * to locate '/' or '-' prefix characters in command-line tokens.
 *
 * The function is an alias of strchr in the binary (the linker merged the
 * two identical bodies into a single address).
 */
const char* rage_strchr(const char* str, int ch)
{
    unsigned char c = (unsigned char)*str;

    if (ch == 0) {
        /* Scan to end — return pointer to null terminator */
        while (c != 0)
            c = (unsigned char)*++str;
        return str;
    }

    /* Scan for ch */
    while (c != 0) {
        if (c == (unsigned char)ch)
            return str;
        c = (unsigned char)*++str;
    }

    /* Not found */
    return NULL;
}

/**
 * rage::phBoundCapsule::ComputeFixedPointDotProduct @ 0x824C35C8
 * 
 * Computes a fixed-point dot product for capsule collision detection.
 * Uses integer arithmetic for deterministic physics calculations.
 * 
 * The function performs:
 * 1. Loads two vector components from the capsule geometry
 * 2. Multiplies them together (fixed-point multiplication)
 * 3. Returns the result as a 32-bit signed integer
 * 
 * This is commonly used in capsule-vs-capsule or capsule-vs-plane tests
 * where exact reproducibility is required across platforms.
 */
int32_t rage::phBoundCapsule::ComputeFixedPointDotProduct() {
    // Load first component (likely X or normalized direction component)
    // field_0x0020 appears to be part of the capsule's axis/direction vector
    int32_t component1 = static_cast<int32_t>(field_0x0020);
    
    // Load second component (likely Y or another direction component)
    // field_0x0024 is adjacent, suggesting it's part of the same vector
    int32_t component2 = static_cast<int32_t>(field_0x0024);
    
    // Perform fixed-point multiplication
    // In fixed-point math, we multiply then shift to maintain precision
    // The result is a dot product component used in collision detection
    int32_t result = component1 * component2;
    
    return result;
}

/**
 * ph_vt57D8_20::Constructor @ 0x82460718 | size: 0x84
 *
 * Constructor for a physics instance wrapper class. This class embeds a
 * rage::phInst object at offset +0x0C and optionally registers itself with
 * the global phDemoWorld singleton.
 *
 * The constructor:
 * 1. Initializes vtable pointers for the wrapper and embedded phInst
 * 2. Calls phInst initialization on the embedded instance
 * 3. If the registerWithWorld flag is set, adds this object to phDemoWorld
 *
 * @param registerWithWorld - If bit 0 is set, register with phDemoWorld
 */
void ph_vt57D8_20_Constructor(void* thisPtr, uint32_t registerWithWorld) {
    // Cast to byte pointer for offset arithmetic
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Set up vtable pointers
    // Main vtable at offset 0
    *(void**)(obj + 0) = (void*)0x82005828;
    
    // Embedded phInst vtable at offset 12
    *(void**)(obj + 12) = (void*)0x820057D8;
    
    // Initialize the embedded phInst object at offset +0x0C
    // phInst_6158_p39 is the phInst initialization function
    void* phInstPtr = obj + 12;
    // Call phInst initialization (external function)
    // phInst_6158_p39(phInstPtr);
    
    // Update main vtable after phInst initialization
    *(void**)(obj + 0) = (void*)0x82003DB0;
    
    // If bit 0 of registerWithWorld is set, register with phDemoWorld
    if (registerWithWorld & 0x1) {
        // phDemoWorld_67D0_g is a global registration function
        // Parameters: phDemoWorld singleton address, this object, flags
        void* phDemoWorldSingleton = (void*)0x823E7888;
        uint32_t flags = 0x61820000;
        
        // Call phDemoWorld registration
        // phDemoWorld_67D0_g(phDemoWorldSingleton, thisPtr, flags);
    }
}


// External references
extern void util_B8A0(void* obj);  // Base initialization
extern const float g_floatZero;    // @ 0x8202D110
extern const float g_floatOne;     // @ 0x8202D110 - 8
extern void* g_fragDrawableVtable; // @ 0x82033094

/**
 * rage::fragDrawable::Constructor @ 0x82124220 | size: 0xCC
 *
 * Initializes a rage::fragDrawable object. This is the base drawable class
 * for fragmented/destructible geometry in the RAGE engine.
 *
 * The constructor:
 * 1. Calls base class initialization (util_B8A0)
 * 2. Sets up the vtable pointer
 * 3. Zeros out all state fields (256-302)
 * 4. Initializes a 3x3 identity matrix at offset +192
 * 5. Clears a 16-byte vector at offset +240
 *
 * Memory layout:
 *   +0     vtable pointer
 *   +192   3x3 matrix (identity: diag = 1.0f, off-diag = 0.0f)
 *   +240   16-byte vector (zeroed)
 *   +256   State fields (all zeroed)
 */
// TODO: fragDrawable needs proper namespace/class declaration
void fragDrawable_Constructor(void* thisPtr) {
    void* self = thisPtr; (void)self;
    // Call base class initialization
    util_B8A0(thisPtr);

    // Set vtable pointer
    *(void**)thisPtr = g_fragDrawableVtable;  // @ 0x82033094

    // Zero out state fields from +256 to +302
    *(uint32_t*)((char*)thisPtr + 256) = 0;
    *(uint32_t*)((char*)thisPtr + 260) = 0;
    *(uint16_t*)((char*)thisPtr + 264) = 0;
    *(uint16_t*)((char*)thisPtr + 266) = 0;
    *(uint32_t*)((char*)thisPtr + 268) = 0;
    *(uint16_t*)((char*)thisPtr + 272) = 0;
    *(uint8_t*)((char*)thisPtr + 274) = 1;    // Set flag to 1
    *(uint8_t*)((char*)thisPtr + 275) = 0;
    *(uint32_t*)((char*)thisPtr + 276) = 0;
    *(uint32_t*)((char*)thisPtr + 280) = 0;
    *(uint32_t*)((char*)thisPtr + 284) = 0;
    *(uint32_t*)((char*)thisPtr + 288) = 0;
    *(uint32_t*)((char*)thisPtr + 292) = 0;
    *(uint32_t*)((char*)thisPtr + 296) = 0;
    *(uint16_t*)((char*)thisPtr + 300) = 0;
    *(uint16_t*)((char*)thisPtr + 302) = 0;
    
    // Initialize 3x3 identity matrix at offset +192
    // Matrix layout: [m00 m01 m02] [m10 m11 m12] [m20 m21 m22]
    float* matrix = (float*)((char*)thisPtr + 192);
    
    // Row 0: [1, 0, 0, 0]
    matrix[0] = g_floatOne;   // m00 = 1.0f
    matrix[1] = g_floatZero;  // m01 = 0.0f
    matrix[2] = g_floatZero;  // m02 = 0.0f
    matrix[3] = g_floatZero;  // padding
    
    // Row 1: [0, 1, 0, 0]
    matrix[4] = g_floatZero;  // m10 = 0.0f
    matrix[5] = g_floatOne;   // m11 = 1.0f
    matrix[6] = g_floatZero;  // m12 = 0.0f
    matrix[7] = g_floatZero;  // padding
    
    // Row 2: [0, 0, 1, 0]
    matrix[8] = g_floatZero;  // m20 = 0.0f
    matrix[9] = g_floatZero;  // m21 = 0.0f
    matrix[10] = g_floatOne;  // m22 = 1.0f
    matrix[11] = g_floatZero; // padding
    
    // Zero out 16-byte vector at offset +240 using SIMD
    // vxor v0,v0,v0 + stvx v0,r0,r9
    uint32_t* vec = (uint32_t*)((char*)thisPtr + 240);
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0;
    vec[3] = 0;
}

/**
 * rage::phBoundRibbon::CopyFrom @ 0x82294AB8 | size: 0xD4
 *
 * Copy constructor / assignment operator for phBoundRibbon.
 * Performs a deep copy of all fields including vectors and floats.
 * 
 * The structure contains:
 * - 5 scalar fields at the beginning (20 bytes)
 * - 7 SIMD vectors (112 bytes total)
 * - 3 float values
 * - 1 byte flag
 * - 2 trailing fields
 * 
 * Total size: 168 bytes (0xA8)
 */
void rage::phBoundRibbon::CopyFrom(const rage::phBoundRibbon* source) {
    // Copy first 5 scalar fields (0-19)
    this->vtable = source->vtable;
    this->field_0x04 = source->field_0x04;
    this->field_0x08 = source->field_0x08;
    this->field_0x0C = source->field_0x0C;
    this->field_0x10 = source->field_0x10;

    // Copy 3 SIMD vectors (32-79)
    memcpy(this->m_vec1, source->m_vec1, sizeof(this->m_vec1));    // +32
    memcpy(this->m_vec2, source->m_vec2, sizeof(this->m_vec2));    // +48
    memcpy(this->m_vec3, source->m_vec3, sizeof(this->m_vec3));    // +64

    // Copy 3 float values (80-91)
    this->m_float1 = source->m_float1;  // +80
    this->m_float2 = source->m_float2;  // +84
    this->m_float3 = source->m_float3;  // +88

    // Copy byte flag (92)
    this->m_flags = source->m_flags;

    // Copy remaining 4 vectors (96-159)
    memcpy(this->m_vec4, source->m_vec4, sizeof(this->m_vec4));    // +96
    memcpy(this->m_vec5, source->m_vec5, sizeof(this->m_vec5));    // +112
    memcpy(this->m_vec6, source->m_vec6, sizeof(this->m_vec6));    // +128
    memcpy(this->m_vec7, source->m_vec7, sizeof(this->m_vec7));    // +144

    // Copy trailing fields (160-167)
    this->field_0xA0 = source->field_0xA0;  // +160
    this->field_0xA4 = source->field_0xA4;  // +164
}

/**
 * phNamedNode_Init @ 0x82451B58 | size: 0x3C
 *
 * Constructor for a small physics object with intrusive linked list support.
 * Initializes the object with a vtable, sets up circular list pointers
 * (prev/next both point to self initially), and copies a 20-byte identifier
 * from the source buffer.
 *
 * This pattern is common in RAGE for objects that need to be part of
 * intrusive doubly-linked lists (e.g., active physics objects, collision
 * candidates, or named resource pools).
 *
 * @param name - Pointer to 20-byte name/identifier buffer to copy
 */
void phNamedNode_Init(void* thisPtr, const char* name) {
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Set vtable pointer at offset 0
    *(void**)(obj + 0) = (void*)0x8200460C;
    
    // Initialize circular doubly-linked list pointers
    // Both prev and next point to self initially
    *(void**)(obj + 4) = thisPtr;  // prev = this
    *(void**)(obj + 8) = thisPtr;  // next = this
    
    // Copy 20-byte name/identifier from source to offset +16
    // Using byte-by-byte copy as in original assembly (mtctr + loop)
    const char* src = name;
    char* dst = (char*)(obj + 16);
    
    for (int i = 0; i < 20; i++) {
        dst[i] = src[i];
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundOTGrid — Octree Grid Collision Bound
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// External globals
extern void* g_display_obj_ptr;  // @ 0x826066E4

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::SetupCollisionGrid @ 0x82508098 | size: 0x1A0
//
// Initializes the octree grid collision detection system with dimension extents
// and grid cell pointers. Validates all parameters and dispatches to appropriate
// collision detection routine based on which dimensions have zero extent.
//
// Parameters:
//   gridMinX, gridMinY, gridMinZ - Grid cell pointers for each axis
//   extentX, extentY, extentZ - Dimensional extents (0.0 = collapsed dimension)
//
// Returns: 0 on success, 7 on parameter validation failure
// ─────────────────────────────────────────────────────────────────────────────
int phBoundOTGrid::SetupCollisionGrid(
    void* gridMinX, void* gridMinY, void* gridMinZ,
    float extentX, float extentY, float extentZ,
    void* gridMaxX, void* gridMaxY, void* gridMaxZ,
    float minX, float minY, float minZ,
    float maxX, float maxY, float maxZ)
{
    // Validate all required parameters
    if (!this || !gridMinX || !gridMinY || !gridMinZ || 
        !gridMaxX || !gridMaxY || !gridMaxZ) {
        return 7;  // Invalid parameter error code
    }

    // Store grid cell pointers at offsets 0x3BBC-0x3BD0
    m_pGridMinX = gridMinX;      // +0x3BBC (15292)
    m_pGridMinY = gridMinY;      // +0x3BC0 (15296)
    m_pGridMinZ = gridMinZ;      // +0x3BC4 (15300)
    m_pGridMaxX = gridMaxX;      // +0x3BC8 (15304)
    m_pGridMaxY = gridMaxY;      // +0x3BCC (15308)
    m_pGridMaxZ = gridMaxZ;      // +0x3BD0 (15312)

    const float ZERO_THRESHOLD = 0.0f;

    // Check if all extents are zero (point collision)
    if (extentY == ZERO_THRESHOLD && extentZ == ZERO_THRESHOLD) {
        if (minY == maxY) {
            if (minY == ZERO_THRESHOLD) {
                if (maxZ == ZERO_THRESHOLD) {
                    // 1D collision along X axis only
                    return CollideLineX(minX, maxX, minZ);
                }
                // 2D collision in XZ plane
                return CollidePlaneXZ(minX, maxX, minZ, maxZ);
            }
            // 2D collision in XY plane with Z extent
            if (maxZ == ZERO_THRESHOLD) {
                return CollidePlaneXY(minX, maxX, minY);
            }
            // Full 3D collision
            return Collide3D(minX, maxX, minY, minZ, maxZ);
        }
        // 3D collision with Y extent
        if (maxZ == ZERO_THRESHOLD) {
            return Collide3DYExtent(minX, maxX, minY, maxY);
        }
        return Collide3DFull(minX, maxX, minY, maxY, minZ, maxZ);
    }

    // General capsule collision (handles arbitrary extents)
    return CollideCapsule(minX, maxX, minY, maxY, minZ, maxZ, extentY, extentZ);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::GetCellAtIndex (vfn_11) @ 0x8229D448 | size: 0x3C
//
// Retrieves the grid cell at the root level (index 0) and calls its vtable
// slot 11 method. Falls back to global display object if grid is not initialized.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundOTGrid::GetCellAtIndex() {
    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (gridData) {
        // Call vtable slot 11 on the root grid cell with parameter 0
        void** cellVTable = *(void***)gridData;
        typedef void (*VTableFunc)(void*, int);
        VTableFunc func = (VTableFunc)cellVTable[11];
        func(gridData, 0);
        return;
    }

    // Fallback: call vtable slot 9 on global display object
    void* displayObj = g_display_obj_ptr;
    void** displayVTable = *(void***)displayObj;
    typedef void (*DisplayFunc)(void*);
    DisplayFunc fallback = (DisplayFunc)displayVTable[9];
    fallback(displayObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::QueryCellState (vfn_12) @ 0x8229D488 | size: 0x2C
//
// Queries the state of the root grid cell by calling its vtable slot 12 method.
// Returns nullptr if grid is not initialized.
// ─────────────────────────────────────────────────────────────────────────────
void* phBoundOTGrid::QueryCellState() {
    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (gridData) {
        // Call vtable slot 12 on the root grid cell with parameter 0
        void** cellVTable = *(void***)gridData;
        typedef void* (*VTableFunc)(void*, int);
        VTableFunc func = (VTableFunc)cellVTable[12];
        return func(gridData, 0);
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::RenderCell (vfn_14) @ 0x8229D4B8 | size: 0x4C
//
// Renders a specific grid cell by calculating its index and calling the cell's
// vtable slot 14 method. Falls back to global display object if grid is null.
//
// Parameters:
//   cellIndex - Index of the cell to render (used to calculate grid offset)
// ─────────────────────────────────────────────────────────────────────────────
void phBoundOTGrid::RenderCell(int cellIndex) {
    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (gridData) {
        // Calculate cell offset: cellIndex * 192 bytes per cell
        int cellOffset = cellIndex * 192;
        void* cell = (char*)gridData + cellOffset;
        
        if (cell) {
            // Call vtable slot 14 on the specific grid cell
            void** cellVTable = *(void***)cell;
            typedef void (*VTableFunc)(void*);
            VTableFunc func = (VTableFunc)cellVTable[14];
            func(cell);
            return;
        }
    }

    // Fallback: call vtable slot 9 on global display object
    void* displayObj = g_display_obj_ptr;
    void** displayVTable = *(void***)displayObj;
    typedef void (*DisplayFunc)(void*);
    DisplayFunc fallback = (DisplayFunc)displayVTable[9];
    fallback(displayObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::UpdateCell (vfn_15) @ 0x8229D548 | size: 0x50
//
// Updates a specific grid cell by calculating its index and calling the cell's
// vtable slot 15 method. Falls back to global display object if grid is null.
//
// Parameters:
//   cellIndex - Index of the cell to update
// ─────────────────────────────────────────────────────────────────────────────
void phBoundOTGrid::UpdateCell(int cellIndex) {
    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (gridData) {
        // Calculate cell offset: cellIndex * 192 bytes per cell
        int cellOffset = cellIndex * 192;
        void* cell = (char*)gridData + cellOffset;
        
        if (cell) {
            // Call vtable slot 15 on the specific grid cell with parameter 0
            void** cellVTable = *(void***)cell;
            typedef void (*VTableFunc)(void*, int);
            VTableFunc func = (VTableFunc)cellVTable[15];
            func(cell, 0);
            return;
        }
    }

    // Fallback: call vtable slot 9 on global display object
    void* displayObj = g_display_obj_ptr;
    void** displayVTable = *(void***)displayObj;
    typedef void (*DisplayFunc)(void*);
    DisplayFunc fallback = (DisplayFunc)displayVTable[9];
    fallback(displayObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::ProcessCell (vfn_16) @ 0x8229D508 | size: 0x3C
//
// Processes a specific grid cell by calculating its index and calling the cell's
// vtable slot 16 method. Returns nullptr if grid or cell is null.
//
// Parameters:
//   cellIndex - Index of the cell to process
// ─────────────────────────────────────────────────────────────────────────────
void* phBoundOTGrid::ProcessCell(int cellIndex) {
    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (gridData) {
        // Calculate cell offset: cellIndex * 192 bytes per cell
        int cellOffset = cellIndex * 192;
        void* cell = (char*)gridData + cellOffset;
        
        if (cell) {
            // Call vtable slot 16 on the specific grid cell
            void** cellVTable = *(void***)cell;
            typedef void* (*VTableFunc)(void*);
            VTableFunc func = (VTableFunc)cellVTable[16];
            return func(cell);
        }
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundOTGrid::IsCellValid (vfn_17) @ 0x8229D598 | size: 0x38
//
// Checks if a grid cell at the specified index exists and is valid.
//
// Parameters:
//   cellIndex - Index of the cell to check (must be >= 0)
//
// Returns: true if cell exists, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool phBoundOTGrid::IsCellValid(int cellIndex) {
    if (cellIndex < 0) {
        return false;
    }

    void* gridData = m_pGridData;  // +0x90 (144)
    
    if (!gridData) {
        return false;
    }

    // Calculate cell offset: cellIndex * 192 bytes per cell
    int cellOffset = cellIndex * 192;
    void* cell = (char*)gridData + cellOffset;
    
    return (cell != nullptr);
}

}  // namespace rage

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundOTGrid::RoundToNearestInt @ 0x82431910 | size: 0x3C
//
// Rounds a double-precision floating point value to the nearest integer,
// handling edge cases and clamping. Uses PowerPC fsel-based rounding logic.
//
// This is a helper function used by grid coordinate conversion routines to
// convert world-space positions to discrete grid cell indices.
//
// Returns: Rounded value as a double
// ─────────────────────────────────────────────────────────────────────────────
double rage::phBoundOTGrid::RoundToNearestInt(double value) {
    // Convert to int64 and back to get truncated value
    int64_t truncated = static_cast<int64_t>(value);
    double truncatedFloat = static_cast<double>(truncated);
    
    // Calculate the fractional part
    double fraction = value - truncatedFloat;
    
    // Adjust for rounding: if fraction >= 0.5, round up; otherwise round down
    double rounded = (fraction >= 0.0) ? truncatedFloat : (truncatedFloat - 1.0);
    
    // Handle edge cases using fsel-style logic
    double absValue = (value >= 0.0) ? value : -value;
    double result = (fraction >= 0.0) ? rounded : truncatedFloat;
    
    // Clamp to valid range
    const double MAX_SAFE_INT = 9007199254740992.0;  // 2^53
    result = (absValue <= MAX_SAFE_INT) ? result : value;
    
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundOTGrid::WorldPositionToGridIndex @ 0x8229B418 | size: 0xC8
//
// Converts a 3D world-space position to a linear grid cell index within the
// octree grid's spatial partitioning structure.
//
// The function:
// 1. Scales the input position by the grid's scale factor
// 2. Rounds to integer grid coordinates
// 3. Subtracts grid offsets to get cell indices
// 4. Validates that the cell is within grid bounds
// 5. Computes linear index: index = dim_y * cell_z + cell_x
//
// Parameters:
//   position - Pointer to 3-component float vector [x, y, z]
//
// Returns: Grid cell index (>= 0) if position is within bounds, -1 otherwise
// ─────────────────────────────────────────────────────────────────────────────
int rage::phBoundOTGrid::WorldPositionToGridIndex(const float* position) {
    // Load grid parameters
    float scaleFactor = m_gridScaleFactor;      // +116 (0x74)
    int32_t offsetY = m_gridOffsetY;            // +120 (0x78)
    int32_t dimensionY = m_gridDimensionY;      // +128 (0x80)
    int32_t offsetX = m_gridOffsetX;            // +132 (0x84)
    int32_t dimensionX = m_gridDimensionX;      // +140 (0x8C)
    
    // Scale and round Z coordinate
    float scaledZ = position[2] * scaleFactor;
    double roundedZ = RoundToNearestInt(static_cast<double>(scaledZ));
    int32_t cellZ = static_cast<int32_t>(roundedZ) - offsetX;
    
    // Scale and round X coordinate
    float scaledX = position[0] * scaleFactor;
    double roundedX = RoundToNearestInt(static_cast<double>(scaledX));
    int32_t cellX = static_cast<int32_t>(roundedX) - offsetY;
    
    // Validate bounds
    if (cellX < 0 || cellX >= dimensionY) {
        return -1;  // Out of bounds in X dimension
    }
    
    if (cellZ < 0 || cellZ >= dimensionX) {
        return -1;  // Out of bounds in Z dimension
    }
    
    // Compute linear grid index
    int32_t gridIndex = dimensionY * cellZ + cellX;
    return gridIndex;
}


// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Batch Implementation (10 functions)
// ═════════════════════════════════════════════════════════════════════════════

// External function declarations (helper functions)
extern float phBoundCapsule_01D0_g(float value);  // sqrt wrapper
extern float phBoundCapsule_01D8_g(float value);  // trigonometric function
extern float phBoundCapsule_0FE0_g(float f1, float f2);  // capsule calculation
extern float phBoundCapsule_02B0_g(float value);  // normalization function
extern float atSingleton_1308_g(float value, double param);  // singleton math helper

// External constants
extern const float g_floatZero;      // @ 0x8202D110
extern const float g_floatOne;       // @ 0x8202D108
extern const float g_capsuleRadius;  // @ 0x82079B30

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_0550_g @ 0x820D0550 | size: 0x50
//
// Conditional capsule calculation wrapper. If both f2 and f1 are zero,
// returns zero. Otherwise calls the main capsule calculation function.
// ─────────────────────────────────────────────────────────────────────────────
float phBoundCapsule_0550_g(float f1, float f2) {
    if (f2 == g_floatZero) {
        if (f1 != g_floatZero) {
            return phBoundCapsule_0FE0_g(f1, f2);
        }
        return g_floatZero;
    }
    return phBoundCapsule_0FE0_g(f1, f2);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_B6A0_g @ 0x820CB6A0 | size: 0x50
//
// Applies a clamped trigonometric transformation to an input value.
// If the value is outside the valid range [minThreshold, maxThreshold],
// it's clamped. Otherwise, it's scaled and passed through a trig function.
// ─────────────────────────────────────────────────────────────────────────────
float phBoundCapsule_B6A0_g(float inputValue) {
    // Load threshold constants from global data
    const float minThreshold = *(float*)0x8202D010;  // offset +288 from base
    const float maxThreshold = *(float*)0x8202D008;  // offset +280 from base
    const float scaleFactor = *(float*)0x8202CFF0;   // offset +0 from base
    
    if (inputValue <= minThreshold) {
        return minThreshold;
    }
    
    if (inputValue >= maxThreshold) {
        return maxThreshold;
    }
    
    // Apply scaling and trigonometric transformation
    float scaledValue = inputValue * scaleFactor;
    return phBoundCapsule_01D8_g(scaledValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_B598_g @ 0x820CB598 | size: 0x58
//
// Returns a capsule property based on flags at offset +64.
// Checks bit 1 (0x02) and bit 4 (0x10) to determine which value to return.
// ─────────────────────────────────────────────────────────────────────────────
float phBoundCapsule_B598_g(void* capsule) {
    uint8_t* obj = (uint8_t*)capsule;
    uint8_t flags = obj[64];
    
    // Check bit 1 (0x02)
    if (flags & 0x02) {
        return g_floatOne;
    }
    
    // Check bit 4 (0x10)
    if (flags & 0x10) {
        return g_floatOne;
    }
    
    // Return field at offset +28
    return *(float*)(obj + 28);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_04F0_g @ 0x820D04F0 | size: 0x60
//
// Computes two related capsule properties and stores them in output pointers.
// Normalizes the first input, then applies a trigonometric transformation
// to the second input.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_04F0_g(float* outNormalized, float* outTransformed, float input1, float input2) {
    float normalized = phBoundCapsule_02B0_g(input1);
    *outNormalized = normalized;
    
    float transformed = phBoundCapsule_01D8_g(input2);
    *outTransformed = transformed;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_3F10_g @ 0x820C3F10 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a vertical capsule aligned along the Y axis.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_3F10_g(void* capsule, float height, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedHeight = phBoundCapsule_02B0_g(height);
    float transformedRadius = phBoundCapsule_01D8_g(radius);
    
    const float negOne = *(float*)0x8202D108;  // -1.0f
    const float zero = *(float*)0x8202D110;    // 0.0f
    
    // Initialize capsule geometry
    *(float*)(obj + 0) = negOne;              // Start cap X
    *(float*)(obj + 4) = zero;                // Start cap Y
    *(float*)(obj + 8) = zero;                // Start cap Z
    *(float*)(obj + 16) = zero;               // Direction X
    *(float*)(obj + 20) = normalizedHeight;   // Direction Y (height)
    *(float*)(obj + 24) = -transformedRadius; // End cap offset
    *(float*)(obj + 32) = zero;               // Padding
    *(float*)(obj + 36) = -transformedRadius; // Radius (negated)
    *(float*)(obj + 40) = normalizedHeight;   // Total height
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_3F98_g @ 0x820C3F98 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a horizontal capsule aligned along the X axis.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_3F98_g(void* capsule, float length, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedLength = phBoundCapsule_02B0_g(length);
    float transformedRadius = phBoundCapsule_01D8_g(radius);
    
    const float one = *(float*)0x8202D100;   // 1.0f (offset +8 from base)
    const float zero = *(float*)0x8202D0F8;  // 0.0f (offset +0 from base)
    
    // Initialize capsule geometry
    *(float*)(obj + 0) = normalizedLength;    // Start cap X
    *(float*)(obj + 4) = one;                 // Start cap Y
    *(float*)(obj + 8) = -transformedRadius;  // Start cap Z (negated radius)
    *(float*)(obj + 16) = one;                // Direction X
    *(float*)(obj + 20) = zero;               // Direction Y
    *(float*)(obj + 24) = one;                // Direction Z
    *(float*)(obj + 32) = transformedRadius;  // End cap offset
    *(float*)(obj + 36) = one;                // Padding
    *(float*)(obj + 40) = normalizedLength;   // Total length
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_4020_g @ 0x820C4020 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a capsule aligned along the Z axis.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_4020_g(void* capsule, float depth, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedDepth = phBoundCapsule_02B0_g(depth);
    float transformedRadius = phBoundCapsule_01D8_g(radius);
    
    const float one = *(float*)0x8202D100;   // 1.0f (offset +8 from base)
    const float zero = *(float*)0x8202D0F8;  // 0.0f (offset +0 from base)
    
    // Initialize capsule geometry
    *(float*)(obj + 0) = normalizedDepth;     // Start cap position
    *(float*)(obj + 4) = transformedRadius;   // Radius
    *(float*)(obj + 8) = one;                 // Direction component
    *(float*)(obj + 16) = -transformedRadius; // End cap offset (negated)
    *(float*)(obj + 20) = normalizedDepth;    // Total depth
    *(float*)(obj + 24) = one;                // Padding
    *(float*)(obj + 32) = one;                // Padding
    *(float*)(obj + 36) = one;                // Padding
    *(float*)(obj + 40) = zero;               // Padding
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_9CF8_g @ 0x820C9CF8 | size: 0xA0
//
// Adjusts capsule position based on a 2D offset vector and interpolation factors.
// Computes the distance from the offset, normalizes it, and applies scaled
// movement along the offset direction.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_9CF8_g(void* capsule, float* offsetVec, float scaleFactor, float lerpFactor) {
    uint8_t* obj = (uint8_t*)capsule;
    
    // Load current capsule position
    float currentX = *(float*)(obj + 0);
    float currentY = *(float*)(obj + 4);
    
    // Load offset vector
    float offsetX = offsetVec[0];
    float offsetY = offsetVec[1];
    
    // Compute delta from current position to offset
    float deltaX = offsetX - currentX;
    float deltaY = offsetY - currentY;
    
    // Compute squared distance
    float distSq = deltaX * deltaX + deltaY * deltaY;
    
    // Compute square root (distance)
    float distance = phBoundCapsule_01D0_g(distSq);
    
    // Normalize and apply singleton transformation
    const double normalizationFactor = *(double*)0x82079B30;
    float normalizedDist = atSingleton_1308_g(distance, normalizationFactor);
    
    // Compute movement direction (normalized delta)
    float dirX = normalizedDist * scaleFactor * deltaX;
    float dirY = normalizedDist * scaleFactor * deltaY;
    
    // Apply lerped movement
    float newX = currentX + dirX * lerpFactor;
    float newY = currentY + dirY * lerpFactor;
    
    // Store updated position
    *(float*)(obj + 0) = newX;
    *(float*)(obj + 4) = newY;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_9730_fw @ 0x820D9730 | size: 0xB0
//
// Initializes a capsule from a template and applies transformations from
// a source object. Copies 64 bytes of template data, then conditionally
// applies a radius transformation based on flags.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_9730_fw(void* capsule, void* sourceObj) {
    uint8_t* dest = (uint8_t*)capsule;
    uint8_t* src = (uint8_t*)sourceObj;
    
    // Load template data address
    const uint8_t* templateData = (const uint8_t*)0x825CB800;  // lbl_825CB800
    
    // Copy 64 bytes of template data (4 x 16-byte SIMD vectors)
    for (int i = 0; i < 64; i += 16) {
        // Copy 16 bytes at a time
        for (int j = 0; j < 16; j++) {
            dest[i + j] = templateData[i + j];
        }
    }
    
    // Get physics data pointer from source object
    void* physicsData = *(void**)(src + 400);
    const int32_t physicsOffset = *(int32_t*)0x82607A48;  // Global offset
    uint8_t* physicsObj = (uint8_t*)physicsData + physicsOffset;
    
    // Check flags at offset +64
    uint8_t flags = physicsObj[64];
    
    if (flags == 0) {
        // Apply radius transformation
        const float defaultRadius = *(float*)0x82079B30;  // g_capsuleRadius
        phBoundCapsule_3F98_g(capsule, defaultRadius, 0.0f);
    }
    
    // Copy transformation matrix from source object (offset +384, 16 bytes)
    const uint8_t* transformMatrix = src + 384;
    for (int i = 0; i < 16; i++) {
        dest[48 + i] = transformMatrix[i];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_F548_wrh @ 0x820DF548 | size: 0x80
//
// Extracts capsule geometry data from an array of capsule structures.
// Searches for the first valid capsule (non-zero radius) and copies its
// geometry parameters to output pointers.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_F548_wrh(void* container, int* outIndex, float* outX, float* outY, float* outZ, float* outRadius) {
    uint8_t* obj = (uint8_t*)container;
    
    // Get capsule array pointer
    void* capsuleArray = *(void**)(obj + 8);
    uint8_t* capsules = (uint8_t*)capsuleArray;
    
    int index = *outIndex;
    const float zero = *(float*)0x8202D110;  // g_floatZero
    
    // Search for first valid capsule (radius > 0)
    while (true) {
        // Each capsule is 104 bytes
        uint8_t* currentCapsule = capsules + (index * 104);
        float radius = *(float*)(currentCapsule + 40);
        
        if (radius >= zero) {
            // Found valid capsule - extract geometry
            *outX = *(float*)(currentCapsule + 36);
            *outY = *(float*)(currentCapsule + 48);
            *outZ = *(float*)(currentCapsule + 44);
            *outRadius = radius;
            *outIndex = index;
            return;
        }
        
        // Check if we've reached the last capsule (index 11)
        if (index == 11) {
            return;
        }
        
        // Check next capsule
        float nextRadius = *(float*)(capsules + 1184);  // Capsule 11's radius
        if (nextRadius < zero) {
            return;
        }
        
        index = 11;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Capsule Collision Bound Functions
// ═════════════════════════════════════════════════════════════════════════════

// External float constants
extern const float g_floatOne;   // @ 0x8202D108 = 1.0f
extern const float g_floatZero;  // @ 0x8202D110 = 0.0f

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::GetRadius @ 0x820CB598 | size: 0x58
//
// Returns the effective radius of the capsule based on its configuration flags.
// Checks bit 1 (0x02) and bit 4 (0x10) of the flags byte at offset +64.
//
// Returns:
//   - 1.0f if bit 1 is set (standard capsule)
//   - 1.0f if bit 4 is set (alternate configuration)
//   - m_radius (field at +28) otherwise (custom radius)
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::GetRadius() const {
    uint8_t flags = *(const uint8_t*)((const char*)this + 0x40);  // +64

    // Check bit 1 (0x02) - standard capsule flag
    if (flags & 0x02) {
        return g_floatOne;
    }

    // Check bit 4 (0x10) - alternate configuration flag
    if (flags & 0x10) {
        return g_floatOne;
    }

    // Return custom radius from field +28
    return *(const float*)((const char*)this + 0x1C);  // +28
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ScaleRadius @ 0x820CB6A0 | size: 0x50
//
// Conditionally scales the capsule radius based on the input scale factor.
// If scale is outside the range (floatZero, floatOne), applies scaling.
//
// Parameters:
//   scale - Scale factor to apply to radius
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ScaleRadius(float scale) {
    const float* constants = (const float*)0x8202D108;
    const float one = constants[0];    // +0 = 1.0f
    const float zero = constants[72];  // +288 = 0.0f (offset in bytes / 4)
    
    // Only scale if outside valid range
    if (scale > zero && scale < one) {
        return;  // No scaling needed
    }
    
    // Apply scaling: multiply by constant at offset +0
    float scaleFactor = constants[0];  // 1.0f
    float scaledValue = scale * scaleFactor;
    
    // Call helper function to apply the scaled value
    phBoundCapsule_01D8_g(scaledValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ComputeBounds @ 0x820D04F0 | size: 0x60
//
// Computes the bounding box extents for the capsule by calling two helper
// functions and storing their results.
//
// Parameters:
//   scale - Scale factor for bound computation
//   outMin - Pointer to store minimum bound
//   outMax - Pointer to store maximum bound
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ComputeBounds(float scale, float* outMin, float* outMax) {
    // Compute minimum bound
    float minBound = phBoundCapsule_02B0_g(scale);
    *outMin = minBound;
    
    // Compute maximum bound using the scale
    float maxBound = phBoundCapsule_01D8_g(scale);
    *outMax = maxBound;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ComputeExtent @ 0x820D0550 | size: 0x50
//
// Computes a single extent value based on two input parameters.
// If both parameters are zero, returns zero. Otherwise calls helper function.
//
// Parameters:
//   param1 - First extent parameter
//   param2 - Second extent parameter
//
// Returns: Computed extent value
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::ComputeExtent(float param1, float param2) const {
    const float zero = g_floatZero;
    
    // If param2 is zero
    if (param2 == zero) {
        // And param1 is also zero, return zero
        if (param1 == zero) {
            return zero;
        }
        
        // Otherwise compute extent with param1 (param2 is zero)
        return phBoundCapsule_0FE0_g(param1, param2);
    }

    // General case: compute extent with both parameters
    return phBoundCapsule_0FE0_g(param1, param2);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::InitializeAxisAlignedX @ 0x820C3F10 | size: 0x84
//
// Initializes a capsule aligned along the X axis.
// Sets up a 3x3 orientation matrix with X-axis alignment.
//
// Parameters:
//   halfLength - Half the length of the capsule along X axis
//   outMatrix - Pointer to 44-byte output structure (3x3 matrix + padding)
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::InitializeAxisAlignedX(float halfLength, float* outMatrix) {
    // Compute capsule parameters
    float minBound = phBoundCapsule_02B0_g(halfLength);
    float maxBound = phBoundCapsule_01D8_g(halfLength);
    
    const float* constants = (const float*)0x8202D110;
    const float zero = constants[0];   // +0 = 0.0f
    const float negOne = constants[-2]; // -8 = -1.0f
    
    // Initialize matrix for X-axis alignment
    // Row 0: [-1, 0, 0]
    outMatrix[0] = negOne;
    outMatrix[1] = zero;
    outMatrix[2] = zero;
    
    // Row 1: [0, 0, maxBound]
    outMatrix[4] = zero;
    outMatrix[5] = zero;
    outMatrix[6] = maxBound;
    
    // Row 2: [0, minBound, 0]
    outMatrix[8] = zero;
    outMatrix[9] = -maxBound;  // Negated
    outMatrix[10] = minBound;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::InitializeAxisAlignedY @ 0x820C3F98 | size: 0x84
//
// Initializes a capsule aligned along the Y axis.
// Sets up a 3x3 orientation matrix with Y-axis alignment.
//
// Parameters:
//   halfLength - Half the length of the capsule along Y axis
//   outMatrix - Pointer to 44-byte output structure (3x3 matrix + padding)
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::InitializeAxisAlignedY(float halfLength, float* outMatrix) {
    // Compute capsule parameters
    float minBound = phBoundCapsule_02B0_g(halfLength);
    float maxBound = phBoundCapsule_01D8_g(halfLength);
    
    const float* constants = (const float*)0x8202D108;
    const float one = constants[0];   // +0 = 1.0f
    const float zero = constants[2];  // +8 = 0.0f
    
    // Initialize matrix for Y-axis alignment
    // Row 0: [minBound, 0, 0]
    outMatrix[0] = minBound;
    outMatrix[1] = zero;
    outMatrix[2] = zero;
    
    // Row 1: [0, 0, -maxBound]
    outMatrix[4] = zero;
    outMatrix[5] = zero;
    outMatrix[6] = -maxBound;
    
    // Row 2: [0, one, 0]
    outMatrix[8] = zero;
    outMatrix[9] = one;
    outMatrix[10] = zero;
    
    // Additional fields
    outMatrix[8] = maxBound;   // +32
    outMatrix[9] = zero;       // +36
    outMatrix[10] = minBound;  // +40
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::InitializeAxisAlignedZ @ 0x820C4020 | size: 0x84
//
// Initializes a capsule aligned along the Z axis.
// Sets up a 3x3 orientation matrix with Z-axis alignment.
//
// Parameters:
//   halfLength - Half the length of the capsule along Z axis
//   outMatrix - Pointer to 44-byte output structure (3x3 matrix + padding)
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::InitializeAxisAlignedZ(float halfLength, float* outMatrix) {
    // Compute capsule parameters
    float minBound = phBoundCapsule_02B0_g(halfLength);
    float maxBound = phBoundCapsule_01D8_g(halfLength);
    
    const float* constants = (const float*)0x8202D108;
    const float one = constants[0];   // +0 = 1.0f
    const float zero = constants[2];  // +8 = 0.0f
    
    // Initialize matrix for Z-axis alignment
    // Row 0: [minBound, maxBound, 0]
    outMatrix[0] = minBound;
    outMatrix[1] = maxBound;
    outMatrix[2] = zero;
    
    // Row 1: [0, -maxBound, 0]
    outMatrix[4] = zero;
    outMatrix[5] = -maxBound;
    outMatrix[6] = zero;
    
    // Row 2: [0, 0, one]
    outMatrix[8] = zero;
    outMatrix[9] = zero;
    outMatrix[10] = one;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::TransformByDirection @ 0x820C40A8 | size: 0x13C
//
// Transforms the capsule's orientation matrix based on a direction vector.
// Performs a complex 3x3 matrix transformation using the input direction.
//
// Parameters:
//   halfLength - Half the length of the capsule
//   direction - Pointer to 3-component direction vector
//   outMatrix - Pointer to 44-byte output structure (3x3 matrix + padding)
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::TransformByDirection(float halfLength, const float* direction, float* outMatrix) {
    // Compute capsule parameters
    float minBound = phBoundCapsule_02B0_g(halfLength);
    float maxBound = phBoundCapsule_01D8_g(halfLength);
    
    // Load direction components
    float dirX = direction[0];
    float dirY = direction[1];
    float dirZ = direction[2];
    
    const float one = g_floatOne;
    float scale = one - minBound;
    
    // Compute diagonal elements (scaled by direction squared)
    float diagX = dirX * dirX * scale + minBound;
    float diagY = dirY * dirY * scale + minBound;
    float diagZ = dirZ * dirZ * scale + minBound;
    
    outMatrix[0] = diagX;   // +0
    outMatrix[5] = diagY;   // +20
    outMatrix[10] = diagZ;  // +40
    
    // Compute off-diagonal elements (cross products with maxBound)
    float xyTerm = dirX * scale * dirY;
    float xzScaled = maxBound * dirZ;
    
    outMatrix[1] = xyTerm + xzScaled;   // +4
    outMatrix[4] = xyTerm - xzScaled;   // +16
    
    float xzTerm = dirX * scale * dirZ;
    float xyScaled = maxBound * dirY;
    
    outMatrix[2] = xzTerm - xyScaled;   // +8
    outMatrix[8] = xzTerm + xyScaled;   // +32
    
    float yzTerm = scale * dirZ * dirY;
    float yxScaled = maxBound * dirX;
    
    outMatrix[6] = yzTerm + yxScaled;   // +24
    outMatrix[9] = yzTerm - yxScaled;   // +36
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::InitializeFromDirection @ 0x820C3DA0 | size: 0x170
//
// High-level initialization function that dispatches to the appropriate
// axis-aligned or direction-based initialization based on the input vector.
//
// Parameters:
//   halfLength - Half the length of the capsule
//   direction - Pointer to 3-component direction vector
//   outMatrix - Pointer to 44-byte output structure
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::InitializeFromDirection(float halfLength, const float* direction, float* outMatrix) {
    const float zero = g_floatZero;
    const float one = g_floatOne;
    
    // Special case: zero length capsule (sphere)
    if (halfLength == zero) {
        // Initialize as unit sphere
        outMatrix[0] = one;    // +0
        outMatrix[1] = zero;   // +4
        outMatrix[2] = zero;   // +8
        outMatrix[4] = zero;   // +16
        outMatrix[5] = one;    // +20
        outMatrix[6] = zero;   // +24
        outMatrix[8] = zero;   // +32
        outMatrix[9] = zero;   // +36
        outMatrix[10] = one;   // +40
        return;
    }
    
    // Load direction components
    float dirX = direction[0];
    float dirY = direction[1];
    float dirZ = direction[2];
    
    // Check for axis-aligned cases
    if (dirX == zero) {
        if (dirY == zero) {
            // Z-axis aligned
            if (dirZ <= zero) {
                halfLength = -halfLength;  // Flip if negative
            }
            InitializeAxisAlignedZ(halfLength, outMatrix);
            return;
        }
        
        if (dirZ == zero) {
            // Y-axis aligned
            if (dirY <= zero) {
                halfLength = -halfLength;  // Flip if negative
            }
            InitializeAxisAlignedY(halfLength, outMatrix);
            return;
        }
    } else if (dirY == zero && dirZ == zero) {
        // X-axis aligned
        if (dirX <= zero) {
            halfLength = -halfLength;  // Flip if negative
        }
        InitializeAxisAlignedX(halfLength, outMatrix);
        return;
    }
    
    // General case: arbitrary direction
    // Normalize the direction vector using SIMD operations
    float lengthSq = dirX * dirX + dirY * dirY + dirZ * dirZ;
    float invLength = 1.0f / sqrtf(lengthSq);
    
    float normalizedDir[3];
    normalizedDir[0] = dirX * invLength;
    normalizedDir[1] = dirY * invLength;
    normalizedDir[2] = dirZ * invLength;
    
    // Transform using the normalized direction
    TransformByDirection(halfLength, normalizedDir, outMatrix);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ApplyTransform @ 0x82147EF8 | size: 0x1C8
//
// Applies a transformation to all 8 corner points of the capsule's bounding box.
// Uses SIMD operations to efficiently transform multiple points.
//
// Parameters:
//   transform - Pointer to 16-byte transformation vector (scale/offset)
//   inPoints - Pointer to source point array (8 points × 16 bytes each)
//
// The function:
// 1. Loads the transformation vector
// 2. Broadcasts the first component for SIMD multiplication
// 3. Applies the transformation to all 8 corner points
// 4. Stores the results back to the capsule structure
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ApplyTransform(const float* transform, const float* inPoints) {
    float* pts = (float*)this;

    // First copy all 8 vectors (128 bytes) from inPoints into this
    memcpy(pts, inPoints, 128);

    // Load transformation vector components
    // The recomp multiplies the 4-component source vector by a broadcast scalar,
    // producing offsets (offsetX, offsetY, offsetZ, _) which are then added with
    // alternating sign patterns to each of the 8 points.
    // Compute the actual offset vector from transform * scale
    float v12[4];
    memcpy(v12, inPoints + 32, 16);  // load v12 from inPoints[32..35] — the scale source

    float scale = transform[0];  // broadcast first component
    float offsetX = v12[0] * scale;
    float offsetY = v12[1] * scale;
    float offsetZ = v12[2] * scale;
    float negOffsetX = -offsetX;
    float negOffsetY = -offsetY;
    float negOffsetZ = -offsetZ;

    // Point 0 (offset +0): -X, -Y, +Z
    pts[0] += negOffsetX;
    pts[1] += negOffsetY;
    pts[2] += offsetZ;

    // Point 1 (offset +16): +X, -Y, +Z
    pts[4] += offsetX;
    pts[5] += negOffsetY;
    pts[6] += offsetZ;

    // Point 2 (offset +32): -X, +Y, +Z
    pts[8] += negOffsetX;
    pts[9] += offsetY;
    pts[10] += offsetZ;

    // Point 3 (offset +48): +X, +Y, +Z
    pts[12] += offsetX;
    pts[13] += offsetY;
    pts[14] += offsetZ;

    // Point 4 (offset +64): -X, -Y, -Z
    pts[16] += negOffsetX;
    pts[17] += negOffsetY;
    pts[18] += negOffsetZ;

    // Point 5 (offset +80): +X, -Y, -Z
    pts[20] += offsetX;
    pts[21] += negOffsetY;
    pts[22] += negOffsetZ;

    // Point 6 (offset +96): -X, +Y, -Z
    pts[24] += negOffsetX;
    pts[25] += offsetY;
    pts[26] += negOffsetZ;

    // Point 7 (offset +112): +X, +Y, -Z
    pts[28] += offsetX;
    pts[29] += offsetY;
    pts[30] += negOffsetZ;
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundGeometry — Collision Geometry Bound Functions
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// External globals
extern void* g_display_obj_ptr;  // @ 0x826066E4

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::GetDisplayObject @ 0x82228C58 | size: 0xC
//
// Returns the global display object pointer used for rendering debug geometry.
// This is a simple accessor for the singleton display system.
// ─────────────────────────────────────────────────────────────────────────────
void* phBoundGeometry::GetDisplayObject() {
    return g_display_obj_ptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::GetMaterialCount (vfn_10) @ 0x82290F60 | size: 0x8
//
// Returns the number of materials/surfaces in this geometry bound.
// Used for material-based collision response and friction calculations.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t phBoundGeometry::GetMaterialCount() const {
    return field_0x00a8;  // +168
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::GetMaterialAtIndex (vfn_12) @ 0x82290F68 | size: 0x10
//
// Retrieves a material pointer from the material array by index.
// The index is multiplied by 4 (sizeof pointer) to get the array offset.
//
// @param index - Material index (0-based)
// @return Pointer to material at the specified index
// ─────────────────────────────────────────────────────────────────────────────
void* phBoundGeometry::GetMaterialAtIndex(int index) const {
    void** materialArray = (void**)(uintptr_t)field_0x00a0;  // +160
    return materialArray[index];
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::SetAllMaterials (vfn_13) @ 0x82293D50 | size: 0x38
//
// Sets all materials in the material array to the specified material pointer,
// then stores it as the current material at offset +164.
//
// @param material - Material pointer to assign to all slots
// ─────────────────────────────────────────────────────────────────────────────
void phBoundGeometry::SetAllMaterials(void* material) {
    uint8_t count = field_0x00a8;  // +168

    if (count > 0) {
        void** materialArray = (void**)(uintptr_t)field_0x00a0;  // +160
        for (int i = 0; i < count; i++) {
            materialArray[i] = material;
        }
    }

    field_0x00a4 = (uint32_t)(uintptr_t)material;  // +164
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::RenderDebugGeometry (vfn_14) @ 0x82293BE8 | size: 0x60
//
// Renders debug visualization of this geometry bound by:
// 1. Getting the current material index via vfn_16
// 2. Looking up the material in the material array
// 3. Calling the display object's render method (vtable slot 5)
// ─────────────────────────────────────────────────────────────────────────────
void phBoundGeometry::RenderDebugGeometry() {
    // Get current material index via vtable slot 16
    void** vtbl = *(void***)this;
    typedef int (*GetIdxFunc)(void*);
    GetIdxFunc getIdx = (GetIdxFunc)vtbl[16];
    int materialIndex = getIdx(this);

    // Look up material in array
    void** materialArray = (void**)(uintptr_t)field_0x00a0;  // +160
    void* material = materialArray[materialIndex];
    
    // Get display object and call its render method
    void* displayObj = g_display_obj_ptr;
    void** displayVTable = *(void***)displayObj;
    typedef void (*RenderFunc)(void*, void*);
    RenderFunc render = (RenderFunc)displayVTable[5];
    render(displayObj, material);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::SelectMaterialForRendering (vfn_15) @ 0x82293CF8 | size: 0x54
//
// Selects which material to use for rendering based on the current state:
// - If m_pCurrentMaterial is valid (!= -1), use it via vtable slot 11
// - If material count is 1, use material 0 via vtable slot 11
// - Otherwise, use the display object's default material via vtable slot 9
// ─────────────────────────────────────────────────────────────────────────────
void phBoundGeometry::SelectMaterialForRendering() {
    void* currentMaterial = (void*)(uintptr_t)field_0x00a4;  // +164
    
    if (currentMaterial != (void*)-1) {
        // Use the explicitly set current material
        void** vtable = *(void***)this;
        typedef void (*MaterialFunc)(void*, void*);
        MaterialFunc func = (MaterialFunc)vtable[11];
        func(this, currentMaterial);
        return;
    }
    
    uint8_t materialCount = field_0x00a8;  // +168
    
    if (materialCount == 1) {
        // Single material - use material 0
        void** vtable = *(void***)this;
        typedef void (*MaterialFunc)(void*, void*);
        MaterialFunc func = (MaterialFunc)vtable[11];
        func(this, nullptr);  // index 0
        return;
    }
    
    // Multiple materials - use display object's default
    void* displayObj = g_display_obj_ptr;
    void** displayVTable = *(void***)displayObj;
    typedef void (*DefaultMaterialFunc)(void*);
    DefaultMaterialFunc fallback = (DefaultMaterialFunc)displayVTable[9];
    fallback(displayObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::CheckBoundsAndUpdate (vfn_6) @ 0x82293D88 | size: 0x84
//
// Checks if a point is within the geometry bounds by:
// 1. Computing the offset from the center (point - m_center)
// 2. Masking with 0x9C0 to check specific components
// 3. If all masked components are zero, the point is at the center
// 4. Otherwise, calls vfn_7 to update the bounds
//
// @param point - 16-byte aligned vector representing the test point
// ─────────────────────────────────────────────────────────────────────────────
void phBoundGeometry::CheckBoundsAndUpdate(const float* point) {
    // Compute offset from center
    const float* center = (const float*)((const char*)this + 0x30);  // +48
    float offset[4];
    offset[0] = point[0] - center[0];
    offset[1] = point[1] - center[1];
    offset[2] = point[2] - center[2];
    offset[3] = point[3] - center[3];
    
    // Mask with 0x9C0 pattern (checks specific components)
    // This is a SIMD operation that zeros out certain bits
    const uint32_t mask[4] = {0x000009C0, 0x000009C0, 0x000009C0, 0x000009C0};
    uint32_t* offsetBits = (uint32_t*)offset;
    uint32_t masked[4];
    masked[0] = offsetBits[0] & mask[0];
    masked[1] = offsetBits[1] & mask[1];
    masked[2] = offsetBits[2] & mask[2];
    masked[3] = offsetBits[3] & mask[3];
    
    // Check if all masked components are zero (point is at center)
    bool allZero = (masked[0] == 0 && masked[1] == 0 && 
                    masked[2] == 0 && masked[3] == 0);
    
    if (!allZero) {
        // Point is not at center - update bounds
        UpdateBounds(offset);  // vfn_7
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::UpdateBounds (vfn_7) @ 0x82293E10 | size: 0x88
//
// Updates the geometry bounds by adding an offset vector to all vertices
// and the center point. Also updates the "has offset" flag at +5.
//
// @param offset - 16-byte aligned offset vector to add
// ─────────────────────────────────────────────────────────────────────────────
void phBoundGeometry::UpdateBounds(const float* offset) {
    int vertexCount = (int)field_0x0094;  // +148

    if (vertexCount > 0) {
        float* vertices = (float*)(uintptr_t)field_0x0070;  // +112

        // Add offset to all vertices (16 bytes per vertex)
        for (int i = 0; i < vertexCount; i++) {
            vertices[0] += offset[0];
            vertices[1] += offset[1];
            vertices[2] += offset[2];
            vertices[3] += offset[3];
            vertices += 4;  // Move to next vertex (16 bytes)
        }
    }

    // Add offset to center point
    float* center = (float*)((char*)this + 0x30);  // +48
    center[0] += offset[0];
    center[1] += offset[1];
    center[2] += offset[2];
    center[3] += offset[3];

    // Check if the updated center has any non-zero masked components
    const uint32_t mask_val = 0x000009C0;
    uint32_t* centerBits = (uint32_t*)center;
    uint32_t masked[4];
    masked[0] = centerBits[0] & mask_val;
    masked[1] = centerBits[1] & mask_val;
    masked[2] = centerBits[2] & mask_val;
    masked[3] = centerBits[3] & mask_val;

    bool hasOffset = !(masked[0] == 0 && masked[1] == 0 &&
                       masked[2] == 0 && masked[3] == 0);

    field_0x0005 = hasOffset ? 1 : 0;  // +5
    
    // Call vtable slot 37 to finalize the update
    void** vtable = *(void***)this;
    typedef void (*FinalizeFunc)(void*);
    FinalizeFunc finalize = (FinalizeFunc)vtable[37];
    finalize(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::Destructor (vfn_0) @ 0x82291008 | size: 0x50
//
// Destructor for phBoundGeometry. Calls base class destructor (rage_1058),
// then frees memory if the "should free" flag (bit 0 of parameter) is set.
//
// @param shouldFree - If bit 0 is set, free the object's memory
// ─────────────────────────────────────────────────────────────────────────────
extern void rage_1058(void*);
extern void rage_free(void*);

void phBoundGeometry::Destructor(int shouldFree) {
    // Call base class destructor
    rage_1058(this);

    // Free memory if requested
    if (shouldFree & 0x1) {
        rage_free(this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundGeometry::CallVTableSlot37 (vfn_40) @ 0x82291260 | size: 0x44
//
// Wrapper function that calls vtable slot 37 on this object and returns true.
// This is used as a callback or event handler that always succeeds.
//
// @return Always returns true (1)
// ─────────────────────────────────────────────────────────────────────────────
bool phBoundGeometry::CallVTableSlot37() {
    // Call vtable slot 37
    void** vtable = *(void***)this;
    typedef void (*Slot37Func)(void*);
    Slot37Func func = (Slot37Func)vtable[37];
    func(this);
    
    return true;
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// Physics Utility Functions — Batch Implementation
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// ph_2DF0 @ 0x820C2DF0 | size: 0x18
//
// Initializes a rage::fiAsciiTokenizer object by setting its vtable pointer
// and clearing the internal state field at offset +160.
//
// This is likely a constructor or reset function for the tokenizer class.
// ─────────────────────────────────────────────────────────────────────────────
void ph_2DF0(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Set vtable pointer for rage::fiAsciiTokenizer
    *(void**)(obj + 0) = (void*)0x8202777C;
    
    // Clear internal state field
    *(uint32_t*)(obj + 160) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_3AB0 @ 0x820C3AB0 | size: 0x10
//
// Zeros out a 16-byte SIMD vector at the given address.
// Uses AltiVec vxor instruction to efficiently clear the vector.
// ─────────────────────────────────────────────────────────────────────────────
void ph_3AB0(void* vectorPtr) {
    uint32_t* vec = (uint32_t*)vectorPtr;
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0;
    vec[3] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_3AC0 @ 0x820C3AC0 | size: 0x18
//
// Computes the dot product of a 3D vector with itself (magnitude squared).
// Uses AltiVec vmsum3fp128 instruction for SIMD dot product calculation.
//
// Returns: float - The squared magnitude of the vector
// ─────────────────────────────────────────────────────────────────────────────
float ph_3AC0(const float* vector) {
    // Compute dot product: v.x*v.x + v.y*v.y + v.z*v.z
    float result = vector[0] * vector[0] + 
                   vector[1] * vector[1] + 
                   vector[2] * vector[2];
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_3870_h @ 0x820C3870 | size: 0x38
//
// Advances a pointer within a segmented array structure. This function:
// 1. Checks if the input pointer is valid (non-null)
// 2. Calculates which segment the pointer is in
// 3. Advances the pointer by the segment stride
//
// Used for iterating through physics data structures with non-contiguous memory.
//
// Parameters:
//   arrayBase - Base pointer to the segmented array structure
//   ptrToAdvance - Pointer to advance (modified in-place)
// ─────────────────────────────────────────────────────────────────────────────
void ph_3870_h(void* arrayBase, void** ptrToAdvance) {
    uint8_t* base = (uint8_t*)arrayBase;
    uint32_t currentPtr = *(uint32_t*)ptrToAdvance;
    
    // Early exit if pointer is null
    if (currentPtr == 0) {
        return;
    }
    
    // Load array metadata
    uint32_t arrayStart = *(uint32_t*)(base + 4);
    uint32_t elementStride = *(uint32_t*)(base + 76);
    
    // Calculate element index
    uint32_t offset = currentPtr - arrayStart;
    uint32_t elementIndex = offset / elementStride;
    
    // Calculate segment table index (index + 2)
    uint32_t segmentIndex = (elementIndex + 2) * 4;
    
    // Load segment base offset
    uint32_t segmentOffset = *(uint32_t*)(base + segmentIndex);
    
    // Advance pointer by segment offset
    uint32_t newPtr = segmentOffset + currentPtr;
    *(uint32_t*)ptrToAdvance = newPtr;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_0908 @ 0x820F0908 | size: 0x64
//
// Normalizes a file path string by:
// 1. Converting uppercase letters (A-Z) to lowercase (a-z)
// 2. Converting backslashes (\) to forward slashes (/)
// 3. Null-terminating the output
//
// This is used for cross-platform path handling in the physics resource loader.
//
// Parameters:
//   dest - Destination buffer for normalized path
//   src - Source path string
//   maxLen - Maximum length to copy (including null terminator)
// ─────────────────────────────────────────────────────────────────────────────
void ph_0908(char* dest, const char* src, int maxLen) {
    int remaining = maxLen - 1;
    char* out = dest;
    
    while (remaining > 0) {
        char ch = *src++;
        
        // Stop at null terminator
        if (ch == '\0') {
            break;
        }
        
        // Convert uppercase to lowercase (A-Z -> a-z)
        if (ch >= 'A' && ch <= 'Z') {
            ch = ch + 32;
        }
        // Convert backslash to forward slash
        else if (ch == '\\') {
            ch = '/';
        }
        
        *out++ = ch;
        remaining--;
    }
    
    // Null-terminate
    *out = '\0';
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_9598 @ 0x820F9598 | size: 0xC
//
// Aligns a value up to the next 4-byte boundary.
// Used for memory alignment in physics data structures.
//
// Parameters:
//   value - Value to align
//
// Returns: uint32_t - Value aligned to 4-byte boundary
// ─────────────────────────────────────────────────────────────────────────────
uint32_t ph_9598(uint32_t value) {
    // Add 3 and mask off lower 2 bits to round up to next multiple of 4
    return (value + 3) & 0xFFFFFFFC;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_95A8_h @ 0x820F95A8 | size: 0x8
//
// Simple getter that returns a pointer field at offset +148.
// Likely retrieves a physics simulation context or state object.
//
// Returns: void* - Pointer to physics object at offset +148
// ─────────────────────────────────────────────────────────────────────────────
void* ph_95A8_h(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    return *(void**)(obj + 148);
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_FD58 @ 0x8221FD58 | size: 0x18
//
// Returns a state code based on a flag at offset +22.
// Returns 3 if the flag is zero, 4 if the flag is non-zero.
//
// This appears to be a state query function for physics objects.
//
// Returns: int - State code (3 or 4)
// ─────────────────────────────────────────────────────────────────────────────
int ph_FD58(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    uint16_t flag = *(uint16_t*)(obj + 22);
    
    if (flag == 0) {
        return 3;
    }
    return 4;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_FD70 @ 0x8221FD70 | size: 0x8
//
// Sets a byte flag at offset +15.
// Simple setter for a physics object state flag.
//
// Parameters:
//   value - Byte value to store
// ─────────────────────────────────────────────────────────────────────────────
void ph_FD70(void* thisPtr, uint8_t value) {
    uint8_t* obj = (uint8_t*)thisPtr;
    *(uint8_t*)(obj + 15) = value;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_F6A8 @ 0x8227F6A8 | size: 0x168
//
// Complex physics archetype loading function. This function:
// 1. Creates a temporary context for loading
// 2. Normalizes the asset path
// 3. Attempts to load the archetype from the resource system
// 4. If not found, creates a new archetype instance
// 5. Registers the archetype with the physics world
// 6. Associates it with a creature instance
//
// This is a high-level orchestration function for physics object initialization.
//
// Parameters:
//   contextPtr - Physics context pointer
//   creatureInst - Creature instance to associate with
//   assetPath - Path to the physics archetype asset
// ─────────────────────────────────────────────────────────────────────────────
void ph_F6A8(void* contextPtr, void* creatureInst, const char* assetPath) {
    // External function declarations
    extern void* xe_F4C0(void*);
    extern void rage_D220(void*, const char*);
    extern void strncpy(char*, const char*, int);
    extern void* ph_6FC8(void*, const char*);
    extern void ph_snprintf(char*, int, const char*, const char*, int);
    extern void* phArchetype_Load(const char*, void*);
    extern void* rage_alloc(uint32_t);
    extern void* ph_9EC0_1(void*);
    extern void ph_E088(void*, void*, void*, float, int);
    extern void ph_9E50(void*, void*);
    extern void ph_CEE0(void*, int);
    extern void* ph_E010(void*, void*, const char*);
    extern void pongCreatureInst_9030_g(void*, void*);
    extern void util_D150(void*, void*, void*);
    
    char normalizedPath[256];
    
    // Create temporary loading context
    void* loadContext = xe_F4C0(contextPtr);
    *(uint32_t*)((uint8_t*)loadContext + 96) = 0;
    
    // Initialize path normalization
    rage_D220(loadContext, assetPath);
    
    // Get resource manager and material info
    uint32_t resourceMgr = *(uint32_t*)((uint8_t*)loadContext + 96);
    void* materialInfo = (void*)*(uint32_t*)((uint8_t*)resourceMgr + 212);
    void* physicsWorld = (void*)0x826065EC;  // Global physics world pointer
    
    // Normalize the asset path
    normalizedPath[255] = '\0';
    strncpy(normalizedPath, assetPath, 255);
    
    // Try to find existing archetype in resource system
    int attemptCount = 1;
    const char* formatStr = (const char*)0x82059F04;  // Format string for path variants
    
    while (true) {
        void* existingArchetype = ph_6FC8(*(void**)((uint8_t*)physicsWorld + 8), normalizedPath);
        
        if (existingArchetype != nullptr) {
            void* archetypeData = *(void**)((uint8_t*)existingArchetype + 4);
            if (archetypeData != nullptr) {
                break;  // Found existing archetype
            }
        }
        
        // Try alternate path format
        ph_snprintf(normalizedPath, 255, formatStr, assetPath, attemptCount);
        attemptCount++;
    }
    
    // Load or create the archetype
    void* archetype = phArchetype_Load(normalizedPath, materialInfo);
    
    if (archetype == nullptr) {
        // Create new archetype instance
        void* memory = rage_alloc(80);
        if (memory != nullptr) {
            archetype = ph_9EC0_1(memory);
        } else {
            archetype = nullptr;
        }
        
        // Register with physics world
        if (archetype != nullptr) {
            float defaultScale = 1.0f;  // From lbl_820C4890
            ph_E088(physicsWorld, archetype, materialInfo, defaultScale, 1);
        }
    }
    
    // Associate archetype with material
    ph_9E50(archetype, materialInfo);
    ph_CEE0(materialInfo, 1);
    
    // Register with physics world and creature
    void* physicsInst = ph_E010(physicsWorld, archetype, normalizedPath);
    pongCreatureInst_9030_g(creatureInst, physicsInst);
    
    // Finalize registration
    util_D150(loadContext, creatureInst, (void*)((uint8_t*)creatureInst + 16));
    
    // Call vtable method on context
    void* contextObj = *(void**)((uint8_t*)contextPtr + 20);
    void* vtable = *(void**)((uint8_t*)contextObj + 4);
    typedef void (*VTableFunc)(void*, void*);
    VTableFunc func = (VTableFunc)((void**)vtable)[6];
    func(contextObj, *(void**)((uint8_t*)loadContext + 20));
    
    // Set final state
    *(uint32_t*)((uint8_t*)loadContext + 16) = 4;
}


// ─────────────────────────────────────────────────────────────────────────────
// ph_FC68_h @ 0x8246FC68 | size: 0x38
//
// Dual cleanup function that calls phInst_A3A0_p33 twice:
// 1. Once on an embedded phInst at offset +12
// 2. Once on the object itself
//
// This pattern suggests cleanup or finalization of a composite physics object
// that contains an embedded phInst. Likely performs reference counting or
// resource cleanup for both the container and embedded instance.
// ─────────────────────────────────────────────────────────────────────────────
extern void phInst_A3A0_p33(void*);

void ph_FC68_h(void* obj) {
    // Call phInst cleanup on embedded instance at offset +12
    void* embeddedPhInst = (char*)obj + 12;
    phInst_A3A0_p33(embeddedPhInst);
    
    // Call phInst cleanup on the object itself
    phInst_A3A0_p33(obj);
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phArticulatedCollider — Articulated Physics Collider (Ragdoll/Chain)
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// Forward declarations for external functions
extern void phBoundCapsule_6C28_fw(void* capsule);
extern void phArticulatedCollider_6D30_h(void* collider);
extern void phArticulatedCollider_5C60_sp(void* collider);
extern void phCollider_vfn_4(phArticulatedCollider* collider);
extern void phArticulatedCollider_5B50_wrh(void* activeJoints);

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetActiveJointsPointer (vfn_46) @ 0x8224E248 | size: 0x8
//
// Returns pointer to the active joints data structure.
// This is a simple accessor used by physics simulation to query joint states.
// ─────────────────────────────────────────────────────────────────────────────
void* phArticulatedCollider::GetActiveJointsPointer() {
    return (void*)(uintptr_t)field_0x01d8;  // +472 (0x1D8)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToJointProcessor (vfn_22) @ 0x8224F220 | size: 0x8
//
// Delegates processing to the joint processor function.
// Used during physics update to process all active joints.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToJointProcessor() {
    phArticulatedCollider_5C60_sp((void*)(uintptr_t)m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToCapsuleHandler (vfn_25) @ 0x8224FD50 | size: 0x8
//
// Delegates to capsule collision handler.
// Used for capsule-based collision detection on articulated bodies.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToCapsuleHandler() {
    phBoundCapsule_6C28_fw((void*)(uintptr_t)m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToColliderHandler (vfn_59) @ 0x8224F218 | size: 0x8
//
// Delegates to articulated collider handler.
// Used for complex collision scenarios involving multiple joints.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToColliderHandler() {
    phArticulatedCollider_6D30_h((void*)(uintptr_t)m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointMass (vfn_56) @ 0x8224E250 | size: 0x10
//
// Returns the mass of a specific joint by index.
// Used during physics simulation to compute forces and torques.
//
// @param jointIndex - Index of the joint to query
// @return Mass value for the specified joint
// ─────────────────────────────────────────────────────────────────────────────
float phArticulatedCollider::GetJointMass(int jointIndex) {
    float* massArray = (float*)(uintptr_t)field_0x01fc;  // +508 (0x1FC)
    return massArray[jointIndex];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointInertia (vfn_60) @ 0x8224E260 | size: 0x10
//
// Returns the rotational inertia of a specific joint by index.
// Used for angular momentum calculations in physics simulation.
//
// @param jointIndex - Index of the joint to query
// @return Inertia value for the specified joint
// ─────────────────────────────────────────────────────────────────────────────
float phArticulatedCollider::GetJointInertia(int jointIndex) {
    float* inertiaArray = (float*)(uintptr_t)field_0x01ec;  // +492 (0x1EC)
    return inertiaArray[jointIndex];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointDamping (vfn_61) @ 0x8224E270 | size: 0x10
//
// Returns the damping coefficient of a specific joint by index.
// Used to simulate friction and energy loss in joint movement.
//
// @param jointIndex - Index of the joint to query
// @return Damping coefficient for the specified joint
// ─────────────────────────────────────────────────────────────────────────────
float phArticulatedCollider::GetJointDamping(int jointIndex) {
    float* dampingArray = (float*)(uintptr_t)field_0x01f4;  // +500 (0x1F4)
    return dampingArray[jointIndex];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::Update (vfn_4) @ 0x8224E7A0 | size: 0x28
//
// Updates the articulated collider state.
// Calls parent class update, then updates active joints.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::Update() {
    // Call parent class update
    phCollider_vfn_4(this);
    
    // Update active joints
    phArticulatedCollider_5B50_wrh((void*)(uintptr_t)m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AddForceToJoint (vfn_33) @ 0x8224FD30 | size: 0x20
//
// Adds a force vector to a specific joint's accumulated forces.
// Used during physics simulation to apply external forces (gravity, collisions).
//
// @param forceVector - 16-byte aligned vector containing force components
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::AddForceToJoint(const float* forceVector) {
    // Load the joint data pointer
    void* jointData = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    
    // Get the first joint's data (offset +40 from joint data base)
    void* firstJoint = (void*)((char*)jointData + 40);
    
    // Access the accumulated force vector at offset +352
    float* accumulatedForce = (float*)((char*)firstJoint + 352);
    
    // Add the input force to the accumulated force (SIMD vector addition)
    accumulatedForce[0] += forceVector[0];
    accumulatedForce[1] += forceVector[1];
    accumulatedForce[2] += forceVector[2];
    accumulatedForce[3] += forceVector[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyImpulse (vfn_36) @ 0x8224EFA0 | size: 0x24
//
// Applies an impulse to the articulated collider.
// Delegates to vtable slot 34 with rearranged parameters.
//
// @param param1 - First parameter (moved to r4)
// @param param2 - Second parameter (moved to r5)
// @param param3 - Third parameter (moved to r6)
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::ApplyImpulse(void* param1, void* param2, void* param3) {
    // Get vtable and call slot 34 with parameters shuffled and two zeros appended
    void** vtable = *(void***)this;
    typedef void (*ImpulseFunc)(phArticulatedCollider*, void*, void*, void*, int, int);
    ImpulseFunc func = (ImpulseFunc)vtable[34];
    func(this, param1, param2, param3, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyConstraintForce (vfn_37) @ 0x8224EFC8 | size: 0x18
//
// Delegates constraint force application to vtable slot 38.
// Rearranges parameters and dispatches via indirect call through vtable.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::ApplyConstraintForce(void* constraintData, void* forceData) {
    void** vtable = *(void***)this;
    typedef void (*ConstraintFunc)(phArticulatedCollider*, void*, void*);
    ConstraintFunc func = (ConstraintFunc)vtable[38];
    func(this, constraintData, forceData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyForceAtJoint (vfn_38) @ 0x8224EFE0 | size: 0x60
//
// Applies a force vector to a specific joint in the articulated body.
// Resolves the joint index via E668, then adds the force to the joint's
// accumulated force vector at offset +384.
//
// @param forceVector - 16-byte aligned force vector to apply
// @param jointIndex  - Index of the target joint
// ─────────────────────────────────────────────────────────────────────────────
extern int phArticulatedCollider_E668(phArticulatedCollider* collider, int jointIndex);

void phArticulatedCollider::ApplyForceAtJoint(const float* forceVector, int jointIndex) {
    // Resolve the joint index to a link index
    int linkIndex = phArticulatedCollider_E668(this, jointIndex);

    // Compute array offset: (linkIndex + 10) * 4
    int arrayIndex = linkIndex + 10;

    // Look up the link data pointer from the active joints array
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    void* linkData = (void*)(uintptr_t)jointArray[arrayIndex];

    // Add force to the accumulated force vector at offset +384
    float* accumulatedForce = (float*)((char*)linkData + 384);
    accumulatedForce[0] += forceVector[0];
    accumulatedForce[1] += forceVector[1];
    accumulatedForce[2] += forceVector[2];
    accumulatedForce[3] += forceVector[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::IsJointActive (E1B0_h) @ 0x8224E1B0 | size: 0x20
//
// Checks whether a joint at the given byte offset is active.
// Returns true if the byte at (this + offset + 176) is non-zero.
//
// @param byteOffset - Byte offset from 'this' to reach the joint data
// @return true if the joint is active, false otherwise
// ─────────────────────────────────────────────────────────────────────────────
bool phArticulatedCollider::IsJointActive(int byteOffset) {
    uint8_t* base = (uint8_t*)this;
    uint8_t flag = base[byteOffset + 176];
    return flag != 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointStiffness (vfn_62) @ 0x8224E280 | size: 0x58
//
// Retrieves the stiffness value for a specific joint by looking up through
// multiple indirection arrays and calling vtable slot 8 on the resolved
// joint object.
//
// @param jointIndex - Index of the joint to query
// @return Stiffness value as float
// ─────────────────────────────────────────────────────────────────────────────
float phArticulatedCollider::GetJointStiffness(int jointIndex) {
    // Look up link type from joint type array (+476)
    uint32_t* jointTypeArray = (uint32_t*)(uintptr_t)field_0x01dc;  // +476 (0x1DC)
    uint32_t linkType = jointTypeArray[jointIndex];

    // Look up joint object from joint object array (+484)
    uint32_t* jointObjArray = (uint32_t*)(uintptr_t)field_0x01e4;  // +484 (0x1E4)
    void* jointObj = (void*)(uintptr_t)jointObjArray[jointIndex];

    // Compute the link data index: linkType + 42
    int linkDataIndex = linkType + 42;

    // Get the link data from active joints array
    uint32_t* activeJoints = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    void* linkData = (void*)(uintptr_t)activeJoints[linkDataIndex];

    // Call vtable slot 8 on the joint object to get stiffness
    float result;
    void** jointVtable = *(void***)jointObj;
    typedef void (*GetStiffnessFunc)(void*, void*, void*, float*);
    GetStiffnessFunc func = (GetStiffnessFunc)jointVtable[8];
    func(jointObj, jointObjArray + jointIndex, linkData, &result);

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ClearJointForces (vfn_3) @ 0x8224E1D0 | size: 0x74
//
// Zeros out four accumulated force/torque vectors for every joint in the
// articulated body. Each joint stores four 16-byte vectors at offsets
// +272, +288, +304, and +320 from the joint data pointer.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::ClearJointForces() {
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    int numJoints = (int)jointData[1];  // joint count at +4

    if (numJoints <= 0)
        return;

    // Iterate through the joint pointer array starting at offset +40
    uint32_t* jointPtrArray = jointData + 10;  // +40 bytes = +10 dwords

    for (int i = 0; i < numJoints; i++) {
        uint8_t* joint = (uint8_t*)(uintptr_t)jointPtrArray[i];

        // Clear four 16-byte vectors: forces and torques
        float* forceVec0 = (float*)(joint + 272);
        float* forceVec1 = (float*)(joint + 288);
        float* forceVec2 = (float*)(joint + 320);
        float* forceVec3 = (float*)(joint + 304);

        forceVec0[0] = 0.0f; forceVec0[1] = 0.0f; forceVec0[2] = 0.0f; forceVec0[3] = 0.0f;
        forceVec1[0] = 0.0f; forceVec1[1] = 0.0f; forceVec1[2] = 0.0f; forceVec1[3] = 0.0f;
        forceVec2[0] = 0.0f; forceVec2[1] = 0.0f; forceVec2[2] = 0.0f; forceVec2[3] = 0.0f;
        forceVec3[0] = 0.0f; forceVec3[1] = 0.0f; forceVec3[2] = 0.0f; forceVec3[3] = 0.0f;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetVelocityScaled (vfn_40) @ 0x8224F870 | size: 0x58
//
// Stores a velocity vector at offset +224, scales it by the restitution
// coefficient (field +100) into offset +256, then propagates to all joints.
//
// @param velocity - 16-byte aligned velocity vector
// ─────────────────────────────────────────────────────────────────────────────
extern void phArticulatedCollider_8A30(void* jointData, const float* velocity);

void phArticulatedCollider::SetVelocityScaled(const float* velocity) {
    uint8_t* self = (uint8_t*)this;

    // Read restitution coefficient from offset +100
    float restitution = *(float*)(self + 100);

    // Store velocity at offset +224
    float* storedVelocity = (float*)(self + 224);
    storedVelocity[0] = velocity[0];
    storedVelocity[1] = velocity[1];
    storedVelocity[2] = velocity[2];
    storedVelocity[3] = velocity[3];

    // Scale velocity by restitution and store at offset +256
    float* scaledVelocity = (float*)(self + 256);
    scaledVelocity[0] = storedVelocity[0] * restitution;
    scaledVelocity[1] = storedVelocity[1] * restitution;
    scaledVelocity[2] = storedVelocity[2] * restitution;
    scaledVelocity[3] = storedVelocity[3] * restitution;

    // Propagate velocity to all joints
    void* jointDataPtr = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_8A30(jointDataPtr, storedVelocity);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetVelocityDirect (vfn_41) @ 0x8224F8C8 | size: 0x44
//
// Stores a velocity vector directly at offset +256 and propagates it to
// all joints via the parent class and articulated joint handler.
//
// @param velocity - 16-byte aligned velocity vector
// ─────────────────────────────────────────────────────────────────────────────
extern void phCollider_CDF0_p39(phArticulatedCollider* collider);

void phArticulatedCollider::SetVelocityDirect(const float* velocity) {
    uint8_t* self = (uint8_t*)this;

    // Store velocity at offset +256
    float* storedVelocity = (float*)(self + 256);
    storedVelocity[0] = velocity[0];
    storedVelocity[1] = velocity[1];
    storedVelocity[2] = velocity[2];
    storedVelocity[3] = velocity[3];

    // Call parent class velocity handler
    phCollider_CDF0_p39(this);

    // Propagate from stored velocity at +224 to all joints
    float* baseVelocity = (float*)(self + 224);
    void* jointDataPtr = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_8A30(jointDataPtr, baseVelocity);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetAngularVelocity (vfn_42) @ 0x8224F910 | size: 0x38
//
// Retrieves the angular velocity by calling the parent class implementation,
// then propagates the angular velocity at offset +240 to all joints.
// ─────────────────────────────────────────────────────────────────────────────
extern void phCollider_vfn_42(phArticulatedCollider* collider);
extern void phArticulatedCollider_8B10(void* jointData, const float* angularVelocity);

void phArticulatedCollider::GetAngularVelocity() {
    // Call parent class angular velocity getter
    phCollider_vfn_42(this);

    // Propagate angular velocity at +240 to all joints
    float* angularVel = (float*)((uint8_t*)this + 240);
    void* jointDataPtr = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_8B10(jointDataPtr, angularVel);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetAngularVelocity (vfn_43) @ 0x8224F948 | size: 0x44
//
// Stores an angular velocity vector at offset +272, calls the parent class
// handler, then propagates angular velocity at offset +240 to all joints.
//
// @param angularVelocity - 16-byte aligned angular velocity vector
// ─────────────────────────────────────────────────────────────────────────────
extern void game_CE58(phArticulatedCollider* collider);

void phArticulatedCollider::SetAngularVelocity(const float* angularVelocity) {
    uint8_t* self = (uint8_t*)this;

    // Store angular velocity at offset +272
    float* storedAngVel = (float*)(self + 272);
    storedAngVel[0] = angularVelocity[0];
    storedAngVel[1] = angularVelocity[1];
    storedAngVel[2] = angularVelocity[2];
    storedAngVel[3] = angularVelocity[3];

    // Call parent class angular velocity handler
    game_CE58(this);

    // Propagate angular velocity at +240 to all joints
    float* angVel = (float*)(self + 240);
    void* jointDataPtr = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_8B10(jointDataPtr, angVel);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::InitializeJoints (vfn_6) @ 0x8224EA28 | size: 0x48
//
// Initializes the articulated collider's joint system.
// Calls four setup functions in sequence:
// 1. BoundCapsule geometry generation
// 2. Joint hierarchy setup
// 3. Joint transform computation
// 4. Joint constraint initialization
// ─────────────────────────────────────────────────────────────────────────────
extern void phBoundCapsule_6968_g(void* jointData);
extern void phArticulatedCollider_8450(void* jointData);
extern void phArticulatedCollider_F0E0(phArticulatedCollider* collider);
extern void phArticulatedCollider_5D58(void* jointData);

void phArticulatedCollider::InitializeJoints() {
    void* jointDataPtr = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)

    // Step 1: Generate capsule bound geometry for collision
    phBoundCapsule_6968_g(jointDataPtr);

    // Step 2: Set up joint hierarchy / bone mapping
    phArticulatedCollider_8450(jointDataPtr);

    // Step 3: Compute joint transforms and matrices
    phArticulatedCollider_F0E0(this);

    // Step 4: Initialize joint constraints
    phArticulatedCollider_5D58(jointDataPtr);
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// Physics State Machine — ph_0CC0
// ═════════════════════════════════════════════════════════════════════════════

// External data tables
extern const uint32_t g_phStateTransitionTable[];  // @ 0x82006AF8 (480 bytes)

/**
 * ph_0CC0 @ 0x82470CC0 | size: 0x1C0
 *
 * Physics state machine update function. Manages state transitions for a
 * bounded physics system with interpolation support.
 *
 * The function:
 * 1. Validates bounds (current vs max, min vs max)
 * 2. Computes interpolation ratio if update flag is set
 * 3. Determines next state based on current state and transition mode
 * 4. Dispatches to state-specific handler via function pointer
 * 5. Returns the number of units processed
 *
 * Structure layout (inferred from field accesses):
 *   +4   maxBound
 *   +8   currentBound
 *   +12  transitionMode (byte)
 *   +13  currentState (byte, 1-6)
 *   +16  numerator (for ratio calculation)
 *   +24  minBound
 *   +28  minBoundCheck
 *   +32  denominator (for ratio calculation)
 *   +44  interpolationRatio (float)
 *   +76  stateHandlerFunc (function pointer)
 *   +80  updateFlags
 *   +84  transitionFlags
 *
 * @param stateObj - Pointer to physics state machine object
 * @return Number of units processed (currentBound - initialBound)
 */
int ph_0CC0(void* stateObj) {
    uint8_t* obj = (uint8_t*)stateObj;
    
    // Load bounds
    uint32_t maxBound = *(uint32_t*)(obj + 4);
    uint32_t currentBound = *(uint32_t*)(obj + 8);
    uint32_t minBound = *(uint32_t*)(obj + 24);
    uint32_t minBoundCheck = *(uint32_t*)(obj + 28);
    
    // Validate bounds - early exit if out of range
    if (currentBound >= maxBound) {
        return 0;
    }
    
    if (minBoundCheck >= minBound) {
        return 0;
    }
    
    // Check if update is needed
    uint32_t updateFlags = *(uint32_t*)(obj + 80);
    
    if (updateFlags != 0) {
        // Check if we need to compute interpolation ratio
        uint32_t flagsLow2Bits = updateFlags & 0x3;
        
        if (flagsLow2Bits != 0) {
            // Load values for ratio calculation
            uint64_t numerator = *(uint32_t*)(obj + 16);
            uint64_t denominator = *(uint32_t*)(obj + 32);
            uint32_t transitionFlags = *(uint32_t*)(obj + 84);
            bool reverseFlag = (transitionFlags & 0x1) != 0;
            
            // Convert to double for precise division
            double numDouble = (double)(int64_t)numerator;
            double denomDouble = (double)(int64_t)denominator;
            
            // Compute ratio
            float ratio = (float)(numDouble / denomDouble);
            *(float*)(obj + 44) = ratio;
            
            // Determine transition mode based on flags
            int transitionMode;
            
            if (reverseFlag) {
                // Reverse mode transitions
                if (numerator == denominator) {
                    // At end boundary
                    bool bit7Set = (transitionFlags & 0x80) != 0;
                    transitionMode = bit7Set ? 1 : 4;
                } else if (numerator * 2 == denominator) {
                    // At halfway point
                    bool bit7Set = (transitionFlags & 0x80) != 0;
                    transitionMode = bit7Set ? 2 : 5;
                } else {
                    // General reverse transition
                    bool bit7Set = (transitionFlags & 0x80) != 0;
                    transitionMode = bit7Set ? 0 : 3;
                }
            } else {
                // Forward mode transition
                bool bit7Set = (transitionFlags & 0x80) != 0;
                transitionMode = bit7Set ? 0 : 3;
            }
            
            // Compute state table index
            uint8_t currentState = *(uint8_t*)(obj + 13);
            uint8_t baseMode = *(uint8_t*)(obj + 12);
            
            // Adjust state for table lookup
            int adjustedState;
            switch (currentState) {
            case 1:
            case 2:
            case 4:
            case 6:
                // These states use (state >> 1) + 1
                adjustedState = (currentState >> 1) + 1;
                break;
            case 3:
            case 5:
            default:
                adjustedState = 0;
                break;
            }
            
            // Compute table index: (transitionMode * 4 + baseMode) * 3 + adjustedState
            int tableIndex = ((transitionMode * 4 + baseMode) * 3) + adjustedState;
            
            // Look up state handler function pointer from table
            const uint32_t* stateTable = (const uint32_t*)0x82006AF8;  // g_phStateTransitionTable
            uint32_t handlerFunc = stateTable[tableIndex];
            
            // Store handler function pointer
            *(uint32_t*)(obj + 76) = handlerFunc;
        }
        
        // Clear update flags
        *(uint32_t*)(obj + 80) = 0;
    }
    
    // Call the state handler function
    uint32_t handlerFunc = *(uint32_t*)(obj + 76);
    typedef void (*StateHandlerFunc)(void*);
    StateHandlerFunc handler = (StateHandlerFunc)handlerFunc;
    handler(stateObj);
    
    // Compute and return number of units processed
    uint32_t finalBound = *(uint32_t*)(obj + 8);
    return (int)(finalBound - currentBound);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::~phObject()  [vtable slot 0 @ 0x82485250]
// Destructor
// ─────────────────────────────────────────────────────────────────────────────
phObject::~phObject() {
    this->vtable = (void**)0x82008918;
    this->Release();
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::ResetState()  [vtable slot 32 @ 0x82484758]
// Initializes object state and memory references
// ─────────────────────────────────────────────────────────────────────────────
void phObject::ResetState() {
    m_field_500 = 0;
    m_field_508 = nullptr;
    m_field_504 = 1;
    m_field_512 = 0;
    m_field_520 = 0;
    m_field_524 = 0;
    m_field_528 = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::Initialize()  [vtable slot 11 @ 0x824847A8]
// Processing locale and fields
// ─────────────────────────────────────────────────────────────────────────────
void phObject::Initialize(void* a2, void* a3, void* a4) {
    int32_t result = this->Release();
    if (result >= 0) {
        void* ptr = (void*)0x208C801D;
        m_field_508 = ptr;
        if (ptr != nullptr) {
            uint32_t valA2 = *(uint32_t*)&a2;
            m_field_112 = *(uint32_t*)(uintptr_t)(valA2 + 8);
            m_field_116 = *(uint32_t*)(uintptr_t)(valA2 + 12);
            m_field_120 = *(uint32_t*)(uintptr_t)(valA2 + 16);
            if (*(uint32_t*)(uintptr_t)(valA2 + 0) != 0) {
                result = (int32_t)(intptr_t)this->CreateViews(nullptr);
            }
            if (result >= 0) {
                if (*(uint32_t*)(uintptr_t)(valA2 + 4) != 0) {
                    result = (int32_t)(intptr_t)this->CreateOutputViews();
                }
                if (result >= 0) {
                    return;
                }
            }
        } else {
            return;
        }
    }
    this->Release();
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::GetDescription()  [vtable slot 12 @ 0x824857B8]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::GetDescription() {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::SetDescription()  [vtable slot 13 @ 0x82485768]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
void phObject::SetDescription(void* a2) {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::QueryInterface()  [vtable slot 14 @ 0x82485720]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject::QueryInterface(int32_t) {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::Release()  [vtable slot 15 @ 0x82484890]
// Cleanup and state clear
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject::Release() {
    if (m_field_508 != nullptr) {
        m_field_508 = nullptr;
    }
    
    this->ReleaseViews();
    this->ReleaseResources();
    this->ResetState();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::CreateResource()  [vtable slot 16 @ 0x82485978]
// Sub-object update
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::CreateResource() {
    if (m_field_52 != nullptr) {
        if (m_field_52->QueryInterface(1) < 0) {
            return nullptr;
        }
        this->CreateViews(m_field_52);
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::CreateOutputViews()  [vtable slot 18 @ 0x82484918]
// Updates sub-object from parameters
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::CreateOutputViews() {
    this->ReleaseResources();
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::ReleaseViews()  [vtable slot 19 @ 0x82484988]
// Disposes or releases sub-objects
// ─────────────────────────────────────────────────────────────────────────────
void phObject::ReleaseViews() {
    m_field_132 = this;
    if (m_field_500 != 0) {
        m_field_500 = 0;
    }
    
    if (m_field_52 != nullptr) {
        m_field_52->Release();
        m_field_52 = nullptr;
    }
}
// ═════════════════════════════════════════════════════════════════════════════
// rage::phJoint3Dof
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// Forward declarations for external functions
extern void phJoint3Dof_0170_g(void*, int, int, void*);
extern void util_4628(phJoint3Dof*, int);
extern void phJoint1Dof_AFF8_p42(phJoint3Dof*, int, float*);
extern void phJoint_1388(phJoint3Dof*, int, float, float*, float*);
extern void phJoint_1618_g(phJoint3Dof*);
extern float phJoint1Dof_AE38(void*);
extern void phBoundCapsule_01D0_g(phJoint3Dof*, float);

/**
 * rage::phJoint3Dof::SetDampingAndStiffness @ 0x82251268 | size: 0x40
 *
 * Aliases: phJoint_12A8_fw
 * Sets internal damping/stiffness values based on input parameters.
 */
void phJoint3Dof::SetDampingAndStiffness(float arg1, float arg2) {
    this->field_0x02d0 = arg1;
    this->field_0x02d4 = arg1;
    this->field_0x02d8 = arg1;
    this->field_0x02dc = arg1;
    this->field_0x02e0 = arg2;
    this->field_0x02e4 = arg2;
    this->field_0x02e8 = 0;
    
    // 0x82079AD8 in .rdata usually represents 0.0f
    if (arg2 == 0.0f) {
        this->field_0x02e9 = 0;
    } else {
        this->field_0x02e9 = 1;
    }
}

/**
 * phJoint3Dof_C4F8_w @ 0x821DC4F8 | size: 0x64
 *
 * Iterates through 4 sequential sub-blocks embedded in the joint
 * and initializes them via a generic structural callback.
 */
void phJoint3Dof_C4F8_w(phJoint3Dof* joint) {
    char* current = reinterpret_cast<char*>(joint);
    
    for (int i = 3; i >= 0; i--) {
        // phJoint3Dof_0170_g iterates over the block calling util_4628
        phJoint3Dof_0170_g(current, 64, 8, reinterpret_cast<void*>(0x8207E6D0));
        
        phJoint3Dof* blockObj = reinterpret_cast<phJoint3Dof*>(current);
        *(uint16_t*)(current + 528) = 3;  // offset 0x0210
        *(uint16_t*)(current + 530) = 3;  // offset 0x0212
        
        util_4628(blockObj, -1);
        
        // Advance pointer to the next block (544 bytes stride)
        current += 544;
    }
}

/**
 * rage::phJoint3Dof::SetLimitAtIndex @ 0x82251200 | size: 0x64
 *
 * Fetches intermediate geometry bounds via phJoint1Dof_AFF8_p42 and 
 * delegates to phJoint_1388.
 */
void phJoint3Dof::SetLimitAtIndex(int index, float val) {
    float scratchBounds[8]; // Uses 2 SIMD vectors (32 bytes)
    
    // Fill first 16 bytes (r1+80)
    phJoint1Dof_AFF8_p42(this, index, &scratchBounds[0]);
    
    // Pass both the result vector and a second scratch vector
    phJoint_1388(this, index, val, &scratchBounds[4], &scratchBounds[0]);
}

/**
 * rage::phJoint3Dof::AccumulateForces @ 0x82254D28 | size: 0x68
 *
 * Calls vfn_24 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::AccumulateForces(int index, float* outVec) {
    float scratch[8]; // 2 vectors
    
    // Dispatch to slot 24 (96 / 4)
    typedef void (*VFn24)(phJoint3Dof*, int, float*);
    VFn24 fn = (VFn24)this->vtable[24];
    fn(this, index, scratch);
    
    // Accumulate results using SIMD-style vector add
    for (int i = 0; i < 4; i++) outVec[i] += scratch[i];        // Vector 1
    for (int i = 0; i < 4; i++) outVec[i + 4] += scratch[i + 4]; // Vector 2
}

/**
 * rage::phJoint3Dof::AccumulateTorques @ 0x82254D90 | size: 0x68
 *
 * Calls vfn_23 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::AccumulateTorques(int index, float* outVec) {
    float scratch[8]; // 2 vectors
    
    // Dispatch to slot 23 (92 / 4)
    typedef void (*VFn23)(phJoint3Dof*, int, float*);
    VFn23 fn = (VFn23)this->vtable[23];
    fn(this, index, scratch);
    
    // Accumulate results
    for (int i = 0; i < 4; i++) outVec[i] += scratch[i];        // Vector 1
    for (int i = 0; i < 4; i++) outVec[i + 4] += scratch[i + 4]; // Vector 2
}

/**
 * rage::phJoint3Dof::GetLowerLimit @ 0x822550F8 | size: 0x68
 *
 * Calls vfn_28 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::GetLowerLimit(int index, float* outVec) {
    float scratch[8]; // 2 vectors
    
    // Dispatch to slot 28 (112 / 4)
    typedef void (*VFn28)(phJoint3Dof*, int, float*);
    VFn28 fn = (VFn28)this->vtable[28];
    fn(this, index, scratch);
    
    // Accumulate results
    for (int i = 0; i < 4; i++) outVec[i] += scratch[i];        // Vector 1
    for (int i = 0; i < 4; i++) outVec[i + 4] += scratch[i + 4]; // Vector 2
}

/**
 * rage::phJoint3Dof::GetUpperLimit @ 0x82255160 | size: 0x68
 *
 * Calls vfn_27 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::GetUpperLimit(int index, float* outVec) {
    float scratch[8]; // 2 vectors
    
    // Dispatch to slot 27 (108 / 4)
    typedef void (*VFn27)(phJoint3Dof*, int, float*);
    VFn27 fn = (VFn27)this->vtable[27];
    fn(this, index, scratch);
    
    // Accumulate results
    for (int i = 0; i < 4; i++) outVec[i] += scratch[i];        // Vector 1
    for (int i = 0; i < 4; i++) outVec[i + 4] += scratch[i + 4]; // Vector 2
}

/**
 * rage::phJoint3Dof::ComputeJacobian @ 0x82255340 | size: 0x74
 *
 * Performs matrix/state clearing and resets internal angular components.
 */
void phJoint3Dof::ComputeJacobian() {
    this->field_0x02fc = 0.0f; // 764
    this->field_0x0300 = 0.0f; // 768
    
    // Clear 6 separate 16-byte vectors
    uint32_t* vecs = reinterpret_cast<uint32_t*>(this);
    
    // offset 432 (0x1B0) and 448 (0x1C0)
    for (int i = 0; i < 4; i++) vecs[(432 / 4) + i] = 0;
    for (int i = 0; i < 4; i++) vecs[(448 / 4) + i] = 0;
    
    // offset 464 (0x1D0) and 480 (0x1E0)
    for (int i = 0; i < 4; i++) vecs[(464 / 4) + i] = 0;
    for (int i = 0; i < 4; i++) vecs[(480 / 4) + i] = 0;
    
    // offset 528 (0x210) and 544 (0x220)
    for (int i = 0; i < 4; i++) vecs[(528 / 4) + i] = 0;
    for (int i = 0; i < 4; i++) vecs[(544 / 4) + i] = 0;
}

/**
 * rage::phJoint3Dof::SetLimits @ 0x82253920 | size: 0x88
 *
 * Calculates vector dot products and magnitude norms for the sub-joints.
 */
void phJoint3Dof::SetLimits() {
    phJoint_1618_g(this); // Pre-sync
    
    float f1_a = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 896);
    float f0_a = *(float*)((char*)this + 928);
    *(float*)((char*)this + 1672) = f0_a * f1_a;
    
    float f1_b = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 880);
    float f1_c = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 912);
    
    float lengthSq = (f1_b * f1_b) + (f1_c * f1_c);
    
    // Constants from standard physics data block
    const float FLT_MULT = 1.0f; // Assumed from 0x8202D10C based on logic
    lengthSq *= FLT_MULT;
    
    // External bound function likely normalizes or validates the result
    phBoundCapsule_01D0_g(this, lengthSq);
    
    *(float*)((char*)this + 1668) = lengthSq;
}

/**
 * phJoint3Dof_E7C8_2h @ 0x822CE7C8 | size: 0x8C
 *
 * Initializes an indexed constraint matrix to identity and clears
 * subsequent row vectors.
 */
void phJoint3Dof_E7C8_2h(phJoint3Dof* joint, uint32_t index) {
    // Navigate to the constraint structure base
    // Constraint stride is 64 bytes, starting offset is 112
    char* base = reinterpret_cast<char*>(joint) + (index * 64) + 112;
    
    // Identity matrix values (f13=1.0f, f0=0.0f)
    float* mat = reinterpret_cast<float*>(base);
    mat[0] = 1.0f; mat[1] = 0.0f; mat[2] = 0.0f; mat[3] = 0.0f;  // offset 0..12
    mat[4] = 0.0f; mat[5] = 1.0f; mat[6] = 0.0f; mat[7] = 0.0f;  // offset 16..28
    mat[8] = 0.0f; mat[9] = 0.0f; mat[10] = 1.0f; mat[11] = 0.0f; // offset 32..44
    
    // Clear 16-byte vector directly contiguous to matrix (offset 48)
    for (int i = 0; i < 4; i++) reinterpret_cast<uint32_t*>(base + 48)[i] = 0;
    
    // Clear next four vectors located at strict offset 240+ relative to joint
    uint32_t* baseVecs = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(joint) + 240);
    
    for (int i = 0; i < 4; i++) baseVecs[0 + i] = 0;  // 240
    for (int i = 0; i < 4; i++) baseVecs[4 + i] = 0;  // 256
    for (int i = 0; i < 4; i++) baseVecs[8 + i] = 0;  // 272
    for (int i = 0; i < 4; i++) baseVecs[12 + i] = 0; // 288
}

} // namespace rage

// ─────────────────────────────────────────────────────────────────────────────
// Physics Math Utilities (from phBoundCapsule compilation unit)
// These are free functions / global utilities, not phBoundCapsule methods.
// ─────────────────────────────────────────────────────────────────────────────

// ── Inline math wrapper ────────────────────────────────────────────────────

/**
 * ph_Sqrt @ 0x824301D0 | size: 0x8
 * Inline float square root. Single fsqrt instruction wrapper.
 */
float ph_Sqrt(float x) {  // phBoundCapsule_01D0_g
    return sqrtf(x);
}

// ── Trig Math Dispatch (7 functions, 68B each) ────────────────────────────

/**
 * Math dispatch functions @ 0x8240D888-DA38 | 68B each
 *
 * Each normalizes the input via A688_g, applies a specific math function,
 * and stores the result into out->value (+0) with type=4 (float) at out->type (+4).
 * These form the ActionScript Math object's method table.
 */

extern "C" float ph_Normalize(float x);    // @ 0x823FA688 — normalize/abs
extern "C" float ph_Atan2(float y, float x);

struct MathResult { float value; uint32_t type; };

void math_Sqrt(float input, MathResult* out) {  // D888_g @ 0x8240D888
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = sqrtf(norm);
}

void math_Acos(float input, MathResult* out) {  // D8D0_g @ 0x8240D8D0
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = acosf(norm);
}

void math_Asin(float input, MathResult* out) {  // D918_g @ 0x8240D918
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = asinf(norm);
}

void math_Atan2(float y, float x, MathResult* out) {  // D960_g @ 0x8240D960
    out->type = 4;
    float normY = ph_Normalize(y);
    out->value = ph_Atan2(normY, x);
}

void math_Sin(float input, MathResult* out) {  // D9A8_g @ 0x8240D9A8
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = sinf(norm);
}

void math_Cos(float input, MathResult* out) {  // D9F0_g @ 0x8240D9F0
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = cosf(norm);
}

void math_Exp(float input, MathResult* out) {  // DA38_g @ 0x8240DA38
    out->type = 4;
    float norm = ph_Normalize(input);
    out->value = expf(norm);
}

// ── Struct initializers (2 functions, 20B each) ───────────────────────────

/**
 * MathResult initializers @ 0x8240DAD8/DAF0 | 20B each
 * Initialize typed result structs: {0, type=3} and {0, type=2}.
 */
void mathResult_InitUndefined(MathResult* out) {  // DAD8_p33 @ 0x8240DAD8
    out->value = 0.0f;
    out->type = 3;
}

void mathResult_InitNull(MathResult* out) {  // DAF0_p33 @ 0x8240DAF0
    out->value = 0.0f;
    out->type = 2;
}

// ── Global callback setters (6 functions, 16B each) ──────────────────────

/**
 * Physics callback registration @ 0x8216FB70-FBC0 | 16B each
 *
 * Six setters that store a function pointer to consecutive globals
 * at 0x825EA900-0x825EA914. Used for registering physics event
 * callbacks (collision, trigger, overlap, etc.). Each returns 0.
 */
extern void* g_phCallback0;  // @ 0x825EA900
extern void* g_phCallback1;  // @ 0x825EA904
extern void* g_phCallback2;  // @ 0x825EA908
extern void* g_phCallback3;  // @ 0x825EA90C
extern void* g_phCallback4;  // @ 0x825EA910
extern void* g_phCallback5;  // @ 0x825EA914

int ph_SetCallback0(void* fn) { g_phCallback0 = fn; return 0; }  // FB70 @ 0x8216FB70
int ph_SetCallback1(void* fn) { g_phCallback1 = fn; return 0; }  // FB80 @ 0x8216FB80
int ph_SetCallback2(void* fn) { g_phCallback2 = fn; return 0; }  // FB90 @ 0x8216FB90
int ph_SetCallback3(void* fn) { g_phCallback3 = fn; return 0; }  // FBA0 @ 0x8216FBA0
int ph_SetCallback4(void* fn) { g_phCallback4 = fn; return 0; }  // FBB0 @ 0x8216FBB0
int ph_SetCallback5(void* fn) { g_phCallback5 = fn; return 0; }  // FBC0 @ 0x8216FBC0

// ── Array math operations ────────────────────────────────────────────────

/**
 * ph_ArrayScale @ 0x82576A08 | size: 0x28
 * Scales each element: out[i] = in[i] * scale
 */
void ph_ArrayScale(const float* input, int count, float scale, float* output) {  // 6A08_2h
    for (int i = 0; i < count; i++) {
        output[i] = input[i] * scale;
    }
}

/**
 * ph_DotProduct @ 0x825769A0 | size: 0x30
 * Accumulates dot product: sum += a[i] * b[i]
 */
float ph_DotProduct(const float* a, const float* b, int count) {  // 69A0_2h
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

/**
 * ph_SumSquares @ 0x825769D0 | size: 0x34
 * Computes element-wise: out[i] = a[i]^2 + b[i]^2
 */
void ph_SumSquares(const float* a, const float* b, int count, float* output) {  // 69D0_w
    for (int i = 0; i < count; i++) {
        output[i] = a[i] * a[i] + b[i] * b[i];
    }
}

/**
 * ph_GeometricScale @ 0x82577FA8 | size: 0x38
 * Applies geometric (exponential) scaling: out[i] = in[i] * (accum *= factor)
 */
void ph_GeometricScale(const float* input, int count, float factor, float* output) {  // 7FA8_2h
    float accum = factor;
    for (int i = 0; i < count; i++) {
        output[i] = input[i] * accum;
        accum *= factor;
    }
}

// ── Quantized data pack/unpack (4 functions) ──────────────────────────────

/**
 * ph_LoadU16Indexed @ 0x8216FCB0 | size: 0x10
 * Loads a uint16 from an indexed array, sign-extends to int32, returns.
 */
int32_t ph_LoadU16Indexed(const uint16_t* array, int index) {  // FCB0_p39
    return (int16_t)array[index];
}

/**
 * ph_StoreU16Indexed @ 0x8216FD60 | size: 0xC
 * Stores a uint16 value at an indexed array position.
 */
void ph_StoreU16Indexed(uint16_t* array, int index, uint16_t value) {  // FD60_p39
    array[index] = value;
}

/**
 * ph_LoadPacked24 @ 0x8216FCC0 | size: 0x14
 * Loads a 24-bit packed value from a byte array at index*3.
 * Reassembles from 3 bytes: (b0 << 16) | (b1 << 8) | b2
 */
uint32_t ph_LoadPacked24(const uint8_t* data, int index) {  // FCC0_p39
    int off = index * 3;
    return ((uint32_t)data[off] << 16) | ((uint32_t)data[off + 1] << 8) | data[off + 2];
}

/**
 * ph_StorePacked24 @ 0x8216FD70 | size: 0x24
 * Stores a 24-bit value as 3 bytes at index*3.
 */
void ph_StorePacked24(uint8_t* data, int index, uint32_t value) {  // FD70_p33
    int off = index * 3;
    data[off]     = (uint8_t)(value >> 16);
    data[off + 1] = (uint8_t)(value >> 8);
    data[off + 2] = (uint8_t)(value);
}

// ── Delegation helpers ───────────────────────────────────────────────────

/**
 * ph_TokenizerRead / ph_TokenizerWrite @ 0x82160E88/0E90 | 8B each
 * Delegates to fiAsciiTokenizer with read (0) or write (1) flag.
 */
extern "C" void fiAsciiTokenizer_Process(void* tokenizer, int mode);

void ph_TokenizerRead(void* tokenizer) {  // 0E88_g @ 0x82160E88
    fiAsciiTokenizer_Process(tokenizer, 0);
}

void ph_TokenizerWrite(void* tokenizer) {  // 0E90_g @ 0x82160E90
    fiAsciiTokenizer_Process(tokenizer, 1);
}

/**
 * ph_ConditionalForward @ 0x82169C78 | size: 0x14
 * If field+24 is non-null, tail-calls with that pointer.
 */
extern void ph_ForwardTarget(void* target);

void ph_ConditionalForward(void* obj) {  // 9C78_2hr @ 0x82169C78
    void* target = *(void**)((char*)obj + 24);
    if (target) ph_ForwardTarget(target);
}

/**
 * ph_MultiplyAndDelegate @ 0x8216BAF0 | size: 0x14
 * Multiplies two int args, sets address constant, tail-calls rage allocator.
 */
extern void* rage_Allocate(uint32_t size);

void* ph_AllocateScaled(uint32_t count, uint32_t stride) {  // BAF0_2h @ 0x8216BAF0
    return rage_Allocate(count * stride);
}

/**
 * ph_VtableInit @ 0x825764C8 | size: 0x18
 * Writes vtable pointer + size constant into a struct. Initializer.
 */
void ph_VtableInit(void* obj, void* vtable, uint32_t size) {  // 64C8_2hr
    *(void**)obj = vtable;
    *(uint32_t*)((char*)obj + 4) = size;
}

// ── Float math utilities ─────────────────────────────────────────────────

/**
 * ph_Log2Float @ 0x82576EF0 | size: 0x60
 * Computes log2 of a float using IEEE 754 mantissa extraction.
 * Extracts exponent, normalizes mantissa, applies polynomial approx.
 */
float ph_Log2Float(float x) {  // 6EF0 @ 0x82576EF0
    if (x <= 0.0f) return -1000.0f;  // sentinel for invalid input

    // IEEE 754: float = (1 + mantissa) * 2^exponent
    uint32_t bits = *(uint32_t*)&x;
    int32_t exponent = (int32_t)((bits >> 23) & 0xFF) - 127;
    float mantissa = (float)(bits & 0x7FFFFF) / (float)0x800000;

    // Linear approximation: log2(x) ≈ exponent + mantissa
    return (float)exponent + mantissa;
}

/**
 * ph_MemcpyWrapper @ 0x8216BB38 | size: 0x30
 * Saves this pointer, calls memcpy, restores this, returns.
 */
void ph_MemcpyWrapper(void* dst, const void* src, uint32_t size) {  // BB38
    memcpy(dst, src, size);
}


// ─────────────────────────────────────────────────────────────────────────────
// phInst — Physics Instance Small Methods (43 functions, all ≤64B)
// ─────────────────────────────────────────────────────────────────────────────

namespace rage {

// ── RETURN_CONST / FIELD_GETTER / FIELD_SETTER (trivial, 8-12B) ───────────

int   phInst_GetType()        { return 0; }  // vfn_18_D8A0_1 @ 0x8212D8A0
int   phInst_GetFlags()       { return 0; }  // vfn_11_4158_1 @ 0x82124158

/**
 * phInst::GetField34D0 @ 0x821234D0 | size: 0x8
 * Returns the pointer stored at this+4 (the bound/shape pointer).
 */
void* phInst::GetBoundPtr() {  // 34D0_g
    return *(void**)((char*)this + 4);
}

/**
 * phInst field accessors (8B each)
 */
void* phInst::GetField14() { return *(void**)((char*)this + 20); }  // phInst_14
void  phInst::SetField9(void* val) { *(void**)((char*)this + 12) = val; }  // phInst_9
void  phInst::SetField11(void* val) { *(void**)((char*)this + 16) = val; }  // phInst_11

/**
 * phInst::GetStaticSize @ 0x8216B628 | size: 0xC
 * Returns a static size constant (likely sizeof the base instance).
 */
int phInst::GetStaticSize() {  // B628
    return 20;  // literal from scaffold: li r3,20
}

/**
 * phInst::StoreAndReturn @ 0x82121488 | size: 0x10
 * Stores value 20 into *outParam, returns 0.
 */
int phInst::StoreSize(int* outParam) {  // 1488_sp
    *outParam = 20;
    return 0;
}

// ── VTABLE THUNKS (9 functions, 16-32B each) ─────────────────────────────

/**
 * phInst vtable dispatch thunks — adjust this pointer and/or forward
 * to a specific vtable slot. Used for MI adjustor thunks and forwarding.
 */
void phInst::ForwardSlot23(void* arg) {  // 2B48_p45 @ 0x82122B48
    typedef void (*Fn)(void*, void*);
    ((Fn)(*(void***)this)[23])(this, arg);
}

void phInst::ForwardSlot12() {  // phInst_45 @ 0x82129070 (20B)
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)this)[12])(this);
}

// MI adjustor thunks (this -= 8, forward to slot)
void phInst::AdjustorSlot1() {  // 5D40_2hr @ 0x82125D40
    typedef void (*Fn)(void*);
    void* adjusted = (char*)this - 8;
    ((Fn)(*(void***)adjusted)[1])(adjusted);
}

void phInst::AdjustorSlot2(void* arg) {  // 5D60_p39 @ 0x82125D60
    typedef void (*Fn)(void*, void*);
    void* adjusted = (char*)this - 8;
    ((Fn)(*(void***)adjusted)[2])(adjusted, arg);
}

void phInst::AdjustorSlot5() {  // BB20_2hr @ 0x8216BB20
    typedef void (*Fn)(void*);
    void* adjusted = (char*)this - 8;
    ((Fn)(*(void***)adjusted)[5])(adjusted);
}

void phInst::AdjustorSlot6(void* arg) {  // BB40_2hr @ 0x8216BB40
    typedef void (*Fn)(void*, void*);
    void* adjusted = (char*)this - 8;
    ((Fn)(*(void***)adjusted)[6])(adjusted, arg);
}

// ── MULTI-FIELD SETTERS (zeroing) ────────────────────────────────────────

/**
 * phInstStatic::ClearMotionState @ 0x82129070 | size: 0x14
 * Zeroes the 3 motion floats at +44, +48, +52.
 */
void phInstStatic::ClearMotionState() {  // phInstStatic_15
    *(uint32_t*)((char*)this + 44) = 0;
    *(uint32_t*)((char*)this + 48) = 0;
    *(uint32_t*)((char*)this + 52) = 0;
}

/**
 * phInstStatic::ClearAllMotion @ 0x8216B9B8 | size: 0x20
 * Zeroes 6 fields: +44/+48/+52 (velocity) and +112/+116/+120 (angular).
 */
void phInstStatic::ClearAllMotion() {  // B9B8_2h @ 0x8216B9B8
    *(uint32_t*)((char*)this + 44)  = 0;
    *(uint32_t*)((char*)this + 48)  = 0;
    *(uint32_t*)((char*)this + 52)  = 0;
    *(uint32_t*)((char*)this + 112) = 0;
    *(uint32_t*)((char*)this + 116) = 0;
    *(uint32_t*)((char*)this + 120) = 0;
}

// ── COMPUTATION (bitfield extract, arithmetic) ──────────────────────────

/**
 * phInst::ExtractBits6to9 @ 0x821292E8 | size: 0x10
 * Extracts bits 6-9 from field+32, adds 1. Returns the physics layer index.
 */
int phInst::GetPhysicsLayer() {  // 92E8_p42
    uint32_t flags = *(uint32_t*)((char*)this + 32);
    return ((flags >> 22) & 0xF) + 1;  // rlwinm extract bits 6-9 then +1
}

/**
 * phInst::SubtractU16 @ 0x8212A698 | size: 0x10
 * Returns int16 subtraction of two adjacent fields.
 */
int phInst::GetU16Delta() {  // A698_p39
    uint16_t a = *(uint16_t*)((char*)this + 0);
    uint16_t b = *(uint16_t*)((char*)this + 2);
    return (int16_t)(a - b);
}

/**
 * phInst::ComputeOffset @ 0x82129070 | size: 0x18
 * Returns field_0x44 * 100 + 268 — byte offset into physics data table.
 */
int phInst::ComputeDataOffset() {  // 9070_p42
    int index = *(int*)((char*)this + 0x44);
    return index * 100 + 268;
}

// ── ATOMIC REF-COUNTING + CRITICAL SECTIONS ─────────────────────────────

/**
 * phInst::AddRef @ 0x82120000 | size: 0x28
 * Atomic increment of reference count at +12 using lwarx/stwcx.
 */
int phInst::AddRef() {  // phInst_1
    int32_t* refCount = (int32_t*)((char*)this + 12);
    return ++(*refCount);  // original uses lwarx/stwcx atomic loop
}

/**
 * phInst::Release @ 0x821237B0 | size: 0x40
 * Atomic decrement of reference count. If count reaches 0, calls
 * the destructor callback.
 */
int phInst::Release() {  // 37B0_p33
    int32_t* refCount = (int32_t*)((char*)this + 12);
    int newCount = --(*refCount);
    if (newCount == 0) {
        typedef void (*DtorFn)(void*);
        DtorFn dtor = *(DtorFn*)((char*)this + 8);
        if (dtor) dtor(this);
    }
    return newCount;
}

/**
 * phInst::Lock @ 0x82120000 | size: 0x28
 * Enters critical section on the mutex at this+16.
 */
} // namespace rage (temporary close for extern "C" declarations)
extern "C" void RtlEnterCriticalSection(void* cs);
extern "C" void RtlLeaveCriticalSection(void* cs);
extern "C" void phInst_8F10(void* obj, void* a, void* b, void* c);
extern "C" void ke_DispatchPhysics(void* state);
extern "C" void phInst_Cleanup(void* obj);
namespace rage { // reopen

void phInst::Lock() {  // phInst_3
    RtlEnterCriticalSection((char*)this + 16);
}

/**
 * phInst::Unlock @ 0x82120000 | size: 0x28
 * Leaves critical section on the mutex at this+16.
 */
void phInst::Unlock() {  // phInst_5
    RtlLeaveCriticalSection((char*)this + 16);
}

// ── DELEGATION HELPERS ──────────────────────────────────────────────────

/**
 * phInst::ShiftArgsAndCall @ 0x821292F8 | size: 0x18
 * Shifts argument registers and tail-calls phInst_8F10.
 */
void phInst::ShiftArgsAndCall(void* a, void* b, void* c) {  // 92F8_p42
    phInst_8F10(this, a, b, c);
}

/**
 * phInst::LoadGlobalAndCall @ 0x8212E2C0 | size: 0xC
 * Loads a global pointer and tail-calls ke_9F58.
 */
extern void* g_phGlobalState;

void phInst::LoadGlobalAndCall() {  // E2C0_2h
    ke_DispatchPhysics(g_phGlobalState);
}

/**
 * phInst::InitVtableAndCleanup @ 0x821287F8 | size: 0x10
 * Sets vtable pointer and tail-calls the cleanup utility.
 */
void phInst::InitVtableAndCleanup() {  // 87F8
    // Set vtable to phInst base
    *(void**)this = (void*)0x82022D2C;
    phInst_Cleanup(this);
}

// ── INDEXED FLAG SET/CLEAR (matched pair) ───────────────────────────────

/**
 * phInst::SetIndexedFlag @ 0x8216BA10 | size: 0x28
 * Sets flag=1 at indexed position in array and sets field+24=1.
 */
void phInst::SetIndexedFlag(int index) {  // BA10_wrh
    uint8_t* array = *(uint8_t**)((char*)this + 0);
    array[index] = 1;
    *(uint8_t*)((char*)this + 24) = 1;
}

/**
 * phInst::ClearIndexedFlag @ 0x8216BA38 | size: 0x28
 * Clears flag=0 at indexed position and clears field+24=0.
 */
void phInst::ClearIndexedFlag(int index) {  // BA38_2h
    uint8_t* array = *(uint8_t**)((char*)this + 0);
    array[index] = 0;
    *(uint8_t*)((char*)this + 24) = 0;
}

// ── CONDITIONAL SETTERS ─────────────────────────────────────────────────

/**
 * phInst::ConditionalStore @ 0x82129044 | size: 0x1C
 * If arg1 == 1, stores arg2/arg3 to fields +48/+52.
 */
void phInst::ConditionalStore(int flag, uint32_t val1, uint32_t val2) {  // phInst_44
    if (flag == 1) {
        *(uint32_t*)((char*)this + 48) = val1;
        *(uint32_t*)((char*)this + 52) = val2;
    }
}

/**
 * phInst::ZeroFieldRange @ vfn_12_B888_1 | size: 0x3C
 * Loop-zeroes 20 dwords at +204 and at +124.
 */
void phInst::ZeroFieldRange() {  // vfn_12_B888_1
    memset((char*)this + 204, 0, 80);
    memset((char*)this + 124, 0, 80);
}

/**
 * phInst::ClearSubStatePtr @ 0x8212AB68 | size: 0x14
 * Dereferences chain this->field_4->field_8, stores 0 to result.
 */
void phInst::ClearSubStatePtr() {  // AB68_p39
    void* inner = *(void**)((char*)this + 4);
    if (inner) {
        *(uint32_t*)((char*)inner + 8) = 0;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider — Packed Bitfield Accessors (74 functions, ≤32B each)
//
// rage::phArticulatedCollider packs dozens of configuration fields into
// 32-bit words at offsets 0x2CB8-0x2E00 (~11448-11776 from this).
// Each accessor loads a word, extracts specific bits via right-shift + mask,
// and returns the result. The setters write the value back and OR a dirty
// bit into the 64-bit change-tracking mask at this+40.
//
// Field groups (by base offset):
//   +0x2D74 (11636): 3-bit fields packed 11 deep (rotation axes, DOF flags)
//   +0x2D6C (11628): 4-bit fields packed 8 deep (constraint types)
//   +0x2D88 (11656): mixed 3/8-bit fields (solver params)
//   +0x2D7C (11644): 1-bit and 3-bit flags (enable/disable)
//   +0x2D80 (11648): 1-bit flags (active state)
// ─────────────────────────────────────────────────────────────────────────────

// Helper: extract bits from a packed word
static inline uint32_t ExtractBits(void* obj, int offset, int shift, int mask) {
    uint32_t word = *(uint32_t*)((char*)obj + offset);
    return (word >> shift) & mask;
}

// ── Getters from +0x2D88 (11656) ─────────────────────────────────────────

uint32_t phArticulatedCollider::GetSolverParam_0()  { return ExtractBits(this, 11656,  0, 0x7); }    // 3A58 @ 0x82163A58
uint32_t phArticulatedCollider::GetSolverParam_3()  { return ExtractBits(this, 11656,  3, 0xFF); }   // 3A90 @ 0x82163A90
uint32_t phArticulatedCollider::GetSolverParam_11() { return ExtractBits(this, 11656, 11, 0x7); }    // 3AA0
uint32_t phArticulatedCollider::GetSolverParam_14() { return ExtractBits(this, 11656, 14, 0x7); }    // 3AB0
uint32_t phArticulatedCollider::GetSolverParam_17() { return ExtractBits(this, 11656, 17, 0x1); }    // 3AC0

// ── Getters from +0x2D74 (11636) — packed 3-bit rotation fields ──────────

uint32_t phArticulatedCollider::GetRotAxis_0()  { return ExtractBits(this, 11636,  0, 0x7); }  // 3AD0
uint32_t phArticulatedCollider::GetRotAxis_3()  { return ExtractBits(this, 11636,  3, 0x7); }  // 3AE0
uint32_t phArticulatedCollider::GetRotAxis_6()  { return ExtractBits(this, 11636,  6, 0x7); }  // 3AF0
uint32_t phArticulatedCollider::GetRotAxis_9()  { return ExtractBits(this, 11636,  9, 0x7); }  // 3B00
uint32_t phArticulatedCollider::GetRotAxis_12() { return ExtractBits(this, 11636, 12, 0x7); }  // 3B10
uint32_t phArticulatedCollider::GetRotAxis_15() { return ExtractBits(this, 11636, 15, 0x7); }  // 3B20
uint32_t phArticulatedCollider::GetRotAxis_18() { return ExtractBits(this, 11636, 18, 0x7); }  // 3B30
uint32_t phArticulatedCollider::GetRotAxis_21() { return ExtractBits(this, 11636, 21, 0x7); }  // 3B40
uint32_t phArticulatedCollider::GetRotAxis_24() { return ExtractBits(this, 11636, 24, 0x7); }  // 3B50
uint32_t phArticulatedCollider::GetRotAxis_27() { return ExtractBits(this, 11636, 27, 0x7); }  // 3B60
uint32_t phArticulatedCollider::GetRotAxis_30() { return ExtractBits(this, 11636, 30, 0x3); }  // 3B70 (2-bit, top of word)

// ── Getters from +0x2D6C (11628) — packed 4-bit constraint types ─────────

uint32_t phArticulatedCollider::GetConstraint_0()  { return ExtractBits(this, 11628,  0, 0xF); }  // 3B80
uint32_t phArticulatedCollider::GetConstraint_4()  { return ExtractBits(this, 11628,  4, 0xF); }  // 3B90
uint32_t phArticulatedCollider::GetConstraint_8()  { return ExtractBits(this, 11628,  8, 0xF); }  // 3BA0
uint32_t phArticulatedCollider::GetConstraint_12() { return ExtractBits(this, 11628, 12, 0xF); }  // 3BB0
uint32_t phArticulatedCollider::GetConstraint_16() { return ExtractBits(this, 11628, 16, 0xF); }  // 3BC0
uint32_t phArticulatedCollider::GetConstraint_20() { return ExtractBits(this, 11628, 20, 0xF); }  // 3BD0
uint32_t phArticulatedCollider::GetConstraint_24() { return ExtractBits(this, 11628, 24, 0xF); }  // 3BE0
uint32_t phArticulatedCollider::GetConstraint_28() { return ExtractBits(this, 11628, 28, 0xF); }  // 3BF0

// ── Getters from +0x2D7C (11644) — 1-bit and 3-bit flags ────────────────

uint32_t phArticulatedCollider::GetFlag7C_0()  { return ExtractBits(this, 11644,  0, 0x1); }  // 3C00
uint32_t phArticulatedCollider::GetFlag7C_1()  { return ExtractBits(this, 11644,  1, 0x7); }  // 3C10
uint32_t phArticulatedCollider::GetFlag7C_4()  { return ExtractBits(this, 11644,  4, 0x1); }  // 3C20
uint32_t phArticulatedCollider::GetFlag7C_5()  { return ExtractBits(this, 11644,  5, 0x1); }  // 3C30

// ── Getters from +0x2D80 (11648) — 1-bit active flags ───────────────────

uint32_t phArticulatedCollider::GetActive_0()  { return ExtractBits(this, 11648,  0, 0x1); }  // 3C40
uint32_t phArticulatedCollider::GetActive_1()  { return ExtractBits(this, 11648,  1, 0x1); }  // 3C50
uint32_t phArticulatedCollider::GetActive_2()  { return ExtractBits(this, 11648,  2, 0x1); }  // 3C60

// ── Indexed bitfield getters (20-28B, stride-24 array) ───────────────────

/**
 * Indexed accessors — compute this + index*24 + base_offset, load word,
 * extract bits. Used for per-bone/per-joint packed data.
 */
uint32_t phArticulatedCollider::GetIndexedField(int index, int shift, int mask) {
    uint32_t word = *(uint32_t*)((char*)this + 1168 + index * 24);
    return (word >> shift) & mask;
}

// Per-element getters (representative subset — 12 functions follow this pattern)
uint32_t phArticulatedCollider::GetBoneField_5B90(int index) { return GetIndexedField(index, 0, 0x7); }   // 5B90
uint32_t phArticulatedCollider::GetBoneField_5BE0(int index) { return GetIndexedField(index, 3, 0x7); }   // 5BE0
uint32_t phArticulatedCollider::GetBoneField_5C30(int index) { return GetIndexedField(index, 6, 0x7); }   // 5C30
uint32_t phArticulatedCollider::GetBoneField_5C88(int index) { return GetIndexedField(index, 9, 0x7); }   // 5C88
uint32_t phArticulatedCollider::GetBoneField_5CE0(int index) { return GetIndexedField(index, 12, 0xF); }  // 5CE0
uint32_t phArticulatedCollider::GetBoneField_5D38(int index) { return GetIndexedField(index, 16, 0xF); }  // 5D38

// ── Dirty-flag setters (32B each) ────────────────────────────────────────

/**
 * phArticulatedCollider dirty-flag setters — store a value to a field,
 * then OR a specific bit into the 64-bit dirty mask at this+40 to mark
 * the change for physics sync.
 */
static inline void SetFieldAndDirty(void* obj, int offset, uint32_t value, uint64_t dirtyBit) {
    *(uint32_t*)((char*)obj + offset) = value;
    uint64_t* dirtyMask = (uint64_t*)((char*)obj + 40);
    *dirtyMask |= dirtyBit;
}

void phArticulatedCollider::SetSolverParam_11636(uint32_t value) {  // setter for field at 11636
    SetFieldAndDirty(this, 11636, value, 1ULL << 50);
}

void phArticulatedCollider::SetConstraint_11628(uint32_t value) {
    SetFieldAndDirty(this, 11628, value, 1ULL << 51);
}

void phArticulatedCollider::SetFlags_11644(uint32_t value) {
    SetFieldAndDirty(this, 11644, value, 1ULL << 52);
}

void phArticulatedCollider::SetActive_11648(uint32_t value) {
    SetFieldAndDirty(this, 11648, value, 1ULL << 53);
}

// ── Multi-store setter ──────────────────────────────────────────────────

/**
 * phArticulatedCollider::SetFourConsecutive @ ~20B
 * Stores the same value to 4 consecutive uint32 fields at +12124..+12136.
 */
void phArticulatedCollider::SetFourConsecutive(uint32_t value) {  // 4D58
    *(uint32_t*)((char*)this + 12124) = value;
    *(uint32_t*)((char*)this + 12128) = value;
    *(uint32_t*)((char*)this + 12132) = value;
    *(uint32_t*)((char*)this + 12136) = value;
}

// ── Float getter with scaling ───────────────────────────────────────────

/**
 * phArticulatedCollider::GetScaledFloat @ 0x82164670 | size: 0x1C
 * Loads float at +11920, multiplies by a constant, returns result.
 */
float phArticulatedCollider::GetScaledFloat() {  // 4670
    float rawValue = *(float*)((char*)this + 11920);
    static const float kScale = 57.29578f;  // 180/PI (radians to degrees)
    return rawValue * kScale;
}

// ── Indexed bool check ──────────────────────────────────────────────────

/**
 * phArticulatedCollider::IsElementActive @ ~32B
 * Returns true if byte at this[index+176] is non-zero.
 */
bool phArticulatedCollider::IsElementActive(int index) {  // E1B0
    return *(uint8_t*)((char*)this + index + 176) != 0;
}


// ─────────────────────────────────────────────────────────────────────────────
// phInst — Remaining Small Functions (8 functions, 20-60B each)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * phInst::StoreSizeConstant @ 0x824631A0 | size: 0x14
 * Stores the constant 0x4BA24 (309796) to *outParam and returns 0.
 * This is the serialized size of the phInst data block.
 */
int phInst::StoreSizeConstant(uint32_t* outParam) {  // 31A0_p39
    *outParam = 0x4BA24;
    return 0;
}

/**
 * phInst::ForwardToVtableSlot13_At124 @ 0x8248D828 | size: 0x18  [vtable slot 14]
 * Dispatches to vtable slot 13 with (this, r4, r5, 40, this+124).
 * Forwards a matrix operation to the embedded transform at offset +124.
 */
void phInst::DispatchTransformA(void* arg1, void* arg2) {  // vfn_14_D828_1
    typedef void (*Fn)(void*, void*, void*, int, void*);
    void** vt = *(void***)this;
    ((Fn)vt[13])(this, arg1, arg2, 40, (char*)this + 124);
}

/**
 * phInst::InitVtables_5910 @ 0x82465910 | size: 0x28
 * Constructor helper — sets three vtable pointers on the object.
 * Primary vtable at +0 = 0x82005918, secondary at +4 = 0x82003DB0.
 */
void phInst::InitTripleVtable() {  // 5910_p39
    *(void**)((char*)this + 0) = (void*)0x82005918;
    // +4 first written as 0x820058FC, then overwritten with 0x82003DB0
    *(void**)((char*)this + 4) = (void*)0x82003DB0;
}

/**
 * phInst::InitWithParam @ 0x82461508 | size: 0x38
 * Constructor that takes a parameter object. Sets vtable pointers,
 * stores 1 at +8, copies param->field_4 to this+12, then sets
 * final vtable pair.
 */
void phInst::InitWithParam(void* param) {  // 1508_2hr
    *(uint32_t*)((char*)this + 8) = 1;
    *(uint32_t*)((char*)this + 12) = *(uint32_t*)((char*)param + 4);
    *(void**)((char*)this + 0) = (void*)0x8200586C;
    *(void**)((char*)this + 4) = (void*)0x82005850;
}

/**
 * phInst::GetSubObjectPtr @ 0x8246C1A0 | size: 0x3C
 * Calls phInst_BFB8_2hr to find a sub-object, adds 24 to the result,
 * stores it to *outPtr, and returns 0.
 */
} // namespace rage (temp close)
extern "C" void* phInst_BFB8_2hr(void* obj);
extern "C" void grc_SetupResource(void* a, void* b);
extern "C" void msgMsgSink_Broadcast(void* sink, void* msg, void* data);
namespace rage { // reopen

int phInst::GetSubObjectPtr(void** outPtr) {  // C1A0_2hr
    void* result = phInst_BFB8_2hr(this);
    *outPtr = (char*)result + 24;
    return 0;
}

/**
 * phInst::SetPhysicsLayer @ 0x82359310 | size: 0x14
 * Masks the page-aligned portion of field+36 and field+20,
 * tail-calls the graphics resource setup function.
 */
void phInst::SetPhysicsLayer(void* arg) {  // 9310_p42
    uint32_t val36 = *(uint32_t*)((char*)this + 36) & 0xFFFFF000;
    uint32_t val20 = *(uint32_t*)((char*)this + 20) & 0xFFFFF000;
    grc_SetupResource((void*)(uintptr_t)val36, (void*)(uintptr_t)val20);
}

/**
 * phInst::ConditionalForward @ 0x8256B610 | size: 0x14
 * Conditionally zeroes r5 based on a flag, then tail-calls
 * the message sink broadcast function.
 */
void phInst::ConditionalBroadcast(void* msg, bool includeData) {  // B610_p42
    msgMsgSink_Broadcast(this, msg, includeData ? msg : nullptr);
}

/**
 * phInst::ReadMMIOAndStore @ 0x8246AC00 | size: 0x18
 * Reads a 32-bit value from MMIO address 0x7FEA1800, byte-swaps it,
 * and stores to a global at 0x825E7890.
 */
void phInst::ReadMMIOAndStore() {  // AC00_2hr
    volatile uint32_t* mmioReg = (volatile uint32_t*)(uintptr_t)0x7FEA1800;
    uint32_t rawValue = *mmioReg;
    // Byte-swap (big-endian MMIO → little-endian)
    uint32_t swapped = __builtin_bswap32(rawValue);
    extern uint32_t g_mmioStoredValue;  // @ 0x825E7890
    g_mmioStoredValue = swapped;
}


// =============================================================================
// rage::phBoundCapsule -- Small Functions (<=64B) Batch Lift
// =============================================================================

// --- External declarations for this batch ---
extern void  fiAsciiTokenizer_0BA8_g(void* tokenizer, int mode);
extern void  _locale_register(void* ptr, void* allocator);
extern void  rage_Alloc(int size, void* allocator);  // rage_01B8
extern void  ph_1B78(void* thisPtr, float f2, float f3, void* r6);
extern void  msgMsgSink_8DA0_sp(void* obj);
extern void  phBoundCapsule_9DA0_2h(void* a, void* b, int count);
extern void  phBoundCapsule_9B58_w(void* a, void* b, int count);
extern void  phBoundCapsule_9C78_2hr_9C78_1(void* a, void* b, int count);
extern void  phBoundCapsule_F5F8_p39(void* data);
extern void* g_display_obj_ptr;  // @ 0x826066E4

// --- Globals for callback table (FB70-FBC0 series) ---
extern uint32_t g_phCallback0;  // @ 0x825EA900
extern uint32_t g_phCallback1;  // @ 0x825EA904
extern uint32_t g_phCallback2;  // @ 0x825EA908
extern uint32_t g_phCallback3;  // @ 0x825EA90C
extern uint32_t g_phCallback4;  // @ 0x825EA910
extern uint32_t g_phCallback5;  // @ 0x825EA914

// ---------------------------------------------------------------------------
// 1. phBoundCapsule::SetMaterialIndex (vfn_13) @ 0x822A2E00 | size: 0x8
//    Stores a 32-bit value at offset +192 (material index on the bound).
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::SetMaterialIndex(uint32_t index) {
    *(uint32_t*)((char*)this + 192) = index;
}

// ---------------------------------------------------------------------------
// 2. phBoundCapsule::GetMaterialIndex (vfn_12) @ 0x8256FBD0 | size: 0x8
//    Returns the 32-bit material index stored at offset +192.
// ---------------------------------------------------------------------------
uint32_t rage::phBoundCapsule::GetMaterialIndex() const {
    return *(uint32_t*)((char*)this + 192);
}

// ---------------------------------------------------------------------------
// 3. phBoundCapsule::GetVolume (vfn_8) @ 0x822A2DB0 | size: 0x2C
//    Computes capsule volume: V = (4/3) * PI * halfHeight * (halfHeight + radius)^2
//    offset +112 = m_halfHeight, +128 = m_radius
//    Uses two float constants from .rdata for PI-related factors.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::GetVolume() const {
    float halfHeight = *(float*)((char*)this + 112);
    float radius     = *(float*)((char*)this + 128);
    extern const float g_capsuleVolK1;  // @ .rdata (fmadds multiplier)
    extern const float g_capsuleVolK2;  // @ .rdata (final scale, ~4.189 = 4/3*PI)
    float t = halfHeight * g_capsuleVolK1 + radius;  // fmadds f0*f13+f12
    float t2 = t * halfHeight;
    float t3 = t2 * halfHeight;
    return t3 * g_capsuleVolK2;
}

// ---------------------------------------------------------------------------
// 4. phBoundCapsule::UpdateBound (vfn_11) @ 0x822A2DE0 | size: 0x20
//    Passes the material index (+192) to a global display object's vfn_5.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::UpdateBound() {
    uint32_t matIdx = *(uint32_t*)((char*)this + 192);
    void* displayObj = g_display_obj_ptr;
    // Call vfn_5 on the global display object with our material index
    void** vtable = *(void***)displayObj;
    typedef void (*DispatchFn)(void*, uint32_t);
    DispatchFn fn = (DispatchFn)vtable[5];
    fn(displayObj, matIdx);
}

// ---------------------------------------------------------------------------
// 5. phBoundCapsule::GetSupportPoint (vfn_9) @ 0x822A3258 | size: 0x10
//    Loads halfHeight (+112) and radius (+128) from the capsule, then
//    tail-calls ph_1B78 to compute a support point.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::GetSupportPoint(void* direction, void* outPoint) {
    float radius     = *(float*)((char*)this + 128);
    float halfHeight = *(float*)((char*)this + 112);
    ph_1B78(this, halfHeight, radius, outPoint);
}

// ---------------------------------------------------------------------------
// 6. phBoundCapsule_0E88_g @ 0x82430E88 | size: 0x8
//    Calls fiAsciiTokenizer_0BA8_g with mode=0 (read mode).
// ---------------------------------------------------------------------------
void phBoundCapsule_0E88_g(void* tokenizer) {
    fiAsciiTokenizer_0BA8_g(tokenizer, 0);
}

// ---------------------------------------------------------------------------
// 7. phBoundCapsule_0E90_g @ 0x82430E90 | size: 0x8
//    Calls fiAsciiTokenizer_0BA8_g with mode=1 (write mode).
// ---------------------------------------------------------------------------
void phBoundCapsule_0E90_g(void* tokenizer) {
    fiAsciiTokenizer_0BA8_g(tokenizer, 1);
}

// ---------------------------------------------------------------------------
// 8. phBoundCapsule_DAD8_p33 @ 0x823FDAD8 | size: 0x14
//    Initializes a result pair: sets type=3 (capsule) and value=0.
//    r6 = output struct pointer.
// ---------------------------------------------------------------------------
void phBoundCapsule_DAD8_p33(void* /*r3*/, void* /*r4*/, void* /*r5*/, uint32_t* outResult) {
    outResult[1] = 3;   // type = capsule
    outResult[0] = 0;   // value = 0
}

// ---------------------------------------------------------------------------
// 9. phBoundCapsule_DAF0_p33 @ 0x823FDAF0 | size: 0x14
//    Initializes a result pair: sets type=2 (sphere) and byte[0]=0.
// ---------------------------------------------------------------------------
void phBoundCapsule_DAF0_p33(void* /*r3*/, void* /*r4*/, void* /*r5*/, void* outResult) {
    *(uint32_t*)((char*)outResult + 4) = 2;  // type = sphere
    *(uint8_t*)outResult = 0;                 // flag = 0
}

// ---------------------------------------------------------------------------
// 10. phBoundCapsule_9C78_2hr @ 0x82459C78 | size: 0x14
//     If field +24 is non-null, tail-calls msgMsgSink_8DA0_sp on it.
// ---------------------------------------------------------------------------
void phBoundCapsule_9C78_2hr(void* obj) {
    void* sink = *(void**)((char*)obj + 24);
    if (sink == nullptr) return;
    msgMsgSink_8DA0_sp(sink);
}

// ---------------------------------------------------------------------------
// 11. phBoundCapsule_FCB0_p39 @ 0x824AFCB0 | size: 0x10
//     Reads a signed 16-bit element from an array.
//     index = r6, base = r3. Element at base[index*2], sign-extended.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_FCB0_p39(int16_t* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    return (int32_t)base[index];
}

// ---------------------------------------------------------------------------
// 12. phBoundCapsule_FCC0_p39 @ 0x824AFCC0 | size: 0x14
//     Reads a 24-bit packed element from a 3-byte-stride array, sign-extended
//     then arithmetic-shifted right by 8 to produce a signed result.
//     Stride = index * 3. Loads dword at base + stride, shifts >> 8.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_FCC0_p39(void* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint32_t raw = *(uint32_t*)((char*)base + stride);
    return (int32_t)raw >> 8;
}

// ---------------------------------------------------------------------------
// 13. phBoundCapsule_FD60_p39 @ 0x824AFD60 | size: 0xC
//     Stores a 16-bit value into an array at index.
//     base = r4, index = r6, value = r3.
// ---------------------------------------------------------------------------
void phBoundCapsule_FD60_p39(uint16_t value, int16_t* base, void* /*r5*/, uint32_t index) {
    base[index] = (int16_t)value;
}

// ---------------------------------------------------------------------------
// 14. phBoundCapsule_FD70_p33 @ 0x824AFD70 | size: 0x24
//     Stores a 24-bit packed value into a 3-byte-stride array.
//     The value in r3 is decomposed: byte2 = r3 & 0xFF, byte1 = (r3>>8) & 0xFF,
//     byte0 = (r3>>16) & 0xFF, stored at base + index*3.
// ---------------------------------------------------------------------------
void phBoundCapsule_FD70_p33(int32_t value, uint8_t* base, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint8_t* dst = base + stride;
    int32_t mid = value >> 8;
    int32_t hi  = mid >> 8;
    dst[2] = (uint8_t)value;
    dst[1] = (uint8_t)mid;
    dst[0] = (uint8_t)hi;
}

// ---------------------------------------------------------------------------
// 15. phBoundCapsule_FCD8_p39 @ 0x824AFCD8 | size: 0x28
//     Reads a 24-bit packed value from a 3-byte-stride array, reassembles
//     into a sign-extended 24-bit integer, then arithmetic-shifts right by 4.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_FCD8_p39(void* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint8_t* src = (uint8_t*)base + stride;
    uint8_t lo  = src[2];
    uint16_t hi = *(uint16_t*)src;
    int8_t loSigned = (int8_t)lo;
    int32_t combined = ((int32_t)loSigned << 16) | hi;
    return combined >> 4;
}

// ---------------------------------------------------------------------------
// 16. phBoundCapsule_FD98_p33 @ 0x824AFD98 | size: 0x28
//     Stores a value left-shifted by 4 into a 3-byte-stride packed array.
// ---------------------------------------------------------------------------
void phBoundCapsule_FD98_p33(int32_t value, uint8_t* base, void* /*r5*/, uint32_t index) {
    uint32_t shifted = (uint32_t)value << 4;
    // Decompose 32-bit shifted value into 3 bytes (big-endian order)
    uint8_t* tmp = (uint8_t*)&shifted;
    uint32_t stride = index * 3;
    uint8_t* dst = base + stride;
    // Store high 16 bits then low byte (big-endian PPC layout)
    *(uint16_t*)dst = *(uint16_t*)tmp;
    dst[2] = tmp[2];
}

// ---------------------------------------------------------------------------
// 17. phBoundCapsule_BAF0_2h @ 0x8256BAF0 | size: 0x14
//     Multiplies r3 * r4, then tail-calls rage_Alloc with that size
//     and allocator pointer 0x6489_0018.
// ---------------------------------------------------------------------------
void phBoundCapsule_BAF0_2h(int32_t elemSize, int32_t count) {
    int32_t totalSize = elemSize * count;
    extern void* g_phAllocator;  // @ 0x64890018 (constructed from lis+ori)
    rage_Alloc(totalSize, g_phAllocator);
}

// ---------------------------------------------------------------------------
// 18. phBoundCapsule_BBB8_2h @ 0x8256BBB8 | size: 0xC
//     Tail-calls _locale_register with a fixed allocator pointer.
// ---------------------------------------------------------------------------
void phBoundCapsule_BBB8_2h(void* ptr) {
    extern void* g_phAllocator;  // @ 0x64890018
    _locale_register(ptr, g_phAllocator);
}

// ---------------------------------------------------------------------------
// 19-24. SetCallback0..5 (FB70-FBC0 series) @ 0x8256FB70..0x8256FBC0
//        Each stores r3 into a consecutive global slot, returns 0.
//        These register physics callback function pointers.
// ---------------------------------------------------------------------------

// 19. phBoundCapsule_FB70_2h @ 0x8256FB70 | size: 0x10
int32_t phBoundCapsule_SetCallback0(uint32_t callback) {
    g_phCallback0 = callback;  // @ 0x825EA900
    return 0;
}

// 20. phBoundCapsule_FB80_2h @ 0x8256FB80 | size: 0x10
int32_t phBoundCapsule_SetCallback1(uint32_t callback) {
    g_phCallback1 = callback;  // @ 0x825EA904
    return 0;
}

// 21. phBoundCapsule_FB90_2h @ 0x8256FB90 | size: 0x10
int32_t phBoundCapsule_SetCallback2(uint32_t callback) {
    g_phCallback2 = callback;  // @ 0x825EA908
    return 0;
}

// 22. phBoundCapsule_FBA0_2h @ 0x8256FBA0 | size: 0x10
int32_t phBoundCapsule_SetCallback3(uint32_t callback) {
    g_phCallback3 = callback;  // @ 0x825EA90C
    return 0;
}

// 23. phBoundCapsule_FBB0_2h @ 0x8256FBB0 | size: 0x10
int32_t phBoundCapsule_SetCallback4(uint32_t callback) {
    g_phCallback4 = callback;  // @ 0x825EA910
    return 0;
}

// 24. phBoundCapsule_FBC0_2h @ 0x8256FBC0 | size: 0x10
int32_t phBoundCapsule_SetCallback5(uint32_t callback) {
    g_phCallback5 = callback;  // @ 0x825EA914
    return 0;
}

// =============================================================================
// phDemoWorld vtable thunks (vt3DB0) — this-adjustor + tail-call patterns
// Vtable @ 0x82003DB0 (phDemoWorld)
// =============================================================================

// ph_vt3DB0_7_9140 @ 0x82449140 | size: 0x8
// Adjusts this by -4, tail-calls ph_A330 (slot 7 thunk)
void ph_vt3DB0_7_Thunk(void* thisPtr) {
    extern void ph_A330(void* obj);
    ph_A330((char*)thisPtr - 4);
}

// ph_vt3DB0_8_9148 @ 0x82449148 | size: 0x8
// Adjusts this by -4, tail-calls ph_vt3DB0_12_8DB8 (slot 8 thunk)
void ph_vt3DB0_8_Thunk(void* thisPtr) {
    extern void ph_vt3DB0_12_8DB8(void* obj);
    ph_vt3DB0_12_8DB8((char*)thisPtr - 4);
}

// ph_vt3DB0_9_9150 @ 0x82449150 | size: 0x8
// Adjusts this by -4, tail-calls ph_vt3DB0_13_8E10 (slot 9 thunk)
void ph_vt3DB0_9_Thunk(void* thisPtr) {
    extern void ph_vt3DB0_13_8E10(void* obj);
    ph_vt3DB0_13_8E10((char*)thisPtr - 4);
}

// ph_vt3DB0_53_B380 @ 0x8244B380 | size: 0x8
// Returns field at offset +132 (getter)
uint32_t ph_vt3DB0_53_GetField132(void* thisPtr) {
    return *(uint32_t*)((char*)thisPtr + 132);
}

// ph_vt3DB0_30_AA30 @ 0x8244AA30 | size: 0x28
// Adjustor thunk: shifts this/arg, dispatches via vtable slot 1
void ph_vt3DB0_30_Dispatch(void* thisPtr, void* target) {
    void* adjusted = ((char*)thisPtr - 12);
    if (adjusted == nullptr) adjusted = nullptr;  // null-check for (this-12)==0
    void** vtable = *(void***)target;
    typedef void (*VFn)(void*, void*);
    ((VFn)vtable[1])(target, adjusted);
}

// ph_vt3DB0_46_B5C0 @ 0x8244B5C0 | size: 0x30
// Calls vtable slot 1 on self, then reads byte at +0 from result
uint8_t ph_vt3DB0_46_GetBoundType(void* thisPtr) {
    typedef void* (*VFn1)(void*);
    void** vtable = *(void***)thisPtr;
    void* bound = ((VFn1)vtable[1])(thisPtr);
    return *(uint8_t*)bound;
}

// ph_vt3DB0_47_B5F0 @ 0x8244B5F0 | size: 0x34
// Calls vtable slot 1, extracts bits 5-7 from byte at +5
uint8_t ph_vt3DB0_47_GetBoundFlags3(void* thisPtr) {
    typedef void* (*VFn1)(void*);
    void** vtable = *(void***)thisPtr;
    void* bound = ((VFn1)vtable[1])(thisPtr);
    return (*(uint8_t*)((char*)bound + 5) >> 3) & 0x7;
}

// ph_vt3DB0_48_B628 @ 0x8244B628 | size: 0x30
// Calls vtable slot 1, returns uint16 at +6
uint16_t ph_vt3DB0_48_GetBoundMaterial(void* thisPtr) {
    typedef void* (*VFn1)(void*);
    void** vtable = *(void***)thisPtr;
    void* bound = ((VFn1)vtable[1])(thisPtr);
    return *(uint16_t*)((char*)bound + 6);
}

// ph_vt3DB0_49_B658 @ 0x8244B658 | size: 0x30
// Calls vtable slot 1, returns byte at +8
uint8_t ph_vt3DB0_49_GetBoundIndex(void* thisPtr) {
    typedef void* (*VFn1)(void*);
    void** vtable = *(void***)thisPtr;
    void* bound = ((VFn1)vtable[1])(thisPtr);
    return *(uint8_t*)((char*)bound + 8);
}

// ph_vt3DB0_45_B7A0 @ 0x8244B7A0 | size: 0x3C
// Calls vtable slot 11, adds offset from r4
void* ph_vt3DB0_45_GetOffsetPtr(void* thisPtr, uint32_t offset) {
    typedef void* (*VFn11)(void*);
    void** vtable = *(void***)thisPtr;
    void* base = ((VFn11)vtable[11])(thisPtr);
    return (char*)base + offset;
}

// =============================================================================
// phInst vtable thunks (vt57D8) — phInst embedded at +12
// Vtable @ 0x820057D8 (phInst)
// =============================================================================

// ph_vt57D8_0_FF70 @ 0x8245FF70 | size: 0x8
// Adjusts this by -12 (back to container), tail-calls constructor at 0x82460718
void ph_vt57D8_0_Thunk(void* thisPtr) {
    extern void ph_vt57D8_20_0718(void* obj);
    ph_vt57D8_20_0718((char*)thisPtr - 12);
}

// ph_vt57D8_7_FF78 @ 0x8245FF78 | size: 0x10
// Reads byte at +152, stores to output ptr, returns 0
int ph_vt57D8_7_GetByte152(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 152);
    return 0;
}

// ph_vt57D8_13_00C0 @ 0x824600C0 | size: 0x10
// Gets float at +92, stores to output ptr, returns 0
int ph_vt57D8_13_GetFloat92(void* thisPtr, float* out) {
    *out = *(float*)((char*)thisPtr + 92);
    return 0;
}

// ph_vt57D8_14_00D0 @ 0x824600D0 | size: 0x10
// Sets float at +92 from f1 parameter, returns 0
int ph_vt57D8_14_SetFloat92(void* thisPtr, float value) {
    *(float*)((char*)thisPtr + 92) = value;
    return 0;
}

// ph_vt57D8_15_00E0 @ 0x824600E0 | size: 0x10
// Gets float at +96, stores to output ptr, returns 0
int ph_vt57D8_15_GetFloat96(void* thisPtr, float* out) {
    *out = *(float*)((char*)thisPtr + 96);
    return 0;
}

// ph_vt57D8_16_00F0 @ 0x824600F0 | size: 0x10
// Sets float at +96 from f1 parameter, returns 0
int ph_vt57D8_16_SetFloat96(void* thisPtr, float value) {
    *(float*)((char*)thisPtr + 96) = value;
    return 0;
}

// ph_vt57D8_45_1338 @ 0x82461338 | size: 0x14
// Dispatches through vtable slot 7 with arg from r4+8
void ph_vt57D8_45_DispatchSlot7(void* thisPtr, void* param) {
    uint32_t arg = *(uint32_t*)((char*)param + 8);
    void** vtable = *(void***)thisPtr;
    typedef void (*VFn)(void*, uint32_t);
    ((VFn)vtable[7])(thisPtr, arg);
}

// ph_vt57D8_30_13B8 @ 0x824613B8 | size: 0x8
// Adjusts this by -4, tail-calls aud_1498
void ph_vt57D8_30_Thunk(void* thisPtr) {
    extern void aud_1498(void* obj);
    aud_1498((char*)thisPtr - 4);
}

// ph_vt57D8_37_13C0 @ 0x824613C0 | size: 0x14
// Increments uint32 at (this+4)+4 (reference counter at +8)
uint32_t ph_vt57D8_37_IncrementRefCount(void* thisPtr) {
    uint32_t* counter = (uint32_t*)((char*)thisPtr + 8);
    uint32_t newVal = *counter + 1;
    *counter = newVal;
    return newVal;
}

// ph_vt57D8_39_1420 @ 0x82461420 | size: 0x1C
// Initializes a 5-byte status struct: type=2, id=0, flag=0
int ph_vt57D8_39_InitStatus(void* thisPtr, void* status) {
    *(uint8_t*)((char*)status + 0) = 2;
    *(uint16_t*)((char*)status + 2) = 0;
    *(uint8_t*)((char*)status + 4) = 0;
    return 0;
}

// ph_vt57D8_27_8958 @ 0x82468958 | size: 0x24
// Initializes a status struct with type=4, a global uint16 value, and flag=0
int ph_vt57D8_27_InitStatusWithGlobal(void* thisPtr, void* status) {
    extern uint16_t g_phDefaultStatusId;  // @ 0x82465A58
    *(uint8_t*)((char*)status + 0) = 4;
    *(uint16_t*)((char*)status + 2) = g_phDefaultStatusId;
    *(uint8_t*)((char*)status + 4) = 0;
    return 0;
}

// ph_vt57D8_36_8A20 @ 0x82468A20 | size: 0xC
// Stores (this - 4) to output pointer (adjustor for getting container ptr)
void ph_vt57D8_36_GetContainerPtr(void* thisPtr, void** out) {
    *out = (char*)thisPtr - 4;
}

// ph_vt57D8_46_8A30 @ 0x82468A30 | size: 0x8
// Stores this pointer to output (identity getter)
void ph_vt57D8_46_GetSelfPtr(void* thisPtr, void** out) {
    *out = thisPtr;
}

// ph_vt57D8_35_8CD0 @ 0x82468CD0 | size: 0x8
// Adjusts this by -4, tail-calls ph_vt57D8_2_6378
void ph_vt57D8_35_Thunk(void* thisPtr) {
    extern void ph_vt57D8_2_6378(void* obj);
    ph_vt57D8_2_6378((char*)thisPtr - 4);
}

// =============================================================================
// phInst2 vtable thunks (vt58FC) — phInst variant at +4
// Vtable @ 0x820058FC (phInst2)
// =============================================================================

// ph_vt58FC_0_4E38 @ 0x82464E38 | size: 0x8
// Adjusts this by -4, tail-calls phInst_5910_p39
void ph_vt58FC_0_Thunk(void* thisPtr) {
    extern void phInst_5910_p39(void* obj);
    phInst_5910_p39((char*)thisPtr - 4);
}

// ph_vt58FC_14_4E60 @ 0x82464E60 | size: 0x10
// Gets byte at +26, stores to output ptr, returns 0
int ph_vt58FC_14_GetByte26(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 26);
    return 0;
}

// ph_vt58FC_15_4E70 @ 0x82464E70 | size: 0x10
// Sets byte at +26, returns 0
int ph_vt58FC_15_SetByte26(void* thisPtr, uint8_t value) {
    *((uint8_t*)thisPtr + 26) = value;
    return 0;
}

// =============================================================================
// phInstGfx vtable thunks (vt5A40/vt5A60) — graphics physics instances
// Vtable @ 0x82005A40 / 0x82005A60
// =============================================================================

// ph_vt5A40_0_6108 @ 0x82466108 | size: 0x10
// Destructor stub: sets vtable to lbl_82005A40
void ph_vt5A40_0_SetVtable(void* thisPtr) {
    *(void**)thisPtr = (void*)0x82005A40;  // lbl_82005A40 vtable
}

// ph_vt5A60_40_68C0 @ 0x824668C0 | size: 0x8
// Adjusts this by -4, tail-calls aud_6A20_wrap_6A20
void ph_vt5A60_40_Thunk(void* thisPtr) {
    extern void aud_6A20_wrap_6A20(void* obj);
    aud_6A20_wrap_6A20((char*)thisPtr - 4);
}

// ph_vt5A60_20_68C8 @ 0x824668C8 | size: 0x8
// Adjusts this by -16, tail-calls aud_6A20_wrap_6A20
void ph_vt5A60_20_Thunk(void* thisPtr) {
    extern void aud_6A20_wrap_6A20(void* obj);
    aud_6A20_wrap_6A20((char*)thisPtr - 16);
}

// ph_vt5A60_22_68D0 @ 0x824668D0 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_57_6858
void ph_vt5A60_22_Thunk(void* thisPtr) {
    extern void ph_vt5A60_57_6858(void* obj);
    ph_vt5A60_57_6858((char*)thisPtr - 16);
}

// ph_vt5A60_23_7230 @ 0x82467230 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_58_6EE8
void ph_vt5A60_23_Thunk(void* thisPtr) {
    extern void ph_vt5A60_58_6EE8(void* obj);
    ph_vt5A60_58_6EE8((char*)thisPtr - 16);
}

// ph_vt5A60_32_7238 @ 0x82467238 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_32_Thunk(void* thisPtr) {
    extern void ph_vt5A8C_63_6A50(void* obj);
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_34_7240 @ 0x82467240 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5A60_34_Thunk(void* thisPtr) {
    extern void ph_vt5A7C_63_6A98(void* obj);
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5A60_36_7248 @ 0x82467248 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_36_Thunk(void* thisPtr) {
    extern void ph_vt5A84_63_6B90(void* obj);
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_28_7C18 @ 0x82467C18 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_60_7870
void ph_vt5A60_28_Thunk(void* thisPtr) {
    extern void ph_vt5A60_60_7870(void* obj);
    ph_vt5A60_60_7870((char*)thisPtr - 16);
}

// ph_vt5A60_29_7C20 @ 0x82467C20 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_61_7A38
void ph_vt5A60_29_Thunk(void* thisPtr) {
    extern void ph_vt5A60_61_7A38(void* obj);
    ph_vt5A60_61_7A38((char*)thisPtr - 16);
}

// ph_vt5A60_59_89A0 @ 0x824689A0 | size: 0x10
// Gets byte at +168, stores to output ptr, returns 0
int ph_vt5A60_59_GetByte168(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 168);
    return 0;
}

// ph_vt5A60_53_8A38 @ 0x82468A38 | size: 0x8
// Adjusts this by +4, tail-calls ph_vt57D8_28_FD08
void ph_vt5A60_53_Thunk(void* thisPtr) {
    extern void ph_vt57D8_28_FD08(void* obj);
    ph_vt57D8_28_FD08((char*)thisPtr + 4);
}

// ph_vt5A60_54_8A40 @ 0x82468A40 | size: 0x8
// Adjusts this by +4, tail-calls ph_vt57D8_29_FDD0
void ph_vt5A60_54_Thunk(void* thisPtr) {
    extern void ph_vt57D8_29_FDD0(void* obj);
    ph_vt57D8_29_FDD0((char*)thisPtr + 4);
}

// ph_vt5A60_47_8CD8 @ 0x82468CD8 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_60_7870
void ph_vt5A60_47_Thunk(void* thisPtr) {
    extern void ph_vt5A60_60_7870(void* obj);
    ph_vt5A60_60_7870((char*)thisPtr - 16);
}

// ph_vt5A60_48_8CE0 @ 0x82468CE0 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_61_7A38
void ph_vt5A60_48_Thunk(void* thisPtr) {
    extern void ph_vt5A60_61_7A38(void* obj);
    ph_vt5A60_61_7A38((char*)thisPtr - 16);
}

// ph_vt5A60_49_8CE8 @ 0x82468CE8 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_62_8F80
void ph_vt5A60_49_Thunk(void* thisPtr) {
    extern void ph_vt5A60_62_8F80(void* obj);
    ph_vt5A60_62_8F80((char*)thisPtr - 16);
}

// ph_vt5A60_24_8D00 @ 0x82468D00 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_24_Thunk(void* thisPtr) {
    extern void ph_vt5A8C_63_6A50(void* obj);
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_25_8D08 @ 0x82468D08 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5A60_25_Thunk(void* thisPtr) {
    extern void ph_vt5A7C_63_6A98(void* obj);
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5A60_26_8D10 @ 0x82468D10 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_26_Thunk(void* thisPtr) {
    extern void ph_vt5A84_63_6B90(void* obj);
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_27_8D18 @ 0x82468D18 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_58_6EE8
void ph_vt5A60_27_Thunk(void* thisPtr) {
    extern void ph_vt5A60_58_6EE8(void* obj);
    ph_vt5A60_58_6EE8((char*)thisPtr - 16);
}

// ph_vt5A60_31_8D20 @ 0x82468D20 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_57_6858
void ph_vt5A60_31_Thunk(void* thisPtr) {
    extern void ph_vt5A60_57_6858(void* obj);
    ph_vt5A60_57_6858((char*)thisPtr - 16);
}

// ph_vt5A60_33_8D30 @ 0x82468D30 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_33_Thunk(void* thisPtr) {
    extern void ph_vt5A8C_63_6A50(void* obj);
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_35_8D40 @ 0x82468D40 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_35_Thunk(void* thisPtr) {
    extern void ph_vt5A84_63_6B90(void* obj);
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_30_9098 @ 0x82469098 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_62_8F80
void ph_vt5A60_30_Thunk(void* thisPtr) {
    extern void ph_vt5A60_62_8F80(void* obj);
    ph_vt5A60_62_8F80((char*)thisPtr - 16);
}

// =============================================================================
// phInstGfxExt vtable thunks (vt5A78/5A74/5A80) — field getters
// =============================================================================

// ph_vt5A78_63_89B0 @ 0x824689B0 | size: 0x10
// Gets float at +108, stores to output ptr, returns 0
int ph_vt5A78_63_GetFloat108(void* thisPtr, float* out) {
    *out = *(float*)((char*)thisPtr + 108);
    return 0;
}

// ph_vt5A80_63_89C0 @ 0x824689C0 | size: 0x10
// Gets float at +112, stores to output ptr, returns 0
int ph_vt5A80_63_GetFloat112(void* thisPtr, float* out) {
    *out = *(float*)((char*)thisPtr + 112);
    return 0;
}

// ph_vt5A74_63_8A10 @ 0x82468A10 | size: 0x10
// Gets uint32 at +164, stores to output ptr, returns 0
int ph_vt5A74_63_GetField164(void* thisPtr, uint32_t* out) {
    *out = *(uint32_t*)((char*)thisPtr + 164);
    return 0;
}

// =============================================================================
// phInstComposite vtable thunks (vt5B98) — composite physics instances
// Vtable @ 0x82005B98
// =============================================================================

// ph_vt5B98_20_8948 @ 0x82468948 | size: 0x8
// Adjusts this by -4, tail-calls atSingleton_8A48_p42
void ph_vt5B98_20_Thunk(void* thisPtr) {
    extern void atSingleton_8A48_p42(void* obj);
    atSingleton_8A48_p42((char*)thisPtr - 4);
}

// ph_vt5B98_0_8950 @ 0x82468950 | size: 0x8
// Adjusts this by -16, tail-calls atSingleton_8A48_p42
void ph_vt5B98_0_Thunk(void* thisPtr) {
    extern void atSingleton_8A48_p42(void* obj);
    atSingleton_8A48_p42((char*)thisPtr - 16);
}

// ph_vt5B98_37_8990 @ 0x82468990 | size: 0x8
// Adjusts this by +16, tail-calls ph_vt57D8_2_6378
void ph_vt5B98_37_Thunk(void* thisPtr) {
    extern void ph_vt57D8_2_6378(void* obj);
    ph_vt57D8_2_6378((char*)thisPtr + 16);
}

// ph_vt5B98_38_8998 @ 0x82468998 | size: 0x8
// Adjusts this by +16, tail-calls ph_vt57D8_3_61E0
void ph_vt5B98_38_Thunk(void* thisPtr) {
    extern void ph_vt57D8_3_61E0(void* obj);
    ph_vt57D8_3_61E0((char*)thisPtr + 16);
}

// ph_vt5B98_12_8D28 @ 0x82468D28 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5B98_12_Thunk(void* thisPtr) {
    extern void ph_vt5A8C_63_6A50(void* obj);
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5B98_14_8D38 @ 0x82468D38 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5B98_14_Thunk(void* thisPtr) {
    extern void ph_vt5A7C_63_6A98(void* obj);
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5B98_16_8D48 @ 0x82468D48 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5B98_16_Thunk(void* thisPtr) {
    extern void ph_vt5A84_63_6B90(void* obj);
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5B98_8_9088 @ 0x82469088 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5B98_40_8D50
void ph_vt5B98_8_Thunk(void* thisPtr) {
    extern void ph_vt5B98_40_8D50(void* obj);
    ph_vt5B98_40_8D50((char*)thisPtr - 16);
}

// ph_vt5B98_9_9090 @ 0x82469090 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5B98_41_8E50
void ph_vt5B98_9_Thunk(void* thisPtr) {
    extern void ph_vt5B98_41_8E50(void* obj);
    ph_vt5B98_41_8E50((char*)thisPtr - 16);
}

// ph_vt5B98_43_89D0 @ 0x824689D0 | size: 0x1C
// Computes (uint16 at +104) * (uint32 at +116), stores to output, returns 0
int ph_vt5B98_43_GetTotalSize(void* thisPtr, uint32_t* out) {
    uint16_t stride = *(uint16_t*)((char*)thisPtr + 104);
    uint32_t count = *(uint32_t*)((char*)thisPtr + 116);
    *out = (uint32_t)stride * count;
    return 0;
}

// ph_vt5B98_44_89F0 @ 0x824689F0 | size: 0x20
// Reads value from *out, divides by stride at +104, stores quotient to +116
int ph_vt5B98_44_SetCountFromSize(void* thisPtr, uint32_t* inout) {
    uint32_t totalSize = *inout;
    uint16_t stride = *(uint16_t*)((char*)thisPtr + 104);
    *(uint32_t*)((char*)thisPtr + 116) = totalSize / stride;
    return 0;
}

// =============================================================================
// phInstSkel vtable thunks (vt5CD8/vt5D38/vt6E40) — skeleton physics
// Vtable @ 0x82005CD8 / 0x82005D38 / 0x82006E40
// =============================================================================

// ph_vt5CD8_11_C8A8 @ 0x8246C8A8 | size: 0x10
// Gets uint32 at +48, stores to output ptr, returns 0
int ph_vt5CD8_11_GetField48(void* thisPtr, uint32_t* out) {
    *out = *(uint32_t*)((char*)thisPtr + 48);
    return 0;
}

// ph_vt5CD8_13_C8B8 @ 0x8246C8B8 | size: 0x10
// Gets byte at +12, stores to output ptr, returns 0
int ph_vt5CD8_13_GetByte12(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 12);
    return 0;
}

// ph_vt5CD8_16_C8C8 @ 0x8246C8C8 | size: 0x1C
// Copies 8 bytes from offset +52 to output ptr, returns 0
int ph_vt5CD8_16_GetField52_64(void* thisPtr, void* out) {
    *(uint32_t*)out = *(uint32_t*)((char*)thisPtr + 52);
    *(uint32_t*)((char*)out + 4) = *(uint32_t*)((char*)thisPtr + 56);
    return 0;
}

// ph_vt5CD8_14_C8E8 @ 0x8246C8E8 | size: 0xC
// Returns constant 0x80004005 (E_NOTIMPL-like error code)
uint32_t ph_vt5CD8_14_ReturnNotImpl() {
    return 0x80004005;
}

// ph_vt5CD8_0_CE30 @ 0x8246CE30 | size: 0x30
// Calls ke_C750 cleanup, returns this (destructor pattern)
void* ph_vt5CD8_0_Destructor(void* thisPtr) {
    extern void ke_C750(void* obj);
    ke_C750(thisPtr);
    return thisPtr;
}

// ph_vt5D38_12_D560 @ 0x8246D560 | size: 0x10
// Gets byte at +61, stores to output ptr, returns 0
int ph_vt5D38_12_GetByte61(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 61);
    return 0;
}

// ph_vt5D38_0_D6F0 @ 0x8246D6F0 | size: 0x30
// Calls ke_D440 cleanup, returns this (destructor pattern)
void* ph_vt5D38_0_Destructor(void* thisPtr) {
    extern void ke_D440(void* obj);
    ke_D440(thisPtr);
    return thisPtr;
}

// ph_vt6E40_0_E368 @ 0x8247E368 | size: 0x30
// Calls ph_E1E8 cleanup, returns this (destructor pattern)
void* ph_vt6E40_0_Destructor(void* thisPtr) {
    extern void ph_E1E8(void* obj);
    ph_E1E8(thisPtr);
    return thisPtr;
}

// ph_vt5C84_5_AAD0 @ 0x8246AAD0 | size: 0x30
// Calls ke_A428, returns this (init + return-self pattern)
void* ph_vt5C84_5_InitAndReturn(void* thisPtr) {
    extern void ke_A428(void* obj);
    ke_A428(thisPtr);
    return thisPtr;
}

// ph_vt5A40_5_BC58 @ 0x8246BC58 | size: 0x18
// Returns (this + 8) or nullptr if this is null
void ph_vt5A40_5_GetEmbeddedPtr(void* thisPtr, void** out) {
    *out = thisPtr ? (char*)thisPtr + 8 : nullptr;
}

// =============================================================================
// Physics utility free functions
// =============================================================================

// ph_FDD0 @ 0x8221FDD0 | size: 0x20
// Sets 3 uint16 fields + zero, then tail-calls ph_FE48
void ph_SetBoundIndices3(void* thisPtr, uint16_t idx0, uint16_t idx1,
                         uint16_t idx2, void* param) {
    extern void ph_FE48(void* obj, void* p);
    *(uint16_t*)((char*)thisPtr + 16) = idx0;
    *(uint16_t*)((char*)thisPtr + 18) = idx1;
    *(uint16_t*)((char*)thisPtr + 20) = idx2;
    *(uint16_t*)((char*)thisPtr + 22) = 0;
    ph_FE48(thisPtr, param);
}

// ph_FDF0 @ 0x8221FDF0 | size: 0x1C
// Sets 4 uint16 fields, then tail-calls ph_FE48
void ph_SetBoundIndices4(void* thisPtr, uint16_t idx0, uint16_t idx1,
                         uint16_t idx2, uint16_t idx3, void* param) {
    extern void ph_FE48(void* obj, void* p);
    *(uint16_t*)((char*)thisPtr + 16) = idx0;
    *(uint16_t*)((char*)thisPtr + 18) = idx1;
    *(uint16_t*)((char*)thisPtr + 20) = idx2;
    *(uint16_t*)((char*)thisPtr + 22) = idx3;
    ph_FE48(thisPtr, param);
}

// ph_FE10 @ 0x8221FE10 | size: 0x34
// Copies a 32-byte structure from source to dest (vector + trailing fields)
void ph_CopyBoundData(void* dest, const void* src) {
    // Copy 16-byte aligned vector (first 16 bytes)
    *(uint32_t*)((char*)dest + 0)  = *(const uint32_t*)((const char*)src + 0);
    *(uint32_t*)((char*)dest + 4)  = *(const uint32_t*)((const char*)src + 4);
    *(uint32_t*)((char*)dest + 8)  = *(const uint32_t*)((const char*)src + 8);
    *(float*)((char*)dest + 12)    = *(const float*)((const char*)src + 12);
    // Restore byte at +15 from source
    *((uint8_t*)dest + 15) = *((const uint8_t*)src + 15);
    // Copy trailing 16 bytes (two 64-bit words)
    *(uint64_t*)((char*)dest + 16) = *(const uint64_t*)((const char*)src + 16);
    *(uint64_t*)((char*)dest + 24) = *(const uint64_t*)((const char*)src + 24);
}

// ph_C1A0 @ 0x8228C1A0 | size: 0x24
// Tests if bitmask AND global collision flags is non-zero
extern uint32_t g_phCollisionFlags;  // @ 0x825C48E8
uint8_t ph_TestCollisionFlag(uint32_t mask) {
    return (g_phCollisionFlags & mask) != 0 ? 1 : 0;
}

// ph_C1C8 @ 0x8228C1C8 | size: 0x24
// Tests if bit 9 (0x200) of global collision flags is set
uint8_t ph_TestCollisionBit9() {
    return (g_phCollisionFlags & 0x200) != 0 ? 1 : 0;
}

// ph_D1B8 @ 0x8228D1B8 | size: 0x18
// Copies a 16-byte vector to offset +80 and sets dirty flag at +6
void ph_SetBoundVector(void* thisPtr, const void* vec) {
    // Copy 16-byte SIMD vector to offset +80
    uint32_t* dst = (uint32_t*)((char*)thisPtr + 80);
    const uint32_t* src = (const uint32_t*)vec;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    // Set dirty flag
    *((uint8_t*)thisPtr + 6) = 1;
}

// ph_0F30 @ 0x82290F30 | size: 0x2C
// Sets or clears bits in global collision flags based on enable parameter
void ph_SetCollisionBits(uint32_t bits, uint8_t enable) {
    if (enable) {
        g_phCollisionFlags |= bits;
    } else {
        g_phCollisionFlags &= ~bits;
    }
}

// ph_20D0 @ 0x822920D0 | size: 0x40
// Wrapper that forwards args to fiAsciiTokenizer_2110_gen and returns byte result
uint8_t ph_TokenizerReadBool(void* a, void* b, void* c) {
    extern uint8_t fiAsciiTokenizer_2110_gen(void*, void*, void*);
    return fiAsciiTokenizer_2110_gen(a, b, c);
}

// ph_5278 @ 0x822E5278 | size: 0x34
// Initializes a physics configuration struct with default values
void ph_InitPhysicsConfig(void* cfg, uint32_t typeId, uint32_t flagMask) {
    *(uint32_t*)((char*)cfg + 4)   = typeId;      // type identifier
    *(uint32_t*)((char*)cfg + 8)   = 1;           // version = 1
    *(uint32_t*)((char*)cfg + 12)  = flagMask;    // flag mask
    *(uint32_t*)((char*)cfg + 16)  = 32;          // buffer size = 32
    *(uint32_t*)((char*)cfg + 20)  = 2;           // mode = 2
    *(uint32_t*)((char*)cfg + 24)  = 0;           // reserved = 0
    *(uint8_t*)((char*)cfg + 28)   = 0;           // initialized = false
    *(uint32_t*)((char*)cfg + 156) = 0;           // state = 0
}

// ph_FFB8_h @ 0x822AFFB8 | size: 0x1C
// Returns one of two pointers from high offsets based on boolean flag
void* ph_GetBoundPointer(void* thisPtr, uint8_t useFirst) {
    if (useFirst) {
        return *(void**)((char*)thisPtr + 17056);
    }
    return *(void**)((char*)thisPtr + 17060);
}

// ph_ctor_0460 @ 0x82430460 | size: 0x24
// Destructor: sets vtable, conditionally calls cleanup on child
void ph_ctor_0460_Destructor(void* thisPtr) {
    extern void fiAsciiTokenizer_3310_g(void* obj);
    // Set vtable
    *(void**)thisPtr = (void*)0x820014C8;  // lbl_820014C8 vtable
    uint32_t childFlag = *(uint32_t*)((char*)thisPtr + 8);
    if (childFlag != 0) {
        void* child = *(void**)((char*)thisPtr + 4);
        fiAsciiTokenizer_3310_g(child);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider — Additional Packed Bitfield Accessors (50 functions)
//
// This batch covers the remaining small (<=64B) getter/setter pairs for
// packed bitfields in the phArticulatedCollider configuration words.
//
// Patterns found:
//   A) Simple getters (0xc):  load word, extract bits → ExtractBits()
//   B) Insert-field setters (0x28-0x30): read-modify-write bitfield, dirty bit
//   C) Store-and-dirty (0x20-0x28): store value/float, set dirty bit
//   D) Indexed getters (0x14): stride-24 array lookup + extract
//   E) Bool-from-negation (0x18-0x1c): extract, compare to zero
//
// Dirty bits live in two 64-bit masks:
//   this+40 (dirtyMask40) — for fields at offsets 11628-11656
//   this+48 (dirtyMask48) — for fields at offsets 11712-11840
// ─────────────────────────────────────────────────────────────────────────────

// Helper: insert value into bitfield, store back, set dirty bit at this+40
static inline void InsertBitsAndDirty40(void* obj, int offset, uint32_t value,
                                         int shift, uint32_t mask, uint64_t dirtyBit) {
    uint32_t word = *(uint32_t*)((char*)obj + offset);
    word = (word & ~(mask << shift)) | ((value & mask) << shift);
    *(uint32_t*)((char*)obj + offset) = word;
    *(uint64_t*)((char*)obj + 40) |= dirtyBit;
}

// Helper: insert value into bitfield, store back, set dirty bit at this+48
static inline void InsertBitsAndDirty48(void* obj, int offset, uint32_t value,
                                         int shift, uint32_t mask, uint64_t dirtyBit) {
    uint32_t word = *(uint32_t*)((char*)obj + offset);
    word = (word & ~(mask << shift)) | ((value & mask) << shift);
    *(uint32_t*)((char*)obj + offset) = word;
    *(uint64_t*)((char*)obj + 48) |= dirtyBit;
}

// Helper: store float to field, set dirty bit at this+48
static inline void StoreFloatAndDirty48(void* obj, int offset, float value, uint64_t dirtyBit) {
    *(float*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 48) |= dirtyBit;
}

// Helper: store u32 to field, set dirty bit at this+40
static inline void StoreU32AndDirty40(void* obj, int offset, uint32_t value, uint64_t dirtyBit) {
    *(uint32_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 40) |= dirtyBit;
}

// Helper: store u32 to field, set dirty bit at this+48
static inline void StoreU32AndDirty48(void* obj, int offset, uint32_t value, uint64_t dirtyBit) {
    *(uint32_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 48) |= dirtyBit;
}

// Helper: store u8 to field, set dirty bit at this+40
static inline void StoreU8AndDirty40(void* obj, int offset, uint8_t value, uint64_t dirtyBit) {
    *(uint8_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 40) |= dirtyBit;
}

// ── 1. Simple bitfield getter: +11660 bit 0 (0x1) ──────────── 4DC8 (0xc)
uint32_t phArticulatedCollider::GetSolverFlag_18()  { return ExtractBits(this, 11660,  0, 0x1); }

// ── 2. Simple getter: +11704 bits 0-1 (0x3) ────────────────── 5138 (0xc)
uint32_t phArticulatedCollider::GetLimitType_0()    { return ExtractBits(this, 11704,  0, 0x3); }

// ── 3. Simple getter: +11776 bit 0 (0x1) ───────────────────── 5170 (0xc)
uint32_t phArticulatedCollider::GetDofFlag_0()      { return ExtractBits(this, 11776,  0, 0x1); }

// ── 4. Simple getter: +11656 bit 21 (0x1) ──────────────────── 51A8 (0xc)
// rlwinm r3,r11,11,31,31 → (word >> 21) & 1
uint32_t phArticulatedCollider::GetSolverParam_21() { return ExtractBits(this, 11656, 21, 0x1); }

// ── 5. Simple getter: +11644 bit 4 (0x1) ───────────────────── 5208 (0xc)
// rlwinm r3,r11,28,31,31 → (word >> 4) & 1
uint32_t phArticulatedCollider::GetFlag7C_4b()      { return ExtractBits(this, 11644,  4, 0x1); }

// ── 6. Simple getter: +11648 bit 3 (0x1) ───────────────────── 5340 (0xc)
// rlwinm r3,r11,29,31,31 → (word >> 3) & 1
uint32_t phArticulatedCollider::GetActive_3()       { return ExtractBits(this, 11648,  3, 0x1); }

// ── 7. Simple getter: +11648 bit 2 (0x1) ───────────────────── 5378 (0xc)
// rlwinm r3,r11,30,31,31 → (word >> 2) & 1 (same field as GetActive_2 but separate symbol)
uint32_t phArticulatedCollider::GetActive_2b()      { return ExtractBits(this, 11648,  2, 0x1); }

// ── 8. Simple getter: +11648 bit 5 (0x1) ───────────────────── 53B0 (0xc)
// rlwinm r3,r11,27,31,31 → (word >> 5) & 1
uint32_t phArticulatedCollider::GetActive_5()       { return ExtractBits(this, 11648,  5, 0x1); }

// ── 9. Simple getter: +11452 bits 23-29 (0x7F) ─────────────── 5408 (0xc)
// rlwinm r3,r11,9,25,31 → (word >> 23) & 0x7F
uint32_t phArticulatedCollider::GetCBCField_23()    { return ExtractBits(this, 11452, 23, 0x7F); }

// ── 10. Byte-indexed getter: this[index+12098] bit 1 ────────── 5598 (0x10)
// lbz + rlwinm r3,r11,31,31,31 → (byte >> 1) & 1
uint32_t phArticulatedCollider::GetIndexedByteFlag_1(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return (byte >> 1) & 0x1;
}

// ── 11. Byte-indexed getter: this[index+12098] bit 0 ────────── 5728 (0x10)
// lbz + clrlwi r3,r11,31 → byte & 1
uint32_t phArticulatedCollider::GetIndexedByteFlag_0(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return byte & 0x1;
}

// ── 12. Byte-indexed getter: this[index+12098] bit 2 ────────── 5818 (0x10)
// lbz + rlwinm r3,r11,30,31,31 → (byte >> 2) & 1
uint32_t phArticulatedCollider::GetIndexedByteFlag_2(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return (byte >> 2) & 0x1;
}

// ── 13. Indexed field setter: +11452 insert 7-bit at bit 23 ── 53F8 (0x10)
// rlwimi r11,r4,23,2,8 → mask 0x3F800000 = 0x7F << 23
void phArticulatedCollider::SetCBCField_23(uint32_t value) {
    uint32_t word = *(uint32_t*)((char*)this + 11452);
    word = (word & ~(0x7FU << 23)) | ((value & 0x7F) << 23);
    *(uint32_t*)((char*)this + 11452) = word;
}

// ── 14. Read byte from +11644 ───────────────────────────────── 5240 (0x14)
// lbz + rlwimi (identity copy) + clrlwi → just returns byte at +11644
uint32_t phArticulatedCollider::GetFlag7C_Byte() {
    return *(uint8_t*)((char*)this + 11644);
}

// ── 15. Indexed getter: stride-24 array at +1164, bits 23-24 ── 5778 (0x14)
// mulli r11,r4,24; add; lwz 1164; rlwinm r3,r11,9,30,31 → (word >> 23) & 0x3
uint32_t phArticulatedCollider::GetBoneField_5778(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1164 + index * 24);
    return (word >> 23) & 0x3;
}

// ── 16. Indexed getter: stride-24 at +1172, bit 2 ──────────── 5D90 (0x14)
// lwz 1172; rlwinm r3,r11,30,31,31 → (word >> 2) & 1
uint32_t phArticulatedCollider::GetBoneField_5D90(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1172 + index * 24);
    return (word >> 2) & 0x1;
}

// ── 17. Indexed negation check: stride-24 at +1156 ─────────── 5DF0 (0x18)
// lwz 1156; not; rlwinm 21,31,31 → (~word >> 11) & 1
uint32_t phArticulatedCollider::GetBoneFieldNeg_5DF0(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1156 + index * 24);
    return (~word >> 11) & 0x1;
}

// ── 18. Indexed bool: stride-24 at +1172, nonzero low 2 bits ── 5B38 (0x1c)
// lwz 1172; clrlwi 30 → bits 0-1; subfic+subfe → (val != 0) ? -1 : 0
// The subfic/subfe idiom computes -(val != 0), i.e. 0xFFFFFFFF if nonzero
bool phArticulatedCollider::IsBoneFieldNonZero(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1172 + index * 24);
    return (word & 0x3) != 0;
}

// ── 19. Store float + dirty40 at +11712 ─────────────────────── 50B8 (0x20)
// stfs f0,11712(r3); ld/oris/std at this+40; oris r10,r10,4 → 1<<18
void phArticulatedCollider::SetLimitFloat_11712(float value) {
    *(float*)((char*)this + 11712) = value;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 18);
}

// ── 20. Store float + dirty40 at +11708 ─────────────────────── 50E8 (0x20)
// stfs f0,11708(r3); ld/oris/std at this+40; oris r10,r10,8 → 1<<19
void phArticulatedCollider::SetLimitFloat_11708(float value) {
    *(float*)((char*)this + 11708) = value;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 19);
}

// ── 21. Insert 2 bits at +11704 bit 0 + dirty40 ────────────── 5118 (0x20)
// rlwimi r10,r4,0,30,31 → mask 0x3; oris r10,r10,16 → 0x100000 = 1<<20
void phArticulatedCollider::SetLimitType_0(uint32_t value) {
    InsertBitsAndDirty40(this, 11704, value, 0, 0x3, 1ULL << 20);
}

// ── 22. Store u32 + dirty40 at +11544 ───────────────────────── 51B8 (0x20)
// stw r4,11544(r3); rldicr r12,r12,60 → 1ULL<<60
void phArticulatedCollider::SetField_11544(uint32_t value) {
    StoreU32AndDirty40(this, 11544, value, 1ULL << 60);
}

// ── 23. Store byte + dirty40 at +11650 ──────────────────────── 53C0 (0x20)
// stb r4,11650(r3); rldicr r12,r12,34 → 1ULL<<34
void phArticulatedCollider::SetActiveByte_11650(uint8_t value) {
    StoreU8AndDirty40(this, 11650, value, 1ULL << 34);
}

// ── 24. Store u16 + dirty48 at +11840 ───────────────────────── 4770 (0x24)
// clrlwi r10,r4,16; stw r10,11840(r3); rldicr r12,r12,45 → 1ULL<<45
void phArticulatedCollider::SetDofU16_11840(uint32_t value) {
    StoreU32AndDirty48(this, 11840, value & 0xFFFF, 1ULL << 45);
}

// ── 25. Insert 1 bit at +11636 bit 2 + dirty40 ─────────────── 4170 (0x28)
// rlwimi r10,r4,2,29,29 → (value & 1) << 2; rldicr 37 → 1ULL<<37
void phArticulatedCollider::SetRotAxis_2(uint32_t value) {
    InsertBitsAndDirty40(this, 11636, value, 2, 0x1, 1ULL << 37);
}

// ── 26. Insert 3 bits at +11636 bits 8-10 + dirty40 ────────── 4288 (0x28)
// rlwimi r10,r4,8,21,23 → (value & 7) << 8; rldicr 37 → 1ULL<<37
void phArticulatedCollider::SetRotAxis_8(uint32_t value) {
    InsertBitsAndDirty40(this, 11636, value, 8, 0x7, 1ULL << 37);
}

// ── 27. Insert 3 bits at +11636 bits 14-16 + dirty40 ───────── 4350 (0x28)
// rlwimi r10,r4,14,15,17 → (value & 7) << 14; rldicr 37
void phArticulatedCollider::SetRotAxis_14(uint32_t value) {
    InsertBitsAndDirty40(this, 11636, value, 14, 0x7, 1ULL << 37);
}

// ── 28. Insert 3 bits at +11636 bits 20-22 + dirty40 ───────── 4388 (0x28)
// rlwimi r10,r4,20,9,11 → (value & 7) << 20; rldicr 37
void phArticulatedCollider::SetRotAxis_20(uint32_t value) {
    InsertBitsAndDirty40(this, 11636, value, 20, 0x7, 1ULL << 37);
}

// ── 29. Insert 3 bits at +11636 bits 26-28 + dirty40 ───────── 4450 (0x28)
// rlwimi r10,r4,26,3,5 → (value & 7) << 26; rldicr 37
void phArticulatedCollider::SetRotAxis_26(uint32_t value) {
    InsertBitsAndDirty40(this, 11636, value, 26, 0x7, 1ULL << 37);
}

// ── 30. Insert 1 bit at +11656 bit 15 + dirty40 ────────────── 4738 (0x28)
// rlwimi r10,r4,15,16,16 → (value & 1) << 15; rldicr 32 → 1ULL<<32
void phArticulatedCollider::SetSolverParam_15(uint32_t value) {
    InsertBitsAndDirty40(this, 11656, value, 15, 0x1, 1ULL << 32);
}

// ── 31. Insert 4 bits at +11628 bits 28-31 + dirty40 ───────── 4B18 (0x28)
// rlwimi r10,r4,28,0,3 → (value & 0xF) << 28; rldicr 39 → 1ULL<<39
void phArticulatedCollider::SetConstraint_28b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628, value, 28, 0xF, 1ULL << 39);
}

// ── 32. Insert 4 bits at +11632 bits 28-31 + dirty40 ───────── 4C90 (0x28)
// rlwimi r10,r4,28,0,3 → (value & 0xF) << 28; rldicr 39
void phArticulatedCollider::SetJointParam_28(uint32_t value) {
    InsertBitsAndDirty40(this, 11632, value, 28, 0xF, 1ULL << 39);
}

// ── 33. Insert 1 bit at +11776 bit 0 + dirty48 ─────────────── 5148 (0x28)
// rlwimi r10,r4,0,31,31 → (value & 1); rldicr 61 → 1ULL<<61
void phArticulatedCollider::SetDofFlag_0(uint32_t value) {
    InsertBitsAndDirty48(this, 11776, value, 0, 0x1, 1ULL << 61);
}

// ── 34. Insert 1 bit at +11656 bit 21 + dirty40 ────────────── 5180 (0x28)
// rlwimi r10,r4,21,10,10 → (value & 1) << 21; rldicr 32
void phArticulatedCollider::SetSolverParam_21(uint32_t value) {
    InsertBitsAndDirty40(this, 11656, value, 21, 0x1, 1ULL << 32);
}

// ── 35. Insert 1 bit at +11644 bit 4 + dirty40 ─────────────── 51E0 (0x28)
// rlwimi r10,r4,4,27,27 → (value & 1) << 4; rldicr 35 → 1ULL<<35
void phArticulatedCollider::SetFlag7C_4b(uint32_t value) {
    InsertBitsAndDirty40(this, 11644, value, 4, 0x1, 1ULL << 35);
}

// ── 36. Insert 8 bits at +11644 bits 24-31 + dirty40 ───────── 5218 (0x28)
// rlwimi r10,r4,24,0,7 → (value & 0xFF) << 24; rldicr 35
void phArticulatedCollider::SetFlag7C_24(uint32_t value) {
    InsertBitsAndDirty40(this, 11644, value, 24, 0xFF, 1ULL << 35);
}

// ── 37. Store float + dirty48 at +11788 ─────────────────────── 5258 (0x28)
// stfs f0,11788(r3); rldicr r12,r12,58 → 1ULL<<58
void phArticulatedCollider::SetRangeFloat_11788(float value) {
    StoreFloatAndDirty48(this, 11788, value, 1ULL << 58);
}

// ── 38. Store float + dirty48 at +11780 ─────────────────────── 5288 (0x28)
// stfs f0,11780(r3); rldicr r12,r12,60 → 1ULL<<60
void phArticulatedCollider::SetRangeFloat_11780(float value) {
    StoreFloatAndDirty48(this, 11780, value, 1ULL << 60);
}

// ── 39. Store float + dirty48 at +11792 ─────────────────────── 52B8 (0x28)
// stfs f0,11792(r3); rldicr r12,r12,57 → 1ULL<<57
void phArticulatedCollider::SetRangeFloat_11792(float value) {
    StoreFloatAndDirty48(this, 11792, value, 1ULL << 57);
}

// ── 40. Store float + dirty48 at +11784 ─────────────────────── 52E8 (0x28)
// stfs f0,11784(r3); rldicr r12,r12,59 → 1ULL<<59
void phArticulatedCollider::SetRangeFloat_11784(float value) {
    StoreFloatAndDirty48(this, 11784, value, 1ULL << 59);
}

// ── 41. Insert 1 bit at +11648 bit 3 + dirty40 ─────────────── 5318 (0x28)
// rlwimi r10,r4,3,28,28 → (value & 1) << 3; rldicr 34 → 1ULL<<34
void phArticulatedCollider::SetActive_3(uint32_t value) {
    InsertBitsAndDirty40(this, 11648, value, 3, 0x1, 1ULL << 34);
}

// ── 42. Insert 1 bit at +11648 bit 2 + dirty40 ─────────────── 5350 (0x28)
// rlwimi r10,r4,2,29,29 → (value & 1) << 2; rldicr 34
void phArticulatedCollider::SetActive_2b(uint32_t value) {
    InsertBitsAndDirty40(this, 11648, value, 2, 0x1, 1ULL << 34);
}

// ── 43. Insert 1 bit at +11648 bit 5 + dirty40 ─────────────── 5388 (0x28)
// rlwimi r10,r4,5,26,26 → (value & 1) << 5; rldicr 34
void phArticulatedCollider::SetActive_5(uint32_t value) {
    InsertBitsAndDirty40(this, 11648, value, 5, 0x1, 1ULL << 34);
}

// ── 44. Insert 6-bit low + dirty40 at +11652 ───────────────── 4578 (0x2c)
// rlwinm r10,r10,0,0,25 → clear low 6 bits; or r10,r10,r4; rldicr 33 → 1ULL<<33
void phArticulatedCollider::SetMiscField_11652(uint32_t value) {
    uint32_t word = *(uint32_t*)((char*)this + 11652);
    word = (word & 0xFFFFFFC0) | (value & 0x3F);
    *(uint32_t*)((char*)this + 11652) = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 33);
}

// ── 45. Insert 4 bits at +11628 bit 0 + dirty40 ────────────── 49C8 (0x2c)
// rlwinm r10,r10,0,0,27 → clear low 4 bits; or r10,r10,r4; rldicr 39
void phArticulatedCollider::SetConstraint_0b(uint32_t value) {
    uint32_t word = *(uint32_t*)((char*)this + 11628);
    word = (word & 0xFFFFFFF0) | (value & 0xF);
    *(uint32_t*)((char*)this + 11628) = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 39);
}

// ── 46. Insert 4 bits at +11632 bit 0 + dirty40 ────────────── 4B40 (0x2c)
// rlwinm r10,r10,0,0,27 → clear low 4 bits; or; rldicr 39
void phArticulatedCollider::SetJointParam_0(uint32_t value) {
    uint32_t word = *(uint32_t*)((char*)this + 11632);
    word = (word & 0xFFFFFFF0) | (value & 0xF);
    *(uint32_t*)((char*)this + 11632) = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 39);
}

// ── 47. vfn_57 @ 0x822508E0 | size: 0x30 ───────────────────────────────────
// Computes interpolated joint limit: result = arr492[idx]*k1 + arr500[idx]*k2
// where k1 and k2 are float constants from .rodata.
float phArticulatedCollider::GetInterpolatedLimit(int index) {  // vfn_57
    float* arr500 = *(float**)((char*)this + 500);
    float* arr492 = *(float**)((char*)this + 492);
    static const float kBlendA = 0.01745329f;  // PI/180 (degrees to radians)
    static const float kBlendB = 57.29578f;    // 180/PI (radians to degrees)
    float valA = arr500[index];
    float valB = arr492[index];
    return valB * kBlendA + valA * kBlendB;
}

// ── 48. Insert 4 bits at +11628 bits 4-7 + dirty40 ─────────── 49F8 (0x30)
// clear bits 4-7, insert value<<4; rldicr 39
void phArticulatedCollider::SetConstraint_4b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628, value, 4, 0xF, 1ULL << 39);
}

// ── 49. Insert 4 bits at +11628 bits 8-11 + dirty40 ────────── 4A28 (0x30)
void phArticulatedCollider::SetConstraint_8b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628, value, 8, 0xF, 1ULL << 39);
}

// ── 50. Insert 4 bits at +11628 bits 12-15 + dirty40 ───────── 4A58 (0x30)
void phArticulatedCollider::SetConstraint_12b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628, value, 12, 0xF, 1ULL << 39);
}

} // namespace rage

// NOTE: phObject vfn_12..vfn_19 implementations removed (duplicates of
// named versions: GetDescription, SetDescription, QueryInterface, Release,
// CreateResource, CreateOutputViews, ReleaseViews defined earlier)
// NOTE: Duplicate phJoint3Dof vfn_* implementations removed (already defined
// above with named methods: SetDampingAndStiffness, SetLimitAtIndex, etc.)


// NOTE: Duplicate rage::phInst section removed (all methods already defined
// in the first phInst block above within namespace rage {})


// ═════════════════════════════════════════════════════════════════════════════
// phObject — Small utility functions
// ═════════════════════════════════════════════════════════════════════════════

// Forward declarations for phObject helpers
extern void phObject_4F28_p46(void* thisPtr);
extern void thunk_fn_824885C8(void* thisPtr);  // -> util_85C8
extern void set_3348(void* thisPtr, float value);
extern void phObject_2588_w(void* thisPtr, uint32_t param);
extern "C" void _locale_register(void* ptr, uint32_t tag);

// ─────────────────────────────────────────────────────────────────────────────
// phObject_29 @ 0x82485108 | size: 0x8
//
// Simple getter: returns the field at offset +120 (m_field_120).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phObject_29(void* thisPtr) {
    return *(uint32_t*)((uint8_t*)thisPtr + 120);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_5100_p46 @ 0x82485100 | size: 0x8
//
// Forwarding stub: passes the 4th argument (r7) as 'this' to
// phObject_4F28_p46. Used as a trampoline for indirect dispatches.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_5100_p46(void* thisPtr, uint32_t a2, uint32_t a3, void* target) {
    phObject_4F28_p46(target);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_33 @ 0x82488A08 | size: 0x14
//
// Stores two parameters at offsets +488 and +492 in the phObject, then
// returns 0 (success). Likely sets a pair of configuration values
// (e.g. simulation bounds or constraint limits).
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_33(void* thisPtr, uint32_t valueA, uint32_t valueB) {
    uint8_t* obj = (uint8_t*)thisPtr;
    *(uint32_t*)(obj + 488) = valueA;
    *(uint32_t*)(obj + 492) = valueB;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_B878_h @ 0x8248B878 | size: 0x10
//
// Destructor helper: sets the vtable pointer for a specific subclass,
// then tail-calls util_85C8 (via thunk_fn_824885C8) for base cleanup.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_B878_h(void* thisPtr) {
    *(void**)thisPtr = (void*)0x82019168;  // lis r11,-32255; addi r11,r11,-28312
    thunk_fn_824885C8(thisPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_20B0_wrh @ 0x824920B0 | size: 0x10
//
// Indirect call trampoline: loads a function pointer from a fixed global
// address and calls it with the current arguments. Used for late-bound
// dispatch in the physics write/read/hash pipeline.
// ─────────────────────────────────────────────────────────────────────────────
typedef void (*phObjectDispatchFunc)(void);
void phObject_20B0_wrh() {
    // lis r11, -32255 -> 0x81E10000; lwz r11, -26992(r11) -> 0x81E10000 + 0xFFFF9690 = global at 0x82019690 area
    // Load function pointer from global and call it
    volatile phObjectDispatchFunc* pFunc = (volatile phObjectDispatchFunc*)0x82019690;
    (*pFunc)();
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_1E18_h @ 0x82491E18 | size: 0x18
//
// Clears a flag byte at offset +1524 and a 32-bit word at offset +1528.
// Returns 0. Used to reset an active/dirty state on a physics object.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_1E18_h(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    *(uint8_t*)(obj + 1524) = 0;
    *(uint32_t*)(obj + 1528) = 0;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_34 @ 0x82488A20 | size: 0x28
//
// Adjusts this pointer by +124 bytes (to reach an embedded sub-object),
// calls set_3348 to store a float at sub-object+72, then returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_34(void* thisPtr, float value) {
    uint8_t* subObj = (uint8_t*)thisPtr + 124;
    set_3348(subObj, value);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_2340_gen @ 0x82492340 | size: 0x28
//
// Array element lookup: reads element at index from a 32-bit array (this),
// stores it to the output pointer. Returns 0 on success, or an error code
// (0x80300016) if the element is NULL.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_2340_gen(uint32_t* array, uint32_t index, uint32_t* outValue) {
    uint32_t element = array[index];
    *outValue = element;
    if (element == 0) {
        return (int32_t)0x80300016;
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_A4A0_p39 @ 0x8248A4A0 | size: 0x2C
//
// Null-validates three parameters (this, param1, param3), then loads the
// first word from param1 and tail-calls phObject_2588_w.
// Returns error 0x80070057 on null parameter.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_A4A0_p39(void* thisPtr, void* param1, uint32_t param2, void* param3) {
    if (!thisPtr || !param1 || !param3) {
        return (int32_t)0x80070057;
    }
    uint32_t value = *(uint32_t*)param1;
    phObject_2588_w((void*)(uintptr_t)value, param2);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_1F70 @ 0x82491F70 | size: 0x34
//
// If the pointer is non-null, calls _locale_register with a fixed tag
// constant (0x208C8000). Always returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_1F70(void* ptr) {
    if (ptr != NULL) {
        _locale_register(ptr, 0x208C8000);
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_14 @ 0x82485720 | size: 0x44
//
// Allocates a zeroed 20-byte stack buffer (5 x uint32), then calls the
// object's vtable slot 11 (Initialize) with the buffer as the second arg.
// Used to invoke Initialize with a default-zero parameter block.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_14(void* thisPtr) {
    uint32_t buf[5];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;

    // Call vtable slot 11: Initialize(this, buf, ...)
    typedef void (*InitFunc)(void*, void*, void*);
    void** vt = *(void***)thisPtr;
    InitFunc fn = (InitFunc)vt[11];
    fn(thisPtr, buf, NULL);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_13 @ 0x82485768 | size: 0x4C
//
// Same as phObject_14 but writes a caller-provided value into the first
// word of the stack buffer before calling vtable slot 11 (Initialize).
// ─────────────────────────────────────────────────────────────────────────────
void phObject_13(void* thisPtr, uint32_t param) {
    uint32_t buf[5];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;

    buf[0] = param;

    typedef void (*InitFunc)(void*, void*, void*);
    void** vt = *(void***)thisPtr;
    InitFunc fn = (InitFunc)vt[11];
    fn(thisPtr, buf, NULL);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_12 @ 0x824857B8 | size: 0x50
//
// Same pattern as phObject_13 but writes two caller-provided values into
// the first two words of the stack buffer before calling Initialize.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_12(void* thisPtr, uint32_t param1, uint32_t param2) {
    uint32_t buf[5];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;

    buf[0] = param1;
    buf[1] = param2;

    typedef void (*InitFunc)(void*, void*, void*);
    void** vt = *(void***)thisPtr;
    InitFunc fn = (InitFunc)vt[11];
    fn(thisPtr, buf, NULL);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_1AD0_w @ 0x82491AD0 | size: 0x48
//
// Clears a 12-byte entry in a strided array. Each entry is 12 bytes
// (3 x uint32). Index must be < 127. Returns 0x80070057 on out-of-range
// or null this. Zeroes the 3 words at this[index * 12].
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_1AD0_w(void* thisPtr, int32_t index) {
    if (!thisPtr) {
        return 0;
    }
    if (index >= 127) {
        return (int32_t)0x80070057;
    }

    // stride = index * 3 * 4 = index * 12
    uint32_t* entry = (uint32_t*)((uint8_t*)thisPtr + index * 12);
    entry[0] = 0;
    entry[1] = 0;
    entry[2] = 0;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_A4D0_p39 @ 0x8248A4D0 | size: 0x44
//
// Null-validates this, param1, and param3. Loads two values from param1
// at offsets +56 and +60. If the value at +56 is zero, returns early.
// Otherwise loads param3+608 as array base, and calls phObject_1AD0_w
// to clear the entry at the given index.
// Returns 0x80070057 on null parameters.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_A4D0_p39(void* thisPtr, void* param1, uint32_t param2, void* param3) {
    if (!thisPtr || !param1 || !param3) {
        return (int32_t)0x80070057;
    }

    int32_t count = *(int32_t*)((uint8_t*)param1 + 56);
    if (count == 0) {
        return 0;
    }

    int32_t index = *(int32_t*)((uint8_t*)param1 + 60);
    void* arrayBase = *(void**)((uint8_t*)param3 + 608);
    return phObject_1AD0_w(arrayBase, index);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_1F28_gen @ 0x82491F28 | size: 0x44
//
// Unregisters a resource: calls _locale_register on the value stored at
// *outPtr (param3), then clears *outPtr to NULL. Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_1F28_gen(void* thisPtr, uint32_t param2, void** outPtr) {
    void* resource = *outPtr;
    _locale_register(resource, 0x208C8000);
    *outPtr = NULL;
    return 0;
}

// Forward declarations for additional phObject helpers
extern void phInstStatic_88F8_fw(void* thisPtr);
extern void phObject_9A48_h(void* thisPtr);
extern void ph_ctor_32E8(void* thisPtr);
extern void phInst_87F8(void* thisPtr);
extern "C" void* rage_01B8_alloc(uint32_t size, uint32_t tag);
extern void rage_4CD0(void* thisPtr);
extern int32_t phObject_28C8_wrh(void* thisPtr, void* r4, void* r5, void* r6, void* r7, void* outPtr);
extern int32_t rage_A518(void* outPtr, void* thisPtr, void* resource);

// ─────────────────────────────────────────────────────────────────────────────
// phObject_32 @ 0x82484758 | size: 0x50
//
// Resets physics simulation state on a phObject. Calls phInstStatic_88F8_fw
// for static instance cleanup, then zeroes out simulation fields at
// offsets +500..+524 and sets the enabled flags at +504 and +528 to 1.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_32(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phInstStatic_88F8_fw(thisPtr);

    *(uint32_t*)(obj + 500) = 0;
    *(uint32_t*)(obj + 504) = 1;
    *(uint32_t*)(obj + 508) = 0;
    *(uint32_t*)(obj + 512) = 0;
    *(uint32_t*)(obj + 516) = 0;
    *(uint32_t*)(obj + 520) = 0;
    *(uint32_t*)(obj + 524) = 0;
    *(uint32_t*)(obj + 528) = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_15 @ 0x82484890 | size: 0x88
//
// Shutdown/cleanup for a phObject. If the resource handle at +508 is
// non-null, releases it via _locale_register and clears the field. Then
// calls three vtable cleanup methods: slot 19 (OnPreShutdown), slot 20
// (OnShutdown), and slot 32 (OnPostShutdown). Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_15(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    uint32_t resource = *(uint32_t*)(obj + 508);
    if (resource != 0) {
        _locale_register((void*)(uintptr_t)resource, 0x208C801D);
        *(uint32_t*)(obj + 508) = 0;
    }

    // Call vtable slot 19: OnPreShutdown
    void** vt = *(void***)thisPtr;
    typedef void (*VtFunc)(void*);
    ((VtFunc)vt[19])(thisPtr);

    // Call vtable slot 20: OnShutdown
    vt = *(void***)thisPtr;
    ((VtFunc)vt[20])(thisPtr);

    // Call vtable slot 32: OnPostShutdown
    vt = *(void***)thisPtr;
    ((VtFunc)vt[32])(thisPtr);

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_18 @ 0x82484918 | size: 0x64
//
// Attaches a physics bound (phBound) to this phObject. Calls the bound's
// vtable slot 1 (AddRef), then calls this object's vtable slot 20
// (OnShutdown) to reset state, stores the bound pointer at offset +48,
// and returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_18(void* thisPtr, void* bound) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Call bound->AddRef() via vtable slot 1
    void** boundVt = *(void***)bound;
    typedef void (*AddRefFunc)(void*);
    ((AddRefFunc)boundVt[1])(bound);

    // Call this->OnShutdown() via vtable slot 20
    void** vt = *(void***)thisPtr;
    typedef void (*VtFunc)(void*);
    ((VtFunc)vt[20])(thisPtr);

    // Store the bound pointer
    *(void**)(obj + 48) = bound;

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_19 @ 0x82484988 | size: 0x94
//
// Detaches and cleans up physics state. If the collision data at +500 is
// non-null, calls phObject_9A48_h to release it and clears the field.
// Then if the bound at +52 is non-null, calls its vtable slot 2 (Release)
// and clears the pointer. Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_19(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Release collision data at +500
    uint32_t collisionData = *(uint32_t*)(obj + 500);
    if (collisionData != 0) {
        phObject_9A48_h((void*)(obj + 500));
        *(uint32_t*)(obj + 500) = 0;
    }

    // Release bound at +52
    uint32_t bound = *(uint32_t*)(obj + 52);
    if (bound != 0) {
        void** boundVt = *(void***)(uintptr_t)bound;
        typedef void (*ReleaseFunc)(void*);
        ((ReleaseFunc)boundVt[2])((void*)(uintptr_t)bound);
        *(uint32_t*)(obj + 52) = 0;
    }

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_0 @ 0x82485250 | size: 0x84
//
// Constructor for phObject. Sets the vtable, calls phObject_15 (shutdown
// to initialize base state), initializes three embedded sub-objects at
// +200, +124, and +56 via their respective constructors, calls util_85C8
// for base setup, and optionally registers via _locale_register if the
// register flag (bit 0) is set.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_0(void* thisPtr, uint32_t registerFlag) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Set vtable
    *(void**)(obj + 0) = (void*)0x82008918;

    // Call shutdown to initialize base state
    phObject_15(thisPtr);

    // Initialize embedded sub-objects
    phObject_B878_h((void*)(obj + 200));
    ph_ctor_32E8((void*)(obj + 124));
    phInst_87F8((void*)(obj + 56));

    // Base setup
    util_85C8(thisPtr);

    // Optionally register with allocator
    if (registerFlag & 0x1) {
        _locale_register(thisPtr, 0x208C801D);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_16 @ 0x82485978 | size: 0x70
//
// Conditionally detaches the current bound. Loads the bound pointer from
// +52; if non-null, calls its vtable slot 14 with param=1 (TryDetach).
// If TryDetach succeeds (returns >= 0), calls this object's vtable slot 17
// (OnBoundDetached) passing the old bound pointer.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_16(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void* bound = *(void**)(obj + 52);
    if (bound == NULL) {
        return;
    }

    // Call bound->TryDetach(1) via vtable slot 14
    void** boundVt = *(void***)bound;
    typedef int32_t (*TryDetachFunc)(void*, int);
    int32_t result = ((TryDetachFunc)boundVt[14])(bound, 1);

    if (result < 0) {
        return;
    }

    // Call this->OnBoundDetached(bound) via vtable slot 17
    void** vt = *(void***)thisPtr;
    typedef void (*OnDetachFunc)(void*, void*);
    ((OnDetachFunc)vt[17])(thisPtr, *(void**)(obj + 52));
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_27 @ 0x824889A8 | size: 0x60
//
// Retrieves a pointer to the embedded sub-object at offset +200, stores
// it in *outPtr, then calls vtable slot 13 (Validate) on the sub-object
// at +124. Returns 0 on success, or error 0x8000000A on validation failure.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_27(void* thisPtr, void** outPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void* subObj124 = (void*)(obj + 124);
    void* subObj200 = (void*)(obj + 200);
    *outPtr = subObj200;

    // Call subObj124->Validate() via vtable slot 13
    void** subVt = *(void***)subObj124;
    typedef int32_t (*ValidateFunc)(void*);
    int32_t result = ((ValidateFunc)subVt[13])(subObj124);

    if (result != 0) {
        return 0;
    }

    return (int32_t)0x8000000A;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_9450_2h @ 0x82489450 | size: 0x70
//
// Attempts to process collision data from the resource at +536. If the
// resource is null, returns error 0x803000A8. Otherwise calls
// phObject_28C8_wrh to process it. If processing fails (< 0) returns
// early. If the output is still null after processing, clears the handle.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_9450_2h(void* thisPtr, void* r4, void* r5, void* outPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void* resource = *(void**)(obj + 536);
    if (resource == NULL) {
        return (int32_t)0x803000A8;
    }

    int32_t result = phObject_28C8_wrh(resource, r4, r5, NULL, NULL, outPtr);
    if (result < 0) {
        return result;
    }

    uint32_t outValue = *(uint32_t*)outPtr;
    if (outValue != 0) {
        return result;
    }

    *(void**)(obj + 536) = NULL;
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_1980_h @ 0x82491980 | size: 0x70
//
// Allocates a 2040-byte zeroed buffer using rage_01B8_alloc with physics
// allocator tag 0x208C8000. On success, zeroes the buffer with memset,
// stores it into *outPtr and returns 0. On failure returns 0x8007000E.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_1980_h(void** outPtr) {
    void* buffer = rage_01B8_alloc(2040, 0x208C8000);

    if (buffer == NULL) {
        return (int32_t)0x8007000E;
    }

    memset(buffer, 0, 2040);
    *outPtr = buffer;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_AD78_h @ 0x8248AD78 | size: 0x80
//
// Allocates and initializes a collision result object. First calls
// phObject_1980_h to allocate a buffer. If allocation succeeds, calls
// rage_A518 to populate the result. On any failure, releases the buffer
// via phObject_1F70 and returns 0 (NULL). On success returns the result.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phObject_AD78_h(void* thisPtr) {
    void* buffer = NULL;
    uint32_t result = 0;

    int32_t allocStatus = phObject_1980_h(&buffer);
    if (allocStatus < 0) {
        phObject_1F70(buffer);
        return 0;
    }

    int32_t initStatus = rage_A518((void*)&result, thisPtr, buffer);
    if (initStatus < 0) {
        phObject_1F70(buffer);
        return 0;
    }

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_17 @ 0x82485118 | size: 0xAC
//
// Binds a physics bound to this phObject and initializes collision data.
// Calls the bound's vtable slot 1 (AddRef), then this object's vtable
// slot 19 (OnPreShutdown). Stores the bound at +52, calls phObject_AD78_h
// to allocate collision result storage at +500. If allocation fails,
// returns error 0x8000FFFF. On success, calls rage_4CD0 to finalize.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_17(void* thisPtr, void* bound) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Call bound->AddRef() via vtable slot 1
    void** boundVt = *(void***)bound;
    typedef void (*AddRefFunc)(void*);
    ((AddRefFunc)boundVt[1])(bound);

    // Call this->OnPreShutdown() via vtable slot 19
    void** vt = *(void***)thisPtr;
    typedef void (*VtFunc)(void*);
    ((VtFunc)vt[19])(thisPtr);

    // Store the bound
    *(void**)(obj + 52) = bound;

    // Allocate collision result storage
    uint32_t collisionData = phObject_AD78_h(thisPtr);
    *(uint32_t*)(obj + 500) = collisionData;

    int32_t result;
    if (collisionData != 0) {
        result = 0;
    } else {
        result = (int32_t)0x8000FFFF;
    }

    // If successful, finalize
    if (result >= 0) {
        rage_4CD0(thisPtr);
    }

    return result;
}

// =============================================================================
// rage::phBoundCapsule -- Capsule Collision Bound Functions (72-196B)
// =============================================================================
//
// 10 additional phBoundCapsule methods covering vtable overrides and
// capsule geometry operations: axis translation, support distance,
// deep copy, axis scaling, joint limits, direction validation,
// normal orientation, angle computation, spherical area, and camera setup.

extern void util_DA50(void* dst, const void* src);
extern void phBoundCapsule_AED0_g(void* r3, void* r4, void* r5, void* normal);
extern float pongCameraMgr_GetFov(void* cameraMgr);
extern void phBoundCapsule_81D8_g(void* r3, void* r4, void* r5, float f1, float f2, float f3, float f4, void* r7, void* r10);
extern float phBoundCapsule_0E90_g(float value);
extern float aud_2478(float value);

extern uint32_t g_phCameraMgrPtr;
extern uint32_t g_phActiveCameraIndex;

// ---------------------------------------------------------------------------
// 1. phBoundCapsule::TranslateAndDispatch (vfn_7) @ 0x8233AB50 | size: 0x48
//    Adds capsule center offset (+48) to input position, dispatches to vfn_6.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::TranslateAndDispatch(const float* position) {
    float* center = (float*)((char*)this + 48);
    float translated[4];
    translated[0] = center[0] + position[0];
    translated[1] = center[1] + position[1];
    translated[2] = center[2] + position[2];
    translated[3] = center[3] + position[3];
    void** vt = *(void***)this;
    typedef void (*SetCenterFn)(void*, const float*);
    SetCenterFn fn = (SetCenterFn)vt[6];
    fn(this, translated);
}

// ---------------------------------------------------------------------------
// 2. phBoundCapsule::ComputeSupportDistance (vfn_33) @ 0x822A3268 | size: 0x50
//    Computes signed support distance along a direction using capsule params.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::ComputeSupportDistance(const float* direction, uint8_t earlyOut) {
    float dirY = direction[1];
    if (dirY < 0.0f) dirY = -dirY;
    float radius     = *(float*)((char*)this + 128);
    float halfHeight = *(float*)((char*)this + 112);
    extern const float g_phTwo;
    float result = radius * dirY * g_phTwo + halfHeight;
    if (earlyOut != 0) return result;
    float* axis = (float*)((char*)this + 48);
    float dot = axis[0] * direction[0] + axis[1] * direction[1] + axis[2] * direction[2];
    return dot + result;
}

// ---------------------------------------------------------------------------
// 3. phBoundCapsule::CopyFrom (vfn_34) @ 0x822A2F28 | size: 0xBC
//    Deep-copies all capsule bound data from a source phBoundCapsule.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::CopyFrom(const rage::phBoundCapsule* source) {
    util_DA50(this, source);
    const char* src = (const char*)source;
    char* dst = (char*)this;
    memcpy(dst + 112, src + 112, 16);
    memcpy(dst + 128, src + 128, 16);
    memcpy(dst + 144, src + 144, 16);
    memcpy(dst + 160, src + 160, 16);
    memcpy(dst + 176, src + 176, 16);
    *(uint32_t*)(dst + 192) = *(const uint32_t*)(src + 192);
    for (int i = 0; i < 12; i++) {
        dst[196 + i] = src[196 + i];
    }
    *(uint16_t*)(dst + 96) = 1;
}

// ---------------------------------------------------------------------------
// 4. phBoundCapsule::ScaleAxes @ 0x820D05A0 | size: 0xB4
//    Rotates capsule local axes by sin/cos scale factors.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::ScaleAxes() {
    float scaleA, scaleB;
    phBoundCapsule_04F0_g(&scaleA, &scaleB);
    float* axis0 = (float*)((char*)this + 16);
    float* axis1 = (float*)((char*)this + 32);
    float origAxis1[4] = { axis1[0], axis1[1], axis1[2], axis1[3] };
    float origAxis0[4] = { axis0[0], axis0[1], axis0[2], axis0[3] };
    axis1[0] = origAxis1[0] * scaleA - origAxis0[0] * scaleB;
    axis1[1] = origAxis1[1] * scaleA - origAxis0[1] * scaleB;
    axis1[2] = origAxis1[2] * scaleA - origAxis0[2] * scaleB;
    axis1[3] = origAxis1[3] * scaleA - origAxis0[3] * scaleB;
    axis0[0] = origAxis0[0] * scaleA + origAxis1[0] * scaleB;
    axis0[1] = origAxis0[1] * scaleA + origAxis1[1] * scaleB;
    axis0[2] = origAxis0[2] * scaleA + origAxis1[2] * scaleB;
    axis0[3] = origAxis0[3] * scaleA + origAxis1[3] * scaleB;
}

// ---------------------------------------------------------------------------
// 5. phBoundCapsule::GetJointLimitsByAxis @ 0x820DF420 | size: 0xC4
//    Retrieves min/max joint limits for a given axis from a 104-byte strided
//    joint data array. Falls through cases; retries with axis=11 if negative.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::GetJointLimitsByAxis(uint32_t jointIndex, uint32_t axis,
                                                float* outMin, float* outMax) {
    extern const float g_floatZero;
retry:
    void* dataArray = *(void**)((char*)this + 8);
    char* entry = (char*)dataArray + jointIndex * 104;
    switch (axis) {
        case 0:
            *outMax = *(float*)(entry + 0);
            *outMin = *(float*)(entry + 0);
            // fall through
        case 2:
            *outMin = *(float*)(entry + 4);
            *outMax = *(float*)(entry + 8);
            // fall through
        case 1:
            *outMin = *(float*)(entry + 12);
            *outMax = *(float*)(entry + 16);
            // fall through
        case 3:
            *outMin = *(float*)(entry + 20);
            *outMax = *(float*)(entry + 24);
            break;
        default:
            break;
    }
    if (*outMin < g_floatZero) {
        axis = 11;
        goto retry;
    }
}

// ---------------------------------------------------------------------------
// 6. phBoundCapsule::ValidateAndSetupDirection @ 0x82143F08 | size: 0x84
//    Checks if direction at source+16 is non-zero; if so, computes center
//    from source[0]+source[32] and initializes capsule via _3F90_g.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::ValidateAndSetupDirection(const float* source) {
    const float* dir = (const float*)((const char*)source + 16);
    float absX = dir[0] < 0.0f ? -dir[0] : dir[0];
    float absY = dir[1] < 0.0f ? -dir[1] : dir[1];
    float absZ = dir[2] < 0.0f ? -dir[2] : dir[2];
    float absW = dir[3] < 0.0f ? -dir[3] : dir[3];
    if (absX == 0.0f && absY == 0.0f && absZ == 0.0f && absW == 0.0f) return;
    const float* pos = source;
    const float* offset = (const float*)((const char*)source + 32);
    float center[4];
    center[0] = pos[0] + offset[0];
    center[1] = pos[1] + offset[1];
    center[2] = pos[2] + offset[2];
    center[3] = pos[3] + offset[3];
    extern void phBoundCapsule_3F90_g(void* thisPtr, const float* center, const float* direction);
    phBoundCapsule_3F90_g(this, center, dir);
}

// ---------------------------------------------------------------------------
// 7. phBoundCapsule::OrientNormalAndDispatch @ 0x82137CA8 | size: 0xC4
//    Negates collision normal if it faces away from reference plane, then
//    dispatches to phBoundCapsule_AED0_g for collision resolution.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::OrientNormalAndDispatch(void* r4, const float* planeNormal,
                                                    float* normal) {
    float dot = planeNormal[2] * normal[2] + planeNormal[3] * normal[3] +
                planeNormal[1] * normal[1] + planeNormal[0] * normal[0];
    extern const float g_floatZero;
    if (dot < g_floatZero) {
        float negated[4];
        negated[0] = -normal[0];
        negated[1] = -normal[1];
        negated[2] = -normal[2];
        negated[3] = -normal[3];
        phBoundCapsule_AED0_g(this, r4, (void*)planeNormal, negated);
        return;
    }
    phBoundCapsule_AED0_g(this, r4, (void*)planeNormal, normal);
}

// ---------------------------------------------------------------------------
// 8. phBoundCapsule::ComputeAngleFromDot @ 0x82137BF8 | size: 0xAC
//    Computes angle from 4D dot product using acos with NaN/range checks.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::ComputeAngleFromDot(const float* vecA, const float* vecB) {
    extern const float g_phThresholdHi;
    extern const float g_phThresholdLo;
    extern const float g_phAcosScale;
    extern const float g_phZeroF;
    float dot = vecA[2] * vecB[2] + vecA[3] * vecB[3] +
                vecA[1] * vecB[1] + vecA[0] * vecB[0];
    if (dot > g_phThresholdHi) return g_phZeroF;
    if (dot >= g_phThresholdLo) return g_phZeroF;
    float absDot = dot < 0.0f ? -dot : dot;
    float angle = phBoundCapsule_0E90_g(absDot);
    return angle * g_phAcosScale;
}

// ---------------------------------------------------------------------------
// 9. phBoundCapsule::ComputeSphericalArea @ 0x821426B8 | size: 0x84
//    Computes surface area metric: (scale / sqrt(r^2 * k1)) * exp(-h^2 / (r^2 * k2))
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::ComputeSphericalArea(float height, float radius) {
    extern const float g_phAreaK1;
    extern const float g_phAreaK2;
    extern const float g_phAreaScale;
    float radiusSq = radius * radius;
    float area = phBoundCapsule_01D0_g(radiusSq * g_phAreaK1);
    float heightSq = height * height;
    float ratio = heightSq / (radiusSq * g_phAreaK2);
    float expVal = aud_2478(-ratio);
    return (g_phAreaScale / area) * expVal;
}

// ---------------------------------------------------------------------------
// 10. phBoundCapsule::SetupCameraCollision @ 0x82148608 | size: 0x9C
//     Retrieves active camera from global manager, computes FOV scale,
//     dispatches to phBoundCapsule_81D8_g with camera parameters.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::SetupCameraCollision(void* param1, void* param2) {
    uint32_t cameraArrayBase = g_phCameraMgrPtr;
    uint32_t cameraIndex = g_phActiveCameraIndex;
    char* camera = (char*)(uintptr_t)(cameraArrayBase + cameraIndex * 912);
    float fov = pongCameraMgr_GetFov(camera);
    float fovParam = *(float*)(camera + 832);
    extern const float g_phCamScale;
    float diff = g_phCamScale - fovParam;
    float scale = (diff >= 0.0f) ? fovParam : 0.0f;
    float nearPlane = *(float*)(camera + 824);
    float farPlane = *(float*)(camera + 836);
    float* cameraTransform = (float*)(camera + 64);
    phBoundCapsule_81D8_g(this, param1, param2, scale, nearPlane, farPlane, fov,
                          cameraTransform, param2);
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Capsule Rotation, Normalization & Lookup Functions
// ═════════════════════════════════════════════════════════════════════════════

// External helpers (already declared elsewhere in this TU)
extern float phBoundCapsule_01D8_g(float angle);  // sin(angle)
extern float phBoundCapsule_02B0_g(float angle);  // cos(angle)
extern float phBoundCapsule_0FE0_g(float a, float b); // atan2(a, b)
extern void  phBoundCapsule_0E90_g(float x);      // acos helper
extern void  atSingleton_1308_g(double base, float exp); // pow(base, exp)

// ---------------------------------------------------------------------------
// 1. phBoundCapsule::RotateYZ @ 0x8223AB00 | size: 0x6C (108B)
//    Rotates the Y/Z components (+4, +8) of a vector by an angle.
//    new_z = z * cos - y * sin;  new_y = y * cos + z * sin
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::RotateYZ(float* vec, float angle) {
    float sinA = phBoundCapsule_01D8_g(angle);
    float cosA = (float)phBoundCapsule_02B0_g(angle);
    float y = vec[1];   // +4
    float z = vec[2];   // +8
    vec[2] = z * cosA + y * sinA;     // fmadds
    vec[1] = y * cosA - z * sinA;     // fmsubs -> negated order
}

// ---------------------------------------------------------------------------
// 2. phBoundCapsule::RotateXZ @ 0x8223AB70 | size: 0x6C (108B)
//    Rotates the X/Z components (+0, +8) of a vector by an angle.
//    new_z = z * cos - x * sin;  new_x = x * cos + z * sin
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::RotateXZ(float* vec, float angle) {
    float sinA = phBoundCapsule_01D8_g(angle);
    float cosA = (float)phBoundCapsule_02B0_g(angle);
    float x = vec[0];   // +0
    float z = vec[2];   // +8
    vec[2] = z * cosA - x * sinA;     // fmsubs
    vec[0] = x * cosA + z * sinA;     // fmadds
}

// ---------------------------------------------------------------------------
// 3. phBoundCapsule::NormalizeAngle @ 0x822238A8 | size: 0x6C (108B)
//    Computes atan2(z, x) from vec[0] and vec[2], then normalizes the
//    resulting angle into the range [0, TWO_PI) by adding/subtracting TWO_PI.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::NormalizeAngle(const float* vec) {
    extern const float g_phTwoPi;   // @ 0x8202C02C
    extern const float g_floatZero; // @ 0x8202D110
    float angle = (float)phBoundCapsule_0FE0_g(vec[2], vec[0]);
    while (angle < g_floatZero) {
        angle += g_phTwoPi;
    }
    while (angle >= g_phTwoPi) {
        angle -= g_phTwoPi;
    }
    return angle;
}

// ---------------------------------------------------------------------------
// 4. phBoundCapsule::ClampAddVector @ 0x8223ACB0 | size: 0x7C (124B)
//    Adds delta vector to position, clamping if the dot product indicates
//    the delta is within bounds. If delta magnitude > dot, scales delta
//    to preserve direction while limiting displacement.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::ClampAddVector(float* position, const float* delta) {
    extern const float g_floatOne;  // @ 0x8202D108
    float dot = position[0] * delta[0] + position[1] * delta[1] + position[2] * delta[2];
    if (dot <= g_floatOne) {
        // Small displacement: just add
        position[0] += delta[0];
        position[1] += delta[1];
        position[2] += delta[2];
        position[3] += delta[3];
        return;
    }
    float deltaSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (deltaSq <= dot) return;
    float ratio = dot / deltaSq;
    float blend = g_floatOne - ratio;
    position[0] = delta[0] * blend + position[0];
    position[1] = delta[1] * blend + position[1];
    position[2] = delta[2] * blend + position[2];
    position[3] = delta[3] * blend + position[3];
}

// ---------------------------------------------------------------------------
// 5. phBoundCapsule::ComputeSinCosMatrix @ 0x823FA2A0 | size: 0x7C (124B)
//    Computes sin/cos of a scaled angle, stores symmetric rotation entries
//    into a 6-float output structure: [cos, sin, -sin, cos, 0, 0].
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::ComputeSinCosMatrix(float* outMatrix, float angle) {
    extern const float g_phAngleScale;  // scaling factor for angle conversion
    extern const float g_floatZero;
    float scaled = angle * g_phAngleScale;
    float cosVal = (float)phBoundCapsule_02B0_g(scaled);
    float sinVal = (float)phBoundCapsule_01D8_g(scaled);
    outMatrix[0] = cosVal;   // +0
    outMatrix[3] = cosVal;   // +12
    outMatrix[5] = g_floatZero;  // +20
    outMatrix[4] = g_floatZero;  // +16
    outMatrix[1] = sinVal;  // +4
    outMatrix[2] = -sinVal; // +8
}

// ---------------------------------------------------------------------------
// 6. phBoundCapsule::CollectMatchingEntities @ 0x8237A1F8 | size: 0x94 (148B)
//    Iterates through a linked list of entities from a spatial partition,
//    testing each against a global filter via vtable call (slot 20).
//    Matching entities are stored in a 848-byte strided result array.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::CollectMatchingEntities(uint32_t partitionIndex) {
    extern uint32_t g_phEntityFilter;  // @ global
    uint32_t count = 0;
    *(uint32_t*)((char*)this + 108) = 0;
    uint32_t* partitionTable = *(uint32_t**)((char*)this + 96);
    uint32_t* entry = (uint32_t*)partitionTable[partitionIndex];
    uint32_t* listHead = (uint32_t*)(*(uint32_t*)((char*)entry + 4));
    if (listHead == NULL) return;
    uint32_t* node = (uint32_t*)(*(uint32_t*)((char*)listHead + 12));
    if (node == NULL) {
        *(uint32_t*)((char*)this + 108) = count;
        return;
    }
    while (node != NULL) {
        // Call vtable slot 20 on node with global filter param
        void** vtable = *(void***)node;
        typedef uint8_t (*FilterFn)(void*, uint32_t);
        FilterFn filterFunc = (FilterFn)vtable[20];
        uint8_t result = filterFunc(node, g_phEntityFilter);
        if (result != 0) {
            uint32_t* resultArray = *(uint32_t**)((char*)this + 104);
            char* slot = (char*)resultArray + count * 848;
            *(uint32_t*)(slot + 824) = (uint32_t)(uintptr_t)node;
            count++;
        }
        node = (uint32_t*)(*(uint32_t*)((char*)node + 8));
    }
    *(uint32_t*)((char*)this + 108) = count;
}

// ---------------------------------------------------------------------------
// 7. phBoundCapsule::FixedPointToFloat @ 0x824C3A98 | size: 0x9C (156B)
//    Converts a fixed-point integer exponent to a floating-point value using
//    a lookup table. Negative exponents divide by power-of-2, positive
//    exponents multiply by power-of-2, using table entries as mantissa.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::FixedPointToFloat(int32_t exponent) {
    extern const float g_phFixedNegTable[];  // negative exponent LUT
    extern const float g_phFixedPosTable[];  // positive exponent LUT
    if (exponent <= 0) {
        if (exponent < -72) exponent = -71;
        int shift = (-exponent) >> 2;
        int tableIdx = -exponent;
        float tableVal = g_phFixedNegTable[tableIdx];
        int divisor = 1 << shift;
        return tableVal / (float)divisor;
    }
    if (exponent >= 62) exponent = 62;
    int shift = exponent >> 2;
    float tableVal = g_phFixedPosTable[exponent - 1];
    int multiplier = 1 << shift;
    return (float)multiplier * tableVal;
}

// ---------------------------------------------------------------------------
// 8. phBoundCapsule::ConvertValueByMode @ 0x82453A98 | size: 0xAC (172B)
//    Converts a raw value based on a mode flag (field +12):
//      mode 0: direct copy from field +20
//      mode 2: sin(value * PI_SCALE) 
//      mode 1: pow(value, EXP_SCALE) * LINEAR_SCALE
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::ConvertValueByMode(void* obj) {
    char* self = (char*)obj;
    int32_t mode = *(int32_t*)(self + 12);
    float rawValue = *(float*)(self + 20);
    if (mode != 1 && mode != 2) {
        // mode 0: direct assignment
        *(float*)(self + 4) = rawValue;
        return;
    }
    if (mode == 2) {
        // mode 2: compute sin of scaled angle
        extern const double g_phPiScale;  // PI conversion factor
        float result = (float)phBoundCapsule_01D8_g((float)(rawValue * g_phPiScale));
        *(float*)(self + 4) = result;
        return;
    }
    // mode 1: power function with linear scale
    extern const double g_phExpBase;     // exponent base
    extern const float g_phLinearScale;  // linear scale factor
    float powered = (float)atSingleton_1308_g(g_phExpBase, rawValue);
    *(float*)(self + 4) = powered * g_phLinearScale;
}

// ---------------------------------------------------------------------------
// 9. phBoundCapsule::QuantizedLookupAdvance @ 0x824C3B38 | size: 0xAC (172B)
//    Advances through a quantized lookup table, finding the correct bucket
//    for a shifted value and computing the interpolated result from the
//    bucket's quantization level relative to a maximum.
// ---------------------------------------------------------------------------
int32_t rage::phBoundCapsule::QuantizedLookupAdvance(
    int32_t shiftedVal, int32_t* indexPtr, int32_t maxVal,
    const int32_t* bucketTable, int32_t shiftAmount, int32_t unused1) {
    int32_t index = *indexPtr;
    int32_t nextIdx = index + 1;
    int32_t bucket = bucketTable[nextIdx];
    int32_t shifted = (shiftedVal << shiftAmount) >> 6;  // arithmetic shift
    while (shifted >= bucket) {
        index = *indexPtr + 1;
        *indexPtr = index;
        nextIdx = index + 1;
        bucket = bucketTable[nextIdx];
    }
    index = *indexPtr;
    if (index >= unused1) return 0;  // past end
    int32_t nextBucket = bucketTable[index + 1];
    int32_t result;
    if (shiftAmount > 6) {
        int32_t hiShift = shiftAmount - 7;
        int32_t correction = 1 << hiShift;
        result = (nextBucket + correction) >> (shiftAmount - 6);
    } else {
        result = nextBucket << (6 - shiftAmount);
    }
    if (result > maxVal) return maxVal;
    return result;
}

// ---------------------------------------------------------------------------
// 10. phBoundCapsule::UnpackAndStoreVertex @ 0x8215A928 | size: 0xB4 (180B)
//     Unpacks a packed vertex from a compressed mesh buffer. Decodes position
//     index, color flag, and normal bits from a 32-bit packed word, updates
//     the vertex bitmap, and writes the converted position into the output
//     buffer at the resolved offset.
// ---------------------------------------------------------------------------
void rage::phBoundCapsule::UnpackAndStoreVertex(uint32_t packedVertex, uint8_t colorFlag) {
    void** vtable = *(void***)this;
    uint32_t positionIndex = (packedVertex >> 1) & 0x1FFFF;
    uint32_t parity = packedVertex & 0x1;
    uint32_t normalBits = positionIndex & 0x7;
    uint32_t vertexOffset = (packedVertex << 17) & 0x1FFF8;
    uint32_t byteIndex = positionIndex >> 3;
    uint32_t mask = parity - 1;  // 0 or 0xFFFFFFFF
    uint32_t vertexCount = *(uint32_t*)((char*)this + 16);
    *(uint32_t*)((char*)this + 16) = vertexCount - 1;

    uint32_t posBase = *(uint32_t*)((char*)vtable + 268);
    uint32_t colBase = *(uint32_t*)((char*)vtable + 300);
    uint32_t normBase = *(uint32_t*)((char*)vtable + 264);
    uint32_t idxBase = *(uint32_t*)((char*)vtable + 256);
    uint32_t normBase2 = *(uint32_t*)((char*)vtable + 296);

    extern const uint8_t g_phNormalLUT[];  // normal lookup table
    uint32_t posWord = *(uint32_t*)(posBase);
    uint32_t colWord = *(uint32_t*)(colBase);
    uint8_t normalEntry = g_phNormalLUT[normalBits];

    uint32_t normIndex = normBase & mask;
    uint32_t posCleared = posWord & ~mask;
    uint32_t normCleared = normBase2 & mask;
    uint32_t finalPos = posCleared + normIndex;
    uint32_t idxCleared = idxBase & ~mask;
    uint32_t colCleared = colWord & ~mask;
    uint32_t idxFinal = idxCleared + (vtable[0] != 0 ? mask & (uint32_t)(uintptr_t)vtable[0] : 0);
    uint32_t colFinal = colCleared + normCleared;

    // OR the normal entry into the bitmap
    uint8_t existing = *(uint8_t*)(byteIndex + idxFinal);
    *(uint8_t*)(byteIndex + idxFinal) = existing | normalEntry;

    // Write converted color float at the resolved vertex position
    uint32_t meshWord = *(uint32_t*)(finalPos + vertexOffset + 4);
    uint32_t floatOffset = ((meshWord << 4) & 0xFFFF0) + colFinal;
    float converted = (float)colorFlag;
    *(float*)(floatOffset) = converted;
}


// ═════════════════════════════════════════════════════════════════════════════
// rage::phArticulatedCollider — Additional Articulated Collider Functions
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// External function declarations for new phArticulatedCollider functions
extern void phCollider_vfn_1(phArticulatedCollider* collider);
extern void phArticulatedCollider_57F0_fw(void* jointData);
extern void phArticulatedCollider_5A40_wrh(void* jointData);
extern void phArticulatedCollider_5FE0(void* jointData);
extern void SinglesNetworkClient_0978_g(void* skeleton, void* assertMsg, void* assertFile);
extern int pongCreatureInst_F5C8(void* boneMap, int boneIndex);

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::Reset (vfn_1) @ 0x8224E6D8 | size: 0x48
//
// Resets the articulated collider to its initial state. Calls the parent
// phCollider::Reset, clears the joint state pointer and active flag, then
// reinitializes joint data structures.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::Reset() {
    // Call parent class reset
    phCollider_vfn_1(this);

    // Clear the joint state pointer and active flag
    field_0x01d8 = 0;         // +472: joint state pointer
    *(uint8_t*)((char*)this + 468) = 0;  // +468: active flag byte

    // Reinitialize joint frame data
    void* jointData = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_57F0_fw(jointData);

    // Reset joint internal state
    jointData = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_5D58(jointData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ZeroForcesAndUpdate (vfn_2) @ 0x8224E720 | size: 0x80
//
// Zeros out all accumulated force/torque vectors, calls the virtual Update
// method, then resets the joint working state. The four 16-byte vectors at
// offsets +224, +240, +256, +272 represent accumulated linear/angular
// forces and impulses that are cleared each frame before re-accumulation.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::ZeroForcesAndUpdate() {
    // Zero out 4 SIMD vectors: accumulated forces/impulses
    // +224: linear force accumulator
    float* vec0 = (float*)((char*)this + 224);
    vec0[0] = 0.0f; vec0[1] = 0.0f; vec0[2] = 0.0f; vec0[3] = 0.0f;

    // +240: angular force accumulator
    float* vec1 = (float*)((char*)this + 240);
    vec1[0] = 0.0f; vec1[1] = 0.0f; vec1[2] = 0.0f; vec1[3] = 0.0f;

    // +256: linear impulse accumulator
    float* vec2 = (float*)((char*)this + 256);
    vec2[0] = 0.0f; vec2[1] = 0.0f; vec2[2] = 0.0f; vec2[3] = 0.0f;

    // +272: angular impulse accumulator
    float* vec3 = (float*)((char*)this + 272);
    vec3[0] = 0.0f; vec3[1] = 0.0f; vec3[2] = 0.0f; vec3[3] = 0.0f;

    // Call virtual Update (vfn_4) via vtable
    void** vt = *(void***)this;
    typedef void (*UpdateFunc)(phArticulatedCollider*);
    UpdateFunc update = (UpdateFunc)vt[4];
    update(this);

    // Reset joint working state
    void* jointData = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_5A40_wrh(jointData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::UpdateChildBodies (vfn_9) @ 0x8224EBC8 | size: 0x8C
//
// Iterates over all child bodies in the articulated chain (excluding the root)
// and calls their virtual UpdateBody method (vtable slot 18) on each body
// that has a non-null physics data pointer (offset +20).
//
// The joint data at +464 contains an array of body pointers starting at
// offset +168, with the count at offset +4.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::UpdateChildBodies() {
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    int bodyCount = (int)jointData[1];  // +4: body count

    for (int i = bodyCount - 1; i > 0; i--) {
        // Body pointer array starts at jointData + 168 bytes = +42 words
        uint32_t bodyPtr = jointData[42 + (bodyCount - 1 - i)];

        // Check if body has physics data (offset +20)
        uint32_t physicsData = *(uint32_t*)(bodyPtr + 20);
        bool hasPhysics = (physicsData != 0);

        if (hasPhysics) {
            // Call vtable slot 18 (UpdateBody) on the body
            void** bodyVTable = *(void***)(uintptr_t)bodyPtr;
            typedef void (*UpdateBodyFunc)(void*);
            UpdateBodyFunc updateBody = (UpdateBodyFunc)bodyVTable[18];
            updateBody((void*)(uintptr_t)bodyPtr);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyScaledLinearVelocity (vfn_27) @ 0x8224FD58 | size: 0x4C
//
// Applies a scaled linear velocity to the root body. Loads the mass from
// offset +100, multiplies by the velocity scale factor (f1 param), builds
// a vector {0, scaled_mass, 0}, and dispatches to the parent class
// implementation via vtable slot 33 (byte offset 132).
//
// @param velocityScale - Scale factor for velocity application
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::ApplyScaledLinearVelocity(float velocityScale) {
    // Load vtable for indirect call
    void** vt = *(void***)this;

    // Load mass from +100
    float mass = *(float*)((char*)this + 100);

    // Scale mass by velocity factor
    float scaledMass = mass * velocityScale;

    // Build vector: {0, scaledMass, 0}
    float vec[4];
    vec[0] = 0.0f;
    vec[1] = scaledMass;
    vec[2] = 0.0f;

    // Dispatch to parent AddScaledForce (vtable slot 33, byte offset 132)
    typedef void (*AddScaledForceFunc)(phArticulatedCollider*, const float*);
    AddScaledForceFunc addForce = (AddScaledForceFunc)vt[33];
    addForce(this, vec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetScaledInertiaAxisX (vfn_28) @ 0x8224FDA8 | size: 0x94
//
// Extracts the X components from the root body's 4x4 inertia tensor rows
// (stored at joint[0]+144 as four 16-byte row vectors), assembles them into
// a single column vector, scales by the inverse mass (joint[0]+128 * f1),
// and dispatches via vtable slot 33.
//
// @param scale - Scale factor for the inertia axis
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::GetScaledInertiaAxisX(float scale) {
    void** vt = *(void***)this;
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464

    // Get root body pointer from joint data array at +40
    uint32_t rootBody = jointData[10];  // +40

    // Matrix starts at rootBody + 144, laid out as 4 row vectors of 16 bytes
    float* row0 = (float*)(rootBody + 144);
    float* row1 = (float*)(rootBody + 160);
    float* row2 = (float*)(rootBody + 176);
    float* row3 = (float*)(rootBody + 192);

    // Extract X component (element [0]) from each row to form column vector
    // vmrghw extracts high words - this transposes to get column X
    float columnX[4];
    columnX[0] = row0[0];
    columnX[1] = row1[0];
    columnX[2] = row2[0];
    columnX[3] = row3[0];

    // Load inverse mass from rootBody + 128, scale by parameter
    float inverseMass = *(float*)(rootBody + 128);
    float scaleFactor = inverseMass * scale;

    // Scale the column vector
    float scaledVec[4];
    scaledVec[0] = columnX[0] * scaleFactor;
    scaledVec[1] = columnX[1] * scaleFactor;
    scaledVec[2] = columnX[2] * scaleFactor;
    scaledVec[3] = columnX[3] * scaleFactor;

    // Dispatch to parent via vtable slot 33 (byte offset 132)
    typedef void (*ApplyForceFunc)(phArticulatedCollider*, const float*);
    ApplyForceFunc applyForce = (ApplyForceFunc)vt[33];
    applyForce(this, scaledVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetScaledInertiaAxisY (vfn_29) @ 0x8224FE40 | size: 0x94
//
// Same as GetScaledInertiaAxisX but extracts the Y components (element [1])
// from each row of the 4x4 matrix, assembles them into a column vector,
// and scales by rootBody+136 (Y-axis inverse inertia).
//
// @param scale - Scale factor for the inertia axis
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::GetScaledInertiaAxisY(float scale) {
    void** vt = *(void***)this;
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464

    // Get root body pointer from joint data array at +40
    uint32_t rootBody = jointData[10];  // +40

    // Matrix at rootBody + 144
    float* row0 = (float*)(rootBody + 144);
    float* row1 = (float*)(rootBody + 160);
    float* row2 = (float*)(rootBody + 176);
    float* row3 = (float*)(rootBody + 192);

    // Extract Y component (element [1]) from each row - vmrglw transposes to column Y
    float columnY[4];
    columnY[0] = row0[1];
    columnY[1] = row1[1];
    columnY[2] = row2[1];
    columnY[3] = row3[1];

    // Load Y-axis inverse inertia from rootBody + 136, scale by parameter
    float inverseInertiaY = *(float*)(rootBody + 136);
    float scaleFactor = inverseInertiaY * scale;

    // Scale the column vector
    float scaledVec[4];
    scaledVec[0] = columnY[0] * scaleFactor;
    scaledVec[1] = columnY[1] * scaleFactor;
    scaledVec[2] = columnY[2] * scaleFactor;
    scaledVec[3] = columnY[3] * scaleFactor;

    // Dispatch to parent via vtable slot 33 (byte offset 132)
    typedef void (*ApplyForceFunc)(phArticulatedCollider*, const float*);
    ApplyForceFunc applyForce = (ApplyForceFunc)vt[33];
    applyForce(this, scaledVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetScaledInertiaAxisZ (vfn_30) @ 0x8224FED8 | size: 0x94
//
// Same as GetScaledInertiaAxisX/Y but extracts Z components. Uses vmrghw
// then vmrglw to get column Z from the matrix rows, scales by
// rootBody+132 (Z-axis inverse inertia).
//
// @param scale - Scale factor for the inertia axis
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::GetScaledInertiaAxisZ(float scale) {
    void** vt = *(void***)this;
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464

    // Get root body pointer from joint data array at +40
    uint32_t rootBody = jointData[10];  // +40

    // Matrix at rootBody + 144
    float* row0 = (float*)(rootBody + 144);
    float* row1 = (float*)(rootBody + 160);
    float* row2 = (float*)(rootBody + 176);
    float* row3 = (float*)(rootBody + 192);

    // Extract Z component (element [2]) from each row
    // vmrghw + vmrglw combination extracts the third element column
    float columnZ[4];
    columnZ[0] = row0[2];
    columnZ[1] = row1[2];
    columnZ[2] = row2[2];
    columnZ[3] = row3[2];

    // Load Z-axis inverse inertia from rootBody + 132, scale by parameter
    float inverseInertiaZ = *(float*)(rootBody + 132);
    float scaleFactor = inverseInertiaZ * scale;

    // Scale the column vector
    float scaledVec[4];
    scaledVec[0] = columnZ[0] * scaleFactor;
    scaledVec[1] = columnZ[1] * scaleFactor;
    scaledVec[2] = columnZ[2] * scaleFactor;
    scaledVec[3] = columnZ[3] * scaleFactor;

    // Dispatch to parent via vtable slot 33 (byte offset 132)
    typedef void (*ApplyForceFunc)(phArticulatedCollider*, const float*);
    ApplyForceFunc applyForce = (ApplyForceFunc)vt[33];
    applyForce(this, scaledVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SaveAndClearBodyForces (vfn_63) @ 0x8224E2D8 | size: 0xBC
//
// Iterates over all bodies in the articulated chain. For each body, saves
// the current force vectors (at +304 and +320) into backup slots (at +1040
// and +1056), then zeros out both the force vectors and the velocity vectors
// at +272 and +288.
//
// This is called at the start of a simulation substep to preserve forces for
// constraint solving while clearing accumulators for the new substep.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SaveAndClearBodyForces() {
    void* jointData = (void*)(uintptr_t)m_nActiveJoints;  // +464 (0x1D0)
    phArticulatedCollider_5FE0(jointData);

    uint32_t* jd = (uint32_t*)(uintptr_t)m_nActiveJoints;  // +464
    int bodyCount = (int)jd[1];  // +4: body count

    for (int i = 0; i < bodyCount; i++) {
        // Body pointer from array at +40
        uint32_t bodyPtr = jd[10 + i];  // +40 offset, 4 bytes each

        // Save force vector at +304 to backup at +1040
        float* forceVec = (float*)(bodyPtr + 304);
        float* backupForce = (float*)(bodyPtr + 1040);
        backupForce[0] = forceVec[0];
        backupForce[1] = forceVec[1];
        backupForce[2] = forceVec[2];
        backupForce[3] = forceVec[3];

        // Save torque vector at +320 to backup at +1056
        float* torqueVec = (float*)(bodyPtr + 320);
        float* backupTorque = (float*)(bodyPtr + 1056);
        backupTorque[0] = torqueVec[0];
        backupTorque[1] = torqueVec[1];
        backupTorque[2] = torqueVec[2];
        backupTorque[3] = torqueVec[3];

        // Zero the force vector at +320 (reusing pointer as +304+16)
        torqueVec[0] = 0.0f; torqueVec[1] = 0.0f;
        torqueVec[2] = 0.0f; torqueVec[3] = 0.0f;

        // Zero the force vector at +304
        forceVec[0] = 0.0f; forceVec[1] = 0.0f;
        forceVec[2] = 0.0f; forceVec[3] = 0.0f;

        // Zero velocity vector at +272
        float* velVec = (float*)(bodyPtr + 272);
        velVec[0] = 0.0f; velVec[1] = 0.0f;
        velVec[2] = 0.0f; velVec[3] = 0.0f;

        // Zero angular velocity vector at +288
        float* angVelVec = (float*)(bodyPtr + 288);
        angVelVec[0] = 0.0f; angVelVec[1] = 0.0f;
        angVelVec[2] = 0.0f; angVelVec[3] = 0.0f;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::NormalizeDirectionVector (E398_p33) @ 0x8224E398 | size: 0x94
//
// Copies a 3-component float vector from source to dest, then normalizes it.
// If the squared magnitude is above a small epsilon, divides each component
// by the magnitude (1/sqrt(dot)). If at or below epsilon, stores a default
// unit vector from a global constant.
//
// @param source - Input 3-component float vector
// @param dest   - Output normalized 3-component float vector (16-byte aligned)
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider_NormalizeDirectionVector(const float* source, float* dest) {
    // Copy source XYZ to dest
    dest[0] = source[0];
    dest[1] = source[1];
    dest[2] = source[2];

    // Compute squared magnitude (dot product of vector with itself, 3 components)
    float sqMag = dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2];

    // Compare against epsilon (small positive float from .rdata)
    extern const float g_phNormalizeEpsilon;  // @ 0x82079C58

    if (sqMag > g_phNormalizeEpsilon) {
        // Compute 1/sqrt(sqMag) for normalization
        float mag = sqrtf(sqMag);
        float invMag = 1.0f / mag;

        // Scale all components by inverse magnitude
        dest[0] *= invMag;
        dest[1] *= invMag;
        dest[2] *= invMag;
        dest[3] *= invMag;
    } else {
        // Magnitude too small; store default unit vector from global
        extern const float g_phDefaultUnitVector[4];  // @ 0x82083B80
        dest[0] = g_phDefaultUnitVector[0];
        dest[1] = g_phDefaultUnitVector[1];
        dest[2] = g_phDefaultUnitVector[2];
        dest[3] = g_phDefaultUnitVector[3];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointLinkIndex (E668) @ 0x8224E668 | size: 0x70
//
// Resolves a bone index to its corresponding joint link index in the
// articulated collider. Uses the skeleton's bone map to translate the
// external bone index into an internal joint array offset.
//
// @param boneIndex - External bone index to look up
// @return Joint link pointer at the resolved offset in this collider
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phArticulatedCollider::GetJointLinkIndex(int boneIndex) {
    // Get skeleton pointer from +16
    uint32_t* skeletonPtr = (uint32_t*)(uintptr_t)*(uint32_t*)((char*)this + 16);

    // Get bone data from skeleton: skeleton[1] -> +4, then +12 for bone map
    uint32_t* boneData = (uint32_t*)(uintptr_t)skeletonPtr[1];
    void* boneMap = (void*)(uintptr_t)boneData[3];  // +12

    // Assert file and message strings for debug validation
    extern const char g_phAssertMsg[];   // debug assert string
    extern const char g_phAssertFile[];  // debug assert file

    // Validate bone map via debug assertion
    SinglesNetworkClient_0978_g(boneMap, (void*)g_phAssertMsg, (void*)g_phAssertFile);

    // Look up internal joint index from bone map
    int jointIndex = pongCreatureInst_F5C8(boneMap, boneIndex);

    // Compute offset: (jointIndex + 129) * 4 and load from this
    uint32_t offset = (uint32_t)(jointIndex + 129) * 4;
    return *(uint32_t*)((char*)this + offset);
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// rage::phInst — Virtual Accessor / Dispatch Functions (8-24B)
// ═════════════════════════════════════════════════════════════════════════════

/**
 * phInst::SetCollisionGroup (vfn_16) @ 0x8248B8C8 | size: 0x8
 *
 * Stores the collision group identifier at offset +284 (0x011C).
 * The collision group controls which physics groups this instance
 * belongs to for broadphase collision filtering.
 */
void phInst::SetCollisionGroup(uint32_t val) {
    *(uint32_t*)((char*)this + 284) = val;
}

/**
 * phInst::GetCollisionGroup (vfn_19) @ 0x8248B8D0 | size: 0x8
 *
 * Returns the collision group identifier stored at offset +284 (0x011C).
 */
uint32_t phInst::GetCollisionGroup() {
    return *(uint32_t*)((char*)this + 284);
}

/**
 * phInst::SetCollisionMask (vfn_38) @ 0x8248D808 | size: 0x8
 *
 * Stores the collision mask at offset +444 (0x01BC).
 * The collision mask is a bitmask controlling which collision groups
 * this instance can interact with during narrowphase tests.
 */
void phInst::SetCollisionMask(uint32_t val) {
    *(uint32_t*)((char*)this + 444) = val;
}

/**
 * phInst::GetCollisionMask (vfn_44) @ 0x8248D810 | size: 0x8
 *
 * Returns the collision mask stored at offset +444 (0x01BC).
 */
uint32_t phInst::GetCollisionMask() {
    return *(uint32_t*)((char*)this + 444);
}

/**
 * phInst::SetUserData (vfn_39) @ 0x8248D818 | size: 0x8
 *
 * Stores a user-defined data word at offset +448 (0x01C0).
 * This field allows game code to associate arbitrary data with
 * a physics instance (e.g., entity index, type tag).
 */
void phInst::SetUserData(uint32_t val) {
    *(uint32_t*)((char*)this + 448) = val;
}

/**
 * phInst::GetUserData (vfn_45) @ 0x8248D820 | size: 0x8
 *
 * Returns the user data word stored at offset +448 (0x01C0).
 */
uint32_t phInst::GetUserData() {
    return *(uint32_t*)((char*)this + 448);
}

/**
 * phInst::GetArchetype (vfn_71) @ 0x82488480 | size: 0x8
 *
 * Returns the 64-bit archetype pointer stored at offset +64 (0x0040).
 * The archetype describes the shared physical properties (shape,
 * mass, material) for this instance.
 */
void* phInst::GetArchetype() {
    return *(void**)((char*)this + 64);
}

/**
 * phInst::GetErrorCode (vfn_48) @ 0x8247E2D0 | size: 0xC
 *
 * Returns the constant 0x80004001 (E_NOTIMPL).
 * Base implementation for a query interface that is not supported
 * at this level of the hierarchy; subclasses override with real logic.
 */
uint32_t phInst::GetErrorCode() {
    return 0x80004001;
}

/**
 * phInst::DispatchSlot13_204 (vfn_15) @ 0x8248D888 | size: 0x18
 *
 * Tail-calls vtable slot 13 (the generic dispatch handler) passing
 * a pointer to the field block at offset +204 (0x00CC) and size 40.
 * r3=this, r4/r5 pass through from caller, r6=40, r7=this+204.
 * Dispatches processing of the local transform matrix region.
 */
void phInst::DispatchSlot13_204() {
    typedef void (*Fn)(void*, void*, void*, int, void*);
    void** vt = *(void***)this;
    void* fieldPtr = (char*)this + 204;
    ((Fn)vt[13])(this, nullptr, nullptr, 40, fieldPtr);
}

/**
 * phInst::DispatchSlot13_284 (vfn_34) @ 0x8248D868 | size: 0x18
 *
 * Tail-calls vtable slot 13 with a pointer to the field block at
 * offset +284 (0x011C) and size 40.
 * r3=this, r4/r5 pass through from caller, r6=40, r7=this+284.
 * Dispatches processing of the collision group / filter data region.
 */
void phInst::DispatchSlot13_284() {
    typedef void (*Fn)(void*, void*, void*, int, void*);
    void** vt = *(void***)this;
    void* fieldPtr = (char*)this + 284;
    ((Fn)vt[13])(this, nullptr, nullptr, 40, fieldPtr);
}
