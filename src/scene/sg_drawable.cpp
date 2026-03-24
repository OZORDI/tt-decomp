#include "sg_drawable.hpp"

// RAGE engine heap free — canonical name declared in rage/rage_cm_types.hpp
extern void rage_free(void* ptr);               // @ 0x820C00C0

// atSingleton base-class destructor body @ 0x821A9420 - ReleaseSingleton
// Tears down the XML/atSingleton child-node list and resets vtable.
namespace rage { void ReleaseSingleton(void* obj); }        // @ 0x821A9420 - ReleaseSingleton

// RAGE serialization field registration
extern "C" void RegisterSerializationField(void* obj, const char* name,
                                           void* fieldPtr, void* typeDesc,
                                           int flags);                        // @ 0x821A8F58

// Global type-IDs for IsType() checks (stored in SDA .data)
extern uint32_t g_sgRMDrawable_typeId;   // @ 0x825C2D78
extern uint32_t g_sgNode_typeId;         // @ 0x825C93D0

// Serialization type descriptors
extern void* g_resourceFieldType;        // @ 0x825CAF88
extern void* g_matrixFieldType;          // @ 0x825CAFA4

// Static name strings (from .rdata)
static const char* const s_sgRMDrawable_name = "sgRMDrawable";  // @ 0x82071B5C

/**
 * sgRMDrawable::~sgRMDrawable
 * @ 0x823D8E30 | size: 0x74
 *
 * Resets vtable, frees the mesh/material resource pointed to by m_pResource,
 * then calls the atSingleton base destructor to tear down the node's
 * child list.  The scalar-dtor wrapper (recomp ABI) handles the optional
 * rage_free(this) after this body returns.
 */
sgRMDrawable::~sgRMDrawable() {
    if (m_pResource) {
        rage_free(m_pResource);
        m_pResource = nullptr;
    }
    rage::ReleaseSingleton(this);
}

/**
 * sgRMDrawable::GetTypeName
 * @ 0x823D8F80 | size: 0x0C
 */
const char* sgRMDrawable::GetTypeName() const {
    return "sgRMDrawable";
}

/**
 * sgRMDrawable::IsType
 * @ 0x823D8DF8 | size: 0x38
 *
 * Checks typeId against two global type-IDs stored in the SDA:
 *   1. sgRMDrawable type-ID
 *   2. sgNode type-ID (base-class chain)
 * Returns 1 if either matches, 0 otherwise.
 */
int sgRMDrawable::IsType(uint32_t typeId) {
    if (typeId == g_sgRMDrawable_typeId) {
        return 1;
    }
    return (typeId == g_sgNode_typeId) ? 1 : 0;
}

/**
 * sgRMDrawable::RegisterFields
 * @ 0x823D8F18 | size: 0x64
 *
 * Registers m_pResource (offset +0x50) and m_transform (offset +0x10) with
 * the RAGE serialization (parStructure) framework for streaming save/load.
 */
void sgRMDrawable::RegisterFields() {
    // Register m_pResource (+0x50) for serialization
    RegisterSerializationField(this, "Mesh", &m_pResource, g_resourceFieldType, 0);
    // Register m_transform (+0x10) for serialization
    RegisterSerializationField(this, "Matrix", m_transform, g_matrixFieldType, 0);
}

/**
 * sgRMDrawable::Render
 * @ 0x823D8D18 | size: 0x0C
 */
void sgRMDrawable::Render() {
    // Slot 22 in the binary returns a static name string.
    // Despite the inherited "Render" name, this slot acts as a secondary
    // type-name getter in the sgNode vtable layout.
    // Original: returns pointer to "sgRMDrawable" in .rdata @ 0x82071B5C
    // No-op in void form — see GetTypeName() for the actual name accessor.
}

/**
 * sgRMDrawable::UpdateBounds
 * @ 0x823D8EA8 | size: 0x6C
 */
void sgRMDrawable::UpdateBounds() {
    if (!m_pPhysics) {
        return;
    }

    // Get the current bounding data from this node (vtable slot 31)
    typedef void* (*GetBoundsFunc)(void*);
    void** selfVtable = *(void***)this;
    GetBoundsFunc getBounds = (GetBoundsFunc)selfVtable[31];
    void* boundsData = getBounds(this);

    // Forward bounds data to the physics object for AABB update (vtable slot 5)
    typedef void (*UpdateFunc)(void*, void*, int, int, int);
    void** physVtable = *(void***)m_pPhysics;
    UpdateFunc updateBounds = (UpdateFunc)physVtable[5];
    updateBounds(m_pPhysics, boundsData, 0, 0, 0);
}

/**
 * sgRMDrawable::SetTransform
 * @ 0x823D8CD8 | size: 0x40
 *
 * Copies the caller's 4×4 matrix into m_transform using four VMX128 SIMD
 * loads (lvx128) and stores (stvx) — a single 64-byte aligned block copy.
 */
void sgRMDrawable::SetTransform(const float mtx[4][4]) {
    // AltiVec-aligned 64-byte copy: original emits four lvx128/stvx pairs
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            m_transform[r][c] = mtx[r][c];
}
