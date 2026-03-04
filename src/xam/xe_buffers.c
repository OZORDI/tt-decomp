/**
 * xe_buffers.c — Xbox Executive Dynamic Buffer Management
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Manages dynamic arrays and buffers used throughout the game engine.
 * These functions handle allocation, resizing, and copying of buffer structures.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Forward declarations */
extern void* xe_EC88(uint32_t size);  /* Memory allocator @ 0x820DEC88 */

/**
 * Dynamic buffer structure
 * 
 * Common structure used for managing dynamically-sized arrays:
 * +0x00: void* data       - Pointer to allocated buffer
 * +0x04: uint16_t count   - Current number of elements
 * +0x06: uint16_t capacity - Maximum number of elements
 */
typedef struct XeBuffer {
    void*    data;      /* +0x00 */
    uint16_t count;     /* +0x04 */
    uint16_t capacity;  /* +0x06 */
} XeBuffer;

// ─────────────────────────────────────────────────────────────────────────────
// xe_8E30()  @ 0x820C8E30 | size: 0x68
// Allocates buffer for uint32_t array (4 bytes per element)
// ─────────────────────────────────────────────────────────────────────────────
void xe_8E30(XeBuffer* buffer, uint16_t capacity) {
    void* data = NULL;
    
    if (capacity != 0) {
        /* Check for overflow: capacity > 0x3FFFFFFF (max safe count for 4-byte elements) */
        if (capacity > 0x3FFFFFFF) {
            /* Overflow - allocate maximum size */
            data = xe_EC88(0xFFFFFFFF);
        } else {
            /* Allocate capacity * 4 bytes */
            uint32_t size = capacity * 4;
            data = xe_EC88(size);
        }
    }
    
    buffer->data = data;
    buffer->capacity = capacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_0DA0()  @ 0x820F0DA0 | size: 0x60
// Initializes buffer with capacity of 1 element (8 bytes)
// ─────────────────────────────────────────────────────────────────────────────
void xe_0DA0(XeBuffer* buffer) {
    if (buffer->capacity == 0) {
        /* First-time initialization: allocate 8 bytes for 1 element */
        buffer->capacity = 1;
        buffer->data = xe_EC88(8);
        buffer->count = 1;
    } else {
        /* Already initialized: just set count to 1 */
        buffer->count = 1;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_FF00()  @ 0x820FFF00 | size: 0x78
// Allocates buffer for uint16_t array (2 bytes per element)
// ─────────────────────────────────────────────────────────────────────────────
void xe_FF00(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            /* Check for overflow: capacity > 0x7FFFFFFF (max safe count for 2-byte elements) */
            if (newCapacity > 0x7FFFFFFF) {
                /* Overflow - allocate maximum size */
                buffer->data = xe_EC88(0xFFFFFFFF);
            } else {
                /* Allocate capacity * 2 bytes */
                uint32_t size = newCapacity * 2;
                buffer->data = xe_EC88(size);
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_CBD8()  @ 0x8213CBD8 | size: 0x78
// Allocates buffer for 64-byte elements (capacity * 64)
// ─────────────────────────────────────────────────────────────────────────────
void xe_CBD8(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            /* Check for overflow: capacity > 0x03FFFFFF (max safe count for 64-byte elements) */
            if (newCapacity > 0x03FFFFFF) {
                /* Overflow - allocate maximum size */
                buffer->data = xe_EC88(0xFFFFFFFF);
            } else {
                /* Allocate capacity * 64 bytes */
                uint32_t size = newCapacity * 64;
                buffer->data = xe_EC88(size);
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_C320()  @ 0x8214C320 | size: 0x68
// Initializes a complex structure with multiple nested buffers
// ─────────────────────────────────────────────────────────────────────────────
extern void xe_C530(void* structure, uint32_t param);  /* @ 0x8214C530 */

void xe_C320(void* structure) {
    /* Zero-initialize the structure (40 bytes) */
    uint32_t* words = (uint32_t*)structure;
    for (int i = 0; i < 10; i++) {
        words[i] = 0;
    }
    
    /* Call initialization helper */
    xe_C530(structure, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_4408()  @ 0x82134408 | size: 0x84
// Allocates aligned memory and initializes a structure with pointer and counter
// ─────────────────────────────────────────────────────────────────────────────
extern void xe_main_thread_init_0038(void);  /* @ 0x820C0038 */
extern uint32_t* g_sda_base;  /* SDA base pointer (r13 register) */

typedef void* (*AllocatorVCall)(void* allocator, uint32_t size, uint32_t alignment);

void xe_4408(void* structure, uint32_t elementCount) {
    uint32_t allocSize;
    
    /* Calculate allocation size: elementCount * 4 bytes */
    /* Check for overflow: elementCount > 0x3FFFFFFF */
    if (elementCount > 0x3FFFFFFF) {
        allocSize = 0xFFFFFFFF;  /* Overflow - use max size */
    } else {
        allocSize = elementCount * 4;
    }
    
    /* Ensure main thread heap is initialized */
    xe_main_thread_init_0038();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(size, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* data = allocFunc(allocator, allocSize, 16);
    
    /* Initialize structure: pointer at +0, zero at +4 */
    uint32_t* words = (uint32_t*)structure;
    words[0] = (uint32_t)(uintptr_t)data;
    words[1] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_62D0()  @ 0x821362D0 | size: 0x5C
// Allocates 24 bytes and stores pointer with capacity of 6
// ─────────────────────────────────────────────────────────────────────────────
void xe_62D0(XeBuffer* buffer) {
    /* Ensure main thread heap is initialized */
    xe_main_thread_init_0038();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(24, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* data = allocFunc(allocator, 24, 16);
    
    buffer->data = data;
    buffer->capacity = 6;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_7E78()  @ 0x82247E78 | size: 0xB0
// Copies vector data from source to destination buffer
// ─────────────────────────────────────────────────────────────────────────────
void xe_7E78(XeBuffer* dest, const XeBuffer* src) {
    uint16_t capacity = src->capacity;
    
    /* Copy capacity to both count and capacity fields */
    dest->capacity = capacity;
    dest->count = capacity;
    
    if (capacity != 0) {
        /* Allocate destination buffer: capacity * 16 bytes per vector */
        /* Check for overflow: capacity > 0x0FFFFFFF */
        uint32_t allocSize;
        if (capacity > 0x0FFFFFFF) {
            allocSize = 0xFFFFFFFF;  /* Overflow */
        } else {
            allocSize = capacity * 16;
        }
        
        dest->data = xe_EC88(allocSize);
    } else {
        dest->data = NULL;
    }
    
    /* Copy vector data if capacity > 0 */
    if (dest->capacity != 0 && dest->data != NULL && src->data != NULL) {
        /* Copy 16-byte vectors using memcpy */
        uint8_t* srcBytes = (uint8_t*)src->data;
        uint8_t* destBytes = (uint8_t*)dest->data;
        
        for (uint16_t i = 0; i < capacity; i++) {
            memcpy(destBytes + (i * 16), srcBytes + (i * 16), 16);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_0048()  @ 0x82160048 | size: 0x60
// Initializes audio control manager structure
// ─────────────────────────────────────────────────────────────────────────────
extern void xe_A750(uint32_t size, uint32_t alignment);  /* @ 0x821AA750 */

void xe_0048(void* structure, uint8_t enableFlag) {
    uint32_t* words = (uint32_t*)structure;
    
    /* Store vtable pointer (calculated address) */
    /* lis r11,-32253 + addi r11,r11,21872 = 0x82163570 */
    words[0] = 0x82163570;
    
    /* Store enable flag at +4 */
    uint8_t* bytes = (uint8_t*)structure;
    bytes[4] = enableFlag;
    
    /* If enabled, allocate 240 bytes with 16-byte alignment */
    if (enableFlag != 0) {
        xe_A750(240, 16);
    }
    
    /* Clear global state variable @ 0x824063F4 */
    uint32_t* globalState = (uint32_t*)0x824063F4;
    *globalState = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_D5F8()  @ 0x821CD5F8 | size: 0x80
// Updates game state based on parameter offset calculations
// ─────────────────────────────────────────────────────────────────────────────
extern void pg_D678_gen(void* structure, int32_t value1, void* ptr, int32_t value2);

void xe_D5F8(void* structure, int32_t param) {
    /* Calculate offset: (param + 0x20000 - 23877) * 4 */
    int32_t offset1 = ((param + 0x20000) - 23877) * 4;
    int32_t* structWords = (int32_t*)((uint8_t*)structure + offset1);
    int32_t value1 = *structWords;
    
    if (value1 > 0) {
        /* Calculate second offset: (param + 0x20000 - 23875) * 4 */
        int32_t offset2 = ((param + 0x20000) - 23875) * 4;
        void* ptr = (void*)((uint8_t*)structure + offset2);
        
        /* Call helper function */
        pg_D678_gen(structure, value1, ptr, 1);
    }
    
    /* Load global pointer and copy float value */
    uint32_t* globalPtr = *(uint32_t**)0x82412968;
    float floatValue = *(float*)((uint8_t*)globalPtr + 48);
    
    /* Calculate destination offset: (param + 0x20000 - 23755) * 4 */
    int32_t offset3 = ((param + 0x20000) - 23755) * 4;
    float* destFloat = (float*)((uint8_t*)structure + offset3);
    *destFloat = floatValue;
}
