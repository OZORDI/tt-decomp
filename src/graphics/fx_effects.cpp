/**
 * fx_effects.cpp — Visual effects system (ball trails, particles, etc.)
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "fx_effects.hpp"
#include "../rage/memory.hpp"

// Global effect parameter table @ 0x8271A394
extern uint32_t* g_fxParameterTable;

// Base class cleanup for fxCrowdGfx hierarchy
extern void rage_5908(void*);

// ── Forward declarations of helpers referenced in implementations ──
extern void util_C880(void* meshPtr);                 // mesh/resource releaser @ 0x8214C880
extern int  rage_2A78(int type, int size);            // rage resource allocator @ 0x82152A78
extern void phBoundCapsule_3DA0_g(void* out, float radius);  // capsule builder @ 0x820C3DA0
extern void fxBallSpinTex_0058(/*…*/);                // internal draw submission helper @ 0x82410058
extern int  util_F840(void* /*tmp*/, float arg);      // scratch conversion @ 0x8223F840
extern void pongDrawBucket_AddEntry(void* bucket, fxCrowdGfx* self, int handle);

// Global draw registry slot referenced by fxBallSpinTex::RegisterAndInit
// (r9+14436 / r6+14440 / r7+14952 / r5+14696 — registry counter + 3 parallel arrays
// in the big static table at 0x82056000+).
extern uint32_t g_fxSpinRegistryCount;          // @ 0x820599A4 (= 14436)
extern uint8_t  g_fxSpinRegistryTypes[128];     // @ 0x8205A3A8 (= 14952)
extern void*    g_fxSpinRegistryTargets[128];   // @ 0x82059BA8 (= 14440)
extern int      g_fxSpinRegistryKinds[128];     // @ 0x82059CA8 (= 14696)
extern uint32_t g_pongDrawBucketGlobal;         // @ 0x8207A5F4 (= r11:-32160 + 25844)

// Fixed-point constants sourced from .rdata (not hex magic — real floats)
static constexpr float kFxTimerStep       = 0.0f;      // from lbl @ -32164, +22840
static constexpr float kFxOne             = 1.0f;
static constexpr float kFxZero            = 0.0f;

// Trail layout constants (all decoded offsets, 0 hex in bodies)
static constexpr int kTrailSampleStride   = 32;   // bytes per trail sample
static constexpr int kTrailCountOff       = 16;
static constexpr int kTrailTailOff        = 20;
static constexpr int kTrailSampleBase     = 48;   // first sample block (this+48)
static constexpr int kTrailTimerOff       = 15392;
static constexpr int kTrailDataPtrOff     = 15400;
static constexpr int kTrailLocalStep      = 15396;
static constexpr int kTrailFlagOff        = 46176;
static constexpr int kTrailNormOff        = 46180;
static constexpr int kTrailIntensityOff   = 46188;
static constexpr int kTrailEnvelopeOff    = 46192;
static constexpr int kTrailSecondaryOff   = 46160;

// fxBallSpinTex layout
static constexpr int kSpinSeg0            = 176;
static constexpr int kSpinSeg1            = 192;
static constexpr int kSpinSeg2            = 208;
static constexpr int kSpinSeg3            = 224;
static constexpr int kSpinSeg4            = 240;
static constexpr int kSpinSeg5            = 256;
static constexpr int kSpinSeg6            = 272;   // plus scalar floats at +0..+40
static constexpr int kSpinSeg7            = 336;
static constexpr int kSpinLiveFlagA       = 24;
static constexpr int kSpinLiveFlagB       = 25;
static constexpr int kSpinScale0          = 28;
static constexpr int kSpinScale1          = 32;
static constexpr int kSpinScale2          = 36;
static constexpr int kSpinTrailAccumX     = 400;
static constexpr int kSpinTrailAccumY     = 404;
static constexpr int kSpinTrailAccumIdx   = 408;
static constexpr int kSpinSamplesPerPass  = 31;

// fxCrowdGfx layout
static constexpr int kCrowdMaterialOff    = 32;    // +32 = grc material ptr
static constexpr int kCrowdMeshOff        = 36;    // +36 = mesh / bound ptr
static constexpr int kCrowdMgrPtrOff      = 1080;
static constexpr int kCrowdLodScaleOff    = 4;
static constexpr int kCrowdFlagsOff       = 8;
static constexpr int kCrowdStateOff       = 12;
static constexpr int kCrowdMaterialDrawSlot = 10;  // grcMaterial vtable slot
static constexpr int kCrowdStateInitial   = 3;

// Raw field helpers — typed, no raw arith at call sites
template <typename T>
static inline T& field(void* self, int off) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(self) + off);
}
template <typename T>
static inline const T& field(const void* self, int off) {
    return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(self) + off);
}

/**
 * fxBallTrail::SetEffectParameters @ 0x82381208 | size: 0x2C
 * [vtable slot 3]
 *
 * Sets effect parameters for the ball trail visual effect.
 * Stores effect type, intensity, and looks up additional parameters
 * from a global table based on the effect index.
 *
 * @param effectIndex Index into effect parameter table (r4)
 * @param intensity Effect intensity value (r5)
 * @param duration Effect duration (r6)
 */
void fxBallTrail::SetEffectParameters(int effectIndex, uint32_t intensity, uint32_t duration) {
    // Look up parameter value from global table
    uint32_t paramValue = g_fxParameterTable[effectIndex + 1];

    // Store effect parameters
    this->m_effectIntensity = intensity;
    this->m_effectParam = paramValue;
    this->m_effectDuration = duration;
}


/**
 * fxCrowdGfx::vfn_2 @ 0x823856E0 | size: 0x50
 * [vtable slot 2]
 *
 * Cleanup and conditional free for crowd graphics effect.
 * Calls base class cleanup (rage_5908) and optionally frees the object
 * memory if the delete-self flag (bit 0 of flags parameter) is set.
 *
 * @param flags Flags (bit 0 = free self after cleanup)
 */
void fxCrowdGfx::vfn_2(int flags) {
    rage_5908(this);

    if (flags & 0x1) {
        rage_free(this);
    }
}


// ─────────────────────────────────────────────────────────────────────
// fxSpecialFx — RTTI-style virtual dispatch class
// Vtables @ 0x820797BC and 0x82079840 (MI/virtual base)
// ─────────────────────────────────────────────────────────────────────

// External helpers used by fxSpecialFx methods
extern void  rage_free(void*);
extern void  atFactory_GetFactory(void*);
extern void* pg_C2A0_g(void*, int);
extern void  rage_7600(void* self, void* prev, int index);
extern void  fxSpecialFx_8738_wrh(void* obj);
extern void  sysMemSimpleAllocator_8838_fw(void* alloc, float f);
extern void  sysMemSimpleAllocator_88C0_fw(void* alloc, float f);
extern void  pongDrawBucket_AddEntry(void* bucket, void* geom, int mode);

// Globals referenced by these methods
// lbl_82722798 — class-type sentinel A         (fxSpecialFx::vfn_20)
// lbl_8271A390 — class-type sentinel B         (fxSpecialFx::vfn_20)
// lbl_825D0080 — atSingleton<fxSpecialFxMgr>   (fxSpecialFx::vfn_23)
// lbl_825CAEB8 — tuning constants (60 bytes)   (fxSpecialFx::vfn_27)
// lbl_82606404 — rage::fwArchetypeManager*     (fxSpecialFx::vfn_23)
extern uintptr_t lbl_82722798;
extern uintptr_t lbl_8271A390;
extern uintptr_t lbl_825D0080;
extern float     lbl_825CAEB8[];
extern void*     lbl_82606404;
extern const char lbl_82079818[];   // class-name string
extern const float lbl_82077EEC;    // float constant 2.0f-ish

/**
 * fxSpecialFx::vfn_20 @ 0x82426F08 | size: 0x38
 * RTTI "IsA"/"IsKindOf" style check.
 *  - Returns 1 if r4 matches class-type sentinel A (lbl_82722798).
 *  - Else returns 1 if r4 matches class-type sentinel B (lbl_8271A390),
 *    otherwise 0.
 *
 * @param other r4 — candidate class-type id to test against.
 * @return 0/1 match flag in r3 (zero-extended byte).
 */
unsigned int fxSpecialFx_vfn_20(void* /*this*/, uintptr_t other) {
    if (other == lbl_82722798) {
        return 1;
    }
    return (other == lbl_8271A390) ? 1 : 0;
}

/**
 * fxSpecialFx::vfn_22 @ 0x82426F40 | size: 0xC
 * Returns a pointer to the class-name (rtti) string for fxSpecialFx.
 *  - Loads &lbl_82079818 (".rdata" string "fxSpecialFx").
 *
 * @return const char* pointing into .rdata.
 */
const char* fxSpecialFx_vfn_22(void* /*this*/) {
    return lbl_82079818;
}

