/**
 * cr_anim_dof.cpp — crAnimDof / crAnimDofQuaternion Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * rage::crAnimDof is the base class for degrees-of-freedom in RAGE's
 * animation system.  crAnimDofQuaternion handles bone rotations via
 * quaternion math (slerp, normalize, blend).
 *
 * This file implements 18 methods across both classes:
 *   crAnimDof:
 *     - GetDefaultVector3 (lazy-init default vec3)
 *     - GetIdentityVector (lazy-init identity vec3)
 *   crAnimDofQuaternion:
 *     - Evaluate (quaternion blend entry point)
 *     - SetDofFlags, SetBlendWeight (simple accessors)
 *     - GetChannelFlags, GetConstraintIndexA/B (null-checked getters)
 *     - SetChannelType, SetCallback (null-checked setters)
 *     - CheckDofCountAndDispatch (DOF count routing)
 *     - InitQuaternionFields (field initialization)
 *     - ResetAnimState (teardown + generator reset)
 *     - DestroySubObjectAt124, DestroySubObjectAt208 (sub-object dtors)
 *     - ScalarDeletingDestructor (full destruction chain)
 *     - EvaluateWrapper (forwarding thunk)
 *   Utility:
 *     - WideCharCompareN (wcsncmp equivalent for DOF name matching)
 */

#include "rage_anim.hpp"
#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// Quaternion blend/copy — takes (outQuat, animContext, srcQuatData) and
// performs a 488-byte quaternion interpolation pipeline.
// @ 0x820C44A0 | size: 0x1E8
extern void crAnimDof_BlendQuaternion(void* outQuat, void* animContext,
                                       float* srcQuatData);

// Sub-object destructor for phInst-derived embedded objects.
// Sets the phInst vtable at +0, then clears field +12.
// @ 0x824887F8 | size: 0x10
extern void phInst_DestroySubObject(void* subObj);

// Clears the base-class datBase field at offset +12.
// @ 0x824885C8 | size: 0x0C
extern void datBase_ClearField12(void* obj);

// Thunk that tail-calls datBase_ClearField12.
// @ 0x82494128
extern void datBase_ClearField12_Thunk(void* obj);

// Parent-class initializer for crAnimDofQuaternion sub-objects.
// @ 0x8248BA30
extern void phInstStatic_Initialize(void* obj);

// One-level-up destructor in the crAnimDofQuaternion hierarchy.
// Handles the parent class's teardown before sub-objects are destroyed.
// @ 0x824858F0
extern void crAnimDofQuaternion_DestroyParent(void* obj);

// Debug log — no-op in release builds (single blr instruction).
// @ 0x8240E6D0
extern void nop_DebugLog(uint32_t level, uint32_t a, uint32_t b);

// Animation state teardown — tears down blend state, clip references,
// and channel bindings on a crAnimDofQuaternion object.
// @ 0x82498188 | size: 0x1F8
extern void crAnimDof_TeardownAnimState(void* obj);

// Factory/generator reset — resets the animation DOF's generator state.
// @ 0x824ADC40 | size: 0x0C
extern void crAnimDof_ResetGenerator(void* obj);

// RAGE heap deallocator (takes obj + allocation-tag flags).
// @ 0x820C02D0
extern void rage_HeapFree(void* obj, uint32_t allocTag);

// Full quaternion DOF evaluation/blending pipeline.
// @ 0x8249F070 | size: ~0x340
extern void crAnimDofQuaternion_EvalPipeline(void* r3, void* r4, uint32_t r5,
                                              uint32_t r6, void* r7, void* r8);

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// Vtable pointers (from .rdata)
// ─────────────────────────────────────────────────────────────────────────────

// Sub-object vtable for the embedded object at +124 (64 bytes, no RTTI).
// @ 0x82008348
extern void* g_crAnimDofQuat_SubVtable124[];

