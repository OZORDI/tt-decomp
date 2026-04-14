/**
 * fx_effects.cpp — Visual effects system (ball trails, particles, etc.)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "fx_effects.hpp"
#include "../rage/memory.hpp"

// Global effect parameter table @ 0x8271A394
extern uint32_t* g_fxParameterTable;

// Base class cleanup for fxCrowdGfx hierarchy
extern void rage_5908(void*);

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
    // Look up parameter value from global table
    uint32_t paramValue = g_fxParameterTable[effectIndex + 1];

    // Store effect parameters
    this->m_effectIntensity = intensity;
    this->m_effectParam = paramValue;
    this->m_effectDuration = duration;
}


/**
 * fxCrowdGfx::vfn_2 @ 0x823856E0 | size: 0x50
 * [vtable slot 2]
 *
 * Cleanup and conditional free for crowd graphics effect.
 * Calls base class cleanup (rage_5908) and optionally frees the object
 * memory if the delete-self flag (bit 0 of flags parameter) is set.
 *
 * @param flags Flags (bit 0 = free self after cleanup)
 */
void fxCrowdGfx::vfn_2(int flags) {
    rage_5908(this);

    if (flags & 0x1) {
        rage_free(this);
    }
}
