#pragma once
/**
 * grcTextureReferenceBase / grcTextureReference / grcLockedTexture
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>
#include "../rage/rage_dat.hpp"

namespace rage {

struct grcTexture;
struct grcTextureFactory;
struct grcSerializerCtx;

// ---------------------------------------------------------------------------
// grcSerializerCtx — lightweight state-machine context passed to Serialize()
// Layout partially confirmed from vfn_26 (grcTextureReferenceBase).
// ---------------------------------------------------------------------------
struct grcSerializerCtx {
    void*    vtable;    // +0x00 – reset to a serialiser vtable at entry
    uint32_t step;      // +0x04 – byte-offset step counter (0, 4, 5, 6, 8, 12, …)
};

// ---------------------------------------------------------------------------
// grcTextureReferenceBase  —  abstract texture reference base
// vtable:  rage::grcTextureReferenceBase @ 0x8203508C   (120 bytes, 27 entries)
// extends: rage::datBase
// ---------------------------------------------------------------------------
struct grcTextureReferenceBase : datBase {
    uint16_t   m_field04;           // +0x04
    uint16_t   m_field06;           // +0x06  – endian-swapped during Serialize()
    uint32_t   m_field08;           // +0x08  – endian-swapped during Serialize()
    void*      m_pInternalData;     // +0x0C  – freed in destructor; returned by GetHandle()

    // ── virtual interface ──
    virtual void  destructor(bool deleteThis);              // slot  0
    virtual void  vfn_1();                                  // slot  1  (abstract/subclass)
    virtual void  vfn_2();                                  // slot  2
    virtual void  vfn_3();                                  // slot  3
    virtual void* GetHandle() const;                        // slot  4  – returns m_pInternalData
    virtual void  vfn_5();                                  // slot  5
    virtual void  vfn_6();                                  // slot  6
    virtual void  ForwardSlot7(void* param);                // slot  7
    virtual void  vfn_8();                                  // slot  8
    virtual void  vfn_9();                                  // slot  9
    virtual void  vfn_10();                                 // slot 10  (abstract/subclass)
    virtual grcTexture* GetTexture() = 0;                   // slot 11  – abstract: returns bound texture
    virtual grcTexture* GetTexture2() = 0;                  // slot 12  – abstract variant (used by LOD path)
    virtual void  vfn_13(void* a, void* b);                 // slot 13
    virtual void  vfn_14(void* a, void* b);                 // slot 14
    virtual void  vfn_15();                                 // slot 15
    virtual void  vfn_16(void* a);                          // slot 16
    virtual void  vfn_17(float f, void* a);                 // slot 17  – takes float arg
    virtual void  vfn_18(void* a, void* b);                 // slot 18
    virtual void  vfn_19();                                 // slot 19  (abstract/subclass)
    virtual void  ForwardSlot20();                          // slot 20
    virtual grcTexture* vfn_21(void* a, void* b, void* c); // slot 21
    virtual void  vfn_22(void* a);                          // slot 22
    virtual void  vfn_23();                                 // slot 23  (abstract/subclass)
    virtual void  vfn_24();                                 // slot 24
    virtual void  vfn_25();                                 // slot 25
    virtual void  Serialize(grcSerializerCtx* pCtx);        // slot 26
};

// ---------------------------------------------------------------------------
// grcTextureReference  —  concrete texture reference
// vtable:  rage::grcTextureReference @ 0x82035104  (120 bytes, 27 entries)
// extends: rage::grcTextureReferenceBase
// ---------------------------------------------------------------------------
struct grcTextureReference : grcTextureReferenceBase {
    grcTexture* m_pTexture;         // +0x10  – ref-counted; released in destructor

    void  DestructorBody();                             // @ 0x8215D5F0  (rage_D5F0)
    void  destructor(bool deleteThis) override;         // slot  0  @ 0x8215D658
    grcTexture* GetTexture()  override;                 // slot 11  @ 0x8215DAC0
    void  ForwardSlot25()     override;                 // slot 25  @ 0x8215DB48
};

// ---------------------------------------------------------------------------
// grcLockedTexture  —  36-byte surface-lock descriptor
// Not an object in the OO sense; no vtable.  Allocated on the heap and
// passed to SetPixel() helpers.
// ---------------------------------------------------------------------------
struct grcLockedTexture {
    uint16_t   m_nWidth;        // +0x00
    uint16_t   m_nHeight;       // +0x02
    uint32_t   m_nFormat;       // +0x04  – 1 = ARGB32, 2 = DXT1, 3 = DXT3/5, …
    uint32_t   field_08;        // +0x08
    uint16_t   m_nPitch;        // +0x0C  – bytes per row
    uint16_t   field_0E;        // +0x0E
    uint32_t*  m_pData;         // +0x10
    uint32_t   field_14;        // +0x14
    uint32_t   field_18;        // +0x18
    uint32_t   field_1C;        // +0x1C
    uint32_t   m_nRefCount;     // +0x20

    void SetPixel(uint32_t col, uint32_t row, uint32_t value);  // @ 0x8215BA38
};

} // namespace rage
