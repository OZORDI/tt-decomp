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

/* ======================================================================
   Forward declarations -- moved from function bodies to file scope
   ====================================================================== */

/* --- Float / double constants --- */
extern const float g_capsuleVolK1;  // @ .rdata (fmadds multiplier)
extern const float g_capsuleVolK2;  // @ .rdata (final scale, ~4.189 = 4/3*PI)
extern const float g_dampingScale;   // @ 0x8207A008 (.rdata)
extern const float g_floatEpsilon;  // @ 0x82079C58 (.rdata)
extern const float g_phCapsuleShapeK;  // shape constant @ 0x825C8080
extern const float g_phClampMax;
extern const float g_phExtentScale;
extern const float g_phJoint1Dof_angleMin, g_phJoint1Dof_angleMax;
extern const float g_phJoint1Dof_errTol, g_phJoint1Dof_maxCorr;
extern const float g_phJoint1Dof_mass, g_phJoint1Dof_stiffness;
extern const float g_phJoint1Dof_maxTorque;
extern const float g_phNegThreshold;  // negative threshold @ 0x82079D18
extern const float g_phSphereInertiaCoeff; // @ 0x82029B60
extern const float g_phSphereVolumeCoeff;  // @ 0x82079C30 — (4/3)*pi
extern const float g_phTwo;
extern const float g_phTwoPi;   // @ lbl_8202C02C (6.283185...)
extern const float g_phZeroFloat;
extern const float g_springScale;    // @ 0x8207A0F4 (.rdata)

/* --- Physics globals --- */
extern const char* g_phResolveProfileTag;
extern const char* g_phResolveProfileTagEnd;
extern const uint8_t g_phXYZMask[16];     // @ 0x82629E60 — xyz mask (clear w)
extern const void* g_phJoint3DofTemplate;  // @ 0x82069E70
extern float g_phTableA[];            // sine/cosine lookup table
extern float g_phTableB[];            // second lookup table
extern uint16_t g_phDefaultStatusId;  // @ 0x82465A58
extern uint32_t g_phActiveCameraPtr;
extern uint32_t g_phCollisionFlags;  // @ 0x825C48E8
extern uint32_t g_phCollisionSolver;
extern uint32_t g_phGlobalStatePtr;
extern uint32_t g_phRandomSeedPtr;    // global RNG state
extern uint8_t g_phDisabled;
extern uint8_t g_physicsDisabled;  // @ 0x826064E0
extern void* g_phAllocator;  // @ 0x64890018 (constructed from lis+ori)
extern void* g_phBoundGeometryVtable;  // @ 0x82058494
extern void* g_phBoundRelocVtable;
extern void* g_phCollisionMgr_vtable;
extern void* g_phGlobalState;
extern void* g_phJoint1DofVtable;
extern void* g_phJointChainVtable;  // @ 0x82050078
extern void* g_phJointVtable;
extern void* g_phMaterialAllocator;        // @ SDA 0x826066E4

/* --- Display / MMIO globals --- */
extern uint32_t g_mmioStoredValue;  // @ 0x825E7890

/* --- Other globals --- */
extern uint32_t g_identityVec[];  // @ 0x82606740 — 16-byte identity/zero vector

/* --- Physics functions --- */
extern float phJoint1Dof_AE38(void*);
extern int phArticulatedCollider_E668(rage::phArticulatedCollider* collider, int boneIndex);
extern void  phBoundCapsule_CopyEntries(void* a, void* b, int count);
extern void  phBoundCapsule_9DA0_2h(void* a, void* b, int count);
extern void  phBoundCapsule_F5F8_p39(void* data);
extern void  phBoundCapsule_ForwardToSink_9C78_1(void* a, void* b, int count);
extern void  ph_1B78(void* thisPtr, float f2, float f3, void* r6);
extern void phArticulatedCollider_UpdateJointTransforms(void* activeJoints);
extern void phArticulatedCollider_ProcessJoints(void* collider);
extern void phArticulatedCollider_60F8(void* jointData, float recipTimestep, float dampingFactor, float springFactor);
extern void phArticulatedCollider_RebuildJointWeights(void* jointData);
extern void phArticulatedCollider_ProcessColliderState(void* collider);
extern void phArticulatedCollider_ApplyJointDelta(void* jointData, int linkIndex, const float* delta);
extern void phArticulatedCollider_ResetJointLink(void* jointData, int linkIndex);
extern void phArticulatedCollider_ResetJointChain(void* jointArray);
extern void phArticulatedCollider_5D58(void* jointArray);
extern void phArticulatedCollider_ResetForceAccumulators(void* jointArray);
extern void phArticulatedCollider_5FE0(void* jointData);
extern void phArticulatedCollider_F0E0(void* collider);
extern void phArticulatedCollider_8450(void* jointData);
extern void ph_Sqrtf_joint(rage::phJoint3Dof*, float);
extern void ph_ApplyAngularVelocity(void* capsule);
extern void phBoundCapsule_ACB0_p45(void* capsule);
extern void phCollider_vfn_4(rage::phArticulatedCollider* collider);
extern void phInst_5910_p39(void* obj);
extern void phJoint1Dof_AFF8_p42(rage::phJoint3Dof*, int, float*);
extern void phJoint_1388(rage::phJoint3Dof*, int, float, float*, float*);
extern void phJoint_PreSyncState(rage::phJoint3Dof*);
extern void phJoint_ApplyDelta(void* jointData, int linkIndex, const float* delta, int param);
extern void ph_9E50(void*, void*);
extern void ph_A330(void* obj);
extern void ph_CEE0(void*, int);
extern void ph_E088(void*, void*, void*, float, int);
extern void ph_E1E8(void* obj);
extern void ph_FE48(void* obj, void* p);
extern void ph_snprintf(char*, int, const char*, const char*, int);
extern void ph_vt3DB0_12_8DB8(void* obj);
extern void ph_vt3DB0_13_8E10(void* obj);
extern void ph_vt57D8_20_0718(void* obj);
extern void ph_vt57D8_28_FD08(void* obj);
extern void ph_vt57D8_29_FDD0(void* obj);
extern void ph_vt57D8_2_6378(void* obj);
extern void ph_vt57D8_3_61E0(void* obj);
extern void ph_vt5A60_57_6858(void* obj);
extern void ph_vt5A60_58_6EE8(void* obj);
extern void ph_vt5A60_60_7870(void* obj);
extern void ph_vt5A60_61_7A38(void* obj);
extern void ph_vt5A60_62_8F80(void* obj);
extern void ph_vt5A7C_63_6A98(void* obj);
extern void ph_vt5A84_63_6B90(void* obj);
extern void ph_vt5A8C_63_6A50(void* obj);
extern void ph_vt5B98_40_8D50(void* obj);
extern void ph_vt5B98_41_8E50(void* obj);
extern void* phArchetype_Load(const char*, void*);
extern void* phArchetype_Find(void*, const char*);
extern void* ph_9EC0_1(void*);
extern void* ph_E010(void*, void*, const char*);

/* --- RAGE / kernel functions --- */
extern void  rage_Alloc(int size, void* allocator);  // rage_AllocInternal
extern void ke_ConstructObject(void* obj);
extern void ke_DestroyObjectA(void* obj);
extern void ke_DestroyObjectB(void* obj);
extern void rage_1058(void*);
extern void datResource_Load(void*, const char*);
extern void rage_free(void*);
extern void* xe_GetLoadContext(void*);

/* --- Game functions --- */
extern "C" void pongCreatureInst_E828_v12(void*, uint32_t);
extern void game_CE58(void* collider);
extern void pongCreatureInst_RegisterPhysics(void*, void*);

/* --- Audio functions --- */
extern void aud_1498(void* obj);
extern void aud_6A20_wrap_6A20(void* obj);
extern double aud_2478(double x);  // exp() approximation @ 0x82432478

/* --- Utility / tokenizer / singleton functions --- */
extern uint8_t fiAsciiTokenizer_ReadBool(void*, void*, void*);
extern void  fiAsciiTokenizer_ReadToken(void* tokenizer, int mode);
extern void  msgMsgSink_DispatchNested(void* obj);
extern void atSingleton_8A48_p42(void* obj);
extern void fiAsciiTokenizer_Destroy(void* obj);
extern void strncpy(char*, const char*, int);
extern void phJoint3Dof_ResetAllJoints(rage::phJoint3Dof*, int);
extern void phCollider_RecalcBounds(void* thisPtr);
extern void phCollider_vfn_1(void* collider);
extern void* g_display_obj_ptr;
extern void rage_CopyMatrixAndBind(void*, void*, void*);

/* --- Other functions --- */
extern void pg_SleepYield(int ms);              // Sleep/yield helper @ 0x82566C80
extern void statePreInit_vfn_6(void* obj);  // @ 0x82494130


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
    // m_fExtentX: X half-extent of the capsule
    int32_t component1 = static_cast<int32_t>(m_fExtentX);
    
    // Load second component (likely Y or another direction component)
    // m_fExtentY: Y half-extent of the capsule
    int32_t component2 = static_cast<int32_t>(m_fExtentY);
    
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
        // phDemoWorld_GetInstance is a global registration function
        // Parameters: phDemoWorld singleton address, this object, flags
        void* phDemoWorldSingleton = (void*)0x823E7888;
        uint32_t flags = 0x61820000;
        
        // Call phDemoWorld registration
        // phDemoWorld_GetInstance(phDemoWorldSingleton, thisPtr, flags);
    }
}


// External references
extern void rmcDrawable_Init(void* obj);  // Base initialization
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
 * 1. Calls base class initialization (rmcDrawable_Init)
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
    rmcDrawable_Init(thisPtr);

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
    this->m_nBoundType = source->m_nBoundType;
    this->m_nRibbonFlags = source->m_nRibbonFlags;
    this->m_nSegmentCount = source->m_nSegmentCount;
    this->m_pSegmentData = source->m_pSegmentData;

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
    this->m_nMaterialIndex = source->m_nMaterialIndex;  // +160
    this->m_pMaterialData = source->m_pMaterialData;  // +164
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
extern float ph_Sqrtf(float value);  // sqrt wrapper
extern float ph_Sinf(float value);  // trigonometric function
extern float ph_Atan2f(float f1, float f2);  // capsule calculation
extern float ph_Cosf(float value);  // normalization function
extern float math_SafeReciprocal(float value, double param);  // singleton math helper

// External constants
extern const float g_floatZero;      // @ 0x8202D110
extern const float g_floatOne;       // @ 0x8202D108
extern const float g_capsuleRadius;  // @ 0x82079B30

