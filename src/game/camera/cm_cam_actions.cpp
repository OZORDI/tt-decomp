/**
 * cm_cam_actions.cpp — cmSampleCamActions camera parameter sampler functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The cmSampleCamActions system is a set of CM node action functions for camera
 * sampling. They read camera parameters (position, rotation, FOV, etc.) from
 * the connection machine graph ports and write them into camera output structures.
 */

#include <cstdint>
#include <cstring>
#include <cmath>

// ── Struct layouts ──────────────────────────────────────────────────────────

// cmCamOutput — output buffer that receives sampled camera parameters.
//   +0x00  m_pNode       owning CM node (vtable[1] = GetSlot())
//   +0x04  m_dirtyFlags  bitmask of which slots have been written
struct cmCamOutput {
    void*    m_pNode;        // +0x00
    uint32_t m_dirtyFlags;   // +0x04
};

// cmCameraAction — action context passed to sampler functions.
//   +0x00  vtable / action ptr
//   +0x04  m_pCamOutput  pointer to the camera output buffer
struct cmCameraAction {
    void*         vtable;        // +0x00
    cmCamOutput*  m_pCamOutput;  // +0x04
};

// cmActionNode — CM graph action node header (partial layout).
//   +0x00  vtable
//   +0x04  m_name       action name (string hash or ptr)
//   +0x10  m_portStatus port status bitmask (low 3 bits = connectivity)
struct cmActionNode {
    void*     vtable;        // +0x00
    uint32_t  m_name;        // +0x04
    uint32_t  _pad_0x08;     // +0x08
    uint32_t  _pad_0x0C;     // +0x0C
    uint32_t  m_portStatus;  // +0x10
};

// cmSampleCamActions_P_VelocityFollow — sampler with delegation support.
//   +0x00  vtable
//   +0x04  m_actionName   action name ptr (passed to rage_DebugLog)
//   +0x1C  m_pReporter    optional reporter object (reset in init)
//   +0x54  m_frameCounter cleared on init
//   +0x58  m_delegateFlag set when primary evaluation failed
//   +0x5C  m_pDelegateNode fallback inner node used when delegation active
//   +0x60  m_tiltLimitLow  tilt clamp (cleared by vfn_13)
//   +0x64  m_tiltLimitHigh tilt clamp (cleared by vfn_13)
//   +0x70  m_velocity[4]  accumulated velocity vec4
struct cmSampleCamActions_P_VelocityFollow {
    void*     vtable;             // +0x00
    void*     m_actionName;       // +0x04
    uint8_t   _pad_0x08[0x14];    // +0x08..+0x1B
    void*     m_pReporter;        // +0x1C
    uint8_t   _pad_0x20[0x34];    // +0x20..+0x53
    uint32_t  m_frameCounter;     // +0x54
    uint8_t   m_delegateFlag;     // +0x58
    uint8_t   _pad_0x59[3];       // +0x59..+0x5B
    void*     m_pDelegateNode;    // +0x5C
    float     m_tiltLimitLow;     // +0x60
    float     m_tiltLimitHigh;    // +0x64
    uint8_t   _pad_0x68[8];       // +0x68..+0x6F
    float     m_velocity[4];      // +0x70..+0x7F
};

// ── External declarations for CM helper functions ───────────────────────────

extern "C" {

// cmAction_ReadPortFloat @ 0x822610B8 | size: 0x8C
float cmAction_ReadPortFloat(void* node, int portIndex);

// cmAction_ReadPortVec4 @ 0x82261148 | size: 0x88
float* cmAction_ReadPortVec4(float* outVec, void* node, int portIndex);

// cmCamOutput_WriteSlot @ 0x82275CC8 | size: 0x6C
void cmCamOutput_WriteSlot(void* camOutput, int slotIndex, void* actionNode,
                            float value);

// cmAction_ReadRotation @ 0x82183220 | size: 0x154
void cmAction_ReadRotation(float* outVec, void* actionNode);

// cmReporter_Reset @ 0x822752F0 | size: 0x90
void cmReporter_Reset(void* reporter);

// cmAction_TeardownPorts @ 0x822771E0 | size: 0x9C
void cmAction_TeardownPorts(void* node);

// rage_free @ 0x820C00C0 | size: 0x60
void rage_free(void* ptr);

// cmSimple_vfn_4 @ 0x822772A0 | size: 0xA4
void cmSimple_vfn_4(void* node, float* out);

// cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10 @ 0x822774B8 | size: 0x90
void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(void* node, void* data);

// fmod_safe @ 0x82431D78 | size: 0x128
float fmod_safe(float value, float range);

// rage_DebugLog @ 0x8240E6D0 | size: 0x4 (nop in retail build)
void rage_DebugLog(const char* fmt, ...);

} // extern "C"

