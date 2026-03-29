/**
 * rage_grm_util.cpp — RAGE Graphics Resource Manager Utilities
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Shader parameter management, FX node lifecycle, geometry buffer cleanup,
 * and shader preset lookup for the RAGE graphics pipeline.
 *
 * Classes/systems implemented:
 *   grmShaderParam        — single shader parameter value (type-tagged union)
 *   grmShaderParamFull    — full shader parameter with name + ownership
 *   grmShaderFxNode       — linked-list node for shader FX parameters
 *   grmShaderEffectBlock  — effect parameter block with 4 sub-arrays
 *   grmGeomIndexBuffer    — geometry index buffer with ownership tracking
 *   grmShaderPreset       — global shader preset registry lookup
 *   datBase               — generic child-object destructor wrapper
 *   sysThread             — thread pool entry unlink and destroy
 *
 * String utilities:
 *   strAppend             — bounded string concatenation (strncat equivalent)
 */

#include <cstdint>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

extern "C" {
    // RAGE heap deallocator.
    // @ 0x820C00C0
    void rage_free(void* ptr);

    // grcTexture ref-counted release — decrements reference count on a
    // texture object and frees it when count reaches zero.
    // @ 0x820C2E18
    void grcTexture_Release(void* texture);

    // Thread pool per-thread cleanup — tears down thread state.
    // @ 0x82176878 | size: 0x1FC
    void sysThread_Cleanup(void* entry);

    // String interning — interns a string into the global string hash table.
    // Returns a deduplicated pointer to the interned copy.
    // @ 0x820C29E0
    void* strHashValue_Intern(void* str);

    // Render state override array destructor — destroys an array of
    // render state override entries with mode flags.
    // @ 0x820EE988
    void grmRenderState_ArrayDtor(void* array, int flags);

    // Texture binding array destructor — destroys an array of texture
    // binding entries with mode flags.
    // @ 0x8224A680
    void grcTextureBinding_ArrayDtor(void* array, int flags);

    // Base effect parameter array destructor — destroys an array of shader
    // effect parameter entries (164 bytes each).
    // @ 0x8212DCE8
    void grmShaderParam_ArrayDtor(void* array, int flags);

    // Path normalization — copies src to dest, converting to lowercase
    // and replacing backslashes with forward slashes.
    // @ 0x820F0908 | size: 0x64
    void strNormalizePath(char* dest, const char* src, int maxLen);

    // Case-insensitive string comparison (CRT).
    int _stricmp(const char* s1, const char* s2);
}

