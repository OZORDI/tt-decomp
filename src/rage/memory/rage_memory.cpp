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

// ═══════════════════════════════════════════════════════════════════════════
// atSingleton Template Functions
// ═══════════════════════════════════════════════════════════════════════════

/**
 * atSingleton_9728_2h @ 0x82169728 | size: 0x18
 * 
 * Merges flags from source object into target singleton's flags field.
 * Performs bitwise OR operation to combine flag bits.
 * 
 * @param target    Pointer to target singleton wrapper
 * @param source    Pointer to source object containing flags at offset +16
 */
void atSingleton_9728_2h(void* target, void* source) {
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
 * atSingleton_6F38_p33 @ 0x821C6F38 | size: 0x14
 * 
 * Clears a specific flag field and jumps to network client initialization.
 * Sets field at offset 0x27EC (10220) to zero.
 * 
 * @param obj    Pointer to singleton object
 */
void atSingleton_6F38_p33(void* obj) {
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
 * atSingleton_E3F0_p44 @ 0x82255E3F0 | size: 0x18
 * 
 * Clears a 16-byte region starting at offset 128.
 * Initializes four consecutive 32-bit fields to zero.
 * 
 * @param obj    Pointer to object
 */
void atSingleton_E3F0_p44(void* obj) {
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
 * atSingleton_6108_p33 @ 0x821C6108 | size: 0x38
 * 
 * Similar to atSingleton_6BC0_w but routes to different handler for mode 3.
 * Conditional state machine dispatcher with alternate mode 3 handler.
 * 
 * @param obj    Pointer to game state object
 */
void atSingleton_6108_p33(void* obj) {
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
 * atSingleton_89F8_2hr @ 0x824489F8 | size: 0x4c (76 bytes)
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
void atSingleton_89F8_2hr(void* pThis, uint32_t param)
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
