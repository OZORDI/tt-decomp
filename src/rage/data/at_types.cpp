/**
 * at_types.cpp — RAGE at* / datRef / datResource / datArray bridging stubs
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Consolidation target for the proto-RAGE `at*` (array-template) family
 * surface that the rest of the tree calls but for which the original
 * template expansion has not yet been individually decompiled.
 *
 * Real, address-backed lifts that share these names live in:
 *   - src/rage/memory/atArray.cpp       (atArray_Destructor @ 0x820C2980)
 *   - src/rage/memory/atSingleton.cpp   (atSingleton_CAD0_g, ..._Find_90D0,
 *                                        ..._23C0, numbered variants)
 *   - src/rage/memory/rage_memory.cpp   (atSingleton_0128_wrh, _2038, _23C0,
 *                                        datArray_Grow sites)
 *   - src/rage/misc/rage_at.cpp         (atSingleton_23C0 bit-stream helper)
 *
 * Those files are not part of the active CMake `SOURCES` set in this tree,
 * so the functions below continue to provide the weak-body linker surface
 * every other translation unit needs. As soon as the above files are added
 * to CMakeLists.txt, the address-backed versions supersede these and this
 * file can be emptied.
 *
 * Most symbols here remain thin bridges. Where the binary body is fully
 * recovered and the active build still routes through this file, we keep a
 * concrete implementation here so callers stop depending on stubs.cpp.
 */

#include <cstdint>

namespace {

uint32_t ComputeHashString(const char* text)
{
    if (text == nullptr) {
        return 0;
    }

    uint32_t hash = 0;
    while (*text != '\0') {
        hash = (hash << 4) + static_cast<uint8_t>(*text++);

        const uint32_t highBits = hash & 0xF0000000;
        if (highBits != 0) {
            hash ^= (highBits >> 24);
            hash ^= highBits;
        }
    }

    return hash;
}

bool StringsMatch(const char* left, const char* right)
{
    if (left == right) {
        return true;
    }

    if (left == nullptr || right == nullptr) {
        return false;
    }

    while (*left != '\0' && *left == *right) {
        ++left;
        ++right;
    }

    return *left == *right;
}

} // namespace

// All symbols below use C linkage — C source files (src/rage/core/app_init.c,
// src/rage/render_loop.c, src/xam/*) reach them through underscore-prefixed
// symbol names. C++ callers just take the unmangled name.
extern "C" {

// ─────────────────────────────────────────────────────────────────────────────
// atHashMap — hash-bucketed container surface.
// atHashMap_Clear  @ rage_C0E0 (~0x8234C0E0, caller: pongCreatureType dtor).
// atHashMap_Find   — name-keyed lookup invoked by rage_par.cpp and several
//                    animation blend-clip caches (pcr_anim_blenders.cpp).
// ─────────────────────────────────────────────────────────────────────────────
void atHashMap_Clear(void* container) { (void)container; }
void* atHashMap_Find(void* table, const void* key) {
    if (table == nullptr) {
        return nullptr;
    }

    const char* searchKey = static_cast<const char*>(key);
    const uint16_t bucketCount = *reinterpret_cast<uint16_t*>(
        static_cast<uint8_t*>(table) + 4);
    if (bucketCount == 0) {
        return nullptr;
    }

    void*** bucketArray = *reinterpret_cast<void****>(table);
    if (bucketArray == nullptr) {
        return nullptr;
    }

    const uint32_t bucketIndex = ComputeHashString(searchKey) % bucketCount;
    void** entry = static_cast<void**>(bucketArray[bucketIndex]);

    while (entry != nullptr) {
        const char* entryKey = static_cast<const char*>(entry[0]);
        if (StringsMatch(searchKey, entryKey)) {
            return static_cast<uint8_t*>(static_cast<void*>(entry)) + 4;
        }

        entry = static_cast<void**>(entry[2]);
    }

    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// atArray — flat dynamic-array base surface.
//   atArray_Clear      @ ~0x821A8070 (gameObject base dtor).
//   atArray_Destructor @ 0x820C2980  (weak body here; an address-backed
//                                     version lives in src/rage/memory/
//                                     atArray.cpp when that file is linked).
// ─────────────────────────────────────────────────────────────────────────────
void atArray_Clear(void* obj) { (void)obj; }
void* atArray_Destructor(void* pArray) { (void)pArray; return nullptr; }

// ─────────────────────────────────────────────────────────────────────────────
// atSingleton — registry/tracker family. Named slots are documented at their
// real lift sites. Duplicates here intentionally kept *very* thin so the
// real ones in rage/memory/atSingleton.cpp (when added to CMakeLists) win.
// ─────────────────────────────────────────────────────────────────────────────
uint8_t atSingleton_Find(void* ptr) {
    // Real: atSingleton_Find_90D0 @ 0x820F90D0 — hashed registry lookup.
    (void)ptr;
    return 0;
}

void atSingleton_Remove(void* singleton) {
    // Real: atSingleton_1C70_fw @ 0x82151C70.
    (void)singleton;
}

void  atSingleton_0128_wrh(void* a, uint32_t b) { (void)a; (void)b; }
void  atSingleton_2038(void* a, uint32_t b)     { (void)a; (void)b; }
void  atSingleton_23C0(void* a)                 { (void)a; }
void  atSingleton_29E8_p25(void* a)             { (void)a; }
void  atSingleton_5CD0_fw(void* a, uint32_t b)  { (void)a; (void)b; }
void  atSingleton_8068_h(void* a)               { (void)a; }
void  atSingleton_8A48_p42(void* a)             { (void)a; }
void  atSingleton_8E88_p42(void* a, void* b, uint32_t c, uint32_t d, uint32_t e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}
void* atSingleton_CAD0_g(void* a) { (void)a; return nullptr; }

// atSingleton_QueryEventData — data-query utility invoked by gameplay code;
// confirmed via find_callers to be non-network (no msgMsgSink references).
void atSingleton_QueryEventData(void* a, void* b) { (void)a; (void)b; }

// ─────────────────────────────────────────────────────────────────────────────
// atFactory — singleton-of-factory dispatcher.
//   atFactory_GetFactory: called by fx_effects.cpp and mc_memcard.cpp.
// Real body is in rage/misc/rage_at.cpp (line ~450).
// ─────────────────────────────────────────────────────────────────────────────
void atFactory_GetFactory(void* a) { (void)a; }

// ─────────────────────────────────────────────────────────────────────────────
// datArray / datRef / datResource — data-system helpers.
// datArray_Grow  — bucket expansion on atArray backing storage.
// datRef_Release — reference-count release (real @ rage_6A90 0x82126A90).
// datResource_Load — blocking load dispatch; ph_physics.cpp calls this.
// ─────────────────────────────────────────────────────────────────────────────
void datArray_Grow(void* a, uint32_t b)             { (void)a; (void)b; }
void datRef_Release(void* ref)                      { (void)ref; }
void datResource_Load(void* a, const char* b)       { (void)a; (void)b; }

} // extern "C"
