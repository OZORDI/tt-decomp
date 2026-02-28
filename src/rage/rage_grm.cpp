/**
 * rage_grm.cpp — Graphics / rendering class implementations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#include "rage_grm.hpp"

// Forward declarations for external functions
extern "C" void fragDrawable_0AA0_2h(void* geom, void* drawable, uint32_t index, void* param);

namespace rage {

// ── rage::spdApical ───────────────────────────────────────────────────────────

void spdApical::vfn_2(float value) {
    unk_10 = value;
}

void spdApical::vfn_3(float value) {
    unk_14 = value;
}

void spdApical::vfn_4(float value) {
    unk_18 = value;
}

void spdApical::vfn_5(float value) {
    unk_1C = value;
}

// ── rage::fragDrawable ────────────────────────────────────────────────────────

/**
 * fragDrawable::ProcessGeometry
 * @ 0x820F1C00 | size: 0x17c
 *
 * Original symbol: fragDrawable_1C00_fw
 * Subsystem: RAGE Graphics / Fragment Drawable
 *
 * Iterates through drawable geometry and processes each one based on a filter criterion.
 * Calls virtual method slot 4 on each geometry object that matches the filter type.
 * 
 * Parameters:
 *   geometryContainer - Object containing geometry array at offset +8
 *   param1 - Parameter passed to geometry virtual method
 *   param2 - Parameter passed to geometry virtual method
 *   filterType - Type filter for geometry processing (extracted from flags bits 27-31)
 *   param3 - Additional parameter passed to geometry virtual method
 */
void fragDrawable::ProcessGeometry(void* geometryContainer, void* param1, void* param2,
                                   uint32_t filterType, void* param3) {
    if (m_geometryCount <= 0) {
        return;
    }
    
    // Global state pointers (SDA-relative globals)
    extern uint32_t* g_iccProfilePtr;      // @ 0x825EB1D0 (r25 + -20016)
    extern void* g_currentGeometry;        // @ 0x82606350 (r26 + 25424)
    
    uint32_t* statePtr = g_iccProfilePtr;
    void* currentGeom = g_currentGeometry;
    
    // Get geometry array from container
    void** geometryArray = (void**)((uint8_t*)geometryContainer + 8);
    
    for (uint16_t i = 0; i < m_geometryCount; i++) {
        void* geom = nullptr;
        uint16_t flags = 0;
        
        // Determine geometry object and its flags
        if (currentGeom != nullptr) {
            geom = currentGeom;
            flags = *(uint16_t*)((uint8_t*)geom + 4);
            
            // Check if bit 0 is set (active flag)
            bool isActive = (flags & 0x1) != 0;
            if (!isActive) {
                // Extract type from flags (bits 27-31 after rotation)
                uint32_t geomType = (flags >> 5) & 0x1F;
                
                // Check if type matches filter
                if (geomType != filterType) {
                    continue;
                }
            }
        } else {
            // Load geometry from array using index
            uint16_t geomIndex = m_pIndices[i];
            geom = geometryArray[geomIndex];
            flags = *(uint16_t*)((uint8_t*)geom + 4);
            
            // Extract type from flags
            uint32_t geomType = (flags >> 5) & 0x1F;
            
            // Check if type matches filter
            if (geomType != filterType) {
                continue;
            }
        }
        
        // Fast path: if state pointer is set, call helper function
        if (statePtr != nullptr) {
            fragDrawable_0AA0_2h(geom, this, i, param3);
            continue;
        }
        
        // Determine if this is the last geometry in a sequence
        bool isLast = false;
        
        if (currentGeom == nullptr) {
            // Check if next geometry has same ID
            if (i + 1 < m_geometryCount) {
                uint16_t nextIndex = m_pIndices[i + 1];
                void* nextGeom = geometryArray[nextIndex];
                
                uint16_t currentId = *(uint16_t*)((uint8_t*)geom + 6);
                uint16_t nextId = *(uint16_t*)((uint8_t*)nextGeom + 6);
                
                isLast = (currentId != nextId);
            } else {
                isLast = true;
            }
        } else {
            isLast = true;
        }
        
        // Determine final isLast flag
        bool finalIsLast = (i + 1 == m_geometryCount) || isLast;
        
        // Call virtual method at slot 4 (offset +16 in vtable)
        void** vtable = *(void***)geom;
        typedef void (*GeomProcessFn)(void* geom, void* drawable, uint32_t index,
                                     void* param1, void* param2, void* param3,
                                     bool isLast);
        GeomProcessFn processFn = (GeomProcessFn)vtable[4];
        processFn(geom, this, i, param1, param2, param3, finalIsLast);
        
        // Reload global state after virtual call
        currentGeom = g_currentGeometry;
        statePtr = g_iccProfilePtr;
    }
}

} // namespace rage