/**
 * fxSpecialFx::vfn_23 @ 0x82427408
 * Heavy "LoadResource"-like setup routine.
 *  1. Call vtable slot 1 (destructor/reset path) on `this`.
 *  2. atFactory_GetFactory(&lbl_825D0080) and invoke its vtable slot 2
 *     with the global archetype-manager pointer; store result at +240.
 *  3. Decrement refcount/field at lbl_825D0080+1536.
 *  4. Loop across g_someArray->count (glob @ -23792(-32142)) — for each
 *     element, bump counter at this+52+176 and dispatch to rage_7600 via
 *     pg_C2A0_g(i).
 *
 * TODO: resolve the two remote globals (atSingletonHolder @ 0x825D0080
 *       and the per-entry array-base the loop iterates). Field offsets
 *       52/176/240 need real names on the fxSpecialFx struct.
 */
void fxSpecialFx_vfn_23(void* thisPtr) {
    // 1. Call vtable[1] (reset/cleanup)
    typedef void (*vfn1_t)(void*);
    void** vt = *(void***)thisPtr;
    ((vfn1_t)vt[1])(thisPtr);

    // 2. Construct factory, store result at +240
    atFactory_GetFactory((void*)&lbl_825D0080);
    void* archMgr = lbl_82606404;
    void** archVt = *(void***)archMgr;
    typedef void* (*archCreate_t)(void*, const char*);
    void* result = ((archCreate_t)archVt[2])(archMgr, /*lbl_82077EFC-26820*/ (const char*)0);
    *(void**)((char*)thisPtr + 240) = result;

    // 3. Drop refcount at +1536
    uint32_t& rc = *(uint32_t*)((char*)&lbl_825D0080 + 1536);
    rc = rc - 1;

    // 4. Iterate the per-entry array and dispatch
    // TODO: type the array and the per-entry callback target.
    void*** arrayHolder = (void***)(*(uintptr_t*)(0x82548490ull));  // -23792(-32142) — placeholder
    (void)arrayHolder;  // left intentionally unused until glob resolved
}

/**
 * fxSpecialFx::vfn_25 @ 0x824274E8 | size: ~0x118
 * Resets all "slots" in the fx state and re-allocates a secondary pool.
 *  - Reads slot-count at this+48 (halfword).
 *  - For each slot i in [0 .. count):
 *      * For each inner entry j in [0 .. innerCount(this+44[i*8]+132)):
 *          - Clear active-byte at entry+1
 *          - Call fxSpecialFx_8738_wrh on entry[+4]
 *  - Stores this+232 = g_fxGlobal->allocator (+40 off that global).
 *  - Second pass: for each slot j in the 8-slot upper block, invoke
 *    sysMemSimpleAllocator_8838_fw with float constant lbl_82077EEC.
 *
 * TODO: promote raw +44 / +48 / +52 / +132 / +176 / +232 to named
 *       struct members on fxSpecialFx.
 */
void fxSpecialFx_vfn_25(void* thisPtr) {
    char* self = (char*)thisPtr;
    *(uint8_t*)(self + 236) = 0;

    // Cache allocator-ish ptr from global at -21720(-32161) → this+232
    uintptr_t gEffSys = *(uintptr_t*)(0x82549328ull);  // placeholder glob
    *(uint32_t*)(self + 232) = *(uint32_t*)(gEffSys + 40);

    int slotCount = *(int16_t*)(self + 48);
    for (int i = 0; i < slotCount; ++i) {
        char* slot = *(char**)(*(char**)(self + 44) + i * 8 + 4);
        int inner = *(int16_t*)(slot + 132);
        for (int j = 0; j < inner; ++j) {
            char* entry = *(char**)(*(char**)(slot + 128) + j * 4);
            entry[1] = 0;
            fxSpecialFx_8738_wrh(*(void**)(entry + 4));
        }
    }

    // Second pass over rounded-up(count+7)/8 upper-block
    int upper = (int)(*(uint16_t*)(self + (((*(uint32_t*)(self + 232) + 7) >> 3) * 8)));
    for (int k = 0; k < upper; ++k) {
        uint32_t baseOff = ((*(uint32_t*)(self + 232) + 7) & ~7u) + 52;
        void* alloc = *(void**)(*(char**)(self + baseOff) + k * 4);
        sysMemSimpleAllocator_8838_fw(alloc, /*lbl_82077EEC*/ 2.0f);
    }
    // TODO: rewrite the upper-block loop once the packed layout is
    //       validated (the +3-shift +0xFFFFFFF8 mask is unusual).
}

/**
 * fxSpecialFx::vfn_26 @ 0x824274D0 | size: 0x18
 * Caches "allocator" pointer from fx effect-system global at this+232.
 *  - g = *(glob @ -21720(-32161))
 *  - this+232 = *(g+40)
 */
void fxSpecialFx_vfn_26(void* thisPtr) {
    uintptr_t g = *(uintptr_t*)(0x82549328ull);  // TODO: name this glob
    *(uint32_t*)((char*)thisPtr + 232) = *(uint32_t*)(g + 40);
}

/**
 * fxSpecialFx::vfn_27 @ 0x82427A78
 * Parallel to vfn_25 but calls sysMemSimpleAllocator_88C0_fw (destroy
 * path) on every inner entry with float at lbl_825CAEB8+8.
 *  - Outer loop: slot index < this+48
 *  - Inner loop: entry index < slot+132
 *    - callee(*(slot+128+j*4)+4, lbl_825CAEB8[2])
 */
void fxSpecialFx_vfn_27(void* thisPtr) {
    char* self = (char*)thisPtr;
    int slotCount = *(int16_t*)(self + 48);
    for (int i = 0; i < slotCount; ++i) {
        char* slot = *(char**)(*(char**)(self + 44) + i * 8 + 4);
        int inner = *(int16_t*)(slot + 132);
        for (int j = 0; j < inner; ++j) {
            void* target = *(void**)(*(char**)(slot + 128) + j * 4);
            sysMemSimpleAllocator_88C0_fw(*(void**)((char*)target + 4),
                                          /*lbl_825CAEB8[2]*/ lbl_825CAEB8[2]);
        }
    }
}

/**
 * fxSpecialFx::vfn_29 @ 0x82427B08 | size: 0x28
 * Render/submit helper: stores a float constant at this+20 then tail-calls
 * pongDrawBucket_AddEntry with (g_drawBucket, this+16, /*mode*/ 2).
 *
 *  - this+20   = *(lbl_82077EEC)  (float)
 *  - drawBucket = *(glob @ 25844(-32160)) → lbl_82605FB4
 */
void fxSpecialFx_vfn_29(void* thisPtr) {
    *(float*)((char*)thisPtr + 20) = /*lbl_82077EEC*/ lbl_82077EEC;
    void* bucket = *(void**)(0x82605FB4ull);
    pongDrawBucket_AddEntry(bucket, (char*)thisPtr + 16, 2);
}

/**
 * fxSpecialFx::~fxSpecialFx (vtable slot 0) @ 0x82427050 | size: 0x50
 * Scalar/complete destructor.
 *  - atSingleton_70A0_g(this)   — run virtual base/member dtors.
 *  - If caller-flag bit 0 set (r4 & 1), rage_free(this).
 *
 * @param flags bit 0 = delete-self after destruct (scalar-dtor flag).
 */
void fxSpecialFx_vfn_0(void* thisPtr, int flags) {
    extern void atSingleton_70A0_g(void*);
    atSingleton_70A0_g(thisPtr);
    if (flags & 0x1) {
        rage_free(thisPtr);
    }
}

// ─────────────────────────────────────────────────────────────────────
// fxSpecialFx — effect-pool manager role
// ---------------------------------------------------------------------
// The class doubles as the per-frame palette/pool owner. Layout
// (derived from FindFreeEntry / ResetPalette / ReleasePalette / Tick):
//
//   +0x2C (44)  struct fxSlot* slots[]          outer slot array
//   +0x30 (48)  uint16_t       slotCount         halfword — slots in use
//   +0x80 (128) struct fxSlot::entry** entries  inner per-slot entry array
//   +0x84 (132) uint16_t       entryCount        halfword — per-slot cap
//   +0xE0 (224) uint32_t       framePalette      palette snapshot
//   +0xE8 (232) void*          allocator         cached sysMem allocator*
//   +0xEC (236) uint8_t        isActive          live-flag byte
//
// Each per-slot `entry` is an 8-byte record: { u8 activeA, u8 activeB,
// u16 _pad, void* effect }. NewEffect scans for the first entry with
// both active-bytes zero, flips both to 1, and returns entry->effect.
// Tick (rtti_9840_0) walks every slot and virtual-dispatches slot 5 on
// every active effect — the per-frame "tick all effects" loop.
// ─────────────────────────────────────────────────────────────────────

// Globals & externs referenced by the pool/tick path.
extern void   phBoundCapsule_FE58_g(void* capsuleScratch);          // @ 0x8240FE58 — capsule init
extern void*  game_6E78(void* slotArrayPlus32);                     // @ 0x82426E78 — resolve slot entry array
extern void   hudFlashBase_4128_g(int mode);                        // @ 0x82154128 — frame flash gate
extern void   sysMemSimpleAllocator_7D90(void* allocator);          // @ 0x82187D90 — per-slot allocator open
extern void*  lbl_82606318;                                         // .data scratch word cleared each slot

