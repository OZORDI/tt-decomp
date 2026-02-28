#pragma once

/**
 * atSingleton - RAGE Engine Singleton Management System
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Core singleton registry and lifecycle management for the RAGE engine.
 * Provides hash-based singleton lookup, reference counting, and observer notifications.
 */

#include <stdint.h>

namespace rage
{

// Forward declarations
class atSingleton;
class SingletonFactory;

/**
 * Singleton registry entry
 * Each entry tracks a registered singleton instance with its hash and metadata
 */
struct SingletonEntry
{
    const char* name;        // Singleton name string
    uint32_t hash;           // ELF hash of singleton name  
    atSingleton* instance;   // Pointer to singleton instance
    uint32_t refCount;       // Reference count
};

/**
 * Dynamic singleton array
 * Grows automatically when capacity is reached
 */
struct SingletonArray
{
    SingletonEntry* entries; // Dynamic array of entries
    uint16_t count;          // Current number of entries
    uint16_t capacity;       // Maximum capacity before reallocation
};

/**
 * Base singleton class
 * All RAGE singletons inherit from this base
 */
class atSingleton
{
public:
    virtual ~atSingleton() = default;
    
    // Core lifecycle - must be implemented by derived classes
    virtual void Initialize() = 0;
    virtual void Release() = 0;
    
    // Observer pattern for event notifications
    virtual void NotifyObservers(uint32_t eventType) = 0;
    
protected:
    SingletonArray* m_pObservers;  // Observer list at +0x0C
    uint32_t m_flags;               // Status flags
    uint32_t m_refCount;            // Reference count
};

/**
 * Singleton factory interface
 * Creates and destroys singleton instances
 */
class SingletonFactory
{
public:
    virtual ~SingletonFactory() = default;
    virtual atSingleton* CreateInstance() = 0;
    virtual void DestroyInstance(atSingleton* instance) = 0;
};

// ============================================================================
// Core Functions
// ============================================================================

/**
 * ComputeHash @ 0x8223A818 | size: 0x50
 * 
 * Compute ELF hash of a string for singleton lookup.
 * Standard ELF hash algorithm used throughout RAGE engine.
 * 
 * @param str String to hash
 * @return 32-bit hash value
 */
uint32_t ComputeHash(const char* str);

/**
 * FindSingleton @ 0x820F90D0 | size: 0xA4
 * 
 * Find singleton instance by hash value.
 * Performs linear search through singleton registry.
 * 
 * @param hash Hash value to search for
 * @return Singleton instance or nullptr if not found
 */
atSingleton* FindSingleton(uint32_t hash);

/**
 * UnregisterSingleton @ 0x820C29E0 | size: 0x74
 * 
 * Remove singleton from registry by name.
 * Computes hash and removes entry from registry.
 * 
 * @param name Singleton name string
 */
void UnregisterSingleton(const char* name);

/**
 * GetFactory @ 0x822E2E60 | size: 0x1E0
 * 
 * Get factory for singleton type by hash.
 * 
 * @param hash Type hash
 * @return Factory instance or nullptr
 */
SingletonFactory* GetFactory(uint32_t hash);

// ============================================================================
// Lifecycle Management
// ============================================================================

/**
 * InitializeSingleton @ 0x821A8588 | size: 0x194
 * 
 * Initialize singleton array with dynamic growth.
 * Handles array reallocation when capacity is reached.
 * 
 * @param singletonArray Array to initialize
 */
void InitializeSingleton(SingletonArray* singletonArray);

/**
 * ReleaseSingleton @ 0x821A9420 - ReleaseSingleton | size: 0x7C
 * 
 * Release singleton and call observer destructors.
 * Iterates through observer list calling virtual destructors.
 * 
 * @param singleton Singleton to release
 */
void ReleaseSingleton(atSingleton* singleton);

/**
 * AcquireReference @ 0x8222AB48 | size: 0x1F8
 * 
 * Increment reference count on singleton.
 * 
 * @param singleton Singleton to reference
 */
void AcquireReference(atSingleton* singleton);

// ============================================================================
// Registry Management
// ============================================================================

/**
 * BindObject @ 0x8224B3F8 | size: 0xFC
 * 
 * Bind singleton to registry with hash.
 * 
 * @param singleton Singleton instance
 * @param hash Hash value
 */
void BindObject(atSingleton* singleton, uint32_t hash);

/**
 * UnbindObject @ 0x8218B410 | size: 0x2E0
 * 
 * Unbind singleton from registry by hash.
 * 
 * @param hash Hash value
 */
void UnbindObject(uint32_t hash);

// ============================================================================
// System Initialization
// ============================================================================

/**
 * InitializeNetSystem @ 0x8234B618 | size: 0x34C
 * 
 * Initialize network singleton system.
 */
void InitializeNetSystem();

// ============================================================================
// Observer Notifications
// ============================================================================

/**
 * NotifyObservers @ 0x8225E998 | size: 0xD0
 * 
 * Notify all observers of singleton event.
 * 
 * @param singleton Singleton instance
 * @param eventType Event type code
 */
void NotifyObservers(atSingleton* singleton, uint32_t eventType);

/**
 * ClearPendingFlag @ 0x8231F728 | size: 0x25C
 * 
 * Clear pending flag on singleton.
 * 
 * @param singleton Singleton instance
 */
void ClearPendingFlag(atSingleton* singleton);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * CallVirtualDestructor @ 0x821E7BC0 | size: 0xC
 * 
 * Call virtual destructor through vtable.
 * Originally misnamed as "NoOpCallback" - actually dispatches to vtable slot 0.
 * 
 * @param obj Object with vtable
 */
void CallVirtualDestructor(void* obj);

} // namespace rage
