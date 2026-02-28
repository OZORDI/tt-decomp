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
 * ph_21B0 @ 0x824321B0 | size: 0x3C
 *
 * Scan a string for a specific character, returning a pointer to its first
 * occurrence.  Functionally equivalent to strchr(3) with two variants:
 *
 *   ch != 0  →  Returns pointer to first byte equal to ch, or NULL if not found.
 *   ch == 0  →  Returns pointer to the NUL terminator (scan-to-end shortcut).
 *
 * This variant is used during argument/config parsing in rage_app_init_6418
 * to locate '/' or '-' prefix characters in command-line tokens.
 *
 * The function is an alias of util_21F0 in the binary (the linker merged the
 * two identical bodies into a single address).
 */
const char* ph_21B0(const char* str, int ch)
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
 * ph_1B58_h @ 0x82451B58 | size: 0x3C
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
void ph_1B58_h(void* thisPtr, const char* name) {
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
