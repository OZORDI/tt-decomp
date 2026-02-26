/**
 * char_view.cpp — Character View System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implementation of character viewing/selection UI classes.
 * Handles character display, camera control, and UI state management.
 */

#include "game/char_view.hpp"
#include <cstring>

// External function declarations
extern "C" {
    void rage_free_00C0(void* ptr);
    void* xe_EC88(uint32_t size);
    void xe_main_thread_init_0038();
    void atSingleton_9420(void* obj);
    void util_1568(void* obj);
    void rage_7630(void* obj);
    void rage_8070(void* obj);
    void xmlNodeStruct_SerializeField(void* obj, const char* name, void* target, void* defaultVal, uint32_t flags);
    int32_t util_2458_FindCharacterIndex(void* gameData, const char* name);
    void nop_8240E6D0(const char* message, void* a, void* b);
    void pg_E6E0(int code, int param1, int param2, int param3);
    void game_28B8(void* manager, int32_t eventType);
    void pg_61E8_g(void* context, float param, int p2, int p3, int p4, int p5);
}

// Global data references
extern uint32_t* g_characterTypeIds;      // @ 0x825C2BC0
extern float g_defaultFloatZero;          // @ 0x825CAF94
extern float g_defaultFloatValue;         // @ 0x825CAF88

// ────────────────────────────────────────────────────────────────────────────
// pongAttractState Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongAttractState::GetContext @ 0x823057E8 | size: 0x18
 * 
 * Returns the UI context object by forwarding to the screen object's vtable.
 */
void* pongAttractState::GetContext() {
    if (!m_pScreenObject) {
        return nullptr;
    }
    
    // Forward to screen object's GetContext method (vtable slot 4)
    typedef void* (*GetContextFunc)(void*);
    void** vtable = *(void***)m_pScreenObject;
    GetContextFunc getContext = (GetContextFunc)vtable[4];
    return getContext(m_pScreenObject);
}

/**
 * pongAttractState::OnExit @ 0x82306EA8 | size: 0x6C
 * 
 * Called when leaving this state - cleans up the UI screen object.
 */
void pongAttractState::OnExit() {
    if (!m_pScreenObject) {
        return;
    }
    
    // Call Release/Cleanup on screen object (vtable slot 5)
    void** vtable = *(void***)m_pScreenObject;
    typedef void (*ReleaseFunc)(void*);
    ReleaseFunc release = (ReleaseFunc)vtable[5];
    release(m_pScreenObject);
    
    // Call destructor with delete flag (vtable slot 0)
    typedef void (*DestructorFunc)(void*, bool);
    DestructorFunc dtor = (DestructorFunc)vtable[0];
    dtor(m_pScreenObject, true);
    
    m_pScreenObject = nullptr;
}

/**
 * pongAttractState::OnEvent @ 0x823058E0 | size: 0xC8
 * 
 * Handles input and system events.
 * Event 6: Creates UI state
 * Events 9, 12: No-op
 * Other events: Forwarded to manager
 */
void pongAttractState::OnEvent(int32_t eventType) {
    // Call some initialization function
    pg_E6E0(2055, 64, 0, 0);
    
    if (eventType == 6) {
        // Special event - create UI state
        // Set up global UI state
        uint32_t* pGlobalUIState = (uint32_t*)0x8271A348;  // Global UI state array
        pGlobalUIState[13] = 2;  // State type
        pGlobalUIState[14] = (uint32_t)m_pScreenObject;
        
        // Call UI initialization
        void* pUIContext = (void*)0x8271A81C;
        pg_61E8_g(pUIContext, 0.0f, 1, 0, 0, 0);
        
        // Clear flag on screen object
        if (m_pScreenObject) {
            *(uint8_t*)((char*)m_pScreenObject + 28) = 0;
        }
    } else if (eventType == 9 || eventType == 12) {
        // No-op events
        return;
    } else {
        // Forward event to manager
        game_28B8(m_pManager, eventType);
        
        // Log unhandled event
        const char* errorMsg = (const char*)0x8205E110;  // Error message string
        nop_8240E6D0(errorMsg, m_pManager, (void*)(intptr_t)eventType);
    }
}

/**
 * pongAttractState::Update @ 0x823059A8 | size: 0xB4
 * 
 * Frame update - called every frame.
 */
