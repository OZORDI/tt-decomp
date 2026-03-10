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

extern "C" {
    void xe_8E30(void* ptr, uint32_t count);
    void* game_DAF0(void* globalObj, uint32_t arg1);
    void nop_8240E6D0(const char* fmt, uint32_t arg);
    int32_t pg_C4E8_g(int32_t val, int32_t min, int32_t max);
}

// Global pointer for gdShotSet related arrays
extern uint32_t lbl_8271A314;

// -----------------------------------------------------------------------------
// gdShotSet::PostLoadChildren()  [vtable slot 22 @ 0x821DCF88]
// Returns the identifier string for attachment data.
// -----------------------------------------------------------------------------
const char* gdShotSet::PostLoadChildren() {
    return "AttachmentData";
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_23()  [vtable slot 23 @ 0x821DD1D8]
// Returns the number of items in the primary data array.
// -----------------------------------------------------------------------------
uint32_t gdShotSet::vfn_23() {
    return m_dataCount1;
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_24()  [vtable slot 24 @ 0x821DD1E0]
// Retrieves an item from the related global array using an index from m_pDataArray2.
// -----------------------------------------------------------------------------
uint32_t gdShotSet::vfn_24(uint32_t index) {
    uint32_t valIndex = m_pDataArray2[index];
    if (valIndex == 0xFFFFFFFF) {
        return 0;
    }
    
    uint32_t* globalData = (uint32_t*)lbl_8271A314;
    uint32_t* globalArray = (uint32_t*)globalData[9]; // +36
    return globalArray[valIndex];
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_25()  [vtable slot 25 @ 0x821DD218]
// Retrieves the raw index value from m_pDataArray2.
// -----------------------------------------------------------------------------
uint32_t gdShotSet::vfn_25(uint32_t index) {
    return m_pDataArray2[index];
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_26()  [vtable slot 26 @ 0x821DD228]
// Retrieves a related pointer, falling back to m_pDataArray1 if the index is invalid (-1).
// -----------------------------------------------------------------------------
void* gdShotSet::vfn_26(uint32_t index) {
    uint32_t valIndex = m_pDataArray2[index];
    if (valIndex == 0xFFFFFFFF) {
        uint32_t* obj = (uint32_t*)m_pDataArray1[index];
        return (void*)obj[4]; // +16
    }
    
    uint32_t* globalData = (uint32_t*)lbl_8271A314;
    uint32_t** globalArray = (uint32_t**)globalData[9]; // +36
    uint32_t* obj = globalArray[valIndex];
    return (void*)obj[4]; // +16
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_27()  [vtable slot 27 @ 0x821DD268]
// Retrieves the index value for the current active item.
// -----------------------------------------------------------------------------
uint32_t gdShotSet::vfn_27() {
    return vfn_25(m_currentIdx);
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_28()  [vtable slot 28 @ 0x821DD280]
// Retrieves the related pointer for the current active item.
// -----------------------------------------------------------------------------
void* gdShotSet::vfn_28() {
    return vfn_26(m_currentIdx);
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_29()  [vtable slot 29 @ 0x821DD318]
// Sets the current active item index.
// -----------------------------------------------------------------------------
void gdShotSet::vfn_29(uint32_t idx) {
    m_currentIdx = idx;
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_30()  [vtable slot 30 @ 0x821DD320]
// Processes and instantiates attachment data for the elements in the set.
// -----------------------------------------------------------------------------
void gdShotSet::vfn_30() {
    xe_8E30(&m_pDataArray2, m_dataCount1);
    
    if (m_dataCount1 == 0) {
        return;
    }
    
    void* pGlobal = (void*)lbl_8271A314;
    const char* errorFormat = "gdCrAttachmentData::PostLoadCreature() - bone '%s' does not exist";
    
    for (uint32_t i = 0; i < m_dataCount1; i++) {
        uint32_t* pItem1 = (uint32_t*)m_pDataArray1[i];
        void* result = game_DAF0(pGlobal, pItem1[4]); // +16
        
        uint16_t count2 = m_dataCount2;
        m_dataCount2 = count2 + 1;
        m_pDataArray2[count2] = (uint32_t)result;
        
        if (m_pDataArray2[i] == 0xFFFFFFFF) {
            uint32_t* pItem2 = (uint32_t*)m_pDataArray1[i];
            nop_8240E6D0(errorFormat, pItem2[4]); // +16
        }
    }
}

// -----------------------------------------------------------------------------
// gdShotSet::vfn_31()  [vtable slot 31 @ 0x821DD3C8]
// Updates the state using pg_C4E8_g if active.
// -----------------------------------------------------------------------------
bool gdShotSet::vfn_31() {
    if (m_bIsActive) {
        m_currentIdx = (uint32_t)pg_C4E8_g(m_currentIdx + 1, 0, m_dataCount2 - 1);
    }
    return m_bIsActive;
}
