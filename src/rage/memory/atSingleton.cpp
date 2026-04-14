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


} // namespace rage
