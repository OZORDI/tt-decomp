#pragma once

#include <stdint.h>

namespace rage
{

// Forward declarations
class atSingleton;

// Singleton registry entry structure
struct SingletonEntry
{
    uint32_t hash;           // ELF hash of singleton name
    atSingleton* instance;   // Pointer to singleton instance
    uint32_t refCount;       // Reference count
    uint32_t flags;          // Status flags
};

// Singleton registry (global at 0x82607C40, ~8192 entries, 65KB)
struct SingletonRegistry
{
    SingletonEntry entries[8192];
    uint32_t count;
    uint32_t capacity;
};

// Base singleton class
class atSingleton
{
public:
    virtual ~atSingleton() = default;
    
    // Core lifecycle
    virtual void Initialize() = 0;
    virtual void Release() = 0;
    
    // Observer pattern
    virtual void NotifyObservers(uint32_t eventType) = 0;
    
protected:
    uint32_t m_refCount;
    uint32_t m_flags;
};

// Singleton factory interface
class SingletonFactory
{
public:
    virtual ~SingletonFactory() = default;
    virtual atSingleton* CreateInstance() = 0;
    virtual void DestroyInstance(atSingleton* instance) = 0;
};

// Function declarations

// Hash computation (ELF hash variant)
uint32_t ComputeHash(const char* str);

// Registry management
atSingleton* FindSingleton(uint32_t hash);
SingletonFactory* GetFactory(uint32_t hash);
void UnregisterSingleton(uint32_t hash);

// Lifecycle management
void InitializeSingleton(atSingleton* singleton);
void ReleaseSingleton(atSingleton* singleton);
void AcquireReference(atSingleton* singleton);

// Binding/unbinding
void BindObject(atSingleton* singleton, uint32_t hash);
void UnbindObject(uint32_t hash);

// Network system
void InitializeNetSystem();

// Observer notifications
void NotifyObservers(atSingleton* singleton, uint32_t eventType);

// Flags management
void ClearPendingFlag(atSingleton* singleton);

// Utility
void NoOpCallback();

// Main game loop thread (originally misidentified as RegisterSingleton)
void MainGameLoopThread();

} // namespace rage
