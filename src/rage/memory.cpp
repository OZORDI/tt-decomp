/**
 * memory.cpp — RAGE memory management system implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * atSingleton class methods and memory allocation utilities.
 */

#include "memory.hpp"

// External function declarations
extern void CCalChannel_60A8_p46(void* obj, uint32_t param);
extern void grc_DCA8(void* obj);
extern void MmFreePhysicalMemory(uint32_t flags, void* address);

// Global data
uint32_t g_grcDevicePtr = 0;  // @ 0x8260637C (SDA +25468)

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_DC30_gen()  @ 0x824ADC30 | size: 0xC
// 
// Allocates memory with specific flags via rage allocator.
// This is a thin wrapper around rage_01B8 with hardcoded flags.
// ─────────────────────────────────────────────────────────────────────────────
void* atSingleton_DC30_gen(uint32_t size) {
    // Flags: 0x248C8001 = allocation flags for singleton objects
    // Likely includes: aligned allocation, zero-init, tracked allocation
    return rage_01B8(size, 0x248C8001);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_DC40_gen()  @ 0x824ADC40 | size: 0xC
// 
// Identical to atSingleton_DC30_gen - allocates with same flags.
// Separate symbol likely for different call sites or inlining control.
// ─────────────────────────────────────────────────────────────────────────────
void* atSingleton_DC40_gen(uint32_t size) {
    return rage_01B8(size, 0x248C8001);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_3BD8_2h()  @ 0x824B3BD8 | size: 0xC
// 
// Clears the vtable pointer (sets to null).
// Called during object destruction to invalidate the object.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_3BD8_2h(atSingleton* obj) {
    obj->vtable = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6508_2h()  @ 0x824B6508 | size: 0xC
// 
// Stores a byte value at offset (base + index + 11).
// Used for setting flags or state bytes in indexed arrays.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6508_2h(uint8_t* base, uint32_t index, uint8_t value) {
    base[index + 11] = value;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6B58_p39()  @ 0x824B6B58 | size: 0xC
// 
// Frees physical memory via Xbox 360 kernel call.
// Wrapper around MmFreePhysicalMemory with flags=0.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6B58_p39(void* address) {
    MmFreePhysicalMemory(0, address);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6438()  @ 0x824B6438 | size: 0xC
// 
// Forwards a call to CCalChannel_60A8_p46 with data from offset +608.
// Likely part of audio or animation channel management.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6438(atSingleton* obj, uint32_t param) {
    void* channelData = *(void**)((uint8_t*)obj + 608);
    CCalChannel_60A8_p46(channelData, param);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6EF0_p46()  @ 0x824B6EF0 | size: 0xC
// 
// Calls grc_DCA8 with the global graphics device pointer.
// Part of the graphics rendering system initialization/cleanup.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6EF0_p46() {
    void* grcDevice = (void*)g_grcDevicePtr;
    grc_DCA8(grcDevice);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_22D0_g()  @ 0x824B22D0 | size: 0x10
// 
// Sets the vtable pointer to a specific vtable address.
// Used during object construction or vtable switching.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_22D0_g(atSingleton* obj) {
    // Vtable address: 0x82003DC8
    obj->vtable = (void*)0x82003DC8;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_4148_g()  @ 0x824B4148 | size: 0x10
// 
// Returns the sum of two float fields at offsets +120 and +92.
// Likely calculates a total time, distance, or accumulated value.
// ─────────────────────────────────────────────────────────────────────────────
float atSingleton_4148_g(atSingleton* obj) {
    float value1 = *(float*)((uint8_t*)obj + 120);
    float value2 = *(float*)((uint8_t*)obj + 92);
    return value1 + value2;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_74B8()  @ 0x820E74B8 | size: 0x68
// 
// Destroys an array of singleton objects and frees the array memory.
// 
// Algorithm:
//   1. Check if refCount (at +6) is non-zero
//   2. Check if vtable pointer is non-zero
//   3. If both checks pass:
//      - Get array count from vtable[-1]
//      - Calculate array size: count * 8 bytes per element
//      - Loop backwards through array, calling destructor on each element
//      - Free the array memory via rage_free_00C0
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_74B8(atSingleton* obj) {
    // Check if object has references
    if (obj->refCount == 0) {
        return;
    }
    
    // Check if vtable is valid
    if (obj->vtable == nullptr) {
        return;
    }
    
    // Get array metadata from vtable[-1]
    uint32_t* vtablePtr = (uint32_t*)obj->vtable;
    uint32_t* arrayMetadata = vtablePtr - 1;
    uint32_t arrayCount = *arrayMetadata;
    
    // Calculate array size and end pointer
    uint32_t arraySize = arrayCount * 8;
    uint8_t* arrayEnd = (uint8_t*)obj->vtable + arraySize;
    
    // Destroy each element in reverse order
    for (int32_t i = arrayCount - 1; i >= 0; i--) {
        uint8_t* element = arrayEnd - 8;
        atSingleton_dtor_7628(element);
        arrayEnd = element;
    }
    
    // Free the array memory
    rage_free_00C0(arrayMetadata);
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_A828_2hr()  @ 0x8245A828 | size: 0x10
// 
// Sets vtable pointer to 0x820054B8.
// Constructor or vtable initialization function.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_A828_2hr(atSingleton* obj) {
    obj->vtable = (void*)0x820054B8;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_DB50_2h()  @ 0x8245DB50 | size: 0x10
// 
// Sets vtable pointer to 0x82005624, then calls atSingleton_FA08_g.
// Chained initialization pattern.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_DB50_2h(atSingleton* obj) {
    obj->vtable = (void*)0x82005624;
    atSingleton_FA08_g(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_FA08_g()  @ 0x8245FA08 | size: 0x10
// 
// Sets vtable pointer to 0x820057A4, then calls atSingleton_22D0_g.
// Multi-stage initialization chain.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_FA08_g(atSingleton* obj) {
    obj->vtable = (void*)0x820057A4;
    atSingleton_22D0_g(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6F98_g()  @ 0x824B6F98 | size: 0x10
// 
// Returns a value from a global structure.
// Loads pointer from global at 0x8200084C, then returns field at offset +16.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t atSingleton_6F98_g() {
    uint32_t* globalPtr = *(uint32_t**)0x8200084C;
    return globalPtr[4];  // offset +16 = index 4
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6108()  @ 0x824B6108 | size: 0x14
// 
// Indirect function call through function pointer at offset +4.
// Loads first parameter from offset +0, then calls function pointer at +4.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6108(void* obj) {
    typedef void (*FuncPtr)(void*);
    void* param = *(void**)obj;
    FuncPtr func = *(FuncPtr*)((uint8_t*)obj + 4);
    func(param);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_9980_2h()  @ 0x824F9980 | size: 0x10
// 
// Returns absolute value of signed integer.
// If value is negative, returns its negation; otherwise returns early.
// ─────────────────────────────────────────────────────────────────────────────
int32_t atSingleton_9980_2h(int32_t value) {
    if (value >= 0) {
        return value;
    }
    return -value;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_6F38_p33()  @ 0x821C6F38 | size: 0x14
// 
// Stores 0 at offset 75756 (0x127EC), then calls SinglesNetworkClient_4FB0_g.
// Network client initialization helper.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_6F38_p33(uint8_t* obj) {
    *(uint32_t*)(obj + 75756) = 0;
    extern void SinglesNetworkClient_4FB0_g(uint8_t* obj);
    SinglesNetworkClient_4FB0_g(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_DB58_sp()  @ 0x8257DB58 | size: 0x10
// 
// Stores current timebase value to global at 0x82619BB0.
// Performance counter / timing utility.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_DB58_sp() {
    extern uint64_t PPC_QUERY_TIMEBASE();
    uint64_t timebase = PPC_QUERY_TIMEBASE();
    *(uint64_t*)0x82619BB0 = timebase;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_AA30_2hr()  @ 0x824FAA30 | size: 0x10
// 
// Calls nt_8948 with parameters (obj, 0, 32768).
// Network or threading utility with specific flags.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_AA30_2hr(void* obj) {
    extern void nt_8948(void* obj, uint32_t param1, uint32_t param2);
    nt_8948(obj, 0, 32768);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_7D58_2h()  @ 0x824F7D58 | size: 0x14
// 
// Modulo operation: returns (value1 % value2).
// Repeatedly subtracts value2 from value1 until result is negative,
// then adds value2 back once to get the remainder.
// ─────────────────────────────────────────────────────────────────────────────
int32_t atSingleton_7D58_2h(int32_t value1, int32_t value2) {
    int32_t result = value1;
    while (result >= 0) {
        result -= value2;
    }
    return result + value2;
}


// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_8620()  @ 0x820C8620 | size: 0x78
// 
// Destroys an array of singleton objects with 80-byte elements.
// 
// Algorithm:
//   1. Calculate array base address (this - 16)
//   2. Get element count from first field
//   3. Calculate total array size: count * 80 bytes
//   4. Loop through array backwards, calling destructor (vtable slot 0) on each
//   5. Free the array memory
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_8620(atSingleton* obj) {
    // Get array base (16 bytes before this pointer)
    uint8_t* arrayBase = (uint8_t*)obj - 16;
    
    // Get element count from first field
    uint32_t elementCount = *(uint32_t*)arrayBase;
    
    // Check if array is valid
    if (elementCount == 0) {
        rage_free_00C0(arrayBase);
        return;
    }
    
    // Calculate array size: count * 5 * 16 = count * 80
    uint32_t arraySize = elementCount * 80;
    
    // Start at end of array (base + 64 + arraySize)
    uint8_t* currentElement = (uint8_t*)obj + 64 + arraySize;
    
    // Destroy each element in reverse order
    for (int32_t i = elementCount - 1; i >= 0; i--) {
        currentElement -= 80;  // Move to previous element
        
        void* elementPtr = *(void**)currentElement;
        if (elementPtr != nullptr) {
            // Call destructor (vtable slot 0) with deleteFlag=1
            void** vtable = *(void***)elementPtr;
            typedef void (*DestructorFunc)(void*, int);
            DestructorFunc destructor = (DestructorFunc)vtable[0];
            destructor(elementPtr, 1);
        }
    }
    
    // Free the array memory
    rage_free_00C0(arrayBase);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_33A8_h()  @ 0x820C33A8 | size: 0x140
// 
// Complex destructor with reference counting and resource cleanup.
// 
// This destructor:
//   1. Sets vtable to 0x82027824
//   2. Cleans up field at +4 if non-zero
//   3. Decrements reference counts for objects in array at +20
//   4. Frees array at +20 if reference count reaches zero
//   5. Frees array at +12 if reference count reaches zero
//   6. Restores vtable to 0x820276C4
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_33A8_h(atSingleton* obj) {
    // Set initial vtable
    obj->vtable = (void*)0x82027824;
    
    // Clean up field at +4 if present
    uint32_t* field4 = (uint32_t*)((uint8_t*)obj + 4);
    if (*field4 != 0) {
        extern void util_36A8(uint32_t value);
        util_36A8(*field4);
    }
    
    // Clear field at +4
    *field4 = 0;
    
    // Process array at offset +20 (reference-counted objects)
    uint16_t arrayCount = *(uint16_t*)((uint8_t*)obj + 24);
    if (arrayCount > 0) {
        void** objectArray = *(void***)((uint8_t*)obj + 20);
        
        for (uint16_t i = 0; i < arrayCount; i++) {
            void* element = objectArray[i];
            if (element == nullptr) {
                continue;
            }
            
            // Decrement reference count at offset +6
            uint16_t* refCount = (uint16_t*)((uint8_t*)element + 6);
            (*refCount)--;
            
            // If reference count reaches zero, destroy the object
            if (*refCount == 0) {
                void** vtable = *(void***)element;
                typedef void (*DestructorFunc)(void*, int);
                DestructorFunc destructor = (DestructorFunc)vtable[0];
                destructor(element, 1);
            }
        }
    }
    
    // Free array at +20 if it exists and isn't tracked
    uint16_t capacity = *(uint16_t*)((uint8_t*)obj + 26);
    if (capacity != 0) {
        void** objectArray = *(void***)((uint8_t*)obj + 20);
        if (objectArray != nullptr) {
            // Check if array is tracked by singleton system
            extern uint8_t atSingleton_Find_90D0(void* ptr);
            if (atSingleton_Find_90D0(objectArray) == 0) {
                // Not tracked - free it via allocator
                extern void* g_allocator_ptr;  // SDA global at r13+0
                void** allocator = *(void***)((uint8_t*)&g_allocator_ptr + 4);
                typedef void (*FreeFunc)(void*, void*);
                FreeFunc freeFunc = (FreeFunc)((void**)allocator)[2];  // vtable slot 2
                freeFunc(allocator, objectArray);
            }
        }
    }
    
    // Free array at +12 if it exists and isn't tracked
    uint16_t count2 = *(uint16_t*)((uint8_t*)obj + 18);
    if (count2 != 0) {
        void** array2 = *(void***)((uint8_t*)obj + 12);
        if (array2 != nullptr) {
            extern uint8_t atSingleton_Find_90D0(void* ptr);
            if (atSingleton_Find_90D0(array2) == 0) {
                extern void* g_allocator_ptr;
                void** allocator = *(void***)((uint8_t*)&g_allocator_ptr + 4);
                typedef void (*FreeFunc)(void*, void*);
                FreeFunc freeFunc = (FreeFunc)((void**)allocator)[2];
                freeFunc(allocator, array2);
            }
        }
    }
    
    // Set final vtable
    obj->vtable = (void*)0x820276C4;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_F950_fw()  @ 0x820CF950 | size: 0xEC
// 
// Hash table lookup by string key.
// 
// Searches a hash table for an entry matching the given key string.
// Returns pointer to the value (at offset +4 in the entry) if found, nullptr otherwise.
// 
// Hash table structure:
//   +0: array of bucket pointers (size determined by field at +4)
//   +4: bucket count
//   
// Bucket entry structure:
//   +0: key string pointer
//   +4: value (returned if key matches)
//   +8: next entry pointer (for collision chaining)
// ─────────────────────────────────────────────────────────────────────────────
void* atSingleton_F950_fw(atSingleton* hashTable, void* keyObj) {
    uint16_t bucketCount = *(uint16_t*)((uint8_t*)hashTable + 4);
    if (bucketCount == 0) {
        return nullptr;
    }
    
    // Calculate hash of key string
    extern uint32_t atSingleton_5338_g(void* keyObj);
    uint32_t hash = atSingleton_5338_g(keyObj);
    
    // Find bucket index: hash % bucketCount
    uint32_t bucketIndex = hash % bucketCount;
    
    // Get bucket array and entry
    void*** bucketArray = *(void****)hashTable;
    void** entry = (void**)bucketArray[bucketIndex];
    
    if (entry == nullptr) {
        return nullptr;
    }
    
    // Get key string from keyObj
    const char* searchKey = *(const char**)keyObj;
    
    // Walk collision chain
    while (entry != nullptr) {
        const char* entryKey = (const char*)entry[0];
        
        // Compare keys
        if (searchKey == entryKey) {
            // Exact pointer match
            return (void*)((uint8_t*)entry + 4);
        }
        
        // String comparison
        bool keysMatch = false;
        if (searchKey != nullptr && entryKey != nullptr) {
            // Compare strings byte by byte
            const char* s1 = searchKey;
            const char* s2 = entryKey;
            while (*s1 != '\0' && *s1 == *s2) {
                s1++;
                s2++;
            }
            keysMatch = (*s1 == *s2);
        } else {
            keysMatch = (searchKey == nullptr && entryKey == nullptr);
        }
        
        if (keysMatch) {
            return (void*)((uint8_t*)entry + 4);
        }
        
        // Move to next entry in chain
        entry = (void**)entry[2];
    }
    
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton_B960_h()  @ 0x820CB960 | size: 0xE0
// 
// Constructor for object with four embedded sub-objects.
// 
// Initializes an object with:
//   - Main vtable at +0: 0x82033E54
//   - Secondary vtable at +4: 0x82033E60
//   - Four embedded objects at offsets +32, +48, +64, +80
//   - Each embedded object initialized with vtable 0x8204FCBC
// 
// After initialization, calls cleanup function on each embedded object,
// then sets final vtables.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_B960_h(atSingleton* obj) {
    // Set initial vtables
    obj->vtable = (void*)0x82033E54;
    *(void**)((uint8_t*)obj + 4) = (void*)0x82033E60;
    
    // Initialize embedded object at +80
    void* embeddedObj80 = (void*)((uint8_t*)obj + 80);
    *(void**)embeddedObj80 = (void*)0x8204FCBC;
    
    extern void atSingleton_AC28_g(void* obj);
    atSingleton_AC28_g(embeddedObj80);
    
    // Check if embedded object has allocated memory and free it
    uint16_t count80 = *(uint16_t*)((uint8_t*)embeddedObj80 + 14);
    if (count80 != 0) {
        void* memory80 = *(void**)((uint8_t*)embeddedObj80 + 8);
        rage_free_00C0(memory80);
    }
    
    // Initialize embedded object at +64
    void* embeddedObj64 = (void*)((uint8_t*)obj + 64);
    *(void**)embeddedObj64 = (void*)0x8204FCBC;
    atSingleton_AC28_g(embeddedObj64);
    
    uint16_t count64 = *(uint16_t*)((uint8_t*)embeddedObj64 + 14);
    if (count64 != 0) {
        void* memory64 = *(void**)((uint8_t*)embeddedObj64 + 8);
        rage_free_00C0(memory64);
    }
    
    // Initialize embedded object at +48
    void* embeddedObj48 = (void*)((uint8_t*)obj + 48);
    *(void**)embeddedObj48 = (void*)0x8204FCBC;
    atSingleton_AC28_g(embeddedObj48);
    
    uint16_t count48 = *(uint16_t*)((uint8_t*)embeddedObj48 + 14);
    if (count48 != 0) {
        void* memory48 = *(void**)((uint8_t*)embeddedObj48 + 8);
        rage_free_00C0(memory48);
    }
    
    // Initialize embedded object at +32
    void* embeddedObj32 = (void*)((uint8_t*)obj + 32);
    *(void**)embeddedObj32 = (void*)0x8204FCBC;
    atSingleton_AC28_g(embeddedObj32);
    
    uint16_t count32 = *(uint16_t*)((uint8_t*)embeddedObj32 + 14);
    if (count32 != 0) {
        void* memory32 = *(void**)((uint8_t*)embeddedObj32 + 8);
        rage_free_00C0(memory32);
    }
    
    // Set final vtables
    *(void**)((uint8_t*)obj + 4) = (void*)0x82027B34;
    obj->vtable = (void*)0x820276C4;
}

// ═════════════════════════════════════════════════════════════════════════════
// atSingleton Vtable Methods
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_15()  [vtable slot 15 @ 0x82229638]
// 
// Returns a pointer to a constant string in .rdata.
// This appears to be a debug or error message getter.
// ─────────────────────────────────────────────────────────────────────────────
const char* atSingleton::vfn_15() {
    // String at 0x8204E918: "t number of points specified"
    // Likely part of a longer error message like "Incorrect number of points specified"
    return (const char*)0x8204E918;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_16()  [vtable slot 16 @ 0x822BDC38]
// 
// Checks game loop state and conditionally calls a page-related function.
// Returns early if certain flags are set, otherwise calls pg_FDF0_fw.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::vfn_16() {
    // Check flag at offset +495
    uint8_t localFlag = *((uint8_t*)this + 495);
    
    if (localFlag != 0) {
        return;
    }
    
    // Load global loop object pointer
    extern uint8_t* g_loop_obj_ptr;  // @ 0x825EAB30
    uint8_t* loopObj = *(uint8_t**)((uintptr_t)&g_loop_obj_ptr - 23740);
    
    // Check if loop is active (flag at +24)
    uint8_t loopActive = loopObj[24];
    
    if (loopActive != 0) {
        return;
    }
    
    // Load page object pointer from global
    void* pageObj = *(void**)((uintptr_t)&g_loop_obj_ptr - 23832);
    
    // Call page function
    extern void pg_FDF0_fw(void*);
    pg_FDF0_fw(pageObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_17()  [vtable slot 17 @ 0x8244A578]
// 
// Manages a flag state and conditionally triggers virtual method calls on a
// child object. Handles bit manipulation for state flags and coordinates
// between parent and child object states.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::vfn_17(int enableFlag) {
    // Load current flag byte at offset +24
    uint8_t currentFlags = *((uint8_t*)this + 24);
    
    // Check if bit 7 is set (0x80 mask)
    bool isEnabled = (currentFlags & 0x80) != 0;
    
    // If requested state matches current state, return early
    if ((enableFlag != 0) == isEnabled) {
        return;
    }
    
    // Update the flag: set or clear bit 7 based on enableFlag
    if (enableFlag != 0) {
        currentFlags |= 0x80;  // Set bit 7
    } else {
        currentFlags &= 0x7F;  // Clear bit 7
    }
    
    *((uint8_t*)this + 24) = currentFlags;
    
    // Get child object at offset +36
    void* childObj = *(void**)((char*)this + 36);
    
    if (childObj == nullptr) {
        return;
    }
    
    // Handle enable case
    if (enableFlag != 0) {
        // Call child's vtable slot 5 with parameter 1
        typedef void (*VTableFunc5)(void*, int);
        void** vtable = *(void***)childObj;
        VTableFunc5 func5 = (VTableFunc5)vtable[5];
        func5(childObj, 1);
        return;
    }
    
    // Handle disable case
    // Check if bit 1 is set in the updated flags
    if ((currentFlags & 0x02) == 0) {
        return;
    }
    
    // Call child's vtable slot 4 (returns status code)
    typedef int (*VTableFunc4)(void*);
    void** vtable = *(void***)childObj;
    VTableFunc4 func4 = (VTableFunc4)vtable[4];
    int status = func4(childObj);
    
    if (status < 0) {
        return;
    }
    
    // Reload flags (may have changed during virtual call)
    currentFlags = *((uint8_t*)this + 24);
    
    // Check if bit 6 is set (0x40 mask)
    if ((currentFlags & 0x40) == 0) {
        return;
    }
    
    // Call child's vtable slot 6
    typedef void (*VTableFunc6)(void*);
    vtable = *(void***)childObj;
    VTableFunc6 func6 = (VTableFunc6)vtable[6];
    func6(childObj);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_18()  [vtable slot 18 @ 0x822F9D88]
// 
// Checks global loop state and conditionally clears a local flag or triggers
// a virtual method call on a child object.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::vfn_18() {
    // Load global loop object pointer and check if paused
    extern uint8_t* g_loop_obj_ptr;  // @ 0x825EAB30
    uint8_t* loopObj = *(uint8_t**)((uintptr_t)&g_loop_obj_ptr - 21712);
    bool isPaused = (loopObj[493] == 0);
    
    if (!isPaused) {
        // Not paused - just clear the flag at +35
        *((uint8_t*)this + 35) = 0;
        return;
    }
    
    // Paused - check local flag at +35
    uint8_t localFlag = *((uint8_t*)this + 35);
    
    if (localFlag == 0) {
        // Flag already clear - get child object and call its vtable slot 6
        void* childObj = *(void**)((char*)this + 24);
        
        typedef void (*VTableFunc6)(void*);
        void** vtable = *(void***)childObj;
        VTableFunc6 func6 = (VTableFunc6)vtable[6];
        func6(childObj);
        
        // Clear the flag
        *((uint8_t*)this + 35) = 0;
        return;
    }
    
    // Flag was set - just clear it
    *((uint8_t*)this + 35) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_19()  [vtable slot 19 @ 0x822C1B10]
// 
// Tail-call dispatcher - loads vtable slot 20 and jumps to it.
// This is a common pattern for forwarding calls through the vtable hierarchy.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::vfn_19() {
    // Load vtable and call slot 20
    typedef void (*VTableFunc20)(void*);
    void** vtable = *(void***)this;
    VTableFunc20 func20 = (VTableFunc20)vtable[20];
    func20(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_20()  [vtable slot 20 @ 0x822FB3B8]
// 
// Indirect dispatcher - loads an object from offset +20, adjusts pointer by +4,
// then calls that object's vtable slot 0 with parameter 0.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::vfn_20() {
    // Load object pointer from offset +20
    void* targetObj = *(void**)((char*)this + 20);
    
    // Adjust pointer by +4 (skip to actual object data)
    void* adjustedObj = (void*)((char*)targetObj + 4);
    
    // Call vtable slot 0 with parameter 0
    typedef void (*VTableFunc0)(void*, int);
    void** vtable = *(void***)adjustedObj;
    VTableFunc0 func0 = (VTableFunc0)vtable[0];
    func0(adjustedObj, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::vfn_21()  [vtable slot 21 @ 0x8245C108]
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
// @return Always returns 0
// ─────────────────────────────────────────────────────────────────────────────
int atSingleton::vfn_21(int transitionType) {
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
    
    // Check if we're in state 3 or (state 4 with transitionType 0)
    bool inState3 = (currentState == 3);
    bool inState4WithType0 = (currentState == 4 && transitionType == 0);
    
    if (!inState3 && !inState4WithType0) {
        // Default: transition to state 5 (Complete)
        extern void atSingleton_AA58_g(void*, int);
        atSingleton_AA58_g(this, 5);
        return result;
    }
    
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
        void* childObj = *(void**)((char*)this + 36);
        typedef void (*VTableFunc5)(void*, int);
        void** vtable = *(void***)childObj;
        VTableFunc5 func5 = (VTableFunc5)vtable[5];
        func5(childObj, 1);
        
        return result;
    }
    
    if (transitionType == 1) {
        // Type 1: Transition to state 4 without initialization
        extern void atSingleton_AA58_g(void*, int);
        atSingleton_AA58_g(this, 4);
        
        // Get object at offset +36 and call its vtable slot 12
        void* childObj = *(void**)((char*)this + 36);
        typedef void (*VTableFunc12)(void*);
        void** vtable = *(void***)childObj;
        VTableFunc12 func12 = (VTableFunc12)vtable[12];
        func12(childObj);
        
        // Clear flag at offset +56
        *((uint8_t*)this + 56) = 0;
        
        return result;
    }
    
    // Fallthrough: transition to state 5
    extern void atSingleton_AA58_g(void*, int);
    atSingleton_AA58_g(this, 5);
    
    return result;
}
