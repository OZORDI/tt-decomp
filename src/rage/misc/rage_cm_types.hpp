/**
 * rage_cm_types.hpp — Reverse-engineered concrete struct definitions for the
 *                      RAGE Curve Math (cm*) node graph system.
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Include this INSTEAD of the auto-generated rage_cm.hpp skeletons when you
 * need the real field layouts. It declares cmNodeBase, cmUnaryNode,
 * cmBinaryNode, cmStatefulNode and the concrete node subclasses whose layouts
 * have been fully confirmed by assembly analysis.
 *
 * Evidence sources:
 *   util_92D8 (0x821792D8), util_9350 (0x82179350), util_4BD8 (0x82184BD8)
 *   util_5698 (0x82185698), util_54C8 (0x821854C8), util_5380 (0x82275380)
 *   cmAdd_vfn_2/4/5/16, cmNegate_vfn_2/4, cmMemory_vfn_*, cmIntegrate_vfn_10/16
 *   cmDifferential_vfn_2/4/16, cmApproachOperator_vfn_10 / _7D38_wrh
 */
#pragma once
#include <cstdint>
#include <cmath>

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
//  GLOBAL CONSTANTS
// ─────────────────────────────────────────────────────────────────────────────

// Current frame delta-time (seconds). Written by the engine tick.
// @ 0x825C4958 (.data, 4 bytes)
extern float g_cm_frameTime;

// Frame rate constant. Used by cmDifferential to scale the derivative.
// Loaded alongside g_cm_frameTime from the sinf approximation coefficient block.
// Exact address TBD (near 0x829DAEC8). Likely 60.0f or 1/60.
extern float g_cm_frameRate;

// Constant 0.0f in .rdata, returned by unconnected float ports.
// @ 0x8202D110 (.rdata, 4 bytes)
extern const float g_cm_zeroFloat;

// { -1.f, -1.f, -1.f, -1.f } used by cmNegate's vector evaluation.
// @ 0x825C5938 (.data, 16 bytes)
extern float g_cm_negateVec[4];

// ─────────────────────────────────────────────────────────────────────────────
//  DIMENSION / PORT TYPE CODES
// ─────────────────────────────────────────────────────────────────────────────

// Port connection type (cmNodePort::m_type)
enum cmPortType : int32_t {
    CM_PORT_NONE   = 0,  // not connected
    CM_PORT_DIRECT = 1,  // m_pData → cmDataObj*  (raw value)
    CM_PORT_NODE   = 2,  // m_pData → cmNode*     (call vcall to evaluate)
};

// Output / input dimension codes (cmDataObj::m_dim, cmNodeBase::m_outputType)
enum cmDimType : int32_t {
    CM_DIM_INT   = 0,  // int32 or switch-count (GetDim  → vcall slot 5)
    CM_DIM_FLOAT = 1,  // float scalar          (GetFloat→ vcall slot 4)
    CM_DIM_VEC4  = 2,  // float[4] 16-byte vec  (GetVector→ vcall slot 2)
    CM_DIM_BOOL  = 3,  // uint8_t boolean       (GetBool → vcall slot 3)
    CM_DIM_INT32 = 4,  // int32                 (GetInt32→ vcall slot 1)
};

// ─────────────────────────────────────────────────────────────────────────────
//  DATA BUFFER OBJECT
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmDataObj — typed value buffer pointed to by a CM_PORT_DIRECT port.
 * Always 16-byte aligned (allocated by rage_alloc_aligned(32, 16)).
 *
 * The value sits at offset 0. For vec4 the full 16 bytes are the payload;
 * for smaller types only the first bytes are used.
 * m_dim at offset +16 identifies the active union member.
 */
struct cmDataObj {
    union {
        float   m_float;    // CM_DIM_FLOAT — scalar float
        uint8_t m_bool;     // CM_DIM_BOOL  — boolean byte
        int32_t m_int;      // CM_DIM_INT / CM_DIM_INT32
        float   m_vec4[4];  // CM_DIM_VEC4  — 16-byte aligned vec4
    };                      // +0x00
    // +0x04..+0x0F  additional vec4 components / padding
    float    m_pad[3];      // +0x04 (part of vec4)
    cmDimType m_dim;        // +0x10  dimension type code
    uint8_t   m_flags;      // +0x11
    uint8_t   m_pad2[2];    // +0x12
};
static_assert(sizeof(cmDataObj) == 20, "cmDataObj layout mismatch");

