/**
 * holder.h — Holder template class for game parameter/fragment management
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Holder is a template class with 6 specializations for managing game parameters,
 * tuning data, and fragment resources. Each specialization has its own vtable and
 * global singleton instance.
 * 
 * The class stores:
 * - Float parameters (tuning values, likely from fragment data files)
 * - Vector/matrix data (transforms, physics parameters)
 * - State flags for initialization and configuration
 * 
 * Vtable addresses (6 specializations):
 * - 0x82032E34, 0x82032E48, 0x82032E5C, 0x82032E70, 0x82032E84, 0x82033290
 * 
 * Global singleton array: 0x825D4560 (6 entries, 28 bytes each = 168 bytes total)
 * 
 * Size: 706 bytes (0x2C2), aligned to 16 bytes = 720 bytes
 */

#pragma once

#include <stdint.h>

// Forward declarations
namespace rage {
    class sysMemAllocator;
}

/**
 * Holder — Template class for game parameter/fragment management
 * 
 * This class manages tuning parameters loaded from fragment type files
 * (referenced in constructor pseudocode: "$/tune/types/fragments").
 * 
 * The 6 vtable addresses represent 6 template specializations, each managing
 * different types of game parameters or fragments.
 */
class Holder {
public:
    // Virtual methods
    virtual ~Holder() = default;                    // [0] destructor
    virtual void* GetSingleton();                   // [1] @ 0x82121BF8 - returns singleton for this specialization
    virtual void* AllocateAndInitialize();          // [2] @ 0x82121C08 - allocates and copies vector data
    
    // Constructor
    Holder();                                       // @ 0x82122798 - initializes all fields to zero
    
    // Parameter setters (call external vtable slot 5 and store result)
    void SetParam1(void* source);                   // @ 0x82123488 - sets +0x04
    void SetParam2(void* source);                   // @ 0x821234C8 - sets +0x08
    void SetParam3(void* source);                   // @ 0x82123508 - sets +0x0C
    void SetParam8(void* source);                   // @ 0x82123548 - sets +0x90
    void SetParam9(void* source);                   // @ 0x82123588 - sets +0x94
    void SetParam10(void* source);                  // @ 0x821235C8 - sets +0x98
    
    // Flag setter
    void SetInitializedFlag();                      // @ 0x82123478 - sets bit 0 at +0x99
    
    // Indirect vtable call (slot 55)
    void CallVtableSlot55();                        // @ 0x82122BF0 - calls vtable +220
    
protected:
    // +0x00: vtable pointer (implicit in C++)
    
    // +0x04-0x0C: Dynamic tuning parameters (set via SetParam1/2/3)
    // These call external vtable slot 5 to retrieve values from fragment data
    float m_tuningParam1;                           // +0x04 - set by SetParam1
    float m_tuningParam2;                           // +0x08 - set by SetParam2
    float m_tuningParam3;                           // +0x0C - set by SetParam3
    
    // +0x10-0x1C: Default identity vector (initialized to zero in constructor)
    // Used as default position, zero vector, or identity quaternion component
    float m_defaultVectorX;                         // +0x10 - initialized to 0.0
    float m_defaultVectorY;                         // +0x14 - initialized to 0.0
    float m_defaultVectorZ;                         // +0x18 - initialized to 0.0
    float m_defaultVectorW;                         // +0x1C - initialized to 0.0
    
    // +0x20-0x8F: Reserved region (uninitialized - may be parent class data or reserved)
    uint8_t _pad0x20[0x70];                         // +0x20 to +0x8F (112 bytes)
    
    // +0x90-0x98: Extended tuning parameters (set dynamically)
    float m_tuningParam8;                           // +0x90 (144) - set by SetParam8
    float m_tuningParam9;                           // +0x94 (148) - set by SetParam9
    float m_tuningParam10;                          // +0x98 (152) - set by SetParam10
    
    // +0x99: Initialization status flags
    uint8_t m_initFlags;                            // +0x99 (153) - bit 0 = initialized
    
    // +0x9A-0xDF: Reserved region (uninitialized)
    uint8_t _pad0x9A[0x46];                         // +0x9A to +0xDF (70 bytes)
    
    // +0xE0-0x11F: Constant transform data (loaded from global data @ 0x82032F48)
    // 4 x vec4 (64 bytes) - likely identity matrix or default transform constants
    float m_constantVectors[16];                    // +0xE0 (224) - 4 vec4s = 64 bytes
    
