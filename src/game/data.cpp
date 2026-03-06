/**
 * data.cpp — Player property manager implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implements plrPropMgr class for managing player properties and assets.
 */

#include "data/gd_data.hpp"
#include "rage/memory.h"
#include <cstdint>
#include <cstring>

// External function declarations
extern "C" {
    void atSingleton_9420(void* obj);
    void rage_free_00C0(void* ptr);
    void game_8F58(void* obj, const char* name, void* dest, void* defaultVal, int flags);
    void game_8FB0(void* obj, int flags);
    void game_8EE8(void* obj);
    void util_6C20(void* obj, uint32_t flags);
}

// External globals
extern uint32_t lbl_825C2D7C;
extern uint32_t lbl_825C93D0;
extern uint32_t lbl_825CAF90;
extern uint32_t lbl_826066C0;

// String constants
static const char* s_propName1 = "prop1";  // @ 0x820716A8
static const char* s_propName2 = "prop2";  // @ 0x820716B8
static const char* s_validateStr = "validate";  // @ 0x8207169C

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::~plrPropMgr()  [vtable slot 0 @ 0x823D45C0]
// Destructor - cleans up resources and calls parent cleanup
// ─────────────────────────────────────────────────────────────────────────────
plrPropMgr::~plrPropMgr() {
    // Set vtable pointer to plrPropMgr vtable
    this->vtable = (void**)0x820717C4;
    
    // Call cleanup function (vfn_24)
    this->vfn_24();
    
    // Set vtable to atSingleton vtable
    this->vtable = (void**)0x821A9420;
    
    // Call atSingleton cleanup
    atSingleton_9420(this);
    
    // Free memory if needed
    // Note: The original checks a flag in r30 (second parameter)
    // but for the destructor we always free
    rage_free_00C0(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::PostLoadProperties()  [vtable slot 20 @ 0x823D4630]
// Validates property names against known values
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::PostLoadProperties() {
    // Check if parameter matches first known value
    if (m_propertyId == lbl_825C2D7C) {
        return 1;
    }
    
    // Check if parameter matches second known value
    if (m_propertyId == lbl_825C93D0) {
        return 1;
    }
    
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::Validate()  [vtable slot 21 @ 0x823D4730]
// Loads and validates property data from configuration
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::Validate() {
    // Load first property
    game_8F58(this, s_propName1, &m_prop1, &lbl_825CAF90, 0);
    
    // Load second property
    game_8F58(this, s_propName2, &m_prop2, &lbl_825CAF90, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::PostLoadChildren()  [vtable slot 22 @ 0x823D4668]
// Returns validation string constant
// ─────────────────────────────────────────────────────────────────────────────
void* plrPropMgr::PostLoadChildren() {
    return (void*)s_validateStr;
}

// ─────────────────────────────────────────────────────────────────────────────
// plrPropMgr::vfn_23()  [vtable slot 23 @ 0x823D47A0]
// Initializes property assets and stores manager reference
// ─────────────────────────────────────────────────────────────────────────────
void plrPropMgr::vfn_23() {
    // Initialize first asset from m_prop1
    m_assets[0] = game_8FB0(m_prop1, 0);
    
    // Initialize remaining 2 assets from m_prop2
    for (int i = 0; i < 2; i++) {
        m_assets[i + 1] = game_8FB0(m_prop2, 0);
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
            // Call asset cleanup
            game_8EE8(asset);
            
            // Free asset memory with specific flags
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
// Sets flags on related objects
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
