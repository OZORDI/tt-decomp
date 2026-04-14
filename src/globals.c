/**
 * globals.c — Global Variables Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All global variable definitions for the project, consolidated from
 * globals.c, globals_extended.c, and globals_linker.cpp.
 */

#include <stddef.h>
#include <stdint.h>
#include "globals.h"

// ============================================================================
// Named externs for top-referenced raw addresses
// Each corresponds to a generated lbl_XXXXXXXX object. Replace raw casts
// with these names; leave TODO for full typing where class is unknown.
// ============================================================================

/* .rdata (RTTI vtables) — typed as opaque bytes in C; real class decls
 * live in C++ headers. TODO: replace with proper forward-declared types
 * in a .hpp consumed via extern "C" once this file is converted. */
extern const uint8_t lbl_8203A910[12];   /* FloatAverager vtbl #1 */
extern const uint8_t lbl_8203A91C[12];   /* FloatAverager vtbl #2 */
extern const uint8_t lbl_82070D78[12];   /* FloatAverager vtbl #3 */
extern const uint8_t lbl_8207166C[12];   /* FloatAverager vtbl #4 */
extern const uint8_t lbl_820533CC[16];   /* cmRefreshableCtor vtbl */
extern const uint8_t lbl_82033094[72];   /* rage::fragDrawable vtbl */

/* .rdata scalar/float constants */
extern const float lbl_8202D110;  /* 4-byte float constant (heavily reused) */
extern const double lbl_8202CFF4; /* 8-byte .rdata constant */

/* .data SDA globals — untyped raw storage */
extern uint32_t lbl_825C803C;     /* 8 bytes .data — aiTypeId / attrHash / xmlNodeStruct_typeId alias */
extern uint32_t lbl_825C8038;     /* 4 bytes .data — aiTypeId / attrHash alias */
extern uint32_t lbl_825CAF88;     /* 4 bytes .data — resource/string field type slot */
extern uint32_t lbl_825C93D0;     /* 8 bytes .data — cameraProperty2 / sgNode_typeId alias */
extern uint8_t  lbl_8261A0C0[16]; /* default viewport rect / scene reset matrix */
extern uint8_t  lbl_82619BE0[16]; /* SIMD comparison mask */
extern float    lbl_825C5938[4];  /* 16 bytes .data — input scale / msgSink init */
extern uint8_t  lbl_825CB800[64]; /* identity matrix ref */
extern uint32_t lbl_8260641C;     /* SDA+25628 — renderer state ptr slot */
extern uint8_t  lbl_82607C40[65616]; /* SDA+31808 — TemplateRegistry backing */
extern uint8_t  lbl_826064CC;     /* SDA+25804 — UI input enable flag */
extern uint8_t  lbl_826063B8[8];  /* SDA+25528 — creature manager slot */

/* .data — UI/system singleton pointer slots */
extern void* lbl_8271A81C;  /* grcDevice / ui_context pointer slot */
extern void* lbl_8271A328;  /* network timing / atSingleton / gameState ptr alias */
extern void* lbl_8271A7B0;  /* network base/state ptr alias */
extern void* lbl_8271A358;  /* UI manager pointer slot */
extern void* lbl_8271A344;  /* rage::cmSampleCamActions ptr (RTTI name nearby) */
extern void* lbl_8271A2E4;  /* game data manager slot */
extern void* lbl_8271A2F0;  /* char view data slot */
extern void* lbl_8271A378;  /* locomotion anim instance slot */
extern void* lbl_8271A3A8;  /* vibration manager slot */

/* Misc — strings / inside-symbol offsets (cannot be typed cleanly; TODO) */
extern const uint32_t lbl_825D07CC;        /* "@cmSampleCamActions@rage@@" RTTI string; +4 = flags */
extern const char     lbl_825EB988[];       /* "leur des 3" localized string */
extern uint8_t        pongPlayer_BA20_g[]; /* 0x821EBAA0 = +0x80 into this symbol */
extern uint8_t        lbl_820176C0[];       /* 0x82017888 = +0x1C8 into this symbol */

// ============================================================================
// Graphics System
// ============================================================================

void* g_grcDevice = (void*)&lbl_8271A81C;  /* was 0x8271A81C */

/* grcDevice instances — the two device pointers used by the rendering pipeline */
// REMOVED: g_pGrcDevice_beginScene (in globals_extended.c)  /* full device for beginScene */
// REMOVED: g_pGrcDevice_clear (in globals_extended.c)  /* compact clear-queue device */

