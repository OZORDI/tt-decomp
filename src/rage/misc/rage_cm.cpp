/**
 * rage_cm.cpp — RAGE Curve/Machine Math Node implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The rage::cm* classes form a visual scripting / data-flow graph system used
 * throughout RAGE for animation control, camera logic, and gameplay scripting.
 * Each class is a self-contained compute node. Nodes are wired together via
 * typed input ports; every frame each node evaluates its inputs and produces an
 * output value of a specific dimension type.
 *
 * ── ARCHITECTURE ──────────────────────────────────────────────────────────────
 *
 * Dimension type codes (stored in cmDataObj::m_dim and cmNode::m_outputType):
 *   CM_DIM_INT   = 0   (int32 / switch-count)
 *   CM_DIM_FLOAT = 1   (scalar float)
 *   CM_DIM_VEC4  = 2   (16-byte aligned float[4])
 *   CM_DIM_BOOL  = 3   (uint8_t boolean)
 *   CM_DIM_INT32 = 4   (int32 or counted int)
 *
 * Port connection type codes (cmNodePort::m_type):
 *   CM_PORT_NONE   = 0  unconnected — evaluate returns zero/false/identity
 *   CM_PORT_DIRECT = 1  m_pData → cmDataObj* holding the raw value
 *   CM_PORT_NODE   = 2  m_pData → cmNode* upstream node; evaluate via vcall
 *
 * ── NODE LAYOUTS (two variants) ───────────────────────────────────────────────
 *
 * Unary nodes (cmDifferential, cmAbs, etc.) — uses util_54C8 for connection:
 *   +0x00  vtable
 *   +0x04  m_outputType  (int32, set by TryConnect on success)
 *   +0x08  m_flags       (bit3 = dirty/enabled toggle)
 *   +0x0C  portA.m_pData (void*)
 *   +0x10  portA.m_type  (int32)
 *   +0x14  m_nConnected  (int32, 0 or 1)
 *
 * Binary nodes (cmAdd, cmSubtract, cmIntegrate, etc.) — uses util_5698:
 *   +0x00  vtable
 *   +0x04  m_outputType
 *   +0x08  m_flags
 *   +0x0C  portA.m_pData
 *   +0x10  portA.m_type
 *   +0x14  portB.m_pData
 *   +0x18  portB.m_type
 *   +0x1C  m_nConnected  (int32, 0-2)
 *
 * ── STATEFUL NODES ───────────────────────────────────────────────────────────
 *
 * cmMemory / cmIntegrate / cmDifferential extend the binary layout with result
 * storage buffers managed by the init virtual (vfn_17/Reset):
 *   +0x20  m_pCurrentValue  (void*, 16-byte aligned alloc)
 *   +0x24  m_pPreviousValue (void*, 16-byte aligned alloc)
 *
 * cmApproachOperator extends further (portA=+0x14, portB=+0x1C, result=+0x28).
 *
 * ── VTABLE SLOT CONVENTIONS ──────────────────────────────────────────────────
 *
 * Slot  0: ~Destructor()
 * Slot  1: GetInt32(int32* out)       — writes 32-bit integer output
 * Slot  2: GetVector(vec4*  out)      — writes 16-byte aligned vec4 output
 * Slot  3: GetBool(uint8_t* out)      — writes boolean output
 * Slot  4: GetFloat(float*  out)      — writes single float output
 * Slot  5: GetDim(int32* out)         — writes dimension count / integer
 * Slot  6: Update()                   — advance stateful nodes one step
 * Slot  8: Sync()                     — internal sync for stateful nodes
 * Slot  9: SetInitialInput(port)      — connect an initial-value port
 * Slot 10: Tick(...)                  — per-frame evaluation (approach/integrate)
 * Slot 14: SetValue(port)             — set current value from a port struct
 * Slot 16: RegisterPorts(node)        — try to connect; set m_outputType on match
 * Slot 17: Reset() / ReInit()         — (re)allocate result buffers
 *
 * ── GLOBALS ──────────────────────────────────────────────────────────────────
 *
 * g_cm_frameTime    @ 0x825C4958  — current frame delta-time (seconds, .data)
 * g_cm_frameRate    @ various     — frame rate constant used by Differential
 * g_cm_negateVec    @ 0x825C5938  — {-1.f, -1.f, -1.f, -1.f} for cmNegate
 * g_cm_zeroFloat    @ 0x8202D110  — 0.0f constant (.rdata) for unconnected ports
 */

#include "rage_cm.hpp"
#include <cmath>
#include <cstring>
#include <cstdint>