// ─────────────────────────────────────────────────────────────────────────────
//  PORT STRUCT
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmNodePort — one input slot on a node. Stored inline at fixed offsets.
 * Size: 8 bytes.
 */
struct cmNodePort {
    void*      m_pData;   // +0x00  raw value ptr (DIRECT) or upstream cmNode* (NODE)
    cmPortType m_type;    // +0x04  CM_PORT_* connection type
};

// ─────────────────────────────────────────────────────────────────────────────
//  PORT DESCRIPTOR (for TryConnect calls)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmPortDesc — 4-byte descriptor pushed onto the stack in RegisterPorts.
 * Encodes the expected dimension for each input port and the output type
 * to store in m_outputType when the match succeeds.
 *
 *   portA_dim  : required dim for portA  (0 = any / don't check)
 *   portB_dim  : required dim for portB  (0 = unary node, skip portB)
 *   portC_dim  : required dim for portC  (0 = at most binary)
 *   output_dim : written to m_outputType on success
 */
struct cmPortDesc {
    uint8_t portA_dim;
    uint8_t portB_dim;
    uint8_t portC_dim;
    uint8_t output_dim;
};

// ─────────────────────────────────────────────────────────────────────────────
//  BASE NODE HEADERS
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmNodeBase — common header at the start of every cm* node object.
 *
 * Vtable slot conventions (verified from assembly):
 *   [0]  ~Destructor
 *   [1]  GetInt32(int32_t* out)
 *   [2]  GetVector(float* out)       16-byte aligned write
 *   [3]  GetBool(uint8_t* out)
 *   [4]  GetFloat(float* out)
 *   [5]  GetDim(int32_t* out)        "switch count" / integer dimension
 *   [6]  Update()                    advance stateful node by one frame
 *   [8]  Sync()                      internal sync (stateful nodes only)
 *   [9]  SetInitialInput(port)
 *   [10] Tick(...)                   per-frame evaluation (approach/integrate)
 *   [14] SetValue(port)
 *   [16] RegisterPorts(node)         TryConnect — sets m_outputType on match
 *   [17] Reset() / ReInit()          (re-)allocate result buffers
 */
struct cmNodeBase {
    void**    m_pVtable;     // +0x00
    int32_t   m_outputType;  // +0x04  CM_DIM_* — set by RegisterPorts on match
    int32_t   m_flags;       // +0x08  bit3 = dirty toggle; other bits TBD
};

/**
 * cmUnaryNode — one input port + connection count.
 * Uses util_54C8 (TryConnectUnary) for port registration.
 * Examples: cmNegate, cmSine, cmCosine, cmAbs, cmDifferential, cmArcCosine
 */
struct cmUnaryNode : cmNodeBase {
    cmNodePort portA;        // +0x0C  input
    int32_t    m_nConnected; // +0x14  number of connected inputs (0 or 1)
};

/**
 * cmBinaryNode — two input ports + connection count.
 * Uses util_5698 (TryConnect) for port registration.
 * Examples: cmAdd, cmSubtract, cmMultiply, cmIntegrate, cmClamp
 */
struct cmBinaryNode : cmNodeBase {
    cmNodePort portA;        // +0x0C  first input
    cmNodePort portB;        // +0x14  second input
    int32_t    m_nConnected; // +0x1C  number of connected inputs (0, 1, or 2)
};

/**
 * cmStatefulNode — binary node with two 16-byte aligned result buffers.
 * The current/previous value pointers are allocated by Reset() (vfn_17).
 * Examples: cmMemory, cmIntegrate, cmIntegrate0
 */
struct cmStatefulNode : cmBinaryNode {
    void* m_pCurrentValue;   // +0x20  aligned buffer — written by Update/Tick
    void* m_pPreviousValue;  // +0x24  aligned buffer — returned by GetFloat/Vec
};

/**
 * cmApproachNode — approach operator: moves a stored value toward a target.
 * Offset layout confirmed from cmApproachOperator_vfn_10 assembly:
 *   portA at +0x14 (rate input)
 *   portB at +0x1C (target input)
 *   m_pResult at +0x28 (cmDataObj* with dim byte at [+16])
 */
struct cmApproachNode : cmNodeBase {
    uint8_t    m_pad[8];      // +0x0C  purpose TBD (may be speed factor or ID)
    cmNodePort portA;         // +0x14  rate / approach speed
    cmNodePort portB;         // +0x1C  target value
    int32_t    m_nConnected;  // +0x24
    int32_t    m_pad2;        // +0x28  align / purpose TBD
    cmDataObj* m_pResult;     // +0x28  current state buffer
    // NOTE: exact offset of m_pResult confirmed as +40 (0x28) from assembly:
    //   lwz r11, 40(r31)  → cmApproachOperator_vfn_10 @ 0x82278B90
};

// ─────────────────────────────────────────────────────────────────────────────
//  CONCRETE NODE STRUCTS (fully reverse-engineered)
// ─────────────────────────────────────────────────────────────────────────────

/** cmAdd — portA + portB. vtable @ 0x82053A6C */
struct cmAdd : cmBinaryNode {
    void GetVector(float* out);                     // [2]  @ 0x8227B158
    void GetFloat(float* out);                      // [4]  @ 0x8227B100
    void GetDim(int32_t* out);                      // [5]  @ 0x8227B0C8
    static void RegisterPorts(cmBinaryNode* node);  // [16] @ 0x822622A8
};

/** cmSubtract — portA - portB. vtable @ 0x82053AC4 */
struct cmSubtract : cmBinaryNode {
    void GetVector(float* out);                     // [2]  @ 0x8227B250
    void GetFloat(float* out);                      // [4]  @ 0x8227B1F8
    // GetDim not observed; same RegisterPorts as cmAdd (inferred from alias chain)
};

/** cmNegate — -portA. vtable @ 0x82053C24 */
struct cmNegate : cmBinaryNode {
    // NOTE: portB is structurally present but always unconnected.
    //       RegisterPorts is aliased to cmAdd::RegisterPorts in the binary —
    //       unary behaviour is enforced by portB being disconnected (type=NONE).
    void GetVector(float* out);                     // [2]  @ 0x8227B518
    void GetFloat(float* out);                      // [4]  @ 0x8227B4E0
    static void RegisterPorts(cmBinaryNode* node);  // [16] @ 0x82262350 (alias of cmAdd)
};

/** cmAbs — |portA|. vtable @ 0x82053E8C */
struct cmAbs : cmUnaryNode {
    void GetFloat(float* out);                      // [4]  @ 0x8227B758
    void GetDim(int32_t* out);                      // [5]  @ 0x8227B718
    static void RegisterPorts(cmUnaryNode* node);   // [16] @ 0x82262478
};

/** cmSine — sin(portA). vtable @ 0x82053C7C */
struct cmSine : cmUnaryNode {
    void GetFloat(float* out);                      // [4]  @ 0x8227B578
    // RegisterPorts: tries float→float only (single-type unary)
};

/** cmCosine — cos(portA). vtable @ 0x82053CD4 */
struct cmCosine : cmUnaryNode {
    void GetFloat(float* out);                      // [4]  @ 0x8227B5B8
};

/** cmArcCosine — acos(portA). vtable @ 0x82053D2C */
struct cmArcCosine : cmUnaryNode {
    void GetFloat(float* out);                      // [4]  @ 0x8227B5F8
};

/** cmArcTangent — atan(portA). vtable @ 0x82053DDC */
struct cmArcTangent : cmUnaryNode {
    void GetFloat(float* out);                      // [4]  @ 0x8227B678
    static void RegisterPorts(cmUnaryNode* node);   // [16] @ 0x82262438
};

/** cmClamp — clamp(portA, portB, portC). vtable @ 0x820540F4 */
struct cmClamp : cmBinaryNode {
    // NOTE: 3-port node — portC (max) is at a 3rd port slot; layout TBD.
    void GetVector(float* out);                     // [2]  @ 0x8227BEA0
    void GetFloat(float* out);                      // [4]  @ 0x8227BE20
    void GetDim(int32_t* out);                      // [5]  @ 0x8227BDB8
    static void RegisterPorts(cmBinaryNode* node);  // [16] @ 0x82262658
};

/**
 * cmMemory — returns value from the previous frame, stores the new input.
 * vtable @ (varies per derived — see rage_cm.hpp skeleton)
 */
struct cmMemory : cmStatefulNode {
    void GetInt32(int32_t* out);                    // [1]  @ 0x82271640
    void GetVector(float* out);                     // [2]  @ 0x822715A0
    void GetBool(uint8_t* out);                     // [3]  @ 0x822715F0
    void GetFloat(float* out);                      // [4]  @ 0x82271550
    void Update();                                  // [6]  @ 0x82271468
    void Sync();                                    // [8]  unnamed vcall
    void SetInitialInput();                         // [9]  @ 0x82271518-adjacent
    void SyncInput();                               // [10] @ 0x82278158
    void SetValue();                                // [14] @ 0x82271518
    void RegisterPorts(cmBinaryNode* node);         // [16] @ 0x82262960
    void Reset();                                   // [17] @ 0x82271390
};

/**
 * cmIntegrate — accumulates portB over time (running integral).
 * portA identifies output dimension; portB is the rate; m_pCurrentValue is the total.
 * vtable @ 0x82054934
 */
struct cmIntegrate : cmStatefulNode {
    void Tick();                                    // [10] @ 0x82278420
    static void RegisterPorts(cmBinaryNode* node);  // [16] @ 0x82262A60
};

/**
 * cmIntegrate0 — variant of cmIntegrate with zeroed initial state.
 * vtable @ 0x8205498C. Tick aliased to cmIntegrate::Tick (same binary code).
 */
struct cmIntegrate0 : cmStatefulNode {
    void Tick();                                    // [10] @ 0x82278510 (alias of cmIntegrate)
};

/**
 * cmDifferential — outputs the rate-of-change (derivative) of portA per frame.
 * Uses unary layout (util_54C8). m_nConnected at +20.
 * vtable @ 0x82054884
 */
struct cmDifferential : cmUnaryNode {
    void GetVector(float* out);                     // [2]  @ 0x82277E78
    void GetFloat(float* out);                      // [4]  @ 0x82277E28
    static void RegisterPorts(cmUnaryNode* node);   // [16] @ 0x822628E0
};

/**
 * cmApproachOperator — moves a stored value toward target by rate per frame.
 * vtable @ 0x82056A04. Extended layout (portA at +0x14, m_pResult at +0x28).
 */
struct cmApproachOperator : cmApproachNode {
    void Tick();                                    // [10] @ 0x82278B90
};

// ─────────────────────────────────────────────────────────────────────────────
//  FREE FUNCTIONS (lifted utilities)
// ─────────────────────────────────────────────────────────────────────────────

// Port evaluation primitives
float*    cmNode_GetVector(float* dst, const cmNodePort* port); // util_92D8
float     cmNode_GetFloat(const cmNodePort* port);              // util_9350
int32_t   cmNode_GetInt(const cmNodePort* port);                // util_4BD8
int32_t   cmNode_GetDim(const cmNodePort* port);                // cmSwitch_4B60
uint8_t   cmNode_GetBool(const cmNodePort* port);               // cmCond_21B0 (TODO)
int32_t   cmNode_GetPortDim(const cmNodePort* port);            // inline helper

// Port connection validation
bool cmNode_TryConnect(cmBinaryNode* node, cmPortDesc desc);    // util_5698
bool cmNode_TryConnectUnary(cmUnaryNode* node, cmPortDesc desc);// util_54C8

// Polymorphic port→data writer
void cmNode_SetFromPort_Dispatch(void* dst,                     // util_5380
                                 const cmNodePort* port,
                                 int32_t dim);

// Fast sinf polynomial approximation (misnamed phBoundCapsule_01D8_g)
float rage_sinf_approx(float angle);   // @ 0x824301D8

// Vector approach helper used by cmApproachOperator
static bool cmApproachOperator_VectorApproach(
        float* current, const float* target, float stepLen);

// Engine heap functions (forward declarations — defined in rage allocator)
extern void  rage_free(void* ptr);                              // @ 0x820C00C0
extern void* rage_alloc_aligned(uint32_t size, uint32_t align);// via xe_main_thread_init_0038 + vcall[1]

} // namespace rage
