/**
 * rage_engine_dtors.cpp — RAGE Engine destructor and cleanup functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Destructors, array cleanup routines, and utility functions for
 * various RAGE engine subsystems (animation, graphics, threading, etc.).
 */
#include <cstdint>

// External function declarations
extern "C" {
    void rage_free_00C0(void* ptr);
    void rage_C788(void* self);
    void net_5B40(void* buffer, int mode);
    void rage_39C0(void* element);
    void rage_EA70(void* element);
    void rage_6348(void* self);
    void rage_2EE0(void* obj);
    void rage_8070(void* self);
    void rage_40F8(void* arrayBase, int mode);
    void atSingleton_AC28_g(void* self);
    void atSingleton_dtor_6160(void* array, int mode);
    void sysMemAllocator_Free_6228(void* ptr, int alignment);
    void rage_threadpool_cleanup_6878(void* thread);
    void cmOperatorCtor_6998_w(void* dst, void* src);
}

// ── Thread pool linked-list head @ 0x825EBCBC ────────────────────────────────
extern "C" uint32_t* g_threadpool_head;


// ─────────────────────────────────────────────────────────────────────────────
// rage_18B8 @ 0x820C18B8 | size: 0x54 (84 bytes)
// alias: LocomotionStateAnim_1910
//
// Generic destructor wrapper. Calls the first virtual method (destructor)
// on the child object at offset +4 if present, then frees self.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_18B8(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);
    void* child = *reinterpret_cast<void**>(obj + 4);

    if (child != nullptr) {
        // Call virtual destructor (vtable slot 0) with mode=1
        void** vtable = *reinterpret_cast<void***>(child);
        typedef void (*DtorFunc)(void*, int);
        DtorFunc dtor = reinterpret_cast<DtorFunc>(vtable[0]);
        dtor(child, 1);
    }

    rage_free_00C0(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_5658 @ 0x820C5658 | size: 0x58 (88 bytes)
// alias: game_56B0
//
// Destructor for a network-aware resource. Calls rage_C788 cleanup first,
// then conditionally releases the buffer via net_5B40 or frees directly.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_5658(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);

    rage_C788(self);

    uint16_t refCount = *reinterpret_cast<uint16_t*>(obj + 30);
    if (refCount != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 24);
        if (buffer == nullptr) {
            rage_free_00C0(self);
            return;
        }
        net_5B40(buffer, 3);
    }

    rage_free_00C0(self);
}

// rage_EA18 @ 0x820EEA18 | size: 0x54 (84 bytes)
// alias: rage_EA70
//
// Array destructor for 64-byte elements. Reads element count from
// offset -4 (prepended count), iterates backwards calling rage_EA70
// on each element, then frees the count+array block.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_EA18(void* arrayBase, int /*mode*/) {
    uint8_t* base = static_cast<uint8_t*>(arrayBase);
    uint32_t* countPtr = reinterpret_cast<uint32_t*>(base - 4);
    uint32_t count = *countPtr;

    // count << 6 = count * 64; iterate backwards
    uint8_t* ptr = base + count * 64;
    for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; i--) {
        ptr -= 64;
        rage_EA70(ptr);
    }

    rage_free_00C0(countPtr);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_7520 @ 0x820E7520 | size: 0x68 (104 bytes)