void pongAttractState::Update(float deltaTime) {
    // TODO: Implement update logic
    // This is a base implementation that derived classes may override
}

/**
 * pongAttractState::GetStateId @ 0x82305A60 | size: 0xC
 * 
 * Returns state identifier.
 */
int32_t pongAttractState::GetStateId() const {
    // TODO: Return proper state ID
    return 0;
}

/**
 * pongAttractState::OnEnter @ 0x82305800 | size: 0xE0
 * 
 * Called when entering this state - allocates and initializes UI screen object.
 */
void pongAttractState::OnEnter() {
    xe_main_thread_init_0038();
    
    // Get allocator from TLS
    void** pTLS = *(void***)0x82600000;  // TLS base (r13)
    void* pAllocator = pTLS[1];
    
    // Allocate 32-byte screen object
    typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
    void** allocVtable = *(void***)pAllocator;
    AllocFunc alloc = (AllocFunc)allocVtable[1];
    void* pScreen = alloc(pAllocator, 32, 16);
    
    if (pScreen) {
        // Initialize screen object
        memset(pScreen, 0, 32);
        
        // Set vtables
        *(void**)pScreen = (void*)0x8205E0F4;  // Primary vtable
        *(void**)((char*)pScreen + 20) = (void*)0x8205E108;  // Secondary vtable
        
        // Set back-pointer to state
        *(void**)((char*)pScreen + 24) = this;
        
        // Register with global UI system
        uint32_t* pGlobalUI = (uint32_t*)0x827F3864;
        uint32_t index = pGlobalUI[0];
        uint32_t* pUIArray1 = (uint32_t*)0x827F3868;
        uint32_t* pUIArray2 = (uint32_t*)0x827F3AC8;
        uint8_t* pUIFlags = (uint8_t*)0x827F3D28;
        
        pUIArray1[index] = 16;
        pUIArray2[index] = (uint32_t)((char*)pScreen + 20);
        pUIFlags[index] = 128;
        pGlobalUI[0] = index + 1;
        
        m_pScreenObject = pScreen;
    } else {
        m_pScreenObject = nullptr;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// charViewCharData Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * charViewCharData::IsCharacterType @ 0x8240B238 | size: 0x48
 * 
 * Checks if the provided type ID matches one of the known character types.
 */
bool charViewCharData::IsCharacterType(uint32_t typeId) {
    // Check against three known character type IDs
    uint32_t typeId1 = *(uint32_t*)0x825C2BC0;
    if (typeId == typeId1) {
        return true;
    }
    
    uint32_t typeId2 = *(uint32_t*)0x825C803C;
    if (typeId == typeId2) {
        return true;
    }
    
    uint32_t typeId3 = *(uint32_t*)0x825C8038;
    return (typeId == typeId3);
}

/**
 * charViewCharData::Serialize @ 0x8240B280 | size: 0x8C
 * 
 * Serializes character view data to XML or binary format.
 */
void charViewCharData::Serialize(void* serializer) {
    void* defaultVal = (void*)0x825CAF94;
    void* defaultVal2 = (void*)0x825CAF88;
    
    // Serialize three character-specific fields
    // TODO: Extract actual XML field names from string table
    xmlNodeStruct_SerializeField(this, "field_10", &m_field_10, defaultVal2, 0);
    xmlNodeStruct_SerializeField(this, "field_14", &m_field_14, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_18", &m_field_18, defaultVal, 0);
}

/**
 * charViewCharData::GetDataInfo @ 0x8230A4F8 | size: 0xC
 * 
 * Get character view data size or type info.
 */
uint32_t charViewCharData::GetDataInfo() {
    // TODO: Return proper data info
    return 0;
}

// ────────────────────────────────────────────────────────────────────────────
// charViewData Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * charViewData::~charViewData @ 0x8240B530 | size: 0x68
 * 
 * Destructor - frees the dynamically allocated view data array.
 */
charViewData::~charViewData() {
    // Free allocated array if it exists
    if (m_allocatedSize > 0 && m_pAllocatedData != nullptr) {
        rage_free_00C0(m_pAllocatedData);
    }
    
    // Call base class destructor
    atSingleton_9420(this);
}

/**
 * charViewData::Initialize @ 0x8240B598 | size: 0x24
 * 
 * Initialize or reset view data.
 */
void charViewData::Initialize() {
    // TODO: Implement initialization logic
}

/**
 * charViewData::LoadViewData @ 0x8240B5C0 | size: 0x1AC
 * 
 * Loads and initializes character view data from game data manager.
 */
void charViewData::LoadViewData() {
    struct GameDataMgr {
        uint8_t padding[28];
        uint32_t characterCount;  // +28
    };
    
    // Get character count from game data manager
    GameDataMgr** ppGameData = (GameDataMgr**)0x825CA174;
    GameDataMgr* pGameData = *ppGameData;
    uint32_t charCount = pGameData ? pGameData->characterCount : 0;
    
    // Allocate array for character view elements (2D positions)
    if (charCount > 0) {
        uint32_t allocSize = charCount * 4;  // 4 bytes per pointer
        if (charCount > 0x3FFFFFFF) {
            allocSize = 0xFFFFFFFF;
        }
        m_pAllocatedData = xe_EC88(allocSize);
    } else {
        m_pAllocatedData = nullptr;
    }
    
    m_allocatedSize = (uint16_t)charCount;
    m_currentCount = 0;
    
    // Initialize array elements with default values (2 floats per element: x, y)
    if (charCount > 0) {
        struct ViewElement {
            float x;
            float y;
        };
        
        ViewElement** pArray = (ViewElement**)m_pAllocatedData;
        void** pTLS = *(void***)0x82600000;
        void* pAllocator = pTLS[1];
        
        for (uint32_t i = 0; i < charCount; i++) {
            xe_main_thread_init_0038();
            
            // Allocate view element (8 bytes: 2 floats)
            typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
            void** vtable = *(void***)pAllocator;
            AllocFunc alloc = (AllocFunc)vtable[1];
            
            ViewElement* pElement = (ViewElement*)alloc(pAllocator, 8, 16);
            if (pElement) {
                pElement->x = 0.0f;
                pElement->y = 0.0f;
            }
            
            pArray[m_currentCount++] = pElement;
        }
    }
    
    // Process linked list of character data
    struct CharDataNode {
        void** vtable;
        uint8_t padding1[4];
        CharDataNode* pNext;      // +8
        uint8_t padding2[4];
        const char* pName;        // +16
        float posX;               // +20
        float posY;               // +24
    };
    
    CharDataNode* pNode = (CharDataNode*)m_pLinkedListHead;
    uint32_t typeId = *(uint32_t*)0x825C2BC0;
    
    while (pNode != nullptr) {
        // Check if this character type should be included
        typedef bool (*TypeCheckFunc)(void*, uint32_t);
        void** vtable = pNode->vtable;
        TypeCheckFunc checkType = (TypeCheckFunc)vtable[20];
        
        if (checkType(pNode, typeId)) {
            // Character matches type - get index and populate array
            if (pNode->pName) {
                int32_t index = util_2458_FindCharacterIndex(pGameData, pNode->pName);
                
                if (index >= 0 && index < (int32_t)m_currentCount) {
                    struct ViewElement {
                        float x;
                        float y;
                    };
                    ViewElement** pArray = (ViewElement**)m_pAllocatedData;
                    ViewElement* pElement = pArray[index];
                    
                    if (pElement) {
                        pElement->x = pNode->posX;
                        pElement->y = pNode->posY;
                    }
                }
            }
        } else {
            // Character doesn't match - log error
            typedef const char* (*GetNameFunc)(void*);
            GetNameFunc getName1 = (GetNameFunc)vtable[19];
            GetNameFunc getName2 = (GetNameFunc)m_vtable[19];
            
            const char* name1 = getName1(pNode);
            const char* name2 = getName2(this);
            
            nop_8240E6D0((const char*)0x8203F2E0, (void*)name2, (void*)name1);
        }
        
        pNode = pNode->pNext;
    }
}

/**
 * charViewData::ValidateData @ 0x8240B310 | size: 0x48
 * 
 * Type checking or validation function.
 */
bool charViewData::ValidateData(uint32_t param) {
    // TODO: Implement validation logic
    return false;
}

/**
 * charViewData::Serialize @ 0x8240B770 | size: 0x47C
 * 
 * Serializes all character view data fields to XML or binary format.
 */
void charViewData::Serialize(void* serializer) {
    void* defaultVal = (void*)0x825CAF94;
    void* defaultVal2 = (void*)0x825CAF90;
    
    // Serialize all 22+ fields
    // TODO: Extract actual XML field names from string table
    xmlNodeStruct_SerializeField(this, "field_20", &m_field_20, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_10", &m_field_10, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_14", &m_field_14, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_18", &m_field_18, defaultVal2, 0);
    xmlNodeStruct_SerializeField(this, "field_1C", &m_field_1C, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_24", &m_field_24, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_F4", &m_field_F4, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_F0", &m_field_F0, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_124", &m_field_124, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_120", &m_field_120, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_154", &m_field_154, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_150", &m_field_150, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_28", &m_field_28, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_2C", &m_field_2C, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_40", &m_field_40, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_4C", &m_field_4C, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_44", &m_field_44, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_50", &m_field_50, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_58", &m_field_58, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_5C", &m_field_5C, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_64", &m_field_64, defaultVal, 0);
    xmlNodeStruct_SerializeField(this, "field_60", &m_field_60, defaultVal, 0);
}

/**
 * charViewData::GetDataInfo @ 0x8231F408 | size: 0xC
 * 
 * Get data info or size.
 */
uint32_t charViewData::GetDataInfo() {
    // TODO: Return proper data info
    return 0;
}

// ────────────────────────────────────────────────────────────────────────────
// charViewCS Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * charViewCS::~charViewCS @ 0x8216BD20 | size: 0x68
 * 
 * Destructor - cleans up embedded object at offset +292.
 */
charViewCS::~charViewCS() {
    // Clean up embedded object at +292
    util_1568(&m_embeddedObject);
    
    // Update vtable pointer
    m_vtable = (void**)0x8203F454;
    
    // Call base cleanup
    rage_7630(this);
}

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewState Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongCharViewState::~pongCharViewState @ 0x8230C490 | size: 0x68
 * 
 * Destructor - calls base class destructor.
 */
pongCharViewState::~pongCharViewState() {
    // Update vtable to base class
    m_vtable = (void**)0x8205E0AC;  // pongAttractState vtable
    
    // Call base class OnExit
    OnExit();
    
    // Update to final vtable
    m_vtable = (void**)0x8205F174;
}

/**
 * pongCharViewState::Update @ 0x8230C5F8 | size: 0xB8
 * 
 * Frame update override.
 */
void pongCharViewState::Update(float deltaTime) {
    // TODO: Implement character view update logic
}

/**
 * pongCharViewState::Render @ 0x8230C6B0 | size: 0xD8
 * 
 * Render character view UI.
 */
void pongCharViewState::Render() {
    // TODO: Implement character view rendering
}

/**
 * pongCharViewState::OnEnterState @ 0x8230A4E8 | size: 0xC
 * 
 * State entry callback.
 */
void pongCharViewState::OnEnterState() {
    // TODO: Implement state entry logic
}

/**
 * pongCharViewState::OnExitState @ 0x8230C4F8 | size: 0x100
 * 
 * State exit callback.
 */
void pongCharViewState::OnExitState() {
    // TODO: Implement state exit logic
}

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewContext Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongCharViewContext::~pongCharViewContext @ 0x8230A600 | size: 0xAC
 * 
 * Destructor - cleans up managed object and embedded object.
 */
pongCharViewContext::~pongCharViewContext() {
    // Update primary vtable
    m_vtable = (void**)0x8205F1BC;
    
    // Update secondary vtable at +20
    m_vtable2 = (void**)0x8205F1D4;
    
    // Delete managed object at +44 if it exists
    if (m_pManagedObject != nullptr) {
        // Call virtual destructor on managed object
        void** pObjVtable = *(void***)m_pManagedObject;
        typedef void (*DestructorFunc)(void*, bool);
        DestructorFunc dtor = (DestructorFunc)pObjVtable[0];
        dtor(m_pManagedObject, true);
        
        m_pManagedObject = nullptr;
    }
    
    // Clean up embedded object at +80
    rage_8070(&m_embeddedObject);
    
    // Update vtables to final state
    m_vtable2 = (void**)0x8205F7B4;
    m_vtable = (void**)0x8205F174;
}

