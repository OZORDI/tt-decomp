/**
 * cm_operators.cpp — Conditional Machine operators and constructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The "cm" subsystem implements a data-flow graph for game logic conditions,
 * reporters, and operators. These classes are template-instantiated constructors
 * that build operator nodes at runtime.
 */

#include "cm_operators.hpp"
#include <stdint.h>
#include <stddef.h>

// Forward declarations
extern "C" void xe_main_thread_init_0038(void);

// External vtable addresses
extern "C" const void* rage_cmIntegrate_vtable;  // @ 0x82054934

// SDA base - points to allocator context
extern uint32_t* g_sda_base;  // @ 0x82600000 (r13)


/**
 * cmWorldRefreshableCtor::Destructor @ 0x82268438 | size: 0xa0
 *
 * Constructor variant that allocates and initializes a rage::cmIntegrate object.
 * This is one of 18 template instantiations of cmWorldRefreshableCtor for different
 * operator types.
 *
 * The function:
 * 1. Ensures thread context is initialized
 * 2. Allocates 40 bytes with 16-byte alignment from the RAGE allocator
 * 3. Zero-initializes the object
 * 4. Sets the vtable to rage::cmIntegrate
 * 5. Clears the "initialized" flag bit at offset +8
 *
 * @return Pointer to the newly constructed rage::cmIntegrate object, or nullptr on failure
 */
void* cmWorldRefreshableCtor_Destructor()
{
    // Initialize main thread context if not already done
    xe_main_thread_init_0038();
    
    // Get allocator context from SDA base
    // SDA offset 0 points to allocator context struct
    uint32_t* allocatorCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    
    // Get allocator object from context offset +4
    void** allocator = (void**)(uintptr_t)allocatorCtx[1];
    
    // Call allocator->Allocate(40, 16) via vtable slot 1
    typedef void* (*AllocateFn)(void* self, uint32_t size, uint32_t alignment);
    AllocateFn allocate = (AllocateFn)allocator[1];
    void* obj = allocate(allocator, 40, 16);
    
    if (obj != nullptr) {
        // Zero-initialize the object fields
        uint32_t* fields = (uint32_t*)obj;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28 - set to 2 (likely a type/state enum)
        fields[8] = 0;  // +32
        fields[9] = 0;  // +36
        
        // Set vtable pointer to rage::cmIntegrate
        fields[0] = (uint32_t)(uintptr_t)&rage_cmIntegrate_vtable;
        
        // Clear bit 0 of the field at offset +8 (clear "initialized" flag)
        fields[2] = fields[2] & ~1u;
    }
    
    return obj;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmLookup — Lookup table operator (2-entry and 3-entry variants)
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations for utility functions
extern "C" float util_9350(void* entry);  // Evaluates an entry and returns float
extern "C" int cmSwitch_4B60(void* switchObj);  // Evaluates switch and returns int
extern "C" int util_4BD8(void* obj);  // Evaluates object and returns int
extern "C" bool cmCond_21B0(void* condObj);  // Evaluates condition and returns bool
extern "C" void util_92D8(void* dest, void* src);  // Copies vector data
extern "C" void cmCond_1038_g(void* condObj, void* dest);  // Evaluates condition into dest

/**
 * cmLookup::RegisterPorts @ 0x8226D840 | size: 0x40
 * 
 * Retrieves cached result based on mode.
 * Mode 1: Returns byte from offset +16 of object at +28
 * Mode 2: Returns int from offset +4 of object at +28
 * Other: Clears cached result to 0
 */
void cmLookup::RegisterPorts() {
    int mode = *(int*)((char*)this + 32);
    
    if (mode == 1) {
        void* obj = *(void**)((char*)this + 28);
        uint8_t value = *(uint8_t*)((char*)obj + 16);
        *(uint32_t*)((char*)this + 4) = value;
    } else if (mode == 2) {
        void* obj = *(void**)((char*)this + 28);
        uint32_t value = *(uint32_t*)((char*)obj + 4);
        *(uint32_t*)((char*)this + 4) = value;
    } else {
        *(uint32_t*)((char*)this + 4) = 0;
    }
}

/**
 * cmLookup::GetDim @ 0x8226D880 | size: 0x9c
 * 
 * Evaluates lookup table and returns matching int result (2-entry variant).
 * Compares key value against two threshold entries and returns the first match,
 * or default value if no match.
 */
void cmLookup::GetDim(int* outResult) {
    // Evaluate the key value
    float keyValue = util_9350((char*)this + 12);
    
    // Check each entry (2 entries at offsets +20 and +36)
    for (int entryIndex = 1; entryIndex <= 2; entryIndex++) {
        void* entry = (char*)this + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        // Check if key <= threshold (with NaN handling)
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            // Found match - evaluate and return this entry's result
            void* resultObj = (char*)this + 12 + entryIndex * 16;
            *outResult = cmSwitch_4B60(resultObj);
            return;
        }
    }
    
    // No match - return default value at offset +52
    *outResult = cmSwitch_4B60((char*)this + 52);
}

