/**
 * rage_engine_dtors.cpp — RAGE Engine Destructor and Cleanup Collection
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Cross-subsystem destructor implementations that span multiple RAGE modules:
 *
 *   datBase:
 *     - datBase_DestroyChild          — generic child-object destructor wrapper
 *
 *   datResource:
 *     - datResource_ReleaseBuffer     — network-aware resource buffer release
 *
 *   grmShaderGroup (Graphics):
 *     - grmShaderGroup_ArrayDtor64    — array dtor for 64-byte shader elements
 *     - grmShaderGroup_ArrayDtor68    — array dtor for 68-byte shader elements
 *     - grmModelGeom_Destroy          — graphics model geometry destructor
 *     - grmShaderGroup_Cleanup        — resource cleanup (buffer + pointer array)
 *     - grmDataBlock_CopyAssign       — copy assignment for managed resources
 *
 *   crAnimFrame (Animation):
 *     - crAnimFrame_Destroy           — animation frame destructor
 *
 *   crIKBodyBoneFinderByName (Animation/IK):
 *     - crIKBodyBoneFinderByName_Dtor — IK bone finder destructor
 *
 *   sysThread (Threading):
 *     - sysThread_UnlinkAndDestroy    — thread pool unlink + cleanup
 */
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

// RAGE heap free.
// @ 0x820C00C0
extern "C" void rage_free(void* ptr);

// datResource cleanup — tears down resource references and tracking data.
// @ 0x8214C788 | size: 0xF4
extern "C" void datResource_Cleanup(void* self);

// Network buffer release — releases a network-allocated buffer.
// mode=3 means "release with deferred cleanup".
// @ 0x82115B40 | size: 0x98
extern "C" void netBuffer_Release(void* buffer, int mode);

// grmShaderGroup element destructor — destroys a single 68-byte shader element.
// @ 0x820E39C0 | size: 0x90
extern "C" void grmShaderGroup_DestroyElement68(void* element);

// grmShaderGroup element destructor — destroys a single 64-byte shader element.
// @ 0x820EEA70 | size: 0xA4
extern "C" void grmShaderGroup_DestroyElement64(void* element);

// crAnimFrame field cleanup — tears down animation clip references.
// @ 0x8224AC28 | size: 0x74
extern "C" void crAnimFrame_CleanupFields(void* self);

// Thread pool per-thread cleanup — tears down thread state (508 bytes).
// @ 0x82176878 | size: 0x1FC
extern "C" void sysThread_Cleanup(void* thread);

// grmShaderGroup pointer-array destructor — iterates pointer array and
// destroys each entry, then frees.
// @ 0x821240F8 | size: 0x6C
extern "C" void grmShaderGroup_PtrArrayDtor(void* arrayBase, int mode);

// Managed resource copy assignment helper — deep-copies resource data.
// @ 0x82126998 | size: 0x94
extern "C" void grmDataBlock_CopyConstruct(void* dst, void* src);

// ─────────────────────────────────────────────────────────────────────────────
// Globals
// ─────────────────────────────────────────────────────────────────────────────

// Thread pool linked-list head — intrusive list of active sysThread objects.
// Each node has a 'next' pointer at offset +20.
// @ 0x825EBCBC (.data, 36 bytes)
extern "C" uint32_t* g_threadpool_head;


// ═══════════════════════════════════════════════════════════════════════════════
//  datBase — Generic Object Destructor
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * datBase_DestroyChild  @ 0x820C18B8 | size: 0x54
 *
 * Generic destructor wrapper.  Reads the child object pointer at self+4,
 * calls its virtual destructor (vtable slot 0, mode=1 = scalar delete),
 * then frees self.
 */
extern "C" void datBase_DestroyChild(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);
    void* child = *reinterpret_cast<void**>(obj + 4);

    if (child != nullptr) {
        void** vtable = *reinterpret_cast<void***>(child);
        typedef void (*DtorFunc)(void*, int);
        DtorFunc dtor = reinterpret_cast<DtorFunc>(vtable[0]);
        dtor(child, 1);
    }

    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  datResource — Network-Aware Resource Destructor
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * datResource_ReleaseBuffer  @ 0x820C5658 | size: 0x58
 *
 * Calls datResource_Cleanup on the resource, then checks refCount at +30.
 * If non-zero and a buffer exists at +24, releases it via netBuffer_Release.
 * Always frees self at the end.
 */
