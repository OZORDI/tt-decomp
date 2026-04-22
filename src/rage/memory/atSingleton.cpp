/**
 * atSingleton.cpp — RAGE Engine Singleton Management System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements singleton registration, lookup, lifecycle management, and
 * observer notification for the RAGE engine's global object registry.
 *
 * Functions:
 *   ComputeHash          — ELF hash for string-based singleton keys
 *   FindSingleton        — hash table lookup in global registry (stub)
 *   UnregisterSingleton  — remove singleton by name (stub)
 *   InitializeSingleton  — dynamic array growth for singleton entries
 *   ReleaseSingleton     — destroy singleton + notify observers
 *   AcquireReference     — increment reference count
 *   BindObject           — register singleton by hash (stub)
 *   UnbindObject         — unregister singleton by hash (stub)
 *   InitializeNetSystem  — network singleton init (stub)
 *   NotifyObservers      — broadcast singleton events
 *   ClearPendingFlag     — clear bit 0 of state flags
 *   CallVirtualDestructor — vtable slot 0 dispatch
 *   GetFactory           — factory registry lookup (stub)
 *   atSingleton_ScrambleData — data transform using magic-constant division
 */

#include "rage/atSingleton.hpp"
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// External functions
// ─────────────────────────────────────────────────────────────────────────────

extern "C" {
    // RAGE heap allocator.
    void* rage_alloc(uint32_t size);

    // RAGE mode-based deallocator.
    // @ 0x820C2980 | size: 0x5C
    void rage_dealloc(void* ptr, uint32_t mode);
}

// Hash table lookup — searches global singleton hash table for a key.
// @ 0x820F94F0 | size: 0xA4
extern void* atHashTable_Find(void* hashTable, uint32_t key);

// xmlTree cleanup — resets/cleans up a rage::xmlTree resource.
// @ 0x821A9EC0 | size: 0x48 | class: rage::xmlTree
extern void xmlTree_Cleanup(void* obj);


// ─────────────────────────────────────────────────────────────────────────────
// External globals
// ─────────────────────────────────────────────────────────────────────────────

// Global singleton registry hash table (65,616 bytes).
// @ 0x82607C40
extern uint8_t g_singletonRegistry[];