// ── Forward declarations for cross-node calls ─────────────────────────────────
namespace rage {

// ── GLOBAL DATA REFERENCES ───────────────────────────────────────────────────

// Current frame delta-time (seconds). Updated each game tick.
extern float g_cm_frameTime;    // @ 0x825C4958

// Constant 0.0f for unconnected port default return.
extern const float g_cm_zeroFloat;  // @ 0x8202D110

// {-1.f, -1.f, -1.f, -1.f} used by cmNegate's vector evaluation.
extern float g_cm_negateVec[4]; // @ 0x825C5938
// ── Missing forward declarations ──────────────────────────────────────────────
extern float g_cm_frameRate;       // @ 0x829DAEC8 — per-frame rate constant (Differential)
void*  rage_alloc_aligned(size_t size, size_t align); // RAGE heap aligned alloc
void   rage_free(void* ptr);                          // RAGE heap free
static void cmNode_SetFromPort_Dispatch(void* dst, const cmNodePort* port, int32_t dim);
uint8_t cmNode_GetBool(const cmNodePort* port);


// ── PORT / DATA STRUCTURES ───────────────────────────────────────────────────

/**
 * cmDataObj — typed data buffer stored in a port's m_pData when m_type==DIRECT.
 * The actual payload sits at offset 0 (float, vec4, bool, or int depending on dim).
 * @ various allocations (16-byte aligned)
 */
struct cmDataObj {
    union {
        float    m_float;    // +0x00  when m_dim == CM_DIM_FLOAT
        uint8_t  m_bool;     // +0x00  when m_dim == CM_DIM_BOOL
        int32_t  m_int;      // +0x00  when m_dim == CM_DIM_INT/INT32
        float    m_vec4[4];  // +0x00  when m_dim == CM_DIM_VEC4 (16-byte aligned)
    };
    // bytes 4–15: padding / additional vec4 components
    uint8_t  m_dim;          // +0x10  dimension type code (CM_DIM_*)
    uint8_t  m_pad[3];       // +0x11
};


// Port type codes
static constexpr int CM_PORT_NONE   = 0;
static constexpr int CM_PORT_DIRECT = 1;
static constexpr int CM_PORT_NODE   = 2;

// Dimension codes
static constexpr int CM_DIM_INT   = 0;
static constexpr int CM_DIM_FLOAT = 1;
static constexpr int CM_DIM_VEC4  = 2;
static constexpr int CM_DIM_BOOL  = 3;
static constexpr int CM_DIM_INT32 = 4;

// ── PORT DESCRIPTOR ───────────────────────────────────────────────────────────

/**
 * cmPortDesc — 4-byte descriptor passed to TryConnect / TryConnectUnary.
 * Describes the required input dimensions and expected output dimension.
 *   portA_dim : required dimension for input port A (0 = any/don't check)
 *   portB_dim : required dimension for input port B (0 = unary node)
 *   portC_dim : required dimension for input port C (0 = at most binary)
 *   output_dim: output dimension written to node->m_outputType on match
 */
struct cmPortDesc {
    uint8_t portA_dim;
    uint8_t portB_dim;
    uint8_t portC_dim;
    uint8_t output_dim;
};

// ── BASE NODE ─────────────────────────────────────────────────────────────────






// ── PORT EVALUATION UTILITIES (lifted from util_92D8, util_9350, etc.) ────────

/**
 * cmNode_GetVector @ 0x821792D8
 *
 * Reads a vec4 from a port and writes it into dst (must be 16-byte aligned).
 * - If port.m_type == DIRECT : copies the vec4 directly from port.m_pData
 * - If port.m_type == NODE   : calls upstream_node->GetVector(dst) via vcall slot 2
 * - Otherwise                : zeroes dst
 *
 * Returns dst.
 */
static float* cmNode_GetVector(float* dst, const cmNodePort* port) {
    switch (port->m_type) {
    case CM_PORT_DIRECT: {
        // The data object holds the vec4 at its start; direct copy
        const float* src = reinterpret_cast<const float*>(port->m_pData);
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
        break;
    }
    case CM_PORT_NODE: {
        // Call virtual slot 2 (GetVector) on the upstream node
        struct cmNode { void** vtable; };
        auto* node = reinterpret_cast<cmNode*>(port->m_pData);
        using GetVecFn = void(*)(void*, float*);
        reinterpret_cast<GetVecFn>(node->vtable[2])(node, dst);
        break;
    }
    default:
        // Unconnected — return zero vector
        dst[0] = dst[1] = dst[2] = dst[3] = 0.0f;
        break;
    }
    return dst;
}

/**
 * cmNode_GetFloat @ 0x82179350
 *
 * Reads a scalar float from a port.
 * - If port.m_type == DIRECT : returns *reinterpret_cast<float*>(port.m_pData)
 * - If port.m_type == NODE   : calls upstream_node->GetFloat(&tmpBuf) via vcall slot 4
 * - Otherwise                : returns 0.0f  (g_cm_zeroFloat)
 */
static float cmNode_GetFloat(const cmNodePort* port) {
    switch (port->m_type) {
    case CM_PORT_DIRECT:
        return *reinterpret_cast<const float*>(port->m_pData);
    case CM_PORT_NODE: {
        struct cmNode { void** vtable; };
        auto* node = reinterpret_cast<cmNode*>(port->m_pData);
        float tmp;
        using GetFltFn = void(*)(void*, float*);
        reinterpret_cast<GetFltFn>(node->vtable[4])(node, &tmp);
        return tmp;
    }
    default:
        return g_cm_zeroFloat;  // 0.0f
    }
}

/**
 * cmNode_GetInt @ 0x82184BD8  (util_4BD8)
 *
 * Reads a 32-bit integer from a port.
 * - DIRECT: returns *(int32_t*)port.m_pData
 * - NODE  : calls vcall slot 1 (GetInt32)
 * - NONE  : returns 0
 */
static int32_t cmNode_GetInt(const cmNodePort* port) {
    switch (port->m_type) {
    case CM_PORT_DIRECT:
        return *reinterpret_cast<const int32_t*>(port->m_pData);
    case CM_PORT_NODE: {
        struct cmNode { void** vtable; };
        auto* node = reinterpret_cast<cmNode*>(port->m_pData);
        int32_t tmp;
        using GetIntFn = void(*)(void*, int32_t*);
        reinterpret_cast<GetIntFn>(node->vtable[1])(node, &tmp);
        return tmp;
    }
    default:
        return 0;
    }
}

/**
 * cmNode_GetDim @ 0x82184B60  (cmSwitch_4B60)
 *
 * Reads an integer dimension count from a port.
 * - DIRECT: returns *(int32_t*)port.m_pData
 * - NODE  : calls vcall slot 5 (GetDim)
 * - NONE  : returns 0
 */
static int32_t cmNode_GetDim(const cmNodePort* port) {
    switch (port->m_type) {
    case CM_PORT_DIRECT:
        return *reinterpret_cast<const int32_t*>(port->m_pData);
    case CM_PORT_NODE: {
        struct cmNode { void** vtable; };
        auto* node = reinterpret_cast<cmNode*>(port->m_pData);
        int32_t tmp;
        using GetDimFn = void(*)(void*, int32_t*);
        reinterpret_cast<GetDimFn>(node->vtable[5])(node, &tmp);
        return tmp;
    }
    default:
        return 0;
    }
}

/**
 * cmNode_GetPortDim — internal helper shared by TryConnect variants.
 *
 * Returns the dimension code of the value currently connected to `port`.
 * For a DIRECT port, reads cmDataObj::m_dim at offset +16.
 * For a NODE port, reads the connected node's m_outputType at offset +4.
 */
static int32_t cmNode_GetPortDim(const cmNodePort* port) {
    switch (port->m_type) {
    case CM_PORT_DIRECT: {
        const uint8_t* obj = reinterpret_cast<const uint8_t*>(port->m_pData);
        return obj[16];  // cmDataObj::m_dim
    }
    case CM_PORT_NODE: {
        const int32_t* node = reinterpret_cast<const int32_t*>(port->m_pData);
        return node[1];  // cmNodeBase::m_outputType at +4
    }
    default:
        return 0;
    }
}

/**
 * cmNode_TryConnect @ 0x82185698  (util_5698)
 *
 * Validates that connected input ports match the required dimensions in desc.
 * On success, writes desc.output_dim to node->m_outputType and returns true.
 *
 * The function uses node->m_nConnected (at a layout-dependent offset) to choose
 * how many ports to validate (1, 2, or 3). The validation tries desc in order:
 *   desc.portC_dim != 0 → require 3 matching ports
 *   desc.portB_dim != 0 → require 2 matching ports
 *   desc.portA_dim != 0 → require 1 matching port
 *   all zeros           → always accept (unconditional)
 *
 * Binary node variant: m_nConnected at node+28.
 */
static bool cmNode_TryConnect(cmBinaryNode* node, cmPortDesc desc) {
    if (desc.portC_dim != 0) {
        // Need 3 connected ports — binary layout has no portC; skip
        // (3-port variant exists in util_5698 but no binary nodes use it)
        if (node->m_nConnected != 3) return false;
        // TODO: portC validation if needed
        return false;
    }

    if (desc.portB_dim != 0) {
        if (node->m_nConnected != 2) return false;
        int32_t dimA = cmNode_GetPortDim(&node->portA);
        int32_t dimB = cmNode_GetPortDim(&node->portB);
        if (dimA != desc.portA_dim) return false;
        if (dimB != desc.portB_dim) return false;
    } else if (desc.portA_dim != 0) {
        if (node->m_nConnected != 1) return false;
        int32_t dimA = cmNode_GetPortDim(&node->portA);
        if (dimA != desc.portA_dim) return false;
    }
    // Success: store the output type and return true
    node->m_outputType = desc.output_dim;
    return true;
}

/**
 * cmNode_TryConnectUnary @ 0x821854C8  (util_54C8)
 *
 * Single-input variant of TryConnect. m_nConnected at node+20.
 * Identical logic but validates only portA.
 */
static bool cmNode_TryConnectUnary(cmUnaryNode* node, cmPortDesc desc) {
    // desc.portB_dim unused for unary; desc.portC_dim unused
    if (desc.portA_dim != 0) {
        if (node->m_nConnected != 1) return false;
        int32_t dimA = cmNode_GetPortDim(&node->portA);
        if (dimA != desc.portA_dim) return false;
    }
    node->m_outputType = desc.output_dim;
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
//  ARITHMETIC NODES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmAdd — adds two inputs together.
 * Supports vec4 (slot 2), float (slot 4), and integer dimension (slot 5) evaluation.
 * Both input ports must carry the same dimension type.
 * vtable @ 0x82053A6C
 */

// cmAdd::GetVector @ 0x8227B158
// Reads portA and portB as vec4, outputs (A + B) component-wise.
void cmAdd::GetVector(float* out) {
    float va[4], vb[4];
    cmNode_GetVector(va, &portA);
    cmNode_GetVector(vb, &portB);
    out[0] = va[0] + vb[0];
    out[1] = va[1] + vb[1];
    out[2] = va[2] + vb[2];
    out[3] = va[3] + vb[3];
}

// cmAdd::GetFloat @ 0x8227B100
// Reads portA and portB as float, outputs (A + B).
void cmAdd::GetFloat(float* out) {
    float a = cmNode_GetFloat(&portA);
    float b = cmNode_GetFloat(&portB);
    *out = a + b;
}

// cmAdd::GetDim @ 0x8227B0C8
// Reads portA and portB as integer dimension counts, outputs (A + B).
void cmAdd::GetDim(int32_t* out) {
    int32_t a = cmNode_GetDim(&portA);
    int32_t b = cmNode_GetDim(&portB);
    *out = a + b;
}

// cmAdd::RegisterPorts @ 0x822622A8
// Tries port descriptors in priority order: vec4+vec4, float+float, int+int.
// Also used by cmNegate (aliased — cmNegate and cmAdd share this registration code).
void cmAdd::RegisterPorts(cmBinaryNode* node) {
    // Try vec4 + vec4 → vec4
    if (cmNode_TryConnect(node, {CM_DIM_VEC4, CM_DIM_VEC4, 0, CM_DIM_VEC4})) return;
    // Try float + float → float
    if (cmNode_TryConnect(node, {CM_DIM_FLOAT, CM_DIM_FLOAT, 0, CM_DIM_FLOAT})) return;
    // Try int32 + int32 → int32
    cmNode_TryConnect(node, {CM_DIM_INT32, CM_DIM_INT32, 0, CM_DIM_INT32});
}

// ── cmSubtract — subtracts portB from portA. vtable @ 0x82053AC4 ─────────────

// cmSubtract::GetFloat @ 0x8227B1F8
void cmSubtract::GetFloat(float* out) {
    float a = cmNode_GetFloat(&portA);
    float b = cmNode_GetFloat(&portB);
    *out = a - b;
}

// cmSubtract::GetVector @ 0x8227B250
void cmSubtract::GetVector(float* out) {
    float va[4], vb[4];
    cmNode_GetVector(va, &portA);
    cmNode_GetVector(vb, &portB);
    out[0] = va[0] - vb[0];
    out[1] = va[1] - vb[1];
    out[2] = va[2] - vb[2];
    out[3] = va[3] - vb[3];
}

// Note: cmSubtract shares RegisterPorts (same descriptor set as cmAdd):
//   {VEC4,VEC4,0,VEC4}, {FLOAT,FLOAT,0,FLOAT}

// ─────────────────────────────────────────────────────────────────────────────
//  UNARY MATH NODES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmNegate — negates the input value. vtable @ 0x82053C24
 *
 * For vec4: multiplies by the constant {-1,-1,-1,-1} (g_cm_negateVec @ 0x825C5938).
 * For float: applies IEEE sign-bit flip (fneg).
 */

// cmNegate::GetVector @ 0x8227B518
// Loads portA as vec4, then multiplies each component by -1 via the negate constant.
void cmNegate::GetVector(float* out) {
    float v[4];
    cmNode_GetVector(v, &portA);
    // Multiply by g_cm_negateVec = {-1,-1,-1,-1}; broadcast w-component as scalar
    float negScale = g_cm_negateVec[0]; // always -1.0f
    out[0] = v[0] * negScale;
    out[1] = v[1] * negScale;
    out[2] = v[2] * negScale;
    out[3] = v[3] * negScale;
}

// cmNegate::GetFloat @ 0x8227B4E0
void cmNegate::GetFloat(float* out) {
    *out = -cmNode_GetFloat(&portA);
}

// cmNegate::RegisterPorts @ 0x82262350
// Uses same descriptor set as cmAdd (aliased to cmAdd::RegisterPorts).
// The unary semantic is enforced by portB always being unconnected.
void cmNegate::RegisterPorts(cmBinaryNode* node) {
    // Aliased to cmAdd::RegisterPorts — same binary code. Call via cmAdd vtable on this.
    reinterpret_cast<cmAdd*>(this)->cmAdd::RegisterPorts(node);
}

// ── cmAbs — absolute value of input. vtable @ 0x82053E8C ─────────────────────

// cmAbs::GetFloat @ 0x8227B758
void cmAbs::GetFloat(float* out) {
    *out = fabsf(cmNode_GetFloat(&portA));
}

// cmAbs::GetDim @ 0x8227B718
void cmAbs::GetDim(int32_t* out) {
    int32_t v = cmNode_GetDim(&portA);
    *out = v < 0 ? -v : v;
}

// cmAbs::RegisterPorts @ 0x82262478
void cmAbs::RegisterPorts(cmUnaryNode* node) {
    if (cmNode_TryConnectUnary(node, {CM_DIM_FLOAT, 0, 0, CM_DIM_FLOAT})) return;
    cmNode_TryConnectUnary(node, {CM_DIM_INT32, 0, 0, CM_DIM_INT32});
}

// ─────────────────────────────────────────────────────────────────────────────
//  TRIGONOMETRIC NODES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmSine — sine of a scalar input angle (radians). vtable @ 0x82053C7C
 *
 * The binary has a fast polynomial sinf approximation inlined as
 * phBoundCapsule_01D8_g @ 0x824301D8. The approximation uses
 * Chebyshev/Taylor coefficients loaded from a lookup table in .rdata.
 * We reconstruct with the standard sinf for accuracy.
 */

// cmSine::GetFloat @ 0x8227B578
void cmSine::GetFloat(float* out) {
    *out = sinf(cmNode_GetFloat(&portA));
}

// ── cmCosine — cosine of input angle. vtable @ 0x82053CD4 ────────────────────

// cmCosine::GetFloat @ 0x8227B5B8
void cmCosine::GetFloat(float* out) {
    *out = cosf(cmNode_GetFloat(&portA));
}

// ── cmArcCosine — inverse cosine. vtable @ 0x82053D2C ────────────────────────

// cmArcCosine::GetFloat @ 0x8227B5F8
void cmArcCosine::GetFloat(float* out) {
    *out = acosf(cmNode_GetFloat(&portA));
}

// ── cmArcTangent — arctangent (1-arg). vtable @ 0x82053DDC ───────────────────

// cmArcTangent::GetFloat @ 0x8227B678
void cmArcTangent::GetFloat(float* out) {
    *out = atanf(cmNode_GetFloat(&portA));
}

// cmArcTangent::RegisterPorts @ 0x82262438
void cmArcTangent::RegisterPorts(cmUnaryNode* node) {
    cmNode_TryConnectUnary(node, {CM_DIM_FLOAT, 0, 0, CM_DIM_FLOAT});
}

// ─────────────────────────────────────────────────────────────────────────────
//  SIGNAL PROCESSING NODES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmLerp — linear interpolation between portA and portB by a scalar t.
 * vtable @ somewhere in cm* range — TODO: confirm address.
 * Three inputs: A (float/vec4), B (float/vec4), t (float).
 */
// TODO: lift cmLerp (3-port node) when address confirmed.

/**
 * cmClamp — clamps portA to [portB_min, portB_max] range.
 * vtable @ 0x820540F4
 */

// cmClamp::GetVector @ 0x8227BEA0
void cmClamp::GetVector(float* out) {
    float va[4], vmin[4], vmax[4];
    cmNode_GetVector(va,   &portA);
    cmNode_GetVector(vmin, &portB);  // portB = min
    // portC would be max; layout is binary+extra — TODO verify portC offset
    // For now reconstruct what the assembly does: clamp each component
    // Assembly shows two binary ports + inline constants in some cases
    out[0] = va[0] < vmin[0] ? vmin[0] : (va[0] > vmax[0] ? vmax[0] : va[0]);
    out[1] = va[1] < vmin[1] ? vmin[1] : (va[1] > vmax[1] ? vmax[1] : va[1]);
    out[2] = va[2] < vmin[2] ? vmin[2] : (va[2] > vmax[2] ? vmax[2] : va[2]);
    out[3] = va[3] < vmin[3] ? vmin[3] : (va[3] > vmax[3] ? vmax[3] : va[3]);
}

// cmClamp::GetFloat @ 0x8227BE20
void cmClamp::GetFloat(float* out) {
    float a    = cmNode_GetFloat(&portA);
    float vmin = cmNode_GetFloat(&portB);
    // TODO: verify portC is the max input; may be a separate field
    float vmax = vmin; // placeholder until portC offset confirmed
    *out = a < vmin ? vmin : (a > vmax ? vmax : a);
}

// cmClamp::GetDim @ 0x8227BDB8
void cmClamp::GetDim(int32_t* out) {
    int32_t a    = cmNode_GetDim(&portA);
    int32_t vmin = cmNode_GetDim(&portB);
    *out = a < vmin ? vmin : a;
    // TODO: clamp max from portC
}

// cmClamp::RegisterPorts @ 0x82262658
void cmClamp::RegisterPorts(cmBinaryNode* node) {
    if (cmNode_TryConnect(node, {CM_DIM_VEC4,  CM_DIM_VEC4,  0, CM_DIM_VEC4}))  return;
    if (cmNode_TryConnect(node, {CM_DIM_FLOAT, CM_DIM_FLOAT, 0, CM_DIM_FLOAT})) return;
    cmNode_TryConnect(node, {CM_DIM_INT32, CM_DIM_INT32, 0, CM_DIM_INT32});
}

// ─────────────────────────────────────────────────────────────────────────────
//  STATEFUL NODES — MEMORY, INTEGRATE, DIFFERENTIAL
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmMemory — stores and returns a value from the previous frame.
 * vtable @ (various), class rage::cmMemory
 *
 * Layout (extends cmBinaryNode):
 *   portA at +12/+16  : new value input (written each frame by Update)
 *   portB at +20/+24  : secondary value connection (e.g. initial value)
 *   m_nConnected +28
 *   m_pCurrentValue  +32  : 16-byte aligned buffer holding the current state
 *   m_pPreviousValue +36  : 16-byte aligned buffer holding the previous state
 *
 * The output functions (vfn_1..4) first call vfn_8 (Sync) to advance the node,
 * then return the value from m_pPreviousValue (the "remembered" value).
 */

// cmMemory::GetInt32 @ 0x82271640
// Syncs, then copies the int32 from m_pPreviousValue to out.
void cmMemory::GetInt32(int32_t* out) {
    Sync();
    *out = *reinterpret_cast<int32_t*>(m_pPreviousValue);
}

// cmMemory::Sync @ 0x822714C8 (vtable slot 8)
// Synchronizes the memory node state. This is a virtual function that
// ensures the node's internal state is consistent before reading values.
void cmMemory::Sync() {
    // TODO: Implement full synchronization logic
    // For now, just call SyncInput to update current value
    SyncInput();
}

// cmMemory::GetVector @ 0x822715A0
// Syncs, then copies the 16-byte vec4 from m_pPreviousValue to out.
void cmMemory::GetVector(float* out) {
    Sync();
    float* src = reinterpret_cast<float*>(m_pPreviousValue);
    out[0] = src[0]; out[1] = src[1]; out[2] = src[2]; out[3] = src[3];
}

// cmMemory::GetBool @ 0x822715F0
// Syncs, then copies the bool byte from m_pPreviousValue to out.
void cmMemory::GetBool(uint8_t* out) {
    Sync();
    *out = *reinterpret_cast<uint8_t*>(m_pPreviousValue);
}

// cmMemory::GetFloat @ 0x82271550
// Syncs, then copies the float from m_pPreviousValue to out.
void cmMemory::GetFloat(float* out) {
    Sync();
    *out = *reinterpret_cast<float*>(m_pPreviousValue);
}

// cmMemory::Update @ 0x82271468
// Evaluates portA (the new input value) into m_pCurrentValue, then marks m_pPreviousValue
// as pointing to the m_pCurrentValue result via a direct-pointer port descriptor pair
// at stack; clears bit 3 of m_flags.
void cmMemory::Update() {
    // Resolve portA into m_pCurrentValue, using the generic SetFromPort dispatcher
    // (util_5380 dispatches by m_outputType: float, bool, vec4, int, dim).
    cmNode_SetFromPort_Dispatch(m_pCurrentValue, &portA, m_outputType);

    // Build a temporary port struct pointing m_pCurrentValue as a direct-data port
    // so that m_pPreviousValue sees the newly written value next frame.
    cmNodePort tempPort;
    tempPort.m_pData = m_pCurrentValue;
    tempPort.m_type  = CM_PORT_DIRECT;
    cmNode_SetFromPort_Dispatch(m_pPreviousValue, &tempPort, m_outputType);

    // Clear bit 3 of flags (marks that the node has been updated this frame).
    m_flags = (m_flags & ~8) ^ 8;  // rlwinm + xori pattern from assembly
}

// cmMemory::SetValue @ 0x82271518
// Writes m_pCurrentValue into m_pPreviousValue as a direct-data link.
void cmMemory::SetValue() {
    cmNodePort tempPort;
    tempPort.m_pData = m_pCurrentValue;
    tempPort.m_type  = CM_PORT_DIRECT;
    cmNode_SetFromPort_Dispatch(m_pPreviousValue, &tempPort, m_outputType);
}

// cmMemory::SetInitialInput @ 0x82271518
// Reads portB (initial value) into m_pPreviousValue.
// Also sets m_pCurrentValue via a direct-connection port.
void cmMemory::SetInitialInput() {
    // This is vfn_9 / vfn_14 — sets the initial accumulator from portB.
    // assembly: addi r4, r31, 20 → portB; lwz r3, 36(r31) → m_pPreviousValue
    cmNode_SetFromPort_Dispatch(m_pPreviousValue, &portB, m_outputType);
}

// cmMemory::SyncInput @ 0x82278158
// Forwarding helper: dispatches portA to m_pCurrentValue.
// Used internally by Sync() which is vtable slot 8.
void cmMemory::SyncInput() {
    cmNode_SetFromPort_Dispatch(m_pCurrentValue, &portA, m_outputType);
}

// cmMemory::Reset / ReInit @ 0x82271390 (vfn_17)
// Frees old value buffers and re-allocates fresh 16-byte aligned memory
// from the engine heap, initialised to all-zero.
// Port descriptor written to dimension 0x10 (offset +16) of the new buffers
// carries the output type from m_outputType.
void cmMemory::Reset() {
    // Free old allocations
    if (m_pCurrentValue)  rage_free(m_pCurrentValue);
    if (m_pPreviousValue) rage_free(m_pPreviousValue);

    // Allocate fresh buffers using the engine heap (rage_alloc, align=16, size=32).
    m_pCurrentValue  = rage_alloc_aligned(/* size = */ 32, /* align = */ 16);
    m_pPreviousValue = rage_alloc_aligned(32, 16);

    if (m_pCurrentValue) {
        // Clear dimension byte at +16 and +17 (these are cmDataObj fields)
        uint8_t* obj = reinterpret_cast<uint8_t*>(m_pCurrentValue);
        obj[16] = 0;
        obj[17] = 0;
        // Construct a cmReporter for the buffer (initialises its type tracking).
        // cmReporterCtor_52F0_g writes m_outputType into obj[16].
        obj[16] = static_cast<uint8_t>(m_outputType);
    }

    if (m_pPreviousValue) {
        uint8_t* obj = reinterpret_cast<uint8_t*>(m_pPreviousValue);
        obj[16] = 0;
        obj[17] = 0;
        obj[16] = static_cast<uint8_t>(m_outputType);
    }
}

// cmMemory::RegisterPorts @ 0x82262960
// Tries all dimension combinations in priority order.
// cmMemory accepts up to 5 port dimension types.
void cmMemory::RegisterPorts(cmBinaryNode* node) {
    if (cmNode_TryConnect(node, {CM_DIM_VEC4,  CM_DIM_VEC4,  0, CM_DIM_VEC4}))  return;
    if (cmNode_TryConnect(node, {CM_DIM_FLOAT, CM_DIM_FLOAT, 0, CM_DIM_FLOAT})) return;
    if (cmNode_TryConnect(node, {CM_DIM_BOOL,  CM_DIM_BOOL,  0, CM_DIM_BOOL}))  return;
    if (cmNode_TryConnect(node, {CM_DIM_INT32, CM_DIM_INT32, 0, CM_DIM_INT32})) return;
    // Dimension 5 (extended int?)
    cmNode_TryConnect(node, {5, 5, 0, 5});
}

// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmIntegrate — numerically integrates (accumulates) its input over time.
 * vtable @ 0x82054934
 *
 * Each frame: accumulated += input * deltaTime
 *
 * portA at +12/+16  : type-sense port (tells the node what dim to produce)
 * portB at +20/+24  : the rate value being integrated (float or vec4)
 * m_pCurrentValue at +32 : current accumulated result
 *
 * Global g_cm_frameTime @ 0x825C4958 supplies the delta-time scalar.
 */

// cmIntegrate::Tick @ 0x82278420 (vfn_10)
// Called every frame to advance the integrator.
void cmIntegrate::Tick() {
    // Determine output dimension from portA's dimension type
    int32_t dim = cmNode_GetPortDim(&portA);

    if (dim == CM_DIM_VEC4) {
        // Vector integration: result += portB_vec * deltaTime
        float* accum = reinterpret_cast<float*>(m_pCurrentValue);
        float rate[4];
        cmNode_GetVector(rate, &portB);

        float dt = g_cm_frameTime;
        accum[0] += rate[0] * dt;
        accum[1] += rate[1] * dt;
        accum[2] += rate[2] * dt;
        accum[3] += rate[3] * dt;
    } else if (dim == CM_DIM_FLOAT) {
        // Scalar integration: result += portB_float * deltaTime
        float* accum = reinterpret_cast<float*>(m_pCurrentValue);
        float rate = cmNode_GetFloat(&portB);
        *accum += rate * g_cm_frameTime;
    }
    // Other dim types not observed — noop
}

// cmIntegrate::RegisterPorts @ 0x82262A60
// Tries vec4 pair then int32 pair. No float-only variant (float uses same slot).
void cmIntegrate::RegisterPorts(cmBinaryNode* node) {
    if (cmNode_TryConnect(node, {CM_DIM_VEC4,  CM_DIM_VEC4,  0, CM_DIM_VEC4}))  return;
    cmNode_TryConnect(node, {CM_DIM_INT32, CM_DIM_INT32, 0, CM_DIM_INT32});
}

// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmDifferential — numerically differentiates (computes rate-of-change) of input.
 * vtable @ 0x82054884
 *
 * Each evaluation: output = input * frameRate * frameTime
 * (effectively: output ≈ input per-second, scaled to frame)
 *
 * Note: uses the fast sinf approximation lookup table address block to load
 * the frame rate constant. g_cm_frameRate is near address 0x829DAEC8.
 *
 * portA at +12/+16  : the value being differentiated
 * m_nConnected at +20 (unary layout — uses util_54C8)
 */

// cmDifferential::GetFloat @ 0x82277E28
// output = portA_float * g_cm_frameRate * g_cm_frameTime
void cmDifferential::GetFloat(float* out) {
    float input = cmNode_GetFloat(&portA);
    *out = input * g_cm_frameRate * g_cm_frameTime;
}

// cmDifferential::GetVector @ 0x82277E78
// Similar but for vec4.
void cmDifferential::GetVector(float* out) {
    float v[4];
    cmNode_GetVector(v, &portA);
    float scale = g_cm_frameRate * g_cm_frameTime;
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
    out[3] = v[3] * scale;
}

// cmDifferential::RegisterPorts @ 0x822628E0
// Uses the unary TryConnect (util_54C8): tries vec4 then int32.
void cmDifferential::RegisterPorts(cmUnaryNode* node) {
    if (cmNode_TryConnectUnary(node, {CM_DIM_VEC4,  0, 0, CM_DIM_VEC4}))  return;
    cmNode_TryConnectUnary(node, {CM_DIM_INT32, 0, 0, CM_DIM_INT32});
}

// ─────────────────────────────────────────────────────────────────────────────
//  APPROACH OPERATOR NODES
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmApproachOperator — moves a stored value toward a target at a given rate.
 * vtable @ 0x82056A04
 *
 * portA at +20/+24 : rate (float or vec4 approach speed)
 * portB at +28/+32 : target value
 * m_pResult at +40 : the current state buffer with dim byte at [+16]
 *
 * Each Tick():
 *   float case: current += rate * dt; if overshot → snap to target
 *   vec4 case : vector approach using normalized delta + distance check
 *
 * g_cm_frameTime used for dt scaling.
 */

/**
 * cmApproachOperator_VectorApproach @ 0x82277D38
 *
 * Moves vector `current` toward vector `target` by `speed*dt` per frame.
 * Uses a fast normalize (vrsqrtefp) to compute step direction.
 * Returns true (1) if the target was already reached or overshot.
 * Writes the updated position back to `current`.
 *
 * Algorithm (lifted from the SIMD assembly):
 *   delta   = current - target
 *   distSq  = dot3(delta, delta)
 *   stepLen = speed * dt
 *   if distSq >= stepLen*stepLen : clamp step, interpolate, compare sign to snap
 *   else                         : snap current = target, return 1 (reached)
 */
static bool cmApproachOperator_VectorApproach(
        float* current, const float* target, float stepLen) {
    float delta[3] = {
        current[0] - target[0],
        current[1] - target[1],
        current[2] - target[2]
    };
    float distSq = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
    float stepSq  = stepLen * stepLen;

    if (distSq >= stepSq) {
        // Not yet reached — take a step toward target
        float invDist = 1.0f / sqrtf(distSq);
        float t = stepLen * invDist;
        current[0] += (target[0] - current[0]) * t;
        current[1] += (target[1] - current[1]) * t;
        current[2] += (target[2] - current[2]) * t;
        // Check if we snapped (vec components match after masking NaN/Inf)
        bool snapped = (current[0] == target[0] &&
                        current[1] == target[1] &&
                        current[2] == target[2]);
        return snapped ? true : false;
    } else {
        // Close enough — snap
        current[0] = target[0];
        current[1] = target[1];
        current[2] = target[2];
        return true;
    }
}

// cmApproachOperator::Tick @ 0x82278B90 (vfn_10)
// Advances the approach by one frame step.
void cmApproachOperator::Tick() {
    void* resultPtr = m_pResult;
    if (!resultPtr) return;

    cmDataObj* result = reinterpret_cast<cmDataObj*>(resultPtr);
    int32_t dim = result->m_dim;

    float dt = g_cm_frameTime;

    if (dim == CM_DIM_FLOAT) {
        // Scalar approach
        float current = result->m_float;
        float rate    = cmNode_GetFloat(&portA);
        float target  = cmNode_GetFloat(&portB);
        float step    = rate * dt;

        if (current < target) {
            current += step;
            if (current > target) current = target;  // clamp overshoot
        } else if (current > target) {
            current -= step;
            if (current < target) current = target;  // clamp overshoot
        }
        result->m_float = current;

    } else if (dim == CM_DIM_INT32) {
        // Vector (vec4) approach using the vector approach helper
        float  target[4];
        float* current = result->m_vec4;

        cmNode_GetVector(target, &portB);
        float rate = cmNode_GetFloat(&portA);
        float step = rate * dt;

        cmApproachOperator_VectorApproach(current, target, step);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  GENERIC PORT→DATA DISPATCHER (util_5380)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmNode_SetFromPort_Dispatch @ 0x82275380  (util_5380)
 *
 * Evaluates `port` and writes the result into `dst` (a cmDataObj buffer).
 * The output type is determined by dst->m_dim:
 *   CM_DIM_FLOAT = 1 : writes float via GetFloat
 *   CM_DIM_BOOL  = 2 : writes bool  via GetBool (cmCond_21B0)
 *   CM_DIM_INT   = 0 : writes int32 via GetDim
 *   CM_DIM_VEC4  = 3 : writes vec4  via GetVector
 *   CM_DIM_INT32 = 4 : writes int32 via GetInt
 */
void cmNode_SetFromPort_Dispatch(void* dst, const cmNodePort* port, int32_t dim) {
    cmDataObj* obj = reinterpret_cast<cmDataObj*>(dst);
    int32_t idx = dim - 1;  // table is 0-indexed with (dim-1)
    if (idx < 0 || idx > 4) return;  // out of range — noop

    switch (dim) {
    case CM_DIM_FLOAT:
        obj->m_float = cmNode_GetFloat(port);
        break;
    case CM_DIM_BOOL:
        obj->m_bool = cmNode_GetBool(port);
        break;
    case CM_DIM_INT:
        obj->m_int = static_cast<int32_t>(cmNode_GetDim(port));
        break;
    case CM_DIM_VEC4:
        cmNode_GetVector(obj->m_vec4, port);
        break;
    case CM_DIM_INT32:
        obj->m_int = cmNode_GetInt(port);
        break;
    default:
        break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  POLYNOMIAL SINE APPROXIMATION (phBoundCapsule_01D8_g @ 0x824301D8)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * rage_sinf_approx @ 0x824301D8
 *
 * Fast Chebyshev polynomial approximation of sinf used throughout the cm* nodes.
 * The function is misnamed in the scaffold (assigned to phBoundCapsule_01D8_g)
 * but is clearly a sin implementation: it takes f1 (angle in radians), computes
 * the absolute value, range-reduces to [0, pi/4] via an index table, evaluates
 * an 8-term polynomial, applies quadrant sign, and returns the result in f1.
 *
 * For decompilation purposes we reconstruct with the CRT sinf; the binary uses
 * this approximation for performance on the Xbox 360's paired-singles FPU.
 */
float rage_sinf_approx(float angle) {
    // TODO: fully reconstruct the Chebyshev polynomial from coefficient table
    // The lookup table at ~0x829DAEC8 (lis -32163, addi -20808) holds the
    // 8 double-precision coefficients. For now, defer to standard sinf.
    return sinf(angle);
}

// ─────────────────────────────────────────────────────────────────────────────
//  STUB IMPLEMENTATIONS (TODO: lift from scaffold when needed)
// ─────────────────────────────────────────────────────────────────────────────

// cmSubtract::GetDim @ undiscovered — TODO
// cmMultiply::GetVector, GetFloat, GetDim — TODO
// cmDivide::GetFloat, GetVector — TODO
// cmMin, cmMax — TODO (likely fsel idiom in assembly)
// cmAngleLerp — TODO (RegisterPorts @ 0x82262620)
// cmAngleLinearApproach::Tick @ 0x82278F10 — TODO
// cmAnglePowerApproach::Tick @ 0x82278E58 — TODO
// cmApproach2 — TODO (cmApproach2_vfn_10 @ 0x82279428)
// cmCapture — TODO (vfn_10 @ 0x822779C8, vfn_17 @ 0x822787F8)
// cmChanged  — TODO (vfn_10 @ 0x82277EF0, vfn_17 @ 0x82278168)
// cmDifferentiate — TODO (vfn_10 @ 0x822782D0)

} // namespace rage


/**
 * cmNode_GetBool @ 0x82184C40  (util_4C40)
 *
 * Reads a boolean from a port.
 * - DIRECT: returns *(uint8_t*)port.m_pData
 * - NODE  : calls vcall slot 3 (GetBool)
 * - NONE  : returns 0
 */
uint8_t cmNode_GetBool(const rage::cmNodePort* port) {
    switch (port->m_type) {
    case rage::CM_PORT_DIRECT:
        return *reinterpret_cast<const uint8_t*>(port->m_pData);
    case rage::CM_PORT_NODE: {
        struct cmNode { void** vtable; };
        auto* node = reinterpret_cast<cmNode*>(port->m_pData);
        uint8_t tmp = 0;
        using GetBoolFn = void(*)(void*, uint8_t*);
        reinterpret_cast<GetBoolFn>(node->vtable[3])(node, &tmp);
        return tmp;
    }
    default:
        return 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  CONTROL REFERENCE NODE
// ─────────────────────────────────────────────────────────────────────────────

/**
 * rage::cmControlRef::GetBool @ 0x8227CCA8 | size: 0x5C (vtable slot 3)
 *
 * Retrieves a boolean value from a control reference by index.
 * The control reference points to a control object that contains an array
 * of control entries. Each entry has a type field that determines how to
 * retrieve the boolean value.
 *
 * Entry types:
 *   1 = Direct byte value - read the byte directly from the entry
 *   2 = Object reference - call the object's GetBool virtual method (slot 3)
 *   Other = Return false (write 0 to output)
 *
 * Layout:
 *   +0x0C = pointer to control object
 *   +0x10 = entry index (uint32_t)
 *   Control object:
 *     +0x54 = pointer to entry array
 *   Entry (8 bytes each):
 *     +0x00 = data pointer (byte* for type 1, object* for type 2)
 *     +0x04 = type code (int32_t)
 */
void cmControlRef::GetBool(uint8_t* out) {
    // Load control object pointer from +12
    void* controlObj = *(void**)((uint8_t*)this + 12);
    
    // Load entry index from +16
    uint32_t entryIndex = *(uint32_t*)((uint8_t*)this + 16);
    
    // Calculate byte offset: index * 8 (each entry is 8 bytes)
    uint32_t entryOffset = entryIndex << 3;  // rlwinm r10,r10,3,0,28
    
    // Get pointer to entry array (at offset +84 of control object)
    void** entryArrayPtr = (void**)((uint8_t*)controlObj + 84);
    void* entryArray = *entryArrayPtr;
    
    // Get pointer to the specific entry
    uint8_t* entry = (uint8_t*)entryArray + entryOffset;
    
    // Load entry type from offset +4
    int32_t entryType = *(int32_t*)(entry + 4);
    
    if (entryType == 1) {
        // Type 1: Direct byte value
        // Load data pointer from offset +0
        uint8_t* dataPtr = *(uint8_t**)entry;
        // Read the byte value
        *out = *dataPtr;
    } else if (entryType == 2) {
        // Type 2: Object reference with virtual GetBool method
        // Load object pointer from offset +0
        void** objPtr = *(void***)entry;
        // Get vtable
        void** vtable = (void**)*objPtr;
        // Call virtual method at slot 3 (GetBool)
        using GetBoolFn = void(*)(void*, uint8_t*);
        reinterpret_cast<GetBoolFn>(vtable[3])(objPtr, out);
    } else {
        // Unknown type: return false
        *out = 0;
    }
}
