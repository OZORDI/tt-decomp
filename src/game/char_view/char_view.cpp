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
#include <cmath>

// External function declarations
extern "C" {
    void rage_free(void* ptr);
    void* rage_Alloc(uint32_t size);
    void rage_AssertMainThread();
    void audControl_Destructor(void* obj);  // @ 0x82161568
    void atArray_Destructor(void* obj);
    void atArray_Clear(void* obj);
    void xmlNodeStruct_SerializeField(void* obj, const char* name, void* target, void* defaultVal, uint32_t flags);
    int32_t util_2458_FindCharacterIndex(void* gameData, const char* name);
    void rage_debugLog(const char* message, void* a, void* b);
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

    // ── charViewCS helper externs ──
    // Parser field-registration helper used by RegisterXmlFields.
    void pongHairData_7B60_g(void* parser, int type, const char* name,
                              void* fieldAddr, int one, int defaultSlot);  // @ 0x82177B60
    // Bound validator tail-called by charViewCS::Validate.
    void phBoundCapsule_DB10_g(void* self);                   // @ 0x8216DB10
    // Large update routine called by charViewCS::Update.
    void charViewCS_Update_body(void* self);                  // @ 0x8216C200 (unlifted, stub)
}

// Global string tables (XML field names) referenced by RegisterXmlFields.
// Addresses in the original binary, content from .rdata lookup.
extern const char g_str_charViewCS_field_9C[];   // +156 field
extern const char g_str_charViewCS_field_A0[];   // +160 field
extern const char g_str_charViewCS_field_A4[];   // +164 field
extern const char g_str_charViewCS_field_A8[];   // +168 field
extern const char g_str_charViewCS_field_AC[];   // +172 field
extern const char g_str_charViewCS_Shadows[];    // +180 field ("Shadows")

// Linked-list sentinels used by PurgeFilteredNodes (at 0x825CA938/944/93C/948).
struct charViewCS_Node { void* field_00; charViewCS_Node* next; charViewCS_Node* prev; void* info; };
extern charViewCS_Node g_charViewCS_freeListHead;    // @ 0x825CA938 (free-list sentinel)
extern charViewCS_Node g_charViewCS_activeListHead;  // @ 0x825CA944 (active-list sentinel)
extern charViewCS_Node* g_charViewCS_activeTail;     // @ 0x825CA948
extern charViewCS_Node* g_charViewCS_freeTail;       // @ 0x825CA93C

// Math constants used by bound clamping (from .data / .rdata constant pool).
extern const float g_charViewCS_boundHalfExtent;     // +84 when flag set / +8 otherwise
extern const float g_charViewCS_scale;               // lbl_825C5938

// Error/label strings.
extern const char g_str_many_bounds_specified[];   // "many bounds specified (Max=%d)"

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
extern const char* g_stateName_attract;     // @ 0x8205E094 — "pongAttractState"
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
        rage_debugLog(g_error_unhandled_event, m_pManager, (void*)(intptr_t)eventType);
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
        rage_debugLog(g_error_attract_exit, stateName, (void*)(intptr_t)eventType);
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
    rage_AssertMainThread();
    
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
        m_pAllocatedData = rage_Alloc(allocSize);
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
            rage_AssertMainThread();
            
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
            GetNameFunc getName2 = (GetNameFunc)m_vtable[19];
            
            const char* name1 = getName1(pNode);
            const char* name2 = getName2(this);
            
            rage_debugLog(g_error_type_mismatch, (void*)name2, (void*)name1);
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
    m_vtable = (void**)g_vtable_char_view_cs;
    
    // Call base cleanup
    atArray_Destructor(this);
}

// ────────────────────────────────────────────────────────────────────────────
// charViewCS virtual + helper methods — 10 functions total
// ────────────────────────────────────────────────────────────────────────────

/**
 * charViewCS::RegisterXmlFields (vfn_3)
 * @ 0x8216DBB8 | size: 0xF4
 *
 * Registers six XML-serialisable fields with the parser. Five of the six are
 * registered with type=6 (numeric) and one (at +180 / "Shadows") with type=1
 * (boolean). All share default-slot=1, scalar=0.
 *
 * TODO: Replace the integer type constants (6 / 1) with named enum values
 *       once the parser type taxonomy is nailed down elsewhere.
 */
