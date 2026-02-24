#pragma once

#include <cstddef>
#include <cstdint>

namespace rage {

/**
 * Allocator
 * 
 * Base class for RAGE engine memory allocators.
 * The engine uses a vtable-based allocator system where different
 * allocators can be swapped in for different contexts.
 * 
 * Original vtable slots:
 * [0] Destructor/Bind
 * [1] Allocate(size, alignment)
 * [2] Free(ptr)
 * [3] Release(buffer)
 * [6] InnerOp(inner, arg)
 */
class Allocator {
public:
    virtual ~Allocator() {}
    
    /**
     * Allocate memory with specified size and alignment.
     * 
     * @param size Size in bytes to allocate
     * @param alignment Alignment requirement (typically 16 bytes)
     * @return Pointer to allocated memory, or nullptr on failure
     */
    virtual void* Allocate(size_t size, size_t alignment = 16) = 0;
    
    /**
     * Free previously allocated memory.
     * 
     * @param ptr Pointer to memory to free
     */
    virtual void Free(void* ptr) = 0;
    
    /**
     * Release a buffer back to the allocator pool.
     * Similar to Free but may have different semantics for pooled allocators.
     * 
     * @param buffer Buffer to release
     */
    virtual void Release(void* buffer) = 0;
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