/**
 * cmLookup::GetFloat @ 0x8226D920 | size: 0x9c
 * 
 * Evaluates lookup table and returns matching float result (2-entry variant).
 */
void cmLookup::GetFloat(float* outResult) {
    float keyValue = util_9350((char*)this + 12);
    
    for (int entryIndex = 1; entryIndex <= 2; entryIndex++) {
        void* entry = (char*)this + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)this + 12 + entryIndex * 16;
            *outResult = util_9350(resultObj);
            return;
        }
    }
    
    *outResult = util_9350((char*)this + 52);
}

/**
 * cmLookup::GetBool @ 0x8226DA78 | size: 0x9c
 * 
 * Evaluates lookup table and returns matching bool result (2-entry variant).
 */
void cmLookup::GetBool(bool* outResult) {
    float keyValue = util_9350((char*)this + 12);
    
    for (int entryIndex = 1; entryIndex <= 2; entryIndex++) {
        void* entry = (char*)this + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)this + 12 + entryIndex * 16;
            *outResult = cmCond_21B0(resultObj);
            return;
        }
    }
    
    *outResult = cmCond_21B0((char*)this + 52);
}

/**
 * cmLookup::GetVector @ 0x8226D9C0 | size: 0xb4
 * 
 * Evaluates lookup table and returns matching vector result (2-entry variant).
 * Uses VMX128 vector operations for 16-byte data.
 */
void cmLookup::GetVector(void* outVector) {
    float keyValue = util_9350((char*)this + 12);
    
    for (int entryIndex = 1; entryIndex <= 2; entryIndex++) {
        void* entry = (char*)this + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)this + 12 + entryIndex * 16;
            util_92D8(outVector, resultObj);
            return;
        }
    }
    
    util_92D8(outVector, (char*)this + 52);
}

/**
 * cmLookup::GetDimValue @ 0x8226DB18 | size: 0x9c
 * 
 * Scalar destructor - evaluates lookup table and returns matching int result (2-entry variant).
 * Similar to GetDim but uses util_4BD8 instead of cmSwitch_4B60.
 */
void cmLookup::GetDimValue(int* outResult) {
    float keyValue = util_9350((char*)this + 12);
    
    for (int entryIndex = 1; entryIndex <= 2; entryIndex++) {
        void* entry = (char*)this + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)this + 12 + entryIndex * 16;
            *outResult = util_4BD8(resultObj);
            return;
        }
    }
    
    *outResult = util_4BD8((char*)this + 52);
}

/**
 * cmLookup::CopyState @ 0x82270D10 | size: 0x3c
 * 
 * Evaluates all 8 condition entries sequentially.
 * Used for batch evaluation of multiple conditions.
 */
void cmLookup::CopyState(void* dest) {
    void* entry = (char*)this + 12;
    
    for (int i = 0; i < 8; i++) {
        cmCond_1038_g(entry, dest);
        entry = (char*)entry + 8;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// cmLookup 3-entry variants (check 3 thresholds instead of 2)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmLookup_GetDim_DBB8_1 @ 0x8226DBB8 | size: 0x9c
 * 
 * 3-entry variant: Returns matching int result from 3-entry lookup table.
 */
void cmLookup_GetDim_DBB8_1(void* self, int* outResult) {
    float keyValue = util_9350((char*)self + 12);
    
    for (int entryIndex = 1; entryIndex <= 3; entryIndex++) {
        void* entry = (char*)self + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)self + 12 + entryIndex * 16;
            *outResult = cmSwitch_4B60(resultObj);
            return;
        }
    }
    
    // Default at offset +68 for 3-entry table
    *outResult = cmSwitch_4B60((char*)self + 68);
}