void charViewCS::RegisterXmlFields(void* parser) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);

    // Six fields register in the same structural pattern.
    pongHairData_7B60_g(parser, 6, g_str_charViewCS_field_9C, self + 156, 1, 0);
    pongHairData_7B60_g(parser, 6, g_str_charViewCS_field_A0, self + 160, 1, 0);
    pongHairData_7B60_g(parser, 6, g_str_charViewCS_field_A4, self + 164, 1, 0);
    pongHairData_7B60_g(parser, 6, g_str_charViewCS_field_A8, self + 168, 1, 0);
    pongHairData_7B60_g(parser, 6, g_str_charViewCS_field_AC, self + 172, 1, 0);
    pongHairData_7B60_g(parser, 1, g_str_charViewCS_Shadows,  self + 180, 1, 0);
}

/**
 * charViewCS::Validate (vfn_4)
 * @ 0x8216DCB0 | size: 0x4
 *
 * Tail-call to the bound-capsule validator. The only instruction in the
 * original is a branch to phBoundCapsule_DB10_g.
 */
void charViewCS::Validate() {
    phBoundCapsule_DB10_g(this);
}

/**
 * charViewCS::GetName (vfn_6)
 * @ 0x8216DB00 | size: 0xC
 *
 * Returns a pointer to the class-name / error-label string at lbl_82036404
 * ("many bounds specified (Max=%d)" — the original binary keeps several
 * diagnostic labels in the same string pool and this slot returns one of
 * them).
 */
const char* charViewCS::GetName() {
    return g_str_many_bounds_specified;
}

/**
 * charViewCS::GetVariantName (vfn_7)
 * @ 0x82177840 | size: 0xC
 *
 * Returns the "Shadows:" configuration label (lbl_82037400, the name of the
 * final XML field this class registers).
 */
const char* charViewCS::GetVariantName() {
    return g_str_charViewCS_Shadows;
}

/**
 * charViewCS::Update (vfn_9)
 * @ 0x8216C200 | size: 0x510
 *
 * Main per-frame update for the character-view camera shot. The body is
 * 1296 bytes of float/vector arithmetic and a handful of dispatches into
 * sub-objects — too large to lift by hand at this pass. Thunk to the still
 * unlifted body keeps behaviour identical while letting this method be
 * resolved by the C++ vtable builder.
 *
 * TODO: Lift the 1296-byte body. Likely does:
 *         1. sample/blend driver state,
 *         2. recompute bounds via RecalcBounds() / DispatchBoundsOrLookAt(),
 *         3. push the result into the embedded control object at +292.
 */
void charViewCS::Update() {
    charViewCS_Update_body(this);
}

/**
 * charViewCS::RecalcBounds (vfn_10)
 * @ 0x8216BED0 | size: 0x48
 *
 * Iterates the four bound slots. For each slot i it inspects the current
 * bound count at this+252:
 *   - if (count == i), or (count >= 4 regardless of i), call
 *     RecalcBoundSlot(i) to rebuild that slot.
 *
 * This is the per-frame entry point into the bound recomputation helper
 * chain (BF18 -> C100).
 */
void charViewCS::RecalcBounds() {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);
    for (int slot = 0; slot < 4; ++slot) {
        int32_t count = *reinterpret_cast<const int32_t*>(self + 252);
        if (count == slot || count >= 4) {
            RecalcBoundSlot(slot);
        }
    }
}

/**
 * charViewCS::PurgeFilteredNodes (vfn_11)
 * @ 0x8216DB88 | size: 0x2C
 *
 * Walks the global active-node linked list starting at
 * g_charViewCS_activeListHead. For each node, looks up its type tag at
 * info->[+4]; if the tag is one of {5, 6, 12, 13, 14, 16} the node is kept
 * and the function will return true. Otherwise the node is unlinked from
 * the active list and pushed onto the free list at g_charViewCS_freeTail.
 *
 * @return true if at least one node matched the keep-filter, false if all
 *         were recycled.
 *
 * TODO: Replace the magic tag-set with named constants once the node
 *       taxonomy is understood ({Direct, Relative, Target, LookAt, ...}).
 */
