/**
 * rage_system.cpp -- RAGE engine system-level utility functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements various RAGE subsystem helpers: threadpool node management,
 * dynamic array destructors, string utilities, resource cleanup, and
 * device state updates.
 */

#include <cstdint>
#include <cstring>

// External function declarations
extern "C" {
    void rage_free(void* ptr);                                       // @ 0x820C00C0
    void rage_threadpool_cleanup(void* node);                        // @ 0x82176878
    void rage_ReleaseResource(void* resource, int flags);            // @ 0x820C18B8
    void datBase_DestroyElement(void* element);                      // @ 0x820E3A50
    void datBase_DestroyArray8(void* obj);                           // @ 0x820E74B8
    void crAnimFrame_Dtor(void* obj);                                // @ 0x8224AC28
    void sysMemAllocator_Free(void* ptr, int alignment);             // @ 0x82186228
    void rage_ReleaseRef(void* ptr);                                 // @ 0x82352EE0
    void datBase_DestroyWithFlag(void* obj, int flags);              // @ 0x82156160
    void rage_UpdateDeviceState(void* devicePtr);                    // @ 0x82357400
    void rage_ApplyRenderState(void* device, void* state,
                               uint32_t flags, uint8_t forceApply);  // @ 0x820EE6E8
    uint8_t datBase_FindInPool(void* ptr);                           // @ 0x820F90D0
    void datBase_DestroyArrayEntries(void* array);                   // @ 0x820E3548
    void LocomotionController_DestroyAnimData(void* obj);            // @ 0x820E3D70
    void rage_ReleaseTextureRef(void* ptr, int flags);               // @ 0x820EE988
    void rage_ReleaseShaderGroup(void* ptr, int flags);              // @ 0x821AA680
    void rage_ReleaseMaterial(void* ptr, int flags);                 // @ 0x8212DCE8
    void rage_ReleaseModelData(void* ptr, int flags);                // @ 0x820EEA18
    void ph_CopyStringToBuffer(char* dest, const char* src,
                                int maxLen);                         // @ 0x820F0908
    int _stricmp(const char* s1, const char* s2);
}

// Threadpool linked list head @ 0x825EBCBC
extern void* g_threadpool_head;

// Device state globals
extern uint32_t g_lastRenderStateId;     // @ 0x825EB98C
extern void*    g_renderStateDevice;     // @ 0x825EB988

// Shader lookup table globals
extern int32_t  g_shaderEntryCount;      // @ 0x8260635C (SDA +25436)
extern void*    g_shaderEntryTable;      // @ 0x82606364 (SDA +25444, g_audio_obj_ptr alias)

// ICC profile pointer @ 0x8260634C
extern void*    g_iccProfilePtr;

// SDA base global @ 0x82600000
extern void*    g_sdaBasePtr;

// ─────────────────────────────────────────────────────────────────────────────
// Named vtable constants (from .rdata, RTTI-resolved)
// ─────────────────────────────────────────────────────────────────────────────
static void* const kVtable_rage_datBase        = (void*)0x820276C4;  // rage::datBase
static void* const kVtable_rage_crAnimFrame    = (void*)0x8204FCBC;  // rage::crAnimFrame
static void* const kVtable_rage_grmShaderPreset= (void*)0x8202F668;  // rage::grmShaderPreset
static void* const kVtable_msgMsgSink          = (void*)0x82027B34;  // msgMsgSink

