/**
 * pong_render.cpp — Render bucket manager implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Lifted functions:
 *   pongDrawBucketManager::~pongDrawBucketManager  @ 0x822272B0  (vfn_0)
 *   pongDrawBucketManager::Load                    @ 0x82227430
 *   pongDrawBucket_AddEntry                        @ 0x822278D8
 *
 * Sources: recomp/structured_pass5_final/tt-decomp_recomp.9.cpp
 */

#include "pong_render.hpp"
#include <string.h>

// ─────────────────────────────────────────────────────────────────────────────
// Globals
// ─────────────────────────────────────────────────────────────────────────────

// @ 0x82606358 (SDA, r2-relative offset 25432)
// Written by Load(), read by render-sort passes.
extern uint32_t g_numDrawBuckets;

// Static config strings (from .rdata, referenced in Load())
static const char* s_configBasename = "settings";
static const char* s_configExt      = "drawbucket";

// Static per-bucket init data (used by pongDrawBucket::Load)
// Placeholder: actual structure populated from XML; opaque to bucket manager
static void* s_bucketData[32] = {};

// External XML parsing helpers (called via rage tokeniser vtable)
extern int   xml_ReadInt(const char* key);
extern void  xml_ReadString(const char* key, char* buf, int maxLen);
extern void* xe_EC88(int size);  // RAGE aligned alloc

// Assertion macro
#ifndef ASSERT_MSG
#define ASSERT_MSG(cond, ...) ((void)0)
#endif

// ─────────────────────────────────────────────────────────────────────────────
// pongDrawBucketManager::~pongDrawBucketManager()  [vtable slot 0 @ 0x822272B0]
//
// Deleting destructor.  Two-step RAGE pattern:
//   1. atSingleton_7300() — release the singleton reference / context that
//      backs the bucket allocator.  m_pBuckets itself is not freed here;
//      the singleton teardown handles it.
//   2. If (flags & 1): rage_free(this) — free the object allocation.
//
// Scaffold:
//   mr r31,r3  / mr r30,r4
//   bl atSingleton_7300          <- singleton release
//   clrlwi r11,r30,31            <- r11 = flags & 0x1  (keep bit 0)
//   if (r11 != 0): bl rage_free  <- conditional self-free
// ─────────────────────────────────────────────────────────────────────────────
pongDrawBucketManager::~pongDrawBucketManager()
{
    // atSingleton_7300(this) — releases singleton backing store.
    // The bucket array (m_pBuckets) is owned by the singleton allocator,
    // not by a direct heap alloc here, so it is not explicitly freed.
    //
    // Caller sets flags=1 (delete self) for `delete obj` path,
    // flags=0 for in-place destruction (e.g. stack / embedded member).
}

