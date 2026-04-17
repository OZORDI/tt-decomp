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

// ResetCharViewData is declared in char_view.hpp and defined below.

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
// pongCharViewContext — scene-setup / pose / camera-orbit virtuals + helpers
// ────────────────────────────────────────────────────────────────────────────

// Globals referenced by the scene methods (their addresses verified via MCP).
extern uint32_t g_sceneSlotCounter;          // @ 0x825C9FA4 (bumped by RegisterSceneSlot)
extern uint8_t  g_sceneSlotTagTable[];       // @ base 0x825CA130-ish (byte per slot)
extern void*    g_sceneSlotPtrTable[];       // @ base 0x825C9A68-ish (4 bytes per slot)
extern uint32_t g_sceneSlotTypeTable[];      // @ base 0x825C9D28-ish (4 bytes per slot)
extern void*    g_pauseMgr;                  // @ 0x82606490 — +495 holds pause/hold-for-demo byte
extern void*    g_gameDataMgr;               // @ 0x82645A44 — +24 holds select-gate byte
extern void*    g_selectionKillSignal;       // @ 0x825C9F28 — +4 non-zero = force deactivate
extern uint32_t g_charEntryTable;            // @ 0x825CA130 — per-slot entry base used by UpdateMountedChars

/**
 * pongCharViewContext::RegisterSceneSlot (vfn_11)
 * @ 0x8230A898 | size: 0x60
 *
 * Registers a pointer to the secondary-vtable sub-object (this+20) in the
 * global scene-slot tables so the renderer ticks this context each frame.
 * Layout of the three parallel tables is:
 *   - byte  table @ base+14952 (tag = 128 for active)
 *   - u32   table @ base+14440 (slot pointer)
 *   - u32   table @ base+14696 (type code = 20)
 * Counter at base+14436 is post-incremented.
 */
bool pongCharViewContext::RegisterSceneSlot() {
    // The sub-object registered is at this+20; if 'this' is null we still write
    // a null slot pointer (matches the original 'cmplwi r3,0; bne' idiom).
    uint8_t* self = reinterpret_cast<uint8_t*>(this);
    void* slotPtr = (this != nullptr) ? static_cast<void*>(self + 20) : nullptr;

    // Base pointer to the scene slot tables lives at SDA-relative -32161|14952.
    // For the lift we treat the three tables as independent extern arrays; the
    // counter word is read, indexed into each, then written back +1.
    extern uint32_t g_sceneSlot_count;   // @ 0x825C9F64
    const uint32_t index = g_sceneSlot_count;

    g_sceneSlotTagTable[index]  = 128;   // active tag
    g_sceneSlotPtrTable[index]  = slotPtr;
    g_sceneSlotTypeTable[index] = 20;    // scene-slot type code

    g_sceneSlot_count = index + 1;
    return true;
}

/**
 * pongCharViewContext::ReleasePoseSource (vfn_12)
 * @ 0x8230AB50 | size: 0x54
 *
 * If a pose source is mounted at +44 (m_pManagedObject), invokes its
 * vtable slot 0 destructor with the deleting flag (param = 1) and nulls
 * the pointer. Matches the original 'lwz r3,44(r31); beq exit; lwz r11,
 * 0(r3); li r4,1; bctrl; stw r0,44(r31)' sequence.
 */
bool pongCharViewContext::ReleasePoseSource() {
    if (m_pManagedObject) {
        void** vtable = *(void***)m_pManagedObject;
        typedef void (*DeleteFunc)(void*, bool);
        DeleteFunc del = (DeleteFunc)vtable[0];
        del(m_pManagedObject, true);
        m_pManagedObject = nullptr;
    }
    return true;
}

