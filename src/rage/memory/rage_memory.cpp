/**
 * rage_memory.cpp — RAGE Engine Memory Management (C++ namespace wrappers)
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * This file provides C++ namespace wrappers for the C-based memory functions
 * implemented in src/crt/heap.c. The linker expects rage:: namespaced symbols
 * when C++ code calls these functions.
 */

#include <cstddef>
#include <cstdint>

// Forward declarations of C functions from heap.c
extern "C" {
    void rage_free(void* ptr);
    void* sysMemAllocator_Allocate(void* ptr, size_t size);
    uint8_t rage_FindSingleton(void* ptr);
}

namespace rage {

// Forward declarations for RAGE types
struct cmNodePort;
struct grcTexture;

/**
 * rage_free @ 0x820C00C0 | size: 0x60
 * 
 * C++ namespace wrapper for the canonical RAGE heap free function.
 * Delegates to the C implementation in heap.c.
 */
void rage_free(void* ptr) {
    ::rage_free(ptr);
}

/**
 * rage_alloc_aligned
 * 
 * Allocate aligned memory from the RAGE heap.
 * Delegates to sysMemAllocator_Allocate.
 * 
 * @param size      Size in bytes to allocate
 * @param alignment Alignment requirement (power of 2)
 * @return          Aligned pointer, or nullptr on failure
 */
void* rage_alloc_aligned(unsigned long size, unsigned long alignment) {
    // sysMemAllocator_Allocate uses the first parameter as alignment hint
    return sysMemAllocator_Allocate((void*)(uintptr_t)alignment, size);
}

// ============================================================================
// Singleton Management Functions
// ============================================================================

/**
 * FindSingleton
 * 
 * Check if a pointer is owned by a registered singleton.
 * Delegates to the C implementation.
 */
uint8_t FindSingleton(void* ptr) {
    return ::rage_FindSingleton(ptr);
}

/**
 * InitializeSingleton
 * 
 * Register a singleton instance with the RAGE singleton registry.
 */
void InitializeSingleton(void* ptr) {
    // TODO: Implement proper singleton registration
    (void)ptr;
}

/**
 * ReleaseSingleton
 * 
 * Unregister a singleton instance from the RAGE singleton registry.
 */
void ReleaseSingleton(void* ptr) {
    // TODO: Implement proper singleton unregistration
    (void)ptr;
}

/**
 * UnregisterSingleton
 * 
 * Unregister a singleton instance (const version).
 */
void UnregisterSingleton(void const* ptr) {
    // TODO: Implement proper singleton unregistration
    (void)ptr;
}

/**
 * BindObject
 * 
 * Bind an object to another object in the RAGE object system.
 */
void BindObject(void* obj1, void* obj2, unsigned int type, unsigned int flags) {
    // TODO: Implement proper object binding
    (void)obj1;
    (void)obj2;
    (void)type;
    (void)flags;
}

/**
 * UnbindObject
 * 
 * Unbind an object from another object in the RAGE object system.
 */
void UnbindObject(void* obj1, void* obj2) {
    // TODO: Implement proper object unbinding
    (void)obj1;
    (void)obj2;
}

/**
 * NotifyObservers
 * 
 * Notify all observers of an object about an event.
 */
void NotifyObservers(void* obj, void* event, unsigned int* eventType) {
    // TODO: Implement proper observer notification
    (void)obj;
    (void)event;
    (void)eventType;
}

/**
 * ClearPendingFlag
 * 
 * Clear a pending flag on an object.
 */
void ClearPendingFlag(void* obj) {
    // TODO: Implement proper flag clearing
    (void)obj;
}

// ============================================================================
// CM (Curve/Machine) System Functions
// ============================================================================

// cmMemory class stub
class cmMemory {
public:
    /**
     * Sync
     * 
     * Synchronize CM memory state.
     * Called before reading values from CM memory nodes.
     */
    void Sync() {
        // TODO: Implement proper CM memory synchronization
    }
};

/**
 * cmNode_GetBool
 * 
 * Get a boolean value from a CM node port.
 */
bool cmNode_GetBool(const cmNodePort* port) {
    // TODO: Implement proper CM node boolean reading
    (void)port;
    return false;
}

// ============================================================================
// Graphics/Texture System Functions
// ============================================================================

/**
 * Release
 * 
 * Release a texture resource.
 */
void Release(grcTexture* texture) {
    // TODO: Implement proper texture release
    (void)texture;
}

} // namespace rage

/* ── External dependencies for atSingleton functions ──────────────────────── */

/* Jump table handler @ 0x821C5C20 */
extern void jumptable_5C20(void* obj);

/* HSM context handlers */
extern void hsmContext_5BC8_fw(void* obj);
extern void hsmContext_5B40_w(void* obj);

/* Network client initialization @ 0x821C4FB0 */
extern void SinglesNetworkClient_4FB0_g(void* obj);

/* Dynamic array resize functions */
extern void atSingleton_22B0(void* array, uint32_t newCapacity);
extern void atSingleton_2038(void* array, uint32_t newCapacity);

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton Template Functions
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSingleton_MergeFlags @ 0x82169728 | size: 0x18
 * 
 * Merges flags from source object into target singleton's flags field.
 * Performs bitwise OR operation to combine flag bits.
 * 
 * @param target    Pointer to target singleton wrapper
 * @param source    Pointer to source object containing flags at offset +16
 */
void atSingleton_MergeFlags(void* target, void* source) {
    // Get pointer to singleton instance from wrapper
    uint32_t* singletonPtr = *(uint32_t**)((char*)target + 4);
    
    // Load existing flags from singleton
    uint32_t existingFlags = *(uint32_t*)((char*)singletonPtr + 4);
    
    // Load new flags from source object
    uint32_t newFlags = *(uint32_t*)((char*)source + 16);
    
    // Merge flags using bitwise OR
    uint32_t mergedFlags = existingFlags | newFlags;
    
    // Store merged flags back to singleton
    *(uint32_t*)((char*)singletonPtr + 4) = mergedFlags;
}

/**
 * atSingleton_ClearFlagAndInitNetwork @ 0x821C6F38 | size: 0x14
 * 
 * Clears a specific flag field and jumps to network client initialization.
 * Sets field at offset 0x27EC (10220) to zero.
 * 
 * @param obj    Pointer to singleton object
 */