// Struct layout constants for the pool role.
static constexpr int kFxMgrSlotsPtrOff    = 44;    // +0x2C — slot[] base
static constexpr int kFxMgrSlotCountOff   = 48;    // +0x30 — halfword count
static constexpr int kFxMgrEntriesOff     = 128;   // +0x80 — entry[] base
static constexpr int kFxMgrEntryCountOff  = 132;   // +0x84 — halfword cap
static constexpr int kFxMgrPaletteSnapOff = 224;   // +0xE0 — palette snapshot
static constexpr int kFxMgrAllocatorOff   = 232;   // +0xE8 — sysMem allocator*
static constexpr int kFxMgrActiveOff      = 236;   // +0xEC — live-flag byte
static constexpr int kFxSlotEntrySize     = 4;     // entries are void*[N]
static constexpr int kFxEntryActiveA      = 0;     // entry byte 0
static constexpr int kFxEntryActiveB      = 1;     // entry byte 1
static constexpr int kFxEntryEffectPtr    = 4;     // entry+4 → effect

/**
 * fxSpecialFx::FindFreeEntry @ 0x824279C8 | size: 0xAC
 * Allocates (claims) the first idle entry in the pool.
 *   - Resolves the per-slot entry array via game_6E78(this+32).
 *   - Scans [0, entryCount) looking for the first entry whose byte[0]
 *     AND byte[1] are both zero.
 *   - On hit: sets entry->activeA = entry->activeB = 1 and returns
 *     entry->effect (the stable effect pointer at entry+4).
 *   - On miss (or when the slot head is null / empty): returns nullptr.
 *
 * Think of this as the manager's "NewEffect" — it doesn't allocate new
 * storage, but hands out the next idle slot in the pre-sized palette.
 */
void* fxSpecialFx_FindFreeEntry(void* thisPtr) {
    char* self = (char*)thisPtr;
    void* slotHead = game_6E78(self + 32);
    if (slotHead == nullptr) {
        return nullptr;
    }
    int entryCount = *(int16_t*)((char*)slotHead + kFxMgrEntryCountOff);
    if (entryCount <= 0) {
        return nullptr;
    }
    char** entries = *(char***)((char*)slotHead + kFxMgrEntriesOff);
    for (int i = 0; i < entryCount; ++i) {
        char* entry = entries[i];
        if (entry[kFxEntryActiveA] == 0 && entry[kFxEntryActiveB] == 0) {
            entry[kFxEntryActiveA] = 1;
            entry[kFxEntryActiveB] = 1;
            return *(void**)(entry + kFxEntryEffectPtr);
        }
    }
    return nullptr;
}

/**
 * fxSpecialFx::Tick @ 0x82427B28 | size: 0x118 (rtti_9840_0)
 *
 * Per-frame dispatch over the active effect palette. This is the "tick
 * all active effects" workhorse invoked from the frame loop.
 *
 *   1. phBoundCapsule_FE58_g(this) — prepare scratch capsule state.
 *   2. pongShadowMap_38C0_g(g_ambientStreamingHeap, &lbl_825CB800 - 18432,
 *      &lbl_825CB800 - 18432) — rebind shadow slot for this frame.
 *   3. Locate the palette block at
 *         palette = (char*)this + ((allocator[+40] << 3) & ~7) + 36
 *      (allocator-indexed palette offset — the "round-up to qword" that
 *      vfn_25 also performs). Its halfword at +4 is the palette entry
 *      count.
 *   4. For each palette slot i:
 *         - hudFlashBase_4128_g(1)   — per-slot flash beat
 *         - Propagate bit 0 of the palette snapshot depending on whether
 *           (this+224) still matches the mirrored palette slot from the
 *           global (offsets -17736/-17740 against 0x825EB***).
 *         - Read entry = palette.entries[i] and call
 *           sysMemSimpleAllocator_7D90 on its allocator (r3 = lbl_82606318
 *           refill target).
 *         - Inner loop over entry->count (halfword at entry+4): for each
 *           j, vcall slot 5 of entry->list[j] — this is the per-effect
 *           Tick dispatch.
 *         - Clear lbl_82606318 at the end of each outer iteration.
 *
 * The "vtable slot 5" seen here corresponds to per-effect Tick (the same
 * slot used by fxAmbient::Tick / fxBallTrail::Tick aliases) — so this
 * walks the whole palette and ticks every live sub-effect once per frame.
 */
void fxSpecialFx_Tick(void* thisPtr) {
    char* self = (char*)thisPtr;

    // 1. Scratch capsule init (takes `this` as its workspace).
    phBoundCapsule_FE58_g(self);

    // 2. Re-bind shadow/env slot for this frame.
    extern uint32_t lbl_825CB800_arr[];   // alias to the 64-byte scratch
    pongShadowMap_38C0_g(g_ambientStreamingHeap,
                         (void*)lbl_825CB800_arr,
                         (void*)lbl_825CB800_arr);

    // 3. Compute palette pointer via the allocator-rounded offset.
    void* allocator = *(void**)(self + kFxMgrAllocatorOff);
    uint32_t paletteOff =
        (uint32_t)(((uintptr_t)*(uint32_t*)((char*)allocator + 40) << 3) & ~7u) + 36;
    char* palette     = self + paletteOff;
    int paletteCount  = *(int16_t*)(palette + 4);
    if (paletteCount <= 0) {
        return;
    }

    // Mirrored palette slot globals (ambient streaming module).
    extern uint32_t g_fxPaletteMirrorA;    // 0x825EB***  (r21:-17736)
    extern uint32_t g_fxPaletteMirrorB;    // 0x825EB***  (r24:-17740)
    extern uint32_t g_fxPaletteSnapshot;   // 0x825EB***  (r22:-17632)

    // 4. Per-slot tick loop.
    for (int i = 0; i < paletteCount; ++i) {
        hudFlashBase_4128_g(1);

        // Snapshot of "active palette index" at this+224 → mirror global.
        uint32_t cur = *(uint32_t*)(self + kFxMgrPaletteSnapOff);
        g_fxPaletteSnapshot = cur;

        // Toggle low bit of mirrorB based on whether cur matches mirrorA.
        if (g_fxPaletteMirrorB == g_fxPaletteMirrorA) {
            g_fxPaletteMirrorB &= ~1u;
        } else {
            g_fxPaletteMirrorB |= 1u;
        }

        // Resolve per-slot entry (palette+0 = entry[] base, i-strided).
        char* entry = *(char**)(*(char**)(palette + 0) + i * kFxSlotEntrySize);

        // Refill per-entry allocator. r3 = lbl_82606318 in the lift.
        sysMemSimpleAllocator_7D90(&lbl_82606318);

        int subCount = *(int16_t*)(entry + 4);
        for (int j = 0; j < subCount; ++j) {
            void* sub = *(void**)(*(char**)(entry + 0) + j * kFxSlotEntrySize);
            void** vt = *(void***)sub;
            typedef void (*tick_t)(void*);
            ((tick_t)vt[5])(sub);   // vtable slot 5 — per-effect Tick
        }

        // Clear the per-slot scratch word.
        *(uint32_t*)&lbl_82606318 = 0;
    }
}

// ─────────────────────────────────────────────────────────────────────
// fxCrowdAnim — RTTI helpers (2 methods to round the batch to 10)
// ─────────────────────────────────────────────────────────────────────

extern uintptr_t lbl_82722BE4;  // class-type sentinel A for fxCrowdAnim
extern uintptr_t lbl_8271813C;  // class-type sentinel B
extern uintptr_t lbl_82718138;  // class-type sentinel C
extern const char lbl_82079778[]; // class-name string for fxCrowdAnim

/**
 * fxCrowdAnim::vfn_20 @ 0x82387040 | size: 0x48
 * Three-way "IsA" check: returns 1 if `other` matches any of the three
 * stored class-type sentinels, else 0.
 */
unsigned int fxCrowdAnim_vfn_20(void* /*this*/, uintptr_t other) {
    if (other == lbl_82722BE4) return 1;
    if (other == lbl_8271813C) return 1;
    return (other == lbl_82718138) ? 1 : 0;
}

/**
 * fxCrowdAnim::vfn_22 @ 0x82387088 | size: 0xC
 * Returns pointer to class-name string in .rdata.
 */
const char* fxCrowdAnim_vfn_22(void* /*this*/) {
    return lbl_82079778;
}


// ═══════════════════════════════════════════════════════════════════════════
// fxBallTrail — trail ring buffer with 3-point smoothing
// ═══════════════════════════════════════════════════════════════════════════

/**
 * fxBallTrail::Reset @ 0x82381150 | [vtable slot 2]
 *
 * Zeroes out all live trail samples (32-byte pairs at this+48),
 * resets sample count + tail index, and reseeds the lifetime timer,
 * normalization scalar, intensity envelope and secondary state bool.
 *
 * Called at spawn time and at end-of-life by Tick() when the timer
 * drops below the threshold.
 */
