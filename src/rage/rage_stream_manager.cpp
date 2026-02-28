/**
 * rage_stream_manager.cpp — RAGE stream buffer manager implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include <cstdint>

// Forward declarations
struct fiStreamBuf;

// External function from game_loop.c
extern "C" int fiStreamBuf_FetchChunk(fiStreamBuf* pBuf);

/**
 * fiStreamBuf_Reset @ 0x822E3AC8 | size: 0x6C
 * 
 * Resets a fiStreamBuf object by flushing any pending data and
 * calling the virtual reset method.
 * 
 * This function:
 * 1. Calls fiStreamBuf_FetchChunk to flush buffered data
 * 2. If successful, stores the reset parameter at offset +12
 * 3. Calls vtable[5] to perform device-specific reset
 * 
 * Parameters:
 *   pBuf       - Pointer to fiStreamBuf object
 *   resetParam - Reset parameter (typically 0)
 * 
 * Returns:
 *   0 on success, -1 on failure
 */
extern "C" int fiStreamBuf_Reset(fiStreamBuf* pBuf, int resetParam) {
    // Flush any pending data in the stream buffer
    int result = fiStreamBuf_FetchChunk(pBuf);
    
    if (result < 0) {
        return -1;
    }
    
    // Access the buffer as byte array for offset-based access
    uint8_t* bufBytes = reinterpret_cast<uint8_t*>(pBuf);
    
    // Get vtable and flags for virtual call
    void** vtable = *reinterpret_cast<void***>(bufBytes + 0);
    uint32_t flags = *reinterpret_cast<uint32_t*>(bufBytes + 4);
    
    // Store reset parameter at offset +12 (writePos in fiStreamBuf)
    *reinterpret_cast<int32_t*>(bufBytes + 12) = resetParam;
    
    // Call vtable[5] - device-specific reset/commit operation
    // Signature: int (*fn)(void* this, uint32_t flags, void* buffer, int offset, int size)
    typedef int (*VTableFn5)(void*, uint32_t, void*, int, int);
    VTableFn5 resetFn = reinterpret_cast<VTableFn5>(vtable[5]);
    resetFn(pBuf, flags, nullptr, 0, 0);
    
    return 0;
}

/**
 * rage_52B0_1 @ 0x822E52B0 | size: 0x5C
 * 
 * Initializes a rageStreamManager object to default state.
 * 
 * This function sets up the stream manager with:
 * - State = 1 (initialized/active)
 * - Capacity = 32 (buffer size limit)
 * - Mode = 2 (default operation mode)
 * - Counter = 0 (reset operation counter)
 * - Active flag = 0 (not currently active)
 * 
 * It also resets the nested fiStreamBuf by calling fiStreamBuf_Reset.
 * 
 * Called by ph_9398 (physics system) during initialization of
 * a large physics structure at offset 0xE6B0.
 * 
 * Parameters:
 *   pManager - Pointer to rageStreamManager object (this pointer)
 */
extern "C" void rage_52B0_1(void* thisPtr) {
    uint8_t* manager = static_cast<uint8_t*>(thisPtr);
    
    // Get the nested fiStreamBuf pointer at offset +12
    fiStreamBuf* pStreamBuf = *reinterpret_cast<fiStreamBuf**>(manager + 12);
    
    // Reset the stream buffer (flush and reinitialize)
    fiStreamBuf_Reset(pStreamBuf, 0);
    
    // Initialize manager state fields
    *reinterpret_cast<uint32_t*>(manager + 24) = 0;      // m_counter = 0
    *reinterpret_cast<uint32_t*>(manager + 8) = 1;       // m_state = 1 (initialized)
    *reinterpret_cast<uint32_t*>(manager + 16) = 32;     // m_capacity = 32
    *reinterpret_cast<uint8_t*>(manager + 28) = 0;       // m_bActive = false
    *reinterpret_cast<uint32_t*>(manager + 156) = 0;     // field_9C = 0
    *reinterpret_cast<uint32_t*>(manager + 20) = 2;      // m_mode = 2 (default)
}