// alias: rage_7588
//
// Array destructor for 68-byte elements. Checks flags at offset +6,
// reads array pointer from offset +0, reads count from offset -4,
// iterates backwards calling rage_39C0, then frees count+array block.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_7520(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);
    uint16_t flags = *reinterpret_cast<uint16_t*>(obj + 6);

    if (flags != 0) {
        void* arrayPtr = *reinterpret_cast<void**>(obj + 0);
        if (arrayPtr == nullptr) {
            return;
        }

        uint8_t* base = static_cast<uint8_t*>(arrayPtr);
        uint32_t* countPtr = reinterpret_cast<uint32_t*>(base - 4);
        uint32_t count = *countPtr;

        // count * 68; iterate backwards
        uint8_t* ptr = base + count * 68;
        for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; i--) {
            ptr -= 68;
            rage_39C0(ptr);
        }

        rage_free_00C0(countPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_B628 @ 0x820EB628 | size: 0x64 (100 bytes)
// alias: crAnimDofFloat_B690_2h
//
// Destructor for rage::crAnimFrame. Optionally destroys child object at
// offset +16 via rage_18B8, resets vtable to crAnimFrame base, calls
// atSingleton cleanup, and conditionally frees buffer at offset +8.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_B628(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);
    void* child = *reinterpret_cast<void**>(obj + 16);

    if (child != nullptr) {
        rage_18B8(child);
    }

    // Reset vtable to rage::crAnimFrame (0x8204FCBC)
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x8204FCBC;

    atSingleton_AC28_g(self);

    uint16_t ownsBuffer = *reinterpret_cast<uint16_t*>(obj + 14);
    if (ownsBuffer != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 8);
        rage_free_00C0(buffer);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_88D8 @ 0x820C88D8 | size: 0x80 (128 bytes)
// alias: atSingleton_8958_g
//
extern "C" void rage_88D8(void* self) {
    // Pointer-to-pointer traversal of the thread pool linked list
    // Each node has a 'next' pointer at offset +20
    void** linkPtr = reinterpret_cast<void**>(&g_threadpool_head);
    void* node = *linkPtr;

    if (node != nullptr) {
        // Walk the list: linkPtr points to head or to a node's next field
        for (;;) {
            node = *linkPtr;
            if (node == self) {
                break;
            }
            // Advance linkPtr to node->next (offset +20)
            linkPtr = reinterpret_cast<void**>(static_cast<uint8_t*>(node) + 20);
            node = *linkPtr;
            if (node == nullptr) {
                break;
            }
        }

        // If we found the node, unlink it
        node = *linkPtr;
        if (node != nullptr) {
            void* next = *reinterpret_cast<void**>(static_cast<uint8_t*>(node) + 20);
            *linkPtr = next;
        }
    }

    rage_threadpool_cleanup_6878(self);
    rage_free_00C0(self);
}



// ─────────────────────────────────────────────────────────────────────────────
// rage_12E0 @ 0x820F12E0 | size: 0x68 (104 bytes)
// alias: grmModelGeom_vfn_3
//
// Destructor for a graphics model geometry resource. Conditionally frees
// the data buffer at offset +12 based on ownership flag at offset +20,
// then optionally destroys the element array via rage_EA18.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_12E0(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);

    int32_t ownsData = *reinterpret_cast<int32_t*>(obj + 20);
    if (ownsData != 0) {
        void* dataBuffer = *reinterpret_cast<void**>(obj + 12);
        rage_free_00C0(dataBuffer);
    }

    int32_t hasArray = *reinterpret_cast<int32_t*>(obj + 8);
    if (hasArray != 0) {
        void* arrayBase = *reinterpret_cast<void**>(obj + 0);
        if (arrayBase == nullptr) {
            rage_free_00C0(self);
            return;
        }
        rage_EA18(arrayBase, 3);
    }

    rage_free_00C0(self);
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_4020 @ 0x82124020 | size: 0x5C (92 bytes)
// alias: ke_4080
//
// Cleanup for a resource with an owned buffer and a pointer array.
// Frees the buffer at offset +12 if ownership flag at offset +18 is set,
// then destroys the pointer array at offset +0 via rage_40F8.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_4020(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);

    uint16_t ownsBuffer = *reinterpret_cast<uint16_t*>(obj + 18);
    if (ownsBuffer != 0) {
        void* buffer = *reinterpret_cast<void**>(obj + 12);
        rage_free_00C0(buffer);
    }

    uint16_t hasArray = *reinterpret_cast<uint16_t*>(obj + 6);
    if (hasArray != 0) {
        void* arrayBase = *reinterpret_cast<void**>(obj + 0);
        if (arrayBase == nullptr) {
            return;
        }
        rage_40F8(arrayBase, 3);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// rage_6A30 @ 0x82126A30 | size: 0x5C (92 bytes)
// alias: rage_6A90
//
// Copy assignment operator for a simple managed resource. If the source
// differs from the destination, frees the old data, clears the pointer,
// and copies from the source via cmOperatorCtor_6998_w.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void* rage_6A30(void* dst, void* src) {
    if (dst != src) {
        uint8_t* dstObj = static_cast<uint8_t*>(dst);

        // Free old data
        void* oldData = *reinterpret_cast<void**>(dstObj + 0);
        rage_free_00C0(oldData);

        // Clear pointer
        *reinterpret_cast<void**>(dstObj + 0) = nullptr;

        // Copy from source
        cmOperatorCtor_6998_w(dst, src);
    }

    return dst;
}

// ─────────────────────────────────────────────────────────────────────────────
// rage_65B0 @ 0x821365B0 | size: 0x68 (104 bytes)
// alias: crIKBodyBoneFinderByName_vfn_0
//
// Destructor for rage::crIKBodyBoneFinderByName. Sets vtable to derived
// class, frees owned arrays at offsets +12 and +4, then resets vtable
// to the base class rage::crIKBodyBoneFinder.
// ─────────────────────────────────────────────────────────────────────────────
extern "C" void rage_65B0(void* self) {
    uint8_t* obj = static_cast<uint8_t*>(self);

    // Set vtable to rage::crIKBodyBoneFinderByName (0x82033C28)
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x82033C28;

    // Free name array at offset +12 if owned (flag at offset +18)
    uint16_t ownsNames = *reinterpret_cast<uint16_t*>(obj + 18);
    if (ownsNames != 0) {
        void* nameArray = *reinterpret_cast<void**>(obj + 12);
        rage_free_00C0(nameArray);
    }

    // Free bone indices at offset +4 if owned (flag at offset +10)
    uint16_t ownsIndices = *reinterpret_cast<uint16_t*>(obj + 10);
    if (ownsIndices != 0) {
        void* indexArray = *reinterpret_cast<void**>(obj + 4);
        rage_free_00C0(indexArray);
    }

    // Reset vtable to base class rage::crIKBodyBoneFinder (0x82033BF0)
    *reinterpret_cast<uint32_t*>(obj + 0) = 0x82033BF0;
}