/**
 * cmLookup_GetFloat_DC58_1 @ 0x8226DC58 | size: 0x9c
 * 
 * 3-entry variant: Returns matching float result from 3-entry lookup table.
 */
void cmLookup_GetFloat_DC58_1(void* self, float* outResult) {
    float keyValue = util_9350((char*)self + 12);
    
    for (int entryIndex = 1; entryIndex <= 3; entryIndex++) {
        void* entry = (char*)self + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)self + 12 + entryIndex * 16;
            *outResult = util_9350(resultObj);
            return;
        }
    }
    
    *outResult = util_9350((char*)self + 68);
}

/**
 * cmLookup_GetBool_DDB0_1 @ 0x8226DDB0 | size: 0x9c
 * 
 * 3-entry variant: Returns matching bool result from 3-entry lookup table.
 */
void cmLookup_GetBool_DDB0_1(void* self, bool* outResult) {
    float keyValue = util_9350((char*)self + 12);
    
    for (int entryIndex = 1; entryIndex <= 3; entryIndex++) {
        void* entry = (char*)self + 20 + (entryIndex - 1) * 16;
        float threshold = util_9350(entry);
        
        bool matches = (keyValue <= threshold);
        
        if (matches) {
            void* resultObj = (char*)self + 12 + entryIndex * 16;
            *outResult = cmCond_21B0(resultObj);
            return;
        }
    }
    
    *outResult = cmCond_21B0((char*)self + 68);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor factory functions
//
// These are virtual method overrides on the 113 cmOperatorCtor vtable
// instantiations. Each allocates memory via the TLS allocator (slot 1)
// and tail-calls the class-specific init/constructor function.
// Pattern: xe_main_thread_init_0038() → get allocator → Allocate(size, 16)
//          → if non-null, call init → return constructed object or nullptr
// ─────────────────────────────────────────────────────────────────────────────

// Init functions called by each factory
extern "C" void* atSingleton_9560_2hr(void* mem);   // → pongInlineCinematics ctor
extern "C" void* phJoint3Dof_8BE8_fw(void* mem);    // → plrPlayerMgr ctor
extern "C" void* ph_ctor_01D0(void* mem);            // → gdCrData ctor
extern "C" void* atSingleton_2560_2hr(void* mem);    // → gdCharPlayerBias ctor
extern "C" void* phJoint3Dof_AA28_w(void* mem);      // → gdLevelData ctor
extern "C" void* ph_ctor_FEC8(void* mem);            // → gdNetData ctor
extern "C" void* atSingleton_E050_2hr(void* mem);    // → gdPlyrLimits ctor
extern "C" void* ph_ctor_FB70(void* mem);            // → lvlLevelMgr ctor
extern "C" void* ph_09B8(void* mem);                 // → gmBallNode ctor
extern "C" void* atSingleton_B358_p46(void* mem);    // → pongCharViewContext ctor

typedef void* (*AllocFn)(void* self, uint32_t size, uint32_t alignment);

/**
 * Helper: allocate from TLS heap with 16-byte alignment.
 * Used by all cmOperatorCtor factory functions below.
 */
static void* cmOperatorCtor_Allocate(uint32_t size) {
    xe_main_thread_init_0038();
    uint32_t* sdaCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    void** allocator = (void**)(uintptr_t)sdaCtx[1];
    AllocFn alloc = (AllocFn)allocator[1];
    return alloc(allocator, size, 16);
}

// ── pongInlineCinematics factory ─────────────────────────────────────────────

/**
 * cmOperatorCtor_94C0_w @ 0x821694C0 | size: 0x64
 *
 * Allocates 12512 bytes and constructs a pongInlineCinematics object.
 */
// cmOperatorCtor_94C0_w @ 0x821694C0
void* cmOperatorCtor_pongInlineCinematics() {
    void* mem = cmOperatorCtor_Allocate(12512);
    if (mem != nullptr) {
        return atSingleton_9560_2hr(mem);
    }
    return nullptr;
}

// ── plrPlayerMgr factory ─────────────────────────────────────────────────────

/**
 * cmOperatorCtor_8B80_w @ 0x82188B80 | size: 0x64
 *
 * Allocates 300 bytes and constructs a plrPlayerMgr object.
 */
// cmOperatorCtor_8B80_w @ 0x82188B80
void* cmOperatorCtor_plrPlayerMgr() {
    void* mem = cmOperatorCtor_Allocate(300);
    if (mem != nullptr) {
        return phJoint3Dof_8BE8_fw(mem);
    }
    return nullptr;
}

// ── gdCrData factory ─────────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0AF0_w @ 0x821D0AF0 | size: 0x64
 *
 * Allocates 368 bytes and constructs a gdCrData object.
 */
// cmOperatorCtor_0AF0_w @ 0x821D0AF0
void* cmOperatorCtor_gdCrData() {
    void* mem = cmOperatorCtor_Allocate(368);
    if (mem != nullptr) {
        return ph_ctor_01D0(mem);
    }
    return nullptr;
}

// ── gdCharPlayerBias factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_2878_w @ 0x821D2878 | size: 0x64
 *
 * Allocates 160 bytes and constructs a gdCharPlayerBias object.
 */
// cmOperatorCtor_2878_w @ 0x821D2878
void* cmOperatorCtor_gdCharPlayerBias() {
    void* mem = cmOperatorCtor_Allocate(160);
    if (mem != nullptr) {
        return atSingleton_2560_2hr(mem);
    }
    return nullptr;
}

// ── gdLevelData factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_AE88_w @ 0x821DAE88 | size: 0x64
 *
 * Allocates 5936 bytes and constructs a gdLevelData object.
 */
// cmOperatorCtor_AE88_w @ 0x821DAE88
void* cmOperatorCtor_gdLevelData() {
    void* mem = cmOperatorCtor_Allocate(5936);
    if (mem != nullptr) {
        return phJoint3Dof_AA28_w(mem);
    }
    return nullptr;
}

// ── gdNetData factory ────────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0030_w @ 0x821F0030 | size: 0x64
 *
 * Allocates 144 bytes and constructs a gdNetData object.
 */
// cmOperatorCtor_0030_w @ 0x821F0030
void* cmOperatorCtor_gdNetData() {
    void* mem = cmOperatorCtor_Allocate(144);
    if (mem != nullptr) {
        return ph_ctor_FEC8(mem);
    }
    return nullptr;
}

// ── gdPlyrLimits factory ─────────────────────────────────────────────────────

/**
 * cmOperatorCtor_E338_w @ 0x8220E338 | size: 0x64
 *
 * Allocates 148 bytes and constructs a gdPlyrLimits object.
 */
// cmOperatorCtor_E338_w @ 0x8220E338
void* cmOperatorCtor_gdPlyrLimits() {
    void* mem = cmOperatorCtor_Allocate(148);
    if (mem != nullptr) {
        return atSingleton_E050_2hr(mem);
    }
    return nullptr;
}

// ── lvlLevelMgr factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_FDC0_w @ 0x8223FDC0 | size: 0x64
 *
 * Allocates 100 bytes and constructs a lvlLevelMgr object.
 */
// cmOperatorCtor_FDC0_w @ 0x8223FDC0
void* cmOperatorCtor_lvlLevelMgr() {
    void* mem = cmOperatorCtor_Allocate(100);
    if (mem != nullptr) {
        return ph_ctor_FB70(mem);
    }
    return nullptr;
}

// ── gmBallNode factory ───────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0950_w @ 0x82280950 | size: 0x64
 *
 * Allocates 336 bytes and constructs a gmBallNode object.
 */
// cmOperatorCtor_0950_w @ 0x82280950
void* cmOperatorCtor_gmBallNode() {
    void* mem = cmOperatorCtor_Allocate(336);
    if (mem != nullptr) {
        return ph_09B8(mem);
    }
    return nullptr;
}

// ── pongCharViewContext factory ──────────────────────────────────────────────

/**
 * cmOperatorCtor_A598_w @ 0x8230A598 | size: 0x64
 *
 * Allocates 388 bytes and constructs a pongCharViewContext object.
 */
// cmOperatorCtor_A598_w @ 0x8230A598
void* cmOperatorCtor_pongCharViewContext() {
    void* mem = cmOperatorCtor_Allocate(388);
    if (mem != nullptr) {
        return atSingleton_B358_p46(mem);
    }
    return nullptr;
}
