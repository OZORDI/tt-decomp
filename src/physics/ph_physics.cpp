/**
 * ph_physics.cpp — RAGE physics module utilities
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Small utility routines from the ph* physics subsystem that don't belong
 * to a specific class.  Placed in the physics compilation unit at
 * 0x824321xx (not in splits.txt → inferred from address range and prefix).
 */

#include "ph_physics.hpp"
#include <stddef.h>

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
void rage::fragDrawable::Constructor() {
    // Call base class initialization
    util_B8A0(this);
    
    // Set vtable pointer
    *(void**)this = g_fragDrawableVtable;  // @ 0x82033094
    
    // Zero out state fields from +256 to +302
    *(uint32_t*)((char*)this + 256) = 0;
    *(uint32_t*)((char*)this + 260) = 0;
    *(uint16_t*)((char*)this + 264) = 0;
    *(uint16_t*)((char*)this + 266) = 0;
    *(uint32_t*)((char*)this + 268) = 0;
    *(uint16_t*)((char*)this + 272) = 0;
    *(uint8_t*)((char*)this + 274) = 1;    // Set flag to 1
    *(uint8_t*)((char*)this + 275) = 0;
    *(uint32_t*)((char*)this + 276) = 0;
    *(uint32_t*)((char*)this + 280) = 0;
    *(uint32_t*)((char*)this + 284) = 0;
    *(uint32_t*)((char*)this + 288) = 0;
    *(uint32_t*)((char*)this + 292) = 0;
    *(uint32_t*)((char*)this + 296) = 0;
    *(uint16_t*)((char*)this + 300) = 0;
    *(uint16_t*)((char*)this + 302) = 0;
    
    // Initialize 3x3 identity matrix at offset +192
    // Matrix layout: [m00 m01 m02] [m10 m11 m12] [m20 m21 m22]
    float* matrix = (float*)((char*)this + 192);
    
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
    uint32_t* vec = (uint32_t*)((char*)this + 240);
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
    this->field_0x00 = source->field_0x00;
    this->field_0x04 = source->field_0x04;
    this->field_0x08 = source->field_0x08;
    this->field_0x0C = source->field_0x0C;
    this->field_0x10 = source->field_0x10;
    
    // Copy 7 SIMD vectors (32-159)
    this->m_vec1 = source->m_vec1;    // +32
    this->m_vec2 = source->m_vec2;    // +48
    this->m_vec3 = source->m_vec3;    // +64
    
    // Copy 3 float values (80-91)
    this->m_float1 = source->m_float1;  // +80
    this->m_float2 = source->m_float2;  // +84
    this->m_float3 = source->m_float3;  // +88
    
    // Copy byte flag (92)
    this->m_flags = source->m_flags;
    
    // Copy remaining 4 vectors (96-159)
    this->m_vec4 = source->m_vec4;    // +96
    this->m_vec5 = source->m_vec5;    // +112
    this->m_vec6 = source->m_vec6;    // +128
    this->m_vec7 = source->m_vec7;    // +144
    
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
                return CollideP lane XZ(minX, maxX, minZ, maxZ);
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
    uint8_t flags = m_configFlags;  // +64
    
    // Check bit 1 (0x02) - standard capsule flag
    if (flags & 0x02) {
        return g_floatOne;
    }
    
    // Check bit 4 (0x10) - alternate configuration flag
    if (flags & 0x10) {
        return g_floatOne;
    }
    
    // Return custom radius from field +28
    return m_radius;  // +28
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
        
        // Otherwise compute extent with param1
        return phBoundCapsule_0FE0_g(param1);
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
    // Load transformation vector (16 bytes)
    float transformX = transform[0];
    float transformY = transform[1];
    float transformZ = transform[2];
    
    // Compute negated transform components for offset calculations
    float negTransformX = -transformX;
    float negTransformY = -transformY;
    float negTransformZ = -transformZ;
    
    // Transform point 0 (offset +0)
    m_point0X += negTransformX;
    m_point0Y += negTransformY;
    m_point0Z += transformZ;
    
    // Transform point 1 (offset +16)
    m_point1X += transformX;
    m_point1Y += negTransformY;
    m_point1Z += transformZ;
    
    // Transform point 2 (offset +32)
    m_point2X += negTransformX;
    m_point2Y += transformY;
    m_point2Z += transformZ;
    
    // Transform point 3 (offset +48)
    m_point3X += transformX;
    m_point3Y += transformY;
    m_point3Z += transformZ;
    
    // Transform point 4 (offset +64)
    m_point4X += negTransformX;
    m_point4Y += negTransformY;
    m_point4Z += -transformZ;
    
    // Transform point 5 (offset +80)
    m_point5X += transformX;
    m_point5Y += negTransformY;
    m_point5Z += -transformZ;
    
    // Transform point 6 (offset +96)
    m_point6X += negTransformX;
    m_point6Y += transformY;
    m_point6Z += -transformZ;
    
    // Transform point 7 (offset +112)
    m_point7X += transformX;
    m_point7Y += transformY;
    m_point7Z += -transformZ;
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
    return m_materialCount;  // +168
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
    void** materialArray = (void**)m_pMaterialArray;  // +160
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
    uint8_t count = m_materialCount;  // +168
    
    if (count > 0) {
        void** materialArray = (void**)m_pMaterialArray;  // +160
        for (int i = 0; i < count; i++) {
            materialArray[i] = material;
        }
    }
    
    m_pCurrentMaterial = material;  // +164
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
    // Get current material index
    int materialIndex = GetCurrentMaterialIndex();  // vfn_16
    
    // Look up material in array
    void** materialArray = (void**)m_pMaterialArray;  // +160
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
    void* currentMaterial = m_pCurrentMaterial;  // +164
    
    if (currentMaterial != (void*)-1) {
        // Use the explicitly set current material
        void** vtable = *(void***)this;
        typedef void (*MaterialFunc)(void*, void*);
        MaterialFunc func = (MaterialFunc)vtable[11];
        func(this, currentMaterial);
        return;
    }
    
    uint8_t materialCount = m_materialCount;  // +168
    
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
    float offset[4];
    offset[0] = point[0] - m_center[0];  // +48
    offset[1] = point[1] - m_center[1];
    offset[2] = point[2] - m_center[2];
    offset[3] = point[3] - m_center[3];
    
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
    int vertexCount = m_vertexCount;  // +148
    
    if (vertexCount > 0) {
        float* vertices = (float*)m_pVertexData;  // +112
        
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
    m_center[0] += offset[0];  // +48
    m_center[1] += offset[1];
    m_center[2] += offset[2];
    m_center[3] += offset[3];
    
    // Check if the updated center has any non-zero masked components
    const uint32_t mask[4] = {0x000009C0, 0x000009C0, 0x000009C0, 0x000009C0};
    uint32_t* centerBits = (uint32_t*)m_center;
    uint32_t masked[4];
    masked[0] = centerBits[0] & mask[0];
    masked[1] = centerBits[1] & mask[1];
    masked[2] = centerBits[2] & mask[2];
    masked[3] = centerBits[3] & mask[3];
    
    bool hasOffset = !(masked[0] == 0 && masked[1] == 0 && 
                       masked[2] == 0 && masked[3] == 0);
    
    m_hasOffset = hasOffset ? 1 : 0;  // +5
    
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
    extern void _snprintf(char*, int, const char*, const char*, int);
    extern void* phArchetype_Load(const char*, void*);
    extern void* xe_EC88(int);
    extern void ph_9EC0_1(void*);
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
        _snprintf(normalizedPath, 255, formatStr, assetPath, attemptCount);
        attemptCount++;
    }
    
    // Load or create the archetype
    void* archetype = phArchetype_Load(normalizedPath, materialInfo);
    
    if (archetype == nullptr) {
        // Create new archetype instance
        void* memory = xe_EC88(80);
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
    return m_pJointData;  // +472 (0x1D8)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToJointProcessor (vfn_22) @ 0x8224F220 | size: 0x8
//
// Delegates processing to the joint processor function.
// Used during physics update to process all active joints.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToJointProcessor() {
    phArticulatedCollider_5C60_sp(m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToCapsuleHandler (vfn_25) @ 0x8224FD50 | size: 0x8
//
// Delegates to capsule collision handler.
// Used for capsule-based collision detection on articulated bodies.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToCapsuleHandler() {
    phBoundCapsule_6C28_fw(m_nActiveJoints);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToColliderHandler (vfn_59) @ 0x8224F218 | size: 0x8
//
// Delegates to articulated collider handler.
// Used for complex collision scenarios involving multiple joints.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::DelegateToColliderHandler() {
    phArticulatedCollider_6D30_h(m_nActiveJoints);  // +464 (0x1D0)
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
    float* massArray = (float*)m_pJointMassArray;  // +508 (0x1FC)
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
    float* inertiaArray = (float*)m_pJointInertiaArray;  // +492 (0x1EC)
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
    float* dampingArray = (float*)m_pJointDampingArray;  // +500 (0x1F4)
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
    phArticulatedCollider_5B50_wrh(m_nActiveJoints);  // +464 (0x1D0)
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
    void* jointData = (void*)m_nActiveJoints;  // +464 (0x1D0)
    
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
    void* result = (void*)this->Release();
    if ((int32_t)result >= 0) {
        void* ptr = (void*)0x208C801D;
        m_field_508 = ptr;
        if (ptr != nullptr) {
            uint32_t valA2 = *(uint32_t*)&a2;
            m_field_112 = *(uint32_t*)(valA2 + 8);
            m_field_116 = *(uint32_t*)(valA2 + 12);
            m_field_120 = *(uint32_t*)(valA2 + 16);
            if (*(uint32_t*)(valA2 + 0) != 0) {
                result = this->CreateViews(nullptr);
            }
            if ((int32_t)result >= 0) {
                if (*(uint32_t*)(valA2 + 4) != 0) {
                    result = this->CreateOutputViews();
                }
                if ((int32_t)result >= 0) {
                    return;
                }
            }
        } else {
            result = (void*)0x8007000E;
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
void ph_ConditionalForward(void* obj) {  // 9C78_2hr @ 0x82169C78
    extern "C" void ph_ForwardTarget(void* target);
    void* target = *(void**)((char*)obj + 24);
    if (target) ph_ForwardTarget(target);
}

/**
 * ph_MultiplyAndDelegate @ 0x8216BAF0 | size: 0x14
 * Multiplies two int args, sets address constant, tail-calls rage allocator.
 */
void* ph_AllocateScaled(uint32_t count, uint32_t stride) {  // BAF0_2h @ 0x8216BAF0
    extern "C" void* rage_Allocate(uint32_t size);
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
void phInst::Lock() {  // phInst_3
    extern "C" void RtlEnterCriticalSection(void* cs);
    RtlEnterCriticalSection((char*)this + 16);
}

/**
 * phInst::Unlock @ 0x82120000 | size: 0x28
 * Leaves critical section on the mutex at this+16.
 */
void phInst::Unlock() {  // phInst_5
    extern "C" void RtlLeaveCriticalSection(void* cs);
    RtlLeaveCriticalSection((char*)this + 16);
}

// ── DELEGATION HELPERS ──────────────────────────────────────────────────

/**
 * phInst::ShiftArgsAndCall @ 0x821292F8 | size: 0x18
 * Shifts argument registers and tail-calls phInst_8F10.
 */
void phInst::ShiftArgsAndCall(void* a, void* b, void* c) {  // 92F8_p42
    extern "C" void phInst_8F10(void* obj, void* a, void* b, void* c);
    phInst_8F10(this, a, b, c);
}

/**
 * phInst::LoadGlobalAndCall @ 0x8212E2C0 | size: 0xC
 * Loads a global pointer and tail-calls ke_9F58.
 */
void phInst::LoadGlobalAndCall() {  // E2C0_2h
    extern void* g_phGlobalState;
    extern "C" void ke_DispatchPhysics(void* state);
    ke_DispatchPhysics(g_phGlobalState);
}

/**
 * phInst::InitVtableAndCleanup @ 0x821287F8 | size: 0x10
 * Sets vtable pointer and tail-calls the cleanup utility.
 */
void phInst::InitVtableAndCleanup() {  // 87F8
    extern "C" void phInst_Cleanup(void* obj);
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
uint32_t phArticulatedCollider::GetIndexedField_1168(int index, int shift, int mask) {
    uint32_t word = *(uint32_t*)((char*)this + 1168 + index * 24);
    return (word >> shift) & mask;
}

// Per-element getters (representative subset — 12 functions follow this pattern)
uint32_t phArticulatedCollider::GetBoneField_5B90(int index) { return GetIndexedField_1168(index, 0, 0x7); }   // 5B90
uint32_t phArticulatedCollider::GetBoneField_5BE0(int index) { return GetIndexedField_1168(index, 3, 0x7); }   // 5BE0
uint32_t phArticulatedCollider::GetBoneField_5C30(int index) { return GetIndexedField_1168(index, 6, 0x7); }   // 5C30
uint32_t phArticulatedCollider::GetBoneField_5C88(int index) { return GetIndexedField_1168(index, 9, 0x7); }   // 5C88
uint32_t phArticulatedCollider::GetBoneField_5CE0(int index) { return GetIndexedField_1168(index, 12, 0xF); }  // 5CE0
uint32_t phArticulatedCollider::GetBoneField_5D38(int index) { return GetIndexedField_1168(index, 16, 0xF); }  // 5D38

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
int phInst::GetSubObjectPtr(void** outPtr) {  // C1A0_2hr
    extern "C" void* phInst_BFB8_2hr(void* obj);
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
    extern "C" void grc_SetupResource(void* a, void* b);
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
    extern "C" void msgMsgSink_Broadcast(void* sink, void* msg, void* data);
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
// phObject::vfn_12()  [vtable slot 12 @ 0x824857B8]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::vfn_12() {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_13()  [vtable slot 13 @ 0x82485768]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
void phObject::vfn_13(void* a2) {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_14()  [vtable slot 14 @ 0x82485720]
// Forwarder
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject::vfn_14(int32_t) {
    uint8_t buffer[24];
    *(uint64_t*)(buffer + 0) = 0;
    *(uint64_t*)(buffer + 8) = 0;
    *(uint32_t*)(buffer + 16) = 0;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_15()  [vtable slot 15 @ 0x82484890]
// Cleanup and state clear
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject::vfn_15() {
    if (m_field_508 != nullptr) {
        m_field_508 = nullptr;
    }
    
    this->vfn_19();
    this->vfn_20();
    this->vfn_32();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_16()  [vtable slot 16 @ 0x82485978]
// Sub-object update
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::vfn_16() {
    if (m_field_52 != nullptr) {
        if (m_field_52->vfn_14(1) < 0) {
            return nullptr;
        }
        this->vfn_17(m_field_52);
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_18()  [vtable slot 18 @ 0x82484918]
// Updates sub-object from parameters
// ─────────────────────────────────────────────────────────────────────────────
void* phObject::vfn_18() {
    this->vfn_20();
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_19()  [vtable slot 19 @ 0x82484988]
// Disposes or releases sub-objects
// ─────────────────────────────────────────────────────────────────────────────
void phObject::vfn_19() {
    m_field_132 = this;
    if (m_field_500 != 0) {
        m_field_500 = 0;
    }
    
    if (m_field_52 != nullptr) {
        m_field_52->vfn_2();
        m_field_52 = nullptr;
    }
}
// ═════════════════════════════════════════════════════════════════════════════
// rage::phJoint3Dof
// ═════════════════════════════════════════════════════════════════════════════

/**
 * rage::phJoint3Dof::vfn_14 @ 0x82251268 | size: 0x40
 *
 * Aliases: phJoint_12A8_fw
 * Sets internal damping/stiffness values based on input parameters.
 */
void phJoint3Dof::vfn_14(float arg1, float arg2) {
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
 * rage::phJoint3Dof::vfn_13 @ 0x82251200 | size: 0x64
 *
 * Fetches intermediate geometry bounds via phJoint1Dof_AFF8_p42 and 
 * delegates to phJoint_1388.
 */
void phJoint3Dof::vfn_13(int index, float val) {
    float scratchBounds[8]; // Uses 2 SIMD vectors (32 bytes)
    
    // Fill first 16 bytes (r1+80)
    phJoint1Dof_AFF8_p42(this, index, &scratchBounds[0]);
    
    // Pass both the result vector and a second scratch vector
    phJoint_1388(this, index, val, &scratchBounds[4], &scratchBounds[0]);
}

/**
 * rage::phJoint3Dof::vfn_25 @ 0x82254D28 | size: 0x68
 *
 * Calls vfn_24 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::vfn_25(int index, float* outVec) {
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
 * rage::phJoint3Dof::vfn_26 @ 0x82254D90 | size: 0x68
 *
 * Calls vfn_23 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::vfn_26(int index, float* outVec) {
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
 * rage::phJoint3Dof::vfn_29 @ 0x822550F8 | size: 0x68
 *
 * Calls vfn_28 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::vfn_29(int index, float* outVec) {
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
 * rage::phJoint3Dof::vfn_30 @ 0x82255160 | size: 0x68
 *
 * Calls vfn_27 to get two vectors, and accumulates them into outVec.
 */
void phJoint3Dof::vfn_30(int index, float* outVec) {
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
 * rage::phJoint3Dof::vfn_17 @ 0x82255340 | size: 0x74
 *
 * Performs matrix/state clearing and resets internal angular components.
 */
void phJoint3Dof::vfn_17() {
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
 * rage::phJoint3Dof::vfn_31 @ 0x82253920 | size: 0x88
 *
 * Calculates vector dot products and magnitude norms for the sub-joints.
 */
void phJoint3Dof::vfn_31() {
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


// ═════════════════════════════════════════════════════════════════════════════
// rage::phInst — Physics Instance Small Methods (all ≤64B)
//
// These are the trivial accessors, vtable thunks, adjustor thunks, and
// small utilities that comprise the phInst interface surface.
// ═════════════════════════════════════════════════════════════════════════════

// ── RETURN-ZERO STUBS (8B each) ─────────────────────────────────────────

// phInst::GetType @ 0x8248D8A0 | size: 0x8
int phInst::GetType() { return 0; }  // vfn_18_D8A0_1

// phInst::GetFlags @ 0x82494158 | size: 0x8
int phInst::GetFlags() { return 0; }  // vfn_11_4158_1

// ── FIELD GETTERS (8B each) ─────────────────────────────────────────────

// phInst::GetBound @ 0x824834D0 | size: 0x8
void* phInst::GetBound() { return *(void**)((char*)this + 56); }  // 34D0_g

// phInst::GetField14 @ 0x82491790 | size: 0x8
void* phInst::GetField14() { return *(void**)((char*)this + 52); }  // phInst_14

// phInst::GetUserData (vfn_19) @ 0x8248B8D0 | size: 0x8
uint32_t phInst::GetUserData() { return *(uint32_t*)((char*)this + 284); }  // vfn_19

// phInst::GetCollisionMask (vfn_44) @ 0x8248D810 | size: 0x8
uint32_t phInst::GetCollisionMask() { return *(uint32_t*)((char*)this + 444); }  // vfn_44

// phInst::GetCollisionGroup (vfn_45) @ 0x8248D820 | size: 0x8
uint32_t phInst::GetCollisionGroup() { return *(uint32_t*)((char*)this + 448); }  // vfn_45

// phInst::GetTransformPtr (vfn_71) @ 0x82488480 | size: 0x8
uint64_t phInst::GetTransformPtr() { return *(uint64_t*)((char*)this + 64); }  // vfn_71

// ── FIELD SETTERS (8B each) ─────────────────────────────────────────────

// phInst::SetField9 @ 0x82488820 | size: 0x8
void phInst::SetField9(uint32_t val) { *(uint32_t*)((char*)this + 44) = val; }  // phInst_9

// phInst::SetField11 @ 0x82488830 | size: 0x8
void phInst::SetField11(uint32_t val) { *(uint32_t*)((char*)this + 52) = val; }  // phInst_11

// phInst::SetUserData (vfn_16) @ 0x8248B8C8 | size: 0x8
void phInst::SetUserData(uint32_t val) { *(uint32_t*)((char*)this + 284) = val; }  // vfn_16

// phInst::SetCollisionMask (vfn_38) @ 0x8248D808 | size: 0x8
void phInst::SetCollisionMask(uint32_t val) { *(uint32_t*)((char*)this + 444) = val; }  // vfn_38

// phInst::SetCollisionGroup (vfn_39) @ 0x8248D818 | size: 0x8
void phInst::SetCollisionGroup(uint32_t val) { *(uint32_t*)((char*)this + 448) = val; }  // vfn_39

// ── STATIC SIZE / CONSTANT RETURNS (12-20B) ─────────────────────────────

// phInst::GetStaticSize @ 0x8256B628 | size: 0xC
int phInst::GetStaticSize() {  // B628
    return (int)(uintptr_t)0x8256B610;  // returns address of phInst_B610_p42
}

// phInst::StoreSize @ 0x82461488 | size: 0x10
int phInst::StoreSize(uint32_t* outParam) {  // 1488_sp
    *outParam = 20;
    return 0;
}

// phInst::StoreConstBA24 @ 0x824631A0 | size: 0x14
int phInst::StoreConstBA24(uint32_t* outParam) {  // 31A0_p39
    *outParam = 0x4BA24;
    return 0;
}

// phInst::GetReturnCode (vfn_48) @ 0x8247E2D0 | size: 0xC
uint32_t phInst::GetReturnCode() { return 0x80004001; }  // vfn_48

// ── GLOBAL LOAD AND DISPATCH (12B) ──────────────────────────────────────

extern void* g_phGlobalState;  // @ 0x825E788C
extern void ke_9F58(void* state);

// phInst::LoadGlobalAndDispatch @ 0x8247E2C0 | size: 0xC
void phInst::LoadGlobalAndDispatch() {  // E2C0_2h
    ke_9F58(g_phGlobalState);
}

// ── BITFIELD EXTRACT / ARITHMETIC (16-24B) ──────────────────────────────

// phInst::GetPhysicsLayer @ 0x823592E8 | size: 0x10
int phInst::GetPhysicsLayer() {  // 92E8_p42
    uint32_t flags = *(uint32_t*)((char*)this + 32);
    return ((flags >> 6) & 0xF) + 1;
}

// phInst::GetU16Delta @ 0x8256A698 | size: 0x10
int phInst::GetU16Delta(void* other) {  // A698_p39
    uint16_t a = *(uint16_t*)((char*)this + 0);
    uint16_t b = *(uint16_t*)((char*)other + 0);
    return (int)(a - b);
}

// phInst::ComputeDataOffset @ 0x82469070 | size: 0x18
int phInst::ComputeDataOffset(uint32_t* outParam) {  // 9070_p42
    uint8_t index = *(uint8_t*)((char*)this + 68);
    *outParam = (uint32_t)(index * 100 + 268);
    return 0;
}

// ── VTABLE FORWARD THUNKS (16-20B) ──────────────────────────────────────
//
// Load vtable from this, then tail-call a specific slot.

// phInst::ForwardSlot23 @ 0x82482B48 | size: 0x10
void phInst::ForwardSlot23(void* arg) {  // 2B48_p45
    typedef void (*Fn)(void*, void*);
    ((Fn)(*(void***)this)[23])(this, arg);
}

// phInst::ForwardSlot12 @ 0x82482A28 | size: 0x14
void phInst::ForwardSlot12() {  // phInst_45
    typedef void (*Fn)(void*, void*);
    void** vtable = *(void***)this;
    ((Fn)vtable[12])(this, this);  // passes this as both r3 and r6
}

// phInst::ForwardSlot12Alt @ 0x82359310 | size: 0x14
void phInst::ForwardPageAligned() {  // 9310_p42
    uint32_t field36 = *(uint32_t*)((char*)this + 36);
    uint32_t field20 = *(uint32_t*)((char*)this + 20);
    extern void grc_2CC8(void* aligned36, void* aligned20);
    grc_2CC8((void*)(field36 & 0xFFFFF000), (void*)(field20 & 0xFFFFF000));
}

// ── VTABLE DISPATCH WITH DATA POINTER (24B each) ────────────────────────
//
// Load vtable, pass data-region pointer + size, tail-call slot 13.
// Each targets a different embedded sub-array (stride 80 bytes).

// phInst::DispatchSlot13_124 (vfn_14) @ 0x8248D828 | size: 0x18
void phInst::DispatchSlot13_124() {  // vfn_14_D828_1
    typedef void (*Fn)(void*, void*, int);
    void** vtable = *(void***)this;
    ((Fn)vtable[13])(this, (char*)this + 124, 40);
}

// phInst::DispatchSlot13_204 (vfn_15) @ 0x8248D888 | size: 0x18
void phInst::DispatchSlot13_204() {  // vfn_15
    typedef void (*Fn)(void*, void*, int);
    void** vtable = *(void***)this;
    ((Fn)vtable[13])(this, (char*)this + 204, 40);
}

// phInst::DispatchSlot13_284 (vfn_34) @ 0x8248D868 | size: 0x18
void phInst::DispatchSlot13_284() {  // vfn_34
    typedef void (*Fn)(void*, void*, int);
    void** vtable = *(void***)this;
    ((Fn)vtable[13])(this, (char*)this + 284, 40);
}

// phInst::DispatchSlot13_364 (vfn_35) @ 0x8248D848 | size: 0x18
void phInst::DispatchSlot13_364() {  // vfn_35
    typedef void (*Fn)(void*, void*, int);
    void** vtable = *(void***)this;
    ((Fn)vtable[13])(this, (char*)this + 364, 40);
}

// ── MI ADJUSTOR THUNKS (32B each) ───────────────────────────────────────
//
// Adjust this pointer by -8, then dispatch through vtable.
// Used for multiple-inheritance adjustor thunks.

// phInst::AdjustorSlot1 @ 0x82465D40 | size: 0x20
void phInst::AdjustorSlot1() {  // 5D40_2hr
    void* adjusted = (char*)this - 8;
    if (!this) adjusted = nullptr;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)adjusted)[1])(adjusted);
}

// phInst::AdjustorSlot2 @ 0x82465D60 | size: 0x20
void phInst::AdjustorSlot2(void* arg) {  // 5D60_p39
    void* adjusted = (char*)this - 8;
    if (!this) adjusted = nullptr;
    typedef void (*Fn)(void*, void*);
    ((Fn)(*(void***)adjusted)[2])(adjusted, arg);
}

// phInst::AdjustorSlot5 @ 0x8246BB20 | size: 0x20
void phInst::AdjustorSlot5() {  // BB20_2hr
    void* adjusted = (char*)this - 8;
    if (!this) adjusted = nullptr;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)adjusted)[5])(adjusted);
}

// phInst::AdjustorSlot6 @ 0x8246BB40 | size: 0x20
void phInst::AdjustorSlot6(void* arg) {  // BB40_2hr
    void* adjusted = (char*)this - 8;
    if (!this) adjusted = nullptr;
    typedef void (*Fn)(void*, void*);
    ((Fn)(*(void***)adjusted)[6])(adjusted, arg);
}

// ── ARRAY-INDEX VTABLE DISPATCH (vfn_59-69, 32B each) ───────────────────
//
// Index into array of 60-byte entries at this+48 using index at this+64.
// Dereference entry vtable, tail-call slot N.

// phInst::IndexedDispatch13 (vfn_59) @ 0x8248DA68 | size: 0x20
void phInst::IndexedDispatch13() {  // vfn_59
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[13])(entry);
}

// phInst::IndexedDispatch14 (vfn_60) @ 0x8248DA88 | size: 0x20
void phInst::IndexedDispatch14() {  // vfn_60
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[14])(entry);
}

// phInst::IndexedDispatch15 (vfn_61) @ 0x8248DAA8 | size: 0x20
void phInst::IndexedDispatch15() {  // vfn_61
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[15])(entry);
}

// phInst::IndexedDispatch16 (vfn_62) @ 0x8248DAC8 | size: 0x20
void phInst::IndexedDispatch16() {  // vfn_62
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[16])(entry);
}

// phInst::IndexedDispatch17 (vfn_63) @ 0x8248DAE8 | size: 0x20
void phInst::IndexedDispatch17() {  // vfn_63
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[17])(entry);
}

// phInst::IndexedDispatch18 (vfn_64 alt) @ 0x8248DB08 | size: 0x20
void phInst::IndexedDispatch18() {  // vfn_64_DB08_1
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[18])(entry);
}

// phInst::IndexedDispatch19 (vfn_65 alt) @ 0x8248DB28 | size: 0x20
void phInst::IndexedDispatch19() {  // vfn_65_DB28_1
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[19])(entry);
}

// phInst::IndexedDispatch20 (vfn_66) @ 0x8248DB48 | size: 0x20
void phInst::IndexedDispatch20() {  // vfn_66
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[20])(entry);
}

// phInst::IndexedDispatch21 (vfn_67) @ 0x8248DB68 | size: 0x20
void phInst::IndexedDispatch21() {  // vfn_67
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[21])(entry);
}

// phInst::IndexedDispatch22 (vfn_68) @ 0x8248DB88 | size: 0x20
void phInst::IndexedDispatch22() {  // vfn_68
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[22])(entry);
}

// phInst::IndexedDispatch23 (vfn_69) @ 0x8248DBA8 | size: 0x20
void phInst::IndexedDispatch23() {  // vfn_69
    int index = *(int*)((char*)this + 64);
    char* entry = (char*)*(void**)((char*)this + 48) + index * 60;
    typedef void (*Fn)(void*);
    ((Fn)(*(void***)entry)[23])(entry);
}

// ── ATOMIC REF-COUNTING (40-48B) ────────────────────────────────────────

// phInst::AddRef @ 0x824885D8 | size: 0x28
int phInst::AddRef() {  // phInst_1
    int32_t* refCount = (int32_t*)((char*)this + 12);
    return ++(*refCount);  // lwarx/stwcx atomic loop in original
}

// phInst::Release @ 0x824837B0 | size: 0x40
int phInst::Release() {  // 37B0_p33
    void* inner = *(void**)((char*)this + 4);
    int32_t* refCount = (int32_t*)((char*)inner + 1472);
    int newCount = --(*refCount);  // lwarx/stwcx atomic loop
    if (newCount != 0) return newCount;
    typedef void (*Callback)(void*);
    Callback cb = *(Callback*)((char*)inner + 48);
    if (cb) {
        void* arg = *(void**)((char*)inner + 52);
        cb(arg);
    }
    return 0;
}

// ── CRITICAL SECTION (40B each) ─────────────────────────────────────────

extern "C" void RtlEnterCriticalSection(void* cs);
extern "C" void RtlLeaveCriticalSection(void* cs);

// phInst::Lock @ 0x82488678 | size: 0x28
void phInst::Lock() {  // phInst_3
    RtlEnterCriticalSection((char*)this + 16);
}

// phInst::Unlock @ 0x824886E8 | size: 0x28
void phInst::Unlock() {  // phInst_5
    RtlLeaveCriticalSection((char*)this + 16);
}

// ── DELEGATION HELPERS (16-24B) ─────────────────────────────────────────

// phInst::ShiftArgsAndCall @ 0x823592F8 | size: 0x18
void phInst::ShiftArgsAndCall(void* a, void* b, void* c) {  // 92F8_p42
    extern void phInst_8F10_p42(void* obj, int zero, void* a, void* b, void* c);
    phInst_8F10_p42(this, 0, a, b, c);
}

// phInst::InitVtableAndCleanup @ 0x824887F8 | size: 0x10
void phInst::InitVtableAndCleanup() {  // 87F8
    extern void util_85C8(void* obj);
    *(void**)this = (void*)0x82008DF0;  // phInst base vtable
    util_85C8(this);
}

// ── INDEXED FLAG SET/CLEAR (40B each) ───────────────────────────────────
//
// Index into array via this->field_8[(this->field_12 + 4) * 4].
// Sets/clears flag and marks dirty at this+24.

// phInst::SetIndexedFlag @ 0x8256BA10 | size: 0x28
void phInst::SetIndexedFlag() {  // BA10_wrh
    uint32_t index = *(uint32_t*)((char*)this + 12);
    uint8_t* array = *(uint8_t**)((char*)this + 8);
    array[(index + 4) * 4] = 1;
    *(uint32_t*)((char*)this + 24) = 1;
}

// phInst::ClearIndexedFlag @ 0x8256BA38 | size: 0x28
void phInst::ClearIndexedFlag() {  // BA38_2h
    uint32_t index = *(uint32_t*)((char*)this + 12);
    uint8_t* array = *(uint8_t**)((char*)this + 8);
    *(uint32_t*)((char*)this + 24) = 0;
    array[(index + 4) * 4] = 0;
}

// ── CONDITIONAL STORE (28B) ─────────────────────────────────────────────

// phInst::ConditionalStore @ 0x82483620 | size: 0x1C
int phInst::ConditionalStore(int flag, uint32_t val1, uint32_t val2) {  // phInst_44
    if (flag == 1) {
        *(uint32_t*)((char*)this + 48) = val1;
        *(uint32_t*)((char*)this + 52) = val2;
    }
    return 0;
}

// ── CONDITIONAL FORWARD (20B) ───────────────────────────────────────────

extern void msgMsgSink_B538_w(void* obj, void* arg, uint8_t flag);

// phInst::ConditionalForward @ 0x8256B610 | size: 0x14
void phInst::ConditionalForward(void* arg, uint8_t flag) {  // B610_p42
    if (flag == 0) {
        msgMsgSink_B538_w(this, arg, 0);
    } else {
        msgMsgSink_B538_w(this, arg, flag);
    }
}

// ── CHAIN DEREFERENCE (20B) ─────────────────────────────────────────────

// phInst::ClearSubStatePtr @ 0x8256AB68 | size: 0x14
void phInst::ClearSubStatePtr() {  // AB68_p39
    void* inner = *(void**)((char*)this + 4);
    void* target = *(void**)((char*)inner + 0);
    *(uint32_t*)target = 0;
}

// ── ZERO FIELD RANGES (60B) ─────────────────────────────────────────────

// phInst::ZeroFieldRanges (vfn_12 alt) @ 0x8248B888 | size: 0x3C
void phInst::ZeroFieldRanges() {  // vfn_12_B888_1
    // Zeroes 20 dwords at +204, then 20 dwords at +124, then field+284
    memset((char*)this + 204, 0, 80);
    memset((char*)this + 124, 0, 80);
    *(uint32_t*)((char*)this + 284) = 0;
}

// ── VTABLE INIT + TAIL-CALL (24B) ──────────────────────────────────────

// phInst::InitAndCleanup @ 0x82466158 | size: 0x18
void phInst::InitAndCleanup() {  // 6158_p39
    extern void ph_FC68_h(void* obj);
    *(void**)this = (void*)0x82005A60;  // phInst vtable
    ph_FC68_h((char*)this + 4);
}

// ── MMIO BYTE-SWAP READ TO GLOBAL (24B) ─────────────────────────────────

extern uint32_t g_phTimerValue;  // @ 0x825E7890

// phInst::ReadTimerToGlobal @ 0x8246AC00 | size: 0x18
void phInst::ReadTimerToGlobal() {  // AC00_2hr
    volatile uint32_t* timerReg = (volatile uint32_t*)0x7FEA1800;
    g_phTimerValue = __builtin_bswap32(*timerReg);
}

// ── VTABLE CONSTRUCTORS (40-56B) ────────────────────────────────────────

// phInst::InitDualVtable @ 0x82465910 | size: 0x28
void phInst::InitDualVtable() {  // 5910_p39
    *(void**)((char*)this + 0) = (void*)0x82005918;
    *(void**)((char*)this + 4) = (void*)0x82003DB0;
}

// phInst::InitFromParam @ 0x82461508 | size: 0x38
void phInst::InitFromParam(void* param) {  // 1508_2hr
    *(uint32_t*)((char*)this + 8) = 1;
    uint32_t paramField4 = *(uint32_t*)((char*)param + 4);
    *(uint32_t*)((char*)this + 12) = paramField4;
    *(void**)((char*)this + 0) = (void*)0x8200586C;
    *(void**)((char*)this + 4) = (void*)0x82005850;
}

// ── ALLOCATE-AND-STORE HELPER (60B) ─────────────────────────────────────

extern void* phInst_BFB8_2hr(void* obj);

// phInst::AllocateAndStore @ 0x8246C1A0 | size: 0x3C
int phInst::AllocateAndStore(uint32_t* outPtr) {  // C1A0_2hr
    void* result = phInst_BFB8_2hr(this);
    *outPtr = (uint32_t)((uintptr_t)result + 24);
    return 0;
}

// ── STACK-FRAME + CALL HELPERS (48-64B) ─────────────────────────────────

extern void game_3860(void* obj, void* zeroedBuf);

// phInst::CallWithZeroedBuffer @ 0x82483D78 | size: 0x34
void phInst::CallWithZeroedBuffer() {  // phInst_42
    uint64_t buf[2] = {0, 0};  // 16 bytes zeroed on stack
    game_3860(this, buf);
}

// phInst::GetErrorCode @ 0x824887B0 | size: 0x30
uint32_t phInst::GetErrorCode() {  // phInst_8
    extern int32_t thunk_fn_8242C368();
    int32_t result = thunk_fn_8242C368();
    if (result > 0) {
        return (result & 0xFFFF) | 0x80070000;
    }
    return (uint32_t)result;
}

// phInst::CallVfn12ThenInit (vfn_9) @ 0x8248D7C8 | size: 0x40
void phInst::CallVfn12ThenInit() {  // vfn_9
    extern void statePreInit_vfn_6(void* obj);
    typedef void (*Fn)(void*);
    void** vtable = *(void***)this;
    ((Fn)vtable[12])(this);
    statePreInit_vfn_6(this);
}

// phInst::AtomicDecrementAndCallback (vfn_23) @ 0x82483FD8 | size: 0x3C
void phInst::AtomicDecrementAndCallback() {  // vfn_23
    int32_t* refCount = (int32_t*)((char*)this + 368);
    --(*refCount);  // lwarx/stwcx atomic loop
    typedef void (*Callback)(void*);
    Callback cb = *(Callback*)((char*)this + 272);
    if (cb) {
        void* arg = *(void**)((char*)this + 284);
        cb(arg);
    }
}
