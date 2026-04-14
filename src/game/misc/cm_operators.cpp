/**
 * cm_operators.cpp — Conditional Machine operators and constructors
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * The "cm" subsystem implements a data-flow graph for game logic conditions,
 * reporters, and operators. These classes are template-instantiated constructors
 * that build operator nodes at runtime.
 */

#include "cm_operators.hpp"
#include <stdint.h>
#include <stddef.h>

// Forward declarations
extern "C" void sysMemAllocator_InitMainThread(void);

// External vtable addresses
extern "C" const void* rage_cmIntegrate_vtable;  // @ 0x82054934

// SDA base - points to allocator context
extern uint32_t* g_sda_base;  // @ 0x82600000 (r13)


/**
 * cmWorldRefreshableCtor::Destructor @ 0x82268438 | size: 0xa0
 *
 * Constructor variant that allocates and initializes a rage::cmIntegrate object.
 * This is one of 18 template instantiations of cmWorldRefreshableCtor for different
 * operator types.
 *
 * The function:
 * 1. Ensures thread context is initialized
 * 2. Allocates 40 bytes with 16-byte alignment from the RAGE allocator
 * 3. Zero-initializes the object
 * 4. Sets the vtable to rage::cmIntegrate
 * 5. Clears the "initialized" flag bit at offset +8
 *
 * @return Pointer to the newly constructed rage::cmIntegrate object, or nullptr on failure
 */
