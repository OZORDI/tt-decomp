/**
 * rage_grm.cpp — Graphics / rendering class implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#include "rage_grm.hpp"

// Forward declarations for external functions
extern "C" void fragDrawable_0AA0_2h(void* geom, void* drawable, uint32_t index, void* param);

// RAGE-internal helpers that appear in the lifted bodies below.  These
// are still raw recomp stubs — kept as extern trampolines so we do not
// have to duplicate their PPC bodies here.
extern "C" void   rage_free_00C0(uint32_t ptr);                       // @ 0x820C00C0
extern "C" void   rage_0F38(void* self);                              // @ 0x820F0F38
extern "C" void   net_2228_h(void* self);                             // @ 0x820F2228
extern "C" void   rage_1028(uint32_t a, uint32_t b, uint32_t c,
                            uint32_t d, uint32_t e);                  // @ 0x820F1028
extern "C" void   grmModelFactory_sm_DeclMap_dtor(void* self);        // @ 0x820F3CA0
extern "C" void   ke_1ED8(uint32_t chunk);                            // @ 0x820F1ED8
extern "C" void   ke_2160(uint32_t chunk);                            // @ 0x820F2160
extern "C" void   fragDrawable_3010(uint32_t chunk);                  // @ 0x820F3010
extern "C" void   grcSetup_vfn_2_CFD0_1(void* self);                  // @ 0x8215CFD0
extern "C" void   xe_main_thread_init_0038();                         // @ 0x820C0038

// SDA global: head of the global descriptor-table list (r13+0 on PPC32).
extern "C" uint32_t g_rageDescriptorHead;                             // @ 0x82600000
// Global grmShaderFactory instance pointer (SDA-relative).
extern "C" uint32_t g_grmShaderFactory_instance;                      // @ 0x82606374

// RTTI type-id cells — compared against by the shader IsOfType pair.
// Both live in .data and are written at link/boot.
extern "C" uint32_t lbl_825C8038;
extern "C" uint32_t lbl_825C803C;

// Shader-reset global: single uint32 drop-active-shader cell.
extern "C" uint32_t g_rageActiveShaderCell;   // VA 0x(-32161<<16) + (-17816) slot

namespace rage {

// ── rage::spdApical ───────────────────────────────────────────────────────────

void spdApical::SetLodNear(float value) {
    unk_10 = value;
}

void spdApical::SetLodFar(float value) {
    unk_14 = value;
}

void spdApical::SetLodFadeStart(float value) {
    unk_18 = value;
}

void spdApical::SetLodFadeEnd(float value) {
    unk_1C = value;
}

// ── rage::fragDrawable ────────────────────────────────────────────────────────

/**
 * fragDrawable::ProcessGeometry
 * @ 0x820F1C00 | size: 0x17c
 *
 * Original symbol: fragDrawable_1C00_fw
 * Subsystem: RAGE Graphics / Fragment Drawable
 *
 * Iterates through drawable geometry and processes each one based on a filter criterion.
 * Calls virtual method slot 4 on each geometry object that matches the filter type.
 * 
 * Parameters:
 *   geometryContainer - Object containing geometry array at offset +8
 *   param1 - Parameter passed to geometry virtual method
 *   param2 - Parameter passed to geometry virtual method
 *   filterType - Type filter for geometry processing (extracted from flags bits 27-31)
 *   param3 - Additional parameter passed to geometry virtual method
 */
