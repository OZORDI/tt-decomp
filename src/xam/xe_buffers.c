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
