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
 */

#include <cstdint>
#include <cstring>
#include <cmath>

// ── External declarations for CM helper functions ───────────────────────────

extern "C" {

// cmSampleCamActions_10B8_g @ 0x822610B8 | size: 0x8C
// Reads a scalar float from port[portIndex] of the action node.
float cmSampleCamActions_10B8_g(void* node, int portIndex);

// cmSampleCamActions_1148_g @ 0x82261148 | size: 0x88
// Reads a vec4 from port[portIndex] into the output buffer.
float* cmSampleCamActions_1148_g(float* outVec, void* node, int portIndex);

// cmSampleCamActions_5CC8_g @ 0x82275CC8 | size: 0x6C
// Writes a float value into the camera output at slotIndex.
void cmSampleCamActions_5CC8_g(void* camOutput, int slotIndex, void* actionNode,
                                float value);

// cmSampleCamActions_3220_g @ 0x82183220 | size: 0x154
// Reads camera rotation/orientation data from the action node.
void cmSampleCamActions_3220_g(float* outVec, void* actionNode);

// cmReporterCtor_52F0_g @ 0x822752F0 | size: 0x90
void cmReporterCtor_52F0_g(void* reporter);

// rage_71E0 @ 0x822771E0 | size: 0x9C
void rage_71E0(void* node);

// rage_free_00C0 @ 0x820C00C0 | size: 0x60
void rage_free_00C0(void* ptr);

// cmSimple_vfn_4 @ 0x822772A0 | size: 0xA4
void cmSimple_vfn_4(void* node, float* out);

// cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10 @ 0x822774B8 | size: 0x90
void cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(void* node, void* data);

// util_1D78 @ 0x82431D78 | size: 0x128
float util_1D78(float value, float range);

// nop_8240E6D0 @ 0x8240E6D0 | size: 0x4
void nop_8240E6D0(const char* fmt, ...);

} // extern "C"

// ── Extern data ─────────────────────────────────────────────────────────────

extern "C" const float lbl_8202D110;  // 0.0f constant (.rdata)
extern "C" void* lbl_82052224;        // rage::cmAction vtable

// ═══════════════════════════════════════════════════════════════════════════════
//  GLOBAL HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_86A8_g @ 0x820E86A8 | size: 0x94
 *
 * Transforms a 3D point by a 4x3 affine matrix.
 * out[i] = mat_row_i . pt + translation_i
 *
 * Matrix layout at r5 (float[16]):
 *   col0=[0,1,2], col1=[4,5,6], col2=[8,9,10], translation=[12,13,14]
 */
extern "C" void cmSampleCamActions_86A8_g(float* out, const float* pt, const float* mat) {
    float x = pt[0], y = pt[1], z = pt[2];

    out[0] = mat[4] * y + mat[8] * z + x * mat[0] + mat[12];
    out[1] = mat[9] * z + mat[1] * x + mat[5] * y + mat[13];
    out[2] = mat[10] * z + mat[2] * x + mat[6] * y + mat[14];
}

/**
 * cmSampleCamActions_29D0_g @ 0x821129D0 | size: 0x88
 *
 * Wraps a value into the range [min, max) with circular wrapping.
 * If min == max, returns min. If out of range, wraps using fmod.
 */
extern "C" float cmSampleCamActions_29D0_g(float value, float min, float max) {
    if (max < min) {
        return value;
    }
    if (max == min) {
        return min;
    }
    if (value < min) {
        float range = max - min;
        float dist = min - value;
        float wrapped = util_1D78(dist, range);
        return max - wrapped;
    }
    if (value <= max) {
        return value;
    }
    float range = max - min;
    float dist = value - max;
    float wrapped = util_1D78(dist, range);
    return wrapped + min;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  CAMERA ACTION SAMPLER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_97F8_g @ 0x821797F8 | size: 0xAC
 *
 * Samples 2 floats from ports 0 and 1. Checks port status; if all connected
 * (status & 7 == 7), sets dirty flag bit 0x4. Writes to cam output slots 0, 1.
 */
extern "C" void cmSampleCamActions_97F8_g(void* actionCtx, void* actionNode) {
    float val0 = cmSampleCamActions_10B8_g(actionNode, 0);
    float val1 = cmSampleCamActions_10B8_g(actionNode, 1);

    uint32_t status = *(uint32_t*)((char*)actionNode + 16);
    if ((status & 0x7) == 7) {
        void* camOutput = *(void**)((char*)actionCtx + 4);
        uint32_t* flags = (uint32_t*)((char*)camOutput + 4);
        *flags |= 0x4;
    }

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 0, actionNode, val0);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 1, actionNode, val1);
}

/**
 * cmSampleCamActions_98A8_g @ 0x821798A8 | size: 0x70
 *
 * Samples 1 float from port 0. Checks port status; if all connected,
 * sets dirty flag bits 0x3. Writes the sampled value to cam output slot 2.
 */
