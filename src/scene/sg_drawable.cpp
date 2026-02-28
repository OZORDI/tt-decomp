#include "sg_drawable.hpp"

// RAGE engine heap free — canonical name declared in rage/rage_cm_types.hpp
extern void rage_free(void* ptr);               // @ 0x820C00C0

// atSingleton base-class destructor body @ 0x821A9420 - ReleaseSingleton
// Tears down the XML/atSingleton child-node list and resets vtable.
namespace rage { void ReleaseSingleton(void* obj); }        // @ 0x821A9420 - ReleaseSingleton

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
    // TODO: load both global type-IDs from SDA and compare
    (void)typeId;
    return 0;
}

/**
 * sgRMDrawable::RegisterFields
 * @ 0x823D8F18 | size: 0x64
 *
 * Registers m_pResource (offset +0x50) and m_transform (offset +0x10) with
 * the RAGE serialization (parStructure) framework for streaming save/load.
 */
void sgRMDrawable::RegisterFields() {
    // TODO: call RAGE parStructure field-registration helpers
}

/**
 * sgRMDrawable::Render
 * @ 0x823D8D18 | size: 0x0C
 */
void sgRMDrawable::Render() {
    // TODO: submit draw call to the rendering pipeline
}

/**
 * sgRMDrawable::UpdateBounds
 * @ 0x823D8EA8 | size: 0x6C
 */
void sgRMDrawable::UpdateBounds() {
    // TODO: recalculate AABB / bounding sphere from m_transform and mesh data
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
