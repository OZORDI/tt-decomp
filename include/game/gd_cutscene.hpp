/**
 * Cutscene System - Game Data Structures
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * This file contains the cutscene system classes used for scripted sequences,
 * camera animations, character animations, and cutscene actions.
 * 
 * The system is organized into three main categories:
 * 1. Name/Identifier classes - Store references to animations and shots
 * 2. Data classes - Core cutscene data and configuration
 * 3. Action classes - Cutscene script actions (loop, if, wait, etc.)
 * 
 * All classes are XML-serializable and use the xmlNodeStruct base system.
 */

#pragma once

#include <cstdint>

// Forward declarations
struct xmlNodeStruct;

// ============================================================================
// Name/Identifier Classes
// ============================================================================

/**
 * gdCSCharCamShotName @ vtable 0x82076CE4
 * 
 * Stores character camera shot name reference for cutscenes.
 */
struct gdCSCharCamShotName {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSCharCamShotName() = default;
    virtual void vfn_20();  // @ 0x8240C420
    virtual void vfn_21();  // PostLoadProperties-related
    virtual const char* GetTypeName();  // @ 0x8240C468 - Returns type string
};

/**
 * gdCSCharAnimData @ vtable 0x82076D4C | size: 0x64 (100 bytes)
 * 
 * Character animation data for cutscenes.
 * Contains animation file name, timing, weight, and emote range parameters.
 * 
 * XML Properties:
 * - FileName: Animation file name
 * - Enabled: Whether animation is enabled (default: true)
 * - TimeOffset: Time offset for animation start (default: -1.0)
 * - Duration: Animation duration
 * - Weight: Animation selection weight (default: 1)
 * - EmoteRangeMin: Minimum emote range value (default: 0.0)
 * - EmoteRangeMax: Maximum emote range value (default: 1.0)
 * - IsEmote: Whether this is an emote animation
 */
struct gdCSCharAnimData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    const char* m_pFileName;        // +0x10 - Animation file name
    uint8_t m_bEnabled;             // +0x14 - Enabled flag (default: 1)
    uint8_t m_padding_15[11];       // +0x15 - Padding
    float m_timeOffset;             // +0x20 - Time offset (default: -1.0)
    float m_field_24;               // +0x24 - (default: -1.0)
    float m_field_28;               // +0x28 - (default: -1.0)
    uint32_t m_field_2C;            // +0x2C
    float m_field_30;               // +0x30 - (default: 0.0)
    float m_field_34;               // +0x34 - (default: 0.0)
    float m_field_38;               // +0x38 - (default: 0.0)
    uint32_t m_field_3C;            // +0x3C
    float m_duration;               // +0x40 - Animation duration (default: 0.0)
    int32_t m_weight;               // +0x44 - Selection weight (default: 1)
    float m_emoteRangeMin;          // +0x48 - Emote range min (default: 0.0)
    float m_emoteRangeMax;          // +0x4C - Emote range max (default: 1.0)
    uint8_t m_bIsEmote;             // +0x50 - Is emote flag (default: 0)
    uint8_t m_padding_51[3];        // +0x51 - Padding
    uint32_t m_field_54;            // +0x54
    uint16_t m_field_58;            // +0x58
    uint16_t m_field_5A;            // +0x5A
    uint32_t m_field_5C;            // +0x5C
    uint16_t m_field_60;            // +0x60
    uint16_t m_field_62;            // +0x62
    
    // Virtual methods
    virtual ~gdCSCharAnimData() = default;
    virtual void vfn_20();  // @ 0x8240C990
    virtual void vfn_21();  // @ 0x8240CBD0 - Register XML properties
    virtual const char* GetTypeName();  // @ 0x8240C9D8
    virtual void vfn_3();  // @ 0x8240CDE8
    
    // Methods
    void PostLoadProperties();  // @ 0x8240CD00 - Validate animation data
};

/**
 * gdCSCharAnimNames @ vtable 0x82076DB4
 * 
 * Collection of character animation names with random selection support.
 * Supports weighted random selection and name filtering.
 */
struct gdCSCharAnimNames {
    void* vtable;                       // +0x00
    uint32_t m_field_04;                // +0x04
    uint32_t m_field_08;                // +0x08
    uint32_t m_field_0C;                // +0x0C
    gdCSCharAnimData** m_pAnimDataArray;  // +0x10 - Array of animation data pointers
    uint16_t m_animCount;               // +0x14 - Number of animations (also at +0x1C)
    uint16_t m_field_16;                // +0x16
    uint32_t* m_pSelectionFlags;        // +0x18 - Flags for which anims pass time filter
    uint16_t m_field_1C;                // +0x1C - Also animation count
    uint16_t m_field_1E;                // +0x1E
    uint32_t* m_pNameMatchFlags;        // +0x20 - Flags for name matching
    uint16_t m_field_24;                // +0x24 - Also animation count
    uint16_t m_field_26;                // +0x26
    
    // Virtual methods
    virtual ~gdCSCharAnimNames() = default;
    virtual void vfn_20();  // @ 0x8240D130
    virtual void vfn_21();
    virtual const char* GetTypeName();  // @ 0x8240D178
    virtual void vfn_3();  // @ 0x8240D340
    
