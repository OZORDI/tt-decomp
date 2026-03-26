/**
 * globals_linker.cpp — Linker symbol definitions for 207 undefined globals
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Every global listed in this file was reported as an undefined symbol at
 * link time.  Types are taken verbatim from the corresponding extern
 * declarations spread across the codebase.  Mutable (.data) symbols get a
 * zero/null initializer; read-only (.rdata) constants use appropriate
 * zero values.
 */

#include <cstdint>
#include <cstddef>

// ---------------------------------------------------------------------------
// Forward struct used by g_swipePatterns (defined in pong_render.cpp as local)
// ---------------------------------------------------------------------------
struct SwipePattern {
    float    duration;
    float    timingParam;
    float    startCorner1[4];
    float    startCorner2[4];
    float    endCorner1[4];
    float    endCorner2[4];
    uint8_t  reverseFlag;
    uint8_t  pauseFlag;
    uint8_t  _pad[14];
};

// ===========================================================================
// AI type IDs
// ===========================================================================
uint32_t* g_aiTypeId_1 = nullptr;      // @ 0x825C60B4
uint32_t* g_aiTypeId_2 = nullptr;      // @ 0x825C803C
uint32_t* g_aiTypeId_3 = nullptr;      // @ 0x825C8038

// ===========================================================================
// Animation
// ===========================================================================
extern const float g_animBlendWeight = 0.0f;   // @ 0x8202D110

// ===========================================================================
// Ball / swing attribute hashes
// ===========================================================================
uint32_t g_attrHash_HitZone    = 0;     // @ 0x825C761C
uint32_t g_attrHash_SwingType1 = 0;     // @ 0x825C8038
uint32_t g_attrHash_SwingType2 = 0;     // @ 0x825C803C
extern const float g_alphaMultiplier  = 0.0f;  // @ 0x82079BE0

// ===========================================================================
// CRT atexit guards
// ===========================================================================
int32_t g_atexit_guard_0 = 0;           // @ 0x82733048
int32_t g_atexit_guard_1 = 0;           // @ 0x8273304C

// ===========================================================================
// Bink video player
// ===========================================================================
void* g_binkVideoPlayer = nullptr;      // @ 0x825EB988

// ===========================================================================
// Camera system
// ===========================================================================
float    g_cameraAdjustment  = 0.0f;    // @ 0x8202CFF4
uint8_t  g_cameraFlagA       = 0;       // SDA+25805
uint8_t  g_cameraFlagB       = 0;       // SDA+25806
uint8_t  g_cameraFlag_7532   = 0;       // SDA
uint8_t  g_cameraFlag_8897   = 0;       // SDA
uint8_t  g_cameraFlag_8898   = 0;       // SDA
uint32_t g_cameraFlags       = 0;       // @ 0x826064D0
uint32_t g_cameraMode        = 0;       // @ 0x825C5EB8
uint32_t g_cameraProperty1   = 0;       // @ 0x825C8A40
uint32_t g_cameraProperty2   = 0;       // @ 0x825C93D0
uint8_t  g_cameraSelector    = 0;       // SDA+25804
int32_t  g_cameraState       = 0;
float    g_cameraThreshold   = 0.0f;

// ===========================================================================
// Network debug flag
// ===========================================================================
uint8_t g_bNetworkDebugFlag = 0;        // @ 0x826065EB

// ===========================================================================
// Character view system (char_view_globals.cpp — not in build)
// ===========================================================================
void*    g_charViewData_ptr       = nullptr;  // @ 0x8271A2F0
uint32_t g_charViewSelectCounter  = 0;        // @ 0x82606490
uint32_t g_charViewSelectFlag     = 0;        // @ 0x82606494
uint32_t g_charViewTypeId         = 0;        // @ 0x825C2BC4
uint32_t g_character_type_id      = 0;        // @ 0x825C2BC0
uint32_t g_character_type_id_2    = 0;        // @ 0x825C803C
uint32_t g_character_type_id_3    = 0;        // @ 0x825C8038

// ===========================================================================
// Network: cmRefreshable vtable
// ===========================================================================
extern const void* g_cmRefreshableCtorVtable = nullptr; // @ 0x820533CC

// ===========================================================================
// Player contact zone constants
// ===========================================================================
extern const float g_contactZoneMinA = 0.0f;
extern const float g_contactZoneMaxA = 0.0f;
extern const float g_contactZoneMinB = 0.0f;
extern const float g_contactZoneMaxB = 0.0f;

// ===========================================================================
// Creature manager
// ===========================================================================
void* g_creatureManager = nullptr;      // @ 0x826063B8