void* g_grcDeviceState = NULL;
void* g_grcDeviceVtable = NULL;
void* g_grcFlagsObj = NULL;
uint32_t g_grcInstanceIdSeed = 0;
uint8_t g_grcReadyFlag = 0;
void* g_grcRenderConfig = NULL;
void* g_grcSetup_vtable = NULL;
void* g_grcSubsystem1 = NULL;
void* g_grcSubsystem2 = NULL;
void* g_grcDefaultNameRecord = NULL;

void* g_pGrcDevice = NULL;
void* g_pGrcRenderer = NULL;
void* g_pGrcRenderer2 = NULL;
void* g_pGrcScene = NULL;
void* g_pGrcSceneB = NULL;
void* g_pSecondaryDevice = NULL;

void* g_displayModeBase = NULL;
uint32_t g_nScreenHeight = 720;
uint32_t g_nScreenWidth = 1280;
void* g_nonWsConfig = NULL;

/* Default viewport rectangle used to reset grcDeviceClear state */
const uint8_t g_defaultViewportRect[16] = { 0 };  /* @ 0x8261A0C0 */

/* Shader constant data table @ 0x8202C328 (.rdata, 440 bytes)
 * Used by grc_CB48 for GPU shader constant initialization */
const uint8_t g_shaderConstantData[440] = {
    /* This is a placeholder - actual data would need to be extracted from binary */
    0
};

// ============================================================================
// Render Loop Globals
// ============================================================================

void* g_render_obj_ptr = NULL;
uint64_t g_renderBeginTimestamp = 0;
uint32_t g_renderClearRingDivisor = 0;
uint32_t g_renderClearRingIdx = 0;
uint32_t g_renderClearRingSaved = 0;
void* g_renderCmdBufBase = NULL;
void* g_renderCmdRingBufPtr = NULL;
uint32_t g_renderCmdRingCounter = 0;
uint32_t g_renderCmdRingCounterSaved = 0;
float g_renderDeltaTime = 0.0f;
float g_renderFrameTime = 0.0f;
uint32_t g_renderStateFlags = 0;
uint64_t g_renderTimingA = 0;
uint64_t g_renderTimingB = 0;
void* g_postRenderConfig = NULL;
void* g_pRenderSetupObj = NULL;
uint32_t g_frameRenderCounter = 0;

// ============================================================================
// Display System
// ============================================================================

void* g_displayObject = NULL;
void* g_display_obj_ptr = NULL;

// ============================================================================
// UI System
// ============================================================================

void* g_uiManager = (void*)&lbl_8271A358;  /* was 0x8271A358 */
void* g_uiManagerPtr = NULL;
void* g_pHudOverlay = NULL;
void* g_pStencilMgrObj = NULL;

// UI input enable flag
// @ SDA+25804 (0x826064CC)
uint8_t g_uiInputFlag = 0;

// ============================================================================
// Game State
// ============================================================================

void* g_game_obj_ptr = NULL;
void* g_gameObject = (void*)&g_game_obj_ptr;  /* was 0x82606604 — resolved to g_game_obj_ptr */
void* g_loop_obj_ptr = NULL;
void* g_pGameLoop = NULL;
void* g_pRootGameObj = NULL;

void* g_pGameState = NULL;
uint8_t g_bAppStarted = 0;
uint8_t g_bInitComplete = 0;
uint8_t g_bRestartPending = 0;
uint32_t g_initStateFlag = 0;
uint32_t g_prngState = 0;
float g_fPrngNormalize = 1.0f / 0xFFFFFFFF;

// ============================================================================
// Audio System
// ============================================================================

void* g_audioObject = NULL;
void* g_audioConfig = NULL;
void* g_audio_obj_ptr = NULL;
void* g_audio_config_ptr = NULL;
uint32_t g_audio_unknown_25436 = 0;
uint32_t g_audio_unknown_25440 = 0;
void* g_audPathTable = NULL;

// ============================================================================
// Asset System
// ============================================================================

void* g_cachedAssetPath = NULL;

// ============================================================================
// Camera System
// ============================================================================

/* rage::cmSampleCamActions global pointer (gate in beginScene) */
void* g_pCamActionSys = NULL;           /* @ 0x8271A344, set at runtime */

/* Camera actions flags gate word (inside lbl_825D07CC + 4) */
uint32_t g_pCamActionsFlags = 0;        /* @ 0x825D07D0 */

void* g_pCamera = NULL;

// ============================================================================
// HSM (Hierarchical State Machine) Globals
// ============================================================================

void* g_hsmContextPtr = NULL;
void* g_hsmMgr = NULL;
void* g_hsmMgrPtr = NULL;
uint32_t g_hsmMgr_flags = 0;

// ============================================================================
// Input System Globals
// ============================================================================

void* g_input_obj_ptr = NULL;
void* g_pInputObj = NULL;
void* g_pInputScoreTable = NULL;
void* g_pButtonStateTable = NULL;
void* g_controllerArrayBase = NULL;

