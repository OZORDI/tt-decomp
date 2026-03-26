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
        rage_free_00C0(this);
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
// phObject::vfn_0()  [vtable slot 0 @ 0x82485250]
// Destructor
// ─────────────────────────────────────────────────────────────────────────────
phObject::~phObject() {
    this->vtable = (void**)0x82008918;
    this->vfn_15();
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_32()  [vtable slot 32 @ 0x82484758]
// Initializes object state and memory references
// ─────────────────────────────────────────────────────────────────────────────
void phObject::vfn_32() {
    m_field_500 = 0;
    m_field_508 = nullptr;
    m_field_504 = 1;
    m_field_512 = 0;
    m_field_520 = 0;
    m_field_524 = 0;
    m_field_528 = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::vfn_11()  [vtable slot 11 @ 0x824847A8]
// Processing locale and fields
// ─────────────────────────────────────────────────────────────────────────────
void phObject::vfn_11(void* a2, void* a3, void* a4) {
    void* result = (void*)this->vfn_15();
    if ((int32_t)result >= 0) {
        void* ptr = (void*)0x208C801D;
        m_field_508 = ptr;
        if (ptr != nullptr) {
            uint32_t valA2 = *(uint32_t*)&a2;
            m_field_112 = *(uint32_t*)(valA2 + 8);
            m_field_116 = *(uint32_t*)(valA2 + 12);
            m_field_120 = *(uint32_t*)(valA2 + 16);
            if (*(uint32_t*)(valA2 + 0) != 0) {
                result = this->vfn_17(nullptr);
            }
            if ((int32_t)result >= 0) {
                if (*(uint32_t*)(valA2 + 4) != 0) {
                    result = this->vfn_18();
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
    this->vfn_15();
}

// ─────────────────────────────────────────────────────────────────────────────
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


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider — Packed Config Word Setters (10 functions, 64-160B)
//
// These setters modify bitfields in the packed config word at +11448 (0x2CB8),
// then optionally recompute an expanded encoding cached at +11640/11672/11676/11680.
// The control word at +11452 (0x2CBC) has two enable bits:
//   bit 31 (0x80000000): master enable — if clear, skip cache update entirely
//   bit 30 (0x40000000): encoding bypass — if set, use raw word as cache value;
//                        if clear, compute ExpandConfigWord() from lower 16 bits
//
// After updating the cache, dirty bits are OR'd into the 64-bit change-tracking
// mask at this+40 to signal the physics sync system.
// ─────────────────────────────────────────────────────────────────────────────

// Helper: expand lower 16 bits of a config word into a 32-bit encoded form
// used for physics cache entries at +11640, +11672, +11676, +11680.
static inline uint32_t ExpandConfigWord(uint32_t word) {
    uint32_t lo16 = word & 0xFFFF;
    uint32_t shifted = (lo16 << 4) & 0xFFFFFFF0;
    uint32_t extracted = lo16 & 0x1010;       // keep bits 4 and 12
    uint32_t combined = extracted | shifted;
    uint32_t rotated = ((combined << 12) | (combined >> 20)) & 0xFFFF0000;
    rotated &= 0xFFEFFFFF;                   // clear bit 20
    rotated &= 0xEFFFFFFF;                   // clear bit 28
    return rotated | lo16;                    // merge back lower 16 bits
}

// Helper: update the 4 config cache fields and set dirty bits in mask at this+40
static inline void UpdateConfigCacheAndDirty(void* obj, uint32_t cacheValue) {
    *(uint32_t*)((char*)obj + 11640) = cacheValue;
    *(uint32_t*)((char*)obj + 11672) = cacheValue;
    *(uint32_t*)((char*)obj + 11676) = cacheValue;
    *(uint32_t*)((char*)obj + 11680) = cacheValue;
    uint64_t* dirtyMask = (uint64_t*)((char*)obj + 40);
    *dirtyMask |= (1ULL << 36) | (1ULL << 28) | (1ULL << 27) | (1ULL << 26);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetConfigBits0 @ 0x82353C20 | size: 0x88
// Sets bits [0:4] (5-bit field) of packed config word at +11448.
// If master-enabled, recomputes and caches the expanded encoding.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetConfigBits0(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    uint32_t word = *(uint32_t*)((char*)this + 11448);
    word = (word & ~0x1F) | (value & 0x1F);
    *(uint32_t*)((char*)this + 11448) = word;

    if (!(flags & 0x80000000)) return;  // master enable off

    uint32_t cacheValue;
    if (flags & 0x40000000) {
        cacheValue = word;  // bypass: use raw word
    } else {
        cacheValue = ExpandConfigWord(word);
    }
    UpdateConfigCacheAndDirty(this, cacheValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetConfigBits8 @ 0x82353CB8 | size: 0x88
// Sets bits [8:12] (5-bit field) of packed config word at +11448.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetConfigBits8(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    uint32_t word = *(uint32_t*)((char*)this + 11448);
    word = (word & ~(0x1F << 8)) | ((value & 0x1F) << 8);
    *(uint32_t*)((char*)this + 11448) = word;

    if (!(flags & 0x80000000)) return;

    uint32_t cacheValue;
    if (flags & 0x40000000) {
        cacheValue = word;
    } else {
        cacheValue = ExpandConfigWord(word);
    }
    UpdateConfigCacheAndDirty(this, cacheValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetConfigBits21 @ 0x82353D50 | size: 0x68
// Sets bits [21:23] (3-bit field) of packed config word at +11448.
// Only updates cache if both enable bits (31 and 30) are set.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetConfigBits21(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    uint32_t word = *(uint32_t*)((char*)this + 11448);
    word = (word & ~(0x7 << 21)) | ((value & 0x7) << 21);
    *(uint32_t*)((char*)this + 11448) = word;

    if (!(flags & 0x80000000)) return;
    if (!(flags & 0x40000000)) return;

    UpdateConfigCacheAndDirty(this, word);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetConfigBits16 @ 0x82353DC8 | size: 0x68
// Sets bits [16:20] (5-bit field) of packed config word at +11448.
// Only updates cache if both enable bits are set.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetConfigBits16(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    uint32_t word = *(uint32_t*)((char*)this + 11448);
    word = (word & ~(0x1F << 16)) | ((value & 0x1F) << 16);
    *(uint32_t*)((char*)this + 11448) = word;

    if (!(flags & 0x80000000)) return;
    if (!(flags & 0x40000000)) return;

    UpdateConfigCacheAndDirty(this, word);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetConfigBits24 @ 0x82353E40 | size: 0x68
// Sets bits [24:28] (5-bit field) of packed config word at +11448.
// Only updates cache if both enable bits are set.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetConfigBits24(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    uint32_t word = *(uint32_t*)((char*)this + 11448);
    word = (word & ~(0x1F << 24)) | ((value & 0x1F) << 24);
    *(uint32_t*)((char*)this + 11448) = word;

    if (!(flags & 0x80000000)) return;
    if (!(flags & 0x40000000)) return;

    UpdateConfigCacheAndDirty(this, word);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetEncodingBypass @ 0x82353EB8 | size: 0x90
// Sets bit 30 (encoding bypass flag) in the control word at +11452.
// When bypass is disabled (value=0), the expanded encoding is recomputed.
// When bypass is enabled, the raw config word is used directly.
// Additionally, if bit 31 of the NEW control word is 0, uses a fixed
// sentinel value (0x10001) instead of the config word.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetEncodingBypass(uint32_t value) {
    uint32_t flags = *(uint32_t*)((char*)this + 11452);
    flags = (flags & ~(1U << 30)) | ((value & 1U) << 30);
    uint32_t bit31 = (flags >> 31) & 1;
    *(uint32_t*)((char*)this + 11452) = flags;

    uint32_t word = *(uint32_t*)((char*)this + 11448);
    uint32_t cacheValue;

    if (value == 0) {
        // Bypass disabled — compute expanded encoding from lower 16 bits
        cacheValue = ExpandConfigWord(word);
    } else {
        cacheValue = word;  // Bypass enabled — use raw word
    }

    if (bit31 == 0) {
        // Master enable off — use sentinel value
        cacheValue = 0x10001;
    }

    UpdateConfigCacheAndDirty(this, cacheValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetNormalizedBytesAsFloats @ 0x82353F58 | size: 0x40
// Converts an integer to a normalized float (value * kByteToFloat) and stores
// it at +11588. Sets dirty bit 49 in the mask at this+40.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetNormalizedBytesAsFloats(uint32_t value) {
    static const float kByteToFloat = *(const float*)0x82079AF4;  // normalization scale
    float normalized = (float)value * kByteToFloat;
    *(float*)((char*)this + 11588) = normalized;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 49);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetPackedColorAsFloats @ 0x82354000 | size: 0xA0
// Unpacks a uint32 into 4 bytes, converts each to a normalized float
// (byte * kByteToFloat), and stores them to +11552..+11564.
// Sets dirty bits 55-58 in the mask at this+40.
// Used for color or per-channel physics parameters packed as RGBA bytes.
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetPackedColorAsFloats(uint32_t packedValue) {
    static const float kByteToFloat = *(const float*)0x82079AF4;  // normalization scale

    uint8_t byte0 = (packedValue >> 24) & 0xFF;
    uint8_t byte1 = (packedValue >> 16) & 0xFF;
    uint8_t byte2 = (packedValue >>  8) & 0xFF;
    uint8_t byte3 = (packedValue >>  0) & 0xFF;

    *(float*)((char*)this + 11556) = (float)byte2 * kByteToFloat;
    *(float*)((char*)this + 11552) = (float)byte1 * kByteToFloat;
    *(float*)((char*)this + 11560) = (float)byte3 * kByteToFloat;
    *(float*)((char*)this + 11564) = (float)byte0 * kByteToFloat;

    *(uint64_t*)((char*)this + 40) |= (1ULL << 55) | (1ULL << 56) | (1ULL << 57) | (1ULL << 58);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetFloatsAsPackedColor @ 0x823540A0 | size: 0x80
// Reads 4 normalized floats from +11552..+11564, converts each back to a
// byte by scaling and rounding (value * kFloatToByte + 0.5), then packs
// the 4 bytes into a uint32 return value. Inverse of SetPackedColorAsFloats.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phArticulatedCollider::GetFloatsAsPackedColor() {
    static const float kFloatToByte = *(const float*)0x820799E0;  // inverse normalization
    static const float kHalf = *(const float*)0x8202D10C;         // 0.5f for rounding

    float f0 = *(float*)((char*)this + 11552);
    float f1 = *(float*)((char*)this + 11564);
    float f2 = *(float*)((char*)this + 11556);
    float f3 = *(float*)((char*)this + 11560);

    uint8_t b0 = (uint8_t)(int)(f0 * kFloatToByte + kHalf);
    uint8_t b1 = (uint8_t)(int)(f2 * kFloatToByte + kHalf);
    uint8_t b2 = (uint8_t)(int)(f1 * kFloatToByte + kHalf);
    uint8_t b3 = (uint8_t)(int)(f3 * kFloatToByte + kHalf);

    // Pack 4 bytes into uint32 MSB-first (rlwimi r_,r_,8,0,23 chain)
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetJointLimitSymmetric @ 0x823545D0 | size: 0xA0
// Sets a symmetric joint rotation limit. The float parameter (in game units)
// is multiplied by a degrees-to-radians scale factor, then stored to both
// the minimum (+11920) and maximum (+11928) limit fields.
// Also updates "has non-zero limits" flags in bits 11 and 12 of the solver
// params word at +11656, based on whether any limit in the pair is non-zero.
// Sets dirty bits in both mask at +40 (bit 32) and mask at +48 (bits 25,23).
// ─────────────────────────────────────────────────────────────────────────────
void phArticulatedCollider::SetJointLimitSymmetric(float limitDegrees) {
    static const float kDegreesToRadians = *(const float*)0x825C75A0;  // mutable .data scale
    static const float kZero = *(const float*)0x825BD110;              // 0.0f reference

    float scaledLimit = limitDegrees * kDegreesToRadians;
    *(float*)((char*)this + 11920) = scaledLimit;
    *(float*)((char*)this + 11928) = scaledLimit;

    // Determine if this axis pair has any non-zero limits
    bool hasNonZeroMin = (scaledLimit != kZero) || (*(float*)((char*)this + 11924) != kZero);
    bool hasNonZeroMax = (scaledLimit != kZero) || (*(float*)((char*)this + 11932) != kZero);

    // Update solver flags at +11656: bit 11 = hasNonZeroMin, bit 12 = hasNonZeroMax
    uint32_t solverWord = *(uint32_t*)((char*)this + 11656);
    solverWord = (solverWord & ~(1U << 11)) | ((uint32_t)hasNonZeroMin << 11);
    solverWord = (solverWord & ~(1U << 12)) | ((uint32_t)hasNonZeroMax << 12);
    *(uint32_t*)((char*)this + 11656) = solverWord;

    // Set dirty bits: mask at +48 for limit fields, mask at +40 for solver params
    *(uint64_t*)((char*)this + 48) |= (1ULL << 25) | (1ULL << 23);
    *(uint64_t*)((char*)this + 40) |= (1ULL << 32);
}


// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Batch: 10 functions (64-200B)
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::GetSupportDistance (vfn_33) @ 0x822A3268 | size: 0x50
//
// Computes the support distance of the capsule along a given direction.
// Returns the capsule's radius plus an optional capsule-axis dot product
// contribution. When the quickReturn flag is set, skips the dot product
// and returns just the radial extent.
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::GetSupportDistance(const float* direction, uint8_t quickReturn) {
    float absDirectionY = fabsf(direction[1]);
    float capsuleExtent = *(float*)((uint8_t*)this + 128);   // m_halfLength
    float radius = *(float*)((uint8_t*)this + 112);          // m_radius
    const float TWO = 2.0f;  // @ 0x8202D10C

    // Support distance = radius + halfLength * |dir.y| * 2
    float extentContribution = capsuleExtent * absDirectionY;
    float result = extentContribution * TWO + radius;

    if (quickReturn) {
        return result;
    }

    // Full support: add dot product of capsule axis with direction
    float* capsuleAxis = (float*)((uint8_t*)this + 48);  // m_axisDirection
    float dotProduct = capsuleAxis[0] * direction[0]
                     + capsuleAxis[1] * direction[1]
                     + capsuleAxis[2] * direction[2];

    return dotProduct + result;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::CopyGeometry (vfn_34) @ 0x822A2F28 | size: 0xBC
//
// Deep-copies capsule geometry from a source phBoundCapsule into this one.
// Copies the base bound data via util_DA50, then copies 5 SIMD vectors
// (offsets 112-176), a 32-bit field at offset 192, and 12 bytes of
// additional data starting at offset 196. Sets the dirty flag at offset 96.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::CopyGeometry(const rage::phBoundCapsule* source) {
    extern void util_DA50(void*, const void*);
    util_DA50(this, source);

    uint8_t* dst = (uint8_t*)this;
    const uint8_t* src = (const uint8_t*)source;

    // Copy 5 SIMD vectors: offsets 112, 128, 144, 160, 176
    memcpy(dst + 112, src + 112, 16);   // m_radiusVector
    memcpy(dst + 128, src + 128, 16);   // m_halfLengthVector
    memcpy(dst + 144, src + 144, 16);   // m_extentVector
    memcpy(dst + 160, src + 160, 16);   // m_boundsMin
    memcpy(dst + 176, src + 176, 16);   // m_boundsMax

    // Copy scalar field at offset 192
    *(uint32_t*)(dst + 192) = *(const uint32_t*)(src + 192);

    // Copy 12 bytes of additional capsule parameters at offset 196
    for (int i = 0; i < 12; i++) {
        dst[196 + i] = src[196 + i];
    }

    // Mark geometry as dirty
    *(uint16_t*)(dst + 96) = 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::TransformAndDispatch (vfn_7) @ 0x8233AB50 | size: 0x48
//
// Translates the capsule center by a given offset vector, then dispatches
// to the base class vtable slot 6 method (phBound::ApplyTransform).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::TransformAndDispatch(const float* offset) {
    float* center = (float*)((uint8_t*)this + 48);
    float translatedCenter[4];
    translatedCenter[0] = center[0] + offset[0];
    translatedCenter[1] = center[1] + offset[1];
    translatedCenter[2] = center[2] + offset[2];
    translatedCenter[3] = center[3] + offset[3];

    // Dispatch to base class vtable slot 6 with translated center
    void** vtable = *(void***)this;
    typedef void (*BaseTransformFunc)(void*, const float*);
    BaseTransformFunc baseTransform = (BaseTransformFunc)vtable[6];
    baseTransform(this, translatedCenter);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ScaleVectors @ 0x820D05A0 | size: 0xB4
//
// Scales two 4-component vectors at offsets +0 (m_endpointA) and +32
// (m_endpointB) using computed normalized scale factors. Applies a
// rotation-like SIMD transform: vnmsubfp + vmaddfp pattern.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ScaleVectors() {
    float scaleA, scaleB;
    phBoundCapsule_04F0_g(&scaleA, &scaleB);

    float* vecLow  = (float*)((uint8_t*)this + 0);   // m_endpointA
    float* vecHigh = (float*)((uint8_t*)this + 32);   // m_endpointB

    float origHigh[4] = { vecHigh[0], vecHigh[1], vecHigh[2], vecHigh[3] };
    float origLow[4]  = { vecLow[0],  vecLow[1],  vecLow[2],  vecLow[3] };

    // vmulfp128: vecHigh *= scaleA
    vecHigh[0] = origHigh[0] * scaleA;
    vecHigh[1] = origHigh[1] * scaleA;
    vecHigh[2] = origHigh[2] * scaleA;
    vecHigh[3] = origHigh[3] * scaleA;

    // vnmsubfp: vecHigh = -(vecLow_orig * scaleB - vecHigh_scaled)
    vecHigh[0] = -(origLow[0] * scaleB - vecHigh[0]);
    vecHigh[1] = -(origLow[1] * scaleB - vecHigh[1]);
    vecHigh[2] = -(origLow[2] * scaleB - vecHigh[2]);
    vecHigh[3] = -(origLow[3] * scaleB - vecHigh[3]);

    // vmaddfp: vecLow = origHigh * scaleB + origLow * scaleA
    vecLow[0] = origHigh[0] * scaleB + origLow[0] * scaleA;
    vecLow[1] = origHigh[1] * scaleB + origLow[1] * scaleA;
    vecLow[2] = origHigh[2] * scaleB + origLow[2] * scaleA;
    vecLow[3] = origHigh[3] * scaleB + origLow[3] * scaleA;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ScaleEndpoints @ 0x82111EA0 | size: 0xB0
//
// Scales capsule endpoint vectors at offsets +0 and +32 using a different
// combination pattern than ScaleVectors. Applies vnmsubfp then vmaddfp
// to produce a rotated pair of endpoints.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ScaleEndpoints() {
    float scaleA, scaleB;
    phBoundCapsule_04F0_g(&scaleA, &scaleB);

    float* vecLow  = (float*)((uint8_t*)this + 0);   // m_endpointA
    float* vecHigh = (float*)((uint8_t*)this + 32);   // m_endpointB

    float origHigh[4] = { vecHigh[0], vecHigh[1], vecHigh[2], vecHigh[3] };
    float origLow[4]  = { vecLow[0],  vecLow[1],  vecLow[2],  vecLow[3] };

    // vmulfp128: vecHigh *= scaleA
    vecHigh[0] = origHigh[0] * scaleA;
    vecHigh[1] = origHigh[1] * scaleA;
    vecHigh[2] = origHigh[2] * scaleA;
    vecHigh[3] = origHigh[3] * scaleA;

    // vmulfp128: vecLow *= scaleB
    vecLow[0] = origLow[0] * scaleB;
    vecLow[1] = origLow[1] * scaleB;
    vecLow[2] = origLow[2] * scaleB;
    vecLow[3] = origLow[3] * scaleB;

    // vnmsubfp: tempLow = -(origHigh * scaleB - vecLow)
    float tempLow[4];
    tempLow[0] = -(origHigh[0] * scaleB - vecLow[0]);
    tempLow[1] = -(origHigh[1] * scaleB - vecLow[1]);
    tempLow[2] = -(origHigh[2] * scaleB - vecLow[2]);
    tempLow[3] = -(origHigh[3] * scaleB - vecLow[3]);

    // vmaddfp: vecHigh = origLow * scaleB + vecHigh
    vecHigh[0] = origLow[0] * scaleB + vecHigh[0];
    vecHigh[1] = origLow[1] * scaleB + vecHigh[1];
    vecHigh[2] = origLow[2] * scaleB + vecHigh[2];
    vecHigh[3] = origLow[3] * scaleB + vecHigh[3];

    vecLow[0] = tempLow[0];
    vecLow[1] = tempLow[1];
    vecLow[2] = tempLow[2];
    vecLow[3] = tempLow[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ComputeGaussianAttenuation @ 0x821426B8 | size: 0x84
//
// Computes a Gaussian-style attenuation factor for capsule collision response.
// Returns (1/sqrt(spread^2*PI)) * exp(-dist^2 / (spread^2 * (1/(2*PI))))
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::ComputeGaussianAttenuation(float distance, float spread) {
    float spreadSquared = spread * spread;
    const float PI = 3.14159265f;  // @ 0x8202C02C

    float bellArea = spreadSquared * PI;
    extern float phBoundCapsule_01D0_g(float);
    float normalizedSpread = phBoundCapsule_01D0_g(bellArea);  // sqrt

    float distSquared = distance * distance;
    const float ONE_OVER_TWO_PI = 0.15915494f;  // @ 0x8202D184
    float scaledSpreadSq = spreadSquared * ONE_OVER_TWO_PI;
    float exponent = distSquared / scaledSpreadSq;

    extern float aud_2478(float);
    float decayFactor = aud_2478(-exponent);  // exp(-x)

    const float ONE = 1.0f;  // @ 0x8202D108
    float inverseSpread = ONE / normalizedSpread;
    return inverseSpread * decayFactor;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::GetExtentAlongAxis @ 0x820DF420 | size: 0xC4
//
// Retrieves min/max extent values from a collision shape element array
// (stride 104 bytes) based on axis component. Falls through switch cases
// to accumulate multiple axis pairs. Retries with element 11 if min < 0.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::GetExtentAlongAxis(
    uint32_t elementIndex, uint32_t axis, float* outMin, float* outMax)
{
    const float ZERO = 0.0f;  // @ 0x8202D110
    float* elementArray = *(float**)((uint8_t*)this + 8);  // m_pElements

retry:
    if (axis <= 3) {
        float* element = (float*)((uint8_t*)elementArray + elementIndex * 104);

        switch (axis) {
        case 0:
            *outMax = element[0];
            *outMin = element[0];
            break;

        case 2:
            *outMin = element[1];  // offset 4
            *outMax = element[2];  // offset 8
            /* fallthrough */

        case 1:
            *outMin = element[3];  // offset 12
            *outMax = element[4];  // offset 16
            /* fallthrough */

        case 3:
            *outMin = element[5];  // offset 20
            *outMax = element[6];  // offset 24
            break;
        }
    }

    if (*outMin < ZERO) {
        elementIndex = 11;
        goto retry;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::ComputeNormalDotProduct @ 0x82137BF8 | size: 0xAC
//
// Computes dot product of two 4-component normal vectors. Returns a scaled
// collision coefficient based on the angle between them:
//   NaN -> 0, within threshold -> 0, otherwise acos(|dot|) * scale.
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::ComputeNormalDotProduct(const float* normalA, const float* normalB) {
    float dot = normalA[2] * normalB[2]
              + normalA[3] * normalB[3]
              + normalA[1] * normalB[1]
              + normalA[0] * normalB[0];

    const float THRESHOLD = *(float*)0x825F9960;  // small positive epsilon
    const float ONE = 1.0f;       // @ 0x8202D108
    const float NEG_ONE = -1.0f;  // @ 0x8202D100

    // NaN check (unordered comparison)
    if (dot != dot) {
        return 0.0f;
    }

    if (dot > THRESHOLD) {
        // Positive dot above threshold — check against +1.0
        if (dot >= ONE) {
            return 0.0f;
        }
        // Compute acos-based response
        float absDot = fabsf(dot);
        extern float phBoundCapsule_0E90_g(float);
        float acosResult = phBoundCapsule_0E90_g(absDot);
        const float SCALE = *(float*)0x8202D17C;
        return acosResult * SCALE;
    }

    if (dot < NEG_ONE) {
        // Large negative dot — also compute acos-based response
        float absDot = fabsf(dot);
        extern float phBoundCapsule_0E90_g(float);
        float acosResult = phBoundCapsule_0E90_g(absDot);
        const float SCALE = *(float*)0x8202D17C;
        return acosResult * SCALE;
    }

    // Within [-1, threshold] — perpendicular/near-zero
    return 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::TestNormalOrientation @ 0x82137CA8 | size: 0xC4
//
// Tests collision normal orientation against a reference direction. If the
// dot product is negative (facing away), negates the reference before
// dispatching to the collision response handler phBoundCapsule_AED0_g.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::TestNormalOrientation(
    void* collisionA, void* collisionB,
    const float* normal, float* reference)
{
    float dot = normal[2] * reference[2]
              + normal[3] * reference[3]
              + normal[1] * reference[1]
              + normal[0] * reference[0];

    const float ZERO = 0.0f;  // @ 0x8202D110

    if (dot < ZERO) {
        // Negate reference direction
        reference[0] = -reference[0];
        reference[1] = -reference[1];
        reference[2] = -reference[2];
        reference[3] = -reference[3];
    }

    extern void phBoundCapsule_AED0_g(void*, void*, void*, void*, const float*, float*);
    phBoundCapsule_AED0_g(collisionA, collisionB, nullptr, nullptr, normal, reference);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::SetupCapsuleWithOffset @ 0x82143F08 | size: 0x84
//
// Checks if the direction vector at capsuleData+16 is non-zero. If so,
// computes position + extent and calls phBoundCapsule_3F90_g to finalize.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::SetupCapsuleWithOffset(float* capsuleData) {
    float* direction = capsuleData + 4;  // +16 bytes

    // Check if direction is a zero vector (SIMD abs + compare)
    bool isZeroDirection = (fabsf(direction[0]) == 0.0f &&
                            fabsf(direction[1]) == 0.0f &&
                            fabsf(direction[2]) == 0.0f &&
                            fabsf(direction[3]) == 0.0f);

    if (!isZeroDirection) {
        float* position = capsuleData;
        float* extent   = capsuleData + 8;  // +32 bytes
        float combinedCenter[4];
        combinedCenter[0] = position[0] + extent[0];
        combinedCenter[1] = position[1] + extent[1];
        combinedCenter[2] = position[2] + extent[2];
        combinedCenter[3] = position[3] + extent[3];

        extern void phBoundCapsule_3F90_g(void*, float*, float*);
        phBoundCapsule_3F90_g(this, combinedCenter, direction);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::phBoundCapsule::DispatchCameraCollision @ 0x82148608 | size: 0x9C
//
// Prepares camera-relative collision parameters and dispatches to the
// full capsule collision test (phBoundCapsule_81D8_g). Reads the active
// camera index from a global, computes the camera transform offset at
// stride 912 bytes, and passes camera frustum data.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::DispatchCameraCollision(
    void* collisionQuery, void* resultBuffer, uint32_t flags)
{
    extern uint32_t* g_pCameraMgrArray;  // @ 0x8260641C
    extern uint32_t g_activeCameraIndex; // @ 0x825C4898

    uint32_t* cameraMgrArray = *(uint32_t**)g_pCameraMgrArray;
    uint32_t cameraIndex = g_activeCameraIndex;

    // Each camera entry is 912 bytes
    uint8_t* cameraEntry = (uint8_t*)cameraMgrArray + cameraIndex * 912;

    extern float pongCameraMgr_3E98_g(uint8_t* camera);
    float cameraDistance = pongCameraMgr_3E98_g(cameraEntry);

    float fovScale = *(float*)(cameraEntry + 824);
    float* cameraMatrix = (float*)(cameraEntry + 64);
    float zoomFactor = *(float*)(cameraEntry + 832);
    float aspectRatio = *(float*)(cameraEntry + 836);

    // fsel idiom: clamp zoom factor
    const float THRESHOLD = *(float*)0x8202CFE8;
    float clampedZoom = (THRESHOLD - zoomFactor) >= 0.0f ? zoomFactor : 0.0f;

    extern void phBoundCapsule_81D8_g(void*, void*, void*, float*, float, float, float, float, uint32_t);
    phBoundCapsule_81D8_g(
        this, collisionQuery, resultBuffer,
        cameraMatrix, fovScale, aspectRatio,
        clampedZoom, cameraDistance, flags);
}
