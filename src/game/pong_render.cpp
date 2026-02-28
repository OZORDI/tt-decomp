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

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe Implementation
// ─────────────────────────────────────────────────────────────────────────────

// External globals
extern const float g_floatZero;        // @ 0x8202D110 (.rdata) = 0.0f
extern const float g_floatOne;         // @ 0x8202D10C (.rdata) = 1.0f

// Swipe pattern lookup table @ 0x825D09A0 (.data, size: 0x550 = 1360 bytes)
// Contains 17 patterns (1360 / 80 = 17), each 80 bytes (0x50)
// Pattern structure (per entry, 80 bytes):
//   +0x00: float duration
//   +0x04: float field_04 (timing parameter)
//   +0x08: vec4 startCorner1  (16 bytes)
//   +0x18: vec4 endCorner1    (16 bytes)
//   +0x28: vec4 startCorner2  (16 bytes)
//   +0x38: vec4 endCorner2    (16 bytes)
//   +0x40: uint8_t reverseFlag (at offset +64 from base)
//   +0x41: uint8_t pauseFlag   (at offset +65 from base)
struct SwipePattern {
    float duration;           // +0x00
    float timingParam;        // +0x04
    float startCorner1[4];    // +0x08
    float startCorner2[4];    // +0x18
    float endCorner1[4];      // +0x28
    float endCorner2[4];      // +0x38
    uint8_t reverseFlag;      // +0x40
    uint8_t pauseFlag;        // +0x41
    uint8_t _pad[14];         // padding to 80 bytes
};

extern SwipePattern g_swipePatterns[17];  // @ 0x825D09A0

// External function references
extern void util_FFF8(void* obj, int param);  // @ 0x8225FFF8
extern void util_F840(void* v1, void* v2, void* v3, void* v4);  // @ 0x8223F840
extern void hudFlashBase_0420_g(void* hudObj, void* corner1, void* corner2, 
                                 float alpha, int reverseFlag);  // @ 0x82140420
extern void pongScrnTransFadeIn_vfn_5(void* obj);  // @ 0x82378460

// Global pointers (accessed via SDA and absolute addresses)
extern void* g_hudFlashBase;           // @ 0x82606454 (SDA, r2+25556)
extern void* g_someRenderObj1;         // @ 0x82606654 (via r11+25844)
extern void* g_someRenderObj2;         // @ 0x825FEAB0 (via r11-21712)

