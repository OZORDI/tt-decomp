/**
 * fx_effects.cpp — Visual effects system (ball trails, particles, etc.)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "fx_effects.hpp"

// Global effect parameter table
extern uint32_t* g_fxParameterTable;  // @ 0x8271A394

/**
 * fxBallTrail::SetEffectParameters @ 0x82381208 | size: 0x2C
 * [vtable slot 3]
 *
 * Sets effect parameters for the ball trail visual effect.
 * Stores effect type, intensity, and looks up additional parameters
 * from a global table based on the effect index.
 *
 * @param effectIndex Index into effect parameter table (r4)
 * @param intensity Effect intensity value (r5)
 * @param duration Effect duration (r6)
 */
void fxBallTrail::SetEffectParameters(int effectIndex, uint32_t intensity, uint32_t duration) {
    // Calculate table lookup index: (effectIndex + 1) * 4
    int tableIndex = (effectIndex + 1) * 4;
    
    // Load global parameter table pointer @ 0x8271A394
    uint32_t* paramTable = *(uint32_t**)0x8271A394;
    
    // Look up parameter value from table
    uint32_t paramValue = paramTable[tableIndex / 4];
    
    // Store intensity at offset 46192 (0xB470)
    *(uint32_t*)((char*)this + 46192) = intensity;
    
    // Store parameter value at offset 15400 (0x3C28)
    *(uint32_t*)((char*)this + 15400) = paramValue;
    
    // Store duration at offset 15404 (0x3C2C)
    *(uint32_t*)((char*)this + 15404) = duration;
}


/**
 * fxCrowdGfx::~fxCrowdGfx @ 0x823856E0 | size: 0x50
 * [vtable slot 2 - destructor]
 *
 * Destructor for crowd graphics effect.
 * Calls base class cleanup (rage_5908) and optionally frees the object
 * memory if the delete-self flag (bit 0 of flags parameter) is set.
 *
 * @param flags Destructor flags (bit 0 = delete self)
 */
fxCrowdGfx::~fxCrowdGfx(int flags) {
    // Call base class cleanup
    extern void rage_5908(void*);
    rage_5908(this);
    
    // Check if we should free the object memory (bit 0 of flags)
    if (flags & 0x1) {
        // Free the object using RAGE memory allocator
        extern void rage_free_00C0(void*);
        rage_free_00C0(this);
    }
}