// ── Extern data ─────────────────────────────────────────────────────────────

extern "C" void* cmAction_vtable;  // rage::cmAction vtable @ 0x82052224 (.rdata)

// Vtable slot signature for cmNode::GetSlot(slotIndex) at vtable[1].
typedef void* (*GetSlotFn)(void*, int);

// ═══════════════════════════════════════════════════════════════════════════════
//  GLOBAL HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * TransformPoint3x4 @ 0x820E86A8 | size: 0x94
 *
 * Transforms a 3D point by a 4x3 affine matrix.
 * out[i] = mat_row_i . pt + translation_i
 */
extern "C" void TransformPoint3x4(float* out, const float* pt, const float* mat) {
    float x = pt[0], y = pt[1], z = pt[2];

    out[0] = mat[4] * y + mat[8] * z + x * mat[0] + mat[12];
    out[1] = mat[9] * z + mat[1] * x + mat[5] * y + mat[13];
    out[2] = mat[10] * z + mat[2] * x + mat[6] * y + mat[14];
}

/**
 * WrapValueInRange @ 0x821129D0 | size: 0x88
 *
 * Wraps a value into the range [min, max) with circular wrapping.
 */
extern "C" float WrapValueInRange(float value, float min, float max) {
    if (max < min) {
        return value;
    }
    if (max == min) {
        return min;
    }
    if (value < min) {
        float range = max - min;
        float dist = min - value;
        float wrapped = fmod_safe(dist, range);
        return max - wrapped;
    }
    if (value <= max) {
        return value;
    }
    float range = max - min;
    float dist = value - max;
    float wrapped = fmod_safe(dist, range);
    return wrapped + min;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  CAMERA ACTION SAMPLER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_HeadingAndFOV @ 0x821797F8 | size: 0xAC
 *
 * Samples 2 floats from ports 0 and 1. If all ports connected (status & 7 == 7),
 * sets dirty bit 0x4 (FOV). Writes to cam output slots 0 (pitch) and 1 (yaw).
 */
extern "C" void cmSampleCamActions_HeadingAndFOV(void* actionCtx, void* actionNode) {
    auto* ctx  = (cmCameraAction*)actionCtx;
    auto* node = (cmActionNode*)actionNode;

    float pitch = cmAction_ReadPortFloat(actionNode, 0);
    float yaw   = cmAction_ReadPortFloat(actionNode, 1);

    if ((node->m_portStatus & 0x7) == 7) {
        ctx->m_pCamOutput->m_dirtyFlags |= 0x4;
    }

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 0, actionNode, pitch);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 1, actionNode, yaw);
}

/**
 * cmSampleCamActions_Rotation @ 0x821798A8 | size: 0x70
 *
 * Samples 1 float from port 0. If all ports connected, sets dirty bits 0x3
 * (pitch + yaw). Writes the sampled value to cam output slot 2 (FOV).
 */
extern "C" void cmSampleCamActions_Rotation(void* actionCtx, void* actionNode) {
    auto* ctx  = (cmCameraAction*)actionCtx;
    auto* node = (cmActionNode*)actionNode;

    float rotationValue = cmAction_ReadPortFloat(actionNode, 0);

    if ((node->m_portStatus & 0x7) == 7) {
        ctx->m_pCamOutput->m_dirtyFlags |= 0x3;
    }

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 2, actionNode, rotationValue);
}

/**
 * cmSampleCamActions_BaseXYZ @ 0x82179918 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2. Writes to cam output slots 0/1/2.
 */
extern "C" void cmSampleCamActions_BaseXYZ(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float val0 = cmAction_ReadPortFloat(actionNode, 0);
    float val1 = cmAction_ReadPortFloat(actionNode, 1);
    float val2 = cmAction_ReadPortFloat(actionNode, 2);

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 0, actionNode, val0);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 1, actionNode, val1);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 2, actionNode, val2);
}

/**
 * cmSampleCamActions_FOV @ 0x821799D0 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2. Writes to cam output slots 5, 4, 3
 * (look-at target, reversed slot order).
 */
