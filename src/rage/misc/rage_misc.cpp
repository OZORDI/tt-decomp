/**
 * rage_misc.cpp — RAGE Engine Miscellaneous Systems
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements miniheap allocator and other utility classes.
 */

#include "rage_misc.hpp"
#include <cstdint>

namespace {
    // Global miniheap state structure @ 0x82402808
    struct MiniheapState {
        uint32_t start;    // +0x00 @ 0x82402808 - heap start address
        uint32_t current;  // +0x04 @ 0x8240280C - current allocation pointer
        uint32_t end;      // +0x08 @ 0x82402810 - heap end address
    };
    
    // Global miniheap instance
    extern MiniheapState g_miniheap;
}

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// miniheapAllocator::ScalarDtor() [vtable slot 1 @ 0x823F89B8]
// Allocates memory from the miniheap with alignment
// ─────────────────────────────────────────────────────────────────────────────
void* miniheapAllocator::Allocate(uint32_t size, uint32_t alignment) {
    // Determine alignment mask based on size
    uint32_t alignMask;
    if (size <= 4) {
        alignMask = 3;  // 4-byte alignment
    } else if (size <= 8) {
        alignMask = 7;  // 8-byte alignment
    } else {
        alignMask = alignment - 1;  // Custom alignment
    }
    
    // Align current pointer
    uint32_t aligned = (g_miniheap.current + alignMask) & ~alignMask;
    
    // Update current pointer
    uint32_t newCurrent = aligned + size;
    g_miniheap.current = newCurrent;
    
    // Check for overflow
    if (newCurrent > g_miniheap.end) {
        // Call error handler (nop in current implementation)
        // Original: calls function @ 0x8240E6D0 with string @ 0x82074BB4
    }
    
    return reinterpret_cast<void*>(aligned);
}

// ─────────────────────────────────────────────────────────────────────────────
// miniheapAllocator::vfn_4() [vtable slot 4 @ 0x823F8958]
// Returns the amount of memory currently used in the miniheap
// ─────────────────────────────────────────────────────────────────────────────
uint32_t miniheapAllocator::GetUsedSize() const {
    return g_miniheap.current - g_miniheap.start;
}

// ─────────────────────────────────────────────────────────────────────────────
// miniheapAllocator::vfn_5() [vtable slot 5 @ 0x823F8970]
// Returns the amount of free memory remaining in the miniheap
// ─────────────────────────────────────────────────────────────────────────────
uint32_t miniheapAllocator::GetFreeSize() const {
    return g_miniheap.end - g_miniheap.current;
}

// ─────────────────────────────────────────────────────────────────────────────
// miniheapAllocator::vfn_20() [vtable slot 20 @ 0x823F8988]
// Checks if an address is within the miniheap range
// ─────────────────────────────────────────────────────────────────────────────
bool miniheapAllocator::IsInRange(uint32_t address) const {
    return (address >= g_miniheap.start) && (address < g_miniheap.end);
}

} // namespace rage
