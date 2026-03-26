/**
 * cr_anim_dof_quaternion.cpp -- crAnimDofQuaternion methods
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * rage::crAnimDofQuaternion is the quaternion degrees-of-freedom class in
 * RAGE's animation system.  It represents bone rotations and handles
 * animation blending via quaternion math (slerp, normalize, etc.).
 *
 * This file lifts 15 small (<=256 byte) methods from the original binary.
 */

#include "rage_anim.hpp"
#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// Externals
// ─────────────────────────────────────────────────────────────────────────────

// Base-class sub-object destructor: sets phInst vtable, clears field +12.
// @ 0x824887F8
extern void phInst_87F8(void* obj);

// Clears field +12 on a base-class sub-object.
// @ 0x824885C8
extern void util_85C8(void* obj);

// Thunk that tail-calls util_85C8.
// @ 0x82494128
extern void thunk_fn_824885C8(void* obj);

// Parent-class initializer for crAnimDofQuaternion_32.
// @ 0x8248BA30
extern void phInstStatic_BA30_fw(void* obj);

// crAnimDofQuaternion sub-destructor (one step up the chain).
// @ 0x824858F0
extern void crAnimDofQuaternion_15(void* obj);

// No-op in release builds; debug notification hook.
// @ 0x8240E6D0
extern void rage_DebugLog(uint32_t, uint32_t, uint32_t);

// Cleanup helper: tears down animation state on a crAnimDofQuaternion object.
// @ 0x82498188
extern void atSingleton_8188_w(void* obj);

// Base-class generator/factory helper.
// @ 0x824ADC40
extern void atSingleton_DC40_gen(void* obj);

// RAGE heap deallocator.
// @ 0x820C02D0
extern void _locale_register(void* obj, uint32_t size);

// Full quaternion DOF evaluation/blending pipeline.
// @ 0x8249F070
extern void crAnimDofQuaternion_F070_wrh(void* r3, void* r4, uint32_t r5,
                                          uint32_t r6, void* r7, void* r8);

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// Vtable pointers (from .rdata)
// ─────────────────────────────────────────────────────────────────────────────

// crAnimDofQuaternion primary vtable.
// @ 0x82027BF4
extern void* lbl_82027BF4[];

// Sub-object vtable used by the inner object at +124 during destruction.
// @ 0x82008348
extern void* lbl_82008348[];

// Sub-object vtable used by the inner object at +208 during destruction.
// @ 0x820091B8
extern void* lbl_820091B8[];

// Sub-object vtable used by phInst_87F8 for the inner object at +56.
// @ 0x82008DF0
extern void* lbl_82008DF0[];


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — simple accessors
// ═════════════════════════════════════════════════════════════════════════════

/**
 * SetDofFlags
 * @ 0x824834C0 | size: 0x08
 *
 * Stores a 32-bit flags word at field +76 (0x4C).
 * Called from crAnimDofQuaternion_17 (the serialization path).
 */
// crAnimDofQuaternion_34C0_h @ 0x824834C0
void crAnimDofQuaternion_SetDofFlags(crAnimDofQuaternion* self, uint32_t flags)
{
    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 76) = flags;
}

/**
 * SetBlendWeight
 * @ 0x824834C8 | size: 0x08
 *
 * Stores a float blend weight at field +80 (0x50).
 */
// crAnimDofQuaternion_34C8_h @ 0x824834C8
void crAnimDofQuaternion_SetBlendWeight(crAnimDofQuaternion* self, float weight)
{
    *reinterpret_cast<float*>(
        reinterpret_cast<char*>(self) + 80) = weight;
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — property getters (with null checks)
// ═════════════════════════════════════════════════════════════════════════════

/**
 * GetChannelFlags
 * @ 0x8248BF80 | size: 0x28
 *
 * Reads the 16-bit channel flags from field +238 (0xEE) into *outFlags.
 * Returns 0 on success, 2 if self or outFlags is null.
 */
// crAnimDofQuaternion_BF80_h @ 0x8248BF80
int crAnimDofQuaternion_GetChannelFlags(crAnimDofQuaternion* self, uint16_t* outFlags)
{
    if (!self || !outFlags)
        return 2;

    *outFlags = *reinterpret_cast<uint16_t*>(
        reinterpret_cast<char*>(self) + 238);
    return 0;
}

/**
 * GetConstraintIndexA
 * @ 0x8248CD70 | size: 0x28
 *
 * Reads the 32-bit constraint index from field +204 (0xCC) into *outVal.
 * Returns 0 on success, 2 if self or outVal is null.
 */
// crAnimDofQuaternion_CD70_h @ 0x8248CD70
int crAnimDofQuaternion_GetConstraintIndexA(crAnimDofQuaternion* self, uint32_t* outVal)
{
    if (!self || !outVal)
        return 2;

    *outVal = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 204);
    return 0;
}