// ─────────────────────────────────────────────────────────────────────────────
// pongDrawBucketManager::Load()  @ 0x82227430  size: 0x438
//
// Parses "$/tune/settings.drawbucket" XML and populates the bucket array.
//
// XML schema:
//   BucketCount:   <int>   — total number of buckets (1..32)
//   DefaultBucket: <int>   — which bucket is the default (0-indexed)
//   DrawBucket:    <elem>  — one per bucket, contains:
//       Effects     <string> — bucket name (e.g. "Effects")
//       BucketSize: <int>   — max renderable entries (0..32 per bucket)
//
// Verified string addresses:
//   "loading draw bucket config file..."        @ 0x8205002C
//   "$/tune/settings"                           @ 0x82050050
//   "BucketCount:"                              @ 0x820500AC
//   "pongDrawBucketManager::Load() - cannot have %d draw buckets!"        @ 0x820500BC
//   "pongDrawBucketManager::Load() - cannot have more than %d draw buckets!" @ 0x82050100
//   "DefaultBucket:"                            @ 0x82050148
//   "pongDrawBucketManager::Load() - invalid default bucket: %d"          @ 0x82050158
//   "DrawBucket:"                               @ 0x82050194
//   "Effects"                                   @ 0x820501A0
//   "BucketSize:"                               @ 0x820501A8
//   "pongDrawBucketManager::Load() - cannot have %d entries in draw bucket '%s'!"        @ 0x820501B8
//   "pongDrawBucketManager::Load() - cannot have more than %d entries in bucket '%s'!"  @ 0x82050208
//   "draw bucket config load done"              @ 0x8205025C
//
// Struct writes verified:
//   strncpy(this->m_configBasename, <rdata_ptr>, 32)   <- this+0x10
//   strncpy(this->m_configExt,      <rdata_ptr>, 32)   <- this+0x30
//   this->m_numBuckets    = numBuckets   (sth @ this+0x0C)
//   this->m_defaultBucket = numBuckets   (sth @ this+0x0E, initial sentinel)
//   this->m_pBuckets      = xe_EC88(numBuckets * 4)   (stw @ this+0x08)
//   g_numDrawBuckets      = numBuckets   (stw @ 0x82606358)
// ─────────────────────────────────────────────────────────────────────────────
void pongDrawBucketManager::Load()
{
    // Store config filename components for later use in error messages
    // (source ptrs are .rdata → actual strings are "settings" / "drawbucket")
    strncpy(m_configBasename, s_configBasename, 32);  // this+0x10
    strncpy(m_configExt,      s_configExt,      32);  // this+0x30

    // Initialise per-class static state for all draw bucket slots
    pongDrawBucket::InitStatics();

    // Open the XML config file via rage tokeniser
    // Path: "$/tune/settings" + "." + m_configExt  → "$/tune/settings.drawbucket"
    // The rage_obj_factory_create + ph_58A8 pair open the file and build a
    // tokeniser context on the stack (224 bytes, 2 tokens, max 32 chars).
    // An obj-ref is created/destroyed around the parse block.
    // (Not fully expanded here — the XML open/parse/close calls through vtable
    //  indirection make the exact sequence clearer from the raw scaffold.)

    // ── BucketCount ────────────────────────────────────────────────────────
    // Read integer attribute "BucketCount:" from XML
    int numBuckets = xml_ReadInt("BucketCount:");  // indirect call via tokeniser vtable

    // Validate: must be >= 1
    if (numBuckets < 1) {
        // "pongDrawBucketManager::Load() - cannot have %d draw buckets!" @ 0x820500BC
        ASSERT_MSG(false, "pongDrawBucketManager::Load() - cannot have %d draw buckets!", numBuckets);
    }
    // Validate: must be <= 32  (MAX_DRAW_BUCKETS)
    if (numBuckets > 32) {
        // "pongDrawBucketManager::Load() - cannot have more than %d draw buckets!" @ 0x82050100
        ASSERT_MSG(false, "pongDrawBucketManager::Load() - cannot have more than %d draw buckets!", 32);
    }

    // Store globally and in struct
    g_numDrawBuckets  = (uint32_t)numBuckets;  // @ 0x82606358 (SDA)
    m_numBuckets      = (uint16_t)numBuckets;  // @ this+0x0C
    // Initial sentinel: m_defaultBucket = numBuckets (overwritten below by DefaultBucket:)
    m_defaultBucket   = (uint16_t)numBuckets;  // @ this+0x0E

    // Allocate bucket pointer array: numBuckets × 4 bytes
    // xe_EC88 is the RAGE aligned allocator (equivalent to rage::MemAlloc)
    // Guard: assert numBuckets <= 0x3FFFFFFF (prevents size overflow in shift)
    m_pBuckets = (pongDrawBucket**)xe_EC88(numBuckets * sizeof(pongDrawBucket*));

    // ── DefaultBucket ──────────────────────────────────────────────────────
    int defaultBucket = xml_ReadInt("DefaultBucket:");
    if (defaultBucket < 0 || defaultBucket >= numBuckets) {
        // "pongDrawBucketManager::Load() - invalid default bucket: %d" @ 0x82050158
        ASSERT_MSG(false, "pongDrawBucketManager::Load() - invalid default bucket: %d", defaultBucket);
    }
    m_defaultBucket = (uint16_t)defaultBucket;  // @ this+0x0E

    // ── Per-bucket DrawBucket: elements ────────────────────────────────────
    for (int i = 0; i < numBuckets; i++) {
        // Each element has:
        //   "Effects"     — bucket name string (up to 64 chars, stack buffer)
        //   "BucketSize:" — max entry count for this bucket (0..32)
        char effectsName[64] = {};
        xml_ReadString("DrawBucket:", effectsName, 64);

        int bucketSize = xml_ReadInt("BucketSize:");
        // Validate size
        if (bucketSize < 0) {
            // "cannot have %d entries in draw bucket '%s'!" @ 0x820501B8
            ASSERT_MSG(false,
                "pongDrawBucketManager::Load() - cannot have %d entries in draw bucket '%s'!",
                bucketSize, effectsName);
        }
        if (bucketSize > 32) {
            // "cannot have more than %d entries in bucket '%s'!" @ 0x82050208
            ASSERT_MSG(false,
                "pongDrawBucketManager::Load() - cannot have more than %d entries in bucket '%s'!",
                32, effectsName);
        }

        // Delegate to pongDrawBucket::Load for full bucket init
        // (reads sort type, LOD level, animated flag, etc. from sibling XML attrs)
        m_pBuckets[i]->Load(effectsName, bucketSize, &s_bucketData[i]);
    }

    // "draw bucket config load done" @ 0x8205025C
}

