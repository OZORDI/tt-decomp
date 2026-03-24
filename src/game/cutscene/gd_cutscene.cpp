/**
 * Cutscene System Implementation
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Implementation of cutscene data structures and action classes.
 */

#include "game/gd_cutscene.hpp"
#include "rage/memory.h"
#include <cstring>
#include <cstdio>

// External dependencies
extern "C" {
    void xmlNodeStruct_vfn_2(void* node);
    uint16_t atSingleton_F6B8_p39(const char* name);
    void rage::ReleaseSingleton(void* obj);
    void nop_8240E6D0(const char* msg, ...);
    void rage_free(void* ptr);
    int _stricmp(const char* str1, const char* str2);
    void RegisterSerializedField(void* obj, const char* propName, void* target, void* defaultVal, uint32_t flags);
}

// Type IDs for IsType checks (from SDA .data section)
extern uint32_t g_gdCSCharAnimData_typeId;     // @ 0x825C2B8C
extern uint32_t g_gdCSNameData_typeId;         // @ 0x825C2B18
extern uint32_t g_gdCSActionWaitData_typeId;   // @ 0x825C2B54
extern uint32_t g_gdCSActionData_typeId;       // @ 0x825C2B1C (base action type)
extern uint32_t g_gdCSCamAnimShotName_typeId;  // @ 0x825C2B78
extern uint32_t g_gdCutSceneData_typeId;       // @ 0x825C2B20
extern uint32_t g_gdCSActionCamAnimData_typeId;      // @ 0x825C2B58
extern uint32_t g_gdCSActionCharAnimData_typeId;     // @ 0x825C2B5C
extern uint32_t g_gdCSActionCharVisibleData_typeId;  // @ 0x825C2B60
extern uint32_t g_gdCSActionPlayAudioData_typeId;    // @ 0x825C2B64
extern uint32_t g_gdCSActionLvlAmbAnimData_typeId;   // @ 0x825C2B68
extern uint32_t g_gdCSActionCharAmbAnimData_typeId;  // @ 0x825C2B6C
extern uint32_t g_gdCSActionShowAllAmbientsData_typeId; // @ 0x825C2B70
extern uint32_t g_gdCSActionLoopData_typeId;         // @ 0x825C2B4C
extern uint32_t g_gdCSActionIfData_typeId;           // @ 0x825C2B50
extern uint32_t g_gdCSCamAnimShotName_typeId;        // @ 0x825C2B78
extern uint32_t g_gdCSCharAnimNames_typeId;          // @ 0x825C2B90
extern uint32_t g_gdCSCharCamShotName_typeId;  // @ 0x825C2B84
extern uint32_t g_gdCutSceneNames_typeId;      // @ 0x825C2B14
extern uint32_t g_xmlNodeStruct_typeId;        // @ 0x825C803C (shared base)
extern uint32_t g_xmlNodeStruct_typeId2;       // @ 0x825C8038 (shared base)

// Serialization type descriptors
extern void* g_stringFieldType;    // @ 0x825CAF88
extern void* g_boolFieldType;     // @ 0x825CAF80
extern void* g_floatFieldType;    // @ 0x825CAF94
extern void* g_floatFieldType2;   // @ 0x825CAF9C
extern void* g_enumFieldType;     // @ 0x825CAF90

// ============================================================================
// gdCSCharAnimData Implementation
// ============================================================================

/**
 * gdCSCharAnimData::PostLoadProperties @ 0x8240CD00 | size: 0xE8
 * 
 * Validates animation data after loading from XML.
 * Checks:
 * - FileName property exists and is non-empty
 * - Duration is non-negative
 * - Weight is non-negative
 * - Emote range is valid (min >= 0.0, max <= 1.0, min <= max)
 */