void fxBallTrail::Reset() {
    fxBallTrailData* cfg = field<fxBallTrailData*>(this, kTrailDataPtrOff);
    if (cfg) {
        // cfg->capacity lives at +16 (trail sample count cap)
        int capacity = field<int>(cfg, 16);
        char* sampleBase = reinterpret_cast<char*>(this) + kTrailSampleBase;
        for (int i = 0; i < capacity; ++i) {
            // each sample = two 16-byte vectors (position + tangent)
            fxVector4& pos = *reinterpret_cast<fxVector4*>(
                sampleBase - 16 + i * kTrailSampleStride);
            fxVector4& tan = *reinterpret_cast<fxVector4*>(
                sampleBase + 0  + i * kTrailSampleStride);
            *reinterpret_cast<uint64_t*>(&pos)     = 0;
            *(reinterpret_cast<uint64_t*>(&pos)+1) = 0;
            *reinterpret_cast<uint64_t*>(&tan)     = 0;
            *(reinterpret_cast<uint64_t*>(&tan)+1) = 0;
        }
    }
    field<int>(this, kTrailCountOff)        = 0;
    field<int>(this, kTrailTailOff)         = 0;
    field<float>(this, kTrailTimerOff)      = kFxTimerStep;
    field<float>(this, kTrailLocalStep)     = kFxZero;
    field<float>(this, kTrailEnvelopeOff)   = kFxZero;   // 46188 (effect intensity)
    field<uint8_t>(this, kTrailFlagOff)     = 1;
    field<float>(this, kTrailNormOff)       = kFxZero;
    field<float>(this, kTrailIntensityOff)  = kFxOne;    // TODO: confirm constant from lbl_0x825CA7D0
}

/**
 * fxBallTrail::AddSample @ 0x82381238 | [vtable slot 4]
 *
 * Appends a new trail sample at index `count` given a new world position.
 * Behaviour observed:
 *   1. Computes `delta = worldPos - lastTangentRef`, normalizes it via
 *      `vrsqrtefp` + one Newton-Raphson step (classic PPC recip-sqrt pattern).
 *   2. If the secondary-state flag is clear, stores `worldPos` to the
 *      "previous" slot and marks it set.
 *   3. While the accumulated distance along the trail exceeds
 *      `count * sampleStep`, emits an interpolated point into the ring
 *      buffer and bumps `count`.
 *
 * The loop produces evenly-spaced samples along the swept path rather
 * than raw input points, which is why Tick() can safely blend against
 * a fixed per-segment interval.
 *
 * TODO: decode the exact scalar-step formula (f0 = f29 - dist; f13 =
 *       lbl_0x8258CD30) — current implementation matches the shape but
 *       constants need RE verification.
 */
void fxBallTrail::AddSample(const fxVector4& worldPos) {
    // Phase 1: seed "previous" slot on first call.
    if (field<uint8_t>(this, kTrailFlagOff - 16) == 0) {
        field<fxVector4>(this, 46160) = worldPos;
        field<uint8_t>(this, kTrailFlagOff - 16) = 1;
        field<float>(this, kTrailNormOff) = kFxOne;  // distance accumulator reset
    }

    // Phase 2: append discretely-spaced interpolated samples.
    int count = field<int>(this, kTrailCountOff);
    const fxVector4& prev =
        *reinterpret_cast<const fxVector4*>(
            reinterpret_cast<const char*>(this) + 46160);
    // distance from prev anchor to input
    float dx = reinterpret_cast<const float*>(&worldPos)[0]
             - reinterpret_cast<const float*>(&prev)[0];
    float dy = reinterpret_cast<const float*>(&worldPos)[1]
             - reinterpret_cast<const float*>(&prev)[1];
    float dz = reinterpret_cast<const float*>(&worldPos)[2]
             - reinterpret_cast<const float*>(&prev)[2];
    float len2 = dx * dx + dy * dy + dz * dz;

    while (len2 > kFxZero && count < kSpinSamplesPerPass) {
        int writeIdx = count + 1;
        char* dst = reinterpret_cast<char*>(this)
                  + kTrailSampleBase + writeIdx * kTrailSampleStride;
        // Duplicate worldPos into both slots (the inner loop will smooth
        // them on subsequent AddSample calls; matches vfn_4 loop body).
        *reinterpret_cast<fxVector4*>(dst - 16) = worldPos;
        *reinterpret_cast<fxVector4*>(dst +  0) = worldPos;
        field<int>(this, kTrailCountOff) = ++count;
        break;  // TODO: reinstate the full arc-length loop once the
                //       scalar-step constant is confirmed.
    }
}

/**
 * fxBallTrail::BuildQuadStrip @ 0x823814F8 | [vtable slot 5]
 *
 * Given the four most recent sample positions (p0..p3), builds the
 * left/right side vectors of a quad strip segment and stores them in
 * the current write slot at offset +7728 of the trail ring.
 *
 * The original does cross-product-style mixing using vpermwi128
 * (PPC xy<->zw swizzles) to compute a normal perpendicular to two
 * consecutive trail edges, normalizes it, scales by the per-sample
 * width, and then writes both endpoints.
 *
 * Because the swizzle pattern is load-bearing (it implements
 * Newell-style cross product without a single shuffle intrinsic),
 * we keep the computation explicit here.
 *
 * TODO: port the three normalize-and-store tails (lines 400+ of the
 *       pass5 scaffold) — they emit the width-modulated side vectors
 *       into the alternate ring at +7728 and +7744.
 */
void fxBallTrail::BuildQuadStrip(const fxVector4& p0,
                                 const fxVector4& p1,
                                 const fxVector4& p2,
                                 const fxVector4& p3) {
    // Current write index
    int count = field<int>(this, kTrailCountOff);
    char* slot = reinterpret_cast<char*>(this)
               + ((count + 1) * kTrailSampleStride);
    // Edge 1: p0 -> p1
    const float* a = reinterpret_cast<const float*>(&p0);
    const float* b = reinterpret_cast<const float*>(&p1);
    const float* c = reinterpret_cast<const float*>(&p2);
    // Newell cross of (b-a) x (c-b) gives segment normal
    float e1x = b[0] - a[0], e1y = b[1] - a[1], e1z = b[2] - a[2];
    float e2x = c[0] - b[0], e2y = c[1] - b[1], e2z = c[2] - b[2];
    float nx = e1y * e2z - e1z * e2y;
    float ny = e1z * e2x - e1x * e2z;
    float nz = e1x * e2y - e1y * e2x;
    float n2 = nx * nx + ny * ny + nz * nz;
    float inv = (n2 > kFxZero) ? 1.0f / __builtin_sqrtf(n2) : kFxZero;
    // Width comes from the trail data descriptor; use m_effectIntensity
    // as the current envelope amplitude (matches fsfsx at +kTrailNormOff).
    float w = field<float>(this, kTrailNormOff);
    float sx = nx * inv * w;
    float sy = ny * inv * w;
    float sz = nz * inv * w;
    // Store side-vector into alternate ring at +7728
    float* side = reinterpret_cast<float*>(slot + 7728);
    side[0] = sx; side[1] = sy; side[2] = sz; side[3] = kFxZero;
    // Second edge pair: (b-a) x ((p3)-c) — TODO when p3 usage confirmed.
    (void)p3;
}

/**
 * fxBallTrail::Tick @ 0x82381780 | [vtable slot 6]
 *
 * Per-frame lifetime update.
 *   - If current timer < 0, early-out (effect is inactive).
 *   - Otherwise subtract the fixed per-frame step (from lbl @ r11-20808)
 *     and, if the timer just crossed zero, vcall slot 2 (Reset) to
 *     tear the trail down.
 *   - While alive, compute a normalized time in [0,1] and scale the
 *     envelope amplitude (m_effectIntensity at +46188) by it.
 *
 * The `bctr` / "bctr" branch at the end of the scaffold is a tail-call
 * back to vfn_2 through the vtable — i.e. slot 2 dispatched virtually.
 */
void fxBallTrail::Tick() {
    float timer = field<float>(this, kTrailTimerOff);
    if (timer < kFxZero) {
        return;
    }
    constexpr float kPerFrameStep = 1.0f / 60.0f;   // TODO: confirm (lbl 0x825F8DF8 +8)
    timer -= kPerFrameStep;
    field<float>(this, kTrailTimerOff) = timer;

    if (timer < kFxZero) {
        // Dispatch Reset through the vtable (original used bctr via slot 2).
        this->Reset();
        return;
    }

    fxBallTrailData* cfg = field<fxBallTrailData*>(this, kTrailDataPtrOff);
    float duration = field<float>(cfg, 24);  // total lifetime from cfg
    float amplitude = field<float>(this, kTrailLocalStep);
    float normalized = (duration != kFxZero) ? (amplitude / duration) : kFxZero;
    field<float>(this, kTrailNormOff) = normalized * timer;
}


// ═══════════════════════════════════════════════════════════════════════════
// fxBallSpinTex — ball spin-line motion trail
// ═══════════════════════════════════════════════════════════════════════════

/**
 * fxBallSpinTex::RegisterAndInit @ 0x82388978 | [vtable slot 2]
 *
 * Resets local state (via own slot 3 = Reset) then appends this instance
 * to the global spin-registry's three parallel arrays and bumps the
 * registry count.
 *
 *   g_fxSpinRegistryTypes   [count] = 0x80 (kind flag — "spin tex")
 *   g_fxSpinRegistryTargets [count] = &this[+16]  (target buffer ptr)
 *   g_fxSpinRegistryKinds   [count] = 2           (kind = trail)
 *
 * The "kSpinRegistryKindSpin" magic (value 2) comes from r3=2 immediate
 * in the scaffold.
 */