extern "C" void cmSampleCamActions_98A8_g(void* actionCtx, void* actionNode) {
    float val = cmSampleCamActions_10B8_g(actionNode, 0);

    uint32_t status = *(uint32_t*)((char*)actionNode + 16);
    if ((status & 0x7) == 7) {
        void* camOutput = *(void**)((char*)actionCtx + 4);
        uint32_t* flags = (uint32_t*)((char*)camOutput + 4);
        *flags |= 0x3;
    }

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 2, actionNode, val);
}

/**
 * cmSampleCamActions_9918_g @ 0x82179918 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2. Writes them to cam output slots 0, 1, 2.
 */
extern "C" void cmSampleCamActions_9918_g(void* actionCtx, void* actionNode) {
    float val0 = cmSampleCamActions_10B8_g(actionNode, 0);
    float val1 = cmSampleCamActions_10B8_g(actionNode, 1);
    float val2 = cmSampleCamActions_10B8_g(actionNode, 2);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 0, actionNode, val0);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 1, actionNode, val1);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 2, actionNode, val2);
}

/**
 * cmSampleCamActions_99D0_g @ 0x821799D0 | size: 0xB8
 *
 * Samples 3 floats from ports 0, 1, 2. Writes to cam output slots 5, 4, 3
 * (reversed slot order).
 */
extern "C" void cmSampleCamActions_99D0_g(void* actionCtx, void* actionNode) {
    float val0 = cmSampleCamActions_10B8_g(actionNode, 0);
    float val1 = cmSampleCamActions_10B8_g(actionNode, 1);
    float val2 = cmSampleCamActions_10B8_g(actionNode, 2);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 5, actionNode, val0);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 4, actionNode, val1);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 3, actionNode, val2);
}

/**
 * cmSampleCamActions_9A88_g @ 0x82179A88 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1.
 * Writes vec4 x/y/z to cam output slots 6/7/8 and the float to slot 9.
 */
extern "C" void cmSampleCamActions_9A88_g(void* actionCtx, void* actionNode) {
    float vec[4];
    cmSampleCamActions_1148_g(vec, actionNode, 0);
    float fval = cmSampleCamActions_10B8_g(actionNode, 1);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 6, actionNode, vec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 7, actionNode, vec[1]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 8, actionNode, vec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 9, actionNode, fval);
}

/**
 * cmSampleCamActions_9B30_g @ 0x82179B30 | size: 0xA8
 *
 * Reads vec4 from port 0 and float from port 1.
 * Writes vec4 x/y/z to cam output slots 10/11/12 and the float to slot 13.
 */
extern "C" void cmSampleCamActions_9B30_g(void* actionCtx, void* actionNode) {
    float vec[4];
    cmSampleCamActions_1148_g(vec, actionNode, 0);
    float fval = cmSampleCamActions_10B8_g(actionNode, 1);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 10, actionNode, vec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 11, actionNode, vec[1]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 12, actionNode, vec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 13, actionNode, fval);
}

/**
 * cmSampleCamActions_9740_fw @ 0x82179740 | size: 0xB8
 *
 * Samples two vec4 values from ports 0 and 1.
 * Writes them to cam output slots 14 and 15 via vcall.
 * Sets dirty flag bits 0x4000 and 0x8000.
 */
extern "C" void cmSampleCamActions_9740_fw(void* actionCtx, void* actionNode) {
    float vec0[4], vec1[4];
    cmSampleCamActions_1148_g(vec0, actionNode, 0);
    cmSampleCamActions_1148_g(vec1, actionNode, 1);

    // Get cam output, write vec0 to slot 14 via vcall slot 1
    void* camOutput = *(void**)((char*)actionCtx + 4);
    void* cmNode = *(void**)((char*)camOutput + 0);
    void** vtable = *(void***)cmNode;
    typedef void* (*GetSlotFn)(void*, int);
    void* slot14ptr = ((GetSlotFn)vtable[1])(cmNode, 14);
    memcpy(slot14ptr, vec0, 16);
    uint32_t* flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x4000;

    // Write vec1 to slot 15
    camOutput = *(void**)((char*)actionCtx + 4);
    cmNode = *(void**)((char*)camOutput + 0);
    vtable = *(void***)cmNode;
    void* slot15ptr = ((GetSlotFn)vtable[1])(cmNode, 15);
    memcpy(slot15ptr, vec1, 16);
    flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x8000;
}

/**
 * cmSampleCamActions_BDE0_fw @ 0x8217BDE0 | size: 0x8C
 *
 * Writes 0.0f to cam output slots 6 and 8.
 * Sets dirty flag bits 0x80 and 0x200.
 */
extern "C" void cmSampleCamActions_BDE0_fw(void* actionCtx, void* actionNode) {
    float zeroVal = lbl_8202D110;  // 0.0f

    void* camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 6, actionNode, zeroVal);

    camOutput = *(void**)((char*)actionCtx + 4);
    uint32_t* flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x80;

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 8, actionNode, zeroVal);

    camOutput = *(void**)((char*)actionCtx + 4);
    flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x200;
}

/**
 * cmSampleCamActions_BE70_g @ 0x8217BE70 | size: 0x88
 *
 * Reads camera rotation via cmSampleCamActions_3220_g, then writes
 * x-component to slot 10 and z-component to slot 12.
 * Sets dirty flag bits 0x800 and 0x2000.
 */
