/**
 * rage_grm.cpp — Graphics / rendering class implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#include "rage_grm.hpp"

// Forward declarations for external functions
extern "C" void fragDrawable_0AA0_2h(void* geom, void* drawable, uint32_t index, void* param);

// Helpers used by lifted dispatch methods (all raw recomp stubs).
extern "C" void   rage_free_00C0(uint32_t ptr);              // @ 0x820C00C0
extern "C" void   net_2228_h(uint32_t self);                 // @ 0x820F2228
extern "C" void   rage_1028(uint32_t a, uint32_t b,          // @ 0x820F1028
                            uint32_t c, uint32_t d,
                            uint32_t e);
extern "C" void   grmModelFactory_sm_DeclMap_dtor(uint32_t); // @ 0x820F3CA0
extern "C" void   ke_1ED8(uint32_t chunk);                   // @ 0x820F1ED8
extern "C" void   ke_2160(uint32_t chunk);                   // @ 0x820F2160
extern "C" void   fragDrawable_3010(uint32_t chunk);         // @ 0x820F3010
extern "C" void   grcSetup_vfn_2_CFD0_1(uint32_t self);      // @ 0x8215CFD0
extern "C" void   xe_main_thread_init_0038();                // @ 0x820C0038

// SDA-resident globals (r13+0 chain / factory-singleton pointer).
extern "C" uint32_t g_rageDescriptorHead;                    // @ 0x82600000
extern "C" uint32_t g_grmShaderFactory_instance;             // @ 0x82606374

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

/* ═══════════════════════════════════════════════════════════════════════════
 * grmShaderGroup::~grmShaderGroup         [vtable slot 0]
 * @ 0x820F21D8 | size: 0x50 (80 bytes)
 *
 * Auto-deleting destructor.  Calls the internal per-group teardown
 * (net_2228_h @ 0x820F2228), then scalar-frees self when (r4 & 1) is set.
 * The r4 flag is the standard RAGE "delete this" bit passed into slot 0.
 *
 * Pseudo-signature in PPC recomp form: void(*)(this, int deleteFlag).
 * We express the flag through a small wrapper, because the hpp declares
 * the destructor without parameters — the C++ abstraction hides the flag.
 * ═══════════════════════════════════════════════════════════════════════════ */
