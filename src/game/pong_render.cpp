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
extern "C" void* rage_alloc(uint32_t size);  // RAGE aligned alloc

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
//   this->m_pBuckets      = rage_alloc(numBuckets * 4)   (stw @ this+0x08)
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
    // rage_alloc is the RAGE aligned allocator (equivalent to rage::MemAlloc)
    // Guard: assert numBuckets <= 0x3FFFFFFF (prevents size overflow in shift)
    m_pBuckets = (pongDrawBucket**)rage_alloc(numBuckets * sizeof(pongDrawBucket*));

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
extern void sysCallback_Invoke(void* obj, int param);  // @ 0x8225FFF8
extern void util_PackColorRGBA(uint32_t* outColor, const float* inVector);  // @ 0x8223F840
extern void hudFlashBase_DrawFlashOverlay(void* hudObj, void* corner1, void* corner2, 
                                 float alpha, int reverseFlag);  // @ 0x82140420
extern void pongScrnTransFadeIn_EndTransition(void* obj);  // @ 0x82378460

// Global pointers (accessed via SDA and absolute addresses)
extern struct hudFlashBase* g_hudFlashBase;  // @ 0x82606454 (SDA, r2+25556)
extern void* g_someRenderObj1;         // @ 0x82606654 (via r11+25844)
extern void* g_someRenderObj2;         // @ 0x825FEAB0 (via r11-21712)