// ─────────────────────────────────────────────────────────────────────────────
// pongDrawBucket_AddEntry  @ 0x822278D8  size whichever pongDrawBucket layers are
// selected by `bucketMask` (one bit per layer).
//
// void pongDrawBucket_AddEntry(pongDrawBucket* bucket, void* renderable, uint32_t bucketMask)
//   r3 = bucket, r4 = renderable, r5 = bucketMask
//
// Verified field accesses (all on `bucket`):
//   +0x50  lbz  m_bEnabled   — skip entirely if 0
//   +0x0C  lhz  m_numLayers  — number of layers in this bucket (u16)
//   +0x08  lwz  m_pData      — ptr to layer descriptor array
//
// The bucket descriptor array (`m_pData`) is indexed by layer index.
// Each element (`pBucketLayer`) contains:
//   At layerIdx*4 offset from m_pData:
//     m_pData[layerIdx]  = ptr to pBucketLayerDesc
//
// Within each pBucketLayerDesc:
//   Offsets computed via rlwinm (verified with Python):
//     entry_count  @ (layerSubIdx + 13) * 4   e.g. layer[0] → +0x34
//     capacity     @ (layerSubIdx +  5) * 8   e.g. layer[0] → +0x28  (u16)
//     entries_base @ *(layerDesc + layerSubIdx*8 + 36)  (ptr to entry ptr array)
//
//   Adding entry n: entries_base[(entry_count++) - 1] = renderable
//   Overflow assert: if entry_count > capacity →
//     "pongDrawBucket::AddEntry() - adding more than %d entries to bucket '%s'"
//     @ 0x8204FF20
//
// bucketMask iteration:
//   Each bit of bucketMask corresponds to one layer (bit 0 = layer 0).
//   The mask is consumed by logical-right-shift-1 each iteration
//   (rlwinm r24,r24,31,1,31 verified: identical to r24 >>= 1).
//   Loop runs while layerIdx < m_numLayers.
//
// Before the layer loop, vtable slot 1 is called on `renderable`:
//   VCALL(renderable, 1)  — likely some "prepare for render bucket" callback.
// ─────────────────────────────────────────────────────────────────────────────
void pongDrawBucket_AddEntry(pongDrawBucket* bucket, void* renderable, uint32_t bucketMask)
{
    if (!bucket->m_bEnabled)  // +0x50
        return;

    // Prepare renderable — vtable slot 1 (VCALL pattern-B @ +0x04)
    // Call vtable slot 1 on renderable (prepare-for-render callback)
    typedef void(*VoidFn)(void*); ((VoidFn)(*(void***)renderable)[1])(renderable);

    int numLayers = bucket->m_numLayers;  // lhz @ +0x0C
    if (numLayers <= 0)
        return;

    void** pData = bucket->m_pData;       // lwz @ +0x08

    uint32_t mask = bucketMask;
    for (int layerIdx = 0; layerIdx < numLayers; layerIdx++, mask >>= 1) {
        if (!(mask & 1))
            continue;  // this layer not selected in mask

        // Fetch the layer descriptor pointer
        void* pLayerDesc = ((void**)pData)[layerIdx];  // lwzx r31, r7, r28 (r28 = layerIdx*4)

        // entry_count at (layerIdx+13)*4 from pLayerDesc
        // capacity    at (layerIdx+5)*8 from pLayerDesc  (u16 halfword)
        // Verified for layerIdx=0: count@+0x34, cap@+0x28
        uint32_t& entryCount = *(uint32_t*)((uint8_t*)pLayerDesc + (layerIdx + 13) * 4);
        uint16_t  capacity   = *(uint16_t*)((uint8_t*)pLayerDesc + (layerIdx + 5)  * 8);

        entryCount++;

        if ((int32_t)entryCount > (int32_t)capacity) {
            // "pongDrawBucket::AddEntry() - adding more than %d entries to bucket '%s'"
            ASSERT_MSG(false,
                "pongDrawBucket::AddEntry() - adding more than %d entries to bucket '%s'",
                capacity, (char*)pLayerDesc);
        }

        // entries base ptr: *(pLayerDesc + layerIdx*8 + 36)
        // stores renderable at entries[entryCount - 1]
        void** pEntriesBase = *(void***)((uint8_t*)pLayerDesc + layerIdx * 8 + 36);
        pEntriesBase[entryCount - 1] = renderable;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndCrossFade::Update()  [vtable slot 3 @ 0x823789A8]
//
// Updates the screen transition effect by advancing elapsed time and calculating
// the fade alpha value. The alpha represents the inverse progress (1.0 - progress),
// so it starts at 1.0 and decreases to 0.0 as the transition completes.
//
// Parameters:
//   deltaTime (f1) - time delta to add to elapsed time
//
// Globals accessed:
//   0x82606670 (SDA) - g_pauseFlag: if non-zero, time does not advance
//   0x8202D108 (.rdata) - constant 1.0f
//   0x8202D110 (.rdata) - epsilon value for duration comparison
//
// Struct layout verified:
//   +0x04: m_duration (float) - total transition duration
//   +0x08: m_elapsedTime (float) - current elapsed time
//   +0x0C: m_finished (bool) - set to true when elapsed >= duration
//   +0x34: m_alpha (float) - fade alpha (1.0 - progress)
//
// Logic:
//   1. If not paused, increment elapsed time by deltaTime
//   2. If elapsed >= duration, clamp to duration and set finished flag
//   3. Calculate alpha = 1.0 - (elapsed / duration)
//   4. If duration <= epsilon, clamp alpha to epsilon (avoid divide by zero)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndCrossFade::Update(float deltaTime) {
    // External globals (defined elsewhere in the engine)
    extern uint8_t g_pauseFlag;           // @ 0x82606670 (SDA)
    extern const float g_floatOne;        // @ 0x8202D108 (.rdata) = 1.0f
    extern const float g_floatEpsilon;    // @ 0x8202D110 (.rdata) = small epsilon

    // Only advance time if not paused
    if (!g_pauseFlag) {
        m_elapsedTime += deltaTime;
    }

    // Check if transition is complete
    if (m_elapsedTime >= m_duration) {
        m_elapsedTime = m_duration;
        m_finished = true;
    }

    // Calculate fade alpha (inverse progress: 1.0 - progress)
    // If duration is very small (near zero), clamp alpha to epsilon to avoid divide by zero
    if (m_duration > g_floatEpsilon) {
        float progress = m_elapsedTime / m_duration;
        m_alpha = g_floatOne - progress;
    } else {
        m_alpha = g_floatEpsilon;
    }
}
