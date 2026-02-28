/**
 * globals_extended.c — Extended Global Variables
 * Rockstar Presents Table Tennis (Xbox 360)
 *
 * This file contains additional global variables that are referenced
 * throughout the codebase but were not in the original globals.c
 */

#include <stdint.h>
#include <stddef.h>

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
// Audio System Globals
// ============================================================================

void* g_audio_obj_ptr = NULL;
void* g_audio_config_ptr = NULL;
uint32_t g_audio_unknown_25436 = 0;
uint32_t g_audio_unknown_25440 = 0;
void* g_audPathTable = NULL;

// ============================================================================
// Graphics/Rendering Globals
// ============================================================================

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
void* g_pGrcDevice_beginScene = NULL;
void* g_pGrcDevice_clear = NULL;
void* g_pGrcRenderer = NULL;
void* g_pGrcRenderer2 = NULL;
void* g_pGrcScene = NULL;
void* g_pGrcSceneB = NULL;
void* g_display_obj_ptr = NULL;
void* g_pSecondaryDevice = NULL;

void* g_displayModeBase = NULL;
uint32_t g_nScreenHeight = 720;
uint32_t g_nScreenWidth = 1280;
void* g_nonWsConfig = NULL;

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
// Game Loop Globals
// ============================================================================

void* g_game_obj_ptr = NULL;
void* g_loop_obj_ptr = NULL;
void* g_pGameLoop = NULL;
void* g_pRootGameObj = NULL;

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
// UI System Globals
// ============================================================================

void* g_uiManagerPtr = NULL;
void* g_pHudOverlay = NULL;
void* g_pStencilMgrObj = NULL;

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

// ============================================================================
// Game State Globals
// ============================================================================

void* g_pGameState = NULL;
uint8_t g_bAppStarted = 0;
uint8_t g_bInitComplete = 0;
uint8_t g_bRestartPending = 0;
uint32_t g_initStateFlag = 0;
uint32_t g_prngState = 0;
float g_fPrngNormalize = 1.0f / 0xFFFFFFFF;

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

void* g_pCamera = NULL;
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
