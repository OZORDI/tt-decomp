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
 * gdCSCharAnimData @ vtable 0x82076D4C
 * 
 * Character animation data for cutscenes.
 * Contains animation name, timing, and playback parameters.
 */
struct gdCSCharAnimData {
    void* vtable;  // +0x00
    uint32_t m_field_04;
    uint32_t m_field_08;
    uint32_t m_field_0C;
    const char* m_pAnimName;  // +0x10 - Animation name string
    uint32_t m_field_14;
    uint32_t m_field_18;
    uint32_t m_field_1C;
    uint32_t m_field_20;
    uint32_t m_field_24;
    uint32_t m_field_28;
    uint32_t m_field_2C;
    uint32_t m_field_30;
    uint32_t m_field_34;
    uint32_t m_field_38;
    uint32_t m_field_3C;
    float m_duration;  // +0x40 - Animation duration
    int32_t m_field_44;  // +0x44
    float m_startTime;  // +0x48 - Start time
    float m_endTime;  // +0x4C - End time
    
    // Virtual methods
    virtual ~gdCSCharAnimData() = default;
    virtual void vfn_20();  // @ 0x8240C990
    virtual void vfn_21();  // @ 0x8240CBD0
    virtual const char* GetTypeName();  // @ 0x8240C9D8
    virtual void vfn_3();  // @ 0x8240CDE8
    
    // Methods
    void PostLoadProperties();  // @ 0x8240CD00
};

/**
 * gdCSCharAnimNames @ vtable 0x82076DB4
 * 
 * Collection of character animation names with random selection support.
 */
struct gdCSCharAnimNames {
    void* vtable;  // +0x00
    uint32_t m_field_04;
    uint32_t m_field_08;
    uint32_t m_field_0C;
    gdCSCharAnimData** m_pAnimDataArray;  // +0x10 - Array of animation data pointers
    uint16_t m_animCount;  // +0x14 - Number of animations
    uint16_t m_field_16;
    uint32_t* m_pSelectionFlags;  // +0x18 - Flags for which anims are selected
    uint16_t m_field_1C;
    uint16_t m_field_1E;
    uint32_t* m_pNameMatchFlags;  // +0x20 - Flags for name matching
    uint16_t m_field_24;
    uint16_t m_field_26;
    
    // Virtual methods
    virtual ~gdCSCharAnimNames() = default;
    virtual void vfn_20();  // @ 0x8240D130
    virtual void vfn_21();
    virtual const char* GetTypeName();  // @ 0x8240D178
    virtual void vfn_3();  // @ 0x8240D340
    
    // Methods
    void FindRandAnimData(uint32_t param1, uint32_t param2, uint32_t param3);  // @ 0x8240D4A8
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
 * Stores cutscene name and references to cutscene assets.
 */
struct gdCutSceneData {
    void* vtable;  // +0x00
    uint32_t m_field_04;
    uint32_t m_field_08;
    uint32_t m_field_0C;
    uint32_t m_field_10;
    const char* m_pCutsceneName;  // +0x14 - Cutscene name string
    uint32_t m_field_18;
    uint16_t m_cutsceneId;  // +0x1C - Cutscene ID
    uint16_t m_field_1E;
    
    // Virtual methods
    virtual ~gdCutSceneData() = default;
    virtual void vfn_20();  // @ 0x8240E8F0
    virtual void vfn_21();  // @ 0x8240E9A8
    virtual const char* GetTypeName();  // @ 0x82347030
    
    // Methods
    void PostLoadProperties();  // @ 0x8240EA10
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
 * gdCSActionIfData @ vtable 0x82077B7C
 * 
 * Conditional action for cutscene scripts.
 * Executes actions based on conditions.
 */
struct gdCSActionIfData {
    void* vtable;  // +0x00
    uint32_t m_field_04;
    uint32_t m_field_08;
    uint32_t m_field_0C;
    uint32_t m_field_10;
    const char* m_pConditionType;  // +0x14 - Condition type string
    uint32_t m_field_18;
    uint32_t m_field_1C;
    void* m_pThenActions;  // +0x20 - Actions to execute if true
    void* m_pElseActions;  // +0x24 - Actions to execute if false
    uint32_t m_conditionEnum;  // +0x28 - Parsed condition type enum
    uint32_t m_conditionParam;  // +0x2C - Condition parameter
    
    // Virtual methods
    virtual ~gdCSActionIfData();  // @ 0x8240EC40 - Frees action lists
    virtual void vfn_20();  // @ 0x8240EBE8
    virtual void vfn_21();  // @ 0x8240ECB0
    virtual const char* GetTypeName();  // @ 0x823472A8
    
    // Methods
    void PostLoadProperties();  // @ 0x8240ED80
};

/**
 * gdCSActionWaitData @ vtable 0x82077BE4
 * 
 * Wait/delay action for cutscene scripts.
 */
struct gdCSActionWaitData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionWaitData() = default;
    virtual void vfn_20();  // @ 0x8240F0F8
    virtual void vfn_21();  // @ 0x8240F150
    virtual const char* GetTypeName();  // @ 0x823473F8
};

/**
 * gdCSActionCamAnimData @ vtable 0x82077C4C
 * 
 * Camera animation action for cutscenes.
 */
struct gdCSActionCamAnimData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionCamAnimData() = default;
    virtual void vfn_20();  // @ 0x8240F1B8
    virtual void vfn_21();  // @ 0x8240F210
    virtual const char* GetTypeName();  // @ 0x82347498
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F2A0
};

/**
 * gdCSActionCharAnimData @ vtable 0x82077CB4
 * 
 * Character animation action for cutscenes.
 */
struct gdCSActionCharAnimData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionCharAnimData() = default;
    virtual void vfn_20();  // @ 0x8240F310
    virtual void vfn_21();  // @ 0x8240F368
    virtual const char* GetTypeName();  // @ 0x82347538
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F450
};

/**
 * gdCSActionCharVisibleData @ vtable 0x82077D1C
 * 
 * Character visibility toggle action for cutscenes.
 */
struct gdCSActionCharVisibleData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionCharVisibleData() = default;
    virtual void vfn_20();  // @ 0x8240F4D8
    virtual void vfn_21();  // @ 0x8240F530
    virtual const char* GetTypeName();  // @ 0x823475E8
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F5C0
};

/**
 * gdCSActionPlayAudioData @ vtable 0x82077D84
 * 
 * Audio playback action for cutscenes.
 */
struct gdCSActionPlayAudioData {
    void* vtable;  // +0x00
    
    // Virtual methods
    virtual ~gdCSActionPlayAudioData() = default;
    virtual void vfn_20();  // @ 0x8240F608
    virtual void vfn_21();  // @ 0x8240F660
    virtual const char* GetTypeName();  // @ 0x82347688
    
    // Methods
    void PostLoadProperties();  // @ 0x8240F6F0
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
    virtual const char* GetTypeName();  // @ 0x823477C8
    
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
    virtual const char* GetTypeName();  // @ 0x82347868
    
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
    virtual const char* GetTypeName();  // @ 0x82347910
};