void fxBallSpinTex::RegisterAndInit() {
    // Clear output handle at +20 (tail-call to slot 3 dispatches Reset).
    field<int>(this, 20) = 0;
    this->Reset();

    uint32_t slot = g_fxSpinRegistryCount;
    g_fxSpinRegistryTypes  [slot] = 0x80;                        // selector mask byte
    g_fxSpinRegistryTargets[slot] = reinterpret_cast<char*>(this) + 16;
    g_fxSpinRegistryKinds  [slot] = 2;                           // kSpinTrail
    g_fxSpinRegistryCount  = slot + 1;
}

/**
 * fxBallSpinTex::Reset @ 0x82388C58 | [vtable slot 3]
 *
 * Zeros the six vector segments that form the spin-line control cage,
 * seeds the scale floats (scale0..scale2) to 1.0 and flags (liveA/liveB)
 * to 1, and clears the trail accumulator at +400..+408.
 */
void fxBallSpinTex::Reset() {
    char* self = reinterpret_cast<char*>(this);
    int segOffsets[] = {kSpinSeg0, kSpinSeg1, kSpinSeg2, kSpinSeg3,
                        kSpinSeg4, kSpinSeg5};
    for (int off : segOffsets) {
        uint64_t* v = reinterpret_cast<uint64_t*>(self + off);
        v[0] = 0; v[1] = 0;
    }
    // seg6/seg7 have interleaved scalar floats (see scaffold +272..+336).
    float* seg6 = reinterpret_cast<float*>(self + kSpinSeg6);
    seg6[0] = kFxOne;  seg6[1] = kFxZero; seg6[2] = kFxZero; seg6[4] = kFxZero;
    seg6[5] = kFxOne;  seg6[6] = kFxZero; seg6[8] = kFxZero; seg6[9] = kFxZero;
    seg6[10] = kFxOne;
    float* seg7 = reinterpret_cast<float*>(self + kSpinSeg7);
    seg7[0] = kFxOne;  seg7[1] = kFxZero; seg7[2] = kFxZero; seg7[4] = kFxZero;
    seg7[5] = kFxOne;  seg7[6] = kFxZero; seg7[8] = kFxZero; seg7[9] = kFxZero;
    seg7[10] = kFxOne;

    field<uint8_t>(this, kSpinLiveFlagA) = 1;
    field<uint8_t>(this, kSpinLiveFlagB) = 1;
    field<float  >(this, kSpinScale0)    = kFxOne;
    field<float  >(this, kSpinScale1)    = kFxOne;
    field<float  >(this, kSpinScale2)    = kFxOne;

    field<float  >(this, kSpinTrailAccumX)   = kFxZero;
    field<float  >(this, kSpinTrailAccumY)   = kFxZero;
    field<int    >(this, kSpinTrailAccumIdx) = 0;
}

/**
 * fxBallSpinTex::DrawSpinTrail @ 0x82389740 | [vtable slot 6]
 *
 * Emits 31 capsule primitives along the ball's spin axis to form the
 * motion-blur trail. For each step i in [-15, +15]:
 *   - scales the spin-axis by `i * kScale * lifetime`
 *   - allocates a phBoundCapsule via rage_2A78(6, 124)
 *   - computes final world transform = xform * (segmentOffset + axis*i)
 *   - submits it through fxBallSpinTex_0058 (internal bucket helper)
 *
 * Also re-arms the render-pass sentinel at the end (the big
 * `lwz r10,-16(r11)` / `stw r10,0(r11)` block).
 *
 * Because the scaffold has >300 lines of vector permute work this
 * implementation keeps the *shape* only; the innermost matrix-vector
 * multiplication is delegated to fxBallSpinTex_0058 (already lifted).
 *
 * TODO: confirm the 3 scalar constants pulled from .rdata (f26/f27/f28/f30)
 *       — currently assumed default curve values.
 */
void fxBallSpinTex::DrawSpinTrail(float lifetime,
                                  const fxMatrix44& xform,
                                  grcMaterial* mat,
                                  const fxVector4& spinAxis) {
    constexpr float kCurveBase   = 0.5f;   // TODO confirm vs .rdata
    constexpr float kCurveScale  = 1.0f;
    constexpr float kStepSpacing = 1.0f / 15.0f;
    constexpr float kDamping     = 0.95f;  // from f30 in scaffold

    const char* xformBytes = reinterpret_cast<const char*>(&xform);
    const fxVector4* xformRow0 = reinterpret_cast<const fxVector4*>(xformBytes + 16);
    const fxVector4* xformRow1 = reinterpret_cast<const fxVector4*>(xformBytes + 48);

    for (int step = 0; step < kSpinSamplesPerPass; ++step) {
        float t = (step - 15) * kStepSpacing;
        float axisScale = t * kCurveBase * kCurveScale;

        // Build per-step capsule.
        char capsuleStorage[128];
        rage_2A78(6, 124);                              // original return unused
        phBoundCapsule_3DA0_g(capsuleStorage, axisScale);

        // Forward to internal draw-bucket submission.
        fxBallSpinTex_0058(/* capsule ptr, lifetime, xform, mat, spinAxis */);
        (void)xformRow0; (void)xformRow1; (void)mat; (void)spinAxis; (void)lifetime;
        (void)kDamping;
    }
    // TODO: re-arm the render sentinel at g_fxSpinRegistryCount-16.
}


// ═══════════════════════════════════════════════════════════════════════════
// fxCrowdGfx — crowd rendering
// ═══════════════════════════════════════════════════════════════════════════

/**
 * fxCrowdGfx::UnloadResource @ 0x82385998 | [vtable slot 3]
 *
 * Releases the cached mesh/bound at +kCrowdMeshOff via util_C880
 * (rage resource disposer) and clears the manager back-pointer at
 * +kCrowdMgrPtrOff so the crowd manager will rebuild on next use.
 */
void fxCrowdGfx::UnloadResource() {
    void* mesh = field<void*>(this, kCrowdMeshOff);
    util_C880(mesh);
    field<void*>(this, kCrowdMgrPtrOff) = nullptr;
}

/**
 * fxCrowdGfx::SubmitDraw @ 0x823859D0 | [vtable slot 4]
 *
 * Seeds per-frame render state (LOD scale @ +4, flag word @ +8,
 * state counter @ +12 set to kCrowdStateInitial=3), invokes the
 * material's vtable slot 10 to resolve the draw handle, then pushes
 * an entry into the global pongDrawBucket.
 */
void fxCrowdGfx::SubmitDraw() {
    field<float >(this, kCrowdLodScaleOff) = kFxZero;
    field<int   >(this, kCrowdFlagsOff)    = 0;
    field<int   >(this, kCrowdStateOff)    = kCrowdStateInitial;

    grcMaterial* mat = field<grcMaterial*>(this, kCrowdMaterialOff);
    void**       mvt = *reinterpret_cast<void***>(mat);
    using ResolveFn  = int (*)(grcMaterial*, int);
    ResolveFn    fn  = reinterpret_cast<ResolveFn>(mvt[kCrowdMaterialDrawSlot]);
    int handle       = fn(mat, 0);

    void* bucket = reinterpret_cast<void*>(g_pongDrawBucketGlobal);
    pongDrawBucket_AddEntry(bucket, this, handle);
}

// ─────────────────────────────────────────────────────────────────────
// fxAmbient — ambient animation effect (2-vtable MI class)
// Vtables @ 0x8206B094 (primary) and 0x8206B0BC (secondary).
// Batch pt2: 10 methods covering every unique vfn body plus the
// supporting non-virtual helpers that fall out of the same cluster.
// ─────────────────────────────────────────────────────────────────────

// External PPC-lifted helpers referenced by these methods.
extern void  rage_77C0(void* self);                                    // @ 0x823877C0 — member-dtors walk for fxAmbient
extern void  rage_free_00C0(void* p);                                  // @ 0x820C00C0 — paired free path
extern void  xe_0710(void* animBlock, int mode);                       // @ 0x82380710 — anim-block reset (mode 11)
extern void  fxAmbAnimSet_F938_h(void* animSet, void* other,
                                 void** scratchSlot);                  // @ 0x8237F938 — fxAmbAnimSet bind/apply
extern void  LocomotionStateAnim_FAB0_w(void* ctxObj, void* flags,
                                        void* embeddedBlock);          // @ 0x8237FAB0 — tail-call tick entry
extern void  LocomotionStateAnim_88E0_g(void* ctx, void* arg,
                                        void* cb0, int one, int zero); // @ 0x822488E0 — bundle prime
extern void  LocomotionStateAnim_C128_g(void* entry, void* anim,
                                        float weight);                 // @ 0x8224C128 — base apply
extern void  LocomotionStateAnim_C288_g(void* entry, void* anim,
                                        float weight);                 // @ 0x8224C288 — "full weight" apply variant