// Sub-object vtable for the embedded object at +208 (76 bytes, no RTTI).
// @ 0x820091B8
extern void* g_crAnimDofQuat_SubVtable208[];

// ─────────────────────────────────────────────────────────────────────────────
// Lazy-initialized global vectors
// ─────────────────────────────────────────────────────────────────────────────

// Guard + storage for the default Vector3 value (all zeros).
extern uint32_t g_defaultVec3Guard;    // @ 0x8271B110
extern float    g_defaultVec3[4];      // @ 0x8271B100

// Guard + storage for the identity/zero vector.
extern uint32_t g_identityVecGuard;    // @ 0x8271B0F0
extern float    g_identityVec[4];      // @ 0x8271B0E0

// Float constant in .rdata (0.0f).
extern float    g_zeroFloat;           // @ 0x8202D110


// ═══════════════════════════════════════════════════════════════════════════════
//  crAnimDof — Base Class
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * crAnimDof::GetDefaultVector3  [vtable slot 14]
 * @ 0x820C4688 | size: 0x3C
 *
 * Returns a pointer to a lazily-initialized default 3-component vector.
 * On first call, sets all three components to 0.0f and sets the guard bit.
 */
float* crAnimDof_GetDefaultVector3()
{
    float* result = g_defaultVec3;

    if (g_defaultVec3Guard & 1)
        return result;

    g_defaultVec3Guard |= 1;
    result[0] = g_zeroFloat;
    result[1] = g_zeroFloat;
    result[2] = g_zeroFloat;
    return result;
}

/**
 * crAnimDof::GetIdentityVector  [vtable slot 15]
 * @ 0x820C46C8 | size: 0x28
 *
 * Returns a pointer to a lazily-initialized identity/zero vector.
 * On first call, sets the guard bit (storage is already zero from BSS).
 */
float* crAnimDof_GetIdentityVector()
{
    float* result = g_identityVec;

    if (g_identityVecGuard & 1)
        return result;

    g_identityVecGuard |= 1;
    return result;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — Quaternion DOF Class
// ═══════════════════════════════════════════════════════════════════════════════

// ─── Evaluation ──────────────────────────────────────────────────────────────

/**
 * crAnimDofQuaternion::Evaluate  [vtable slot 4]
 * @ 0x820C4BF0 | size: 0x10
 *
 * Evaluates the quaternion DOF by tail-calling the quaternion blend helper.
 * Rearranges arguments: output buffer first, then animation context, then
 * the internal quaternion data at object offset +16.
 */
void crAnimDofQuaternion_Evaluate(crAnimDofQuaternion* self,
                                   void* animContext, float* outQuat)
{
    crAnimDof_BlendQuaternion(outQuat, animContext,
                              reinterpret_cast<float*>(
                                  reinterpret_cast<char*>(self) + 16));
}

/**
 * crAnimDofQuaternion::EvaluateWrapper
 * @ 0x8249F3B0 | size: 0x54
 *
 * Stack-frame wrapper that forwards to the full quaternion evaluation
 * pipeline (crAnimDofQuaternion_EvalPipeline @ 0x8249F070).
 */
void crAnimDofQuaternion_EvaluateWrapper(void* r3, void* r4, uint32_t r5,
                                          uint32_t r6, void* r7, void* r8)
{
    crAnimDofQuaternion_EvalPipeline(r3, r4, r5, r6, r7, r8);
}

// ─── Simple Accessors ────────────────────────────────────────────────────────

/**
 * SetDofFlags  @ 0x824834C0 | size: 0x08
 * Stores a 32-bit flags word at field +76 (0x4C).
 */
void crAnimDofQuaternion_SetDofFlags(crAnimDofQuaternion* self, uint32_t flags)
{
    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 76) = flags;
}

/**
 * SetBlendWeight  @ 0x824834C8 | size: 0x08
 * Stores a float blend weight at field +80 (0x50).
 */