void fragDrawable::ProcessGeometry(void* geometryContainer, void* param1, void* param2,
                                   uint32_t filterType, void* param3) {
    if (m_geometryCount <= 0) {
        return;
    }
    
    // Global state pointers (SDA-relative globals)
    extern uint32_t* g_iccProfilePtr;      // @ 0x825EB1D0 (r25 + -20016)
    extern void* g_currentGeometry;        // @ 0x82606350 (r26 + 25424)
    
    uint32_t* statePtr = g_iccProfilePtr;
    void* currentGeom = g_currentGeometry;
    
    // Get geometry array from container
    void** geometryArray = (void**)((uint8_t*)geometryContainer + 8);
    
    for (uint16_t i = 0; i < m_geometryCount; i++) {
        void* geom = nullptr;
        uint16_t flags = 0;
        
        // Determine geometry object and its flags
        if (currentGeom != nullptr) {
            geom = currentGeom;
            flags = *(uint16_t*)((uint8_t*)geom + 4);
            
            // Check if bit 0 is set (active flag)
            bool isActive = (flags & 0x1) != 0;
            if (!isActive) {
                // Extract type from flags (bits 27-31 after rotation)
                uint32_t geomType = (flags >> 5) & 0x1F;
                
                // Check if type matches filter
                if (geomType != filterType) {
                    continue;
                }
            }
        } else {
            // Load geometry from array using index
            uint16_t geomIndex = m_pIndices[i];
            geom = geometryArray[geomIndex];
            flags = *(uint16_t*)((uint8_t*)geom + 4);
            
            // Extract type from flags
            uint32_t geomType = (flags >> 5) & 0x1F;
            
            // Check if type matches filter
            if (geomType != filterType) {
                continue;
            }
        }
        
        // Fast path: if state pointer is set, call helper function
        if (statePtr != nullptr) {
            fragDrawable_0AA0_2h(geom, this, i, param3);
            continue;
        }
        
        // Determine if this is the last geometry in a sequence
        bool isLast = false;
        
        if (currentGeom == nullptr) {
            // Check if next geometry has same ID
            if (i + 1 < m_geometryCount) {
                uint16_t nextIndex = m_pIndices[i + 1];
                void* nextGeom = geometryArray[nextIndex];
                
                uint16_t currentId = *(uint16_t*)((uint8_t*)geom + 6);
                uint16_t nextId = *(uint16_t*)((uint8_t*)nextGeom + 6);
                
                isLast = (currentId != nextId);
            } else {
                isLast = true;
            }
        } else {
            isLast = true;
        }
        
        // Determine final isLast flag
        bool finalIsLast = (i + 1 == m_geometryCount) || isLast;
        
        // Call virtual method at slot 4 (offset +16 in vtable)
        void** vtable = *(void***)geom;
        typedef void (*GeomProcessFn)(void* geom, void* drawable, uint32_t index,
                                     void* param1, void* param2, void* param3,
                                     bool isLast);
        GeomProcessFn processFn = (GeomProcessFn)vtable[4];
        processFn(geom, this, i, param1, param2, param3, finalIsLast);
        
        // Reload global state after virtual call
        currentGeom = g_currentGeometry;
        statePtr = g_iccProfilePtr;
    }
}

// ── rage::grmModel ────────────────────────────────────────────────────────────

/*
 * grmModel::~grmModel        [vtable slot 0]
 * @ 0x820F0E78 | size: 0x6c
 *
 * Auto-deleting destructor.  Frees the heap-allocated geometry buffer at
 * this+0x0C, rewrites the vtable to the base-class slot (at +0x7744
 * from the same SDA page, i.e. lbl_8202F3B4 — grmModel's own vtable),
 * then, if bit 0 of the delete-flag is set, frees the object itself.
 *
 * The original recomp re-stores the vtable unconditionally after the
 * first free; we preserve that ordering because the free() can call
 * back through vtable during its own bookkeeping.
 */
// Host-side plain dtor: the PPC scalar-deleting-dtor body lives in
// ScalarDtor(flags) below, which the VM dispatches for vtable slot 0.
grmModel::~grmModel() = default;

