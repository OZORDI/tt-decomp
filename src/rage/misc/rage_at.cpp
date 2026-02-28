/**
 * rage_at.cpp — atSingleton and atSafeDLList implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * RAGE engine "at" subsystem - singleton pattern and safe doubly-linked lists
 */

#include "rage_at.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ConditionalVirtualDispatch @ 0x82132BA0 | size: 0x74
// 
// Conditionally calls either vtable slot 37 or slot 11 based on a flag at +0x4.
// If the flag is non-zero, calls slot 37 twice (once directly, once through
// another virtual dispatch). Otherwise, calls slot 11.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::ConditionalVirtualDispatch() {
    // Check flag at offset +0x4 (likely m_bInitialized or similar state flag)
    uint8_t flag = *(uint8_t*)((char*)this + 4);
    
    if (flag != 0) {
        // Path 1: Flag is set - call vtable slot 37
        // This appears to be a cleanup or finalization method
        typedef void (*VTableFunc37)(void*);
        void** vtable = *(void***)this;
        VTableFunc37 func37 = (VTableFunc37)vtable[37];
        func37(this);
        
        // Call slot 37 again through another virtual dispatch
        // The second parameter (0x82027D04) is a 2-byte .rdata constant
        // likely used as a parameter or flag
        vtable = *(void***)this;
        func37 = (VTableFunc37)vtable[37];
        // Note: Original assembly suggests this might take a parameter
        // but the exact signature is unclear from the scaffold
        func37(this);
    } else {
        // Path 2: Flag is clear - call vtable slot 11
        // This appears to be a different operation, possibly initialization
        typedef void (*VTableFunc11)(void*);
        void** vtable = *(void***)this;
        VTableFunc11 func11 = (VTableFunc11)vtable[11];
        func11(this);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::LookupStringIndex @ 0x8225F250 | size: 0x70
// 
// Searches for a string in a global lookup table and returns its index + 0x1001.
// The lookup table is at 0x825C5320 and contains 35 string pointer entries.
// Returns 0 if not found, otherwise returns (index + 0x1001) masked to 16 bits.
// ─────────────────────────────────────────────────────────────────────────────
uint16_t atSingleton::LookupStringIndex(const char* searchStr) {
    // Global string lookup table at 0x825C5320
    // Each entry is 8 bytes: [const char* str, uint32_t data]
    extern const char* g_atSingletonStringTable[];  // 35 entries
    
    const char** tablePtr = (const char**)0x825C5320;
    const char** tableEnd = (const char**)((uintptr_t)tablePtr + 280);
    
    int index = 0;
    
    // Iterate through the lookup table
    while (tablePtr < tableEnd) {
        const char* tableStr = *tablePtr;
        const char* searchPtr = searchStr;
        const char* tableStrPtr = tableStr;
        
        // Compare strings character by character
        int diff = 0;
        while (*tableStrPtr != '\0') {
            diff = *tableStrPtr - *searchPtr;
            if (diff != 0) {
                break;
            }
            tableStrPtr++;
            searchPtr++;
        }
        
        // If strings match (diff == 0 after null terminator)
        if (diff == 0) {
            // Return index + 0x1001, masked to 16 bits
            return (uint16_t)((index + 0x1001) & 0xFFFF);
        }
        
        // Move to next entry (8 bytes per entry)
        tablePtr = (const char**)((uintptr_t)tablePtr + 8);
        index++;
    }
    
    // Not found
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::HandleStateTransition @ 0x8245C108 | size: 0xF4
// [vtable slot 21]
//
// State machine handler for singleton lifecycle transitions.
// Manages transitions between states 3, 4, and 5 based on current state
// and input parameter.
//
// State values (at offset +32):
//   3 = Ready/Idle
//   4 = Active/Processing
//   5 = Complete/Finalized
//
// @param transitionType Type of transition requested (0 or 1)
// @return 0 on success
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::HandleStateTransition(int transitionType) {
    int result = 0;
    
    // Load current state from offset +32
    int currentState = *(int*)((char*)this + 32);
    
    // Handle state 5 (Complete) - no transitions allowed
    if (currentState == 5) {
        return result;
    }
    
    // Handle state 4 (Active)
    if (currentState == 4) {
        int counter = *(int*)((char*)this + 16);
        
        // If counter is 0 and transitionType is 0, return early
        if (counter == 0 && transitionType == 0) {
            return result;
        }
    }
    
    // Check if we're in state 3 or 4
    if (currentState == 3 || (currentState == 4 && transitionType == 0)) {
        // Handle transition based on type
        if (transitionType < 1) {
            // Type 0: Transition to state 4 with initialization
            uint8_t flags = *((uint8_t*)this + 212);
            flags |= 0x80;  // Set bit 7
            *((uint8_t*)this + 212) = flags;
            
            // Set counter to 1
            *(int*)((char*)this + 16) = 1;
            
            // Call state transition helper
            extern void atSingleton_AA58_g(void*, int);
            atSingleton_AA58_g(this, 4);
            
            // Get object at offset +36 and call its vtable slot 5
            void* obj = *(void**)((char*)this + 36);
            typedef void (*VTableFunc5)(void*, int);
            void** vtable = *(void***)obj;
            VTableFunc5 func5 = (VTableFunc5)vtable[5];
            func5(obj, 1);
            
            return result;
        } else if (transitionType == 1) {
            // Type 1: Transition to state 4 without initialization
            extern void atSingleton_AA58_g(void*, int);
            atSingleton_AA58_g(this, 4);
            
            // Get object at offset +36 and call its vtable slot 12
            void* obj = *(void**)((char*)this + 36);
            typedef void (*VTableFunc12)(void*);
            void** vtable = *(void***)obj;
            VTableFunc12 func12 = (VTableFunc12)vtable[12];
            func12(obj);
            
            // Clear flag at offset +56
            *((uint8_t*)this + 56) = 0;
            
            return result;
        }
    }
    
    // Default: transition to state 5 (Complete)
    extern void atSingleton_AA58_g(void*, int);
    atSingleton_AA58_g(this, 5);
    
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::InitializeFlags @ 0x824DA7B0 | size: 0x30
//
// Conditionally initializes three flag fields based on a check at offset +3628.
// If the field at +3628 is 0, returns 1 (early exit).
// Otherwise, sets fields at +280, +3352, and +19376 to 1, and returns 0.
//
// @return 1 if already initialized (field at +3628 is 0), 0 if flags were set
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::InitializeFlags() {
    // Check initialization flag at offset +3628
    int checkValue = *(int*)((char*)this + 3628);
    
    if (checkValue == 0) {
        // Already initialized - return 1
        return 1;
    }
    
    // Not initialized - set three flags to 1
    *(int*)((char*)this + 280) = 1;
    *(int*)((char*)this + 3352) = 1;
    *(int*)((char*)this + 19376) = 1;
    
    // Return 0 to indicate flags were set
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::Constructor @ 0x821CB418 | size: 0x510
//
// Primary constructor for atSingleton - initializes a massive structure with
// multiple arrays and sub-structures. This appears to be a template-based
// singleton manager that can handle up to 900 instances of various types.
//
// Structure layout (approximate):
// - Offset 0: vtable pointer
// - Offsets 16-16400: Array of 128 vector-aligned structures (128 bytes each)
// - Offsets 16416+: Array of 900 elements (48 bytes each)
// - Offsets 70512+: First managed sub-structure (43200 bytes)
// - Offsets 113776+: Second managed sub-structure (43200 bytes)
// - Offsets 157040+: Array of 12 large sub-structures (21636 bytes each)
// - Offsets 416672+: Final array of 900 elements (12 bytes each)
//
// Total structure size: ~700KB (massive singleton manager)
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::Constructor() {
    // Set vtable pointer
    // Note: vtable address would be set by compiler in real code
    // This is just showing the initialization pattern
    
    // Initialize first large array (128 elements, 128 bytes each)
    // Uses vector operations for bulk zero-initialization
    float* arrayPtr = (float*)((char*)this + 16);
    for (int i = 0; i < 128; i++) {
        // Each element is 128 bytes with specific float fields
        // Offsets 0, 8, 12, 16, 24, 28, 32 are floats initialized to 0.0f
        // Offsets 40-103 are vector-initialized (16-byte aligned blocks)
        // Offsets 104, 108, 112 are floats initialized to 0.0f
        
        float* elem = arrayPtr + (i * 32);  // 128 bytes = 32 floats
        for (int j = 0; j < 32; j++) {
            elem[j] = 0.0f;
        }
    }
    
    // Initialize field at offset 16400
    *(int*)((char*)this + 16400) = 0;
    
    // Initialize second large array (900 elements, 48 bytes each)
    float* array2Ptr = (float*)((char*)this + 16416);
    for (int i = 0; i < 900; i++) {
        float* elem = array2Ptr + (i * 12);  // 48 bytes = 12 floats
        // Initialize specific float fields at offsets 0, 4, 16, 20, 24, 32, 36, 40
        elem[0] = 0.0f;   // +0
        elem[1] = 0.0f;   // +4
        elem[4] = 0.0f;   // +16
        elem[5] = 0.0f;   // +20
        elem[6] = 0.0f;   // +24
        elem[8] = 0.0f;   // +32
        elem[9] = 0.0f;   // +36
        elem[10] = 0.0f;  // +40
    }
    
    // Initialize three control fields
    *(int*)((char*)this + 16416 + 43208) = 0;
    *(int*)((char*)this + 16416 + 43204) = 0;
    *(int*)((char*)this + 16416 + 43200) = 0;
    
    // Initialize additional float fields
    float* controlBlock = (float*)((char*)this + 16416 + 65536 - 22320);
    controlBlock[0] = 0.0f;
    controlBlock[1] = 0.0f;
    controlBlock[4] = 0.0f;
    controlBlock[5] = 0.0f;
    controlBlock[6] = 0.0f;
    controlBlock[8] = 0.0f;
    controlBlock[9] = 0.0f;
    controlBlock[10] = 0.0f;
    
    // Initialize first managed sub-structure at offset 70512
    InitializeSubStructure((char*)this + 70512);
    
    // Initialize second managed sub-structure at offset 113776
    InitializeSubStructure((char*)this + 113776);
    
    // Initialize array of 12 large sub-structures starting at offset 157040
    char* largeArrayBase = (char*)this + 157040;
    for (int i = 0; i < 12; i++) {
        InitializeLargeSubStructure(largeArrayBase + (i * 21636));
    }
    
    // Initialize final array (900 elements, 12 bytes each) at offset 416672
    float* finalArrayPtr = (float*)((char*)this + 416672);
    for (int i = 0; i < 900; i++) {
        float* elem = finalArrayPtr + (i * 3);  // 12 bytes = 3 floats
        elem[0] = 0.0f;  // +0
        elem[2] = 0.0f;  // +8
        *(int*)&elem[1] = 0;  // +4 is an int
    }
    
    // Additional initialization at end of structure
    char* endBlock = (char*)this + 416672 + 10800;
    *(int*)(endBlock + 0) = 0;
    *(int*)(endBlock + 4) = 0;
    *(int*)(endBlock + 8) = 0;
    *(float*)(endBlock + 12) = 0.0f;
    *(float*)(endBlock + 16) = 0.0f;
    *(float*)(endBlock + 20) = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::InitializeSubStructure (helper) @ 0x821CB978 | size: 0x98
//
// Initializes a 43200-byte sub-structure containing an array of 900 elements
// (48 bytes each). Each element has specific float and int fields.
//
// @param basePtr Pointer to the start of the sub-structure
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::InitializeSubStructure(void* basePtr) {
    char* base = (char*)basePtr;
    
    // Initialize array of 900 elements (48 bytes each)
    for (int i = 0; i < 900; i++) {
        char* elem = base + (i * 48);
        
        // Initialize fields within each element
        *(float*)(elem + 0) = 0.0f;
        *(int*)(elem + 4) = -1;  // Special value: -1
        *(uint16_t*)(elem + 8) = 0;
        *(uint16_t*)(elem + 10) = 0;
        *(float*)(elem + 16) = 0.0f;
        *(float*)(elem + 20) = 0.0f;
        *(float*)(elem + 24) = 0.0f;
        *(float*)(elem + 32) = 0.0f;
    }
    
    // Initialize control fields at end of array
    *(int*)(base + 43208) = 0;
    *(int*)(base + 43204) = 0;
    *(int*)(base + 43200) = 0;
    
    // Initialize additional control block
    char* controlBlock = base + 43216;
    *(float*)(controlBlock + 0) = 0.0f;
    *(int*)(controlBlock + 4) = -1;
    *(uint16_t*)(controlBlock + 8) = 0;
    *(uint16_t*)(controlBlock + 10) = 0;
    *(float*)(controlBlock + 16) = 0.0f;
    *(float*)(controlBlock + 20) = 0.0f;
    *(float*)(controlBlock + 24) = 0.0f;
    *(float*)(controlBlock + 32) = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::InitializeLargeSubStructure (helper) @ 0x821CBA10 | size: 0x60
//
// Initializes a 21636-byte sub-structure containing an array of 900 elements
// (24 bytes each). Each element has specific float and int fields.
//
// @param basePtr Pointer to the start of the sub-structure
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::InitializeLargeSubStructure(void* basePtr) {
    char* base = (char*)basePtr;
    
    // Initialize array of 900 elements (24 bytes each)
    for (int i = 0; i < 900; i++) {
        char* elem = base + (i * 24);
        
        // Initialize fields within each element
        *(float*)(elem + 0) = 0.0f;
        *(int*)(elem + 8) = 0;
        *(float*)(elem + 12) = 0.0f;
        *(int*)(elem + 20) = 0;
    }
    
    // Initialize control fields at end of array
    *(int*)(base + 21600) = 0;
    *(int*)(base + 21604) = 0;
    *(int*)(base + 21608) = 0;
    
    // Initialize final control block
    *(float*)(base + 21612) = 0.0f;
    *(int*)(base + 21616) = 0;
    *(float*)(base + 21620) = 0.0f;
    *(int*)(base + 21624) = 0;
    *(float*)(base + 21628) = 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ReadBits48 @ 0x824D2748 | size: 0xCC
//
// Bitstream reader - reads 48 bits (6 bytes) from buffer and accumulates them
// into the 64-bit accumulator. This is an optimized bulk read function that
// reads 6 bytes at once, compared to ReadBits8 which reads 1 byte at a time.
//
// Bitstream structure (inferred from usage):
//   +0x00: uint64_t accumulator (bit buffer)
//   +0x08: int32_t bitPosition (negative = bits available in accumulator)
//   +0x0C: uint8_t* readPtr (current read position in buffer)
//   +0x10: uint8_t* endPtr (end of buffer)
//   +0x14: int32_t field_0x14 (unknown)
//   +0x18: int32_t errorFlag (set to 2 on underflow)
//
// The bitPosition field is negative when there are bits available in the
// accumulator. For example, -32 means 32 bits are available to read.
// When bitPosition becomes positive, the accumulator needs refilling.
//
// This function reads 6 bytes in big-endian order, shifts them left by
// (48 - bitPosition) bits, and adds them to the accumulator. The new
// bitPosition is old + 48.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::ReadBits48() {
    // Loop until we have enough bits or hit end of buffer
    while (true) {
        uint8_t* readPtr = *(uint8_t**)((char*)this + 0x0C);
        uint8_t* endPtr = *(uint8_t**)((char*)this + 0x10);
        
        // Check if we have at least 6 bytes available
        // (endPtr - 4) because we need 6 bytes, and the check is (readPtr < endPtr - 4)
        if (readPtr >= endPtr - 4) {
            // Not enough bytes - call ReadBits8 to handle refill/error
            int result = ReadBits8();
            if (result != 1) {
                return;
            }
            // ReadBits8 returned 1, meaning we should retry
            continue;
        }
        
        // Read 6 bytes in big-endian order
        uint64_t byte0 = readPtr[0];
        uint64_t byte1 = readPtr[1];
        uint64_t byte2 = readPtr[2];
        uint64_t byte3 = readPtr[3];
        uint64_t byte4 = readPtr[4];
        uint64_t byte5 = readPtr[5];
        
        // Construct 48-bit value from 6 bytes (big-endian)
        uint64_t value48 = (byte0 << 40) | (byte1 << 32) | (byte2 << 24) | 
                           (byte3 << 16) | (byte4 << 8) | byte5;
        
        // Update read pointer (advance by 6 bytes)
        *(uint8_t**)((char*)this + 0x0C) = readPtr + 6;
        
        // Get current bit position (negative = bits available)
        int32_t bitPosition = *(int32_t*)((char*)this + 0x08);
        
        // Calculate shift amount: we want to shift left by (48 - bitPosition)
        // Since bitPosition is negative, this becomes (48 - (-N)) = (48 + N)
        // But we need to negate it first to get the positive shift amount
        int32_t shiftAmount = 48 - bitPosition;
        
        // Shift the 48-bit value and add to accumulator
        uint64_t accumulator = *(uint64_t*)((char*)this + 0x00);
        accumulator += (value48 << shiftAmount);
        *(uint64_t*)((char*)this + 0x00) = accumulator;
        
        // Update bit position (add 48 bits)
        *(int32_t*)((char*)this + 0x08) = bitPosition + 48;
        
        return;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ReadBits8 @ 0x824D2688 | size: 0xC0
//
// Bitstream reader - reads 8 bits (1 byte) from buffer and accumulates it
// into the 64-bit accumulator. This is the single-byte read function used
// when ReadBits48 runs out of buffer space or for small reads.
//
// Returns:
//   0 = success
//   1 = retry (buffer was refilled, call again)
//
// The function handles buffer underflow by checking field_0x18 and setting
// error flags when necessary.
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::ReadBits8() {
    uint8_t* readPtr = *(uint8_t**)((char*)this + 0x0C);
    uint8_t* endPtr = *(uint8_t**)((char*)this + 0x10);
    
    // Check if we have at least 1 byte available
    if (readPtr <= endPtr) {
        // Read loop: accumulate bytes until we reach end of buffer
        while (readPtr <= endPtr) {
            // Read one byte
            uint8_t byteValue = *readPtr;
            readPtr++;
            
            // Get current bit position
            int32_t bitPosition = *(int32_t*)((char*)this + 0x08);
            
            // Calculate shift amount: (40 - bitPosition)
            int32_t shiftAmount = 40 - bitPosition;
            
            // Shift the byte and add to accumulator
            uint64_t accumulator = *(uint64_t*)((char*)this + 0x00);
            accumulator += ((uint64_t)byteValue << shiftAmount);
            *(uint64_t*)((char*)this + 0x00) = accumulator;
            
            // Update bit position (add 8 bits)
            *(int32_t*)((char*)this + 0x08) = bitPosition + 8;
            
            // Update read pointer
            *(uint8_t**)((char*)this + 0x0C) = readPtr;
            
            // Reload end pointer for next iteration
            endPtr = *(uint8_t**)((char*)this + 0x10);
        }
        
        // Check if bit position is non-negative (meaning we have enough bits)
        int32_t bitPosition = *(int32_t*)((char*)this + 0x08);
        if (bitPosition >= 0) {
            return 0;  // Success
        }
    }
    
    // Buffer underflow handling
    int32_t field_0x18 = *(int32_t*)((char*)this + 0x18);
    
    if (field_0x18 == 1) {
        // Call helper function to refill buffer
        extern void atSingleton_23C0(void*);
        atSingleton_23C0(this);
        return 1;  // Retry
    }
    
    // Check if bit position is very negative (< -16)
    int32_t bitPosition = *(int32_t*)((char*)this + 0x08);
    if (bitPosition < -16) {
        // Check field_0x14
        int32_t field_0x14 = *(int32_t*)((char*)this + 0x14);
        if (field_0x14 == 0) {
            // Set error flag
            *(int32_t*)((char*)this + 0x14) = 2;
        }
        
        // Reset bit position to maximum (127 = all bits consumed)
        *(int32_t*)((char*)this + 0x08) = 127;
    }
    
    return 0;  // Success (or error handled)
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::InsertAtPosition @ 0x825537D8 | size: 0xEC
//
// Inserts a data pointer into a linked list at the specified position.
// Uses a free list to allocate nodes efficiently.
//
// @param data Pointer to data to insert (must not be NULL)
// @param position Position to insert at:
//                 -1 = append to end
//                  0 = prepend to beginning
//                 >0 = insert after N nodes
// @return 1 on success, 0 on failure
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::InsertAtPosition(void* data, int position) {
    // Validate parameters
    if (this == nullptr || data == nullptr) {
        return 0;
    }
    
    // Check if position is within bounds (position <= count)
    int count = *(int*)((char*)this + 16);
    if (position > count) {
        return 0;
    }
    
    // Pop a node from the free list (offset +8)
    void** freeListHead = (void**)((char*)this + 8);
    void* node = *freeListHead;
    
    // Get next free node and update free list head
    void* nextFree = *(void**)node;
    *freeListHead = nextFree;
    
    // If free list is now empty, clear the tail pointer
    if (nextFree == nullptr) {
        *(void**)((char*)this + 12) = nullptr;
    }
    
    // Store data pointer in node (offset +4)
    *((void**)((char*)node + 4)) = data;
    
    // Insert node based on position
    if (position == 0) {
        // Prepend: insert at head
        void** headPtr = (void**)this;
        void* oldHead = *headPtr;
        
        // Link new node to old head
        *(void**)node = oldHead;
        
        // Update head to new node
        *headPtr = node;
        
        // If list was empty, update tail
        void** tailPtr = (void**)((char*)this + 4);
        if (oldHead == nullptr) {
            *tailPtr = node;
        }
    } else if (position == -1) {
        // Append: insert at tail
        void** tailPtr = (void**)((char*)this + 4);
        void* oldTail = *tailPtr;
        
        // New node has no next
        *(void**)node = nullptr;
        
        // Update tail to new node
        *tailPtr = node;
        
        // If list was empty, update head
        void** headPtr = (void**)this;
        if (oldTail == nullptr) {
            *headPtr = node;
        } else {
            // Link old tail to new node
            *(void**)oldTail = node;
        }
    } else {
        // Insert at position N: walk to node N-1 and insert after it
        void* current = *(void**)this;
        
        // Walk to position-1
        for (int i = 1; i < position; i++) {
            current = *(void**)current;
        }
        
        // Insert after current
        void* next = *(void**)current;
        *(void**)node = next;
        *(void**)current = node;
        
        // If we inserted at the end, update tail
        if (next == nullptr) {
            *(void**)((char*)this + 4) = node;
        }
    }
    
    // Increment count
    count++;
    *(int*)((char*)this + 16) = count;
    
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::GetBufferSize @ 0x8245DBC8 | size: 0x28
// [vtable slot 40]
//
// Calculates the total size of a 2D buffer by multiplying its dimensions.
// Returns 0 if the buffer is not allocated or count is zero.
//
// The buffer descriptor is stored at offset +52 (0x34) and contains:
//   +0: uint16_t width
//   +2: uint16_t height
//
// The count/validity flag is at offset +56 (0x38).
//
// @return Total buffer size (width * height), or 0 if not allocated
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::GetBufferSize() {
    // Check if buffer is allocated (count at offset +56)
    uint16_t count = *(uint16_t*)((char*)this + 56);
    
    if (count == 0) {
        // Buffer not allocated or empty
        return 0;
    }
    
    // Load buffer descriptor pointer from offset +52
    void* bufferDesc = *(void**)((char*)this + 52);
    
    // Read dimensions from descriptor
    uint16_t height = *(uint16_t*)((char*)bufferDesc + 2);
    uint16_t width = *(uint16_t*)((char*)bufferDesc + 0);
    
    // Return total size (width * height)
    return width * height;
}