void crAnimDofQuaternion_SetBlendWeight(crAnimDofQuaternion* self, float weight)
{
    *reinterpret_cast<float*>(
        reinterpret_cast<char*>(self) + 80) = weight;
}

// ─── Property Getters (null-checked) ─────────────────────────────────────────

/**
 * GetChannelFlags  @ 0x8248BF80 | size: 0x28
 * Reads 16-bit channel flags from field +238 (0xEE).
 * Returns 0 on success, 2 if self or outFlags is null.
 */
int crAnimDofQuaternion_GetChannelFlags(crAnimDofQuaternion* self,
                                         uint16_t* outFlags)
{
    if (!self || !outFlags)
        return 2;

    *outFlags = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<char*>(self) + 238);
    return 0;
}

/**
 * GetConstraintIndexA  @ 0x8248CD70 | size: 0x28
 * Reads 32-bit constraint index from field +204 (0xCC).
 */
int crAnimDofQuaternion_GetConstraintIndexA(crAnimDofQuaternion* self,
                                             uint32_t* outVal)
{
    if (!self || !outVal)
        return 2;

    *outVal = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 204);
    return 0;
}

/**
 * GetConstraintIndexB  @ 0x8248CD98 | size: 0x28
 * Reads 32-bit constraint index from field +208 (0xD0).
 */
int crAnimDofQuaternion_GetConstraintIndexB(crAnimDofQuaternion* self,
                                             uint32_t* outVal)
{
    if (!self || !outVal)
        return 2;

    *outVal = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 208);
    return 0;
}

// ─── Property Setters (null-checked) ─────────────────────────────────────────

/**
 * SetChannelType  @ 0x8248BFA8 | size: 0x2C
 * Stores 16-bit channel type at field +236 (0xEC).
 */
int crAnimDofQuaternion_SetChannelType(crAnimDofQuaternion* self,
                                        uint16_t value)
{
    if (!self || value == 0)
        return 2;

    *reinterpret_cast<uint16_t*>(
        reinterpret_cast<char*>(self) + 236) = value;
    return 0;
}

/**
 * SetCallback  @ 0x8248C300 | size: 0x2C
 * Installs a callback function pointer at field +592 (0x250).
 * Sets the callback-enabled flag at field +552 (0x228).
 */
void crAnimDofQuaternion_SetCallback(crAnimDofQuaternion* self, void* callback)
{
    if (callback) {
        *reinterpret_cast<void**>(
            reinterpret_cast<char*>(self) + 592) = callback;
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<char*>(self) + 552) = 1;
    } else {
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<char*>(self) + 552) = 0;
    }
}

// ─── DOF Count Dispatch ──────────────────────────────────────────────────────

/**
 * CheckDofCountAndDispatch  @ 0x8249B168
 * Reads the DOF count from the object's base data at +60.
 * If count <= 2, sets field +56 to 0 (simple mode).
 * If count > 2, sets field +56 to 4 (complex mode) and invokes
 * an optional callback at field +512.
 */
void crAnimDofQuaternion_CheckDofCountAndDispatch(crAnimDofQuaternion* self)
{
    void* basePtr = *reinterpret_cast<void**>(self);
    int32_t dofCount = *reinterpret_cast<int32_t*>(
        reinterpret_cast<char*>(basePtr) + 60);

    if (dofCount <= 2) {
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<char*>(self) + 56) = 0;
        return;
    }

    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 56) = 4;

    typedef void (*CallbackFn)();
    CallbackFn callback = *reinterpret_cast<CallbackFn*>(
        reinterpret_cast<char*>(self) + 512);
    if (callback)
        callback();
}

// ─── Initialization ──────────────────────────────────────────────────────────

/**
 * InitQuaternionFields  @ 0x824852E8 | size: 0x40
 * Calls the parent initializer, then zeroes four blend accumulator fields
 * at offsets 660-672 (0x294-0x2A0).
 */