void atSingleton_ClearFlagAndInitNetwork(void* obj) {
    // Calculate offset: (1 << 16) | 10220 = 0x27EC = 10220
    const uint32_t offset = 0x27EC;
    
    // Clear the flag field
    *(uint32_t*)((char*)obj + offset) = 0;
    
    // Continue to network client initialization
    SinglesNetworkClient_4FB0_g(obj);
}

/**
 * atSingleton_4D18_w @ 0x82244D18 | size: 0x20
 * 
 * Checks if the least significant bit of a 16-bit field is set.
 * Returns boolean result as uint8_t.
 * 
 * @param obj    Pointer to object with flags at offset 0
 * @return       1 if LSB is set, 0 otherwise
 */
uint8_t atSingleton_4D18_w(void* obj) {
    // Load 16-bit value from object
    uint16_t value = *(uint16_t*)obj;
    
    // Check if least significant bit is set
    bool isSet = (value & 0x1) != 0;
    
    return isSet ? 1 : 0;
}

/**
 * atSingleton_vfn_6 @ 0x82222BF0 | size: 0x24
 * 
 * Validates that an index is within valid range.
 * Checks if index is non-negative and less than count stored at offset +4.
 * 
 * @param obj      Pointer to singleton object
 * @param index    Index to validate
 * @return         1 if index is valid, 0 otherwise
 */
uint8_t atSingleton_vfn_6(void* obj, int32_t index) {
    // Check if index is negative
    if (index < 0) {
        return 0;
    }
    
    // Load count from object
    uint32_t count = *(uint32_t*)((char*)obj + 4);
    
    // Check if index is within bounds
    if (index < (int32_t)count) {
        return 1;
    }
    
    return 0;
}

/**
 * atSingleton_ClearRegion128 @ 0x82255E3F0 | size: 0x18
 * 
 * Clears a 16-byte region starting at offset 128.
 * Initializes four consecutive 32-bit fields to zero.
 * 
 * @param obj    Pointer to object
 */
void atSingleton_ClearRegion128(void* obj) {
    // Clear 16 bytes (4 x uint32_t) starting at offset 128
    *(uint32_t*)((char*)obj + 128) = 0;
    *(uint32_t*)((char*)obj + 132) = 0;
    *(uint32_t*)((char*)obj + 136) = 0;
    *(uint32_t*)((char*)obj + 140) = 0;
}

/**
 * atSingleton_EB38_p @ 0x82256EB38 | size: 0x18
 * 
 * Indirect function call wrapper.
 * Loads function pointer from object and calls it with adjusted parameters.
 * 
 * @param wrapper    Wrapper object containing function pointer at offset 0
 * @param arg1       First argument (becomes r3 in call)
 * @param arg2       Second argument (becomes r4 in call)
 */
void atSingleton_EB38_p(void* wrapper, void* arg1, void* arg2) {
    // Load function pointer from wrapper
    typedef void (*FuncPtr)(void*, void*);
    FuncPtr func = *(FuncPtr*)wrapper;
    
    // Call function with arguments
    func(arg1, arg2);
}

/**
 * atSingleton_6BC0_w @ 0x821C6BC0 | size: 0x38
 * 
 * Conditional state machine dispatcher based on game mode.
 * Routes to different handlers based on mode value at offset +12.
 * 
 * @param obj    Pointer to game state object
 */
void atSingleton_6BC0_w(void* obj) {
    // Load global game state pointer
    uint32_t* globalState = *(uint32_t**)0x825EA13C;
    
    // Load mode from global state
    int32_t mode = *(int32_t*)((char*)globalState + 12);
    
    if (mode == 1) {
        // Mode 1: Jump table handler
        jumptable_5C20(obj);
        return;
    }
    
    if (mode == 3) {
        // Mode 3: HSM context handler
        hsmContext_5BC8_fw(obj);
        return;
    }
    
    // Default: Clear flag at offset 0x27EC
    const uint32_t offset = 0x27EC;  // (1 << 16) | 10220
    *(uint32_t*)((char*)obj + offset) = 0;
}

/**
 * atSingleton_DispatchByGameMode @ 0x821C6108 | size: 0x38
 * 
 * Similar to atSingleton_6BC0_w but routes to different handler for mode 3.
 * Conditional state machine dispatcher with alternate mode 3 handler.
 * 
 * @param obj    Pointer to game state object
 */
void atSingleton_DispatchByGameMode(void* obj) {
    // Load global game state pointer
    uint32_t* globalState = *(uint32_t**)0x825EA13C;
    
    // Load mode from global state
    int32_t mode = *(int32_t*)((char*)globalState + 12);
    
    if (mode == 1) {
        // Mode 1: Jump table handler
        jumptable_5C20(obj);
        return;
    }
    
    if (mode == 3) {
        // Mode 3: Alternate HSM context handler
        hsmContext_5B40_w(obj);
        return;
    }
    
    // Default: Clear flag at offset 0x27EC
    const uint32_t offset = 0x27EC;  // (1 << 16) | 10220
    *(uint32_t*)((char*)obj + offset) = 0;
}

/**
 * atSingleton_D078_fw @ 0x8212D078 | size: 0x58
 * 
 * Dynamic array allocator with automatic growth.
 * Checks if array is full and expands capacity if needed.
 * Returns pointer to next available slot.
 * 
 * @param array    Pointer to dynamic array structure
 * @return         Pointer to next available element
 */
void* atSingleton_D078_fw(void* array) {
    // Load current capacity and count
    uint32_t capacity = *(uint32_t*)((char*)array + 8);
    uint32_t count = *(uint32_t*)((char*)array + 4);
    
    // Check if array is full
    if (count == capacity) {
        // Expand capacity by 256 elements
        uint32_t newCapacity = capacity + 256;
        atSingleton_22B0(array, newCapacity);
    }
    
    // Get base pointer to array data
    void* data = *(void**)array;
    
    // Calculate offset to next element (4 bytes per element)
    uint32_t offset = count * 4;
    
    // Increment count
    *(uint32_t*)((char*)array + 4) = count + 1;
    
    // Return pointer to next element
    return (void*)((char*)data + offset);
}

/**
 * atSingleton_2150_fw @ 0x82132150 | size: 0x58
 * 
 * Dynamic array allocator for larger elements (224 bytes each).
 * Similar to atSingleton_D078_fw but with different element size.
 * 
 * @param array    Pointer to dynamic array structure
 * @return         Pointer to next available element
 */
