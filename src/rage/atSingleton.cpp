/**
 * atSingleton - RAGE Engine Singleton Management System
 * Rockstar Presents Table Tennis (Xbox 360)
 */

#include "rage/atSingleton.hpp"
#include <string.h>

// External dependencies
extern "C" {
    void* xe_EC88(uint32_t size);  // Memory allocator
    void rage_2980(void* ptr, uint32_t mode);  // Memory deallocator
    void memcpy(void* dest, const void* src, uint32_t size);
}

namespace rage
{

// ============================================================================
// Core Hash Function
// ============================================================================

/**
 * ComputeHash @ 0x8223A818 | size: 0x50
 * 
 * Standard ELF hash algorithm.
 * Used throughout RAGE engine for string-based lookups.
 */
uint32_t ComputeHash(const char* str)
{
    if (!str)
        return 0;
    
    uint32_t hash = 0;
    
    while (*str)
    {
        // hash = (hash << 4) + byte
        hash = (hash << 4) + static_cast<uint8_t>(*str++);
        
        // temp = hash & 0xF0000000
        uint32_t temp = hash & 0xF0000000;
        
        if (temp)
        {
            // hash ^= (temp >> 24)
            hash ^= (temp >> 24);
            // hash ^= temp
            hash ^= temp;
        }
    }
    
    return hash;
}

// ============================================================================
// Singleton Lookup
// ============================================================================

/**
 * FindSingleton @ 0x820F90D0 | size: 0xA4
 * 
 * Find singleton by hash in the global registry.
 * The assembly shows this searches a hash table structure.
 */
atSingleton* FindSingleton(uint32_t hash)
{
    // TODO: Implement hash table lookup
    // Assembly shows:
    // - Check if hash is negative (special case)
    // - Call game_94F0 for hash table lookup
    // - Extract entry using (index * 8) offset
    // - Return instance pointer
    
    return nullptr;  // Placeholder
}

// ============================================================================
// Registry Management
// ============================================================================

/**
 * UnregisterSingleton @ 0x820C29E0 | size: 0x74
 * 
 * Remove singleton from registry by name.
 * Computes string length, allocates copy, and performs removal.
 */
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
    void* nameCopy = xe_EC88(allocSize);
    
    if (nameCopy)
    {
        // Copy string
        memcpy(nameCopy, name, allocSize);
        
        // TODO: Perform actual unregistration
        // Assembly shows this calls into registry removal logic
    }
}

// ============================================================================
// Lifecycle Management
// ============================================================================

/**
 * InitializeSingleton @ 0x821A8588 | size: 0x194
 * 
 * Initialize singleton array with dynamic growth.
 * Complex function that handles array reallocation when capacity is reached.
 */
void InitializeSingleton(SingletonArray* singletonArray)
{
    if (!singletonArray)
        return;
    
    // Check if array needs to grow
    if (singletonArray->count >= singletonArray->capacity)
    {
        // Calculate new capacity (grow by 16 entries)
        uint16_t newCapacity = singletonArray->capacity + 16;
        
        // Clamp to maximum (0x5555 = 21845 entries)
        if (newCapacity > 0x5555)
        {
            // Allocation failed
            return;
        }
        
        // Calculate allocation size: count * 3 * 4 bytes per entry
        // Each entry is 12 bytes (3 uint32_t fields)
        uint32_t allocSize = newCapacity * 3 * sizeof(uint32_t);
        
        // Allocate new array
        SingletonEntry* newEntries = static_cast<SingletonEntry*>(xe_EC88(allocSize + 4));
        
        if (newEntries)
        {
            // Store capacity at start
            *reinterpret_cast<uint32_t*>(newEntries) = newCapacity;
            
            // Actual entries start after capacity field
            SingletonEntry* entries = reinterpret_cast<SingletonEntry*>(
                reinterpret_cast<uint8_t*>(newEntries) + 4
            );
            
            // Initialize new entries to zero
            for (uint16_t i = 0; i < newCapacity; i++)
            {
                entries[i].name = nullptr;
                entries[i].hash = 0;
                entries[i].instance = nullptr;
                entries[i].refCount = 0;
            }
            
            // Copy existing entries
            if (singletonArray->count > 0)
            {
                for (uint16_t i = 0; i < singletonArray->count; i++)
                {
                    // Copy name string
                    const char* oldName = singletonArray->entries[i].name;
                    if (oldName)
                    {
                        char* newName = static_cast<char*>(UnregisterSingleton(oldName));
                        entries[i].name = newName;
                    }
                    
                    // Copy hash
                    uint32_t hash = singletonArray->entries[i].hash;
                    if (hash)
                    {
                        // Verify singleton still exists
                        atSingleton* instance = FindSingleton(hash);
                        entries[i].instance = instance;
                    }
                    
                    // Copy other fields
                    entries[i].hash = singletonArray->entries[i].hash;
                    entries[i].refCount = singletonArray->entries[i].refCount;
                }
            }
            
            // Free old array
            if (singletonArray->entries)
            {
                rage_2980(singletonArray->entries, 3);
            }
            
            // Update array pointer
            singletonArray->entries = entries;
            singletonArray->capacity = newCapacity;
        }
    }
    
    // Add new entry at end
    uint16_t index = singletonArray->count;
    singletonArray->count++;
}