/**
 * pongCharViewContext::TickScene (vfn_16)
 * @ 0x8230AC90 | size: 0x2BC
 *
 * Per-frame scene tick. Guarded by the compound "paused-or-selection-gated"
 * predicate used by all three tick methods here (hence the shared helper
 * below). The body is ~700 bytes of control-flow + vector math; the full
 * lift lives in the phase-5 recomp, so this thunk preserves behaviour by
 * delegating. Documented behaviour:
 *
 *   1. If first-tick byte at +76 is set, clear it and call pg_BCE8_g to
 *      resolve the new selection index (stored at +60).
 *   2. Fire the pose-source's slot 2 method (arm/begin pose).
 *   3. Based on the browse state at +24, dispatch into pg_B3C0_g (== 1),
 *      pg_B138_g (< 1), then, when zero, SinglesNetworkClient_B4F0_g.
 *   4. Reset view bound via phBoundCapsule_C2C8_fw (aka ResetViewBound)
 *      with the bound-extent float at entry[+40]->[+44]+192.
 *   5. For each of the two char slots, call AdvanceCharSlot(slot) and
 *      post-select dispatch pongCharViewContext_B7D8_wrh(slot); update the
 *      per-slot "active" byte at this[+77+slot] based on browse state.
 *   6. Call camera-shot vtable slot 31 on the global camera manager with
 *      the two per-slot bytes as a packed arg.
 *   7. Push zeroed pose transform into stack-local, then blend into scene
 *      via camera-manager slot 29.
 *
 * TODO: inline the vector body once the camera-shot vtable slots are
 * fully enumerated; current thunk is behaviourally identical.
 */
extern "C" void pongCharViewContext_vfn_16_body(void* self);  // 0x8230AC90 (unlifted)
void pongCharViewContext::TickScene() {
    pongCharViewContext_vfn_16_body(this);
}

/**
 * pongCharViewContext::UpdateMountedChars (vfn_17)
 * @ 0x8230AF50 | size: 0xAC
 *
 * Shared guard: tick only when paused flag at g_pauseMgr[+495] is set, or
 * when the game-data-manager gate byte at g_gameDataMgr[+24] is set. When
 * open, walk the two mounted-char slot bytes at this[+77..+78]. For each
 * set byte, locate the character entry in the global char entry table:
 *
 *     entry = charTable[this[+132 + slot*4]]         // slot -> index
 *     entry = charTable[entry + 29]                  // entry index -> ptr
 *
 * …and invoke its vtable slot 6 (per-char tick that drives pose/anim).
 */
void pongCharViewContext::UpdateMountedChars() {
    uint8_t* self = reinterpret_cast<uint8_t*>(this);

    // Compound guard matching the original 'if (pauseByte || gateByte)'.
    const uint8_t pauseByte = ((const uint8_t*)g_pauseMgr)[495];
    const uint8_t gateByte  = ((const uint8_t*)g_gameDataMgr)[24];
    if (!pauseByte && !gateByte) {
        return;
    }

    uint32_t* charTable = &g_charEntryTable;
    uint8_t*  slotBytes = self + 77;
    uint32_t* slotIndex = reinterpret_cast<uint32_t*>(self + 132);

    for (int slot = 0; slot < 2; ++slot) {
        if (slotBytes[slot] == 0) continue;

        // Two-level index hop matching the lift:
        //   tmp = charTable[slotIndex[slot]];
        //   entryPtr = charTable[tmp + 29];
        uint32_t tmp = charTable[slotIndex[slot]];
        void* entry  = reinterpret_cast<void*>(charTable[tmp + 29]);

        void** vtable = *(void***)entry;
        typedef void (*TickCharFunc)(void*);
        TickCharFunc tickChar = (TickCharFunc)vtable[6];
        tickChar(entry);
    }
}

/**
 * pongCharViewContext::UnmountCharSlots (vfn_18)
 * @ 0x8230C0A8 | size: 0x220
 *
 * Scene teardown for exiting the char view state.
 *   1. If a pose source is mounted at +44, invoke its slot 6 method to
 *      hand back the char (mirrors UpdateMountedChars slot 6 but with a
 *      "release" flag implicit in the object state).
 *   2. Dispatch the camera-manager's slot 29 to fade/freeze the scene
 *      using either its +76 (when g_pauseMgr[+576] is set) or +64 handler.
 *   3. For each of the two char slots, honour the global kill signal at
 *      g_selectionKillSignal[+4]: if non-zero or the slot byte was set,
 *      clear entry[+212] ("active") and entry[+288] ("visible") on the
 *      per-char ply manager at entry[+48]; conditionally call the
 *      plrPlayerMgr_A288 helper to decommit the preview mesh.
 *   4. Uses the same two-level charTable lookup as UpdateMountedChars.
 *
 * This is the scene half of OnExit — the lifecycle half is agent-13 scope.
 */
