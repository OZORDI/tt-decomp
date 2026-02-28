/**
 * xe_graphics.c — Xbox Executive Graphics Context Initialization
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * Initializes graphics-related context structures used by the Xbox 360
 * rendering pipeline. This module allocates and configures transformation
 * matrices and rendering state.
 */

#include <stdint.h>
#include <stddef.h>

/* Forward declarations */
extern void xe_main_thread_init_0038(void);

/* Allocator interface - accessed via SDA (Small Data Area) at r13+0 */
typedef void* (*AllocatorVCall)(void* allocator, int32_t size, int32_t alignment);

/* SDA base pointer (r13 register) */
extern uint32_t* g_sda_base;

/* Global pointer to graphics context @ 0x824063C4 */
static void** g_graphicsContextPtr = (void**)0x824063C4;

/* Float constants from .rdata section */
static const float* kFloatConstant1 = (const float*)0x8202D10C;  /* Used at +0x08 */
static const float* kFloatConstant2 = (const float*)0x8202D108;  /* lbl_8202D110 - 8 */
static const float* kFloatConstant3 = (const float*)0x8202D110;  /* Used multiple times */
static const float* kFloatZero = (const float*)0x823C8A50;       /* 0.0f */

/**
 * Graphics context structure (128 bytes)
 * 
 * This structure appears to hold transformation matrices and rendering state.
 * The layout suggests a 3x3 or 4x4 matrix with additional metadata.
 */
typedef struct GraphicsContext {
    uint32_t field_0x00;        /* +0x00: Set to 2 */
    uint32_t field_0x04;        /* +0x04: Set to 2 */
    float    field_0x08;        /* +0x08: Float from kFloatConstant1 */
    float    field_0x0C;        /* +0x0C: Float (0.0f) */
    uint8_t  padding1[8];       /* +0x10-0x17 */
    uint8_t  field_0x18;        /* +0x18: Set to 0 */
    uint8_t  padding2[7];       /* +0x19-0x1F */
    
    /* Matrix or vector data starting at +0x20 */
    float    matrix[9];         /* +0x20-0x43: 3x3 matrix or 9 floats */
    uint8_t  padding3[12];      /* +0x44-0x4F */
    
    /* 16-byte aligned vector (zeroed) */
    uint8_t  vector[16];        /* +0x50-0x5F: VMX vector, zeroed */
    
    uint8_t  padding4[4];       /* +0x60-0x63 */
    uint32_t field_0x64;        /* +0x64: Set to 0 */
    uint32_t field_0x68;        /* +0x68: Set to 0 */
    uint32_t field_0x6C;        /* +0x6C: Set to 0 */
    uint32_t field_0x70;        /* +0x70: Set to 0 */
    uint32_t field_0x74;        /* +0x74: Set to 0 */
    uint32_t field_0x78;        /* +0x78: Set to -1 (0xFFFFFFFF) */
    uint8_t  padding5[4];       /* +0x7C-0x7F */
} GraphicsContext;

/**
 * xe_E420 @ 0x8213E420 | size: 0x100
 *
 * Initializes the global graphics context structure.
 * 
 * This function:
 * 1. Ensures the main thread heap is initialized
 * 2. Allocates 128 bytes for the graphics context (16-byte aligned)
 * 3. Initializes the structure with default values and transformation matrices
 * 4. Stores the pointer in a global variable
 * 
 * If allocation fails, the global pointer is set to NULL.
 */
void xe_E420(void) {
    /* Ensure main thread TLS and allocator are initialized */
    xe_main_thread_init_0038();
    
    /* Get allocator from SDA context */
    uint32_t* sdaContext = (uint32_t*)(uintptr_t)g_sda_base[0];
    void* allocator = (void*)(uintptr_t)sdaContext[1];
    
    /* Get allocator vtable and call Allocate(128, 16) */
    void** vtable = *(void***)allocator;
    AllocatorVCall allocFunc = (AllocatorVCall)vtable[1];
    
    GraphicsContext* ctx = (GraphicsContext*)allocFunc(allocator, 128, 16);
    
    if (ctx != NULL) {
        /* Initialize structure fields */
        ctx->field_0x00 = 2;
        ctx->field_0x04 = 2;
        ctx->field_0x08 = *kFloatConstant1;
        ctx->field_0x0C = *kFloatZero;
        ctx->field_0x18 = 0;
        
        /* Initialize matrix values */
        ctx->matrix[0] = *kFloatConstant2;  /* +0x20 */
        ctx->matrix[1] = *kFloatConstant3;  /* +0x24 */
        ctx->matrix[2] = *kFloatConstant3;  /* +0x28 */
        ctx->matrix[4] = *kFloatConstant3;  /* +0x30 */
        ctx->matrix[5] = *kFloatZero;       /* +0x34 */
        ctx->matrix[6] = *kFloatConstant3;  /* +0x38 */
        ctx->matrix[8] = *kFloatConstant3;  /* +0x40 */
        ctx->matrix[9] = *kFloatConstant3;  /* +0x44 (beyond matrix[8]) */
        ctx->matrix[10] = *kFloatZero;      /* +0x48 (beyond matrix[8]) */
        
        /* Zero the VMX vector (16 bytes at +0x50) */
        for (int i = 0; i < 16; i++) {
            ctx->vector[i] = 0;
        }
        
        /* Initialize remaining fields */
        ctx->field_0x64 = 0;
        ctx->field_0x68 = 0;
        ctx->field_0x6C = 0;
        ctx->field_0x70 = 0;
        ctx->field_0x74 = 0;
        ctx->field_0x78 = 0xFFFFFFFF;  /* -1 */
        
        /* Store pointer to global */
        *g_graphicsContextPtr = ctx;
    } else {
        /* Allocation failed - store NULL */
        *g_graphicsContextPtr = NULL;
    }
}