// ============================================================================
// Player System Globals
// ============================================================================

void* g_pPlayerSlotTable = NULL;
void* g_pPlayerDataTable = NULL;
void* g_pPlayerDataTable_slot = NULL;
void* g_pPlayerLerpTable = NULL;
void* g_plrPlayerMgr_state = NULL;

// ============================================================================
// Network System Globals
// ============================================================================

void* g_pNetSystem = NULL;
void* g_pNetMsgPoolBase = NULL;
void* g_pNetMsgSingleton = NULL;
void* g_singles_network_client = NULL;

// ============================================================================
// Physics System Globals
// ============================================================================

void* g_pPhysicsWorld = NULL;
uint32_t g_phFrameCounter = 0;
uint32_t g_phFrameIndex = 0;
void* g_matMgrGuard = NULL;

// ============================================================================
// Template Registry System
// ============================================================================

// @ 0x82607C40 (SDA r13+31808)
struct TemplateRegistry g_templateRegistry = {0};

// ============================================================================
// Matrix Comparison System
// ============================================================================

// Reference identity matrix @ 0x825CB800
const float g_identityMatrixRef[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

// SIMD comparison mask @ 0x82619BE0
// This mask is applied during matrix comparisons to ignore certain components
const uint32_t g_vectorComparisonMask[4] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

// ============================================================================
// CRT/Exit System Globals
// ============================================================================

uint8_t g_allocatorInitFlag = 0;
int g_argc = 0;
char** g_argv = NULL;
char g_argv_buf[1024] = {0};

void** g_atexit_table_cur = NULL;
void** g_atexit_table_end = NULL;
void** g_exit_end = NULL;
void** g_exit_start = NULL;
void** g_on_exit_end = NULL;
void** g_on_exit_start = NULL;

uint8_t g_doexit_called = 0;
uint32_t g_doexit_flag = 0;
uint32_t g_doexit_type = 0;

// ============================================================================
// Credits/Dialog System Globals
// ============================================================================

void* g_creditsRoll = NULL;
uint8_t g_creditsRollVisible = 0;
void* g_pCreditsRollObj = NULL;
uint32_t g_creditsAssetId_A = 0;
uint32_t g_creditsAssetId_B = 0;
uint32_t g_creditsAssetId_C = 0;
void* g_creditsColourSchema = NULL;
void* g_creditsContextData = NULL;
void* g_creditsEntriesFieldDesc = NULL;
void* g_creditsEntriesSchema = NULL;
void* g_creditsExitTable = NULL;
void* g_creditsNameFieldDesc = NULL;
void* g_creditsNameSchema = NULL;
void* g_creditsSettingsSchema = NULL;
void* g_creditsTransTable = NULL;

uint32_t g_dialogAssetId_A = 0;
uint32_t g_dialogAssetId_B = 0;
uint32_t g_dialogAssetId_C = 0;
void* g_dialogFieldSchema = NULL;
void* g_dialogPageGroup = NULL;
void* g_pDialogRollObj = NULL;

// ============================================================================
// Frontend System Globals
// ============================================================================

uint32_t g_frontendAssetId_A = 0;
uint32_t g_frontendAssetId_B = 0;
uint32_t g_frontendAssetId_C = 0;
void* g_frontendFieldSchema_1 = NULL;
void* g_frontendFieldSchema_2 = NULL;

// ============================================================================
// Settings System Globals
// ============================================================================

uint32_t g_settingsAssetId_A = 0;
uint32_t g_settingsAssetId_B = 0;
uint32_t g_settingsAssetId_C = 0;
void* g_gameSettings = NULL;

// ============================================================================
// Attract State Globals
// ============================================================================

uint32_t g_attractStateCount = 0;
void** g_attractStateCtxPtrs = NULL;
uint32_t* g_attractStateFlags = NULL;
uint32_t* g_attractStateSizes = NULL;

// ============================================================================
// Training/Drill System Globals
// ============================================================================

void* g_pTrainingData = NULL;
void* g_pTrainingStateMachine = NULL;
void* g_pDrillSaveData = NULL;
float g_fDrillScoreScale = 1.0f;

// Drill cleanup parameter @ 0x825C74FC (.data, 4 bytes)
// Used by pongDrillSoftShot::ScalarDestructor for cleanup validation
uint32_t g_drillCleanupParam = 0;

// Shot type validation globals for noSoftShotsTipData
// Used by PostLoadProperties @ 0x821F2678 to validate allowed shot types
uint32_t g_shotType1_825C5F50 = 0;  // @ 0x825C5F50 (.data, 4 bytes)
uint32_t g_shotType2_825C803C = 0;  // @ 0x825C803C (.data, 4 bytes)
uint32_t g_shotType3_825C8038 = 0;  // @ 0x825C8038 (.data, 4 bytes)

// ============================================================================
// Threading Globals
// ============================================================================

void* g_mainThreadHandle = NULL;
void* g_mainThreadXtf = NULL;
void* g_threadpool_head = NULL;
void* g_pgThreadInitGate = NULL;
void* g_semIO = NULL;
void* g_semRender = NULL;
void* g_semUpdate = NULL;

// ============================================================================
// SWF (Scaleform) Globals
// ============================================================================

void* g_swfInstancePool = NULL;
void* g_swfScriptArrayPool = NULL;
void* g_swfScriptObjectPool = NULL;

// ============================================================================
// Misc Game Globals
// ============================================================================

void* g_pClockObj = NULL;
void* g_pGmLogic = NULL;
void* g_pLerpQueue = NULL;
void* g_pPostEffects = NULL;
void* g_geomSingleton = NULL;
void* g_objFactory = NULL;
void* g_movieRecord = NULL;
void* g_pgStreamerPool = NULL;
uint32_t g_numDrawBuckets = 0;
void* g_serializationRegistry = NULL;
void* g_configCallback = NULL;
uint8_t g_bProfilingActive = 0;
uint8_t g_bNonWidescreen = 0;
uint8_t g_faceAnimDirty = 0;
float g_fadeTime = 0.0f;
float g_hitVectorFlip = 1.0f;

// ============================================================================
// Swing/Shot System Globals
// ============================================================================

uint32_t g_swingCountFlag = 0;
uint32_t g_swingInFlightFlag = 0;
float g_swingPhaseThreshold = 0.0f;
const char** g_shotTypeNames = NULL;
const char* g_szServeStartedTypeName = "ServeStarted";
float g_recoveryTimerThreshold = 0.0f;

// ============================================================================
// Physics Constants
// ============================================================================

float g_kFrameAlpha = 0.9f;
float g_kFrameSnapThresh = 0.001f;
float g_kFrameToSecScale = 1.0f / 60.0f;
float g_kOneMinusAlpha = 0.1f;
float g_kPowerBlend = 0.5f;
float g_kSwingRadiusConst = 1.0f;
float g_kSwingScaleFactor = 1.0f;
float g_kTicksPerSec = 60.0f;
float g_kZero = 0.0f;
float g_ticksToSeconds = 1.0f / 60.0f;

// ============================================================================
// Gate Globals
// ============================================================================

uint8_t g_gate_appStarted = 0;
uint8_t g_gate_dumpLeaks = 0;
uint8_t g_gate_logActiveFlag = 0;
uint8_t g_gate_setProcessName = 0;
uint8_t g_gate_startLogging = 0;

// ============================================================================
// Tip System Globals
// ============================================================================

uint32_t g_tipType_Play1 = 0;
uint32_t g_tipType_Play2 = 0;
uint32_t g_tipType_Play3 = 0;

// ============================================================================
// XML Type Globals
// ============================================================================

void* g_xmlType_Bool = NULL;
void* g_xmlType_Int = NULL;
void* g_xmlType_ShotType = NULL;

// ============================================================================
// Rage Main Globals
// ============================================================================

uint32_t g_rage_main_call_depth = 0;

// ============================================================================
// Allocator Globals
// ============================================================================

void* g_pAllocator = NULL;
void* g_pAllocatorBase = NULL;
void* g_mainAllocTable = NULL;
void* g_sda_base = NULL;

// ============================================================================
// Command Line Globals
// ============================================================================

void* g_pCmdArgRegistry = NULL;
const char* g_pExeName = "default.xex";
const char* g_empty_str = "";

// ============================================================================
// String Constants
// ============================================================================

const char* k_memtrack_key = "memtrack";
const char* k_unknown_opt_fmt = "Unknown option: %s\n";
const char* k_winsock_no_dll = "WSAStartup failed: no DLL\n";
const char* k_winsock_bad_ver = "WSAStartup failed: bad version\n";
const char* k_subsysInitErrFmt = "Subsystem init failed: %s\n";
const char* k_sceneRenderErr = "Scene render error\n";
const char* k_sceneRenderTimingFmt = "Scene render: %.2fms\n";
const char* k_pgThreadNameFmt = "PG Thread %d";
const char* k_rageThreadNameFmt = "RAGE Thread %d";
const char* k_refreshRateOffset = "refreshRateOffset";
const char* k_pageSpacing = "pageSpacing";
const char* k_scrollSpeed = "scrollSpeed";
const char* k_textScale = "textScale";
const char* k_standardAspectScale = "standardAspectScale";
const char* k_default_xex_path = "default.xex";
const char* k_disc_path_prefix = "game:\\";
const char* k_leaks_path = "leaks.txt";
const char* k_logmem_csv = "logmem.csv";
const char* k_audBasePath = "audio\\";
const char* k_audPathFmt = "%s%s";
const char* k_audPathFmt2 = "%s%s.xma";
const char* k_aspectHiDef = "16:9";
const char* k_aspectStandard = "4:3";
const char* k_buttonCountError = "Button count mismatch";
const char* k_buttonCountField = "buttonCount";
const char* k_colourField = "colour";
const char* k_defaultFPS = "60.0";
const char* k_dialogTypeError = "Invalid dialog type";
const char* k_dialogTypeField = "dialogType";
const char* k_displayModeTolerance = "0.01";
const char* k_factoryParam1 = "param1";
const char* k_fadeInTime = "fadeInTime";
const char* k_fadeOutTime = "fadeOutTime";
const char* k_fpsFmtStr = "FPS: %.1f";
const char* k_fpsLowerBound = "30.0";
const char* k_fpsUpperBound = "120.0";
const char* k_frontendFieldName_1 = "field1";
const char* k_frontendFieldName_2 = "field2";
const char* k_holdTime = "holdTime";
const char* k_initRefreshRate = "60.0";
const char* k_initSceneTime = "0.0";
const char* k_lineSpacing = "lineSpacing";
const char* k_SAVING = "SAVING";
const char* kFaceAnimStartPhase = "0.0";

// ============================================================================
// Globals merged from globals_linker.cpp
// ============================================================================

// Forward struct used by g_swipePatterns
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

// AI type IDs
uint32_t* g_aiTypeId_1 = NULL;             // @ 0x825C60B4
uint32_t* g_aiTypeId_2 = NULL;             // @ 0x825C803C
uint32_t* g_aiTypeId_3 = NULL;             // @ 0x825C8038

// Animation
const float g_animBlendWeight = 0.0f;      // @ 0x8202D110

// Ball / swing attribute hashes
uint32_t g_attrHash_HitZone    = 0;        // @ 0x825C761C
uint32_t g_attrHash_SwingType1 = 0;        // @ 0x825C8038
uint32_t g_attrHash_SwingType2 = 0;        // @ 0x825C803C
const float g_alphaMultiplier  = 0.0f;     // @ 0x82079BE0

// CRT atexit guards
int32_t g_atexit_guard_0 = 0;              // @ 0x82733048
int32_t g_atexit_guard_1 = 0;              // @ 0x8273304C

// Bink video player
void* g_binkVideoPlayer = NULL;            // @ 0x825EB988

// Camera system
float    g_cameraAdjustment  = 0.0f;       // @ 0x8202CFF4
uint8_t  g_cameraFlagA       = 0;
uint8_t  g_cameraFlagB       = 0;
uint8_t  g_cameraFlag_7532   = 0;
uint8_t  g_cameraFlag_8897   = 0;
uint8_t  g_cameraFlag_8898   = 0;
uint32_t g_cameraFlags       = 0;          // @ 0x826064D0
uint32_t g_cameraMode        = 0;          // @ 0x825C5EB8
uint32_t g_cameraProperty1   = 0;          // @ 0x825C8A40
uint32_t g_cameraProperty2   = 0;          // @ 0x825C93D0
uint8_t  g_cameraSelector    = 0;
int32_t  g_cameraState       = 0;
float    g_cameraThreshold   = 0.0f;

// Network debug flag
uint8_t g_bNetworkDebugFlag = 0;           // @ 0x826065EB

// Character view system
void*    g_charViewData_ptr       = NULL;  // @ 0x8271A2F0
uint32_t g_charViewSelectCounter  = 0;     // @ 0x82606490
uint32_t g_charViewSelectFlag     = 0;     // @ 0x82606494
uint32_t g_charViewTypeId         = 0;     // @ 0x825C2BC4
uint32_t g_character_type_id      = 0;     // @ 0x825C2BC0
uint32_t g_character_type_id_2    = 0;     // @ 0x825C803C
uint32_t g_character_type_id_3    = 0;     // @ 0x825C8038

// Network: cmRefreshableCtor vtable (RTTI, 20 vtables / virtual base MI)
/* TODO: retype as `const cmRefreshableCtor_vtbl*` */
const uint8_t* g_cmRefreshableCtorVtable = lbl_820533CC;  // @ 0x820533CC

// Player contact zone constants
const float g_contactZoneMinA = 0.0f;
const float g_contactZoneMaxA = 0.0f;
const float g_contactZoneMinB = 0.0f;
const float g_contactZoneMaxB = 0.0f;

// Creature manager
void* g_creatureManager = NULL;            // @ 0x826063B8

// Render state
void* g_currentRenderState = NULL;         // @ 0x821EBAA0

// Debug mode
uint8_t g_debugMode = 0;                   // @ 0x825C64AD

// Default constants
const float g_defaultDuration            = 0.0f;  // @ 0x825C755C
float       g_defaultVibrationIntensity  = 0.0f;  // @ 0x8202D110
const float g_default_float_zero         = 0.0f;  // @ 0x825CAF94
const float g_default_float_value        = 0.0f;  // @ 0x825CAF88
const float g_default_float_value_2      = 0.0f;  // @ 0x825CAF90

// Draw bucket
void* g_drawBucket = NULL;                 // @ 0x826064F4

// Error / debug strings
const char* g_error_attract_exit      = "";
const char* g_error_charview_enter    = "";
const char* g_error_charview_exit     = "";
const char* g_error_type_mismatch     = "";
const char* g_error_unhandled_event   = "";

// Event type table
void* g_eventTypeTable = NULL;             // @ 0x82017888

// Float constants (network / physics)
const float g_floatConstant1 = 0.0f;       // @ 0x82079AD4
const float g_floatConstant2 = 0.0f;       // @ 0x8202D110
const float g_floatConstant3 = 0.0f;       // @ 0x8202D110
const float g_floatConstant4 = 0.0f;       // @ 0x825C7600
const float g_floatEpsilon   = 0.0f;       // @ 0x8202D110
const float g_floatNegOne    = 0.0f;       // @ 0x8202D110
const float g_floatOne       = 0.0f;       // @ 0x8202D110
const float g_floatZero      = 0.0f;       // @ 0x8202D110

// FloatAverager vtables (network classes) — RTTI: FloatAverager (4 vtables, MI)
/* TODO: retype as `const FloatAverager_vtbl*` once header exists */
const uint8_t* g_FloatAverager_vtable_1 = lbl_8203A910;   // @ 0x8203A910
const uint8_t* g_FloatAverager_vtable_2 = lbl_8203A91C;   // @ 0x8203A91C
const uint8_t* g_FloatAverager_vtable_3 = lbl_82070D78;   // @ 0x82070D78
const uint8_t* g_FloatAverager_vtable_4 = lbl_8207166C;   // @ 0x8207166C

// Physics: rage::fragDrawable vtable
/* TODO: retype as `const rage::fragDrawable_vtbl*` */
const uint8_t* g_fragDrawableVtable = lbl_82033094;       // @ 0x82033094

// Frame sync object
void* g_frameSyncObj = NULL;               // @ 0x825F64F4

// Game data manager
void* g_gameDataMgr       = NULL;          // @ 0x8271A2E4
void* g_game_data_manager  = NULL;         // @ 0x825CA174
void* g_game_state_ptr     = NULL;         // @ 0x8271A2F8

// Game data type IDs
uint32_t g_gdEventBase_typeId    = 0;      // @ 0x825C5800
uint32_t g_gdRivalryData_typeId  = 0;      // @ 0x825C5F68
uint32_t g_gdRivalry_typeId      = 0;      // @ 0x825C5F6C
uint32_t g_gdTierMember_typeId   = 0;      // @ 0x825C5F70
uint32_t g_gdTier_typeId         = 0;      // @ 0x825C5F80
uint32_t g_gdVibEvent_typeId     = 0;      // @ 0x825C5DF4

// Global streamer
void* g_globalStreamer = NULL;             // @ 0x82606600

// UI state
uint32_t* g_global_ui_state = NULL;        // @ 0x8271A348

// HUD
void* g_hudFlashBase   = NULL;            // @ 0x82606454
float g_hudRenderParam = 0.0f;            // @ 0x82140110

// Player adjustment / input / timing constants
float       g_kAdjustmentScale     = 0.0f;  // @ 0x82079BAC
const float g_kAdjustmentThreshold = 0.0f;  // @ 0x8202D110
const float g_kInputScale          = 0.0f;  // @ 0x825C5938
const float g_kInputThresholdHigh  = 0.0f;  // @ 0x8202D110
const float g_kMaxAdjustment       = 0.0f;  // @ 0x82079CD8
const float g_kQuantScale1         = 0.0f;  // @ 0x82079FFC
const float g_kQuantScale2         = 0.0f;  // @ 0x82079FF8
const float g_kSpecialThreshold    = 0.0f;  // @ 0x825C8A50
float       g_kTimingConstant      = 0.0f;  // @ 0x825C4930
const float g_kZeroThreshold       = 0.0f;  // @ 0x8202D108

// Local user pointer
void* g_localUserPtr = NULL;

// Locomotion / creature animation
void*       g_locomotionAnimInstance = NULL;  // @ 0x8271A378
const float g_lookAtMultiplier       = 0.0f;  // @ 0x8202D184
const float g_lookAtRefAngle         = 0.0f;  // @ 0x8202D10C

// Match result / player select tables
void* g_matchResultTable  = NULL;          // @ 0x82027884

// Matrix / resource field types
void* g_matrixFieldType   = NULL;          // @ 0x825CAFA4

// Message sink
uint32_t      g_msgSinkBitfield      = 0;        // @ 0x825B0C30
const float   g_msgSinkInitFloat     = 0.0f;     // @ 0x825C5938
const uint32_t g_msgSinkTemplate[4]  = {0};      // @ 0x8241A0C0

// Network globals
uint32_t  g_netMessageHolderCount = 0;     // @ 0x826066A4
int32_t   g_networkMode           = 0;
void*     g_networkStatePtr       = NULL;  // @ 0x8271A36C
uint32_t  g_networkTimestamp      = 0;
void*     g_networkTimingObject   = NULL;  // @ 0x8271A328

// Network pointer globals
void*     g_pAtSingleton              = NULL;   // @ 0x821EAB08
void*     g_pAtSingletonPtr           = NULL;   // @ 0x8271A328
void*     g_pCleanupVtable            = NULL;   // @ 0x8206C394
uint8_t*  g_pDataRequestMsgPoolBase   = NULL;   // @ 0x825D129C
void*     g_pDataRequestMsgSingleton  = NULL;   // @ 0x825D1290
uint8_t*  g_pDataSendMsgPoolBase      = NULL;   // @ 0x825D12B4
void*     g_pDataSendMsgSingleton     = NULL;   // @ 0x825D12A8
void*     g_pGameStatePtr             = NULL;   // @ 0x8271A328
void**    g_pInputArrayTable          = NULL;   // @ 0x825EAB28
void**    g_pMessageQueuePool         = NULL;   // @ 0x825D128C
void*     g_pNetworkBase              = NULL;   // @ 0x8271A7B0
void*     g_pNetworkState             = NULL;   // @ 0x8271A7B0
uint32_t* g_pNetworkStatePtr          = NULL;   // @ 0x8271A7B0
void*     g_pNetworkTimer             = NULL;   // @ 0x8201A328
void*     g_pPlayerArrayBase          = NULL;   // @ 0x8211A31C
uint8_t*  g_pPlayerInputTable         = NULL;
uint8_t*  g_pTournamentMsgPool        = NULL;   // @ 0x825D1464
void*     g_pTournamentMsgSingleton   = NULL;   // @ 0x825D1458

// Pause flag
uint8_t g_pauseFlag = 0;                  // @ 0x82606670

// Physics callbacks
void* g_phCallback0 = NULL;               // @ 0x825EA900
void* g_phCallback1 = NULL;               // @ 0x825EA904
void* g_phCallback2 = NULL;               // @ 0x825EA908
void* g_phCallback3 = NULL;               // @ 0x825EA90C
void* g_phCallback4 = NULL;               // @ 0x825EA910
void* g_phCallback5 = NULL;               // @ 0x825EA914

// Physics system
void** g_physicsStateArray = NULL;
void*  g_physicsSystem     = NULL;         // @ 0x82606568

// Player movement message pool
uint8_t* g_playerMovementMsgPool          = NULL; // @ 0x825D1164
void*    g_playerMovementMsgPoolSingleton  = NULL; // @ 0x825D1158

// Player property types
uint32_t g_playerPropType1 = 0;            // @ 0x82062D7C
uint32_t g_playerPropType2 = 0;            // @ 0x820693D0

// Player select / slot state
void*    g_playerSelectTable     = NULL;   // @ 0x82027888
int32_t* g_playerSlotIndices     = NULL;
int32_t* g_playerSlotStates      = NULL;
void*    g_player_controller_ptr = NULL;   // @ 0x825C6564
void*    g_player_manager_ptr    = NULL;   // @ 0x8271A2D0

// Random state
uint64_t g_randomState = 0;               // @ 0x825DA268

// Recovery constants
const float g_recoveryScale     = 0.0f;
float       g_recoveryThreshold = 0.0f;

// Render system
void* g_renderContext        = NULL;       // @ 0x826063D4
int   g_renderModeIndex      = 0;          // @ 0x821F0898
void* g_renderStateStack[16] = {0};        // @ 0x821EBA9C
void* g_rendererState        = NULL;       // @ 0x8260641C

// Resource / string field types
void* g_resourceFieldType = NULL;          // @ 0x825CAF88
void* g_stringFieldType   = NULL;          // @ 0x825CAF88

// Scene render objects
void* g_sceneRenderObj  = NULL;            // @ 0x82606430
void* g_sceneRenderObj2 = NULL;            // @ 0x8260641C

// Scene reset matrix
uint32_t g_sceneResetMatrix[4] = {0};      // @ 0x8261A0C0

// Selected character index
int32_t g_selectedCharacterIndex = -1;     // @ 0x825C76B8

// Scene graph type IDs
uint32_t g_sgNode_typeId       = 0;        // @ 0x825C93D0
uint32_t g_sgRMDrawable_typeId = 0;        // @ 0x825C2D78

// Slow motion manager
void* g_slowmo_manager_ptr = NULL;         // @ 0x825C6540

// Misc render objects
void* g_someRenderObj1 = NULL;             // @ 0x82606654
void* g_someRenderObj2 = NULL;             // @ 0x825FEAB0

// State name strings
const char* g_stateName_attract  = "";     // @ 0x8205E094
const char* g_stateName_charView = "";     // @ 0x8205EF5C

// Game data error strings
const char* g_str_gdLadder_duplicateLevel       = "";
const char* g_str_gdLadder_missingLadder        = "";
const char* g_str_gdLadder_unknownNodeType      = "";
const char* g_str_gdRivalryData_noCharName      = "";
const char* g_str_gdRivalryData_noRivalName     = "";
const char* g_str_gdRivalryData_unknownRival    = "";
const char* g_str_gdRivalry_duplicateChar       = "";
const char* g_str_gdRivalry_missingRival        = "";
const char* g_str_gdRivalry_unknownChar         = "";
const char* g_str_gdRivalry_unknownNodeType     = "";
const char* g_str_gdTierMember_noCharName       = "";
const char* g_str_gdTierMember_unknownChar      = "";
const char* g_str_gdTier_noTierName             = "";
const char* g_str_gdTier_unknownNodeType        = "";

// Pong strings
const char* g_str_pongDrill_tooManySuccesses = "";
const char* g_str_pongMover_calcInitMatrix   = "";

// Swipe patterns array
struct SwipePattern g_swipePatterns[17] = {{0}};  // @ 0x825D09A0

// Network RTTI / type name strings
const char g_szCreateMachineRTTI[]          = "";  // @ 0x825DEEB0
const char g_szDataRequestTypeName[]        = "";  // @ 0x8206EA74
const char g_szDataSendTypeName[]           = "";  // @ 0x8206EA88
const char g_szDefaultError[]               = "";  // @ 0x82027423
const char g_szInvalidError[]               = "";  // @ 0x8204EF04
const char g_szMessageType[]                = "";  // @ 0x8205DFB0
const char g_szNetworkDebugMsg[]            = "";  // @ 0x8205AE98
const char g_szRageRTTI[]                   = "";  // @ 0x825DEECC
const char g_szTopSpin[]                    = "";  // @ 0x8206CB54
const char g_szTournamentCompleteTypeName[] = "";  // @ 0x8206EC40

// Thread-local storage base
void** g_tls_base = NULL;                 // @ 0x82600000

// UI list manager
void* g_uiListManager = NULL;             // @ 0x825D16AC

// UI context / objects
void*    g_ui_context         = NULL;      // @ 0x8271A81C
uint8_t  g_ui_object_flags[64]  = {0};     // @ 0x825F3A68
void*    g_ui_object_ptrs[256]  = {0};     // @ 0x825F3968
uint32_t g_ui_object_types[256] = {0};     // @ 0x825F3868
uint32_t g_ui_state_counter     = 0;       // @ 0x825F3864

// Vibration system
void*    g_vibrationMgr        = NULL;     // @ 0x8271A3A8
uint8_t* g_vibrationStateArray = NULL;     // @ 0x8271A364

// Vtable pointers
void* g_vtable_char_view_cs              = NULL;
void* g_vtable_pong_attract_context      = NULL;
void* g_vtable_pong_attract_context_2    = NULL;
void* g_vtable_pong_char_view_context    = NULL;
void* g_vtable_pong_char_view_context_2  = NULL;
void* g_vtable_pong_char_view_context_3  = NULL;
void* g_vtable_pong_char_view_state      = NULL;

// XEX exec info
void* g_xex_exec_info = NULL;             // @ 0x825E6E64

// XML node struct type IDs
uint32_t g_xmlNodeStruct_typeId  = 0;      // @ 0x825C803C
uint32_t g_xmlNodeStruct_typeId2 = 0;      // @ 0x825C8038

// Zero / constant floats
const float g_zeroConstant = 0.0f;         // @ 0x8202D110
const float g_zeroFloat    = 0.0f;         // @ 0x82079AD8