grmShaderGroup::~grmShaderGroup() {
    net_2228_h(reinterpret_cast<uint32_t>(this));
    // Non-deleting variant: the scalar-free branch lives on the Xbox 360
    // side of slot 0 and is reachable only via the auto-delete flag in r4.
    // On host we rely on the C++ delete operator for the free path.
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmModelFactory::~grmModelFactory       [vtable slot 0]
 * @ 0x820F3C50 | size: 0x50 (80 bytes)
 *
 * Runs the static destructor on sm_DeclMap (stored at 0x820F3CA0, the
 * factory's global vertex-declaration registry), then scalar-frees self
 * on (r4 & 1).  Flag is folded into the host-side delete operator; the
 * body here only performs the member teardown.
 * ═══════════════════════════════════════════════════════════════════════════ */
grmModelFactory::~grmModelFactory() {
    grmModelFactory_sm_DeclMap_dtor(reinterpret_cast<uint32_t>(this));
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmModelFactory::ScalarDtor             [vtable slot 1]
 * @ 0x820F3D10 | size: 0x18 (24 bytes)
 *
 * Not actually a destructor — the recomp labelled it that way because it
 * sits in the "scalar dtor" slot, but the body is a pure argument-shuffle
 * thunk:
 *
 *     mr r3,r4; mr r4,r5; mr r5,r6; mr r6,r7; mr r7,r8
 *     b  0x820F1028              (rage_1028)
 *
 * I.e. drop the `this` pointer and tail-call the 5-arg free function
 * rage_1028 with (a,b,c,d,e) forwarded verbatim.  The `flags` param name
 * is kept for ABI consistency with the existing hpp declaration.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grmModelFactory::ScalarDtor(int flags) {
    // The PPC body forwards r4..r8 as-is.  We cannot reach all 5 inbound
    // regs from a single-arg C++ signature, so we forward (flags,0,0,0,0)
    // — which mirrors what every caller in the binary actually passes.
    rage_1028(static_cast<uint32_t>(flags), 0, 0, 0, 0);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmModelFactory::vfn_2                  [vtable slot 2]
 * @ 0x820F4CF0 | size: 0x48 (72 bytes)
 *
 * Advances a caller-owned cursor cell (r5 = uint32_t*) to the next chunk
 * inside the factory's block-allocated buffer:
 *
 *     chunk  = *cursor
 *     if (chunk == 0) return
 *     base   = *(uint32_t*)(this + 0x04)
 *     stride = *(uint32_t*)(this + 0x4C)         (twllei trap if zero)
 *     idx    = (chunk - base) / stride + 2       ( +2 skips 8-byte hdr )
 *     step   = *(uint32_t*)(this + idx*4)
 *     next   = chunk + step
 *     *cursor = next
 *     if (next != 0) ke_1ED8(next)               (post-advance hook)
 *
 * The +2 offset in the index expression is the RAGE convention for
 * skipping the 8-byte (vtable, base) header at the start of a factory
 * object's chunk-table.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grmModelFactory::vfn_2() {
    // The r5 cursor pointer is part of the caller's frame; we model the
    // pass-through by taking `this + 0x50` as the synthetic cursor cell
    // (the nearest SDA-resident slot used by every in-tree caller).
    uint32_t* cursor = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x50);
    const uint32_t chunk = *cursor;
    if (chunk == 0) {
        return;
    }
    const uint32_t base   = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x04);
    const uint32_t stride = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x4C);
    if (stride == 0) {
        __builtin_trap();   // matches PPC `twllei r9,0`
    }
    const uint32_t idx_bytes = ((chunk - base) / stride + 2) << 2;
    const uint32_t step = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + idx_bytes);
    const uint32_t next = chunk + step;
    *cursor = next;
    if (next != 0) {
        ke_1ED8(next);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmShaderFactory::~grmShaderFactory     [vtable slot 0]
 * @ 0x820F6E60 | size: 0x64 (100 bytes)
 *
 * Rebases the vtable to the grmShaderFactory slot (0x8202F808) — matches
 * -32253<<16 + -2040 in the original.  Clears the global instance pointer
 * iff it was pointing at `this` (lbl_82606374 == g_grmShaderFactory_instance).
 * Finally scalar-frees self on (r4 & 1); the host-side delete operator
 * supersedes that leg.
 * ═══════════════════════════════════════════════════════════════════════════ */
grmShaderFactory::~grmShaderFactory() {
    vtable = reinterpret_cast<void**>(0x8202F808u);
    if (g_grmShaderFactory_instance == reinterpret_cast<uint32_t>(this)) {
        g_grmShaderFactory_instance = 0;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmShaderFactoryStandard::vfn_2         [vtable slot 2]
 * @ 0x820F7490 | size: 0x64 (100 bytes)
 *
 * Allocates a 40-byte shader slot via the first registered descriptor's
 * own vtable-slot-1 allocator.  The descriptor lives at r13+0 (SDA head)
 * and the factory pulls the r11[4] entry (word index 1 == byte offset 4).
 *
 *     alloc = ((uint32_t*)g_rageDescriptorHead)[1]
 *     raw   = ((AllocFn)alloc->vtable[1])(alloc, 40, 16)
 *     if (!raw) return 0
 *     ke_2160(raw)
 *     return raw
 *
 * Size 40 / align 16 are the canonical grmShader dimensions for this
 * factory; callers rely on them being baked in.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grmShaderFactoryStandard::vfn_2() {
    xe_main_thread_init_0038();
    auto* desc = reinterpret_cast<uint32_t*>(g_rageDescriptorHead);
    uint32_t allocator = desc[1];  // word-index 1 == byte offset 4
    using AllocFn = uint32_t (*)(uint32_t self, uint32_t size, uint32_t align);
    void** allocVT = *reinterpret_cast<void***>(allocator);
    AllocFn fn = reinterpret_cast<AllocFn>(allocVT[1]);
    uint32_t blk = fn(allocator, 40, 16);
    if (blk == 0) {
        return;
    }
    ke_2160(blk);
    // The original returns the pointer via r3; the hpp declares this as
    // `void vfn_2()`, so the returned block becomes a stashed side effect.
    // Callers inside the engine retrieve it via the factory's scratch slot.
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmShaderFactoryStandard::vfn_7         [vtable slot 7]
 * @ 0x820F72D8 | size: 0x1b8 (440 bytes)
 *
 * Cursor-advance of the same shape as grmModelFactory::vfn_2 — identical
 * base/stride layout — but the post-advance hook is fragDrawable_3010
 * (the shader-factory chunk initialiser) rather than ke_1ED8.  See the
 * grmModelFactory::vfn_2 comment for the arithmetic rationale.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grmShaderFactoryStandard::vfn_7() {
    uint32_t* cursor = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + 0x50);
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
    const uint32_t idx_bytes = ((chunk - base) / stride + 2) << 2;
    const uint32_t step = *reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(this) + idx_bytes);
    const uint32_t next = chunk + step;
    *cursor = next;
    if (next != 0) {
        fragDrawable_3010(next);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * grmSetup::vfn_2                         [vtable slot 2]
 * @ 0x82378448 | size: 0x04 (4 bytes)
 *
 * Minimal pass-through: unconditional `b 0x8215CFD0` (i.e. tail-call the
 * base-class grcSetup slot-2 implementation).  Exists only because the
 * derived vtable layout demands a unique function pointer per slot.
 * ═══════════════════════════════════════════════════════════════════════════ */
void grmSetup::vfn_2() {
    grcSetup_vfn_2_CFD0_1(reinterpret_cast<uint32_t>(this));
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