extern "C" void cmSampleCamActions_FOV(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float val0 = cmAction_ReadPortFloat(actionNode, 0);
    float val1 = cmAction_ReadPortFloat(actionNode, 1);
    float val2 = cmAction_ReadPortFloat(actionNode, 2);

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 5, actionNode, val0);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 4, actionNode, val1);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 3, actionNode, val2);
}

/**
 * cmSampleCamActions_Orientation @ 0x82179A88 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1. Writes vec4 x/y/z to slots 6/7/8
 * and float to slot 9 (orientation vec3 + angle).
 */
extern "C" void cmSampleCamActions_Orientation(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float orientVec[4];
    cmAction_ReadPortVec4(orientVec, actionNode, 0);
    float orientAngle = cmAction_ReadPortFloat(actionNode, 1);

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 6, actionNode, orientVec[0]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 7, actionNode, orientVec[1]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 8, actionNode, orientVec[2]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 9, actionNode, orientAngle);
}

/**
 * cmSampleCamActions_LookAtTarget @ 0x82179B30 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1. Writes vec4 x/y/z to slots
 * 10/11/12 and float to slot 13 (secondary orientation vec3 + angle).
 */
extern "C" void cmSampleCamActions_LookAtTarget(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float targetVec[4];
    cmAction_ReadPortVec4(targetVec, actionNode, 0);
    float targetAngle = cmAction_ReadPortFloat(actionNode, 1);

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 10, actionNode, targetVec[0]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 11, actionNode, targetVec[1]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 12, actionNode, targetVec[2]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 13, actionNode, targetAngle);
}

/**
 * cmSampleCamActions_OrientationPair @ 0x82179740 | size: 0xB8
 *
 * Samples two vec4 values from ports 0 and 1. Writes via vcall slot 1 (GetSlot)
 * to cam output slots 14 and 15. Sets dirty bits 0x4000 and 0x8000.
 */
extern "C" void cmSampleCamActions_OrientationPair(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float orientVec0[4], orientVec1[4];
    cmAction_ReadPortVec4(orientVec0, actionNode, 0);
    cmAction_ReadPortVec4(orientVec1, actionNode, 1);

    // Write orientVec0 to slot 14 via vcall GetSlot
    {
        cmCamOutput* camOutput = ctx->m_pCamOutput;
        void* cmNode = camOutput->m_pNode;
        void** vtable = *(void***)cmNode;
        void* slot14ptr = ((GetSlotFn)vtable[1])(cmNode, 14);
        memcpy(slot14ptr, orientVec0, 16);
        camOutput->m_dirtyFlags |= 0x4000;
    }

    // Write orientVec1 to slot 15 via vcall GetSlot
    {
        cmCamOutput* camOutput = ctx->m_pCamOutput;
        void* cmNode = camOutput->m_pNode;
        void** vtable = *(void***)cmNode;
        void* slot15ptr = ((GetSlotFn)vtable[1])(cmNode, 15);
        memcpy(slot15ptr, orientVec1, 16);
        camOutput->m_dirtyFlags |= 0x8000;
    }
}

/**
 * cmSampleCamActions_ZeroTiltRoll @ 0x8217BDE0 | size: 0x8C
 *
 * Writes 0.0f to slots 6 and 8 (orientation X/Z). Sets dirty bits 0x80 (slot 7)
 * and 0x200 (slot 9) to mark the full orientation group as dirty.
 */
extern "C" void cmSampleCamActions_ZeroTiltRoll(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 6, actionNode, 0.0f);
    ctx->m_pCamOutput->m_dirtyFlags |= 0x80;

    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 8, actionNode, 0.0f);
    ctx->m_pCamOutput->m_dirtyFlags |= 0x200;
}

/**
 * cmSampleCamActions_RotationXZ @ 0x8217BE70 | size: 0x88
 *
 * Reads camera rotation, writes x-component to slot 10 and z-component to slot 12.
 * Sets dirty bits 0x800 (slot 11) and 0x2000 (slot 13).
 */
