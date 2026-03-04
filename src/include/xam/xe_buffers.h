/**
 * xe_buffers.h — Xbox Executive Dynamic Buffer Management
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#pragma once

#include <stdint.h>

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

/* Buffer allocation functions */
void xe_8E30(XeBuffer* buffer, uint16_t capacity);      /* @ 0x820C8E30 - uint32_t array */
void xe_0DA0(XeBuffer* buffer);                         /* @ 0x820F0DA0 - 8-byte init */
void xe_FF00(XeBuffer* buffer, uint16_t newCapacity);   /* @ 0x820FFF00 - uint16_t array */
void xe_CBD8(XeBuffer* buffer, uint16_t newCapacity);   /* @ 0x8213CBD8 - 64-byte elements */
void xe_C320(void* structure);                          /* @ 0x8214C320 - complex init */
void xe_4408(void* structure, uint32_t elementCount);   /* @ 0x82134408 - aligned alloc */
void xe_62D0(XeBuffer* buffer);                         /* @ 0x821362D0 - 24-byte alloc */
void xe_7E78(XeBuffer* dest, const XeBuffer* src);      /* @ 0x82247E78 - vector copy */
void xe_0048(void* structure, uint8_t enableFlag);      /* @ 0x82160048 - audio control */
void xe_D5F8(void* structure, int32_t param);           /* @ 0x821CD5F8 - state update */
