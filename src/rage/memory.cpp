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
            extern void rage_DebugLog(const char* format, ...);
            rage_DebugLog("Failed to load animation: %s from %s or %s", animName, animName, animName);  /* UNVERIFIED — string not found in binary */
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

// ─────────────────────────────────────────────────────────────────────────────
// phBound_WriteChannelData()  @ 0x82123680 | size: 0x6C
//
// Writes physics bound channel data during serialization.
// Looks up a bound by its flags pointer, copies channel data at offset +168
// into a destination buffer at offset +176.
//
// Parameters:
//   r3 = destination object (phBound)
//   r4 = source descriptor
// ─────────────────────────────────────────────────────────────────────────────
void phBound_WriteChannelData(uint8_t* dest, uint8_t* descriptor) {
    extern void ph_5908(void* flagsPtr, const void* key, uint32_t mode);
    extern void rage_3F18(void* global, void* dataPtr, void* outBuf);
    extern void ke_0E08(void* destBuf, void* srcData);

    // Look up bound data from descriptor's flags field
    void* flagsPtr = *(void**)(descriptor + 4);
    ph_5908(flagsPtr, (void*)0x82027660, 1);

    // Get the data pointer from flags->field_12
    void* flagsPtr2 = *(void**)(descriptor + 4);
    void* dataPtr = *(void**)((uint8_t*)flagsPtr2 + 12);
    void* global = *(void**)0x8272A374;
    rage_3F18(global, dataPtr, dest + 168);

    // Copy channel data: unlock and write from +168 to +176
    flagsPtr = *(void**)(descriptor + 4);
    ph_5908(flagsPtr, (void*)0x8202766C, 1);

    void* srcData = *(void**)(dest + 168);
    ke_0E08(dest + 176, srcData);
}

// ─────────────────────────────────────────────────────────────────────────────
// phBound_WriteChannelDataAlt()  @ 0x821236F0 | size: 0x6C
//
// Variant of phBound_WriteChannelData with different offsets.
// Reads from offset +172 and writes to offset +380.
// Same channel lock/unlock/copy pattern.
//
// Parameters:
//   r3 = destination object
//   r4 = source descriptor
// ─────────────────────────────────────────────────────────────────────────────
void phBound_WriteChannelDataAlt(uint8_t* dest, uint8_t* descriptor) {
    extern void ph_5908(void* flagsPtr, const void* key, uint32_t mode);
    extern void rage_3F18(void* global, void* dataPtr, void* outBuf);
    extern void ke_0E08(void* destBuf, void* srcData);

    // Lock channel for reading
    void* flagsPtr = *(void**)(descriptor + 4);
    ph_5908(flagsPtr, (void*)0x82027660, 1);

    // Copy data from descriptor into offset +172
    void* flagsPtr2 = *(void**)(descriptor + 4);
    void* dataPtr = *(void**)((uint8_t*)flagsPtr2 + 12);
    void* global = *(void**)0x8272A374;
    rage_3F18(global, dataPtr, dest + 172);

    // Unlock channel
    flagsPtr = *(void**)(descriptor + 4);
    ph_5908(flagsPtr, (void*)0x8202766C, 1);

    // Write channel data from +172 to +380
    void* srcData = *(void**)(dest + 172);
    ke_0E08(dest + 380, srcData);
}

