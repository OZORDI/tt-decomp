/**
 * fiDevice.cpp — RAGE file device layer
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Implements the core file device lookup and stream open functions:
 *   fiDeviceMemory_AllocateDeviceMemory  — allocate from the memory device
 *   fiDevice_GetDevice                   @ 0x822E12A0 | size: 0x244
 *   fiStream_Open                        @ 0x822E3518 | size: 0x134
 *
 * The file device system uses a global device table at 0x8262F368 that maps
 * path prefixes ("memory:", "tcpip:", "cache:", "game:") to fiDevice instances.
 * Each device entry is 276 bytes and contains a mount-point string, a priority
 * half-word at +264, and a device pointer array at +268.
 */

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

extern "C" {

// ── External dependencies ───────────────────────────────────────────────────

// Global device table: array of fiDevice entries
// @ 0x8262F368 (.data, 16 bytes header)
//   +0x00  void*    m_pEntries     — base pointer to entry array
//   +0x04  uint16_t m_nCount       — number of registered devices
//   +0x06  uint16_t m_nCapacity    — allocated capacity
extern uint8_t lbl_8262F368[];  // device table header

// Named device singletons (returned for known prefixes)
// @ 0x825D0074 — fiDeviceMemory singleton (for "memory:" prefix)
// @ 0x825D06C4 — fiDeviceTcpIp singleton (for "tcpip:" prefix)
// @ 0x825D0070 — fiDeviceLocal singleton (for "cache:" and "game:" prefixes)
extern void* lbl_825D0074;  // g_pMemoryDevice
extern void* lbl_825D06C4;  // g_pTcpIpDevice
extern void* lbl_825D0070;  // g_pLocalDevice

// Fallback device pointer
// @ 0x825C481C (.data, 4 bytes)
extern void* lbl_825C481C;  // g_pFallbackDevice

// Debug log (no-op in release)
extern void nop_8240E6D0(const char* fmt, ...);

// File stream callback pointer
// @ 0x825F5CF0 (.data, 4 bytes)
extern void* lbl_825F5CF0;  // g_pFileOpenCallback

// Internal helpers
extern int  pg_1218(void* entry, const char* path, uint16_t priority);
extern void ref_fi_34C0(const char* path);
extern void pg_3378(const char* path, int handle, void* device);

/**
 * fiDeviceMemory_AllocateDeviceMemory @ synthetic
 *
 * Allocates a block of memory from the memory device. In the original
 * Xbox 360 binary this goes through the physical memory allocator;
 * on the host platform we use standard malloc.
 */
void* fiDeviceMemory_AllocateDeviceMemory(size_t size) {
    if (size == 0)
        return nullptr;
    return std::malloc(size);
}

/**
 * fiDevice_GetDevice @ 0x822E12A0 | size: 0x244
 *
 * Resolves a file path to the appropriate fiDevice instance.
 *
 * Fast-path prefix matching (strncmp):
 *   "memory:" (7 chars) → g_pMemoryDevice  @ 0x825D0074
 *   "tcpip:"  (6 chars) → g_pTcpIpDevice   @ 0x825D06C4
 *   "cache:"  (6 chars) → g_pLocalDevice    @ 0x825D0070
 *   "game:"   (5 chars) → g_pLocalDevice    @ 0x825D0070
 *
 * If no prefix matches, walks the global device table looking for the
 * best-priority mount that claims the path. Falls back to g_pFallbackDevice
 * if no mount matches.
 */
void* fiDevice_GetDevice(const char* path) {
    if (!path)
        return nullptr;

    // Fast-path: known prefixes
    if (std::strncmp(path, "memory:", 7) == 0)
        return &lbl_825D0074;

    if (std::strncmp(path, "tcpip:", 6) == 0)
        return &lbl_825D06C4;

    if (std::strncmp(path, "cache:", 6) == 0)
        return &lbl_825D0070;

    if (std::strncmp(path, "game:", 5) == 0)
        return &lbl_825D0070;

    // Slow path: walk the device table
    uint8_t* tableHeader = lbl_8262F368;
    void*    pEntries    = *reinterpret_cast<void**>(tableHeader + 0);
    uint16_t count       = *reinterpret_cast<uint16_t*>(tableHeader + 4);

    int32_t bestPriority = 0;
    int32_t bestIndex    = -1;

    if (count > 0 && pEntries != nullptr) {
        uint8_t* entryBase = static_cast<uint8_t*>(pEntries);
        for (int32_t i = 0; i < count; i++) {
            // Each entry is 276 bytes; priority half-word at +264
            uint8_t* entry = entryBase + i * 276;
            uint16_t priority = *reinterpret_cast<uint16_t*>(entry + 264);

            int match = pg_1218(entry, path, priority);
            if (match == 0 && static_cast<int32_t>(priority) > bestPriority) {
                bestPriority = static_cast<int32_t>(priority);
                bestIndex = i;
            }
        }
    }

    void* result = nullptr;

    if (bestIndex != -1) {
        uint8_t* entryBase = static_cast<uint8_t*>(pEntries);
        uint8_t* bestEntry = entryBase + bestIndex * 276;
        uint16_t devType = *reinterpret_cast<uint16_t*>(bestEntry + 272);

        if (devType == 1) {
            // Direct device pointer
            void** ppDevice = *reinterpret_cast<void***>(bestEntry + 268);
            result = *ppDevice;
        } else {
            // Walk the device pointer array backwards
            int32_t idx = devType - 1;
            void** ppDevArray = *reinterpret_cast<void***>(bestEntry + 268);

            while (idx >= 0 && ppDevArray != nullptr) {
                void* dev = ppDevArray[idx];
                // Call vtable slot 1 (Open) to test if device claims this path
                typedef int (*OpenFn)(void*, const char*, int);
                void** vtable = *reinterpret_cast<void***>(&dev);
                OpenFn openFn = reinterpret_cast<OpenFn>(vtable[1]);
                int handle = openFn(dev, path, 1);

                if (handle != -1) {
                    // Found a device that claims this path — close the test handle
                    typedef void (*CloseFn)(void*, int);
                    CloseFn closeFn = reinterpret_cast<CloseFn>(vtable[6]);
                    closeFn(dev, handle);

                    // Reload and return the device
                    pEntries = *reinterpret_cast<void**>(tableHeader + 0);
                    bestEntry = static_cast<uint8_t*>(pEntries) + bestIndex * 276;
                    ppDevArray = *reinterpret_cast<void***>(bestEntry + 268);
                    result = ppDevArray[idx];
                    break;
                }

                // Reload table pointer (may have been modified)
                pEntries = *reinterpret_cast<void**>(tableHeader + 0);
                idx--;
            }
        }
    } else {
        // No mount matched — use fallback device
        result = lbl_825C481C;
        if (result == nullptr) {
            nop_8240E6D0("fiDevice::GetDevice: no device found for '%s'", path);
        }
    }

    return result;
}

/**
 * fiStream_Open @ 0x822E3518 | size: 0x134
 *
 * Opens a file stream by resolving the path to a device, then calling
 * the device's Open vtable method. If the open succeeds, constructs a
 * fiStream wrapper around the device handle.
 *
 * Parameters:
 *   path — file path (e.g. "game:/data/config.xml")
 *   mode — open mode (0 = read, 1 = write, etc.)
 *
 * Returns: pointer to the fiStream object, or calls ref_fi_34C0 on failure.
 */
void* fiStream_Open(const char* path, int mode) {
    if (!path)
        return nullptr;

    // Resolve path to a device
    void* device = fiDevice_GetDevice(path);
    if (device == nullptr) {
        ref_fi_34C0(path);
        return nullptr;
    }

    // Call device vtable slot 1 (Open)
    void** vtable = *reinterpret_cast<void***>(&device);
    typedef int (*OpenFn)(void*, const char*, int);
    OpenFn openFn = reinterpret_cast<OpenFn>(vtable[1]);
    int handle = openFn(device, path, mode);

    bool success = (handle != -1);

    // Debug log the open attempt
    nop_8240E6D0("fiStream::Open(%s, %d) = %s",
                  path, mode, success ? "OK" : "FAIL");

    if (!success) {
        ref_fi_34C0(path);
        return nullptr;
    }

    // Check file-open callback
    if (lbl_825F5CF0 != nullptr) {
        typedef uint8_t (*CallbackFn)(const char*, int);
        CallbackFn callback = reinterpret_cast<CallbackFn>(lbl_825F5CF0);
        uint8_t allowed = callback(path, mode);

        if (allowed == 0) {
            // Callback rejected — close the handle and fail
            typedef void (*CloseFn)(void*, int);
            CloseFn closeFn = reinterpret_cast<CloseFn>(vtable[6]);
            closeFn(device, handle);
            ref_fi_34C0(path);
            return nullptr;
        }
    }

    // Construct the fiStream wrapper
    pg_3378(path, handle, device);
    return nullptr;  // pg_3378 handles the return via r3
}

} // extern "C"
