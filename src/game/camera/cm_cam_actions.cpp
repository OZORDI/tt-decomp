/**
 * cm_cam_actions.cpp — cmSampleCamActions camera parameter sampler functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The cmSampleCamActions system is a set of CM node action functions for camera
 * sampling. They read camera parameters (position, rotation, FOV, etc.) from
 * the connection machine graph ports and write them into camera output structures.
 *
 * Camera action context layout:
 *   +0x00  vtable / action ptr
 *   +0x04  m_pCamOutput — pointer to camera output structure
 *
 * Camera output structure:
 *   +0x00  m_pNode      — pointer to the owning CM node
 *   +0x04  m_dirtyFlags — bitmask of which parameters have been written
 *
 * Camera output slot mapping (dirty flag bit = 1 << slot):
 *   Slots 0, 1     (0x0003) = rotation (pitch + yaw)
 *   Slot  2        (0x0004) = FOV
 *   Slots 3, 4, 5  (0x0038) = look-at target XYZ
 *   Slots 6–9      (0x03C0) = orientation (vec3 + angle)
 *   Slots 10–13    (0x3C00) = secondary orientation (vec3 + angle)
 *   Slots 14, 15   (0xC000) = orientation vec4 pair
 */

#include <cstdint>
#include <cstring>
#include <cmath>

// ── External declarations for CM helper functions ───────────────────────────

extern "C" {

// cmAction_ReadPortFloat @ 0x822610B8 | size: 0x8C
// Reads a scalar float from port[portIndex] of the action node.
float cmAction_ReadPortFloat(void* node, int portIndex);

// cmAction_ReadPortVec4 @ 0x82261148 | size: 0x88
// Reads a vec4 from port[portIndex] into the output buffer.
float* cmAction_ReadPortVec4(float* outVec, void* node, int portIndex);

// cmCamOutput_WriteSlot @ 0x82275CC8 | size: 0x6C
// Writes a float value into the camera output at slotIndex and sets dirty bit.
void cmCamOutput_WriteSlot(void* camOutput, int slotIndex, void* actionNode,
                            float value);

// cmAction_ReadRotation @ 0x82183220 | size: 0x154
// Reads camera rotation/orientation data from the action node.
void cmAction_ReadRotation(float* outVec, void* actionNode);

// cmReporter_Reset @ 0x822752F0 | size: 0x90
// Resets a cmReporter value to zero based on its output type (switch on type 0-5).
void cmReporter_Reset(void* reporter);

// cmAction_TeardownPorts @ 0x822771E0 | size: 0x9C
// Iterates over CM action ports and tears them down.
void cmAction_TeardownPorts(void* node);

// rage_free @ 0x820C00C0 | size: 0x60
void rage_free(void* ptr);

// cmSimple_vfn_4 @ 0x822772A0 | size: 0xA4
void cmSimple_vfn_4(void* node, float* out);

// cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10 @ 0x822774B8 | size: 0x90
void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(void* node, void* data);

// fmod_safe @ 0x82431D78 | size: 0x128
// Safe floating-point modulo with NaN/infinity handling.
float fmod_safe(float value, float range);

// rage_DebugLog @ 0x8240E6D0 | size: 0x4 (nop in retail build)
void rage_DebugLog(const char* fmt, ...);

} // extern "C"

// ── Extern data ─────────────────────────────────────────────────────────────

extern "C" void* cmAction_vtable;  // rage::cmAction vtable @ 0x82052224 (.rdata)

// ═══════════════════════════════════════════════════════════════════════════════
//  GLOBAL HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * TransformPoint3x4 @ 0x820E86A8 | size: 0x94
 *
 * Transforms a 3D point by a 4x3 affine matrix.
 * out[i] = mat_row_i . pt + translation_i
 *
 * Matrix layout at r5 (float[16]):
 *   col0=[0,1,2], col1=[4,5,6], col2=[8,9,10], translation=[12,13,14]
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
 * If min == max, returns min. If out of range, wraps using fmod.
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
 * Samples 2 floats from ports 0 and 1. Checks port status; if all connected
 * (status & 7 == 7), sets dirty flag bit 0x4 (FOV). Writes to cam output
 * slots 0 (pitch) and 1 (yaw).
 */
extern "C" void cmSampleCamActions_HeadingAndFOV(void* actionCtx, void* actionNode) {
    float pitch = cmAction_ReadPortFloat(actionNode, 0);
    float yaw = cmAction_ReadPortFloat(actionNode, 1);

    uint32_t status = *(uint32_t*)((char*)actionNode + 16);
    if ((status & 0x7) == 7) {
        void* camOutput = *(void**)((char*)actionCtx + 4);
        uint32_t* dirtyFlags = (uint32_t*)((char*)camOutput + 4);
        *dirtyFlags |= 0x4;
    }

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 0, actionNode, pitch);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 1, actionNode, yaw);
}