// ===========================================================================
// Render state
// ===========================================================================
void* g_currentRenderState = nullptr;   // @ 0x821EBAA0

// ===========================================================================
// Debug mode
// ===========================================================================
uint8_t g_debugMode = 0;               // @ 0x825C64AD

// ===========================================================================
// Default constants
// ===========================================================================
extern const float g_defaultDuration            = 0.0f;  // @ 0x825C755C
float       g_defaultVibrationIntensity  = 0.0f;  // @ 0x8202D110
extern const float g_default_float_zero         = 0.0f;  // @ 0x825CAF94
extern const float g_default_float_value        = 0.0f;  // @ 0x825CAF88
extern const float g_default_float_value_2      = 0.0f;  // @ 0x825CAF90

// ===========================================================================
// Draw bucket
// ===========================================================================
void* g_drawBucket = nullptr;           // @ 0x826064F4

// ===========================================================================
// Error / debug strings (char_view_globals.cpp — not in build)
// ===========================================================================
extern const char* g_error_attract_exit      = "";
extern const char* g_error_charview_enter    = "";
extern const char* g_error_charview_exit     = "";
extern const char* g_error_type_mismatch     = "";
extern const char* g_error_unhandled_event   = "";

// ===========================================================================
// Event type table
// ===========================================================================
void* g_eventTypeTable = nullptr;       // @ 0x82017888

// ===========================================================================
// Float constants (network / physics)
// ===========================================================================
extern const float g_floatConstant1 = 0.0f;    // @ 0x82079AD4
extern const float g_floatConstant2 = 0.0f;    // @ 0x8202D110
extern const float g_floatConstant3 = 0.0f;    // @ 0x8202D110
extern const float g_floatConstant4 = 0.0f;    // @ 0x825C7600
extern const float g_floatEpsilon   = 0.0f;    // @ 0x8202D110
extern const float g_floatNegOne    = 0.0f;    // @ 0x8202D110
extern const float g_floatOne       = 0.0f;    // @ 0x8202D110
extern const float g_floatZero      = 0.0f;    // @ 0x8202D110

// ===========================================================================
// FloatAverager vtables (network classes)
// ===========================================================================
void* g_FloatAverager_vtable_1 = nullptr; // @ 0x8203A910
void* g_FloatAverager_vtable_2 = nullptr; // @ 0x8203A91C
void* g_FloatAverager_vtable_3 = nullptr; // @ 0x82070D78
void* g_FloatAverager_vtable_4 = nullptr; // @ 0x8207166C

// ===========================================================================
// Physics: fragDrawable vtable
// ===========================================================================
void* g_fragDrawableVtable = nullptr;   // @ 0x82033094

// ===========================================================================
// Frame sync object
// ===========================================================================
void* g_frameSyncObj = nullptr;         // @ 0x825F64F4

// ===========================================================================
// Game data manager
// ===========================================================================
void* g_gameDataMgr       = nullptr;    // @ 0x8271A2E4
void* g_game_data_manager  = nullptr;   // @ 0x825CA174
void* g_game_state_ptr     = nullptr;   // @ 0x8271A2F8

// ===========================================================================
// Game data type IDs
// ===========================================================================
uint32_t g_gdEventBase_typeId    = 0;   // @ 0x825C5800
uint32_t g_gdRivalryData_typeId  = 0;   // @ 0x825C5F68
uint32_t g_gdRivalry_typeId      = 0;   // @ 0x825C5F6C
uint32_t g_gdTierMember_typeId   = 0;   // @ 0x825C5F70
uint32_t g_gdTier_typeId         = 0;   // @ 0x825C5F80
uint32_t g_gdVibEvent_typeId     = 0;   // @ 0x825C5DF4

// ===========================================================================
// Global streamer
// ===========================================================================
void* g_globalStreamer = nullptr;        // @ 0x82606600

// ===========================================================================
// UI state (char_view_globals.cpp — not in build)
// ===========================================================================
uint32_t* g_global_ui_state = nullptr;  // @ 0x8271A348

// ===========================================================================
// HUD
// ===========================================================================
void* g_hudFlashBase   = nullptr;       // @ 0x82606454
float g_hudRenderParam = 0.0f;          // @ 0x82140110

