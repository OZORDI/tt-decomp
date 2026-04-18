/**
 * rage_grm.hpp — Graphics / rendering class declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 26 classes sourced from RTTI, vtable layout, offset clusters,
 * and debug strings.  Unresolved virtual slots remain vfn_N.
 */
#pragma once
#include <cstdint>

namespace rage {

// ── rage::fragDrawable  [vtable @ 0x82033094] ──────────────────────────
struct fragDrawable {
    void**      vtable;           // +0x00
    uint8_t     pad_04[0x08];     // +0x04
    uint16_t*   m_pIndices;       // +0x0C  array of geometry indices
    uint8_t     pad_10[0x06];     // +0x10
    uint16_t    m_geometryCount;  // +0x16

    // ── virtual methods ──
    virtual ~fragDrawable();                  // [0] @ 0x821242f0
    virtual void ScalarDtor(int flags); // [1] @ 0x8225b880
    virtual void vfn_2();  // [2] @ 0x82125268
    virtual void vfn_3();  // [3] @ 0x82124f70
    virtual void vfn_4();  // [4] @ 0x8225bde8
    virtual void vfn_5();  // [5] @ 0x8225c7e0
    virtual void vfn_6();  // [6] @ 0x8225c838
    virtual void vfn_7();  // [7] @ 0x8225c910
    virtual void vfn_8();  // [8] @ 0x8225c430
    virtual void vfn_9();  // [9] @ 0x8225c368
    virtual void vfn_10();  // [10] @ 0x82124168
    virtual void vfn_11();  // [11] @ 0x8225c018
    virtual void vfn_12();  // [12] @ 0x8225c0a0
    virtual void vfn_13();  // [13] @ 0x8225bf88
    virtual void vfn_14();  // [14] @ 0x8225c158
    virtual void vfn_15();  // [15] @ 0x8225c238
    virtual void vfn_16();  // [16] @ 0x82124540
    
    // ── non-virtual methods ──
    void ProcessGeometry(void* geometryContainer, void* param1, void* param2, 
                        uint32_t filterType, void* param3);  // @ 0x820F1C00
};


// ── rage::fragHeapAllocator  [vtable @ 0x82033324] ──────────────────────────
struct fragHeapAllocator {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags); // [1] @ 0x8212b0e0
    virtual void vfn_2();  // [2] @ 0x8212b078
    virtual void vfn_20();  // [20] @ 0x8212b240
};

// ── rage::fragInst  [vtable @ 0x82032D2C] ──────────────────────────
struct fragInst {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~fragInst();                  // [0] @ 0x8211d368
    virtual void vfn_8();  // [8] @ 0x8211ec48
    virtual void vfn_30();  // [30] @ 0x82120120
    virtual void vfn_40();  // [40] @ 0x82121088
    virtual void vfn_44();  // [44] @ 0x821212b0
};

// ── rage::fragType  [vtable @ 0x82032CA4] ──────────────────────────
struct fragType {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~fragType();                  // [0] @ 0x8211bd10
    virtual void ScalarDtor(int flags); // [1] @ 0x8211c508
};

// ── rage::fragTypeChild  [vtable @ 0x82033040] ──────────────────────────
struct fragTypeChild {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~fragTypeChild();                  // [0] @ 0x82122e78
};

// ── rage::grmEdgeModel  [vtable @ 0x8206A070] ──────────────────────────
// Confirmed methods: Create
struct grmEdgeModel {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmEdgeModel();                  // [0] @ 0x82375db8

    // ── non-virtual methods (from debug strings) ──
    void Create();
};

// ── rage::grmModel  [vtable @ 0x8202F3B4, size 0x18 = 6 slots] ─────
// Geometry/model container.  Holds a pointer to a heap-allocated
// shader-group/geometry array at +0x0C; dtor frees that buffer and
// conditionally frees the object itself (scalar-dtor flag in r4 bit 0).
struct grmModel {
    void**      vtable;           // +0x00
    uint8_t     pad_04[0x08];     // +0x04
    uint32_t    m_pGeometry;      // +0x0C  heap-allocated geometry buffer

    // ── virtual methods ──
    // [0] The PPC vtable slot 0 is the scalar-deleting-dtor idiom:
    //   free(m_pGeometry); *vtable = grmModel_base_vtable;
    //   if (r4 & 1) free(this);
    // Host C++ cannot express dtor-with-params, so the body lives in
    // ScalarDtor(flags); the plain ~grmModel() is the no-free leg.
    virtual ~grmModel();                            // [0] @ 0x820f0e78  size 0x6c
    virtual void ScalarDtor(int flags);             // [0] PPC scalar-deleting-dtor body
};

