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

    // Reset vtable to rage::crAnimFrame base @ 0x8204FCBC
    *(void**)obj = (void*)0x8204FCBC;

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
