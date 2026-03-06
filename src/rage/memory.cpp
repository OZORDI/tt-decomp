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

