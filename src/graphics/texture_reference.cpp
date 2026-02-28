/**
 * grcTextureReferenceBase / grcTextureReference — RAGE texture reference wrapper
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * grcTextureReferenceBase is the abstract base for all texture reference objects.
 * It wraps an underlying grcTexture* and delegates most vtable calls to it. When
 * no inner texture is bound, virtual methods that need one fall back to the global
 * grcTextureFactory singleton (g_pTextureFactory @ 0x825EBA20) to obtain a default.
 *
 * The class owns a heap-allocated internal data block (m_pInternalData, offset +0x0C)
 * that is freed in the destructor. m_field06 (uint16) and m_field08 (uint32) are
 * endian-swapped during serialisation depending on a global dirty flag.
 *
 * grcTextureReference is the concrete subclass that holds the actual grcTexture*
 * (m_pTexture, offset +0x10) and implements GetTexture() (slot 11).
 *
 * grcLockedTexture is a separate 36-byte descriptor passed to SetPixel() / BA38 to
 * describe a temporarily locked texture surface.
 *
 * Vtable: rage::grcTextureReferenceBase @ 0x8203508C  (120 bytes, 27 entries)
 *         rage::grcTextureReference    @ 0x82035104  (120 bytes, 27 entries)
 */

#include "texture_reference.hpp"
#include "rage/memory.h"