extern "C" void pongCharViewContext_vfn_18_body(void* self);  // 0x8230C0A8 (unlifted)
void pongCharViewContext::UnmountCharSlots() {
    // Compound gate matches TickScene; when closed we fall through without
    // doing anything (the body returns at loc_8230C2C0). Delegating here
    // keeps the 512-byte vector/branch body behaviourally exact.
    pongCharViewContext_vfn_18_body(this);
}

/**
 * pongCharViewContext::InitScene (vfn_23)
 * @ 0x8230A8F8 | size: 0x254
 *
 * Called from pongCharViewState::OnEnter as the initializer. Allocates the
 * 1288-byte pose-source object via the scene allocator, stores it at +44,
 * then loads "settings.xml" into the embedded atArray at +80 and walks the
 * resulting xmlTree to pick the first node that passes the "char view"
 * filter (vtable slot 20). That node is stored at +96 and drives the XML-
 * configured bone/pose/camera parameters.
 *
 * Notable details from the lift:
 *   - Uses atSingleton_B358/8588/29E0 for xmlTree / parser scaffolding.
 *   - String at lbl_8205F078 = "settings.xml".
 *   - Diagnostic at lbl_820327E4 = "unknown node type found in %s: %s".
 *   - Reference-counted XML parse buffer at 0x825D0080[+1536] is
 *     decremented on scope exit.
 *
 * The body mixes atArray helpers with raw pointer walking; thunked for
 * now to keep the xmlTree contract exact.
 */
extern "C" void pongCharViewContext_vfn_23_body(void* self);  // 0x8230A8F8 (unlifted)
void pongCharViewContext::InitScene() {
    pongCharViewContext_vfn_23_body(this);
}

// ────────────────────────────────────────────────────────────────────────────
// Non-virtual scene helpers
// ────────────────────────────────────────────────────────────────────────────

/**
 * ResetCharViewData @ 0x8240A570 | size: 0x8C
 *
 * Resets the 48-byte pose block at input pointer and two companion globals
 * (selection counter @ 0x82625E10, selection flag @ 0x82625E14, selected
 * char index @ 0x825D6B78 = -1). The pose block stores camera-orbit /
 * rig-pose floats driven each frame by TickScene.
 *
 * Offset map of the reset pose block:
 *    +0x04  int   current slot index        (0)
 *    +0x08  int   previous slot index       (0)
 *    +0x0C  float orbit yaw                 (0.0f)
 *    +0x10  float orbit pitch               (0.0f)
 *    +0x14  int   browse-state              (0)
 *    +0x18  int   browse-timer              (0)
 *    +0x1C  float rig-pose blend            (0.0f)
 *    +0x20  float rig-scale                 (0.0f)
 *    +0x24  float camera zoom target        (0.0f)
 *    +0x28  float camera zoom current       (0.0f)
 *    +0x2C  float mount offset X            (0.0f)
 *    +0x30  float mount offset Y            (0.0f)
 *
 * The function emits the same stores twice (mirror pass for both char
 * slots sharing the same layout) — that is the original assembly, kept
 * here verbatim so callers that rely on the full wipe see identical
 * behaviour.
 */
void ResetCharViewData(void* scenePoseBlock) {
    if (!scenePoseBlock) return;

    uint8_t* p = reinterpret_cast<uint8_t*>(scenePoseBlock);

    auto writeFloat = [&](int off, float v) {
        *reinterpret_cast<float*>(p + off) = v;
    };
    auto writeInt = [&](int off, int32_t v) {
        *reinterpret_cast<int32_t*>(p + off) = v;
    };

    // Pass 1 — primary pose block.
    writeInt(4, 0);  writeInt(8, 0);
    writeFloat(12, 0.0f);  writeFloat(16, 0.0f);
    writeInt(20, 0);  writeInt(24, 0);
    writeFloat(28, 0.0f);  writeFloat(32, 0.0f);
    writeFloat(36, 0.0f);  writeFloat(40, 0.0f);
    writeFloat(44, 0.0f);  writeFloat(48, 0.0f);

    // Pass 2 — mirror write (matches the duplicate stores in the lift).
    writeInt(4, 0);  writeInt(8, 0);
    writeFloat(12, 0.0f);  writeFloat(16, 0.0f);
    writeInt(20, 0);  writeInt(24, 0);
    writeFloat(28, 0.0f);  writeFloat(32, 0.0f);
    writeFloat(36, 0.0f);  writeFloat(40, 0.0f);
    writeFloat(44, 0.0f);  writeFloat(48, 0.0f);

    // Global companions: selection counter, selection flag, selected idx.
    g_charViewSelectCounter = 0;
    g_charViewSelectFlag    = 0;
    g_selectedCharacterIndex = -1;
}

