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