namespace rage {

// ---------------------------------------------------------------------------
// Global texture factory singleton
// @ 0x825EBA20   .data   4 bytes
// ---------------------------------------------------------------------------
extern grcTextureFactory* g_pTextureFactory;   // SDA-relative, r13 base

// Global endian-swap / dirty-disc flag used during serialisation
// @ 0x82606562   .data   1 byte   SDA offset +25954
extern uint8_t g_bTextureEndianDirty;

// ---------------------------------------------------------------------------
// Struct definitions are in texture_reference.hpp — see that file.

// Additional declarations for incomplete types used in this TU.
struct grcTexture {
    void** vtable;
    virtual void vfn_7(void* param);
    virtual void vfn_20();
    // slot 25 — forwarded by grcTextureReferenceBase::ForwardSlot25
    // Called via raw vtable offset: slot 25 = byte offset +100
    void vfn_25_raw() { (*(void(**)(grcTexture*))((char*)this + 100))(this); }
};
struct grcTextureFactory {
    void** vtable;
    virtual grcTexture* GetDefaultTexture();
};

// RAGE_ASSERT — stripped in release builds, mapped to no-op here
#ifndef RAGE_ASSERT
#define RAGE_ASSERT(expr) ((void)(expr))
#endif

// Release helper for grcTexture ref-counting (rage_2E18)
extern void Release(grcTexture* pTex);

// ---------------------------------------------------------------------------
// grcTextureReferenceBase  —  virtual destructor (slot 0)
// @ 0x8215D580 | size: 0x6C
//
// Resets the vtable, frees m_pInternalData (via rage_free), then sets the
// datBase vtable and optionally frees `this` if deleteThis is set.
// This is the base-class-only cleanup path; the subclass destructor
// (grcTextureReference_vfn_0) extends this by also releasing m_pTexture.
// ---------------------------------------------------------------------------
void grcTextureReferenceBase::destructor(bool deleteThis)
{
    // Reset vtable to this class before cleanup (standard dtor preamble)
    // vtable reset to grcTextureReferenceBase @ 0x8203508C (PPC preamble, no-op in C++)

    rage_free(m_pInternalData);

    // Reset vtable down to datBase (inlined base-class destruction)
    // vtable reset to datBase @ 0x820276C4 (PPC base-class chain, no-op in C++)

    if (deleteThis)
        rage_free(this);
}

// ---------------------------------------------------------------------------
// grcTextureReferenceBase::GetHandle  —  slot 4
// @ 0x8215D6A8 | size: 0x08
//
// Returns the raw internal data pointer. Used by callers that need the
// underlying platform handle without going through the texture wrapper.
// ---------------------------------------------------------------------------
void* grcTextureReferenceBase::GetHandle() const
{
    return m_pInternalData;   // +0x0C
}

// ---------------------------------------------------------------------------
// grcTextureReferenceBase  —  slot 7 forwarding method
// @ 0x8215D6B0 | size: 0x6C
//
// Delegates vtable slot 7 to the bound inner texture (obtained via GetTexture,
// slot 11). If no inner texture is bound, falls back to the global factory
// singleton so the call is never silently dropped.
//
// r4 (param) is forwarded unchanged; semantics depend on the concrete texture
// type's slot 7 implementation.  TODO: identify slot-7 name when texture
// vtable layout is confirmed.
// ---------------------------------------------------------------------------
void grcTextureReferenceBase::ForwardSlot7(void* param)
{
    grcTexture* pTex = GetTexture();   // VCALL slot 11

    if (pTex)
    {
        // Forward to the inner texture's slot 7
        pTex->vfn_7(param);            // VCALL slot 7, byte offset +28
    }
    else
    {
        // Fallback: use the factory singleton's default texture
        grcTexture* pDefault = g_pTextureFactory->GetDefaultTexture();  // VCALL slot 7
        pDefault->vfn_7(param);
    }
}

// ---------------------------------------------------------------------------
// grcTextureReferenceBase  —  slot 20 forwarding method
// @ 0x8215D510 | size: 0x6C
//
// Same delegation pattern as slot 7 but for vtable slot 20 (byte offset +80).
// Falls back to g_pTextureFactory when no inner texture is bound.
//
// TODO: identify slot-20 semantic name once grcTexture vtable is mapped.
// ---------------------------------------------------------------------------
void grcTextureReferenceBase::ForwardSlot20()
{
    grcTexture* pTex = GetTexture();   // VCALL slot 11

    if (pTex)
    {
        pTex->vfn_20();                // VCALL slot 20, byte offset +80
    }
    else
    {
        // Factory fallback
        grcTexture* pDefault = g_pTextureFactory->GetDefaultTexture();
        pDefault->vfn_20();            // VCALL slot 20
    }
}

// ---------------------------------------------------------------------------
// grcTextureReferenceBase::Serialize  —  slot 26
// @ 0x82151B38 | size: 0x138
//
// State-machine serialiser for the texture reference.  The caller passes a
// serialiser context (pCtx, r4) that tracks progress via a step counter at
// offset +0x04.  Steps progress: 0 → 4 → 5 → 6 → 8 → 12.
//
// When g_bTextureEndianDirty is set the function byte-swaps m_field06 (uint16)
// and m_field08 (uint32) in place so that the binary representation is
// endian-correct for the target format.  The assert-like checks call
// nop_8240E6D0 (a no-op in this build — presumably stripped debug asserts).
//
// pCtx layout (partial):
//   +0x00  vtable  — reset to lbl_8202F698 at entry
//   +0x04  uint32  — step counter (stream byte offset progress)
// ---------------------------------------------------------------------------
void grcTextureReferenceBase::Serialize(grcSerializerCtx* pCtx)
{
    // Assign serialiser vtable (0x8202F698 — 3-slot base, identity unknown)
    pCtx->vtable = reinterpret_cast<void*>(0x8202F698u);

    // Initialise step counter to 4 on first call
    if (pCtx->step == 0)
        pCtx->step = 4;

    // Step 4: first field
    RAGE_ASSERT(pCtx->step == 4);   // nop_8240E6D0 in release build
    pCtx->step++;   // → 5

    // Step 5: second field
    RAGE_ASSERT(pCtx->step == 5);
    pCtx->step++;   // → 6

    // Step 6: third field
    RAGE_ASSERT(pCtx->step == 6);
    pCtx->step += 2;    // skip step 7 → 8

    // Conditional endian-swap of m_field06 (uint16) when dirty
    if (g_bTextureEndianDirty)
    {
        uint16_t v = m_field06;
        m_field06 = static_cast<uint16_t>((v >> 8) | (v << 8));
    }

    RAGE_ASSERT(pCtx->step == 8);
    pCtx->step += 4;    // → 12

    // Conditional endian-swap of m_field08 (uint32) when dirty
    if (g_bTextureEndianDirty)
    {
        uint32_t v = m_field08;
        m_field08 = ((v & 0x000000FFu) << 24)
                  | ((v & 0x0000FF00u) <<  8)
                  | ((v & 0x00FF0000u) >>  8)
                  | ((v & 0xFF000000u) >> 24);
    }

    RAGE_ASSERT(pCtx->step == 12);
}

// ---------------------------------------------------------------------------
// grcLockedTexture::SetPixel  (static helper, named BA38_p46)
// @ 0x8215BA38 | size: 0x248
//
// Writes a single pixel (or block-encoded value) into the locked texture
// surface at position (col, row), dispatching on m_nFormat (1-based index).
//
// Parameters:
//   pTex   — locked texture descriptor
//   col    — column  (pixel x for uncompressed; block-column for compressed)
//   row    — row     (pixel y for uncompressed; block-row    for compressed)
//   value  — ARGB pixel value, or DXT colour/alpha selector for compressed formats
//
// Format dispatch (m_nFormat - 1):
//   0  (fmt 1) — 32-bit ARGB, stride-based row addressing, stwx store
//   1  (fmt 2) — DXT1 / DXT3 compressed block; encodes value as colour endpoint
//                and updates the 4x4 block's colour index bits at (col, row)
//   2  (fmt 3) — DXT block with 16-bit-per-texel alpha; similar to fmt 2
//   5  (fmt 6) — 8-bit channel (byte) with rotation extract, stbx store
//   6  (fmt 7) — 8-bit channel, direct stbx store
//  13  (fmt 14) — 32-bit ARGB (same as fmt 1)
//  14  (fmt 15) — 32-bit ARGB (same as fmt 1)
//  3,4,7..12   — no-op (unsupported or planar formats not handled here)
//
// For DXT formats the function manipulates the 8-byte DXT1 block in-place:
//   bytes 0-1 : colour0 endpoint (uint16, initialised to 0x0000)
//   bytes 2-3 : colour1 endpoint (uint16, initialised to 0xFFFF)
//   byte  4   : 2-bit index field, one pair of bits per texel in the block
//               bit-pairs are addressed by the 2-bit sub-position within
//               the texel (col&3, row&3) packed as a block coordinate.
// ---------------------------------------------------------------------------
void grcLockedTexture::SetPixel(uint32_t col, uint32_t row, uint32_t value)
{
    uint32_t fmt = m_nFormat - 1;   // 0-based format index

    if (fmt > 14)
        return;

    switch (fmt)
    {
    // ── Format 1 (ARGB32) : stride-based linear write ──────────────────────
    case 0:
    case 13:
    case 14:
    {
        // dest = m_pData + (row * m_nPitch + col * 4) bytes
        uint32_t byteOff = static_cast<uint32_t>(m_nPitch) * row + (col << 2);
        reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(m_pData) + byteOff
        )[0] = value;
        break;
    }

    // ── Format 2 (DXT1 colour block) ───────────────────────────────────────
    case 1:
    {
        // Compute DXT block position.
        // Blocks are 4×4 texels; width is stored in bits 31:2 of m_nWidth.
        // block_x = col / 4,  block_y = row / 4
        // block index in row = (m_nWidth >> 2) * (row / 4) + (col / 4)
        // Each DXT1 block is 8 bytes; sub-texel is (col&3, row&3).

        uint32_t blockW  = (m_nWidth >> 2);            // blocks per row
        uint32_t blockRow = row >> 2;
        uint32_t blockCol = col >> 2;
        uint32_t blockIdx = blockW * blockRow + blockCol;

        // sub-texel position within block: 2 bits each packed as low bits
        uint32_t subX = col & 3;   // 0-3
        uint32_t subY = row & 3;   // 0-3
        uint32_t bitPos = subX + (subY * 4) * 2;       // TODO: verify packing

        // DXT1 block at blockIdx * 8 bytes into the data buffer
        struct Dxt1Block { uint16_t c0; uint16_t c1; uint8_t idx[4]; };
        uint8_t* pBlocks = reinterpret_cast<uint8_t*>(m_pData);
        Dxt1Block* pBlock = reinterpret_cast<Dxt1Block*>(pBlocks + blockIdx * 8);

        // Initialise block endpoints on first touch
        pBlock->c0 = 0x0000;
        pBlock->c1 = 0xFFFF;

        // Compute bit shift within the index byte for this texel
        uint32_t byteIdx = bitPos >> 3;
        uint32_t bitShift = (subX & 7u) * 2u;          // 2-bit index shift

        // Clear the existing 2-bit index for this texel, then write new value
        uint8_t mask = static_cast<uint8_t>(3u << bitShift);
        pBlock->idx[byteIdx] &= ~mask;

        uint32_t clampedValue = value & 0xFFFFFF00u;    // strip low byte
        if      (value == 0xFFFFFFFF00u) { /* colour1 */  pBlock->idx[byteIdx] |= static_cast<uint8_t>(1u << bitShift); }
        else if (value != 0u)            { /* colour0+1 */ pBlock->idx[byteIdx] |= static_cast<uint8_t>(2u << bitShift); }
        else                             { /* colour0 */   pBlock->idx[byteIdx] |=  static_cast<uint8_t>(3u << bitShift); }
        // (void)clampedValue;  // suppress warning
        break;
    }

    // ── Format 3 (DXT3 / 16-bit-alpha block) ───────────────────────────────
    case 2:
    {
        // Similar to fmt 2 but blocks are 16 bytes (4-bit alpha per texel
        // precedes the colour block).
        uint32_t blockW   = (m_nWidth >> 2);
        uint32_t blockRow = row >> 2;
        uint32_t blockCol = col >> 2;
        uint32_t blockIdx = blockW * blockRow + blockCol;
        uint32_t subX     = col & 3;
        uint32_t subY     = row & 3;
        uint32_t bitPos   = subX + (subY * 4) * 2;

        // DXT3 block is 16 bytes; colour block starts at offset +8
        uint8_t* pBlock16 = reinterpret_cast<uint8_t*>(m_pData) + blockIdx * 16;
        uint16_t* pColour = reinterpret_cast<uint16_t*>(pBlock16 + 8);

        pColour[3] = 0xFFFF;    // c1 = white
        pColour[2] = 0;         // word before colour endpoints
        pColour[0] = 0;         // c0 = black
        pColour[1] = 0;

        uint32_t bitShift = (subX & 1u) * 2u;
        uint32_t byteOff  = bitPos >> 3;
        uint8_t  mask     = static_cast<uint8_t>(3u << bitShift);
        uint8_t* pIdx     = pBlock16 + 10 + byteOff;
        *pIdx &= ~mask;

        uint32_t alphaLow = value & 0xFFFFFFu;
        if      (value == 0xFFFFFFFF00u) { *pIdx |= static_cast<uint8_t>(1u << bitShift); }
        else if (alphaLow != 0u)         { *pIdx |= static_cast<uint8_t>(2u << bitShift); }
        else                             { *pIdx |= static_cast<uint8_t>(3u << bitShift); }
        break;
    }

    // ── Format 6 (8-bit, rotated extract) ──────────────────────────────────
    case 5:
    {
        uint8_t byte = static_cast<uint8_t>((value >> 8) & 0xFF);  // rlwinm r6,r6,8,24,31
        uint32_t byteOff = static_cast<uint32_t>(m_nPitch) * row + col;
        reinterpret_cast<uint8_t*>(m_pData)[byteOff] = byte;
        break;
    }

    // ── Format 7 (8-bit, direct low byte) ──────────────────────────────────
    case 6:
    {
        uint8_t byte = static_cast<uint8_t>(value & 0xFF);
        uint32_t byteOff = static_cast<uint32_t>(m_nPitch) * row + col;
        reinterpret_cast<uint8_t*>(m_pData)[byteOff] = byte;
        break;
    }

    // ── Unsupported formats — no-op ─────────────────────────────────────────
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
// grcTextureReference  —  destructor body (rage_D5F0)
// @ 0x8215D5F0 | size: 0x68
//
// Extends the base destructor to also release m_pTexture (+0x10) via
// the ref-counted Release() helper (rage_2E18).
// Called from the slot-0 virtual destructor below; never called directly.
// ---------------------------------------------------------------------------
void grcTextureReference::DestructorBody()
{
    // Preamble: reset vtable to this class (matches grcTextureReference @ 0x82035104)
    // [vtable reset - compiler managed]

    // Release the inner texture (decrement refcount; deletes if it reaches zero)
    if (m_pTexture)
        Release(m_pTexture);    // rage_2E18 @ 0x820C2E18

    // Chain to base-class cleanup (resets vtable to grcTextureReferenceBase, then datBase)
    // [vtable reset - compiler managed]
    rage_free(m_pInternalData);
    // [vtable reset - compiler managed]
}

// ---------------------------------------------------------------------------
// grcTextureReference  —  virtual destructor (slot 0)
// @ 0x8215D658 | size: 0x50
// ---------------------------------------------------------------------------
void grcTextureReference::destructor(bool deleteThis)
{
    DestructorBody();
    if (deleteThis)
        rage_free(this);
}

// ---------------------------------------------------------------------------
// grcTextureReference::GetTexture  —  slot 11
// @ 0x8215DAC0 | size: 0x28
//
// Returns the "resolved" inner texture object by calling m_pTexture->vfn_12().
// vfn_12 on the underlying grcTexture likely returns the platform resource
// handle (e.g. IDirect3DTexture9 equivalent for Xenon).
// Returns nullptr if m_pTexture is not set.
// ---------------------------------------------------------------------------
grcTexture* grcTextureReference::GetTexture()
{
    if (m_pTexture)
        // VCALL grcTexture slot 12 — resource/variant texture getter
        return (*(grcTexture*(**)(grcTexture*))((char*)m_pTexture + 48))(m_pTexture);
    return nullptr;
}

// ---------------------------------------------------------------------------
// grcTextureReference  —  slot 25 forwarding
// @ 0x8215DB48 | size: 0x24
//
// Simple one-way delegation to m_pTexture->vfn_25().
// No factory fallback; returns early if no inner texture.
// ---------------------------------------------------------------------------
void grcTextureReference::ForwardSlot25()
{
    if (!m_pTexture)
        return;
    ((void(*)(grcTexture*))(*(void***)m_pTexture)[25])(m_pTexture);  // VCALL grcTexture slot 25
}

} // namespace rage
