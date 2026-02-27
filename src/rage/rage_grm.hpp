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

// ── rage::grmModel  [vtable @ 0x8202F3B4] ──────────────────────────
struct grmModel {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmModel();                  // [0] @ 0x820f0e78
};

// ── rage::grmModelFactory  [vtable @ 0x8202F5A8] ──────────────────────────
struct grmModelFactory {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmModelFactory();                  // [0] @ 0x820f3c50
    virtual void ScalarDtor(int flags); // [1] @ 0x820f3d10
    virtual void vfn_2();  // [2] @ 0x820f4cf0
};

// ── rage::grmModelGeom  [vtable @ 0x8202F3CC] ──────────────────────────
struct grmModelGeom {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmModelGeom();                  // [0] @ 0x820f0ee8
    virtual void ScalarDtor(int flags); // [1] @ 0x820f19b0
    virtual void vfn_3();  // [3] @ 0x820f1348
};

// ── rage::grmSetup  [vtable @ 0x8204E984] ──────────────────────────
struct grmSetup {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags); // [1] @ 0x82378218
    virtual void vfn_2();  // [2] @ 0x82378448
    virtual void vfn_6();  // [6] @ 0x82378338
};

// ── rage::grmShader  [vtable @ 0x8202F5C4] ──────────────────────────
struct grmShader {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmShader();                  // [0] @ 0x820f5260
    virtual void ScalarDtor(int flags); // [1] @ 0x820f5360
    virtual void vfn_6();  // [6] @ 0x820f5330
    virtual void vfn_7();  // [7] @ 0x820f5350
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
struct grmShaderFactory {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual ~grmShaderFactory();                  // [0] @ 0x820f6e60
};

// ── rage::grmShaderFactoryStandard  [vtable @ 0x8202F830] ──────────────────────────
struct grmShaderFactoryStandard {
    void**      vtable;           // +0x00

    // ── virtual methods ──
    virtual void ScalarDtor(int flags); // [1] @ 0x820f6ec8
    virtual void vfn_2();  // [2] @ 0x820f7490
    virtual void vfn_3();  // [3] @ 0x820f7640
    virtual void vfn_4();  // [4] @ 0x820f74f8
    virtual void vfn_5();  // [5] @ 0x820f7190
    virtual void vfn_6();  // [6] @ 0x820f7290
    virtual void vfn_7();  // [7] @ 0x820f72d8
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
    virtual ~grmShaderGroup();                  // [0] @ 0x820f21d8
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