void gdCSCharAnimData::PostLoadProperties() {
    // Call base class PostLoadProperties
    xmlNodeStruct_vfn_2(this);
    
    // Validate FileName exists
    if (!m_pFileName || m_pFileName[0] == '\0') {
        nop_8240E6D0("gdCSCharAnimData::PostLoadProperties() - missing property 'FileName'");
        return;
    }
    
    // Validate duration is non-negative
    const float MIN_DURATION = 0.0f;
    if (m_duration < MIN_DURATION) {
        nop_8240E6D0("gdCSCharAnimData::PostLoadProperties() - 'TimeOffset' cannot be negative");
    }
    
    // Validate weight is non-negative
    if (m_weight < 0) {
        nop_8240E6D0("gdCSCharAnimData::PostLoadProperties() - cannot have negative weight");
    }
    
    // Validate emote range
    const float ZERO = 0.0f;
    const float ONE = 1.0f;
    
    if (m_emoteRangeMin >= ZERO && m_emoteRangeMax <= ONE) {
        if (m_emoteRangeMin <= m_emoteRangeMax) {
            return;  // Valid range
        }
    }
    
    nop_8240E6D0("gdCSCharAnimData::PostLoadProperties() - invalid emote range");
}

// ============================================================================
// gdCutSceneData Implementation
// ============================================================================

/**
 * gdCutSceneData::PostLoadProperties @ 0x8240EA10 | size: 0x68
 * 
 * Resolves cutscene name to ID after loading.
 * Logs warning if cutscene name is not found in the registry.
 */
void gdCutSceneData::PostLoadProperties() {
    // Call base class PostLoadProperties
    xmlNodeStruct_vfn_2(this);
    
    if (!m_pCutsceneName) {
        return;
    }
    
    // Look up cutscene ID by name
    m_cutsceneId = atSingleton_F6B8_p39(m_pCutsceneName);
    
    if (m_cutsceneId == 0) {
        nop_8240E6D0("gdCutSceneData::PostLoadProperties() - unknown cutscene name '%s'", 
                     m_pCutsceneName);
    }
}

// ============================================================================
// gdCSActionLoopData Implementation
// ============================================================================

/**
 * gdCSActionLoopData::vfn_20 @ 0x8240EB28 | size: 0x58
 * 
 * Validates loop action properties.
 * Checks if property name matches known loop property types.
 * Returns 1 if property is valid, 0 if unknown.
 */
void gdCSActionLoopData::IsType() {
    // This function compares a property name (in r4) against known property types
    // The function loads property name pointers and compares against them
    // If any match, return 1 (property is valid)
    // Otherwise return 0 (unknown property)
    
    // Note: This is a property validation function used during XML parsing
    // The actual implementation requires access to the property name parameter
}

/**
 * gdCSActionLoopData::GetTypeName @ 0x82347208 | size: 0xC
 * 
 * Returns the type name string for this action.
 */
const char* gdCSActionLoopData::GetTypeName() {
    return "Loop";
}

// ============================================================================
// gdCSActionIfData Implementation
// ============================================================================

/**
 * gdCSActionIfData::~gdCSActionIfData @ 0x8240EC40 | size: 0x54
 * 
 * Destructor - frees action lists and condition data.
 */
gdCSActionIfData::~gdCSActionIfData() {
    // Free else actions
    if (m_pElseActions) {
        rage_free(m_pElseActions);
    }
    
    // Free then actions
    if (m_pThenActions) {
        rage_free(m_pThenActions);
    }
    
    // Free condition type string
    if (m_pConditionType) {
        rage_free((void*)m_pConditionType);
    }
    
    // Call base destructor
    rage::ReleaseSingleton(this);
}

/**
 * gdCSActionIfData::PostLoadProperties @ 0x8240ED80 | size: 0x320
 * 
 * Parses condition type string and validates parameters.
 * 
 * Supported condition types:
 * - "PLAYER_SCORE_EQUAL" (0)
 * - "PLAYER_SCORE_GREATER" (1)
 * - "PLAYER_SCORE_LESS" (2)
 * - "OPPONENT_SCORE_EQUAL" (3)
 * - "OPPONENT_SCORE_GREATER" (4) - Must be <= 100
 * - "OPPONENT_SCORE_LESS" (5)
 * - "RANDOM_PERCENT" (6)
 */