void grmModel::ScalarDtor(int flags) {
    // Install grmModel base vtable prior to the first free.
    vtable = reinterpret_cast<void**>(0x8202F3B4);

    // Release the heap-allocated geometry/shader-group buffer.
    rage_free_00C0(m_pGeometry);

    // Reinstall the (now-bare) vtable — defensive, matches original.
    vtable = reinterpret_cast<void**>(0x8202F3B4);

    // Scalar-dtor flag: delete this when bit 0 is set.
    if (flags & 0x1) {
        rage_free_00C0(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
    }
}

// ── rage::grmModelGeom ────────────────────────────────────────────────────────

/*
 * grmModelGeom::~grmModelGeom   [vtable slot 0]
 * @ 0x820F0EE8 | size: 0x50
 *
 * Chains to rage_0F38 (an internal grmModel teardown shared with
 * other derivatives), then scalar-frees self on bit 0 of deleteFlag.
 */
grmModelGeom::~grmModelGeom() = default;

void grmModelGeom::ScalarDtor(int flags) {
    rage_0F38(this);
    if (flags & 0x1) {
        rage_free_00C0(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
    }
}

/*
 * grmModelGeom::GetGeometryEntry  [vtable slot 1]
 * @ 0x820F19B0 | size: 0x18
 *
 * Indexes the geometry-entry array stored at *(this + 4).  Stride is
 * 40 bytes (0x28) — computed via ((idx<<2)+idx)<<3, a classic
 * compile-time expansion of "idx * 40".  Returns a raw pointer
 * expressed as uint32_t (PPC32 VA).
 */
uint32_t grmModelGeom::GetGeometryEntry(uint32_t index) {
    const uint32_t base_ptr = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 4);
    // stride = (index * 5) * 8 == index * 40
    const uint32_t offset = ((index << 2) + index) << 3;
    return base_ptr + offset;
}

// ── rage::grmModelFactory ─────────────────────────────────────────────────────

/*
 * grmModelFactory::~grmModelFactory  [vtable slot 0]
 * @ 0x820F3C50 | size: 0x50
 *
 * Runs the static-storage destructor on sm_DeclMap, then scalar-frees
 * self on bit 0 of deleteFlag.  sm_DeclMap lives at 0x820F3CA0 and
 * owns the global vertex-declaration map for the factory.
 */
grmModelFactory::~grmModelFactory() = default;

void grmModelFactory::ScalarDtor(int flags) {
    grmModelFactory_sm_DeclMap_dtor(this);
    if (flags & 0x1) {
        rage_free_00C0(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
    }
}

/*
 * grmModelFactory::Dispatch5         [vtable slot 1]
 * @ 0x820F3D10 | size: 0x18
 *
 * Pure argument-shuffle trampoline: drops the `this` pointer and
 * forwards (a,b,c,d,e) verbatim to rage_1028.  This is the textbook
 * PPC pattern for a thunk to a static/free function — used here as a
 * vtable-reachable entry point for a non-member factory routine.
 */
void grmModelFactory::Dispatch5(uint32_t a, uint32_t b, uint32_t c,
                                uint32_t d, uint32_t e) {
    rage_1028(a, b, c, d, e);
}

// ── rage::grmShader ───────────────────────────────────────────────────────────

/*
 * grmShader::IsOfType        [vtable slot 0]
 * @ 0x820F5260 | size: 0x50
 *
 * RAGE RTTI hook.  Returns true when the queried type-id equals either
 * of two sentinel globals.  IDA shows r11 reused uninitialised for
 * one branch of the compare — in practice the comparison that matters
 * after the thunk is `typeId == lbl_825C8038`.
 */
bool grmShader::IsOfType(uint32_t typeId) {
    // The first compare uses a link-patched immediate which IDA models
    // as an uninitialised r11.  In the shipped binary it resolves to
    // lbl_825C8038 as well, so we collapse the compare conservatively.
    return typeId == lbl_825C8038;
}

/*
 * grmShader::IsOfTypeExt     [vtable slot 6]
 * @ 0x820F5330 | size: 0x20
 *
 * Extended RTTI hook: also matches the 8-byte sibling type-id at
 * lbl_825C803C (the derived-class cell) — used by grmShaderFx and
 * grmShaderBasic subclasses to accept either tag.
 */
bool grmShader::IsOfTypeExt(uint32_t typeId) {
    return (typeId == lbl_825C803C) || (typeId == lbl_825C8038);
}

/*
 * grmShader::Reset           [vtable slot 7]
 * @ 0x820F5350 | size: 0x10
 *
 * Clears the global "currently-bound shader" cell.  Minimal body:
 * store zero, return.  Called by the render front-end between passes.
 */
void grmShader::Reset() {
    g_rageActiveShaderCell = 0;
}

// ── rage::grmShaderGroup ──────────────────────────────────────────────────────

/*
 * grmShaderGroup::~grmShaderGroup   [vtable slot 0]
 * @ 0x820F21D8 | size: 0x50
 *
 * Chains to net_2228_h (per-group teardown), then scalar-frees self
 * on bit 0 of deleteFlag.  Same skeleton as grmModelGeom's dtor.
 */
grmShaderGroup::~grmShaderGroup() = default;

void grmShaderGroup::ScalarDtor(int flags) {
    net_2228_h(this);
    if (flags & 0x1) {
        rage_free_00C0(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
    }
}

// ── rage::grmModelFactory::vfn_2 ─────────────────────────────────────────────
/*
 * grmModelFactory::vfn_2      [vtable slot 2]
 * @ 0x820F4CF0 | size: 0x48
 *
 * Cursor-advance for the factory's chunked allocator.  Given a caller-
 * owned cursor cell, compute the next chunk inside the factory buffer:
 *
 *     chunk    = *cursor
 *     if !chunk return                     (nothing allocated yet)
 *     offset   = chunk - m_bufferBase
 *     stride   = m_chunkStride             (tw-trap if zero)
 *     index    = offset / stride + 2       (+2 skips the 8-byte header)
 *     next     = ((uint32_t*)this)[index]  (indexed load off this)
 *     *cursor  = chunk + next
 *     if *cursor == 0 return
 *     ke_1ED8(*cursor)                      (post-advance hook)
 */
void grmModelFactory::vfn_2(uint32_t* cursor) {
    const uint32_t chunk = *cursor;
    if (chunk == 0) {
        return;
    }

    const uint32_t base   = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x04);
    const uint32_t stride = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x4C);
    if (stride == 0) {
        __builtin_trap();  // matches PPC `twllei r9,0`
    }

    const uint32_t index_bytes = ((chunk - base) / stride + 2) << 2;
    const uint32_t step = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + index_bytes);
    const uint32_t next = step + chunk;
    *cursor = next;

    if (next != 0) {
        ke_1ED8(next);
    }
}