extern "C" void datResource_ReleaseBuffer(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    datResource_Cleanup(self);

    uint16_t refCount = *reinterpret_cast<uint16_t*>(obj + 30);
    if (refCount != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 24);
        if (buffer == nullptr) {
            rage_free(self);
            return;
        }
        netBuffer_Release(buffer, 3);
    }

    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmShaderGroup — Graphics Array Destructors
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * grmShaderGroup_ArrayDtor64  @ 0x820EEA18 | size: 0x54
 *
 * Array destructor for 64-byte shader elements.  Reads element count
 * from the uint32_t at (arrayBase - 4) (prepended count convention),
 * iterates backwards calling grmShaderGroup_DestroyElement64 on each,
 * then frees the entire block (count prefix + array data).
 */
extern "C" void grmShaderGroup_ArrayDtor64(void* arrayBase, int /*mode*/)
{
    uint8_t* base = static_cast<uint8_t*>(arrayBase);
    uint32_t* countPtr = reinterpret_cast<uint32_t*>(base - 4);
    uint32_t count = *countPtr;

    // Iterate backwards through 64-byte elements
    uint8_t* ptr = base + count * 64;
    for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; i--) {
        ptr -= 64;
        grmShaderGroup_DestroyElement64(ptr);
    }

    rage_free(countPtr);
}

/**
 * grmShaderGroup_ArrayDtor68  @ 0x820E7520 | size: 0x68
 *
 * Array destructor for 68-byte shader elements.  Checks flags at +6,
 * reads array pointer from +0, reads count from (array - 4),
 * iterates backwards calling grmShaderGroup_DestroyElement68.
 */
extern "C" void grmShaderGroup_ArrayDtor68(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);
    uint16_t flags = *reinterpret_cast<uint16_t*>(obj + 6);

    if (flags != 0) {
        void* arrayPtr = *reinterpret_cast<void**>(obj + 0);
        if (arrayPtr == nullptr)
            return;

        uint8_t* base = static_cast<uint8_t*>(arrayPtr);
        uint32_t* countPtr = reinterpret_cast<uint32_t*>(base - 4);
        uint32_t count = *countPtr;

        // Iterate backwards through 68-byte elements
        uint8_t* ptr = base + count * 68;
        for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; i--) {
            ptr -= 68;
            grmShaderGroup_DestroyElement68(ptr);
        }

        rage_free(countPtr);
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  grmModelGeom — Graphics Model Geometry Destructor
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * grmModelGeom_Destroy  @ 0x820F12E0 | size: 0x68
 *
 * Destructor for a graphics model geometry resource.
 * Frees the data buffer at +12 if ownership flag at +20 is set,
 * then destroys the element array at +0 via grmShaderGroup_ArrayDtor64.
 */
extern "C" void grmModelGeom_Destroy(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    int32_t ownsData = *reinterpret_cast<int32_t*>(obj + 20);
    if (ownsData != 0) {
        void* dataBuffer = *reinterpret_cast<void**>(obj + 12);
        rage_free(dataBuffer);
    }

    int32_t hasArray = *reinterpret_cast<int32_t*>(obj + 8);
    if (hasArray != 0) {
        void* arrayBase = *reinterpret_cast<void**>(obj + 0);
        if (arrayBase == nullptr) {
            rage_free(self);
            return;
        }
        grmShaderGroup_ArrayDtor64(arrayBase, 3);
    }

    rage_free(self);
}

/**
 * grmShaderGroup_Cleanup  @ 0x82124020 | size: 0x5C
 *
 * Cleanup for a resource with an owned buffer and a pointer array.
 * Frees the buffer at +12 if ownership flag at +18 is set,
 * then destroys the pointer array at +0 via grmShaderGroup_PtrArrayDtor.
 */
extern "C" void grmShaderGroup_Cleanup(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    uint16_t ownsBuffer = *reinterpret_cast<uint16_t*>(obj + 18);
    if (ownsBuffer != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 12);
        rage_free(buffer);
    }

    uint16_t hasArray = *reinterpret_cast<uint16_t*>(obj + 6);
    if (hasArray != 0) {
        void* arrayBase = *reinterpret_cast<void**>(obj + 0);
        if (arrayBase == nullptr)
            return;
        grmShaderGroup_PtrArrayDtor(arrayBase, 3);
    }
}