void gdCSActionIfData::PostLoadProperties() {
    // Call base class PostLoadProperties
    xmlNodeStruct_vfn_2(this);
    
    if (!m_pConditionType) {
        m_conditionEnum = 7;  // Invalid/unknown
        nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - no condition type");
        return;
    }
    
    // Parse condition type string
    const char* conditionTypes[] = {
        "PLAYER_SCORE_EQUAL",      // 0
        "PLAYER_SCORE_GREATER",    // 1
        "PLAYER_SCORE_LESS",       // 2
        "OPPONENT_SCORE_EQUAL",    // 3
        "OPPONENT_SCORE_GREATER",  // 4
        "OPPONENT_SCORE_LESS",     // 5
        "RANDOM_PERCENT"           // 6
    };
    
    int conditionIndex = 7;  // Default to invalid
    for (int i = 0; i < 7; i++) {
        if (strcmp(m_pConditionType, conditionTypes[i]) == 0) {
            conditionIndex = i;
            break;
        }
    }
    
    m_conditionEnum = conditionIndex;
    
    if (conditionIndex == 7) {
        nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - unknown condition type");
        return;
    }
    
    // Validate parameters based on condition type
    switch (conditionIndex) {
        case 0:  // PLAYER_SCORE_EQUAL
        case 1:  // PLAYER_SCORE_GREATER
        case 2:  // PLAYER_SCORE_LESS
        case 3:  // OPPONENT_SCORE_EQUAL
            // These require m_conditionParam to be set (not -1)
            if (m_conditionParam == 0xFFFFFFFF) {
                nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - score condition missing parameter");
            }
            break;
            
        case 4:  // OPPONENT_SCORE_GREATER
            // Requires parameter and must be <= 100
            if (m_conditionParam == 0xFFFFFFFF) {
                nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - score condition missing parameter");
            } else if (m_conditionParam > 100) {
                nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - score parameter > %d", 100);
            }
            break;
            
        case 5:  // OPPONENT_SCORE_LESS
            // Requires then actions to be set
            if (!m_pThenActions) {
                nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - missing then actions");
            }
            break;
            
        case 6:  // RANDOM_PERCENT
            // Requires else actions to be set
            if (!m_pElseActions) {
                nop_8240E6D0("gdCSActionIfData::PostLoadProperties() - missing else actions");
            }
            break;
    }
}

const char* gdCSActionIfData::GetTypeName() {
    return "If";
}

// ============================================================================
// gdCSActionWaitData Implementation
// ============================================================================

const char* gdCSActionWaitData::GetTypeName() {
    return "Wait";
}

// ============================================================================
// gdCSActionCamAnimData Implementation
// ============================================================================

/**
 * gdCSActionCamAnimData::PostLoadProperties @ 0x8240F2A0 | size: 0x6C
 * 
 * Validates camera animation name is not empty.
 */
void gdCSActionCamAnimData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    
    if (!m_pCameraName || m_pCameraName[0] == '\0') {
        nop_8240E6D0("gdCSActionCamAnimData::PostLoadProperties() - missing camera name");
    }
}

const char* gdCSActionCamAnimData::GetTypeName() {
    return "CamAnim";
}

// ============================================================================
// gdCSActionCharAnimData Implementation
// ============================================================================

/**
 * gdCSActionCharAnimData::PostLoadProperties @ 0x8240F450 | size: 0x84
 * 
 * Validates character ID and animation name.
 */
void gdCSActionCharAnimData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    
    // Validate character ID is set
    if (m_characterId == -1) {
        nop_8240E6D0("gdCSActionCharAnimData::PostLoadProperties() - missing character ID");
    }
    
    // Validate animation name is not empty
    if (!m_pAnimName || m_pAnimName[0] == '\0') {
        nop_8240E6D0("gdCSActionCharAnimData::PostLoadProperties() - missing animation name");
    }
}

