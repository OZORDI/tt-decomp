/**
 * cr_anim_dof.cpp -- crAnimDof / crAnimDofQuaternion virtual methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * rage::crAnimDof is the base class for degrees-of-freedom in RAGE's
 * animation system.  These two methods provide lazy-initialized default
 * vectors and a quaternion evaluation entry point.
 *
 * This file lifts 3 small (8-60 byte) methods from the original binary.
 */

#include "rage_anim.hpp"
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// Externals
// ─────────────────────────────────────────────────────────────────────────────

// Quaternion blend/copy helper used by crAnimDofQuaternion::Evaluate.
// @ 0x820C44A0
extern void phBoundCapsule_44A0_h(void* animContext, float time, float* outQuat);

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// Globals (lazy-initialized singletons)
// ─────────────────────────────────────────────────────────────────────────────

// Guard + storage for the default Vector3 value.
// @ 0x8271B110 (guard, 4 bytes) / 0x8271B100 (vector, 16 bytes)
extern uint32_t lbl_8271B110;
extern float    lbl_8271B100[4];

// Guard + storage for the identity/zero vector.
// @ 0x8271B0F0 (guard, 4 bytes) / 0x8271B0E0 (vector, 16 bytes)
extern uint32_t lbl_8271B0F0;
extern float    lbl_8271B0E0[4];

// Float constant in .rdata — the default value (likely 0.0f).
// @ 0x8202D110
extern float    lbl_8202D110;


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDof — base-class lazy-initialized default vectors
// ═════════════════════════════════════════════════════════════════════════════

/**
 * GetDefaultVector3
 * @ 0x820C4688 | size: 0x3C (60 bytes) | vtable slot 14
 *
 * Returns a pointer to a lazily-initialized default 3-component vector.
 * On first call, sets all three components to the .rdata float constant
 * (0.0f) and marks the guard bit.  Subsequent calls return immediately.
 */
// crAnimDof_vfn_14 @ 0x820C4688
float* crAnimDof_GetDefaultVector3()
{
    float* result = lbl_8271B100;

    if (lbl_8271B110 & 1)
        return result;

    lbl_8271B110 |= 1;
    result[0] = lbl_8202D110;
    result[1] = lbl_8202D110;
    result[2] = lbl_8202D110;
    return result;
}

/**
 * GetIdentityVector
 * @ 0x820C46C8 | size: 0x28 (40 bytes) | vtable slot 15
 *
 * Returns a pointer to a lazily-initialized identity/zero vector.
 * On first call, marks the guard bit (storage is already zero-init
 * from BSS).  Subsequent calls return immediately.
 */
// crAnimDof_vfn_15 @ 0x820C46C8
float* crAnimDof_GetIdentityVector()
{
    float* result = lbl_8271B0E0;

    if (lbl_8271B0F0 & 1)
        return result;

/**
 * crAnimDofQuaternion::Evaluate
 * @ 0x820C4BF0 | size: 0x10 (16 bytes) | vtable slot 4
 *
 * Evaluates the quaternion DOF by tail-calling the quaternion
 * blend/copy helper, rearranging arguments so the output buffer
 * comes first and a pointer to internal quaternion data at +16
 * is passed as the source.
 */
// crAnimDofQuaternion_vfn_4 @ 0x820C4BF0
void crAnimDofQuaternion_Evaluate(crAnimDofQuaternion* self, void* animContext,
                                   float* outQuat)
{
    phBoundCapsule_44A0_h(outQuat, animContext,
                          reinterpret_cast<float*>(
                              reinterpret_cast<char*>(self) + 16));
}

} // namespace rage