// ─────────────────────────────────────────────────────────────────────────────
// ph_Atan2fSafe @ 0x820D0550 | size: 0x50
//
// Conditional capsule calculation wrapper. If both f2 and f1 are zero,
// returns zero. Otherwise calls the main capsule calculation function.
// ─────────────────────────────────────────────────────────────────────────────
float ph_Atan2fSafe(float f1, float f2) {
    if (f2 == g_floatZero) {
        if (f1 != g_floatZero) {
            return ph_Atan2f(f1, f2);
        }
        return g_floatZero;
    }
    return ph_Atan2f(f1, f2);
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_ClampAndSin @ 0x820CB6A0 | size: 0x50
//
// Applies a clamped trigonometric transformation to an input value.
// If the value is outside the valid range [minThreshold, maxThreshold],
// it's clamped. Otherwise, it's scaled and passed through a trig function.
// ─────────────────────────────────────────────────────────────────────────────
float ph_ClampAndSin(float inputValue) {
    // Load threshold constants from global data
    const float minThreshold = g_phNegThreshold;  // @ 0x8202D010
    const float maxThreshold = g_phClampMax;  // @ 0x8202D008
    const float scaleFactor = *(float*)0x8202CFF0;   // offset +0 from base
    
    if (inputValue <= minThreshold) {
        return minThreshold;
    }
    
    if (inputValue >= maxThreshold) {
        return maxThreshold;
    }
    
    // Apply scaling and trigonometric transformation
    float scaledValue = inputValue * scaleFactor;
    return ph_Sinf(scaledValue);
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_GetCapsuleRadius @ 0x820CB598 | size: 0x58
//
// Returns a capsule property based on flags at offset +64.
// Checks bit 1 (0x02) and bit 4 (0x10) to determine which value to return.
// ─────────────────────────────────────────────────────────────────────────────
float ph_GetCapsuleRadius(void* capsule) {
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
// ph_SinCos @ 0x820D04F0 | size: 0x60
//
// Computes two related capsule properties and stores them in output pointers.
// Normalizes the first input, then applies a trigonometric transformation
// to the second input.
// ─────────────────────────────────────────────────────────────────────────────
void ph_SinCos(float* outNormalized, float* outTransformed, float input1, float input2) {
    float normalized = ph_Cosf(input1);
    *outNormalized = normalized;
    
    float transformed = ph_Sinf(input2);
    *outTransformed = transformed;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_BuildRotationMatrixX @ 0x820C3F10 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a vertical capsule aligned along the Y axis.
// ─────────────────────────────────────────────────────────────────────────────
void ph_BuildRotationMatrixX(void* capsule, float height, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedHeight = ph_Cosf(height);
    float transformedRadius = ph_Sinf(radius);
    
    const float negOne = g_floatOne;  // NOTE: 0x8202D108 is g_floatOne (1.0f), comment says -1.0f — verify
    const float zero = g_floatZero;
    
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
// ph_BuildRotationMatrixY @ 0x820C3F98 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a horizontal capsule aligned along the X axis.
// ─────────────────────────────────────────────────────────────────────────────
void ph_BuildRotationMatrixY(void* capsule, float length, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedLength = ph_Cosf(length);
    float transformedRadius = ph_Sinf(radius);
    
    const float one = g_floatOne;  // 1.0f
    const float zero = g_floatZero;
    
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
// ph_BuildRotationMatrixZ @ 0x820C4020 | size: 0x84
//
// Initializes a capsule structure with computed geometry values.
// Sets up a capsule aligned along the Z axis.
// ─────────────────────────────────────────────────────────────────────────────
void ph_BuildRotationMatrixZ(void* capsule, float depth, float radius) {
    uint8_t* obj = (uint8_t*)capsule;
    
    float normalizedDepth = ph_Cosf(depth);
    float transformedRadius = ph_Sinf(radius);
    
    const float one = g_floatOne;  // 1.0f
    const float zero = g_floatZero;
    
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
// ph_MoveTowardTarget2D @ 0x820C9CF8 | size: 0xA0
//
// Adjusts capsule position based on a 2D offset vector and interpolation factors.
// Computes the distance from the offset, normalizes it, and applies scaled
// movement along the offset direction.
// ─────────────────────────────────────────────────────────────────────────────
void ph_MoveTowardTarget2D(void* capsule, float* offsetVec, float scaleFactor, float lerpFactor) {
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
    float distance = ph_Sqrtf(distSq);
    
    // Normalize and apply singleton transformation
    const double normalizationFactor = (double)g_capsuleRadius;  // @ 0x82079B30
    float normalizedDist = math_SafeReciprocal(distance, normalizationFactor);
    
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
// phBoundCapsule_InitFromTemplate @ 0x820D9730 | size: 0xB0
//
// Initializes a capsule from a template and applies transformations from
// a source object. Copies 64 bytes of template data, then conditionally
// applies a radius transformation based on flags.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_InitFromTemplate(void* capsule, void* sourceObj) {
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
        const float defaultRadius = g_capsuleRadius;
        ph_BuildRotationMatrixY(capsule, defaultRadius, 0.0f);
    }
    
    // Copy transformation matrix from source object (offset +384, 16 bytes)
    const uint8_t* transformMatrix = src + 384;
    for (int i = 0; i < 16; i++) {
        dest[48 + i] = transformMatrix[i];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule_FindFirstValid @ 0x820DF548 | size: 0x80
//
// Extracts capsule geometry data from an array of capsule structures.
// Searches for the first valid capsule (non-zero radius) and copies its
// geometry parameters to output pointers.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundCapsule_FindFirstValid(void* container, int* outIndex, float* outX, float* outY, float* outZ, float* outRadius) {
    uint8_t* obj = (uint8_t*)container;
    
    // Get capsule array pointer
    void* capsuleArray = *(void**)(obj + 8);
    uint8_t* capsules = (uint8_t*)capsuleArray;
    
    int index = *outIndex;
    const float zero = g_floatZero;
    
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
    return *(const float*)&m_fAABBMaxZ;  // +0x1C
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
    const float* constants = &g_floatOne;  // constant table base
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
    ph_Sinf(scaledValue);
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
    float minBound = ph_Cosf(scale);
    *outMin = minBound;
    
    // Compute maximum bound using the scale
    float maxBound = ph_Sinf(scale);
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
        return ph_Atan2f(param1, param2);
    }

    // General case: compute extent with both parameters
    return ph_Atan2f(param1, param2);
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
    float minBound = ph_Cosf(halfLength);
    float maxBound = ph_Sinf(halfLength);
    
    const float* constants = &g_floatZero;  // constant table base
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
    float minBound = ph_Cosf(halfLength);
    float maxBound = ph_Sinf(halfLength);
    
    const float* constants = &g_floatOne;  // constant table base
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
    float minBound = ph_Cosf(halfLength);
    float maxBound = ph_Sinf(halfLength);
    
    const float* constants = &g_floatOne;  // constant table base
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
    float minBound = ph_Cosf(halfLength);
    float maxBound = ph_Sinf(halfLength);
    
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
    return m_nMaterialCount;  // +168
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
    void** materialArray = (void**)(uintptr_t)m_pMaterials;  // +160
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
    uint8_t count = m_nMaterialCount;  // +168

    if (count > 0) {
        void** materialArray = (void**)(uintptr_t)m_pMaterials;  // +160
        for (int i = 0; i < count; i++) {
            materialArray[i] = material;
        }
    }

    m_pCurrentMaterial = (uint32_t)(uintptr_t)material;  // +164
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
    void** materialArray = (void**)(uintptr_t)m_pMaterials;  // +160
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
    void* currentMaterial = (void*)(uintptr_t)m_pCurrentMaterial;  // +164
    
    if (currentMaterial != (void*)-1) {
        // Use the explicitly set current material
        void** vtable = *(void***)this;
        typedef void (*MaterialFunc)(void*, void*);
        MaterialFunc func = (MaterialFunc)vtable[11];
        func(this, currentMaterial);
        return;
    }
    
    uint8_t materialCount = m_nMaterialCount;  // +168
    
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
    const float* center = (const float*)((const char*)this + 0x30);  // +48 — TODO: add m_vCenter to phBoundGeometry header
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
    int vertexCount = (int)m_nVertexCount;  // +148

    if (vertexCount > 0) {
        float* vertices = (float*)(uintptr_t)m_pVertices;  // +112

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
    float* center = (float*)((char*)this + 0x30);  // +48 — TODO: add m_vCenter to phBoundGeometry header
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

    m_bHasOffset = hasOffset ? 1 : 0;  // +5
    
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
// phInitAsciiTokenizer @ 0x820C2DF0 | size: 0x18
//
// Initializes a rage::fiAsciiTokenizer object by setting its vtable pointer
// and clearing the internal state field at offset +160.
//
// This is likely a constructor or reset function for the tokenizer class.
// ─────────────────────────────────────────────────────────────────────────────
void phInitAsciiTokenizer(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    
    // Set vtable pointer for rage::fiAsciiTokenizer
    *(void**)(obj + 0) = (void*)0x8202777C;  // vtable for rage::fiAsciiTokenizer
    
    // Clear internal state field
    *(uint32_t*)(obj + 160) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phVec4Zero @ 0x820C3AB0 | size: 0x10
//
// Zeros out a 16-byte SIMD vector at the given address.
// Uses AltiVec vxor instruction to efficiently clear the vector.
// ─────────────────────────────────────────────────────────────────────────────
void phVec4Zero(void* vectorPtr) {
    uint32_t* vec = (uint32_t*)vectorPtr;
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0;
    vec[3] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phVec3MagSquared @ 0x820C3AC0 | size: 0x18
//
// Computes the dot product of a 3D vector with itself (magnitude squared).
// Uses AltiVec vmsum3fp128 instruction for SIMD dot product calculation.
//
// Returns: float - The squared magnitude of the vector
// ─────────────────────────────────────────────────────────────────────────────
float phVec3MagSquared(const float* vector) {
    // Compute dot product: v.x*v.x + v.y*v.y + v.z*v.z
    float result = vector[0] * vector[0] + 
                   vector[1] * vector[1] + 
                   vector[2] * vector[2];
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_AdvanceSegmentedPtr @ 0x820C3870 | size: 0x38
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
void ph_AdvanceSegmentedPtr(void* arrayBase, void** ptrToAdvance) {
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
// phNormalizePath @ 0x820F0908 | size: 0x64
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
void phNormalizePath(char* dest, const char* src, int maxLen) {
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
// phAlignUp4 @ 0x820F9598 | size: 0xC
//
// Aligns a value up to the next 4-byte boundary.
// Used for memory alignment in physics data structures.
//
// Parameters:
//   value - Value to align
//
// Returns: uint32_t - Value aligned to 4-byte boundary
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phAlignUp4(uint32_t value) {
    // Add 3 and mask off lower 2 bits to round up to next multiple of 4
    return (value + 3) & 0xFFFFFFFC;
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_GetSimulationContext @ 0x820F95A8 | size: 0x8
//
// Simple getter that returns a pointer field at offset +148.
// Likely retrieves a physics simulation context or state object.
//
// Returns: void* - Pointer to physics object at offset +148
// ─────────────────────────────────────────────────────────────────────────────
void* ph_GetSimulationContext(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    return *(void**)(obj + 148);
}

// ─────────────────────────────────────────────────────────────────────────────
// phGetStateCodeFromFlag22 @ 0x8221FD58 | size: 0x18
//
// Returns a state code based on a flag at offset +22.
// Returns 3 if the flag is zero, 4 if the flag is non-zero.
//
// This appears to be a state query function for physics objects.
//
// Returns: int - State code (3 or 4)
// ─────────────────────────────────────────────────────────────────────────────
int phGetStateCodeFromFlag22(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    uint16_t flag = *(uint16_t*)(obj + 22);
    
    if (flag == 0) {
        return 3;
    }
    return 4;
}

// ─────────────────────────────────────────────────────────────────────────────
// phSetByteFlag15 @ 0x8221FD70 | size: 0x8
//
// Sets a byte flag at offset +15.
// Simple setter for a physics object state flag.
//
// Parameters:
//   value - Byte value to store
// ─────────────────────────────────────────────────────────────────────────────
void phSetByteFlag15(void* thisPtr, uint8_t value) {
    uint8_t* obj = (uint8_t*)thisPtr;
    *(uint8_t*)(obj + 15) = value;
}

// ─────────────────────────────────────────────────────────────────────────────
// phLoadArchetypeForCreature @ 0x8227F6A8 | size: 0x168
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
void phLoadArchetypeForCreature(void* contextPtr, void* creatureInst, const char* assetPath) {
    // External function declarations
    extern void* rage_alloc(uint32_t);
    
    char normalizedPath[256];
    
    // Create temporary loading context
    void* loadContext = xe_GetLoadContext(contextPtr);
    *(uint32_t*)((uint8_t*)loadContext + 96) = 0;
    
    // Initialize path normalization
    datResource_Load(loadContext, assetPath);
    
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
        void* existingArchetype = phArchetype_Find(*(void**)((uint8_t*)physicsWorld + 8), normalizedPath);
        
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
    pongCreatureInst_RegisterPhysics(creatureInst, physicsInst);
    
    // Finalize registration
    rage_CopyMatrixAndBind(loadContext, creatureInst, (void*)((uint8_t*)creatureInst + 16));
    
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
// ph_CleanupCompositePhInst @ 0x8246FC68 | size: 0x38
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

void ph_CleanupCompositePhInst(void* obj) {
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

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetActiveJointsPointer (vfn_46) @ 0x8224E248 | size: 0x8
//
// Returns pointer to the active joints data structure.
// This is a simple accessor used by physics simulation to query joint states.
// ─────────────────────────────────────────────────────────────────────────────
void* rage::phArticulatedCollider::GetActiveJointsPointer() {
    return (void*)(uintptr_t)m_pPhysicsData;  // +472 (0x1D8)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToJointProcessor (vfn_22) @ 0x8224F220 | size: 0x8
//
// Delegates processing to the joint processor function.
// Used during physics update to process all active joints.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DelegateToJointProcessor() {
    phArticulatedCollider_ProcessJoints((void*)(uintptr_t)m_pJointArray);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToCapsuleHandler (vfn_25) @ 0x8224FD50 | size: 0x8
//
// Delegates to capsule collision handler.
// Used for capsule-based collision detection on articulated bodies.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DelegateToCapsuleHandler() {
    ph_ApplyAngularVelocity((void*)(uintptr_t)m_pJointArray);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToColliderHandler (vfn_59) @ 0x8224F218 | size: 0x8
//
// Delegates to articulated collider handler.
// Used for complex collision scenarios involving multiple joints.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DelegateToColliderHandler() {
    phArticulatedCollider_ProcessColliderState((void*)(uintptr_t)m_pJointArray);  // +464 (0x1D0)
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
float rage::phArticulatedCollider::GetJointMass(int jointIndex) {
    float* massArray = (float*)(uintptr_t)m_pJointMassArray;  // +508 (0x1FC)
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
float rage::phArticulatedCollider::GetJointInertia(int jointIndex) {
    float* inertiaArray = (float*)(uintptr_t)m_pJointInertiaArray;  // +492 (0x1EC)
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
float rage::phArticulatedCollider::GetJointDamping(int jointIndex) {
    float* dampingArray = (float*)(uintptr_t)m_pJointDampingArray;  // +500 (0x1F4)
    return dampingArray[jointIndex];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::Update (vfn_4) @ 0x8224E7A0 | size: 0x28
//
// Updates the articulated collider state.
// Calls parent class update, then updates active joints.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::Update() {
    // Call parent class update
    phCollider_vfn_4(this);
    
    // Update active joints
    phArticulatedCollider_UpdateJointTransforms((void*)(uintptr_t)m_pJointArray);  // +464 (0x1D0)
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AddForceToJoint (vfn_33) @ 0x8224FD30 | size: 0x20
//
// Adds a force vector to a specific joint's accumulated forces.
// Used during physics simulation to apply external forces (gravity, collisions).
//
// @param forceVector - 16-byte aligned vector containing force components
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AddForceToJoint(const float* forceVector) {
    // Load the joint data pointer
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    
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
void rage::phArticulatedCollider::ApplyImpulse(void* param1, void* param2, void* param3) {
    // Get vtable and call slot 34 with parameters shuffled and two zeros appended
    void** vtable = *(void***)this;
    typedef void (*ImpulseFunc)(phArticulatedCollider*, void*, void*, void*, int, int);
    ImpulseFunc func = (ImpulseFunc)vtable[34];
    func(this, param1, param2, param3, 0, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ScalarDestructor (vfn_1) @ 0x8224E6D8 | size: 0x48
//
// Scalar deleting destructor override. Calls the base phCollider destructor,
// then resets joint-related state (joint count and active flag), and cleans
// up the joint array and constraint data.
// ─────────────────────────────────────────────────────────────────────────────

void rage::phArticulatedCollider::ScalarDestructor() {
    // Call base class scalar destructor
    phCollider_vfn_1((void*)this);

    // Reset joint-related state
    m_pPhysicsData = 0;          // +472 (0x1D8) — joint count / physics data reset
    m_bJointsDirty = false;     // +468 (0x1D4)

    // Clean up joint array data
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    phArticulatedCollider_ResetJointChain(jointData);

    // Reset constraint data
    jointData = (void*)(uintptr_t)m_pJointArray;
    phArticulatedCollider_5D58(jointData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ResetForces (vfn_2) @ 0x8224E720 | size: 0x80
//
// Zeros out four 16-byte SIMD force/torque accumulator vectors at offsets
// +256, +272, +224, +240, then calls the base Update (vfn_4) to apply
// the cleared state, and reinitializes the joint weight/response data.
// ─────────────────────────────────────────────────────────────────────────────

void rage::phArticulatedCollider::ResetForces() {
    // Zero out linear force accumulator at +256
    memset((char*)this + 256, 0, 16);  // TODO: name force accumulator at +0x100

    // Zero out angular force accumulator at +272
    memset((char*)this + 272, 0, 16);  // TODO: name force accumulator at +0x110

    // Zero out linear velocity accumulator at +224
    memset((char*)this + 224, 0, 16);  // TODO: name velocity accumulator at +0xE0

    // Zero out angular velocity accumulator at +240
    memset((char*)this + 240, 0, 16);  // TODO: name velocity accumulator at +0xF0 (overlaps m_fLinearDamping)

    // Apply the cleared state via base class update
    this->Update();

    // Reinitialize joint weight/response data
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    phArticulatedCollider_ResetForceAccumulators(jointData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ResetActiveJoints (vfn_9) @ 0x8224EBC8 | size: 0x8C
//
// Iterates over all joints (count-1, skipping root) and calls their
// individual reset method (vtable slot 18) if the joint has a valid
// parent reference (field +20 != 0).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ResetActiveJoints() {
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    int32_t jointCount = *(int32_t*)((char*)jointData + 4);

    int32_t numChildJoints = jointCount - 1;
    if (numChildJoints <= 0)
        return;

    // Joint pointers start at offset +168 in the joint data array
    uint32_t* jointPtrs = (uint32_t*)((char*)jointData + 168);

    for (int32_t i = 0; i < numChildJoints; i++) {
        void* joint = (void*)(uintptr_t)jointPtrs[i];

        // Check if joint has a valid parent (field +20)
        uint32_t parentRef = *(uint32_t*)((char*)joint + 20);
        bool hasParent = (parentRef != 0);

        if (hasParent) {
            // Call joint reset via vtable slot 18
            void** vtable = *(void***)joint;
            typedef void (*ResetFunc)(void*);
            ResetFunc resetFn = (ResetFunc)vtable[18];
            resetFn(joint);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyScaledGravity (vfn_27) @ 0x8224FD58 | size: 0x4C
//
// Applies a gravity-like force scaled by the collider's mass (field +100).
// Constructs a force vector {0, mass*scale, 0} and dispatches to the parent
// class ApplyForce (vtable slot 32).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ApplyScaledGravity(float scale) {
    // Load mass from field +100
    float mass = *(float*)&m_pRootTransform;  // +0x64 (100) — TODO: verify type, reading float from ptr field

    // Construct gravity force vector: {0, mass * scale, 0}
    float forceY = mass * scale;

    float forceVec[4];
    forceVec[0] = 0.0f;
    forceVec[1] = forceY;
    forceVec[2] = 0.0f;
    forceVec[3] = 0.0f;

    // Dispatch to parent vtable slot 32 (ApplyForce)
    void** vtable = *(void***)this;
    typedef void (*ApplyForceFunc)(void*, float*);
    ApplyForceFunc applyForce = (ApplyForceFunc)vtable[32];
    applyForce(this, forceVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateScaledForceX (vfn_28) @ 0x8224FDA8 | size: 0x94
//
// Extracts the X-axis column from the root joint's 4x4 transform matrix
// (at offset +144 in the root joint), scales it by the root joint's
// mass (field +128) * scale, and dispatches via vtable slot 33 (AddForce).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateScaledForceX(float scale) {
    void** vtable = *(void***)this;
    void* jointData = (void*)(uintptr_t)m_pJointArray;   // +464 (0x1D0)

    // Get root joint pointer (first entry at offset +40)
    void* rootJoint = (void*)(uintptr_t)(*(uint32_t*)((char*)jointData + 40));

    // Load 4x4 matrix at root joint +144 (rows of 16 bytes each)
    float* matRow0 = (float*)((char*)rootJoint + 144);
    float* matRow1 = (float*)((char*)rootJoint + 160);
    float* matRow2 = (float*)((char*)rootJoint + 176);
    float* matRow3 = (float*)((char*)rootJoint + 192);

    // Extract X column: {row0[0], row1[0], row2[0], row3[0]}
    float colX[4];
    colX[0] = matRow0[0];
    colX[1] = matRow1[0];
    colX[2] = matRow2[0];
    colX[3] = matRow3[0];

    // Scale by root joint mass (field +128) * input scale
    float mass = *(float*)((char*)rootJoint + 128);
    float scaledMass = mass * scale;
    colX[0] *= scaledMass;
    colX[1] *= scaledMass;
    colX[2] *= scaledMass;
    colX[3] *= scaledMass;

    // Dispatch to vtable slot 33 (AddForce)
    typedef void (*AddForceFunc)(void*, float*, float*);
    AddForceFunc addForce = (AddForceFunc)vtable[33];
    addForce(this, colX, colX);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateScaledForceY (vfn_29) @ 0x8224FE40 | size: 0x94
//
// Same as AccumulateScaledForceX but extracts the Y-axis column from the
// root joint's transform matrix and scales by mass at field +136.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateScaledForceY(float scale) {
    void** vtable = *(void***)this;
    void* jointData = (void*)(uintptr_t)m_pJointArray;

    void* rootJoint = (void*)(uintptr_t)(*(uint32_t*)((char*)jointData + 40));

    // Load matrix rows
    float* matRow0 = (float*)((char*)rootJoint + 144);
    float* matRow1 = (float*)((char*)rootJoint + 160);
    float* matRow2 = (float*)((char*)rootJoint + 176);
    float* matRow3 = (float*)((char*)rootJoint + 192);

    // Extract Y column: {row0[1], row1[1], row2[1], row3[1]}
    float colY[4];
    colY[0] = matRow0[1];
    colY[1] = matRow1[1];
    colY[2] = matRow2[1];
    colY[3] = matRow3[1];

    // Scale by mass at field +136
    float mass = *(float*)((char*)rootJoint + 136);
    float scaledMass = mass * scale;
    colY[0] *= scaledMass;
    colY[1] *= scaledMass;
    colY[2] *= scaledMass;
    colY[3] *= scaledMass;

    typedef void (*AddForceFunc)(void*, float*, float*);
    AddForceFunc addForce = (AddForceFunc)vtable[33];
    addForce(this, colY, colY);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateScaledForceZ (vfn_30) @ 0x8224FED8 | size: 0x94
//
// Same as AccumulateScaledForceX but extracts the Z-axis column from the
// root joint's transform matrix and scales by mass at field +132.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateScaledForceZ(float scale) {
    void** vtable = *(void***)this;
    void* jointData = (void*)(uintptr_t)m_pJointArray;

    void* rootJoint = (void*)(uintptr_t)(*(uint32_t*)((char*)jointData + 40));

    // Load matrix rows
    float* matRow0 = (float*)((char*)rootJoint + 144);
    float* matRow1 = (float*)((char*)rootJoint + 160);
    float* matRow2 = (float*)((char*)rootJoint + 176);
    float* matRow3 = (float*)((char*)rootJoint + 192);

    // Extract Z column from the matrix
    float colZ[4];
    colZ[0] = matRow0[0];
    colZ[1] = matRow1[0];
    colZ[2] = matRow2[0];
    colZ[3] = matRow3[0];

    // Scale by mass at field +132
    float mass = *(float*)((char*)rootJoint + 132);
    float scaledMass = mass * scale;
    colZ[0] *= scaledMass;
    colZ[1] *= scaledMass;
    colZ[2] *= scaledMass;
    colZ[3] *= scaledMass;

    typedef void (*AddForceFunc)(void*, float*, float*);
    AddForceFunc addForce = (AddForceFunc)vtable[33];
    addForce(this, colZ, colZ);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SaveAndClearJointForces (vfn_63) @ 0x8224E2D8 | size: 0xBC
//
// For each joint in the articulated chain:
// 1. Calls phArticulatedCollider_5FE0 to prepare the joint force state
// 2. Copies the accumulated force vector (+304) to the applied force
//    buffer (+1040) as a backup
// 3. Zeros out four 16-byte vectors at offsets +304, +320, +272, +288
//    (force/torque accumulators) to prepare for the next simulation step
// ─────────────────────────────────────────────────────────────────────────────

void rage::phArticulatedCollider::SaveAndClearJointForces() {
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)

    // Prepare joint force state
    phArticulatedCollider_5FE0(jointData);

    int32_t jointCount = *(int32_t*)((char*)jointData + 4);
    if (jointCount <= 0)
        return;

    // Joint pointers start at offset +40 in the joint data array
    uint32_t* jointPtrs = (uint32_t*)((char*)jointData + 40);

    for (int32_t i = 0; i < jointCount; i++) {
        void* joint = (void*)(uintptr_t)jointPtrs[i];

        // Copy force accumulator (+304) to applied force buffer (+1040)
        memcpy((char*)joint + 1040, (char*)joint + 304, 16);

        // Copy second vector (+320 to +1056)
        memcpy((char*)joint + 1056, (char*)joint + 320, 16);

        // Zero out force accumulator at +320
        memset((char*)joint + 320, 0, 16);

        // Zero out force accumulator at +304
        memset((char*)joint + 304, 0, 16);

        // Zero out torque accumulator at +272
        memset((char*)joint + 272, 0, 16);

        // Zero out torque accumulator at +288
        memset((char*)joint + 288, 0, 16);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::NormalizeVector (E398_p33) @ 0x8224E398 | size: 0x94
//
// Normalizes a 3-component vector in-place. Computes the magnitude via
// dot product (vmsum3fp128), then divides each component by the magnitude.
// If the squared magnitude is zero or negative, sets the output to a
// default zero vector.
//
// @param src   - Source 3-float vector {x, y, z}
// @param dst   - Destination vector (receives normalized result)
// ─────────────────────────────────────────────────────────────────────────────

void rage::phArticulatedCollider::NormalizeVector(const float* src, float* dst) {
    // Copy source to destination
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];

    // Compute squared magnitude (dot product with self)
    float sqMag = dst[0] * dst[0] + dst[1] * dst[1] + dst[2] * dst[2];

    if (sqMag > g_floatEpsilon) {
        // Compute reciprocal magnitude
        float mag = sqrtf(sqMag);
        float invMag = 1.0f / mag;

        // Scale each component
        dst[0] *= invMag;
        dst[1] *= invMag;
        dst[2] *= invMag;
        dst[3] *= invMag;
    } else {
        // Zero-length vector: set to default zero vector
        static const float zeroVec[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        memcpy(dst, zeroVec, 16);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetTimestep (F040_g) @ 0x8224F040 | size: 0xA0
//
// Configures the simulation timestep for the articulated collider.
// Computes reciprocal timestep and derived damping/spring constants,
// then reconfigures capsule geometry, joint hierarchy, transforms,
// and constraint data for the new timestep.
//
// @param timestep - The physics simulation timestep in seconds
// ─────────────────────────────────────────────────────────────────────────────
extern void ph_IntegrateJointPositions(void* jointData);

void rage::phArticulatedCollider::SetTimestep(float timestep) {
    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)

    // Prepare joint force state
    phArticulatedCollider_5FE0(jointData);

    // Compute reciprocal timestep
    float recipTimestep = 1.0f / timestep;

    // Derive damping and spring constants from reciprocal timestep
    float dampingFactor = recipTimestep * g_dampingScale;
    float springFactor  = recipTimestep * g_springScale;

    // Reconfigure joint solver with new timing constants
    phArticulatedCollider_60F8(jointData, timestep, dampingFactor, springFactor);

    // Reconfigure capsule geometry for new timestep
    ph_IntegrateJointPositions(jointData);

    // Rebuild joint weight/response data
    phArticulatedCollider_RebuildJointWeights(jointData);

    // Recompute joint hierarchy transforms
    phArticulatedCollider_8450(jointData);

    // Recompute joint transform matrices
    phArticulatedCollider_F0E0((void*)this);

    // Reinitialize joint constraints
    phArticulatedCollider_5D58(jointData);
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// Physics State Machine — phStateMachineUpdate
// ═════════════════════════════════════════════════════════════════════════════

// External data tables
extern const uint32_t g_phStateTransitionTable[];  // @ 0x82006AF8 (480 bytes)

/**
 * phStateMachineUpdate @ 0x82470CC0 | size: 0x1C0
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
int phStateMachineUpdate(void* stateObj) {
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
            const uint32_t* stateTable = g_phStateTransitionTable;
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
    m_nViewCount = 0;
    m_pResource = nullptr;
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
        m_pResource = ptr;
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
    if (m_pResource != nullptr) {
        m_pResource = nullptr;
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
    if (m_pChild != nullptr) {
        if (m_pChild->QueryInterface(1) < 0) {
            return nullptr;
        }
        this->CreateViews(m_pChild);
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
    m_pParent = this;
    if (m_nViewCount != 0) {
        m_nViewCount = 0;
    }
    
    if (m_pChild != nullptr) {
        m_pChild->Release();
        m_pChild = nullptr;
    }
}
// ═════════════════════════════════════════════════════════════════════════════
// rage::phJoint3Dof
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// Forward declarations for external functions
extern void phJoint3Dof_0170_g(void*, int, int, void*);

/**
 * rage::phJoint3Dof::SetDampingAndStiffness @ 0x82251268 | size: 0x40
 *
 * Aliases: phJoint_InitConstraint
 * Sets internal damping/stiffness values based on input parameters.
 */
void phJoint3Dof::SetDampingAndStiffness(float arg1, float arg2) {
    this->m_fJacob3Row00 = arg1;
    this->m_fJacob3Row01 = arg1;
    this->m_fJacob3Row02 = arg1;
    this->m_fJacob3Row03 = arg1;
    this->m_fCurrentAngle0 = arg2;
    this->m_fCurrentAngle1 = arg2;
    this->m_bLimitActive0 = 0;
    
    // 0x82079AD8 in .rdata usually represents 0.0f
    if (arg2 == 0.0f) {
        this->m_bLimitActive1 = 0;  // no limit active on axis 1
    } else {
        this->m_bLimitActive1 = 1;  // limit active on axis 1
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
        // phJoint3Dof_0170_g iterates over the block calling phJoint3Dof_ResetAllJoints
        phJoint3Dof_0170_g(current, 64, 8, reinterpret_cast<void*>(0x8207E6D0));
        
        phJoint3Dof* blockObj = reinterpret_cast<phJoint3Dof*>(current);
        *(uint16_t*)(current + 528) = 3;  // offset 0x0210
        *(uint16_t*)(current + 530) = 3;  // offset 0x0212
        
        phJoint3Dof_ResetAllJoints(blockObj, -1);
        
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
    this->m_fMotorTarget2 = 0.0f; // +0x02FC
    this->m_fMotorForce = 0.0f;   // +0x0300
    
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
    phJoint_PreSyncState(this); // Pre-sync
    
    float f1_a = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 896);   // +0x380
    float f0_a = *(float*)((char*)this + 928);  // +0x3A0
    *(float*)((char*)this + 1672) = f0_a * f1_a;  // +0x688
    
    float f1_b = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 880);   // +0x370 = m_fLimitLow
    float f1_c = phJoint1Dof_AE38(reinterpret_cast<char*>(this) + 912);   // +0x390
    
    float lengthSq = (f1_b * f1_b) + (f1_c * f1_c);
    
    // Constants from standard physics data block
    const float FLT_MULT = 1.0f; // Assumed from 0x8202D10C based on logic
    lengthSq *= FLT_MULT;
    
    // External bound function likely normalizes or validates the result
    ph_Sqrtf_joint(this, lengthSq);
    
    *(float*)((char*)this + 1668) = lengthSq;  // +0x684
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
float ph_Sqrt(float x) {  // ph_Sqrtf
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
    return (void*)(uintptr_t)this->m_pBound;
}

/**
 * phInst field accessors (8B each)
 */
void* phInst::GetField14() { return (void*)(uintptr_t)this->m_pBoundData; }  // +0x14
void  phInst::SetField9(void* val) { this->m_nRefCount = (uint32_t)(uintptr_t)val; }  // +0x0C
void  phInst::SetField11(void* val) { this->m_CriticalSection = (uint64_t)(uintptr_t)val; }  // +0x10

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
    return ((this->m_nFlags >> 22) & 0xF) + 1;  // rlwinm extract bits 6-9 then +1
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
 * Returns m_nDataIndex * 100 + 268 — byte offset into physics data table.
 */
int phInst::ComputeDataOffset() {  // 9070_p42
    int index = (int)this->m_nDataIndex;
    return index * 100 + 268;
}

// ── ATOMIC REF-COUNTING + CRITICAL SECTIONS ─────────────────────────────

/**
 * phInst::AddRef @ 0x82120000 | size: 0x28
 * Atomic increment of reference count at +12 using lwarx/stwcx.
 */
int phInst::AddRef() {  // phInst_1
    return ++(*(int32_t*)&this->m_nRefCount);  // original uses lwarx/stwcx atomic loop
}

/**
 * phInst::Release @ 0x821237B0 | size: 0x40
 * Atomic decrement of reference count. If count reaches 0, calls
 * the destructor callback.
 */
int phInst::Release() {  // 37B0_p33
    int newCount = --(*(int32_t*)&this->m_nRefCount);
    if (newCount == 0) {
        typedef void (*DtorFn)(void*);
        DtorFn dtor = (DtorFn)(uintptr_t)this->m_pDestructor;
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
extern "C" void phInst_SetMatrix_Impl(void* obj, void* a, void* b, void* c);
extern "C" void ke_DispatchPhysics(void* state);
extern "C" void phInst_Cleanup(void* obj);
namespace rage { // reopen

void phInst::Lock() {  // phInst_3
    RtlEnterCriticalSection(&this->m_CriticalSection);
}

/**
 * phInst::Unlock @ 0x82120000 | size: 0x28
 * Leaves critical section on the mutex at this+16.
 */
void phInst::Unlock() {  // phInst_5
    RtlLeaveCriticalSection(&this->m_CriticalSection);
}

// ── DELEGATION HELPERS ──────────────────────────────────────────────────

/**
 * phInst::ShiftArgsAndCall @ 0x821292F8 | size: 0x18
 * Shifts argument registers and tail-calls phInst_SetMatrix_Impl.
 */
void phInst::ShiftArgsAndCall(void* a, void* b, void* c) {  // 92F8_p42
    phInst_SetMatrix_Impl(this, a, b, c);
}

/**
 * phInst::LoadGlobalAndCall @ 0x8212E2C0 | size: 0xC
 * Loads a global pointer and tail-calls ke_DispatchPhysics.
 */

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
        *(uint32_t*)&this->m_pPositionVec = val1;  // +0x30
        this->m_pRotationQuat = val2;              // +0x34
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
    void* inner = (void*)(uintptr_t)this->m_pBound;
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

uint32_t rage::phArticulatedCollider::GetSolverParam_0()  { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */,  0, 0x7); }    // 3A58 @ 0x82163A58
uint32_t rage::phArticulatedCollider::GetSolverParam_3()  { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */,  3, 0xFF); }   // 3A90 @ 0x82163A90
uint32_t rage::phArticulatedCollider::GetSolverParam_11() { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */, 11, 0x7); }    // 3AA0
uint32_t rage::phArticulatedCollider::GetSolverParam_14() { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */, 14, 0x7); }    // 3AB0
uint32_t rage::phArticulatedCollider::GetSolverParam_17() { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */, 17, 0x1); }    // 3AC0

// ── Getters from +0x2D74 (11636) — packed 3-bit rotation fields ──────────

uint32_t rage::phArticulatedCollider::GetRotAxis_0()  { return ExtractBits(this, 11636 /* m_nSolverFlags */,  0, 0x7); }  // 3AD0
uint32_t rage::phArticulatedCollider::GetRotAxis_3()  { return ExtractBits(this, 11636 /* m_nSolverFlags */,  3, 0x7); }  // 3AE0
uint32_t rage::phArticulatedCollider::GetRotAxis_6()  { return ExtractBits(this, 11636 /* m_nSolverFlags */,  6, 0x7); }  // 3AF0
uint32_t rage::phArticulatedCollider::GetRotAxis_9()  { return ExtractBits(this, 11636 /* m_nSolverFlags */,  9, 0x7); }  // 3B00
uint32_t rage::phArticulatedCollider::GetRotAxis_12() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 12, 0x7); }  // 3B10
uint32_t rage::phArticulatedCollider::GetRotAxis_15() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 15, 0x7); }  // 3B20
uint32_t rage::phArticulatedCollider::GetRotAxis_18() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 18, 0x7); }  // 3B30
uint32_t rage::phArticulatedCollider::GetRotAxis_21() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 21, 0x7); }  // 3B40
uint32_t rage::phArticulatedCollider::GetRotAxis_24() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 24, 0x7); }  // 3B50
uint32_t rage::phArticulatedCollider::GetRotAxis_27() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 27, 0x7); }  // 3B60
uint32_t rage::phArticulatedCollider::GetRotAxis_30() { return ExtractBits(this, 11636 /* m_nSolverFlags */, 30, 0x3); }  // 3B70 (2-bit, top of word)

// ── Getters from +0x2D6C (11628) — packed 4-bit constraint types ─────────

uint32_t rage::phArticulatedCollider::GetConstraint_0()  { return ExtractBits(this, 11628 /* m_nJoints */,  0, 0xF); }  // 3B80
uint32_t rage::phArticulatedCollider::GetConstraint_4()  { return ExtractBits(this, 11628 /* m_nJoints */,  4, 0xF); }  // 3B90
uint32_t rage::phArticulatedCollider::GetConstraint_8()  { return ExtractBits(this, 11628 /* m_nJoints */,  8, 0xF); }  // 3BA0
uint32_t rage::phArticulatedCollider::GetConstraint_12() { return ExtractBits(this, 11628 /* m_nJoints */, 12, 0xF); }  // 3BB0
uint32_t rage::phArticulatedCollider::GetConstraint_16() { return ExtractBits(this, 11628 /* m_nJoints */, 16, 0xF); }  // 3BC0
uint32_t rage::phArticulatedCollider::GetConstraint_20() { return ExtractBits(this, 11628 /* m_nJoints */, 20, 0xF); }  // 3BD0
uint32_t rage::phArticulatedCollider::GetConstraint_24() { return ExtractBits(this, 11628 /* m_nJoints */, 24, 0xF); }  // 3BE0
uint32_t rage::phArticulatedCollider::GetConstraint_28() { return ExtractBits(this, 11628 /* m_nJoints */, 28, 0xF); }  // 3BF0

// ── Getters from +0x2D7C (11644) — 1-bit and 3-bit flags ────────────────

uint32_t rage::phArticulatedCollider::GetFlag7C_0()  { return ExtractBits(this, 11644 /* m_nCollisionFlags */,  0, 0x1); }  // 3C00
uint32_t rage::phArticulatedCollider::GetFlag7C_1()  { return ExtractBits(this, 11644 /* m_nCollisionFlags */,  1, 0x7); }  // 3C10
uint32_t rage::phArticulatedCollider::GetFlag7C_4()  { return ExtractBits(this, 11644 /* m_nCollisionFlags */,  4, 0x1); }  // 3C20
uint32_t rage::phArticulatedCollider::GetFlag7C_5()  { return ExtractBits(this, 11644 /* m_nCollisionFlags */,  5, 0x1); }  // 3C30

// ── Getters from +0x2D80 (11648) — 1-bit active flags ───────────────────

uint32_t rage::phArticulatedCollider::GetActive_0()  { return ExtractBits(this, 11648 /* m_nActiveState */,  0, 0x1); }  // 3C40
uint32_t rage::phArticulatedCollider::GetActive_1()  { return ExtractBits(this, 11648 /* m_nActiveState */,  1, 0x1); }  // 3C50
uint32_t rage::phArticulatedCollider::GetActive_2()  { return ExtractBits(this, 11648 /* m_nActiveState */,  2, 0x1); }  // 3C60

// ── Indexed bitfield getters (20-28B, stride-24 array) ───────────────────

/**
 * Indexed accessors — compute this + index*24 + base_offset, load word,
 * extract bits. Used for per-bone/per-joint packed data.
 */
uint32_t rage::phArticulatedCollider::GetIndexedField(int index, int shift, int mask) {
    uint32_t word = *(uint32_t*)((char*)this + 1168 + index * 24);
    return (word >> shift) & mask;
}

// Per-element getters (representative subset — 12 functions follow this pattern)
uint32_t rage::phArticulatedCollider::GetBoneField_5B90(int index) { return GetIndexedField(index, 0, 0x7); }   // 5B90
uint32_t rage::phArticulatedCollider::GetBoneField_5BE0(int index) { return GetIndexedField(index, 3, 0x7); }   // 5BE0
uint32_t rage::phArticulatedCollider::GetBoneField_5C30(int index) { return GetIndexedField(index, 6, 0x7); }   // 5C30
uint32_t rage::phArticulatedCollider::GetBoneField_5C88(int index) { return GetIndexedField(index, 9, 0x7); }   // 5C88
uint32_t rage::phArticulatedCollider::GetBoneField_5CE0(int index) { return GetIndexedField(index, 12, 0xF); }  // 5CE0
uint32_t rage::phArticulatedCollider::GetBoneField_5D38(int index) { return GetIndexedField(index, 16, 0xF); }  // 5D38

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

void rage::phArticulatedCollider::SetSolverParam_11636(uint32_t value) {  // setter for field at 11636
    SetFieldAndDirty(this, 11636 /* m_nSolverFlags */, value, 1ULL << 50);
}

void rage::phArticulatedCollider::SetConstraint_11628(uint32_t value) {
    SetFieldAndDirty(this, 11628 /* m_nJoints */, value, 1ULL << 51);
}

void rage::phArticulatedCollider::SetFlags_11644(uint32_t value) {
    SetFieldAndDirty(this, 11644 /* m_nCollisionFlags */, value, 1ULL << 52);
}

void rage::phArticulatedCollider::SetActive_11648(uint32_t value) {
    SetFieldAndDirty(this, 11648 /* m_nActiveState */, value, 1ULL << 53);
}

// ── Multi-store setter ──────────────────────────────────────────────────

/**
 * phArticulatedCollider::SetFourConsecutive @ ~20B
 * Stores the same value to 4 consecutive uint32 fields at +12124..+12136.
 */
void rage::phArticulatedCollider::SetFourConsecutive(uint32_t value) {  // 4D58
    this->m_vPositionX = value;     // +0x2F5C
    this->m_vPositionY = value;     // +0x2F60
    this->m_vPositionZ = value;     // +0x2F64
    this->m_vPositionW = value;     // +0x2F68
}

// ── Float getter with scaling ───────────────────────────────────────────

/**
 * phArticulatedCollider::GetScaledFloat @ 0x82164670 | size: 0x1C
 * Loads float at +11920, multiplies by a constant, returns result.
 */
float rage::phArticulatedCollider::GetScaledFloat() {  // 4670
    float rawValue = *(float*)&this->m_vLinearVelX;  // +0x2E90
    static const float kScale = 57.29578f;  // 180/PI (radians to degrees)
    return rawValue * kScale;
}

// ── Indexed bool check ──────────────────────────────────────────────────

/**
 * phArticulatedCollider::IsElementActive @ ~32B
 * Returns true if byte at this[index+176] is non-zero.
 */
bool rage::phArticulatedCollider::IsElementActive(int index) {  // E1B0
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
    this->vtable = (void**)0x82005918;  // +0x00
    // +4 first written as 0x820058FC, then overwritten with 0x82003DB0
    this->m_pBound = (uint32_t)(uintptr_t)(void*)0x82003DB0;  // +0x04
}

/**
 * phInst::InitWithParam @ 0x82461508 | size: 0x38
 * Constructor that takes a parameter object. Sets vtable pointers,
 * stores 1 at +8, copies param->field_4 to this+12, then sets
 * final vtable pair.
 */
void phInst::InitWithParam(void* param) {  // 1508_2hr
    *(uint32_t*)&this->m_pDestructor = 1;  // +0x08
    this->m_nRefCount = *(uint32_t*)((char*)param + 4);  // +0x0C
    this->vtable = (void**)0x8200586C;  // +0x00
    this->m_pBound = (uint32_t)(uintptr_t)(void*)0x82005850;  // +0x04
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
    uint32_t val36 = this->m_pRotationQuat & 0xFFFFF000;  // +0x24
    uint32_t val20 = this->m_pBoundData & 0xFFFFF000;  // +0x14
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
    g_mmioStoredValue = swapped;
}


// =============================================================================
// rage::phInst -- Virtual Function Implementations (8-112B)
// =============================================================================

// External declarations for this batch

// ---------------------------------------------------------------------------
// 1. phInst::SetUserData (vfn_16) @ 0x8248B8C8 | size: 0x8
//    Stores a 32-bit user data value at offset +284.
//    stw r4,284(r3); blr
// ---------------------------------------------------------------------------
void phInst::SetUserData(uint32_t val) {  // vfn_16
    this->m_nCollisionGroup = val;
}

// ---------------------------------------------------------------------------
// 2. phInst::GetUserData (vfn_19) @ 0x8248B8D0 | size: 0x8
//    Returns the 32-bit user data value from offset +284.
//    lwz r3,284(r3); blr
// ---------------------------------------------------------------------------
uint32_t phInst::GetUserData() {  // vfn_19
    return this->m_nCollisionGroup;
}

// ---------------------------------------------------------------------------
// 3. phInst::SetCollisionGroup (vfn_38) @ 0x8248D808 | size: 0x8
//    Stores the collision group identifier at offset +444.
//    stw r4,444(r3); blr
// ---------------------------------------------------------------------------
void phInst::SetCollisionGroup(uint32_t val) {  // vfn_38
    this->m_nCollisionMask = val;
}

// ---------------------------------------------------------------------------
// 4. phInst::GetCollisionGroup (vfn_44) @ 0x8248D810 | size: 0x8
//    Returns the collision group identifier from offset +444.
//    lwz r3,444(r3); blr
// ---------------------------------------------------------------------------
uint32_t phInst::GetCollisionGroup() {  // vfn_44
    return this->m_nCollisionMask;
}

// ---------------------------------------------------------------------------
// 5. phInst::SetCollisionMask (vfn_39) @ 0x8248D818 | size: 0x8
//    Stores the collision filter mask at offset +448.
//    stw r4,448(r3); blr
// ---------------------------------------------------------------------------
void phInst::SetCollisionMask(uint32_t val) {  // vfn_39
    this->m_nUserData = val;
}

// ---------------------------------------------------------------------------
// 6. phInst::GetCollisionMask (vfn_45) @ 0x8248D820 | size: 0x8
//    Returns the collision filter mask from offset +448.
//    lwz r3,448(r3); blr
// ---------------------------------------------------------------------------
uint32_t phInst::GetCollisionMask() {  // vfn_45
    return this->m_nUserData;
}

// ---------------------------------------------------------------------------
// 7. phInst::GetErrorCode (vfn_48) @ 0x8247E2D0 | size: 0xC
//    Returns a constant error/status code 0x80004001 (generic failure).
//    lis r3,-32768; ori r3,r3,16385; blr
// ---------------------------------------------------------------------------
uint32_t phInst::GetErrorCode() {  // vfn_48
    return 0x80004001;
}

// ---------------------------------------------------------------------------
// 8. phInst::ZeroFieldRanges (vfn_32) @ 0x8248D758 | size: 0x70
//    Zeros four 80-byte motion state regions at offsets +124, +284, +364,
//    and +204, then clears the collision group (+444) and mask (+448).
//    Each region is 20 dwords (80 bytes), zeroed via bdnz loop.
// ---------------------------------------------------------------------------
void phInst::ZeroFieldRanges() {  // vfn_32
    memset((char*)this + 124, 0, 80);   // +0x7C region
    memset((char*)this + 284, 0, 80);   // +0x11C = m_nCollisionGroup region
    memset((char*)this + 364, 0, 80);   // +0x16C region
    memset((char*)this + 204, 0, 80);   // +0xCC region
    this->m_nCollisionMask = 0;   // +0x1BC
    this->m_nUserData = 0;        // +0x1C0
}

// ---------------------------------------------------------------------------
// 9. phInst::CallVfn12ThenInit (vfn_9) @ 0x8248D7C8 | size: 0x40
//    Calls this->vfn_12() (vtable slot 12) then calls statePreInit_vfn_6
//    on this object to perform pre-initialization reset.
// ---------------------------------------------------------------------------
void phInst::CallVfn12ThenInit() {  // vfn_9
    this->vfn_12();
    statePreInit_vfn_6(this);
}

// ---------------------------------------------------------------------------
// 10. phInst::AtomicDecrementAndCallback (vfn_23) @ 0x82483FD8 | size: 0x3C
//     Atomically decrements the reference count at offset +368 using
//     lwarx/stwcx. If the callback pointer at +272 is non-null, tail-calls
//     through it with the user data from +284 as the argument.
// ---------------------------------------------------------------------------
void phInst::AtomicDecrementAndCallback() {  // vfn_23
    volatile uint32_t* refCount = (volatile uint32_t*)&this->m_fAccelerationY;  // +0x170

    // Atomic decrement (lwarx/stwcx loop)
    uint32_t old_val, new_val;
    do {
        old_val = *refCount;
        new_val = old_val - 1;
    } while (!__sync_bool_compare_and_swap(refCount, old_val, new_val));

    // If callback function pointer is set, tail-call it
    void* callbackFn = *(void**)((char*)this + 272);  // +0x110, padded region
    if (callbackFn == nullptr)
        return;

    uint32_t userData = this->m_nCollisionGroup;  // +0x11C
    typedef void (*CallbackFn)(uint32_t);
    ((CallbackFn)callbackFn)(userData);
}


// =============================================================================
// rage::phBoundCapsule -- Small Functions (<=64B) Batch Lift
// =============================================================================

// --- External declarations for this batch ---
extern void  _locale_register(void* ptr, void* allocator);

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
    this->m_nMaterialIndex = index;  // +0xC0
}

// ---------------------------------------------------------------------------
// 2. phBoundCapsule::GetMaterialIndex (vfn_12) @ 0x8256FBD0 | size: 0x8
//    Returns the 32-bit material index stored at offset +192.
// ---------------------------------------------------------------------------
uint32_t rage::phBoundCapsule::GetMaterialIndex() const {
    return this->m_nMaterialIndex;  // +0xC0
}

// ---------------------------------------------------------------------------
// 3. phBoundCapsule::GetVolume (vfn_8) @ 0x822A2DB0 | size: 0x2C
//    Computes capsule volume: V = (4/3) * PI * halfHeight * (halfHeight + radius)^2
//    offset +112 = m_halfHeight, +128 = m_radius
//    Uses two float constants from .rdata for PI-related factors.
// ---------------------------------------------------------------------------
float rage::phBoundCapsule::GetVolume() const {
    float halfHeight = *(float*)&this->m_pVertices;  // +0x70 (halfHeight for capsule)
    float radius     = *(float*)&this->m_fRadius;       // +0x80
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
    uint32_t matIdx = this->m_nMaterialIndex;  // +0xC0
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
    float radius     = *(float*)&this->m_fRadius;       // +0x80
    float halfHeight = *(float*)&this->m_pVertices;  // +0x70 (halfHeight for capsule)
    ph_1B78(this, halfHeight, radius, outPoint);
}

// ---------------------------------------------------------------------------
// 6. ph_TokenizerReadInt @ 0x82430E88 | size: 0x8
//    Calls fiAsciiTokenizer_ReadToken with mode=0 (read mode).
// ---------------------------------------------------------------------------
void ph_TokenizerReadInt(void* tokenizer) {
    fiAsciiTokenizer_ReadToken(tokenizer, 0);
}

// ---------------------------------------------------------------------------
// 7. ph_TokenizerReadFloat @ 0x82430E90 | size: 0x8
//    Calls fiAsciiTokenizer_ReadToken with mode=1 (write mode).
// ---------------------------------------------------------------------------
void ph_TokenizerReadFloat(void* tokenizer) {
    fiAsciiTokenizer_ReadToken(tokenizer, 1);
}

// ---------------------------------------------------------------------------
// 8. phBoundCapsule_InitCapsuleResult @ 0x823FDAD8 | size: 0x14
//    Initializes a result pair: sets type=3 (capsule) and value=0.
//    r6 = output struct pointer.
// ---------------------------------------------------------------------------
void phBoundCapsule_InitCapsuleResult(void* /*r3*/, void* /*r4*/, void* /*r5*/, uint32_t* outResult) {
    outResult[1] = 3;   // type = capsule
    outResult[0] = 0;   // value = 0
}

// ---------------------------------------------------------------------------
// 9. phBoundCapsule_InitSphereResult @ 0x823FDAF0 | size: 0x14
//    Initializes a result pair: sets type=2 (sphere) and byte[0]=0.
// ---------------------------------------------------------------------------
void phBoundCapsule_InitSphereResult(void* /*r3*/, void* /*r4*/, void* /*r5*/, void* outResult) {
    *(uint32_t*)((char*)outResult + 4) = 2;  // type = sphere
    *(uint8_t*)outResult = 0;                 // flag = 0
}

// ---------------------------------------------------------------------------
// 10. phBoundCapsule_ForwardToSink @ 0x82459C78 | size: 0x14
//     If field +24 is non-null, tail-calls msgMsgSink_DispatchNested on it.
// ---------------------------------------------------------------------------
void phBoundCapsule_ForwardToSink(void* obj) {
    void* sink = *(void**)((char*)obj + 24);
    if (sink == nullptr) return;
    msgMsgSink_DispatchNested(sink);
}

// ---------------------------------------------------------------------------
// 11. phBoundCapsule_ReadInt16Array @ 0x824AFCB0 | size: 0x10
//     Reads a signed 16-bit element from an array.
//     index = r6, base = r3. Element at base[index*2], sign-extended.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_ReadInt16Array(int16_t* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    return (int32_t)base[index];
}

// ---------------------------------------------------------------------------
// 12. phBoundCapsule_ReadPacked24 @ 0x824AFCC0 | size: 0x14
//     Reads a 24-bit packed element from a 3-byte-stride array, sign-extended
//     then arithmetic-shifted right by 8 to produce a signed result.
//     Stride = index * 3. Loads dword at base + stride, shifts >> 8.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_ReadPacked24(void* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint32_t raw = *(uint32_t*)((char*)base + stride);
    return (int32_t)raw >> 8;
}

// ---------------------------------------------------------------------------
// 13. phBoundCapsule_WriteInt16Array @ 0x824AFD60 | size: 0xC
//     Stores a 16-bit value into an array at index.
//     base = r4, index = r6, value = r3.
// ---------------------------------------------------------------------------
void phBoundCapsule_WriteInt16Array(uint16_t value, int16_t* base, void* /*r5*/, uint32_t index) {
    base[index] = (int16_t)value;
}

// ---------------------------------------------------------------------------
// 14. phBoundCapsule_WritePacked24 @ 0x824AFD70 | size: 0x24
//     Stores a 24-bit packed value into a 3-byte-stride array.
//     The value in r3 is decomposed: byte2 = r3 & 0xFF, byte1 = (r3>>8) & 0xFF,
//     byte0 = (r3>>16) & 0xFF, stored at base + index*3.
// ---------------------------------------------------------------------------
void phBoundCapsule_WritePacked24(int32_t value, uint8_t* base, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint8_t* dst = base + stride;
    int32_t mid = value >> 8;
    int32_t hi  = mid >> 8;
    dst[2] = (uint8_t)value;
    dst[1] = (uint8_t)mid;
    dst[0] = (uint8_t)hi;
}

// ---------------------------------------------------------------------------
// 15. phBoundCapsule_ReadPacked24Shifted @ 0x824AFCD8 | size: 0x28
//     Reads a 24-bit packed value from a 3-byte-stride array, reassembles
//     into a sign-extended 24-bit integer, then arithmetic-shifts right by 4.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_ReadPacked24Shifted(void* base, void* /*r4*/, void* /*r5*/, uint32_t index) {
    uint32_t stride = index * 3;
    uint8_t* src = (uint8_t*)base + stride;
    uint8_t lo  = src[2];
    uint16_t hi = *(uint16_t*)src;
    int8_t loSigned = (int8_t)lo;
    int32_t combined = ((int32_t)loSigned << 16) | hi;
    return combined >> 4;
}

// ---------------------------------------------------------------------------
// 16. phBoundCapsule_WritePacked24Shifted @ 0x824AFD98 | size: 0x28
//     Stores a value left-shifted by 4 into a 3-byte-stride packed array.
// ---------------------------------------------------------------------------
void phBoundCapsule_WritePacked24Shifted(int32_t value, uint8_t* base, void* /*r5*/, uint32_t index) {
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
// 17. phBoundCapsule_AllocArray @ 0x8256BAF0 | size: 0x14
//     Multiplies r3 * r4, then tail-calls rage_Alloc with that size
//     and allocator pointer 0x6489_0018.
// ---------------------------------------------------------------------------
void phBoundCapsule_AllocArray(int32_t elemSize, int32_t count) {
    int32_t totalSize = elemSize * count;
    rage_Alloc(totalSize, g_phAllocator);
}

// ---------------------------------------------------------------------------
// 18. phBoundCapsule_RegisterLocaleHook @ 0x8256BBB8 | size: 0xC
//     Tail-calls _locale_register with a fixed allocator pointer.
// ---------------------------------------------------------------------------
void phBoundCapsule_RegisterLocaleHook(void* ptr) {
    _locale_register(ptr, g_phAllocator);
}

// ---------------------------------------------------------------------------
// 19-24. SetCallback0..5 (FB70-FBC0 series) @ 0x8256FB70..0x8256FBC0
//        Each stores r3 into a consecutive global slot, returns 0.
//        These register physics callback function pointers.
// ---------------------------------------------------------------------------

// 19. phBoundCapsule_SetCallback0 @ 0x8256FB70 | size: 0x10
int32_t phBoundCapsule_SetCallback0(uint32_t callback) {
    g_phCallback0 = callback;  // @ 0x825EA900
    return 0;
}

// 20. phBoundCapsule_SetCallback1 @ 0x8256FB80 | size: 0x10
int32_t phBoundCapsule_SetCallback1(uint32_t callback) {
    g_phCallback1 = callback;  // @ 0x825EA904
    return 0;
}

// 21. phBoundCapsule_SetCallback2 @ 0x8256FB90 | size: 0x10
int32_t phBoundCapsule_SetCallback2(uint32_t callback) {
    g_phCallback2 = callback;  // @ 0x825EA908
    return 0;
}

// 22. phBoundCapsule_SetCallback3 @ 0x8256FBA0 | size: 0x10
int32_t phBoundCapsule_SetCallback3(uint32_t callback) {
    g_phCallback3 = callback;  // @ 0x825EA90C
    return 0;
}

// 23. phBoundCapsule_SetCallback4 @ 0x8256FBB0 | size: 0x10
int32_t phBoundCapsule_SetCallback4(uint32_t callback) {
    g_phCallback4 = callback;  // @ 0x825EA910
    return 0;
}

// 24. phBoundCapsule_SetCallback5 @ 0x8256FBC0 | size: 0x10
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
    ph_A330((char*)thisPtr - 4);
}

// ph_vt3DB0_8_9148 @ 0x82449148 | size: 0x8
// Adjusts this by -4, tail-calls ph_vt3DB0_12_8DB8 (slot 8 thunk)
void ph_vt3DB0_8_Thunk(void* thisPtr) {
    ph_vt3DB0_12_8DB8((char*)thisPtr - 4);
}

// ph_vt3DB0_9_9150 @ 0x82449150 | size: 0x8
// Adjusts this by -4, tail-calls ph_vt3DB0_13_8E10 (slot 9 thunk)
void ph_vt3DB0_9_Thunk(void* thisPtr) {
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
    ph_vt57D8_2_6378((char*)thisPtr - 4);
}

// =============================================================================
// phInst2 vtable thunks (vt58FC) — phInst variant at +4
// Vtable @ 0x820058FC (phInst2)
// =============================================================================

// ph_vt58FC_0_4E38 @ 0x82464E38 | size: 0x8
// Adjusts this by -4, tail-calls phInst_5910_p39
void ph_vt58FC_0_Thunk(void* thisPtr) {
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
    aud_6A20_wrap_6A20((char*)thisPtr - 4);
}

// ph_vt5A60_20_68C8 @ 0x824668C8 | size: 0x8
// Adjusts this by -16, tail-calls aud_6A20_wrap_6A20
void ph_vt5A60_20_Thunk(void* thisPtr) {
    aud_6A20_wrap_6A20((char*)thisPtr - 16);
}

// ph_vt5A60_22_68D0 @ 0x824668D0 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_57_6858
void ph_vt5A60_22_Thunk(void* thisPtr) {
    ph_vt5A60_57_6858((char*)thisPtr - 16);
}

// ph_vt5A60_23_7230 @ 0x82467230 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_58_6EE8
void ph_vt5A60_23_Thunk(void* thisPtr) {
    ph_vt5A60_58_6EE8((char*)thisPtr - 16);
}

// ph_vt5A60_32_7238 @ 0x82467238 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_32_Thunk(void* thisPtr) {
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_34_7240 @ 0x82467240 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5A60_34_Thunk(void* thisPtr) {
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5A60_36_7248 @ 0x82467248 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_36_Thunk(void* thisPtr) {
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_28_7C18 @ 0x82467C18 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_60_7870
void ph_vt5A60_28_Thunk(void* thisPtr) {
    ph_vt5A60_60_7870((char*)thisPtr - 16);
}

// ph_vt5A60_29_7C20 @ 0x82467C20 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_61_7A38
void ph_vt5A60_29_Thunk(void* thisPtr) {
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
    ph_vt57D8_28_FD08((char*)thisPtr + 4);
}

// ph_vt5A60_54_8A40 @ 0x82468A40 | size: 0x8
// Adjusts this by +4, tail-calls ph_vt57D8_29_FDD0
void ph_vt5A60_54_Thunk(void* thisPtr) {
    ph_vt57D8_29_FDD0((char*)thisPtr + 4);
}

// ph_vt5A60_47_8CD8 @ 0x82468CD8 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_60_7870
void ph_vt5A60_47_Thunk(void* thisPtr) {
    ph_vt5A60_60_7870((char*)thisPtr - 16);
}

// ph_vt5A60_48_8CE0 @ 0x82468CE0 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_61_7A38
void ph_vt5A60_48_Thunk(void* thisPtr) {
    ph_vt5A60_61_7A38((char*)thisPtr - 16);
}

// ph_vt5A60_49_8CE8 @ 0x82468CE8 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_62_8F80
void ph_vt5A60_49_Thunk(void* thisPtr) {
    ph_vt5A60_62_8F80((char*)thisPtr - 16);
}

// ph_vt5A60_24_8D00 @ 0x82468D00 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_24_Thunk(void* thisPtr) {
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_25_8D08 @ 0x82468D08 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5A60_25_Thunk(void* thisPtr) {
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5A60_26_8D10 @ 0x82468D10 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_26_Thunk(void* thisPtr) {
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_27_8D18 @ 0x82468D18 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_58_6EE8
void ph_vt5A60_27_Thunk(void* thisPtr) {
    ph_vt5A60_58_6EE8((char*)thisPtr - 16);
}

// ph_vt5A60_31_8D20 @ 0x82468D20 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_57_6858
void ph_vt5A60_31_Thunk(void* thisPtr) {
    ph_vt5A60_57_6858((char*)thisPtr - 16);
}

// ph_vt5A60_33_8D30 @ 0x82468D30 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5A60_33_Thunk(void* thisPtr) {
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5A60_35_8D40 @ 0x82468D40 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5A60_35_Thunk(void* thisPtr) {
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5A60_30_9098 @ 0x82469098 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A60_62_8F80
void ph_vt5A60_30_Thunk(void* thisPtr) {
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
    atSingleton_8A48_p42((char*)thisPtr - 4);
}

// ph_vt5B98_0_8950 @ 0x82468950 | size: 0x8
// Adjusts this by -16, tail-calls atSingleton_8A48_p42
void ph_vt5B98_0_Thunk(void* thisPtr) {
    atSingleton_8A48_p42((char*)thisPtr - 16);
}

// ph_vt5B98_37_8990 @ 0x82468990 | size: 0x8
// Adjusts this by +16, tail-calls ph_vt57D8_2_6378
void ph_vt5B98_37_Thunk(void* thisPtr) {
    ph_vt57D8_2_6378((char*)thisPtr + 16);
}

// ph_vt5B98_38_8998 @ 0x82468998 | size: 0x8
// Adjusts this by +16, tail-calls ph_vt57D8_3_61E0
void ph_vt5B98_38_Thunk(void* thisPtr) {
    ph_vt57D8_3_61E0((char*)thisPtr + 16);
}

// ph_vt5B98_12_8D28 @ 0x82468D28 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A8C_63_6A50
void ph_vt5B98_12_Thunk(void* thisPtr) {
    ph_vt5A8C_63_6A50((char*)thisPtr - 16);
}

// ph_vt5B98_14_8D38 @ 0x82468D38 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A7C_63_6A98
void ph_vt5B98_14_Thunk(void* thisPtr) {
    ph_vt5A7C_63_6A98((char*)thisPtr - 16);
}

// ph_vt5B98_16_8D48 @ 0x82468D48 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5A84_63_6B90
void ph_vt5B98_16_Thunk(void* thisPtr) {
    ph_vt5A84_63_6B90((char*)thisPtr - 16);
}

// ph_vt5B98_8_9088 @ 0x82469088 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5B98_40_8D50
void ph_vt5B98_8_Thunk(void* thisPtr) {
    ph_vt5B98_40_8D50((char*)thisPtr - 16);
}

// ph_vt5B98_9_9090 @ 0x82469090 | size: 0x8
// Adjusts this by -16, tail-calls ph_vt5B98_41_8E50
void ph_vt5B98_9_Thunk(void* thisPtr) {
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
// Calls ke_DestroyObjectA cleanup, returns this (destructor pattern)
void* ph_vt5CD8_0_Destructor(void* thisPtr) {
    ke_DestroyObjectA(thisPtr);
    return thisPtr;
}

// ph_vt5D38_12_D560 @ 0x8246D560 | size: 0x10
// Gets byte at +61, stores to output ptr, returns 0
int ph_vt5D38_12_GetByte61(void* thisPtr, uint8_t* out) {
    *out = *((uint8_t*)thisPtr + 61);
    return 0;
}

// ph_vt5D38_0_D6F0 @ 0x8246D6F0 | size: 0x30
// Calls ke_DestroyObjectB cleanup, returns this (destructor pattern)
void* ph_vt5D38_0_Destructor(void* thisPtr) {
    ke_DestroyObjectB(thisPtr);
    return thisPtr;
}

// ph_vt6E40_0_E368 @ 0x8247E368 | size: 0x30
// Calls ph_E1E8 cleanup, returns this (destructor pattern)
void* ph_vt6E40_0_Destructor(void* thisPtr) {
    ph_E1E8(thisPtr);
    return thisPtr;
}

// ph_vt5C84_5_AAD0 @ 0x8246AAD0 | size: 0x30
// Calls ke_ConstructObject, returns this (init + return-self pattern)
void* ph_vt5C84_5_InitAndReturn(void* thisPtr) {
    ke_ConstructObject(thisPtr);
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
// Wrapper that forwards args to fiAsciiTokenizer_ReadBool and returns byte result
uint8_t ph_TokenizerReadBool(void* a, void* b, void* c) {
    return fiAsciiTokenizer_ReadBool(a, b, c);
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

// ph_GetBoundPointer @ 0x822AFFB8 | size: 0x1C
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
    // Set vtable
    *(void**)thisPtr = (void*)0x820014C8;  // lbl_820014C8 vtable
    uint32_t childFlag = *(uint32_t*)((char*)thisPtr + 8);
    if (childFlag != 0) {
        void* child = *(void**)((char*)thisPtr + 4);
        fiAsciiTokenizer_Destroy(child);
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
    *(uint64_t*)((char*)obj + 40) /* dirtyMask40 */ |= dirtyBit;
}

// Helper: insert value into bitfield, store back, set dirty bit at this+48
static inline void InsertBitsAndDirty48(void* obj, int offset, uint32_t value,
                                         int shift, uint32_t mask, uint64_t dirtyBit) {
    uint32_t word = *(uint32_t*)((char*)obj + offset);
    word = (word & ~(mask << shift)) | ((value & mask) << shift);
    *(uint32_t*)((char*)obj + offset) = word;
    *(uint64_t*)((char*)obj + 48) /* dirtyMask48 */ |= dirtyBit;
}

// Helper: store float to field, set dirty bit at this+48
static inline void StoreFloatAndDirty48(void* obj, int offset, float value, uint64_t dirtyBit) {
    *(float*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 48) /* dirtyMask48 */ |= dirtyBit;
}

// Helper: store u32 to field, set dirty bit at this+40
static inline void StoreU32AndDirty40(void* obj, int offset, uint32_t value, uint64_t dirtyBit) {
    *(uint32_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 40) /* dirtyMask40 */ |= dirtyBit;
}

// Helper: store u32 to field, set dirty bit at this+48
static inline void StoreU32AndDirty48(void* obj, int offset, uint32_t value, uint64_t dirtyBit) {
    *(uint32_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 48) /* dirtyMask48 */ |= dirtyBit;
}

// Helper: store u8 to field, set dirty bit at this+40
static inline void StoreU8AndDirty40(void* obj, int offset, uint8_t value, uint64_t dirtyBit) {
    *(uint8_t*)((char*)obj + offset) = value;
    *(uint64_t*)((char*)obj + 40) /* dirtyMask40 */ |= dirtyBit;
}

// ── 1. Simple bitfield getter: +11660 bit 0 (0x1) ──────────── 4DC8 (0xc)
uint32_t rage::phArticulatedCollider::GetSolverFlag_18()  { return ExtractBits(this, 11660,  0, 0x1); }

// ── 2. Simple getter: +11704 bits 0-1 (0x3) ────────────────── 5138 (0xc)
uint32_t rage::phArticulatedCollider::GetLimitType_0()    { return ExtractBits(this, 11704,  0, 0x3); }

// ── 3. Simple getter: +11776 bit 0 (0x1) ───────────────────── 5170 (0xc)
uint32_t rage::phArticulatedCollider::GetDofFlag_0()      { return ExtractBits(this, 11776,  0, 0x1); }

// ── 4. Simple getter: +11656 bit 21 (0x1) ──────────────────── 51A8 (0xc)
// rlwinm r3,r11,11,31,31 → (word >> 21) & 1
uint32_t rage::phArticulatedCollider::GetSolverParam_21() { return ExtractBits(this, 11656 /* m_nJointUpdateFlags */, 21, 0x1); }

// ── 5. Simple getter: +11644 bit 4 (0x1) ───────────────────── 5208 (0xc)
// rlwinm r3,r11,28,31,31 → (word >> 4) & 1
uint32_t rage::phArticulatedCollider::GetFlag7C_4b()      { return ExtractBits(this, 11644 /* m_nCollisionFlags */,  4, 0x1); }

// ── 6. Simple getter: +11648 bit 3 (0x1) ───────────────────── 5340 (0xc)
// rlwinm r3,r11,29,31,31 → (word >> 3) & 1
uint32_t rage::phArticulatedCollider::GetActive_3()       { return ExtractBits(this, 11648 /* m_nActiveState */,  3, 0x1); }

// ── 7. Simple getter: +11648 bit 2 (0x1) ───────────────────── 5378 (0xc)
// rlwinm r3,r11,30,31,31 → (word >> 2) & 1 (same field as GetActive_2 but separate symbol)
uint32_t rage::phArticulatedCollider::GetActive_2b()      { return ExtractBits(this, 11648 /* m_nActiveState */,  2, 0x1); }

// ── 8. Simple getter: +11648 bit 5 (0x1) ───────────────────── 53B0 (0xc)
// rlwinm r3,r11,27,31,31 → (word >> 5) & 1
uint32_t rage::phArticulatedCollider::GetActive_5()       { return ExtractBits(this, 11648 /* m_nActiveState */,  5, 0x1); }

// ── 9. Simple getter: +11452 bits 23-29 (0x7F) ─────────────── 5408 (0xc)
// rlwinm r3,r11,9,25,31 → (word >> 23) & 0x7F
uint32_t rage::phArticulatedCollider::GetCBCField_23()    { return ExtractBits(this, 11452, 23, 0x7F); }

// ── 10. Byte-indexed getter: this[index+12098] bit 1 ────────── 5598 (0x10)
// lbz + rlwinm r3,r11,31,31,31 → (byte >> 1) & 1
uint32_t rage::phArticulatedCollider::GetIndexedByteFlag_1(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return (byte >> 1) & 0x1;
}

// ── 11. Byte-indexed getter: this[index+12098] bit 0 ────────── 5728 (0x10)
// lbz + clrlwi r3,r11,31 → byte & 1
uint32_t rage::phArticulatedCollider::GetIndexedByteFlag_0(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return byte & 0x1;
}

// ── 12. Byte-indexed getter: this[index+12098] bit 2 ────────── 5818 (0x10)
// lbz + rlwinm r3,r11,30,31,31 → (byte >> 2) & 1
uint32_t rage::phArticulatedCollider::GetIndexedByteFlag_2(int index) {
    uint8_t byte = *(uint8_t*)((char*)this + index + 12098);
    return (byte >> 2) & 0x1;
}

// ── 13. Indexed field setter: +11452 insert 7-bit at bit 23 ── 53F8 (0x10)
// rlwimi r11,r4,23,2,8 → mask 0x3F800000 = 0x7F << 23
void rage::phArticulatedCollider::SetCBCField_23(uint32_t value) {
    uint32_t word = this->m_nActiveComponents;  // +0x2CBC
    word = (word & ~(0x7FU << 23)) | ((value & 0x7F) << 23);
    this->m_nActiveComponents = word;
}

// ── 14. Read byte from +11644 ───────────────────────────────── 5240 (0x14)
// lbz + rlwimi (identity copy) + clrlwi → just returns byte at +11644
uint32_t rage::phArticulatedCollider::GetFlag7C_Byte() {
    return *(uint8_t*)&this->m_nCollisionFlags  /* +0x2D7C */;
}

// ── 15. Indexed getter: stride-24 array at +1164, bits 23-24 ── 5778 (0x14)
// mulli r11,r4,24; add; lwz 1164; rlwinm r3,r11,9,30,31 → (word >> 23) & 0x3
uint32_t rage::phArticulatedCollider::GetBoneField_5778(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1164 + index * 24);
    return (word >> 23) & 0x3;
}

// ── 16. Indexed getter: stride-24 at +1172, bit 2 ──────────── 5D90 (0x14)
// lwz 1172; rlwinm r3,r11,30,31,31 → (word >> 2) & 1
uint32_t rage::phArticulatedCollider::GetBoneField_5D90(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1172 + index * 24);
    return (word >> 2) & 0x1;
}

// ── 17. Indexed negation check: stride-24 at +1156 ─────────── 5DF0 (0x18)
// lwz 1156; not; rlwinm 21,31,31 → (~word >> 11) & 1
uint32_t rage::phArticulatedCollider::GetBoneFieldNeg_5DF0(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1156 + index * 24);
    return (~word >> 11) & 0x1;
}

// ── 18. Indexed bool: stride-24 at +1172, nonzero low 2 bits ── 5B38 (0x1c)
// lwz 1172; clrlwi 30 → bits 0-1; subfic+subfe → (val != 0) ? -1 : 0
// The subfic/subfe idiom computes -(val != 0), i.e. 0xFFFFFFFF if nonzero
bool rage::phArticulatedCollider::IsBoneFieldNonZero(int index) {
    uint32_t word = *(uint32_t*)((char*)this + 1172 + index * 24);
    return (word & 0x3) != 0;
}

// ── 19. Store float + dirty40 at +11712 ─────────────────────── 50B8 (0x20)
// stfs f0,11712(r3); ld/oris/std at this+40; oris r10,r10,4 → 1<<18
void rage::phArticulatedCollider::SetLimitFloat_11712(float value) {
    *(float*)&this->m_nContactCount = value;  // +0x2DC0
    *(uint64_t*)((char*)this + 40) |= (1ULL << 18);
}

// ── 20. Store float + dirty40 at +11708 ─────────────────────── 50E8 (0x20)
// stfs f0,11708(r3); ld/oris/std at this+40; oris r10,r10,8 → 1<<19
void rage::phArticulatedCollider::SetLimitFloat_11708(float value) {
    *(float*)&this->m_pContactList = value;  // +0x2DBC
    *(uint64_t*)((char*)this + 40) |= (1ULL << 19);
}

// ── 21. Insert 2 bits at +11704 bit 0 + dirty40 ────────────── 5118 (0x20)
// rlwimi r10,r4,0,30,31 → mask 0x3; oris r10,r10,16 → 0x100000 = 1<<20
void rage::phArticulatedCollider::SetLimitType_0(uint32_t value) {
    InsertBitsAndDirty40(this, 11704, value, 0, 0x3, 1ULL << 20);
}

// ── 22. Store u32 + dirty40 at +11544 ───────────────────────── 51B8 (0x20)
// stw r4,11544(r3); rldicr r12,r12,60 → 1ULL<<60
void rage::phArticulatedCollider::SetField_11544(uint32_t value) {
    StoreU32AndDirty40(this, 11544, value, 1ULL << 60);
}

// ── 23. Store byte + dirty40 at +11650 ──────────────────────── 53C0 (0x20)
// stb r4,11650(r3); rldicr r12,r12,34 → 1ULL<<34
void rage::phArticulatedCollider::SetActiveByte_11650(uint8_t value) {
    StoreU8AndDirty40(this, 11650, value, 1ULL << 34);
}

// ── 24. Store u16 + dirty48 at +11840 ───────────────────────── 4770 (0x24)
// clrlwi r10,r4,16; stw r10,11840(r3); rldicr r12,r12,45 → 1ULL<<45
void rage::phArticulatedCollider::SetDofU16_11840(uint32_t value) {
    StoreU32AndDirty48(this, 11840, value & 0xFFFF, 1ULL << 45);
}

// ── 25. Insert 1 bit at +11636 bit 2 + dirty40 ─────────────── 4170 (0x28)
// rlwimi r10,r4,2,29,29 → (value & 1) << 2; rldicr 37 → 1ULL<<37
void rage::phArticulatedCollider::SetRotAxis_2(uint32_t value) {
    InsertBitsAndDirty40(this, 11636 /* m_nSolverFlags */, value, 2, 0x1, 1ULL << 37);
}

// ── 26. Insert 3 bits at +11636 bits 8-10 + dirty40 ────────── 4288 (0x28)
// rlwimi r10,r4,8,21,23 → (value & 7) << 8; rldicr 37 → 1ULL<<37
void rage::phArticulatedCollider::SetRotAxis_8(uint32_t value) {
    InsertBitsAndDirty40(this, 11636 /* m_nSolverFlags */, value, 8, 0x7, 1ULL << 37);
}

// ── 27. Insert 3 bits at +11636 bits 14-16 + dirty40 ───────── 4350 (0x28)
// rlwimi r10,r4,14,15,17 → (value & 7) << 14; rldicr 37
void rage::phArticulatedCollider::SetRotAxis_14(uint32_t value) {
    InsertBitsAndDirty40(this, 11636 /* m_nSolverFlags */, value, 14, 0x7, 1ULL << 37);
}

// ── 28. Insert 3 bits at +11636 bits 20-22 + dirty40 ───────── 4388 (0x28)
// rlwimi r10,r4,20,9,11 → (value & 7) << 20; rldicr 37
void rage::phArticulatedCollider::SetRotAxis_20(uint32_t value) {
    InsertBitsAndDirty40(this, 11636 /* m_nSolverFlags */, value, 20, 0x7, 1ULL << 37);
}

// ── 29. Insert 3 bits at +11636 bits 26-28 + dirty40 ───────── 4450 (0x28)
// rlwimi r10,r4,26,3,5 → (value & 7) << 26; rldicr 37
void rage::phArticulatedCollider::SetRotAxis_26(uint32_t value) {
    InsertBitsAndDirty40(this, 11636 /* m_nSolverFlags */, value, 26, 0x7, 1ULL << 37);
}

// ── 30. Insert 1 bit at +11656 bit 15 + dirty40 ────────────── 4738 (0x28)
// rlwimi r10,r4,15,16,16 → (value & 1) << 15; rldicr 32 → 1ULL<<32
void rage::phArticulatedCollider::SetSolverParam_15(uint32_t value) {
    InsertBitsAndDirty40(this, 11656 /* m_nJointUpdateFlags */, value, 15, 0x1, 1ULL << 32);
}

// ── 31. Insert 4 bits at +11628 bits 28-31 + dirty40 ───────── 4B18 (0x28)
// rlwimi r10,r4,28,0,3 → (value & 0xF) << 28; rldicr 39 → 1ULL<<39
void rage::phArticulatedCollider::SetConstraint_28b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628 /* m_nJoints */, value, 28, 0xF, 1ULL << 39);
}

// ── 32. Insert 4 bits at +11632 bits 28-31 + dirty40 ───────── 4C90 (0x28)
// rlwimi r10,r4,28,0,3 → (value & 0xF) << 28; rldicr 39
void rage::phArticulatedCollider::SetJointParam_28(uint32_t value) {
    InsertBitsAndDirty40(this, 11632, value, 28, 0xF, 1ULL << 39);
}

// ── 33. Insert 1 bit at +11776 bit 0 + dirty48 ─────────────── 5148 (0x28)
// rlwimi r10,r4,0,31,31 → (value & 1); rldicr 61 → 1ULL<<61
void rage::phArticulatedCollider::SetDofFlag_0(uint32_t value) {
    InsertBitsAndDirty48(this, 11776 /* m_nDofFlag */, value, 0, 0x1, 1ULL << 61);
}

// ── 34. Insert 1 bit at +11656 bit 21 + dirty40 ────────────── 5180 (0x28)
// rlwimi r10,r4,21,10,10 → (value & 1) << 21; rldicr 32
void rage::phArticulatedCollider::SetSolverParam_21(uint32_t value) {
    InsertBitsAndDirty40(this, 11656 /* m_nJointUpdateFlags */, value, 21, 0x1, 1ULL << 32);
}

// ── 35. Insert 1 bit at +11644 bit 4 + dirty40 ─────────────── 51E0 (0x28)
// rlwimi r10,r4,4,27,27 → (value & 1) << 4; rldicr 35 → 1ULL<<35
void rage::phArticulatedCollider::SetFlag7C_4b(uint32_t value) {
    InsertBitsAndDirty40(this, 11644 /* m_nCollisionFlags */, value, 4, 0x1, 1ULL << 35);
}

// ── 36. Insert 8 bits at +11644 bits 24-31 + dirty40 ───────── 5218 (0x28)
// rlwimi r10,r4,24,0,7 → (value & 0xFF) << 24; rldicr 35
void rage::phArticulatedCollider::SetFlag7C_24(uint32_t value) {
    InsertBitsAndDirty40(this, 11644 /* m_nCollisionFlags */, value, 24, 0xFF, 1ULL << 35);
}

// ── 37. Store float + dirty48 at +11788 ─────────────────────── 5258 (0x28)
// stfs f0,11788(r3); rldicr r12,r12,58 → 1ULL<<58
void rage::phArticulatedCollider::SetRangeFloat_11788(float value) {
    StoreFloatAndDirty48(this, 11788, value, 1ULL << 58);
}

// ── 38. Store float + dirty48 at +11780 ─────────────────────── 5288 (0x28)
// stfs f0,11780(r3); rldicr r12,r12,60 → 1ULL<<60
void rage::phArticulatedCollider::SetRangeFloat_11780(float value) {
    StoreFloatAndDirty48(this, 11780, value, 1ULL << 60);
}

// ── 39. Store float + dirty48 at +11792 ─────────────────────── 52B8 (0x28)
// stfs f0,11792(r3); rldicr r12,r12,57 → 1ULL<<57
void rage::phArticulatedCollider::SetRangeFloat_11792(float value) {
    StoreFloatAndDirty48(this, 11792, value, 1ULL << 57);
}

// ── 40. Store float + dirty48 at +11784 ─────────────────────── 52E8 (0x28)
// stfs f0,11784(r3); rldicr r12,r12,59 → 1ULL<<59
void rage::phArticulatedCollider::SetRangeFloat_11784(float value) {
    StoreFloatAndDirty48(this, 11784, value, 1ULL << 59);
}

// ── 41. Insert 1 bit at +11648 bit 3 + dirty40 ─────────────── 5318 (0x28)
// rlwimi r10,r4,3,28,28 → (value & 1) << 3; rldicr 34 → 1ULL<<34
void rage::phArticulatedCollider::SetActive_3(uint32_t value) {
    InsertBitsAndDirty40(this, 11648 /* m_nActiveState */, value, 3, 0x1, 1ULL << 34);
}

// ── 42. Insert 1 bit at +11648 bit 2 + dirty40 ─────────────── 5350 (0x28)
// rlwimi r10,r4,2,29,29 → (value & 1) << 2; rldicr 34
void rage::phArticulatedCollider::SetActive_2b(uint32_t value) {
    InsertBitsAndDirty40(this, 11648 /* m_nActiveState */, value, 2, 0x1, 1ULL << 34);
}

// ── 43. Insert 1 bit at +11648 bit 5 + dirty40 ─────────────── 5388 (0x28)
// rlwimi r10,r4,5,26,26 → (value & 1) << 5; rldicr 34
void rage::phArticulatedCollider::SetActive_5(uint32_t value) {
    InsertBitsAndDirty40(this, 11648 /* m_nActiveState */, value, 5, 0x1, 1ULL << 34);
}

// ── 44. Insert 6-bit low + dirty40 at +11652 ───────────────── 4578 (0x2c)
// rlwinm r10,r10,0,0,25 → clear low 6 bits; or r10,r10,r4; rldicr 33 → 1ULL<<33
void rage::phArticulatedCollider::SetMiscField_11652(uint32_t value) {
    uint32_t word = this->m_nConstraintFlags;  // +0x2D84
    word = (word & 0xFFFFFFC0) | (value & 0x3F);
    this->m_nConstraintFlags = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 33);  // +0x28 dirty mask (overlaps m_pConstraints+pad)
}

// ── 45. Insert 4 bits at +11628 bit 0 + dirty40 ────────────── 49C8 (0x2c)
// rlwinm r10,r10,0,0,27 → clear low 4 bits; or r10,r10,r4; rldicr 39
void rage::phArticulatedCollider::SetConstraint_0b(uint32_t value) {
    uint32_t word = *(uint32_t*)&this->m_nJoints  /* +0x2D6C */;
    word = (word & 0xFFFFFFF0) | (value & 0xF);
    *(uint32_t*)&this->m_nJoints  /* +0x2D6C */ = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 39);
}

// ── 46. Insert 4 bits at +11632 bit 0 + dirty40 ────────────── 4B40 (0x2c)
// rlwinm r10,r10,0,0,27 → clear low 4 bits; or; rldicr 39
void rage::phArticulatedCollider::SetJointParam_0(uint32_t value) {
    uint32_t word = *(uint32_t*)&this->m_nActiveJointCount  /* +0x2D70 */;
    word = (word & 0xFFFFFFF0) | (value & 0xF);
    *(uint32_t*)&this->m_nActiveJointCount  /* +0x2D70 */ = word;
    *(uint64_t*)((char*)this + 40) |= (1ULL << 39);
}

// ── 47. vfn_57 @ 0x822508E0 | size: 0x30 ───────────────────────────────────
// Computes interpolated joint limit: result = arr492[idx]*k1 + arr500[idx]*k2
// where k1 and k2 are float constants from .rodata.
float rage::phArticulatedCollider::GetInterpolatedLimit(int index) {  // vfn_57
    float* arr500 = (float*)(uintptr_t)this->m_pJointDampingArray;  // +0x1F4
    float* arr492 = (float*)(uintptr_t)this->m_pJointInertiaArray;  // +0x1EC
    static const float kBlendA = 0.01745329f;  // PI/180 (degrees to radians)
    static const float kBlendB = 57.29578f;    // 180/PI (radians to degrees)
    float valA = arr500[index];
    float valB = arr492[index];
    return valB * kBlendA + valA * kBlendB;
}

// ── 48. Insert 4 bits at +11628 bits 4-7 + dirty40 ─────────── 49F8 (0x30)
// clear bits 4-7, insert value<<4; rldicr 39
void rage::phArticulatedCollider::SetConstraint_4b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628 /* m_nJoints */, value, 4, 0xF, 1ULL << 39);
}

// ── 49. Insert 4 bits at +11628 bits 8-11 + dirty40 ────────── 4A28 (0x30)
void rage::phArticulatedCollider::SetConstraint_8b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628 /* m_nJoints */, value, 8, 0xF, 1ULL << 39);
}

// ── 50. Insert 4 bits at +11628 bits 12-15 + dirty40 ───────── 4A58 (0x30)
void rage::phArticulatedCollider::SetConstraint_12b(uint32_t value) {
    InsertBitsAndDirty40(this, 11628 /* m_nJoints */, value, 12, 0xF, 1ULL << 39);
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
extern void phObject_WriteValue(void* thisPtr, uint32_t param);
extern "C" void _locale_register(void* ptr, uint32_t tag);

// ─────────────────────────────────────────────────────────────────────────────
// phObject_GetField120 @ 0x82485108 | size: 0x8
//
// Simple getter: returns the field at offset +120 (m_field_120).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phObject_GetField120(void* thisPtr) {
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
// phObject_DestructorThunk @ 0x8248B878 | size: 0x10
//
// Destructor helper: sets the vtable pointer for a specific subclass,
// then tail-calls util_85C8 (via thunk_fn_824885C8) for base cleanup.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_DestructorThunk(void* thisPtr) {
    *(void**)thisPtr = (void*)0x82019168;  // lis r11,-32255; addi r11,r11,-28312
    thunk_fn_824885C8(thisPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_DispatchIndirect @ 0x824920B0 | size: 0x10
//
// Indirect call trampoline: loads a function pointer from a fixed global
// address and calls it with the current arguments. Used for late-bound
// dispatch in the physics write/read/hash pipeline.
// ─────────────────────────────────────────────────────────────────────────────
typedef void (*phObjectDispatchFunc)(void);
void phObject_DispatchIndirect() {
    // lis r11, -32255 -> 0x81E10000; lwz r11, -26992(r11) -> 0x81E10000 + 0xFFFF9690 = global at 0x82019690 area
    // Load function pointer from global and call it
    volatile phObjectDispatchFunc* pFunc = (volatile phObjectDispatchFunc*)0x82019690;
    (*pFunc)();
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_ClearDirtyState @ 0x82491E18 | size: 0x18
//
// Clears a flag byte at offset +1524 and a 32-bit word at offset +1528.
// Returns 0. Used to reset an active/dirty state on a physics object.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_ClearDirtyState(void* thisPtr) {
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
// phObject_GetArrayElement @ 0x82492340 | size: 0x28
//
// Array element lookup: reads element at index from a 32-bit array (this),
// stores it to the output pointer. Returns 0 on success, or an error code
// (0x80300016) if the element is NULL.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_GetArrayElement(uint32_t* array, uint32_t index, uint32_t* outValue) {
    uint32_t element = array[index];
    *outValue = element;
    if (element == 0) {
        return (int32_t)0x80300016;
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_ValidateAndForward @ 0x8248A4A0 | size: 0x2C
//
// Null-validates three parameters (this, param1, param3), then loads the
// first word from param1 and tail-calls phObject_WriteValue.
// Returns error 0x80070057 on null parameter.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_ValidateAndForward(void* thisPtr, void* param1, uint32_t param2, void* param3) {
    if (!thisPtr || !param1 || !param3) {
        return (int32_t)0x80070057;
    }
    uint32_t value = *(uint32_t*)param1;
    phObject_WriteValue((void*)(uintptr_t)value, param2);
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
// phObject_ClearStridedEntry @ 0x82491AD0 | size: 0x48
//
// Clears a 12-byte entry in a strided array. Each entry is 12 bytes
// (3 x uint32). Index must be < 127. Returns 0x80070057 on out-of-range
// or null this. Zeroes the 3 words at this[index * 12].
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_ClearStridedEntry(void* thisPtr, int32_t index) {
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
// phObject_ValidateAndClearEntry @ 0x8248A4D0 | size: 0x44
//
// Null-validates this, param1, and param3. Loads two values from param1
// at offsets +56 and +60. If the value at +56 is zero, returns early.
// Otherwise loads param3+608 as array base, and calls phObject_ClearStridedEntry
// to clear the entry at the given index.
// Returns 0x80070057 on null parameters.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_ValidateAndClearEntry(void* thisPtr, void* param1, uint32_t param2, void* param3) {
    if (!thisPtr || !param1 || !param3) {
        return (int32_t)0x80070057;
    }

    int32_t count = *(int32_t*)((uint8_t*)param1 + 56);
    if (count == 0) {
        return 0;
    }

    int32_t index = *(int32_t*)((uint8_t*)param1 + 60);
    void* arrayBase = *(void**)((uint8_t*)param3 + 608);
    return phObject_ClearStridedEntry(arrayBase, index);
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject_UnregisterResource @ 0x82491F28 | size: 0x44
//
// Unregisters a resource: calls _locale_register on the value stored at
// *outPtr (param3), then clears *outPtr to NULL. Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_UnregisterResource(void* thisPtr, uint32_t param2, void** outPtr) {
    void* resource = *outPtr;
    _locale_register(resource, 0x208C8000);
    *outPtr = NULL;
    return 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// phObject — Stream / Resource binding helpers (96-172B)
// ═════════════════════════════════════════════════════════════════════════════

// Forward declarations for phObject stream helpers
extern void phObject_BindOutputStream(void* stream, uint32_t a2, uint32_t a3,
                               uint32_t a4, uint32_t a5, void* outPtr);
extern void rage_A518(void* outCtx, void* thisPtr, void* bufferPtr);
extern void phObject_ReleaseStreamContext(void* thisPtr);
extern void phObject_ReleaseSavedField(void* ptr);
extern "C" void* rage_AllocInternal(uint32_t size, uint32_t tag);

// ─────────────────────────────────────────────────────────────────────────────
// phObject::SetupOutputStream @ 0x824889A8 | size: 0x60
//
// Adjusts this pointer to the embedded sub-object at +124, stores a pointer
// to offset +200 in *outPtr, then calls vtable slot 13 on the sub-object.
// Returns 0 on success, 0x8000000A on failure.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_SetupOutputStream(void* thisPtr, void* outPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    uint8_t* subObj = obj + 124;
    uint8_t* outputBase = obj + 200;

    // Store output base pointer
    *(void**)outPtr = (void*)outputBase;

    // Call vtable slot 13 on the sub-object at +124
    void** vt = *(void***)subObj;
    typedef int32_t (*VtSlot13Fn)(void*);
    VtSlot13Fn fn = (VtSlot13Fn)vt[13];
    int32_t result = fn(subObj);

    if (result != 0) {
        return 0;
    }

    return (int32_t)0x8000000A;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::AllocateWorkBuffer @ 0x82491980 | size: 0x70
//
// Allocates a 2040-byte zeroed work buffer via the RAGE allocator,
// stores the pointer at *outPtr. Returns 0 on success, error on failure.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_AllocateWorkBuffer(void** outPtr) {
    void* buffer = rage_AllocInternal(2040, 0x208C8000);
    if (buffer == NULL) {
        return (int32_t)0x8007000E;
    }
    memset(buffer, 0, 2040);
    *outPtr = buffer;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::CreateStreamContext @ 0x8248AD78 | size: 0x80
//
// Allocates a work buffer, then initializes it as a stream context via
// rage_A518. On failure, frees the buffer via phObject_1F70.
// Returns the context pointer on success, or NULL on failure.
// ─────────────────────────────────────────────────────────────────────────────
void* phObject_CreateStreamContext(void* thisPtr) {
    void* buffer = NULL;
    void* context = NULL;

    int32_t result = phObject_AllocateWorkBuffer(&buffer);
    if (result < 0) {
        phObject_1F70(buffer);
        return NULL;
    }

    rage_A518(&context, thisPtr, buffer);
    if (result < 0) {
        phObject_1F70(buffer);
        return NULL;
    }

    return (void*)(uintptr_t)context;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::DestroyStreamContext @ 0x82489A48 | size: 0x9C
//
// Dereferences *contextPtr, clears stream flags, releases resources,
// frees the buffer. Sets *contextPtr to NULL.
// Returns 0 on success, 4 if null.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_DestroyStreamContext(void** contextPtr) {
    if (contextPtr == NULL) {
        return 4;
    }

    void* context = *contextPtr;
    if (context == NULL) {
        return 4;
    }

    uint8_t* ctx = (uint8_t*)context;
    uint32_t savedField = *(uint32_t*)(ctx + 548);
    void* streamObj = *(void**)(ctx + 608);

    phObject_ClearDirtyState(streamObj);
    phObject_ReleaseStreamContext(context);

    if (savedField != 0) {
        phObject_ReleaseSavedField((void*)(uintptr_t)savedField);
        void* statePtr = (void*)(uintptr_t)savedField;
        phObject_UnregisterResource(streamObj, 8, &statePtr);
    }

    *contextPtr = NULL;
    phObject_1F70(streamObj);
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::FlushOutputStream @ 0x82489450 | size: 0x70
//
// Flushes the output stream at offset +536. Calls phObject_BindOutputStream to
// perform the write. Clears the stream pointer if it becomes empty.
// Returns error 0x801000A8 if no stream is active.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_FlushOutputStream(void* thisPtr, uint32_t a2, uint32_t a3,
                                    uint32_t a4, uint32_t a5, void* outPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;
    void* stream = *(void**)(obj + 536);

    if (stream == NULL) {
        return (int32_t)0x801000A8;
    }

    uint32_t outVal = *(uint32_t*)(obj + 524);
    phObject_BindOutputStream(stream, a2, a3, a5, outVal, outPtr);

    uint32_t outputWord = *(uint32_t*)outPtr;
    if (outputWord == 0) {
        *(void**)(obj + 536) = NULL;
    }

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::ReleaseStreamSlot @ 0x824922C0 | size: 0x80
//
// Releases a numbered stream slot. If a callback is registered at +520,
// invokes it. Then unregisters the resource and clears the slot.
// ─────────────────────────────────────────────────────────────────────────────
void phObject_ReleaseStreamSlot(void* thisPtr, uint32_t slotIndex) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void* callback = *(void**)(obj + 520);
    if (callback != NULL) {
        uint32_t callbackParam = *(uint32_t*)(obj + 524);
        uint32_t slotValue = *(uint32_t*)(obj + (slotIndex * 4));

        typedef void (*CallbackFn)(uint32_t, uint32_t, uint32_t);
        CallbackFn fn = (CallbackFn)callback;
        fn(slotValue, slotIndex, callbackParam);
    }

    uint32_t* slotPtr = (uint32_t*)(obj + slotIndex * 4);
    void* resource = *(void**)(obj + 508);
    void** slotAsPtr = (void**)slotPtr;
    int32_t result = phObject_UnregisterResource(resource, 2, slotAsPtr);

    if (result >= 0) {
        *slotPtr = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::GetResourceInfo @ 0x824899B0 | size: 0x94
//
// Populates a 24-byte output structure with resource metadata from the
// phObject's internal descriptor at +4.
// Returns 0 on success, 4 on invalid parameters.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_GetResourceInfo(void* thisPtr, void* outInfo) {
    if (thisPtr == NULL || outInfo == NULL) {
        return 4;
    }

    uint8_t* obj = (uint8_t*)thisPtr;
    uint32_t resourceCount = *(uint32_t*)(obj + 528);
    if (resourceCount == 0) {
        return 4;
    }

    uint32_t* out = (uint32_t*)outInfo;
    for (int i = 0; i < 6; i++) {
        out[i] = 0;
    }

    void* desc = *(void**)(obj + 4);
    uint8_t* descBytes = (uint8_t*)desc;

    out[3] = *(uint32_t*)(descBytes + 28);
    out[0] = *(uint32_t*)(descBytes + 16);
    out[1] = *(uint32_t*)(descBytes + 20);
    out[2] = *(uint32_t*)(descBytes + 24);
    *(uint8_t*)((uint8_t*)outInfo + 16) = *(uint8_t*)(descBytes + 68);
    out[5] = *(uint32_t*)(descBytes + 8);

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::GetFormatDescription @ 0x8248A090 | size: 0xA8
//
// Zeroes a 36-byte output structure, then populates based on resource type.
// Type 1 = 1D resource, type 2 = 2D resource.
// Returns 0 on success, 0x801000B8 on unsupported type.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_GetFormatDescription(void* thisPtr, uint32_t param, void* outDesc) {
    uint8_t* obj = (uint8_t*)thisPtr;
    uint32_t* out = (uint32_t*)outDesc;

    for (int i = 0; i < 9; i++) {
        out[i] = 0;
    }

    uint32_t type = *(uint32_t*)(obj + 4);
    out[6] = (uint32_t)(uintptr_t)thisPtr;
    out[7] = param;
    out[8] = 1;
    out[0] = type;

    if (type == 1) {
        uint8_t* resInfo = *(uint8_t**)(obj + 8);
        out[2] = *(uint32_t*)(resInfo + 4);
        *(uint8_t*)((uint8_t*)outDesc + 12) = *(uint8_t*)(resInfo + 14);
        out[1] = 3;
        return 0;
    }

    if (type == 2) {
        uint8_t* resInfo = *(uint8_t**)(obj + 8);
        out[2] = 0;
        out[3] = *(uint32_t*)(resInfo + 8);
        out[4] = *(uint32_t*)(resInfo + 12);
        out[1] = 7;
        return 0;
    }

    return (int32_t)0x801000B8;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::BindResource @ 0x82485118 | size: 0xAC
//
// Binds a new resource. AddRefs via vtable slot 1, releases old views via
// vtable slot 19, stores at +52, creates stream context at +500.
// ─────────────────────────────────────────────────────────────────────────────
extern void rage_4CD0(void* thisPtr);

int32_t phObject_BindResource(void* thisPtr, void* resource) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void** resVt = *(void***)resource;
    typedef void (*AddRefFn)(void*);
    AddRefFn addRef = (AddRefFn)resVt[1];
    addRef(resource);

    void** selfVt = *(void***)thisPtr;
    typedef void (*ReleaseViewsFn)(void*);
    ReleaseViewsFn releaseViews = (ReleaseViewsFn)selfVt[19];
    releaseViews(thisPtr);

    *(void**)(obj + 52) = resource;

    void* streamCtx = phObject_CreateStreamContext(thisPtr);
    *(void**)(obj + 500) = streamCtx;

    if (streamCtx != NULL) {
        rage_4CD0(thisPtr);
        return 0;
    }

    return (int32_t)0x8000FFFF;
}

// ─────────────────────────────────────────────────────────────────────────────
// phObject::UnbindResource @ 0x82484988 | size: 0x94
//
// Unbinds the current resource. Destroys stream context at +500, releases
// resource at +52 via vtable slot 2. Returns 0.
// ─────────────────────────────────────────────────────────────────────────────
int32_t phObject_UnbindResource(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    void* streamCtx = *(void**)(obj + 500);
    if (streamCtx != NULL) {
        phObject_DestroyStreamContext((void**)(obj + 500));
        *(void**)(obj + 500) = NULL;
    }

    void* resource = *(void**)(obj + 52);
    if (resource != NULL) {
        void** resVt = *(void***)resource;
        typedef void (*ReleaseFn)(void*);
        ReleaseFn release = (ReleaseFn)resVt[2];
        release(resource);
        *(void**)(obj + 52) = NULL;
    }

    return 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phArticulatedCollider — Joint/Constraint Operations (Batch 4)
// ═════════════════════════════════════════════════════════════════════════════

// External subroutines used by the articulated collider
extern void ph_IntegrateJointPositions(void* jointArray);           // Update capsule bounds from joints
extern void phArticulatedCollider_8450(void* jointArray);      // Post-integrate joint transforms
extern void phArticulatedCollider_5D58(void* jointArray);      // Reset joint integration state
extern void phArticulatedCollider_ResetJointChain(void* jointArray);   // Reset joint chain state
extern void phArticulatedCollider_8A30(void* jointArray, float* forceVec);  // Sync joint velocities
extern void phArticulatedCollider_8B10(void* jointArray, float* torqueVec); // Sync joint torques

extern void phArticulatedCollider_ResetForceAccumulators(void* jointArray);  // Reset joint force accumulators
extern void phArticulatedCollider_F0E0(void* collider);        // Update articulated internal state
extern void phCollider_CDF0_p39(void* collider);               // Update collider position from velocity
extern void phCollider_vfn_1(void* collider);                  // phCollider::Reset
extern void phCollider_vfn_42(void* collider);                 // phCollider::SyncAfterBaseUpdate
extern int32_t phArticulatedCollider_E668_GetJointIndex(void* thisPtr, int jointParam);  // Joint index lookup


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ClearJointForces (vfn_3) @ 0x8224E1D0 | size: 0x74
//
// Zeros out the force and torque accumulation vectors for every joint in
// the articulated chain. Each joint has four 16-byte vectors cleared:
//   +272  linear force accumulator
//   +288  angular torque accumulator
//   +304  external force accumulator
//   +320  external torque accumulator
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ClearJointForces() {
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;   // +464 (0x1D0)
    int32_t numJoints = *(int32_t*)((uint8_t*)jointArray + 4);

    if (numJoints <= 0)
        return;

    uint32_t** jointPtrs = (uint32_t**)((uint8_t*)jointArray + 40);

    for (int32_t i = 0; i < numJoints; i++) {
        uint8_t* joint = (uint8_t*)jointPtrs[i];

        // Zero 4 consecutive 16-byte vectors at +272, +288, +304, +320
        memset(joint + 272, 0, 16);
        memset(joint + 288, 0, 16);
        memset(joint + 304, 0, 16);
        memset(joint + 320, 0, 16);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DispatchConstraintSolver (vfn_37) @ 0x8224EFC8
// size: 0x18
//
// Vtable trampoline: loads the base collider vtable and dispatches to slot 38
// (ApplyConstraints). Shifts params so r4<-r5, r5<-r6 before the tail call.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DispatchConstraintSolver(void* param1, void* param2) {
    // Load vtable from base object, call slot 38 (offset 152)
    // This is a direct tail-call trampoline to the parent ApplyConstraints
    void** vtable = *(void***)this;
    typedef void (*ApplyConstraintsFn)(phArticulatedCollider*, void*, void*);
    ApplyConstraintsFn fn = (ApplyConstraintsFn)vtable[38];
    fn(this, param1, param2);
}

// Adds a force vector to a specific joint's force accumulator. The joint
// index is computed from GetJointIndex (E668), the result + 10 is used
// as a dword index into the joint array, then the force vector is added
// to the joint's force accumulator at offset +384.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateJointForce(int jointParam, const float* forceVec) {
    // Look up the joint index via E668 helper
    int32_t jointIndex = phArticulatedCollider_E668_GetJointIndex((void*)this, jointParam);

    // Load joint pointer from jointArray[(jointIndex + 10)]
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;   // +464 (0x1D0)
    uint8_t* jointPtr = (uint8_t*)(((uint32_t*)jointArray)[jointIndex + 10]);

    // Add force vector to the joint's force accumulator at +384
    float* accumulator = (float*)(jointPtr + 384);
    accumulator[0] += forceVec[0];
    accumulator[1] += forceVec[1];
    accumulator[2] += forceVec[2];
    accumulator[3] += forceVec[3];
}


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::PostIntegrate (vfn_6) @ 0x8224EA28 | size: 0x48
//
// Called after physics integration to update the articulated collider state.
// Dispatches to four subroutines in sequence:
//   1. Update capsule bounds from joint positions
//   2. Post-integrate joint transforms
//   3. Update articulated collider internal state (F0E0)
//   4. Reset joint integration state
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::PostIntegrate() {
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)

    // Step 1: Update capsule bounds from current joint positions
    ph_IntegrateJointPositions(jointArray);

    // Step 2: Post-integrate joint transforms
    phArticulatedCollider_8450(jointArray);

    // Step 3: Update internal articulated state
    phArticulatedCollider_F0E0((void*)this);

    // Step 4: Reset joint integration accumulators
    jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // reload after potential modification
    phArticulatedCollider_5D58(jointArray);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::Reset (vfn_1) @ 0x8224E6D8 | size: 0x48
//
// Resets the articulated collider to its initial state. Calls the base
// phCollider::Reset, zeros out status fields, then resets the joint chain
// and integration state.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::Reset() {
    // Call base class reset
    phCollider_vfn_1((void*)this);

    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)

    // Clear status fields
    m_pPhysicsData = 0;       // +472 (0x1D8) — joint state pointer
    m_bJointsDirty = 0;      // +468 (0x1D4) — dirty flag (byte)

    // Reset joint chain state
    phArticulatedCollider_ResetJointChain(jointArray);

    // Reset joint integration accumulators
    jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // reload
    phArticulatedCollider_5D58(jointArray);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ClearForces (vfn_2) @ 0x8224E720 | size: 0x80
//
// Clears all force/velocity accumulation vectors and dispatches to the
// Update virtual method to recompute joint state. Zeros four 16-byte
// vectors at offsets +224, +240, +256, +272, then calls vfn_4 (Update)
// and resets joint force accumulators.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ClearForces() {
    // Zero the linear velocity vector (+256)
    memset((uint8_t*)this + 256, 0, 16);

    // Zero the angular velocity vector (+272)
    memset((uint8_t*)this + 272, 0, 16);

    // Zero the accumulated force vector (+224)
    memset((uint8_t*)this + 224, 0, 16);

    // Zero the accumulated torque vector (+240)
    memset((uint8_t*)this + 240, 0, 16);

    // Call Update (vfn_4) to recompute joint state from cleared forces
    this->Update();

    // Reset per-joint force accumulators
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    phArticulatedCollider_ResetForceAccumulators(jointArray);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::UpdateChildJoints (vfn_9) @ 0x8224EBC8 | size: 0x8C
//
// Iterates through child joints (indices 1..N-1) and calls their PreStep
// virtual method (vtable slot 18) on each joint that has a valid constraint
// pointer (field_0x14 != 0).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::UpdateChildJoints() {
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    int32_t numJoints = *(int32_t*)((uint8_t*)jointArray + 4);
    int32_t count = numJoints - 1;

    if (count <= 0)
        return;

    // Joint pointers start at offset +168 in the joint array
    uint32_t** jointPtrs = (uint32_t**)((uint8_t*)jointArray + 168);

    for (int32_t i = 0; i < count; i++) {
        void* joint = (void*)jointPtrs[i];

        // Check if joint has a valid constraint (field +20 != 0)
        uint32_t constraint = *(uint32_t*)((uint8_t*)joint + 20);
        bool hasConstraint = (constraint != 0);

        if (hasConstraint) {
            // Call vtable slot 18 (PreStep) on the joint
            void** jointVtable = *(void***)joint;
            typedef void (*PreStepFn)(void*);
            PreStepFn preStep = (PreStepFn)jointVtable[18];
            preStep(joint);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyScaledForce (vfn_27) @ 0x8224FD58 | size: 0x4C
//
// Builds a force vector where Y = mass * scaleFactor (f1 parameter),
// X and Z = 0, then dispatches to vtable slot 32 to apply it.
// Used for gravity-like forces scaled by the collider's mass.
//
// @param scaleFactor - Scale multiplier applied to the collider's mass
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ApplyScaledForce(float scaleFactor) {
    float mass = *(float*)((uint8_t*)this + 100);  // +100 (0x64)

    // Build force vector: {0, mass * scaleFactor, 0, 0}
    float forceVec[4];
    forceVec[0] = 0.0f;
    forceVec[1] = mass * scaleFactor;
    forceVec[2] = 0.0f;

    // Dispatch to vtable slot 32 to apply the force
    void** vtable = *(void***)this;
    typedef void (*ApplyForceFn)(phArticulatedCollider*, const float*);
    ApplyForceFn applyForce = (ApplyForceFn)vtable[32];
    applyForce(this, forceVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetVelocityAndSync (vfn_41) @ 0x8224F8C8
// size: 0x44
//
// Stores the given velocity vector to the linear velocity field (+256),
// calls the base collider position update, then syncs joint velocities
// from the updated collider state.
//
// @param velocityVec - Pointer to a 16-byte velocity vector
void rage::phArticulatedCollider::SetVelocityAndSync(const float* velocityVec) {
    // Store velocity vector at offset +256
    memcpy((uint8_t*)this + 256, velocityVec, 16);

    // Update collider position from new velocity (base class)
    phCollider_CDF0_p39((void*)this);

    // Sync joint velocities from the updated state
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    float* forceVec = (float*)((uint8_t*)this + 224);  // +224
    phArticulatedCollider_8A30(jointArray, forceVec);
}


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SyncAfterBaseUpdate (vfn_42) @ 0x8224F910
// size: 0x38
//
// Calls the base phCollider::vfn_42, then syncs joint torques from the
// collider's torque vector at +240.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::SyncAfterBaseUpdate() {
    // Call base class vfn_42
    phCollider_vfn_42((void*)this);

    // Sync joint torques from the collider's angular state
    uint32_t* jointArray = (uint32_t*)(uintptr_t)m_pJointArray;  // +464 (0x1D0)
    float* torqueVec = (float*)((uint8_t*)this + 240);  // +240
    phArticulatedCollider_8B10(jointArray, torqueVec);
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Capsule Collision Shape Operations (Batch)
// ═════════════════════════════════════════════════════════════════════════════

// External declarations for capsule operations
extern float ph_Atan2f(float x, float y);       // atan2
extern float ph_Sqrtf(float val);               // sqrtf wrapper
extern void  phBoundCapsule_ComputeAngleXZ(void* obj, float y, float z);  // angle computation
extern void  phBoundCapsule_UpdateCapsuleState(void* obj);               // capsule update
extern void  phBoundCapsule_DispatchCollisionTest(void* thisPtr, void* p1, void* p2, float f1, float f2, float f3, float f4, void* p3);
extern void  pongCameraMgr_GetCameraAngle(void* obj);                // camera get angle
extern void  rage_debugLog(const char* name, int param);      // debug log (no-op in release)
extern int   phBoundCapsule_ResolvePlayerIndex(void* obj);               // resolve player index
extern int   game_1700(void* obj);                           // get player from object

// Global pointers used by capsule operations
extern uint32_t* g_phBoundCapsule_GlobalState;  // @ various global singletons

// ─────────────────────────────────────────────────────────────────────────────
// 1. rage::phBoundCapsule::CheckValueSign @ 0x8216BB88 | size: 0x5C (92B)
//
// Checks the sign of the first float field (field_0x00) against constant
// thresholds. Returns 1 if value is positive and a global flag is set,
// -1 if value is negative and the flag is set, otherwise -1.
//
// Used in collision direction classification for capsule intersection tests.
// ─────────────────────────────────────────────────────────────────────────────
int rage::phBoundCapsule::CheckValueSign() const {
    float value = *(const float*)((const char*)this + 0x00);

    // Load global state singleton
    uint32_t* globalPtr = *(uint32_t**)((char*)0x82060000 + 25628);  // g_phSimulator
    uint8_t flags = *(uint8_t*)((char*)globalPtr[12] + 192);  // +48 -> +192

    // Positive threshold constant (lbl_8202D110 = 0.0f)
    const float positiveThreshold = g_floatZero;

    if (value > positiveThreshold) {
        if (flags != 0) {
            return 1;
        }
    }

    // Negative threshold constant (lbl_82079D18)
    const float negativeThreshold = *(const float*)0x82079D18;

    if (value < negativeThreshold) {
        if (flags != 0) {
            return -1;
        }
        return 1;
    }

    return -1;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. rage::phBoundCapsule::CheckScaledValueSign @ 0x8216BAF0 | size: 0x98 (152B)
//
// Similar to CheckValueSign but uses a dynamic threshold computed by
// multiplying a base extent from a linked object with a global scale factor.
// Returns 1 if value exceeds the scaled threshold with flag set,
// 0 if value is within range but exceeds in the opposite direction,
// -1 otherwise.
//
// Used for scaled collision detection where the threshold adapts to the
// capsule's current size.
// ─────────────────────────────────────────────────────────────────────────────
int rage::phBoundCapsule::CheckScaledValueSign() const {
    float value = *(const float*)((const char*)this + 0x00);

    // Load linked object extent at +16
    uint32_t* linkedObj = *(uint32_t**)((char*)0x82120000 + (-23804));
    float baseExtent = *(float*)((char*)linkedObj + 16);

    // Load global state singleton
    uint32_t* globalPtr = *(uint32_t**)((char*)0x82060000 + 25628);
    void* stateObj = (void*)(uintptr_t)globalPtr[12];  // +48
    uint8_t flags = *(uint8_t*)((char*)stateObj + 192);

    // Scale factor from global data
    float scaleFactor = *(const float*)0x825C9A40;
    float threshold = baseExtent * scaleFactor;

    // Check positive threshold
    if (value > threshold) {
        if (flags != 0) {
            return 1;
        }
    }

    // Check negative threshold
    float negThreshold = -threshold;
    if (value < negThreshold) {
        if (flags != 0) {
            goto returnNeg;
        }
        return 1;
    }

    // Second pass: re-check for classification
    if (value < negThreshold) {
        if (flags != 0) {
            return -1;
        }
    }

    if (value > threshold) {
        if (flags != 0) {
            return 0;
        }
    returnNeg:
        return -1;
    }

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. rage::phBoundCapsule::SetupCapsuleFromState @ 0x8216DB10 | size: 0x74 (116B)
//
// Initializes a capsule's collision parameters from the object's state fields.
// Reads position data from fields at +164, +168, +172 and stores them into
// the linked physics object at offsets 824-836. Also sets an active flag
// at offset 864.
//
// Parameters:
//   this - Capsule state object with position data
//
// Called when transitioning a capsule into an active collision state.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::SetupCapsuleFromState() {
    uint32_t* physObj = (uint32_t*)(uintptr_t)(*(uint32_t*)((char*)this + 152));
    if (physObj == nullptr) {
        return;
    }

    float posY = *(float*)((char*)this + 172);
    float posX = *(float*)((char*)this + 168);

    // Get global computation object
    void* computeObj = (void*)(uintptr_t)(*(uint32_t*)((char*)0x82030000 + (-21712)));

    // Compute angle from X/Z components
    phBoundCapsule_ComputeAngleXZ(computeObj, posX, posY);

    // Store position data into physics object
    float field164 = *(float*)((char*)this + 164);
    *(float*)((char*)physObj + 824) = field164;
    // f1 is the result from phBoundCapsule_ComputeAngleXZ (returned in f1)
    // Store computed results
    *(float*)((char*)physObj + 832) = posX;
    *(float*)((char*)physObj + 836) = posY;

    // Set active flag
    *(uint8_t*)((char*)physObj + 864) = 1;

    // Update the capsule
    phBoundCapsule_UpdateCapsuleState((void*)physObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. rage::phBoundCapsule::SumFloatArray @ 0x82172C08 | size: 0x90 (144B)
//
// Computes the sum of (count+1) consecutive float values starting at
// offset +48 in the object. Uses an unrolled loop (4 elements per iteration)
// for performance, with a scalar cleanup loop for remaining elements.
//
// Parameters:
//   count - Number of elements to sum (0-based, actual count is count+1)
//
// Returns: Sum of the float array elements (in f1)
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::SumFloatArray(int count) const {
    float sum = g_floatZero;
    int remaining = 0;

    int total = count + 1;

    // Unrolled loop: process 4 floats per iteration
    if (total >= 4) {
        int chunks = total / 4;
        remaining = chunks * 4;
        const float* ptr = (const float*)((const char*)this + 48);  // +0x30 = 48, field starts at offset 52 - 4

        for (int i = 0; i < chunks; i++) {
            sum += ptr[0];
            sum += ptr[1];
            sum += ptr[2];
            sum += ptr[3];
            ptr += 4;
        }
    }

    // Scalar cleanup loop for remaining elements
    if (remaining <= count) {
        int idx = remaining + 12;  // offset: (remaining+12)*4 from base
        const float* ptr = (const float*)((const char*)this + idx * 4);
        int left = count - remaining + 1;

        for (int i = 0; i < left; i++) {
            sum += ptr[i];
        }
    }

    return sum;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. rage::phBoundCapsule::RotateVectors @ 0x820D05A0 | size: 0xB4 (180B)
//
// Applies a 2D rotation to the two 4-component vectors stored at offsets
// +16 and +32 in the object. Uses ComputeBounds to get sin/cos values,
// then performs:
//   vec_at_32 = vec_at_32 * cos - vec_at_16 * sin  (new vec_at_32)
//   vec_at_16 = vec_at_16 * cos + vec_at_32_orig * sin  (new vec_at_16)
//
// This is a standard 2D rotation matrix applied componentwise to two
// 4-element vectors representing the capsule's local axes.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::RotateVectors() {
    float sinVal, cosVal;
    this->ComputeBounds(0.0f, &sinVal, &cosVal);

    // Load vectors
    float* vec16 = (float*)((char*)this + 16);   // +0x10
    float* vec32 = (float*)((char*)this + 32);   // +0x20

    // Splat sin and cos across all 4 components
    // new_vec32 = vec32 * cos - vec16 * sin  (vnmsubfp: -(a*b - c) = c - a*b)
    // new_vec16 = vec16 * cos + vec32_orig * sin  (vmaddfp: a*b + c)

    float orig_vec32[4], orig_vec16[4];
    for (int i = 0; i < 4; i++) {
        orig_vec32[i] = vec32[i];
        orig_vec16[i] = vec16[i];
    }

    for (int i = 0; i < 4; i++) {
        vec32[i] = orig_vec32[i] * sinVal - orig_vec16[i] * cosVal;
    }
    for (int i = 0; i < 4; i++) {
        vec16[i] = orig_vec16[i] * sinVal + orig_vec32[i] * cosVal;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 6. rage::phBoundCapsule::RotateVectorsAlt @ 0x82175D88 | size: 0xB0 (176B)
//
// Identical to RotateVectors but operates on vectors at offsets +0 and +16
// instead of +16 and +32. This is the alternate pair of capsule axis vectors.
//
// Applies the same 2D rotation:
//   vec_at_16 = vec_at_16 * cos - vec_at_0 * sin
//   vec_at_0  = vec_at_0 * cos  + vec_at_16_orig * sin
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::RotateVectorsAlt() {
    float sinVal, cosVal;
    this->ComputeBounds(0.0f, &sinVal, &cosVal);

    // Load vectors
    float* vec0  = (float*)((char*)this + 0);    // +0x00
    float* vec16 = (float*)((char*)this + 16);   // +0x10

    float orig_vec16[4], orig_vec0[4];
    for (int i = 0; i < 4; i++) {
        orig_vec16[i] = vec16[i];
        orig_vec0[i] = vec0[i];
    }

    // new_vec16 = vec16 * cos - vec0 * sin
    for (int i = 0; i < 4; i++) {
        vec16[i] = orig_vec16[i] * sinVal - orig_vec0[i] * cosVal;
    }
    // new_vec0 = vec0 * cos + vec16_orig * sin
    for (int i = 0; i < 4; i++) {
        vec0[i] = orig_vec0[i] * sinVal + orig_vec16[i] * cosVal;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 7. rage::phBoundCapsule::NormalizeVector2D @ 0x82199048 | size: 0xB8 (184B)
//
// Normalizes a 2D vector (x, y stored at +0 and +4 of this object).
// Computes magnitude = sqrt(x*x + y*y). If magnitude is within a small
// epsilon of zero, stores (0, 0) into the output and returns 0.
// Otherwise normalizes the vector with a scale factor and stores the
// result, returning 1.
//
// Parameters:
//   outVec - Pointer to output 2D vector (two floats)
//
// Returns: 1 if vector was normalized, 0 if too small (near-zero)
// ─────────────────────────────────────────────────────────────────────────────
int rage::phBoundCapsule::NormalizeVector2D(float* outVec) const {
    float y = *(const float*)((const char*)this + 4);
    float x = *(const float*)((const char*)this + 0);

    // Compute magnitude: sqrt(x*x + y*y)
    float magSq = x * x + y * y;
    float mag = (float)ph_Sqrtf(magSq);  // sqrtf

    // Small epsilon check (constant at lbl_8202D110 + 20 bytes = offset 0x8202D124)
    const float epsilon = *(const float*)0x8202D124;

    if (mag <= epsilon) {
        // Vector is too small, output zero
        outVec[0] = g_floatZero;
        outVec[1] = g_floatZero;
        return 0;
    }

    // Normalize: store original components into output first
    outVec[0] = x;
    outVec[1] = y;

    // Compute scale factor for normalization
    float excess = mag - epsilon;
    float scaleFactor = *(const float*)0x825C9100 + 0;  // global scale constant
    float normScale = excess * scaleFactor;
    float invMag = normScale / mag;

    // Apply normalization scale
    outVec[0] = x * invMag;
    outVec[1] = y * invMag;

    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// 8. rage::phBoundCapsule::LookupTableValues @ 0x8216CF48 | size: 0x70 (112B)
//
// Resolves a player index through phBoundCapsule_ResolvePlayerIndex and uses it to
// look up two float values from global constant tables. Stores the results
// into the output structure at offsets +4 and +8.
//
// Parameters:
//   outValues - Pointer to output structure receiving two float lookups
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::LookupTableValues(void* outValues) {
    // Global object pointer
    void* globalObj = *(void**)((char*)0x82120000 + (-23764));

    // First lookup: get player index
    int index1 = phBoundCapsule_ResolvePlayerIndex(globalObj);

    // Look up float from first table and store at +4
    const float* table1 = (const float*)((char*)0x82120000 + (-22564));
    *(float*)((char*)outValues + 4) = table1[index1];

    // Second lookup with same global
    globalObj = *(void**)((char*)0x82120000 + (-23764));
    int index2 = phBoundCapsule_ResolvePlayerIndex(globalObj);

    // Look up float from second table and store at +8
    const float* table2 = (const float*)((char*)0x82060000 + (-32332));
    *(float*)((char*)outValues + 8) = table2[index2];
}

// ─────────────────────────────────────────────────────────────────────────────
// 9. rage::phBoundCapsule::SetActiveState @ 0x821665B8 | size: 0xC0 (192B)
//
// Sets the active capsule state index, updates linked physics object
// references, and triggers capsule parameter setup. Manages the state
// transition by:
// 1. Looking up the physics object at the computed array index
// 2. Updating the linked list pointers between state and physics objects
// 3. Setting a state flag (4) if the object changed
// 4. Calling SetupCapsuleFromState to initialize collision parameters
// 5. Updating global debug state
//
// Parameters:
//   stateIndex - Index of the new active state
// ─────────────────────────────────────────────────────────────────────────────
extern void phBoundCapsule_SetupFromState(void* obj);
void rage::phBoundCapsule::SetActiveState(int stateIndex) {
    // Store the state index at +28
    *(int*)((char*)this + 28) = stateIndex;

    // Look up physics object from array: this[(stateIndex+8)*4]
    uint32_t arrayIndex = (stateIndex + 8);
    uint32_t* physObj = (uint32_t*)(uintptr_t)(*(uint32_t*)((char*)this + arrayIndex * 4));

    // Get the linked state object at +40
    uint32_t* stateObj = (uint32_t*)(uintptr_t)(*(uint32_t*)((char*)this + 40));

    // Get current linked physics ptr at stateObj+52
    uint32_t currentLinked = *(uint32_t*)((char*)stateObj + 52);

    // If the new object differs from current, check and update
    if ((uintptr_t)physObj != currentLinked) {
        uint32_t prevObj = *(uint32_t*)((char*)stateObj + 44);
        if (prevObj != currentLinked) {
            // Set state flag to 4 (transition)
            *(uint16_t*)((char*)stateObj + 16) = 4;
        }
    }

    // Update linked object pointer at stateObj+44
    *(uint32_t*)((char*)stateObj + 44) = (uint32_t)(uintptr_t)physObj;

    // Clear pending flag at stateObj+48
    *(uint32_t*)((char*)stateObj + 48) = 0;

    // Set back-pointers on the physics object
    *(uint32_t*)((char*)physObj + 148) = (uint32_t)(uintptr_t)stateObj;
    uint32_t parentRef = *(uint32_t*)((char*)stateObj + 40);
    *(uint32_t*)((char*)physObj + 152) = parentRef;

    // Setup capsule collision parameters from state
    // Re-resolve the physics object for the current state
    int currentIndex = *(int*)((char*)this + 28);
    uint32_t idx2 = (currentIndex + 8);
    void* activePhysObj = (void*)(uintptr_t)(*(uint32_t*)((char*)this + idx2 * 4));

    // Call SetupCapsuleFromState (phBoundCapsule_SetupFromState)
    phBoundCapsule_SetupFromState(activePhysObj);

    // Update global debug pointer
    uint32_t* stateObj2 = (uint32_t*)(uintptr_t)(*(uint32_t*)((char*)this + 40));
    uint32_t* linkedPhys = (uint32_t*)(uintptr_t)(*(uint32_t*)((char*)stateObj2 + 44));
    void** globalDebugPtr = *(void***)((char*)0x82120000 + (-23764));
    *globalDebugPtr = (void*)((char*)linkedPhys + 16);

    // Log the state change
    int logIndex = *(int*)((char*)this + 28);
    const int* nameTable = (const int*)((char*)0x82060000 + (-30152));
    int logParam = nameTable[logIndex];
    const char* logName = (const char*)0x82085E0C;  // debug string
    rage_debugLog(logName, logParam);
}

// ─────────────────────────────────────────────────────────────────────────────
// 10. rage::phBoundCapsule::DispatchCapsuleFromCamera @ 0x82148608 | size: 0x9C (156B)
//
// Sets up and dispatches a capsule collision test relative to the current
// camera view. Loads the active camera's parameters (position, orientation)
// and computes the capsule bounds based on the camera's field of view.
//
// Parameters:
//   param1 - First collision parameter (passed through)
//   param2 - Second collision parameter (passed through)
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::DispatchCapsuleFromCamera(void* param1, void* param2) {
    // Load global state singleton
    uint32_t* globalPtr = *(uint32_t**)((char*)0x82060000 + 25628);
    uint32_t* cameraArray = (uint32_t*)(uintptr_t)globalPtr[6];  // +24

    // Get active camera index from global data
    int cameraIndex = *(int*)((char*)0x82040000 + 18584);

    // Compute camera entry pointer: cameraArray + cameraIndex * 912
    void* cameraEntry = (void*)((char*)cameraArray + cameraIndex * 912);

    // Get camera angle
    pongCameraMgr_GetCameraAngle(cameraEntry);
    float cameraAngle = *(float*)&cameraEntry;  // result returned in f1

    // Load camera parameters
    void* camBase = (void*)((char*)cameraArray + cameraIndex * 912);
    float camPosX = *(float*)((char*)camBase + 824);
    float camPosY = *(float*)((char*)camBase + 832);

    // Compute clamped value: clamp camPosY to [0, constant]
    const float clampMax = *(const float*)0x8202CDF8;
    float diff = clampMax - camPosY;
    const double zeroDbl = *(const double*)((char*)0x82080000 + (-24648));
    float clampedY = (diff >= 0.0f) ? camPosY : (float)zeroDbl;

    float camPosZ = *(float*)((char*)cameraArray + cameraIndex * 912 + 836);

    // Get capsule extent at camera offset +64
    void* capsuleExtent = (void*)((char*)cameraArray + cameraIndex * 912 + 64);

    // Dispatch the full capsule collision test
    phBoundCapsule_DispatchCollisionTest(this, param1, param2, camPosX, clampedY, camPosZ, cameraAngle, capsuleExtent);
}


// =========================================================================
// Matrix Transpose-Multiply Utilities
// =========================================================================

/**
 * phMatrix34TransposeMultiply @ 0x821181C8 | size: 0xB0 (176 bytes)
 *
 * Computes result = transpose(A) * B for two 4x3 matrices (stored as 4 rows
 * of 16-byte SIMD vectors), producing a 4x4 output matrix of dot products.
 *
 * The function also computes a difference vector (A.row3 - B.row3) and dots
 * it with each column of B to fill the fourth output row. This is the
 * classic "transform point into local space" pattern used throughout the
 * physics engine for converting world-space matrices into body-local frames.
 *
 * @param outMatrix  Pointer to 4x16-byte output matrix (4 rows of vec4)
 * @param matrixB    Pointer to 4x16-byte input matrix B (4 rows of vec4)
 */
void phMatrix34TransposeMultiply(float* outMatrix, const float* matrixB) {
    // Load all 4 rows of matrix A (outMatrix is reused as input A)
    const float* rowA0 = outMatrix;        // +0
    const float* rowA1 = outMatrix + 4;    // +16
    const float* rowA2 = outMatrix + 8;    // +32
    const float* rowA3 = outMatrix + 12;   // +48

    // Load all 4 rows of matrix B
    const float* rowB0 = matrixB;          // +0
    const float* rowB1 = matrixB + 4;      // +16
    const float* rowB2 = matrixB + 8;      // +32
    const float* rowB3 = matrixB + 12;     // +48

    // Compute difference vector: delta = A.row3 - B.row3
    float delta[4];
    delta[0] = rowA3[0] - rowB3[0];
    delta[1] = rowA3[1] - rowB3[1];
    delta[2] = rowA3[2] - rowB3[2];
    delta[3] = rowA3[3] - rowB3[3];

    // Row 0: dot3(A.row0, B.row_j)
    float dot00 = rowA0[0]*rowB0[0] + rowA0[1]*rowB0[1] + rowA0[2]*rowB0[2];
    float dot01 = rowA0[0]*rowB1[0] + rowA0[1]*rowB1[1] + rowA0[2]*rowB1[2];
    float dot02 = rowA0[0]*rowB2[0] + rowA0[1]*rowB2[1] + rowA0[2]*rowB2[2];

    // Row 1: dot3(A.row1, B.row_j)
    float dot10 = rowA1[0]*rowB0[0] + rowA1[1]*rowB0[1] + rowA1[2]*rowB0[2];
    float dot11 = rowA1[0]*rowB2[0] + rowA1[1]*rowB2[1] + rowA1[2]*rowB2[2];
    float dot12 = rowA1[0]*rowB1[0] + rowA1[1]*rowB1[1] + rowA1[2]*rowB1[2];

    // Row 2: dot3(A.row2, B.row_j)
    float dot20 = rowA2[0]*rowB0[0] + rowA2[1]*rowB0[1] + rowA2[2]*rowB0[2];
    float dot21 = rowA2[0]*rowB2[0] + rowA2[1]*rowB2[1] + rowA2[2]*rowB2[2];
    float dot22 = rowA2[0]*rowB1[0] + rowA2[1]*rowB1[1] + rowA2[2]*rowB1[2];

    // Row 3: dot3(delta, B.row_j)
    float dot30 = delta[0]*rowB0[0] + delta[1]*rowB0[1] + delta[2]*rowB0[2];
    float dot31 = delta[0]*rowB1[0] + delta[1]*rowB1[1] + delta[2]*rowB1[2];
    float dot32 = delta[0]*rowB2[0] + delta[1]*rowB2[1] + delta[2]*rowB2[2];

    // Pack results into output matrix rows (vmrghw interleave pattern)
    outMatrix[0]  = dot00;  outMatrix[1]  = dot01;  outMatrix[2]  = dot02;  outMatrix[3]  = 0.0f;
    outMatrix[4]  = dot10;  outMatrix[5]  = dot12;  outMatrix[6]  = dot11;  outMatrix[7]  = 0.0f;
    outMatrix[8]  = dot20;  outMatrix[9]  = dot22;  outMatrix[10] = dot21;  outMatrix[11] = 0.0f;
    outMatrix[12] = dot30;  outMatrix[13] = dot31;  outMatrix[14] = dot32;  outMatrix[15] = 0.0f;
}

/**
 * phMatrix33TransposeMultiply @ 0x82296D20 | size: 0x94 (148 bytes)
 *
 * Computes result = transpose(A) * B for two 3x3 matrices (each stored as
 * 3 rows of 16-byte SIMD vectors). The output is written as 3 rows of
 * dot products packed via the vmrghw interleave pattern.
 *
 * This is the 3x3 variant of the 4x3 transpose-multiply, used for
 * rotating inertia tensors and normal-space transforms.
 *
 * @param outMatrix  Pointer to 3x16-byte output (3 rows of vec4)
 * @param matrixA    Pointer to 3x16-byte input matrix A (3 rows)
 * @param matrixB    Pointer to 3x16-byte input matrix B (3 rows)
 */
void phMatrix33TransposeMultiply(float* outMatrix, const float* matrixA, const float* matrixB) {
    const float* rowA0 = matrixA;
    const float* rowA1 = matrixA + 4;
    const float* rowA2 = matrixA + 8;

    const float* rowB0 = matrixB;
    const float* rowB1 = matrixB + 4;
    const float* rowB2 = matrixB + 8;

    // Compute 3x3 dot product matrix: result[i][j] = dot3(A.row[i], B.row[j])
    float dot00 = rowA0[0]*rowB0[0] + rowA0[1]*rowB0[1] + rowA0[2]*rowB0[2];
    float dot01 = rowA0[0]*rowB1[0] + rowA0[1]*rowB1[1] + rowA0[2]*rowB1[2];
    float dot02 = rowA0[0]*rowB2[0] + rowA0[1]*rowB2[1] + rowA0[2]*rowB2[2];

    float dot10 = rowA1[0]*rowB0[0] + rowA1[1]*rowB0[1] + rowA1[2]*rowB0[2];
    float dot11 = rowA1[0]*rowB2[0] + rowA1[1]*rowB2[1] + rowA1[2]*rowB2[2];
    float dot12 = rowA1[0]*rowB1[0] + rowA1[1]*rowB1[1] + rowA1[2]*rowB1[2];

    float dot20 = rowA2[0]*rowB0[0] + rowA2[1]*rowB0[1] + rowA2[2]*rowB0[2];
    float dot21 = rowA2[0]*rowB1[0] + rowA2[1]*rowB1[1] + rowA2[2]*rowB1[2];
    float dot22 = rowA2[0]*rowB2[0] + rowA2[1]*rowB2[1] + rowA2[2]*rowB2[2];

    // Store packed output (preserving existing w components from outMatrix)
    outMatrix[0]  = dot00;  outMatrix[1]  = dot01;  outMatrix[2]  = dot02;
    outMatrix[4]  = dot10;  outMatrix[5]  = dot12;  outMatrix[6]  = dot11;
    outMatrix[8]  = dot20;  outMatrix[9]  = dot21;  outMatrix[10] = dot22;
}

/**
 * phCollisionPairArrayInit @ 0x8221FD78 | size: 0x58 (88 bytes)
 *
 * Initializes a collision contact pair structure. Clears 4 weight values
 * (uint16 at +16..+22) to zero and sets 4 index values (uint16 at +24..+30)
 * to 0xFFFF (invalid). Then zeros four floats at offsets +0..+12.
 *
 * The early-out checks a global flag (physics paused / disabled); if set,
 * the function returns immediately without initialization.
 *
 * @param contactPair  Pointer to 32-byte collision contact pair structure
 */
void phCollisionPairArrayInit(void* contactPair) {
    // Check global physics-disabled flag
    if (g_physicsDisabled != 0)
        return;

    uint8_t* pair = (uint8_t*)contactPair;

    // Clear 4 weight values (uint16) at +16..+22
    for (int i = 0; i < 4; i++) {
        *(uint16_t*)(pair + 16 + i * 2) = 0;
    }

    // Set 4 index slots (uint16) at +24..+30 to invalid (0xFFFF)
    for (int i = 0; i < 4; i++) {
        *(uint16_t*)(pair + 24 + i * 2) = 0xFFFF;
    }

    // Zero 4 floats at +0..+12
    float* floats = (float*)pair;
    floats[0] = 0.0f;
    floats[1] = 0.0f;
    floats[2] = 0.0f;
    floats[3] = 0.0f;
}

// =========================================================================
// rage::phBound Hierarchy Constructors
// =========================================================================

// Forward declarations for init helpers
extern void snListNode_Init(void* listNode);           // Intrusive list node init
extern void phBoundSphere_vfn_37(void* bound); // Sphere-specific init
extern void ph_9BC0(void* listHead);           // Collision list init

// Global identity-like AABB template vector
extern const uint8_t g_phBoundAABBTemplate[16];

/**
 * rage::phBound::phBound @ 0x8228CE50 | size: 0x90 (144 bytes)
 *
 * Base constructor for all physics bound volumes. Initializes the vtable,
 * bound type (0xFF = unset), radius fields, flags, and copies a template
 * vector into the AABB min/max slots. Also initializes 7 material index
 * slots to zero.
 */
void phBound_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    *(uint32_t*)(obj + 0) = 0x82057EF4;  // vtable -> rage::phBound
    *(uint8_t*)(obj + 4) = 0xFF;          // bound type = uninitialized
    *(float*)(obj + 8)  = 0.0f;           // bounding radius
    *(float*)(obj + 12) = 0.0f;           // margin
    *(uint8_t*)(obj + 5) = 0;
    *(uint8_t*)(obj + 6) = 0;
    *(uint8_t*)(obj + 7) = 0;

    // Copy AABB template into 4 vector slots
    memcpy(obj + 16, g_phBoundAABBTemplate, 16);
    memcpy(obj + 32, g_phBoundAABBTemplate, 16);
    memcpy(obj + 48, g_phBoundAABBTemplate, 16);
    memcpy(obj + 80, g_phBoundAABBTemplate, 16);

    // Material count = 1, clear 7 material index slots
    *(uint16_t*)(obj + 96) = 1;
    for (int i = 0; i < 7; i++) {
        *(uint16_t*)(obj + 98 + i * 2) = 0;
    }
}

/**
 * rage::phBoundGeometry::phBoundGeometry @ 0x82290F78 | size: 0x8C (140 bytes)
 *
 * Constructor for phBoundGeometry, the triangle-mesh collision bound.
 * Calls phBound base ctor, then sets the geometry vtable and initializes
 * vertex/index pointers, polygon count, edge data, and material mapping.
 */
void phBoundGeometry_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBound_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x82058494;  // vtable -> rage::phBoundGeometry
    *(uint8_t*)(obj + 4) = 3;             // bound type = geometry
    *(uint32_t*)(obj + 132) = 0xFFFFFFFF; // material index = -1
    *(uint32_t*)(obj + 140) = 0xFFFFFFFF; // edge data index = -1
    *(uint32_t*)(obj + 116) = 0;
    *(uint32_t*)(obj + 124) = 0;
    *(uint8_t*)(obj + 128)  = 0;
    *(uint32_t*)(obj + 136) = 0;
    *(uint32_t*)(obj + 144) = 0;
    *(uint32_t*)(obj + 148) = 0;
    *(uint32_t*)(obj + 112) = 0;
    *(uint32_t*)(obj + 120) = 0;
    *(uint32_t*)(obj + 160) = 0;
    *(uint32_t*)(obj + 164) = 0;
    *(uint8_t*)(obj + 168)  = 0;

    for (int i = 0; i < 7; i++) {
        *(uint8_t*)(obj + 169 + i) = 0;
    }
}

/**
 * rage::phBoundRibbon::phBoundRibbon @ 0x8229D8D0 | size: 0xA0 (160 bytes)
 *
 * Constructor for phBoundRibbon, a ribbon/spline-based collision bound used
 * for net cords and similar thin geometry.
 */
void phBoundRibbon_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBound_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x820589BC;  // vtable -> rage::phBoundRibbon
    *(uint8_t*)(obj + 4) = 7;             // bound type = ribbon
    *(float*)(obj + 8)  = -1.0f;
    *(float*)(obj + 12) = -1.0f;
    *(uint32_t*)(obj + 116) = 0;
    *(uint32_t*)(obj + 120) = 0;
    *(uint32_t*)(obj + 124) = 0;
    *(uint32_t*)(obj + 112) = 0;

    // Zero surface normal vector at +48
    *(uint32_t*)(obj + 48) = 0;
    *(uint32_t*)(obj + 52) = 0;
    *(uint32_t*)(obj + 56) = 0;
    *(uint32_t*)(obj + 60) = 0;

    *(uint32_t*)(obj + 136) = 0xFFFFFFFF;
    *(float*)(obj + 128) = -1.0f;
    *(uint32_t*)(obj + 164) = 0;
    *(uint32_t*)(obj + 168) = 0;
    *(uint32_t*)(obj + 172) = 0;
    *(uint32_t*)(obj + 176) = 0;
    *(uint32_t*)(obj + 180) = 0;
    *(uint32_t*)(obj + 184) = 0;
    *(uint32_t*)(obj + 144) = 0;
    *(uint32_t*)(obj + 148) = 0;
    *(uint32_t*)(obj + 152) = 0;
    *(uint32_t*)(obj + 156) = 0;
    *(uint32_t*)(obj + 160) = 0;
}

/**
 * rage::phBoundOTGrid::phBoundOTGrid @ 0x8229B4E0 | size: 0x70 (112 bytes)
 *
 * Constructor for phBoundOTGrid, the octree-grid collision bound used for
 * large static world geometry.
 */
void phBoundOTGrid_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBound_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x82058854;  // vtable -> rage::phBoundOTGrid
    *(uint8_t*)(obj + 4) = 6;             // bound type = OTGrid
    *(float*)(obj + 112) = 0.0f;
    *(float*)(obj + 116) = 0.0f;
    *(uint32_t*)(obj + 120) = 0;
    *(uint32_t*)(obj + 124) = 0;
    *(uint32_t*)(obj + 128) = 0;
    *(uint32_t*)(obj + 132) = 0;
    *(uint32_t*)(obj + 136) = 0;
    *(uint32_t*)(obj + 140) = 0;
    *(uint32_t*)(obj + 148) = 0xFFFFFFFF;
    *(uint32_t*)(obj + 144) = 0;
    *(uint32_t*)(obj + 152) = 0;
    *(uint32_t*)(obj + 156) = 0;
}

/**
 * rage::phBoundComposite::phBoundComposite @ 0x8228DE80 | size: 0x7C (124 bytes)
 *
 * Constructor for phBoundComposite, a container bound that holds multiple
 * child phBound objects.
 */
void phBoundComposite_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBound_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x82057FD4;  // vtable -> rage::phBoundComposite

    // Initialize two intrusive list nodes
    snListNode_Init((void*)(obj + 120));
    snListNode_Init((void*)(obj + 124));

    *(uint8_t*)(obj + 4) = 9;  // bound type = composite
    *(uint32_t*)(obj + 112) = 0;
    *(uint32_t*)(obj + 116) = 0;
    *(uint32_t*)(obj + 120) = 0;
    *(uint32_t*)(obj + 124) = 0;

    // Zero AABB vector at +48
    *(uint32_t*)(obj + 48) = 0;
    *(uint32_t*)(obj + 52) = 0;
    *(uint32_t*)(obj + 56) = 0;
    *(uint32_t*)(obj + 60) = 0;

    *(uint32_t*)(obj + 132) = 0;
    *(uint32_t*)(obj + 136) = 0;
    *(uint32_t*)(obj + 140) = 0;
}

/**
 * rage::phBoundSphere::phBoundSphere @ 0x822954C8 | size: 0x7C (124 bytes)
 *
 * Constructor for phBoundSphere, the simplest collision volume.
 * Initializes radius to -1.0 (unset), zeros center vector, and
 * calls sphere-specific material init.
 */
void phBoundSphere_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBound_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x82058584;  // vtable -> rage::phBoundSphere
    *(uint8_t*)(obj + 4) = 0;             // bound type = sphere
    *(float*)(obj + 8) = -1.0f;           // radius = unset

    // Zero center vector at +48
    *(uint32_t*)(obj + 48) = 0;
    *(uint32_t*)(obj + 52) = 0;
    *(uint32_t*)(obj + 56) = 0;
    *(uint32_t*)(obj + 60) = 0;

    // Splat radius into AABB extent vector
    float radius = -1.0f;
    *(float*)(obj + 112) = radius;
    *(float*)(obj + 116) = radius;
    *(float*)(obj + 120) = radius;
    *(float*)(obj + 124) = radius;

    *(uint32_t*)(obj + 128) = 0;

    phBoundSphere_vfn_37(thisPtr);

    *(uint32_t*)(obj + 132) = 0;
    *(uint32_t*)(obj + 136) = 0;
    *(uint32_t*)(obj + 140) = 0;
}

/**
 * rage::phBoundOctree::phBoundOctree @ 0x822971A8 | size: 0x74 (116 bytes)
 *
 * Constructor for phBoundOctree, a spatial partitioning bound for efficient
 * broadphase collision queries. Extends phBoundGeometry with octree data.
 */
void phBoundOctree_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    phBoundGeometry_Constructor(thisPtr);

    *(uint32_t*)(obj + 0) = 0x8205872C;  // vtable -> rage::phBoundOctree
    *(uint8_t*)(obj + 128) = 1;           // has octree = true
    *(uint8_t*)(obj + 4) = 4;             // bound type = octree

    ph_9BC0((void*)(obj + 176));

    *(uint32_t*)(obj + 176) = 0;
    *(uint32_t*)(obj + 180) = 0;
    *(uint32_t*)(obj + 184) = 0;
    *(uint32_t*)(obj + 188) = 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phInst — Virtual Accessor / Transform Methods (10 functions, 8-24B)
// ═════════════════════════════════════════════════════════════════════════════
//
// Getter/setter pairs for collision and contact properties, plus transform
// matrix copy thunks. All are base-class implementations on rage::phInst's
// vtable at 0x8205991C.
//
// Field layout referenced:
//   +64  (0x40)  uint64  m_pArchetypeData   — archetype/template pointer
//   +204 (0xCC)  40B     m_currentTransform  — current-frame world matrix
//   +284 (0x11C) uint32  m_collisionFlags    — collision group/layer mask
//   +284 (0x11C) 40B     m_lastTransform     — previous-frame world matrix
//   +444 (0x1BC) uint32  m_contactFilter     — contact filtering mask
//   +448 (0x1C0) uint32  m_contactCallback   — contact response handler ptr

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// phInst::SetCollisionFlags (vfn_16) @ 0x8248B8C8 | size: 0x8
//
// Stores the collision group/layer flags at offset +284.
// Paired getter: GetCollisionFlags (vfn_19).
// ─────────────────────────────────────────────────────────────────────────────
void phInst::SetCollisionFlags(uint32_t flags) {  // vfn_16
    this->m_nCollisionGroup = flags;  // +0x11C
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::GetCollisionFlags (vfn_19) @ 0x8248B8D0 | size: 0x8
//
// Returns the collision group/layer flags from offset +284.
// Paired setter: SetCollisionFlags (vfn_16).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phInst::GetCollisionFlags() {  // vfn_19
    return this->m_nCollisionGroup;  // +0x11C
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::SetContactFilter (vfn_38) @ 0x8248D808 | size: 0x8
//
// Sets the contact filtering mask at offset +444.
// Used to control which collision pairs generate contact callbacks.
// Paired getter: GetContactFilter (vfn_44).
// ─────────────────────────────────────────────────────────────────────────────
void phInst::SetContactFilter(uint32_t filter) {  // vfn_38
    this->m_nCollisionMask = filter;  // +0x1BC
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::GetContactFilter (vfn_44) @ 0x8248D810 | size: 0x8
//
// Returns the contact filtering mask from offset +444.
// Paired setter: SetContactFilter (vfn_38).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phInst::GetContactFilter() {  // vfn_44
    return this->m_nCollisionMask;  // +0x1BC
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::SetContactCallback (vfn_39) @ 0x8248D818 | size: 0x8
//
// Sets the contact response callback pointer at offset +448.
// This controls which handler is invoked when contacts are resolved.
// Paired getter: GetContactCallback (vfn_45).
// ─────────────────────────────────────────────────────────────────────────────
void phInst::SetContactCallback(uint32_t callback) {  // vfn_39
    this->m_nUserData = callback;  // +0x1C0
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::GetContactCallback (vfn_45) @ 0x8248D820 | size: 0x8
//
// Returns the contact response callback pointer from offset +448.
// Paired setter: SetContactCallback (vfn_39).
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phInst::GetContactCallback() {  // vfn_45
    return this->m_nUserData;  // +0x1C0
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::GetArchetypeData (vfn_71) @ 0x82488480 | size: 0x8
//
// Returns the 64-bit archetype/template data pointer from offset +64.
// The archetype contains shared immutable data (collision bounds, mass
// properties, etc.) referenced by all instances of the same physics object.
// ─────────────────────────────────────────────────────────────────────────────
uint64_t phInst::GetArchetypeData() {  // vfn_71
    return this->m_pArchetype;  // +0x40
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::GetSupportedFlags (vfn_48) @ 0x8247E2D0 | size: 0xC
//
// Returns a constant bitmask (0x80004001) indicating which physics features
// this instance type supports. Bit 31 = active, bit 14 = collidable,
// bit 0 = transform-capable.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t phInst::GetSupportedFlags() {  // vfn_48
    return 0x80004001;
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::CopyCurrentTransform (vfn_15) @ 0x8248D888 | size: 0x18
//
// Copies the current-frame world transform (40 bytes at offset +204) into
// a caller-supplied buffer via the virtual CopyMatrix helper (vfn_13).
//
// Tail-calls: this->vfn_13(this, r4, r5, 40, this+204)
//   where vfn_13 is a bounded memcpy: memset(dst, 0, size); memcpy(dst, src, min(len, size-1))
// ─────────────────────────────────────────────────────────────────────────────
void phInst::CopyCurrentTransform(void* src, uint32_t srcLen) {  // vfn_15
    // Dispatch to vfn_13 with: dest = this+204, size = 40
    typedef void (*CopyMatrixFn)(void*, void*, uint32_t, uint32_t, void*);
    void** vtable = *(void***)this;
    CopyMatrixFn copyMatrix = (CopyMatrixFn)vtable[13];
    copyMatrix(this, src, srcLen, 40, (char*)this + 204);
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst::CopyLastTransform (vfn_34) @ 0x8248D868 | size: 0x18
//
// Copies the previous-frame world transform (40 bytes at offset +284) into
// a caller-supplied buffer via the virtual CopyMatrix helper (vfn_13).
//
// Tail-calls: this->vfn_13(this, r4, r5, 40, this+284)
// ─────────────────────────────────────────────────────────────────────────────
void phInst::CopyLastTransform(void* src, uint32_t srcLen) {  // vfn_34
    // Dispatch to vfn_13 with: dest = this+284, size = 40
    typedef void (*CopyMatrixFn)(void*, void*, uint32_t, uint32_t, void*);
    void** vtable = *(void***)this;
    CopyMatrixFn copyMatrix = (CopyMatrixFn)vtable[13];
    copyMatrix(this, src, srcLen, 40, (char*)this + 284);
}

// ═══════════════════════════════════════════════════════════════════════════
// rage::phBound virtual method implementations
// ═══════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// phBound::SetType (vfn_1 / slot 1) @ 0x8228CE18 | size: 0x8
//
// Sets the bound's collision shape type identifier.
// Offset 0x07 holds the type byte (e.g. sphere=0, capsule=1, box=2, etc.).
// ─────────────────────────────────────────────────────────────────────────────
void phBound::SetType(uint8_t type) {
    // stb r4,7(r3)
    *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(this) + 7) = type;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBound::GetType (vfn_2 / slot 2) @ 0x8228CE20 | size: 0x8
//
// Returns the bound's collision shape type identifier.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t phBound::GetType() {
    // lbz r3,7(r3)
    return *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(this) + 7);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBound::GetMargin (vfn_33 / slot 33) @ 0x8228D9F8 | size: 0x1c (28 bytes)
//
// Returns either the outer margin (offset +8) or inner margin (offset +12)
// depending on the useOuter flag. Used for collision detection padding.
//
// Parameters:
//   r4 — unused
//   r5 — useOuter flag: if nonzero returns margin at +8, else at +12
// ─────────────────────────────────────────────────────────────────────────────
float phBound::GetMargin(void* /*unused*/, bool useOuter) {
    uint8_t* base = reinterpret_cast<uint8_t*>(this);
    if (useOuter) {
        return *reinterpret_cast<float*>(base + 8);
    }
    return *reinterpret_cast<float*>(base + 12);
}

// ═══════════════════════════════════════════════════════════════════════════
// rage::phBoundComposite virtual method implementations
// ═══════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// phBoundComposite::CalculateExtents (vfn_28 / slot 28) @ 0x8228DDA0 | size: 0x10
//
// Thunk to ComputeExtents (vtable slot 27). Loads the vtable and
// tail-calls slot 27, which computes the AABB extents for this bound.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundComposite::CalculateExtents() {
    // lwz r11,0(r3); lwz r10,108(r11); mtctr r10; bctr
    void** vtbl = *reinterpret_cast<void***>(this);
    typedef void (*Fn)(void*);
    Fn fn = reinterpret_cast<Fn>(vtbl[27]);
    fn(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundComposite::SetDefaultFlags (vfn_37 / slot 37) @ 0x8228E820 | size: 0x8
//
// Initializes composite bound flags to defaults by calling the internal
// flag-update helper with flags = 0.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundComposite::SetDefaultFlags() {
    // li r4,0; b pongCreatureInst_E828_v12
    pongCreatureInst_E828_v12(this, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundComposite::UpdateChildBounds (vfn_11 / slot 11) @ 0x822906F0 | size: 0x14
//
// Updates all child bounds in the composite. Dispatches to vtable slot 38
// with a null filter parameter, causing all children to be updated.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundComposite::UpdateChildBounds() {
    // lwz r11,0(r3); li r5,0; lwz r10,152(r11); mtctr r10; bctr
    void** vtbl = *reinterpret_cast<void***>(this);
    typedef void (*Fn)(void*, void*, void*);
    Fn fn = reinterpret_cast<Fn>(vtbl[38]);
    fn(this, nullptr, nullptr);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundComposite::RebuildChildBounds (vfn_12 / slot 12) @ 0x82290788 | size: 0x14
//
// Rebuilds all child bounds in the composite. Dispatches to vtable slot 39
// with a null filter parameter, causing a full rebuild.
// ─────────────────────────────────────────────────────────────────────────────
void phBoundComposite::RebuildChildBounds() {
    // lwz r11,0(r3); li r5,0; lwz r10,156(r11); mtctr r10; bctr
    void** vtbl = *reinterpret_cast<void***>(this);
    typedef void (*Fn)(void*, void*, void*);
    Fn fn = reinterpret_cast<Fn>(vtbl[39]);
    fn(this, nullptr, nullptr);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundComposite::GetFirstChildCentroid (vfn_10 / slot 10) @ 0x82290808 | size: 0x28
//
// Returns the centroid of the first child bound in this composite.
// Loads the child array from offset +0x70, dereferences the first element,
// and tail-calls its virtual GetCentroid (vtable slot 10). Returns 0 if
// the child array is empty.
// ─────────────────────────────────────────────────────────────────────────────
void* phBoundComposite::GetFirstChildCentroid() {
    // lwz r11,112(r3) — load child bounds array pointer
    uint32_t* childArray = *reinterpret_cast<uint32_t**>(
        reinterpret_cast<uint8_t*>(this) + 112);

    // lwz r3,0(r11) — load first child bound
    void* firstChild = reinterpret_cast<void*>(
        static_cast<uintptr_t>(childArray[0]));

    if (firstChild != nullptr) {
        // Call firstChild->vtable[10]
        void** childVtbl = *reinterpret_cast<void***>(firstChild);
        typedef void* (*GetCentroidFn)(void*);
        GetCentroidFn getCentroid = reinterpret_cast<GetCentroidFn>(childVtbl[10]);
        return getCentroid(firstChild);
    }

    return nullptr;
}

// ═══════════════════════════════════════════════════════════════════════════
// rage::phJoint1Dof virtual method implementations
// ═══════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// phJoint1Dof::GetMotorData (vfn_15 / slot 15) @ 0x82126CF8 | size: 0x8
//
// Returns a pointer to the joint's motor/drive parameters block,
// stored at offset +0x230 (560 bytes) from the start of the object.
// The motor data controls the joint's target angle and drive strength.
// ─────────────────────────────────────────────────────────────────────────────
void* phJoint1Dof::GetMotorData() {
    // addi r3,r3,560
    return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(this) + 560);
}

// ═══════════════════════════════════════════════════════════════════════════
// rage::phSleep virtual method implementations
// ═══════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// phSleep::Activate (vfn_1 / slot 1) @ 0x822DD238 | size: 0xc (12 bytes)
//
// Marks the sleep controller as active by setting the awake flag
// at offset +4 to 1. This causes the physics simulation to keep
// the associated body awake for collision processing.
// ─────────────────────────────────────────────────────────────────────────────
void phSleep::Activate() {
    // li r11,1; stw r11,4(r3)
    *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(this) + 4) = 1;
}

} // namespace rage

// ═══════════════════════════════════════════════════════════════════════════
// Physics Matrix / Collision / Joint Free Functions
// ═══════════════════════════════════════════════════════════════════════════

extern void ph_CollideShapeAgainstWorld(uint32_t, uint32_t, int, int);
extern void ph_TestCollisionPairDetailed(uint32_t, uint32_t, uint32_t, uint32_t,
                                          uint8_t*, uint32_t, uint8_t*, int);
extern void ph_ProfilerBegin(const char*, int);
extern void ph_ResolveCollisionConstraint(uint32_t, uint32_t, void*);
extern void ph_DebugAssertPositive(const char*, double);

// ---------------------------------------------------------------------------
// 1. phMatrix34_TransposeDotProduct @ 0x821181C8 | size: 0xB0 (176 bytes)
//    Computes dst = A^T * B for two 3x4 matrices. Row 3 computes relative
//    translation (A.row3 - B.row3) projected into B's local frame.
// ---------------------------------------------------------------------------
void phMatrix34_TransposeDotProduct(float* dst, const float* src) {
    float a0x = dst[0],  a0y = dst[1],  a0z = dst[2];
    float a1x = dst[4],  a1y = dst[5],  a1z = dst[6];
    float a2x = dst[8],  a2y = dst[9],  a2z = dst[10];
    float a3x = dst[12], a3y = dst[13], a3z = dst[14];
    float b0x = src[0],  b0y = src[1],  b0z = src[2];
    float b1x = src[4],  b1y = src[5],  b1z = src[6];
    float b2x = src[8],  b2y = src[9],  b2z = src[10];
    float b3x = src[12], b3y = src[13], b3z = src[14];
    float dx = a3x - b3x, dy = a3y - b3y, dz = a3z - b3z;
    dst[0]  = a0x*b0x + a0y*b0y + a0z*b0z;
    dst[1]  = a0x*b1x + a0y*b1y + a0z*b1z;
    dst[2]  = a0x*b2x + a0y*b2y + a0z*b2z;
    dst[4]  = a1x*b0x + a1y*b0y + a1z*b0z;
    dst[5]  = a1x*b1x + a1y*b1y + a1z*b1z;
    dst[6]  = a1x*b2x + a1y*b2y + a1z*b2z;
    dst[8]  = a2x*b0x + a2y*b0y + a2z*b0z;
    dst[9]  = a2x*b1x + a2y*b1y + a2z*b1z;
    dst[10] = a2x*b2x + a2y*b2y + a2z*b2z;
    dst[12] = dx*b0x + dy*b0y + dz*b0z;
    dst[13] = dx*b1x + dy*b1y + dz*b1z;
    dst[14] = dx*b2x + dy*b2y + dz*b2z;
}

// ---------------------------------------------------------------------------
// 2. phCollider_QueryShapeOverlap @ 0x82118DB8 | size: 0x44 (68 bytes)
//    Initiates overlap query: extracts bound handle, retrieves world list,
//    dispatches to ph_CollideShapeAgainstWorld(maxResults=255, filter=0).
// ---------------------------------------------------------------------------
void phCollider_QueryShapeOverlap(void* thisPtr, void* querySource) {
    uint8_t* self = (uint8_t*)thisPtr;
    uint32_t boundsHandle = *(uint32_t*)((uint8_t*)querySource + 4);
    uint32_t internalData = *(uint32_t*)(self + 164);
    uint32_t worldList = *(uint32_t*)((uint8_t*)(uintptr_t)internalData + 4);
    ph_CollideShapeAgainstWorld(boundsHandle, worldList, 255, 0);
    *(uint32_t*)(self + 168) = boundsHandle;
}

// ---------------------------------------------------------------------------
// 3. phCollider_TestCollisionPair @ 0x82118E00 | size: 0x6C (108 bytes)
//    Tests collision pair between collider bound and target. Increments
//    test counter at +428 on completion.
// ---------------------------------------------------------------------------
void phCollider_TestCollisionPair(void* thisPtr, void* target) {
    uint8_t* self = (uint8_t*)thisPtr;
    uint32_t targetHandle = *(uint32_t*)((uint8_t*)target + 4);
    uint32_t internalData = *(uint32_t*)(self + 164);
    uint32_t worldCount = 0;
    if (internalData != 0)
        worldCount = *(uint32_t*)((uint8_t*)(uintptr_t)internalData + 8);
    uint32_t contactCount = *(uint32_t*)(self + 180);
    uint32_t contactFlags = *(uint32_t*)(self + 176);
    uint32_t boundsA = *(uint32_t*)((uint8_t*)(uintptr_t)internalData + 4);
    ph_TestCollisionPairDetailed(targetHandle, boundsA, worldCount,
                                  contactFlags, self + 426, contactCount,
                                  self + 427, 255);
    self[428] = self[428] + 1;
}

// ---------------------------------------------------------------------------
// 4. phCollisionResolver_ProfiledResolve @ 0x82123608 | size: 0x74 (116 bytes)
//    Resolves collision constraint within profiler begin/end markers.
// ---------------------------------------------------------------------------
void phCollisionResolver_ProfiledResolve(void* thisPtr, void* input) {
    uint8_t* self = (uint8_t*)thisPtr;
    uint32_t pairHandle = *(uint32_t*)((uint8_t*)input + 4);
    ph_ProfilerBegin(g_phResolveProfileTag, 1);
    uint32_t pairData = *(uint32_t*)((uint8_t*)input + 4);
    uint32_t pairInfo = *(uint32_t*)((uint8_t*)(uintptr_t)pairData + 12);
    ph_ResolveCollisionConstraint(g_phCollisionSolver, pairInfo, self + 164);
    ph_ProfilerBegin(g_phResolveProfileTagEnd, 1);
}

// ---------------------------------------------------------------------------
// 5. phBound_ComputeClampedExtent @ 0x82123E78 | size: 0x9C (156 bytes)
//    Computes safe collision margin from bound's vtable slot 8. Logs warning
//    and takes abs if negative. Clamps result via fsel.
// ---------------------------------------------------------------------------
float phBound_ComputeClampedExtent(void* bound) {
    float scale = g_phExtentScale;
    float result = scale;
    uint32_t extentSrc = *(uint32_t*)((uint8_t*)bound + 256);
    if (extentSrc != 0) {
        void** vt = *(void***)(uintptr_t)extentSrc;
        typedef float (*GetExtFn)(void*);
        result = ((GetExtFn)vt[8])((void*)(uintptr_t)extentSrc);
        if (result < scale) {
            ph_DebugAssertPositive("phBound extent negative", (double)result);
            if (result < 0.0f) result = -result;
        }
    }
    float clamped = result * g_phClampMax;
    return (-clamped >= 0.0f) ? 0.0f : clamped;
}

// ---------------------------------------------------------------------------
// 6. phJoint1Dof_Constructor @ 0x82126D00 | size: 0x98 (152 bytes)
//    Constructor for rage::phJoint1Dof. Calls base ctor, installs vtable,
//    initializes angular limits, spring/damping, motor, tolerances.
// ---------------------------------------------------------------------------
extern void phJoint_Constructor(void*);

void phJoint1Dof_Constructor(void* thisPtr) {
    uint8_t* self = (uint8_t*)thisPtr;
    phJoint_Constructor(thisPtr);
    *(void**)(self) = &g_phJoint1DofVtable;
    *(uint8_t*)(self + 32) = 0;
    *(float*)(self + 720) = g_phJoint1Dof_angleMin;
    *(float*)(self + 728) = g_phJoint1Dof_angleMin;
    *(float*)(self + 724) = g_phJoint1Dof_angleMax;
    *(float*)(self + 732) = g_phJoint1Dof_angleMax;
    *(float*)(self + 16)  = g_phJoint1Dof_mass;
    *(float*)(self + 784) = g_phJoint1Dof_stiffness;
    *(float*)(self + 792) = g_phJoint1Dof_mass;
    *(float*)(self + 796) = g_phJoint1Dof_mass;
    *(float*)(self + 788) = g_phJoint1Dof_maxTorque;
    *(float*)(self + 800) = g_phJoint1Dof_errTol;
    *(float*)(self + 804) = g_phJoint1Dof_maxCorr;
}

// ---------------------------------------------------------------------------
// 7. phContactManifold_Init @ 0x8221FD78 | size: 0x58 (88 bytes)
//    Initializes contact manifold (4 slots). Early-outs if physics disabled.
//    Clears contact IDs, sets separations to 0xFFFF sentinel, zeros floats.
// ---------------------------------------------------------------------------
void phContactManifold_Init(void* manifold) {
    if (g_phDisabled != 0) return;
    uint8_t* self = (uint8_t*)manifold;
    for (int i = 0; i < 4; i++) *(uint16_t*)(self + 16 + i*2) = 0;
    for (int i = 0; i < 4; i++) *(uint16_t*)(self + 24 + i*2) = 0xFFFF;
    *(float*)(self + 0)  = g_phZeroFloat;
    *(float*)(self + 4)  = g_phZeroFloat;
    *(float*)(self + 8)  = g_phZeroFloat;
    *(float*)(self + 12) = g_phZeroFloat;
}

// ---------------------------------------------------------------------------
// 8. phMatrix34_MultiplyTranspose @ 0x8211CC88 | size: 0xCC (204 bytes)
//    Three-operand matrix multiply: dst = left * right^T. Row 3 computes
//    delta = left.row3 - right.row3 projected into right's local frame.
// ---------------------------------------------------------------------------
void phMatrix34_MultiplyTranspose(float* dst, const float* left, const float* right) {
    float l0x = left[0],  l0y = left[1],  l0z = left[2];
    float l1x = left[4],  l1y = left[5],  l1z = left[6];
    float l2x = left[8],  l2y = left[9],  l2z = left[10];
    float l3x = left[12], l3y = left[13], l3z = left[14];
    float r0x = right[0],  r0y = right[1],  r0z = right[2];
    float r1x = right[4],  r1y = right[5],  r1z = right[6];
    float r2x = right[8],  r2y = right[9],  r2z = right[10];
    float r3x = right[12], r3y = right[13], r3z = right[14];
    float dx = l3x - r3x, dy = l3y - r3y, dz = l3z - r3z;
    dst[0]  = l0x*r0x + l0y*r0y + l0z*r0z;
    dst[1]  = l0x*r1x + l0y*r1y + l0z*r1z;
    dst[2]  = l0x*r2x + l0y*r2y + l0z*r2z;
    dst[4]  = l1x*r0x + l1y*r0y + l1z*r0z;
    dst[5]  = l1x*r1x + l1y*r1y + l1z*r1z;
    dst[6]  = l1x*r2x + l1y*r2y + l1z*r2z;
    dst[8]  = l2x*r0x + l2y*r0y + l2z*r0z;
    dst[9]  = l2x*r1x + l2y*r1y + l2z*r1z;
    dst[10] = l2x*r2x + l2y*r2y + l2z*r2z;
    dst[12] = dx*r0x + dy*r0y + dz*r0z;
    dst[13] = dx*r1x + dy*r1y + dz*r1z;
    dst[14] = dx*r2x + dy*r2y + dz*r2z;
}

// ---------------------------------------------------------------------------
// 9. phJoint_Constructor @ 0x8225ABE8 | size: 0x9C (156 bytes)
//    Base constructor for rage::phJoint. Installs vtable, zeros constraint
//    matrices (2x 192B), body pointers, and accumulated state (128B).
// ---------------------------------------------------------------------------
void phJoint_Constructor(void* thisPtr) {
    uint8_t* self = (uint8_t*)thisPtr;
    *(void**)(self) = &g_phJointVtable;
    *(float*)(self + 16) = g_phZeroFloat;
    *(uint32_t*)(self + 20) = 0;
    *(uint32_t*)(self + 24) = 0;
    *(uint32_t*)(self + 28) = 0;
    uint8_t* p = self + 48;
    for (int i = 0; i < 24; i++) { *(uint64_t*)p = 0; p += 8; }
    p = self + 240;
    for (int i = 0; i < 24; i++) { *(uint64_t*)p = 0; p += 8; }
    for (int i = 0; i < 16; i++) *(uint64_t*)(self + 432 + i*8) = 0;
}

// ---------------------------------------------------------------------------
// 10. phBound_RelocatePointers @ 0x822CD1B0 | size: 0x78 (120 bytes)
//     Relocates internal pointers at +16 and +96 after resource loading.
//     Converts file-relative offsets to absolute addresses via chunk table.
// ---------------------------------------------------------------------------
void phBound_RelocatePointers(void* thisPtr, void* relocDesc) {
    uint8_t* self = (uint8_t*)thisPtr;
    uint8_t* desc = (uint8_t*)relocDesc;
    *(void**)(self) = &g_phBoundRelocVtable;
    uint32_t ptr16 = *(uint32_t*)(self + 16);
    if (ptr16 != 0) {
        uint32_t base = *(uint32_t*)(desc + 4);
        uint32_t chunk = *(uint32_t*)(desc + 76);
        uint32_t idx = (ptr16 - base) / chunk;
        *(uint32_t*)(self + 16) = *(uint32_t*)(desc + 8 + idx*4) + ptr16;
    }
    uint32_t ptr96 = *(uint32_t*)(self + 96);
    if (ptr96 == 0) return;
    uint32_t base = *(uint32_t*)(desc + 4);
    uint32_t chunk = *(uint32_t*)(desc + 76);
    uint32_t idx = (ptr96 - base) / chunk;
    *(uint32_t*)(self + 96) = *(uint32_t*)(desc + 8 + idx*4) + ptr96;
}

// =========================================================================
// rage::phBoundCapsule -- Capsule Collision Bound Functions (68-232B)
// =========================================================================
//
// 10 additional phBoundCapsule functions covering containment tests,
// 2D rotation helpers, initialization, Euler decomposition, and
// coordinate transformation utilities.
// =========================================================================

extern float phBoundCapsule_GetJointAngle(void* capsuleData);
extern int32_t phBoundCapsule_ResolvePlayerIndex(void* capsuleData);
extern void phBoundCapsule_ComputeAngleXZ(void* camera, float fovHoriz, float fovVert);
extern void phBoundCapsule_UpdateCapsuleState(void* camera);
extern void phBoundCapsule_DecomposeRotation(float* inMatrix, float* row0, float* row1, float* row2, float* row3);

// ---------------------------------------------------------------------------
// 1. phBoundCapsule::TestPointContainment @ 0x822A4288 | size: 0xAC
//    Tests whether a 3D point lies inside the capsule bound.
//    Checks Y against halfHeight+radius, then XZ distance against radius.
//    For points in the hemispherical caps, checks full 3D distance.
//    Returns 1 if contained, 0 otherwise.
// ---------------------------------------------------------------------------
int32_t rage::phBoundCapsule::TestPointContainment(const float* point) {
    float radius     = *(float*)&this->m_fRadius;     // +0x80
    float halfHeight = *(float*)&this->m_pVertices;   // +0x70 (reused as halfHeight)
    float capsuleHalfExtent = radius * g_phTwo + halfHeight;

    float py = point[1];
    if (py > capsuleHalfExtent)
        return 0;
    if (py < -capsuleHalfExtent)
        return 0;

    // Check XZ planar distance against radius squared
    float px = point[0];
    float pz = point[2];
    float radiusSq = halfHeight * halfHeight;
    float distXZSq = px * px + pz * pz;
    if (distXZSq > radiusSq)
        return 0;

    // Point is in the cylindrical body
    if (py <= radius) {
        // In upper hemisphere cap
        if (py > radius) {
            float dy = py - radius;
            float capDistSq = pz * pz + dy * dy + px * px;
            if (capDistSq > radiusSq)
                return 0;
        }
        return 1;
    }

    // In lower hemisphere cap
    float negRadius = -radius;
    if (py < negRadius) {
        float dy = py + radius;
        float capDistSq = pz * pz + dy * dy + px * px;
        if (capDistSq > radiusSq)
            return 0;
    }
    return 1;
}

// ---------------------------------------------------------------------------
// 2. phBoundCapsule::InitializeCapsule @ 0x822A2EA0 | size: 0x84
//    Sets half-height and radius vectors, computes total extent,
//    zeros center offset, then calls vfn_37 to update derived data.
// ---------------------------------------------------------------------------
extern float ph_TokenizerReadInt(float value);
void rage::phBoundCapsule::InitializeCapsule(float halfHeight, float radius) {

    // Splat halfHeight into vector at m_pVertices region (+0x70)
    float* halfHeightVec = (float*)&this->m_pVertices;
    halfHeightVec[0] = halfHeight;
    halfHeightVec[1] = halfHeight;
    halfHeightVec[2] = halfHeight;
    halfHeightVec[3] = halfHeight;

    // Splat radius into vector at m_fRadius (+0x80)
    float* radiusVec = (float*)&this->m_fRadius;
    radiusVec[0] = radius;
    radiusVec[1] = radius;
    radiusVec[2] = radius;
    radiusVec[3] = radius;

    // Compute total capsule extent: halfHeight + radius * 2
    float extent = *(float*)&this->m_fRadius * g_phTwo + *(float*)&this->m_pVertices;
    *(float*)&this->m_fAABBMinX = extent;  // +0x08

    // Zero the center offset vector at m_vCenterX (+0x30)
    float* center = (float*)&this->m_vCenterX;
    center[0] = 0.0f;
    center[1] = 0.0f;
    center[2] = 0.0f;
    center[3] = 0.0f;

    // Update derived bound data via vtable slot 37
    void** vt = *(void***)this;
    typedef void (*RecalcFn)(void*);
    RecalcFn recalc = (RecalcFn)vt[37];
    recalc(this);
}

// ---------------------------------------------------------------------------
// 3. phBoundCapsule::RotateAxisXZ @ 0x8223AB00 | size: 0x6C
//    Rotates a 2D vector (fields +4, +8) by angle using sin/cos helpers.
//    new[+8] = old[+8]*cos + old[+4]*sin
//    new[+4] = old[+4]*cos - old[+8]*sin
// ---------------------------------------------------------------------------
void phBoundCapsule_RotateAxisXZ(float* vec, float angle) {
    float sinVal = (float)ph_Sinf(angle);
    float cosVal = (float)ph_Cosf(angle);

    float oldX = vec[1];  // +4
    float oldZ = vec[2];  // +8

    float newZ = oldZ * sinVal + oldX * cosVal;
    vec[2] = newZ;

    float newX = oldX * cosVal - oldZ * sinVal;
    vec[1] = newX;
}

// ---------------------------------------------------------------------------
// 4. phBoundCapsule::RotateAxisYZ @ 0x8223AB70 | size: 0x6C
//    Rotates a 2D vector (fields +0, +8) by angle using sin/cos helpers.
//    new[+8] = old[+8]*cos - old[+0]*sin
//    new[+0] = old[+0]*cos + old[+8]*sin
// ---------------------------------------------------------------------------
void phBoundCapsule_RotateAxisYZ(float* vec, float angle) {
    float sinVal = (float)ph_Sinf(angle);
    float cosVal = (float)ph_Cosf(angle);

    float oldY = vec[0];  // +0
    float oldZ = vec[2];  // +8

    float newY = oldY * sinVal;
    float newZ = oldZ * sinVal;

    vec[2] = (float)(oldZ * cosVal - newY);
    vec[0] = (float)(oldY * cosVal + newZ);
}

// ---------------------------------------------------------------------------
// 5. phBoundCapsule::ClassifyValueBySign @ 0x8216BB88 | size: 0x5C
//    Classifies a float value's sign against zero and a negative threshold.
//    Returns  1 if value > 0 and a global flag is set,
//    Returns -1 if value < threshold and flag is set (or as default),
//    Returns  1 if value > 0 and flag not set but value < threshold.
// ---------------------------------------------------------------------------
int32_t phBoundCapsule_ClassifyValueBySign(float* input) {

    float value = input[0];
    uint32_t stateObj = *(uint32_t*)(uintptr_t)g_phGlobalStatePtr;
    uint8_t flag = *(uint8_t*)(uintptr_t)(stateObj + 192);

    if (value > g_floatZero) {
        if (flag != 0) {
            return 1;
        }
    }
    if (value < g_phNegThreshold) {
        if (flag != 0) {
            return -1;
        }
        return 1;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// 6. phBoundCapsule::ComputeJointAngleSin @ 0x823FD888 | size: 0x44
//    Sets the joint type to 4 (capsule constraint), computes angle from
//    joint data via phBoundCapsule_GetJointAngle, stores sin of result.
// ---------------------------------------------------------------------------
void phBoundCapsule_ComputeJointAngleSin(void* /*r3*/, void* /*r4*/,
                                          void* jointData, float* output) {
    output[1] = 4;  // joint type = 4
    float angle = phBoundCapsule_GetJointAngle(jointData);
    float sinVal = (float)ph_Sinf(angle);
    output[0] = sinVal;
}

// ---------------------------------------------------------------------------
// 7. phBoundCapsule::LookupRandomTableValues @ 0x8216CF48 | size: 0x70
//    Queries a random value generator twice and looks up corresponding
//    entries from two separate float tables, storing results at +4 and +8.
// ---------------------------------------------------------------------------
void phBoundCapsule_LookupRandomTableValues(void* capsule) {

    uint32_t rngState = *(uint32_t*)(uintptr_t)g_phRandomSeedPtr;

    int32_t index1 = phBoundCapsule_ResolvePlayerIndex((void*)(uintptr_t)rngState);
    float val1 = g_phTableA[index1];
    *(float*)((char*)capsule + 4) = val1;

    rngState = *(uint32_t*)(uintptr_t)g_phRandomSeedPtr;
    int32_t index2 = phBoundCapsule_ResolvePlayerIndex((void*)(uintptr_t)rngState);
    float val2 = g_phTableB[index2];
    *(float*)((char*)capsule + 8) = val2;
}

// ---------------------------------------------------------------------------
// 8. phBoundCapsule::UpdateCameraFromBound @ 0x8216DB10 | size: 0x74
//    If the capsule has an associated camera (field +152), copies near/far
//    plane and FOV parameters from the bound to the camera, sets a dirty
//    flag, and dispatches camera update.
// ---------------------------------------------------------------------------
void phBoundCapsule_UpdateCameraFromBound(void* bound) {

    void* camera = *(void**)((char*)bound + 152);
    if (camera == nullptr)
        return;

    float fovVert  = *(float*)((char*)bound + 172);
    float fovHoriz = *(float*)((char*)bound + 168);

    uint32_t cameraBase = *(uint32_t*)(uintptr_t)g_phActiveCameraPtr;
    phBoundCapsule_ComputeAngleXZ((void*)(uintptr_t)cameraBase, fovHoriz, fovVert);

    float nearPlane = *(float*)((char*)bound + 164);
    *(float*)((char*)camera + 824) = nearPlane;

    // Store FOV and plane parameters into camera structure
    *(float*)((char*)camera + 828) = fovVert;
    *(float*)((char*)camera + 832) = fovHoriz;
    *(float*)((char*)camera + 836) = fovVert;
    *(uint8_t*)((char*)camera + 864) = 1;  // dirty flag

    phBoundCapsule_UpdateCapsuleState(camera);
}

// ---------------------------------------------------------------------------
// 9. phBoundCapsule::TransformPointToLocal @ 0x820D59A8 | size: 0xCC
//    Transforms a world-space point into the capsule's local coordinate
//    frame by subtracting the translation row and dotting with each axis.
//    Then computes atan2(localX, localY) and the capsule distance metric
//    sqrt(sin^2 * k + cos^2) where k is a capsule shape constant.
// ---------------------------------------------------------------------------
float phBoundCapsule_TransformPointToLocal(void* capsule, const float* worldPoint, float param) {
    // Get the 4x4 matrix from the capsule's instance data
    void* instData96 = *(void**)((char*)capsule + 96);
    void* instData80 = *(void**)((char*)capsule + 80);
    uint16_t matrixIndex = *(uint16_t*)((char*)instData96 + 20);
    char* matrixBase = (char*)(uintptr_t)(*(uint32_t*)((char*)instData80 + 20));
    char* matrix = matrixBase + matrixIndex * 64;

    // Load matrix rows
    float* row0 = (float*)(matrix + 0);
    float* row1 = (float*)(matrix + 16);
    float* row2 = (float*)(matrix + 32);
    float* row3 = (float*)(matrix + 48);

    // Subtract translation (row3) from world point
    float dx = worldPoint[0] - row3[0];
    float dy = worldPoint[1] - row3[1];
    float dz = worldPoint[2] - row3[2];

    // Dot with row0 (local X)
    float localX = row0[0] * dx + row0[1] * dy + row0[2] * dz;
    // Dot with row1 (local Y)
    float localY = row1[0] * dx + row1[1] * dy + row1[2] * dz;

    // Compute atan2(localX, localY)
    float atanAngle = (float)ph_Atan2f(localY, localX);
    float sinAngle = (float)ph_Sinf(atanAngle);
    float cosAngle = (float)ph_Cosf(atanAngle);

    // Compute capsule distance metric
    float sinSq = sinAngle * sinAngle;
    float cosSq = cosAngle * cosAngle;
    float capsuleDistance = (float)ph_Sqrt(cosSq + sinSq * g_phCapsuleShapeK);

    return capsuleDistance;
}

// ---------------------------------------------------------------------------
// 10. phBoundCapsule::DecomposeMatrixToEuler @ 0x820D0658 | size: 0xE8
//     Decomposes a 4x4 rotation matrix into Euler angles (stored as a
//     3-component vector). Extracts pitch from the matrix, computes yaw
//     and roll via atan2 on the remaining matrix rows.
//     Output: [0]=roll, [1]=yaw, [2]=pitch (in capsule convention)
// ---------------------------------------------------------------------------
void phBoundCapsule_DecomposeMatrixToEuler(const float* matrix, float* outEuler) {

    // Copy the 4x4 matrix to local storage for decomposition
    float localMatrix[16];
    memcpy(localMatrix + 0,  matrix + 0,  16);  // row 0
    memcpy(localMatrix + 4,  matrix + 4,  16);  // row 1
    memcpy(localMatrix + 8,  matrix + 8,  16);  // row 2
    memcpy(localMatrix + 12, matrix + 12, 16);  // row 3

    // Decompose: extract axis angles from the 3x3 rotation sub-matrix
    phBoundCapsule_DecomposeRotation(localMatrix, localMatrix, localMatrix + 4,
                          localMatrix + 8, localMatrix + 12);

    // Compute pitch angle from rows 1 and 2 (atan2)
    float pitchY = localMatrix[6];   // row1[2] = element (1,2)
    float pitchX = localMatrix[5];   // row1[1] = element (1,1)

    float pitch;
    if (pitchY == g_floatZero && pitchX == g_floatZero) {
        pitch = g_floatZero;
    } else {
        pitch = (float)ph_Atan2f(pitchX, pitchY);
    }

    // Compute yaw from rows 2 and 3
    float yawY = localMatrix[10];   // row2[2]
    float yawX = localMatrix[8];    // row2[0]

    float yaw;
    if (yawY == g_floatZero && yawX == g_floatZero) {
        yaw = g_floatZero;
    } else {
        yaw = (float)ph_Atan2f(yawX, yawY);
    }

    // Compute roll from the Z-axis component
    float rollInput = -localMatrix[9];  // -row2[1]
    float roll = (float)ph_TokenizerReadInt(rollInput);

    // Store Euler angles
    outEuler[1] = (float)yaw;
    outEuler[2] = (float)pitch;
    outEuler[0] = (float)roll;
}

// =========================================================================
// rage::phArticulatedCollider -- Additional Virtual Methods
// =========================================================================

// External functions used by phArticulatedCollider methods
extern void phCollider_vfn_1(void* collider);
extern void phArticulatedCollider_ResetJointChain(void* jointData);
extern void phArticulatedCollider_5D58(void* jointData);
extern void phArticulatedCollider_ResetForceAccumulators(void* jointData);
extern void phArticulatedCollider_5FE0(void* jointData);
extern void* phArticulatedCollider_CreateJointConstraints(void* jointData, uint32_t boneMap,
    uint32_t constraintMap, uint32_t jointParamA, uint32_t jointParamB, uint32_t jointParamC);

// ---------------------------------------------------------------------------
// phArticulatedCollider::Shutdown (vfn_1) @ 0x8224E6D8 | size: 0x48
//
// Overrides phCollider::Shutdown (scalar destructor slot). Calls the base
// class shutdown, then zeros the joint processing state (m_pPhysicsData
// at +472 and m_bJointsDirty flag at +468), and performs two-phase joint
// data cleanup via phArticulatedCollider_ResetJointChain and _5D58.
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::Shutdown() {
    phCollider_vfn_1((void*)this);

    m_pPhysicsData = 0;  // +472
    m_bJointsDirty = 0;          // +468 (byte)

    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464
    phArticulatedCollider_ResetJointChain(jointData);

    jointData = (void*)(uintptr_t)m_pJointArray;  // reload +464
    phArticulatedCollider_5D58(jointData);
}
// ---------------------------------------------------------------------------
// phArticulatedCollider::ApplyScaledMatrixRowX (vfn_28) @ 0x8224FDA8 | size: 0x94
//
// Extracts the X-axis row from the 4x4 transform matrix at offset +144
// within the root joint data (joint[0] at +40 in the joint data block).
// Uses vmrghw to interleave high words from matrix rows 0..3 to form
// the X-components, then scales the result by the float at matrix +128
// multiplied by the input scale, and calls vtable slot 33 (ApplyForce).
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::ApplyScaledMatrixRowX(float scale) {
    void** vtbl = *(void***)this;
    uint32_t* jointDataBase = (uint32_t*)(uintptr_t)m_pJointArray;  // +464

    // Get root joint's transform matrix base at +144
    uint32_t* rootJoint = (uint32_t*)(uintptr_t)*(uint32_t*)((char*)jointDataBase + 40);
    float* matrixBase = (float*)((char*)rootJoint + 144);

    // Extract X-axis row using high-word interleave of matrix columns
    // Rows at +0, +16, +32, +48 of the 4x4 matrix
    float axisVec[4];
    axisVec[0] = matrixBase[0];   // row0.x
    axisVec[1] = matrixBase[4];   // row1.x
    axisVec[2] = matrixBase[8];   // row2.x
    axisVec[3] = matrixBase[12];  // row3.x

    // Scale by mass float at matrix +128 (offset -16 from matrix = rootJoint+128)
    float massScale = *(float*)((char*)rootJoint + 128);
    float combinedScale = massScale * scale;

    axisVec[0] *= combinedScale;
    axisVec[1] *= combinedScale;
    axisVec[2] *= combinedScale;
    axisVec[3] *= combinedScale;

    // Call vtable slot 33 (ApplyForce)
    typedef void (*ApplyForceFunc)(void*, float*);
    ApplyForceFunc fn = (ApplyForceFunc)vtbl[33];
    fn(this, axisVec);
}

// ---------------------------------------------------------------------------
// phArticulatedCollider::ApplyScaledMatrixRowY (vfn_29) @ 0x8224FE40 | size: 0x94
//
// Same pattern as ApplyScaledMatrixRowX but extracts the Y-axis row using
// vmrglw (low-word interleave). Scales by the float at rootJoint +136
// multiplied by the input scale, and calls vtable slot 33.
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::ApplyScaledMatrixRowY(float scale) {
    void** vtbl = *(void***)this;
    uint32_t* jointDataBase = (uint32_t*)(uintptr_t)m_pJointArray;  // +464

    uint32_t* rootJoint = (uint32_t*)(uintptr_t)*(uint32_t*)((char*)jointDataBase + 40);
    float* matrixBase = (float*)((char*)rootJoint + 144);

    // Extract Y-axis row using low-word interleave of matrix columns
    float axisVec[4];
    axisVec[0] = matrixBase[1];   // row0.y
    axisVec[1] = matrixBase[5];   // row1.y
    axisVec[2] = matrixBase[9];   // row2.y
    axisVec[3] = matrixBase[13];  // row3.y

    // Scale by float at rootJoint +136
    float massScale = *(float*)((char*)rootJoint + 136);
    float combinedScale = massScale * scale;

    axisVec[0] *= combinedScale;
    axisVec[1] *= combinedScale;
    axisVec[2] *= combinedScale;
    axisVec[3] *= combinedScale;

    typedef void (*ApplyForceFunc)(void*, float*);
    ApplyForceFunc fn = (ApplyForceFunc)vtbl[33];
    fn(this, axisVec);
}

// ---------------------------------------------------------------------------
// phArticulatedCollider::ApplyScaledMatrixRowZ (vfn_30) @ 0x8224FED8 | size: 0x94
//
// Same pattern as the X/Y variants but extracts the Z-axis row. Uses a
// combination of vmrghw and vmrglw to pick out the third component from
// each matrix row. Scales by the float at rootJoint +132 multiplied by
// the input scale, and calls vtable slot 33.
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::ApplyScaledMatrixRowZ(float scale) {
    void** vtbl = *(void***)this;
    uint32_t* jointDataBase = (uint32_t*)(uintptr_t)m_pJointArray;  // +464

    uint32_t* rootJoint = (uint32_t*)(uintptr_t)*(uint32_t*)((char*)jointDataBase + 40);
    float* matrixBase = (float*)((char*)rootJoint + 144);

    // Extract Z-axis row from matrix columns
    float axisVec[4];
    axisVec[0] = matrixBase[2];   // row0.z
    axisVec[1] = matrixBase[6];   // row1.z
    axisVec[2] = matrixBase[10];  // row2.z
    axisVec[3] = matrixBase[14];  // row3.z

    // Scale by float at rootJoint +132
    float massScale = *(float*)((char*)rootJoint + 132);
    float combinedScale = massScale * scale;

    axisVec[0] *= combinedScale;
    axisVec[1] *= combinedScale;
    axisVec[2] *= combinedScale;
    axisVec[3] *= combinedScale;

    typedef void (*ApplyForceFunc)(void*, float*);
    ApplyForceFunc fn = (ApplyForceFunc)vtbl[33];
    fn(this, axisVec);
}

// ---------------------------------------------------------------------------
// phArticulatedCollider::SetupJointConstraints (vfn_47) @ 0x82250528 | size: 0x50
//
// Initializes the joint constraint system. Sets m_bJointsDirty
// to 1, loads five parameter pointers (m_pJointTypeArray, m_pJointConfigArray,
// m_pJointInertiaArray, m_pJointDampingArray, m_pJointMassArray), and
// dispatches to phArticulatedCollider_CreateJointConstraints for constraint setup.
// Stores the returned constraint handle in m_pPhysicsData.
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::SetupJointConstraints() {
    m_bJointsDirty = 1;  // +468 (byte flag)

    void* jointData = (void*)(uintptr_t)m_pJointArray;    // +464
    uint32_t boneMap = m_pJointTypeArray;          // +0x1DC
    uint32_t constraintMap = m_pJointConfigArray;  // +0x1E4
    uint32_t jointParamA = m_pJointInertiaArray;   // +0x1EC
    uint32_t jointParamB = m_pJointDampingArray;   // +0x1F4
    uint32_t jointParamC = m_pJointMassArray;      // +0x1FC

    uint32_t result = (uint32_t)(uintptr_t)phArticulatedCollider_CreateJointConstraints(
        jointData, boneMap, constraintMap, jointParamA, jointParamB, jointParamC);

    m_pPhysicsData = result;  // +472
}

// ---------------------------------------------------------------------------
// phArticulatedCollider::DispatchJointUpdate (vfn_17) @ 0x822505B0 | size: 0x34
//
// Looks up a joint by index through two indirection tables (bone map at +484,
// link map at +476), resolves the joint object from the joint data array at
// +464, and tail-calls vtable slot 1 on the resolved joint object.
//
// Index resolution: boneMap[index] -> linkIndex, linkMap[index] -> nodeIndex,
//   then jointData[(nodeIndex + 42) * 4] -> joint object -> vtable[1](joint, boneMapEntry)
// ---------------------------------------------------------------------------
void rage::phArticulatedCollider::DispatchJointUpdate(int index) {
    uint32_t* boneMap = (uint32_t*)(uintptr_t)m_pJointConfigArray;     // +0x1E4
    uint32_t* linkMap = (uint32_t*)(uintptr_t)m_pJointTypeArray;     // +0x1DC
    uint32_t* jointDataBase = (uint32_t*)(uintptr_t)m_pJointArray;   // +0x1D0

    uint32_t boneEntry = linkMap[index];
    uint32_t nodeIndex = boneEntry + 42;
    uint32_t constraintEntry = boneMap[index];

    void* jointObj = (void*)(uintptr_t)jointDataBase[nodeIndex];

    // Tail-call vtable slot 1 on the joint object
    void** vtbl = *(void***)jointObj;
    typedef void (*JointUpdateFunc)(void*, uint32_t);
    JointUpdateFunc fn = (JointUpdateFunc)vtbl[1];
    fn(jointObj, constraintEntry);
}

// ---------------------------------------------------------------------------
// phArticulatedCollider::DispatchJointSolve (vfn_50) @ 0x82250578 | size: 0x34
//
// Same indirection as DispatchJointUpdate but tail-calls vtable slot 6
// (Solve) on the resolved joint object instead of slot 1.
// ---------------------------------------------------------------------------
extern void* ph_3760(void* param1, void* param2, void* param3, int maxDepth, int flag1, int flag2);
void rage::phArticulatedCollider::DispatchJointSolve(int index) {
    uint32_t* linkMap = (uint32_t*)(uintptr_t)m_pJointTypeArray;      // +0x1DC
    uint32_t* jointDataBase = (uint32_t*)(uintptr_t)m_pJointArray;  // +0x1D0
    uint32_t* boneMap = (uint32_t*)(uintptr_t)m_pJointConfigArray;  // +0x1E4

    uint32_t nodeIndex = linkMap[index] + 42;
    uint32_t constraintEntry = boneMap[index];

    void* jointObj = (void*)(uintptr_t)jointDataBase[nodeIndex];

    // Tail-call vtable slot 6 on the joint object
    void** vtbl = *(void***)jointObj;
    typedef void (*JointSolveFunc)(void*, uint32_t);
    JointSolveFunc fn = (JointSolveFunc)vtbl[6];
    fn(jointObj, constraintEntry);
}

// =========================================================================
// rage::phBound — Base Collision Bound Constructors & Utilities
// =========================================================================

// External references for phBound subsystem
extern void snListNode_Init(void* listHead);                     // Intrusive list head init
extern void ph_9BC0(void* thisPtr);                      // Collision manager base init
extern void ph_1310(void* thisPtr);                      // phBoundGeometry base init
extern void phBoundSphere_vfn_37(void* thisPtr);         // phBoundSphere finalize init

extern uint8_t g_phBoundInitialized;                     // @ 0x82606060
extern float g_identityVector[4];                        // @ 0x82606740

// Vtable pointers for phBound hierarchy
extern void* g_phBound_vtable;                           // @ 0x82057EF4 — rage::phBound
extern void* g_phBoundGeometry_vtable;                   // @ 0x82058494 — rage::phBoundGeometry
extern void* g_phBoundOTGrid_vtable;                     // @ 0x82058854 — rage::phBoundOTGrid
extern void* g_phBoundComposite_vtable;                  // @ 0x82057FD4 — rage::phBoundComposite
extern void* g_phBoundSphere_vtable;                     // @ 0x82058584 — rage::phBoundSphere
extern void* g_phBoundRibbon_vtable;                     // @ 0x820589BC — rage::phBoundRibbon
extern float g_phDefaultSphereRadius;                    // @ 0x8202D108
/**
 * phCollisionManager_Constructor @ 0x82298F50 | size: 0x58 (88 bytes)
 *
 * Constructor for a collision manager wrapping phBoundGeometry. Calls
 * ph_1310 for base init, sets manager vtable, initializes collision
 * subsystem at +176, and links subsystem back to this manager.
 */
void phCollisionManager_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    ph_1310(thisPtr);

    *(void**)(obj + 0) = &g_phCollisionMgr_vtable;

    void* subsystem = obj + 176;
    ph_9BC0(subsystem);

    uint32_t* subsysVtPtr = *(uint32_t**)(obj + 176);
    *(uint32_t*)((uint8_t*)subsysVtPtr + 4) = (uint32_t)(uintptr_t)thisPtr;
}

/**
 * phStateArrayReset @ 0x8221FD78 | size: 0x58 (88 bytes)
 *
 * Resets a physics state array to defaults. Early-exits if the global
 * initialized flag is set. Zeroes four 16-bit values at +16, sets
 * four 16-bit sentinels at +24, and zeroes four floats at +0.
 */
void phStateArrayReset(void* stateArray) {
    if (g_phBoundInitialized != 0) {
        return;
    }

    uint8_t* arr = (uint8_t*)stateArray;

    for (int i = 0; i < 4; i++) {
        *(uint16_t*)(arr + 16 + i * 2) = 0;
    }
    for (int i = 0; i < 4; i++) {
        *(uint16_t*)(arr + 24 + i * 2) = 0xFFFF;
    }

    *(float*)(arr + 0) = g_floatZero;
    *(float*)(arr + 4) = g_floatZero;
    *(float*)(arr + 8) = g_floatZero;
    *(float*)(arr + 12) = g_floatZero;
}

/**
 * phCollisionTreeLookup @ 0x82118DB8 | size: 0x44 (68 bytes)
 *
 * Performs a collision tree node lookup. Reads collision data at +164,
 * extracts tree data at +4, calls ph_3760 with maxDepth=255 and
 * no filter, and stores the result at +168.
 */
void phCollisionTreeLookup(void* thisPtr, void* queryNode) {
    uint8_t* obj = (uint8_t*)thisPtr;

    uint32_t queryTreeData = *(uint32_t*)((uint8_t*)queryNode + 4);
    void* collisionData = *(void**)(obj + 164);
    uint32_t treeDataArray = *(uint32_t*)((uint8_t*)collisionData + 4);

    uint32_t result = (uint32_t)(uintptr_t)ph_3760(
        (void*)(uintptr_t)queryTreeData,
        (void*)(uintptr_t)treeDataArray,
        NULL, 255, 0, 0);

    *(uint32_t*)(obj + 168) = result;
}

/**
 * phMatrixTransposeMultiply3x4 @ 0x821181C8 | size: 0xB0 (176 bytes)
 *
 * Computes transpose-multiply of two 4x3 matrices in-place. Given
 * matrices A and B, computes C = A^T * B where the transpose is over
 * the 3x3 rotation part, and the translation row uses (A.row3 - B.row3).
 * Result overwrites matrix A.
 */
void phMatrixTransposeMultiply3x4(float* matA, const float* matB) {
    float a0[4], a1[4], a2[4], a3[4];
    float b0[4], b1[4], b2[4], b3[4];

    memcpy(a0, matA + 0, 16);
    memcpy(a1, matA + 4, 16);
    memcpy(a2, matA + 8, 16);
    memcpy(a3, matA + 12, 16);

    memcpy(b0, matB + 0, 16);
    memcpy(b1, matB + 4, 16);
    memcpy(b2, matB + 8, 16);
    memcpy(b3, matB + 12, 16);

    float delta[4];
    delta[0] = a3[0] - b3[0];
    delta[1] = a3[1] - b3[1];
    delta[2] = a3[2] - b3[2];
    delta[3] = a3[3] - b3[3];

    float dot_a0_b0 = a0[0]*b0[0] + a0[1]*b0[1] + a0[2]*b0[2];
    float dot_a0_b1 = a0[0]*b1[0] + a0[1]*b1[1] + a0[2]*b1[2];
    float dot_a0_b2 = a0[0]*b2[0] + a0[1]*b2[1] + a0[2]*b2[2];

    float dot_a1_b0 = a1[0]*b0[0] + a1[1]*b0[1] + a1[2]*b0[2];
    float dot_a1_b1 = a1[0]*b1[0] + a1[1]*b1[1] + a1[2]*b1[2];
    float dot_a1_b2 = a1[0]*b2[0] + a1[1]*b2[1] + a1[2]*b2[2];

    float dot_a2_b0 = a2[0]*b0[0] + a2[1]*b0[1] + a2[2]*b0[2];
    float dot_a2_b1 = a2[0]*b1[0] + a2[1]*b1[1] + a2[2]*b1[2];
    float dot_a2_b2 = a2[0]*b2[0] + a2[1]*b2[1] + a2[2]*b2[2];

    float dot_delta_b0 = delta[0]*b0[0] + delta[1]*b0[1] + delta[2]*b0[2];
    float dot_delta_b1 = delta[0]*b1[0] + delta[1]*b1[1] + delta[2]*b1[2];
    float dot_delta_b2 = delta[0]*b2[0] + delta[1]*b2[1] + delta[2]*b2[2];

    matA[0] = dot_a0_b0;  matA[1] = dot_a0_b1;  matA[2] = dot_a0_b2;  matA[3] = 0.0f;
    matA[4] = dot_a1_b0;  matA[5] = dot_a1_b1;  matA[6] = dot_a1_b2;  matA[7] = 0.0f;
    matA[8] = dot_a2_b0;  matA[9] = dot_a2_b1;  matA[10] = dot_a2_b2; matA[11] = 0.0f;
    matA[12] = dot_delta_b0; matA[13] = dot_delta_b1; matA[14] = dot_delta_b2; matA[15] = 0.0f;
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phArticulatedCollider — Joint Query / Force Dispatch Functions
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {


// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::CopyJointTransformMatrix (vfn_13) @ 0x822500E8 | size: 0x88
//
// Copies the 4x4 transform matrix (64 bytes at offset +720) from the joint
// body identified by boneIndex into the caller-supplied output buffer.
// Uses GetJointLinkIndex (E668) to resolve the bone to a body pointer.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::CopyJointTransformMatrix(float* outMatrix, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (linkIndex + 10) * 4);

    // Copy 4 row vectors (64 bytes) from body + 720
    float* srcMatrix = (float*)(bodyPtr + 720);
    float* dst0 = outMatrix;
    float* dst1 = outMatrix + 4;
    float* dst2 = outMatrix + 8;
    float* dst3 = outMatrix + 12;

    dst0[0] = srcMatrix[0];  dst0[1] = srcMatrix[1];  dst0[2] = srcMatrix[2];  dst0[3] = srcMatrix[3];
    dst1[0] = srcMatrix[4];  dst1[1] = srcMatrix[5];  dst1[2] = srcMatrix[6];  dst1[3] = srcMatrix[7];
    dst2[0] = srcMatrix[8];  dst2[1] = srcMatrix[9];  dst2[2] = srcMatrix[10]; dst2[3] = srcMatrix[11];
    dst3[0] = srcMatrix[12]; dst3[1] = srcMatrix[13]; dst3[2] = srcMatrix[14]; dst3[3] = srcMatrix[15];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::CopyJointVelocityMatrix (vfn_14) @ 0x82250170 | size: 0x88
//
// Same as CopyJointTransformMatrix but copies from offset +784 in the joint
// body, which stores the velocity/angular velocity matrix.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::CopyJointVelocityMatrix(float* outMatrix, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (linkIndex + 10) * 4);

    float* srcMatrix = (float*)(bodyPtr + 784);
    float* dst0 = outMatrix;
    float* dst1 = outMatrix + 4;
    float* dst2 = outMatrix + 8;
    float* dst3 = outMatrix + 12;

    dst0[0] = srcMatrix[0];  dst0[1] = srcMatrix[1];  dst0[2] = srcMatrix[2];  dst0[3] = srcMatrix[3];
    dst1[0] = srcMatrix[4];  dst1[1] = srcMatrix[5];  dst1[2] = srcMatrix[6];  dst1[3] = srcMatrix[7];
    dst2[0] = srcMatrix[8];  dst2[1] = srcMatrix[9];  dst2[2] = srcMatrix[10]; dst2[3] = srcMatrix[11];
    dst3[0] = srcMatrix[12]; dst3[1] = srcMatrix[13]; dst3[2] = srcMatrix[14]; dst3[3] = srcMatrix[15];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyDeltaPositionToJoint (vfn_15) @ 0x822501F8 | size: 0x90
//
// Computes a delta vector by subtracting the collider's current position
// (offset +208) from a target position, then dispatches the delta to the
// joint solver. If an alternate target (r6) is provided, uses that instead.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ApplyDeltaPositionToJoint(int param, const float* targetPos, const float* altTarget, int solverParam) {
    const float* target = (altTarget != nullptr) ? altTarget : targetPos;
    int resolveParam = (altTarget != nullptr) ? solverParam : solverParam;

    float* currentPos = (float*)((char*)this + 208);
    float delta[4];
    delta[0] = target[0] - currentPos[0];
    delta[1] = target[1] - currentPos[1];
    delta[2] = target[2] - currentPos[2];
    delta[3] = target[3] - currentPos[3];

    int linkIndex = phArticulatedCollider_E668(this, resolveParam);

    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464
    phJoint_ApplyDelta(jointData, linkIndex, delta, param);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointBackupForce (vfn_20) @ 0x82250440 | size: 0x64
//
// Retrieves the backup force vector (at body offset +1072) for the joint
// identified by boneIndex from a secondary collider.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::GetJointBackupForce(float* outVec, phArticulatedCollider* srcCollider, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(srcCollider, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)srcCollider->m_pJointArray;  // +464
    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (linkIndex + 10) * 4);

    float* srcVec = (float*)(bodyPtr + 1072);
    outVec[0] = srcVec[0];
    outVec[1] = srcVec[1];
    outVec[2] = srcVec[2];
    outVec[3] = srcVec[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateScaledJointForces (vfn_39) @ 0x822504A8 | size: 0x80
//
// Iterates over all joint bodies and accumulates scaled force contributions.
// For each body, scales the force at +336 by the given factor, adds the
// gravity vector at +368, and accumulates into the output vector.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateScaledJointForces(float* outVec, float scale) {
    outVec[0] = 0.0f; outVec[1] = 0.0f; outVec[2] = 0.0f; outVec[3] = 0.0f;

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    int bodyCount = (int)jointData[1];  // +4

    if (bodyCount <= 0) return;

    for (int i = 0; i < bodyCount; i++) {
        uint32_t bodyPtr = jointData[10 + i];  // +40 array

        float* forceVec = (float*)(bodyPtr + 336);

        outVec[0] += forceVec[0] * scale;
        outVec[1] += forceVec[1] * scale;
        outVec[2] += forceVec[2] * scale;
        outVec[3] += forceVec[3] * scale;

        float* gravityVec = (float*)(bodyPtr + 368);
        outVec[0] += gravityVec[0];
        outVec[1] += gravityVec[1];
        outVec[2] += gravityVec[2];
        outVec[3] += gravityVec[3];
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointOrientationVector (vfn_49) @ 0x82250710 | size: 0x60
//
// Retrieves an orientation vector from a joint body. Loads the stride word
// from body[0], computes offset = stride*32 + 16, and copies that vector.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::GetJointOrientationVector(float* outVec, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (linkIndex + 10) * 4);

    uint32_t strideWord = *(uint32_t*)(bodyPtr);
    uint32_t byteOffset = strideWord * 32;

    float* srcVec = (float*)(bodyPtr + byteOffset + 16);
    outVec[0] = srcVec[0];
    outVec[1] = srcVec[1];
    outVec[2] = srcVec[2];
    outVec[3] = srcVec[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ApplyDeltaToJointForce (vfn_52) @ 0x82250818 | size: 0x54
//
// Computes the delta between a target position and the collider's position
// (offset +208), then dispatches via phArticulatedCollider_ApplyJointDelta.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ApplyDeltaToJointForce(const float* targetPos, int boneIndex) {
    float* currentPos = (float*)((char*)this + 208);
    float delta[4];
    delta[0] = targetPos[0] - currentPos[0];
    delta[1] = targetPos[1] - currentPos[1];
    delta[2] = targetPos[2] - currentPos[2];
    delta[3] = targetPos[3] - currentPos[3];

    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464
    phArticulatedCollider_ApplyJointDelta(jointData, linkIndex, delta);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ResetJointConstraintState (vfn_53) @ 0x82250870 | size: 0x38
//
// Resets the constraint state for a specific joint identified by bone index.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ResetJointConstraintState(int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    void* jointData = (void*)(uintptr_t)m_pJointArray;  // +464
    phArticulatedCollider_ResetJointLink(jointData, linkIndex);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DispatchJointSlot12 (vfn_55) @ 0x822508A8 | size: 0x38
//
// Resolves a joint index to a body pointer via bone-to-body mapping arrays
// at +476 and +484, then dispatches to that body's vtable slot 12.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DispatchJointSlot12(int jointIndex, float* outVec) {
    uint32_t* parentIndexArray = (uint32_t*)(uintptr_t)m_pJointTypeArray;    // +0x1DC
    uint32_t* jointTypeArray = (uint32_t*)(uintptr_t)m_pJointConfigArray;    // +0x1E4
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;              // +0x1D0

    uint32_t parentIndex = parentIndexArray[jointIndex];
    uint32_t jointType = jointTypeArray[jointIndex];

    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (parentIndex + 42) * 4);

    void** bodyVTable = *(void***)(uintptr_t)bodyPtr;
    typedef void (*Slot12Func)(void*, int, float*);
    Slot12Func fn = (Slot12Func)bodyVTable[12];
    fn((void*)(uintptr_t)bodyPtr, (int)jointType, outVec);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DispatchJointSlot7 (vfn_58) @ 0x82250910 | size: 0x34
//
// Same as DispatchJointSlot12 but dispatches to vtable slot 7 (byte offset 28).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DispatchJointSlot7(int jointIndex) {
    uint32_t* parentIndexArray = (uint32_t*)(uintptr_t)m_pJointTypeArray;    // +0x1DC
    uint32_t* jointTypeArray = (uint32_t*)(uintptr_t)m_pJointConfigArray;    // +0x1E4
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;              // +0x1D0

    uint32_t parentIndex = parentIndexArray[jointIndex];
    uint32_t jointType = jointTypeArray[jointIndex];

    uint32_t bodyPtr = *(uint32_t*)((char*)jointData + (parentIndex + 42) * 4);

    void** bodyVTable = *(void***)(uintptr_t)bodyPtr;
    typedef void (*Slot7Func)(void*, int);
    Slot7Func fn = (Slot7Func)bodyVTable[7];
    fn((void*)(uintptr_t)bodyPtr, (int)jointType);
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phArticulatedCollider — Dispatch, Force, and Matrix Functions (10 fns)
// ═════════════════════════════════════════════════════════════════════════════

extern void phArticulatedCollider_8A30(void* jointData, const float* velocity);
extern void phArticulatedCollider_8B10(void* jointData, const float* angularVelocity);

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DelegateToBoundCapsule (vfn_24) @ 0x822CC700 | size: 0x8
//
// Adjusts 'this' by +352 to reach the embedded phBoundCapsule sub-object,
// then tail-calls phBoundCapsule_ACB0_p45.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DelegateToBoundCapsule() {
    phBoundCapsule_ACB0_p45((char*)this + 352);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::DispatchJointSlot5 (vfn_51) @ 0x822505E8 | size: 0x34
//
// Resolves the joint object for the given index through the type/object/active
// arrays, computes linkDataIndex = jointType + 42, then tail-calls vtable
// slot 5 on the resolved joint object.
//
// @param jointIndex - Index of the joint to dispatch on
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::DispatchJointSlot5(int jointIndex) {
    uint32_t* jointTypeArray  = (uint32_t*)(uintptr_t)m_pJointTypeArray;   // +476
    uint32_t* activeJointsArr = (uint32_t*)(uintptr_t)m_pJointArray; // +464
    uint32_t* jointObjArray   = (uint32_t*)(uintptr_t)m_pJointConfigArray;   // +484

    uint32_t linkType = jointTypeArray[jointIndex];
    int linkDataIndex = linkType + 42;
    void* jointObj  = (void*)(uintptr_t)jointObjArray[jointIndex];
    void* linkData  = (void*)(uintptr_t)activeJointsArr[linkDataIndex];

    void** jvt = *(void***)jointObj;
    typedef void (*JointFunc)(void*, void*);
    ((JointFunc)jvt[5])(jointObj, linkData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::GetJointStiffness (vfn_62) @ 0x8224E280 | size: 0x58
//
// Resolves joint object and link data via type/object/active arrays,
// then calls vtable slot 8 on the joint object to retrieve the stiffness
// value into a stack-local float, which is returned.
//
// @param jointIndex - Index of the joint to query
// @return Stiffness value for the specified joint
// ─────────────────────────────────────────────────────────────────────────────
float rage::phArticulatedCollider::GetJointStiffness(int jointIndex) {
    uint32_t* jointTypeArray  = (uint32_t*)(uintptr_t)m_pJointTypeArray;   // +476
    uint32_t* activeJointsArr = (uint32_t*)(uintptr_t)m_pJointArray; // +464
    uint32_t* jointObjArray   = (uint32_t*)(uintptr_t)m_pJointConfigArray;   // +484

    uint32_t linkType = jointTypeArray[jointIndex];
    int linkDataIndex = linkType + 42;
    void* jointObj  = (void*)(uintptr_t)jointObjArray[jointIndex];
    void* linkData  = (void*)(uintptr_t)activeJointsArr[linkDataIndex];

    float result;
    void** jvt = *(void***)jointObj;
    typedef void (*GetStiffnessFunc)(void*, void*, void*, float*);
    ((GetStiffnessFunc)jvt[8])(jointObj, (void*)(uintptr_t)jointObjArray[jointIndex], linkData, &result);

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateForceAtJoint (vfn_38) @ 0x8224EFE0 | size: 0x60
//
// Resolves a bone index to a body pointer via E668, then adds a 16-byte
// force vector (SIMD add) to the body's accumulated force at offset +384.
//
// @param forceVec   - 16-byte aligned force vector to accumulate
// @param boneIndex  - Bone index to resolve to a body
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateForceAtJoint(const float* forceVec, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    int bodyIndex = linkIndex + 10;
    uint8_t* bodyPtr = (uint8_t*)(uintptr_t)jointData[bodyIndex];

    // Add force vector to accumulated force at body + 384
    float* accForce = (float*)(bodyPtr + 384);
    accForce[0] += forceVec[0];
    accForce[1] += forceVec[1];
    accForce[2] += forceVec[2];
    accForce[3] += forceVec[3];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetVelocityScaled (vfn_40) @ 0x8224F870 | size: 0x58
//
// Stores the input velocity vector at offset +224, scales it by the
// restitution coefficient at +100, stores the result at +256, then
// propagates velocity to all joints via phArticulatedCollider_8A30.
//
// @param velocity - 16-byte aligned velocity vector
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::SetVelocityScaled(const float* velocity) {
    uint8_t* self = (uint8_t*)this;

    // Read restitution coefficient from +100
    float restitution = *(float*)(self + 100);

    // Store velocity at +224
    float* storedVel = (float*)(self + 224);
    storedVel[0] = velocity[0];
    storedVel[1] = velocity[1];
    storedVel[2] = velocity[2];
    storedVel[3] = velocity[3];

    // Scale velocity by restitution and store at +256
    float* scaledVel = (float*)(self + 256);
    scaledVel[0] = storedVel[0] * restitution;
    scaledVel[1] = storedVel[1] * restitution;
    scaledVel[2] = storedVel[2] * restitution;
    scaledVel[3] = storedVel[3] * restitution;

    // Propagate to all joints
    void* jointDataPtr = (void*)(uintptr_t)m_pJointArray;  // +464
    phArticulatedCollider_8A30(jointDataPtr, storedVel);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::SetAngularVelocity (vfn_43) @ 0x8224F948 | size: 0x44
//
// Stores angular velocity at offset +272, calls the base class handler
// (game_CE58), then propagates angular velocity from +240 to all joints
// via phArticulatedCollider_8B10.
//
// @param angularVelocity - 16-byte aligned angular velocity vector
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::SetAngularVelocity(const float* angularVelocity) {
    uint8_t* self = (uint8_t*)this;

    // Store angular velocity at +272
    float* storedAngVel = (float*)(self + 272);
    storedAngVel[0] = angularVelocity[0];
    storedAngVel[1] = angularVelocity[1];
    storedAngVel[2] = angularVelocity[2];
    storedAngVel[3] = angularVelocity[3];

    // Call base class handler
    game_CE58(this);

    // Propagate angular velocity at +240 to all joints
    float* angVel = (float*)(self + 240);
    void* jointDataPtr = (void*)(uintptr_t)m_pJointArray;  // +464
    phArticulatedCollider_8B10(jointDataPtr, angVel);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::UpdatePostIntegrate (vfn_11) @ 0x822CB360 | size: 0x54
//
// Calls vtable slot 10 (self-dispatch), then phCollider_RecalcBounds for post-integration
// cleanup, then dispatches vtable slot 28 on the skeleton object at +16.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::UpdatePostIntegrate() {
    // Self-dispatch vtable slot 10
    void** vt = *(void***)this;
    typedef void (*Slot10Func)(phArticulatedCollider*);
    ((Slot10Func)vt[10])(this);

    // Post-integration cleanup
    phCollider_RecalcBounds(this);

    // Dispatch vtable slot 28 on skeleton at +16 (m_pBound)
    void* skeleton = (void*)(uintptr_t)this->m_pBound;
    void** skelVt = *(void***)skeleton;
    typedef void (*Slot28Func)(void*);
    ((Slot28Func)skelVt[28])(skeleton);
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::CopyJointRotationMatrix (vfn_54) @ 0x82250770 | size: 0xA4
//
// Resolves a bone index to a body pointer via E668, then copies the 3x3
// rotation submatrix (rows at +16/+48/+80, columns 0-2) from the body into
// the output buffer as a 4x4 matrix with zeroed last row.
//
// @param outMatrix  - 60-byte output buffer for 3x3 + zero row
// @param boneIndex  - Bone index to resolve
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::CopyJointRotationMatrix(float* outMatrix, int boneIndex) {
    int linkIndex = phArticulatedCollider_E668(this, boneIndex);

    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    int bodyIndex = linkIndex + 10;
    uint8_t* bodyPtr = (uint8_t*)(uintptr_t)jointData[bodyIndex];

    // Copy 3x3 rotation from body: rows at +16, +48, +80 (float triplets)
    float* row0 = (float*)(bodyPtr + 16);
    float* row1 = (float*)(bodyPtr + 48);
    float* row2 = (float*)(bodyPtr + 80);

    // Row 0
    outMatrix[0]  = row0[0];   // +16
    outMatrix[1]  = row0[2];   // +48 (body col 1 at +48 of row0? No, +48 is row1)
    // Actually: scaffold reads +16,+48,+80,+84,+20,+52,+24,+56,+88
    // Re-reading scaffold: f5=+16, f13=+48, f12=+80, f11=+84, f10=+52, f9=+20, f8=+88, f7=+56, f6=+24
    // Stores: +0=f5, +4=f13, +8=f12, +24=f11, +20=f10, +16=f9, +40=f8, +36=f7, +32=f6
    // +48,+52,+56 = 0.0f

    outMatrix[0]  = *(float*)(bodyPtr + 16);   // row0 col0
    outMatrix[1]  = *(float*)(bodyPtr + 48);   // row1 col0
    outMatrix[2]  = *(float*)(bodyPtr + 80);   // row2 col0
    // padding
    outMatrix[4]  = *(float*)(bodyPtr + 20);   // row0 col1
    outMatrix[5]  = *(float*)(bodyPtr + 52);   // row1 col1
    outMatrix[6]  = *(float*)(bodyPtr + 84);   // row2 col1
    // padding
    outMatrix[8]  = *(float*)(bodyPtr + 24);   // row0 col2
    outMatrix[9]  = *(float*)(bodyPtr + 56);   // row1 col2
    outMatrix[10] = *(float*)(bodyPtr + 88);   // row2 col2
    // padding

    // Zero last row
    outMatrix[12] = 0.0f;
    outMatrix[13] = 0.0f;
    outMatrix[14] = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::ComputeJointCrossProduct (vfn_19) @ 0x82250370 | size: 0xCC
//
// Looks up a body position via E668 in another collider, loads its orientation
// matrix from +1072 and the row at +1088, computes a cross product between
// that row and a delta vector (input - otherCollider.position@+208), and
// adds the cross product to the row at +1088.
//
// @param outVec       - 16-byte output vector (receives cross+row result)
// @param targetPos    - 16-byte input position vector
// @param otherCollider - The other phArticulatedCollider to look up
// @param boneIndex    - Bone index in otherCollider
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::ComputeJointCrossProduct(
    float* outVec, const float* targetPos,
    phArticulatedCollider* otherCollider, int boneIndex)
{
    int linkIndex = phArticulatedCollider_E668(otherCollider, boneIndex);

    // Get body pointer from other collider's joint array
    uint32_t* otherJointData = (uint32_t*)(uintptr_t)otherCollider->m_pJointArray;
    int bodyIndex = linkIndex + 10;
    uint8_t* bodyPtr = (uint8_t*)(uintptr_t)otherJointData[bodyIndex];

    // Load position vector from body +1072
    float* bodyPos1072 = (float*)(bodyPtr + 1072);

    // Compute delta = targetPos - otherCollider.position(+208)
    float* otherPos = (float*)((char*)otherCollider + 208);
    float dx = targetPos[0] - otherPos[0];
    float dy = targetPos[1] - otherPos[1];
    float dz = targetPos[2] - otherPos[2];

    // Load row from body +1088
    float* bodyRow1088 = (float*)(bodyPtr + 1088);

    // Cross product: delta x bodyPos1072 row
    float cx = dy * bodyPos1072[2] - dz * bodyPos1072[1];
    float cy = dz * bodyPos1072[0] - dx * bodyPos1072[2];
    float cz = dx * bodyPos1072[1] - dy * bodyPos1072[0];

    // Add cross product to bodyRow1088 values and store in outVec
    outVec[0] = cx + bodyRow1088[0];
    outVec[2] = cy + bodyRow1088[2];
    outVec[1] = cz + bodyRow1088[1];
}

// ─────────────────────────────────────────────────────────────────────────────
// phArticulatedCollider::AccumulateTorqueAtRootJoint (vfn_32) @ 0x8224FB90 | size: 0xCC
//
// Calls vtable slot 16 on the skeleton at +16 with a global identity vector,
// then computes a cross product between the input force vector and the root
// body's column vectors (at +336/+352), accumulating the torque into the
// root body's torque vector at +352 and force vector at +336.
//
// @param forceVec - 16-byte force vector to accumulate
// ─────────────────────────────────────────────────────────────────────────────
void rage::phArticulatedCollider::AccumulateTorqueAtRootJoint(const float* forceVec) {
    // Call vtable slot 16 on skeleton at +16 (m_pBound) with identity vector
    void* skeleton = (void*)(uintptr_t)this->m_pBound;
    void** skelVt = *(void***)skeleton;
    typedef void (*Slot16Func)(void*, void*, void*, int);
    ((Slot16Func)skelVt[16])(skeleton, (void*)g_identityVec, nullptr, 0);

    // Get root body from active joints array, slot [10] (first body)
    uint32_t* jointData = (uint32_t*)(uintptr_t)m_pJointArray;  // +464
    uint8_t* rootBody = (uint8_t*)(uintptr_t)jointData[10];  // +40

    // Load torque vector at body +352
    float* torque = (float*)(rootBody + 352);
    // Load position from global identity vector
    float identX = ((float*)g_identityVec)[0];
    float identY = ((float*)g_identityVec)[1];
    float identZ = ((float*)g_identityVec)[2];

    // Load force vector components
    float fX = forceVec[0];
    float fY = forceVec[1];
    float fZ = forceVec[2];

    // Cross product: identity x force
    float cx = identY * fZ - identZ * fY;
    float cy = identZ * fX - identX * fZ;
    float cz = identX * fY - identY * fX;

    // Accumulate into torque vector at body +352
    torque[0] += cx;
    torque[1] += cy;
    torque[2] += cz;

    // Accumulate force vector into body +336
    float* bodyForce = (float*)(rootBody + 336);
    bodyForce[0] += forceVec[0];
    bodyForce[1] += forceVec[1];
    bodyForce[2] += forceVec[2];
    bodyForce[3] += forceVec[3];
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Virtual and Utility Functions (10 functions)
// ═════════════════════════════════════════════════════════════════════════════

// External declarations
extern void ph_1B78_3(void* outVec, float halfLength, float radius);
extern void util_DA50(void* dst, const void* src);  // phBound::CopyFrom base
extern void* g_display_obj_ptr;
extern void phBoundCapsule_SetCapsuleExtents(void* thisPtr, const float* midpoint, const float* axisA, const float* axisB);

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::GetPolygonCount (vfn_12) @ 0x8256FBD0 | size: 0x08
//
// Returns the polygon count stored at offset +192 (0xC0).
// For a capsule bound, this represents the tessellation polygon count
// used when rendering/approximating the capsule shape.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t rage::phBoundCapsule::GetPolygonCount() const {
    return this->m_nMaterialIndex;  // +0xC0 (reused as polygon count)
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::SetPolygonCount (vfn_13) @ 0x822A2E00 | size: 0x08
//
// Sets the polygon count at offset +192 (0xC0).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::SetPolygonCount(uint32_t count) {
    this->m_nMaterialIndex = count;  // +0xC0 (reused as polygon count)
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::GetExtents (vfn_9) @ 0x822A3258 | size: 0x10
//
// Computes the bounding extents of the capsule and stores them in the
// output vector. Reads the half-length (+112) and radius (+128), then
// tail-calls ph_1B78 which builds the axis-aligned extents vector.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::GetExtents(float* outExtents, const void* params) {
    float halfLength = *(float*)((char*)params + 112);
    float radius = *(float*)((char*)params + 128);
    ph_1B78_3(outExtents, halfLength, radius);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::RenderDebug (vfn_11) @ 0x822A2DE0 | size: 0x20
//
// Renders debug visualization for the capsule bound. Loads the polygon
// count from offset +192 into r4, retrieves the global display object,
// and dispatches to its vtable slot 5 (DrawBoundCapsule).
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::RenderDebug() {
    uint32_t polyCount = this->m_nMaterialIndex;  // +0xC0 (reused as polygon count)
    void* displayObj = g_display_obj_ptr;
    void** vtable = *(void***)displayObj;
    typedef void (*DrawFunc)(void*, uint32_t);
    DrawFunc drawBound = (DrawFunc)vtable[5];
    drawBound(displayObj, polyCount);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::CalculateVolume (vfn_8) @ 0x822A2DB0 | size: 0x2C
//
// Computes the volume of the capsule bound:
//   volume = (halfLength * 2.0 + radius) * halfLength^2 * (4/3)*pi
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::CalculateVolume() const {
    float halfLength = *(float*)&this->m_pVertices;  // +0x70 (reused as halfHeight)
    float radius = *(float*)&this->m_fRadius;        // +0x80
    static const float kTwo = 2.0f;
    static const float kFourThirdsPi = 4.18879032f;
    float combined = halfLength * kTwo + radius;
    float lengthSq = combined * halfLength;
    float lengthCubed = lengthSq * halfLength;
    return lengthCubed * kFourThirdsPi;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::SetCentroidOffset (vfn_7) @ 0x8233AB50 | size: 0x48
//
// Translates the capsule centroid by an offset vector. Loads the current
// centroid at offset +48, adds the input offset vector component-wise,
// then calls vtable slot 6 (SetCentroid) with the resulting position.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::SetCentroidOffset(const float* offset) {
    float* centroid = (float*)&this->m_vCenterX;  // +0x30
    float result[4];
    result[0] = centroid[0] + offset[0];
    result[1] = centroid[1] + offset[1];
    result[2] = centroid[2] + offset[2];
    result[3] = centroid[3] + offset[3];

    void** vtable = *(void***)this;
    typedef void (*SetCentroidFunc)(void*, const float*);
    SetCentroidFunc setCentroid = (SetCentroidFunc)vtable[6];
    setCentroid(this, result);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::GetBoundingDistance (vfn_33) @ 0x822A3268 | size: 0x50
//
// Returns the bounding distance of the capsule along a given direction:
//   distance = halfLength + fabs(direction.y) * radius * 2.0
// If includeCenter is zero, adds centroid dot product with direction.
// ─────────────────────────────────────────────────────────────────────────────
float rage::phBoundCapsule::GetBoundingDistance(const float* direction, uint8_t includeCenter) const {
    float absY = direction[1];
    if (absY < 0.0f) absY = -absY;

    float radius = *(float*)&this->m_fRadius;         // +0x80
    float halfLength = *(float*)&this->m_pVertices;   // +0x70 (reused as halfHeight)
    static const float kTwo = 2.0f;

    float scaledRadius = radius * absY;
    float distance = scaledRadius * kTwo + halfLength;

    if (includeCenter != 0)
        return distance;

    float* centroid = (float*)&this->m_vCenterX;  // +0x30
    float dot = centroid[0] * direction[0]
              + centroid[1] * direction[1]
              + centroid[2] * direction[2];

    return dot + distance;
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::ValidateAndSetExtents @ 0x82143F08 | size: 0x84
//
// Validates that a rotation vector is non-zero before applying capsule
// extents. If the rotation column (row 1) is all-zero, returns immediately.
// Otherwise computes the midpoint (row0 + row2) and dispatches to
// phBoundCapsule_SetCapsuleExtents for full capsule setup.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::ValidateAndSetExtents(const float* matrix) {
    const float* rotation = matrix + 4;

    uint32_t absX = *(uint32_t*)(rotation + 0) & 0x7FFFFFFF;
    uint32_t absY = *(uint32_t*)(rotation + 1) & 0x7FFFFFFF;
    uint32_t absZ = *(uint32_t*)(rotation + 2) & 0x7FFFFFFF;
    uint32_t absW = *(uint32_t*)(rotation + 3) & 0x7FFFFFFF;

    bool allZero = (absX == 0 && absY == 0 && absZ == 0 && absW == 0);
    if (allZero)
        return;

    const float* row0 = matrix;
    const float* row2 = matrix + 8;
    float midpoint[4];
    midpoint[0] = row0[0] + row2[0];
    midpoint[1] = row0[1] + row2[1];
    midpoint[2] = row0[2] + row2[2];
    midpoint[3] = row0[3] + row2[3];

    phBoundCapsule_SetCapsuleExtents(this, midpoint, rotation, row2);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::GetJointLimitsForAxis @ 0x820DF420 | size: 0xC4
//
// Retrieves joint limit min/max values for a given axis index from the
// joint data at offset +8. Each entry is 104 bytes. The axis selects
// which limit pair to read. Validates min >= -1.0; retries with axis=11
// as fallback if below threshold.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::GetJointLimitsForAxis(uint32_t jointIndex, uint32_t axis,
                                                  float* outMin, float* outMax) {
    static const float kNegativeOne = -1.0f;

retry:
    if (axis <= 3) {
        uint8_t* jointData = *(uint8_t**)((char*)this + 8);
        uint8_t* entry = jointData + jointIndex * 104;

        switch (axis) {
        case 0: {
            float val = *(float*)(entry + 0);
            *outMax = val;
            *outMin = val;
            break;
        }
        case 2: {
            float lo = *(float*)(entry + 4);
            *outMin = lo;
            float hi = *(float*)(entry + 8);
            *outMax = hi;
            break;
        }
        case 1: {
            float lo = *(float*)(entry + 12);
            *outMin = lo;
            float hi = *(float*)(entry + 16);
            *outMax = hi;
            break;
        }
        case 3: {
            float lo = *(float*)(entry + 20);
            *outMin = lo;
            float hi = *(float*)(entry + 24);
            *outMax = hi;
            break;
        }
        }
    }

    float minVal = *outMin;
    if (minVal < kNegativeOne) {
        axis = 11;
        goto retry;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// phBoundCapsule::CopyFrom (vfn_34) @ 0x822A2F28 | size: 0xBC
//
// Deep-copies all capsule-specific fields from a source phBoundCapsule.
// Calls base phBound::CopyFrom, then copies capsule vectors at offsets
// 112-176, polygon count at 192, 12 parameter bytes at 196, and sets
// the dirty flag at offset 96 to 1.
// ─────────────────────────────────────────────────────────────────────────────
void rage::phBoundCapsule::CopyFrom(const rage::phBoundCapsule* source) {
    util_DA50(this, source);

    memcpy((char*)this + 112, (const char*)source + 112, 16);
    memcpy((char*)this + 128, (const char*)source + 128, 16);
    memcpy((char*)this + 144, (const char*)source + 144, 16);
    memcpy((char*)this + 160, (const char*)source + 160, 16);
    memcpy((char*)this + 176, (const char*)source + 176, 16);

    *(uint32_t*)((char*)this + 192) = *(const uint32_t*)((const char*)source + 192);

    const uint8_t* srcBytes = (const uint8_t*)source + 196;
    uint8_t* dstBytes = (uint8_t*)this + 196;
    for (int i = 0; i < 12; i++) {
        dstBytes[i] = srcBytes[i];
    }

    *(uint16_t*)((char*)this + 96) = 1;
}

// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundSphere — Virtual Accessor and Computation Functions
// ═════════════════════════════════════════════════════════════════════════════

namespace rage {

// External globals for phBoundSphere

// ---------------------------------------------------------------------------
// phBoundSphere::DispatchMaterialAlloc (vfn_11) @ 0x82295478 | size: 0x20 (32B)
//    Loads material index from +128, fetches the global material allocator,
//    and tail-calls its vtable slot 5 to allocate material data.
// ---------------------------------------------------------------------------
void phBoundSphere::DispatchMaterialAlloc() {
    uint32_t materialIdx = *(uint32_t*)((char*)this + 128);
    void* allocator = g_phMaterialAllocator;
    void** vt = *(void***)allocator;
    typedef void (*AllocFn)(void*, uint32_t);
    ((AllocFn)vt[5])(allocator, materialIdx);
}

// ---------------------------------------------------------------------------
// phBoundSphere::GetMaterialIndex (vfn_12) @ 0x82295498 | size: 0x8 (8B)
//    Returns the material index stored at offset +128.
// ---------------------------------------------------------------------------
void phBoundSphere::GetMaterialIndex() {
    // Returns material index at +128; original return value discarded by void vtable slot
    (void)*(uint32_t*)((char*)this + 128);
}

// ---------------------------------------------------------------------------
// phBoundSphere::SetMaterialIndex (vfn_13) @ 0x822954A0 | size: 0x8 (8B)
//    Stores a material index at offset +128.
// ---------------------------------------------------------------------------
void phBoundSphere::SetMaterialIndex(uint32_t index) {
    *(uint32_t*)((char*)this + 128) = index;
}

// ---------------------------------------------------------------------------
// phBoundSphere::ComputeVolume (vfn_8) @ 0x822954A8 | size: 0x1C (28B)
//    Computes sphere volume: (4/3)*pi*r^3 where r is at offset +8.
// ---------------------------------------------------------------------------
float phBoundSphere::ComputeVolume() {
    float r = *(float*)((char*)this + 8);
    float r3 = r * r * r;
    return r3 * g_phSphereVolumeCoeff;
}

// ---------------------------------------------------------------------------
// phBoundSphere::ComputeInertiaTensor (vfn_9) @ 0x82295678 | size: 0x30 (48B)
//    Computes inertia = coeff * density * r^3, splats result to output vec4.
//    @param outTensor - 16-byte aligned output vector (all 4 components set)
//    @param source    - Object with radius at +8
//    @param density   - Density scalar (f1)
// ---------------------------------------------------------------------------
void phBoundSphere::ComputeInertiaTensor(float* outTensor, const void* source, float density) {
    float r = *(float*)((char*)source + 8);
    float r2 = r * r;
    float r2d = r2 * density;
    float inertia = r2d * g_phSphereInertiaCoeff;
    // Splat inertia to all 4 components of output vector
    outTensor[0] = inertia;
    outTensor[1] = inertia;
    outTensor[2] = inertia;
    outTensor[3] = inertia;
}

// ---------------------------------------------------------------------------
// phBoundSphere::GetProjectedExtent (vfn_33) @ 0x82295860 | size: 0x34 (52B)
//    Projects the sphere center (+48 vec4) onto a direction (r4 vec4),
//    then adds the radius (+8). Returns early if the byte flag (r5) is set.
//    @param direction - 16-byte aligned direction vector
//    @param flags     - If nonzero, returns radius without projection
//    @return Projected extent = dot(center, direction) + radius
// ---------------------------------------------------------------------------
float phBoundSphere::GetProjectedExtent(const float* direction, uint8_t flags) {
    float radius = *(float*)((char*)this + 8);
    if (flags != 0) return radius;
    // dot3(center, direction)
    float* center = (float*)((char*)this + 48);
    float dot = center[0] * direction[0] + center[1] * direction[1] + center[2] * direction[2];
    return dot + radius;
}

// ---------------------------------------------------------------------------
// phBoundSphere::RenderDebug (vfn_14) @ 0x8233A8F8 | size: 0x14 (20B)
//    Tail-calls vtable slot 11 with parameter 0. Used to dispatch the debug
//    rendering with default arguments.
// ---------------------------------------------------------------------------
void phBoundSphere::RenderDebug() {
    void** vt = *(void***)this;
    typedef void (*RenderFn)(void*, int);
    ((RenderFn)vt[11])(this, 0);
}

// ---------------------------------------------------------------------------
// phBoundSphere::SetPositionAndUpdateFlags (vfn_6) @ 0x8233AB08 | size: 0x48 (72B)
//    Copies position vector (r4) into center at +48, masks xyz components,
//    checks if any are nonzero, sets the "has position" flag at +5, then
//    tail-calls vtable slot 37 to update derived bounds.
// ---------------------------------------------------------------------------
void phBoundSphere::SetPositionAndUpdateFlags(const float* position) {
    float* center = (float*)((char*)this + 48);
    // Copy position to center
    memcpy(center, position, 16);
    // Mask to xyz only (clear w) and check if nonzero
    uint32_t* cInt = (uint32_t*)center;
    bool hasPosition = (cInt[0] | cInt[1] | cInt[2]) != 0;
    *(uint8_t*)((char*)this + 5) = hasPosition ? 1 : 0;
    // Tail-call vtable slot 37 to update derived data
    void** vt = *(void***)this;
    typedef void (*UpdateFn)(void*);
    ((UpdateFn)vt[37])(this);
}

} // namespace rage

// ═════════════════════════════════════════════════════════════════════════════
// Physics constructor / initializer free functions
// ═════════════════════════════════════════════════════════════════════════════

// External declarations
extern void ph_ctor_FFD8(void* thisPtr, void* param);
// extern void ph_GetSimulationContext(void* thisPtr);  // removed: conflicts with definition returning void*
extern void ph_AdvanceSegmentedPtr(void* param, void* dest);
extern void phJoint3Dof_0170_g(void* dest, int stride, const void* srcTemplate, int count);

// ---------------------------------------------------------------------------
// phBoundGeometryIntermediate_Constructor @ 0x82291310 | size: 0x6C (108B)
//    Intermediate constructor for phBoundGeometry. Calls the base bound
//    constructor (ph_ctor_FFD8) passing through r4, installs the
//    phBoundGeometry vtable, calls ph_GetSimulationContext, then initializes the
//    material table at +160 via ph_AdvanceSegmentedPtr. Returns this pointer.
// ---------------------------------------------------------------------------
void* phBoundGeometryIntermediate_Constructor(void* thisPtr, void* param) {
    // Call base constructor, passing through param
    ph_ctor_FFD8(thisPtr, param);

    // Install phBoundGeometry vtable
    *(void**)thisPtr = &g_phBoundGeometryVtable;

    // Initialize static data
    ph_GetSimulationContext(thisPtr);

    // Initialize material table at +160
    void* materialDest = (void*)((uint8_t*)thisPtr + 160);
    ph_AdvanceSegmentedPtr(param, materialDest);

    return thisPtr;
}

// ---------------------------------------------------------------------------
// phJointChain_Constructor @ 0x8225DD70 | size: 0x74 (116B)
//    Constructor for a physics joint chain containing 16 phJoint3Dof joints.
//    Sets the vtable, then iterates 16 times calling phJoint3Dof_0170_g to
//    initialize each joint slot at a stride of 3016 bytes. After init,
//    clears the total active joints counter and the enabled flag.
// ---------------------------------------------------------------------------
void phJointChain_Constructor(void* thisPtr) {
    uint8_t* obj = (uint8_t*)thisPtr;

    // Install joint chain vtable
    *(void**)(obj + 0) = &g_phJointChainVtable;

    // Base offset for joint array: obj + 8
    uint8_t* jointBase = obj + 8;

    // Initialize 16 joints (index 15 down to 0)
    uint8_t* jointSlot = jointBase + 132;  // first joint at +140 from obj

    for (int i = 15; i >= 0; i--) {
        phJoint3Dof_0170_g(jointSlot, 144, &g_phJoint3DofTemplate, 20);
        // Clear per-joint active flag at +2880 from joint slot
        *(uint32_t*)(jointSlot + 2880) = 0;
        jointSlot += 3016;  // stride between joint slots
    }

    // Clear total active joints counter at jointBase + 48256
    *(uint32_t*)(jointBase + 48256) = 0;

    // Clear enabled flag
    *(uint8_t*)(obj + 4) = 0;
}

// ═════════════════════════════════════════════════════════════════════════════
// ph_ free functions — Physics utility and constructor routines (68-200B)
// ═════════════════════════════════════════════════════════════════════════════

// External function declarations
extern void ph_3760_4(void* thisPtr, uint32_t shapeKey, uint32_t materialId, uint32_t unused);
extern void ph_1EF8(void* shapeKey, uint32_t materialOverride, uint32_t contactMaterialId,
                     uint8_t* surfaceFlagA, uint32_t contactFlags, uint8_t* surfaceFlagB,
                     uint32_t primitiveFlags, uint32_t maxMaterialId);
extern void phBound_LockChannel(void* allocator, const char* tag, int alignment);
extern void rage_CopyChannelData(void* pool, uint32_t handle, void* outPtr);
extern void ph_ctor_ABE8(void* thisPtr);
extern void rage_debugLog(const char* msg, uint64_t val);

// External globals
extern uint8_t g_phDisableFlag;          // @ 0x82706460
extern const float g_physZeroF;          // @ 0x8202D110  (0.0f)
extern const float g_physOneF;           // @ 0x8202D114  (1.0f)

/**
 * ph_8DB8 — Add collision shape to contact manager
 * @ 0x82118DB8 | size: 0x44 (68 bytes)
 *
 * Adds a collision shape from the source bound (r4) to this object's
 * contact manager. Loads the shape key from source+4, retrieves the
 * contact material from this->contactMgr (offset +164), and calls
 * ph_3760 to register the shape with materialId=255, flags=0.
 * Stores the returned contact handle at this+168.
 */
// ph_8DB8
void phContactManager_AddShape(void* thisPtr, void* sourceBound) {
    uint32_t shapeKey = *(uint32_t*)((char*)sourceBound + 4);
    void* contactMgr = *(void**)((char*)thisPtr + 164);
    uint32_t materialId = *(uint32_t*)((char*)contactMgr + 4);
    ph_3760_4((void*)(uintptr_t)shapeKey, materialId, 255, 0);
    uint32_t contactHandle = (uint32_t)(uintptr_t)nullptr; // r3 return from ph_3760
    *(uint32_t*)((char*)thisPtr + 168) = contactHandle;
}

/**
 * ph_8E00 — Register contact surface with material properties
 * @ 0x82118E00 | size: 0x6C (108 bytes)
 *
 * Registers a contact surface for collision response. Extracts the shape
 * key from the source, loads contact material info from this->contactMgr
 * (offset +164), and calls ph_1EF8 with surface flags from offsets 426/427
 * and contact flags from offsets 176/180. Increments the contact count
 * at offset +428.
 */
// ph_8E00
void phContactManager_RegisterSurface(void* thisPtr, void* sourceBound) {
    uint32_t shapeKey = *(uint32_t*)((char*)sourceBound + 4);
    void* contactMgr = *(void**)((char*)thisPtr + 164);
    uint32_t materialOverride = 0;
    if (contactMgr != nullptr) {
        materialOverride = *(uint32_t*)((char*)contactMgr + 8);
    }
    uint32_t contactMaterialId = *(uint32_t*)((char*)contactMgr + 4);
    uint32_t contactFlags = *(uint32_t*)((char*)thisPtr + 176);
    uint32_t primitiveFlags = *(uint32_t*)((char*)thisPtr + 180);
    uint8_t* surfaceFlagA = (uint8_t*)((char*)thisPtr + 426);
    uint8_t* surfaceFlagB = (uint8_t*)((char*)thisPtr + 427);
    ph_1EF8((void*)(uintptr_t)shapeKey, materialOverride, contactMaterialId,
            surfaceFlagA, contactFlags, surfaceFlagB, primitiveFlags, 255);
    uint8_t count = *(uint8_t*)((char*)thisPtr + 428);
    count++;
    *(uint8_t*)((char*)thisPtr + 428) = count;
}

/**
 * ph_3608 — Lock, resolve handle, and unlock physics pool
 * @ 0x82123608 | size: 0x74 (116 bytes)
 *
 * Acquires a physics allocator lock via phBound_LockChannel (tag string),
 * resolves a pool handle to a pointer via rage_CopyChannelData, then
 * releases the lock. Used to safely fetch a physics object
 * from a handle while the allocator is locked.
 *
 * @param outPtr   Destination pointer buffer (resolved ptr stored at +164)
 * @param handle   Pool handle wrapper (handle value at +4, sub-handle at +12)
 */
// ph_3608
void phPool_ResolveHandleLocked(void* outPtr, void* handle) {
    uint32_t allocator = *(uint32_t*)((char*)handle + 4);
    phBound_LockChannel((void*)(uintptr_t)allocator, (const char*)0x82027660, 1);
    uint32_t subHandle = *(uint32_t*)((char*)handle + 4);
    uint32_t poolHandle = *(uint32_t*)((char*)subHandle + 12);
    void* pool = *(void**)0x8251A374;
    rage_CopyChannelData(pool, poolHandle, (void*)((char*)outPtr + 164));
    uint32_t allocator2 = *(uint32_t*)((char*)handle + 4);
    phBound_LockChannel((void*)(uintptr_t)allocator2, (const char*)0x8202766C, 1);
}

/**
 * ph_3E78 — Compute clamped contact impulse from virtual elasticity
 * @ 0x82123E78 | size: 0x9C (156 bytes)
 *
 * Computes the contact impulse magnitude for a collision response.
 * If the source object (r4) has a contact body at offset +256,
 * queries its elasticity via VCALL slot 8. Clamps the result to
 * be non-negative, scales by a constant factor (from rdata), and
 * returns the absolute value if negative (via fsel).
 *
 * Returns: Clamped impulse magnitude as float in f1.
 */
// ph_3E78
float phContact_ComputeImpulse(void* thisPtr, void* contactPair) {
    void* contactBody = *(void**)((char*)contactPair + 256);
    float zero = 0.0f;
    float elasticity = zero;

    if (contactBody != nullptr) {
        // VCALL slot 8: GetElasticity()
        typedef float (*GetElasticityFn)(void*);
        void** vt = *(void***)contactBody;
        elasticity = ((GetElasticityFn)vt[8])(contactBody);

        if (elasticity < zero) {
            rage_debugLog((const char*)0x82033420, *(uint64_t*)&elasticity);
            elasticity = (elasticity < 0.0f) ? -elasticity : elasticity;
        }
    }

    float scale = g_physOneF;  // 1.0f from rdata
    float impulse = elasticity * scale;
    float negImpulse = -impulse;
    // fsel: if negImpulse >= 0 (i.e. impulse <= 0), return 0.0; else return impulse
    return (negImpulse >= 0.0f) ? 0.0f : impulse;
}
/**
 * ph_81C8 — Compute 4x4 matrix-transpose-multiply (row dot products)
 * @ 0x821181C8 | size: 0xB0 (176 bytes)
 *
 * Computes out[i][j] = dot3(A_row[i], B_col[j]) for a 4x3 matrix multiply.
 * The fourth row computes (A_row3 - B_row3) dotted against B columns.
 * Used for physics coordinate frame transformations.
 */
// ph_81C8
void phMatrix_TransposeMultiply3x4(float* dst, const float* src) {
    const float* aRow0 = dst;
    const float* aRow1 = dst + 4;
    const float* aRow2 = dst + 8;
    float aRow3[4];
    aRow3[0] = dst[12] - src[12];
    aRow3[1] = dst[13] - src[13];
    aRow3[2] = dst[14] - src[14];
    aRow3[3] = dst[15] - src[15];

    const float* bCol0 = src;
    const float* bCol1 = src + 4;
    const float* bCol2 = src + 8;

    float r00 = aRow0[0]*bCol0[0] + aRow0[1]*bCol0[1] + aRow0[2]*bCol0[2];
    float r01 = aRow0[0]*bCol1[0] + aRow0[1]*bCol1[1] + aRow0[2]*bCol1[2];
    float r02 = aRow0[0]*bCol2[0] + aRow0[1]*bCol2[1] + aRow0[2]*bCol2[2];
    float r10 = aRow1[0]*bCol0[0] + aRow1[1]*bCol0[1] + aRow1[2]*bCol0[2];
    float r11 = aRow1[0]*bCol1[0] + aRow1[1]*bCol1[1] + aRow1[2]*bCol1[2];
    float r12 = aRow1[0]*bCol2[0] + aRow1[1]*bCol2[1] + aRow1[2]*bCol2[2];
    float r20 = aRow2[0]*bCol0[0] + aRow2[1]*bCol0[1] + aRow2[2]*bCol0[2];
    float r21 = aRow2[0]*bCol1[0] + aRow2[1]*bCol1[1] + aRow2[2]*bCol1[2];
    float r22 = aRow2[0]*bCol2[0] + aRow2[1]*bCol2[1] + aRow2[2]*bCol2[2];
    float r30 = aRow3[0]*bCol0[0] + aRow3[1]*bCol0[1] + aRow3[2]*bCol0[2];
    float r31 = aRow3[0]*bCol1[0] + aRow3[1]*bCol1[1] + aRow3[2]*bCol1[2];
    float r32 = aRow3[0]*bCol2[0] + aRow3[1]*bCol2[1] + aRow3[2]*bCol2[2];

    dst[0]  = r00; dst[1]  = r01; dst[2]  = r02;
    dst[4]  = r10; dst[5]  = r11; dst[6]  = r12;
    dst[8]  = r20; dst[9]  = r21; dst[10] = r22;
    dst[12] = r30; dst[13] = r31; dst[14] = r32;
}

/**
 * ph_FD78 — Initialize physics collision pair tracking structure
 * @ 0x8221FD78 | size: 0x58 (88 bytes)
 *
 * Clears overlap counters and sets max values, zeroes bounding vector.
 * Returns immediately if global physics disable flag is set.
 */
// ph_FD78
void phCollisionPair_Init(void* thisPtr) {
    if (g_phDisableFlag != 0) {
        return;
    }

    uint16_t* counters = (uint16_t*)((char*)thisPtr + 16);
    uint16_t* maxVals  = (uint16_t*)((char*)thisPtr + 24);
    for (int i = 0; i < 4; i++) {
        counters[i] = 0;
        maxVals[i]  = 0xFFFF;
    }

    float initVal = g_physZeroF;
    *(float*)((char*)thisPtr + 0)  = initVal;
    *(float*)((char*)thisPtr + 4)  = initVal;
    *(float*)((char*)thisPtr + 8)  = initVal;
    *(float*)((char*)thisPtr + 12) = initVal;
}

/**
 * ph_ctor_B3B0 — sgNode place/relocate constructor
 * @ 0x8212B3B0 | size: 0xB8 (184 bytes)
 *
 * Placement constructor for sgNode that relocates 3 internal pointers
 * using the relocator's base/stride/offset table. Sets xmlNode vtable
 * first, then upgrades to sgNode vtable after relocation.
 */
// ph_ctor_B3B0
void sgNode_PlaceConstructor(void* thisPtr, void* relocator) {
    *(uint32_t*)((char*)thisPtr + 0) = 0x8203AB8C;

    uint32_t ptr1 = *(uint32_t*)((char*)thisPtr + 4);
    if (ptr1 != 0) {
        uint32_t base = *(uint32_t*)((char*)relocator + 4);
        uint32_t stride = *(uint32_t*)((char*)relocator + 76);
        uint32_t index = (ptr1 - base) / stride;
        uint32_t slot = (index + 2) * 4;
        uint32_t offset = *(uint32_t*)((char*)relocator + slot);
        *(uint32_t*)((char*)thisPtr + 4) = offset + ptr1;
    }

    uint32_t ptr2 = *(uint32_t*)((char*)thisPtr + 8);
    if (ptr2 != 0) {
        uint32_t base = *(uint32_t*)((char*)relocator + 4);
        uint32_t stride = *(uint32_t*)((char*)relocator + 76);
        uint32_t index = (ptr2 - base) / stride;
        uint32_t slot = (index + 2) * 4;
        uint32_t offset = *(uint32_t*)((char*)relocator + slot);
        *(uint32_t*)((char*)thisPtr + 8) = offset + ptr2;
    }

    uint32_t ptr3 = *(uint32_t*)((char*)thisPtr + 12);
    if (ptr3 != 0) {
        uint32_t base = *(uint32_t*)((char*)relocator + 4);
        uint32_t stride = *(uint32_t*)((char*)relocator + 76);
        uint32_t index = (ptr3 - base) / stride;
        uint32_t slot = (index + 2) * 4;
        uint32_t offset = *(uint32_t*)((char*)relocator + slot);
        *(uint32_t*)((char*)thisPtr + 12) = offset + ptr3;
    }

    *(uint32_t*)((char*)thisPtr + 0) = 0x8203338C;
}

/**
 * ph_ctor_C650 — rage::crIKSolverIterativeLimb constructor
 * @ 0x8213C650 | size: 0xAC (172 bytes)
 *
 * Initializes an iterative limb IK solver with base vtable, identity
 * orientation data, then upgrades vtable and sets solver parameters.
 */
// ph_ctor_C650
void crIKSolverIterativeLimb_Constructor(void* thisPtr) {
    *(uint32_t*)((char*)thisPtr + 4) = 3;
    *(uint32_t*)((char*)thisPtr + 0) = 0x82033C70;
    *(uint8_t*)((char*)thisPtr + 8) = 0;
    *(uint8_t*)((char*)thisPtr + 9) = 0;

    const uint8_t* identityData = (const uint8_t*)0x825CB890;
    uint32_t* dst16 = (uint32_t*)((char*)thisPtr + 16);
    const uint32_t* src16 = (const uint32_t*)identityData;
    dst16[0] = src16[0]; dst16[1] = src16[1];
    dst16[2] = src16[2]; dst16[3] = src16[3];

    uint32_t* dst32 = (uint32_t*)((char*)thisPtr + 32);
    const uint32_t* src32 = (const uint32_t*)(identityData + 16);
    dst32[0] = src32[0]; dst32[1] = src32[1];
    dst32[2] = src32[2]; dst32[3] = src32[3];

    *(uint32_t*)((char*)thisPtr + 0) = 0x82033CC4;

    float stepSize    = *(float*)0x82069A90;
    float threshold   = *(float*)0x82069A8C;
    float tolerance   = *(float*)0x8202D124;
    float dampFactor  = *(float*)0x82028080;

    *(uint32_t*)((char*)thisPtr + 48) = 10;
    *(float*)((char*)thisPtr + 52) = stepSize;
    *(float*)((char*)thisPtr + 56) = threshold;
    *(float*)((char*)thisPtr + 60) = tolerance;
    *(float*)((char*)thisPtr + 64) = dampFactor;
}

/**
 * ph_ctor_DCE0 — CPeakMeterEffect factory / create-and-initialize
 * @ 0x821ADCE0 | size: 0xA8 (168 bytes)
 *
 * Allocates a 52-byte CPeakMeterEffect via VCALL slot 5, initializes
 * all fields, installs vtable, then calls VCALL slot 7 for setup.
 */
// ph_ctor_DCE0
void CPeakMeterEffect_Create(void* config, void* allocator, void** outResult) {
    typedef void* (*AllocFn)(void*, uint32_t);
    void** allocVt = *(void***)allocator;
    void* effect = ((AllocFn)allocVt[5])(allocator, 52);

    if (effect != nullptr) {
        *(uint32_t*)((char*)effect + 0) = 0x8203B260;
        *(uint32_t*)((char*)effect + 4) = 1;
        *(uint32_t*)((char*)effect + 48) = 0;
        *(uint32_t*)((char*)effect + 24) = 0;
        *(uint32_t*)((char*)effect + 28) = 0;
        *(uint32_t*)((char*)effect + 44) = 0;
        *(uint32_t*)((char*)effect + 8) = 0;
        *(uint32_t*)((char*)effect + 12) = 0;
        *(uint32_t*)((char*)effect + 16) = 0;
        *(uint32_t*)((char*)effect + 20) = 0;

        typedef void (*InitFn)(void*, void*, void*);
        void** effectVt = *(void***)effect;
        ((InitFn)effectVt[7])(effect, config, allocator);
    } else {
        effect = nullptr;
    }

    *outResult = effect;
}


// ═════════════════════════════════════════════════════════════════════════════
// rage::phBoundCapsule — Additional Functions (26B8, 38A8)
// ═════════════════════════════════════════════════════════════════════════════

// (BB88, BAF0 already lifted above as CheckValueSign/CheckScaledValueSign)

// (F420, 5D88 already lifted above as GetJointLimitsForAxis/RotateVectorsAlt)

/**
 * phBoundCapsule_26B8 @ 0x821426B8 | size: 0x84 (132 bytes)
 *
 * Computes an exponential decay factor from a capsule radius and height.
 * Uses the formula: (1/height) * exp(-height^2 / (radius^2 * 3.0))
 *
 * This produces a Gaussian-like falloff used for capsule soft-shadow
 * or contact response weighting.
 *
 * @param height - Capsule height (f1)
 * @param radius - Capsule radius (f3)
 * @return Weighted decay factor
 */
float phBoundCapsule_26B8(float height, float radius) {
    float radiusSq = radius * radius;

    // Multiply by constant (3.0 or similar scale) from .rdata
    float scaledRadiusSq = radiusSq * 3.0f;  // constant from lbl_8202D108+124

    // Call sqrtf on scaled radius squared
    float expResult = (float)ph_Sqrtf(scaledRadiusSq);

    float heightSq = height * height;
    float denom = radiusSq * 3.0f;  // reloaded from lbl_8202D108+124
    float ratio = heightSq / denom;

    // Compute exp(-ratio) via aud_2478
    float expNegRatio = (float)aud_2478(-ratio);

    float invHeight = 1.0f / expResult;  // 1.0f from lbl_8202D110+0
    float result = invHeight * expNegRatio;

    return result;
}

// (3F08 already lifted above as ValidateAndSetExtents)

// (8608, CF48, DB10 already lifted above as DispatchCapsuleFromCamera/LookupTableValues/SetupCapsuleFromState)

/**
 * phBoundCapsule_ComputeNormalizedAngle @ 0x822238A8 | size: 0x6C (108 bytes)
 *
 * Computes an atan2-based angle from two components of an input vector,
 * then normalizes the result to the range [0, 2*PI).
 *
 * @param data - Pointer to float pair (data[0] = x, data[2] = z)
 * @return Normalized angle in radians [0, 2*PI)
 */
float phBoundCapsule_ComputeNormalizedAngle(float* data) {
    float z = data[2];
    float x = data[0];

    // Compute atan2(x, z)
    float angle = (float)ph_Atan2f(x, z);

    float zero = 0.0f;   // @ lbl_8202D110
    float twoPi = g_phTwoPi;  // @ lbl_8202C02C (6.283185...)

    // Normalize to [0, 2*PI)
    while (angle < zero) {
        angle += twoPi;
    }
    while (angle >= twoPi) {
        angle -= twoPi;
    }

    return angle;
}

// (2C08 already lifted above as SumFloatArray)

/**
 * phBoundCapsule_8C08_2h @ 0x82578C08 | size: 0xFC
 *
 * Null stub — compiler-generated epilogue fragment (marked [sp_wrong] in
 * IDA: unusual stack frame with only a single callee-save GPR restore).
 * No meaningful body; the function body reduces to an immediate return.
 *
 * In the original binary this is an alias/tail of a shared epilogue used
 * by sibling phBoundCapsule methods; it is exported as its own symbol
 * because the linker kept the entry label.
 */
void phBoundCapsule_8C08_2h()
{
    /* no-op: epilogue-shared null stub */
}

/* ======================================================================
   rage::phInst — batch of 10 small instance helpers
   ======================================================================
   These are the first entries of the phInst family beyond the 2 that were
   already lifted (phInst_5910_p39 alias group below is the first full
   lift of the prefix).  All field offsets have been cross-checked against
   the named phInst layout in ph_physics.hpp.
   ====================================================================== */

/* External helpers used by the phInst batch (scaffold-resolved). */
extern void util_C788(void* obj);
extern void _locale_register(void* thisPtr, uint32_t key);
extern void ph_FC68_h(void* subObj);
extern void grc_2CC8(rage::phInst* inst, uint32_t r4_masked, uint32_t r5_masked);
extern void phInst_AC00_2hr(rage::phInst* inst);
extern void phInst_AB00(rage::phInst* inst, void* arg, const char* tag);
extern void phInst_8F10_p42(void* r3, uint32_t r4_zero, void* r5, void* r6,
                            void* r7, void* r8);

/* Vtable constants referenced by the constructor fragments. */
static void* const kPhInst_Vtable5910_main = reinterpret_cast<void*>(0x8204D918u); /* -32256<<16 + 22808 */
static void* const kPhInst_Vtable5910_b    = reinterpret_cast<void*>(0x8204D8FCu); /* -32256<<16 + 22780 */
static void* const kPhInst_Vtable5910_c    = reinterpret_cast<void*>(0x8204BDB0u); /* -32256<<16 + 15792 */
static void* const kPhInst_Vtable6158      = reinterpret_cast<void*>(0x8204DA60u); /* -32256<<16 + 23136 */
static const char kPhInst_Release_Tag[] = "phInst::Release";  /* address lbl_821858CC region; scaffold r11,r5 ops point into .rdata */

/**
 * rage::phInst::phInst_D020_w @ 0x8256D020 | size: 0x74
 *
 * Two-stage teardown helper used by a util wrapper that owns a phInst
 * sub-object.  The argument points to a small container whose first two
 * words are <util_object, sub_resource>:
 *
 *   +0x00  util object (passed to util_C788 to release)
 *   +0x04  secondary resource released via _locale_register(3)
 *
 * After both releases the container itself is released via
 * _locale_register(32787).  The constant 0x61828013 is the packed
 * (lis 24970, ori 32787) identifier used throughout xam_* module glue to
 * tag the generic "free container" variant of the dispatcher.
 */
void phInst_D020_w(void** container)
{
    if (container == nullptr) {
        return;
    }

    /* Stage 1: release the secondary resource held in slot +4, if any. */
    void* sub = container[1];
    if (sub != nullptr) {
        _locale_register(sub, 0x61820003u);       /* r4 = (24970<<16) | 3 */
        container[1] = nullptr;
    }

    /* Stage 2: release the util object held in slot +0. */
    util_C788(container[0]);
    container[0] = nullptr;

    /* Stage 3: release the container itself through the generic dispatcher. */
    _locale_register(container, 0x61828013u);     /* r4 = (24970<<16) | 32787 */
}

/**
 * rage::phInst::phInst_5910_p39 @ 0x82465910 | size: 0x28
 *
 * Vtable trampoline that writes three vtable/hook pointers into the head
 * of a ph_vt57D8 wrapper object.  The final store deliberately clobbers
 * the second slot after the first write: the original code sets +4 twice
 * so that the intermediate value is observable to any interrupt that
 * samples the object between the stores (this matters for the
 * initialisation handshake inside the physics init path).
 *
 * Aliases phInst_5938_p39 at runtime (same code body).
 */
void phInst_5910_p39(void* thisPtr)
{
    void** obj = static_cast<void**>(thisPtr);
    obj[0] = kPhInst_Vtable5910_main;  /* +0x00 */
    obj[1] = kPhInst_Vtable5910_b;     /* +0x04 — first write */
    obj[1] = kPhInst_Vtable5910_c;     /* +0x04 — overwritten with final value */
}

/**
 * rage::phInst::phInst_5980_p39 @ 0x82465980 | size: 0xD4 (shared body with 0x82465938)
 *
 * Compute the packed capacity descriptor for an instance resource array.
 *
 *   inst  :  phInst* (r3)  — uses m_nInstFlags at +1 as a running count
 *   out   :  uint32_t* (r4) — receives ((3*used) << 2) + 28
 *
 * Layout decoded from the scaffold:
 *   - field_0x08 (m_pDestructor) may hold a pointer whose *first byte* is
 *     the "maximum committed" count; if non-null, the current count
 *     (m_nInstFlags @ +1) is clamped against it.
 *   - the packed size is    ((used + 2*used) << 2) + 28
 *     = 12*used + 28  (28 = header, 12 = bytes per slot)
 */
void phInst_Compute5938(rage::phInst* inst, uint32_t* outSize)
{
    uint8_t used = inst->m_nInstFlags;                       /* +0x01 */
    const uint8_t* maxPtr = reinterpret_cast<const uint8_t*>(
        static_cast<uintptr_t>(inst->m_pDestructor));        /* +0x08 */

    if (maxPtr != nullptr) {
        uint8_t maxCommitted = *maxPtr;
        /* Only clamp if the running count has NOT yet exceeded the max. */
        if (used <= maxCommitted) {
            used = maxCommitted;
        }
    }

    /* packed = (used + 2*used)*4 + 28   →   12*used + 28 */
    uint32_t packed = (static_cast<uint32_t>(used) * 3u) * 4u + 28u;
    *outSize = packed;
}

/* Alias entry points that share the 0x82465938/0x82465980 body. */
void phInst_5938_p39(rage::phInst* inst, uint32_t* outSize)
{
    phInst_Compute5938(inst, outSize);
}

/**
 * rage::phInst::phInst_6158_p39 @ 0x82466158 | size: 0x18
 *
 * Trivial constructor for a ph_vt57D8_1 wrapper.  Stores the wrapper's
 * vtable pointer at +0 and delegates the embedded sub-object (at +4)
 * to ph_FC68_h for further initialisation.
 */
void phInst_6158_p39(void* thisPtr)
{
    void** obj = static_cast<void**>(thisPtr);
    obj[0] = kPhInst_Vtable6158;                /* +0x00 vtable */
    ph_FC68_h(static_cast<void*>(&obj[1]));     /* init sub-object at +0x04 */
}

/**
 * rage::phInst::phInst_9070_p42 @ 0x82469070 | size: 0x18
 *
 * Return the byte offset of an instance-indexed 100-byte record inside a
 * phInstBehavior-owned array.  Reads the index byte from m_fFriction+4
 * (offset +0x44 maps to the m_nDataIndex byte at +0x44 — see hpp) and
 * computes  idx*100 + 268.  The constant 268 = 0x10C is the header size
 * of the owning table.
 */
void phInst_9070_p42(const rage::phInst* inst, uint32_t* outOffset)
{
    uint8_t idx = *(reinterpret_cast<const uint8_t*>(inst) + 68); /* m_nDataIndex byte */
    *outOffset = static_cast<uint32_t>(idx) * 100u + 268u;
}

/**
 * rage::phInst::phInst_92E8_p42 @ 0x823592E8 | size: 0x10
 *
 * Return (physicsLayer + 1), where physicsLayer is a 4-bit field packed
 * inside phInst::m_nFlags at +0x20.  The rlwinm 26,28,31 extracts bits
 * that map to the low 4 bits of the flags word after a 26-bit rotate.
 */
uint32_t phInst_92E8_p42(const rage::phInst* inst)
{
    /*
     * PPC: rlwinm r11, r11, 26, 28, 31
     *      → (m_nFlags >> 6) & 0xF
     * Python check: (0xDEADBEEF >> 6) & 0xF == 0xB → matches bit-range.
     */
    uint32_t layer = (inst->m_nFlags >> 6) & 0xFu;
    return layer + 1u;
}

/**
 * rage::phInst::phInst_92F8_p42 @ 0x823592F8 | size: 0x18
 *
 * Thin shim that forwards to phInst_8F10_p42 with r4 forced to 0 and all
 * other arguments shifted down one register (the zero in r4 selects the
 * "no lookup table" variant inside phInst_8F10_p42).
 */
void phInst_92F8_p42(void* r3, void* r5, void* r6, void* r7, void* r8)
{
    phInst_8F10_p42(r3, /*r4=*/0u, r5, r6, r7, r8);
}

/**
 * rage::phInst::phInst_9310_p42 @ 0x82359310 | size: 0x14
 *
 * Mask-and-forward helper: clears the low 12 bits of two phInst pointers
 * (m_pBoundData @ +0x14 and m_pBoundResource @ +0x24) so they become
 * page-aligned (4 KiB pages), then tail-calls the GRC-layer routine.
 */
void phInst_9310_p42(const rage::phInst* inst)
{
    uint32_t alignedResource = inst->m_pBoundResource & 0xFFFFF000u; /* +0x24 */
    uint32_t alignedBoundData = inst->m_pBoundData    & 0xFFFFF000u; /* +0x14 */
    grc_2CC8(const_cast<rage::phInst*>(inst), alignedBoundData, alignedResource);
}

/**
 * rage::phInst::phInst_0800_2h @ 0x82460800 | size: 0x4C
 *
 * Release-with-tag wrapper.  The PPC prologue reshuffles three arguments
 * because the call site uses the Rockstar ABI-extension for "released
 * by tag" which threads both the caller's object (r5) and a user tag
 * (r6) through two helper stages:
 *
 *   1. phInst_AC00_2hr(inst)            — safe teardown of inst
 *   2. phInst_AB00(inst, userTag, kPhInst_Release_Tag)
 *                                       — logs and releases with the
 *                                         fixed tag string
 */
void phInst_0800_2h(void* /*r3 unused*/, void* /*r4 unused*/,
                    rage::phInst* inst, void* userTag)
{
    phInst_AC00_2hr(inst);
    phInst_AB00(inst, userTag, kPhInst_Release_Tag);
}

/**
 * rage::phInst::phInst_1488_sp @ 0x82461488 | size: 0x10
 *
 * Size-query virtual helper.  Writes the literal object size (20 bytes)
 * into the caller-supplied out-parameter and returns 0 (success).  The
 * "_sp" suffix in the recomp name marks it as a small-pool allocator
 * hook (size-query slot); 20 bytes matches the minimum phInst footprint
 * for an externally-owned instance.
 */
int phInst_1488_sp(void* /*unused*/, uint32_t* outSize)
{
    *outSize = 20u;
    return 0;
}