namespace rage {


// ═══════════════════════════════════════════════════════════════════════════════
//  String Utilities
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * strAppend  @ 0x820F0888 | size: 0x7C
 *
 * Appends `src` to the end of `dest`, respecting `maxLen` as the total
 * buffer size.  Equivalent to a bounded strncat.  Always null-terminates.
 * If `dest` is NULL, the append starts at position 0.
 */
void strAppend(char* dest, const char* src, int maxLen)
{
    uint32_t destLen = 0;

    if (dest != nullptr) {
        const char* p = dest;
        while (*p != '\0')
            p++;
        destLen = static_cast<uint32_t>(p - dest);
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


// ═══════════════════════════════════════════════════════════════════════════════
//  datBase — Generic Child-Object Destructor
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * datBase_DestroyChild  @ 0x820C18B8 | size: 0x54
 *
 * Destroys a datBase-derived container.  If the object has a child at +4,
 * calls that child's virtual destructor (vtable slot 0, mode=1 = scalar
 * delete), then frees the container's own memory.
 */
struct datBaseContainer {
    void** vtable;   // +0x00
    void*  child;    // +0x04
};

void datBase_DestroyChild(datBaseContainer* self, int flags)
{
    if (self->child != nullptr) {
        void** childVtable = *reinterpret_cast<void***>(self->child);
        typedef void (*DtorFn)(void* obj, int flags);
        DtorFn dtor = reinterpret_cast<DtorFn>(childVtable[0]);
        dtor(self->child, 1);
    }
    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  sysThread — Thread Pool Entry Unlink and Destroy
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * sysThread_UnlinkAndDestroy  @ 0x820C88D8 | size: 0x80
 *
 * Removes a thread pool entry from the global singly-linked list
 * (g_threadpool_head), then calls sysThread_Cleanup and frees the entry.
 * Each node has a 'next' pointer at offset +0x14.
 */
struct ThreadPoolEntry {
    uint8_t          pad[20];  // +0x00
    ThreadPoolEntry* next;     // +0x14
};

extern ThreadPoolEntry* g_threadpool_head; // @ 0x825EBCBC

void sysThread_UnlinkAndDestroy(ThreadPoolEntry* self)
{
    if (g_threadpool_head != nullptr) {
        ThreadPoolEntry** linkPtr = &g_threadpool_head;

        while (*linkPtr != nullptr) {
            if (*linkPtr == self)
                break;
            linkPtr = &(*linkPtr)->next;
            if (*linkPtr == nullptr)
                break;
        }

        if (*linkPtr != nullptr) {
            *linkPtr = (*linkPtr)->next;
        }
    }

    sysThread_Cleanup(self);
    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderParam — Shader Parameter Value Management
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Shader parameter type tags:
 *   0 = none/cleared
 *   6 = texture reference (grcTexture*)
 *   8 = raw buffer (owned, freed directly)
 *   9 = raw buffer (owned, freed directly)
 */
struct grmShaderParam {
    void*    data;     // +0x00: parameter value / pointer to data
    uint8_t  pad[16];  // +0x04
    int32_t  type;     // +0x14: type tag (see above)
};

/**
 * grmShaderParam_ReleaseValue  @ 0x820F6348 | size: 0xC0
 *
 * Releases a shader parameter value based on its type tag.
 *   Type 6 (texture ref): released via grcTexture_Release
 *   Type 8/9 (raw buffers): freed directly via rage_free
 * The type is always reset to 0 after release.
 */
void grmShaderParam_ReleaseValue(grmShaderParam* self)
{
    int32_t type = self->type;

    if (type == 6) {
        if (self->data != nullptr) {
            grcTexture_Release(self->data);
            self->type = 0;
            return;
        }
    } else if (type == 8 || type == 9) {
        rage_free(self->data);
        self->type = 0;
        return;
    }

    self->type = 0;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderParamFull — Full Shader Parameter with Name + Ownership
// ═══════════════════════════════════════════════════════════════════════════════

struct grmShaderParamFull {
    void*    data;       // +0x00: parameter value
    uint32_t hash;       // +0x04: parameter name hash
    uint32_t flags;      // +0x08: usage flags
    uint32_t index;      // +0x0C: parameter index in shader
    void*    nameStr;    // +0x10: interned name string
    uint32_t typeTag;    // +0x14: type tag (same as grmShaderParam)
    uint8_t  pad_18[4];  // +0x18
    uint8_t  owned;      // +0x1C: 1 if this param owns its name string
};

/**
 * grmShaderParamFull_CopyFrom  @ 0x820F6408 | size: 0x7C
 *
 * Deep-copies a shader parameter from `src` to `self`.
 * Releases the destination's current value, frees its old name string,
 * copies all data fields, interns a new copy of the source name, and
 * marks the copy as owned.
 */
void grmShaderParamFull_CopyFrom(grmShaderParamFull* self,
                                  const grmShaderParamFull* src)
{
    grmShaderParam_ReleaseValue(reinterpret_cast<grmShaderParam*>(self));
    rage_free(self->nameStr);

    self->data    = src->data;
    self->hash    = src->hash;
    self->flags   = src->flags;
    self->index   = src->index;

    self->nameStr = strHashValue_Intern(src->nameStr);
    self->owned   = 1;
    self->typeTag = src->typeTag;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderFxNode — Shader FX Parameter Linked List
// ═══════════════════════════════════════════════════════════════════════════════

struct grmShaderFxNode {
    uint8_t          pad_00[16]; // +0x00: parameter data (overlaps grmShaderParam)
    void*            name;       // +0x10: interned name string
    uint8_t          pad_14[4];  // +0x14
    grmShaderFxNode* next;       // +0x18: next node in linked list
};

/**
 * grmShaderFxNode_DestroyRecursive  @ 0x820EF008 | size: 0x54
 *
 * Recursively destroys a shader FX node linked list.  For each node:
 *   1. Releases the parameter value (type-based cleanup)
 *   2. Frees the interned name string
 *   3. Recurses into the next node
 *   4. Frees the node itself
 */
void grmShaderFxNode_DestroyRecursive(grmShaderFxNode* self, int flags)
{
    grmShaderParam_ReleaseValue(reinterpret_cast<grmShaderParam*>(self));
    rage_free(self->name);

    if (self->next != nullptr) {
        grmShaderFxNode_DestroyRecursive(self->next, 1);
    }

    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderEffectBlock — Effect Parameter Block with 4 Sub-Arrays
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Layout of the 64-byte effect parameter block (4 sub-arrays):
 *   +0x00/+0x04/+0x0C : base effect parameters (grmShaderParam_ArrayDtor or rage_free)
 *   +0x10/+0x18       : texture bindings (grcTextureBinding_ArrayDtor)
 *   +0x1C/+0x24       : render state overrides (grmRenderState_ArrayDtor)
 *   +0x28/+0x30       : sampler states (freed directly)
 */
struct grmShaderEffectBlock {
    void*    dataPtr;          // +0x00: base parameter data
    void*    dataPtr2;         // +0x04: alternate base parameter pointer
    uint8_t  pad_08[4];        // +0x08
    int32_t  baseParamCount;   // +0x0C: count for base parameters
    void*    textureBindings;  // +0x10: texture binding array
    uint8_t  pad_14[4];        // +0x14
    int32_t  textureCount;     // +0x18: count for texture bindings
    void*    renderStates;     // +0x1C: render state override array
    uint8_t  pad_20[4];        // +0x20
    int32_t  renderStateCount; // +0x24: count for render states
    void*    samplerStates;    // +0x28: sampler state array
    uint8_t  pad_2C[4];        // +0x2C
    int32_t  samplerCount;     // +0x30: count for sampler states
};

/**
 * grmShaderEffectBlock_Destroy  @ 0x820EEA70 | size: 0xA4
 *
 * Destroys a shader effect parameter block.  Releases the 4 sub-arrays
 * in reverse order using type-specific destructors:
 *   1. Sampler states → rage_free
 *   2. Render state overrides → grmRenderState_ArrayDtor
 *   3. Texture bindings → grcTextureBinding_ArrayDtor
 *   4. Base parameters → grmShaderParam_ArrayDtor (or rage_free if no alt ptr)
 */
void grmShaderEffectBlock_Destroy(grmShaderEffectBlock* self)
{
    // Free sampler states
    if (self->samplerCount != 0) {
        rage_free(self->samplerStates);
    }

    // Destroy render state overrides
    if (self->renderStateCount != 0 && self->renderStates != nullptr) {
        grmRenderState_ArrayDtor(self->renderStates, 3);
    }

    // Destroy texture bindings
    if (self->textureCount != 0 && self->textureBindings != nullptr) {
        grcTextureBinding_ArrayDtor(self->textureBindings, 3);
    }

    // Destroy base parameters
    if (self->baseParamCount != 0) {
        if (self->dataPtr2 != nullptr) {
            grmShaderParam_ArrayDtor(self->dataPtr2, 3);
        } else {
            rage_free(self->dataPtr);
        }
    } else {
        rage_free(self->dataPtr);
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderEffectBlock Array Destructor
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * grmShaderEffectBlockArray_Destroy  @ 0x820EEA18 | size: 0x54
 *
 * Destroys a counted array of 64-byte grmShaderEffectBlock elements.
 * Count is stored at array[-1] (prepended count convention).
 * Iterates backwards, calling grmShaderEffectBlock_Destroy on each.
 */
void grmShaderEffectBlockArray_Destroy(void* array, int flags)
{
    uint32_t* countPtr = static_cast<uint32_t*>(array) - 1;
    uint32_t count = *countPtr;

    uint8_t* elem = static_cast<uint8_t*>(array) + count * 64;
    for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; i--) {
        elem -= 64;
        grmShaderEffectBlock_Destroy(
            reinterpret_cast<grmShaderEffectBlock*>(elem));
    }

    rage_free(countPtr);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmGeomIndexBuffer — Geometry Index Buffer Destructor
// ═══════════════════════════════════════════════════════════════════════════════

struct grmGeomIndexBuffer {
    void*    indexData;     // +0x00: pointer to index array
    uint8_t  pad_04[4];    // +0x04
    int32_t  entryCount;   // +0x08: number of entries
    void*    dataBuffer;   // +0x0C: raw data buffer
    uint8_t  pad_10[4];    // +0x10
    int32_t  ownsBuffer;   // +0x14: 1 if buffer is owned
};

/**
 * grmGeomIndexBuffer_Destroy  @ 0x820F12E0 | size: 0x68
 *
 * Destroys a geometry index buffer.  Frees the data buffer if owned,
 * then destroys the index data via the shader effect block array destructor.
 */
void grmGeomIndexBuffer_Destroy(grmGeomIndexBuffer* self)
{
    if (self->ownsBuffer != 0) {
        rage_free(self->dataBuffer);
    }

    if (self->entryCount != 0) {
        if (self->indexData == nullptr) {
            rage_free(self);
            return;
        }
        grmShaderEffectBlockArray_Destroy(self->indexData, 3);
    }

    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderPreset — Global Shader Preset Registry Lookup
// ═══════════════════════════════════════════════════════════════════════════════

struct grmShaderPresetEntry {
    uint8_t  pad[8];    // +0x00
    char*    name;      // +0x08: preset name string
    uint8_t  rest[16];  // +0x0C
};

// Global shader preset registry
extern int32_t              g_shaderPresetCount;  // @ 0x8260635C
extern grmShaderPresetEntry* g_shaderPresetArray; // @ 0x82606364

/**
 * grmShaderPreset_FindByName  @ 0x820F5FC0 | size: 0x80
 *
 * Searches the global shader preset registry for a preset matching the
 * given name (case-insensitive).  The name is first normalized to
 * lowercase with forward slashes via strNormalizePath.
 *
 * Returns the preset index on match, or -1 if not found.
 */
int32_t grmShaderPreset_FindByName(const char* name)
{
    char normalizedName[1024];
    strNormalizePath(normalizedName, name, 1024);

    for (int32_t i = 0; i < g_shaderPresetCount; i++) {
        if (_stricmp(normalizedName, g_shaderPresetArray[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

} // namespace rage