// ── rage::grmModelFactory  [vtable @ 0x8202F5A8] ──────────────────────────
struct grmModelFactory {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // [0] PPC scalar-deleting-dtor: calls static destructor on sm_DeclMap then
    //     conditionally frees the object if (flag & 1).  See ScalarDtor below.
    virtual ~grmModelFactory();                     // [0] @ 0x820f3c50  size 0x50
    virtual void ScalarDtor(int flags);             // [0] PPC scalar-deleting-dtor body
    // [1] 5-arg trampoline: shuffles r4..r8 into r3..r7 and tail-calls
    //     rage_1028 (likely a static Create/Register forwarder).
    virtual void Dispatch5(uint32_t a, uint32_t b, uint32_t c,
                           uint32_t d, uint32_t e);  // [1] @ 0x820f3d10  size 0x18
    virtual void vfn_2(uint32_t* cursor);   // [2] @ 0x820f4cf0
};

// ── rage::grmModelGeom  [vtable @ 0x8202F3CC] ──────────────────────────
// Derived from grmModel; dtor chains through grmModel internals (rage_0F38).
struct grmModelGeom {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // [0] PPC scalar-deleting-dtor — see ScalarDtor below for body.
    virtual ~grmModelGeom();                        // [0] @ 0x820f0ee8  size 0x50
    virtual void ScalarDtor(int flags);             // [0] PPC scalar-deleting-dtor body
    // [1] geometry accessor: returns &((uint8_t*)(this[1]))[index * 0x28]
    //     — fetches a 40-byte geometry entry from an array at this+0x04.
    virtual uint32_t GetGeometryEntry(uint32_t index); // [1] @ 0x820f19b0  size 0x18
    virtual void vfn_3();                    // [3] @ 0x820f1348
};

// ── rage::grmSetup  [vtable @ 0x8204E984] ──────────────────────────
// Dispatch-side bootstrap object; slot 1 installs two singletons
// (grmShaderFactoryStandard + grmShaderFx) into SDA globals, then slot 2
// is a thin alias that forwards to the grcSetup base-class implementation.
struct grmSetup {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags);         // [1] @ 0x82378218
    // [2] Base-class passthrough: unconditionally tail-calls
    //     grcSetup_vfn_2_CFD0_1 (the rage::grcSetup slot-2 implementation).
    virtual void vfn_2();                       // [2] @ 0x82378448  size 0x4
    virtual void vfn_6();                       // [6] @ 0x82378338

    // Scalar-dtor free helper: original PPC slot-0 branch frees the
    // object when bit 0 of the delete flag is set.
    void ScalarDeleteSelf(int deleteFlag);
};

// ── rage::grmShader  [vtable @ 0x8202F5C4, size 0x4c = 19 slots] ──────
// Base shader-binding object.  Slots 0/1 are RAGE's standard
// RTTI type-check pair (compare param against a global type-id cell
// at lbl_825C8038 / lbl_825C803C).  Slot 1 (scalar-dtor) is the
// real draw-loop dispatcher — see .cpp for the analysis.
struct grmShader {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // [0] RTTI IsOfType: returns (param == classId_A) || (param == classId_B).
    //     classId_A is loaded from uninitialized r11 in this stub — the
    //     real impl is patched at link/runtime; behavioural net effect:
    //     returns (param == lbl_825C8038).
    virtual bool IsOfType(uint32_t typeId);         // [0] @ 0x820f5260  size 0x50
    // [1] BatchDraw: iterates child primitives calling slots 11/8/9/7
    //     with caller-supplied floats f1..f5 and four int args boxed
    //     through the FP pipeline (int→double→float).  Real draw loop.
    virtual void BatchDraw(int32_t i0, int32_t i1, int32_t i2, int32_t i3,
                           float f1, float f2, float f3, float f4, float f5,
                           uint32_t ctx);               // [1] @ 0x820f5360  size 0x190
    // [6] RTTI IsOfTypeExt: (param == lbl_825C803C) || (param == lbl_825C8038).
    virtual bool IsOfTypeExt(uint32_t typeId);      // [6] @ 0x820f5330  size 0x20
    // [7] Reset: clears a global state word (-17816 off g_state r10)
    //     — seen in many RAGE shader subsystems as "drop-active-shader".
    virtual void Reset();                            // [7] @ 0x820f5350  size 0x10
};

// ── rage::grmShaderBasic  [vtable @ 0x8202F8FC] ──────────────────────────
struct grmShaderBasic {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void vfn_2();  // [2] @ 0x820f7a08
};