void crAnimDofQuaternion_InitQuaternionFields(crAnimDofQuaternion* self)
{
    phInstStatic_Initialize(self);

    char* p = reinterpret_cast<char*>(self);
    *reinterpret_cast<uint32_t*>(p + 660) = 0;  // blend count A
    *reinterpret_cast<uint32_t*>(p + 664) = 0;  // blend count B
    *reinterpret_cast<uint32_t*>(p + 668) = 0;  // blend accumulator X
    *reinterpret_cast<uint32_t*>(p + 672) = 0;  // blend accumulator Y
}

// ─── Cleanup / Teardown ──────────────────────────────────────────────────────

/**
 * ResetAnimState  @ 0x82498380 | size: 0x50
 * Resets animation state: issues a debug notification, tears down
 * animation state, then resets the generator.
 */
void crAnimDofQuaternion_ResetAnimState(crAnimDofQuaternion* self)
{
    if (!self)
        return;

    nop_DebugLog(2, 0, 0);
    crAnimDof_TeardownAnimState(self);
    crAnimDof_ResetGenerator(self);
}

// ─── Sub-Object Destructors ──────────────────────────────────────────────────

/**
 * DestroySubObjectAt124  @ 0x82483430 | size: 0x10
 * Sets the sub-object's vtable to the intermediate vtable (@ 0x82008348),
 * then calls phInst_DestroySubObject to clear the phInst base.
 */
void crAnimDofQuaternion_DestroySubObjectAt124(void* subObj)
{
    *reinterpret_cast<void**>(subObj) = g_crAnimDofQuat_SubVtable124;
    phInst_DestroySubObject(subObj);
}

/**
 * DestroySubObjectAt208  @ 0x8248D748 | size: 0x10
 * Sets the sub-object's vtable to the intermediate vtable (@ 0x820091B8),
 * then calls datBase_ClearField12 via its thunk.
 */
void crAnimDofQuaternion_DestroySubObjectAt208(void* subObj)
{
    *reinterpret_cast<void**>(subObj) = g_crAnimDofQuat_SubVtable208;
    datBase_ClearField12_Thunk(subObj);
}

// ─── Scalar Deleting Destructor ──────────────────────────────────────────────

/**
 * ScalarDeletingDestructor  @ 0x824864B8 | size: 0x84
 *
 * Full destruction chain for crAnimDofQuaternion:
 *   1. Call parent-level destructor (crAnimDofQuaternion_DestroyParent)
 *   2. Destroy sub-object at +208
 *   3. Destroy sub-object at +124
 *   4. Destroy sub-object at +56 (phInst)
 *   5. Clear base-class field +12
 *   6. If flags & 1, free memory
 */
void crAnimDofQuaternion_ScalarDeletingDestructor(crAnimDofQuaternion* self,
                                                   uint32_t flags)
{
    char* p = reinterpret_cast<char*>(self);

    crAnimDofQuaternion_DestroyParent(self);
    crAnimDofQuaternion_DestroySubObjectAt208(p + 208);
    crAnimDofQuaternion_DestroySubObjectAt124(p + 124);
    phInst_DestroySubObject(p + 56);
    datBase_ClearField12(self);

    if (flags & 1) {
        rage_HeapFree(self, 0x208C8017);
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Utility — Wide-char string comparison
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * WideCharCompareN  @ 0x825693E8 | size: 0x50
 *
 * Compares two 16-bit wide-character strings up to `count` characters.
 * Equivalent to wcsncmp().  Used by the DOF deserialization to match
 * channel type name strings.
 */
int crAnimDof_WideCharCompareN(const uint16_t* str1,
                                const uint16_t* str2,
                                uint32_t count)
{
    if (count == 0)
        return 0;

    for (uint32_t i = 0; i < count - 1; i++) {
        if (str1[i] == 0 || str1[i] != str2[i])
            return static_cast<int>(str1[i]) - static_cast<int>(str2[i]);
    }

    return static_cast<int>(str1[count - 1]) - static_cast<int>(str2[count - 1]);
}

} // namespace rage