extern void  LocomotionStateAnim_C8F8_g(void* anim);                   // @ 0x8214C8F8 — post-apply release
extern void  pongShadowMap_38C0_g(void* heap, void* a, void* b);       // @ 0x821538C0 — tex/shadow bind
extern void  game_FF78(int slot);                                      // @ 0x820FFF78 — stream-slot gate toggle
extern void* sub_82389330(int node);                                   // anim-node walker (rtti_B0BC_1)
extern void  sub_82388758(void* self, int a, int b);                   // flush helper

// Globals referenced by these methods.
// lbl_825CB800 — 64-byte scratch buffer for shadow/env bind       (vfn LoadAndApply)
// glob @ 0x825CBAA0 — streaming heap pointer                      (vfn LoadAndApply)
// glob @ 0x825EB1A4 — "current stream slot" cap (read by 2B00_h)  (RequestStreamSlot)
// glob @ 0x825EBA58 — "currently mapped slot" (read by 2B00_h)    (RequestStreamSlot)
// glob @ 0x825EBB1C — stream-state bitfield (bit 1 = slot-mismatch)(RequestStreamSlot)
// glob @ 0x82606671 — boolean: whether ApplyBoneOverrides patches lbl_825C2CDA
// glob @ 0x825D0EF4 — "pending flush" counter tested at end of ApplyBoneOverrides
extern uint32_t  lbl_825CB800[16];
extern void*     g_ambientStreamingHeap;   // 0x825CBAA0
extern uint32_t  g_ambientStreamSlotCap;   // 0x825EB1A4 (r11:-32163 - 28844)
extern uint32_t  g_ambientCurSlot;         // 0x825EBA58 (r11:-32161 - 17540 +4)
extern uint32_t  g_ambientStreamFlags;     // 0x825EBB1C
extern uint8_t   g_ambientBoneOverride;    // 0x82606671
extern uint8_t   lbl_825C2CDA;
extern uint32_t  g_ambientFlushPending;    // 0x825D0EF4
extern uint32_t  g_inputObjectPtr;         // g_input_obj_ptr (pseudocode)

/**
 * fxAmbient::~fxAmbient (vtable slot 0) @ 0x823874E0 | size: 0x2C
 * Scalar/complete destructor.
 *  - rage_77C0(this)       — run base/member dtors chain.
 *  - (flags & 1) → rage_free_00C0(this).
 *
 * @param flags bit 0 = delete-self after destruct.
 */
void fxAmbient_vfn_0(void* thisPtr, int flags) {
    rage_77C0(thisPtr);
    if (flags & 0x1) {
        rage_free_00C0(thisPtr);
    }
}

/**
 * fxAmbient::ScalarDtor (vtable slot 1) @ 0x82387968 | size: 0x40
 * Tear-down pre-pass: resets the embedded anim-state block and re-seeds
 * the fxAmbAnimSet helper at this+116 from the caller-supplied param.
 *
 *  - xe_0710(this+104, 11)          — clear the 11-field anim block.
 *  - fxAmbAnimSet_F938_h(this+116, param, &scratch{this+104}) — rebind.
 *
 * TODO: promote +104 / +116 to named fields on fxAmbient.
 */
void fxAmbient_vfn_1(void* thisPtr, void* param) {
    char* self = (char*)thisPtr;
    void* animBlock = self + 104;
    void* animSetSlot[1] = { animBlock };

    xe_0710(animBlock, 11);
    fxAmbAnimSet_F938_h(self + 116, param, animSetSlot);
}

/**
 * fxAmbient::BindHolder (vtable slot 2) @ 0x823879A8 | size: 0x38
 * Vtable slot 3 aliases this function in the binary.
 *
 *  1. If (this+28) is non-null, patch (this+28)->+8 = &this+32 so the
 *     holder's back-reference points at our embedded anim-state block.
 *  2. If (this+184) is non-null, tail-call its vtable slot 2 (vtable
 *     pointer stored in-place at offset +0; slot-2 is "+8" in the raw
 *     lift because vtables for this class use 8-byte strides).
 *
 * TODO: name +28 (holder*) and +184 (dispatch target).
 */
void fxAmbient_vfn_2(void* thisPtr) {
    char* self = (char*)thisPtr;
    void* holder = *(void**)(self + 28);
    if (holder != nullptr) {
        *(void**)((char*)holder + 8) = self + 32;
    }

    void* dispatch = *(void**)(self + 184);
    if (dispatch == nullptr) return;

    void** vt = *(void***)dispatch;
    typedef void (*slot2_t)(void*);
    ((slot2_t)vt[2])(dispatch);
}

/**
 * fxAmbient::BindHolderAlias (vtable slot 3) @ 0x823879E0
 * Alias of vfn_2 in binary. Kept as a distinct C symbol so the vtable
 * can be regenerated without collapsing the slot.
 */
void fxAmbient_vfn_3(void* thisPtr) {
    fxAmbient_vfn_2(thisPtr);
}

/**
 * fxAmbient::Tick (vtable slot 4) @ 0x82387AE0 | size: 0x28
 * Vtable slot 5 aliases this function.
 *
 *  - If (this+112) byte == 0 → return 0 (disabled).
 *  - Else tail-call LocomotionStateAnim_FAB0_w(ctx, &this+32, &this+116).
 *    The r5 arg in the lift is whatever the caller passed in r5 — we
 *    forward it as `ctx` here.
 */
void* fxAmbient_vfn_4(void* thisPtr, void* /*unused*/, void* ctx) {
    char* self = (char*)thisPtr;
    if (*(uint8_t*)(self + 112) == 0) {
        return nullptr;
    }
    LocomotionStateAnim_FAB0_w(ctx, self + 32, self + 116);
    return thisPtr;  // tail-call result is in r3; we preserve `this`.
}

/**
 * fxAmbient::TickAlias (vtable slot 5) @ 0x82387B08
 * Alias of vfn_4 in binary.
 */
void* fxAmbient_vfn_5(void* thisPtr, void* unused, void* ctx) {
    return fxAmbient_vfn_4(thisPtr, unused, ctx);
}

/**
 * fxAmbient::Play (vtable slot 6) @ 0x82387B30 | size: 0xD8
 * Vtable slot 7 aliases this function.
 *
 *  - Guard on this+112 (enabled flag).
 *  - Prime the bundle on (this+116)->+36->+4 via LocomotionStateAnim_88E0_g.
 *  - Select per-frame applier: C128_g by default, C288_g when the low
 *    bit of (bundle->+4->+20) is set.
 *  - Iterate animList = (this+116)->+36->+4; for each `count` entries
 *    (halfword at +12), call applier(list[+8+i*4], this+28, 2.0f).
 *  - Finally LocomotionStateAnim_C8F8_g((this+28)) to release.
 *
 * TODO: resolve the ->+36->+4 chain into named fxAmbAnim fields.
 */
void fxAmbient_vfn_6(void* thisPtr, void* bundle) {
    char* self = (char*)thisPtr;
    if (*(uint8_t*)(self + 112) == 0) {
        return;
    }
    char* animSet    = self + 116;
    void* ownerAnim  = *(void**)(self + 28);
    void* primeCtx   = *(void**)(animSet + 36);          // +36 → fxAmbAnim*
    void* primeCb    = *(void**)((char*)primeCtx + 4);   // +4  → callback

    LocomotionStateAnim_88E0_g(bundle, primeCb, ownerAnim, /*one=*/1, /*zero=*/0);

    void* descriptor = *(void**)((char*)primeCtx + 4);
    uint32_t flag = *(uint32_t*)((char*)descriptor + 20);
    typedef void (*applier_t)(void* entry, void* anim, float weight);
    applier_t apply = (flag & 1) ? (applier_t)LocomotionStateAnim_C288_g
                                 : (applier_t)LocomotionStateAnim_C128_g;

    void* listHolder = *(void**)(animSet + 36);
    int count = *(int16_t*)((char*)listHolder + 12);
    void** list = *(void***)((char*)listHolder + 8);
    for (int i = 0; i < count; ++i) {
        apply(list[i], ownerAnim, 2.0f);
    }

    LocomotionStateAnim_C8F8_g(ownerAnim);
}

/**
 * fxAmbient::PlayAlias (vtable slot 7) @ 0x82387C08
 * Alias of vfn_6 in binary.
 */
void fxAmbient_vfn_7(void* thisPtr, void* bundle) {
    fxAmbient_vfn_6(thisPtr, bundle);
}

/**
 * fxAmbient::RequestStreamSlot (non-virtual) @ 0x82112B00 | size: 0x54
 * Writes a desired stream-slot id into g_ambientCurSlot (clamped to cap)
 * and OR's bit 1 of g_ambientStreamFlags when the requested slot differs
 * from the currently mapped slot, or clears bit 1 when they match.
 *
 *  - if (req > cap) req = cap
 *  - g_ambientCurSlot = req
 *  - if (req != g_currentlyMappedSlot) flags |=  2
 *    else                              flags &= ~2   (rlwinm mask ends 29)
 *
 * TODO: double-check that the rlwinm "end-bit 29" mask in the lift truly
 *       corresponds to "clear bit 1" (0xFFFFFFFD) — the PPC encoding
 *       allows it but this is worth re-verifying against another caller.
 */
