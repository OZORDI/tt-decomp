/**
 * grc_render.hpp — RAGE graphics render classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * 9 classes (grcTextureReferenceBase + grcTextureReference are in
 * texture_reference.hpp — include that file directly; they are NOT
 * redeclared here to avoid duplicate-definition errors).
 *
 * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map
 *          | debug_string_field_map | rtti_class_hierarchy
 */
#pragma once
#include <stdint.h>

// grcTextureReferenceBase and grcTextureReference live here:
#include "texture_reference.hpp"

namespace rage {

// ── rage::grcDirtyDisc  [vtable @ 0x82034A68] ─────────────────────────────
// Manages disc-read error handling and retry logic.
// vfn_2 (0x82151168) performs complex initialization with fiber TLS setup.
struct grcDirtyDisc {
    void** vtable;  // +0x00

    virtual ~grcDirtyDisc();             // [0] @ 0x82151110
    virtual void ScalarDtor(int flags);  // [1] @ 0x821511F8
    virtual void vfn_2();                // [2] @ 0x82151168  fiber TLS init
};

// ── rage::grcRenderTargetXenon  [vtable @ 0x82035344] ─────────────────────
struct grcRenderTargetXenon {
    void** vtable;  // +0x00

    virtual ~grcRenderTargetXenon();    // [0] @ 0x8215E750
    virtual void vfn_7();               // [7] @ 0x8215EB28
    virtual void vfn_8();               // [8] @ 0x8215DD58
    virtual void vfn_9();               // [9] @ 0x8215DD60
    virtual void vfn_13();              // [13] @ 0x8215DD68
    virtual void vfn_14();              // [14] @ 0x8215DD78
    virtual void vfn_15();              // [15] @ 0x8215DDB0
    virtual void vfn_16();              // [16] @ 0x8215DD90
    virtual void vfn_18();              // [18] @ 0x82151250
    virtual void vfn_20();              // [20] @ 0x8215EB10
    virtual void vfn_29();              // [29] @ 0x8215EC28
};

// ── rage::grcSetup  [vtable @ 0x82035068] ────────────────────────────────
struct grcSetup {
    void** vtable;  // +0x00

    virtual void ScalarDtor(int flags); // [1] @ 0x8215CC18
    virtual void vfn_2();               // [2] @ 0x82161D40
    virtual void vfn_3();               // [3] @ 0x82161DB8
    virtual void vfn_4();               // [4] @ 0x8215D0D0
    virtual void vfn_5();               // [5] @ 0x82162178
    virtual void vfn_6();               // [6] @ 0x821620D0
    virtual void vfn_13();              // [13] @ 0x82162420
};

// ── rage::grcTextureDictionaryReference  [vtable @ 0x8203517C] ───────────
struct grcTextureDictionaryReference {
    void** vtable;  // +0x00

    virtual ~grcTextureDictionaryReference();  // [0] @ 0x8215DCF0
};

// ── rage::grcTextureFactory  [vtable @ 0x82034B8C] ────────────────────────
// Global singleton @ 0x825EBA20 (SDA-relative, r13 base)
struct grcTextureFactory {
    void** vtable;  // +0x00

    virtual ~grcTextureFactory();  // [0] @ 0x82151338

    // [13] @ 0x821514E8 — texture lookup by name (hash table + strcmp)
    virtual void vfn_13();

    // [14] @ 0x821515A0 — texture creation from .wtd file
    virtual void vfn_14();

    // [15] @ 0x821518E8 — texture finalization / GPU upload
    virtual void vfn_15();
};

// ── rage::grcTextureFactoryString  [vtable @ 0x82035504] ─────────────────
struct grcTextureFactoryString {
    void** vtable;  // +0x00

    virtual void ScalarDtor(int flags); // [1] @ 0x8215FEF8
    virtual void vfn_2();               // [2] @ 0x8215FE80
    virtual void vfn_3();               // [3] @ 0x82160038
};

// ── rage::grcTextureFactoryXenon  [vtable @ 0x82035444] ──────────────────
// Platform-specific Xenon texture factory; handles DXT compression,
// tiling, and GPU memory management.
struct grcTextureFactoryXenon {
    void** vtable;  // +0x00

    virtual ~grcTextureFactoryXenon();   // [0] @ 0x8215E0E0
    virtual void ScalarDtor(int flags);  // [1] @ 0x8215E260
    virtual void vfn_2();                // [2] @ 0x8215E180
    virtual void vfn_3();                // [3] @ 0x8215E688
    virtual void vfn_4();                // [4] @ 0x8215E6E0
    virtual void vfn_5();                // [5] @ 0x8215E6F8
    virtual void vfn_6();                // [6] @ 0x8215E308
    virtual void vfn_7();                // [7] @ 0x8215E390
    virtual void vfn_8();                // [8] @ 0x8215E528
    virtual void vfn_10();               // [10] @ 0x8215FCB0
};

// ── rage::grcTextureString  [vtable @ 0x8203548C] ────────────────────────
struct grcTextureString {
    void** vtable;  // +0x00

    virtual ~grcTextureString();  // [0] @ 0x8215FFC8
    virtual void vfn_14();        // [14] @ 0x8215FE70
};

// ── rage::grcTextureXenon  [vtable @ 0x820353CC] ─────────────────────────
struct grcTextureXenon {
    void** vtable;  // +0x00

    virtual ~grcTextureXenon();   // [0] @ 0x8215F198
    virtual void vfn_7();         // [7] @ 0x8215FB88
    virtual void vfn_13();        // [13] @ 0x8215DDB8
    virtual void vfn_14();        // [14] @ 0x8215DDC8
    virtual void vfn_15();        // [15] @ 0x8215DDE8
    virtual void vfn_16();        // [16] @ 0x8215DDE0
    virtual void vfn_18();        // [18] @ 0x8215DDF0
    virtual void vfn_20();        // [20] @ 0x8215FC98
    virtual void vfn_21();        // [21] @ 0x8215FAB0
    virtual void vfn_22();        // [22] @ 0x8215FB70
};

} // namespace rage

// ── grcDevice GPU command submission ──────────────────────────────────────
// Non-class free functions that operate on the two flavours of grcDevice.

// grcDevice_beginScene_5E78 @ 0x82305E78
// Gates scene start on render-state conditions, then dispatches to the
// attached grcRenderTargetXenon.  pDevice points to the 0x8271A81C instance.
extern void grcDevice_beginScene(void* pDevice);

// grcDevice_clear_9290 @ 0x82379290
// Advances the render-command ring, copies viewport rects into the new
// entry, executes it, then resets queue state.  pDevice points to 0x8271A320.
extern void grcDevice_clear(void* pDevice);

// Default viewport rectangle (16 bytes, float[4]) @ 0x8261A0C0
extern const uint8_t g_defaultViewportRect[16];
