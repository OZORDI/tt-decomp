#pragma once

/**
 * RAGE Singleton Registry System
 * 
 * Provides centralized management for singleton instances.
 */

#include <cstdint>
#include <cstddef>

namespace rage {

/**
 * Singleton Registry Entry
 * 
 * 8-byte structure matching PowerPC layout.
 * Uses 32-bit values for cross-platform compatibility.
 */
struct SingletonEntry {
    uint32_t instance;   // Pointer stored as 32-bit offset
    uint32_t type_id;    // Type identifier
};

/**
 * Validates if a pointer is a registered singleton.
 * 
 * @param ptr Pointer to validate
 * @return 0 if singleton (don't free), 1 if not singleton (can free)
 */
uint8_t ValidateSingletonPointer(void* ptr);

/**
 * Finds the index of a singleton in the registry.
 * 
 * @param ptr Pointer to search for
 * @return Index in registry, or -1 if not found
 */
int32_t FindSingletonIndex(void* ptr);

/**
 * Registers a new singleton instance.
 * 
 * @param instance Pointer to singleton instance
 * @param type_id Type identifier
 * @return true if registered successfully
 */
bool RegisterSingleton(void* instance, uint32_t type_id);

/**
 * Unregisters a singleton instance.
 * 
 * @param instance Pointer to singleton instance
 * @return true if unregistered successfully
 */
bool UnregisterSingleton(void* instance);

/**
 * Gets the current number of registered singletons.
 */
uint32_t GetSingletonCount();

} // namespace rage

// C interface for compatibility
#ifdef __cplusplus
extern "C" {
#endif

/**
 * atSingleton_Find_90D0
 * @ 0x820F90D0 | size: 0xA4
 * 
 * Validates if a pointer is a registered singleton.
 */
uint8_t atSingleton_Find_90D0(void* ptr);

/**
 * game_94F0
 * @ 0x820F94F0 | size: 0xA4
 * 
 * Finds singleton index in registry.
 */
int32_t game_94F0(void* ptr);

#ifdef __cplusplus
}
#endif
