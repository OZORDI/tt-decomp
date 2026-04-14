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
