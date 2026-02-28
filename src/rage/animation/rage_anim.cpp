/**
 * rage_anim.cpp — Animation system implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE animation DOF (Degree of Freedom) classes for blending and interpolation.
 */

#include "rage/animation/rage_anim.hpp"
#include <algorithm>

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// crAnimDofFloat::Blend()  [vtable slot 7 @ 0x8216EC30]
// 
// Blends the current float value towards a target value using linear interpolation.
// The blend factor is clamped to [0.0, 1.0] range.
// ─────────────────────────────────────────────────────────────────────────────
void crAnimDofFloat::Blend(void* targetObj, float blendFactor) {
    // Clamp blend factor to [0.0, 1.0]
    float clampedBlend = std::clamp(blendFactor, 0.0f, 1.0f);
    
    // Get target value by calling virtual method slot 16 on the target object
    // This is a virtual dispatch to get the target float value
    float targetValue = 0.0f;
    if (targetObj) {
        void** vtable = *reinterpret_cast<void***>(targetObj);
        typedef float (*GetValueFunc)(void*);
        GetValueFunc getValueFn = reinterpret_cast<GetValueFunc>(vtable[16]);
        targetValue = getValueFn(targetObj);
    }
    
    // Linear interpolation: current + (target - current) * blend
    float currentValue = m_value;
    float delta = targetValue - currentValue;
    m_value = currentValue + delta * clampedBlend;
}

} // namespace rage