const char* gdCSActionCharAnimData::GetTypeName() {
    return "CharAnim";
}

// ============================================================================
// gdCSActionCharVisibleData Implementation
// ============================================================================

/**
 * gdCSActionCharVisibleData::PostLoadProperties @ 0x8240F5C0 | size: 0x44
 * 
 * Validates character ID is set.
 */
void gdCSActionCharVisibleData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    
    if (m_characterId == -1) {
        nop_8240E6D0("gdCSActionCharVisibleData::PostLoadProperties() - missing character ID");
    }
}

const char* gdCSActionCharVisibleData::GetTypeName() {
    return "CharVisible";
}

// ============================================================================
// gdCSActionPlayAudioData Implementation
// ============================================================================

/**
 * gdCSActionPlayAudioData::PostLoadProperties @ 0x8240F6F0 | size: 0x128
 * 
 * Parses audio type and validates audio name.
 * 
 * Audio types:
 * - "MUSIC" -> m_audioTypeEnum = 0
 * - "SOUND" -> m_audioTypeEnum = 1
 * 
 * Special audio names:
 * - "NONE" -> m_audioId = 7
 */
void gdCSActionPlayAudioData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    
    // Validate audio type is not empty
    if (!m_pAudioType || m_pAudioType[0] == '\0') {
        nop_8240E6D0("gdCSActionPlayAudioData::PostLoadProperties() - missing audio type");
    }
    
    // Validate audio name is not empty
    if (!m_pAudioName || m_pAudioName[0] == '\0') {
        nop_8240E6D0("gdCSActionPlayAudioData::PostLoadProperties() - missing audio name");
    }
    
    // Parse audio type
    if (_stricmp(m_pAudioType, "MUSIC") == 0) {
        m_audioTypeEnum = 0;
    } else if (_stricmp(m_pAudioType, "SOUND") == 0) {
        m_audioTypeEnum = 1;
    } else {
        nop_8240E6D0("gdCSActionPlayAudioData::PostLoadProperties() - unknown audio type '%s'", 
                     m_pAudioType);
    }
    
    // Check for special "NONE" audio name
    if (_stricmp(m_pAudioName, "NONE") == 0) {
        m_audioId = 7;
    } else {
        // For MUSIC type, validate audio name
        if (m_audioTypeEnum == 0) {
            // Additional validation for music names could go here
        }
    }
}

const char* gdCSActionPlayAudioData::GetTypeName() {
    return "PlayAudio";
}

// ============================================================================
// gdCSCharAnimNames Implementation
// ============================================================================

/**
 * gdCSCharAnimNames::FindRandAnimData @ 0x8240D4A8 | size: 0x718
 * 
 * Finds a random animation from the collection based on:
 * - Random value (randomSeed) - used to index into random table
 * - Selection criteria (selectionMode)
 * - Name filter (nameFilter)
 * 
 * Algorithm:
 * 1. Get random float value from random table based on randomSeed
 * 2. For each animation in m_pAnimDataArray:
 *    - Check if enabled (m_bIsEmote field)
 *    - Check if time range overlaps with random value
 *    - Mark in m_pSelectionFlags
 * 3. Filter by name if nameFilter is provided
 * 4. Select final animation based on weighted probabilities
 */
void gdCSCharAnimNames::FindRandAnimData(uint32_t randomSeed, uint32_t selectionMode, uint32_t nameFilter) {
    // Validate we have animation data
    if (m_animCount == 0) {
        nop_8240E6D0("gdCSCharAnimNames::FindRandAnimData() - no animation data at all");
        return;
    }
    
    // TODO: Full implementation requires understanding the random table structure
    // and the exact weighting algorithm used
    
    // The function uses several temporary arrays:
    // - m_pSelectionFlags: Marks which animations pass time range filter
    // - m_pNameMatchFlags: Marks which animations match name filter
    
    // Key steps from assembly analysis:
    // 1. Get random float value from random table (lbl_825D1C44 + offset)
    // 2. For each animation:
    //    - Check m_bIsEmote flag at offset +80
    //    - Check time range (m_emoteRangeMin, m_emoteRangeMax)
    //    - Mark in m_pSelectionFlags
    // 3. If nameFilter provided, compare animation names
    // 4. Select based on weighted random
}