    // +0x120-0x12F: Padding between constant and tuning data
    uint8_t _pad0x120[0x10];                        // +0x120 to +0x12F (16 bytes)
    
    // +0x130-0x14F: Tuning parameter array (initialized to 0.0 in constructor)
    // Used for blend weights, animation params, or physics tuning
    float m_tuningParams[8];                        // +0x130 (304) - 8 floats = 32 bytes
    
    // +0x150-0x1AF: Reserved region (uninitialized)
    uint8_t _pad0x150[0x60];                        // +0x150 to +0x1AF (96 bytes)
    
    // +0x1B0-0x20F: Transform cache (cleared in constructor loop)
    // 6 x vec4 (96 bytes) - likely cached transforms, bone matrices, or physics state
    float m_transformCache[24];                     // +0x1B0 (432) - 6 vec4s = 96 bytes
    
    // +0x210-0x28F: Animation/state data (cleared in constructor loop)
    // 8 x vec4 (128 bytes) - likely animation blend data or state vectors
    float m_animationState[32];                     // +0x210 (528) - 8 vec4s = 128 bytes
    
    // +0x290-0x2B3: Extended tuning parameters (initialized to 0.0 in constructor)
    // 7 floats with gaps suggesting interleaved int/enum fields
    float m_extendedParam0;                         // +0x290 (656) - initialized to 0.0
    float m_extendedParam1;                         // +0x294 (660) - initialized to 0.0
    float m_extendedParam2;                         // +0x298 (664) - initialized to 0.0
    uint32_t _pad0x29C;                             // +0x29C (668) - gap (likely int/enum)
    float m_extendedParam3;                         // +0x2A0 (672) - initialized to 0.0
    float m_extendedParam4;                         // +0x2A4 (676) - initialized to 0.0
    float m_extendedParam5;                         // +0x2A8 (680) - initialized to 0.0
    uint32_t _pad0x2AC;                             // +0x2AC (684) - gap (likely int/enum)
    float m_extendedParam6;                         // +0x2B0 (688) - initialized to 0.0
    uint8_t _pad0x2B4[0x0C];                        // +0x2B4 to +0x2BF (12 bytes)
    
    // +0x2C0-0x2C1: State tracking flags (cleared in constructor)
    uint8_t m_stateFlags;                           // +0x2C0 (704) - general state flags
    uint8_t m_statusFlags;                          // +0x2C1 (705) - status flags
    
    // Padding to 16-byte alignment
    uint8_t _pad0x2C2[0x0E];                        // +0x2C2 to +0x2CF (14 bytes)
    
} __attribute__((aligned(16)));  // 16-byte alignment for AltiVec operations

// Compile-time size and offset verification
static_assert(sizeof(Holder) == 720, "Holder struct size mismatch - should be 720 bytes (16-byte aligned)");
static_assert(offsetof(Holder, m_tuningParam1) == 0x04, "Holder::m_tuningParam1 offset mismatch");
static_assert(offsetof(Holder, m_defaultVectorX) == 0x10, "Holder::m_defaultVectorX offset mismatch");
static_assert(offsetof(Holder, m_tuningParam8) == 0x90, "Holder::m_tuningParam8 offset mismatch");
static_assert(offsetof(Holder, m_initFlags) == 0x99, "Holder::m_initFlags offset mismatch");
static_assert(offsetof(Holder, m_constantVectors) == 0xE0, "Holder::m_constantVectors offset mismatch");
static_assert(offsetof(Holder, m_tuningParams) == 0x130, "Holder::m_tuningParams offset mismatch");
static_assert(offsetof(Holder, m_transformCache) == 0x1B0, "Holder::m_transformCache offset mismatch");
static_assert(offsetof(Holder, m_animationState) == 0x210, "Holder::m_animationState offset mismatch");
static_assert(offsetof(Holder, m_extendedParam0) == 0x290, "Holder::m_extendedParam0 offset mismatch");
static_assert(offsetof(Holder, m_stateFlags) == 0x2C0, "Holder::m_stateFlags offset mismatch");

// Global singleton array (6 specializations, 28 bytes each)
// Each Holder_vfn_1 variant returns a different entry from this array
extern uint8_t g_HolderSingletons[168];  // @ 0x825D4560 - 6 * 28 bytes
