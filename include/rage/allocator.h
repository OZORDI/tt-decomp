#pragma once

#include <cstddef>
#include <cstdint>

namespace rage {

/**
 * Allocator (sysMemAllocator)
 *
 * Base class for RAGE engine memory allocators.
 * The engine uses a vtable-based allocator system where different
 * allocators can be swapped in for different contexts.
 *
 * Vtable layout (from recomp scaffold):
 *   slot  0 (+0x00) : destructor
 *   slot  1 (+0x04) : Allocate(ptr, size)
 *   slot  2 (+0x08) : Free(ptr)
 *   slots 3-16      : reserved / other methods
 *   slot 17 (+0x44) : IsAddressOwned() -> bool
 */
class Allocator {
public:
    virtual ~Allocator() {}

    /**
     * Allocate memory.
     *
     * @param ptr  Hint/base address for alignment (may be NULL)
     * @param size Size in bytes to allocate
     * @return Pointer to allocated memory, or nullptr on failure
     */
    virtual void* Allocate(void* ptr, size_t size) = 0;

    /**
     * Free previously allocated memory.
     *
     * @param ptr Pointer to memory to free
     */
    virtual void Free(void* ptr) = 0;
};

/**
 * Get the current thread's allocator context.
 * 
 * This retrieves the allocator from TLS (Thread-Local Storage).
 * On Xbox 360, this was accessed via the r13 register.
 * 
 * @return Pointer to the current allocator
 */
Allocator* GetAllocator();

/**
 * Bind a temporary allocator context to the current thread.
 * 
 * This is used during subsystem initialization to temporarily
 * swap in a different allocator.
 * 
 * @param allocator Allocator to bind, or nullptr to unbind
 */
void BindAllocator(Allocator* allocator);

} // namespace rage