// ─────────────────────────────────────────────────────────────────────────────
// fragDrawable_LoadDamageShape()  @ 0x82124EF0 | size: 0x7C
//
// Loads a damage shape for a fragDrawable by name comparison.
// If the shape name does NOT match "dShape%d", looks it up via hash
// and replaces the existing shape pointer at offset +288.
//
// Parameters:
//   r3 = fragDrawable object
//   r4 = shape descriptor (vtable slot 1 called to get name)
// ─────────────────────────────────────────────────────────────────────────────
void fragDrawable_LoadDamageShape(uint8_t* drawable, void* shapeDesc) {
    extern int _stricmp(const char* a, const char* b);
    extern void* atSingleton_29E0_g(const char* name);
    extern void rage_free_00C0(void* ptr);

    // Get shape name from descriptor via vtable slot 1
    char nameBuffer[128];
    void* descFlags = *(void**)((uint8_t*)shapeDesc + 4);
    // VCALL slot 1: copies name into nameBuffer
    typedef void (*GetNameFunc)(void*, char*);
    void** vtable = *(void***)descFlags;
    GetNameFunc getName = (GetNameFunc)vtable[1];
    getName(descFlags, nameBuffer);

    // Compare against "dShape%d" pattern
    if (_stricmp(nameBuffer, "dShape%d") != 0) {
        // Not the default shape - look up by name hash
        void* newShape = atSingleton_29E0_g(nameBuffer);

        // Free the old shape at offset +288
        void* oldShape = *(void**)(drawable + 288);
        rage_free_00C0(oldShape);

        // Store new shape
        *(void**)(drawable + 288) = newShape;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pgArray_FindActiveEntry()  @ 0x82118128 | size: 0x9C
//
// Searches a pgArray-style container for the first active entry starting
// from a given index, stepping by a given stride.
// An entry is "active" if its byte at offset +16 is non-zero.
//
// Parameters:
//   r3 = pgArray pointer (count at +28, data at +24)
//   r4 = start index offset
//   r5 = stride (step between probes)
//   r6 = wrapFlag (if nonzero, clamp start index to [0, count-1])
//
// Returns: index of first active entry, or -1 if none found
// ─────────────────────────────────────────────────────────────────────────────
int32_t pgArray_FindActiveEntry(uint8_t* array, int32_t startOffset, int32_t stride, uint8_t wrapFlag) {
    extern int32_t pg_C4E8_g(int32_t value, int32_t min, int32_t max);

    int32_t index = startOffset + stride;

    // Optionally clamp index to valid range
    if (wrapFlag != 0) {
        uint16_t count = *(uint16_t*)(array + 28);
        index = pg_C4E8_g(index, 0, count - 1);
    }

    // Search forward by stride for an active entry
    while (index >= 0) {
        uint16_t count = *(uint16_t*)(array + 28);
        if (index >= count) {
            break;
        }

        void** dataArray = *(void***)(array + 24);
        void* entry = dataArray[index];
        uint8_t activeFlag = *(uint8_t*)((uint8_t*)entry + 16);
        if (activeFlag != 0) {
            return index;
        }

        index += stride;
        if (index < 0) {
            break;
        }
    }

    return -1;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyPointerArray()  @ 0x820F3468 | size: 0xA0
//
// Destroys a dynamically allocated pointer array.
// For each non-null pointer in the array, checks if it is tracked
// by the singleton system. If not tracked, frees it via the allocator.
// Finally frees the array itself.
//
// Parameters:
//   r3 = object with refCount at +6 and vtable/array pointer at +0
// ─────────────────────────────────────────────────────────────────────────────
void datBase_DestroyPointerArray(atSingleton* obj) {
    extern bool atSingleton_Find_90D0(void* ptr);
    extern void rage_free_00C0(void* ptr);

    // Only process if refCount is non-zero
    if (obj->refCount == 0) {
        return;
    }

    // Only process if vtable/data pointer is valid
    void* dataPtr = obj->vtable;
    if (dataPtr == nullptr) {
        return;
    }

    // Array metadata is stored at dataPtr[-1] (4 bytes before)
    uint32_t* arrayBase = (uint32_t*)dataPtr - 1;
    uint32_t elementCount = *arrayBase;

    // Walk the array in reverse, freeing untracked pointers
    uint32_t** ptrArray = (uint32_t**)dataPtr;
    for (int32_t i = elementCount - 1; i >= 0; i--) {
        void* element = ptrArray[i];
        if (element == nullptr) {
            continue;
        }

        // Check if element is tracked by singleton system
        if (!atSingleton_Find_90D0(element)) {
            // Not tracked - free via allocator (SDA vtable slot 2)
            void** sdaPtr = *(void***)0x82600000;
            void* allocator = ((void**)sdaPtr)[1];
            typedef void (*FreeFunc)(void*, void*);
            void** allocVtable = *(void***)allocator;
            FreeFunc freeFunc = (FreeFunc)allocVtable[2];
            freeFunc(allocator, element);
        }
    }

    // Free the array base
    rage_free_00C0(arrayBase);
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst_SumWeights()  @ 0x82118438 | size: 0xA8
//
// Sums float values at offset +144 from each element in a pointer array.
// Uses loop unrolling (4 elements at a time) for performance.
// The pointer array is at offset +180 of the object, and the
// element count is at offset +427 (byte).
//
// Parameters:
//   r3 = phInst-like object with bone/weight array
//
// Returns: sum of all weight floats (in f1)
// ─────────────────────────────────────────────────────────────────────────────
float phInst_SumWeights(uint8_t* obj) {
    uint8_t totalCount = *(uint8_t*)(obj + 427);
    float sum = 0.0f;

    // Process 4 elements at a time (unrolled loop)
    uint32_t unrolledCount = 0;
    if (totalCount >= 4) {
        void** ptrArray = *(void***)(obj + 180);
        uint32_t batches = ((totalCount - 4) >> 2) + 1;
        unrolledCount = batches * 4;
        void** current = ptrArray + 2;  // start at index 2 (offset +8 bytes)

        for (uint32_t b = 0; b < batches; b++) {
            void* e0 = current[-2];
            void* e1 = current[-1];
            void* e2 = current[0];
            void* e3 = current[1];
            current += 4;

            sum += *(float*)((uint8_t*)e0 + 144);
            sum += *(float*)((uint8_t*)e1 + 144);
            sum += *(float*)((uint8_t*)e2 + 144);
            sum += *(float*)((uint8_t*)e3 + 144);
        }
    }

    // Process remaining elements one at a time
    if ((int32_t)unrolledCount < totalCount) {
        void** ptrArray = *(void***)(obj + 180);
        uint32_t remaining = totalCount - unrolledCount;
        void** current = ptrArray + unrolledCount;

        for (uint32_t i = 0; i < remaining; i++) {
            void* entry = current[i];
            sum += *(float*)((uint8_t*)entry + 144);
        }
    }

    return sum;
}

// ─────────────────────────────────────────────────────────────────────────────
// netFirewall_TryAddByMaxCount()  @ 0x82117EF0 | size: 0xB8
//
// Attempts to add a network entry to a firewall filter.
// Checks if the filter has capacity (count < maxCount), then validates
// that the entry's sequence number meets the minimum threshold.
// If valid, calls the virtual add function and increments the count.
//
// Parameters:
//   r3 = firewall manager
//   r4 = channel index
//   r5 = filter descriptor (count at +4, maxCount at +8, minSeq at +36)
// ─────────────────────────────────────────────────────────────────────────────
void netFirewall_TryAddByMaxCount(uint8_t* manager, uint32_t channelIdx, uint8_t* filter) {
    extern bool atSingleton_7068_fw(uint8_t* manager, uint32_t channelIdx, void* entry);

    // Check if filter has capacity
    int32_t currentCount = *(int32_t*)(filter + 4);
    int32_t maxCount = *(int32_t*)(filter + 8);
    bool hasCapacity = (currentCount < maxCount);

    if (!hasCapacity) {
        return;
    }

    // Look up channel data from global array
    void** globalArray = *(void***)0x8272A324;
    void* channelData = *(void**)((uint8_t*)globalArray + channelIdx * 8 + 252);

    // Get sequence number via vtable slot 2 (mode=21)
    typedef void* (*GetSeqFunc)(void*, int32_t);
    void** vtable = *(void***)channelData;
    GetSeqFunc getSeq = (GetSeqFunc)vtable[2];
    void* seqResult = getSeq(channelData, 21);

    // Check minimum sequence threshold
    int32_t seqValue = *(int32_t*)seqResult;
    int32_t minSeq = *(int32_t*)(filter + 36);
    if (seqValue < minSeq) {
        return;
    }

    // Get entry data via filter's virtual function (vtable slot 0)
    typedef void* (*GetEntryFunc)(void*);
    void** filterVtable = *(void***)filter;
    GetEntryFunc getEntry = (GetEntryFunc)filterVtable[0];
    void* entry = getEntry(filter);

    // Try to add the entry
    if (atSingleton_7068_fw(manager, channelIdx, entry)) {
        // Success - increment count
        int32_t count = *(int32_t*)(filter + 4);
        *(int32_t*)(filter + 4) = count + 1;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// netFirewall_TryAddByDualRange()  @ 0x82117D28 | size: 0xE4
//
// Attempts to add a network entry to a firewall filter with dual range check.
// First checks capacity, then validates both minimum and maximum sequence
// number thresholds before adding the entry.
//
// Parameters:
//   r3 = firewall manager
//   r4 = channel index
//   r5 = filter descriptor
// ─────────────────────────────────────────────────────────────────────────────
void netFirewall_TryAddByDualRange(uint8_t* manager, uint32_t channelIdx, uint8_t* filter) {
    extern bool atSingleton_7068_fw(uint8_t* manager, uint32_t channelIdx, void* entry);

    // Check if filter has capacity
    int32_t currentCount = *(int32_t*)(filter + 4);
    int32_t maxCount = *(int32_t*)(filter + 8);
    bool hasCapacity = (currentCount < maxCount);

    if (!hasCapacity) {
        return;
    }

    // Look up channel data from global array
    void** globalArray = *(void***)0x8272A324;
    void* channelData = *(void**)((uint8_t*)globalArray + channelIdx * 8 + 252);

    // Get sequence number via vtable slot 2 (mode=20)
    typedef void* (*GetSeqFunc)(void*, int32_t);
    void** channelVtable = *(void***)channelData;
    GetSeqFunc getSeq = (GetSeqFunc)channelVtable[2];
    void* seqResult = getSeq(channelData, 20);

    // Check minimum sequence threshold
    int32_t seqValue = *(int32_t*)seqResult;
    int32_t minSeq = *(int32_t*)(filter + 36);
    if (seqValue < minSeq) {
        return;
    }

    // Get sequence number via vtable slot 2 (mode=2) for max check
    getSeq = (GetSeqFunc)(*(void***)channelData)[2];
    seqResult = getSeq(channelData, 2);

    // Check maximum sequence threshold
    int32_t seqValue2 = *(int32_t*)seqResult;
    int32_t maxSeq = *(int32_t*)(filter + 40);
    if (seqValue2 > maxSeq) {
        return;
    }

    // Get entry data via filter's virtual function (vtable slot 0)
    typedef void* (*GetEntryFunc)(void*);
    void** filterVtable = *(void***)filter;
    GetEntryFunc getEntry = (GetEntryFunc)filterVtable[0];
    void* entry = getEntry(filter);

    // Try to add the entry
    if (atSingleton_7068_fw(manager, channelIdx, entry)) {
        // Success - increment count
        int32_t count = *(int32_t*)(filter + 4);
        *(int32_t*)(filter + 4) = count + 1;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// netFirewall_TryAddByFrameRange()  @ 0x82117E10 | size: 0xDC
//
// Attempts to add a network entry to a firewall filter with frame-based
// range check. Checks capacity, then validates the current frame count
// against the filter's minimum threshold before adding.
//
// Parameters:
//   r3 = firewall manager
//   r4 = channel index
//   r5 = filter descriptor
// ─────────────────────────────────────────────────────────────────────────────
void netFirewall_TryAddByFrameRange(uint8_t* manager, uint32_t channelIdx, uint8_t* filter) {
    extern bool atSingleton_7068_fw(uint8_t* manager, uint32_t channelIdx, void* entry);

    // Check if filter has capacity
    int32_t currentCount = *(int32_t*)(filter + 4);
    int32_t maxCount = *(int32_t*)(filter + 8);
    bool hasCapacity = (currentCount < maxCount);

    if (!hasCapacity) {
        return;
    }

    // Get frame counter from a global object
    void* frameCounter = *(void**)0x825FAB2C;
    int32_t sentFrames = *(int32_t*)((uint8_t*)frameCounter + 28);
    int32_t recvFrames = *(int32_t*)((uint8_t*)frameCounter + 32);
    int32_t totalFrames = sentFrames + recvFrames + 1;

    // Check minimum frame threshold
    int32_t minFrames = *(int32_t*)(filter + 36);
    if (totalFrames < minFrames) {
        return;
    }

    // Look up channel data and get sequence via vtable slot 2 (mode=13)
    void** globalArray = *(void***)0x8272A324;
    void* channelData = *(void**)((uint8_t*)globalArray + channelIdx * 8 + 252);
    typedef void* (*GetSeqFunc)(void*, int32_t);
    void** channelVtable = *(void***)channelData;
    GetSeqFunc getSeq = (GetSeqFunc)channelVtable[2];
    void* seqResult = getSeq(channelData, 13);

    // Check maximum sequence threshold
    int32_t seqValue = *(int32_t*)seqResult;
    int32_t maxSeq = *(int32_t*)(filter + 40);
    if (seqValue > maxSeq) {
        return;
    }

    // Get entry data via filter's virtual function (vtable slot 0)
    typedef void* (*GetEntryFunc)(void*);
    void** filterVtable = *(void***)filter;
    GetEntryFunc getEntry = (GetEntryFunc)filterVtable[0];
    void* entry = getEntry(filter);

    // Try to add the entry
    if (atSingleton_7068_fw(manager, channelIdx, entry)) {
        // Success - increment count
        int32_t count = *(int32_t*)(filter + 4);
        *(int32_t*)(filter + 4) = count + 1;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// grmShaderFx_FindOrRegisterByName()  @ 0x820F0468 | size: 0x110
//
// Searches a global shader effect registry (128 entries at 0x825EAF88)
// for a shader matching the given name. If found, returns its index.
// If not found, finds an empty slot, registers the shader, and returns
// the slot index. Returns -1 if the registry is full.
//
// Parameters:
//   r3 = shader name string
//
// Returns: index of shader in registry, or -1 if full
// ─────────────────────────────────────────────────────────────────────────────
int32_t grmShaderFx_FindOrRegisterByName(const char* name) {
    extern int _stricmp(const char* a, const char* b);
    extern void rage_FFE8(int32_t slotIndex, const char* name);
    extern void nop_8240E6D0(const char* msg);

    // Global shader registry: 128 entries (512 bytes / 4 bytes per pointer)
    void** registry = (void**)0x825EAF88;
    const uint32_t maxEntries = 128;

    // First pass: search for existing shader by name
    for (uint32_t i = 0; i < maxEntries; i++) {
        void* entry = registry[i];
        if (entry == nullptr) {
            continue;
        }

        // Compare entry's name (at offset +0) with target
        const char* entryName = *(const char**)entry;
        if (_stricmp(entryName, name) == 0) {
            return (int32_t)i;
        }
    }

    // Not found - check if allocator allows new registrations
    void** sdaPtr = *(void***)0x82600000;
    void* allocator = ((void**)sdaPtr)[1];
    typedef bool (*CanAllocFunc)(void*);
    void** allocVtable = *(void***)allocator;
    CanAllocFunc canAlloc = (CanAllocFunc)allocVtable[17];
    if (!canAlloc(allocator)) {
        // Cannot allocate - find first empty slot
        uint32_t emptySlot = 0;
        for (uint32_t i = 0; i < maxEntries; i++) {
            if (registry[i] == nullptr) {
                // Found empty slot - register the shader
                rage_FFE8(emptySlot, name);

                if (registry[emptySlot] != nullptr) {
                    // Registration succeeded
                    nop_8240E6D0((const char*)0x8203F288);
                    return (int32_t)emptySlot;
                }

                // Registration failed
                nop_8240E6D0((const char*)0x8203F2B4);
                return (int32_t)emptySlot;
            }
            emptySlot++;
        }
    }

    // Registry full
    return -1;
}

// ─────────────────────────────────────────────────────────────────────────────
// fxHairGroup_Destroy()  @ 0x820FFCA8 | size: 0x118
//
// Destructor for fxHairGroup. Sets vtable to fxHairGroup, iterates over
// the object array (at +36, count at +40), calling each element's
// destructor. Then frees the array memory if not tracked.
// Finally resets fields and restores vtable to rage::datBase.
//
// Parameters:
//   r3 = fxHairGroup object
// ─────────────────────────────────────────────────────────────────────────────
void fxHairGroup_Destroy(uint8_t* obj) {
    extern bool atSingleton_Find_90D0(void* ptr);

    // Set vtable to fxHairGroup
    *(void**)obj = (void*)0x82030040;

    // Destroy all elements in the array
    uint16_t count = *(uint16_t*)(obj + 40);
    if (count > 0) {
        void** elements = *(void***)(obj + 36);

        for (int32_t i = 0; i < count; i++) {
            void* element = elements[i];
            if (element != nullptr) {
                // Call destructor via vtable slot 0, deleteFlag=1
                typedef void (*DtorFunc)(void*, int32_t);
                void** vtable = *(void***)element;
                DtorFunc dtor = (DtorFunc)vtable[0];
                dtor(element, 1);
            }
        }
    }

    // Free the element array if it exists and is not tracked
    void* arrayPtr = *(void**)(obj + 36);
    if (arrayPtr != nullptr) {
        if (!atSingleton_Find_90D0(arrayPtr)) {
            // Not tracked - free via allocator (SDA vtable slot 2)
            void** sdaPtr = *(void***)0x82600000;
            void* allocator = ((void**)sdaPtr)[1];
            typedef void (*FreeFunc)(void*, void*);
            void** allocVtable = *(void***)allocator;
            FreeFunc freeFunc = (FreeFunc)allocVtable[2];
            freeFunc(allocator, arrayPtr);
        }
    }

    // Reset array fields
    *(void**)(obj + 36) = nullptr;
    *(uint16_t*)(obj + 40) = 0;
    *(uint16_t*)(obj + 42) = 0;

    // Restore vtable to rage::datBase
    *(void**)obj = (void*)0x820276C4;
}

// ─────────────────────────────────────────────────────────────────────────────
// gdGameData_Destructor()  @ 0x821C7900 | size: 0x48
// vtable slot 0 [destructor]
//
// Destructor for gdGameData singleton. Sets vtable to gdGameData's vtable,
// then conditionally frees the object memory if deleteFlag is set.
// ─────────────────────────────────────────────────────────────────────────────
void gdGameData_Destructor(atSingleton* obj, int deleteFlag) {
    // Set vtable to gdGameData vtable (0x8203CDF4)
    obj->vtable = (void*)0x8203CDF4;

    if (deleteFlag & 1) {
        rage_free(obj);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// gdGameData_LoadProperties()  @ 0x821C85D0 | size: 0xC
// vtable slot 2
//
// Loads game data properties by passing the global game data pointer
// to the property loader function (game_C0D8).
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t g_gdGameDataPtr;  // @ 0x8271A310

void gdGameData_LoadProperties(atSingleton* obj) {
    extern void game_C0D8(void* gameData);
    game_C0D8((void*)g_gdGameDataPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// gdGameData_ReplaceLoadedProperty()  @ 0x821E8430 | size: 0x7C
// vtable slot 3
//
// Replaces a loaded property at a given slot index (0-3).
// Each slot is 124 bytes. Destroys old property, looks up new property
// via atSingleton_D6E8_g using a property name table, then links
// the replacement into the property chain.
// ─────────────────────────────────────────────────────────────────────────────
extern void atSingleton_D6E8_g(void* param, const char* name);

void* gdGameData_ReplaceLoadedProperty(atSingleton* obj, void* registry, uint32_t slotIndex) {
    if (slotIndex > 3) {
        return nullptr;
    }

    // Calculate slot address: base + slotIndex * 124 + 8
    uint8_t* slotBase = (uint8_t*)obj + slotIndex * 124 + 8;

    // Destroy the existing property at this slot
    extern void datBase_DestroyVirtual(void* obj);
    datBase_DestroyVirtual(slotBase);

    // Look up replacement property by name
    // Property name table at 0x82041748: "tLoadProperties() - 'TotalNumTries' must be positive"
    void* result = (void*)slotBase;
    extern void* atSingleton_D6E8_g_ret(void* registry, const char* name);
    void* entry = atSingleton_D6E8_g_ret(registry, (const char*)0x82041748);

    // Link the replacement property into the chain
    uint32_t* entryNext = (uint32_t*)((uint8_t*)entry + 4);
    *entryNext = (uint32_t)(uintptr_t)slotBase;

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// gdGameData_ClearLoadedFlag()  @ 0x821F0400 | size: 0xC
// vtable slot 4
//
// Clears the loaded flag byte at offset +8 in the object.
// Called when game data needs to be reloaded or invalidated.
// ─────────────────────────────────────────────────────────────────────────────
void gdGameData_ClearLoadedFlag(atSingleton* obj) {
    *((uint8_t*)obj + 8) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// gdGameData_GetPropertyString()  @ 0x82229638 | size: 0xC
// vtable slot 15
//
// Returns a pointer to a constant string used for property identification.
// The string at 0x8204E918 is "t number of points specified".
// ─────────────────────────────────────────────────────────────────────────────
const char* gdGameData_GetPropertyString() {
    return (const char*)0x8204E918;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_CopyDataAndResolve()  @ 0x821257F0 | size: 0x84
//
// Copies 48 bytes of data from source into the destination object,
// then resolves a name reference via atSingleton_29E0_g and stores
// the result at offset +48. Clears fields at offsets +52, +56, +60.
// Used by fragDrawable to initialize draw data from a template.
// ─────────────────────────────────────────────────────────────────────────────
extern void* atSingleton_29E0_g(const void* key);

void datBase_CopyDataAndResolve(void* dest, void* nameSource, const void* srcData) {
    // Copy first 48 bytes (two 16-byte vectors + four 32-bit fields)
    memcpy(dest, srcData, 32);
    uint32_t* dst32 = (uint32_t*)((uint8_t*)dest + 32);
    const uint32_t* src32 = (const uint32_t*)((const uint8_t*)srcData + 32);
    dst32[0] = src32[0];
    dst32[1] = src32[1];
    dst32[2] = src32[2];
    dst32[3] = src32[3];

    // Resolve name from nameSource and store at offset +48
    const void* name = *(const void**)nameSource;
    void* resolved = atSingleton_29E0_g(name);

    uint32_t* destFields = (uint32_t*)((uint8_t*)dest + 48);
    destFields[0] = (uint32_t)(uintptr_t)resolved;
    destFields[1] = 0;
    destFields[2] = 0;
    destFields[3] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_CheckGlobalActiveFlag()  @ 0x820D9C18 | size: 0x54
//
// Checks whether a global system is in an active state by reading
// a chain of pointers from global data and testing bit flags.
// Returns true if bit 2 is set and bit 0 is clear in the status word.
//
// Pointer chain: g_systemPtr -> +20 -> +9736 -> +0 = status word
// Tests: if bit 0 is set, return false. If bit 2 is set, return true.
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t g_systemRootPtr;  // @ 0x8271A2F8

bool datBase_CheckGlobalActiveFlag() {
    uint32_t* root = (uint32_t*)g_systemRootPtr;
    uint32_t* level1 = (uint32_t*)(*(uint32_t*)((uint8_t*)root + 20));
    uint32_t* level2 = (uint32_t*)(*(uint32_t*)((uint8_t*)level1 + 9736));
    uint32_t statusWord = *level2;

    // If bit 0 is set, system is inactive
    if (statusWord & 0x1) {
        return false;
    }

    // If bit 2 is set, system is active
    if (statusWord & 0x4) {
        return true;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// phInst_FormatAndStoreName()  @ 0x82118D30 | size: 0x88
//
// Formats a name string from a physics instance and stores it
// at offset +444 in the object. Uses memset to clear a 64-byte
// buffer, reads name via vtable slot 1 on the source object's
// field at +4, then passes it through a global formatter at
// offset +908 from global pointer at SDA+25528.
// ─────────────────────────────────────────────────────────────────────────────
void phInst_FormatAndStoreName(uint8_t* obj, void* source) {
    // Clear 64-byte name buffer on stack
    char nameBuffer[64];
    nameBuffer[0] = '\0';
    memset(nameBuffer + 1, 0, 63);

    // Get name from source's field at +4 via vtable slot 1
    void* sourceField = *(void**)((uint8_t*)source + 4);
    typedef void (*GetNameFunc)(void*, char*, uint32_t);
    void** vtable = *(void***)sourceField;
    GetNameFunc getName = (GetNameFunc)vtable[1];
    getName(sourceField, nameBuffer, 64);

    // Format name through global formatter
    uint32_t* globalBase = *(uint32_t**)(0x826063B8);
    uint8_t* formatter = (uint8_t*)globalBase + 908;
    typedef uint8_t (*FormatFunc)(void*, const char*);
    FormatFunc format = (FormatFunc)(*(uint32_t**)formatter)[3];
    uint8_t result = format(formatter, nameBuffer);

    // Store formatted result at offset +444
    *(obj + 444) = result;
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyTreeNode()  @ 0x82131238 | size: 0x9C
//
// Recursively destroys a tree node with 8 children stored at
// offsets +28 through +56 (4 bytes each). For each non-null child,
// recursively calls itself. After children are cleaned up, checks
// if the node is tracked by the singleton system and either frees
// directly or unregisters through the allocator.
// ─────────────────────────────────────────────────────────────────────────────
extern bool atSingleton_Find_90D0(void* ptr);

void datBase_DestroyTreeNode(void* node, int deleteFlag) {
    uint8_t* nodePtr = (uint8_t*)node;

    // Recursively destroy 8 child nodes at offsets +28 to +56
    uint32_t* children = (uint32_t*)(nodePtr + 28);
    for (int i = 0; i < 8; i++) {
        void* child = (void*)children[i];
        if (child != nullptr) {
            datBase_DestroyTreeNode(child, 1);
        }
    }

    // Check if node has allocator tracking (field at +24)
    int32_t hasTracking = *(int32_t*)(nodePtr + 24);
    if (hasTracking == 0) {
        rage_free(node);
        return;
    }

    // Check data pointer at +16
    void* dataPtr = *(void**)(nodePtr + 16);
    if (dataPtr == nullptr) {
        rage_free(node);
        return;
    }

    // Check if data is registered in singleton system
    if (atSingleton_Find_90D0(dataPtr)) {
        rage_free(node);
        return;
    }

    // Unregister through allocator (SDA+0 -> +4 -> vtable slot 2)
    extern void* g_allocator_ptr;
    void** allocatorBase = *(void***)&g_allocator_ptr;
    void* allocator = *(void**)((uint8_t*)allocatorBase + 4);
    typedef void (*FreeFn)(void*, void*);
    void** allocVtable = *(void***)allocator;
    FreeFn freeFunc = (FreeFn)allocVtable[2];
    freeFunc(allocator, dataPtr);

    rage_free(node);
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyArray192()  @ 0x8214BE18 | size: 0xCC
//
// Destroys an array of 192-byte elements with singleton tracking.
// For each element, checks if it is tracked by the singleton system
// before freeing through the allocator. Also frees secondary
// arrays stored at offsets +4 and +8 in the container.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_DestroyArray192(uint8_t* container) {
    // Get array pointer from container offset +0
    void* arrayPtr = *(void**)container;
    if (arrayPtr != nullptr) {
        uint8_t* arrayBase = (uint8_t*)arrayPtr - 16;
        uint32_t elementCount = *(uint32_t*)arrayBase;

        // Calculate stride: count * 3 * 64 = count * 192
        uint32_t totalSize = elementCount * 192;
        uint8_t* currentElement = (uint8_t*)arrayPtr + totalSize;

        // Destroy each element in reverse order
        for (int32_t i = elementCount - 1; i >= 0; i--) {
            currentElement -= 192;
            void* element = *(void**)currentElement;

            if (element != nullptr) {
                // Check if element is tracked by singleton system
                if (!atSingleton_Find_90D0(element)) {
                    // Unregister through allocator
                    extern void* g_allocator_ptr;
                    void** allocatorBase = *(void***)&g_allocator_ptr;
                    void* allocator = *(void**)((uint8_t*)allocatorBase + 4);
                    typedef void (*FreeFn)(void*, void*);
                    void** allocVtable = *(void***)allocator;
                    FreeFn freeFunc = (FreeFn)allocVtable[2];
                    freeFunc(allocator, element);
                }
            }
        }

        // Free the array memory
        rage_free(arrayBase);
    }

    // Free secondary buffer at +4
    void* buffer1 = *(void**)(container + 4);
    if (buffer1 != nullptr) {
        rage_free(buffer1);
        *(void**)(container + 4) = nullptr;
    }

    // Free secondary buffer at +8
    void* buffer2 = *(void**)(container + 8);
    if (buffer2 != nullptr) {
        rage_free(buffer2);
        *(void**)(container + 8) = nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// datBase_DestroyHashMap()  @ 0x82156160 | size: 0xC4
//
// Destroys a hash map structure with 8-byte entries. Each entry
// contains a pointer that may be tracked by the singleton system.
// After clearing all entries, frees the backing array through
// net_5B40 (a memory pool free function), then clears the container.
// ─────────────────────────────────────────────────────────────────────────────
void datBase_DestroyHashMap(uint8_t* hashMap) {
    int16_t entryCount = *(int16_t*)(hashMap + 4);

    if (entryCount > 0) {
        int32_t offset = 0;
        for (int32_t i = 0; i < entryCount; i++) {
            void* arrayBase = *(void**)hashMap;
            uint8_t* entry = (uint8_t*)arrayBase + offset;

            void* element = *(void**)entry;
            if (element != nullptr) {
                // Check if element is tracked
                if (!atSingleton_Find_90D0(element)) {
                    // Unregister through allocator
                    extern void* g_allocator_ptr;
                    void** allocatorBase = *(void***)&g_allocator_ptr;
                    void* allocator = *(void**)((uint8_t*)allocatorBase + 4);
                    typedef void (*FreeFn)(void*, void*);
                    void** allocVtable = *(void***)allocator;
                    FreeFn freeFunc = (FreeFn)allocVtable[2];
                    freeFunc(allocator, element);
                }
            }

            // Clear entry fields
            *(uint32_t*)entry = 0;
            *(uint16_t*)(entry + 4) = 0;
            *(uint16_t*)(entry + 6) = 0;

            offset += 8;
        }
    }

    // Free backing array through net_5B40
    void* arrayPtr = *(void**)hashMap;
    if (arrayPtr != nullptr) {
        extern void net_5B40(void* ptr, int mode);
        net_5B40(arrayPtr, 3);
    }

    // Clear the container
    *(uint32_t*)hashMap = 0;
    *(uint16_t*)(hashMap + 4) = 0;
    *(uint16_t*)(hashMap + 6) = 0;

    rage_free(hashMap);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::Destructor()  @ 0x821C7900 | size: 0x48
//
// Virtual destructor (vtable slot 0) for atSingleton base class.
// Sets the vtable pointer, then conditionally frees the object memory
// based on the delete flag (r4 & 1).
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_Destructor(atSingleton* obj, uint32_t deleteFlag) {
    // Set vtable to atSingleton base vtable
    obj->vtable = (void*)0x820274B4;

    // If deleteFlag bit 0 is set, free the object memory
    if (deleteFlag & 1) {
        rage_free(obj);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::CountLinkedResources()  @ 0x821C85D0 | size: 0xC
//
// Virtual method (vtable slot 2). Loads a global data pointer and
// passes it to game_C0D8 for linked resource enumeration.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_CountLinkedResources(atSingleton* obj) {
    extern uint32_t g_resourceRegistryPtr;  // @ 0x8271A310
    void* registry = (void*)g_resourceRegistryPtr;
    extern void game_C0D8(void* registry);
    game_C0D8(registry);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ClearDirtyFlag()  @ 0x821F0400 | size: 0xC
//
// Virtual method (vtable slot 4). Clears the byte at offset +8,
// resetting a dirty/modified state flag to false.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_ClearDirtyFlag(atSingleton* obj) {
    *(uint8_t*)((uint8_t*)obj + 8) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::GetTypeInfo()  @ 0x82229638 | size: 0xC
//
// Virtual method (vtable slot 15). Returns a pointer to the RTTI/type
// information structure for this atSingleton specialization.
// ─────────────────────────────────────────────────────────────────────────────
void* atSingleton_GetTypeInfo() {
    return (void*)0x82029718;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::GetDictionaryEntryCount()  @ 0x821E7170 | size: 0xC
//
// Virtual method override. Reads a container pointer at offset +316,
// then returns the uint16 count at offset +20 of that container.
// ─────────────────────────────────────────────────────────────────────────────
uint16_t atSingleton_GetDictionaryEntryCount(atSingleton* obj) {
    uint8_t* container = *(uint8_t**)((uint8_t*)obj + 316);
    return *(uint16_t*)(container + 20);
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::DestructorThunk()  @ 0x821CAD48 | size: 0x8
//
// Destructor thunk that adjusts the this pointer by -4 bytes and
// tail-calls atSingleton_vfn_0_AA28_1 (a base-class destructor).
// Used for multiple-inheritance this-pointer adjustment.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_DestructorThunk(atSingleton* obj) {
    extern void atSingleton_vfn_0_AA28_1(atSingleton* obj);
    atSingleton_vfn_0_AA28_1((atSingleton*)((uint8_t*)obj - 4));
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::ReplaceSlotEntry()  @ 0x821E8430 | size: 0x7C
//
// Virtual method (vtable slot 3). Replaces an entry in one of 4 indexed
// slots (0-3). Each slot is 124 bytes. The method:
//   1. Validates slot index <= 3
//   2. Calls the destructor on the existing slot contents
//   3. Registers the new entry via atSingleton_D6E8_g
//   4. Links the new entry's back-pointer to the slot
//   Always returns 0.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t atSingleton_ReplaceSlotEntry(atSingleton* obj, void* nameBuffer, uint32_t slotIndex) {
    if (slotIndex > 3) {
        return 0;
    }

    // Calculate slot address: base + slotIndex * 124 + 8
    uint8_t* slotBase = (uint8_t*)obj + slotIndex * 124 + 8;

    // Destroy existing slot contents via vtable slot 0
    typedef void (*DtorFunc)(void*);
    void** vtable = *(void***)slotBase;
    DtorFunc dtor = (DtorFunc)vtable[0];
    dtor(slotBase);

    // Register the new entry with the singleton name registry
    extern void* atSingleton_D6E8_g(void* nameBuffer, uint8_t* slotBase, void* param);
    void* entry = atSingleton_D6E8_g(nameBuffer, slotBase, (void*)0x82021748);

    // Link back-pointer: entry->field4 = slotBase
    *(uint8_t**)(*(uint8_t**)((uint8_t*)entry + 4)) = slotBase;

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::IsSimulationActive()  @ 0x820D9C18 | size: 0x54
//
// Checks the physics simulation state by traversing a global chain:
//   globalPtr -> +20 -> +9736 -> +0 (state flags)
// Returns true (1) if bit 2 of the state flags is set and bit 0 is clear,
// indicating an active simulation. Returns false (0) otherwise.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t atSingleton_IsSimulationActive() {
    extern uint32_t g_simManagerPtr;  // @ 0x8271A2F8
    uint32_t* simManager = (uint32_t*)g_simManagerPtr;
    uint32_t* level1 = (uint32_t*)simManager[5];       // +20
    uint32_t* level2 = (uint32_t*)level1[2434];        // +9736
    uint32_t stateFlags = level2[0];

    // If bit 0 is set, simulation is not active
    if (stateFlags & 1) {
        return 0;
    }

    // Check if bit 2 is set (simulation running)
    if (stateFlags & 4) {
        return 1;
    }

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::InitShapeFromSource()  @ 0x821257F0 | size: 0x84
//
// Initializes an object by copying 48 bytes of shape/transform data from
// a source buffer (two 16-byte vectors + 4 uint32 fields), then hashes
// a name key via atSingleton_29E0_g and stores it at +48, clearing the
// remaining 3 fields (+52, +56, +60) to zero.
//
// Layout at dest:
//   +0x00  vec4 (16 bytes)
//   +0x10  vec4 (16 bytes)
//   +0x20  uint32 field[4] (16 bytes)
//   +0x30  nameHash (from atSingleton_29E0_g)
//   +0x34  0
//   +0x38  0
//   +0x3C  0
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_InitShapeFromSource(uint8_t* dest, const void* nameKey, const uint8_t* src) {
    // Copy first 16-byte vector
    *(uint32_t*)(dest + 0)  = *(uint32_t*)(src + 0);
    *(uint32_t*)(dest + 4)  = *(uint32_t*)(src + 4);
    *(uint32_t*)(dest + 8)  = *(uint32_t*)(src + 8);
    *(uint32_t*)(dest + 12) = *(uint32_t*)(src + 12);

    // Copy second 16-byte vector
    *(uint32_t*)(dest + 16) = *(uint32_t*)(src + 16);
    *(uint32_t*)(dest + 20) = *(uint32_t*)(src + 20);
    *(uint32_t*)(dest + 24) = *(uint32_t*)(src + 24);
    *(uint32_t*)(dest + 28) = *(uint32_t*)(src + 28);

    // Copy 4 scalar fields
    *(uint32_t*)(dest + 32) = *(uint32_t*)(src + 32);
    *(uint32_t*)(dest + 36) = *(uint32_t*)(src + 36);
    *(uint32_t*)(dest + 40) = *(uint32_t*)(src + 40);
    *(uint32_t*)(dest + 44) = *(uint32_t*)(src + 44);

    // Hash the name key and store result
    extern void* atSingleton_29E0_g(const void* key);
    void* nameHash = atSingleton_29E0_g(*(void**)nameKey);
    *(void**)(dest + 48) = nameHash;

    // Clear remaining fields
    *(uint32_t*)(dest + 52) = 0;
    *(uint32_t*)(dest + 56) = 0;
    *(uint32_t*)(dest + 60) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton::LookupAndStorePropertyByte()  @ 0x82118D30 | size: 0x88
//
// Looks up a property value by name from a session/context object,
// then stores the result as a byte at offset +444 of the target object.
//
// Steps:
//   1. Zero-initialize a 64-byte name buffer on the stack
//   2. Call vcall slot 1 on the context's field +4 to format the name
//   3. Call a global property lookup function with the formatted name
//   4. Store the returned byte at offset +444 (0x1BC) of the target
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_LookupAndStorePropertyByte(uint8_t* obj, void* context) {
    // Initialize 64-byte name buffer
    char nameBuffer[64];
    nameBuffer[0] = 0;
    memset(nameBuffer + 1, 0, 63);

    // Get the context's string provider at +4 and call vcall slot 1
    // to format the property name into the buffer
    void* stringProvider = *(void**)((uint8_t*)context + 4);
    typedef void (*FormatFunc)(void*, char*, uint32_t);
    void** providerVtable = *(void***)stringProvider;
    FormatFunc formatName = (FormatFunc)providerVtable[1];
    formatName(stringProvider, nameBuffer, 64);

    // Look up the property value using a global registry
    // The registry is at SDA offset +25528, property table at +908
    extern uint32_t g_propertyRegistryPtr;  // SDA-relative
    uint8_t* registry = (uint8_t*)g_propertyRegistryPtr;
    uint8_t* propertyTable = registry + 908;
    typedef uint8_t (*LookupFunc)(uint8_t*, char*);
    LookupFunc lookup = (LookupFunc)(*(void**)((uint8_t*)propertyTable + 12));
    uint8_t result = lookup(propertyTable, nameBuffer);

    // Store the result byte at offset +444
    *(uint8_t*)(obj + 444) = result;
}