// ── rage::grmShaderBasicClamped  [vtable @ 0x8202F85C] ──────────────────────────
struct grmShaderBasicClamped {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmShaderBasicClamped();                  // [0] @ 0x820f7118
    virtual void vfn_2();  // [2] @ 0x820f7ca0
    virtual void vfn_3();  // [3] @ 0x820f7d08
    virtual void vfn_4();  // [4] @ 0x820f7d78
    virtual void vfn_8();  // [8] @ 0x820f7e50
    virtual void vfn_10();  // [10] @ 0x820f7cf8
    virtual void vfn_13();  // [13] @ 0x820f7f00
    virtual void vfn_16();  // [16] @ 0x82491788
};

// ── rage::grmShaderFactory  [vtable @ 0x8202F808] ──────────────────────────
// Dispatch-side singleton/registry for grmShader instances.  Slot 0 is the
// auto-deleting destructor which also drops the global instance pointer
// (lbl_82606374) iff (sm_pInstance == this).
struct grmShaderFactory {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // [0] destructor body: rebases vtable and clears the global instance
    //     pointer if (sm_pInstance == this).  The PPC scalar-dtor flag
    //     (r4 & 1 → free(this)) lives in ScalarDeleteSelf below; on the
    //     host the C++ delete operator handles that leg.
    virtual ~grmShaderFactory();                  // [0] @ 0x820f6e60  size 0x64

    // Scalar-deleting helper that mirrors the PPC dtor's free(this)
    // branch — callable explicitly when reproducing the guest ABI.
    void ScalarDeleteSelf(int deleteFlag);
};

// ── rage::grmShaderFactoryStandard  [vtable @ 0x8202F830] ──────────────────────────
// Concrete grmShaderFactory subclass.  Layout: only vtable at +0x00 has
// been verified against the live binary; interior members stay as an
// opaque tail pad so we don't commit to speculative offsets.
struct grmShaderFactoryStandard {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags);         // [1] @ 0x820f6ec8
    // [2] Allocates a 40-byte shader slot via the first registered
    //     descriptor's vtable-slot-1 allocator.  On success, calls
    //     ke_2160 on the fresh block and returns it via r3 in the
    //     PPC ABI.  C++ signature returns uint32_t (the raw VA).
    virtual uint32_t Allocate40();              // [2] @ 0x820f7490  size 0x64
    virtual void vfn_3();                       // [3] @ 0x820f7640
    virtual void vfn_4();                       // [4] @ 0x820f74f8
    virtual void vfn_5();                       // [5] @ 0x820f7190
    virtual void vfn_6();                       // [6] @ 0x820f7290
    // [7] Cursor-advance for the shader-factory's chunked allocator:
    //     same shape as grmModelFactory::vfn_2 (buffer-base at +0x04,
    //     stride at +0x4C, per-chunk step indexed off this).  The
    //     post-advance hook is fragDrawable_3010 (shader-factory slot
    //     initialiser).
    virtual void AdvanceCursor(uint32_t* cursor);   // [7] @ 0x820f72d8  size 0x1b8
};

// ── rage::grmShaderFx  [vtable @ 0x8202F2DC] ──────────────────────────
struct grmShaderFx {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmShaderFx();                  // [0] @ 0x820eef70
    virtual void ScalarDtor(int flags); // [1] @ 0x820efee0
    virtual void vfn_2();  // [2] @ 0x820ef0c8
    virtual void vfn_3();  // [3] @ 0x820efb30
    virtual void vfn_4();  // [4] @ 0x820efbe0
    virtual void vfn_5();  // [5] @ 0x820efd28
    virtual void vfn_6();  // [6] @ 0x820efa08
    virtual void vfn_7();  // [7] @ 0x820efae8
    virtual void vfn_8();  // [8] @ 0x820ef928
    virtual void vfn_9();  // [9] @ 0x820ef9c8
    virtual void vfn_10();  // [10] @ 0x820ef9e8
    virtual void vfn_11();  // [11] @ 0x820ef060
    virtual void vfn_12();  // [12] @ 0x820f0710
    virtual void vfn_13();  // [13] @ 0x820f0738
    virtual void vfn_14();  // [14] @ 0x820ef7a0
    virtual void vfn_15();  // [15] @ 0x820ef880
    virtual void vfn_16();  // [16] @ 0x820f0730
    virtual void vfn_17();  // [17] @ 0x820f0578
};