/**
 * cmSampleCamActions_Rotation @ 0x821798A8 | size: 0x70
 *
 * Samples 1 float from port 0. Checks port status; if all connected,
 * sets dirty flag bits 0x3 (pitch + yaw). Writes the sampled value to
 * cam output slot 2 (FOV).
 */
extern "C" void cmSampleCamActions_Rotation(void* actionCtx, void* actionNode) {
    float rotationValue = cmAction_ReadPortFloat(actionNode, 0);

    uint32_t status = *(uint32_t*)((char*)actionNode + 16);
    if ((status & 0x7) == 7) {
        void* camOutput = *(void**)((char*)actionCtx + 4);
        uint32_t* dirtyFlags = (uint32_t*)((char*)camOutput + 4);
        *dirtyFlags |= 0x3;
    }

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 2, actionNode, rotationValue);
}

/**
 * cmSampleCamActions_BaseXYZ @ 0x82179918 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2.
 * Writes them to cam output slots 0 (pitch), 1 (yaw), 2 (FOV).
 */
extern "C" void cmSampleCamActions_BaseXYZ(void* actionCtx, void* actionNode) {
    float val0 = cmAction_ReadPortFloat(actionNode, 0);
    float val1 = cmAction_ReadPortFloat(actionNode, 1);
    float val2 = cmAction_ReadPortFloat(actionNode, 2);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 0, actionNode, val0);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 1, actionNode, val1);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 2, actionNode, val2);
}

/**
 * cmSampleCamActions_FOV @ 0x821799D0 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2. Writes to cam output slots 5, 4, 3
 * (look-at target, reversed slot order).
 */
extern "C" void cmSampleCamActions_FOV(void* actionCtx, void* actionNode) {
    float val0 = cmAction_ReadPortFloat(actionNode, 0);
    float val1 = cmAction_ReadPortFloat(actionNode, 1);
    float val2 = cmAction_ReadPortFloat(actionNode, 2);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 5, actionNode, val0);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 4, actionNode, val1);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 3, actionNode, val2);
}

/**
 * cmSampleCamActions_Orientation @ 0x82179A88 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1.
 * Writes vec4 x/y/z to cam output slots 6/7/8 and the float to slot 9
 * (orientation vec3 + angle).
 */
extern "C" void cmSampleCamActions_Orientation(void* actionCtx, void* actionNode) {
    float orientVec[4];
    cmAction_ReadPortVec4(orientVec, actionNode, 0);
    float orientAngle = cmAction_ReadPortFloat(actionNode, 1);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 6, actionNode, orientVec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 7, actionNode, orientVec[1]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 8, actionNode, orientVec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 9, actionNode, orientAngle);
}

/**
 * cmSampleCamActions_LookAtTarget @ 0x82179B30 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1.
 * Writes vec4 x/y/z to cam output slots 10/11/12 and the float to slot 13
 * (secondary orientation vec3 + angle).
 */
extern "C" void cmSampleCamActions_LookAtTarget(void* actionCtx, void* actionNode) {
    float targetVec[4];
    cmAction_ReadPortVec4(targetVec, actionNode, 0);
    float targetAngle = cmAction_ReadPortFloat(actionNode, 1);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 10, actionNode, targetVec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 11, actionNode, targetVec[1]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 12, actionNode, targetVec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 13, actionNode, targetAngle);
}

/**
 * cmSampleCamActions_OrientationPair @ 0x82179740 | size: 0xB8
 *
 * Samples two vec4 values from ports 0 and 1.
 * Writes them to cam output slots 14 and 15 via vcall.
 * Sets dirty flag bits 0x4000 and 0x8000 (orientation vec4 pair).
 */
extern "C" void cmSampleCamActions_OrientationPair(void* actionCtx, void* actionNode) {
    float orientVec0[4], orientVec1[4];
    cmAction_ReadPortVec4(orientVec0, actionNode, 0);
    cmAction_ReadPortVec4(orientVec1, actionNode, 1);

    // Get cam output, write orientVec0 to slot 14 via vcall slot 1
    void* camOutput = *(void**)((char*)actionCtx + 4);
    void* cmNode = *(void**)((char*)camOutput + 0);
    void** vtable = *(void***)cmNode;
    typedef void* (*GetSlotFn)(void*, int);
    void* slot14ptr = ((GetSlotFn)vtable[1])(cmNode, 14);
    memcpy(slot14ptr, orientVec0, 16);
    uint32_t* dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x4000;

    // Write orientVec1 to slot 15
    camOutput = *(void**)((char*)actionCtx + 4);
    cmNode = *(void**)((char*)camOutput + 0);
    vtable = *(void***)cmNode;
    void* slot15ptr = ((GetSlotFn)vtable[1])(cmNode, 15);
    memcpy(slot15ptr, orientVec1, 16);
    dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x8000;
}