bool charViewCS::PurgeFilteredNodes() {
    bool kept = false;

    charViewCS_Node* const activeHead = &g_charViewCS_activeListHead;
    charViewCS_Node* iter = g_charViewCS_activeTail;
    charViewCS_Node* nextAnchor = activeHead;

    while (iter != activeHead) {
        int32_t tag = *reinterpret_cast<const int32_t*>(
            reinterpret_cast<const uint8_t*>(iter->info) + 4);
        charViewCS_Node* next = iter->next;

        if (tag == 5 || tag == 6 || tag == 12 || tag == 13 ||
            tag == 14 || tag == 16) {
            nextAnchor = iter;
            kept = true;
        } else {
            // Unlink from active list.
            if (iter->next) iter->next->prev = iter->prev;
            if (iter->prev) iter->prev->next = iter->next;

            // Push onto free list (head = &lbl_825CA938, tail = freeTail).
            if (g_charViewCS_freeTail) {
                g_charViewCS_freeTail->prev = iter;
            }
            iter->prev = &g_charViewCS_freeListHead;
            iter->next = g_charViewCS_freeTail;
            g_charViewCS_freeTail = iter;
        }

        iter = next;
    }

    (void)nextAnchor;  // bookkeeping variable in the original; unused here.
    return kept;
}

/**
 * charViewCS::RecalcBoundSlot
 * @ 0x8216BF18 | size: 0x1E4
 *
 * Unpacks three int8 components from a 20-byte descriptor stored at
 *   bounds_array + slot*20  (where bounds_array = this[+248])
 * into normalized floats in [-1, 1], clamps each against the
 * bound half-extent loaded from this[+248]->[+32], and forwards the
 * normalized axis value to ApplyAxis for each non-zero axis.
 *
 * Layout of the descriptor (bytes +12, +13, +15 are the packed int8 axis
 * samples; the offsets match the IDA lift).
 *
 * Numerical constants:
 *   bias       = lbl_821F9974 (= 127.5f)
 *   scale      = lbl_821F9970 (= 1/127.5f)
 *   zero       = lbl_825C5938 (= 0.0f)
 *   one        = lbl_82030FD0 (= 1.0f)
 */
void charViewCS::RecalcBoundSlot(int slot) {
    const uint8_t* self = reinterpret_cast<const uint8_t*>(this);

    const uint8_t* boundArray = *reinterpret_cast<const uint8_t* const*>(self + 248);
    const float halfExtent = *reinterpret_cast<const float*>(boundArray + 32);

    // Descriptor pointer = boundArray + slot * 20 (see lift: r10 = slot*20).
    const uint8_t* desc = boundArray + slot * 20;

    const float bias  = 127.5f;
    const float scale = 1.0f / 127.5f;
    const float zero  = 0.0f;
    const float one   = 1.0f;

    // Lambda: unpack a uint8 sample -> normalized float in [-1, 1], clamped
    // to the bound half-extent, then dispatch to ApplyAxis(axis).
    auto processAxis = [&](int byteOffset, int axis) {
        uint8_t rawU = desc[byteOffset];
        float raw = static_cast<float>(rawU);
        float v = (raw - bias) * scale;                    // normalize
        if (v < zero) v = zero;
        if (v > one)  v = one;

        float normalized;
        if (v > halfExtent) {
            normalized = (v - halfExtent) / (one - halfExtent);
        } else if (v < -halfExtent) {
            normalized = (v + halfExtent) / (one - halfExtent);
        } else {
            normalized = zero;
        }

        if (normalized * zero != zero) {
            // NaN sentinel — original lift multiplies by zero to check.
            ApplyAxis(axis);
        }
    };

    processAxis(12, 0);   // X
    processAxis(15, 1);   // Y
    processAxis(13, 2);   // Z
}