// ── rage::grmShaderFactory ───────────────────────────────────────────────────
/*
 * grmShaderFactory::~grmShaderFactory   [vtable slot 0]
 * @ 0x820F6E60 | size: 0x64
 *
 * Auto-deleting destructor.  Rebases the vtable to grmShaderFactory's own
 * (lbl_8202F808 == -32253<<16 + -2040), then clears the global instance
 * pointer iff it was pointing at this.  Finally scalar-frees self on
 * bit 0 of deleteFlag.
 */
// Host-side plain dtor: rebases vtable and clears the global instance
// pointer if it was pointing at this.  The scalar-dtor free leg lives
// in ScalarDeleteSelf (declared in the header) — on host the C++
// delete operator handles that leg naturally, so the dtor body only
// covers the non-free work.
grmShaderFactory::~grmShaderFactory() {
    vtable = reinterpret_cast<void**>(0x8202F808);

    if (g_grmShaderFactory_instance ==
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this))) {
        g_grmShaderFactory_instance = 0;
    }
}

// Mirrors the PPC scalar-dtor free(this) leg — used when reproducing
// the guest ABI via vtable slot 0.
void grmShaderFactory::ScalarDeleteSelf(int deleteFlag) {
    if (deleteFlag & 0x1) {
        rage_free_00C0(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
    }
}

// ── rage::grmShaderFactoryStandard ───────────────────────────────────────────
/*
 * grmShaderFactoryStandard::Allocate40  [vtable slot 2]
 * @ 0x820F7490 | size: 0x64
 *
 * Allocates a 40-byte shader slot via the first global descriptor's
 * vtable-slot-1 allocator (ke_malloc-equivalent on r13+0 chain, r4=40,
 * r5=16).  On success, runs the generic chunk-init hook ke_2160 and
 * returns the new pointer; on failure returns 0.
 */
uint32_t grmShaderFactoryStandard::Allocate40() {
    xe_main_thread_init_0038();

    uint32_t* descTable = reinterpret_cast<uint32_t*>(g_rageDescriptorHead);
    // descTable[1] is the 0x04 slot (first registered descriptor).
    uint32_t allocator = descTable[1];
    using AllocFn = uint32_t (*)(uint32_t self, uint32_t size, uint32_t align);
    // vtable slot 1 on the allocator (byte offset +4).
    void** allocVT = *reinterpret_cast<void***>(allocator);
    AllocFn fn = reinterpret_cast<AllocFn>(allocVT[1]);

    uint32_t blk = fn(allocator, 40, 16);
    if (blk == 0) {
        return 0;
    }

    ke_2160(blk);
    return blk;
}

/*
 * grmShaderFactoryStandard::AdvanceCursor  [vtable slot 7]
 * @ 0x820F72D8 | size: 0x1b8
 *
 * Same cursor-advance shape as grmModelFactory::vfn_2 (buffer-base at
 * +0x04, stride at +0x4C, per-chunk step indexed off this).  Difference:
 * the post-advance hook is fragDrawable_3010 instead of ke_1ED8 — the
 * shader-factory's slot initialiser.
 */
void grmShaderFactoryStandard::AdvanceCursor(uint32_t* cursor) {
    const uint32_t chunk = *cursor;
    if (chunk == 0) {
        return;
    }

    const uint32_t base   = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x04);
    const uint32_t stride = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x4C);
    if (stride == 0) {
        __builtin_trap();
    }

    const uint32_t index_bytes = ((chunk - base) / stride + 2) << 2;
    const uint32_t step = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + index_bytes);
    const uint32_t next = step + chunk;
    *cursor = next;

    if (next != 0) {
        fragDrawable_3010(next);
    }
}

