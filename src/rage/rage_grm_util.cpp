/**
 * rage_grm_util.cpp — RAGE Engine graphics/shader utility functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements shader parameter management, string utilities, thread pool
 * cleanup, and object lifecycle helpers used across the RAGE engine.
 */

#include <cstdint>
#include <cstring>

// Forward declarations for external RAGE functions
extern "C" {
    void rage_free_00C0(void* ptr);
    void rage_2E18(void* ptr);
    void rage_threadpool_cleanup_6878(void* entry);
    void* atSingleton_29E0_g(void* ptr);
    void util_E988(void* array, int flags);
    void atSingleton_A680_g(void* array, int flags);
    void atSingleton_DCE8_gen(void* array, int flags);
    void ph_0908(char* dest, const char* src, int maxLen);
    int  _stricmp(const char* s1, const char* s2);
}

namespace rage {

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_0888 @ 0x820F0888 | size: 0x7c (124 bytes)
 *
 * Appends a source string to the end of a destination string, respecting
 * a maximum buffer size. Equivalent to a bounded strncat.
 *
 * If dest is NULL, the append starts at position 0.
 * The result is always null-terminated.
 * ═══════════════════════════════════════════════════════════════════════════ */
void StringAppend(char* dest, const char* src, int maxLen) {
    uint32_t destLen = 0;

    if (dest != nullptr) {
        const char* p = dest;
        while (*p != '\0') {
            p++;
        }
        destLen = (uint32_t)(p - dest);
    }

    char* out = dest + destLen;
    int remaining = maxLen - destLen - 1;
    char ch = *src;

    while (ch != '\0') {
        if (remaining <= 0) {
            *out = '\0';
            return;
        }
        src++;
        *out = ch;
        remaining--;
        out++;
        ch = *src;
    }

    *out = '\0';
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_18B8 @ 0x820C18B8 | size: 0x54 (84 bytes)
 *
 * Destroys a datBase-derived container. If the object has a child at +0x04,
 * calls that child's virtual destructor (vtable slot 0) with deleting flag,
 * then frees the container's own memory.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct datBaseContainer {
    void** vtable;   // +0x00
    void*  child;    // +0x04
};

void datBaseContainer_Destroy(datBaseContainer* self, int flags) {
    if (self->child != nullptr) {
        void** childVtable = *(void***)self->child;
        typedef void (*DtorFn)(void* obj, int flags);
        DtorFn dtor = (DtorFn)childVtable[0];
        dtor(self->child, 1);
    }
    rage_free_00C0(self);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_88D8 @ 0x820C88D8 | size: 0x80 (128 bytes)
 *
 * Removes a thread pool entry from the global singly-linked list, then
 * performs thread pool cleanup and frees the entry. Each node has a 'next'
 * pointer at offset +0x14.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ThreadPoolEntry {
    uint8_t          pad[20];  // +0x00
    ThreadPoolEntry* next;     // +0x14
};

extern ThreadPoolEntry* g_threadpool_head; // @ 0x825EBCBC

void ThreadPoolEntry_RemoveAndDestroy(ThreadPoolEntry* self) {
    if (g_threadpool_head != nullptr) {
        ThreadPoolEntry** linkPtr = &g_threadpool_head;

        while (*linkPtr != nullptr) {
            if (*linkPtr == self) {
                break;
            }
            linkPtr = &(*linkPtr)->next;
            if (*linkPtr == nullptr) {
                break;
            }
        }

        if (*linkPtr != nullptr) {
            *linkPtr = (*linkPtr)->next;
        }
    }

    rage_threadpool_cleanup_6878(self);
    rage_free_00C0(self);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_6348 @ 0x820F6348 | size: 0xc0 (192 bytes)
 *
 * Releases a shader parameter value based on its type tag at +0x14.
 * Type 6 (texture reference) is released via rage_2E18. Types 8 and 9
 * (raw buffers) are freed directly. The type is reset to 0 after release.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ShaderParam {
    void*    data;     // +0x00
    uint8_t  pad[16];  // +0x04
    int32_t  type;     // +0x14
};

void ShaderParam_ReleaseValue(ShaderParam* self) {
    int32_t type = self->type;

    if (type == 6) {
        if (self->data != nullptr) {
            rage_2E18(self->data);
            self->type = 0;
            return;
        }
    } else if (type == 8) {
        rage_free_00C0(self->data);
        self->type = 0;
        return;
    } else if (type == 9) {
        rage_free_00C0(self->data);
        self->type = 0;
        return;
    }

    self->type = 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_6408 @ 0x820F6408 | size: 0x7c (124 bytes)
 *
 * Deep-copies a shader parameter from source to destination. Releases the
 * destination's current value, copies all fields, duplicates the name
 * string via atSingleton_29E0_g, and marks the copy as owned.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ShaderParamFull {
    void*    data;      // +0x00
    uint32_t field_04;  // +0x04
    uint32_t field_08;  // +0x08
    uint32_t field_0C;  // +0x0C
    void*    nameStr;   // +0x10
    uint32_t field_14;  // +0x14
    uint8_t  pad_18[4]; // +0x18
    uint8_t  owned;     // +0x1C
};

void ShaderParam_CopyFrom(ShaderParamFull* self, const ShaderParamFull* src) {
    ShaderParam_ReleaseValue((ShaderParam*)self);
    rage_free_00C0(self->nameStr);

    self->data     = src->data;
    self->field_04 = src->field_04;
    self->field_08 = src->field_08;
    self->field_0C = src->field_0C;

    self->nameStr  = atSingleton_29E0_g(src->nameStr);
    self->owned    = 1;
    self->field_14 = src->field_14;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_F008 @ 0x820EF008 | size: 0x54 (84 bytes)
 *
 * Recursively destroys a shader FX node linked list. Cleans up each node's
 * parameter value, frees its name string at +0x10, recurses into the next
 * node at +0x18, then frees the node itself.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ShaderFxNode {
    uint8_t       pad_00[16]; // +0x00
    void*         name;       // +0x10
    uint8_t       pad_14[4];  // +0x14
    ShaderFxNode* next;       // +0x18
};

void ShaderFxNode_DestroyRecursive(ShaderFxNode* self, int flags) {
    ShaderParam_ReleaseValue((ShaderParam*)self);
    rage_free_00C0(self->name);

    if (self->next != nullptr) {
        ShaderFxNode_DestroyRecursive(self->next, 1);
    }

    rage_free_00C0(self);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_EA70 @ 0x820EEA70 | size: 0xa4 (164 bytes)
 *
 * Destroys a single shader effect parameter block containing four
 * sub-arrays with per-type cleanup:
 *   +0x28/+0x30: sampler states (freed directly)
 *   +0x1C/+0x24: render state overrides (util_E988)
 *   +0x10/+0x18: texture bindings (atSingleton_A680_g)
 *   +0x00-04/+0x0C: base effect parameters (atSingleton_DCE8_gen or free)
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ShaderEffectParamBlock {
    void*    dataPtr;       // +0x00
    void*    dataPtr2;      // +0x04
    uint8_t  pad_08[4];     // +0x08
    int32_t  dataCount;     // +0x0C
    void*    subArray1;     // +0x10
    uint8_t  pad_14[4];     // +0x14
    int32_t  subCount1;     // +0x18
    void*    subArray2;     // +0x1C
    uint8_t  pad_20[4];     // +0x20
    int32_t  subCount2;     // +0x24
    void*    subArray3;     // +0x28
    uint8_t  pad_2C[4];     // +0x2C
    int32_t  subCount3;     // +0x30
};

void ShaderEffectParamBlock_Destroy(ShaderEffectParamBlock* self) {
    if (self->subCount3 != 0) {
        rage_free_00C0(self->subArray3);
    }

    if (self->subCount2 != 0 && self->subArray2 != nullptr) {
        util_E988(self->subArray2, 3);
    }

    if (self->subCount1 != 0 && self->subArray1 != nullptr) {
        atSingleton_A680_g(self->subArray1, 3);
    }

    if (self->dataCount != 0) {
        if (self->dataPtr2 != nullptr) {
            atSingleton_DCE8_gen(self->dataPtr2, 3);
        } else {
            rage_free_00C0(self->dataPtr);
        }
    } else {
        rage_free_00C0(self->dataPtr);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_EA18 @ 0x820EEA18 | size: 0x54 (84 bytes)
 *
 * Destroys a counted array of shader effect parameter blocks. The count is
 * stored at array[-1]. Each element is 64 bytes. Iterates backwards calling
 * ShaderEffectParamBlock_Destroy on each, then frees the backing store.
 * ═══════════════════════════════════════════════════════════════════════════ */
void ShaderEffectParamArray_Destroy(void* array, int flags) {
    uint32_t* countPtr = (uint32_t*)array - 1;
    uint32_t count = *countPtr;
    int32_t i = (int32_t)count - 1;
    uint8_t* elem = (uint8_t*)array + count * 64;

    while (i >= 0) {
        elem -= 64;
        ShaderEffectParamBlock_Destroy((ShaderEffectParamBlock*)elem);
        i--;
    }

    rage_free_00C0(countPtr);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_12E0 @ 0x820F12E0 | size: 0x68 (104 bytes)
 *
 * Destroys a geometry index buffer. Frees the data buffer if the allocated
 * flag is set, then destroys the index data via the counted array destructor
 * or frees the structure directly.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct GeomIndexBuffer {
    void*    indexData;    // +0x00
    uint8_t  pad_04[4];   // +0x04
    int32_t  entryCount;  // +0x08
    void*    dataBuffer;  // +0x0C
    uint8_t  pad_10[4];   // +0x10
    int32_t  allocated;   // +0x14
};

void GeomIndexBuffer_Destroy(GeomIndexBuffer* self) {
    if (self->allocated != 0) {
        rage_free_00C0(self->dataBuffer);
    }

    if (self->entryCount != 0) {
        if (self->indexData == nullptr) {
            rage_free_00C0(self);
            return;
        }
        ShaderEffectParamArray_Destroy(self->indexData, 3);
    }

    rage_free_00C0(self);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * rage_5FC0 @ 0x820F5FC0 | size: 0x80 (128 bytes)
 *
 * Searches the global shader preset registry for a preset matching the
 * given name (case-insensitive comparison). The name is first normalized
 * to lowercase with forward slashes via ph_0908.
 *
 * Returns the preset index on match, or -1 if not found.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct ShaderPresetEntry {
    uint8_t  pad[8];    // +0x00
    char*    name;      // +0x08
    uint8_t  rest[16];  // +0x0C
};

extern int32_t           g_shaderPresetCount; // @ 0x8260635C
extern ShaderPresetEntry* g_shaderPresetArray; // @ 0x82606364

int32_t ShaderPreset_FindByName(const char* name) {
    char normalizedName[1024];
    ph_0908(normalizedName, name, 1024);

    for (int32_t i = 0; i < g_shaderPresetCount; i++) {
        if (_stricmp(normalizedName, g_shaderPresetArray[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

} // namespace rage
