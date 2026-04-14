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
void AllocateU32Buffer(XeBuffer* buffer, uint16_t capacity);      /* @ 0x820C8E30 - uint32_t array */
void EnsureBufferInit8(XeBuffer* buffer);                         /* @ 0x820F0DA0 - 8-byte init */
void AllocateU16BufferOnce(XeBuffer* buffer, uint16_t newCapacity);   /* @ 0x820FFF00 - uint16_t array */
void Allocate64ByteElementBuffer(XeBuffer* buffer, uint16_t newCapacity);   /* @ 0x8213CBD8 - 64-byte elements */
void InitStructWith10WordsZero(void* structure);                          /* @ 0x8214C320 - complex init */
void AllocateU32AlignedWithCounter(void* structure, uint32_t elementCount);   /* @ 0x82134408 - aligned alloc */
void AllocateFixed6Capacity24Bytes(XeBuffer* buffer);                         /* @ 0x821362D0 - 24-byte alloc */
void CopyVec16Buffer(XeBuffer* dest, const XeBuffer* src);      /* @ 0x82247E78 - vector copy */
void InitAudioControlManager(void* structure, uint8_t enableFlag);      /* @ 0x82160048 - audio control */
void UpdateParamOffsetState(void* structure, int32_t param);           /* @ 0x821CD5F8 - state update */