// ── rage::grmSetup ───────────────────────────────────────────────────────────
/*
 * grmSetup::vfn_2           [vtable slot 2]
 * @ 0x82378448 | size: 0x04
 *
 * Thin branch-through to the grcSetup base-class slot-2 implementation.
 * This is the RAGE idiom for "no override — call up" when the vtable
 * layout requires a distinct function address per derived class.
 */
void grmSetup::vfn_2() {
    grcSetup_vfn_2_CFD0_1(this);
}

} // namespace rage

namespace rage {

/* ═══════════════════════════════════════════════════════════════════════════
 * fragDrawable_41B8 @ 0x821241B8 | size: 0x68 (104 bytes)
 *
 * Compares two strings lexicographically.
 *
 * This function performs a signed byte-by-byte comparison of two null-terminated
 * strings, similar to strcmp but returns a boolean result.
 *
 * Parameters:
 *   pString1 - Pointer to first string (stored at offset +0 of first parameter)
 *   pString2 - Pointer to second string (stored at offset +0 of second parameter)
 *
 * Returns:
 *   1 (true) if string1 < string2 lexicographically
 *   0 (false) otherwise
 *
 * Algorithm:
 *   1. Load string pointers from offset +0 of each parameter
 *   2. Compare bytes while both are non-zero and equal
 *   3. When mismatch or null found, compare final bytes as signed chars
 *   4. Return 1 if first string's byte < second string's byte, else 0
 * ═══════════════════════════════════════════════════════════════════════════ */
uint8_t fragDrawable_41B8(void* pParam1, void* pParam2)
{
    /* Load string pointers from parameters */
    const char* str1 = *(const char**)pParam1;
    const char* str2 = *(const char**)pParam2;
    
    /* Compare strings byte-by-byte */
    while (*str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    
    /* Compare final bytes as signed chars */
    int8_t byte1 = (int8_t)*str1;
    int8_t byte2 = (int8_t)*str2;
    
    /* Return 1 if str1 < str2, else 0 */
    return (byte1 < byte2) ? 1 : 0;
}

} // namespace rage
