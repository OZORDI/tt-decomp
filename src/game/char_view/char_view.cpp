/**
 * char_view.cpp — Character View System Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Implementation of character viewing/selection UI classes.
 * Handles character display, camera control, and UI state management.
 */

#include "game/char_view.hpp"
#include "game/char_view_globals.hpp"
#include <cstring>

// External function declarations
extern "C" {
    void rage_free(void* ptr);
    void* rage_alloc(uint32_t size);
    void sysMemAllocator_InitMainThread();
    void audControl_Destructor(void* obj);  // @ 0x82161568
    void atArray_Destructor(void* obj);
    void atArray_Clear(void* obj);
    void xmlNodeStruct_SerializeField(void* obj, const char* name, void* target, void* defaultVal, uint32_t flags);
    int32_t util_2458_FindCharacterIndex(void* gameData, const char* name);
    void rage_DebugLog(const char* message, void* a, void* b);
    void PostPageGroupMessage(int code, int param1, int param2, int param3);
    void PostStateTransitionRequest(void* manager, int32_t eventType);
    void FadePageGroup(void* context, float param, int p2, int p3, int p4, int p5);
    void* GetStateContextName(void* manager);               // @ 0x822228B8
    void SetupCharViewDisplay(void* screenObj);              // @ 0x8230A6B0
    void ReleaseSceneObject(void* sceneObj);                 // @ 0x8218A2E8
    void ResetBoundObject(void* bound, int param);           // @ 0x821665B8
    void ResetViewBound(void* screenObj, int param);         // @ 0x8230C2C8
    void FadePageGroupOut(void* context, float fade, int flags, int p2, int p3);  // @ 0x823061E8
    void xmlNodeStruct_BaseInitialize(void* obj);            // @ 0x821A8988
}

// Forward declaration
void ResetCharViewData(void* charViewData);  // @ 0x8240A570

// ── Additional globals for char view state ──────────────────────────────────
extern void*    g_charViewData_ptr;         // @ 0x8271A2F0 — pointer to active char view data
extern uint32_t g_charViewSelectCounter;    // @ 0x82606490
extern uint32_t g_charViewSelectFlag;       // @ 0x82606494
extern int32_t  g_selectedCharacterIndex;   // @ 0x825C76B8 — set to -1 on reset
extern uint32_t g_charViewTypeId;           // @ 0x825C2BC4
extern void*    g_sceneRenderObj;           // @ 0x82606430
extern void*    g_sceneRenderObj2;          // @ 0x8260641C
extern void*    g_loop_obj_ptr;             // @ 0x825EAB30
extern const char* g_error_attract_exit;    // @ 0x8205E064
extern const char* g_error_charview_enter;  // @ 0x8205F0F8
extern const char* g_error_charview_exit;   // @ 0x8205F134
extern const char* g_stateName_attract;     // @ 0x8205E094 — "point_off_serves"
extern const char* g_stateName_charView;    // @ 0x8205EF5C

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
 * Event 6: Creates UI state and initializes UI context
 * Events 9, 12: No-op (ignored events)
 * Other events: Forwarded to manager
 */
void pongAttractState::OnEvent(int32_t eventType) {
    // Call initialization function
    PostPageGroupMessage(2055, 64, 0, 0);
    
    if (eventType == 6) {
        // Event 6: Create UI state
        // Set up global UI state array
        g_global_ui_state[13] = 2;  // State type
        g_global_ui_state[14] = (uint32_t)(uintptr_t)m_pScreenObject;  // Screen object pointer
        
        // Initialize UI context
        FadePageGroup(g_ui_context, 0.0f, 1, 0, 0, 0);
        
        // Clear flag on screen object
        if (m_pScreenObject) {
            *(uint8_t*)((char*)m_pScreenObject + 28) = 0;
        }
    } else if (eventType == 9 || eventType == 12) {
        // Events 9 and 12: No-op (ignored)
        return;
    } else {
        // Forward unhandled event to manager
        PostStateTransitionRequest(m_pManager, eventType);
        
        // Log unhandled event error
        rage_DebugLog(g_error_unhandled_event, m_pManager, (void*)(intptr_t)eventType);
    }
}

/**
 * pongAttractState::OnExitEvent @ 0x823059A8 | size: 0xB4
 *
 * Handles exit-phase events for the attract state.
 * Event 6: Sets UI state to "exiting" (state 3), triggers fade-out.
 * Events 8, 12: No-op (silently ignored).
 * Other: Forwards to state manager and logs unhandled event.
 */