extern "C" void cmSampleCamActions_BE70_g(void* actionCtx, void* actionNode) {
    float rotVec[4];
    cmSampleCamActions_3220_g(rotVec, actionNode);

    void* camOutput = *(void**)((char*)actionCtx + 4);
    uint32_t* flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x800;

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 10, actionNode, rotVec[0]);

    camOutput = *(void**)((char*)actionCtx + 4);
    cmSampleCamActions_5CC8_g(camOutput, 12, actionNode, rotVec[2]);

    camOutput = *(void**)((char*)actionCtx + 4);
    flags = (uint32_t*)((char*)camOutput + 4);
    *flags |= 0x2000;
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
    float zero = lbl_8202D110;
    *(float*)((char*)self + 96) = zero;
    *(float*)((char*)self + 100) = zero;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  cmSampleCamActions::P_VelocityFollow VTABLE METHODS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_0 @ 0x821793D0 | size: 0x5C
 *
 * Destructor. Sets vtable to cmAction base, tears down, optionally frees.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_0(void* self, int flags) {
    *(void**)self = &lbl_82052224;
    rage_71E0(self);
    if (flags & 1) {
        rage_free_00C0(self);
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_3 @ 0x82260F40 | size: 0x4C
 *
 * Init/reset. Clears field +84, inits reporter if present, calls vfn_12.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_3(void* self) {
    *(uint32_t*)((char*)self + 84) = 0;

    void* reporter = *(void**)((char*)self + 28);
    if (reporter != nullptr) {
        cmReporterCtor_52F0_g(reporter);
    }

    void** vtable = *(void***)self;
    typedef void (*VFn12)(void*);
    ((VFn12)vtable[12])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_2 @ 0x82260F90 | size: 0xA0
 *
 * Update. Calls vfn_11, then vfn_15. If vfn_15 returns false and field +92
 * is non-null, sets delegation flag and logs debug message.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_2(void* self) {
    void** vtable = *(void***)self;
    typedef void (*VFn11)(void*);
    ((VFn11)vtable[11])(self);

    vtable = *(void***)self;
    typedef uint8_t (*VFn15)(void*);
    uint8_t result = ((VFn15)vtable[15])(self);

    if ((result & 0xFF) == 0) {
        void* field92 = *(void**)((char*)self + 92);
        void* field4 = *(void**)((char*)self + 4);
        if (field92 != nullptr) {
            *(uint8_t*)((char*)self + 88) = 1;
            uint32_t subField = *(uint32_t*)((char*)field92 + 4);
            nop_8240E6D0("%s", field4, subField);
        } else {
            nop_8240E6D0("%s", field4);
        }
    }
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_4 @ 0x82261030 | size: 0x24
 *
 * Delegated GetFloat. If flag +88 set, delegates to inner node +92 vtable[4].
 * Otherwise falls through to cmSimple_vfn_4.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_4(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* innerNode = *(void**)((char*)self + 92);
        void** vtable = *(void***)innerNode;
        typedef void (*VFn4)(void*);
        ((VFn4)vtable[4])(innerNode);
        return;
    }
    cmSimple_vfn_4(self, nullptr);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_5 @ 0x82261058 | size: 0x30
 *
 * Delegated GetDim. If flag +88 set, delegates to inner node +92 vtable[13].
 * Otherwise calls own vtable[13].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_5(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* innerNode = *(void**)((char*)self + 92);
        void** vtable = *(void***)innerNode;
        typedef void (*VFn13)(void*);
        ((VFn13)vtable[13])(innerNode);
        return;
    }
    void** vtable = *(void***)self;
    typedef void (*VFn13)(void*);
    ((VFn13)vtable[13])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_6 @ 0x82261088 | size: 0x30
 *
 * Delegated SyncPorts. If flag +88 set, delegates to inner node +92 vtable[6].
 * Otherwise calls own vtable[14].
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_6(void* self) {
    if (*(uint8_t*)((char*)self + 88) != 0) {
        void* innerNode = *(void**)((char*)self + 92);
        void** vtable = *(void***)innerNode;
        typedef void (*VFn6)(void*);
        ((VFn6)vtable[6])(innerNode);
        return;
    }
    void** vtable = *(void***)self;
    typedef void (*VFn14)(void*);
    ((VFn14)vtable[14])(self);
}

/**
 * cmSampleCamActions_P_VelocityFollow_vfn_10 @ 0x8217B188 | size: 0x4C
 *
 * Position update. Calls parent vfn_10, then subtracts input position
 * from velocity vector at offset +112.
 */
extern "C" void cmSampleCamActions_P_VelocityFollow_vfn_10(void* self, void* posData) {
    cmSampleCamActions_H_SmoothLookAt_LimitedTilt_vfn_10(self, posData);

    float* velocity = (float*)((char*)self + 112);
    const float* pos = (const float*)posData;
    velocity[0] -= pos[0];
    velocity[1] -= pos[1];
    velocity[2] -= pos[2];
    velocity[3] -= pos[3];
}