// ===========================================================================
// Player adjustment / input / timing constants
// ===========================================================================
float       g_kAdjustmentScale     = 0.0f;  // @ 0x82079BAC
extern const float g_kAdjustmentThreshold = 0.0f;  // @ 0x8202D110
extern const float g_kInputScale          = 0.0f;  // @ 0x825C5938
extern const float g_kInputThresholdHigh  = 0.0f;  // @ 0x8202D110
extern const float g_kMaxAdjustment       = 0.0f;  // @ 0x82079CD8
extern const float g_kQuantScale1         = 0.0f;  // @ 0x82079FFC
extern const float g_kQuantScale2         = 0.0f;  // @ 0x82079FF8
extern const float g_kSpecialThreshold    = 0.0f;  // @ 0x825C8A50
float       g_kTimingConstant      = 0.0f;  // @ 0x825C4930
extern const float g_kZeroThreshold       = 0.0f;  // @ 0x8202D108

// ===========================================================================
// Local user pointer
// ===========================================================================
void* g_localUserPtr = nullptr;

// ===========================================================================
// Locomotion / creature animation
// ===========================================================================
void*       g_locomotionAnimInstance = nullptr;  // @ 0x8271A378
extern const float g_lookAtMultiplier       = 0.0f;     // @ 0x8202D184
extern const float g_lookAtRefAngle         = 0.0f;     // @ 0x8202D10C

// ===========================================================================
// Match result / player select tables
// ===========================================================================
void* g_matchResultTable  = nullptr;    // @ 0x82027884

// ===========================================================================
// Matrix / resource field types
// ===========================================================================
void* g_matrixFieldType   = nullptr;    // @ 0x825CAFA4

// ===========================================================================
// Message sink
// ===========================================================================
uint32_t      g_msgSinkBitfield      = 0;        // @ 0x825B0C30
extern const float   g_msgSinkInitFloat     = 0.0f;     // @ 0x825C5938
extern const uint32_t g_msgSinkTemplate[4]  = {0};      // @ 0x8241A0C0

// ===========================================================================
// Network globals
// ===========================================================================
uint32_t  g_netMessageHolderCount = 0;      // @ 0x826066A4
int32_t   g_networkMode           = 0;
void*     g_networkStatePtr       = nullptr; // @ 0x8271A36C
uint32_t  g_networkTimestamp      = 0;
void*     g_networkTimingObject   = nullptr; // @ 0x8271A328

// ===========================================================================
// Network pointer globals
// ===========================================================================
void*     g_pAtSingleton              = nullptr;  // @ 0x821EAB08
void*     g_pAtSingletonPtr           = nullptr;  // @ 0x8271A328
void*     g_pCleanupVtable            = nullptr;  // @ 0x8206C394
uint8_t*  g_pDataRequestMsgPoolBase   = nullptr;  // @ 0x825D129C
void*     g_pDataRequestMsgSingleton  = nullptr;  // @ 0x825D1290
uint8_t*  g_pDataSendMsgPoolBase      = nullptr;  // @ 0x825D12B4
void*     g_pDataSendMsgSingleton     = nullptr;  // @ 0x825D12A8
void*     g_pGameStatePtr             = nullptr;  // @ 0x8271A328
void**    g_pInputArrayTable          = nullptr;  // @ 0x825EAB28
void**    g_pMessageQueuePool         = nullptr;  // @ 0x825D128C
void*     g_pNetworkBase              = nullptr;  // @ 0x8271A7B0
void*     g_pNetworkState             = nullptr;  // @ 0x8271A7B0
uint32_t* g_pNetworkStatePtr          = nullptr;  // @ 0x8271A7B0
void*     g_pNetworkTimer             = nullptr;  // @ 0x8201A328
void*     g_pPlayerArrayBase          = nullptr;  // @ 0x8211A31C
uint8_t*  g_pPlayerInputTable         = nullptr;
uint8_t*  g_pTournamentMsgPool        = nullptr;  // @ 0x825D1464
void*     g_pTournamentMsgSingleton   = nullptr;  // @ 0x825D1458

// ===========================================================================
// Pause flag
// ===========================================================================
uint8_t g_pauseFlag = 0;               // @ 0x82606670

// ===========================================================================
// Physics callbacks
// ===========================================================================
void* g_phCallback0 = nullptr;          // @ 0x825EA900
void* g_phCallback1 = nullptr;          // @ 0x825EA904
void* g_phCallback2 = nullptr;          // @ 0x825EA908
void* g_phCallback3 = nullptr;          // @ 0x825EA90C
void* g_phCallback4 = nullptr;          // @ 0x825EA910
void* g_phCallback5 = nullptr;          // @ 0x825EA914

// ===========================================================================
// Physics system
// ===========================================================================
void** g_physicsStateArray = nullptr;
void*  g_physicsSystem     = nullptr;   // @ 0x82606568

