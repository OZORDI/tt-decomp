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
//
//  Searches the global singleton registry hash table for a singleton
//  matching the given hash.
//
//  Scaffold analysis:
//    - If hash < 0: looks up in g_singletonRegistry via atHashTable_Find
//    - Uses (index * 8) to compute entry offset in the hash table
//    - Has a special case for hash == -1 (not found) that calls into
//      locale registration (game_8EE8 + _locale_register)
//    - Returns 0 on failure, 1 on special-case success
//
//  TODO: Fully implement based on scaffold — complex negative-hash logic
//  with locale registration side-effects.
// ═══════════════════════════════════════════════════════════════════════════════
atSingleton* FindSingleton(uint32_t hash)
{
    // TODO: Full implementation requires atHashTable_Find integration
    // and negative-hash special case handling.
    return nullptr;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  UnregisterSingleton
//
//  @ 0x820C29E0 | size: 0x74
//
//  Removes a singleton from the registry by name.  Computes string length,
//  allocates a copy, then performs the actual removal.
//
//  TODO: The removal logic after the string copy is missing.
// ═══════════════════════════════════════════════════════════════════════════════
void UnregisterSingleton(const char* name)
{
    if (!name)
        return;

    // Calculate string length
    const char* p = name;
    while (*p) p++;
    uint32_t length = static_cast<uint32_t>(p - name);

    // Allocate memory for string copy
    uint32_t allocSize = length + 1;
    void* nameCopy = rage_alloc(allocSize);

    if (nameCopy) {
        std::memcpy(nameCopy, name, allocSize);

        // TODO: Perform actual unregistration via registry removal
    }
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
//  BindObject / UnbindObject
//
//  @ 0x8224B3F8 | size: 0xFC / @ 0x8218B410 | size: 0x2E0
//
//  Register/unregister a singleton in the global registry by hash.
//  TODO: Full implementation requires atHashTable integration.
// ═══════════════════════════════════════════════════════════════════════════════
void BindObject(atSingleton* singleton, uint32_t hash)
{
    if (!singleton || !hash)
        return;

    if (FindSingleton(hash))
        return;

    // TODO: Add to registry via atHashTable
}

void UnbindObject(uint32_t hash)
{
    // TODO: Remove from registry via atHashTable
}


// ═══════════════════════════════════════════════════════════════════════════════
//  InitializeNetSystem
//
//  @ 0x8234B618 | size: 0x34C (844 bytes)
//
//  Initializes the network singleton system.
//  TODO: Large function — needs scaffold analysis.
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
//  @ 0x822E2E60 | size: 0x1E0
//
//  Looks up the factory for a singleton type by hash.
//  TODO: Factory registry lookup implementation.
// ═══════════════════════════════════════════════════════════════════════════════
SingletonFactory* GetFactory(uint32_t hash)
{
    // TODO: Factory registry lookup
    return nullptr;
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