void* atSingleton_2150_fw(void* array) {
    // Load current capacity and count
    uint32_t capacity = *(uint32_t*)((char*)array + 8);
    uint32_t count = *(uint32_t*)((char*)array + 4);
    
    // Check if array is full
    if (count == capacity) {
        // Expand capacity by 256 elements
        uint32_t newCapacity = capacity + 256;
        atSingleton_2038(array, newCapacity);
    }
    
    // Get base pointer to array data
    void* data = *(void**)array;
    
    // Calculate offset to next element (224 bytes per element)
    uint32_t offset = count * 224;
    
    // Increment count
    *(uint32_t*)((char*)array + 4) = count + 1;
    
    // Return pointer to next element
    return (void*)((char*)data + offset);
}


/* ── External dependencies for atSingleton bit stream ─────────────────────── */

/* Bit stream refill function @ 0x824D23C0 */
extern void atSingleton_23C0(void* pBitStream);


/* ═══════════════════════════════════════════════════════════════════════════
 * atSingleton_2688 @ 0x824D2688 | size: 0xc0 (192 bytes)
 *
 * Processes a bit stream by reading bytes and accumulating them into a 64-bit
 * buffer with bit shifting.
 *
 * This function is part of the atSingleton bit stream reader, which maintains
 * a sliding window of bits for efficient bit-level data access.
 *
 * Bit stream structure layout:
 *   +0x00 (0)    m_accumulator  - 64-bit bit accumulator (u64)
 *   +0x08 (8)    m_bitCount     - Number of valid bits in accumulator (s32)
 *   +0x0C (12)   m_readPos      - Current read position in buffer (u32)
 *   +0x10 (16)   m_endPos       - End position of buffer (u32)
 *   +0x14 (20)   m_errorFlag    - Error state flag (u32)
 *   +0x18 (24)   m_mode         - Stream mode (1 = needs refill) (u32)
 *
 * Algorithm:
 *   1. While read position <= end position:
 *      a. Read byte from buffer at read position
 *      b. Calculate shift amount: 40 - current bit count
 *      c. Shift byte left by shift amount
 *      d. Add shifted byte to accumulator
 *      e. Increment bit count by 8
 *      f. Increment read position
 *   2. If mode == 1 (refill needed), call refill function
 *   3. If bit count < -16, set error flag and reset bit count to 127
 *   4. Return 0 (success) or 1 (refill occurred)
 * ═══════════════════════════════════════════════════════════════════════════ */
int atSingleton_2688(void* pBitStream)
{
    uint8_t* stream = (uint8_t*)pBitStream;
    
    /* Load stream state */
    uint64_t accumulator = *(uint64_t*)(stream + 0);
    int32_t bitCount = *(int32_t*)(stream + 8);
    uint32_t readPos = *(uint32_t*)(stream + 12);
    uint32_t endPos = *(uint32_t*)(stream + 16);
    uint32_t mode = *(uint32_t*)(stream + 24);
    
    /* Read bytes from buffer while within bounds */
    while (readPos <= endPos) {
        /* Read next byte */
        uint8_t byte = stream[readPos];
        readPos++;
        
        /* Calculate shift amount (40 - bitCount) */
        int32_t shiftAmount = 40 - bitCount;
        
        /* Shift byte and add to accumulator */
        uint64_t shiftedByte = (uint64_t)byte << shiftAmount;
        accumulator += shiftedByte;
        
        /* Update bit count */
        bitCount += 8;
    }
    
    /* Store updated state */
    *(uint64_t*)(stream + 0) = accumulator;
    *(int32_t*)(stream + 8) = bitCount;
    *(uint32_t*)(stream + 12) = readPos;
    
    /* Check if bit count is valid */
    if (bitCount >= 0) {
        return 0;  /* Success */
    }
    
    /* Check if refill is needed (mode == 1) */
    if (mode == 1) {
        atSingleton_23C0(pBitStream);
        return 1;  /* Refill occurred */
    }
    
    /* Handle underflow error (bit count < -16) */
    if (bitCount < -16) {
        uint32_t errorFlag = *(uint32_t*)(stream + 20);
        if (errorFlag == 0) {
            *(uint32_t*)(stream + 20) = 2;  /* Set error flag */
        }
        *(int32_t*)(stream + 8) = 127;  /* Reset bit count */
    }
    
    return 0;
}


/* ── External dependencies for atSingleton list operations ────────────────── */

/* List node processing function @ 0x82450128 */
extern void atSingleton_0128_wrh(void* pNode, uint32_t param);


/* ═══════════════════════════════════════════════════════════════════════════
 * atSingleton_TraverseLinkedList @ 0x824489F8 | size: 0x4c (76 bytes)
 *
 * Traverses a linked list and calls a processing function on each node.
 *
 * This function iterates through a singly-linked list structure, calling
 * atSingleton_0128_wrh on each node with a provided parameter.
 *
 * atSingleton layout (partial):
 *   +0x2C (44)   m_listEnd     - End sentinel for linked list
 *   +0x30 (48)   m_listHead    - Head of linked list
 *
 * List node layout:
 *   -0x0C (-12)  m_data        - Node data (offset from node pointer)
 *   +0x04 (4)    m_next        - Pointer to next node
 *
 * Algorithm:
 *   1. Load list head from offset +48
 *   2. Calculate list end address (this + 44)
 *   3. While current node != list end:
 *      a. If node is non-null, calculate data pointer (node - 12)
 *      b. Call processing function with data pointer and parameter
 *      c. Load next node from current node + 4
 *   4. Return when list end is reached
 * ═══════════════════════════════════════════════════════════════════════════ */