// ===========================================================================
// Player movement message pool
// ===========================================================================
uint8_t* g_playerMovementMsgPool          = nullptr; // @ 0x825D1164
void*    g_playerMovementMsgPoolSingleton  = nullptr; // @ 0x825D1158

// ===========================================================================
// Player property types
// ===========================================================================
uint32_t g_playerPropType1 = 0;         // @ 0x82062D7C
uint32_t g_playerPropType2 = 0;         // @ 0x820693D0

// ===========================================================================
// Player select / slot state
// ===========================================================================
void*    g_playerSelectTable     = nullptr;  // @ 0x82027888
int32_t* g_playerSlotIndices     = nullptr;
int32_t* g_playerSlotStates      = nullptr;
void*    g_player_controller_ptr = nullptr;  // @ 0x825C6564
void*    g_player_manager_ptr    = nullptr;  // @ 0x8271A2D0

// ===========================================================================
// Random state
// ===========================================================================
uint64_t g_randomState = 0;             // @ 0x825DA268

// ===========================================================================
// Recovery constants
// ===========================================================================
extern const float g_recoveryScale     = 0.0f;
float       g_recoveryThreshold = 0.0f;

// ===========================================================================
// Render system
// ===========================================================================
void* g_renderContext        = nullptr;  // @ 0x826063D4
int   g_renderModeIndex      = 0;        // @ 0x821F0898
void* g_renderStateStack[16] = {};       // @ 0x821EBA9C
void* g_rendererState        = nullptr;  // @ 0x8260641C

// ===========================================================================
// Resource / string field types
// ===========================================================================
void* g_resourceFieldType = nullptr;    // @ 0x825CAF88
void* g_stringFieldType   = nullptr;    // @ 0x825CAF88

// ===========================================================================
// Scene render objects (char_view_globals.cpp — not in build)
// ===========================================================================
void* g_sceneRenderObj  = nullptr;      // @ 0x82606430
void* g_sceneRenderObj2 = nullptr;      // @ 0x8260641C

// ===========================================================================
// Scene reset matrix
// ===========================================================================
uint32_t g_sceneResetMatrix[4] = {0};   // @ 0x8261A0C0

// ===========================================================================
// Selected character index
// ===========================================================================
int32_t g_selectedCharacterIndex = -1;  // @ 0x825C76B8

// ===========================================================================
// Scene graph type IDs
// ===========================================================================
uint32_t g_sgNode_typeId       = 0;     // @ 0x825C93D0
uint32_t g_sgRMDrawable_typeId = 0;     // @ 0x825C2D78

// ===========================================================================
// Slow motion manager
// ===========================================================================
void* g_slowmo_manager_ptr = nullptr;   // @ 0x825C6540

// ===========================================================================
// Misc render objects
// ===========================================================================
void* g_someRenderObj1 = nullptr;       // @ 0x82606654
void* g_someRenderObj2 = nullptr;       // @ 0x825FEAB0

// ===========================================================================
// State name strings (char_view_globals.cpp — not in build)
// ===========================================================================
extern const char* g_stateName_attract  = "";  // @ 0x8205E094
extern const char* g_stateName_charView = "";  // @ 0x8205EF5C

// ===========================================================================
// Game data error strings (gd_data_strings.cpp — not in build)
// ===========================================================================
extern const char* g_str_gdLadder_duplicateLevel       = "";  // @ 0x82044D30
extern const char* g_str_gdLadder_missingLadder        = "";  // @ 0x82044DA8
extern const char* g_str_gdLadder_unknownNodeType      = "";  // @ 0x82044D70
extern const char* g_str_gdRivalryData_noCharName      = "";  // @ 0x82044598
extern const char* g_str_gdRivalryData_noRivalName     = "";  // @ 0x820445E0
extern const char* g_str_gdRivalryData_unknownRival    = "";  // @ 0x82044620
extern const char* g_str_gdRivalry_duplicateChar       = "";  // @ 0x820446A8
extern const char* g_str_gdRivalry_missingRival        = "";  // @ 0x82044728
extern const char* g_str_gdRivalry_unknownChar         = "";  // @ 0x82044670
extern const char* g_str_gdRivalry_unknownNodeType     = "";  // @ 0x820446F0
extern const char* g_str_gdTierMember_noCharName       = "";  // @ 0x82044778
extern const char* g_str_gdTierMember_unknownChar      = "";  // @ 0x820447BC
extern const char* g_str_gdTier_noTierName             = "";  // @ 0x8204480C
extern const char* g_str_gdTier_unknownNodeType        = "";  // @ 0x82044844