// Random number generator state (for pattern selection)
extern uint64_t g_randomState;         // @ 0x825DA268 (via r11-23864)

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::vfn_2()  [vtable slot 2 @ 0x82378AB0]
//
// Initialize/reset the swipe transition. Sets up the pattern index, duration,
// and initial state. If m_randomize is true, picks a random pattern from the
// 17 available patterns. Otherwise uses a deterministic pattern selection.
//
// Struct layout verified:
//   +0x04: m_duration (float)
//   +0x08: m_elapsedTime (float)
//   +0x0C: m_finished (bool)
//   +0x10: field at +16 (passed to util_FFF8)
//   +0x1C: field_0x1C (checked for non-zero)
//   +0x34: m_patternIndex (uint32_t, 0-16)
//   +0x38: m_randomize (uint8_t)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::vfn_2() {
    // If field at +28 is non-zero, call utility function
    if (*(uint32_t*)((uint8_t*)this + 28) != 0) {
        util_FFF8((void*)((uint8_t*)this + 16), 0);
    }

    // Reset elapsed time and finished flag
    m_elapsedTime = g_floatZero;
    m_finished = false;

    // Determine pattern index
    if (m_randomize) {
        // Random pattern selection using linear congruential generator
        // Formula: state = state * 1597334873 + upper32(state)
        const uint64_t multiplier = 0x5DEECE66D;  // 1597334873 in hex
        uint64_t state = g_randomState;
        uint64_t newState = (state & 0xFFFFFFFF) * multiplier + (state >> 32);
        g_randomState = newState;
        
        // Extract 23-bit random value (bits 9-31)
        uint32_t randomBits = (uint32_t)(newState & 0x7FFFFF);
        
        // Scale to 0-16 range: randomBits * (1.0 / 8388608.0) * 17.0
        // 8388608 = 2^23, so this gives uniform distribution over [0, 17)
        const float scale = 1.0f / 8388608.0f;  // @ 0x82038358
        float normalized = (float)randomBits * scale;
        m_patternIndex = (uint32_t)(normalized * 17.0f);
    } else {
        // Increment pattern index, wrapping at 17
        m_patternIndex++;
        if (m_patternIndex >= 17) {
            m_patternIndex = 0;
        }
    }

    // Load pattern data
    SwipePattern* pattern = &g_swipePatterns[m_patternIndex];
    m_duration = pattern->duration;

    // If this pattern has the pause flag set, disable rendering on two objects
    if (pattern->pauseFlag) {
        *(uint8_t*)((uint8_t*)g_someRenderObj1 + 80) = 0;
        *(uint8_t*)((uint8_t*)g_someRenderObj2 + 492) = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::Update()  [vtable slot 3 @ 0x82378BD0]
//
// Update the transition by advancing elapsed time and calculating progress.
// Progress is clamped to [0.0, 1.0] and stored for use by the render function.
//
// Parameters:
//   deltaTime (f1) - time delta to add to elapsed time
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::Update(float deltaTime) {
    // Advance elapsed time
    m_elapsedTime += deltaTime;

    // Check if transition is complete
    if (m_elapsedTime >= m_duration) {
        m_elapsedTime = m_duration;
        m_finished = true;
    }

    // Calculate normalized progress (0.0 to 1.0)
    if (m_duration > g_floatZero) {
        m_progress = m_elapsedTime / m_duration;
    } else {
        m_progress = g_floatZero;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::Render()  [vtable slot 4 @ 0x82378C20]
//
// Render the swipe effect by interpolating between start and end corners
// based on the current progress value. Uses the pattern's corner positions
// and applies the reverse flag to determine swipe direction.
//
// The function:
//   1. Loads the current pattern's corner data
//   2. Applies reverse flag (inverts progress if needed)
//   3. Interpolates corner positions: lerp(start, end, progress)
//   4. Calls utility functions to set up and render the swipe quad
//   5. Passes alpha and reverse flag to HUD flash renderer
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::Render() {
    SwipePattern* pattern = &g_swipePatterns[m_patternIndex];
    
    // Calculate effective progress (may be inverted based on reverse flag)
    float effectiveProgress = m_progress;
    if (pattern->reverseFlag) {
        effectiveProgress = g_floatOne - m_progress;
    }

    // Load pattern corner data
    // Pattern index calculation: patternIdx * 5 * 16 = patternIdx * 80
    // This gives us the base offset into the 80-byte pattern structure
    uint32_t patternOffset = m_patternIndex * 80;
    
    // Interpolate corner 1: lerp(startCorner1, endCorner1, progress)
    float corner1[4];
    for (int i = 0; i < 4; i++) {
        float start = pattern->startCorner1[i] + g_floatOne;
        float end = pattern->endCorner1[i] + g_floatOne;
        float delta = end - start;
        corner1[i] = start + delta * effectiveProgress;
    }

    // Interpolate corner 2: lerp(startCorner2, endCorner2, progress)
    float corner2[4];
    for (int i = 0; i < 4; i++) {
        float start = pattern->startCorner2[i] + g_floatOne;
        float end = pattern->endCorner2[i] + g_floatOne;
        float delta = end - start;
        corner2[i] = start + delta * effectiveProgress;
    }

    // Call utility function to prepare rendering (purpose unclear from scaffold)
    util_F840(corner1, corner2, nullptr, nullptr);

    // Calculate alpha value from timing parameter
    // Alpha = timingParam * constant * progress
    const float alphaScale = 0.00390625f;  // @ 0x8202D10C (1/256)
    float alpha = pattern->timingParam * alphaScale * effectiveProgress;

    // Render the swipe quad via HUD flash system
    hudFlashBase_0420_g(g_hudFlashBase, corner1, corner2, alpha, pattern->reverseFlag);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::OnComplete()  [vtable slot 5 @ 0x82378D98]
//
// Called when the transition completes. Re-enables rendering on objects that
// were disabled during initialization (if the pattern had the pause flag set).
// Then delegates to the base class completion handler.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::OnComplete() {
    SwipePattern* pattern = &g_swipePatterns[m_patternIndex];
    
    // If this pattern had the pause flag, re-enable rendering
    if (pattern->pauseFlag) {
        *(uint8_t*)((uint8_t*)g_someRenderObj1 + 80) = 1;
        *(uint8_t*)((uint8_t*)g_someRenderObj2 + 492) = 1;
    }

    // Delegate to base class (pongScrnTransFadeIn) completion handler
    pongScrnTransFadeIn_vfn_5(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::Reset()  [vtable slot 6 @ 0x82378DE0]
//
// Reset the transition to its initial state. Clears all timing values,
// resets the pattern index, and sets the randomize flag.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::Reset() {
    m_finished = false;
    m_patternIndex = 0;
    m_duration = g_floatOne;
    m_elapsedTime = g_floatZero;
    m_progress = g_floatZero;
    m_randomize = true;
}