/**
 * charViewCS::ApplyAxis
 * @ 0x8216C100 | size: 0x78
 *
 * Writes a clamped position value for one axis into the per-slot position
 * array at this[+64 + axis*4]. Picks the bound half-extent from either
 *   - lbl_821F2034 (+84) when byte at this[+180] ("Shadows") is non-zero,
 *   - lbl_821F1FB8 (+8)  otherwise.
 *
 * Pseudocode of the clamp:
 *     pos      = positions[axis]
 *     extent   = halfExtentTable[shadowsFlag ? on : off]
 *     base     = boundDesc[axis].weight   (at +80)
 *     offset   = boundDesc[axis].offset   (at +68)
 *     newPos   = pos + base * extent * scaleParam
 *     newPos   = max(newPos - offset, -offset)  // via fsel trick
 *     positions[axis] = newPos
 */
void charViewCS::ApplyAxis(int axis) {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);

    // Halfextent table: flag at +180 selects +84 vs +8 of table base.
    const bool shadowsFlag = self[180] != 0;
    const float extent = shadowsFlag
        ? *(reinterpret_cast<const float*>(&g_charViewCS_boundHalfExtent) + 21)
        :  g_charViewCS_boundHalfExtent;

    uint8_t* boundArray = *reinterpret_cast<uint8_t**>(self + 248);
    // Per-slot descriptor stride in the lift is (axis + axis*8) * 16 = axis*144,
    // which is the same 16-byte vector layout used elsewhere in the bound table.
    uint8_t* desc = boundArray + static_cast<uintptr_t>(axis) * 144;

    const float base   = *reinterpret_cast<const float*>(desc + 80);
    const float offset = *reinterpret_cast<const float*>(desc + 68);

    float& pos = *reinterpret_cast<float*>(self + 64 + axis * 4);

    const float scaleParam = g_charViewCS_scale;
    float candidate = pos + base * extent * scaleParam;

    // Branch-free max of (candidate - offset, -offset) via fsel in the lift.
    float adjusted = candidate - offset;
    if (adjusted < -offset) adjusted = -offset;
    // Clamp upward bound as well (matches the second fsel in the lift).
    if (adjusted > offset)  adjusted =  offset;

    pos = adjusted + offset;
}

/**
 * charViewCS::DispatchBoundsOrLookAt
 * @ 0x8216C710 | size: 0x10C
 *
 * State-dependent dispatcher that forwards into the embedded sub-object at
 * this+292 (the object audControl_Destructor cleans up):
 *
 *   - If field +284 != 1: call vtable slot 19 (a predicate). If it returns
 *     non-zero, tail-call vtable slot 4 (handler A).
 *   - Else (+284 == 1): compute s = |x| + |y| + |z| from floats at
 *     +256/+260/+264 and compare against the bound threshold at
 *     bounds->[+36]. If s < threshold, call slot 19 and if true tail-call
 *     slot 4. Otherwise call slot 19 and if true tail-call slot 3.
 *
 * The fall-through path ends without dispatching when the predicate returns
 * zero.
 */
void charViewCS::DispatchBoundsOrLookAt() {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void* embedded = self + 292;
    void*** vt = reinterpret_cast<void***>(embedded);

    auto callPredicate19 = [&]() -> bool {
        using Fn = bool (*)(void*);
        Fn fn = reinterpret_cast<Fn>((*vt)[19]);
        return fn(embedded) != 0;
    };
    auto tailCallSlot = [&](int slot) {
        using Fn = void (*)(void*);
        Fn fn = reinterpret_cast<Fn>((*vt)[slot]);
        fn(embedded);
    };

    const int32_t state = *reinterpret_cast<const int32_t*>(self + 284);

    if (state != 1) {
        // Non-state-1: predicate -> slot 4 handler.
        if (!callPredicate19()) return;
        tailCallSlot(4);
        return;
    }

    // State 1: evaluate bound budget.
    const float absX = std::fabs(*reinterpret_cast<const float*>(self + 256));
    const float absY = std::fabs(*reinterpret_cast<const float*>(self + 260));
    const float absZ = std::fabs(*reinterpret_cast<const float*>(self + 264));
    const uint8_t* bounds = *reinterpret_cast<uint8_t* const*>(self + 248);
    const float threshold = *reinterpret_cast<const float*>(bounds + 36);

    const float sum = absY + absX + absZ;

    if (sum >= threshold) {
        // Over-budget: slot 3 handler.
        if (!callPredicate19()) return;
        tailCallSlot(3);
    } else {
        // Under budget: slot 4 handler.
        if (!callPredicate19()) return;
        tailCallSlot(4);
    }
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
        rage_debugLog(g_error_charview_enter, stateName, (void*)(intptr_t)eventType);
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
        rage_debugLog(g_error_charview_exit, stateName, (void*)(intptr_t)eventType);
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
    rage_AssertMainThread();

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
    m_vtable = (void**)g_vtable_pong_char_view_context;
    
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
    m_vtable = (void**)g_vtable_pong_char_view_state;
}