/**
 * cmSampleCamActions_ZeroTiltRoll @ 0x8217BDE0 | size: 0x8C
 *
 * Writes 0.0f to cam output slots 6 and 8 (orientation X and Z).
 * Sets dirty flag bits 0x80 (slot 7) and 0x200 (slot 9) to mark the
 * full orientation group as dirty.
 */
extern "C" void cmSampleCamActions_ZeroTiltRoll(void* actionCtx, void* actionNode) {
    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 6, actionNode, 0.0f);

    camOutput = *(void**)((char*)actionCtx + 4);
    uint32_t* dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x80;

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 8, actionNode, 0.0f);

    camOutput = *(void**)((char*)actionCtx + 4);
    dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x200;
}

/**
 * cmSampleCamActions_RotationXZ @ 0x8217BE70 | size: 0x88
 *
 * Reads camera rotation via cmAction_ReadRotation, then writes
 * x-component to slot 10 and z-component to slot 12.
 * Sets dirty flag bits 0x800 (slot 11) and 0x2000 (slot 13) to mark
 * the secondary orientation group as dirty.
 */
extern "C" void cmSampleCamActions_RotationXZ(void* actionCtx, void* actionNode) {
    float rotVec[4];
    cmAction_ReadRotation(rotVec, actionNode);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    uint32_t* dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x800;

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 10, actionNode, rotVec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmCamOutput_WriteSlot(camOutput, 12, actionNode, rotVec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    dirtyFlags = (uint32_t*)((char*)camOutput + 4);
    *dirtyFlags |= 0x2000;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::H_SmoothLookAt_LimitedTilt VTABLE METHODS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_13 @ 0x8217B550 | size: 0x14
 *
 * Resets tilt limit parameters to 0.0f at offsets +96 and +100.
 */
extern "C" void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_13(void* self) {
    *(float*)((char*)self + 96) = 0.0f;
    *(float*)((char*)self + 100) = 0.0f;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::P_VelocityFollow VTABLE METHODS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_0 @ 0x821793D0 | size: 0x5C
 *
 * Destructor. Sets vtable to rage::cmAction base, tears down ports,
 * optionally frees memory.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_0(void* self, int flags) {
    *(void**)self = &cmAction_vtable;
    cmAction_TeardownPorts(self);
    if (flags & 1) {
        rage_free(self);
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_3 @ 0x82260F40 | size: 0x4C
 *
 * Init/reset. Clears field +84, resets reporter if present, calls vfn_12.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_3(void* self) {
    *(uint32_t*)((char*)self + 84) = 0;

    void* reporter = *(void**)((char*)self + 28);
    if (reporter != nullptr) {
        cmReporter_Reset(reporter);
    }

    void** vtable = *(void***)self;
    typedef void (*VFn12)(void*);
    ((VFn12)vtable[12])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_2 @ 0x82260F90 | size: 0xA0
 *
 * Update. Calls vfn_11 (evaluate), then vfn_15 (validate). If validation
 * fails and a delegation node exists (+92), sets delegation flag and logs
 * debug message.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_2(void* self) {
    void** vtable = *(void***)self;
    typedef void (*VFn11)(void*);
    ((VFn11)vtable[11])(self);

    vtable = *(void***)self;
    typedef uint8_t (*VFn15)(void*);
    uint8_t isValid = ((VFn15)vtable[15])(self);

    if ((isValid & 0xFF) == 0) {
        void* delegateNode = *(void**)((char*)self + 92);
        void* actionName = *(void**)((char*)self + 4);
        if (delegateNode != nullptr) {
            *(uint8_t*)((char*)self + 88) = 1;
            uint32_t delegateName = *(uint32_t*)((char*)delegateNode + 4);
            rage_DebugLog("%s", actionName, delegateName);
        } else {
            rage_DebugLog("%s", actionName);
        }
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_4 @ 0x82261030 | size: 0x24
 *
 * Delegated GetFloat. If delegation flag (+88) is set, delegates to the
 * inner node (+92) vtable[4]. Otherwise falls through to cmSimple_vfn_4.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_4(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* delegateNode = *(void**)((char*)self + 92);
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
 * Delegated GetDim. If delegation flag (+88) is set, delegates to the
 * inner node (+92) vtable[13]. Otherwise calls own vtable[13].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_5(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* delegateNode = *(void**)((char*)self + 92);
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
 * Delegated SyncPorts. If delegation flag (+88) is set, delegates to the
 * inner node (+92) vtable[6]. Otherwise calls own vtable[14].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_6(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* delegateNode = *(void**)((char*)self + 92);
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
 * Position update. Calls parent H_SmoothLookAt_LimitedTilt vfn_10 for base
 * position processing, then subtracts the input position from the velocity
 * vector at offset +112, computing the relative velocity.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_10(void* self, void* posData) {
    cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(self, posData);

    float* velocity = (float*)((char*)self + 112);
    const float* inputPos = (const float*)posData;
    velocity[0] -= inputPos[0];
    velocity[1] -= inputPos[1];
    velocity[2] -= inputPos[2];
    velocity[3] -= inputPos[3];
}