// ─────────────────────────────────────────────────────────────────────────────
// rage_UnlinkAndFreeThreadpoolNode @ 0x820C88D8 | size: 0x80
//
// Removes a node from the global threadpool linked list, calls threadpool
// cleanup, then frees the node memory. Each node has a 'next' pointer at
// offset +20, forming a singly-linked list rooted at g_threadpool_head.
// ─────────────────────────────────────────────────────────────────────────────
void rage_UnlinkAndFreeThreadpoolNode(void* node) {
    // Walk the linked list to find and unlink this node
    void** current = (void**)&g_threadpool_head;
    void* head = *current;

    if (head != nullptr) {
        // Traverse until we find our node or reach end of list
        while (*current != nullptr) {
            if (*current == node) {
                // Unlink: skip over this node by pointing to its next
                void* nextNode = *(void**)((uint8_t*)(*current) + 20);
                *current = nextNode;
                break;
            }
            // Advance: next pointer is at offset +20
            current = (void**)((uint8_t*)(*current) + 20);
        }
    }

    // Clean up threadpool resources associated with this node
    rage_threadpool_cleanup(node);

    // Free the node memory
    rage_free(node);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_DestroyArray68 @ 0x820E7520 | size: 0x68
//
// Destroys a dynamic array whose elements are 68 bytes each. The array
// metadata (element count) is stored at offset -4 from the data pointer.
// Iterates backward through elements calling the element destructor,
// then frees the backing memory.
//
// Parameters:
//   arrayObj - Pointer to an 8-byte structure:
//              +0: data pointer (array of 68-byte elements)
//              +6: reference count (uint16_t)
// ─────────────────────────────────────────────────────────────────────────────
void rage_DestroyArray68(void* arrayObj) {
    uint16_t refCount = *(uint16_t*)((uint8_t*)arrayObj + 6);
    if (refCount == 0) {
        return;
    }

    void* dataPtr = *(void**)arrayObj;
    if (dataPtr == nullptr) {
        return;
    }

    // Element count is stored 4 bytes before the data pointer
    uint32_t* countPtr = (uint32_t*)((uint8_t*)dataPtr - 4);
    uint32_t elementCount = *countPtr;

    // Calculate end of array
    uint8_t* elementPtr = (uint8_t*)dataPtr + elementCount * 68;

    // Destroy elements in reverse order
    for (int32_t i = elementCount - 1; i >= 0; i--) {
        elementPtr -= 68;
        datBase_DestroyElement(elementPtr);
    }

    // Free the array (including the count prefix)
    rage_free(countPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_DestroyArray104WithRelease @ 0x820E7588 | size: 0x9C
//
// Destroys a dynamic array of 104-byte elements. Each element contains a
// sub-object at offset +64 that may need reference release. The array
// metadata is at offset -4 from the data pointer.
//
// For each element (iterating backward):
//   1. Check refCount at element+6
//   2. If the sub-object at element+0 exists and is not found in the pool,
//      release it via the global allocator's vtable slot 2
//
// Parameters:
//   dataPtr - Pointer to the first element of the array
// ─────────────────────────────────────────────────────────────────────────────
void rage_DestroyArray104WithRelease(void* dataPtr) {
    // Element count is stored 4 bytes before the data pointer
    uint32_t* countPtr = (uint32_t*)((uint8_t*)dataPtr - 4);
    uint32_t elementCount = *countPtr;
    int32_t index = elementCount - 1;

    if (index >= 0) {
        // Start at the last element's sub-object (offset +64 from end)
        uint8_t* subObj = (uint8_t*)dataPtr + elementCount * 104 + 64;

        while (index >= 0) {
            subObj -= 104;

            uint16_t refCount = *(uint16_t*)(subObj + 6);
            if (refCount != 0) {
                void* innerPtr = *(void**)subObj;
                if (innerPtr != nullptr) {
                    // Check if this object is in the global pool
                    uint8_t found = datBase_FindInPool(innerPtr);
                    if (found == 0) {
                        // Not in pool - release via global allocator vtable slot 2
                        void* sdaBase = *(void**)((uint8_t*)&g_sdaBasePtr);
                        void* allocator = *(void**)((uint8_t*)sdaBase + 4);
                        void** vtable = *(void***)allocator;
                        typedef void (*FreeFn)(void*, void*);
                        FreeFn freeFn = (FreeFn)vtable[2];
                        freeFn(allocator, innerPtr);
                    }
                }
            }

            index--;
        }
    }

    // Free the array backing memory
    rage_free(countPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_crAnimFrame_Dtor @ 0x820EB628 | size: 0x64
//
// Destructor for a rage::crAnimFrame-derived object. Releases the resource
// at offset +16, resets vtable to crAnimFrame base, calls crAnimFrame
// destructor, then conditionally frees the sub-array at offset +8.
// Called by pcrSwingBlender::~pcrSwingBlender and pcrServeBlender dtor.
// ─────────────────────────────────────────────────────────────────────────────
void rage_crAnimFrame_Dtor(void* obj) {
    // Release the resource pointer at offset +16 if present
    void* resource = *(void**)((uint8_t*)obj + 16);
    if (resource != nullptr) {
        rage_ReleaseResource(resource, 1);
    }

    // Reset vtable to rage::crAnimFrame base
    *(void**)obj = kVtable_rage_crAnimFrame;

    // Call crAnimFrame base destructor
    crAnimFrame_Dtor(obj);

    // Free sub-array at offset +8 if it has a reference count
    uint16_t subRefCount = *(uint16_t*)((uint8_t*)obj + 14);
    if (subRefCount != 0) {
        void* subArray = *(void**)((uint8_t*)obj + 8);
        rage_free(subArray);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_CleanupRenderResources @ 0x820EC748 | size: 0x8C
//
// Frees multiple render resource pointers from an object, each with a
// different deallocation strategy:
//   +8:  rage_free (simple heap free)
//   +12: sysMemAllocator_Free with 16-byte alignment
//   +16: conditional ref release (checks bit 24 flag)
//   +20: datBase destructor with flag=1
// Finally frees the object itself.
// ─────────────────────────────────────────────────────────────────────────────
void rage_CleanupRenderResources(void* obj) {
    // Free buffer at offset +8
    void* buffer = *(void**)((uint8_t*)obj + 8);
    if (buffer != nullptr) {
        rage_free(buffer);
    }

    // Free aligned memory at offset +12
    void* alignedMem = *(void**)((uint8_t*)obj + 12);
    if (alignedMem != nullptr) {
        sysMemAllocator_Free(alignedMem, 16);
    }

    // Conditionally release ref-counted object at offset +16
    void* refObject = *(void**)((uint8_t*)obj + 16);
    if (refObject != nullptr) {
        uint32_t flags = *(uint32_t*)refObject;
        // Check if bit 24 is set (owned reference flag)
        if ((flags & 0x01000000) != 0) {
            rage_ReleaseRef(refObject);
        }
    }

    // Destroy managed object at offset +20
    void* managedObj = *(void**)((uint8_t*)obj + 20);
    if (managedObj != nullptr) {
        datBase_DestroyWithFlag(managedObj, 1);
    }

    // Free the container object itself
    rage_free(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_ApplyDeviceRenderState @ 0x820EE910 | size: 0x74
//
// Applies render state changes to the graphics device. Compares the current
// state ID with the cached last-applied ID; if different, updates the device.
// Then assembles parameters from the object and calls the full render state
// application function.
//
// Called by grmModelGeom's scalar destructor and game rendering code.
// ─────────────────────────────────────────────────────────────────────────────
void rage_ApplyDeviceRenderState(void* stateObj) {
    uint32_t stateId = *(uint32_t*)((uint8_t*)stateObj + 4);

    // Only update device if state ID has changed
    if (stateId != g_lastRenderStateId) {
        g_lastRenderStateId = stateId;
        rage_UpdateDeviceState(g_renderStateDevice);
    }

    // Check if ICC profile override is active
    uint8_t hasIccOverride = *(uint8_t*)((uint8_t*)stateObj + 37);
    void* iccOverride = nullptr;
    if (hasIccOverride != 0) {
        iccOverride = g_iccProfilePtr;
    }

    // Gather remaining render state parameters
    uint8_t forceApply = *(uint8_t*)((uint8_t*)stateObj + 39);
    uint32_t renderFlags = *(uint32_t*)((uint8_t*)stateObj + 32);
    void* devicePtr = *(void**)stateObj;

    // Apply the full render state
    rage_ApplyRenderState(devicePtr, iccOverride, renderFlags, forceApply);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_DestroyMultiResourceObject @ 0x820EEA70 | size: 0xA4
//
// Cascading destructor that releases up to four different resource types
// from an object, then frees the object's root data pointer. Each resource
// has an ownership flag at a separate offset that controls whether the
// resource should be released.
//
// Layout:
//   +0:  root data pointer          +12: root data ownership flag
//   +4:  material pointer           (no separate ownership - always checked)
//   +16: shader group pointer       +24: shader group ownership flag
//   +28: texture ref pointer        +36: texture ref ownership flag
//   +40: vertex buffer pointer      +48: vertex buffer ownership flag
// ─────────────────────────────────────────────────────────────────────────────
void rage_DestroyMultiResourceObject(void* obj) {
    // Release vertex buffer at offset +40 if ownership flag (+48) is set
    int32_t ownsVertexBuffer = *(int32_t*)((uint8_t*)obj + 48);
    if (ownsVertexBuffer != 0) {
        void* vertexBuffer = *(void**)((uint8_t*)obj + 40);
        rage_free(vertexBuffer);
    }

    // Release texture ref at offset +28 if ownership flag (+36) is set
    int32_t ownsTextureRef = *(int32_t*)((uint8_t*)obj + 36);
    if (ownsTextureRef != 0) {
        void* textureRef = *(void**)((uint8_t*)obj + 28);
        if (textureRef != nullptr) {
            rage_ReleaseTextureRef(textureRef, 3);
        }
    }

    // Release shader group at offset +16 if ownership flag (+24) is set
    int32_t ownsShaderGroup = *(int32_t*)((uint8_t*)obj + 24);
    if (ownsShaderGroup != 0) {
        void* shaderGroup = *(void**)((uint8_t*)obj + 16);
        if (shaderGroup != nullptr) {
            rage_ReleaseShaderGroup(shaderGroup, 3);
        }
    }

    // Release material data at offset +4 if ownership flag (+12) is set
    int32_t ownsMaterial = *(int32_t*)((uint8_t*)obj + 12);
    if (ownsMaterial != 0) {
        void* materialPtr = *(void**)((uint8_t*)obj + 4);
        if (materialPtr == nullptr) {
            // No material - just free the root data and return
            void* rootData = *(void**)obj;
            rage_free(rootData);
            return;
        }
        rage_ReleaseMaterial(materialPtr, 3);
    }

    // Free root data pointer at offset +0
    void* rootData = *(void**)obj;
    rage_free(rootData);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_DestroyAnimElement @ 0x820E39C0 | size: 0x90
//
// Destroys a single animation element that contains four dynamically
// allocated sub-arrays at offsets +8, +16, +24, and +32. Each sub-array
// has a corresponding uint16 ownership flag at offsets +14, +22, +30, +38.
// If ownership flag is set, the sub-array memory is freed.
// The sub-array at +8 may contain 104-byte elements requiring special
// release logic via rage_DestroyArray104WithRelease.
//
// Called by rage_3D70 (LocomotionController destructor chain) and
// rage_DestroyArray68.
// ─────────────────────────────────────────────────────────────────────────────
void rage_DestroyAnimElement(void* element) {
    // First call the inner element destructor
    datBase_DestroyElement(element);

    // Free sub-array at offset +32 if owned
    uint16_t ownsArray3 = *(uint16_t*)((uint8_t*)element + 38);
    if (ownsArray3 != 0) {
        void* array3 = *(void**)((uint8_t*)element + 32);
        rage_free(array3);
    }

    // Free sub-array at offset +24 if owned
    uint16_t ownsArray2 = *(uint16_t*)((uint8_t*)element + 30);
    if (ownsArray2 != 0) {
        void* array2 = *(void**)((uint8_t*)element + 24);
        rage_free(array2);
    }

    // Free sub-array at offset +16 if owned
    uint16_t ownsArray1 = *(uint16_t*)((uint8_t*)element + 22);
    if (ownsArray1 != 0) {
        void* array1 = *(void**)((uint8_t*)element + 16);
        rage_free(array1);
    }

    // Free sub-array at offset +8 if owned (special handling for 104-byte elements)
    uint16_t ownsArray0 = *(uint16_t*)((uint8_t*)element + 14);
    if (ownsArray0 != 0) {
        void* array0 = *(void**)((uint8_t*)element + 8);
        if (array0 == nullptr) {
            // No data - just call the base cleanup and return
            datBase_DestroyArray8(element);
            return;
        }
        // Array contains 104-byte elements that need reference release
        rage_DestroyArray104WithRelease(array0);
    }

    // Final cleanup of the element itself
    datBase_DestroyArray8(element);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_StringAppend @ 0x820F0888 | size: 0x7C
//
// Appends a source string to a destination buffer, respecting a maximum
// buffer size. Finds the end of the existing string in dest, then copies
// characters from src until either src is exhausted or the buffer is full.
// Always null-terminates the result.
//
// Parameters:
//   dest    - Destination buffer (may already contain a string)
//   src     - Source string to append
//   maxLen  - Maximum total buffer size including null terminator
// ─────────────────────────────────────────────────────────────────────────────
void rage_StringAppend(char* dest, const char* src, int maxLen) {
    // Find the length of the existing string in dest
    int destLen = 0;
    if (dest != nullptr) {
        const char* p = dest;
        while (*p != '\0') {
            p++;
        }
        destLen = (int)(p - dest);
    }

    // Calculate remaining space (minus 1 for null terminator)
    int remaining = maxLen - destLen - 1;
    char* writePos = dest + destLen;

    // Copy characters from src while there is space
    while (*src != '\0') {
        if (remaining <= 0) {
            break;
        }
        *writePos = *src;
        src++;
        writePos++;
        remaining--;
    }

    // Null-terminate
    *writePos = '\0';
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_DestroyModelGeomData @ 0x820F12E0 | size: 0x68
//
// Cleans up a model geometry data structure. Frees the sub-data at offset +12
// if the ownership flag at offset +20 is set, then releases the root object
// at offset +0 via rage_ReleaseModelData if ownership flag at offset +8 is
// set. Finally frees the container itself.
//
// Called by rage_1028, rage_44F8, rage_47C0 (model geometry builders).
// ─────────────────────────────────────────────────────────────────────────────
void rage_DestroyModelGeomData(void* obj) {
    // Free sub-data at offset +12 if ownership flag (+20) is set
    int32_t ownsSubData = *(int32_t*)((uint8_t*)obj + 20);
    if (ownsSubData != 0) {
        void* subData = *(void**)((uint8_t*)obj + 12);
        rage_free(subData);
    }

    // Release root data at offset +0 if ownership flag (+8) is set
    int32_t ownsRootData = *(int32_t*)((uint8_t*)obj + 8);
    if (ownsRootData != 0) {
        void* rootData = *(void**)obj;
        if (rootData == nullptr) {
            // No root data - just free the container
            rage_free(obj);
            return;
        }
        rage_ReleaseModelData(rootData, 3);
    }

    // Free the container
    rage_free(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// Additional external declarations for new functions
// ─────────────────────────────────────────────────────────────────────────────
extern "C" {
    void grmShaderParam_ReleaseValues(void* obj);                    // @ 0x820F6348
    void cmOperator_CopyFrom(void* dest, void* src);                // @ 0x82126998
    void grmShaderFx_Destroy(void* obj, int flags);                  // @ 0x820EF008 (self-recursive)
    void msgMsgSink_Cleanup(void* obj);                              // @ 0x821A8070
    void atSingleton_DestroyContents(void* obj);                     // @ 0x8214BE18
    void* parMember_LookupStruct(void* obj, void* r4_param);        // @ 0x8234B968
    void* parMember_FindField(void* obj, void* structDef);           // @ 0x8234BBC0
    void parMember_StoreField(void* obj, void* value);               // @ 0x8234D620
    void parMember_CleanupContext(void* ctx);                        // @ 0x8234AEA0
    void rage_DecrefAndRelease(void* obj);                           // @ 0x820C2E18
    void grmShaderPtrArray_DestroyEntries(void* dataPtr, int flags); // @ 0x821240F8 (forward decl)
}


// ─────────────────────────────────────────────────────────────────────────────
// grmShaderParam_ReleaseByType @ 0x820F6408 | size: 0x7C
//
// Releases the data pointer held by a shader parameter based on its type tag.
// Type 6: managed reference release. Type 8/9: simple heap free.
// Any other type: just clears the type tag.
//
// Layout:
//   +0:  data pointer
//   +20: type tag (uint32_t)
// ─────────────────────────────────────────────────────────────────────────────
void grmShaderParam_ReleaseByType(void* param) {
    uint32_t type = *(uint32_t*)((uint8_t*)param + 20);

    if (type == 6) {
        // Managed reference - release through ref manager
        void* data = *(void**)param;
        if (data != nullptr) {
            rage_DecrefAndRelease(data);
        }
        *(uint32_t*)((uint8_t*)param + 20) = 0;
        return;
    }

    if (type == 8) {
        void* data = *(void**)param;
        rage_free(data);
        *(uint32_t*)((uint8_t*)param + 20) = 0;
        return;
    }

    if (type == 9) {
        void* data = *(void**)param;
        rage_free(data);
        *(uint32_t*)((uint8_t*)param + 20) = 0;
        return;
    }

    // Unknown or no-op type - just clear
    *(uint32_t*)((uint8_t*)param + 20) = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// grmShaderFx_Destroy @ 0x820EF008 | size: 0x54
//
// Destructor for rage::grmShaderFx. Releases shader parameters, frees the
// parameter data buffer at +16, recursively destroys the linked sibling
// shader at +24, then frees the object itself.
//
// Called by grmShaderFx_vfn_0, grmShaderPreset_Destroy, grmShaderBasicClamped.
// ─────────────────────────────────────────────────────────────────────────────
void grmShaderFx_Destroy(void* obj, int flags) {
    // Release all shader parameter values
    grmShaderParam_ReleaseValues(obj);

    // Free the parameter data buffer at offset +16
    void* paramData = *(void**)((uint8_t*)obj + 16);
    rage_free(paramData);

    // Recursively destroy linked sibling shader at offset +24
    void* sibling = *(void**)((uint8_t*)obj + 24);
    if (sibling != nullptr) {
        grmShaderFx_Destroy(sibling, 1);
    }

    // Free the shader object itself
    rage_free(obj);
}

// ─────────────────────────────────────────────────────────────────────────────
// grmShaderPreset_Destroy @ 0x820F5710 | size: 0x84
//
// Destructor for rage::grmShaderPreset. Resets vtable to grmShaderPreset,
// frees internal buffers, destroys the linked shader FX, releases the
// sub-object at +12 via virtual destructor, then resets vtable to datBase.
//
// Layout:
//   +0:  vtable pointer
//   +4:  sub-object pointer (with virtual dtor)
//   +8:  buffer pointer (freed directly)
//   +12: sub-object pointer (with virtual dtor)
//   +20: buffer pointer (freed directly)
// ─────────────────────────────────────────────────────────────────────────────
void grmShaderPreset_Destroy(void* obj) {
    // Set vtable to rage::grmShaderPreset
    *(void**)obj = kVtable_rage_grmShaderPreset;

    // Free buffer at offset +8
    void* buffer1 = *(void**)((uint8_t*)obj + 8);
    rage_free(buffer1);

    // Free buffer at offset +20
    void* buffer2 = *(void**)((uint8_t*)obj + 20);
    rage_free(buffer2);

    // Destroy linked shader FX at offset +4
    void* shaderFx = *(void**)((uint8_t*)obj + 4);
    if (shaderFx != nullptr) {
        grmShaderFx_Destroy(shaderFx, 1);
    }

    // Release sub-object at offset +12 via virtual destructor (slot 0)
    void* subObj = *(void**)((uint8_t*)obj + 12);
    if (subObj != nullptr) {
        void** vtable = *(void***)subObj;
        typedef void (*DtorFn)(void*, int);
        DtorFn dtorFn = (DtorFn)vtable[0];
        dtorFn(subObj, 1);
    }

    // Reset vtable to rage::datBase
    *(void**)obj = kVtable_rage_datBase;
}

// ─────────────────────────────────────────────────────────────────────────────
// atArray_ClearOwned16 @ 0x82122C00 | size: 0x60
//
// Clears a 20-byte atArray-like structure with an ownership byte at +16.
// If the top bit of the ownership byte is set, frees the backing data
// at +0 before clearing. Preserves the low 7 bits of the ownership byte.
//
// Layout:
//   +0:  data pointer
//   +4:  (uint16_t) count
//   +6:  (uint16_t) capacity
//   +12: (uint32_t) aux field
//   +16: (uint8_t) ownership flags (bit 7 = owns data)
// ─────────────────────────────────────────────────────────────────────────────
void atArray_ClearOwned16(void* obj) {
    uint8_t flags = *(uint8_t*)((uint8_t*)obj + 16);

    // Check if ownership bit (any of bits 1-7) is set
    if ((flags & 0x80) != 0) {
        void* data = *(void**)obj;
        rage_free(data);
    }

    // Preserve only the low 7 bits of flags
    uint8_t preservedFlags = *(uint8_t*)((uint8_t*)obj + 16) & 0x7F;

    // Clear all fields
    *(void**)obj = nullptr;
    *(uint16_t*)((uint8_t*)obj + 4) = 0;
    *(uint16_t*)((uint8_t*)obj + 6) = 0;
    *(uint32_t*)((uint8_t*)obj + 12) = 0;
    *(uint8_t*)((uint8_t*)obj + 16) = preservedFlags;
}

// ─────────────────────────────────────────────────────────────────────────────
// atArray_ClearOwned20 @ 0x82125D60 | size: 0x60
//
// Clears a 24-byte atArray-like structure with an ownership byte at +20.
// Same pattern as atArray_ClearOwned16 but with wider layout (includes
// additional uint32 fields at +4, +8, and +16).
//
// Layout:
//   +0:  data pointer
//   +4:  (uint32_t) field1
//   +8:  (uint32_t) field2
//   +16: (uint32_t) field3
//   +20: (uint8_t) ownership flags (bit 7 = owns data)
// ─────────────────────────────────────────────────────────────────────────────
void atArray_ClearOwned20(void* obj) {
    uint8_t flags = *(uint8_t*)((uint8_t*)obj + 20);

    // Check if ownership bit (any of bits 1-7) is set
    if ((flags & 0x80) != 0) {
        void* data = *(void**)obj;
        rage_free(data);
    }

    // Preserve only the low 7 bits of flags
    uint8_t preservedFlags = *(uint8_t*)((uint8_t*)obj + 20) & 0x7F;

    // Clear all fields
    *(void**)obj = nullptr;
    *(uint32_t*)((uint8_t*)obj + 4) = 0;
    *(uint32_t*)((uint8_t*)obj + 8) = 0;
    *(uint32_t*)((uint8_t*)obj + 16) = 0;
    *(uint8_t*)((uint8_t*)obj + 20) = preservedFlags;
}

// ─────────────────────────────────────────────────────────────────────────────
// grmShaderGroup_DestroyOwnedArrays @ 0x82124020 | size: 0x5C
//
// Destroys owned sub-arrays from a shader group structure. Frees the
// secondary array at +12 if owned (flag at +18), then destroys the
// primary array of pointers at +0 if owned (flag at +6) via
// grmShaderPtrArray_DestroyEntries.
//
// Layout:
//   +0:  primary pointer array (array of object pointers)
//   +6:  (uint16_t) primary ownership flag
//   +12: secondary data array
//   +18: (uint16_t) secondary ownership flag
// ─────────────────────────────────────────────────────────────────────────────
void grmShaderGroup_DestroyOwnedArrays(void* obj) {
    // Free secondary array at +12 if owned
    uint16_t ownsSecondary = *(uint16_t*)((uint8_t*)obj + 18);
    if (ownsSecondary != 0) {
        void* secondaryArray = *(void**)((uint8_t*)obj + 12);
        rage_free(secondaryArray);
    }

    // Destroy primary pointer array at +0 if owned
    uint16_t ownsPrimary = *(uint16_t*)((uint8_t*)obj + 6);
    if (ownsPrimary != 0) {
        void* primaryArray = *(void**)obj;
        if (primaryArray == nullptr) {
            return;
        }
        // Destroy entries in the pointer array with cleanup flag 3
        grmShaderPtrArray_DestroyEntries(primaryArray, 3);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// grmShaderPtrArray_DestroyEntries @ 0x821240F8 | size: 0x6C
//
// Destroys all non-null entries in an array of object pointers. The element
// count is stored at offset -4 from the data pointer. Each entry is a
// pointer to an object with a virtual destructor at vtable slot 0.
// Iterates backward through the array calling the virtual destructor
// with flags=1, then frees the array backing memory.
//
// Parameters:
//   dataPtr - Pointer to the first element of the pointer array
// ─────────────────────────────────────────────────────────────────────────────
void grmShaderPtrArray_DestroyEntries(void* dataPtr, int flags) {
    // Element count is stored 4 bytes before the data pointer
    uint32_t* countPtr = (uint32_t*)((uint8_t*)dataPtr - 4);
    uint32_t elementCount = *countPtr;

    // Calculate end of array (each element is a 4-byte pointer)
    void** elementPtr = (void**)((uint8_t*)dataPtr + elementCount * 4);

    // Destroy elements in reverse order
    for (int32_t i = elementCount - 1; i >= 0; i--) {
        elementPtr--;
        void* entry = *elementPtr;
        if (entry != nullptr) {
            // Call virtual destructor (vtable slot 0) with flags=1
            void** vtable = *(void***)entry;
            typedef void (*DtorFn)(void*, int);
            DtorFn dtorFn = (DtorFn)vtable[0];
            dtorFn(entry, 1);
        }
    }

    // Free the array backing memory (including count prefix)
    rage_free(countPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// cmOperator_Assign @ 0x82126A30 | size: 0x5C
//
// Assignment operator for a cmOperator object. If the source and destination
// are different, frees the destination's current data, clears it, then
// copies from the source using cmOperator_CopyFrom. Returns a pointer
// to the destination.
// ─────────────────────────────────────────────────────────────────────────────
void* cmOperator_Assign(void* dest, void* src) {
    if (dest != src) {
        // Free existing data in destination
        void* data = *(void**)dest;
        rage_free(data);

        // Clear destination
        *(void**)dest = nullptr;

        // Copy from source
        cmOperator_CopyFrom(dest, src);
    }

    return dest;
}

// ─────────────────────────────────────────────────────────────────────────────
// datRef_Release @ 0x82126A90 | size: 0x80
//
// Decrements the reference count of a ref-counted object at offset +32.
// When the count reaches zero, calls the content destructor, frees the
// owned sub-array at +24 if flagged, then frees the object itself.
// Returns the new reference count (0 if freed).
//
// Layout:
//   +24: sub-array data pointer
//   +30: (uint16_t) sub-array ownership flag
//   +32: (uint16_t) reference count
// ─────────────────────────────────────────────────────────────────────────────
uint16_t datRef_Release(void* obj) {
    // Decrement reference count at +32 (with wrapping via +65536-1 then mask)
    uint16_t refCount = *(uint16_t*)((uint8_t*)obj + 32);
    refCount = (uint16_t)((refCount + 65536 - 1) & 0xFFFF);
    *(uint16_t*)((uint8_t*)obj + 32) = refCount;

    if (refCount == 0) {
        // Reference count hit zero - destroy the object
        atSingleton_DestroyContents(obj);

        // Free owned sub-array at +24
        uint16_t ownsSubArray = *(uint16_t*)((uint8_t*)obj + 30);
        if (ownsSubArray != 0) {
            void* subArray = *(void**)((uint8_t*)obj + 24);
            rage_free(subArray);
        }

        // Free the object itself
        rage_free(obj);
        return 0;
    }

    return refCount;
}

// ─────────────────────────────────────────────────────────────────────────────
// msgMsgSink_DestroySubObjects @ 0x82116420 | size: 0x6C
//
// Destructor helper for a msgMsgSink-derived object. Frees the owned
// sub-array at +24 if flagged, cleans up the embedded sub-object at +8
// via msgMsgSink_Cleanup, then resets vtables. The object has an inner
// msgMsgSink at +4 and an outer datBase wrapper at +0.
//
// Layout:
//   +0:  datBase vtable pointer
//   +4:  msgMsgSink vtable pointer (inner sub-object)
//   +8:  embedded sub-object (cleaned up via msgMsgSink_Cleanup)
//   +24: sub-array data pointer
//   +30: (uint16_t) sub-array ownership flag
// ─────────────────────────────────────────────────────────────────────────────
void msgMsgSink_DestroySubObjects(void* obj) {
    // Free owned sub-array at +24
    uint16_t ownsSubArray = *(uint16_t*)((uint8_t*)obj + 30);
    if (ownsSubArray != 0) {
        void* subArray = *(void**)((uint8_t*)obj + 24);
        rage_free(subArray);
    }

    // Clean up embedded sub-object at +8
    void* subObj = (void*)((uint8_t*)obj + 8);
    msgMsgSink_Cleanup(subObj);

    // Compute inner vtable pointer (+4 from obj, or nullptr if obj is null)
    void* innerPtr = (obj != nullptr) ? (void*)((uint8_t*)obj + 4) : nullptr;

    // Reset inner vtable to msgMsgSink base
    *(void**)innerPtr = kVtable_msgMsgSink;

    // Reset outer vtable to rage::datBase
    *(void**)obj = kVtable_rage_datBase;
}

// ─────────────────────────────────────────────────────────────────────────────
// parMember_DeserializeField @ 0x82123F18 | size: 0x8C
//
// Deserializes a single field from a parMember structure. Looks up the
// struct definition via parMember_LookupStruct, finds the field within
// the struct using parMember_FindField, reads the field value by calling
// the member's virtual read function (vtable slot 3), stores the value
// into the output pointer, then calls the member's virtual apply function
// (vtable slot 1). Returns 1 on success.
//
// Parameters:
//   obj    - The parMember instance
//   r4     - Serialization context
//   outPtr - Pointer to receive the deserialized value
// ─────────────────────────────────────────────────────────────────────────────
int parMember_DeserializeField(void* obj, void* r4Param, void* outPtr) {
    // Look up the struct definition
    void* structCtx = parMember_LookupStruct(obj, r4Param);

    // Get the struct definition from the context
    void* structDef = *(void**)structCtx;

    // Find the specific field within the struct
    void* field = parMember_FindField(obj, structDef);

    // Read the field value using the field's virtual function at +28 (slot 3 at offset 12)
    void** fieldBase = (void**)((uint8_t*)field + 28);
    uint32_t readFnAddr = *(uint32_t*)((uint8_t*)fieldBase + 12);
    typedef void* (*ReadFn)(void*);
    ReadFn readFn = (ReadFn)readFnAddr;
    void* value = readFn(field);

    // Store the deserialized value into the output
    parMember_StoreField(field, value);
    *(void**)outPtr = value;

    // Apply the value via the member's vtable slot 1
    void** vtable = *(void***)field;
    typedef void (*ApplyFn)(void*, void*, void*);
    ApplyFn applyFn = (ApplyFn)vtable[1];
    applyFn(field, structDef, value);

    // Clean up the lookup context
    parMember_CleanupContext(structCtx);
    rage_free(structCtx);

    return 1;
}