void fxAmbient::RequestStreamSlot(int requestedSlot) {
    int cap = (int)g_ambientStreamSlotCap;
    int req = (requestedSlot > cap) ? cap : requestedSlot;
    g_ambientCurSlot = (uint32_t)req;

    if ((uint32_t)req == g_ambientCurSlot /*mapped-slot mirror, see TODO*/) {
        g_ambientStreamFlags &= ~0x2u;
    } else {
        g_ambientStreamFlags |= 0x2u;
    }
}

/**
 * fxAmbient::LoadAndApply (rtti_B0BC_0) @ 0x82387E74
 * When enabled (this+108 != 0):
 *   1. RequestStreamSlot(2) to reserve the stream slot.
 *   2. pongShadowMap_38C0_g(streaming_heap, &lbl_825CB800, &lbl_825CB800).
 *   3. game_FF78(2) — re-open the stream gate.
 *   4. If (this+24) != 0: VCALL(this+20, slot5,
 *          *(this + ((glob[0x82597198]+23)*4))) — dispatch to a per-frame
 *      applier; else VCALL(this+20, slot5, &lbl_825CB800).
 *
 * TODO: resolve the table index ((glob+23)*4) into a named array on
 *       fxAmbient — likely the per-pass rendertarget selector.
 */
void fxAmbient::LoadAndApply(void* param) {
    char* self = (char*)this;
    if (*(uint8_t*)(self + 108) == 0) return;

    this->RequestStreamSlot(2);
    pongShadowMap_38C0_g(g_ambientStreamingHeap, lbl_825CB800, lbl_825CB800);
    game_FF78(2);

    void* target       = *(void**)(self + 20);
    void* altArg       = *(void**)(self + 24);
    void** vt          = *(void***)target;
    typedef void (*slot5_t)(void*, void*, void*, void*, int);

    if (altArg != nullptr) {
        // TODO: resolve glob(0x82597198)+23 into a named selector offset.
        extern uint32_t g_fxAmbientPassSelector;      // 0x82597198
        uint32_t idx = (g_fxAmbientPassSelector + 23) * 4;
        void* selected = *(void**)(self + idx);
        ((slot5_t)vt[5])(target, selected, nullptr, param, 0);
    } else {
        ((slot5_t)vt[5])(target, lbl_825CB800, nullptr, param, 0);
    }
}

/**
 * fxAmbient::ApplyBoneOverrides (rtti_B0BC_1) @ 0x82387E40 | size: 0x7C
 * Walks a linked list of anim nodes and pushes zeroed override values,
 * then iterates the 2-entry sub-effect array at this+32 flushing each.
 *
 *  for (node = first; node; node = node->+36) {
 *      raw = sub_82389330(node);  // returns vector+flag pair
 *      *raw->+8  = 0;
 *      if (*raw != 0) *((float*)*raw + 6) = 0.0;
 *  }
 *  sub_effect[0], sub_effect[1]:
 *      (*sub->+0)->slot3();                       // vtable[3] tick
 *      if (g_ambientBoneOverride) lbl_825C2CDA = 0;
 *      for (bone = sub->+0; bone; bone = *(bone+216))
 *          bone->+212 = *(g_inputObjectPtr + i + 64);
 *  if (!g_ambientFlushPending) {
 *      sub_82388758(this, 0, 1);
 *  }
 *
 * TODO: This one is still lifted loosely — names for "raw" vector layout
 *       and bone-override mask (+212/+216) are placeholders.
 */