void pongAttractState::OnExitEvent(int32_t eventType) {
    PostPageGroupMessage(2056, 64, 0, 0);

    if (eventType == 6) {
        // Transition UI state to "exiting"
        uint32_t* pUiState = (uint32_t*)g_global_ui_state;
        pUiState[13] = 3;

        // Fade out the UI context
        FadePageGroupOut(g_ui_context, 0.0f, -1, 0, 0);
    } else if (eventType == 8 || eventType == 12) {
        return;
    } else {
        // Forward unhandled event to state manager
        void* stateName = GetStateContextName(m_pManager);
        rage_DebugLog(g_error_attract_exit, stateName, (void*)(intptr_t)eventType);
    }
}

/**
 * pongAttractState::GetStateName @ 0x82305A60 | size: 0xC
 *
 * Returns the name of this attract state: "point_off_serves".
 */
const char* pongAttractState::GetStateName() const {
    return g_stateName_attract;
}

/**
 * pongAttractState::OnEnter @ 0x82305800 | size: 0xE0
 * 
 * Called when entering this state - allocates and initializes UI screen object.
 * Creates a pongAttractContext object (32 bytes) with dual vtables for multiple inheritance.
 */
void pongAttractState::OnEnter() {
    sysMemAllocator_InitMainThread();
    
    // Get allocator from TLS (thread-local storage)
    void** pTLS = g_tls_base;
    void* pAllocator = pTLS[1];  // Allocator is at TLS offset +4
    
    // Allocate 32-byte pongAttractContext object
    typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
    void** allocVtable = *(void***)pAllocator;
    AllocFunc alloc = (AllocFunc)allocVtable[1];
    void* pScreen = alloc(pAllocator, 32, 16);  // 32 bytes, 16-byte aligned
    
    if (pScreen) {
        // Initialize screen object to zero
        memset(pScreen, 0, 32);
        
        // Set vtables for pongAttractContext (multiple inheritance)
        *(void**)pScreen = g_vtable_pong_attract_context;  // +0: Primary vtable
        *(void**)((char*)pScreen + 20) = g_vtable_pong_attract_context_2;  // +20: Secondary vtable
        
        // Set back-pointer to owning state
        *(void**)((char*)pScreen + 24) = this;  // +24: Parent state pointer
        
        // Register with global UI system
        uint32_t index = g_ui_state_counter;
        
        g_ui_object_types[index] = 16;  // UI object type ID
        g_ui_object_ptrs[index] = (void*)((char*)pScreen + 20);  // Pointer to secondary vtable
        g_ui_object_flags[index] = 128;  // Active flag
        
        g_ui_state_counter = index + 1;  // Increment counter
        
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
 * Used for filtering characters in view data loading.
 */
bool charViewCharData::IsCharacterType(uint32_t typeId) {
    // Check against three known character type IDs
    if (typeId == g_character_type_id) {
        return true;
    }
    
    if (typeId == g_character_type_id_2) {
        return true;
    }
    
    return (typeId == g_character_type_id_3);
}

/**
 * charViewCharData::Serialize @ 0x8240B280 | size: 0x8C
 * 
 * Serializes character view data to XML or binary format.
 * Uses xmlNodeStruct_SerializeField to write three character-specific fields.
 */
void charViewCharData::Serialize(void* serializer) {
    // Serialize three character-specific fields with default values
    // TODO: Extract actual XML field names from string table
    xmlNodeStruct_SerializeField(this, "field_10", &m_field_10, (void*)&g_default_float_value, 0);
    xmlNodeStruct_SerializeField(this, "field_14", &m_field_14, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_18", &m_field_18, (void*)&g_default_float_zero, 0);
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
        rage_free(m_pAllocatedData);
    }
    
    // Call base class destructor
    rage::ReleaseSingleton(this);
}

/**
 * charViewData::Initialize @ 0x8240B598 | size: 0x24
 *
 * Computes clamped min/max range from two source float fields (+0xB8, +0xBC)
 * and stores min at +0xC4, max at +0xC0. Then calls base class Initialize.
 */
void charViewData::Initialize() {
    // Access float fields at offsets +0xB8 and +0xBC (within padding region)
    float* pFields = reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xB8);
    float valueA = pFields[0];  // +0xB8
    float valueB = pFields[1];  // +0xBC

    // Compute min and max
    float* pResults = reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xC0);
    pResults[0] = (valueA >= valueB) ? valueA : valueB;  // +0xC0 = max
    pResults[1] = (valueA >= valueB) ? valueB : valueA;  // +0xC4 = min

    // Call base class Initialize
    xmlNodeStruct_BaseInitialize(this);
}