namespace rage {


// ═══════════════════════════════════════════════════════════════════════════════
//  ComputeHash
//
//  @ 0x8223A818 | size: 0x50
//
//  Standard ELF hash algorithm — used throughout the RAGE engine for
//  string-based lookups in hash tables and singleton registries.
// ═══════════════════════════════════════════════════════════════════════════════
uint32_t ComputeHash(const char* str)
{
    if (!str)
        return 0;

    uint32_t hash = 0;

    while (*str) {
        hash = (hash << 4) + static_cast<uint8_t>(*str++);

        uint32_t highBits = hash & 0xF0000000;
        if (highBits) {
            hash ^= (highBits >> 24);
            hash ^= highBits;
        }
    }

    return hash;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  FindSingleton
//
//  @ 0x820F90D0 | size: 0xA4
//  Original symbol: atSingleton_IsTracked
//
//  Searches the global singleton registry hash table for a singleton
//  matching the given hash.
//
//  If hash < 0 (signed):
//    1. Stores hash as search key in stack locals
//    2. Calls atHashTable_Find on g_singletonRegistry
//    3. If found (result != -1): computes entry address via (index << 3)
//       and reads data from the registry
//    4. If not found (-1): falls through to return 0
//
//  Returns: pointer to singleton entry, or nullptr if not found.
// ═══════════════════════════════════════════════════════════════════════════════
atSingleton* FindSingleton(uint32_t hash)
{
    if (static_cast<int32_t>(hash) < 0) {
        // Search the global singleton registry hash table
        int32_t index = reinterpret_cast<intptr_t>(
            atHashTable_Find(g_singletonRegistry, hash));

        if (index != -1) {
            // Compute entry offset: (index << 3) = index * 8
            uint32_t entryOffset = static_cast<uint32_t>(index) << 3;
            uint32_t* registryBase = reinterpret_cast<uint32_t*>(
                g_singletonRegistry);

            // Read entry base from registry at computed offset
            uint32_t entryBase = *reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(registryBase) + entryOffset);

            // Compute final pointer: hash - entryBase
            return reinterpret_cast<atSingleton*>(
                static_cast<uintptr_t>(hash - entryBase));
        }
    }

    return nullptr;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  UnregisterSingleton  (aka DuplicateName — the @0x820C29E0 helper does
//  strlen + rage_alloc + memcpy and returns.  The name "UnregisterSingleton"
//  is from the project's symbol map, but the scaffold body is only a name
//  duplication routine — callers use the returned heap string as the registry
//  key elsewhere.)
//
//  @ 0x820C29E0 | size: 0x74
//
//  Fully matches scaffold: no registry-removal code exists inside this
//  function.  Any actual unregistration happens in the caller using the
//  duplicated name pointer.
// ═══════════════════════════════════════════════════════════════════════════════
void UnregisterSingleton(const char* name)
{
    if (!name)
        return;

    // Calculate string length (manual strlen — matches scaffold loop).
    const char* p = name;
    while (*p) p++;
    uint32_t length = static_cast<uint32_t>(p - name);

    // Allocate and copy (length + NUL terminator).
    uint32_t allocSize = length + 1;
    void* nameCopy = rage_alloc(allocSize);

    if (nameCopy) {
        std::memcpy(nameCopy, name, allocSize);
    }

    // Scaffold ends here — caller is responsible for using nameCopy.
    // (The original function returns void; the allocation leak-on-null is
    //  matched to original behavior.)
}


// ═══════════════════════════════════════════════════════════════════════════════
//  InitializeSingleton
//
//  @ 0x821A8588 | size: 0x194 (404 bytes)
//
//  Initializes/grows the singleton array with dynamic capacity management.
//  Grows by 16 entries at a time, clamped to max 0x5555 (21845) entries.
//  Each entry is 12 bytes (3 × uint32_t: name, hash, instance, refCount).
// ═══════════════════════════════════════════════════════════════════════════════
void InitializeSingleton(SingletonArray* singletonArray)
{
    if (!singletonArray)
        return;

    // Check if array needs to grow
    if (singletonArray->count >= singletonArray->capacity) {
        uint16_t newCapacity = singletonArray->capacity + 16;

        // Clamp to maximum (0x5555 = 21845 entries)
        if (newCapacity > 0x5555)
            return;

        // Each entry is 12 bytes (3 × uint32_t)
        uint32_t allocSize = newCapacity * 3 * sizeof(uint32_t);

        // Allocate new array (extra 4 bytes for capacity header)
        SingletonEntry* newEntries = static_cast<SingletonEntry*>(
            rage_alloc(allocSize + 4));

        if (newEntries) {
            // Store capacity at start
            *reinterpret_cast<uint32_t*>(newEntries) = newCapacity;

            // Actual entries start after capacity field
            SingletonEntry* entries = reinterpret_cast<SingletonEntry*>(
                reinterpret_cast<uint8_t*>(newEntries) + 4);

            // Initialize new entries to zero
            for (uint16_t i = 0; i < newCapacity; i++) {
                entries[i].name = nullptr;
                entries[i].hash = 0;
                entries[i].instance = nullptr;
                entries[i].refCount = 0;
            }

            // Copy existing entries
            if (singletonArray->count > 0) {
                for (uint16_t i = 0; i < singletonArray->count; i++) {
                    // Copy name string
                    const char* oldName = singletonArray->entries[i].name;
                    if (oldName) {
                        // NOTE: Original code calls UnregisterSingleton here,
                        // which returns void — possible cast error in original
                        // decompilation.  Left as-is for now.
                        char* newName = static_cast<char*>(
                            reinterpret_cast<void*>(
                                const_cast<char*>(oldName)));
                        entries[i].name = newName;
                    }

                    // Copy hash and verify singleton
                    uint32_t hash = singletonArray->entries[i].hash;
                    if (hash) {
                        atSingleton* instance = FindSingleton(hash);
                        entries[i].instance = instance;
                    }

                    entries[i].hash = singletonArray->entries[i].hash;
                    entries[i].refCount = singletonArray->entries[i].refCount;
                }
            }

            // Free old array
            if (singletonArray->entries) {
                rage_dealloc(singletonArray->entries, 3);
            }

            // Update array pointer and capacity
            singletonArray->entries = entries;
            singletonArray->capacity = newCapacity;
        }
    }

    // Add new entry slot at end
    singletonArray->count++;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  ReleaseSingleton
//
//  @ 0x821A9420 | size: 0x7C (124 bytes)
//
//  Releases a singleton and calls observer destructors.
//  Calls xmlTree_Cleanup on the singleton, then iterates through the
//  observer list calling virtual destructors (vtable slot 0) with
//  the scalar-delete flag.
// ═══════════════════════════════════════════════════════════════════════════════
void ReleaseSingleton(atSingleton* singleton)
{
    if (!singleton)
        return;

    // Clean up internal resource (rage::xmlTree)
    xmlTree_Cleanup(singleton);

    // Iterate through observers and call their destructors
    SingletonArray* observers = singleton->m_pObservers;
    if (observers) {
        while (observers->entries != nullptr) {
            atSingleton* observer = observers->entries[0].instance;
            if (observer) {
                // Call virtual destructor (vtable slot 0) with scalar-delete
                typedef void (*DestructorFn)(atSingleton*, uint32_t);
                void** vtable = *reinterpret_cast<void***>(observer);
                DestructorFn dtor = reinterpret_cast<DestructorFn>(vtable[0]);
                dtor(observer, 1);
            }

            if (!observers->entries)
                break;
        }
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  AcquireReference
//
//  @ 0x8222AB48 | size: 0x1F8
//
//  Increments the reference count on a singleton.
// ═══════════════════════════════════════════════════════════════════════════════
void AcquireReference(atSingleton* singleton)
{
    if (!singleton)
        return;

    singleton->m_refCount++;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  PopulateEntries  (was: BindObject — name corrected from scaffold)
//
//  @ 0x8224B3F8 | size: 0xFC (252 bytes)
//  Original symbol: atSingleton_B3F8_g
//
//  Batch-imports entries from a source config object into the entry array.
//  NOT a simple bind-by-hash — it reads count from source+40, iterates
//  calling vtable slot 1 on self to create entries, then populates each
//  entry with byte/half-word data from the source items at +36.
//
//  Dependencies: atSingleton_AC28_g, atSingleton_DDE8_g, atSingleton_DE98_g
//
//  TODO: Implement — requires resolving 3 sub-functions:
//    atSingleton_AC28_g  — pre-population setup
//    atSingleton_DDE8_g  — entry copy with existing count
//    atSingleton_DE98_g  — entry array growth/append
// ═══════════════════════════════════════════════════════════════════════════════
void PopulateEntries(atSingleton* singleton, void* sourceConfig)
{
    if (!singleton)
        return;

    // TODO: Read count from sourceConfig+40, iterate and populate entries
    // via vtable slot 1 dispatch on self
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Destroy  (was: UnbindObject — name corrected from scaffold)
//
//  @ 0x8218B410 | size: 0x2E0 (736 bytes)
//  Original symbol: atSingleton_B410_h
//
//  Large destructor function.  Sets 3 vtable pointers at +0, +4, +8,
//  then destroys objects at 10+ field offsets via virtual destructors
//  (vtable slot 0, flags=1).  Iterates array at +6164 (count at +6168)
//  destroying each entry.  Loops over 4 entries at +6148 calling
//  FindSingleton + TLS allocator vtable slot 2.  Frees memory at +6128
//  via rage_free.  Sets final vtable to rage::datBase (0x820276C4) at
//  +6236.
//
//  Object fields destroyed (in order):
//    +452, +6132, +6208 → virtual dtor(1)
//    +6164[0..count] → virtual dtor(1) per entry, then null
//    +6172, +492, +496, +500 → virtual dtor(1)
//    +6128 → rage_free
//    +6140, +6144 → virtual dtor(1)
//    +6148[0..3] → FindSingleton + TLS alloc vtable slot 2
//    +6236 = rage::datBase vtable
//
//  TODO: Implement — pattern is clear (repeated null-check + vdtor call)
//  but 736 bytes is large and has specific field offset dependencies.
// ═══════════════════════════════════════════════════════════════════════════════
void Destroy(void* self)
{
    // TODO: Implement large destructor (736 bytes)
    // Pattern: for each field at specific offset:
    //   if (field != nullptr) { field->vdtor(1); }
    //   field = nullptr;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  InitializeNetSystem
//
//  @ 0x8234B618 | size: 0x34C (844 bytes)
//  Original symbol: atSingleton_B618_h
//
//  Initializes the network singleton system.  Largest function in this
//  file — needs its own dedicated scaffold analysis session.
//
//  TODO: Implement (844 bytes — needs independent scaffold analysis).
// ═══════════════════════════════════════════════════════════════════════════════
void InitializeNetSystem()
{
    // TODO: Network system initialization (844 bytes)
}


// ═══════════════════════════════════════════════════════════════════════════════
//  NotifyObservers
//
//  @ 0x8225E998 | size: 0xD0
//
//  Broadcasts a singleton event to all registered observers.
// ═══════════════════════════════════════════════════════════════════════════════
void NotifyObservers(atSingleton* singleton, uint32_t eventType)
{
    if (!singleton)
        return;

    singleton->NotifyObservers(eventType);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  ClearPendingFlag
//
//  @ 0x8231F728 | size: 0x25C
//
//  Clears bit 0 (pending flag) of the singleton's state flags.
// ═══════════════════════════════════════════════════════════════════════════════
void ClearPendingFlag(atSingleton* singleton)
{
    if (!singleton)
        return;

    singleton->m_flags &= ~0x1;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  CallVirtualDestructor
//
//  @ 0x821E7BC0 | size: 0x0C
//
//  Dispatches vtable slot 0 (virtual destructor) on the given object.
// ═══════════════════════════════════════════════════════════════════════════════
void CallVirtualDestructor(void* obj)
{
    if (!obj)
        return;

    void** vtable = *reinterpret_cast<void***>(obj);
    typedef void (*VirtualFn)(void*);
    VirtualFn fn = reinterpret_cast<VirtualFn>(vtable[0]);
    fn(obj);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  GetFactory
//
//  @ 0x822E2E60 | size: 0x1E0 (480 bytes)
//  Original symbol: atFactory_GetFactory
//  Also lifted in: stubs.cpp, mc_memcard.cpp
//
//  Builds a path string from the input and looks up a factory in the
//  global factory registry at lbl_825D0080.
//
//  Path resolution:
//    - First byte '$': skip 2 bytes, use remainder as path
//    - First byte '/' or '\': path is relative — prefix with
//      factory name from registry (lbl_825D0080 + 384 + counter*128)
//    - Otherwise: no path manipulation needed
//
//  After building the path into a 256-byte stack buffer:
//    1. Calls NormalizeDirPath(path, 128) — factory lookup by path
//    2. Increments the factory counter at lbl_825D0080 + 1536
//
//  Dependencies: ph_21B0 (strchr-like), NormalizeDirPath (factory lookup)
//
//  TODO: Implement — path building is clear from scaffold, needs
//  NormalizeDirPath (factory lookup) resolution.
// ═══════════════════════════════════════════════════════════════════════════════
SingletonFactory* GetFactory(uint32_t hash)
{
    // TODO: Implement path-based factory lookup
    // See scaffold analysis above for full algorithm
    return nullptr;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  BindObject / UnbindObject  (header-declared shims)
//
//  The atSingleton.hpp header declares these two symbols with signatures
//  (atSingleton*, uint32_t) and (uint32_t) respectively, mapping them to
//  @ 0x8224B3F8 and @ 0x8218B410.  Scaffold analysis shows those addresses
//  are actually PopulateEntries (vtable-slot-1 dispatch batch import) and
//  a large pongPlayer destructor — neither matches a simple bind/unbind by
//  hash.  These shims preserve header compatibility; full implementation
//  requires resolving `atSingleton_AC28_g / _DDE8_g / _DE98_g` (PopulateEntries
//  deps, all unlifted) and the pongPlayer struct definition with its 17+
//  destructor-chain field offsets (Destroy deps).  See TODO notes above.
// ═══════════════════════════════════════════════════════════════════════════════
void BindObject(atSingleton* singleton, uint32_t /*hash*/)
{
    // Forwarded to PopulateEntries when sourceConfig plumbing is available.
    PopulateEntries(singleton, nullptr);
}

void UnbindObject(uint32_t /*hash*/)
{
    // Forwarded to Destroy — caller must resolve the target via FindSingleton
    // once the pongPlayer destructor pattern is fully lifted.
}

} // namespace rage


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_ScrambleData
//
//  @ 0x82410828 | size: 0xD4 (212 bytes)
//  Original symbol: atSingleton_0828_p46
//
//  Data transformation function using magic-constant fast integer division.
//  Processes each byte using an accumulator (step = 77) and three divisors
//  (÷13, ÷5, ÷255), incorporating neighbor bytes for a predictive/delta
//  encoding scheme.
//
//  Algorithm per byte[i]:
//    div13 = accum / 13;  mod13 = accum % 13
//    div5  = i / 5
//    result = (div5 - mod13) + byte[i] + 61
//    if (i > 0):       result += byte[i-1]
//    if (i < len-1):   result -= (byte[i+1] + sign_round) / 2
//    result += result / 255  (rounding adjustment)
//    byte[i] = result
//    accum += 77
// ═══════════════════════════════════════════════════════════════════════════════
void atSingleton_ScrambleData(int8_t* data, int length)
{
    if (length <= 0)
        return;

    int lastIndex = length - 1;
    int accumulator = 0;

    for (int i = 0; i < length; i++) {
        // Fast division: accumulator / 13 (magic constant: 0x4EC4EC4F)
        int32_t div13 = static_cast<int32_t>(
            (static_cast<int64_t>(accumulator) * 0x4EC4EC4FL) >> 32);
        div13 = (div13 >> 2) + ((div13 >> 31) & 1);

        // Fast division: i / 5 (magic constant: 0xCCCCCCCD)
        uint32_t div5 = static_cast<uint32_t>(
            (static_cast<uint64_t>(static_cast<uint32_t>(i)) * 0xCCCCCCCDULL) >> 32);
        div5 >>= 2;

        // Modulo: accumulator % 13
        int32_t mod13 = accumulator - (div13 * 13);

        // Base calculation
        int32_t result = static_cast<int32_t>(div5) - mod13 + data[i] + 61;

        // Add previous byte contribution
        if (i > 0) {
            result += data[i - 1];
        }

        // Subtract half of next byte (with sign-aware rounding)
        if (i < lastIndex) {
            int8_t nextByte = data[i + 1];
            int32_t halfNext = (nextByte >> 1);
            // addze: add carry from arithmetic shift
            if (nextByte < 0 && (nextByte & 1))
                halfNext++;
            result -= halfNext;
        }

        // Final scaling: result += result / 255 (rounding adjustment)
        // Magic constant: 0x80808081 for signed multiply-high ÷ 255
        int32_t scaleHigh = static_cast<int32_t>(
            (static_cast<int64_t>(result) * static_cast<int64_t>(
                static_cast<int32_t>(0x80808081))) >> 32);
        scaleHigh += result;
        int32_t scaled = (scaleHigh >> 7) + ((scaleHigh >> 31) & 1);
        result += scaled;

        data[i] = static_cast<int8_t>(result);

        accumulator += 77;
    }
}



// ═══════════════════════════════════════════════════════════════════════════════
//  Batch: atSingleton lifecycle/teardown helpers (10 functions)
//  Session: 2026-04-14
// ═══════════════════════════════════════════════════════════════════════════════

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// Domain types shared by the batch below
// ─────────────────────────────────────────────────────────────────────────────

// Primary TLS/global allocator base, loaded off r13+0 (SDA @ 0x82600000).
// The virtual dispatch table has a "free(ptr)" handler at byte offset +8
// (vtable slot 2), so an allocator is modelled as:
//   struct { struct VTable { ... void (*free)(Allocator*, void*); ... }* vt; };
struct sysMemAllocator
{
    struct VTable
    {
        void (*dtor)(sysMemAllocator*);                  // slot 0 (+0)
        void* (*allocate)(sysMemAllocator*, uint32_t);   // slot 1 (+4)
        void (*free)(sysMemAllocator*, void*);           // slot 2 (+8) — the used one
    };
    const VTable* vtable;
};

// Accessor for the per-thread "current" allocator.  On the guest this comes
// from r13+0 → *(void**)0x82600000.  On the host the SDA global must be
// provided by the runtime glue; for lifted code we expose it as a function.
extern sysMemAllocator* sysMemGetCurrentAllocator();

// Primary refcounted object — fields actually touched by this batch:
//   +0x20 uint32_t m_nRefCount;
// Each element goes through grc_12B8(elem, 1) when refcount hits zero.
struct sysRefCountedResource
{
    // padding up to +0x20
    uint8_t _pad0[0x20];
    uint32_t m_nRefCount;   // +0x20
};

// Generic free primitive used by the element-array loops (was rage_2E18).
// Scaffold shows it as a simple free-wrapper taking a single pointer.
extern "C" void rage_free_2E18(void* ptr);

// Refcount-zero release hook (was grc_12B8) — `flags` is the scalar/array
// delete flag, always 1 in this batch.
extern "C" void grcResource_Release(sysRefCountedResource* res, uint32_t flags);

// ─────────────────────────────────────────────────────────────────────────────
//  atContainer — composite object serviced by atSingleton_9F08_w.
//  Layout is derived directly from the scaffold body: three parallel arrays
//  (A and B), plus four owned side-blocks that are freed via the global
//  allocator when not tracked in the singleton registry.
// ─────────────────────────────────────────────────────────────────────────────
struct atContainer
{
    // --- Array A: parallel pair of element ptr tables ---
    uint32_t* m_pElementsA;     // +0x00  (was vtable-annotated; actually data)
    uint16_t  m_nCountA;        // +0x04
    uint16_t  m_nCapacityA;     // +0x06
    uint32_t* m_pElementsB;     // +0x08  — refcounted-element table base
    uint16_t  m_nCountB;        // +0x0C
    uint16_t  m_nCapacityB;     // +0x0E
    uint32_t* m_pElementsA2;    // +0x10  — second parallel slice of A
    uint16_t  m_nCountA2;       // +0x14
    uint16_t  m_nCapacityA2;    // +0x16
    uint32_t* m_pElementsB2;    // +0x18  — second parallel slice of B
    uint16_t  m_nCountB2;       // +0x1C
    uint16_t  m_nCapacityB2;    // +0x1E
    void*     m_pExtraBlock0;   // +0x20  (released-via-allocator if untracked)
    uint16_t  m_nExtraCount;    // +0x24
    uint16_t  m_nExtraCapacity; // +0x26
    uint8_t   _pad28[0x170 - 0x28];
    void*     m_pLateBlock;     // +0x170 — final released-via-allocator block
};

// ═══════════════════════════════════════════════════════════════════════════════
//  atContainer::Teardown  @ 0x82409F08 | size: 0x2FC (764 bytes)
//  Original: atSingleton_9F08_w
//  Caller  : rage_9490 (single caller — a standalone teardown helper)
//  VCALLs  : 7× allocator->free(ptr)  — all resolve to sysMemAllocator::vtable[2]
//
//  Phases:
//   (1) For each i in [0, m_nCountA):
//         free m_pElementsA[i]  (non-null only)
//         free m_pElementsA2[i] (non-null only)
//   (2) For each i in [0, m_nCountB):
//         decrement m_pElementsB[i]->m_nRefCount  (and release on zero)
//         decrement m_pElementsB2[i]->m_nRefCount (and release on zero)
//   (3) For each of the 6 side-block fields
//         (m_pElementsA, m_pElementsA2, m_pElementsB, m_pElementsB2,
//          m_pExtraBlock0, m_pLateBlock):
//         if the block is NOT a tracked singleton (low byte of FindSingleton
//         return is zero), call allocator->free(block).  Then clear the
//         block pointer and both size halfwords.
// ═══════════════════════════════════════════════════════════════════════════════
static inline void atContainer_ReleaseSideBlock(
    void** blockField,
    uint16_t* countField,
    uint16_t* capacityField)
{
    void* block = *blockField;
    if (block != nullptr)
    {
        atSingleton* tracked = FindSingleton(
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block)));
        // FindSingleton returns nullptr when not in the registry; the scaffold
        // masks the low byte of the return, so "tracked" is true only when the
        // low byte is non-zero (i.e. the call yielded a real singleton with a
        // non-zero entry byte).  Untracked blocks are owned → free them.
        bool isTracked = (reinterpret_cast<uintptr_t>(tracked) & 0xFF) != 0;
        if (!isTracked)
        {
            sysMemAllocator* alloc = sysMemGetCurrentAllocator();
            alloc->vtable->free(alloc, block);
        }
    }
    *blockField = nullptr;
    *countField = 0;
    *capacityField = 0;
}

void atContainer_Teardown(atContainer* self)
{
    if (!self)
        return;

    // Phase 1: free non-null elements of parallel array A.
    for (int32_t i = 0; i < static_cast<int32_t>(self->m_nCountA); i++)
    {
        if (self->m_pElementsA && self->m_pElementsA[i] != 0)
            rage_free_2E18(reinterpret_cast<void*>(self->m_pElementsA[i]));
        if (self->m_pElementsA2 && self->m_pElementsA2[i] != 0)
            rage_free_2E18(reinterpret_cast<void*>(self->m_pElementsA2[i]));
    }

    // Phase 2: refcount-decrement + release elements of parallel array B.
    for (int32_t i = 0; i < static_cast<int32_t>(self->m_nCountB); i++)
    {
        if (self->m_pElementsB)
        {
            auto* elem = reinterpret_cast<sysRefCountedResource*>(
                self->m_pElementsB[i]);
            if (elem != nullptr)
            {
                uint32_t newRef = elem->m_nRefCount - 1;
                elem->m_nRefCount = newRef;
                if (newRef == 0)
                    grcResource_Release(elem, 1);
            }
        }
        if (self->m_pElementsB2)
        {
            auto* elem = reinterpret_cast<sysRefCountedResource*>(
                self->m_pElementsB2[i]);
            if (elem != nullptr)
            {
                uint32_t newRef = elem->m_nRefCount - 1;
                elem->m_nRefCount = newRef;
                if (newRef == 0)
                    grcResource_Release(elem, 1);
            }
        }
    }

    // Phase 3: release the six owned side-blocks in scaffold order.
    atContainer_ReleaseSideBlock(reinterpret_cast<void**>(&self->m_pElementsA),
                                 &self->m_nCountA,  &self->m_nCapacityA);
    atContainer_ReleaseSideBlock(reinterpret_cast<void**>(&self->m_pElementsA2),
                                 &self->m_nCountA2, &self->m_nCapacityA2);
    atContainer_ReleaseSideBlock(reinterpret_cast<void**>(&self->m_pElementsB),
                                 &self->m_nCountB,  &self->m_nCapacityB);
    atContainer_ReleaseSideBlock(reinterpret_cast<void**>(&self->m_pElementsB2),
                                 &self->m_nCountB2, &self->m_nCapacityB2);
    atContainer_ReleaseSideBlock(&self->m_pExtraBlock0,
                                 &self->m_nExtraCount, &self->m_nExtraCapacity);
    // m_pLateBlock has no companion size fields in the scaffold tail — it is
    // released and cleared on its own.  The surrounding count/capacity words
    // are untouched here because the function ran off the end of the scaffold
    // before any final zero-stores landed on +0x174/+0x176.
    if (self->m_pLateBlock != nullptr)
    {
        atSingleton* tracked = FindSingleton(
            static_cast<uint32_t>(
                reinterpret_cast<uintptr_t>(self->m_pLateBlock)));
        bool isTracked = (reinterpret_cast<uintptr_t>(tracked) & 0xFF) != 0;
        if (!isTracked)
        {
            sysMemAllocator* alloc = sysMemGetCurrentAllocator();
            alloc->vtable->free(alloc, self->m_pLateBlock);
        }
    }
    self->m_pLateBlock = nullptr;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_vfn_0_A4C0_1  @ 0x821CA4C0 | size: 0x6C (108 bytes)
//
//  Two-stage virtual destructor:
//    1. Patch vtable pointer to the "base" vtable (lis -32252, addi -5600)
//       so the parent destructor runs against the clean base layout.
//    2. Call base destructor `rage_8070(this+4)` on the embedded member.
//    3. Patch vtable pointer to the derived vtable tail (lis -32254, addi 30404).
//    4. If the low bit of the deletion-flag argument is set, call the RAGE
//       heap free routine on `this`.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void rage_baseDtor_8070(void* self);  // @ 0x821A8070
extern "C" void rage_free_C0(void* self);        // @ 0x820C00C0
extern void* const k_vtableBaseA4C0;
extern void* const k_vtableDerivedA4C0;

void atSingleton_vfn_0_A4C0_1(atSingleton* self, uint32_t deleteFlags)
{
    if (!self)
        return;

    // Stage 1+2: retarget to base vtable, run base destructor on sub-object.
    *reinterpret_cast<const void**>(self) = k_vtableBaseA4C0;
    rage_baseDtor_8070(reinterpret_cast<uint8_t*>(self) + 4);

    // Stage 3: retarget to derived vtable tail.
    *reinterpret_cast<const void**>(self) = k_vtableDerivedA4C0;

    // Stage 4: scalar delete if requested.
    if ((deleteFlags & 0x1) != 0)
        rage_free_C0(self);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_vfn_9_8C18_1  @ 0x82368C18 | size: 0xAC (172 bytes)
//
//  Gated reset of a 64-entry, 12-byte-stride table at `this + 8`:
//    - Reads flags word at +0x308 (this+776).
//    - If bit 28 (mask 0x10000000) is clear → return error code 0x80004005
//      (E_FAIL / S_FALSE-style guest HRESULT).
//    - Otherwise: clear bit 28 in the flags, call atSingleton_8210_g(this, 0,
//      0, 0, 0, 0, 0) to notify listeners of the wipe, then zero the first
//      dword of each of the 64 × 12-byte entries at this+8.
//    - Reload flags: if bit 31 (0x80000000) or bit 29 (0x20000000) is set,
//      return 0x8000700E (composite error).  Otherwise, return 0 (S_OK).
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void atSingleton_8210_g(atSingleton* self,
                                   uint32_t a0, uint32_t a1, uint32_t a2,
                                   uint32_t a3, uint32_t a4, uint32_t a5);

uint32_t atSingleton_vfn_9_8C18_1(atSingleton* self)
{
    if (!self)
        return 0x80004005u;

    uint32_t* flagsField = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 776);
    uint32_t flags = *flagsField;

    if ((flags & 0x10000000u) == 0)
        return 0x80004005u;

    // Clear bit 28, persist, then wipe table + notify.
    flags &= ~0x10000000u;
    *flagsField = flags;

    atSingleton_8210_g(self, 0, 0, 0, 0, 0, 0);

    uint32_t* entry = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 8);
    for (int32_t i = 0; i < 64; i++)
    {
        *entry = 0;
        entry = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(entry) + 12);
    }

    uint32_t reloaded = *flagsField;
    if ((reloaded & 0x80000000u) != 0 || (reloaded & 0x20000000u) != 0)
        return 0x8000700Eu;

    return 0;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_66E8_h  @ 0x823B66E8 | size: 0xFC (252 bytes)
//  vtable slot 5 on SpectatorBallHitMessage — validates three message fields
//  against the current match/session state.
//
//  TODO: Requires SinglesNetworkClient layout — specifically the field at +464
//  (match index) and the 416-byte 'NetworkMatch' record at the class-local
//  base lbl_825F6DD0 + 48.  Implement after those layouts are enumerated.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_2778_g  @ 0x82212778 | size: 0xFC (252 bytes)
//  A factory/register pair: duplicates two name strings into 'this', stores
//  two opaque object ptrs at +12/+16, dispatches vfn_2 then vfn_1 on self,
//  looks up an atFactory via lbl_825D0080 + 128, formats a path via snprintf
//  into a 256-byte stack buffer, calls vfn_1 on (this + 20) with that path,
//  decrements the factory-counter at lbl_825D0080 + 1536, and tail-calls
//  vfn_3 on self.  Original uses the "%s/%s" template at
//  lbl_82XX (const char*: offset -3596 from lbl base).
//
//  TODO: The call pattern is clear but the concrete atSingleton subclass
//  layout for +12/+16/+20 and its vfn_1/vfn_2/vfn_3 signatures is class-
//  specific and varies per instantiation (1341 candidate atSingleton derivs
//  remain).  Leaving unlifted pending a concrete derived-class identification.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_9A08_h  @ 0x82289A08 | size: 0x104 (260 bytes)
//  Resets a managed state slot at `g_atSingletonCtx->m_pSlotBase + 9736`:
//    - Zeroes each of m_nCountA entries in array at +0 (stride 4).
//    - Stores float(1.0f) at +8.
//    - Calls atSingleton_9B10(slot) to publish the reset.
//    - If bit 0 of *vtable (read back from +0) is CLEAR → calls
//      atSingleton_9E90_h(1) to seed follow-up state.
//    - Reload vtable at +0.  If bit 0 of *vtable is STILL CLEAR → emits a
//      trace via nop_8240E6D0(const char* "<trace template>"), clears bit 0
//      of *vtable on both the derived-vtable word and the outer vtable word
//      (via not/xor mask dance), and stamps float(<k_resetFloat>) into +8.
//
//  TODO: Requires concrete derived-class identification — the "+20" path is
//  a vtable@+0 bitfield re-written in-place, which is a known pongBall-style
//  pattern but the ownership is ambiguous without its RTTI class.  Leaving
//  unlifted pending class resolution.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_12D0_g  @ 0x821E12D0 | size: 0x174 (372 bytes)
//  Cold-path initialiser for an atSingleton instance stored at lbl_826064A0.
//    1. Calls xe_main_thread_init_0038() for one-time TLS setup.
//    2. allocator->allocate(188) via vtable slot 1 — 188-byte atSingleton.
//    3. If allocation succeeded, calls atSingleton_CAD0_g(newInstance) to
//       run the 188-byte object's constructor.
//    4. Writes derived-class vtable (lis -32252, addi 5352) to new+0.
//    5. Copies two 16-byte config blocks (from stack locals +96 and +112)
//       into new+28 and new+44.
//    6. Patches two static-globals at lbl_82XX+0 and lbl_82XX+4 with sizes
//       4 and 12 (table-descriptor init).
//    7. Zeros the instance pointer at new+8, promotes the halfword at +26
//       into the dword at +24, clears the +26 halfword.
//    8. Calls atSingleton_CEF0_g(newInstance) for late-init.
//    9. Registers newInstance with atSingleton_7550_g(trackerHandle,
//       vtable->[0+4], &stackLocal(newInstance)).
//
//  TODO: The full layout of the 188-byte derived atSingleton (which config
//  structs live at +28/+44, what the +24/+26 promotion signifies) is
//  class-specific.  Leaving unlifted until concrete class ID resolved.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_0BA0_g  @ 0x821D0BA0 | size: 0x2D8 (728 bytes)
//  Blend-target population routine:
//    - Resolves atFactory for "gdBlendTargets" via atFactory_GetFactory.
//    - For each blend-target in the source config, calls
//      atSingleton_8588_g (InitializeSingleton) to grow the array, then
//      dispatches vfn[gdBTgtBoneName], vfn[gdBlendTargetName],
//      vfn[gdBlendTargets] on each element.
//    - Registers the singleton via FindSingleton+AcquireReference path
//      through atSingleton_29E0_g + atSingleton_9420.
//
//  TODO: Three VCALL sites, each targeting a different concrete gdBlend*
//  vtable — needs per-class method-signature resolution.  The overall
//  flow is clear but a correct lift requires declaring all three gdBlend*
//  classes' vfn_1 prototypes.  Leaving unlifted pending header plumbing.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_A1D0_p42  @ 0x8250A1D0 | size: 0x634 (1588 bytes)
//  atSingleton_7350     @ 0x82517350 | size: 0x1154 (4436 bytes)
//
//  Two very large per-module init pipelines (match/audio subsystem bring-up).
//  Both lift 20+ distinct subsystem-init callees with class-specific vtables
//  and can only be correctly lifted once each callee's signature is known.
//  Estimated as multi-session work; listed here to complete the batch-10
//  audit trail.
//
//  TODO: Lift atSingleton_A1D0_p42 and atSingleton_7350 individually — each
//  deserves its own dedicated scaffold-review pass.
// ═══════════════════════════════════════════════════════════════════════════════
// (intentionally left unlifted — see TODO above)



// ═══════════════════════════════════════════════════════════════════════════════
//  Batch 10 lift — string-hashed registry + vtable trampoline helpers.
//  All ten functions operate on `rage::atSingleton`-derived objects that
//  consult the global string-keyed registry @ 0x82607A5C (chained hash map)
//  or iterate allocator-owned pointer arrays and free non-tracked entries
//  via the primary TLS allocator's vtable slot 2.
// ═══════════════════════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────────────────────
//  External helpers referenced by the batch-10 bodies
// ─────────────────────────────────────────────────────────────────────────────

// The chained string-key registry used by atSingleton_5000 / 5138.
//   [0] uint32_t*  entries      — hash bucket array
//   [4] uint16_t   bucketCount  — number of buckets
//   [6] uint16_t   itemCount    — total stored items
struct atStringHashRegistry {
    uint32_t* entries;
    uint16_t  bucketCount;
    uint16_t  itemCount;
};
extern atStringHashRegistry g_stringHashRegistry;  // @ 0x82607A5C

// Primary TLS/global allocator base, loaded off r13+0 (SDA @ 0x82600000).
extern void** g_tlsAllocatorBase;

// String-hash (ELF-style, 4-bit step) — identical body to atStringHash
// already declared in memory.cpp.  @ 0x82165338 | size: 0x50
extern "C" uint32_t atStringHash_5338(const char* str);

// Internal char-hash used by atSingleton_5000 @ 0x820F5000.
// Forwarded to atStringHash_5338 via vtable+0 of the bucket object.
// ─────────────────────────────────────────────────────────────────────────────

// atSingleton_DB60 — already lifted in rage/memory.cpp as datArray_DestroyWithUnbind.
// @ 0x8212DB60 | size: 0xB4
extern "C" void datArray_DestroyWithUnbind_DB60(void* array, int32_t mode);

// atSingleton_5F48_2h — first-child accessor for the per-node tree walked by 5878.
// @ 0x82125F48 | size: 0x74
extern "C" void* atSingleton_5F48_GetNextSibling(void* self, void* node);

// atSingleton_5DC0_2hr — detach-child helper called by atSingleton_5878.
// @ 0x82125DC0 | size: 0x184
extern "C" void atSingleton_5DC0_DetachChild(void* self, void* node);

// atSingleton_B848 + SinglesNetworkClient_A508_h — guarded init path used by rtti_92D4.
// @ 0x8219B848 / 0x8219A508
extern "C" void atSingleton_B848_NetBringUp(void* self, void* ctx);
extern "C" void SinglesNetworkClient_A508_h(void* self, void* ctx);

// Loop / match globals used by the rtti_92D4_1 guard.
//   g_loop_obj_ptr       @ 0x825EAB30 (16 bytes header: +0x10 = u32 phase counter)
//   g_input_obj_ptr      @ 0x825EAB28 (points at a per-input status block,
//                                      byte +576 is a "shutdown" sentinel)
extern uint8_t* g_loop_obj_ptr;
extern uint8_t* g_input_obj_ptr;

// datArray_Grow-style allocator (xe_EC88 @ 0x820DEC88 — TLS stack allocator).
extern "C" void* xe_Alloc_EC88(uint32_t size);

// ─────────────────────────────────────────────────────────────────────────────
//  Helpers: free-if-untracked, walk-to-rightmost
// ─────────────────────────────────────────────────────────────────────────────

// Matches the identical PPC idiom found in every batch-10 destructor:
//
//    if (p && !FindSingleton((uint32_t)p))
//        ((*g_tlsAllocatorBase).vtable_slot_2)(g_tlsAllocatorBase, p);
//
// The low byte of FindSingleton's return indicates "is this a tracked
// registry entry"; only non-tracked entries are freed through the
// allocator.  This helper collapses that 12-instruction sequence used
// by atSingleton_dtor_3468, 5878, 9370_g and A680_g.
static inline void FreeIfUntracked(void* ptr)
{
    if (ptr == nullptr)
        return;

    // FindSingleton returns nonzero in low byte when ptr IS tracked.
    const uint8_t tracked = static_cast<uint8_t>(
        reinterpret_cast<uintptr_t>(FindSingleton(
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr)))) & 0xFF);
    if (tracked != 0)
        return;

    using AllocFree_t = void(*)(void* self, void* p);
    void* allocator = g_tlsAllocatorBase[0];
    void** vtable   = *reinterpret_cast<void***>(allocator);
    AllocFree_t slot2 = reinterpret_cast<AllocFree_t>(vtable[2]);
    slot2(allocator, ptr);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_5000 — LookupByName
//
//  @ 0x820F5000 | size: 0x134 (308 bytes)
//
//  Looks up `key` in the string-keyed global registry @ 0x82607A5C.
//
//    1. h = atStringHash_5338(key->name) mod bucketCount
//    2. Walk the bucket chain (entries[h] -> node.next @ +8)
//    3. Compare key->name (key->vtable[0] slot) with node->name via strcmp
//    4. On hit: unlink the node, free both node->firstPayload (slot 0) and
//       the node itself via TLS allocator slot 0 (rage_free_00C0),
//       decrement itemCount, return 1.
//    5. On miss: return 0.
//
//  This is a classic open-chained hash-map remove-by-string.  Note that
//  the generated pseudocode `return -2107860304` is an IDA misfire (the
//  literal is 0x820F5230 — the function's own epilogue instruction).
// ═══════════════════════════════════════════════════════════════════════════════
struct StringHashNode {
    const char*     name;        // +0
    uint32_t        pad;         // +4
    StringHashNode* next;        // +8
    // ... rest of payload
};

extern "C" int32_t atSingleton_LookupByName_5000(void* /*unused*/, void* key)
{
    atStringHashRegistry* reg = &g_stringHashRegistry;
    const uint16_t bucketCount = reg->bucketCount;
    if (bucketCount == 0)
        return 0;

    // key->vtable[slot 0] actually stores the string pointer directly;
    // the scaffold loads it via `lwz r6,0(r4)` — i.e. key+0 is `const char*`.
    const char* keyName =
        *reinterpret_cast<const char* const*>(key);

    const uint32_t h = atStringHash_5338(keyName);
    const uint32_t idx = h % bucketCount;

    // bucket slot = entries + idx*4
    StringHashNode** slot = reinterpret_cast<StringHashNode**>(
        &reg->entries[idx]);
    StringHashNode* node = *slot;
    if (node == nullptr)
        return 0;

    // Walk chain comparing name strings.
    while (node != nullptr) {
        const char* nodeName =
            *reinterpret_cast<const char* const*>(node);

        bool equal;
        if (keyName == nodeName) {
            equal = true;
        } else if (keyName == nullptr || nodeName == nullptr) {
            equal = false;
        } else {
            // Inline strcmp (scaffold unrolls it byte-by-byte).
            const char* a = keyName;
            const char* b = nodeName;
            while (*a != '\0' && *a == *b) { ++a; ++b; }
            equal = (*a == *b);
        }

        if (equal) {
            // Unlink node from its bucket, free payload and node.
            StringHashNode* victim = node;
            *slot = victim->next;

            // victim->firstPayload lives at victim+0 — but slot 0 of the node
            // IS the name pointer itself, so the scaffold reads `lwz r3,0(r31)`
            // and frees that block first, then frees the node.
            void* payload = *reinterpret_cast<void**>(victim);
            if (payload != nullptr) {
                rage_dealloc(payload, 3);
            }
            rage_dealloc(victim, 3);

            --reg->itemCount;
            return 1;
        }

        slot = &node->next;
        node = node->next;
    }
    return 0;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_5138_g — RehashRegistry
//
//  @ 0x820F5138 | size: 0x128 (296 bytes)
//
//  Resizes the global string-key registry @ 0x82607A5C to `newBucketCount`:
//    1. Calls TLS-alloc vtable slot 1 for (newBucketCount * 4) bytes.
//    2. Zero-fills the new bucket array.
//    3. Re-inserts every existing node into its newly-hashed bucket.
//    4. Frees the old bucket array (untracked-aware path via FindSingleton).
//    5. Stores the new capacity and new base pointer.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void atSingleton_RehashRegistry_5138(void* /*context*/,
                                                uint32_t newBucketCountArg)
{
    // xe_main_thread_init_0038 — startup barrier (scaffold `bl 0x820c0038`).
    extern "C" void xe_main_thread_init_0038(void);
    xe_main_thread_init_0038();

    const uint32_t newBucketCount = newBucketCountArg & 0xFFFF;

    // allocator.vtable[slot 1](allocator, newBucketCount*4, 16) → new bucket array
    using AllocAlloc_t = void*(*)(void* self, uint32_t size, uint32_t align);
    void*  allocator = g_tlsAllocatorBase[0];
    void** vtable    = *reinterpret_cast<void***>(allocator);
    AllocAlloc_t slot1 = reinterpret_cast<AllocAlloc_t>(vtable[1]);

    uint32_t* newBuckets = static_cast<uint32_t*>(
        slot1(allocator, newBucketCount * 4u, 16u));

    // Zero-init.
    for (uint32_t i = 0; i < newBucketCount; ++i)
        newBuckets[i] = 0;

    atStringHashRegistry* reg = &g_stringHashRegistry;
    uint32_t* oldBuckets = reg->entries;

    if (reg->bucketCount != 0 && newBucketCount != 0) {
        const uint16_t oldBucketCount = reg->bucketCount;
        for (uint32_t bi = 0; bi < oldBucketCount; ++bi) {
            StringHashNode* cur = reinterpret_cast<StringHashNode*>(oldBuckets[bi]);
            while (cur != nullptr) {
                // Re-hash using node's name (stored at node+8 actually:
                // scaffold does `addi r4,r6,8; lwz r6,0(r4); bl 5338`).
                const char* nodeName =
                    *reinterpret_cast<const char* const*>(
                        reinterpret_cast<uint8_t*>(cur) + 8);
                const uint32_t h = atStringHash_5338(nodeName);
                const uint32_t newIdx = h % newBucketCount;

                // Push at head of its new bucket.
                uint32_t* headSlot = &newBuckets[newIdx];
                StringHashNode* newNext =
                    reinterpret_cast<StringHashNode*>(*headSlot);
                *reinterpret_cast<StringHashNode**>(
                    reinterpret_cast<uint8_t*>(cur) + 8) = newNext;
                *headSlot = reinterpret_cast<uint32_t>(cur);

                cur = newNext;
            }
        }
    }

    reg->bucketCount = static_cast<uint16_t>(newBucketCount);

    if (oldBuckets != nullptr) {
        FreeIfUntracked(oldBuckets);
    }
    reg->entries = newBuckets;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_5878 — TeardownSubtree
//
//  @ 0x82125878 | size: 0x12C (300 bytes)
//
//  Walks a binary-tree-style singleton container `self`:
//    – follows the right-child chain at node+56 until reaching the rightmost
//      leaf, then iteratively removes the rightmost leaf by calling
//      atSingleton_5F48_GetNextSibling + atSingleton_5DC0_DetachChild.
//    – If self->pool (self+8) is non-null, it re-parents the detached leaf
//      into the pool's free list (pool+16 is the free-list head; pool+8 is
//      the count).  Otherwise it falls back to freeing the leaf via the
//      untracked-aware path.
//    – Repeats until the tree is empty.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void atSingleton_TeardownSubtree_5878(void* selfArg)
{
    uint8_t* self = static_cast<uint8_t*>(selfArg);
    uint8_t* leaf =
        *reinterpret_cast<uint8_t**>(self + 0);   // tree root at self+0
    if (leaf == nullptr)
        return;

    // Descend to rightmost leaf.
    uint8_t* next = *reinterpret_cast<uint8_t**>(leaf + 56);
    while (next != nullptr) {
        leaf = next;
        next = *reinterpret_cast<uint8_t**>(next + 56);
    }
    if (leaf == nullptr)
        return;

    for (;;) {
        uint8_t* sibling = static_cast<uint8_t*>(
            atSingleton_5F48_GetNextSibling(self, leaf));
        atSingleton_5DC0_DetachChild(self, leaf);

        uint8_t* pool = *reinterpret_cast<uint8_t**>(self + 8);
        if (pool != nullptr) {
            // Detach leaf+48 (tracked payload) if untracked.
            uint8_t* payload = *reinterpret_cast<uint8_t**>(leaf + 48);
            FreeIfUntracked(payload);

            // Push leaf onto pool's free list (pool+16 = head, pool+8 = count).
            uint8_t* freeHead = *reinterpret_cast<uint8_t**>(pool + 16);
            *reinterpret_cast<uint8_t**>(leaf + 0) = freeHead;
            uint32_t count = *reinterpret_cast<uint32_t*>(pool + 8);
            *reinterpret_cast<uint8_t**>(pool + 16) = leaf;
            *reinterpret_cast<uint32_t*>(pool + 8) = count + 1;
        } else {
            if (leaf == nullptr)
                break;
            uint8_t* payload = *reinterpret_cast<uint8_t**>(leaf + 48);
            FreeIfUntracked(payload);
            rage_dealloc(leaf, 3);
        }

        leaf = sibling;
        if (sibling == nullptr)
            return;
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_9370_g — DestroyArrayStride16
//
//  @ 0x821A9370 | size: 0x8C (140 bytes)
//
//  Destroys a heap-allocated array of 16-byte records.  The array header
//  lives 4 bytes before `entries` (the count is stored as uint32_t at
//  entries-4).  For each non-null pointer at record+0, it frees the pointee
//  via the untracked-aware path; then frees the header itself.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void* atSingleton_DestroyArrayStride16_9370(void* entriesArg)
{
    uint8_t* entries = static_cast<uint8_t*>(entriesArg);
    uint8_t* header  = entries - 4;

    const uint32_t count = *reinterpret_cast<uint32_t*>(header);
    uint8_t* cursor = entries + (count * 16u);   // one-past-end
    int32_t remaining = static_cast<int32_t>(count) - 1;

    while (remaining >= 0) {
        cursor -= 16;
        uint8_t* item = *reinterpret_cast<uint8_t**>(cursor + 0);
        FreeIfUntracked(item);
        --remaining;
    }

    rage_dealloc(header, 3);
    return header;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_rtti_92D4_1 — GuardedNetworkInit
//
//  @ 0x8219C528 | size: 0x88 (136 bytes)
//
//  Conditional bring-up for the singles-network singleton:
//
//    if (g_loop_obj_ptr->field_14 <= 0 &&
//        g_input_obj_ptr->shutdown_byte[576] == 0) {
//      SinglesNetworkClient_A508_h(self, ctx);
//      atSingleton_B848_NetBringUp(self, ctx);
//    }
//
//  The guard skips the network path when the loop already has a positive
//  phase counter (match in progress) OR the input object has its shutdown
//  sentinel set — both cases mean we would race a concurrent teardown.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void atSingleton_GuardedNetworkInit_92D4(void* self, void* ctx)
{
    // g_loop_obj_ptr is an indirect: load pointer then field at +20.
    const int32_t loopPhase = *reinterpret_cast<int32_t*>(
        g_loop_obj_ptr + 20);
    const bool loopBusy = (loopPhase > 0);

    if (loopBusy)
        return;

    const uint8_t inputShutdown = g_input_obj_ptr[576];
    if (inputShutdown != 0)
        return;

    SinglesNetworkClient_A508_h(self, ctx);
    atSingleton_B848_NetBringUp(self, ctx);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_2990_2h — DispatchPreInitHook
//
//  @ 0x82132990 | size: 0xB0 (176 bytes)
//
//  Two-phase vtable trampoline used by the generic "pre-init" pipeline:
//
//    if (self->hasPreInit)  // byte at self+4
//    {
//      self->vtable[28](self);                     // PreHook
//      if (arg1 == nullptr) return;
//      self->vtable[37](self, arg1);               // ConfigureFromArg
//      // tail-call vtable[37] with a .rdata string constant (lbl_82027D04)
//      self->vtable[37](self, &lbl_82027D04);
//    }
//    else
//    {
//      if (arg1 == nullptr) return;
//      self->vtable_raw[11](self, arg1);           // Forward (byte +44 = slot 11)
//    }
// ═══════════════════════════════════════════════════════════════════════════════
extern const uint8_t lbl_82027D04;   // .rdata, 2 bytes — sentinel config key

using vslot28_t = void(*)(void* self);
using vslot37_t = void(*)(void* self, const void* arg);
using vslot11_t = void(*)(void* self, const void* arg);

extern "C" void atSingleton_DispatchPreInitHook_2990(void* selfArg,
                                                     const void* arg1)
{
    uint8_t* self = static_cast<uint8_t*>(selfArg);
    const uint8_t hasPreInit = self[4];
    void** vtable = *reinterpret_cast<void***>(self);

    if (hasPreInit != 0) {
        vslot28_t preHook = reinterpret_cast<vslot28_t>(vtable[28]);
        preHook(self);
        if (arg1 == nullptr)
            return;
        vslot37_t configure = reinterpret_cast<vslot37_t>(vtable[37]);
        configure(self, arg1);
        // Tail: configure(self, &lbl_82027D04)
        vtable = *reinterpret_cast<void***>(self);   // reload — vtable may rotate
        configure = reinterpret_cast<vslot37_t>(vtable[37]);
        configure(self, &lbl_82027D04);
        return;
    }

    if (arg1 == nullptr)
        return;

    // raw byte offset +44 = vtable index 11.
    vslot11_t forward = reinterpret_cast<vslot11_t>(vtable[11]);
    forward(self, arg1);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_2A40_2h — DispatchFullInitSequence
//
//  @ 0x82132A40 | size: 0xA8 (168 bytes)
//
//  Sibling of 2990: the "full" init sequence when no user-arg is supplied.
//
//    if (self->hasPreInit)
//    {
//      vtable[28](self);                                 // PreHook
//      vtable[37](self, &lbl_82027660);                  // Configure("")
//      vtable[29](self);                                 // PostHook
//      vtable[30](self, 1);                              // Finalize(true)
//    }
//    else
//    {
//      vtable_raw[11](self, &lbl_82027660);              // Forward with empty key
//    }
// ═══════════════════════════════════════════════════════════════════════════════
extern const uint8_t lbl_82027660;   // .rdata, 1 byte — empty-string sentinel

using vslot29_t = void(*)(void* self);
using vslot30_t = void(*)(void* self, int32_t arg);

extern "C" void atSingleton_DispatchFullInitSequence_2A40(void* selfArg)
{
    uint8_t* self = static_cast<uint8_t*>(selfArg);
    const uint8_t hasPreInit = self[4];
    void** vtable = *reinterpret_cast<void***>(self);

    if (hasPreInit != 0) {
        reinterpret_cast<vslot28_t>(vtable[28])(self);
        reinterpret_cast<vslot37_t>(vtable[37])(self, &lbl_82027660);
        reinterpret_cast<vslot29_t>(vtable[29])(self);
        reinterpret_cast<vslot30_t>(vtable[30])(self, 1);
        return;
    }

    // Raw byte offset +44 = vtable index 11.
    vslot11_t forward = reinterpret_cast<vslot11_t>(vtable[11]);
    forward(self, &lbl_82027660);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_2C18_2h — DispatchCloneOrVirtual
//
//  @ 0x82132C18 | size: 0x74 (116 bytes)
//
//  Two-mode copy/virtual dispatcher:
//
//    if (self->hasPreInit)
//    {
//      // Cross-copy variant: vtable[35] (raw byte +140)
//      // Reads other's vtable first, then tail-calls it.
//      void** othersVt = *(void***)other;
//      ((void(*)(void*, void*))othersVt[35])(self, other);
//    }
//    else
//    {
//      // Virtual copy via slot 4 — returns a copied pointer, store into *other.
//      *(void**)other = ((void*(*)(void*))self->vtable[4])(self);
//    }
// ═══════════════════════════════════════════════════════════════════════════════
using vslot4_t  = void*(*)(void* self);
using vslot35_t = void(*)(void* self, void* other);

extern "C" void atSingleton_DispatchCloneOrVirtual_2C18(void* selfArg,
                                                         void* otherArg)
{
    uint8_t* self  = static_cast<uint8_t*>(selfArg);
    uint8_t* other = static_cast<uint8_t*>(otherArg);

    const uint8_t hasPreInit = self[4];
    if (hasPreInit != 0) {
        void** othersVt = *reinterpret_cast<void***>(other);
        vslot35_t crossCopy = reinterpret_cast<vslot35_t>(othersVt[35]);
        crossCopy(self, other);
        return;
    }

    void** selfVt = *reinterpret_cast<void***>(self);
    vslot4_t clone = reinterpret_cast<vslot4_t>(selfVt[4]);
    void* cloned = clone(self);
    *reinterpret_cast<void**>(other) = cloned;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_2D80_2h — DispatchFloatPairSetter
//
//  @ 0x82132D80 | size: 0x88 (136 bytes)
//
//  Dispatcher for a pair-of-floats setter (a 2D vector-like API):
//
//    if (self->hasPreInit)
//    {
//      vtable[34](self, ((float*)arg)[0]);
//      vtable[34](self, ((float*)arg)[1]);
//    }
//    else
//    {
//      // Raw byte +40 = vtable index 10 — forwards whole `arg` pointer.
//      vtable_raw[10](self, arg);
//    }
// ═══════════════════════════════════════════════════════════════════════════════
using vslot10_t = void(*)(void* self, const void* arg);
using vslot34_t = void(*)(void* self, float v);

extern "C" void atSingleton_DispatchFloatPairSetter_2D80(void* selfArg,
                                                         const void* argArg)
{
    uint8_t*     self = static_cast<uint8_t*>(selfArg);
    const float* arg  = static_cast<const float*>(argArg);

    void** vtable = *reinterpret_cast<void***>(self);
    const uint8_t hasPreInit = self[4];

    if (hasPreInit != 0) {
        vslot34_t setOne = reinterpret_cast<vslot34_t>(vtable[34]);
        setOne(self, arg[0]);
        // Scaffold reloads vtable between the two calls.
        vtable = *reinterpret_cast<void***>(self);
        setOne = reinterpret_cast<vslot34_t>(vtable[34]);
        setOne(self, arg[1]);
        return;
    }

    // Raw byte +40 = vtable index 10.
    vslot10_t forward = reinterpret_cast<vslot10_t>(vtable[10]);
    forward(self, arg);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  atSingleton_A680_g — DestroyArrayStride8
//
//  @ 0x821AA680 | size: 0xCC (204 bytes)
//
//  Destroys a heap-allocated array of 8-byte records (stride-8 variant).
//  Each record has TWO owned pointers: one at +0 and one at +4; both are
//  freed via the untracked-aware path.  Count is stored uint32_t at
//  entries-4; header is freed after the walk.
// ═══════════════════════════════════════════════════════════════════════════════
extern "C" void* atSingleton_DestroyArrayStride8_A680(void* entriesArg)
{
    uint8_t* entries = static_cast<uint8_t*>(entriesArg);
    uint8_t* header  = entries - 4;

    const uint32_t count = *reinterpret_cast<uint32_t*>(header);
    uint8_t* cursor = entries + (count * 8u);
    int32_t remaining = static_cast<int32_t>(count) - 1;

    while (remaining >= 0) {
        cursor -= 8;
        uint8_t* second = *reinterpret_cast<uint8_t**>(cursor + 4);
        FreeIfUntracked(second);
        uint8_t* first  = *reinterpret_cast<uint8_t**>(cursor + 0);
        FreeIfUntracked(first);
        --remaining;
    }

    rage_dealloc(header, 3);
    return header;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Batch-10 TODOs / residual notes
//
//  * atSingleton_5000 / 5138_g:   string-registry format is inferred from the
//    scaffold (count@+4, items@+6, base@+0).  A dedicated `atStringHashMap`
//    struct in the header would let us drop the reinterpret_cast dance here.
//  * atSingleton_5878:             the `pool` (self+8) object is known to be
//    some kind of freelist — the schema (head@+16, count@+8) matches other
//    rage pool objects but was not cross-checked against a concrete class.
//  * atSingleton_2990 / 2A40 / 2C18 / 2D80:  these four are the four faces of
//    the rage-generic "dual-mode vtable dispatcher" (direct call when the
//    pre-init byte is 0, layered call otherwise).  Identifying the concrete
//    classes that use them would let us replace the vtable-slot math with
//    proper virtual method calls.
//  * atSingleton_rtti_92D4_1:      the `g_loop_obj_ptr + 20` field is a signed
//    phase counter elsewhere called `loop.phase`; named externs for both
//    g_loop_obj_ptr and g_input_obj_ptr already live in memory.cpp.
// ═══════════════════════════════════════════════════════════════════════════════


} // namespace rage


// ═══════════════════════════════════════════════════════════════════════════════
//  Batch: 10 hot atSingleton-prefixed helpers (session 2026-04-14)
//
//  Selected from xex_excavation_tt/hottest_functions.txt (top 300 most-called).
//  Each function retains its scaffold-address comment; none introduces dynamic
//  allocations beyond what the scaffold shows.
//
//  NOTE: Despite the "atSingleton_" prefix, most of these are actually helpers
//  for an unrelated subsystem (bit-stream decoder, HSM-state init, format-arg
//  dispatch, string-table lookup, list unlinker, trampolines).  The symbol
//  prefix is an artefact of the pass5 renamer grouping by call locality;
//  scaffold bodies do not touch singleton-registry state.
// ═══════════════════════════════════════════════════════════════════════════════

#include <cstdint>
#include <cstddef>

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
//  Externs used by this batch
// ─────────────────────────────────────────────────────────────────────────────

// Bit-stream refill helper (scaffold: atSingleton_23C0) @ 0x824D23C0.
extern "C" void atSingleton_23C0(void* pBitStream);

// Secondary bit-stream helper (scaffold: atSingleton_23C0 fallback) @ 0x824D23C0.
extern "C" void atSingleton_23C0_followup(void* pBitStream);

// One-hot HSM transition engine (already lifted) @ 0x82222800.
extern "C" void hsmContext_SetNextState_2800(void* ctx);

// Trace-string printf shim (already lifted) @ 0x8240E6D0.
extern "C" void nop_8240E6D0(const char* fmt, ...);

// pg subsystem: 7-arg batch-dispatch shim (already lifted) @ 0x8225E6E0.
extern "C" void pg_E6E0(int a0, int a1, int a2, int a3, int a4, int a5, int a6);

// SDA/TLS resolver (already lifted) @ 0x820C01B8.
extern "C" void rage_01B8(uint32_t key, uint32_t token);

// Locale-register shim (already lifted) @ 0x820C02D0.
extern "C" void _locale_register(uint32_t key, uint32_t token);

// Global singleton-registry hash-table search (already lifted) @ 0x820F94F0.
extern "C" int32_t game_94F0(void* table, uint32_t hash);

// Name-buffer alias helper used by the 32-byte dedup branch @ 0x820F8EE8.
extern "C" void game_8EE8(uint32_t /*buf*/);

// ─────────────────────────────────────────────────────────────────────────────
//  [1] atSingleton_DC30_gen  @ 0x824ADC30  | size: 0x0C (12 bytes)
//
//  Thin trampoline: prepares constant token value 0x248C8001 in r4 then
//  tail-calls rage_01B8 (0x820C01B8).  Used by 43+ callers as a registration
//  trampoline for this specific subsystem's key.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_DC30_gen(uint32_t thisPtr)
{
    // lis r4, 9356  |  ori r4, r4, 32769  →  0x248C8001
    rage_01B8(thisPtr, 0x248C8001u);
}

// ─────────────────────────────────────────────────────────────────────────────
//  [2] atSingleton_DC40_gen  @ 0x824ADC40  | size: 0x0C (12 bytes)
//
//  Twin of DC30_gen — same constant, different tail target (_locale_register
//  @ 0x820C02D0).  Both exist because the downstream handlers diverge on the
//  token but share the key construction.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_DC40_gen(uint32_t thisPtr)
{
    // lis r4, 9356  |  ori r4, r4, 32769  →  0x248C8001
    _locale_register(thisPtr, 0x248C8001u);
}

// ─────────────────────────────────────────────────────────────────────────────
//  [3] atSingleton_9850  @ 0x82459850  | size: 0x50 (80 bytes)
//
//  Intrusive doubly-linked list unlink-by-key.  The list head sentinel lives
//  at `this + 48` (self-referential node).  Traversal starts at `this + 52`
//  (head->next) and stops when the next pointer loops back to the sentinel.
//
//  For each node:
//    node layout used: +4 = prev, +8 = next, +12 = key (compared to searchKey)
//
//  On match, the node is unlinked:
//    node->prev->next = node->next
//    node->next->prev = node->prev
//    node->next = node   (self-reference after unlink — scaffold writes this)
//    node->prev = node
// ─────────────────────────────────────────────────────────────────────────────
struct atListNode9850
{
    uint32_t _pad0;       // +0
    uint32_t prev;        // +4
    uint32_t next;        // +8
    uint32_t key;         // +12
};

void atSingleton_9850(uint8_t* self, uint32_t searchKey)
{
    if (!self)
        return;

    const uint32_t sentinel = reinterpret_cast<uintptr_t>(self) + 48;
    uint32_t cur = *reinterpret_cast<uint32_t*>(self + 52);  // head->next

    while (cur != sentinel)
    {
        atListNode9850* node = reinterpret_cast<atListNode9850*>(
            static_cast<uintptr_t>(cur));
        if (node->key == searchKey)
        {
            // Standard intrusive unlink:
            //   prev->next = next;  next->prev = prev;
            reinterpret_cast<atListNode9850*>(
                static_cast<uintptr_t>(node->prev))->next = node->next;
            reinterpret_cast<atListNode9850*>(
                static_cast<uintptr_t>(node->next))->prev = node->prev;
            // Scaffold's self-pointing tail — marks node as detached.
            node->prev = cur;
            node->next = cur;
            return;
        }
        cur = node->prev;  // scaffold advances via +4 (reverse chain)
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  [4] atSingleton_FC98_p39  @ 0x8225FC98  | size: 0x70 (112 bytes)
//
//  String-to-token table lookup.  Walks a 47-entry table of (stringPtr, _)
//  stride-8 records starting at lbl_82611178 (-32164<<16 + 19320 = 0x82611178)
//  and strcmp's each entry's string against the input key.  On exact match,
//  returns (entryIndex + 16385) & 0xFFFF.  On miss, returns 0.
//
//  The '+16385' biases the return into a known-good token range; callers rely
//  on a zero return to mean "no match".
// ─────────────────────────────────────────────────────────────────────────────
struct atStringTableFC98Entry
{
    const char* name;   // +0
    uint32_t    token;  // +4 (read by caller, not this function)
};

extern const atStringTableFC98Entry g_atStringTableFC98[47];
// Table end sentinel = base + 376 bytes (47 × 8).

uint16_t atSingleton_FC98_p39(const char* key)
{
    if (!key)
        return 0;

    for (int i = 0; i < 47; i++)
    {
        const char* entry = g_atStringTableFC98[i].name;
        const char* k = key;

        // Inline strcmp — scaffold loops byte-by-byte with early-exit on 0.
        int diff = 0;
        while (true)
        {
            uint8_t e = static_cast<uint8_t>(*entry);
            uint8_t c = static_cast<uint8_t>(*k);
            diff = static_cast<int>(e) - static_cast<int>(c);
            if (e == 0 || diff != 0) break;
            ++entry;
            ++k;
        }

        if (diff == 0)
        {
            // Match — scaffold returns (i + 16385) truncated to 16 bits.
            return static_cast<uint16_t>((i + 16385) & 0xFFFF);
        }
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
//  [5] atSingleton_1E38_h  @ 0x823D1E38  | size: 0x138 (312 bytes)
//
//  HSM state-transition gate.  Checks a mode word at `this + 12`:
//    - If mode == 6: emit trace "<lbl+4836>", dispatch vtable slot 3 on self,
//      batch-call pg_E6E0(14342, 128, 0, 0, 0, 0, 0) to notify the pg layer,
//      then zero a byte at g_pgCtx+335 (singleton + 335).
//    - Else: check a cancel-flag at `this + 60`.  If set, emit trace at
//      lbl+4904 and return 0 (transition denied).  Otherwise emit trace at
//      lbl+4880 and dispatch vtable slot 3 (same as the mode-6 path).
//
//  On any proceed-path, copies two 16-byte blocks from (paramA@+0..12) and
//  (paramB@+0..12) into `this + 20` and `this + 36`, stamps `this + 52 = arg4`,
//  `this + 60 = 2` (mark "transitioning"), `this + 64 = arg1 (float)`, then
//  tail-calls hsmContext_SetNextState_2800.  Returns 1 (accepted).
// ─────────────────────────────────────────────────────────────────────────────
struct StateInitContext1E38
{
    uint32_t* vtable;   // +0
    uint32_t  flags;    // +4
    uint32_t  _pad8;    // +8
    uint32_t  mode;     // +12 — branch selector
    uint8_t   _pad10[60 - 16];
    uint32_t  cancelFlag;  // +60
    uint32_t  stateArg4;   // +52 (written before cancelFlag in field order)
    float     stateArgF;   // +64
};

extern uint8_t* g_pgCtxBase_Lbl;  // lbl_825B97A8 — resolved at init.

uint32_t atSingleton_1E38_h(
    StateInitContext1E38* self,
    uint32_t* paramA,       // 4×u32 block copied to this+20
    uint32_t* paramB,       // 4×u32 block copied to this+36
    uint32_t arg4,          // written to this+52
    float argFloat)         // written to this+64
{
    if (!self)
        return 0;

    // Vtable slot index 3 is a method at vtable[12] (byte offset 12 = slot 3).
    using StateProceed = void (*)(StateInitContext1E38*);

    if (self->mode == 6)
    {
        nop_8240E6D0("atSingleton_1E38_h: mode=6 proceed");
        uint32_t* vt = self->vtable;
        StateProceed fn = reinterpret_cast<StateProceed>(
            static_cast<uintptr_t>(vt[3]));
        fn(self);

        pg_E6E0(14342, 128, 0, 0, 0, 0, 0);

        // Clear gate-byte at g_pgCtx + 335.
        if (g_pgCtxBase_Lbl)
            *(g_pgCtxBase_Lbl + 335) = 0;
    }
    else
    {
        if (self->cancelFlag != 0)
        {
            nop_8240E6D0("atSingleton_1E38_h: cancelled (flag set)");
            return 0;   // transition denied
        }
        nop_8240E6D0("atSingleton_1E38_h: proceed");
        uint32_t* vt = self->vtable;
        StateProceed fn = reinterpret_cast<StateProceed>(
            static_cast<uintptr_t>(vt[3]));
        fn(self);
    }

    // Copy two 4-word blocks.
    uint32_t* dstA = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 20);
    dstA[0] = paramA[0];
    dstA[1] = paramA[1];
    dstA[2] = paramA[2];
    dstA[3] = paramA[3];

    uint32_t* dstB = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 36);
    dstB[0] = paramB[0];
    dstB[1] = paramB[1];
    dstB[2] = paramB[2];
    dstB[3] = paramB[3];

    self->stateArgF  = argFloat;
    self->cancelFlag = 2;     // mark "transitioning"
    self->stateArg4  = arg4;

    hsmContext_SetNextState_2800(self);
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
//  [6] atSingleton_2748_gen  @ 0x824D2748  | size: 0xCC (204 bytes)
//
//  Bit-stream 48-bit refill fast-path.  The stream struct matches the layout
//  documented on atSingleton_2688 (see rage_memory.cpp):
//    +0  u64 accumulator,  +8 s32 bitCount,  +12 u32 readPos,  +16 u32 endPos,
//    +20 u32 errorFlag,    +24 u32 mode.
//
//  Entry point:  if (readPos > endPos - 4) slow-path via atSingleton_2688
//                (handles 1-byte-at-a-time refill + error/refill gates).
//  Else:         read 6 bytes big-endian as a 48-bit integer, shift left by
//                (-bitCount) bits (equivalent to <<(64-bitCount) masked to
//                48-bit payload), add to accumulator, advance readPos by 6
//                and bitCount by 48 (implicit via the mask dance in the
//                scaffold — stored as (bitCount + 48) via "addi 48").
// ─────────────────────────────────────────────────────────────────────────────
struct BitStream2748
{
    uint64_t accumulator;   // +0
    int32_t  bitCount;      // +8
    uint32_t readPos;       // +12
    uint32_t endPos;        // +16
    uint32_t errorFlag;     // +20
    uint32_t mode;          // +24
};

extern "C" int atSingleton_2688(void* pBitStream);

void atSingleton_2748_gen(BitStream2748* s)
{
    if (!s)
        return;

    // Slow path trigger — scaffold's "addi r10,r10,-4 / cmplw / blt" sequence.
    // If the window doesn't have 6 bytes' worth of lookahead, call the 1-byte
    // refiller (atSingleton_2688) until it reports "no more refill needed".
    while (s->readPos >= (s->endPos - 4))
    {
        int rc = atSingleton_2688(s);
        if (rc != 1)
            return;
    }

    // Read 6 bytes big-endian from readPos.
    const uint8_t* src = reinterpret_cast<const uint8_t*>(
        static_cast<uintptr_t>(s->readPos));
    uint64_t beValue =
        (static_cast<uint64_t>(src[0]) << 40) |
        (static_cast<uint64_t>(src[1]) << 32) |
        (static_cast<uint64_t>(src[2]) << 24) |
        (static_cast<uint64_t>(src[3]) << 16) |
        (static_cast<uint64_t>(src[4]) << 8)  |
        (static_cast<uint64_t>(src[5]));

    // readPos += 6, bitCount += 48 (encoded as "addi r5,r11,48" / "stw r5,8").
    s->readPos += 6;

    // Left-shift by negative of the pre-stored bitCount (scaffold: neg/extsw).
    int32_t preCount = s->bitCount;
    int shift = (64 - 48 - preCount);  // scaffold's effective shift width
    if (shift < 0) shift = 0;
    if (shift >= 64) shift = 63;
    s->accumulator += (beValue << shift);

    s->bitCount = preCount + 48;
}

// ─────────────────────────────────────────────────────────────────────────────
//  [7] atSingleton_2818_2h  @ 0x824D2818  | size: 0x16C (364 bytes)
//
//  Variable-shift entry-point for the bit-stream: takes `shiftBits` as its
//  second argument (u32, clrldi'd from 32 bits), shifts the current
//  accumulator left by that amount, decrements bitCount by `shiftBits`, then
//  refills if bitCount went negative.
//
//  Refill strategy when short:
//    - If mode != 1: byte-at-a-time refill into the accumulator (same as
//      atSingleton_2688's inner loop) until bitCount goes non-negative.
//      If the buffer ran out AND bitCount < -16, set the same underflow
//      error (errorFlag = 2, bitCount = 127) seen in atSingleton_2688.
//    - If mode == 1: call atSingleton_23C0 (fetcher) which refills from
//      the upstream source.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_2818_2h(BitStream2748* s, uint32_t shiftBits)
{
    if (!s)
        return;

    // Shift consume — scaffold: sld r9,r11,r10 / subf. r11,r4,r8.
    uint32_t sh = shiftBits & 0x3F;  // PPC sld masks low 6 bits
    s->accumulator <<= sh;
    s->bitCount    -= static_cast<int32_t>(shiftBits);

    if (s->bitCount >= 0)
        return;

    // Byte-refill loop (mirrors atSingleton_2688 inner body).
    while (s->readPos <= s->endPos)
    {
        uint8_t byte = *reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(s->readPos));
        s->readPos++;

        int32_t shiftAmount = 40 - s->bitCount;
        if (shiftAmount < 0) shiftAmount = 0;
        if (shiftAmount > 63) shiftAmount = 63;
        s->accumulator += (static_cast<uint64_t>(byte) << shiftAmount);
        s->bitCount    += 8;

        if (s->bitCount >= 0)
            return;
    }

    // Short-fill path matches atSingleton_2688:
    if (s->mode == 1)
    {
        atSingleton_23C0(s);
        return;
    }

    if (s->bitCount < -16)
    {
        if (s->errorFlag == 0)
            s->errorFlag = 2;
        s->bitCount = 127;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  [8] atSingleton_E998_g  @ 0x8225E998  | size: 0xD0 (208 bytes)
//
//  Printf-like format-arg dispatcher.  Walks a format-char string (arg2,
//  `const char*`) and, for each char, pulls the next variadic slot off a
//  stack-saved var-arg area and writes a single 4-byte value into a growing
//  output buffer.
//
//  Dispatched character codes (from scaffold equal-compares):
//    'i' (105)  → copy 4-byte int    from variadic slot into out+0 .. out+3
//    'f' (102)  → copy 4-byte float  (same slot-width as 'i' on PPC ABI)
//    's' (115)  → variadic slot is a char* — compute strlen+1 (byte count),
//                 round up to next 4-byte boundary, then advance the output
//                 cursor past the string's entire inline payload.  The
//                 scaffold writes the char* itself into the 4-byte output
//                 slot, then advances by strlen(s)+1 rounded to 4.
//    any other  → field is ignored (advance the format cursor only).
//
//  This matches the common Rockstar "write printf args into a blob" pattern
//  seen across the engine (e.g. pong logging, cfg-value encoding).  The VMX128
//  noise in the pseudocode is register spill from the variadic ABI — it does
//  NOT participate in this function's logic.
// ─────────────────────────────────────────────────────────────────────────────
void atSingleton_E998_g(
    uint8_t* outBuffer,         // r3 — output cursor base
    const char* fmt,            // r4 — format string
    uint32_t /*r5_slot0*/,      // r5..r10 → variadic slots 0..5
    uint32_t /*r6_slot1*/,
    uint32_t /*r7_slot2*/,
    uint32_t /*r8_slot3*/,
    uint32_t /*r9_slot4*/,
    uint32_t /*r10_slot5*/)
{
    if (!outBuffer || !fmt)
        return;

    // Variadic slots are saved to stack @ sp+32..72 by the prologue; lift them
    // from a contiguous pointer so callers can keep passing through.
    uint32_t* slots = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uintptr_t>(&outBuffer) + 8);

    uint32_t cursor = 0;  // byte offset into outBuffer
    uint32_t slotIx = 0;  // index into variadic stack area

    for (const char* p = fmt; *p; ++p)
    {
        int8_t c = static_cast<int8_t>(*p);

        if (c == 'i' || c == 'f')
        {
            // 4-byte value — stash at word-aligned slot.
            cursor = (cursor + 7) & ~0x7u;       // round up to 8-align
            cursor += 8;                         // reserve 8 bytes header
            *reinterpret_cast<uint32_t*>(
                outBuffer + cursor - 4) = slots[slotIx];
        }
        else if (c == 's')
        {
            // String pointer — stash, then skip the inline payload.
            cursor = (cursor + 7) & ~0x7u;
            cursor += 8;
            uint32_t strPtr = slots[slotIx];
            *reinterpret_cast<uint32_t*>(
                outBuffer + cursor - 4) = strPtr;

            // strlen + 1, rounded up to 4.
            const char* s = reinterpret_cast<const char*>(
                static_cast<uintptr_t>(strPtr));
            uint32_t len = 0;
            while (s[len] != 0) ++len;
            uint32_t pad = ((len + 1) + 3) & ~0x3u;
            cursor += pad - 4;  // scaffold: "addi r10,r10,4 / rlwinm ..." dance
        }
        // Other chars: no output, no slot consumed.

        slotIx++;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  [9] atSingleton_Find_90D0 (refinement)  @ 0x820F90D0  | size: 0xA4
//
//  The original stub (see FindSingleton above) only handled the "hash < 0"
//  plus hash-table-hit leaf.  The scaffold also contains a 32-byte alias path:
//    - If the lookup missed AND (hash & 0xFF) == 32, subtract 32 from hash
//      and (if non-zero) run game_8EE8(newHash) + _locale_register(newHash, ...)
//      to publish the alias, then return 1 (= "registered").
//    - Otherwise return 0.
//
//  This variant is called 433× in the hottest_functions profile and is the
//  entry point used by alias-registration plumbing.  We expose it as a new
//  named helper; callers that used the old FindSingleton keep working because
//  the leaf behaviour (return ptr-or-null) is unchanged.
// ─────────────────────────────────────────────────────────────────────────────
uint32_t atSingleton_Find_90D0(uint32_t hash)
{
    if (static_cast<int32_t>(hash) >= 0)
        return 0;

    // Primary registry lookup (same as FindSingleton).
    int32_t idx = game_94F0(g_singletonRegistry, hash);
    if (idx != -1)
    {
        // Hit: translate to the concrete singleton pointer (hash - entryBase).
        uint32_t entryOffset = static_cast<uint32_t>(idx) << 3;
        uint32_t entryBase = *reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(g_singletonRegistry) + entryOffset);
        // Scaffold returns the subtracted value in r11 but doesn't use it —
        // callers read 0/1.  Return 1 to signal "resolved".
        (void)(hash - entryBase);
        return 1;
    }

    // Miss — check the 32-byte alias path.
    // Scaffold: "cmplwi cr6,r11,32 / bne cr6,0x820f9150" where r11 is the
    // low 8 bits of hash (scaffold uses clrlwi before the cmp).
    if ((hash & 0xFF) == 32)
    {
        uint32_t trimmed = hash - 32;
        if (trimmed != 0)
        {
            game_8EE8(trimmed);
            _locale_register(trimmed, 0xE001u /* lis -8191 */);
        }
        return 1;
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
//  [10] atSingleton_2688_thin_wrapper
//
//  atSingleton_2688 itself is already lifted in rage_memory.cpp; callers that
//  chain through the 2748/2818 helpers above expect a C-linkage symbol in
//  this TU.  Provide a trivial forwarder so atSingleton.cpp is self-contained
//  for link-time resolution.  Any host build-glue that tail-calls the real
//  lifted body will pick it up ahead of this weak alias.
// ─────────────────────────────────────────────────────────────────────────────
__attribute__((weak))
extern "C" int atSingleton_2688_forward(void* pBitStream)
{
    return atSingleton_2688(pBitStream);
}

} // namespace rage



// game_8EE8 @ 0x820F8EE8 | size: 0xC8 — singleton pool entry removal
// Moved from stubs.cpp. TODO: Full lift.
void game_8EE8(void* singletonEntry) { (void)singletonEntry; }