// ────────────────────────────────────────────────────────────────────────────
// pongCharViewContext singleton methods — UI registration, lifecycle, and
// confirmation helpers. The heavy Update/pose logic (vfn_16 / vfn_18) is
// intentionally *not* lifted here; it lives with the pose/animation work
// because it manipulates per-player state fields at +452/+212/+288.
// ────────────────────────────────────────────────────────────────────────────

// Helper externs used by the pongCharViewContext singleton methods. These
// are already declared elsewhere in the lift but we forward-declare what is
// strictly needed here so the file compiles standalone.
extern "C" {
    // pongCharViewContext_vfn_11 helpers — UI state array writeback.
    // (g_ui_state_counter / g_ui_object_ptrs / g_ui_object_types are already
    //  extern'd via char_view_globals.hpp.)

    // pongCharViewContext_C400_p46 helpers.
    void  hsmContext_SetNextState_2800(void* hsm, int32_t eventCode);  // @ 0x82222800
    void  util_94B8(void* dialogTpl, int32_t a, int32_t b, void* strA, void* strB);  // @ 0x823794B8
    extern void* lbl_8271A320;            // @ 0x8271A320 — active charView data
    extern uint8_t g_noSelectionDialogTpl[20];  // @ 0x825C9F28 — 20-byte dialog template
}

/**
 * pongCharViewContext::RegisterWithUI (vfn_11)
 * @ 0x8230A898 | size: 0x60
 *
 * Pushes this context onto the global UI state array as a fixed-size slot
 * (type=20, flags=0x80). When called with `this == nullptr` the object-pointer
 * slot is zeroed but the type/flag slots are still populated — matches the
 * pattern seen in pongAttractState::OnEnter. After the push, g_ui_state_counter
 * is incremented by 1.
 *
 * Returns 1 in the original (always) — kept as void here since no caller
 * examines the value.
 */
void pongCharViewContext::RegisterWithUI() {
    // The stored UI object pointer is (this + 20) — the secondary (MI) vtable,
    // matching how the attract context is registered. The original clears
    // this to zero when r3 == 0 (cmplwi cr6,r3,0), but a properly-dispatched
    // virtual call on a non-null object makes that branch unreachable.
    void* pSecondary = reinterpret_cast<uint8_t*>(this) + 20;

    uint32_t index = g_ui_state_counter;

    g_ui_object_flags[index] = 0x80;            // active flag
    g_ui_object_ptrs[index]  = pSecondary;      // +secondary vtable pointer
    g_ui_object_types[index] = 20;              // UI slot-type ID

    g_ui_state_counter = index + 1;
}

/**
 * pongCharViewContext::ReleaseManagedObject (vfn_12)
 * @ 0x8230AB50 | size: 0x54
 *
 * Tears down the managed character-data object stored at +44 (the hudCharView
 * instance allocated in vfn_23). Calls its virtual destructor (slot 0 with
 * delete-flag=1), then nulls the pointer. Always returns 1 in the original.
 */