extern "C" void cmSampleCamActions_RotationXZ(void* actionCtx, void* actionNode) {
    auto* ctx = (cmCameraAction*)actionCtx;

    float rotVec[4];
    cmAction_ReadRotation(rotVec, actionNode);

    ctx->m_pCamOutput->m_dirtyFlags |= 0x800;
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 10, actionNode, rotVec[0]);
    cmCamOutput_WriteSlot(ctx->m_pCamOutput, 12, actionNode, rotVec[2]);
    ctx->m_pCamOutput->m_dirtyFlags |= 0x2000;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::H_SmoothLookAt_LimitedTilt VTABLE METHODS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_13 @ 0x8217B550 | size: 0x14
 *
 * Resets tilt limit parameters (m_tiltLimitLow / m_tiltLimitHigh) to 0.0f.
 */
extern "C" void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_13(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;
    obj->m_tiltLimitLow  = 0.0f;  // +0x60
    obj->m_tiltLimitHigh = 0.0f;  // +0x64
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::P_VelocityFollow VTABLE METHODS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_0 @ 0x821793D0 | size: 0x5C
 *
 * Destructor. Sets vtable to rage::cmAction base, tears down ports, optionally
 * frees memory.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_0(void* self, int flags) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;
    obj->vtable = &cmAction_vtable;
    cmAction_TeardownPorts(self);
    if (flags & 1) {
        rage_free(self);
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_3 @ 0x82260F40 | size: 0x4C
 *
 * Init/reset. Clears m_frameCounter, resets reporter if present, calls vfn_12.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_3(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    obj->m_frameCounter = 0;

    if (obj->m_pReporter != nullptr) {
        cmReporter_Reset(obj->m_pReporter);
    }

    void** vtable = *(void***)self;
    typedef void (*VFn12)(void*);
    ((VFn12)vtable[12])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_2 @ 0x82260F90 | size: 0xA0
 *
 * Update. Calls vfn_11 (evaluate), then vfn_15 (validate). If validation fails
 * and a delegation node exists, sets delegation flag and logs debug message.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_2(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    void** vtable = *(void***)self;
    typedef void (*VFn11)(void*);
    ((VFn11)vtable[11])(self);

    vtable = *(void***)self;
    typedef uint8_t (*VFn15)(void*);
    uint8_t isValid = ((VFn15)vtable[15])(self);

    if ((isValid & 0xFF) == 0) {
        void* delegateNode = obj->m_pDelegateNode;
        void* actionName   = obj->m_actionName;
        if (delegateNode != nullptr) {
            obj->m_delegateFlag = 1;
            uint32_t delegateName = ((cmActionNode*)delegateNode)->m_name;
            rage_DebugLog("%s", actionName, delegateName);
        } else {
            rage_DebugLog("%s", actionName);
        }
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_4 @ 0x82261030 | size: 0x24
 *
 * Delegated GetFloat. If m_delegateFlag is set, delegates to the inner node
 * vtable[4]. Otherwise falls through to cmSimple_vfn_4.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_4(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    if (obj->m_delegateFlag != 0) {
        void* delegateNode = obj->m_pDelegateNode;
        void** vtable = *(void***)delegateNode;
        typedef void (*VFn4)(void*);
        ((VFn4)vtable[4])(delegateNode);
        return;
    }
    cmSimple_vfn_4(self, nullptr);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_5 @ 0x82261058 | size: 0x30
 *
 * Delegated GetDim. If m_delegateFlag is set, delegates to inner node vtable[13].
 * Otherwise calls own vtable[13].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_5(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    if (obj->m_delegateFlag != 0) {
        void* delegateNode = obj->m_pDelegateNode;
        void** vtable = *(void***)delegateNode;
        typedef void (*VFn13)(void*);
        ((VFn13)vtable[13])(delegateNode);
        return;
    }
    void** vtable = *(void***)self;
    typedef void (*VFn13)(void*);
    ((VFn13)vtable[13])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_6 @ 0x82261088 | size: 0x30
 *
 * Delegated SyncPorts. If m_delegateFlag is set, delegates to inner node
 * vtable[6]. Otherwise calls own vtable[14].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_6(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    if (obj->m_delegateFlag != 0) {
        void* delegateNode = obj->m_pDelegateNode;
        void** vtable = *(void***)delegateNode;
        typedef void (*VFn6)(void*);
        ((VFn6)vtable[6])(delegateNode);
        return;
    }
    void** vtable = *(void***)self;
    typedef void (*VFn14)(void*);
    ((VFn14)vtable[14])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_10 @ 0x8217B188 | size: 0x4C
 *
 * Position update. Calls parent H_SmoothLookAt_LimitedTilt vfn_10, then
 * subtracts the input position from m_velocity, computing relative velocity.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_10(void* self, void* posData) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;

    cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(self, posData);

    const float* inputPos = (const float*)posData;
    obj->m_velocity[0] -= inputPos[0];
    obj->m_velocity[1] -= inputPos[1];
    obj->m_velocity[2] -= inputPos[2];
    obj->m_velocity[3] -= inputPos[3];
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmAction — shared action dispatch / fallback method
// ═══════════════════════════════════════════════════════════════════════════════

// String anchors (used by rage_DebugLog; NOPed in retail, but preserved for
// static analysis and symbol cross-referencing).
//   0x820521A0  "Warning: Can't run action '%s'. Using '%s' as a fallback"
//   0x820521DC  "Warning: Can't run action '%s'"
extern "C" const char kWarn_CmAction_UsingFallback[];  // @ 0x820521A0
extern "C" const char kWarn_CmAction_CantRun[];        // @ 0x820521DC

/**
 * cmAction_RunWithFallback @ 0x82260F90 | size: 0x90
 *
 * Shared action-dispatch method installed at vtable slot 14 (0x38) of many
 * cmAction-derived samplers (cmSampleCamActions::*, etc.). Referenced from
 * vtables 0x82037874 and 0x820378BC (and dozens more via the same pattern).
 *
 * Pattern:
 *   1. vtable[11]  (offset 0x2C) — primary setup / prepare-ports hook.
 *   2. vtable[15]  (offset 0x3C) — primary evaluate (returns bool).
 *   3. If the primary returned true, we're done.
 *   4. Otherwise, if m_pDelegateNode (+0x5C) is set: flip the delegation flag
 *      (+0x58 = 1) so subsequent vfn dispatch reroutes through the fallback
 *      node (see the trampolines at 0x82261030 / 0x82261058 / 0x82261088), and
 *      log a "using fallback" warning with both action names.
 *   5. Otherwise, log a plain "can't run action" warning.
 *
 * In the retail build rage_DebugLog is NOPed so the warnings are suppressed;
 * the side-effect of setting m_delegateFlag is still observable.
 */
extern "C" void cmAction_RunWithFallback(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;  // base cmAction layout
    void** vtable = *(void***)self;

    typedef void (*SetupFn)(void*);
    typedef unsigned char (*EvaluateFn)(void*);

    // vtable[11] — prepare / setup
    ((SetupFn)vtable[11])(self);

    // vtable[15] — evaluate (returns bool)
    unsigned char ok = ((EvaluateFn)vtable[15])(self);
    if (ok != 0) {
        return;
    }

    // Primary failed: try fallback delegate if present.
    void* delegate = obj->m_pDelegateNode;            // +0x5C
    if (delegate != nullptr) {
        // +0x58 = delegation active flag
        obj->m_delegateFlag = 1;
        const char* delegateName = *(const char**)((char*)delegate + 4);
        rage_DebugLog(kWarn_CmAction_UsingFallback,
                      obj->m_actionName, delegateName);
        return;
    }

    rage_DebugLog(kWarn_CmAction_CantRun, obj->m_actionName);
}

// ─────────────────────────────────────────────────────────────────────────────
// Delegation trampolines (inline dispatch stubs that share cmAction_RunWithFallback's
// flag convention). When m_delegateFlag (+0x58) is set, forward to the delegate
// node's vtable slot; otherwise fall through to the base behavior.
//
// These are raw tail-call trampolines in the binary. Reproduced here as small
// C wrappers for documentation — the real hooks would need assembly tail calls
// to match byte-for-byte.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmAction_DispatchSlot4 @ 0x82261030 | size: 0x28
 *
 * If m_delegateFlag set, forward to delegate->vtable[4]; else tail-call to
 * cmSimple_vfn_4 (0x822772A0).
 */
extern "C" void cmAction_DispatchSlot4(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;
    if (obj->m_delegateFlag != 0) {
        void* delegate = obj->m_pDelegateNode;
        void** dvt = *(void***)delegate;
        typedef void (*Slot4Fn)(void*);
        ((Slot4Fn)dvt[4])(delegate);
        return;
    }
    float tmp;
    cmSimple_vfn_4(self, &tmp);
}

/**
 * cmAction_DispatchSlot13 @ 0x82261058 | size: 0x30
 *
 * If m_delegateFlag set, forward to delegate->vtable[13]; else forward to
 * self->vtable[13] (i.e. call the virtual on self).
 */
extern "C" void cmAction_DispatchSlot13(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;
    typedef void (*Slot13Fn)(void*);
    if (obj->m_delegateFlag != 0) {
        void* delegate = obj->m_pDelegateNode;
        void** dvt = *(void***)delegate;
        ((Slot13Fn)dvt[13])(delegate);
        return;
    }
    void** vt = *(void***)self;
    ((Slot13Fn)vt[13])(self);
}

/**
 * cmAction_DispatchSlot6 @ 0x82261088 | size: 0x30
 *
 * If m_delegateFlag set, forward to delegate->vtable[6]; else forward to
 * self->vtable[14] (slot 0x38 — the "run with fallback" method itself but
 * routed through slot 14's own override in subclasses).
 */
extern "C" void cmAction_DispatchSlot6(void* self) {
    auto* obj = (cmSampleCamActions_P_VelocityFollow*)self;
    typedef void (*Slot6Fn)(void*);
    if (obj->m_delegateFlag != 0) {
        void* delegate = obj->m_pDelegateNode;
        void** dvt = *(void***)delegate;
        ((Slot6Fn)dvt[6])(delegate);
        return;
    }
    void** vt = *(void***)self;
    ((Slot6Fn)vt[14])(self);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::H_SmoothLookAt_LimitedTilt::vfn_14 — smooth look-at update
// ═══════════════════════════════════════════════════════════════════════════════

// Additional externs used by vfn_14.

extern "C" {

// cmSampleCamActions_9430_g @ 0x82179430 | size: 0x238
// Given target position (r4), camera position (r3), and a roll vec (r7),
// computes rotation deltas (pitch, yaw, roll) into the vec3 at r5.
void cmSampleCamActions_9430_g(float* outCamPos, float* outEulers,
                                float* outDelta, void* rollTable, int mode);

// cmSampleCamActions_6E48_g @ 0x82276E48 | size: 0x70
// Reads a float from a camera output slot: r3=outputNode, r4=slotIndex,
// r5=actionNode. Returns value in f1.
float cmSampleCamActions_6E48_g(void* outputNode, int slotIndex,
                                 void* actionNode);

} // extern "C"

// Static constants referenced from .rdata by vfn_14.
//   lbl_8202D110 = 0.0f                  (+0x8217B2CC)
//   lbl_8202C02C = 360.0f or 2*PI        (wrap range constant — tilt full rotation)
//   lbl_8203708C + 0x14 = "Warning: target == camera in h_smoothlookat"  @0x820375C0 (actually 0x820375C0)
//   0x82028084 = 0.01f                   (small Y-offset to break target==camera)

static constexpr float kSmoothLookAt_Zero      = 0.0f;      // lbl_8202D110
static constexpr float kSmoothLookAt_WrapRange = 360.0f;    // lbl_8202C02C  (tilt wrap period, deg)
static constexpr float kSmoothLookAt_NudgeY    = 0.01f;     // @0x82028084 — used when target == camera
static constexpr const char* kWarn_TargetEqCam = "Warning: target == camera in h_smoothlookat";  // @0x820375C0

/**
 * cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_14 @ 0x8217B1D8 | size: 0x378
 *
 * Smooth look-at camera parameter sampler. Each frame, this reads:
 *   port 0: smoothing alpha input (clamped to >= 0, then alpha = max(0, a) - 1.0f
 *           selected through fsel — produces a damping factor).
 *   port 1: target position (vec4)
 *   port 2: desired tilt value (f31)
 *   port 3: tilt smoothing alpha (f29)
 *   port 4: tilt low limit (f28)
 *   port 5: tilt high limit (f30)
 *
 * Then the current camera position is read via cmAction_ReadRotation. If the
 * target and camera positions are bitwise identical (ignoring the sign bit),
 * the function emits the debug warning
 *   "Warning: target == camera in h_smoothlookat"
 * and nudges the target by (0, 0, 0.01) to avoid a zero-length direction
 * vector / NaN in the subsequent look-at computation.
 *
 * cmSampleCamActions_9430_g then computes the pitch/yaw/roll euler deltas
 * required to look from the camera position toward the target. The yaw
 * component is smoothed toward the current slot-13 yaw (read back via
 * cmSampleCamActions_6E48_g), with the desired tilt f31 clamped to
 * [f28, f30] whenever the smoothed yaw leaves that range.
 *
 * Each of the 3 euler components is wrapped through WrapValueInRange into
 * [0, 360) for both current and target, then if one crossed the wrap seam
 * the other is offset by ±360 so the subsequent lerp takes the short path.
 *
 * Finally, output = lerp(current, target, alpha) is written back to the
 * camera output via GetSlot() vcalls for slots 1, 3, and 5, setting dirty
 * bits 0x1 | 0x2 | 0x10 | 0x8 | 0x20 (pitch / yaw / roll group).
 */
extern "C" void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_14(
        void* selfNode, void* outputCtx) {
    auto*         self    = (cmActionNode*)selfNode;
    cmCamOutput*  output  = ((cmCameraAction*)outputCtx)->m_pCamOutput;

    // --- Read smoothing alpha from port 0.
    // Pattern: fsel(val, val, 0)  = max(val, 0)        [only when val != NaN]
    //          then subtract 1.0; fsel(diff, 1.0, max) picks the second arg
    //          when diff>=0 — i.e. alpha = (val >= 1.0) ? 1.0 : max(val, 0).
    float alphaIn = cmAction_ReadPortFloat(self, 0);
    float alphaClampedLo = (alphaIn >= 0.0f) ? alphaIn : 0.0f;
    float alpha = (alphaClampedLo - 1.0f >= 0.0f) ? 1.0f : alphaClampedLo;

    // --- Read target position from port 1.
    float targetPos[4];
    cmAction_ReadPortVec4(targetPos, self, 1);

    // --- Read tilt-limit parameters from ports 2-5.
    float tiltDesired  = cmAction_ReadPortFloat(self, 2);   // f31
    float tiltSmoothA  = cmAction_ReadPortFloat(self, 3);   // f29
    float tiltLow      = cmAction_ReadPortFloat(self, 4);   // f28
    float tiltHigh     = cmAction_ReadPortFloat(self, 5);   // f30

    // --- Read current camera position/rotation.
    // cmAction_ReadRotation fills a vec4 at +128 on the PPC stack. For our
    // purposes this is a 3D position used for the look-at source.
    float camPos[4];
    cmAction_ReadRotation(camPos, self);

    // --- Detect target == camera by comparing the three position lanes with
    // the sign bit masked out (vand with 0x7FFFFFFF SIMD mask, then vcmpeqfp).
    bool targetEqualsCamera = true;
    for (int i = 0; i < 3; ++i) {
        // Mask out sign bit on both sides.
        union { float f; uint32_t u; } a{targetPos[i]}, b{camPos[i]};
        if ((a.u & 0x7FFFFFFFu) != (b.u & 0x7FFFFFFFu)) {
            targetEqualsCamera = false;
            break;
        }
    }

    if (targetEqualsCamera) {
        rage_DebugLog(kWarn_TargetEqCam);
        // Nudge target by (0, 0, 0.01) (lane 3 gets 0.0; lane 2 gets the
        // small offset — matches the stfs f27,112(r1); stfs f27,116(r1);
        // stfs f0,120(r1) sequence with f0 = 0.01f).
        targetPos[0] += 0.0f;
        targetPos[1] += 0.0f;
        targetPos[2] += kSmoothLookAt_NudgeY;
        // targetPos[3] gets the v127 w-lane sum; harmless since we only use xyz.
    }

    // --- Compute look-at eulers (pitch/yaw/roll deltas) into localEulers.
    // cmSampleCamActions_9430_g writes a 3-float delta set into outEulers
    // (at +80 on the original stack frame).
    float localEulers[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float tmpVec[4]      = {0.0f, 0.0f, 0.0f, 0.0f};
    cmSampleCamActions_9430_g(camPos, targetPos, localEulers, /*rollTable*/ nullptr, 0);

    // --- Smooth yaw toward current slot-13 reading and clamp tilt.
    float currentYaw = cmSampleCamActions_6E48_g(output, 13, self);
    localEulers[1] -= currentYaw;                  // stfs f10,84(r1)

    float selfFieldYaw = *(float*)((uint8_t*)self + 96);   // +0x60 yaw state
    float selfFieldAcc = *(float*)((uint8_t*)self + 100);  // +0x64 smooth acc

    // Clamp desired tilt to [tiltLow, tiltHigh] if current drifts out of band.
    if ((selfFieldYaw > tiltHigh && tiltDesired > kSmoothLookAt_Zero) ||
        (selfFieldYaw < tiltLow  && tiltDesired < kSmoothLookAt_Zero)) {
        tiltDesired = kSmoothLookAt_Zero;
    }

    // Integrate tilt smoothing:
    //   acc += (tiltDesired - acc) * tiltSmoothA
    //   yaw += acc
    float tiltDelta = tiltDesired - selfFieldAcc;
    float newAcc    = selfFieldAcc + tiltDelta * tiltSmoothA;
    *(float*)((uint8_t*)self + 100) = newAcc;

    float newYaw = selfFieldYaw + newAcc;
    *(float*)((uint8_t*)self + 96)  = newYaw;

    // Fold tilt into pitch lane (stack+80) and push into stack+112 buffer.
    float pitchCorr = newYaw + localEulers[0];   // +80
    localEulers[0]  = pitchCorr;
    // pitch is stored in tmpVec[0] — read from slot 3 and slot 5 to smooth.
    tmpVec[0] = pitchCorr;
    tmpVec[1] = cmSampleCamActions_6E48_g(output, 3, self);
    tmpVec[2] = cmSampleCamActions_6E48_g(output, 5, self);

    // --- For each of the 3 euler components, wrap both tmpVec (current) and
    // localEulers (target) into [0, 360). If they are on opposite sides of
    // the wrap seam, shift one by ±360 so the lerp below takes the short path.
    for (int i = 0; i < 3; ++i) {
        tmpVec[i]      = WrapValueInRange(tmpVec[i],      kSmoothLookAt_Zero, kSmoothLookAt_WrapRange);
        localEulers[i] = WrapValueInRange(localEulers[i], kSmoothLookAt_Zero, kSmoothLookAt_WrapRange);

        float cur = tmpVec[i];
        float tgt = localEulers[i];
        if (cur < kSmoothLookAt_Zero) {
            // cur sign-negative in original mask; replicate shortest-path fix.
            if (tgt > cur + kSmoothLookAt_Zero) {
                localEulers[i] = tgt - kSmoothLookAt_WrapRange;
            }
        } else if (cur > kSmoothLookAt_Zero) {
            float diff = cur - kSmoothLookAt_Zero;
            if (localEulers[i] < diff) {
                localEulers[i] = localEulers[i] + kSmoothLookAt_WrapRange;
            }
        }
    }

    // --- Lerp: out = current + (target - current) * alpha_vec(w-lane broadcast)
    // The original broadcasts the w-lane of (alpha, alpha, alpha, alpha)
    // from a +144 stack buffer where only +144 received `alpha`. Every lane
    // therefore gets the scalar alpha.
    float lerped[4];
    for (int i = 0; i < 3; ++i) {
        lerped[i] = tmpVec[i] + (localEulers[i] - tmpVec[i]) * alpha;
    }
    lerped[3] = 0.0f;

    // --- Write lerped[0..2] to slots 1, 3, 5 via GetSlot() vcalls, OR in
    // dirty bits 0x1 | 0x2 | 0x10 | 0x8 | 0x20.
    void*  cmNode     = output->m_pNode;
    void** cmVtable   = *(void***)cmNode;
    // Two back-to-back writes to the flags word BEFORE slot 1 vcall:
    //   flags |= 0x1;
    //   flags |= 0x2;
    output->m_dirtyFlags |= 0x1;
    output->m_dirtyFlags |= 0x2;

    // Slot 1 (pitch).
    {
        float* slotPtr = (float*)((GetSlotFn)cmVtable[1])(cmNode, 1);
        *slotPtr = lerped[0];
        output->m_dirtyFlags |= 0x10;
    }
    // Slot 3 (yaw).
    {
        // Vtable may have been reloaded; refetch for safety (matches
        // "lwz r3,0(r31); bctrl" pattern where r31 was reloaded).
        cmVtable  = *(void***)cmNode;
        float* slotPtr = (float*)((GetSlotFn)cmVtable[1])(cmNode, 3);
        *slotPtr = lerped[1];
        output->m_dirtyFlags |= 0x8;
    }
    // Slot 5 (roll).
    {
        cmVtable  = *(void***)cmNode;
        float* slotPtr = (float*)((GetSlotFn)cmVtable[1])(cmNode, 5);
        *slotPtr = lerped[2];
        output->m_dirtyFlags |= 0x20;
    }
}