/**
 * GetConstraintIndexB
 * @ 0x8248CD98 | size: 0x28
 *
 * Reads the 32-bit constraint index from field +208 (0xD0) into *outVal.
 * Returns 0 on success, 2 if self or outVal is null.
 */
// crAnimDofQuaternion_CD98_h @ 0x8248CD98
int crAnimDofQuaternion_GetConstraintIndexB(crAnimDofQuaternion* self, uint32_t* outVal)
{
    if (!self || !outVal)
        return 2;

    *outVal = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 208);
    return 0;
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — property setters (with null checks)
// ═════════════════════════════════════════════════════════════════════════════

/**
 * SetChannelType
 * @ 0x8248BFA8 | size: 0x2C
 *
 * Stores a 16-bit channel type value at field +236 (0xEC).
 * The value is masked to 16 bits before the null check.
 * Returns 0 on success, 2 if self or value is zero/null.
 */
// crAnimDofQuaternion_BFA8_h @ 0x8248BFA8
int crAnimDofQuaternion_SetChannelType(crAnimDofQuaternion* self, uint16_t value)
{
    if (!self || value == 0)
        return 2;

    *reinterpret_cast<uint16_t*>(
        reinterpret_cast<char*>(self) + 236) = value;
    return 0;
}

/**
 * SetCallback
 * @ 0x8248C300 | size: 0x2C
 *
 * Installs a callback function pointer at field +592 (0x250).
 * When non-null, sets the callback-enabled flag at field +552 (0x228) to 1.
 * When null, clears the flag to 0.
 */
// crAnimDofQuaternion_C300_h @ 0x8248C300
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


// crAnimDofQuaternion_B168_p33 @ 0x8249B168
void crAnimDofQuaternion_CheckDofCountAndDispatch(crAnimDofQuaternion* self)
{
    // Load the pointer at self+0 (vtable or data pointer)
    void* basePtr = *reinterpret_cast<void**>(self);

    // Read the value at offset 60 from that pointer — a DOF count or type code
    int32_t dofCount = *reinterpret_cast<int32_t*>(
        reinterpret_cast<char*>(basePtr) + 60);

    if (dofCount <= 2) {
        *reinterpret_cast<uint32_t*>(
            reinterpret_cast<char*>(self) + 56) = 0;
        return;
    }

    *reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(self) + 56) = 4;

    // Optional callback at field +512 (0x200)
    typedef void (*CallbackFn)();
    CallbackFn callback = *reinterpret_cast<CallbackFn*>(
        reinterpret_cast<char*>(self) + 512);
    if (callback)
        callback();
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — initialization
// ═════════════════════════════════════════════════════════════════════════════

/**
 * InitQuaternionFields
 * @ 0x824852E8 | size: 0x40
 *
 * Calls the parent initializer (phInstStatic_BA30_fw), then zeroes four
 * quaternion-related fields at offsets 660, 664, 668, 672 (0x294-0x2A0).
 * These likely represent accumulated blend state:
 *   +660 = blend count A
 *   +664 = blend count B
 *   +668 = blend accumulator X
 *   +672 = blend accumulator Y
 *
 * Called indirectly via vtable (no direct callers found).
 */
