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
extern void* rage_alloc(uint32_t size);  /* Memory allocator @ 0x820DEC88 */

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
            data = rage_alloc(0xFFFFFFFF);
        } else {
            /* Allocate capacity * 4 bytes */
            uint32_t size = capacity * 4;
            data = rage_alloc(size);
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
        buffer->data = rage_alloc(8);
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
                buffer->data = rage_alloc(0xFFFFFFFF);
            } else {
                /* Allocate capacity * 2 bytes */
                uint32_t size = newCapacity * 2;
                buffer->data = rage_alloc(size);
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
                buffer->data = rage_alloc(0xFFFFFFFF);
            } else {
                /* Allocate capacity * 64 bytes */
                uint32_t size = newCapacity * 64;
                buffer->data = rage_alloc(size);
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
extern void sysMemAllocator_InitMainThread(void);  /* @ 0x820C0038 */
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
    sysMemAllocator_InitMainThread();
    
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
    sysMemAllocator_InitMainThread();
    
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
        
        dest->data = rage_alloc(allocSize);
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

// ─────────────────────────────────────────────────────────────────────────────
// xe_B958()  @ 0x820EB958 | size: 0x8C
// Allocates and initializes a 24-byte pcrEmoteData structure
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t* g_float_one_ptr;  /* @ 0x8202D110 */

void* xe_B958(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(24, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 24, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Store vtable pointer for pcrEmoteData @ 0x8202EB64 */
        words[0] = 0x8202EB64;
        
        /* Initialize fields */
        words[1] = 0;  /* +0x04 */
        words[2] = 0;  /* +0x08 */
        words[3] = 0;  /* +0x0C */
        words[4] = 0;  /* +0x10 */
        
        /* Load float value from global (likely 1.0f) */
        float* floatPtr = (float*)g_float_one_ptr;
        float* objFloat = (float*)&words[5];
        *objFloat = *floatPtr;  /* +0x14 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_D100()  @ 0x820FD100 | size: 0x9C
// Copies buffer data from source to destination (4-byte elements)
// ─────────────────────────────────────────────────────────────────────────────
void xe_D100(XeBuffer* dest, const XeBuffer* src) {
    uint16_t capacity = src->count;
    
    /* Copy capacity to both fields */
    dest->count = capacity;
    dest->capacity = capacity;
    
    /* Allocate destination buffer if capacity > 0 */
    if (capacity != 0) {
        uint32_t allocSize = capacity * 4;
        dest->data = rage_alloc(allocSize);
    } else {
        dest->data = NULL;
    }
    
    /* Copy data if both buffers are valid */
    if (dest->count != 0 && dest->data != NULL && src->data != NULL) {
        uint32_t* srcData = (uint32_t*)src->data;
        uint32_t* destData = (uint32_t*)dest->data;
        
        for (uint16_t i = 0; i < capacity; i++) {
            destData[i] = srcData[i];
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_1E48()  @ 0x82101E48 | size: 0xA0
// Allocates and initializes a 32-byte game logic structure
// ─────────────────────────────────────────────────────────────────────────────
void* xe_1E48(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(32, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 32, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Calculate addresses for initialization */
        /* lis r11,-32254 + addi r10,r11,31540 = 0x82017B34 */
        uint32_t addr1 = 0x82017B34;
        /* lis r11,-32253 + addi r9,r11,340 = 0x82020154 */
        uint32_t addr2 = 0x82020154;
        /* lis r11,-32253 + addi r8,r11,476 = 0x820201DC */
        uint32_t addr3 = 0x820201DC;
        
        /* Initialize structure fields */
        words[0] = addr2;  /* vtable or function pointer @ +0x00 */
        words[1] = 0;      /* +0x04 */
        words[2] = 0;      /* +0x08 */
        words[3] = 0;      /* +0x0C */
        words[4] = addr3;  /* overwrite addr1 with addr3 @ +0x10 */
        words[5] = 0;      /* +0x14 */
        words[6] = 0;      /* +0x18 */
        words[7] = 0;      /* +0x1C */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_3508()  @ 0x820F3508 | size: 0xBC
// Allocates buffer with capacity check and zero-initialization
// ─────────────────────────────────────────────────────────────────────────────
void xe_3508(XeBuffer* buffer, uint16_t capacity) {
    void* data = NULL;
    
    if (capacity != 0) {
        /* Check for overflow: capacity > 0x3FFFFFFF */
        uint32_t allocSize;
        if (capacity > 0x3FFFFFFF) {
            allocSize = 0xFFFFFFFF;  /* Overflow - use max size */
        } else {
            allocSize = capacity * 4;
            /* Additional safety check: ensure allocSize + 4 doesn't overflow */
            if (allocSize > 0xFFFFFFFB) {
                allocSize = 0xFFFFFFFF;
            } else {
                allocSize += 4;  /* Add 4 bytes for header */
            }
        }
        
        data = rage_alloc(allocSize);
        
        if (data != NULL) {
            uint32_t* header = (uint32_t*)data;
            uint32_t* elements = header + 1;  /* Skip 4-byte header */
            
            /* Store capacity in header */
            *header = capacity;
            
            /* Zero-initialize all elements */
            for (uint16_t i = 0; i < capacity; i++) {
                elements[i] = 0;
            }
            
            /* Return pointer to elements (after header) */
            data = elements;
        }
    }
    
    buffer->data = data;
    buffer->capacity = capacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_73E0()  @ 0x820E73E0 | size: 0xD8
// Allocates buffer for 16-byte elements with float initialization
// ─────────────────────────────────────────────────────────────────────────────
extern float g_float_zero;  /* @ 0x8202D108 */

void xe_73E0(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            /* Check for overflow: capacity > 0x0FFFFFFF (max for 16-byte elements) */
            uint32_t allocSize;
            if (newCapacity > 0x0FFFFFFF) {
                allocSize = 0xFFFFFFFF;  /* Overflow */
            } else {
                allocSize = newCapacity * 16;
                /* Safety check for allocSize + 4 */
                if (allocSize > 0xFFFFFFFB) {
                    allocSize = 0xFFFFFFFF;
                } else {
                    allocSize += 4;  /* Add header */
                }
            }
            
            void* data = rage_alloc(allocSize);
            
            if (data != NULL) {
                uint32_t* header = (uint32_t*)data;
                uint8_t* elements = (uint8_t*)(header + 1);
                
                /* Store capacity in header */
                *header = newCapacity;
                
                /* Initialize each 16-byte element */
                for (uint16_t i = 0; i < newCapacity; i++) {
                    uint32_t* elem = (uint32_t*)(elements + (i * 16));
                    float* floatElem = (float*)(elements + (i * 16));
                    
                    elem[0] = 0;  /* +0x00 */
                    elem[1] = 0;  /* +0x04 */
                    elem[2] = 0;  /* +0x08 */
                    floatElem[3] = g_float_zero;  /* +0x0C - float value */
                }
                
                buffer->data = elements;
            } else {
                buffer->data = NULL;
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_D6F8()  @ 0x820FD6F8 | size: 0xFC
// Allocates large buffer (100KB+) for game state management
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t* g_large_buffer_ptr;  /* @ 0x8260637C */

void* xe_D6F8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Allocate 100KB + 32 bytes (0x1 << 16 | 35296 = 101,920 bytes) */
    uint32_t allocSize = 101920;
    
    /* Get allocator vtable and call Allocate(size, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* buffer = allocFunc(allocator, allocSize, 16);
    
    if (buffer != NULL) {
        uint32_t* words = (uint32_t*)buffer;
        
        /* Zero-initialize first 12 bytes */
        words[0] = 0;  /* +0x00 */
        words[1] = 0;  /* +0x04 */
        words[2] = 0;  /* +0x08 */
        words[38] = 0; /* +0x98 (152) */
        
        /* Zero-initialize 16 words starting at offset 156 */
        for (int i = 0; i < 16; i++) {
            words[39 + i] = 0;  /* +0x9C to +0xDB */
        }
        
        /* Initialize flags at specific offsets */
        uint8_t* bytes = (uint8_t*)buffer;
        bytes[0x10100] = 0;  /* +100,256 */
        bytes[0x10101] = 0;  /* +100,257 */
        
        /* Check global flag to determine initialization */
        extern uint32_t* g_system_flags;  /* @ 0x821DA170 */
        uint32_t flagValue = g_system_flags[1];
        
        if (flagValue != 0) {
            bytes[144] = 0;  /* +0x90 */
        } else {
            bytes[144] = 1;  /* +0x90 */
        }
        
        /* Set remaining flags */
        bytes[145] = 1;  /* +0x91 */
        bytes[146] = 1;  /* +0x92 */
        bytes[147] = 1;  /* +0x93 */
        bytes[148] = 1;  /* +0x94 */
        
        /* Store buffer pointer in global */
        g_large_buffer_ptr = (uint32_t*)buffer;
        
        return buffer;
    }
    
    /* Clear global pointer on failure */
    g_large_buffer_ptr = NULL;
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_0780()  @ 0x820F0780 | size: 0x108
// Allocates buffer for 56-byte elements with complex initialization
// ─────────────────────────────────────────────────────────────────────────────
extern float g_float_values[3];  /* @ 0x8202D1A8, 0x8202D1A0, 0x8202D238 */

void xe_0780(XeBuffer* buffer, uint16_t capacity) {
    void* data = NULL;
    
    if (capacity != 0) {
        /* Check for overflow: capacity > 0x049249 (max for 56-byte elements) */
        uint32_t allocSize;
        if (capacity > 0x049249) {
            allocSize = 0xFFFFFFFF;  /* Overflow */
        } else {
            allocSize = capacity * 56;
            if (allocSize > 0xFFFFFFFB) {
                allocSize = 0xFFFFFFFF;
            } else {
                allocSize += 4;  /* Add header */
            }
        }
        
        data = rage_alloc(allocSize);
        
        if (data != NULL) {
            uint32_t* header = (uint32_t*)data;
            uint8_t* elements = (uint8_t*)(header + 1);
            
            /* Store capacity in header */
            *header = capacity;
            
            /* Initialize each 56-byte element */
            for (uint16_t i = 0; i < capacity; i++) {
                uint32_t* elem = (uint32_t*)(elements + (i * 56));
                float* floatElem = (float*)(elements + (i * 56));
                
                /* Zero-initialize first 32 bytes */
                elem[0] = 0;  /* +0x00 */
                elem[1] = 0;  /* +0x04 */
                elem[2] = 1;  /* +0x08 - set to 1 */
                elem[3] = 0x8202F5A8;  /* +0x0C - pointer/address */
                elem[4] = 0;  /* +0x10 */
                elem[5] = 0;  /* +0x14 */
                elem[6] = 0;  /* +0x18 */
                elem[7] = 0;  /* +0x1C */
                elem[8] = 0;  /* +0x20 */
                
                /* Initialize float values */
                floatElem[9] = g_float_values[0];   /* +0x24 */
                floatElem[10] = g_float_values[1];  /* +0x28 */
                floatElem[11] = g_float_values[2];  /* +0x2C */
                
                /* Final fields */
                elem[12] = 1;  /* +0x30 - set to 1 */
                elem[13] = 0;  /* +0x34 */
            }
            
            data = elements;
        }
    }
    
    buffer->data = data;
    buffer->capacity = capacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_3CA8()  @ 0x82213CA8 | size: 0x7C
// Allocates and initializes a 16-byte gdUnlockConditionMultiBounce object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_3CA8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(16, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 16, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Initialize gdUnlockConditionMultiBounce object */
        words[0] = 0x8204B244;  /* vtable for gdUnlockConditionMultiBounce */
        words[1] = 0;           /* +0x04 */
        words[2] = 0;           /* +0x08 */
        words[3] = 0;           /* +0x0C */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_75F8()  @ 0x823475F8 | size: 0x90
// Allocates and initializes a 28-byte gdCSActionCharVisibleData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_75F8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(28, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 28, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize gdCSActionCharVisibleData object */
        words[0] = 0x82077D1C;  /* vtable for gdCSActionCharVisibleData */
        words[1] = 0;           /* +0x04 */
        words[2] = 0;           /* +0x08 */
        words[3] = 0;           /* +0x0C */
        bytes[16] = 1;          /* +0x10 - enable flag */
        words[5] = 0xFFFFFFFF;  /* +0x14 - -1 */
        bytes[24] = 1;          /* +0x18 - enable flag */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5A70()  @ 0x82115A70 | size: 0xCC
// Allocates buffer for 8-byte elements with header
// ─────────────────────────────────────────────────────────────────────────────
void xe_5A70(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        buffer->count = newCapacity;
        
        if (newCapacity != 0) {
            /* Check for overflow: capacity > 0x1FFFFFFF (max for 8-byte elements) */
            uint32_t allocSize;
            if (newCapacity > 0x1FFFFFFF) {
                allocSize = 0xFFFFFFFF;  /* Overflow */
            } else {
                allocSize = newCapacity * 8;
                /* Safety check for allocSize + 4 */
                if (allocSize > 0xFFFFFFFB) {
                    allocSize = 0xFFFFFFFF;
                } else {
                    allocSize += 4;  /* Add header */
                }
            }
            
            void* data = rage_alloc(allocSize);
            
            if (data != NULL) {
                uint32_t* header = (uint32_t*)data;
                uint8_t* elements = (uint8_t*)(header + 1);
                
                /* Store capacity in header */
                *header = newCapacity;
                
                /* Zero-initialize each 8-byte element */
                for (uint16_t i = 0; i < newCapacity; i++) {
                    uint32_t* elem = (uint32_t*)(elements + (i * 8));
                    uint16_t* halfwords = (uint16_t*)(elements + (i * 8));
                    
                    elem[0] = 0;        /* +0x00 */
                    halfwords[2] = 0;   /* +0x04 */
                    halfwords[3] = 0;   /* +0x06 */
                }
                
                buffer->data = elements;
            } else {
                buffer->data = NULL;
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5BE8()  @ 0x82115BE8 | size: 0xD0
// Allocates and initializes a 56-byte serveTipData object with nested structure
// ─────────────────────────────────────────────────────────────────────────────
extern float g_float_value_22840;  /* @ offset 22840 from base */

void* xe_5BE8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(56, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 56, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        float* floats = (float*)obj;
        
        /* Initialize serveTipData object */
        words[0] = 0x82043300;  /* vtable for hitTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x8204330C;  /* second vtable for hitTipData */
        
        /* Initialize remaining fields */
        words[9] = 0xFFFFFFFF;   /* +0x24 - -1 */
        words[10] = 0xFFFFFFFF;  /* +0x28 - -1 */
        words[11] = 0xFFFFFFFF;  /* +0x2C - -1 */
        words[12] = 0xFFFFFFFF;  /* +0x30 - -1 */
        floats[13] = g_float_value_22840;  /* +0x34 - float value */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5CC8()  @ 0x82115CC8 | size: 0xC4
// Allocates and initializes a 52-byte focusMeterTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5CC8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(52, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 52, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize focusMeterTipData object */
        words[0] = 0x82043398;  /* vtable for serveTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x820433A4;  /* second vtable for serveTipData */
        
        /* Initialize remaining fields */
        words[9] = 0xFFFFFFFF;   /* +0x24 - -1 */
        words[10] = 0xFFFFFFFF;  /* +0x28 - -1 */
        words[11] = 0xFFFFFFFF;  /* +0x2C - -1 */
        words[12] = 0xFFFFFFFF;  /* +0x30 - -1 */
        words[3] = 1;            /* +0x0C - overwrite with 1 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5DA0()  @ 0x82115DA0 | size: 0xC8
// Allocates and initializes a 48-byte forcedFromTableCenterTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5DA0(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(48, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 48, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize forcedFromTableCenterTipData object */
        words[0] = 0x82043430;  /* vtable for focusMeterTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x8204343C;  /* second vtable for focusMeterTipData */
        
        /* Initialize remaining fields */
        words[9] = 3;            /* +0x24 - value 3 */
        words[10] = 0;           /* +0x28 */
        words[11] = 6;           /* +0x2C - value 6 */
        words[3] = 2;            /* +0x0C - value 2 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5E78()  @ 0x82115E78 | size: 0xBC
// Allocates and initializes a 44-byte noSoftShotsTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5E78(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(44, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize noSoftShotsTipData object */
        words[0] = 0x820434A0;  /* vtable for forcedFromTableCenterTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x820434AC;  /* second vtable for forcedFromTableCenterTipData */
        
        /* Initialize remaining fields */
        words[9] = 4;            /* +0x24 - value 4 */
        words[10] = 4;           /* +0x28 - value 4 */
        words[3] = 4;            /* +0x0C - value 4 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_5F48()  @ 0x82115F48 | size: 0xC8
// Allocates and initializes a 48-byte tooFarFromTableTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5F48(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(48, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 48, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize tooFarFromTableTipData object */
        words[0] = 0x82043510;  /* vtable for noSoftShotsTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 0;          /* +0x10 - disable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x8204351C;  /* second vtable for noSoftShotsTipData */
        
        /* Initialize remaining fields */
        words[9] = 2;            /* +0x24 - value 2 */
        words[10] = 0;           /* +0x28 */
        words[11] = 48;          /* +0x2C - value 48 */
        words[3] = 5;            /* +0x0C - value 5 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_6020()  @ 0x82116020 | size: 0xC4
// Allocates and initializes a 44-byte badSoftShotTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_6020(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(44, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize badSoftShotTipData object */
        words[0] = 0x82043580;  /* vtable for tooFarFromTableTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x8204358C;  /* second vtable for tooFarFromTableTipData */
        
        /* Initialize remaining fields */
        words[9] = 2;            /* +0x24 - value 2 */
        words[10] = 50;          /* +0x28 - value 50 */
        words[3] = 6;            /* +0x0C - value 6 */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// xe_60F8()  @ 0x821160F8 | size: 0xC4
// Allocates and initializes a 44-byte backspinTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_60F8(void) {
    /* Ensure main thread heap is initialized */
    sysMemAllocator_InitMainThread();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(44, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    void* obj = allocFunc(allocator, 44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize backspinTipData object */
        words[0] = 0x820435F0;  /* vtable for badSoftShotTipData (parent) */
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */
        
        /* Initialize nested structure at +20 */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = 0x820282EC;  /* vtable for rage::xmlNodeStruct */
        nested[1] = 0;           /* +0x04 */
        nested[2] = 0;           /* +0x08 */
        nested[3] = 0;           /* +0x0C */
        
        /* Overwrite nested vtable with second vtable */
        nested[0] = 0x820435FC;  /* second vtable for badSoftShotTipData */
        
        /* Initialize remaining fields */
        words[9] = 2;            /* +0x24 - value 2 */
        words[10] = 52;          /* +0x28 - value 52 */
        words[3] = 7;            /* +0x0C - value 7 */
        
        return obj;
    }
    
    return NULL;
}
