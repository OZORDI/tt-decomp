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

/* Overflow guard constants: UINT32_MAX / element_size
 * Used to check if (count * elem_size) would overflow uint32_t */
#define MAX_SAFE_COUNT_2   0x7FFFFFFFU  /* UINT32_MAX / 2  — for 2-byte elements  */
#define MAX_SAFE_COUNT_4   0x3FFFFFFFU  /* UINT32_MAX / 4  — for 4-byte elements  */
#define MAX_SAFE_COUNT_8   0x1FFFFFFFU  /* UINT32_MAX / 8  — for 8-byte elements  */
#define MAX_SAFE_COUNT_16  0x0FFFFFFFU  /* UINT32_MAX / 16 — for 16-byte elements */
#define MAX_SAFE_COUNT_56  0x04924924U  /* UINT32_MAX / 56 — for 56-byte elements */
#define MAX_SAFE_COUNT_64  0x03FFFFFFU  /* UINT32_MAX / 64 — for 64-byte elements */
#define ALLOC_HEADER_ROOM  0xFFFFFFFBU  /* UINT32_MAX - 4, for header overflow check */

/* Vtable externs — resolved from .rdata RTTI */
extern void* vtable_pcrEmoteData;                    /* 0x8202EB64 */
extern void* vtable_rage_grmModelFactory;            /* 0x8202F5A8 */
extern void* vtable_rage_xmlNodeStruct;              /* 0x820282EC */
extern void* vtable_hitTipData;                      /* 0x82043300 — primary vtable */
extern void* vtable_hitTipData_base;                 /* 0x8204330C — secondary vtable (MI) */
extern void* vtable_serveTipData;                    /* 0x82043398 — primary vtable */
extern void* vtable_serveTipData_base;               /* 0x820433A4 — secondary vtable (MI) */
extern void* vtable_focusMeterTipData;               /* 0x82043430 — primary vtable */
extern void* vtable_focusMeterTipData_base;          /* 0x8204343C — secondary vtable (MI) */
extern void* vtable_forcedFromTableCenterTipData;       /* 0x820434A0 — primary vtable */
extern void* vtable_forcedFromTableCenterTipData_base;  /* 0x820434AC — secondary vtable (MI) */
extern void* vtable_noSoftShotsTipData;              /* 0x82043510 — primary vtable */
extern void* vtable_noSoftShotsTipData_base;         /* 0x8204351C — secondary vtable (MI) */
extern void* vtable_tooFarFromTableTipData;          /* 0x82043580 — primary vtable */
extern void* vtable_tooFarFromTableTipData_base;     /* 0x8204358C — secondary vtable (MI) */
extern void* vtable_badSoftShotTipData;              /* 0x820435F0 — primary vtable */
extern void* vtable_badSoftShotTipData_base;         /* 0x820435FC — secondary vtable (MI) */
extern void* vtable_gdUnlockConditionMultiBounce;    /* 0x8204B244 */
extern void* vtable_gdCSActionCharVisibleData;       /* 0x82077D1C */

/* Main-thread heap init + SDA context root (used by every allocator call) */
extern void sysMemAllocator_InitMainThread(void);  /* @ 0x820C0038 */
extern uint32_t* g_sda_base;                       /* SDA base (r13) */

/* ─── rage::sysMemAllocator typed virtual interface ─────────────────────────
 * RTTI vtable at 0x820332C4. Slot[1] is Allocate(size, alignment).
 * Backing impl: sysMemAllocator_Allocate_61A0 @ 0x821861A0. */
typedef struct sysMemAllocator sysMemAllocator;
typedef struct sysMemAllocatorVtable {
    void* vfn_0;                                                  /* slot 0 */
    void* (*Allocate)(sysMemAllocator* self, uint32_t size, uint32_t align);
    /* ... additional slots (Free, etc.) */
} sysMemAllocatorVtable;
struct sysMemAllocator {
    const sysMemAllocatorVtable* vtable;
};