// ── rage::grmShaderGroup  [vtable @ 0x8202F528] ──────────────────────────
struct grmShaderGroup {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // [0] PPC scalar-deleting-dtor: calls net_2228_h (internal teardown) then
    //     conditionally frees the object if (flag & 1).  See ScalarDtor below.
    virtual ~grmShaderGroup();                      // [0] @ 0x820f21d8  size 0x50
    virtual void ScalarDtor(int flags);             // [0] PPC scalar-deleting-dtor body
};

// ── rage::grmShaderPreset  [vtable @ 0x8202F668] ──────────────────────────
struct grmShaderPreset {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmShaderPreset();                  // [0] @ 0x820f5ce8
};

// ── rage::rmPtxBirth  [vtable @ 0x82079A94] ──────────────────────────
struct rmPtxBirth {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~rmPtxBirth();                  // [0] @ 0x82429438
};

// ── rage::rmPtxKeyframe  [vtable @ 0x82079A88] ──────────────────────────
struct rmPtxKeyframe {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~rmPtxKeyframe();                  // [0] @ 0x82429488
};

// ── rage::rmPtxSystem  [vtable @ 0x82079AA4] ──────────────────────────
struct rmPtxSystem {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~rmPtxSystem();                  // [0] @ 0x8242a1c8
    virtual void ScalarDtor(int flags); // [1] @ 0x8242a7b8
    virtual void vfn_2();  // [2] @ 0x8242a610
    virtual void vfn_3();  // [3] @ 0x8242af50
    virtual void vfn_4();  // [4] @ 0x8242acc8
    virtual void vfn_5();  // [5] @ 0x8242b290
    virtual void vfn_6();  // [6] @ 0x8242ab58
    virtual void vfn_8();  // [8] @ 0x8242b358
    virtual void vfn_9();  // [9] @ 0x8242b398
};

// ── rage::rmcDrawable  [vtable @ 0x8204FFB4] ──────────────────────────
struct rmcDrawable {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~rmcDrawable();                  // [0] @ 0x8225b9b8
};

// ── rage::rmcDrawableBase  [vtable @ 0x8204FF80] ──────────────────────────
struct rmcDrawableBase {
    void**      vtable;           // +0x00
};

// ── rage::rmcTypeFileParser  [vtable @ 0x82050078] ──────────────────────────
struct rmcTypeFileParser {
    void**      vtable;           // +0x00
};

// ── rage::spdApical  [vtable @ 0x82032F48] ──────────────────────────
struct spdApical {
    void**      vtable;           // +0x00
    uint8_t     pad_04[0x0C];     // +0x04  padding to reach verified fields
    float       unk_10;           // +0x10
    float       unk_14;           // +0x14
    float       unk_18;           // +0x18
    float       unk_1C;           // +0x1C

    // ── virtual methods ──
    virtual ~spdApical();                           // [0] @ 0x82122750
    // TODO: slot 1 was auto-labeled ScalarDtor but is NOT a destructor.
    // Scaffold shows it takes (this, r4=dst_offset, r5=src_vec_ptr), copies
    // 4 AltiVec vectors from r5 into the object, then calls
    // LocomotionStateAnim_7B90_g and spdShaft_5160_h.  Likely an animation
    // state init/update method.  Signature unverified — left as vfn_1.
    virtual void vfn_1(void* dst, void* src);       // [1] @ 0x821226a8
    virtual void vfn_2(float value);  // [2] @ 0x82122730  unk_10 = value
    virtual void vfn_3(float value);  // [3] @ 0x82122738  unk_14 = value
    virtual void vfn_4(float value);  // [4] @ 0x82122740  unk_18 = value
    virtual void vfn_5(float value);  // [5] @ 0x82122748  unk_1C = value

    // ── non-virtual methods ──
    void SetLodNear(float value);     // unk_10 = value
    void SetLodFar(float value);      // unk_14 = value
    void SetLodFadeStart(float value); // unk_18 = value
    void SetLodFadeEnd(float value);  // unk_1C = value
};

// ── rage::spdShaft  [vtable @ 0x82032F68] ──────────────────────────
struct spdShaft {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    // TODO: slot 1 was auto-labeled ScalarDtor but is NOT a destructor.
    // Scaffold shows it calls spdApical_vfn_1 (base update), then iterates
    // 6 animation bones via spdShaft_25A8, and writes a bool at +0x2C0.
    // Signature unverified — left as vfn_1.
    virtual void vfn_1(void* dst, void* src);       // [1] @ 0x821228b8
    virtual void vfn_2();  // [2] @ 0x823d4b68
    virtual void vfn_3();  // [3] @ 0x823d4ce8
    virtual void vfn_4();  // [4] @ 0x823d4e68
    virtual void vfn_5();  // [5] @ 0x823d4fe8
};

} // namespace rage