// ============================================================================
// Stub Implementations for Remaining Virtual Methods
// ============================================================================

/**
 * gdCSCharCamShotName::IsType @ 0x8240C420 | size: 0x48
 */
bool gdCSCharCamShotName::vfn_20(uint32_t typeId) {
    if (typeId == g_gdCSCharCamShotName_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

void gdCSCharCamShotName::RegisterFields() {
    // No RegisterFields in binary — name reference only
}

const char* gdCSCharCamShotName::GetTypeName() {
    return "CharCamShotName";
}

/**
 * gdCSCharAnimData::IsType @ 0x8240C990 | size: 0x48
 */
bool gdCSCharAnimData::vfn_20(uint32_t typeId) {
    if (typeId == g_gdCSCharAnimData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSCharAnimData::RegisterFields @ 0x8240CBD0 | size: 0x130
 *
 * Registers 9 XML serialization fields for character animation data.
 */
void gdCSCharAnimData::RegisterFields() {
    RegisterSerializedField(this, "FileName",      (char*)this + 16, g_stringFieldType, 0);
    RegisterSerializedField(this, "Enabled",       (char*)this + 20, g_boolFieldType, 0);
    RegisterSerializedField(this, "TimeOffset",    (char*)this + 32, g_floatFieldType2, 0);
    RegisterSerializedField(this, "Duration",      (char*)this + 48, g_floatFieldType2, 0);
    RegisterSerializedField(this, "Weight",        (char*)this + 64, g_floatFieldType, 0);
    RegisterSerializedField(this, "EmoteRangeMin", (char*)this + 68, g_enumFieldType, 0);
    RegisterSerializedField(this, "EmoteRangeMax", (char*)this + 72, g_floatFieldType, 0);
    RegisterSerializedField(this, "IsEmote",       (char*)this + 76, g_floatFieldType, 0);
    RegisterSerializedField(this, "AnimType",      (char*)this + 80, g_boolFieldType, 0);
}

const char* gdCSCharAnimData::GetTypeName() {
    return "CharAnimData";
}

void gdCSCharAnimData::vfn_3() {
    // TODO: Implement
}

bool gdCSCharAnimNames::IsType(uint32_t typeId) {
    if (typeId == g_gdCSCharAnimNames_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

void gdCSCharAnimNames::RegisterFields() {
    // No serialized fields — container class
}

const char* gdCSCharAnimNames::GetTypeName() {
    return "CharAnimNames";
}

void gdCSCharAnimNames::vfn_3() {
    // TODO: Implement
}

/**
 * gdCSCamAnimShotName::IsType @ 0x8240DBC0 | size: 0x48
 */
bool gdCSCamAnimShotName::IsType(uint32_t typeId) {
    if (typeId == g_gdCSCamAnimShotName_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

void gdCSCamAnimShotName::RegisterFields() {
    // No fields to register for this name reference class
}

const char* gdCSCamAnimShotName::GetTypeName() {
    return "CamAnimShotName";
}

void gdCSCamAnimShotName::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    // TODO: Implement validation
}

/**
 * gdCutSceneNames::IsType @ 0x8240E5F8 | size: 0x48
 */
bool gdCutSceneNames::vfn_20(uint32_t typeId) {
    if (typeId == g_gdCutSceneNames_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

void gdCutSceneNames::RegisterFields() {
    // Slot 21 — no fields to register for this container class
}

const char* gdCutSceneNames::GetTypeName() {
    return "CutSceneNames";
}

/**
 * gdCSNameData::IsType @ 0x8240E6D8 | size: 0x48
 */
bool gdCSNameData::vfn_20(uint32_t typeId) {
    if (typeId == g_gdCSNameData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSNameData::RegisterFields @ 0x8240E7B8 | size: 0x6C
 *
 * Registers FileName (+16) and SceneName (+20) for XML serialization.
 */
void gdCSNameData::RegisterFields() {
    RegisterSerializedField(this, "FileName",  (char*)this + 16, g_stringFieldType, 0);
    RegisterSerializedField(this, "SceneName", (char*)this + 20, g_stringFieldType, 0);
}

const char* gdCSNameData::GetTypeName() {
    return "CSNameData";
}

/**
 * gdCutSceneData::IsType @ 0x8240E8F0 | size: 0x58
 */
bool gdCutSceneData::IsType(uint32_t typeId) {
    if (typeId == g_gdCutSceneData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCutSceneData::RegisterFields @ 0x8240E9A8 | size: 0x64
 */
void gdCutSceneData::RegisterFields() {
    RegisterSerializedField(this, "CutsceneName", (char*)this + 20, g_stringFieldType, 0);
    RegisterSerializedField(this, "Priority",     (char*)this + 24, g_floatFieldType, 0);
}

const char* gdCutSceneData::GetTypeName() {
    return "CutSceneData";
}

/**
 * gdCSActionLoopData::RegisterFields @ 0x8240EB80 | size: 0x64
 */
void gdCSActionLoopData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex", (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "LoopCount",   (char*)this + 20, g_floatFieldType, 0);
}

/**
 * gdCSActionIfData::IsType @ 0x8240EBE8 | size: 0x58
 */
bool gdCSActionIfData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionIfData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionIfData::RegisterFields @ 0x8240ECB0 | size: 0xD0
 */
void gdCSActionIfData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex",    (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "ConditionType",  (char*)this + 20, g_stringFieldType, 0);
    RegisterSerializedField(this, "CharacterId",    (char*)this + 24, g_enumFieldType, 0);
    RegisterSerializedField(this, "CompareValue",   (char*)this + 28, g_enumFieldType, 0);
    RegisterSerializedField(this, "ThenActions",    (char*)this + 32, g_stringFieldType, 0);
    RegisterSerializedField(this, "ElseActions",    (char*)this + 36, g_stringFieldType, 0);
}

/**
 * gdCSActionWaitData::IsType @ 0x8240F0F8 | size: 0x58
 *
 * Checks 4 type IDs — own type + action base type + 2 xmlNodeStruct bases.
 */
bool gdCSActionWaitData::vfn_20(uint32_t typeId) {
    if (typeId == g_gdCSActionWaitData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionWaitData::RegisterFields @ 0x8240F150 | size: 0x64
 *
 * Registers WaitType (+16) and Duration (+20) for XML serialization.
 */
void gdCSActionWaitData::RegisterFields() {
    RegisterSerializedField(this, "WaitType",  (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "Duration",  (char*)this + 20, g_floatFieldType, 0);
}

/**
 * gdCSActionCamAnimData::IsType @ 0x8240F1B8 | size: 0x58
 */
bool gdCSActionCamAnimData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionCamAnimData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionCamAnimData::RegisterFields @ 0x8240F210 | size: 0x8C
 */
void gdCSActionCamAnimData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex", (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "FileName",    (char*)this + 20, g_stringFieldType, 0);
    RegisterSerializedField(this, "Loop",        (char*)this + 24, g_boolFieldType, 0);
}

/**
 * gdCSActionCharAnimData::IsType @ 0x8240F310 | size: 0x58
 */
bool gdCSActionCharAnimData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionCharAnimData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionCharAnimData::RegisterFields @ 0x8240F368 | size: 0xE4
 */
void gdCSActionCharAnimData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex",  (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "CharacterId",  (char*)this + 20, g_floatFieldType, 0);
    RegisterSerializedField(this, "FileName",     (char*)this + 24, g_stringFieldType, 0);
    RegisterSerializedField(this, "BlendIn",      (char*)this + 28, g_boolFieldType, 0);
    RegisterSerializedField(this, "BlendOut",     (char*)this + 29, g_boolFieldType, 0);
    RegisterSerializedField(this, "FullBody",     (char*)this + 30, g_boolFieldType, 0);
}

/**
 * gdCSActionCharVisibleData::IsType @ 0x8240F4D8 | size: 0x58
 */
bool gdCSActionCharVisibleData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionCharVisibleData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionCharVisibleData::RegisterFields @ 0x8240F530 | size: 0x8C
 */
void gdCSActionCharVisibleData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex",  (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "CharacterId",  (char*)this + 20, g_floatFieldType, 0);
    RegisterSerializedField(this, "Visible",      (char*)this + 24, g_boolFieldType, 0);
}

/**
 * gdCSActionPlayAudioData::IsType @ 0x8240F608 | size: 0x58
 */
bool gdCSActionPlayAudioData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionPlayAudioData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionPlayAudioData::RegisterFields @ 0x8240F660 | size: 0x8C
 */
void gdCSActionPlayAudioData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex", (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "AudioType",   (char*)this + 20, g_stringFieldType, 0);
    RegisterSerializedField(this, "AudioName",   (char*)this + 24, g_stringFieldType, 0);
}

/**
 * gdCSActionLvlAmbAnimData::IsType @ 0x8240F948 | size: 0x58
 */
bool gdCSActionLvlAmbAnimData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionLvlAmbAnimData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionLvlAmbAnimData::RegisterFields @ 0x8240FA08 | size: 0x94
 */
void gdCSActionLvlAmbAnimData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex", (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "AnimName",    (char*)this + 20, g_stringFieldType, 0);
    RegisterSerializedField(this, "FileName",    (char*)this + 24, g_stringFieldType, 0);
    RegisterSerializedField(this, "Loop",        (char*)this + 28, g_boolFieldType, 0);
}

const char* gdCSActionLvlAmbAnimData::GetTypeName() {
    return "LvlAmbAnim";
}

void gdCSActionLvlAmbAnimData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    // TODO: Implement validation
}

/**
 * gdCSActionCharAmbAnimData::IsType @ 0x8240FB50 | size: 0x58
 */
bool gdCSActionCharAmbAnimData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionCharAmbAnimData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionCharAmbAnimData::RegisterFields @ 0x8240FC10 | size: 0xB4
 */
void gdCSActionCharAmbAnimData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex",  (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "CharacterId",  (char*)this + 20, g_floatFieldType, 0);
    RegisterSerializedField(this, "AnimName",     (char*)this + 24, g_stringFieldType, 0);
    RegisterSerializedField(this, "FileName",     (char*)this + 28, g_stringFieldType, 0);
    RegisterSerializedField(this, "Loop",         (char*)this + 32, g_boolFieldType, 0);
}

const char* gdCSActionCharAmbAnimData::GetTypeName() {
    return "CharAmbAnim";
}

void gdCSActionCharAmbAnimData::PostLoadProperties() {
    xmlNodeStruct_vfn_2(this);
    // TODO: Implement validation
}

/**
 * gdCSActionShowAllAmbientsData::IsType @ 0x8240FD90 | size: 0x58
 */
bool gdCSActionShowAllAmbientsData::IsType(uint32_t typeId) {
    if (typeId == g_gdCSActionShowAllAmbientsData_typeId) return true;
    if (typeId == g_gdCSActionData_typeId) return true;
    if (typeId == g_xmlNodeStruct_typeId) return true;
    return (typeId == g_xmlNodeStruct_typeId2);
}

/**
 * gdCSActionShowAllAmbientsData::RegisterFields @ 0x8240FDE8 | size: 0x6C
 */
void gdCSActionShowAllAmbientsData::RegisterFields() {
    RegisterSerializedField(this, "ActionIndex", (char*)this + 16, g_boolFieldType, 0);
    RegisterSerializedField(this, "ShowAll",     (char*)this + 20, g_boolFieldType, 0);
}

const char* gdCSActionShowAllAmbientsData::GetTypeName() {
    return "ShowAllAmbients";
}