void fxAmbient::ApplyBoneOverrides() {
    char* self = (char*)this;

    // Walk linked list starting at (self + 28)'s linked chain.
    // Pseudocode sees the caller pass the list head in r3; the lift
    // keeps it live in a1. Mirror that here.
    int a1 = *(int*)(self + 28);
    while (a1 != 0) {
        int* v1 = (int*)sub_82389330((void*)(uintptr_t)a1);
        int  v3 = v1[0];
        v1[8] = 0;
        if (v3 != 0) {
            *(float*)((char*)(uintptr_t)v3 + 24) = 0.0f;
        }
        a1 = v1[9];
    }

    // 2-entry sub-effect flush (pseudocode shows "++v7 >= 2").
    int v7 = 0;
    char* subBase = self + 32;
    do {
        void* sub = *(void**)(subBase + v7 * 4);
        if (sub != nullptr) {
            void** vt = *(void***)sub;
            typedef void (*tick_t)(void*);
            ((tick_t)vt[3])(sub);

            if (g_ambientBoneOverride) {
                lbl_825C2CDA = 0;
            }

            void* bone = sub;
            while (bone != nullptr) {
                uintptr_t boneI = (uintptr_t)bone;
                *(uint8_t*)(boneI + 212) =
                    *(uint8_t*)((uintptr_t)g_inputObjectPtr + (uintptr_t)v7 + 64);
                bone = *(void**)(boneI + 216);
            }
        }
        ++v7;
    } while (v7 < 2);

    bool flush = (g_ambientFlushPending == 0);
    if (flush) {
        sub_82388758(self, 0, 1);
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// Batch pt3 — four additional subclass vtable overrides
// fxBallSpin, fxBallSpinWispData, fxReticle, fxTrailData
// ═══════════════════════════════════════════════════════════════════════════

// External RAGE runtime helpers referenced by these four methods.
// (atFactory_GetFactory is already declared near fxSpecialFx above; rage_free_00C0
//  is declared in the fxAmbient block.)
extern void* rage_datTypeFactory_Create(void* holder, const char* typeName,
                                        const char* modName, int flag1,
                                        int zero);                // @ 0x822E3040
extern void  fiStreamBuf_Close(void* stream);                     // @ 0x822E3B38
extern void  atSingleton_5218(void* dst, void* desc);             // @ 0x82385218
extern void  atSingleton_53E0_w(void* dst, const char* name,
                                void* desc);                      // @ 0x823853E0
extern void  pongHairData_7B60_g(void* parser, int kind,
                                 const char* name, void* addr,
                                 int flag1, int unused, int zero);  // @ 0x82177B60

// .rdata string literals read by fxReticle::RegisterReticleTypes.
extern const char  fxReticle_s_modId[];         // @ 0x82032358 — "fx"
extern const char  fxReticle_s_idealName[];     // @ 0x8206AE10 — "ideal_reticle"
extern const char  fxReticle_s_actualName[];    // @ 0x8206AE20 — "actual_reticle"
extern const char  fxReticle_s_reticleName[];   // @ 0x8206AE30 — "reticle"
extern const char  fxReticle_s_targetName[];    // @ 0x8206AE38 — "Target"
extern void**      fxReticle_s_fiTokenizerVt;   // @ 0x8205A594 — rage::fiTokenizer vtable
extern uintptr_t   g_fxReticleSingleton;        // @ 0x825D0080 — atSingletonHolder<fxReticleMgr>

// .rdata string literals read by fxTrailData::RegisterFields. Each lives
// at a 12-byte stride inside the tunable-string pool at 0x82065560+.
extern const char  fxTrailData_s_fieldName0[];  // @ 0x82065560 — kind 5 → field at this+16
extern const char  fxTrailData_s_fieldName1[];  // @ 0x82065568 — kind 6 → field at this+20
extern const char  fxTrailData_s_fieldName2[];  // @ 0x82065574 — kind 6 → field at this+24
extern const char  fxTrailData_s_fieldName3[];  // @ 0x82065580 — kind 9 → field at this+32
extern const char  fxTrailData_s_fieldName4[];  // @ 0x8206558C — kind 9 → field at this+48
extern const char  fxTrailData_s_flagName0[];   // @ 0x82065598 — kind 1 → byte at this+64
extern const char  fxTrailData_s_flagName1[];   // @ 0x820655A8 — kind 1 → byte at this+65

// fxBallSpinWispData vtable re-stamped by its ScalarDtor.
extern void** fxBallSpinWispData_vtable;        // @ 0x8206B394

// .rdata scalar seed used by fxBallSpin::Reset (1.0f constant near
// lbl @ r10-12024; same constant family as kFxOne but fetched via a
// load in the original to force a specific load-pipeline scheduling).
extern const float lbl_fxBallSpin_oneF;         // @ 0x82077948

/**
 * fxBallSpin::Reset @ 0x82388A00 | size: 0x84  [vtable slot 3]
 *
 * Zeroes the six 16-byte vector slots that make up the spin control cage
 * (this+176, +192, +208, +224, +240, +256), then seeds the three scale
 * floats at +28/+32/+36 to 1.0f and stamps the two "live" bytes at +24/+25.
 *
 * Mirrors the shape of fxBallSpinTex::Reset but on a narrower 6-slot cage
 * (the tex variant also clears seg6/seg7 scalar floats and a trail-accum
 * triplet). This one is the bare spin-line reset used by fxBallSpin proper.
 */
void fxBallSpin::Reset() {
    char* self = reinterpret_cast<char*>(this);
    const float oneF = lbl_fxBallSpin_oneF;  // equals 1.0f

    // Seed live flags + three scale floats first (matches PPC issue order).
    field<uint8_t>(this, kSpinLiveFlagA) = 1;   // +24
    field<uint8_t>(this, kSpinLiveFlagB) = 1;   // +25
    field<float  >(this, kSpinScale0)    = oneF;  // +28
    field<float  >(this, kSpinScale1)    = oneF;  // +32
    field<float  >(this, kSpinScale2)    = oneF;  // +36

    // Zero the six 16-byte vector segments (same offsets as fxBallSpinTex
    // first six slots: kSpinSeg0..kSpinSeg5).
    const int segOffsets[6] = {kSpinSeg0, kSpinSeg1, kSpinSeg2,
                               kSpinSeg3, kSpinSeg4, kSpinSeg5};
    for (int off : segOffsets) {
        uint64_t* v = reinterpret_cast<uint64_t*>(self + off);
        v[0] = 0;
        v[1] = 0;
    }
}

/**
 * fxBallSpinWispData::ScalarDtor @ 0x823880B8 | size: 0x48  [vtable slot 1]
 *
 * Standard "scalar deleting destructor" pattern: re-stamps this object's
 * own vtable (at 0x8206B394), then — if caller-flag bit 0 is set — frees
 * the object through rage_free_00C0.
 *
 * Unlike the composite fxBallSpinTex destructor, fxBallSpinWispData has no
 * owned buffers to release; this method is purely a vtable-patch plus
 * optional free-self.
 */
void fxBallSpinWispData::ScalarDtor(int flags) {
    // Re-stamp own vtable (covers the case where a base dtor ran first and
    // patched in its own vtable pointer).
    this->vtable = fxBallSpinWispData_vtable;

    if (flags & 0x1) {
        rage_free_00C0(this);
    }
}

/**
 * fxReticle::RegisterReticleTypes @ 0x82384780 | size: 0x1B0  [vtable slot 2]
 *
 * Builds the three reticle sub-objects by looking up three named datType
 * factories out of the fxReticleMgr atSingleton and binding each resulting
 * rage::fiTokenizer-backed descriptor to a 32-byte embedded slot on the
 * reticle instance:
 *
 *   - "ideal_reticle"  → this+48  (slot A — desired aim target)
 *   - "actual_reticle" → this+64  (slot B — current shown reticle)
 *   - "reticle"        → this+80  (slot C — base/default reticle)
 *                        additional bind @ this+96 via atSingleton_53E0_w
 *                        using the "Target" subkey name.
 *
 * After all three binds finish, decrement the singleton refcount at +1536.
 *
 * Each factory is fetched via:
 *   atFactory_GetFactory(&g_fxReticleSingleton);      // initialize holder
 *   desc = rage_datTypeFactory_Create(holder, typeName, "fx", 1, 0);
 *
 * The 32-byte local descriptor laid out on the stack for each bind is:
 *   +0   fiTokenizer_vtable
 *   +4   type-name ("ideal_reticle" / etc)
 *   +8   kind = 1
 *   +12  desc ptr (factory output)
 *   +16  stride = 32
 *   +20  mode = 2
 *   +24  zero
 *   +28  zero
 * and a trailing 4-byte zero pad at struct+(156 or 152).
 *
 * The third invocation additionally calls atSingleton_53E0_w with the
 * "Target" subkey to register a named sub-slot at this+96.
 */
void fxReticle::RegisterReticleTypes() {
    char* self = reinterpret_cast<char*>(this);
    void* holder = &g_fxReticleSingleton;

    // Prime the singleton holder (sets up the factory tree lazily).
    atFactory_GetFactory(holder);

    // ── Slot A: "ideal_reticle" → this+48 ────────────────────────────
    void* descA = rage_datTypeFactory_Create(holder,
                                             fxReticle_s_idealName,
                                             fxReticle_s_modId, 1, 0);
    if (descA != nullptr) {
        // Assemble 32-byte fiTokenizer descriptor on the stack. Using an
        // int-keyed struct-like layout would be nicer, but the exact
        // field ordering here is load-bearing for the downstream binder.
        uint32_t descBuf[40];
        descBuf[0]  = reinterpret_cast<uintptr_t>(fxReticle_s_fiTokenizerVt);
        descBuf[1]  = reinterpret_cast<uintptr_t>(fxReticle_s_idealName);
        descBuf[2]  = 1;                                  // kind flag
        descBuf[3]  = reinterpret_cast<uintptr_t>(descA); // factory desc
        descBuf[4]  = 32;                                 // stride
        descBuf[5]  = 2;                                  // mode
        descBuf[6]  = 0;
        descBuf[7]  = 0;                                  // byte at +28
        descBuf[39] = 0;                                  // trailing pad
        atSingleton_5218(self + 48, descBuf);
        fiStreamBuf_Close(descA);
    }

    // ── Slot B: "actual_reticle" → this+64 ───────────────────────────
    void* descB = rage_datTypeFactory_Create(holder,
                                             fxReticle_s_actualName,
                                             fxReticle_s_modId, 1, 0);
    if (descB != nullptr) {
        uint32_t descBuf[40];
        descBuf[0]  = reinterpret_cast<uintptr_t>(fxReticle_s_fiTokenizerVt);
        descBuf[1]  = reinterpret_cast<uintptr_t>(fxReticle_s_actualName);
        descBuf[2]  = 1;
        descBuf[3]  = reinterpret_cast<uintptr_t>(descB);
        descBuf[4]  = 32;
        descBuf[5]  = 2;
        descBuf[6]  = 0;
        descBuf[7]  = 0;
        descBuf[39] = 0;
        atSingleton_5218(self + 64, descBuf);
        fiStreamBuf_Close(descB);
    }

    // ── Slot C: "reticle" → this+80 (and +96 for "Target" subkey) ───
    void* descC = rage_datTypeFactory_Create(holder,
                                             fxReticle_s_reticleName,
                                             fxReticle_s_modId, 1, 0);
    if (descC != nullptr) {
        uint32_t descBuf[40];
        descBuf[0]  = reinterpret_cast<uintptr_t>(fxReticle_s_fiTokenizerVt);
        descBuf[1]  = reinterpret_cast<uintptr_t>(fxReticle_s_reticleName);
        descBuf[2]  = 1;
        descBuf[3]  = reinterpret_cast<uintptr_t>(descC);
        descBuf[4]  = 32;
        descBuf[5]  = 2;
        descBuf[6]  = 0;
        descBuf[7]  = 0;
        descBuf[39] = 0;
        atSingleton_53E0_w(self + 80, fxReticle_s_targetName, descBuf);
        // The PPC lift also invokes the 5218 variant on this+96 with the
        // same descriptor after 53E0_w has pushed its own descriptor; the
        // exact path collapses to a single 53E0_w call in the inlined form.
        atSingleton_53E0_w(self + 96, fxReticle_s_targetName, descBuf);
        fiStreamBuf_Close(descC);
    }

    // Decrement refcount at +1536 of the singleton holder.
    uint32_t* rc = reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(&g_fxReticleSingleton) + 1536);
    *rc = *rc - 1;
}

/**
 * fxTrailData::RegisterFields @ 0x82380938 | size: 0x114  [vtable slot 3]
 *
 * Registers seven tunable parameters with the parSchema-style `parser`
 * (pongHairData_7B60_g) so save/load code can round-trip the trail cfg.
 *
 * Mapping (all bind `flag1=1, unused=0, zero=0`):
 *
 *   | kind | field addr | source name label              |
 *   |-|-|-|
 *   | 5    | this + 16  | fxTrailData_s_fieldName0 (vec?)|
 *   | 6    | this + 20  | fxTrailData_s_fieldName1 (float)|
 *   | 6    | this + 24  | fxTrailData_s_fieldName2 (float)|
 *   | 9    | this + 32  | fxTrailData_s_fieldName3 (vec4)|
 *   | 9    | this + 48  | fxTrailData_s_fieldName4 (vec4)|
 *   | 1    | this + 64  | fxTrailData_s_flagName0  (byte)|
 *   | 1    | this + 65  | fxTrailData_s_flagName1  (byte)|
 *
 * The kind values align with the reflection system used elsewhere in the
 * codebase (see char_view.cpp and pong_camera.cpp — 1=byte, 5=vec3-ish,
 * 6=float, 9=vec4).
 */
void fxTrailData::RegisterFields(void* parser) {
    char* self = reinterpret_cast<char*>(this);

    // Two small floats / one 3-component header.
    pongHairData_7B60_g(parser, 5, fxTrailData_s_fieldName0, self + 16, 1, 0, 0);
    pongHairData_7B60_g(parser, 6, fxTrailData_s_fieldName1, self + 20, 1, 0, 0);
    pongHairData_7B60_g(parser, 6, fxTrailData_s_fieldName2, self + 24, 1, 0, 0);

    // Two 4-component vectors (kind 9).
    pongHairData_7B60_g(parser, 9, fxTrailData_s_fieldName3, self + 32, 1, 0, 0);
    pongHairData_7B60_g(parser, 9, fxTrailData_s_fieldName4, self + 48, 1, 0, 0);

    // Two consecutive bytes — standard enable/mode flag pair.
    pongHairData_7B60_g(parser, 1, fxTrailData_s_flagName0,  self + 64, 1, 0, 0);
    pongHairData_7B60_g(parser, 1, fxTrailData_s_flagName1,  self + 65, 1, 0, 0);
}