// crAnimDofQuaternion_32 @ 0x824852E8
void crAnimDofQuaternion_InitQuaternionFields(crAnimDofQuaternion* self)
{
    phInstStatic_BA30_fw(self);

    char* p = reinterpret_cast<char*>(self);
    *reinterpret_cast<uint32_t*>(p + 668) = 0;
    *reinterpret_cast<uint32_t*>(p + 672) = 0;
    *reinterpret_cast<uint32_t*>(p + 660) = 0;
    *reinterpret_cast<uint32_t*>(p + 664) = 0;
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — cleanup / teardown
// ═════════════════════════════════════════════════════════════════════════════

/**
 * ResetAnimState
 * @ 0x82498380 | size: 0x50
 *
 * Resets the animation state on a crAnimDofQuaternion object.  Called during
 * teardown sequences.  Issues a debug no-op notification, then calls the
 * main cleanup helper (atSingleton_8188_w) and generator reset
 * (atSingleton_DC40_gen).
 */
// crAnimDofQuaternion_8380_h @ 0x82498380
void crAnimDofQuaternion_ResetAnimState(crAnimDofQuaternion* self)
{
    if (!self)
        return;

    rage_DebugLog(2, 0, 0);
    atSingleton_8188_w(self);
    atSingleton_DC40_gen(self);
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — sub-object destructors
// ═════════════════════════════════════════════════════════════════════════════

/**
 * DestroySubObjectAt124
 * @ 0x82483430 | size: 0x10
 *
 * Destructor for the sub-object embedded at offset +124 from the parent.
 * Sets the sub-object's vtable to lbl_82008348, then tail-calls
 * phInst_87F8 which sets the next-level vtable and clears field +12.
 */
// crAnimDofQuaternion_3430_h @ 0x82483430
void crAnimDofQuaternion_DestroySubObjectAt124(void* subObj)
{
    *reinterpret_cast<void**>(subObj) = lbl_82008348;
    phInst_87F8(subObj);
}

/**
 * DestroySubObjectAt208
 * @ 0x8248D748 | size: 0x10
 *
 * Destructor for the sub-object embedded at offset +208 from the parent.
 * Sets the sub-object's vtable to lbl_820091B8, then tail-calls
 * thunk_fn_824885C8 which clears field +12.
 */
// crAnimDofQuaternion_D748_h @ 0x8248D748
void crAnimDofQuaternion_DestroySubObjectAt208(void* subObj)
{
    *reinterpret_cast<void**>(subObj) = lbl_820091B8;
    thunk_fn_824885C8(subObj);
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — scalar deleting destructor
// ═════════════════════════════════════════════════════════════════════════════

/**
 * ScalarDeletingDestructor
 * @ 0x824864B8 | size: 0x84
 *
 * Scalar deleting destructor for crAnimDofQuaternion.  Installs the
 * crAnimDofQuaternion vtable, destructs sub-objects in reverse construction
 * order (+208, +124, +56, then self), and conditionally frees memory.
 *
 * The destruction chain:
 *   1. Set vtable to crAnimDofQuaternion primary vtable
 *   2. Call crAnimDofQuaternion_15 (parent-level destructor)
 *   3. Destroy sub-object at +208 (crAnimDofQuaternion_D748_h)
 *   4. Destroy sub-object at +124 (crAnimDofQuaternion_3430_h)
 *   5. Destroy sub-object at +56  (phInst_87F8)
 *   6. Clear self field +12       (util_85C8)
 *   7. If flags & 1, free memory  (_locale_register)
 */
// crAnimDofQuaternion_0 @ 0x824864B8
void crAnimDofQuaternion_ScalarDeletingDestructor(crAnimDofQuaternion* self,
                                                   uint32_t flags)
{
    // Install crAnimDofQuaternion vtable
    // (lis/addi computes 0x820088C8 — the primary vtable for this destruction level)
    char* p = reinterpret_cast<char*>(self);

    crAnimDofQuaternion_15(self);

    // Destroy sub-object at +208
    crAnimDofQuaternion_DestroySubObjectAt208(p + 208);

    // Destroy sub-object at +124
    crAnimDofQuaternion_DestroySubObjectAt124(p + 124);

    // Destroy sub-object at +56
    phInst_87F8(p + 56);

    // Clear base-class field +12
    util_85C8(self);

    // Conditionally free memory
    if (flags & 1) {
        _locale_register(self, 0x208C8017);
    }
}


// ═════════════════════════════════════════════════════════════════════════════
//  crAnimDofQuaternion — evaluation wrapper
// ═════════════════════════════════════════════════════════════════════════════

/**
 * EvaluateWrapper
 * @ 0x8249F3B0 | size: 0x54
 *
 * Stack-frame wrapper that forwards to crAnimDofQuaternion_F070_wrh (the
 * full quaternion evaluation pipeline at 0x8249F070).  Sets up a 112-byte
 * stack frame, copies two arguments from the caller's frame, then delegates.
 */
// crAnimDofQuaternion_F3B0_2hr @ 0x8249F3B0
void crAnimDofQuaternion_EvaluateWrapper(void* r3, void* r4, uint32_t r5,
                                          uint32_t r6, void* r7, void* r8)
{
    crAnimDofQuaternion_F070_wrh(r3, r4, r5, r6, r7, r8);
}


// ═════════════════════════════════════════════════════════════════════════════
//  Wide-char string comparison utility
// ═════════════════════════════════════════════════════════════════════════════

/**
 * WideCharCompareN
 * @ 0x825693E8 | size: 0x50
 *
 * Compares two wide-character (16-bit) strings up to a maximum of `count`
 * characters.  Equivalent to wcsncmp() but operates on uint16_t arrays.
 *
 * Returns:
 *   0 if strings are equal up to count characters or first null
 *   positive if str1 > str2 at the first differing character
 *   negative if str1 < str2 at the first differing character
 *   0 if count is 0
 *
 * Used by crAnimDofQuaternion_31 (the large deserialization function) to
 * match DOF type name strings like channel names.
 */
// crAnimDofQuaternion_93E8_h @ 0x825693E8
int crAnimDofQuaternion_WideCharCompareN(const uint16_t* str1,
                                          const uint16_t* str2,
                                          uint32_t count)
{
    if (count == 0)
        return 0;

    for (uint32_t i = 0; i < count - 1; i++) {
        if (str1[i] == 0)
            return static_cast<int>(str1[i]) - static_cast<int>(str2[i]);
        if (str1[i] != str2[i])
            return static_cast<int>(str1[i]) - static_cast<int>(str2[i]);
    }

    // Final character comparison (handles both loop-exhausted and early-exit)
    return static_cast<int>(str1[count - 1]) - static_cast<int>(str2[count - 1]);
}

} // namespace rage