/**
 * ReleaseSingleton @ 0x821A9420 - ReleaseSingleton | size: 0x7C
 * 
 * Release singleton and call observer destructors.
 * Sets vtable and iterates through observer list.
 */
void ReleaseSingleton(atSingleton* singleton)
{
    if (!singleton)
        return;
    
    // Set vtable pointer (assembly shows two different vtable addresses)
    // First vtable: 0x8203AB6C
    // Second vtable: 0x82017CB4
    
    // Call xmlTree_9EC0_g (cleanup function)
    // TODO: extern void xmlTree_9EC0_g(void* obj);
    // xmlTree_9EC0_g(singleton);
    
    // Iterate through observers and call their destructors
    SingletonArray* observers = singleton->m_pObservers;
    if (observers)
    {
        while (observers->entries != nullptr)
        {
            atSingleton* observer = observers->entries[0].instance;
            if (observer)
            {
                // Call virtual destructor (vtable slot 0) with parameter 1
                typedef void (*DestructorFunc)(atSingleton*, uint32_t);
                void** vtable = *reinterpret_cast<void***>(observer);
                DestructorFunc dtor = reinterpret_cast<DestructorFunc>(vtable[0]);
                dtor(observer, 1);
            }
            
            // Check if there are more observers
            if (!observers->entries)
                break;
        }
    }
    
    // Set final vtable
    // *reinterpret_cast<void**>(singleton) = finalVtable;
}

/**
 * AcquireReference @ 0x8222AB48 | size: 0x1F8
 * 
 * Increment reference count on singleton.
 */
void AcquireReference(atSingleton* singleton)
{
    if (!singleton)
        return;
    
    singleton->m_refCount++;
}

// ============================================================================
// Registry Binding
// ============================================================================

/**
 * BindObject @ 0x8224B3F8 | size: 0xFC
 * 
 * Bind singleton to registry with hash.
 */
void BindObject(atSingleton* singleton, uint32_t hash)
{
    if (!singleton || !hash)
        return;
    
    // Check if already bound
    if (FindSingleton(hash))
        return;
    
    // TODO: Add to registry
}

/**
 * UnbindObject @ 0x8218B410 | size: 0x2E0
 * 
 * Unbind singleton from registry by hash.
 */
void UnbindObject(uint32_t hash)
{
    // TODO: Remove from registry
}

// ============================================================================
// System Initialization
// ============================================================================

/**
 * InitializeNetSystem @ 0x8234B618 | size: 0x34C
 * 
 * Initialize network singleton system.
 */
void InitializeNetSystem()
{
    // TODO: Network system initialization
}

// ============================================================================
// Observer Notifications
// ============================================================================

/**
 * NotifyObservers @ 0x8225E998 | size: 0xD0
 * 
 * Notify all observers of singleton event.
 */
void NotifyObservers(atSingleton* singleton, uint32_t eventType)
{
    if (!singleton)
        return;
    
    singleton->NotifyObservers(eventType);
}

/**
 * ClearPendingFlag @ 0x8231F728 | size: 0x25C
 * 
 * Clear pending flag on singleton.
 */
void ClearPendingFlag(atSingleton* singleton)
{
    if (!singleton)
        return;
    
    singleton->m_flags &= ~0x1;  // Clear bit 0 (pending flag)
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * CallVirtualDestructor @ 0x821E7BC0 | size: 0xC
 * 
 * Call virtual destructor through vtable.
 * Originally misnamed as "NoOpCallback".
 */
void CallVirtualDestructor(void* obj)
{
    if (!obj)
        return;
    
    // Load vtable pointer from object
    void** vtable = *reinterpret_cast<void***>(obj);
    
    // Call first virtual function (destructor at slot 0)
    typedef void (*VirtualFunc)(void*);
    VirtualFunc func = reinterpret_cast<VirtualFunc>(vtable[0]);
    func(obj);
}

/**
 * GetFactory @ 0x822E2E60 | size: 0x1E0
 * 
 * Get factory for singleton type by hash.
 */
SingletonFactory* GetFactory(uint32_t hash)
{
    // TODO: Factory registry lookup
    return nullptr;
}

} // namespace rage