void atSingleton_TraverseLinkedList(void* pThis, uint32_t param)
{
    uint8_t* singleton = (uint8_t*)pThis;
    
    /* Load list head and calculate list end address */
    uint32_t currentNode = *(uint32_t*)(singleton + 48);
    uint32_t listEnd = (uint32_t)(uintptr_t)(singleton + 44);
    
    /* Traverse linked list until we reach the end sentinel */
    while (currentNode != listEnd) {
        /* Calculate data pointer (node - 12) if node is non-null */
        void* pData = NULL;
        if (currentNode != 0) {
            pData = (void*)(currentNode - 12);
        }
        
        /* Call processing function on node data */
        atSingleton_0128_wrh(pData, param);
        
        /* Load next node pointer from current node + 4 */
        currentNode = *(uint32_t*)(currentNode + 4);
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// atSingleton Batch: 10 functions (108B–284B)
// Physics write-back, hash table probe, drawable field loading,
// array destructor, weight accumulation, capacity-gated insertion,
// input readiness check
// ═══════════════════════════════════════════════════════════════════════════

/* ── External dependencies ────────────────────────────────────────────────── */

extern void ph_5908(void* physObj, const char* tag, int mode);
extern void rage_3F18(void* globalTable, void* dataBlock, void* destBuffer);
extern void ke_0E08(void* destTransform, void* srcTransform);
extern int32_t pg_C4E8_g(int32_t value, int32_t rangeMin, int32_t rangeMax);
extern void* atSingleton_29E0_g(const void* key);
extern void rage_free_00C0(void* ptr);
extern uint8_t atSingleton_Find_90D0(void* ptr);
extern uint8_t atSingleton_7068_fw(void* hashMap, uint32_t playerIndex, void* entry);

/* Global pointers */
extern uint32_t* lbl_8271A374;          // physics write-back global table
extern uint32_t* lbl_8271A324;          // player slot table base
extern uint32_t* lbl_8271A364;          // session state pointer
extern uint32_t* g_input_obj_ptr;       // @ 0x825EAB28
extern uint32_t* g_render_obj_ptr;      // @ 0x825EAB2C
extern uint32_t  lbl_825CA1A0[];        // network session object A
extern uint32_t  lbl_825CA1B4[];        // network session object B

/* Float constant for weight accumulation (likely 0.0f) */
extern const float lbl_8202D110;        // @ 0x8202D110

/* String constants for physics tags */
extern const char lbl_82027660[];       // @ 0x82027660 - physics write tag A
extern const char lbl_8202766C[];       // @ 0x8202766C - physics write tag B


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_3680_w @ 0x82123680 | size: 0x6C (108 bytes)
// Writes physics simulation data from a data block into a bone transform.
// Loads the data block from the singleton, copies via the global table,
// then applies the result to a transform at offset +176.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_3680_w(void* pBoneState, void* pSingleton) {
    uint8_t* singleton = (uint8_t*)pSingleton;
    uint8_t* boneState = (uint8_t*)pBoneState;

    // Get the physics object pointer from singleton +4
    void* physObj = *(void**)(singleton + 4);

    // Begin physics write-back (tag A)
    ph_5908(physObj, lbl_82027660, 1);

    // Reload physics object (may have changed during ph_5908)
    void* physObjReloaded = *(void**)(singleton + 4);

    // Get the data block pointer at offset +12 within the physics object
    void* dataBlock = *(void**)((uint8_t*)physObjReloaded + 12);

    // Copy data from global table through data block into dest buffer at +168
    void* destBuffer = (void*)(boneState + 168);
    rage_3F18(lbl_8271A374, dataBlock, destBuffer);

    // Reload physics object again for end tag
    void* physObjFinal = *(void**)(singleton + 4);

    // End physics write-back (tag B)
    ph_5908(physObjFinal, lbl_8202766C, 1);

    // Apply the transform: copy from +168 source into +176 destination
    void* srcTransform = *(void**)(boneState + 168);
    ke_0E08((void*)(boneState + 176), srcTransform);
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_36F0_w @ 0x821236F0 | size: 0x6C (108 bytes)
// Same pattern as atSingleton_3680_w but with different bone offsets.
// Writes physics data into transform at offsets +172 and +380.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_36F0_w(void* pBoneState, void* pSingleton) {
    uint8_t* singleton = (uint8_t*)pSingleton;
    uint8_t* boneState = (uint8_t*)pBoneState;

    // Get the physics object pointer from singleton +4
    void* physObj = *(void**)(singleton + 4);

    // Begin physics write-back (tag A)
    ph_5908(physObj, lbl_82027660, 1);

    // Reload physics object
    void* physObjReloaded = *(void**)(singleton + 4);

    // Get the data block pointer at offset +12 within the physics object
    void* dataBlock = *(void**)((uint8_t*)physObjReloaded + 12);

    // Copy data from global table through data block into dest buffer at +172
    void* destBuffer = (void*)(boneState + 172);
    rage_3F18(lbl_8271A374, dataBlock, destBuffer);

    // Reload physics object again for end tag
    void* physObjFinal = *(void**)(singleton + 4);

    // End physics write-back (tag B)
    ph_5908(physObjFinal, lbl_8202766C, 1);

    // Apply the transform: copy from +172 source into +380 destination
    void* srcTransform = *(void**)(boneState + 172);
    ke_0E08((void*)(boneState + 380), srcTransform);
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_4EF0 @ 0x82124EF0 | size: 0x7C (124 bytes)
// (aliased as fragDrawable_vfn_3)
// Loads a named field from a data source into a fragDrawable.
// Reads a 128-byte field name string via vtable call, compares it
// case-insensitively against a known key. If it doesn't match, hashes
// the field name and stores the result at offset +288, freeing the old value.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_4EF0(void* pDrawable, void* pDataSource) {
    uint8_t* drawable = (uint8_t*)pDrawable;
    uint8_t* dataSource = (uint8_t*)pDataSource;

    // Get the inner object from data source +4
    void* innerObj = *(void**)(dataSource + 4);

    // Call vtable slot 1 to read a field name string (up to 128 bytes)
    // into a local buffer (stack variable in original)
    char fieldNameBuffer[128];
    // vtable slot 1: ReadString(innerObj, buffer, maxLen)
    void** vtable = *(void***)innerObj;
    typedef void (*ReadStringFn)(void*, char*, int);
    ReadStringFn readString = (ReadStringFn)vtable[1];
    readString(innerObj, fieldNameBuffer, 128);

    // Compare field name against known key (case-insensitive)
    extern int _stricmp(const char* a, const char* b);
    extern const char lbl_8202769C[];  // known field key string
    if (_stricmp(fieldNameBuffer, lbl_8202769C) != 0) {
        // Hash the field name to get a data object
        void* fieldData = atSingleton_29E0_g(fieldNameBuffer);

        // Free the old value at offset +288
        void* oldValue = *(void**)(drawable + 288);
        rage_free_00C0(oldValue);

        // Store the new hashed field data
        *(void**)(drawable + 288) = fieldData;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_dtor_3468 @ 0x820F3468 | size: 0xA0 (160 bytes)
// Destructor for a dynamic pointer array with singleton ownership tracking.
// Iterates backwards through the array, checking each element against
// the singleton registry. Elements not owned by singletons are freed
// via the system allocator. Finally frees the array storage itself.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_dtor_3468(void* pArray) {
    uint8_t* arrayObj = (uint8_t*)pArray;

    // Check element count at offset +6 (uint16)
    uint16_t elementCount = *(uint16_t*)(arrayObj + 6);
    if (elementCount == 0) {
        return;
    }

    // Get the data pointer at offset +0
    uint32_t* dataPtr = *(uint32_t**)(arrayObj);
    if (dataPtr == nullptr) {
        return;
    }

    // The count is stored at dataPtr[-1] (4 bytes before the data start)
    uint32_t* countPtr = dataPtr - 1;
    uint32_t count = *countPtr;
    int32_t idx = count - 1;

    // Calculate end pointer: dataPtr + (count * 4) bytes
    uint8_t* elementPtr = (uint8_t*)dataPtr + (count * 4);

    // Iterate backwards through the array
    while (idx >= 0) {
        elementPtr -= 4;
        void* element = *(void**)elementPtr;

        if (element != nullptr) {
            // Check if element is owned by a singleton
            uint8_t isSingleton = atSingleton_Find_90D0(element);
            if (!isSingleton) {
                // Not singleton-owned: free via system allocator
                // Load allocator from SDA global at r13+0 -> offset +4 -> vtable slot 2
                extern uint32_t lbl_82600000;
                void* allocator = *(void**)((uint8_t*)&lbl_82600000 + 4);
                void** allocVtable = *(void***)allocator;
                typedef void (*FreeFn)(void*, void*);
                FreeFn freeFn = (FreeFn)allocVtable[2];
                freeFn(allocator, element);
            }
        }

        idx--;
    }

    // Free the array storage (countPtr points to allocation start)
    rage_free_00C0(countPtr);
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_8128_g @ 0x82118128 | size: 0x9C (156 bytes)
// Hash table linear probe search.
// Starting from a given index, searches forward (with wrapping) through
// a hash table for an entry whose active flag (byte at offset +16) is set.
// If wrapFlag is true, clamps the start index to valid range first.
// Returns the index of the found entry, or -1 if none found.
// ─────────────────────────────────────────────────────────────────────────────
int32_t atSingleton_8128_g(void* pHashTable, uint32_t startIndex, uint32_t stride, uint8_t wrapFlag) {
    uint8_t* hashTable = (uint8_t*)pHashTable;

    // Combine start index with stride to get effective start position
    int32_t searchIdx = (int32_t)(startIndex + stride);

    // If wrapFlag is set, clamp the index to valid range [0, capacity-1]
    if (wrapFlag) {
        uint16_t capacity = *(uint16_t*)(hashTable + 28);
        searchIdx = pg_C4E8_g(searchIdx, 0, (int32_t)capacity - 1);
    }

    // Linear probe: search forward through the table
    if (searchIdx >= 0) {
        uint16_t capacity = *(uint16_t*)(hashTable + 28);
        uint32_t entryOffset = (uint32_t)searchIdx * 4;
        uint32_t strideBytes = stride * 4;

        while (searchIdx < (int32_t)capacity) {
            // Get the entry pointer from the table at offset +24
            uint32_t* tableData = *(uint32_t**)(hashTable + 24);
            void* entry = *(void**)((uint8_t*)tableData + entryOffset);

            // Check if the active flag at entry +16 is set
            uint8_t activeFlag = *(uint8_t*)((uint8_t*)entry + 16);
            if (activeFlag != 0) {
                return searchIdx;
            }

            // Advance by stride
            searchIdx += (int32_t)stride;
            entryOffset += strideBytes;

            // If we wrapped below zero, stop
            if (searchIdx < 0) {
                break;
            }
        }
    }

    return -1;
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_AccumulateWeights @ 0x82118438 | size: 0xA8 (168 bytes)
// Accumulates float weights from an array of object pointers.
// Each object has a weight float at offset +144. The function sums all
// weights, processing 4 elements at a time (loop unrolling) for elements
// at index >= 4, then handles the remainder one at a time.
// Returns the accumulated sum in f1.
// ─────────────────────────────────────────────────────────────────────────────
float atSingleton_AccumulateWeights(void* pWeightArray) {
    uint8_t* weightArray = (uint8_t*)pWeightArray;

    // Read element count from offset +427 (byte field)
    uint8_t elementCount = *(uint8_t*)(weightArray + 427);

    // Start with initial weight value from constant (likely 0.0f)
    float totalWeight = lbl_8202D110;

    // Unrolled loop index: process 4 elements at a time starting from index 4
    int32_t unrolledIdx = 0;
    if (elementCount >= 4) {
        // Get the pointer array at offset +180
        uint32_t** ptrArray = *(uint32_t***)(weightArray + 180);

        // Calculate number of groups of 4: (count - 4) / 4 + 1
        uint32_t adjustedCount = (uint32_t)(elementCount - 4);
        uint32_t numGroups = (adjustedCount >> 2) + 1;
        unrolledIdx = (int32_t)(numGroups * 4);

        // Process 4 entries per iteration
        uint32_t** entryPtr = ptrArray + 2;  // start at index 2 (offset +8 from base)
        for (uint32_t group = numGroups; group > 0; group--) {
            float w0 = *(float*)((uint8_t*)entryPtr[-2] + 144);
            float w1 = *(float*)((uint8_t*)entryPtr[-1] + 144);
            float w2 = *(float*)((uint8_t*)entryPtr[0] + 144);
            float w3 = *(float*)((uint8_t*)entryPtr[1] + 144);

            totalWeight = w0 + totalWeight;
            totalWeight = w1 + totalWeight;
            totalWeight = w2 + totalWeight;
            totalWeight = w3 + totalWeight;

            entryPtr += 4;
        }
    }

    // Handle remaining elements (index unrolledIdx to elementCount-1)
    if (unrolledIdx < (int32_t)elementCount) {
        uint32_t** ptrArray = *(uint32_t***)(weightArray + 180);
        int32_t remaining = (int32_t)elementCount - unrolledIdx;
        uint32_t** entryPtr = ptrArray + unrolledIdx;

        for (int32_t i = remaining; i > 0; i--) {
            float weight = *(float*)((uint8_t*)*entryPtr + 144);
            totalWeight = weight + totalWeight;
            entryPtr++;
        }
    }

    return totalWeight;
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_HashMapInsertA @ 0x82117D28 | size: 0xE4 (228 bytes)
// Capacity-gated hash map insertion (variant A).
// Checks if the entry count is below the maximum capacity. If room exists,
// reads the player slot entry for the given index, calls vtable slot 2
// to get a sequence number, validates it against min/max thresholds,
// then calls the virtual GetKey method and inserts via atSingleton_7068_fw.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_HashMapInsertA(void* pHashMap, uint32_t playerIndex, void* pEntry) {
    uint8_t* entry = (uint8_t*)pEntry;

    // Check capacity: count (+4) vs max (+8)
    int32_t count = *(int32_t*)(entry + 4);
    int32_t maxCapacity = *(int32_t*)(entry + 8);
    if (count >= maxCapacity) {
        return;
    }

    // Get the player slot table and compute slot address
    // playerIndex * 8 gives the byte offset into the slot table
    uint8_t* slotTable = (uint8_t*)lbl_8271A324;
    uint8_t* slotEntry = slotTable + (playerIndex * 8);

    // Get the network object at slot offset +252
    void* netObj = *(void**)(slotEntry + 252);

    // Call vtable slot 2 on netObj with arg=20 to get sequence number
    void** netVtable = *(void***)netObj;
    typedef void* (*GetSequenceFn)(void*, int);
    GetSequenceFn getSequence = (GetSequenceFn)netVtable[2];
    void* seqResult = getSequence(netObj, 20);

    // Check sequence number against minimum threshold at entry +36
    int32_t seqNum = *(int32_t*)seqResult;
    int32_t minThreshold = *(int32_t*)(entry + 36);
    if (seqNum < minThreshold) {
        return;
    }

    // Re-read netObj vtable and call slot 2 again with arg=2 for key lookup
    void** netVtable2 = *(void***)netObj;
    typedef void* (*GetKeyFn)(void*, int);
    GetKeyFn getKey = (GetKeyFn)netVtable2[2];
    void* keyResult = getKey(netObj, 2);

    // Check key value against maximum threshold at entry +40
    int32_t keyVal = *(int32_t*)keyResult;
    int32_t maxThreshold = *(int32_t*)(entry + 40);
    if (keyVal > maxThreshold) {
        return;
    }

    // Call the entry's virtual function (vtable slot 0) to get the insert value
    void** entryVtable = *(void***)entry;
    typedef void* (*GetValueFn)(void*);
    GetValueFn getValue = (GetValueFn)entryVtable[0];
    void* insertValue = getValue(entry);

    // Insert into the hash map
    uint8_t inserted = atSingleton_7068_fw(pHashMap, playerIndex, insertValue);
    if (inserted) {
        // Increment the entry count
        int32_t currentCount = *(int32_t*)(entry + 4);
        *(int32_t*)(entry + 4) = currentCount + 1;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_HashMapInsertB @ 0x82117E10 | size: 0xDC (220 bytes)
// Capacity-gated hash map insertion (variant B).
// Similar to atSingleton_HashMapInsertA but uses a different sequence threshold:
// reads frame counts from the render object and sums them, comparing
// against the entry's minimum threshold before proceeding.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_HashMapInsertB(void* pHashMap, uint32_t playerIndex, void* pEntry) {
    uint8_t* entry = (uint8_t*)pEntry;

    // Check capacity: count (+4) vs max (+8)
    int32_t count = *(int32_t*)(entry + 4);
    int32_t maxCapacity = *(int32_t*)(entry + 8);
    if (count >= maxCapacity) {
        return;
    }

    // Load the render object and read frame counts at offsets +28 and +32
    int32_t minThreshold = *(int32_t*)(entry + 36);
    void* renderObj = *(void**)g_render_obj_ptr;
    int32_t frameCountA = *(int32_t*)((uint8_t*)renderObj + 28);
    int32_t frameCountB = *(int32_t*)((uint8_t*)renderObj + 32);
    int32_t totalFrames = frameCountA + frameCountB + 1;

    // Get the player slot table and compute slot address
    uint8_t* slotTable = (uint8_t*)lbl_8271A324;
    uint8_t* slotEntry = slotTable + (playerIndex * 8);
    void* netObj = *(void**)(slotEntry + 252);

    // Check total frames against minimum threshold
    if (totalFrames < minThreshold) {
        return;
    }

    // Call vtable slot 2 on netObj with arg=13 to get sequence value
    void** netVtable = *(void***)netObj;
    typedef void* (*GetSequenceFn)(void*, int);
    GetSequenceFn getSequence = (GetSequenceFn)netVtable[2];
    void* seqResult = getSequence(netObj, 13);

    // Check sequence value against maximum threshold at entry +40
    int32_t seqVal = *(int32_t*)seqResult;
    int32_t maxThreshold = *(int32_t*)(entry + 40);
    if (seqVal > maxThreshold) {
        return;
    }

    // Call the entry's virtual function (vtable slot 0) to get insert value
    void** entryVtable = *(void***)entry;
    typedef void* (*GetValueFn)(void*);
    GetValueFn getValue = (GetValueFn)entryVtable[0];
    void* insertValue = getValue(entry);

    // Insert into the hash map
    uint8_t inserted = atSingleton_7068_fw(pHashMap, playerIndex, insertValue);
    if (inserted) {
        // Increment the entry count
        int32_t currentCount = *(int32_t*)(entry + 4);
        *(int32_t*)(entry + 4) = currentCount + 1;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_HashMapInsertC @ 0x82117EF0 | size: 0xB8 (184 bytes)
// Capacity-gated hash map insertion (variant C).
// Checks capacity, then reads the player slot's network object, calls
// vtable slot 2 with arg=21 to get a sequence number, validates against
// min threshold, then calls the entry's virtual GetKey to insert.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_HashMapInsertC(void* pHashMap, uint32_t playerIndex, void* pEntry) {
    uint8_t* entry = (uint8_t*)pEntry;

    // Check capacity: count (+4) vs max (+8)
    int32_t count = *(int32_t*)(entry + 4);
    int32_t maxCapacity = *(int32_t*)(entry + 8);
    if (count >= maxCapacity) {
        return;
    }

    // Get the player slot table and compute slot address
    uint8_t* slotTable = (uint8_t*)lbl_8271A324;
    uint8_t* slotEntry = slotTable + (playerIndex * 8);

    // Get the network object at slot offset +252
    void* netObj = *(void**)(slotEntry + 252);

    // Call vtable slot 2 on netObj with arg=21 to get sequence number
    void** netVtable = *(void***)netObj;
    typedef void* (*GetSequenceFn)(void*, int);
    GetSequenceFn getSequence = (GetSequenceFn)netVtable[2];
    void* seqResult = getSequence(netObj, 21);

    // Check sequence number against minimum threshold at entry +36
    int32_t seqNum = *(int32_t*)seqResult;
    int32_t minThreshold = *(int32_t*)(entry + 36);
    if (seqNum < minThreshold) {
        return;
    }

    // Call the entry's virtual function (vtable slot 0) to get insert value
    void** entryVtable = *(void***)entry;
    typedef void* (*GetValueFn)(void*);
    GetValueFn getValue = (GetValueFn)entryVtable[0];
    void* insertValue = getValue(entry);

    // Insert into the hash map
    uint8_t inserted = atSingleton_7068_fw(pHashMap, playerIndex, insertValue);
    if (inserted) {
        // Increment the entry count
        int32_t currentCount = *(int32_t*)(entry + 4);
        *(int32_t*)(entry + 4) = currentCount + 1;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_7FA8_w @ 0x82117FA8 | size: 0x11C (284 bytes)
// Checks whether the game is ready to accept player input.
// Evaluates multiple conditions: network session state, session readiness
// flag, input device connection count, keyboard/button input state,
// and controller connection status. Returns true only when all conditions
// indicate the game is in a valid input-accepting state.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t atSingleton_7FA8_w() {
    // Check network session object A: if session pointer (+4) is non-null, input blocked
    uint32_t* sessionObjA = (uint32_t*)((uint8_t*)lbl_825CA1A0 + 4);
    if (*sessionObjA != 0) {
        return 0;
    }

    // Check session state: load session state pointer, check readiness flag at +60
    void* sessionState = *(void**)lbl_8271A364;
    uint8_t readyFlag = *(uint8_t*)((uint8_t*)sessionState + 60);
    if (readyFlag == 0) {
        return 0;
    }

    // Check input object: device connection count at +20 must be > 0
    void* inputObj = *(void**)g_input_obj_ptr;
    int32_t deviceCount = *(int32_t*)((uint8_t*)inputObj + 20);
    if (deviceCount > 0) {
        return 0;
    }

    // Check keyboard/button input state at +334 (byte flag)
    uint8_t keyboardActive = *(uint8_t*)((uint8_t*)inputObj + 334);
    if (keyboardActive != 0) {
        // Keyboard is active, input allowed regardless of other checks
    } else {
        // Check alternate input flag at +340
        int32_t altInputFlag = *(int32_t*)((uint8_t*)inputObj + 340);
        bool hasAltInput = (altInputFlag != 0);
        if (!hasAltInput) {
            // No keyboard and no alt input: input not ready
            return 0;
        }
    }

    // Check controller connection: both slots at +56 and +60 must be connected (==1)
    int32_t controllerSlotA = *(int32_t*)((uint8_t*)inputObj + 56);
    int32_t controllerSlotB = *(int32_t*)((uint8_t*)inputObj + 60);

    bool slotAConnected = (controllerSlotA == 1);
    bool slotBConnected = (controllerSlotB == 1);

    int32_t connectedCount = (slotAConnected ? 1 : 0) + (slotBConnected ? 1 : 0);

    // Both controllers must be connected (count >= 2)
    if (connectedCount >= 2) {
        // Both connected: check network session object B
        uint32_t* sessionObjB = (uint32_t*)((uint8_t*)lbl_825CA1B4 + 4);
        if (*sessionObjB != 0) {
            return 0;
        }
    }

    // All checks passed: input is ready
    return 1;
}

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton MI-base Adjustor Thunks (Virtual Destructor Forwarding)
// ═══════════════════════════════════════════════════════════════════════════

/* External destructor targets */
extern void atSingleton_vfn_0_A8F8_1(void* obj, uint32_t flags);
extern void atSingleton_vfn_0_AA28_1(void* obj, uint32_t flags);
extern void atSingleton_vfn_0_AC18_1(void* obj, uint32_t flags);
extern void atSingleton_vfn_0_AC78_1(void* obj, uint32_t flags);
extern void atSingleton_vfn_0_AAA0_1(void* obj, uint32_t flags);

/* HSM context handler for mode 3 routing */
extern void hsmContext_5ED8_w(void* obj);

/**
 * atSingleton_rtti_CE54_0  @ 0x821CAD40 | size: 0x8
 *
 * MI-base adjustor thunk. Adjusts `this` pointer by -4 bytes to recover
 * the primary base, then delegates to atSingleton_vfn_0_A8F8_1 destructor.
 */
void atSingleton_rtti_CE54_0(void* adjustedThis, uint32_t flags) {
    void* realThis = (void*)((char*)adjustedThis - 4);
    atSingleton_vfn_0_A8F8_1(realThis, flags);
}

/**
 * atSingleton_vfn_0_AD48_1  @ 0x821CAD48 | size: 0x8
 *
 * MI-base adjustor thunk. Adjusts `this` pointer by -4 bytes to recover
 * the primary base, then delegates to atSingleton_vfn_0_AA28_1 destructor.
 */
void atSingleton_vfn_0_AD48_1(void* adjustedThis, uint32_t flags) {
    void* realThis = (void*)((char*)adjustedThis - 4);
    atSingleton_vfn_0_AA28_1(realThis, flags);
}

/**
 * atSingleton_rtti_CFA4_0  @ 0x821CAD50 | size: 0x8
 *
 * MI-base adjustor thunk. Adjusts `this` pointer by -4 bytes to recover
 * the primary base, then delegates to atSingleton_vfn_0_AC18_1 destructor.
 */
void atSingleton_rtti_CFA4_0(void* adjustedThis, uint32_t flags) {
    void* realThis = (void*)((char*)adjustedThis - 4);
    atSingleton_vfn_0_AC18_1(realThis, flags);
}

/**
 * atSingleton_rtti_CFF4_0  @ 0x821CAD60 | size: 0x8
 *
 * MI-base adjustor thunk. Adjusts `this` pointer by -4 bytes to recover
 * the primary base, then delegates to atSingleton_vfn_0_AC78_1 destructor.
 */
void atSingleton_rtti_CFF4_0(void* adjustedThis, uint32_t flags) {
    void* realThis = (void*)((char*)adjustedThis - 4);
    atSingleton_vfn_0_AC78_1(realThis, flags);
}

/**
 * atSingleton_rtti_D01C_0  @ 0x821CAD68 | size: 0x8
 *
 * MI-base adjustor thunk. Adjusts `this` pointer by -4 bytes to recover
 * the primary base, then delegates to atSingleton_vfn_0_AAA0_1 destructor.
 */
void atSingleton_rtti_D01C_0(void* adjustedThis, uint32_t flags) {
    void* realThis = (void*)((char*)adjustedThis - 4);
    atSingleton_vfn_0_AAA0_1(realThis, flags);
}

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton State Machine Dispatchers
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSingleton_62F0_w  @ 0x821C62F0 | size: 0x2C
 *
 * Conditional state dispatcher. Checks global game mode and routes
 * to the HSM context handler for mode 1, otherwise clears the
 * completion flag at offset 0x27EC.
 *
 * @param obj    Pointer to game state object
 */
void atSingleton_62F0_w(void* obj) {
    // Load global game state pointer @ 0x8271A33C
    uint32_t* globalState = *(uint32_t**)0x8271A33C;

    // Load mode from global state offset +12
    int32_t mode = *(int32_t*)((char*)globalState + 12);

    if (mode == 1) {
        hsmContext_5BC8_fw(obj);
        return;
    }

    // Default: Clear flag at offset 0x27EC
    *(uint32_t*)((char*)obj + 0x27EC) = 0;
}

/**
 * atSingleton_DispatchMode1Only  @ 0x821C70F8 | size: 0x2C
 *
 * Conditional state dispatcher. Checks global game mode and routes
 * to the jump table handler for mode 1, otherwise clears the
 * completion flag at offset 0x27EC.
 *
 * @param obj    Pointer to game state object
 */
void atSingleton_DispatchMode1Only(void* obj) {
    // Load global game state pointer @ 0x8271A33C
    uint32_t* globalState = *(uint32_t**)0x8271A33C;

    // Load mode from global state offset +12
    int32_t mode = *(int32_t*)((char*)globalState + 12);

    if (mode == 1) {
        jumptable_5C20(obj);
        return;
    }

    // Default: Clear flag at offset 0x27EC
    *(uint32_t*)((char*)obj + 0x27EC) = 0;
}

/**
 * atSingleton_DispatchMode1And3  @ 0x821C72E8 | size: 0x38
 *
 * Conditional state dispatcher with two-way routing. Checks global
 * game mode: routes to jump table handler for mode 1, to HSM context
 * exit handler for mode 3, otherwise clears the completion flag.
 *
 * @param obj    Pointer to game state object
 */
void atSingleton_DispatchMode1And3(void* obj) {
    // Load global game state pointer @ 0x8271A33C
    uint32_t* globalState = *(uint32_t**)0x8271A33C;

    // Load mode from global state offset +12
    int32_t mode = *(int32_t*)((char*)globalState + 12);

    if (mode == 1) {
        jumptable_5C20(obj);
        return;
    }

    if (mode == 3) {
        hsmContext_5ED8_w(obj);
        return;
    }

    // Default: Clear flag at offset 0x27EC
    *(uint32_t*)((char*)obj + 0x27EC) = 0;
}

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton Flag / Status Query
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSingleton_9C18_2hr  @ 0x820D9C18 | size: 0x54
 *
 * Checks whether a deeply-nested flag indicates availability.
 * Traverses a global pointer chain to reach a status word, then
 * checks bit 0 (active/busy) and bit 2 (ready) to determine
 * if the resource is available.
 *
 * @return  1 if available (not active AND ready-bit set), 0 otherwise
 */
uint8_t atSingleton_9C18_2hr() {
    // Load global pointer chain: @ 0x8271A2F8 -> +20 -> +9736 -> +0
    uint32_t* globalPtr = *(uint32_t**)0x8271A2F8;
    uint32_t* level1 = *(uint32_t**)((char*)globalPtr + 20);
    uint32_t* level2 = *(uint32_t**)((char*)level1 + 9736);
    uint32_t statusWord = *level2;

    // Check bit 0: if set, resource is active/busy -> not available
    if (statusWord & 0x1) {
        return 0;
    }

    // Check bit 2: if set, resource is ready -> available
    if (statusWord & 0x4) {
        return 1;
    }

    return 0;
}

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton Array Initialization
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSingleton_InitWeightArray  @ 0x821CB978 | size: 0x98
 *
 * Initializes a large array of 900 entries (48 bytes each) starting at
 * offset +0 of the object. Each entry consists of:
 *   - float at +0   (set to 0.0f)
 *   - int32 at +4   (set to -1)
 *   - uint16 at +8  (set to 0)
 *   - uint16 at +10 (set to 0)
 *   - float at +16  (set to 0.0f)
 *   - float at +20  (set to 0.0f)
 *   - float at +24  (set to 0.0f)
 *   - float at +32  (set to 0.0f)
 *
 * After the array, initializes a trailing record at offset 43200 with
 * similar structure, and clears 3 uint32 fields at offsets 43200, 43204, 43208.
 *
 * @param obj    Pointer to the object to initialize
 */
void atSingleton_InitWeightArray(void* obj) {
    uint8_t* base = (uint8_t*)obj;
    float zero = 0.0f;

    // Initialize 900 entries, 48 bytes each
    for (int32_t i = 899; i >= 0; i--) {
        uint8_t* entry = base + (899 - i) * 48;
        *(float*)(entry + 0) = zero;
        *(int32_t*)(entry + 4) = -1;
        *(uint16_t*)(entry + 8) = 0;
        *(uint16_t*)(entry + 10) = 0;
        *(float*)(entry + 16) = zero;
        *(float*)(entry + 20) = zero;
        *(float*)(entry + 24) = zero;
        *(float*)(entry + 32) = zero;
    }

    // Clear trailing control fields
    *(uint32_t*)(base + 43208) = 0;  // offset 0xA8C8
    *(uint32_t*)(base + 43204) = 0;  // offset 0xA8C4
    *(uint32_t*)(base + 43200) = 0;  // offset 0xA8C0

    // Initialize trailing record at offset 43216 (0x82XX + 65536 - 22320)
    uint8_t* trailing = base + 43216;
    *(float*)(trailing + 0) = zero;
    *(int32_t*)(trailing + 4) = -1;
    *(uint16_t*)(trailing + 8) = 0;
    *(uint16_t*)(trailing + 10) = 0;
    *(float*)(trailing + 16) = zero;
    *(float*)(trailing + 20) = zero;
    *(float*)(trailing + 24) = zero;
    *(float*)(trailing + 32) = zero;
}