/**
 * charViewData::LoadViewData @ 0x8240B5C0 | size: 0x1AC
 * 
 * Loads and initializes character view data from game data manager.
 * Allocates array for 2D position elements (one per character), then
 * processes linked list of character data to populate positions.
 */
void charViewData::LoadViewData() {
    struct GameDataMgr {
        uint8_t padding[28];
        uint32_t characterCount;  // +28
    };
    
    // Get character count from game data manager singleton
    GameDataMgr* pGameData = *(GameDataMgr**)&g_game_data_manager;
    uint32_t charCount = pGameData ? pGameData->characterCount : 0;
    
    // Allocate array for character view elements (2D positions)
    if (charCount > 0) {
        uint32_t allocSize = charCount * 4;  // 4 bytes per pointer
        if (charCount > 0x3FFFFFFF) {
            allocSize = 0xFFFFFFFF;
        }
        m_pAllocatedData = rage_alloc(allocSize);
    } else {
        m_pAllocatedData = nullptr;
    }
    
    m_allocatedSize = (uint16_t)charCount;
    m_currentCount = 0;
    
    // Initialize array elements with default 2D positions (0.0f, 0.0f)
    if (charCount > 0) {
        struct ViewElement {
            float x;
            float y;
        };
        
        ViewElement** pArray = (ViewElement**)m_pAllocatedData;
        void** pTLS = g_tls_base;
        void* pAllocator = pTLS[1];
        
        for (uint32_t i = 0; i < charCount; i++) {
            sysMemAllocator_InitMainThread();
            
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
    
    while (pNode != nullptr) {
        // Check if this character type should be included
        typedef bool (*TypeCheckFunc)(void*, uint32_t);
        void** vtable = pNode->vtable;
        TypeCheckFunc checkType = (TypeCheckFunc)vtable[20];
        
        if (checkType(pNode, g_character_type_id)) {
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
            // Character doesn't match type - log error
            typedef const char* (*GetNameFunc)(void*);
            GetNameFunc getName1 = (GetNameFunc)vtable[19];
            GetNameFunc getName2 = (GetNameFunc)(*(void***)this)[19];
            
            const char* name1 = getName1(pNode);
            const char* name2 = getName2(this);
            
            rage_DebugLog(g_error_type_mismatch, (void*)name2, (void*)name1);
        }
        
        pNode = pNode->pNext;
    }
}

/**
 * charViewData::ValidateData @ 0x8240B310 | size: 0x48
 *
 * Checks whether the given type ID matches any of the three known
 * charViewData type identifiers. Returns true if it's a supported type.
 */
bool charViewData::ValidateData(uint32_t param) {
    if (param == g_charViewTypeId) {
        return true;
    }
    if (param == g_character_type_id_2) {
        return true;
    }
    return (param == g_character_type_id_3);
}

/**
 * charViewData::Serialize @ 0x8240B770 | size: 0x47C
 * 
 * Serializes all character view data fields to XML or binary format.
 * Writes 22+ fields with default float values.
 */
void charViewData::Serialize(void* serializer) {
    // Serialize all 22+ fields with appropriate default values
    // TODO: Extract actual XML field names from string table
    xmlNodeStruct_SerializeField(this, "field_20", &m_field_20, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_10", &m_field_10, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_14", &m_field_14, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_18", &m_field_18, (void*)&g_default_float_value_2, 0);
    xmlNodeStruct_SerializeField(this, "field_1C", &m_field_1C, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_24", &m_field_24, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_F4", &m_field_F4, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_F0", &m_field_F0, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_124", &m_field_124, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_120", &m_field_120, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_154", &m_field_154, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_150", &m_field_150, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_28", &m_field_28, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_2C", &m_field_2C, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_40", &m_field_40, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_4C", &m_field_4C, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_44", &m_field_44, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_50", &m_field_50, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_58", &m_field_58, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_5C", &m_field_5C, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_64", &m_field_64, (void*)&g_default_float_zero, 0);
    xmlNodeStruct_SerializeField(this, "field_60", &m_field_60, (void*)&g_default_float_zero, 0);
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
    audControl_Destructor(&m_embeddedObject);
    
    // Update vtable pointer
    *(void**)this = (void*)g_vtable_char_view_cs;
    
    // Call base cleanup
    atArray_Destructor(this);
}

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewState Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongCharViewState::~pongCharViewState @ 0x8230C490 | size: 0x68
 *
 * Destructor — cleans up the screen object via the parent OnExit,
 * then resets vtable to the ultimate base class (rage::datBase).
 */
pongCharViewState::~pongCharViewState() {
    // Call parent OnExit to release UI screen object
    pongAttractState::OnExit();
}

/**
 * pongCharViewState::OnEnterEvent @ 0x8230C5F8 | size: 0xB8
 *
 * Handles enter-phase events for the character view state.
 * Event 6: Sets up the character view display — registers screen object
 *          in the UI state array, initializes the network display,
 *          resets char view data, and posts message 2065.
 * Event 12: No-op.
 * Other: Forwards to state manager and logs unhandled event.
 */
void pongCharViewState::OnEnterEvent(int32_t eventType) {
    if (eventType == 6) {
        // Register screen object in global UI state
        uint32_t* pUiState = (uint32_t*)g_global_ui_state;
        pUiState[13] = 2;
        pUiState[14] = (uint32_t)(uintptr_t)m_pScreenObject;

        // Initialize character view display
        SetupCharViewDisplay(m_pScreenObject);

        // Reset the global char view data
        ResetCharViewData(g_charViewData_ptr);

        // Enable the char view loop flag
        ((uint8_t*)g_loop_obj_ptr)[577] = 1;

        // Post enter message
        PostPageGroupMessage(2065, 64, 0, 0);
    } else if (eventType == 12) {
        return;
    } else {
        // Forward unhandled event to state manager
        void* stateName = GetStateContextName(m_pManager);
        rage_DebugLog(g_error_charview_enter, stateName, (void*)(intptr_t)eventType);
    }
}

/**
 * pongCharViewState::OnExitEvent @ 0x8230C6B0 | size: 0xD8
 *
 * Handles exit-phase events for the character view state.
 * Event 6: Cleans up the character view display — releases scene objects,
 *          resets physics bounds, resets char view data, clears the loop
 *          flag, and posts message 2066.
 * Event 12: No-op.
 * Other: Forwards to state manager and logs unhandled event.
 */
void pongCharViewState::OnExitEvent(int32_t eventType) {
    if (eventType == 6) {
        // Set UI state to "exiting char view"
        uint32_t* pUiState = (uint32_t*)g_global_ui_state;
        pUiState[13] = 3;

        void* screenObj = m_pScreenObject;

        // Release scene render objects
        ReleaseSceneObject(g_sceneRenderObj);
        ResetBoundObject(g_sceneRenderObj2, 0);
        ResetViewBound(screenObj, 0);

        // Reset the global char view data
        ResetCharViewData(g_charViewData_ptr);

        // Disable the char view loop flag
        ((uint8_t*)g_loop_obj_ptr)[577] = 0;

        // Post exit message
        PostPageGroupMessage(2066, 64, 0, 0);
    } else if (eventType == 12) {
        return;
    } else {
        // Forward unhandled event to state manager
        void* stateName = GetStateContextName(m_pManager);
        rage_DebugLog(g_error_charview_exit, stateName, (void*)(intptr_t)eventType);
    }
}

/**
 * pongCharViewState::GetStateName @ 0x8230A4E8 | size: 0xC
 *
 * Returns the name string for this character view state.
 */
const char* pongCharViewState::GetStateName() const {
    return g_stateName_charView;
}

/**
 * pongCharViewState::OnEnter @ 0x8230C4F8 | size: 0x100
 *
 * Allocates and initializes a 100-byte pongCharViewContext object with
 * multiple-inheritance vtables (primary + secondary). Sets up all fields
 * to initial values, then calls the context's initialization method.
 */
void pongCharViewState::OnEnter() {
    sysMemAllocator_InitMainThread();

    // Get allocator from TLS
    void** pTLS = g_tls_base;
    void* pAllocator = pTLS[1];

    // Allocate 100-byte pongCharViewContext
    typedef void* (*AllocFunc)(void*, uint32_t, uint32_t);
    void** allocVtable = *(void***)pAllocator;
    AllocFunc alloc = (AllocFunc)allocVtable[1];
    void* pContext = alloc(pAllocator, 100, 16);

    if (pContext) {
        // Zero-initialize the entire context
        memset(pContext, 0, 100);

        // Set up primary vtable for pongCharViewContext
        *(void**)pContext = &g_vtable_pong_char_view_context;

        // Initialize fields to defaults
        // +4, +8, +12, +16: already zeroed
        // +20: secondary vtable (overwritten below)
        *(void**)((char*)pContext + 20) = &g_vtable_pong_char_view_context_2;
        // +44: nullptr (managed object)
        // +56, +60, +64: set to -1 (invalid indices)
        *(int32_t*)((char*)pContext + 56) = -1;
        *(int32_t*)((char*)pContext + 60) = -1;
        *(int32_t*)((char*)pContext + 64) = -1;
        // +76, +77, +78: flags (already zeroed)

        // Initialize embedded rage::xmlTree at +80
        char* pEmbedded = (char*)pContext + 80;
        *(void**)pEmbedded = &g_vtable_char_view_cs;  // xmlTree vtable
        *(uint32_t*)(pEmbedded + 4) = 0;
        *(uint32_t*)(pEmbedded + 8) = 0;
        *(uint16_t*)(pEmbedded + 12) = 0;
        *(uint16_t*)(pEmbedded + 14) = 0;

        // +96: zero (already zeroed)
        // +28, +32, +36, +40: zero (already zeroed)

        m_pScreenObject = pContext;
    } else {
        m_pScreenObject = nullptr;
    }

    // Call initialization on the context (vtable slot 23)
    if (m_pScreenObject) {
        typedef void (*InitFunc)(void*);
        void** vtable = *(void***)m_pScreenObject;
        InitFunc init = (InitFunc)vtable[23];
        init(m_pScreenObject);
    }
}

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewContext Implementation
// ────────────────────────────────────────────────────────────────────────────

/**
 * pongCharViewContext::~pongCharViewContext @ 0x8230A600 | size: 0xAC
 * 
 * Destructor - cleans up managed object and embedded object.
 * Uses multiple inheritance with three vtables.
 */
pongCharViewContext::~pongCharViewContext() {
    // Update primary vtable
    *(void**)this = (void*)g_vtable_pong_char_view_context;

    // Update secondary vtable at +20
    m_vtable2 = (void**)g_vtable_pong_char_view_context_2;

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
    atArray_Clear(&m_embeddedObject);

    // Update vtables to final state
    m_vtable2 = (void**)g_vtable_pong_char_view_context_3;
    *(void**)this = (void*)g_vtable_pong_char_view_state;
}


// ────────────────────────────────────────────────────────────────────────────
// ResetCharViewData — Global state reset for character view system
// ────────────────────────────────────────────────────────────────────────────

/**
 * ResetCharViewData @ 0x8240A570 | size: 0x8C
 *
 * Resets all fields on the global character view data structure to zero/default.
 * Called during both enter and exit phases of the character view state to ensure
 * clean state. Also clears related global counters and sets the selected
 * character index to -1 (invalid).
 *
 * The data structure has float fields at offsets +12, +16, +28, +32, +36, +40,
 * +44, +48 and integer/pointer fields at +4, +8, +20, +24.
 */
void ResetCharViewData(void* charViewData) {
    if (!charViewData) return;

    char* pData = (char*)charViewData;

    // Reset all integer/pointer fields to zero
    *(uint32_t*)(pData + 4)  = 0;
    *(uint32_t*)(pData + 8)  = 0;
    *(uint32_t*)(pData + 20) = 0;
    *(uint32_t*)(pData + 24) = 0;

    // Reset all float fields to 0.0f
    *(float*)(pData + 12) = 0.0f;
    *(float*)(pData + 16) = 0.0f;
    *(float*)(pData + 28) = 0.0f;
    *(float*)(pData + 32) = 0.0f;
    *(float*)(pData + 36) = 0.0f;
    *(float*)(pData + 40) = 0.0f;
    *(float*)(pData + 44) = 0.0f;
    *(float*)(pData + 48) = 0.0f;

    // Clear global selection counters
    g_charViewSelectCounter = 0;
    g_charViewSelectFlag = 0;

    // Reset selected character index to invalid
    g_selectedCharacterIndex = -1;
}