    // Methods
    void FindRandAnimData(uint32_t randomSeed, uint32_t selectionMode, uint32_t nameFilter);  // @ 0x8240D4A8
};

/**
 * gdCSCamAnimShotName @ vtable 0x82077024
 * 
 * Camera animation shot name reference.
 */
struct gdCSCamAnimShotName {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSCamAnimShotName() = default;
    virtual void vfn_20();  // @ 0x8240DBC0
    virtual void vfn_21();  // @ 0x8240DD18
    virtual const char* GetTypeName();  // @ 0x8240DC08
    
    // Methods
    void PostLoadProperties();  // @ 0x8240DDC0
};

/**
 * gdCutSceneNames @ vtable 0x820779DC
 * 
 * Collection of cutscene names.
 */
struct gdCutSceneNames {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCutSceneNames() = default;
    virtual void vfn_20();  // @ 0x8240E5F8
    virtual void vfn_21();
    virtual const char* GetTypeName();  // @ 0x8240E640
};

/**
 * gdCSNameData @ vtable 0x82077A44
 * 
 * Generic cutscene name data container.
 */
struct gdCSNameData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSNameData() = default;
    virtual void vfn_20();
    virtual void vfn_21();
    virtual const char* GetTypeName();
};

// ============================================================================
// Core Data Classes
// ============================================================================

/**
 * gdCutSceneData @ vtable 0x82077AAC
 * 
 * Main cutscene data container.
 * Stores cutscene name and resolves it to an ID during PostLoadProperties.
 */
struct gdCutSceneData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    uint32_t m_field_10;            // +0x10
    const char* m_pCutsceneName;    // +0x14 - Cutscene name string
    uint32_t m_field_18;            // +0x18
    uint16_t m_cutsceneId;          // +0x1C - Resolved cutscene ID
    uint16_t m_field_1E;            // +0x1E
    
    // Virtual methods
    virtual ~gdCutSceneData() = default;
    virtual void vfn_20();  // @ 0x8240E8F0
    virtual void vfn_21();  // @ 0x8240E9A8
    virtual const char* GetTypeName();  // @ 0x82347030
    
    // Methods
    void PostLoadProperties();  // @ 0x8240EA10 - Resolve name to ID
};

// ============================================================================
// Action Classes
// ============================================================================

/**
 * gdCSActionLoopData @ vtable 0x82077B14
 * 
 * Loop action for cutscene scripts.
 * Repeats a sequence of actions.
 */
struct gdCSActionLoopData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionLoopData() = default;
    virtual void vfn_20();  // @ 0x8240EB28 - Property validation
    virtual void vfn_21();  // @ 0x8240EB80
    virtual const char* GetTypeName();  // @ 0x82347208 - Returns "Loop"
};

/**
 * gdCSActionIfData @ vtable 0x82077B7C | size: ~0x30 (48 bytes)
 * 
 * Conditional action for cutscene scripts.
 * Executes actions based on game state conditions.
 * 
 * Supported condition types:
 * - PLAYER_SCORE_EQUAL (0)
 * - PLAYER_SCORE_GREATER (1)
 * - PLAYER_SCORE_LESS (2)
 * - OPPONENT_SCORE_EQUAL (3)
 * - OPPONENT_SCORE_GREATER (4)
 * - OPPONENT_SCORE_LESS (5)
 * - RANDOM_PERCENT (6)
 */
struct gdCSActionIfData {
    void* vtable;                       // +0x00
    uint32_t m_field_04;                // +0x04
    uint32_t m_field_08;                // +0x08
    uint32_t m_field_0C;                // +0x0C
    uint32_t m_field_10;                // +0x10
    const char* m_pConditionType;       // +0x14 - Condition type string
    uint32_t m_field_18;                // +0x18
    uint32_t m_field_1C;                // +0x1C
    void* m_pThenActions;               // +0x20 - Actions to execute if true
    void* m_pElseActions;               // +0x24 - Actions to execute if false
    uint32_t m_conditionEnum;           // +0x28 - Parsed condition type (0-6, 7=invalid)
    uint32_t m_conditionParam;          // +0x2C - Condition parameter (score value, etc.)
    
    // Virtual methods
    virtual ~gdCSActionIfData();  // @ 0x8240EC40 - Frees action lists
    virtual void vfn_20();  // @ 0x8240EBE8
    virtual void vfn_21();  // @ 0x8240ECB0
    virtual const char* GetTypeName();  // @ 0x823472A8 - Returns "If"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240ED80 - Parse condition and validate
};

/**
 * gdCSActionWaitData @ vtable 0x82077BE4 | size: ~0x18 (24 bytes)
 * 
 * Wait/delay action for cutscene scripts.
 * 
 * XML Properties:
 * - WaitType: Type of wait condition
 * - Duration: Wait duration in seconds
 */
struct gdCSActionWaitData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    const char* m_pWaitType;        // +0x10 - Wait type string
    float m_duration;               // +0x14 - Wait duration
    
    // Virtual methods
    virtual ~gdCSActionWaitData() = default;
    virtual void vfn_20();  // @ 0x8240F0F8
    virtual void vfn_21();  // @ 0x8240F150 - Register XML properties
    virtual const char* GetTypeName();  // @ 0x823473F8 - Returns "Wait"
};

