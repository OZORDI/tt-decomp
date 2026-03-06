/**
 * data.cpp — Player property manager implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements plrPropMgr class for managing player properties and assets.
 */

#include "data/gd_data.hpp"
#include <cstdint>

// External function declarations
extern "C" {
    void atSingleton_9420(void* obj);
    void rage_free_00C0(void* ptr);
    void game_8F58(void* obj, const char* name, void* dest, void* defaultVal, int flags);
    void* game_8FB0(void* obj, int flags);
    void game_8EE8(void* obj);
    void util_6C20(void* obj, uint32_t flags);
}

// External globals
extern uint32_t lbl_825C2D7C;
extern uint32_t lbl_825C93D0;
extern uint32_t lbl_825CAF90;
extern uint32_t lbl_826066C0;

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::~plrPropMgr()  [vtable slot 0 @ 0x823D45C0]
// Destructor - cleans up resources and calls parent cleanup
// ─────────────────────────────────────────────────────────────────────────────
plrPropMgr::~plrPropMgr() {
    // Update vtable to plrPropMgr
    this->vtable = (void**)0x820717C4;
    
    // Clean up assets
    this->vfn_24();
    
    // Update vtable to atSingleton
    this->vtable = (void**)0x821A9420;
    
    // Call atSingleton destructor
    atSingleton_9420(this);
    
    // Note: The original checks a flag (r30 & 1) to decide whether to free
    // For now, we always call free (the flag comes from the calling convention)
    rage_free_00C0(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::PostLoadProperties()  [vtable slot 20 @ 0x823D4630]
// Validates property ID against known values
// Returns 1 if valid, 0 otherwise
// ─────────────────────────────────────────────────────────────────────────────
uint8_t plrPropMgr::PostLoadProperties() {
    // Check against first known property ID
    if (m_propertyId == lbl_825C2D7C) {
        return 1;
    }
    
    // Check against second known property ID
    if (m_propertyId == lbl_825C93D0) {
        return 1;
    }
    
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::Validate()  [vtable slot 21 @ 0x823D4730]
// Loads property data from configuration
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::Validate() {
    // Load first property with default value
    game_8F58(this, (const char*)0x820716A8, &m_prop1, &lbl_825CAF90, 0);
    
    // Load second property with default value
    game_8F58(this, (const char*)0x820716B8, &m_prop2, &lbl_825CAF90, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::PostLoadChildren()  [vtable slot 22 @ 0x823D4668]
// Returns pointer to validation string constant
// ─────────────────────────────────────────────────────────────────────────────
void* plrPropMgr::PostLoadChildren() {
    return (void*)0x8207169C;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::vfn_23()  [vtable slot 23 @ 0x823D47A0]
// Initializes property assets and stores global manager reference
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::vfn_23() {
    // Initialize first asset from m_prop1
    m_assets[0] = game_8FB0(m_prop1, 0);
    
    // Initialize remaining 2 assets from m_prop2 in a loop
    for (int i = 1; i < 3; i++) {
        m_assets[i] = game_8FB0(m_prop2, 0);
    }
    
    // Store global reference to this manager
    lbl_826066C0 = (uint32_t)this;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::vfn_24()  [vtable slot 24 @ 0x823D47F8]
// Cleans up all asset resources
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::vfn_24() {
    // Clean up all 3 assets
    for (int i = 0; i < 3; i++) {
        void* asset = m_assets[i];
        if (asset) {
            // Call asset cleanup function
            game_8EE8(asset);
            
            // Free asset memory with specific flags (0xFFFFE001)
            util_6C20(asset, 0xFFFFE001);
        }
    }
    
    // Clear all asset pointers
    m_assets[0] = nullptr;
    m_assets[1] = nullptr;
    m_assets[2] = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::vfn_25()  [vtable slot 25 @ 0x823D4860]
// Sets flags on related objects at specific offsets
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::vfn_25() {
    // Set flags on first related object
    if (m_relatedObj1) {
        uint8_t* obj = (uint8_t*)m_relatedObj1;
        obj[288] = 1;
        obj[290] = 1;
        obj[291] = 1;
    }
    
    // Set flags on second related object
    if (m_relatedObj2) {
        uint8_t* obj = (uint8_t*)m_relatedObj2;
        obj[288] = 1;
        obj[290] = 1;
        obj[291] = 1;
    }
}