// ===========================================================================
// Pong strings (pong_strings.cpp — not in build)
// ===========================================================================
extern const char* g_str_pongDrill_tooManySuccesses = "";  // @ 0x82033970
extern const char* g_str_pongMover_calcInitMatrix   = "";  // @ 0x8202A088

// ===========================================================================
// Swipe patterns array
// ===========================================================================
SwipePattern g_swipePatterns[17] = {};   // @ 0x825D09A0

// ===========================================================================
// Network RTTI / type name strings (char arrays)
// ===========================================================================
extern const char g_szCreateMachineRTTI[]          = "";  // @ 0x825DEEB0
extern const char g_szDataRequestTypeName[]        = "";  // @ 0x8206EA74
extern const char g_szDataSendTypeName[]           = "";  // @ 0x8206EA88
extern const char g_szDefaultError[]               = "";  // @ 0x82027423
extern const char g_szInvalidError[]               = "";  // @ 0x8204EF04
extern const char g_szMessageType[]                = "";  // @ 0x8205DFB0
extern const char g_szNetworkDebugMsg[]            = "";  // @ 0x8205AE98
extern const char g_szRageRTTI[]                   = "";  // @ 0x825DEECC
extern const char g_szTopSpin[]                    = "";  // @ 0x8206CB54
extern const char g_szTournamentCompleteTypeName[] = "";  // @ 0x8206EC40

// ===========================================================================
// Thread-local storage base (char_view_globals.cpp — not in build)
// ===========================================================================
void** g_tls_base = nullptr;            // @ 0x82600000

// ===========================================================================
// UI list manager
// ===========================================================================
void* g_uiListManager = nullptr;        // @ 0x825D16AC

// ===========================================================================
// UI context / objects (char_view_globals.cpp — not in build)
// ===========================================================================
void*    g_ui_context         = nullptr;      // @ 0x8271A81C
uint8_t  g_ui_object_flags[64]  = {0};        // @ 0x825F3A68
void*    g_ui_object_ptrs[256]  = {nullptr};  // @ 0x825F3968
uint32_t g_ui_object_types[256] = {0};        // @ 0x825F3868
uint32_t g_ui_state_counter     = 0;          // @ 0x825F3864

// ===========================================================================
// Vibration system
// ===========================================================================
void*    g_vibrationMgr        = nullptr;  // @ 0x8271A3A8
uint8_t* g_vibrationStateArray = nullptr;  // @ 0x8271A364

// ===========================================================================
// Vtable pointers (char_view_globals.cpp — not in build)
// ===========================================================================
void* g_vtable_char_view_cs              = nullptr;  // @ 0x8203F454
void* g_vtable_pong_attract_context      = nullptr;  // @ 0x8205E0F4
void* g_vtable_pong_attract_context_2    = nullptr;  // @ 0x8205E108
void* g_vtable_pong_char_view_context    = nullptr;  // @ 0x8205F1BC
void* g_vtable_pong_char_view_context_2  = nullptr;  // @ 0x8205F1D4
void* g_vtable_pong_char_view_context_3  = nullptr;  // @ 0x8205F7B4
void* g_vtable_pong_char_view_state      = nullptr;  // @ 0x8205F174

// ===========================================================================
// XEX exec info (startup.c)
// ===========================================================================
void* g_xex_exec_info = nullptr;         // @ 0x825E6E64

// ===========================================================================
// XML node struct type IDs
// ===========================================================================
uint32_t g_xmlNodeStruct_typeId  = 0;    // @ 0x825C803C
uint32_t g_xmlNodeStruct_typeId2 = 0;    // @ 0x825C8038

// ===========================================================================
// Zero / constant floats
// ===========================================================================
extern const float g_zeroConstant = 0.0f;       // @ 0x8202D110
extern const float g_zeroFloat    = 0.0f;       // @ 0x82079AD8

// ── Additional float constants ──────────────────────────────────────────────

// ── String constants ────────────────────────────────────────────────────────

// ── Template/vtable globals ─────────────────────────────────────────────────

// ── dcamPolarCam vtable stubs (use __asm__ to match asm-label references) ────
void dcamPolarCam_vfn_6_impl() __asm__("dcamPolarCam_vfn_6");
void dcamPolarCam_vfn_6_impl() {}
void dcamPolarCam_vfn_7_impl() __asm__("dcamPolarCam_vfn_7");
void dcamPolarCam_vfn_7_impl() {}
void dcamPolarCam_vfn_8_impl() __asm__("dcamPolarCam_vfn_8");
void dcamPolarCam_vfn_8_impl() {}
void dcamPolarCam_vfn_9_impl() __asm__("dcamPolarCam_vfn_9");
void dcamPolarCam_vfn_9_impl() {}