/**
 * gdCSActionCamAnimData @ vtable 0x82077C4C | size: ~0x18 (24 bytes)
 * 
 * Camera animation action for cutscenes.
 */
struct gdCSActionCamAnimData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    uint32_t m_field_10;            // +0x10
    const char* m_pCameraName;      // +0x14 - Camera animation name
    
    // Virtual methods
    virtual ~gdCSActionCamAnimData() = default;
    virtual void vfn_20();  // @ 0x8240F1B8
    virtual void vfn_21();  // @ 0x8240F210
    virtual const char* GetTypeName();  // @ 0x82347498 - Returns "CamAnim"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F2A0 - Validate camera name
};

/**
 * gdCSActionCharAnimData @ vtable 0x82077CB4 | size: ~0x1C (28 bytes)
 * 
 * Character animation action for cutscenes.
 */
struct gdCSActionCharAnimData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    uint32_t m_field_10;            // +0x10
    int32_t m_characterId;          // +0x14 - Character ID (-1 = invalid)
    const char* m_pAnimName;        // +0x18 - Animation name
    
    // Virtual methods
    virtual ~gdCSActionCharAnimData() = default;
    virtual void vfn_20();  // @ 0x8240F310
    virtual void vfn_21();  // @ 0x8240F368
    virtual const char* GetTypeName();  // @ 0x82347538 - Returns "CharAnim"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F450 - Validate character ID and anim name
};

/**
 * gdCSActionCharVisibleData @ vtable 0x82077D1C | size: ~0x18 (24 bytes)
 * 
 * Character visibility toggle action for cutscenes.
 */
struct gdCSActionCharVisibleData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    uint32_t m_field_10;            // +0x10
    int32_t m_characterId;          // +0x14 - Character ID (-1 = invalid)
    
    // Virtual methods
    virtual ~gdCSActionCharVisibleData() = default;
    virtual void vfn_20();  // @ 0x8240F4D8
    virtual void vfn_21();  // @ 0x8240F530
    virtual const char* GetTypeName();  // @ 0x823475E8 - Returns "CharVisible"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F5C0 - Validate character ID
};

/**
 * gdCSActionPlayAudioData @ vtable 0x82077D84 | size: ~0x24 (36 bytes)
 * 
 * Audio playback action for cutscenes.
 * 
 * Audio types:
 * - "MUSIC" (0)
 * - "SOUND" (1)
 * 
 * Special audio names:
 * - "NONE" (ID: 7)
 */
struct gdCSActionPlayAudioData {
    void* vtable;                   // +0x00
    uint32_t m_field_04;            // +0x04
    uint32_t m_field_08;            // +0x08
    uint32_t m_field_0C;            // +0x0C
    uint32_t m_field_10;            // +0x10
    const char* m_pAudioType;       // +0x14 - "MUSIC" or "SOUND"
    const char* m_pAudioName;       // +0x18 - Audio asset name
    uint32_t m_audioTypeEnum;       // +0x1C - 0=MUSIC, 1=SOUND
    uint32_t m_audioId;             // +0x20 - Resolved audio ID (7="NONE")
    
    // Virtual methods
    virtual ~gdCSActionPlayAudioData() = default;
    virtual void vfn_20();  // @ 0x8240F608
    virtual void vfn_21();  // @ 0x8240F660
    virtual const char* GetTypeName();  // @ 0x82347688 - Returns "PlayAudio"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F6F0 - Parse audio type and validate
};

/**
 * gdCSActionLvlAmbAnimData @ vtable 0x82077DEC
 * 
 * Level ambient animation action for cutscenes.
 */
struct gdCSActionLvlAmbAnimData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionLvlAmbAnimData() = default;
    virtual void vfn_20();  // @ 0x8240F948
    virtual void vfn_21();  // @ 0x8240FA08
    virtual const char* GetTypeName();  // @ 0x823477C8 - Returns "LvlAmbAnim"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240FAA0
};

/**
 * gdCSActionCharAmbAnimData @ vtable 0x82077E54
 * 
 * Character ambient animation action for cutscenes.
 */
struct gdCSActionCharAmbAnimData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionCharAmbAnimData() = default;
    virtual void vfn_20();  // @ 0x8240FB50
    virtual void vfn_21();  // @ 0x8240FC10
    virtual const char* GetTypeName();  // @ 0x82347868 - Returns "CharAmbAnim"
    
    // Methods
    void PostLoadProperties();  // @ 0x8240FCC8
};

/**
 * gdCSActionShowAllAmbientsData @ vtable 0x82077EBC
 * 
 * Show all ambient objects action for cutscenes.
 */
struct gdCSActionShowAllAmbientsData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionShowAllAmbientsData() = default;
    virtual void vfn_20();  // @ 0x8240FD90
    virtual void vfn_21();  // @ 0x8240FDE8
    virtual const char* GetTypeName();  // @ 0x82347910 - Returns "ShowAllAmbients"
};