void pongCharViewContext::ReleaseManagedObject() {
    if (m_pManagedObject == nullptr) {
        return;
    }

    // Virtual destructor with "delete" flag (slot 0).
    using DtorFn = void (*)(void*, int32_t);
    void** vtable = *reinterpret_cast<void***>(m_pManagedObject);
    DtorFn dtor = reinterpret_cast<DtorFn>(vtable[0]);
    dtor(m_pManagedObject, 1);

    m_pManagedObject = nullptr;
}

/**
 * pongCharViewContext::NotifySlotActivity (vfn_17)
 * @ 0x8230AF50 | size: 0xAC
 *
 * Iterates the two player slots (bytes +77, +78 — the "active" flags set by
 * vfn_16) and, for each slot whose byte is non-zero, dispatches slot-6 (the
 * "activity notify" method) on the corresponding UI object at
 *   g_ui_object_ptrs[ slot + 29 ]
 * (the +29 offset is how this class's two UI slots are keyed in the global
 * pointer table — slot 29/30 are the char-view object entries).
 *
 * Guarded by the standard "main-thread && not mid-shutdown" predicate used
 * throughout the charView code (TLS flag @ 0x82600000+495 must be zero and
 * the char-view enable flag at g_loop_obj_ptr+24 must be non-zero).
 */
void pongCharViewContext::NotifySlotActivity() {
    // Predicate: same check as vfn_16. Bail out if char view is mid-shutdown.
    uint8_t* tls495 = reinterpret_cast<uint8_t*>(g_tls_base) + 495;
    bool ready = (*tls495 != 0);
    if (!ready) {
        uint8_t* loopFlag = reinterpret_cast<uint8_t*>(g_loop_obj_ptr) + 24;
        ready = (*loopFlag != 0);
    }
    if (!ready) return;

    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    for (int slot = 0; slot < 2; ++slot) {
        uint8_t active = self[77 + slot];
        if (active == 0) continue;

        // Global object-pointer table is indexed by (slot + 29) * 4 bytes.
        void* uiObj = g_ui_object_ptrs[slot + 29];
        if (uiObj == nullptr) continue;

        // Dispatch vtable slot 6 (the "notify activity" method).
        using NotifyFn = void (*)(void*);
        void** vtable = *reinterpret_cast<void***>(uiObj);
        NotifyFn notify = reinterpret_cast<NotifyFn>(vtable[6]);
        notify(uiObj);
    }
}

/**
 * pongCharViewContext::ConfirmSelection (helper @ 0x8230C400)
 * @ 0x8230C400 | size: 0x8C
 *
 * "Accept" handler wired to the char-view menu's primary button. Always
 * requests a state transition (event 6) via hsmContext_SetNextState_2800,
 * then checks the two-player character indices stored at +16 / +20 of the
 * active charView data object. If either slot is unassigned (-1), the
 * confirmation is aborted by presenting a modal dialog via util_94B8 with
 * template lbl_8205F838 (the "please select a character" warning).
 */
void pongCharViewContext::ConfirmSelection() {
    // Request the state transition regardless — the caller always does this.
    hsmContext_SetNextState_2800(g_tls_base, 6);

    // Validate that both player slots have an assigned character.
    struct CharViewData {
        uint8_t  padding[16];
        int32_t  slotA;   // +16
        int32_t  slotB;   // +20
    };
    auto* data = static_cast<CharViewData*>(lbl_8271A320);

    bool bothAssigned = (data->slotA != -1) && (data->slotB != -1);
    if (bothAssigned) {
        return;  // All good; the state transition proceeds.
    }

    // Missing selection — pop the warning dialog. The 20-byte template at
    // lbl_825C9F28 is two int64 + one int32 (unpacked into r6/r7/r8/r9 in
    // the original). Pass both halves verbatim.
    uint64_t tplLo = *reinterpret_cast<uint64_t*>(&g_noSelectionDialogTpl[0]);
    uint64_t tplHi = *reinterpret_cast<uint64_t*>(&g_noSelectionDialogTpl[8]);
    util_94B8(reinterpret_cast<void*>(&g_noSelectionDialogTpl[0]),
              2, 1,
              reinterpret_cast<void*>(tplLo),
              reinterpret_cast<void*>(tplHi));
}