/**
 * grmDataBlock_CopyAssign  @ 0x82126A30 | size: 0x5C
 *
 * Copy-assignment operator for a managed graphics data block.
 * If dst != src: frees old data at dst+0, nulls the pointer,
 * then deep-copies from src via grmDataBlock_CopyConstruct.
 */
extern "C" void* grmDataBlock_CopyAssign(void* dst, void* src)
{
    if (dst != src) {
        uint8_t* dstObj = static_cast<uint8_t*>(dst);

        // Free old data
        void* oldData = *reinterpret_cast<void**>(dstObj + 0);
        rage_free(oldData);

        // Clear pointer
        *reinterpret_cast<void**>(dstObj + 0) = nullptr;

        // Copy from source
        grmDataBlock_CopyConstruct(dst, src);
    }

    return dst;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  crAnimFrame — Animation Frame Destructor
//
//  RTTI vtable: rage::crAnimFrame  @ 0x8204FCBC
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * crAnimFrame_Destroy  @ 0x820EB628 | size: 0x64
 *
 * Destructor for rage::crAnimFrame.  Optionally destroys the child object
 * at +16 via datBase_DestroyChild, resets vtable to crAnimFrame base,
 * calls crAnimFrame_CleanupFields, and frees owned buffer at +8.
 */
extern "C" void crAnimFrame_Destroy(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);
    void* child = *reinterpret_cast<void**>(obj + 16);

    if (child != nullptr) {
        datBase_DestroyChild(child);
    }

    // Reset vtable to rage::crAnimFrame base
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x8204FCBC;

    crAnimFrame_CleanupFields(self);

    uint16_t ownsBuffer = *reinterpret_cast<uint16_t*>(obj + 14);
    if (ownsBuffer != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 8);
        rage_free(buffer);
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  sysThread — Thread Pool Management
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * sysThread_UnlinkAndDestroy  @ 0x820C88D8 | size: 0x80
 *
 * Unlinks a sysThread from the global thread pool linked list
 * (g_threadpool_head), then calls sysThread_Cleanup and frees the object.
 *
 * Thread pool is an intrusive singly-linked list with 'next' at offset +20.
 */
extern "C" void sysThread_UnlinkAndDestroy(void* self)
{
    // Walk the thread pool linked list to find and unlink this thread
    void** linkPtr = reinterpret_cast<void**>(&g_threadpool_head);
    void* node = *linkPtr;

    if (node != nullptr) {
        for (;;) {
            node = *linkPtr;
            if (node == self)
                break;

            // Advance to node->next (offset +20)
            linkPtr = reinterpret_cast<void**>(
                static_cast<uint8_t*>(node) + 20);
            node = *linkPtr;
            if (node == nullptr)
                break;
        }

        // If found, unlink: prev->next = node->next
        node = *linkPtr;
        if (node != nullptr) {
            void* next = *reinterpret_cast<void**>(
                static_cast<uint8_t*>(node) + 20);
            *linkPtr = next;
        }
    }

    sysThread_Cleanup(self);
    rage_free(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  crIKBodyBoneFinderByName — IK Bone Finder Destructor
//
//  RTTI vtables:
//    rage::crIKBodyBoneFinderByName  @ 0x82033C28
//    rage::crIKBodyBoneFinder (base) @ 0x82033BF0
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * crIKBodyBoneFinderByName_Dtor  @ 0x821365B0 | size: 0x68
 *
 * Destructor for rage::crIKBodyBoneFinderByName.
 * Sets vtable to derived class, frees owned arrays (name array at +12,
 * bone index array at +4) based on ownership flags, then resets vtable
 * to the base class rage::crIKBodyBoneFinder.
 */
extern "C" void crIKBodyBoneFinderByName_Dtor(void* self)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    // Set vtable to rage::crIKBodyBoneFinderByName
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x82033C28;

    // Free name array at +12 if owned (flag at +18)
    uint16_t ownsNames = *reinterpret_cast<uint16_t*>(obj + 18);
    if (ownsNames != 0) {
        void* nameArray = *reinterpret_cast<void**>(obj + 12);
        rage_free(nameArray);
    }

    // Free bone index array at +4 if owned (flag at +10)
    uint16_t ownsIndices = *reinterpret_cast<uint16_t*>(obj + 10);
    if (ownsIndices != 0) {
        void* indexArray = *reinterpret_cast<void**>(obj + 4);
        rage_free(indexArray);
    }

    // Reset vtable to base class rage::crIKBodyBoneFinder
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x82033BF0;
}