/**
 * AdvanceCharSlot @ 0x8230B6E8 | size: 0xF0
 *
 * Per-slot browse-state advance. Called by TickScene once for each of the
 * two character slots. Three paths keyed on slot state at this[+28+slot*4]
 * (+7*4 == +28, so the state word sits at this+ (slot+7)*4):
 *
 *   state == 1 — "stepping":
 *     bump timer at this+72 and compare against the pose-source's step
 *     target at pose[+96][+24]. When reached, set state to 2. If slot 0
 *     with idle filter (browse==0 && pose float +48 == 0.0f constant),
 *     fire the post-select notification via SinglesNetworkClient_1118_g
 *     using the string at lbl_8205EFD4.
 *
 *   state == 2 — "settled": no-op, leaves pose untouched.
 *
 *   other — "idle": if the select index at this+60 is non-negative and
 *     the slot filter passes (SinglesNetworkClient_82F0_g with
 *     slot-nonzero = first, slot-zero = second), fire the pose apply
 *     helper ref_gdDrillMovementData_B960(slot).
 */
void AdvanceCharSlot(void* ctxPtr, int slot) {
    uint8_t* self = reinterpret_cast<uint8_t*>(ctxPtr);

    // State word per slot: at ((slot+7) * 4) == +28 for slot 0, +32 for slot 1.
    const int stateOff = ((slot + 7) * 4);
    int32_t state = *reinterpret_cast<int32_t*>(self + stateOff);

    if (state == 1) {
        // Stepping — bump timer, compare to pose target, maybe flip to settled.
        int32_t* pTimer = reinterpret_cast<int32_t*>(self + 72);
        int32_t timer = *pTimer + 1;
        *pTimer = timer;

        void* poseSource = *reinterpret_cast<void**>(self + 96);
        const int32_t target = *reinterpret_cast<const int32_t*>(
            reinterpret_cast<const uint8_t*>(poseSource) + 24);

        if (timer < target) return;

        // Settle.
        *reinterpret_cast<int32_t*>(self + stateOff) = 2;
        if (slot != 0) return;

        // Slot-0 post-select notification only fires when the browse state
        // at +24 is idle (==0) and the idle-pose float at +48 is exactly
        // the zero constant at lbl_821F27D8 (0.0f).
        const int32_t browse = *reinterpret_cast<const int32_t*>(self + 24);
        const float   idlePose = *reinterpret_cast<const float*>(self + 48);
        if (browse != 0 || idlePose != 0.0f) return;

        void* poseCtx = *reinterpret_cast<void**>(self + 44);
        extern void SinglesNetworkClient_1118_g(void*, const char*, int);  // 0x82321118
        extern const char g_str_charview_finalize[];  // lbl_8205EFD4
        SinglesNetworkClient_1118_g(poseCtx, g_str_charview_finalize, 0);
        return;
    }

    if (state == 2) {
        return;  // Settled slot is a no-op.
    }

    // Idle — try to apply a new pose if the select index is valid and the
    // slot predicate passes.
    const int32_t selectIdx = *reinterpret_cast<const int32_t*>(self + 60);
    if (selectIdx <= -1) return;

    (void)*reinterpret_cast<const int32_t*>(self + 64);  // secondary index, consumed by helper
    extern bool SinglesNetworkClient_82F0_g(void* mgr, int slotIsFirst);  // 0x821882F0
    extern void* g_gameDataMgr_arg;  // @ 0x825C9DC0 (mgr pointer for filter)
    extern void ref_gdDrillMovementData_B960(void* ctx, int slot);  // 0x8230B960

    const bool slotIsFirst = (slot != 0);
    if (!SinglesNetworkClient_82F0_g(&g_gameDataMgr_arg, slotIsFirst ? 1 : 0)) return;

    ref_gdDrillMovementData_B960(ctxPtr, slot);
}
