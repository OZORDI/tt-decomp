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
    
    // +0x04-0x0C: Tuning parameters (set dynamically via external calls)
    // These are set by SetParam1/2/3 which call external vtable slot 5
    float m_tuningValue1;                           // +0x04 - set by SetParam1
    float m_tuningValue2;                           // +0x08 - set by SetParam2
    float m_tuningValue3;                           // +0x0C - set by SetParam3
    
    // +0x10-0x1C: Default vector (initialized to 0.0 in constructor)
    // 4 consecutive floats forming a vec4 or quaternion identity
    float m_defaultX;                               // +0x10 - initialized to 0.0
    float m_defaultY;                               // +0x14 - initialized to 0.0
    float m_defaultZ;                               // +0x18 - initialized to 0.0
    float m_defaultW;                               // +0x1C - initialized to 0.0
    
    // +0x20-0x8F: Unknown region (likely more parameters or embedded structures)
    uint8_t _pad0x20[0x70];                         // +0x20 to +0x8F (112 bytes)
    
    // +0x90-0x98: Additional tuning parameters (set dynamically)
    float m_tuningValue8;                           // +0x90 (144) - set by SetParam8
    float m_tuningValue9;                           // +0x94 (148) - set by SetParam9
    float m_tuningValue10;                          // +0x98 (152) - set by SetParam10
    
    // +0x99: Initialization flags
    uint8_t m_flags;                                // +0x99 (153) - bit 0 = initialized
    
    // +0x9A-0xDF: Unknown region
    uint8_t _pad0x9A[0x46];                         // +0x9A to +0xDF (70 bytes)
    
    // +0xE0-0x12F: Vector data (AltiVec/VMX optimized, 16-byte aligned)
    // Likely 4x4 matrices or vec4 arrays for transforms/physics
    float m_vectorData[20];                         // +0xE0 (224) - 5 vec4s = 80 bytes
    
    // +0x130-0x14F: Float array (additional parameters)
    float m_floatArray[8];                          // +0x130 (304) - 32 bytes
    
    // +0x150-0x1AF: Unknown region
    uint8_t _pad0x150[0x60];                        // +0x150 to +0x1AF (96 bytes)
    
    // +0x1B0-0x20F: Vector region (cleared in constructor loop)
    float m_vectorRegion[24];                       // +0x1B0 (432) - 6 vec4s = 96 bytes
    
    // +0x210-0x28F: Vector loop region (cleared in constructor loop)
    float m_vectorLoop[32];                         // +0x210 (528) - 8 vec4s = 128 bytes
    
    // +0x290-0x2BF: Unknown region
    uint8_t _pad0x290[0x30];                        // +0x290 to +0x2BF (48 bytes)
    
    // +0x2C0-0x2C1: State flags
    uint8_t m_stateFlag1;                           // +0x2C0 (704) - cleared in constructor
    uint8_t m_stateFlag2;                           // +0x2C1 (705) - cleared in constructor
    
    // Padding to 16-byte alignment
    uint8_t _pad0x2C2[0x0E];                        // +0x2C2 to +0x2CF (14 bytes)
    
} __attribute__((aligned(16)));  // 16-byte alignment for AltiVec operations

// Compile-time size verification
static_assert(sizeof(Holder) == 720, "Holder struct size mismatch - should be 720 bytes (16-byte aligned)");
static_assert(offsetof(Holder, m_tuningValue1) == 0x04, "Holder::m_tuningValue1 offset mismatch");
static_assert(offsetof(Holder, m_tuningValue8) == 0x90, "Holder::m_tuningValue8 offset mismatch");
static_assert(offsetof(Holder, m_flags) == 0x99, "Holder::m_flags offset mismatch");
static_assert(offsetof(Holder, m_vectorData) == 0xE0, "Holder::m_vectorData offset mismatch");
static_assert(offsetof(Holder, m_stateFlag1) == 0x2C0, "Holder::m_stateFlag1 offset mismatch");

// Global singleton array (6 specializations, 28 bytes each)
// Each Holder_vfn_1 variant returns a different entry from this array
extern uint8_t g_HolderSingletons[168];  // @ 0x825D4560 - 6 * 28 bytes