/* Context block @ *g_sda_base[0] — allocator lives at +4 */
typedef struct SdaAllocCtx {
    uint32_t    unk0;
    sysMemAllocator* allocator;
} SdaAllocCtx;

/* Resolve the per-thread main allocator and call its virtual Allocate(). */
static inline void* xe_alloc_aligned(uint32_t size, uint32_t alignment) {
    sysMemAllocator_InitMainThread();
    SdaAllocCtx* ctx = *(SdaAllocCtx**)&g_sda_base[0];
    sysMemAllocator* a = ctx->allocator;
    return a->vtable->Allocate(a, size, alignment);
}

// ─────────────────────────────────────────────────────────────────────────────
// AllocateU32Buffer()  @ 0x820C8E30 | size: 0x68
// Allocates buffer for uint32_t array (4 bytes per element)
// ─────────────────────────────────────────────────────────────────────────────
void AllocateU32Buffer(XeBuffer* buffer, uint16_t capacity) {
    void* data = NULL;
    
    if (capacity != 0) {
        if (capacity > MAX_SAFE_COUNT_4) {
            data = rage_alloc(UINT32_MAX);
        } else {
            data = rage_alloc(capacity * 4);
        }
    }

    buffer->data = data;
    buffer->capacity = capacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// EnsureBufferInit8()  @ 0x820F0DA0 | size: 0x60
// Initializes buffer with capacity of 1 element (8 bytes)
// ─────────────────────────────────────────────────────────────────────────────
void EnsureBufferInit8(XeBuffer* buffer) {
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
// AllocateU16BufferOnce()  @ 0x820FFF00 | size: 0x78
// Allocates buffer for uint16_t array (2 bytes per element)
// ─────────────────────────────────────────────────────────────────────────────
void AllocateU16BufferOnce(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            if (newCapacity > MAX_SAFE_COUNT_2) {
                buffer->data = rage_alloc(UINT32_MAX);
            } else {
                buffer->data = rage_alloc(newCapacity * 2);
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// Allocate64ByteElementBuffer()  @ 0x8213CBD8 | size: 0x78
// Allocates buffer for 64-byte elements (capacity * 64)
// ─────────────────────────────────────────────────────────────────────────────
void Allocate64ByteElementBuffer(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            if (newCapacity > MAX_SAFE_COUNT_64) {
                buffer->data = rage_alloc(UINT32_MAX);
            } else {
                buffer->data = rage_alloc(newCapacity * 64);
            }
        } else {
            buffer->data = NULL;
        }
    }
    
    /* Update current count */
    buffer->count = newCapacity;
}

// ─────────────────────────────────────────────────────────────────────────────
// InitStructWith10WordsZero()  @ 0x8214C320 | size: 0x68
// Initializes a complex structure with multiple nested buffers
// ─────────────────────────────────────────────────────────────────────────────
extern void xe_C530(void* structure, uint32_t param);  /* @ 0x8214C530 */

void InitStructWith10WordsZero(void* structure) {
    /* Zero-initialize the structure (40 bytes) */
    uint32_t* words = (uint32_t*)structure;
    for (int i = 0; i < 10; i++) {
        words[i] = 0;
    }

    /* Call initialization helper */
    xe_C530(structure, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// AllocateU32AlignedWithCounter()  @ 0x82134408 | size: 0x84
// Allocates aligned memory and initializes a structure with pointer and counter
// ─────────────────────────────────────────────────────────────────────────────
void AllocateU32AlignedWithCounter(void* structure, uint32_t elementCount) {
    uint32_t allocSize;

    /* Calculate allocation size: elementCount * 4 bytes */
    if (elementCount > MAX_SAFE_COUNT_4) {
        allocSize = UINT32_MAX;
    } else {
        allocSize = elementCount * 4;
    }

    void* data = xe_alloc_aligned(allocSize, 16);

    /* Initialize structure: pointer at +0, zero at +4 */
    uint32_t* words = (uint32_t*)structure;
    words[0] = (uint32_t)(uintptr_t)data;
    words[1] = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// AllocateFixed6Capacity24Bytes()  @ 0x821362D0 | size: 0x5C
// Allocates 24 bytes and stores pointer with capacity of 6
// ─────────────────────────────────────────────────────────────────────────────
void AllocateFixed6Capacity24Bytes(XeBuffer* buffer) {
    void* data = xe_alloc_aligned(24, 16);
    
    buffer->data = data;
    buffer->capacity = 6;
}

// ─────────────────────────────────────────────────────────────────────────────
// CopyVec16Buffer()  @ 0x82247E78 | size: 0xB0
// Copies vector data from source to destination buffer
// ─────────────────────────────────────────────────────────────────────────────
void CopyVec16Buffer(XeBuffer* dest, const XeBuffer* src) {
    uint16_t capacity = src->capacity;
    
    /* Copy capacity to both count and capacity fields */
    dest->capacity = capacity;
    dest->count = capacity;
    
    if (capacity != 0) {
        /* Allocate destination buffer: capacity * 16 bytes per vector */
        uint32_t allocSize;
        if (capacity > MAX_SAFE_COUNT_16) {
            allocSize = UINT32_MAX;
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
// InitAudioControlManager()  @ 0x82160048 | size: 0x60
// Initializes audio control manager structure
// ─────────────────────────────────────────────────────────────────────────────
extern void xe_A750(uint32_t size, uint32_t alignment);  /* @ 0x821AA750 */

/* Mid-symbol .rdata/.data words that IDA hasn't split into standalone
 * globals. They are surfaced as externs so the body stops embedding raw
 * addresses. Rename once the owning class / global lands — these live
 * partway into larger symbols so their permanent names depend on the
 * class of the owning object, not on anything we can infer here. */
extern uint32_t audVoice_vtable_word;  /* @ 0x82163570 (audVoiceSfx_PlayByEntry+0xD8) */
extern uint32_t g_swfCMD_globalState;  /* @ 0x824063F4 (swfCMD_6290_p46+0x164) */

void InitAudioControlManager(void* structure, uint8_t enableFlag) {
    uint32_t* words = (uint32_t*)structure;

    /* Store vtable pointer (raw word sitting mid-audVoiceSfx_PlayByEntry) */
    words[0] = audVoice_vtable_word;

    /* Store enable flag at +4 */
    uint8_t* bytes = (uint8_t*)structure;
    bytes[4] = enableFlag;

    /* If enabled, allocate 240 bytes with 16-byte alignment */
    if (enableFlag != 0) {
        xe_A750(240, 16);
    }

    /* Clear global state variable (inside SWF command data block) */
    g_swfCMD_globalState = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// UpdateParamOffsetState()  @ 0x821CD5F8 | size: 0x80
// Updates game state based on parameter offset calculations
// ─────────────────────────────────────────────────────────────────────────────
extern void pg_D678_gen(void* structure, int32_t value1, void* ptr, int32_t value2);

/* Pointer word living inside atSingleton_2940_p39+0x28 — holds the active
 * atSingleton instance. Permanent name awaits the atSingleton_2940_p39
 * lift; until then this anchor name captures the access pattern. */
extern uint32_t* g_atSingleton_2940_ptr;  /* @ 0x82412968 */

void UpdateParamOffsetState(void* structure, int32_t param) {
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
    
    /* Load global singleton pointer and copy float at +0x30 (48) */
    uint32_t* globalPtr = (uint32_t*)g_atSingleton_2940_ptr;
    float floatValue = *(float*)((uint8_t*)globalPtr + 48);
    
    /* Calculate destination offset: (param + 0x20000 - 23755) * 4 */
    int32_t offset3 = ((param + 0x20000) - 23755) * 4;
    float* destFloat = (float*)((uint8_t*)structure + offset3);
    *destFloat = floatValue;
}

// ─────────────────────────────────────────────────────────────────────────────
// AllocatePcrEmoteData()  @ 0x820EB958 | size: 0x8C
// Allocates and initializes a 24-byte pcrEmoteData structure
// ─────────────────────────────────────────────────────────────────────────────
extern uint32_t* g_float_one_ptr;  /* @ 0x8202D110 */

void* AllocatePcrEmoteData(void) {
    void* obj = xe_alloc_aligned(24, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Store vtable pointer for pcrEmoteData */
        words[0] = (uint32_t)(uintptr_t)&vtable_pcrEmoteData;
        
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
// CopyU32Buffer()  @ 0x820FD100 | size: 0x9C
// Copies buffer data from source to destination (4-byte elements)
// ─────────────────────────────────────────────────────────────────────────────
void CopyU32Buffer(XeBuffer* dest, const XeBuffer* src) {
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
// AllocateVtableTrackedStruct32()  @ 0x82101E48 | size: 0xA0
// Allocates and initializes a 32-byte game logic structure
// ─────────────────────────────────────────────────────────────────────────────
void* AllocateVtableTrackedStruct32(void) {
    void* obj = xe_alloc_aligned(32, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Mid-.rdata vtable/fptr words; parent class not yet identified.
         * Declared as extern so the body stops baking in the addresses.
         * The lbl_ anchor names are the stable shape until the owning
         * class header lands; they point partway into a larger symbol
         * so there is no better name derivable from static data alone. */
        extern uint32_t lbl_82020120_vtable_word;  /* @ 0x82020154 (lbl_82020120+0x34) */
        extern uint32_t lbl_820201D8_vtable_word;  /* @ 0x820201DC (lbl_820201D8+0x04) */

        /* Initialize structure fields */
        words[0] = lbl_82020120_vtable_word;  /* vtable pointer @ +0x00 */
        words[1] = 0;      /* +0x04 */
        words[2] = 0;      /* +0x08 */
        words[3] = 0;      /* +0x0C */
        words[4] = lbl_820201D8_vtable_word;  /* secondary vtable @ +0x10 */
        words[5] = 0;      /* +0x14 */
        words[6] = 0;      /* +0x18 */
        words[7] = 0;      /* +0x1C */
        
        return obj;
    }
    
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// AllocateU32BufferZeroed()  @ 0x820F3508 | size: 0xBC
// Allocates buffer with capacity check and zero-initialization
// ─────────────────────────────────────────────────────────────────────────────
void AllocateU32BufferZeroed(XeBuffer* buffer, uint16_t capacity) {
    void* data = NULL;
    
    if (capacity != 0) {
        uint32_t allocSize;
        if (capacity > MAX_SAFE_COUNT_4) {
            allocSize = UINT32_MAX;
        } else {
            allocSize = capacity * 4;
            if (allocSize > ALLOC_HEADER_ROOM) {
                allocSize = UINT32_MAX;
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
// Allocate16ByteElementBufferOnce()  @ 0x820E73E0 | size: 0xD8
// Allocates buffer for 16-byte elements with float initialization
// ─────────────────────────────────────────────────────────────────────────────
extern float g_float_zero;  /* @ 0x8202D108 */

void Allocate16ByteElementBufferOnce(XeBuffer* buffer, uint16_t newCapacity) {
    if (buffer->capacity == 0) {
        /* First allocation */
        buffer->capacity = newCapacity;
        
        if (newCapacity != 0) {
            uint32_t allocSize;
            if (newCapacity > MAX_SAFE_COUNT_16) {
                allocSize = UINT32_MAX;
            } else {
                allocSize = newCapacity * 16;
                if (allocSize > ALLOC_HEADER_ROOM) {
                    allocSize = UINT32_MAX;
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
    /* Allocate 100KB + 32 bytes (0x1 << 16 | 35296 = 101,920 bytes) */
    uint32_t allocSize = 101920;
    
    void* buffer = xe_alloc_aligned(allocSize, 16);
    
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
        uint32_t allocSize;
        if (capacity > MAX_SAFE_COUNT_56) {
            allocSize = UINT32_MAX;
        } else {
            allocSize = capacity * 56;
            if (allocSize > ALLOC_HEADER_ROOM) {
                allocSize = UINT32_MAX;
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
                elem[3] = (uint32_t)(uintptr_t)&vtable_rage_grmModelFactory;  /* +0x0C */
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
    void* obj = xe_alloc_aligned(16, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        
        /* Initialize gdUnlockConditionMultiBounce object */
        words[0] = (uint32_t)(uintptr_t)&vtable_gdUnlockConditionMultiBounce;
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
    void* obj = xe_alloc_aligned(28, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize gdCSActionCharVisibleData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_gdCSActionCharVisibleData;
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
            uint32_t allocSize;
            if (newCapacity > MAX_SAFE_COUNT_8) {
                allocSize = UINT32_MAX;
            } else {
                allocSize = newCapacity * 8;
                if (allocSize > ALLOC_HEADER_ROOM) {
                    allocSize = UINT32_MAX;
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
// Allocates and initializes a 56-byte hitTipData object with nested structure
// ─────────────────────────────────────────────────────────────────────────────
extern float g_float_value_22840;  /* @ offset 22840 from base */

void* xe_5BE8(void) {
    void* obj = xe_alloc_aligned(56, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        float* floats = (float*)obj;
        
        /* Initialize serveTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_hitTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_hitTipData_base;
        
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
// Allocates and initializes a 52-byte serveTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5CC8(void) {
    void* obj = xe_alloc_aligned(52, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize serveTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_serveTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_serveTipData_base;
        
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
// Allocates and initializes a 48-byte focusMeterTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5DA0(void) {
    void* obj = xe_alloc_aligned(48, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize focusMeterTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_focusMeterTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_focusMeterTipData_base;
        
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
// Allocates and initializes a 44-byte forcedFromTableCenterTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5E78(void) {
    void* obj = xe_alloc_aligned(44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize forcedFromTableCenterTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_forcedFromTableCenterTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_forcedFromTableCenterTipData_base;
        
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
// Allocates and initializes a 48-byte noSoftShotsTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_5F48(void) {
    void* obj = xe_alloc_aligned(48, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize noSoftShotsTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_noSoftShotsTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 0;          /* +0x10 - disable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_noSoftShotsTipData_base;
        
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
// Allocates and initializes a 44-byte tooFarFromTableTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_6020(void) {
    void* obj = xe_alloc_aligned(44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize tooFarFromTableTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_tooFarFromTableTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_tooFarFromTableTipData_base;
        
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
// Allocates and initializes a 44-byte badSoftShotTipData object
// ─────────────────────────────────────────────────────────────────────────────
void* xe_60F8(void) {
    void* obj = xe_alloc_aligned(44, 16);
    
    if (obj != NULL) {
        uint32_t* words = (uint32_t*)obj;
        uint8_t* bytes = (uint8_t*)obj;
        
        /* Initialize badSoftShotTipData object */
        words[0] = (uint32_t)(uintptr_t)&vtable_badSoftShotTipData;
        words[1] = 0;           /* +0x04 */
        words[2] = 1;           /* +0x08 - count/flag */
        bytes[16] = 1;          /* +0x10 - enable flag */

        /* Initialize nested rage::xmlNodeStruct at +20, then set MI vtable */
        uint32_t* nested = (uint32_t*)(bytes + 20);
        nested[0] = (uint32_t)(uintptr_t)&vtable_rage_xmlNodeStruct;
        nested[1] = 0;
        nested[2] = 0;
        nested[3] = 0;
        nested[0] = (uint32_t)(uintptr_t)&vtable_badSoftShotTipData_base;
        
        /* Initialize remaining fields */
        words[9] = 2;            /* +0x24 - value 2 */
        words[10] = 52;          /* +0x28 - value 52 */
        words[3] = 7;            /* +0x0C - value 7 */
        
        return obj;
    }
    
    return NULL;
}