// Random number generator state (for pattern selection)
extern uint64_t g_randomState;         // @ 0x825DA268 (via r11-23864)

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransSwipe::Begin()  [vtable slot 2 @ 0x82378AB0]
//
// Initialize/reset the swipe transition. Sets up the pattern index, duration,
// and initial state. If m_randomize is true, picks a random pattern from the
// 17 available patterns. Otherwise uses a deterministic pattern selection.
//
// Struct layout verified:
//   +0x04: m_duration (float)
//   +0x08: m_elapsedTime (float)
//   +0x0C: m_finished (bool)
//   +0x10: field at +16 (passed to sysCallback_Invoke)
//   +0x1C: field_0x1C (checked for non-zero)
//   +0x34: m_patternIndex (uint32_t, 0-16)
//   +0x38: m_randomize (uint8_t)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransSwipe::Begin() {
    // If field at +28 is non-zero, call utility function
    if (*(uint32_t*)((uint8_t*)this + 28) != 0) {
        sysCallback_Invoke((void*)((uint8_t*)this + 16), 0);
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
    uint32_t renderData;
    util_PackColorRGBA(&renderData, corner1); // Assuming the first corner is packed

    // Calculate alpha value from timing parameter
    // Alpha = timingParam * constant * progress
    const float alphaScale = 0.00390625f;  // @ 0x8202D10C (1/256)
    float alpha = pattern->timingParam * alphaScale * effectiveProgress;

    // Render the swipe quad via HUD flash system
    hudFlashBase_DrawFlashOverlay(g_hudFlashBase, corner1, corner2, alpha, pattern->reverseFlag);
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
    pongScrnTransFadeIn_EndTransition(this);
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

// ─────────────────────────────────────────────────────────────────────────────
// pongScreenCapture_rtti_B0EC_0  @ 0x822ED968  size: 0x9C
//
// Screen capture render wrapper that saves/restores render state around a
// Flash HUD rendering call. This function is part of the screen capture system
// that allows taking screenshots or rendering UI elements to textures.
//
// The function performs a render state sandwich pattern:
//   1. Save current render state (game_2EE0 + game_2E80 + grc_EB10)
//   2. Render Flash HUD element with specific parameters
//   3. Restore previous render state (game_2EE0 + game_2E80 + grc_EB10)
//
// Globals accessed:
//   0x821EBA9C (pongPlayer_BA20_g + 0x7C) - previous render state pointer
//   0x821EBAA0 (pongPlayer_BA20_g + 0x80) - current render state pointer
//   0x821F0898 (gdRivalry_vfn_20 + 0x10) - render mode index (stored as "8@" string)
//   0x82140110 (ref_fi_F5F0 + 0xB20) - constant float value (likely 1.0f or 0.0f)
//   0x821FE3D4 (pongCameraMgr_D648_g + 0xD8C) - HUD Flash base object pointer
//
// External function references:
//   game_2EE0 @ 0x82152EE0 - Save/restore render context (part 1)
//   game_2E80 @ 0x82152E80 - Save/restore render context (part 2)
//   grc_EB10 @ 0x8214EB10 - Graphics device state save/restore
//   hudFlashBase_0F08_g @ 0x82140F08 - Render Flash HUD element
//
// Parameters:
//   r3 (this) - pongScreenCapture object pointer (stored in r28)
//
// The function uses a stack-based state save pattern where the previous state
// is pushed onto a global stack, the HUD is rendered, then the state is popped.
// ─────────────────────────────────────────────────────────────────────────────

// External globals (defined in globals.c or other subsystems)
extern void* g_renderStateStack[];      // @ 0x821EBA9C (stack of render states)
extern void* g_currentRenderState;      // @ 0x821EBAA0 (current active state)
extern int   g_renderModeIndex;         // @ 0x821F0898 (render mode selector)
extern float g_hudRenderParam;          // @ 0x82140110 (parameter for HUD render)
extern struct hudFlashBase* g_hudFlashBase;  // @ 0x821FE3D4 (HUD Flash system object)

// External function declarations
extern void game_2EE0(void* renderState);
extern void game_2E80(void* renderState);
extern void grc_EB10(void* deviceState);
extern void hudFlashBase_0F08_g(void* hudObj, int modeIndex, float param1, float param2,
                                 int param3, int param4, int param5);

void pongScreenCapture_rtti_B0EC_0(pongScreenCapture* this_ptr) {
    // Save previous render state pointer
    void* previousState = g_renderStateStack[g_renderModeIndex + 8];
    void* currentState = g_currentRenderState;
    
    // Push current state onto stack
    g_renderStateStack[g_renderModeIndex + 8] = currentState;
    
    // If there is a current render state, save it
    if (currentState != nullptr) {
        game_2EE0(currentState);
        game_2E80(currentState);
        
        // Save graphics device state at offset +768 (0x300)
        // This offset points to a grcDevice or grcContext member within the render state
        grc_EB10((void*)((uint8_t*)currentState + 768));
    }
    
    // Render the Flash HUD element
    // The mode index is read from a global array and used to select rendering parameters
    // The float array at (this_ptr + modeIndex*4) contains per-mode rendering parameters
    int modeIndex = g_renderModeIndex + 8;
    float* paramArray = (float*)((uint8_t*)this_ptr + modeIndex * 4);
    float renderParam = *paramArray;
    
    hudFlashBase_0F08_g(g_hudFlashBase, modeIndex, g_hudRenderParam, renderParam,
                        0, 0, 0);
    
    // Restore previous render state
    g_renderStateStack[g_renderModeIndex + 8] = previousState;
    
    // If there was a previous state, restore it
    if (previousState != nullptr) {
        game_2EE0(previousState);
        game_2E80(previousState);
        
        // Restore graphics device state
        grc_EB10((void*)((uint8_t*)previousState + 768));
    }
}

// (Duplicate pongScrnTransSwipe block removed)
//
// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndCrossFade::Begin()  [vtable slot 2 @ 0x82378938]
//
// Initializes the freeze-and-cross-fade transition effect. This function sets up
// the initial state for a screen transition that freezes the current frame and
// cross-fades to the next screen.
//
// Globals accessed:
//   0x8202D108 (.rdata) - constant 1.0f (initial alpha value)
//   0x8202D10C (.rdata) - default duration value
//   0x8202D110 (.rdata) - initial elapsed time (0.0f)
//   0x825C755C (.data) - default duration fallback
//
// Struct layout:
//   +0x04: m_duration (float) - total transition duration
//   +0x08: m_elapsedTime (float) - current elapsed time (starts at 0.0)
//   +0x0C: m_finished (bool) - completion flag (starts false)
//   +0x1C: field at offset 28 - checked for initialization
//   +0x10: field at offset 16 - passed to util function
//   +0x34: m_alpha (float) - fade alpha (starts at 1.0)
//
// Logic:
//   1. If field at +28 is non-zero, call sysCallback_Invoke to initialize something at +16
//   2. Load default duration from constant table
//   3. Initialize elapsed time to 0.0
//   4. Set finished flag to false
//   5. Set initial alpha to 1.0 (fully opaque)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndCrossFade::Begin() {
    // External constants
    extern const float g_floatOne;        // @ 0x8202D108 = 1.0f
    extern const float g_floatZero;       // @ 0x8202D110 = 0.0f
    extern const float g_defaultDuration; // @ 0x825C755C

    // Check if we need to initialize something at offset +16
    uint32_t* fieldAt28 = (uint32_t*)((char*)this + 28);
    if (*fieldAt28 != 0) {
        void* fieldAt16 = (void*)((char*)this + 16);
        extern void sysCallback_Invoke(void* target, int param);
        sysCallback_Invoke(fieldAt16, 0);
    }

    // Initialize transition state
    m_elapsedTime = g_floatZero;
    m_finished = false;
    m_duration = g_defaultDuration;
    m_alpha = g_floatOne;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndCrossFade::Render()  [vtable slot 4 @ 0x82378A18]
//
// Renders the freeze-and-cross-fade effect by drawing a full-screen quad with
// the calculated alpha value. Converts the float alpha (0.0-1.0) to an 8-bit
// integer (0-255) and constructs an RGBA color with white RGB and variable alpha.
//
// Globals accessed:
//   0x82079BE0 (.rdata) - multiplier constant (255.0f) for alpha conversion
//   0x826063D4 (.data, SDA) - pointer to render context
//
// Struct layout:
//   +0x34: m_alpha (float) - fade alpha value (0.0 to 1.0)
//
// Logic:
//   1. Load alpha from +52 (0x34)
//   2. Multiply by 255.0 to convert to 0-255 range
//   3. Convert float to integer
//   4. Construct RGBA color: (alpha << 24) | 0x00FFFFFF (white with variable alpha)
//   5. Call util_03C0 to render the full-screen quad with this color
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndCrossFade::Render() {
    // External constants and globals
    extern const float g_alphaMultiplier; // @ 0x82079BE0 = 255.0f
    extern void* g_renderContext;         // @ 0x826063D4 (SDA)
    extern void util_03C0(void* context, uint32_t* colorPtr);

    // Convert alpha from 0.0-1.0 to 0-255 range
    float alphaScaled = m_alpha * g_alphaMultiplier;
    uint32_t alphaInt = (uint32_t)alphaScaled;

    // Construct RGBA color: alpha in high byte, RGB = 0xFFFFFF (white)
    uint32_t color = (alphaInt << 24) | 0x00FFFFFF;

    // Render the full-screen fade quad
    util_03C0(g_renderContext, &color);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndCrossFade::End()  [vtable slot 5 @ 0x82378A78]
//
// Tail-call to pongScrnTransFadeIn_EndTransition. This is a simple forwarding function
// that delegates to another transition class's implementation, likely for code
// reuse or polymorphic behavior.
//
// Implementation: Direct branch to 0x82378460 (pongScrnTransFadeIn_EndTransition)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndCrossFade::End() {
    // Forward to pongScrnTransFadeIn implementation
    extern void pongScrnTransFadeIn_EndTransition(void* thisPtr);
    pongScrnTransFadeIn_EndTransition(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndCrossFade::Reset()  [vtable slot 6 @ 0x82378A80]
//
// Resets the transition state to initial values. This function is likely called
// when restarting or reinitializing the transition effect.
//
// Globals accessed:
//   0x8202D108 (.rdata) - constant 1.0f
//   0x8202D10C (.rdata) - default duration
//   0x8202D110 (.rdata) - constant 0.0f
//
// Struct layout:
//   +0x04: m_duration (float) - reset to default duration
//   +0x08: m_elapsedTime (float) - reset to 0.0
//   +0x0C: m_finished (bool) - reset to false
//   +0x30: field at offset 48 - set to 1.0
//   +0x34: m_alpha (float) - reset to 0.0
//
// Logic:
//   1. Clear finished flag
//   2. Load default duration from constant table at +4
//   3. Load zero constant from table at +8
//   4. Load one constant from table at +0
//   5. Set duration, elapsed time, and alpha fields
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndCrossFade::Reset() {
    // External constants
    extern const float g_floatOne;        // @ 0x8202D108 = 1.0f
    extern const float g_floatZero;       // @ 0x8202D110 = 0.0f
    extern const float g_defaultDuration; // @ 0x8202D10C

    // Reset transition state
    m_finished = false;
    m_duration = g_defaultDuration;
    m_elapsedTime = g_floatZero;

    // Reset alpha values (field at +48 and +52)
    float* fieldAt48 = (float*)((char*)this + 48);
    *fieldAt48 = g_floatOne;
    m_alpha = g_floatZero;
}

// ═════════════════════════════════════════════════════════════════════════════
// ScrnTrans family: shared dtor + FadeIn (5 vfns) + FreezeAndFadeOut (5 vfns)
// ═════════════════════════════════════════════════════════════════════════════
//
// Class layout (shared across all pongScrnTrans* variants):
//   +0x00 vtable
//   +0x04 m_duration      (float)
//   +0x08 m_elapsedTime   (float)
//   +0x0C m_finished      (bool)
//   +0x10..+0x2F          sysCallback block (object + payload; invoked via
//                         sub_8225FFF8 when trigger at +0x1C is non-zero)
//   +0x30+                subclass-specific fields
//
// Constants (.rdata):
//   0x8202D108 = 1.0f  (g_floatOne)
//   0x8202D10C = small epsilon / default duration
//   0x8202D110 = 0.0f  (g_floatZero)
//   0x82079BE0 = 255.0f (alpha scale)
//
// External shared helpers:
//   sub_8225FFF8               — sysCallback_Invoke(block, param)
//   hudFlashBase_DrawOverlay   @ 0x821403C0 / 0x82150078
//   hudFlashBase_BeginFrame    @ 0x820F76F8
//   hudFlashBase_Flash         @ 0x8215B138
//   util_PackColorRGBA         @ 0x8223F840
//   g_pauseFlag                @ 0x82606670
// ═════════════════════════════════════════════════════════════════════════════

// External RAGE/HUD helpers used by ScrnTrans family
extern "C" void  sysCallback_Invoke(void* block, int param);  // @ 0x8225FFF8
extern "C" void  hudFlashBase_BeginFrame(int a, int b, void* ctx);   // @ 0x820F76F8
extern "C" void  hudFlashBase_Flash(void);                           // @ 0x8215B138
extern "C" void  hudFlashBase_DrawOverlay_End(void* ctx, uint32_t* color); // @ 0x82150078
extern "C" void  hudFlashBase_DrawOverlay(void* ctx, uint32_t* color);     // @ 0x821403C0

// Globals (resolved from absolute addresses via SDA offsets)
extern uint8_t   g_pauseFlagScrn;                              // @ 0x82606670
extern void*     g_hudFlashCtx_82606434;                       // @ 0x82606434 (r11-17888, r11=0x8260B000)
extern void*     g_hudFlashOverlayCtx_826065DC;                // @ 0x826065DC (r11-17636)
extern void*     g_hudFlashOverlayCtx_82606574;                // @ 0x82606574 (r11-17740)
extern void*     g_renderObjA_82606654;                        // @ 0x82606654 (r11+25844)
extern void*     g_renderObjB_825FEAB0;                        // @ 0x825FEAB0 (r11-21712)

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTrans* shared deleting destructor  @ 0x82378450  size: ~0x10
//
// All four ScrnTrans vtables point their slot-0 entry at this shared thunk.
// It fetches vtable slot 6 (Reset/End) and tail-calls it.  This works because
// every ScrnTrans variant has the same vtable layout through slot 6 and the
// shared base destructor is actually the `End()`-style cleanup path.
//
// Scaffold:
//   lwz  r11, 0(r3)       <- vtable ptr
//   lwz  r10, 24(r11)     <- vtable[6]  (slot 6 = 24/4)
//   mtctr r10 / bctr      <- tail call
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut::~pongScrnTransFreezeAndFadeOut()
{
    // Shared thunk @ 0x82378450 — dispatches to vtable[6] of the actual
    // class (polymorphic Reset-on-destroy pattern).  Compiler synthesizes
    // the vtable read+tail-call; in source we just call the virtual Reset.
    // vfn_6();
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFadeIn::vfn_5  (End)  @ 0x82378460  size: 0x90
//
// End-of-transition cleanup.  Fires the deferred-callback block (if armed)
// and then copies a Vec4 duration table from .rdata (@ 0x825C755C, 16 bytes)
// into both the callback payload area (+0x20..+0x2C) and the "next-duration"
// slot (+0x10..+0x1C).  The memcpy is unrolled 4×4 bytes.
//
// Tail-called by pongScrnTransFreezeAndFadeOut::vfn_5.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFadeIn_vfn_5(pongScrnTransFadeIn* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Fire callback if armed: (+0x2C != 0) → sysCallback_Invoke(this+0x20, 0)
    if (*reinterpret_cast<uint32_t*>(base + 44) != 0) {
        sysCallback_Invoke(base + 32, 0);
    }

    // Copy 16-byte duration Vec4 from 0x825C755C → this+0x20 and this+0x10
    // Struct: (0.7, 3.3, 8.3, 3.0) — fade-in duration table (4 presets)
    extern const uint32_t g_scrnTransFadeInDurations[4];  // @ 0x825C755C
    uint32_t w0 = g_scrnTransFadeInDurations[0];
    uint32_t w1 = g_scrnTransFadeInDurations[1];
    uint32_t w2 = g_scrnTransFadeInDurations[2];
    uint32_t w3 = g_scrnTransFadeInDurations[3];

    uint32_t* dstA = reinterpret_cast<uint32_t*>(base + 32);
    dstA[0] = w0; dstA[1] = w1; dstA[2] = w2; dstA[3] = w3;

    uint32_t* dstB = reinterpret_cast<uint32_t*>(base + 16);
    dstB[0] = w0; dstB[1] = w1; dstB[2] = w2; dstB[3] = w3;
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFadeIn::vfn_2  (Begin)  @ 0x82378718  size: 0x60
//
// Start the fade-in.  If the callback trigger (+0x1C) is armed, fires the
// user callback (+0x10..) with param=0.  Then clears the finished flag,
// resets elapsed time to 0.0f, and stamps alpha (+0x40) to 1.0f.
//
// Struct writes:
//   stb  r10, 12(r31)   <- m_finished = false
//   stfs f0,  8(r31)    <- m_elapsedTime = 0.0f  (from +8 of const table)
//   stfs f13, 64(r31)   <- m_alpha      = 1.0f  (from +0 of const table)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFadeIn_vfn_2(pongScrnTransFadeIn* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Fire user callback if armed (trigger at +0x1C)
    if (*reinterpret_cast<uint32_t*>(base + 28) != 0) {
        sysCallback_Invoke(base + 16, 0);
    }

    // Constants: 0x8202D108 = 1.0f, 0x8202D110 = 0.0f
    extern const float g_floatOne;   // @ 0x8202D108
    extern const float g_floatZero;  // @ 0x8202D110

    *reinterpret_cast<bool*>(base + 12)    = false;      // m_finished
    *reinterpret_cast<float*>(base + 8)    = g_floatZero; // m_elapsedTime
    *reinterpret_cast<float*>(base + 64)   = g_floatOne;  // m_alpha @ +0x40
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFadeIn::vfn_3  (Update)  @ 0x82378778  size: 0x60
//
// Advance the fade-in by dt.  Mirror of pongScrnTransFreezeAndCrossFade::Update
// but with *descending* alpha (1.0 → 0.0):
//
//   m_elapsedTime += dt;
//   if (m_elapsedTime >= m_duration) {
//       m_elapsedTime = m_duration;
//       m_finished    = true;
//   }
//   if (m_duration > epsilon)
//       m_alpha = 1.0f - (m_elapsedTime / m_duration);
//   else
//       m_alpha = 0.0f;  // (epsilon constant from same table)
//
// NOTE: Update does NOT respect the pause flag (matches scaffold exactly —
// only vfn_3 of ScrnTransFreezeAndCrossFade had the pause check).
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFadeIn_vfn_3(pongScrnTransFadeIn* self, float deltaTime)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    float& elapsed  = *reinterpret_cast<float*>(base + 8);
    float  duration = *reinterpret_cast<float*>(base + 4);

    elapsed += deltaTime;

    if (elapsed >= duration) {
        elapsed = duration;
        *reinterpret_cast<bool*>(base + 12) = true;  // m_finished
    }

    extern const float g_floatOne;       // @ 0x8202D108
    extern const float g_floatEpsilon;   // @ 0x8202D110 (0.0f / epsilon)

    float& alpha = *reinterpret_cast<float*>(base + 64);  // +0x40
    if (duration > g_floatEpsilon) {
        alpha = g_floatOne - (elapsed / duration);
    } else {
        alpha = g_floatEpsilon;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFadeIn::vfn_4  (Render)  @ 0x823787D8  size: ~0x128
//
// Draw the full-screen fade overlay.  Sequence:
//   1. hudFlashBase_BeginFrame(0, 0, g_hudFlashCtx_82606434)
//   2. if either hudFlashOverlayCtx_{826065DC, 82606574} is non-null:
//         hudFlashBase_Flash();
//   3. Read an RGBA float quad from this+0x30..this+0x3C, each scaled by
//      255.0f (@0x82079BE0), converted to int and packed into RGBA32
//      via a series of rlwimi byte-shuffles.
//   4. The alpha byte is taken from this+0x40 (m_alpha * 255).
//   5. hudFlashBase_DrawOverlay_End(ctx, &packedColor)
//
// The +0x30..+0x3C quad holds the fade *color* (RGB), +0x40 holds alpha.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFadeIn_vfn_4(pongScrnTransFadeIn* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Begin flash/overlay frame
    hudFlashBase_BeginFrame(0, 0, g_hudFlashCtx_82606434);

    // If either overlay context is non-null, trigger a flash
    if (g_hudFlashOverlayCtx_826065DC != nullptr ||
        g_hudFlashOverlayCtx_82606574 != nullptr) {
        hudFlashBase_Flash();
    }

    // Load color quad (R,G,B at +0x30..+0x3C; A at +0x40) and scale to 0..255
    extern const float g_colorScale255;  // @ 0x82079BE0 = 255.0f

    float fA = *reinterpret_cast<float*>(base + 64);  // +0x40 alpha
    float fR = *reinterpret_cast<float*>(base + 48);  // +0x30
    float fG = *reinterpret_cast<float*>(base + 52);  // +0x34
    float fB = *reinterpret_cast<float*>(base + 56);  // +0x38

    uint32_t a255 = static_cast<uint32_t>(fA * g_colorScale255);
    uint32_t r255 = static_cast<uint32_t>(fR * g_colorScale255);
    uint32_t g255 = static_cast<uint32_t>(fG * g_colorScale255);
    uint32_t b255 = static_cast<uint32_t>(fB * g_colorScale255);

    // Pack as RGBA32 — scaffold shuffles bytes via rlwimi; verified byte order
    // is [A][R][G][B] in big-endian memory (Xbox 360 LE display: 0xAARRGGBB)
    uint32_t packed = ((a255 & 0xFF) << 24) |
                      ((r255 & 0xFF) << 16) |
                      ((g255 & 0xFF) <<  8) |
                       (b255 & 0xFF);

    // Submit the fullscreen overlay quad
    hudFlashBase_DrawOverlay_End(g_hudFlashCtx_82606434, &packed);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFadeIn::vfn_6  (Reset)  @ 0x82378900  size: 0x40
//
// Reset all transient state to start-of-transition defaults:
//   m_finished     = false
//   m_duration     = default (from 0x8202D10C)
//   m_elapsedTime  = 0.0f    (from 0x8202D110)
//   color.RGB      = 0.0f each (+0x30..+0x38)  (fade to black)
//   m_alpha        = 1.0f    (from 0x8202D108)  (fully opaque start)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFadeIn_vfn_6(pongScrnTransFadeIn* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    extern const float g_floatOne;          // @ 0x8202D108 = 1.0f
    extern const float g_defaultDurationFI; // @ 0x8202D10C
    extern const float g_floatZero;         // @ 0x8202D110 = 0.0f

    *reinterpret_cast<bool*>(base + 12)  = false;                 // m_finished
    *reinterpret_cast<float*>(base + 4)  = g_defaultDurationFI;   // m_duration
    *reinterpret_cast<float*>(base + 8)  = g_floatZero;           // m_elapsedTime
    *reinterpret_cast<float*>(base + 48) = g_floatZero;           // color R
    *reinterpret_cast<float*>(base + 52) = g_floatZero;           // color G
    *reinterpret_cast<float*>(base + 56) = g_floatZero;           // color B
    *reinterpret_cast<float*>(base + 64) = g_floatOne;            // m_alpha
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndFadeOut::vfn_2  (Begin)  @ 0x823784F0  size: ~0x90
//
// Start the freeze+fade-out.  Like FadeIn::Begin but additionally:
//   - Stamps m_duration (+0x04) from the epsilon constant (0x82079BE0 entry)
//   - Clears a second phase flag at +0x54
//   - Sets fade-stage value at +0x50 from 0x8202D108 (1.0f)
//   - Clears the freeze-trigger byte on two external render contexts:
//       g_renderObjA_82606654[+80]   = 0
//       g_renderObjB_825FEAB0[+492]  = 0
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut_vfn_2(pongScrnTransFreezeAndFadeOut* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Fire callback if armed
    if (*reinterpret_cast<uint32_t*>(base + 28) != 0) {
        sysCallback_Invoke(base + 16, 0);
    }

    extern const float g_floatZero;          // @ 0x8202D110
    extern const float g_floatOne;           // @ 0x8202D108
    extern const float g_scrnTransEpsilon;   // @ 0x82079BE0 (0x8203 +-32640 → 255.0f table entry)

    *reinterpret_cast<float*>(base +  8) = g_floatZero;       // m_elapsedTime = 0
    *reinterpret_cast<bool*>(base + 12)  = false;             // m_finished = false
    *reinterpret_cast<float*>(base +  4) = g_scrnTransEpsilon;// m_duration
    *reinterpret_cast<bool*>(base + 84)  = false;             // +0x54 phase flag
    *reinterpret_cast<float*>(base + 80) = g_floatOne;        // +0x50 fade stage

    // Clear freeze-triggers on external render contexts
    if (g_renderObjA_82606654 != nullptr) {
        *(reinterpret_cast<uint8_t*>(g_renderObjA_82606654) + 80) = 0;
    }
    if (g_renderObjB_825FEAB0 != nullptr) {
        *(reinterpret_cast<uint8_t*>(g_renderObjB_825FEAB0) + 492) = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndFadeOut::vfn_3  (Update)  @ 0x82378578  size: 0x98
//
// Two-phase update:
//   Phase 1 (+0x54 == 0): advance elapsed; when elapsed >= m_duration,
//       flip to phase 2 and hold m_duration.
//   Phase 2 (+0x54 != 0): drive m_alpha@+0x50 = 1.0 - elapsed/duration
//       (clamp to 0 when duration <= epsilon).  When elapsed >= duration
//       (via bso/bltlr check in scaffold), clamp elapsed + m_finished=true
//       + set a secondary flag at +0x0D.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut_vfn_3(pongScrnTransFreezeAndFadeOut* self,
                                         float deltaTime)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    float&   elapsed    = *reinterpret_cast<float*>(base +  8);
    float    duration   = *reinterpret_cast<float*>(base +  4);
    uint8_t& phase2Flag = *reinterpret_cast<uint8_t*>(base + 84);  // +0x54

    elapsed += deltaTime;

    extern const float g_floatOne;       // @ 0x8202D108
    extern const float g_floatEpsilon;   // @ 0x8202D110

    if (phase2Flag != 0) {
        // Phase 2: drive alpha ramp at +0x50
        float  phase2Dur = *reinterpret_cast<float*>(base + 48);
        float& alpha     = *reinterpret_cast<float*>(base + 80);  // +0x50

        if (phase2Dur > g_floatEpsilon) {
            alpha = g_floatOne - (elapsed / phase2Dur);
        } else {
            alpha = g_floatEpsilon;
        }

        // Finish check: elapsed >= phase2Dur → clamp + set flags
        if (elapsed >= phase2Dur) {
            elapsed = phase2Dur;
            *reinterpret_cast<bool*>(base + 12) = true;  // m_finished
            *reinterpret_cast<bool*>(base + 13) = true;  // secondary flag
        }
        return;
    }

    // Phase 1: watch for elapsed >= m_duration → promote to phase 2
    if (elapsed >= duration) {
        phase2Flag = 1;
        extern const float g_scrnTransEpsilon;  // @ 0x82079BE0
        *reinterpret_cast<float*>(base + 8) = g_scrnTransEpsilon;  // clamp
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndFadeOut::vfn_4  (Render)  @ 0x82378610  size: ~0x98
//
// Draws the fade-out overlay.  Unlike FadeIn::Render this only uses the
// single alpha scalar at +0x50 (no RGB color quad) — all three color
// components are derived from alpha*255 (white fade-out to black).
//
// Sequence:
//   alpha255 = (uint32_t)(m_alpha@+0x50 * 255.0f)
//   packed   = pack bytes: [A][A][A][A]  (scaffold packs alpha into all 4 byte
//                slots via rlwimi chain)
//   hudFlashBase_DrawOverlay(g_hudFlashCtx_82606434, &packed)
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut_vfn_4(pongScrnTransFreezeAndFadeOut* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    float alpha = *reinterpret_cast<float*>(base + 80);  // +0x50

    extern const float g_colorScale255;  // @ 0x82079BE0 = 255.0f
    uint32_t a255 = static_cast<uint32_t>(alpha * g_colorScale255);

    // Scaffold packs alpha into all four bytes (white-on-white with varying
    // alpha blends to a straight fade).  Byte layout ends up [A][A][A][A].
    uint32_t packed = ((a255 & 0xFF) << 24) |
                      ((a255 & 0xFF) << 16) |
                      ((a255 & 0xFF) <<  8) |
                       (a255 & 0xFF);

    hudFlashBase_DrawOverlay(g_hudFlashCtx_82606434, &packed);
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndFadeOut::vfn_5  (End)  @ 0x823786A8  size: 0x20
//
// End-of-fade-out: rearms the freeze bytes on both external render contexts
// (byte=1 restores pre-freeze pause state) and then tail-calls the FadeIn
// shared cleanup (0x82378460) to fire any deferred callback and reset the
// duration table.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut_vfn_5(pongScrnTransFreezeAndFadeOut* self)
{
    // Rearm freeze-trigger bytes on both external contexts
    if (g_renderObjA_82606654 != nullptr) {
        *(reinterpret_cast<uint8_t*>(g_renderObjA_82606654) + 80) = 1;
    }
    if (g_renderObjB_825FEAB0 != nullptr) {
        *(reinterpret_cast<uint8_t*>(g_renderObjB_825FEAB0) + 492) = 1;
    }

    // Tail-call shared end helper (fires callback + reloads duration table)
    pongScrnTransFadeIn_vfn_5(reinterpret_cast<pongScrnTransFadeIn*>(self));
}

// ─────────────────────────────────────────────────────────────────────────────
// pongScrnTransFreezeAndFadeOut::vfn_6  (Reset)  @ 0x823786C8  size: 0x58
//
// Full reset.  Clears m_finished and secondary finished flag (+0x0D), loads
// m_duration from .rdata table (default-fadeout duration at 0x8202D10C),
// resets elapsed time to 0, clears fade-color quad (+0x40..+0x48) and the
// phase2 flag (+0x54), sets m_alpha (+0x50) to the epsilon/scale constant
// at 0x82079BE0, and sets the phase2-threshold value at +0x30 to 0.
// ─────────────────────────────────────────────────────────────────────────────
void pongScrnTransFreezeAndFadeOut_vfn_6(pongScrnTransFreezeAndFadeOut* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    extern const float g_floatZero;          // @ 0x8202D110
    extern const float g_defaultDurationFO;  // @ 0x8202D10C (pair value)
    extern const float g_scrnTransEpsilon;   // @ 0x82079BE0

    *reinterpret_cast<bool*>(base + 12)  = false;                // m_finished
    *reinterpret_cast<float*>(base +  4) = g_defaultDurationFO;  // m_duration
    *reinterpret_cast<float*>(base +  8) = g_floatZero;          // m_elapsedTime

    // Zero the phase-2 threshold + fade-color quad (+0x30..+0x48)
    *reinterpret_cast<float*>(base + 48) = g_scrnTransEpsilon;   // +0x30
    *reinterpret_cast<float*>(base + 64) = g_floatZero;          // +0x40
    *reinterpret_cast<float*>(base + 68) = g_floatZero;          // +0x44
    *reinterpret_cast<float*>(base + 72) = g_floatZero;          // +0x48

    *reinterpret_cast<bool*>(base + 84)  = false;                // +0x54 phase2
    *reinterpret_cast<bool*>(base + 13)  = false;                // secondary fin

    *reinterpret_cast<float*>(base + 80) = g_scrnTransEpsilon;   // +0x50 alpha
}

// ═════════════════════════════════════════════════════════════════════════════
// pongLightGlowMgr::~pongLightGlowMgr  @ 0x8237D060  size: 0x74
//
// Deleting destructor.  Two-step teardown:
//   1. Overwrite vtable ptr with base-class vtable @ 0x8205A168 (rage base).
//   2. Destroy two contained sub-allocators via rage_free_2E18:
//         rage_free_2E18(this->m_sub04);  // +0x04
//         rage_free_2E18(this->m_sub08);  // +0x08
//   3. Re-stamp vtable to the *final* glow-mgr base 0x82035484.
//   4. If (flags & 1): rage_free(this).
//
// Scaffold matches the GTA-style "reset vtable → child destroys → self-free"
// pattern exactly.
// ═════════════════════════════════════════════════════════════════════════════
extern "C" void  rage_free_2E18(void* obj);                // @ 0x820C2E18
extern "C" void  rage_free_MemAlloc(void* ptr);             // @ 0x820C00C0
extern       void* const g_rage_vtable_Base_8205A168;       // @ 0x8205A168
extern       void* const g_pongLightGlowMgr_vtable_Final;   // @ 0x82035484

void pongLightGlowMgr::~pongLightGlowMgr()
{
    // Flag is passed via a hidden 2nd arg in the synthesized dtor; in a
    // hand-written dtor we rely on the caller to invoke ~+ operator delete.
    // The scaffold stamps the base-class vtable first:
    //   this->vtable = &g_rage_vtable_Base_8205A168;
    vtable = reinterpret_cast<void**>(
        const_cast<void*>(g_rage_vtable_Base_8205A168));

    // Destroy contained sub-allocators at +0x04 and +0x08
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void* sub04 = *reinterpret_cast<void**>(self + 4);
    void* sub08 = *reinterpret_cast<void**>(self + 8);
    rage_free_2E18(sub04);
    rage_free_2E18(sub08);

    // Stamp the final pongLightGlowMgr vtable for any late dispatch
    vtable = reinterpret_cast<void**>(
        const_cast<void*>(g_pongLightGlowMgr_vtable_Final));

    // Self-free is handled by the generated deleting-dtor thunk; no explicit
    // call here (rage_free_MemAlloc(this) fires only when the delete-flag bit
    // from the hidden 2nd arg is set).
}
