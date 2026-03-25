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
// rage_Alloc()  @ 0x824ADC30 | size: 0xC
// 
// Allocates memory with specific flags via rage allocator.
// This is a thin wrapper around rage_01B8 with hardcoded flags.
// ─────────────────────────────────────────────────────────────────────────────
void* rage_Alloc(uint32_t size) {
    // Flags: 0x248C8001 = allocation flags for singleton objects
    // Likely includes: aligned allocation, zero-init, tracked allocation
    return rage_01B8(size, 0x248C8001);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_AllocRegistered()  @ 0x824ADC40 | size: 0xC
// 
// Identical to rage_Alloc - allocates with same flags.
// Separate symbol likely for different call sites or inlining control.
// ─────────────────────────────────────────────────────────────────────────────
void* rage_AllocRegistered(uint32_t size) {
    return rage_01B8(size, 0x248C8001);
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_ClearVtable()  @ 0x824B3BD8 | size: 0xC
// 
// Clears the vtable pointer (sets to null).
// Called during object destruction to invalidate the object.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_ClearVtable(atSingleton* obj) {
    obj->vtable = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_SetByteAtIndex()  @ 0x824B6508 | size: 0xC
// 
// Stores a byte value at offset (base + index + 11).
// Used for setting flags or state bytes in indexed arrays.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_SetByteAtIndex(uint8_t* base, uint32_t index, uint8_t value) {
    base[index + 11] = value;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_FreePhysicalMemory()  @ 0x824B6B58 | size: 0xC
// 
// Frees physical memory via Xbox 360 kernel call.
// Wrapper around MmFreePhysicalMemory with flags=0.
// ─────────────────────────────────────────────────────────────────────────────
void rage_FreePhysicalMemory(void* address) {
    MmFreePhysicalMemory(0, address);
}

// ─────────────────────────────────────────────────────────────────────────────
// CCalChannel_ForwardCall()  @ 0x824B6438 | size: 0xC
// 
// Forwards a call to CCalChannel_60A8_p46 with data from offset +608.
// Likely part of audio or animation channel management.
// ─────────────────────────────────────────────────────────────────────────────
void CCalChannel_ForwardCall(atSingleton* obj, uint32_t param) {
    void* channelData = *(void**)((uint8_t*)obj + 608);
    CCalChannel_60A8_p46(channelData, param);
}

// ─────────────────────────────────────────────────────────────────────────────
// grcDevice_Callback()  @ 0x824B6EF0 | size: 0xC
// 
// Calls grc_DCA8 with the global graphics device pointer.
// Part of the graphics rendering system initialization/cleanup.
// ─────────────────────────────────────────────────────────────────────────────
void grcDevice_Callback() {
    void* grcDevice = (void*)g_grcDevicePtr;
    grc_DCA8(grcDevice);
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_SetVtable()  @ 0x824B22D0 | size: 0x10
// 
// Sets the vtable pointer to a specific vtable address.
// Used during object construction or vtable switching.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_SetVtable(atSingleton* obj) {
    // Vtable address: 0x82003DC8
    obj->vtable = (void*)0x82003DC8;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_GetFloatSum()  @ 0x824B4148 | size: 0x10
// 
// Returns the sum of two float fields at offsets +120 and +92.
// Likely calculates a total time, distance, or accumulated value.
// ─────────────────────────────────────────────────────────────────────────────
float datBase_GetFloatSum(atSingleton* obj) {
    float value1 = *(float*)((uint8_t*)obj + 120);
    float value2 = *(float*)((uint8_t*)obj + 92);
    return value1 + value2;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyArray8()  @ 0x820E74B8 | size: 0x68
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
//      - Free the array memory via rage_free
// ─────────────────────────────────────────────────────────────────────────────
void datBase_DestroyArray8(atSingleton* obj) {
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
        datBase_Dtor(element);
        arrayEnd = element;
    }
    
    // Free the array memory
    rage_free(arrayMetadata);
}


// ─────────────────────────────────────────────────────────────────────────────
// datBase_SetVtable_054B8()  @ 0x8245A828 | size: 0x10
// 
// Sets vtable pointer to 0x820054B8.
// Constructor or vtable initialization function.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_SetVtable_054B8(atSingleton* obj) {
    obj->vtable = (void*)0x820054B8;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_InitChain_05624()  @ 0x8245DB50 | size: 0x10
// 
// Sets vtable pointer to 0x82005624, then calls datBase_InitChain_057A4.
// Chained initialization pattern.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_InitChain_05624(atSingleton* obj) {
    obj->vtable = (void*)0x82005624;
    datBase_InitChain_057A4(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_InitChain_057A4()  @ 0x8245FA08 | size: 0x10
// 
// Sets vtable pointer to 0x820057A4, then calls datBase_SetVtable.
// Multi-stage initialization chain.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_InitChain_057A4(atSingleton* obj) {
    obj->vtable = (void*)0x820057A4;
    datBase_SetVtable(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// KeTimeStampBundle_GetField16()  @ 0x824B6F98 | size: 0x10
// 
// Returns a value from a global structure.
// Loads pointer from global at 0x8200084C, then returns field at offset +16.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t KeTimeStampBundle_GetField16() {
    uint32_t* globalPtr = *(uint32_t**)0x8200084C;
    return globalPtr[4];  // offset +16 = index 4
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_CallIndirect()  @ 0x824B6108 | size: 0x14
// 
// Indirect function call through function pointer at offset +4.
// Loads first parameter from offset +0, then calls function pointer at +4.
// ─────────────────────────────────────────────────────────────────────────────
void rage_CallIndirect(void* obj) {
    typedef void (*FuncPtr)(void*);
    void* param = *(void**)obj;
    FuncPtr func = *(FuncPtr*)((uint8_t*)obj + 4);
    func(param);
}

// ─────────────────────────────────────────────────────────────────────────────
// math_Abs()  @ 0x824F9980 | size: 0x10
// 
// Returns absolute value of signed integer.
// If value is negative, returns its negation; otherwise returns early.
// ─────────────────────────────────────────────────────────────────────────────
int32_t math_Abs(int32_t value) {
    if (value >= 0) {
        return value;
    }
    return -value;
}

// ─────────────────────────────────────────────────────────────────────────────
// SinglesNetworkClient_ResetAndInit()  @ 0x821C6F38 | size: 0x14
// 
// Stores 0 at offset 75756 (0x127EC), then calls SinglesNetworkClient_4FB0_g.
// Network client initialization helper.
// ─────────────────────────────────────────────────────────────────────────────
void SinglesNetworkClient_ResetAndInit(uint8_t* obj) {
    *(uint32_t*)(obj + 75756) = 0;
    extern void SinglesNetworkClient_4FB0_g(uint8_t* obj);
    SinglesNetworkClient_4FB0_g(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_StoreTimebase()  @ 0x8257DB58 | size: 0x10
// 
// Stores current timebase value to global at 0x82619BB0.
// Performance counter / timing utility.
// ─────────────────────────────────────────────────────────────────────────────
void rage_StoreTimebase() {
    extern uint64_t PPC_QUERY_TIMEBASE();
    uint64_t timebase = PPC_QUERY_TIMEBASE();
    *(uint64_t*)0x82619BB0 = timebase;
}

// ─────────────────────────────────────────────────────────────────────────────
// nt_InitThread()  @ 0x824FAA30 | size: 0x10
// 
// Calls nt_8948 with parameters (obj, 0, 32768).
// Network or threading utility with specific flags.
// ─────────────────────────────────────────────────────────────────────────────
void nt_InitThread(void* obj) {
    extern void nt_8948(void* obj, uint32_t param1, uint32_t param2);
    nt_8948(obj, 0, 32768);
}

// ─────────────────────────────────────────────────────────────────────────────
// math_ModPositive()  @ 0x824F7D58 | size: 0x14
// 
// Modulo operation: returns (value1 % value2).
// Repeatedly subtracts value2 from value1 until result is negative,
// then adds value2 back once to get the remainder.
// ─────────────────────────────────────────────────────────────────────────────
int32_t math_ModPositive(int32_t value1, int32_t value2) {
    int32_t result = value1;
    while (result >= 0) {
        result -= value2;
    }
    return result + value2;
}


// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyArray80()  @ 0x820C8620 | size: 0x78
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
void datBase_DestroyArray80(atSingleton* obj) {
    // Get array base (16 bytes before this pointer)
    uint8_t* arrayBase = (uint8_t*)obj - 16;
    
    // Get element count from first field
    uint32_t elementCount = *(uint32_t*)arrayBase;
    
    // Check if array is valid
    if (elementCount == 0) {
        rage_free(arrayBase);
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
    rage_free(arrayBase);
}

// ─────────────────────────────────────────────────────────────────────────────
// pgDictionary_Destroy()  @ 0x820C33A8 | size: 0x140
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
void pgDictionary_Destroy(atSingleton* obj) {
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
            extern uint8_t datBase_IsTracked(void* ptr);
            if (datBase_IsTracked(objectArray) == 0) {
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
            extern uint8_t datBase_IsTracked(void* ptr);
            if (datBase_IsTracked(array2) == 0) {
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
// datBase_HashTableLookup()  @ 0x820CF950 | size: 0xEC
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
void* datBase_HashTableLookup(atSingleton* hashTable, void* keyObj) {
    uint16_t bucketCount = *(uint16_t*)((uint8_t*)hashTable + 4);
    if (bucketCount == 0) {
        return nullptr;
    }
    
    // Calculate hash of key string
    extern uint32_t datBase_HashString(void* keyObj);
    uint32_t hash = datBase_HashString(keyObj);
    
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
// datBase_DestroyWithAnimFrames()  @ 0x820CB960 | size: 0xE0
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
void datBase_DestroyWithAnimFrames(atSingleton* obj) {
    // Set initial vtables
    obj->vtable = (void*)0x82033E54;
    *(void**)((uint8_t*)obj + 4) = (void*)0x82033E60;
    
    // Initialize embedded object at +80
    void* embeddedObj80 = (void*)((uint8_t*)obj + 80);
    *(void**)embeddedObj80 = (void*)0x8204FCBC;
    
    extern void crAnimFrame_Clear(void* obj);
    crAnimFrame_Clear(embeddedObj80);
    
    // Check if embedded object has allocated memory and free it
    uint16_t count80 = *(uint16_t*)((uint8_t*)embeddedObj80 + 14);
    if (count80 != 0) {
        void* memory80 = *(void**)((uint8_t*)embeddedObj80 + 8);
        rage_free(memory80);
    }
    
    // Initialize embedded object at +64
    void* embeddedObj64 = (void*)((uint8_t*)obj + 64);
    *(void**)embeddedObj64 = (void*)0x8204FCBC;
    crAnimFrame_Clear(embeddedObj64);
    
    uint16_t count64 = *(uint16_t*)((uint8_t*)embeddedObj64 + 14);
    if (count64 != 0) {
        void* memory64 = *(void**)((uint8_t*)embeddedObj64 + 8);
        rage_free(memory64);
    }
    
    // Initialize embedded object at +48
    void* embeddedObj48 = (void*)((uint8_t*)obj + 48);
    *(void**)embeddedObj48 = (void*)0x8204FCBC;
    crAnimFrame_Clear(embeddedObj48);
    
    uint16_t count48 = *(uint16_t*)((uint8_t*)embeddedObj48 + 14);
    if (count48 != 0) {
        void* memory48 = *(void**)((uint8_t*)embeddedObj48 + 8);
        rage_free(memory48);
    }
    
    // Initialize embedded object at +32
    void* embeddedObj32 = (void*)((uint8_t*)obj + 32);
    *(void**)embeddedObj32 = (void*)0x8204FCBC;
    crAnimFrame_Clear(embeddedObj32);
    
    uint16_t count32 = *(uint16_t*)((uint8_t*)embeddedObj32 + 14);
    if (count32 != 0) {
        void* memory32 = *(void**)((uint8_t*)embeddedObj32 + 8);
        rage_free(memory32);
    }
    
    // Set final vtables
    *(void**)((uint8_t*)obj + 4) = (void*)0x82027B34;
    obj->vtable = (void*)0x820276C4;
}

// ═════════════════════════════════════════════════════════════════════════════
// atSingleton Vtable Methods
// ═════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::GetDebugName()  [vtable slot 15 @ 0x82229638]
// 
// Returns a pointer to a constant string in .rdata.
// This appears to be a debug or error message getter.
// ─────────────────────────────────────────────────────────────────────────────
const char* atSingleton::GetDebugName() {
    // String at 0x8204E918: "t number of points specified"
    // Likely part of a longer error message like "Incorrect number of points specified"
    return (const char*)0x8204E918;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::UpdatePage()  [vtable slot 16 @ 0x822BDC38]
// 
// Checks game loop state and conditionally calls a page-related function.
// Returns early if certain flags are set, otherwise calls pg_FDF0_fw.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::UpdatePage() {
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
// atSingleton::SetEnabled()  [vtable slot 17 @ 0x8244A578]
// 
// Manages a flag state and conditionally triggers virtual method calls on a
// child object. Handles bit manipulation for state flags and coordinates
// between parent and child object states.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::SetEnabled(int enableFlag) {
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
// atSingleton::OnPauseCheck()  [vtable slot 18 @ 0x822F9D88]
// 
// Checks global loop state and conditionally clears a local flag or triggers
// a virtual method call on a child object.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::OnPauseCheck() {
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
// atSingleton::Shutdown()  [vtable slot 19 @ 0x822C1B10]
// 
// Tail-call dispatcher - loads vtable slot 20 and jumps to it.
// This is a common pattern for forwarding calls through the vtable hierarchy.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::Shutdown() {
    // Load vtable and call slot 20
    typedef void (*VTableFunc20)(void*);
    void** vtable = *(void***)this;
    VTableFunc20 func20 = (VTableFunc20)vtable[20];
    func20(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::DestroyTarget()  [vtable slot 20 @ 0x822FB3B8]
// 
// Indirect dispatcher - loads an object from offset +20, adjusts pointer by +4,
// then calls that object's vtable slot 0 with parameter 0.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton::DestroyTarget() {
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
// atSingleton::OnTransition()  [vtable slot 21 @ 0x8245C108]
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
int atSingleton::OnTransition(int transitionType) {
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
        extern void datBase_SetState(void*, int);
        datBase_SetState(this, 5);
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
        extern void datBase_SetState(void*, int);
        datBase_SetState(this, 4);
        
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
        extern void datBase_SetState(void*, int);
        datBase_SetState(this, 4);
        
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
    extern void datBase_SetState(void*, int);
    datBase_SetState(this, 5);
    
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// game_CheckStateFlags()  @ 0x820D9C18 | size: 0x54
// 
// Checks game state flags to determine if a specific condition is met.
// 
// Returns true if:
//   - Bit 0 of flags is NOT set (inverted check)
//   - AND bit 2 of flags IS set
// Otherwise returns false.
// 
// This appears to check if the game is in a specific state (e.g., paused but not loading).
// ─────────────────────────────────────────────────────────────────────────────
uint8_t game_CheckStateFlags() {
    // Navigate through global structure chain to get flags
    // Global at 0x82120000 - 23816 = 0x821BA0F8
    extern uint32_t* g_gameStatePtr;  // At 0x821BA0F8
    
    uint32_t* level1 = *(uint32_t**)0x821BA0F8;
    uint32_t* level2 = (uint32_t*)((uint8_t*)level1 + 20);
    uint32_t* level3 = (uint32_t*)((uint8_t*)level2 + 9736);
    uint32_t flags = *(uint32_t*)level3;
    
    // Check if bit 0 is NOT set
    bool bit0NotSet = ((flags & 0x1) == 0);
    
    if (!bit0NotSet) {
        return 0;
    }
    
    // Check if bit 2 is set
    bool bit2Set = ((flags & 0x4) != 0);
    
    return bit2Set ? 1 : 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// fragDrawable_CopyData48()  @ 0x821257F0 | size: 0x84
// 
// Copies 48 bytes of data using VMX128 vector instructions, then initializes
// additional fields.
// 
// Structure being initialized:
//   +0..+31: First 32 bytes (copied via vector)
//   +16..+47: Next 32 bytes (copied via vector)
//   +32..+47: Additional 16 bytes (copied as scalars)
//   +48: Result from rage_ConvertParam
//   +52..+60: Zeroed fields
// ─────────────────────────────────────────────────────────────────────────────
void fragDrawable_CopyData48(atSingleton* dest, void* param1, void* sourceData) {
    // Copy first 32 bytes using vector load/store
    uint8_t* src = (uint8_t*)sourceData;
    uint8_t* dst = (uint8_t*)dest;
    
    // Copy bytes 0-15
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i];
    }
    
    // Copy bytes 16-31
    for (int i = 16; i < 32; i++) {
        dst[i] = src[i];
    }
    
    // Copy bytes 32-47 as individual words
    *(uint32_t*)(dst + 32) = *(uint32_t*)(src + 32);
    *(uint32_t*)(dst + 36) = *(uint32_t*)(src + 36);
    *(uint32_t*)(dst + 40) = *(uint32_t*)(src + 40);
    *(uint32_t*)(dst + 44) = *(uint32_t*)(src + 44);
    
    // Get value from param1 and call function
    uint32_t* param1Ptr = (uint32_t*)param1;
    extern uint32_t rage_ConvertParam(uint32_t value);
    uint32_t result = rage_ConvertParam(*param1Ptr);
    
    // Store result and zero remaining fields
    *(uint32_t*)(dst + 48) = result;
    *(uint32_t*)(dst + 52) = 0;
    *(uint32_t*)(dst + 56) = 0;
    *(uint32_t*)(dst + 60) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_FindListTail()  @ 0x82125F48 | size: 0x74
// 
// Traverses a linked list to find the last node.
// 
// Algorithm:
//   1. Start at node in field +60 of input object
//   2. If that node exists, follow +56 links until reaching last node
//   3. If no node at +60, check node at +52
//   4. Validate that +52 node's +56 link points back to input
//   5. Return the last node found, or nullptr
// 
// This appears to be finding the tail of a doubly-linked list.
// ─────────────────────────────────────────────────────────────────────────────
void* datBase_FindListTail(void* obj) {
    uint8_t* objBytes = (uint8_t*)obj;
    
    // Try to get node from offset +60
    void* node = *(void**)(objBytes + 60);
    
    if (node != nullptr) {
        // Follow +56 links to find last node
        void* nextNode = *(void**)((uint8_t*)node + 56);
        while (nextNode != nullptr) {
            node = nextNode;
            nextNode = *(void**)((uint8_t*)node + 56);
        }
        return node;
    }
    
    // No node at +60, try +52
    void* node52 = *(void**)(objBytes + 52);
    if (node52 == nullptr) {
        return nullptr;
    }
    
    // Validate that node52's +56 link points back to obj
    void* backLink = *(void**)((uint8_t*)node52 + 56);
    if (backLink == obj) {
        return node52;
    }
    
    // Recursively search from node52
    return datBase_FindListTail(node52);
}

// ─────────────────────────────────────────────────────────────────────────────
// ph_LookupStringAndStoreResult()  @ 0x82118D30 | size: 0x88
// 
// Generates a string representation and stores a result byte.
// 
// Algorithm:
//   1. Create a 64-byte buffer on stack
//   2. Zero-initialize the buffer
//   3. Call vtable function on param's +4 field to generate string
//   4. Look up string in global table
//   5. Store result byte at offset +444 in destination object
// ─────────────────────────────────────────────────────────────────────────────
void ph_LookupStringAndStoreResult(atSingleton* dest, void* param) {
    // Create and zero-initialize 64-byte buffer
    char buffer[64];
    buffer[0] = 0;
    
    extern void memset(void* ptr, int value, size_t size);
    memset(buffer + 1, 0, 63);
    
    // Get object from param's +4 field and call vtable slot 1
    void* paramObj = *(void**)((uint8_t*)param + 4);
    void** vtable = *(void***)paramObj;
    typedef void (*ToStringFunc)(void*, char*, int);
    ToStringFunc toString = (ToStringFunc)vtable[1];
    toString(paramObj, buffer, 64);
    
    // Look up string in global table
    extern uint32_t* g_globalTable;  // At 0x82200000 + 25528
    uint32_t* globalTable = *(uint32_t**)0x82206398;
    void* lookupTable = (void*)((uint8_t*)globalTable + 908);
    
    // Call lookup function (vtable slot 3)
    void** lookupVtable = *(void***)lookupTable;
    typedef uint8_t (*LookupFunc)(void*, const char*);
    LookupFunc lookup = (LookupFunc)lookupVtable[3];
    uint8_t result = lookup(lookupTable, buffer);
    
    // Store result at offset +444
    *((uint8_t*)dest + 444) = result;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_SyncProperty()  @ 0x8212BF40 | size: 0x74
// 
// Conditional vtable dispatch based on flag.
// 
// If byte at +4 is non-zero:
//   - Calls vtable slot 39 (offset +156) with uint16 from output
// Otherwise:
//   - Calls vtable slot 3 (offset +12) and stores result in output
// ─────────────────────────────────────────────────────────────────────────────
void datBase_SyncProperty(void* obj, uint16_t* output) {
    uint8_t* objBytes = (uint8_t*)obj;
    uint8_t flag = objBytes[4];
    void* targetObj = *(void**)objBytes;
    
    if (flag != 0) {
        // Call vtable slot 39 with existing value
        void** vtable = *(void***)targetObj;
        typedef void (*Slot39Func)(void*, uint16_t);
        Slot39Func func = (Slot39Func)vtable[39];
        func(targetObj, *output);
    } else {
        // Call vtable slot 3 and store result
        void** vtable = *(void***)targetObj;
        typedef uint16_t (*Slot3Func)(void*);
        Slot3Func func = (Slot3Func)vtable[3];
        *output = func(targetObj);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// crAnimation_LoadByName()  @ 0x820C8958 | size: 0x168
// 
// Loads an animation by name with fallback logic.
// 
// Algorithm:
//   1. Try to load animation from primary source
//   2. If not found and createIfMissing flag is false, return nullptr
//   3. If not found, build filename with extension and try loading
//   4. If still not found, try alternate extension
//   5. If found, register animation with manager
//   6. If all attempts fail, log error
// 
// This is a resource loading function with multiple fallback paths.
// ─────────────────────────────────────────────────────────────────────────────
void* crAnimation_LoadByName(atSingleton* loader, const char* animName, uint8_t createIfMissing) {
    // Try to load from physics system
    void* physicsSystem = *(void**)((uint8_t*)loader + 4);
    extern void* ph_6FC8(void* system);
    void* animation = ph_6FC8(physicsSystem);
    
    if (animation != nullptr) {
        return animation;
    }
    
    // If not creating missing, return nullptr
    if (createIfMissing == 0) {
        return nullptr;
    }
    
    // Build filename with extension
    char filename[64];
    extern void _snprintf(char* buffer, size_t size, const char* format, ...);
    _snprintf(filename, 64, "%s", animName);
    
    // Find last '.' in filename
    extern char* game_0EA0(const char* str, int ch);
    char* dotPos = game_0EA0(filename, '.');
    if (dotPos != nullptr) {
        *dotPos = '\0';  // Truncate at dot
    }
    
    // Get global animation cache
    extern uint8_t* g_animCache;  // At 0x825D0080
    uint8_t* animCache = (uint8_t*)0x825D0080;
    
    // Try loading with first extension
    void* baseObj = *(void**)loader;
    extern void crAnimCache_SetSource(void* cache, void* obj);
    crAnimCache_SetSource(animCache, baseObj);
    
    extern uint8_t rage_3118(void* cache, const char* name, const char* ext);
    uint8_t found = rage_3118(animCache, filename, "");
    
    if (found) {
        extern void* crAnimation_ctor_8F98(const char* name, int flag1, int flag2);
        animation = crAnimation_ctor_8F98(animName, 1, 0);
    }
    
    // Decrement reference count
    uint32_t* refCount = (uint32_t*)(animCache + 1536);
    (*refCount)--;
    
    if (animation == nullptr) {
        // Try alternate extension
        crAnimCache_SetSource(animCache, baseObj);
        found = rage_3118(animCache, filename, "");
        
        if (found) {
            animation = crAnimation_ctor_8F98(animName, 1, 0);
        }
        
        (*refCount)--;
        
        if (animation == nullptr) {
            // Log error
            extern void nop_8240E6D0(const char* format, ...);
            nop_8240E6D0("Failed to load animation: %s from %s or %s", animName, animName, animName);  /* UNVERIFIED — string not found in binary */
            return nullptr;
        }
    }
    
    // Register animation with manager
    extern void ph_RegisterAnimation(void* system, const char* name, void* anim);
    ph_RegisterAnimation(physicsSystem, animName, animation);
    
    return animation;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_StoreTimebase2()  @ 0x8257C940 | size: 0x14
// 
// Stores current PowerPC timebase counter to a global variable.
// Used for performance profiling and timing measurements.
// ─────────────────────────────────────────────────────────────────────────────
void rage_StoreTimebase2() {
    extern uint64_t PPC_QUERY_TIMEBASE();
    extern uint64_t g_timebase_storage;  // @ 0x825DAEF0
    
    g_timebase_storage = PPC_QUERY_TIMEBASE();
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_LookupByIndex()  @ 0x8225FE80 | size: 0x18
// 
// Array lookup function - retrieves pointer from indexed array.
// Formula: array[(index * 8) - 8]
// ─────────────────────────────────────────────────────────────────────────────
void* rage_LookupByIndex(uint32_t index) {
    extern uint32_t g_singleton_array[];  // @ 0x825C4960
    
    // Calculate array offset: (index * 8) - 8
    uint32_t offset = (index << 3) - 8;
    return (void*)g_singleton_array[offset / 4];
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_CallVtableSlot22()  @ 0x82454300 | size: 0x18
// 
// Virtual function call through vtable slot 22.
// Loads object from offset +48, then calls its vtable method at slot 22.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_CallVtableSlot22(atSingleton* obj) {
    atSingleton* target = (atSingleton*)((uint8_t*)obj + 48);
    
    // Call vtable slot 22 (offset 88 = 22 * 4)
    void** vtable = (void**)target->vtable;
    typedef void (*VTableFunc)(atSingleton*);
    VTableFunc func = (VTableFunc)vtable[22];
    func(target);
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_4097()  @ 0x8225F148 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C5320, index = (id & 0xFFFF) - 4097
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_4097(uint32_t id) {
    extern uint32_t g_lookup_table_F148[];  // @ 0x825C5320
    
    uint16_t index = (id & 0xFFFF) - 4097;
    return (void*)g_lookup_table_F148[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_12289()  @ 0x8225EE58 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C57A0, index = (id & 0xFFFF) - 12289
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_12289(uint32_t id) {
    extern uint32_t g_lookup_table_EE58[];  // @ 0x825C57A0
    
    uint16_t index = (id & 0xFFFF) - 12289;
    return (void*)g_lookup_table_EE58[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_2049()  @ 0x8225EFD0 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C5438, index = (id & 0xFFFF) - 2049
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_2049(uint32_t id) {
    extern uint32_t g_lookup_table_EFD0[];  // @ 0x825C5438
    
    uint16_t index = (id & 0xFFFF) - 2049;
    return (void*)g_lookup_table_EFD0[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_18433()  @ 0x8225F2C0 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C5218, index = (id & 0xFFFF) - 18433
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_18433(uint32_t id) {
    extern uint32_t g_lookup_table_F2C0[];  // @ 0x825C5218
    
    uint16_t index = (id & 0xFFFF) - 18433;
    return (void*)g_lookup_table_F2C0[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_20481()  @ 0x8225F438 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C51B0, index = (id & 0xFFFF) - 20481
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_20481(uint32_t id) {
    extern uint32_t g_lookup_table_F438[];  // @ 0x825C51B0
    
    uint16_t index = (id & 0xFFFF) - 20481;
    return (void*)g_lookup_table_F438[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// snSession_LookupById_10241()  @ 0x8225F5B0 | size: 0x1C
// 
// Lookup table access - maps 16-bit ID to pointer.
// Table base: 0x825C5140, index = (id & 0xFFFF) - 10241
// ─────────────────────────────────────────────────────────────────────────────
void* snSession_LookupById_10241(uint32_t id) {
    extern uint32_t g_lookup_table_F5B0[];  // @ 0x825C5140
    
    uint16_t index = (id & 0xFFFF) - 10241;
    return (void*)g_lookup_table_F5B0[index * 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_InitFields14552()  @ 0x8255F918 | size: 0x18
// 
// Initializes three consecutive fields and calls another initialization function.
// Sets fields at offsets +14552, +14556, +14560 to specific values.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_InitFields14552(atSingleton* obj) {
    extern void datBase_InitDefaults(atSingleton* obj);
    
    uint32_t* fields = (uint32_t*)((uint8_t*)obj + 14552);
    fields[0] = 1;  // +14552
    fields[1] = 0;  // +14556
    fields[2] = 1;  // +14560
    
    datBase_InitDefaults(obj);
}