void* cmWorldRefreshableCtor_Destructor()
{
    // Initialize main thread context if not already done
    sysMemAllocator_InitMainThread();
    
    // Get allocator context from SDA base
    // SDA offset 0 points to allocator context struct
    uint32_t* allocatorCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    
    // Get allocator object from context offset +4
    void** allocator = (void**)(uintptr_t)allocatorCtx[1];
    
    // Call allocator->Allocate(40, 16) via vtable slot 1
    typedef void* (*AllocateFn)(void* self, uint32_t size, uint32_t alignment);
    AllocateFn allocate = (AllocateFn)allocator[1];
    void* obj = allocate(allocator, 40, 16);
    
    if (obj != nullptr) {
        // Zero-initialize the object fields
        uint32_t* fields = (uint32_t*)obj;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28 - set to 2 (likely a type/state enum)
        fields[8] = 0;  // +32
        fields[9] = 0;  // +36
        
        // Set vtable pointer to rage::cmIntegrate
        fields[0] = (uint32_t)(uintptr_t)&rage_cmIntegrate_vtable;
        
        // Clear bit 0 of the field at offset +8 (clear "initialized" flag)
        fields[2] = fields[2] & ~1u;
    }
    
    return obj;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmLookup — Lookup table operator (2-entry and 3-entry variants)
// ─────────────────────────────────────────────────────────────────────────────

// Forward declarations for utility functions
extern "C" float cmOperator_EvalFloat(void* port);    // Evaluates a cmPort and returns float
extern "C" int cmSwitch_4B60(void* port);              // Evaluates a cmPort and returns int
extern "C" int cmNode_GetInt(void* port);              // Evaluates a cmPort and returns int
extern "C" bool cmCond_21B0(void* port);               // Evaluates a cmPort as bool
extern "C" void cmNode_GetVector(void* dest, void* port);  // Evaluates a cmPort into vector
extern "C" void cmCond_1038_g(void* port, void* dest);     // Match+CopyState via port

/**
 * cmLookup::RegisterPorts @ 0x8226D840 | size: 0x40
 * 
 * Retrieves cached result based on mode.
 * Mode 1: Returns byte from offset +16 of object at +28
 * Mode 2: Returns int from offset +4 of object at +28
 * Other: Clears cached result to 0
 */
void cmLookup::RegisterPorts() {
    int32_t portType = m_entries[0].valuePort.m_type;  // +0x20

    if (portType == 1) {
        // DIRECT: read byte at offset +16 of data object (cmDataObj dim field)
        uint8_t* pData = (uint8_t*)(uintptr_t)m_entries[0].valuePort.m_pData;
        m_cachedResult = pData[16];
    } else if (portType == 2) {
        // NODE: read m_cachedResult (+4) of upstream cmOperator node
        uint32_t* pNode = (uint32_t*)(uintptr_t)m_entries[0].valuePort.m_pData;
        m_cachedResult = pNode[1];  // upstream->m_cachedResult
    } else {
        m_cachedResult = 0;
    }
}

/**
 * cmLookup::GetDim @ 0x8226D880 | size: 0x9c
 * 
 * Evaluates lookup table and returns matching int result (2-entry variant).
 * Compares key value against two threshold entries and returns the first match,
 * or default value if no match.
 */
void cmLookup::GetDim(int* outResult) {
    // Evaluate the key value
    float keyValue = cmOperator_EvalFloat(&m_keyPort);  // +0x0C

    // Check each entry (2 entries)
    for (int i = 0; i < 2; i++) {
        float threshold = cmOperator_EvalFloat(&m_entries[i].condPort);  // threshold port

        if (keyValue <= threshold) {
            // Found match - evaluate and return this entry's result
            *outResult = cmSwitch_4B60(&m_entries[i].valuePort);
            return;
        }
    }

    // No match - return default value (2-entry: default port overlaps entries[2])
    cmPort* defaultPort = (cmPort*)&m_entries[2];
    *outResult = cmSwitch_4B60(defaultPort);
}

/**
 * cmLookup::GetFloat @ 0x8226D920 | size: 0x9c
 *
 * Evaluates lookup table and returns matching float result (2-entry variant).
 */
void cmLookup::GetFloat(float* outResult) {
    float keyValue = cmOperator_EvalFloat(&m_keyPort);

    for (int i = 0; i < 2; i++) {
        float threshold = cmOperator_EvalFloat(&m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmOperator_EvalFloat(&m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&m_entries[2];
    *outResult = cmOperator_EvalFloat(defaultPort);
}

/**
 * cmLookup::GetBool @ 0x8226DA78 | size: 0x9c
 *
 * Evaluates lookup table and returns matching bool result (2-entry variant).
 */
void cmLookup::GetBool(bool* outResult) {
    float keyValue = cmOperator_EvalFloat(&m_keyPort);

    for (int i = 0; i < 2; i++) {
        float threshold = cmOperator_EvalFloat(&m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmCond_21B0(&m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&m_entries[2];
    *outResult = cmCond_21B0(defaultPort);
}

/**
 * cmLookup::GetVector @ 0x8226D9C0 | size: 0xb4
 *
 * Evaluates lookup table and returns matching vector result (2-entry variant).
 * Uses VMX128 vector operations for 16-byte data.
 */
void cmLookup::GetVector(void* outVector) {
    float keyValue = cmOperator_EvalFloat(&m_keyPort);

    for (int i = 0; i < 2; i++) {
        float threshold = cmOperator_EvalFloat(&m_entries[i].condPort);

        if (keyValue <= threshold) {
            cmNode_GetVector(outVector, &m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&m_entries[2];
    cmNode_GetVector(outVector, defaultPort);
}

/**
 * cmLookup::GetDimValue @ 0x8226DB18 | size: 0x9c
 * 
 * Scalar destructor - evaluates lookup table and returns matching int result (2-entry variant).
 * Similar to GetDim but uses cmNode_GetInt instead of cmSwitch_4B60.
 */
void cmLookup::GetDimValue(int* outResult) {
    float keyValue = cmOperator_EvalFloat(&m_keyPort);

    for (int i = 0; i < 2; i++) {
        float threshold = cmOperator_EvalFloat(&m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmNode_GetInt(&m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&m_entries[2];
    *outResult = cmNode_GetInt(defaultPort);
}

/**
 * cmLookup::CopyState @ 0x82270D10 | size: 0x3c
 * 
 * Evaluates all 8 condition entries sequentially.
 * Used for batch evaluation of multiple conditions.
 */
void cmLookup::CopyState(void* dest) {
    // Iterate all ports starting at m_keyPort (+0x0C), 8 ports x 8 bytes each
    cmPort* port = &m_keyPort;

    for (int i = 0; i < 8; i++) {
        cmCond_1038_g(port, dest);
        port++;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// cmLookup 3-entry variants (check 3 thresholds instead of 2)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmLookup_GetDim_DBB8_1 @ 0x8226DBB8 | size: 0x9c
 * 
 * 3-entry variant: Returns matching int result from 3-entry lookup table.
 */
void cmLookup_GetDim_DBB8_1(void* self, int* outResult) {
    cmLookup* node = (cmLookup*)self;
    float keyValue = cmOperator_EvalFloat(&node->m_keyPort);

    for (int i = 0; i < 3; i++) {
        float threshold = cmOperator_EvalFloat(&node->m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmSwitch_4B60(&node->m_entries[i].valuePort);
            return;
        }
    }

    // Default port at +0x44 for 3-entry table
    cmPort* defaultPort = &node->m_defaultPort;
    *outResult = cmSwitch_4B60(defaultPort);
}

/**
 * cmLookup_GetFloat_DC58_1 @ 0x8226DC58 | size: 0x9c
 * 
 * 3-entry variant: Returns matching float result from 3-entry lookup table.
 */
void cmLookup_GetFloat_DC58_1(void* self, float* outResult) {
    cmLookup* node = (cmLookup*)self;
    float keyValue = cmOperator_EvalFloat(&node->m_keyPort);

    for (int i = 0; i < 3; i++) {
        float threshold = cmOperator_EvalFloat(&node->m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmOperator_EvalFloat(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = &node->m_defaultPort;
    *outResult = cmOperator_EvalFloat(defaultPort);
}

/**
 * cmLookup_GetBool_DDB0_1 @ 0x8226DDB0 | size: 0x9c
 * 
 * 3-entry variant: Returns matching bool result from 3-entry lookup table.
 */
void cmLookup_GetBool_DDB0_1(void* self, bool* outResult) {
    cmLookup* node = (cmLookup*)self;
    float keyValue = cmOperator_EvalFloat(&node->m_keyPort);

    for (int i = 0; i < 3; i++) {
        float threshold = cmOperator_EvalFloat(&node->m_entries[i].condPort);

        if (keyValue <= threshold) {
            *outResult = cmCond_21B0(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = &node->m_defaultPort;
    *outResult = cmCond_21B0(defaultPort);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor factory functions
//
// These are virtual method overrides on the 113 cmOperatorCtor vtable
// instantiations. Each allocates memory via the TLS allocator (slot 1)
// and tail-calls the class-specific init/constructor function.
// Pattern: sysMemAllocator_InitMainThread() → get allocator → Allocate(size, 16)
//          → if non-null, call init → return constructed object or nullptr
// ─────────────────────────────────────────────────────────────────────────────

// Init functions called by each factory
extern "C" void* atSingleton_9560_2hr(void* mem);   // → pongInlineCinematics ctor
extern "C" void* phJoint3Dof_8BE8_fw(void* mem);    // → plrPlayerMgr ctor
extern "C" void* ph_ctor_01D0(void* mem);            // → gdCrData ctor
extern "C" void* atSingleton_2560_2hr(void* mem);    // → gdCharPlayerBias ctor
extern "C" void* phJoint3Dof_AA28_w(void* mem);      // → gdLevelData ctor
extern "C" void* ph_ctor_FEC8(void* mem);            // → gdNetData ctor
extern "C" void* atSingleton_E050_2hr(void* mem);    // → gdPlyrLimits ctor
extern "C" void* ph_ctor_FB70(void* mem);            // → lvlLevelMgr ctor
extern "C" void* ph_09B8(void* mem);                 // → gmBallNode ctor
extern "C" void* atSingleton_B358_p46(void* mem);    // → pongCharViewContext ctor

typedef void* (*AllocFn)(void* self, uint32_t size, uint32_t alignment);

/**
 * Helper: allocate from TLS heap with 16-byte alignment.
 * Used by all cmOperatorCtor factory functions below.
 */
static void* cmOperatorCtor_Allocate(uint32_t size) {
    sysMemAllocator_InitMainThread();
    uint32_t* sdaCtx = (uint32_t*)(uintptr_t)g_sda_base[0];
    void** allocator = (void**)(uintptr_t)sdaCtx[1];
    AllocFn alloc = (AllocFn)allocator[1];
    return alloc(allocator, size, 16);
}

// ── pongInlineCinematics factory ─────────────────────────────────────────────

/**
 * cmOperatorCtor_94C0_w @ 0x821694C0 | size: 0x64
 *
 * Allocates 12512 bytes and constructs a pongInlineCinematics object.
 */
// cmOperatorCtor_94C0_w @ 0x821694C0
void* cmOperatorCtor_pongInlineCinematics() {
    void* mem = cmOperatorCtor_Allocate(12512);
    if (mem != nullptr) {
        return atSingleton_9560_2hr(mem);
    }
    return nullptr;
}

// ── plrPlayerMgr factory ─────────────────────────────────────────────────────

/**
 * cmOperatorCtor_8B80_w @ 0x82188B80 | size: 0x64
 *
 * Allocates 300 bytes and constructs a plrPlayerMgr object.
 */
// cmOperatorCtor_8B80_w @ 0x82188B80
void* cmOperatorCtor_plrPlayerMgr() {
    void* mem = cmOperatorCtor_Allocate(300);
    if (mem != nullptr) {
        return phJoint3Dof_8BE8_fw(mem);
    }
    return nullptr;
}

// ── gdCrData factory ─────────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0AF0_w @ 0x821D0AF0 | size: 0x64
 *
 * Allocates 368 bytes and constructs a gdCrData object.
 */
// cmOperatorCtor_0AF0_w @ 0x821D0AF0
void* cmOperatorCtor_gdCrData() {
    void* mem = cmOperatorCtor_Allocate(368);
    if (mem != nullptr) {
        return ph_ctor_01D0(mem);
    }
    return nullptr;
}

// ── gdCharPlayerBias factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_2878_w @ 0x821D2878 | size: 0x64
 *
 * Allocates 160 bytes and constructs a gdCharPlayerBias object.
 */
// cmOperatorCtor_2878_w @ 0x821D2878
void* cmOperatorCtor_gdCharPlayerBias() {
    void* mem = cmOperatorCtor_Allocate(160);
    if (mem != nullptr) {
        return atSingleton_2560_2hr(mem);
    }
    return nullptr;
}

// ── gdLevelData factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_AE88_w @ 0x821DAE88 | size: 0x64
 *
 * Allocates 5936 bytes and constructs a gdLevelData object.
 */
// cmOperatorCtor_AE88_w @ 0x821DAE88
void* cmOperatorCtor_gdLevelData() {
    void* mem = cmOperatorCtor_Allocate(5936);
    if (mem != nullptr) {
        return phJoint3Dof_AA28_w(mem);
    }
    return nullptr;
}

// ── gdNetData factory ────────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0030_w @ 0x821F0030 | size: 0x64
 *
 * Allocates 144 bytes and constructs a gdNetData object.
 */
// cmOperatorCtor_0030_w @ 0x821F0030
void* cmOperatorCtor_gdNetData() {
    void* mem = cmOperatorCtor_Allocate(144);
    if (mem != nullptr) {
        return ph_ctor_FEC8(mem);
    }
    return nullptr;
}

// ── gdPlyrLimits factory ─────────────────────────────────────────────────────

/**
 * cmOperatorCtor_E338_w @ 0x8220E338 | size: 0x64
 *
 * Allocates 148 bytes and constructs a gdPlyrLimits object.
 */
// cmOperatorCtor_E338_w @ 0x8220E338
void* cmOperatorCtor_gdPlyrLimits() {
    void* mem = cmOperatorCtor_Allocate(148);
    if (mem != nullptr) {
        return atSingleton_E050_2hr(mem);
    }
    return nullptr;
}

// ── lvlLevelMgr factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_FDC0_w @ 0x8223FDC0 | size: 0x64
 *
 * Allocates 100 bytes and constructs a lvlLevelMgr object.
 */
// cmOperatorCtor_FDC0_w @ 0x8223FDC0
void* cmOperatorCtor_lvlLevelMgr() {
    void* mem = cmOperatorCtor_Allocate(100);
    if (mem != nullptr) {
        return ph_ctor_FB70(mem);
    }
    return nullptr;
}

// ── gmBallNode factory ───────────────────────────────────────────────────────

/**
 * cmOperatorCtor_0950_w @ 0x82280950 | size: 0x64
 *
 * Allocates 336 bytes and constructs a gmBallNode object.
 */
// cmOperatorCtor_0950_w @ 0x82280950
void* cmOperatorCtor_gmBallNode() {
    void* mem = cmOperatorCtor_Allocate(336);
    if (mem != nullptr) {
        return ph_09B8(mem);
    }
    return nullptr;
}

// ── pongCharViewContext factory ──────────────────────────────────────────────

/**
 * cmOperatorCtor_A598_w @ 0x8230A598 | size: 0x64
 *
 * Allocates 388 bytes and constructs a pongCharViewContext object.
 */
// cmOperatorCtor_A598_w @ 0x8230A598
void* cmOperatorCtor_pongCharViewContext() {
    void* mem = cmOperatorCtor_Allocate(388);
    if (mem != nullptr) {
        return atSingleton_B358_p46(mem);
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmCond — Conditional node (if/elseif/else selection)
//
// Layout: each cmCond variant holds N condition/value port pairs followed
// by one default value port. Entries are 16 bytes each (8-byte cond port +
// 8-byte value port). The condition port is evaluated as bool; the first
// true condition selects its paired value port for output.
//
//   +0x00  vtable
//   +0x04  m_outputType (set by vfn_16 / RegisterPorts)
//   +0x08  m_flags
//   +0x0C  entries[0].condPort   (8 bytes)
//   +0x14  entries[0].valuePort  (8 bytes)
//   +0x1C  entries[1].condPort   ...
//   ...
//   +end   defaultPort           (8 bytes)
//
// 5 vtable instantiations: 2-cond, 3-cond, 4-cond, 5-cond, 6-cond
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmCond_21B0 @ 0x822621B0 | size: 0x78
 *
 * Core bool evaluator for a condition port pair (8 bytes: pData + type).
 *   type==1 (DIRECT): read bool byte directly from pData[0]
 *   type==2 (NODE):   call upstream node->GetBool(slot 3) and return result
 *   otherwise:        return false
 */
extern "C" bool cmCond_21B0(void* condPort) {
    cmPort* port = (cmPort*)condPort;

    if (port->m_type == 1) {
        // DIRECT: read bool byte at offset 0 of the data object
        void* pData = (void*)(uintptr_t)port->m_pData;
        return *(uint8_t*)pData != 0;
    } else if (port->m_type == 2) {
        // NODE: call upstream node->GetBool via vtable slot 3
        void* pNode = (void*)(uintptr_t)port->m_pData;
        void** vtbl = *(void***)pNode;
        typedef void (*VFn_GetBool)(void* self, uint8_t* out);
        uint8_t tmp;
        ((VFn_GetBool)vtbl[3])(pNode, &tmp);
        return tmp != 0;
    }

    return false;
}

/**
 * cmCond_1038_g @ 0x82271038 | size: 0x78
 *
 * Evaluates a condition port pair. If the condition is a NODE (type==2),
 * calls a match function on the node (dest vtable slot 3 with the node).
 * If the match succeeds, calls the node's CopyState (vtable slot 18)
 * passing dest.
 */
extern "C" void cmCond_1038_g(void* condPort, void* dest) {
    cmPort* port = (cmPort*)condPort;

    if (port->m_type != 2) return;

    void* pNode = (void*)(uintptr_t)port->m_pData;

    // dest->Match(pNode) via vtable slot 3
    void** destVtbl = *(void***)dest;
    typedef bool (*VFn_Match)(void* self, void* node);
    bool matched = ((VFn_Match)destVtbl[3])(dest, pNode);

    if (!matched) return;

    // pNode->CopyState(dest) via vtable slot 18
    void** nodeVtbl = *(void***)pNode;
    typedef void (*VFn_CopyState)(void* self, void* dest);
    ((VFn_CopyState)nodeVtbl[18])(pNode, dest);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmCond — 2-condition vtable (vtable @ 0x820557FC)
//   entries[0..1] at offsets +12..+43, default at +44
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmCond_vfn_16 @ 0x8226CC98 | size: 0x40
 *
 * RegisterPorts — reads the connected port at +20 to determine
 * the output dimension type (m_outputType at +4).
 */
void cmCond_vfn_16(void* self) {
    cmCond* node = (cmCond*)self;
    cmPort* valuePort = &node->m_entries[0].valuePort;

    if (valuePort->m_type == 1) {
        // DIRECT: read dim from cmDataObj at +16
        uint8_t* pData = (uint8_t*)(uintptr_t)valuePort->m_pData;
        node->m_outputType = (uint32_t)pData[16];
    } else if (valuePort->m_type == 2) {
        // NODE: read m_outputType at +4 of upstream node
        uint32_t* pNode = (uint32_t*)(uintptr_t)valuePort->m_pData;
        node->m_outputType = pNode[1];
    } else {
        node->m_outputType = 0;
    }
}

/**
 * cmCond_vfn_5 @ 0x8226CCD8 | size: 0x70
 *
 * GetDim — evaluates 2-cond node, returns int dimension from matching port.
 */
void cmCond_vfn_5(void* self, int32_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 2; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmSwitch_4B60(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[2];
    *out = cmSwitch_4B60(defaultPort);
}

/**
 * cmCond_vfn_4 @ 0x8226CD48 | size: 0x70
 *
 * GetFloat — evaluates 2-cond node, returns float from matching port.
 */
void cmCond_vfn_4(void* self, float* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 2; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmOperator_EvalFloat(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[2];
    *out = cmOperator_EvalFloat(defaultPort);
}

/**
 * cmCond_vfn_3 @ 0x8226CDB8 | size: 0x70
 *
 * GetBool — evaluates 2-cond node, returns bool from matching port.
 */
void cmCond_vfn_3(void* self, uint8_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 2; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = (uint8_t)cmCond_21B0(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[2];
    *out = (uint8_t)cmCond_21B0(defaultPort);
}

/**
 * cmCond_vfn_2 @ 0x8226CE28 | size: 0x88
 *
 * GetVector — evaluates 2-cond node, returns vec4 from matching port.
 */
void cmCond_vfn_2(void* self, void* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 2; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            cmNode_GetVector(out, &node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[2];
    cmNode_GetVector(out, defaultPort);
}

/**
 * cmCond_vfn_1 @ 0x8226CEB0 | size: 0x70
 *
 * GetInt32 — evaluates 2-cond node, returns int32 from matching port.
 */
void cmCond_vfn_1(void* self, int32_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 2; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmNode_GetInt(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[2];
    *out = cmNode_GetInt(defaultPort);
}

/**
 * cmCond_vfn_18 @ 0x82270C10 | size: 0x3c
 *
 * CopyState — iterates all 5 ports (2 cond + 2 value + 1 default)
 * starting at +12, stepping by 8 bytes, calling cmCond_1038_g on each.
 */
void cmCond_vfn_18(void* self, void* dest) {
    cmCond* node = (cmCond*)self;
    // 2-cond: 5 ports = 2 cond + 2 value + 1 default, each 8 bytes
    cmPort* port = (cmPort*)&node->m_entries[0];

    for (int i = 0; i < 5; i++) {
        cmCond_1038_g(&port[i], dest);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// cmCond — 3-condition vtable (vtable @ 0x82055854)
//   entries[0..2] at offsets +12..+59, default at +60
// ─────────────────────────────────────────────────────────────────────────────

/**
 * cmCond_vfn_5_CF20_1 @ 0x8226CF20 | size: 0x70
 *
 * GetDim — 3-cond variant.
 */
void cmCond_vfn_5_CF20_1(void* self, int32_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 3; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmSwitch_4B60(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[3];
    *out = cmSwitch_4B60(defaultPort);
}

/**
 * cmCond_vfn_4_CF90_1 @ 0x8226CF90 | size: 0x70
 *
 * GetFloat — 3-cond variant.
 */
void cmCond_vfn_4_CF90_1(void* self, float* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 3; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmOperator_EvalFloat(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[3];
    *out = cmOperator_EvalFloat(defaultPort);
}

/**
 * cmCond_vfn_3_D000_1 @ 0x8226D000 | size: 0x70
 *
 * GetBool — 3-cond variant.
 */
void cmCond_vfn_3_D000_1(void* self, uint8_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 3; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = (uint8_t)cmCond_21B0(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[3];
    *out = (uint8_t)cmCond_21B0(defaultPort);
}

/**
 * cmCond_vfn_2_D070_1 @ 0x8226D070 | size: 0x88
 *
 * GetVector — 3-cond variant.
 */
void cmCond_vfn_2_D070_1(void* self, void* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 3; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            cmNode_GetVector(out, &node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[3];
    cmNode_GetVector(out, defaultPort);
}

/**
 * cmCond_vfn_1_D0F8_1 @ 0x8226D0F8 | size: 0x70
 *
 * GetInt32 — 3-cond variant.
 */
void cmCond_vfn_1_D0F8_1(void* self, int32_t* out) {
    cmCond* node = (cmCond*)self;

    for (int i = 0; i < 3; i++) {
        if (cmCond_21B0(&node->m_entries[i].condPort)) {
            *out = cmNode_GetInt(&node->m_entries[i].valuePort);
            return;
        }
    }

    cmPort* defaultPort = (cmPort*)&node->m_entries[3];
    *out = cmNode_GetInt(defaultPort);
}

/**
 * cmCond_vfn_18_0C50_1 @ 0x82270C50 | size: 0x3c
 *
 * CopyState — 3-cond variant, iterates 7 ports.
 */
void cmCond_vfn_18_0C50_1(void* self, void* dest) {
    cmCond* node = (cmCond*)self;
    // 3-cond: 7 ports = 3 cond + 3 value + 1 default, each 8 bytes
    cmPort* port = (cmPort*)&node->m_entries[0];

    for (int i = 0; i < 7; i++) {
        cmCond_1038_g(&port[i], dest);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor factory functions — batch 2
//
// Additional factory constructors covering:
//  - Simple allocate+init factories (0x64 pattern)
//  - Inline vtable-set constructors (0x78 pattern: alloc, zero, set vtable)
//  - Pass-through factories with extra arguments
//  - Miscellaneous cm operator helpers
// ─────────────────────────────────────────────────────────────────────────────

// Init functions called by batch 2 factories
extern "C" void* ph_ctor_97F0(void* mem);                    // → fxLightShaftTune ctor
extern "C" void* ke_6E80(void* mem, void* a1, void* a2, void* a3);  // → fragDrawable ctor
extern "C" void* pongNetMessageHolder_89B8_w(void* mem, void* a1);   // → pongNetMessageHolder ctor
extern "C" double jumptable_2B80(void* obj);                 // → evaluates float from object
extern "C" void hudFlashBase_76F8_g(void* mgr, int a1, void* a2);   // → HUD flash base handler

// External vtable addresses for inline constructors
extern "C" const void* rage_cmNoise_vtable;           // @ 0x82054464
extern "C" const void* rage_cmVectorNoise_vtable;     // @ 0x820544BC
extern "C" const void* rage_cmSphericalNoise_vtable;  // @ 0x82054514
extern "C" const void* rage_cmGNoise_vtable;          // @ 0x8205456C
extern "C" const void* rage_cmVectorGNoise_vtable;    // @ 0x820545C4
extern "C" const void* rage_cmInfo_vtable;            // @ 0x82054BF4
extern "C" const void* rage_cmDevArg_vtable;          // @ 0x82054CA4
extern "C" const void* rage_cmLocalRestart_vtable;    // @ 0x82054D54
extern "C" const void* rage_cmDifferential_vtable;    // @ 0x82054884

// Global singleton pointers (SDA-relative)
extern "C" void* g_sda_6404;   // @ 0x82606404 — singleton manager object
extern "C" uint8_t g_sda_660D; // @ 0x8260660D — flag byte

// ── fxLightShaftTune factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_9978_w @ 0x82379978 | size: 0x64
 *
 * Allocates 192 bytes and constructs an fxLightShaftTune object.
 */
// cmOperatorCtor_9978_w @ 0x82379978
void* cmOperatorCtor_fxLightShaftTune() {
    void* mem = cmOperatorCtor_Allocate(192);
    if (mem != nullptr) {
        return ph_ctor_97F0(mem);
    }
    return nullptr;
}

// ── rage::cmNoise inline constructor ─────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7BB8_1 @ 0x82267BB8 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmNoise.
 */
// cmOperatorCtor_vfn_0_7BB8_1 @ 0x82267BB8
void* cmOperatorCtor_cmNoise() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmNoise_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmVectorNoise inline constructor ───────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7C30_1 @ 0x82267C30 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmVectorNoise.
 */
// cmOperatorCtor_vfn_0_7C30_1 @ 0x82267C30
void* cmOperatorCtor_cmVectorNoise() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmVectorNoise_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSphericalNoise inline constructor ────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7CA8_1 @ 0x82267CA8 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmSphericalNoise.
 */
// cmOperatorCtor_vfn_0_7CA8_1 @ 0x82267CA8
void* cmOperatorCtor_cmSphericalNoise() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSphericalNoise_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmGNoise inline constructor ────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7D20_1 @ 0x82267D20 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmGNoise.
 */
// cmOperatorCtor_vfn_0_7D20_1 @ 0x82267D20
void* cmOperatorCtor_cmGNoise() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmGNoise_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmVectorGNoise inline constructor ──────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7D98_1 @ 0x82267D98 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmVectorGNoise.
 */
// cmOperatorCtor_vfn_0_7D98_1 @ 0x82267D98
void* cmOperatorCtor_cmVectorGNoise() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmVectorGNoise_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmLocalRestart inline constructor ──────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_8F08_1 @ 0x82268F08 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmLocalRestart.
 */
// cmOperatorCtor_vfn_0_8F08_1 @ 0x82268F08
void* cmOperatorCtor_cmLocalRestart() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmLocalRestart_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmDevArg inline constructor ────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_94F0_1 @ 0x822694F0 | size: 0x78
 *
 * Allocates 20 bytes, zero-initializes, and sets vtable to rage::cmDevArg.
 */
// cmOperatorCtor_vfn_0_94F0_1 @ 0x822694F0
void* cmOperatorCtor_cmDevArg() {
    void* mem = cmOperatorCtor_Allocate(20);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmDevArg_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmInfo inline constructor ──────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_9608_1 @ 0x82269608 | size: 0x78
 *
 * Allocates 12 bytes, zero-initializes, and sets vtable to rage::cmInfo.
 */
// cmOperatorCtor_vfn_0_9608_1 @ 0x82269608
void* cmOperatorCtor_cmInfo() {
    void* mem = cmOperatorCtor_Allocate(12);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[0] = (uint32_t)(uintptr_t)&rage_cmInfo_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmDifferential inline constructor ──────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7E10_1 @ 0x82267E10 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets field+20 to 1,
 * and sets vtable to rage::cmDifferential.
 */
// cmOperatorCtor_vfn_0_7E10_1 @ 0x82267E10
void* cmOperatorCtor_cmDifferential() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20 — initial state/mode value
        fields[0] = (uint32_t)(uintptr_t)&rage_cmDifferential_vtable;
        return mem;
    }
    return nullptr;
}

// ── fragDrawable pass-through factory ────────────────────────────────────────

/**
 * cmOperatorCtor_4B68_w @ 0x82154B68 | size: 0x74
 *
 * Allocates 228 bytes and constructs a fragDrawable object,
 * forwarding 4 arguments to the init function.
 */
// cmOperatorCtor_4B68_w @ 0x82154B68
void* cmOperatorCtor_fragDrawable(void* a1, void* a2, void* a3) {
    void* mem = cmOperatorCtor_Allocate(228);
    if (mem != nullptr) {
        return ke_6E80(mem, a1, a2, a3);
    }
    return nullptr;
}

// ── pongNetMessageHolder pass-through factory ────────────────────────────────

/**
 * cmOperatorCtor_8BD0_w @ 0x82158BD0 | size: 0x78
 *
 * Allocates 24 bytes and constructs a pongNetMessageHolder object,
 * forwarding 2 arguments to the init function.
 */
// cmOperatorCtor_8BD0_w @ 0x82158BD0
void* cmOperatorCtor_pongNetMessageHolder(void* a1) {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        return pongNetMessageHolder_89B8_w(mem, a1);
    }
    return nullptr;
}

// ── cmOperatorCtor float evaluator ───────────────────────────────────────────

/**
 * cmOperatorCtor_6690 @ 0x82186690 | size: 0x68
 *
 * Evaluates a float value from the object at offset +4.
 * Returns 1 if the object is valid and has a non-zero first byte,
 * storing the evaluated float into outResult. Returns 0 otherwise.
 */
// cmOperatorCtor_6690 @ 0x82186690
int cmOperatorCtor_EvalFloat(void* self, float* outResult) {
    // self is a cmPort-like struct: {vtable/ptr, pObj}
    uint32_t* fields = (uint32_t*)self;
    void* obj = (void*)(uintptr_t)fields[1];  // +4: pointer to evaluatable object
    if (obj != nullptr) {
        uint8_t flag = *(uint8_t*)obj;
        if (flag != 0) {
            *outResult = (float)jumptable_2B80(obj);
            return 1;
        }
    }
    return 0;
}

// ── Singleton manager vtable dispatch helpers ────────────────────────────────

/**
 * cmOperatorCtor_C220_2hr @ 0x822EC220 | size: 0x74
 *
 * Calls vtable slot 2 (Register) on the global singleton manager with
 * the given object. If the global flag is set, also calls vtable slot 13
 * (SetEnabled) with args (1, 1) on the result.
 *
 * @param obj  Object to register with the singleton manager
 * @return     Pointer returned by the Register call
 */
// cmOperatorCtor_C220_2hr @ 0x822EC220
void* cmOperatorCtor_SingletonRegister(void* obj) {
    void** mgr = (void**)g_sda_6404;

    // mgr->Register(obj) via vtable slot 2
    void** mgrVtbl = *(void***)mgr;
    typedef void* (*VFn_Register)(void* self, void* obj);
    void* result = ((VFn_Register)mgrVtbl[2])(mgr, obj);

    if (g_sda_660D != 0) {
        // result->SetEnabled(1, 1) via vtable slot 13
        void** resVtbl = *(void***)result;
        typedef void (*VFn_SetEnabled)(void* self, int a1, int a2);
        ((VFn_SetEnabled)resVtbl[13])(result, 1, 1);
    }

    return result;
}

/**
 * cmOperatorCtor_C298_2h @ 0x822EC298 | size: 0x20
 *
 * Passes the given object (or the global singleton if null) plus
 * zero-initialized args to hudFlashBase_76F8_g.
 */
// cmOperatorCtor_C298_2h @ 0x822EC298
void cmOperatorCtor_HudFlashDispatch(void* obj) {
    if (obj == nullptr) {
        obj = g_sda_6404;
    }
    hudFlashBase_76F8_g(obj, 0, nullptr);
}

/**
 * cmOperatorCtor_C3F0_2h @ 0x822EC3F0 | size: 0x28
 *
 * Loads the global singleton manager and tail-calls vtable slot 10
 * with the original args shuffled (r3→r4, r4→r5, mgr→r3).
 */
// cmOperatorCtor_C3F0_2h @ 0x822EC3F0
void cmOperatorCtor_SingletonDispatchSlot10(void* a1, void* a2) {
    void** mgr = (void**)g_sda_6404;
    // mgr->Dispatch(a1, a2, 1) via vtable slot 10
    void** mgrVtbl = *(void***)mgr;
    typedef void (*VFn_Dispatch)(void* self, void* a1, void* a2, int a3);
    ((VFn_Dispatch)mgrVtbl[10])(mgr, a1, a2, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor camera action factories — batch 3
//
// These 10 factory functions all create cmAction objects (144 bytes) for
// camera parameter sampling. Each allocates memory, builds two 16-byte
// function descriptor tables and a type/flags struct on the stack, then
// tail-calls cmActionCtor_11D0_fw to initialize the cmAction node.
//
// The only variation between them is:
//   1. The action sampler function pointer (stored in the init descriptor)
//   2. The dirty-flag bitmask indicating which camera output slots are written
//
// Common function pointer tables contain:
//   - rage_DebugLog (nop in release) for unused callbacks
//   - phDemoWorld_F120 for port registration
//   - atSingleton_EB38_p for teardown
//
// Dirty flag bitmask semantics (per cm_cam_actions.cpp):
//   0x0007 = base camera params (position XYZ)
//   0x0038 = look-at target (XYZ)
//   0x03C0 = orientation (vec4 + angle)
//   0x3C00 = secondary orientation (vec4 + angle)
//   0x3FFF = all 14 scalar slots
//   0xC000 = vec4 slots 14+15
// ─────────────────────────────────────────────────────────────────────────────

// Action sampler functions (camera parameter readers)
extern "C" void atSingleton_9728_2h(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_OrientationPair(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_HeadingAndFOV(void* actionCtx, void* actionNode);

// Common callback functions shared by all camera action factories
extern "C" void rage_DebugLog(const char* fmt, ...);     // @ 0x8240E6D0 — nop in release
extern "C" void phDemoWorld_F120(void* node);             // @ 0x8217F120 — port registration
extern "C" void atSingleton_EB38_p(void* node);           // @ 0x8225EB38 — teardown

// cmAction init function
extern "C" void* cmActionCtor_11D0_fw(void* mem, void* callerArg, int portCount,
                                       void* typeFlags, void* initDesc,
                                       void* funcTable1, void* funcTable2);

/**
 * Helper: builds the common function descriptor tables and calls
 * cmActionCtor_11D0_fw to construct a cmAction camera sampler node.
 *
 * @param callerArg   Opaque argument forwarded from the factory caller
 * @param actionFn    The camera action sampler function pointer
 * @param dirtyFlags  Bitmask of camera output slots this action writes
 * @return            Constructed cmAction object, or nullptr on allocation failure
 */
static void* cmOperatorCtor_CreateCamAction(void* callerArg, void* actionFn, uint32_t dirtyFlags) {
    void* mem = cmOperatorCtor_Allocate(144);
    if (mem == nullptr) {
        return nullptr;
    }

    // Type/flags descriptor (passed as r6)
    uint32_t typeDesc[1] = { dirtyFlags };

    // Init descriptor: {actionFn, atSingleton_EB38_p, 0, 0}
    void* initDesc[4] = { actionFn, (void*)atSingleton_EB38_p, nullptr, nullptr };

    // Function table 1: {rage_DebugLog, phDemoWorld_F120 data, ...}
    // These are copied from the stack-built structs at sp+96 and sp+112
    // Table 1 (sp+144 = copy of sp+96): {rage_DebugLog, phDemoWorld_F120, 0, atSingleton_EB38_p}
    void* funcTable1[4] = { (void*)rage_DebugLog, (void*)phDemoWorld_F120,
                            nullptr, (void*)atSingleton_EB38_p };

    // Table 2 (sp+160 = copy of sp+112): {rage_DebugLog, ?, 0, atSingleton_EB38_p}
    void* funcTable2[4] = { (void*)rage_DebugLog, nullptr,
                            nullptr, (void*)atSingleton_EB38_p };

    return cmActionCtor_11D0_fw(mem, callerArg, 2, typeDesc, initDesc, funcTable2, funcTable1);
}

// ── Camera position XYZ + vec4 slots 14+15 factory (flags=0xC000) ────────────

/**
 * cmOperatorCtor_CAA0_w @ 0x8217CAA0 | size: 0x124
 *
 * Creates a cmAction that samples two vec4 values from ports 0 and 1,
 * writing them to camera output vec4 slots 14 and 15.
 * Uses atSingleton_9728_2h as the action sampler.
 */
// cmOperatorCtor_CAA0_w @ 0x8217CAA0
void* cmOperatorCtor_CamActionVec4Pair_A(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 0xC000);
}

/**
 * cmOperatorCtor_CBC8_w @ 0x8217CBC8 | size: 0x124
 *
 * Creates a cmAction that samples two vec4 values from ports 0 and 1,
 * writing them to camera output vec4 slots 14 and 15.
 * Uses cmSampleCamActions_OrientationPair as the action sampler.
 */
// cmOperatorCtor_CBC8_w @ 0x8217CBC8
void* cmOperatorCtor_CamActionVec4Pair_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_OrientationPair, 0xC000);
}

// ── Camera base params factory (flags=0x7 = position XYZ) ────────────────────

/**
 * cmOperatorCtor_CCF0_w @ 0x8217CCF0 | size: 0x120
 *
 * Creates a cmAction that samples camera base position parameters (XYZ).
 * Dirty flags 0x7 = slots 0, 1, 2.
 */
// cmOperatorCtor_CCF0_w @ 0x8217CCF0
void* cmOperatorCtor_CamActionBaseXYZ(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 7);
}

// ── Camera rotation factory (flags=0x3 = pitch+yaw) ──────────────────────────

/**
 * cmOperatorCtor_CE10_w @ 0x8217CE10 | size: 0x120
 *
 * Creates a cmAction that samples camera rotation (pitch and yaw).
 * Dirty flags 0x3 = slots 0, 1.
 */
// cmOperatorCtor_CE10_w @ 0x8217CE10
void* cmOperatorCtor_CamActionRotation(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 3);
}

// ── Camera FOV factory (flags=0x4 = FOV slot) ────────────────────────────────

/**
 * cmOperatorCtor_CF30_w @ 0x8217CF30 | size: 0x120
 *
 * Creates a cmAction that samples the camera field of view.
 * Dirty flags 0x4 = slot 2.
 */
// cmOperatorCtor_CF30_w @ 0x8217CF30
void* cmOperatorCtor_CamActionFOV(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 4);
}

// ── Camera look-at target factory (flags=0x38 = target XYZ) ──────────────────

/**
 * cmOperatorCtor_D050_w @ 0x8217D050 | size: 0x120
 *
 * Creates a cmAction that samples the camera look-at target position.
 * Dirty flags 0x38 = slots 3, 4, 5.
 */
// cmOperatorCtor_D050_w @ 0x8217D050
void* cmOperatorCtor_CamActionLookAtTarget(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 56);
}

// ── Camera orientation factory (flags=0x3C0 = orientation vec4) ──────────────

/**
 * cmOperatorCtor_D170_w @ 0x8217D170 | size: 0x120
 *
 * Creates a cmAction that samples camera orientation (vec4 + angle).
 * Dirty flags 0x3C0 = slots 6, 7, 8, 9.
 */
// cmOperatorCtor_D170_w @ 0x8217D170
void* cmOperatorCtor_CamActionOrientation(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 960);
}

// ── Camera secondary orientation factory (flags=0x3C00) ──────────────────────

/**
 * cmOperatorCtor_D290_w @ 0x8217D290 | size: 0x120
 *
 * Creates a cmAction that samples secondary camera orientation.
 * Dirty flags 0x3C00 = slots 10, 11, 12, 13.
 */
// cmOperatorCtor_D290_w @ 0x8217D290
void* cmOperatorCtor_CamActionSecondaryOrientation(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 15360);
}

// ── Camera all-slots factory (flags=0x3FFF = all 14 scalar slots) ────────────

/**
 * cmOperatorCtor_D3B0_w @ 0x8217D3B0 | size: 0x120
 *
 * Creates a cmAction that samples all 14 scalar camera output slots.
 * Dirty flags 0x3FFF = all slots 0-13.
 */
// cmOperatorCtor_D3B0_w @ 0x8217D3B0
void* cmOperatorCtor_CamActionAllSlots(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)atSingleton_9728_2h, 16383);
}

// ── Camera 2-float sampler factory (flags=0x7, different sampler) ────────────

/**
 * cmOperatorCtor_D4D0_w @ 0x8217D4D0 | size: 0x120
 *
 * Creates a cmAction that samples 2 floats from ports 0 and 1, checking
 * port status and writing to camera output slots 0 and 1.
 * Uses cmSampleCamActions_HeadingAndFOV as the action sampler.
 * Dirty flags 0x7 = base position slots.
 */
// cmOperatorCtor_D4D0_w @ 0x8217D4D0
void* cmOperatorCtor_CamAction2Float(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_HeadingAndFOV, 7);
}

// ── cmOperatorCtor factory functions — batch 4 (logic & arithmetic operators) ─

extern "C" const void* rage_cmEquals_vtable;         // @ 0x8205461C
extern "C" const void* rage_cmGreaterThan_vtable;    // @ 0x82054674
extern "C" const void* rage_cmLessThan_vtable;       // @ 0x820546CC
extern "C" const void* rage_cmOr_vtable;             // @ 0x82054724
extern "C" const void* rage_cmXor_vtable;            // @ 0x8205477C
extern "C" const void* rage_cmAnd_vtable;            // @ 0x820547D4
extern "C" const void* rage_cmNot_vtable;            // @ 0x8205482C
extern "C" const void* rage_cmAdd_vtable;            // @ 0x82053A6C
extern "C" const void* rage_cmSubtract_vtable;       // @ 0x82053AC4
extern "C" const void* rage_cmMultiply_vtable;       // @ 0x82053B1C

// ── rage::cmEquals factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6698_1 @ 0x82266698 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmEquals vtable.
 */
// cmOperatorCtor_vfn_0_6698_1 @ 0x82266698
void* cmOperatorCtor_cmEquals() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmEquals_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmGreaterThan factory ──────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6730_1 @ 0x82266730 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmGreaterThan vtable.
 */
// cmOperatorCtor_vfn_0_6730_1 @ 0x82266730
void* cmOperatorCtor_cmGreaterThan() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmGreaterThan_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmLessThan factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_67C8_1 @ 0x822667C8 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmLessThan vtable.
 */
// cmOperatorCtor_vfn_0_67C8_1 @ 0x822667C8
void* cmOperatorCtor_cmLessThan() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmLessThan_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmOr factory ───────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6860_1 @ 0x82266860 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmOr vtable.
 */
// cmOperatorCtor_vfn_0_6860_1 @ 0x82266860
void* cmOperatorCtor_cmOr() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmOr_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmXor factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_68F8_1 @ 0x822668F8 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmXor vtable.
 */
// cmOperatorCtor_vfn_0_68F8_1 @ 0x822668F8
void* cmOperatorCtor_cmXor() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmXor_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnd factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6990_1 @ 0x82266990 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmAnd vtable.
 */
// cmOperatorCtor_vfn_0_6990_1 @ 0x82266990
void* cmOperatorCtor_cmAnd() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnd_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmNot factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6A28_1 @ 0x82266A28 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmNot vtable. Unary operator.
 */
// cmOperatorCtor_vfn_0_6A28_1 @ 0x82266A28
void* cmOperatorCtor_cmNot() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmNot_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAdd factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6AB8_1 @ 0x82266AB8 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmAdd vtable.
 */
// cmOperatorCtor_vfn_0_6AB8_1 @ 0x82266AB8
void* cmOperatorCtor_cmAdd() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAdd_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSubtract factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6B50_1 @ 0x82266B50 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmSubtract vtable.
 */
// cmOperatorCtor_vfn_0_6B50_1 @ 0x82266B50
void* cmOperatorCtor_cmSubtract() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSubtract_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmMultiply factory ─────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6BE8_1 @ 0x82266BE8 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmMultiply vtable.
 */
// cmOperatorCtor_vfn_0_6BE8_1 @ 0x82266BE8
void* cmOperatorCtor_cmMultiply() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmMultiply_vtable;
        return mem;
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor camera action factories — batch 4
//
// 10 additional cmAction camera sampler factories. Same mechanical pattern
// as batch 3: allocate 144 bytes, build function descriptor tables on the
// stack, tail-call cmActionCtor_11D0_fw. Each uses a unique combination of
// action sampler function and dirty-flag bitmask.
//
// Dirty flag semantics (same as batch 3):
//   0x0003 = rotation (pitch + yaw)
//   0x0004 = FOV
//   0x0007 = base camera params (position XYZ)
//   0x0038 = look-at target (XYZ)
//   0x03C0 = orientation (vec4 + angle)
//   0x3C00 = secondary orientation (vec4 + angle)
//   0x3FFF = all 14 scalar slots
// ─────────────────────────────────────────────────────────────────────────────

// Action sampler functions (camera parameter readers — batch 4)
extern "C" void cmSampleCamActions_Rotation(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_BaseXYZ(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_FOV(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_Orientation(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_LookAtTarget(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_9BD8_g(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_9D80_g(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_A060_g(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_A238_g(void* actionCtx, void* actionNode);
extern "C" void cmSampleCamActions_A488_g(void* actionCtx, void* actionNode);

/**
 * cmOperatorCtor_D5F0_w @ 0x8217D5F0 | size: 0x120
 *
 * Creates a cmAction that samples camera rotation (pitch + yaw).
 * Uses cmSampleCamActions_Rotation as the action sampler.
 * Dirty flags 0x3 = slots 0, 1.
 */
// cmOperatorCtor_D5F0_w @ 0x8217D5F0
void* cmOperatorCtor_CamActionRotation_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_Rotation, 3);
}

/**
 * cmOperatorCtor_D710_w @ 0x8217D710 | size: 0x120
 *
 * Creates a cmAction that samples camera base position parameters (XYZ).
 * Uses cmSampleCamActions_BaseXYZ as the action sampler.
 * Dirty flags 0x7 = slots 0, 1, 2.
 */
// cmOperatorCtor_D710_w @ 0x8217D710
void* cmOperatorCtor_CamActionBaseXYZ_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_BaseXYZ, 7);
}

/**
 * cmOperatorCtor_D830_w @ 0x8217D830 | size: 0x120
 *
 * Creates a cmAction that samples the camera field of view.
 * Uses cmSampleCamActions_FOV as the action sampler.
 * Dirty flags 0x4 = slot 2.
 */
// cmOperatorCtor_D830_w @ 0x8217D830
void* cmOperatorCtor_CamActionFOV_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_FOV, 4);
}

/**
 * cmOperatorCtor_D950_w @ 0x8217D950 | size: 0x120
 *
 * Creates a cmAction that samples camera base position parameters (XYZ).
 * Uses cmSampleCamActions_Orientation as the action sampler.
 * Dirty flags 0x7 = slots 0, 1, 2.
 */
// cmOperatorCtor_D950_w @ 0x8217D950
void* cmOperatorCtor_CamActionBaseXYZ_C(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_Orientation, 7);
}

/**
 * cmOperatorCtor_DA70_w @ 0x8217DA70 | size: 0x120
 *
 * Creates a cmAction that samples the camera look-at target position.
 * Uses cmSampleCamActions_LookAtTarget as the action sampler.
 * Dirty flags 0x38 = slots 3, 4, 5.
 */
// cmOperatorCtor_DA70_w @ 0x8217DA70
void* cmOperatorCtor_CamActionLookAtTarget_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_LookAtTarget, 56);
}

/**
 * cmOperatorCtor_DB90_w @ 0x8217DB90 | size: 0x120
 *
 * Creates a cmAction that samples camera orientation (vec4 + angle).
 * Uses cmSampleCamActions_9BD8_g as the action sampler.
 * Dirty flags 0x3C0 = slots 6, 7, 8, 9.
 */
// cmOperatorCtor_DB90_w @ 0x8217DB90
void* cmOperatorCtor_CamActionOrientation_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_9BD8_g, 960);
}

/**
 * cmOperatorCtor_DCB0_w @ 0x8217DCB0 | size: 0x120
 *
 * Creates a cmAction that samples secondary camera orientation.
 * Uses cmSampleCamActions_9D80_g as the action sampler.
 * Dirty flags 0x3C00 = slots 10, 11, 12, 13.
 */
// cmOperatorCtor_DCB0_w @ 0x8217DCB0
void* cmOperatorCtor_CamActionSecondaryOrientation_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_9D80_g, 15360);
}

/**
 * cmOperatorCtor_DDD0_w @ 0x8217DDD0 | size: 0x120
 *
 * Creates a cmAction that samples the camera look-at target position.
 * Uses cmSampleCamActions_A060_g as the action sampler.
 * Dirty flags 0x38 = slots 3, 4, 5.
 */
// cmOperatorCtor_DDD0_w @ 0x8217DDD0
void* cmOperatorCtor_CamActionLookAtTarget_C(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_A060_g, 56);
}

/**
 * cmOperatorCtor_DEF0_w @ 0x8217DEF0 | size: 0x120
 *
 * Creates a cmAction that samples the camera look-at target position.
 * Uses cmSampleCamActions_A238_g as the action sampler.
 * Dirty flags 0x38 = slots 3, 4, 5.
 */
// cmOperatorCtor_DEF0_w @ 0x8217DEF0
void* cmOperatorCtor_CamActionLookAtTarget_D(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_A238_g, 56);
}

/**
 * cmOperatorCtor_E010_w @ 0x8217E010 | size: 0x120
 *
 * Creates a cmAction that samples all 14 scalar camera output slots.
 * Uses cmSampleCamActions_A488_g as the action sampler.
 * Dirty flags 0x3FFF = all slots 0-13.
 */
// cmOperatorCtor_E010_w @ 0x8217E010
void* cmOperatorCtor_CamActionAllSlots_B(void* callerArg) {
    return cmOperatorCtor_CreateCamAction(callerArg, (void*)cmSampleCamActions_A488_g, 16383);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperatorCtor factory functions — batch 5 (math & trig operators)
//
// 10 additional CM node factories for math/trig operators.
// Binary operators (cmDivide, cmModulo, cmMax, cmMin, cmArcTangent):
//   allocate 32 bytes, zero-init, set numInputs=2, install vtable.
// Unary operators (cmNegate, cmSine, cmCosine, cmArcCosine, cmTangent):
//   allocate 24 bytes, zero-init, set numInputs=1, install vtable.
// ─────────────────────────────────────────────────────────────────────────────

extern "C" const void* rage_cmDivide_vtable;       // @ 0x82053B74
extern "C" const void* rage_cmModulo_vtable;       // @ 0x82053BCC
extern "C" const void* rage_cmMax_vtable;          // @ 0x82053F3C
extern "C" const void* rage_cmMin_vtable;          // @ 0x82053F94
extern "C" const void* rage_cmNegate_vtable;       // @ 0x82053C24
extern "C" const void* rage_cmSine_vtable;         // @ 0x82053C7C
extern "C" const void* rage_cmCosine_vtable;       // @ 0x82053CD4
extern "C" const void* rage_cmArcCosine_vtable;    // @ 0x82053D2C
extern "C" const void* rage_cmTangent_vtable;      // @ 0x82053D84
extern "C" const void* rage_cmArcTangent_vtable;   // @ 0x82053DDC

// ── rage::cmDivide factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6C80_1 @ 0x82266C80 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmDivide vtable. Binary arithmetic operator.
 */
// cmOperatorCtor_vfn_0_6C80_1 @ 0x82266C80
void* cmOperatorCtor_cmDivide() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmDivide_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmModulo factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6D18_1 @ 0x82266D18 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmModulo vtable. Binary arithmetic operator.
 */
// cmOperatorCtor_vfn_0_6D18_1 @ 0x82266D18
void* cmOperatorCtor_cmModulo() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmModulo_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmMax factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6DB0_1 @ 0x82266DB0 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmMax vtable. Binary comparison operator.
 */
// cmOperatorCtor_vfn_0_6DB0_1 @ 0x82266DB0
void* cmOperatorCtor_cmMax() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmMax_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmMin factory ──────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6E48_1 @ 0x82266E48 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmMin vtable. Binary comparison operator.
 */
// cmOperatorCtor_vfn_0_6E48_1 @ 0x82266E48
void* cmOperatorCtor_cmMin() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmMin_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmNegate factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6EE0_1 @ 0x82266EE0 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmNegate vtable. Unary arithmetic operator.
 */
// cmOperatorCtor_vfn_0_6EE0_1 @ 0x82266EE0
void* cmOperatorCtor_cmNegate() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmNegate_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSine factory ─────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_6F70_1 @ 0x82266F70 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmSine vtable. Unary trigonometric operator.
 */
// cmOperatorCtor_vfn_0_6F70_1 @ 0x82266F70
void* cmOperatorCtor_cmSine() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSine_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmCosine factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7090_1 @ 0x82267090 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmCosine vtable. Unary trigonometric operator.
 */
// cmOperatorCtor_vfn_0_7090_1 @ 0x82267090
void* cmOperatorCtor_cmCosine() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmCosine_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmArcCosine factory ────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7000_1 @ 0x82267000 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmArcCosine vtable. Unary trigonometric operator.
 */
// cmOperatorCtor_vfn_0_7000_1 @ 0x82267000
void* cmOperatorCtor_cmArcCosine() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmArcCosine_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmTangent factory ──────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7120_1 @ 0x82267120 | size: 0x8C
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmTangent vtable. Unary trigonometric operator.
 */
// cmOperatorCtor_vfn_0_7120_1 @ 0x82267120
void* cmOperatorCtor_cmTangent() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmTangent_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmArcTangent factory ───────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_71B0_1 @ 0x822671B0 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmArcTangent vtable. Binary trigonometric operator (atan2).
 */
// cmOperatorCtor_vfn_0_71B0_1 @ 0x822671B0
void* cmOperatorCtor_cmArcTangent() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmArcTangent_vtable;
        return mem;
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// CM operator inline constructors — animation camera and math operators
// ─────────────────────────────────────────────────────────────────────────────

extern "C" const void* rage_cmAnimCamOrientation_vtable;  // @ 0x820381C4
extern "C" const void* rage_cmAnimCamFov_vtable;          // @ 0x8203821C
extern "C" const void* rage_cmAnimBonePosition_vtable;    // @ 0x82038324
extern "C" const void* rage_cmAnimBoneOrientation_vtable; // @ 0x8203837C
extern "C" const void* rage_cmAnimDuration_vtable;        // @ 0x820382CC
extern "C" const void* rage_cmAnimCamFStop_vtable;        // @ 0x82038274
extern "C" const void* rage_cmAbs_vtable;                 // @ 0x82053E8C
extern "C" const void* rage_cmSign_vtable;                // @ 0x82053EE4
extern "C" const void* rage_cmInverse_vtable;             // @ 0x82053E34
extern "C" const void* rage_cmLerp_vtable;                // @ 0x82053FEC

// ── rage::cmAnimCamOrientation inline constructor ────────────────────────────

/**
 * cmOperatorCtor_vfn_0_5AD0_1 @ 0x82185AD0 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets field+28 to 2 (numInputs),
 * and sets vtable to rage::cmAnimCamOrientation.
 */
// cmOperatorCtor_vfn_0_5AD0_1 @ 0x82185AD0
void* cmOperatorCtor_cmAnimCamOrientation() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimCamOrientation_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnimCamFov inline constructor ────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_5B68_1 @ 0x82185B68 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets field+28 to 2 (numInputs),
 * and sets vtable to rage::cmAnimCamFov.
 */
// cmOperatorCtor_vfn_0_5B68_1 @ 0x82185B68
void* cmOperatorCtor_cmAnimCamFov() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimCamFov_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnimBonePosition inline constructor ──────────────────────────────

/**
 * cmOperatorCtor_vfn_0_5C00_1 @ 0x82185C00 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets field+36 to 3 (numInputs),
 * and sets vtable to rage::cmAnimBonePosition.
 */
// cmOperatorCtor_vfn_0_5C00_1 @ 0x82185C00
void* cmOperatorCtor_cmAnimBonePosition() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimBonePosition_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnimBoneOrientation inline constructor ───────────────────────────

/**
 * cmOperatorCtor_vfn_0_5CA0_1 @ 0x82185CA0 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets field+36 to 3 (numInputs),
 * and sets vtable to rage::cmAnimBoneOrientation.
 */
// cmOperatorCtor_vfn_0_5CA0_1 @ 0x82185CA0
void* cmOperatorCtor_cmAnimBoneOrientation() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimBoneOrientation_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnimDuration inline constructor ──────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_5D40_1 @ 0x82185D40 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets field+20 to 1 (numInputs),
 * and sets vtable to rage::cmAnimDuration.
 */
// cmOperatorCtor_vfn_0_5D40_1 @ 0x82185D40
void* cmOperatorCtor_cmAnimDuration() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimDuration_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAnimCamFStop inline constructor ──────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_5DD0_1 @ 0x82185DD0 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets field+28 to 2 (numInputs),
 * and sets vtable to rage::cmAnimCamFStop.
 */
// cmOperatorCtor_vfn_0_5DD0_1 @ 0x82185DD0
void* cmOperatorCtor_cmAnimCamFStop() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAnimCamFStop_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAbs inline constructor ───────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7248_1 @ 0x82267248 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets field+20 to 1 (numInputs),
 * and sets vtable to rage::cmAbs.
 */
// cmOperatorCtor_vfn_0_7248_1 @ 0x82267248
void* cmOperatorCtor_cmAbs() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAbs_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSign inline constructor ──────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_72D8_1 @ 0x822672D8 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets field+20 to 1 (numInputs),
 * and sets vtable to rage::cmSign.
 */
// cmOperatorCtor_vfn_0_72D8_1 @ 0x822672D8
void* cmOperatorCtor_cmSign() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSign_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmInverse inline constructor ───────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7368_1 @ 0x82267368 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets field+20 to 1 (numInputs),
 * and sets vtable to rage::cmInverse.
 */
// cmOperatorCtor_vfn_0_7368_1 @ 0x82267368
void* cmOperatorCtor_cmInverse() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmInverse_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmLerp inline constructor ──────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_73F8_1 @ 0x822673F8 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets field+36 to 3 (numInputs),
 * and sets vtable to rage::cmLerp.
 */
// cmOperatorCtor_vfn_0_73F8_1 @ 0x822673F8
void* cmOperatorCtor_cmLerp() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmLerp_vtable;
        return mem;
    }
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage::cmAngleLerp / cmRange / cmClamp / cmAngleDiff / cmAbsAngleDiff /
// cmClampRange / cmSlowIn / cmSlowOut / cmSlowInOut factories
// and gdDrillSoftShotData factory
// ─────────────────────────────────────────────────────────────────────────────

extern "C" const void* rage_cmAngleLerp_vtable;     // @ 0x82054044
extern "C" const void* rage_cmRange_vtable;          // @ 0x8205409C
extern "C" const void* rage_cmClamp_vtable;          // @ 0x820540F4
extern "C" const void* rage_cmAngleDiff_vtable;      // @ 0x820541A4
extern "C" const void* rage_cmAbsAngleDiff_vtable;   // @ 0x8205414C
extern "C" const void* rage_cmClampRange_vtable;     // @ 0x820541FC
extern "C" const void* rage_cmSlowIn_vtable;         // @ 0x82054254
extern "C" const void* rage_cmSlowOut_vtable;        // @ 0x820542AC
extern "C" const void* rage_cmSlowInOut_vtable;      // @ 0x82054304
extern "C" const void* gdDrillSoftShotData_vtable;   // @ 0x820411FC

// Init function for gdDrillSoftShotData
extern "C" void* util_E330(void* mem);               // base initializer for drill data

// Float constants
extern "C" const float lbl_825C4930;   // @ 0x825C4930  (gdDrillSoftShotData +104)
extern "C" const float lbl_8202CFE8;   // @ 0x8202CFE8  (gdDrillSoftShotData +108)
extern "C" const float lbl_8202D0F0;   // @ 0x8202D0F0  (gdDrillSoftShotData +112)

// ── rage::cmAngleLerp factory ────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7498_1 @ 0x82267498 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets numInputs=3,
 * and installs rage::cmAngleLerp vtable.
 */
// cmOperatorCtor_vfn_0_7498_1 @ 0x82267498
void* cmOperatorCtor_cmAngleLerp() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAngleLerp_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmRange factory ────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7538_1 @ 0x82267538 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets numInputs=3,
 * and installs rage::cmRange vtable.
 */
// cmOperatorCtor_vfn_0_7538_1 @ 0x82267538
void* cmOperatorCtor_cmRange() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmRange_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmClamp factory ────────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_75D8_1 @ 0x822675D8 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets numInputs=3,
 * and installs rage::cmClamp vtable.
 */
// cmOperatorCtor_vfn_0_75D8_1 @ 0x822675D8
void* cmOperatorCtor_cmClamp() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmClamp_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAngleDiff factory ────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7678_1 @ 0x82267678 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmAngleDiff vtable.
 */
// cmOperatorCtor_vfn_0_7678_1 @ 0x82267678
void* cmOperatorCtor_cmAngleDiff() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAngleDiff_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmAbsAngleDiff factory ─────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7710_1 @ 0x82267710 | size: 0x94
 *
 * Allocates 32 bytes, zero-initializes, sets numInputs=2,
 * and installs rage::cmAbsAngleDiff vtable.
 */
// cmOperatorCtor_vfn_0_7710_1 @ 0x82267710
void* cmOperatorCtor_cmAbsAngleDiff() {
    void* mem = cmOperatorCtor_Allocate(32);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 2;  // +28  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmAbsAngleDiff_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmClampRange factory ───────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_77A8_1 @ 0x822677A8 | size: 0x9c
 *
 * Allocates 40 bytes, zero-initializes, sets numInputs=3,
 * and installs rage::cmClampRange vtable.
 */
// cmOperatorCtor_vfn_0_77A8_1 @ 0x822677A8
void* cmOperatorCtor_cmClampRange() {
    void* mem = cmOperatorCtor_Allocate(40);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 0;  // +20
        fields[6] = 0;  // +24
        fields[7] = 0;  // +28
        fields[8] = 0;  // +32
        fields[9] = 3;  // +36  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmClampRange_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSlowIn factory ───────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7848_1 @ 0x82267848 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmSlowIn vtable.
 */
// cmOperatorCtor_vfn_0_7848_1 @ 0x82267848
void* cmOperatorCtor_cmSlowIn() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSlowIn_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSlowOut factory ──────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_78D8_1 @ 0x822678D8 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmSlowOut vtable.
 */
// cmOperatorCtor_vfn_0_78D8_1 @ 0x822678D8
void* cmOperatorCtor_cmSlowOut() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSlowOut_vtable;
        return mem;
    }
    return nullptr;
}

// ── rage::cmSlowInOut factory ────────────────────────────────────────────────

/**
 * cmOperatorCtor_vfn_0_7968_1 @ 0x82267968 | size: 0x8c
 *
 * Allocates 24 bytes, zero-initializes, sets numInputs=1,
 * and installs rage::cmSlowInOut vtable.
 */
// cmOperatorCtor_vfn_0_7968_1 @ 0x82267968
void* cmOperatorCtor_cmSlowInOut() {
    void* mem = cmOperatorCtor_Allocate(24);
    if (mem != nullptr) {
        uint32_t* fields = (uint32_t*)mem;
        fields[1] = 0;  // +4
        fields[2] = 0;  // +8
        fields[3] = 0;  // +12
        fields[4] = 0;  // +16
        fields[5] = 1;  // +20  numInputs
        fields[0] = (uint32_t)(uintptr_t)&rage_cmSlowInOut_vtable;
        return mem;
    }
    return nullptr;
}

// ── gdDrillSoftShotData factory ──────────────────────────────────────────────

/**
 * cmOperatorCtor_F088_w @ 0x821DF088 | size: 0xa4
 *
 * Allocates 116 bytes, calls base initializer util_E330, then sets
 * vtable to gdDrillSoftShotData and configures drill soft-shot parameters:
 *   +96  = 5  (numEntries)
 *   +100 = 3  (numThresholds)
 *   +104 = float from 0x825C4930 (power)
 *   +108 = float from 0x8202CFE8 (speed)
 *   +112 = float from 0x8202D0F0 (spin)
 */
// cmOperatorCtor_F088_w @ 0x821DF088
void* cmOperatorCtor_gdDrillSoftShotData() {
    void* mem = cmOperatorCtor_Allocate(116);
    if (mem != nullptr) {
        util_E330(mem);

        uint32_t* fields = (uint32_t*)mem;
        float* ffields = (float*)mem;

        ffields[26] = lbl_825C4930;   // +104  power
        fields[24]  = 5;              // +96   numEntries
        fields[0]   = (uint32_t)(uintptr_t)&gdDrillSoftShotData_vtable;
        fields[25]  = 3;              // +100  numThresholds
        ffields[27] = lbl_8202CFE8;   // +108  speed
        ffields[28] = lbl_8202D0F0;   // +112  spin

        return mem;
    }
    return nullptr;
}
