# Missing Symbols - Complete List

This file contains ALL undefined symbols that need to be implemented.
Total: ~500+ symbols across functions, methods, and global variables.

## Instructions

For each symbol:
1. Search in binary: `mcp_tt_decomp_phase5_search_symbols(query="SymbolName")`
2. Get function info: `mcp_tt_decomp_phase5_get_function_info(function_name="actual_name")`
3. Get assembly: `mcp_tt_decomp_phase5_get_function_recomp(function_name="actual_name")`
4. Properly decompile into clean C++ with meaningful name
5. Add to appropriate source file (NOT a stub file!)

---

## 1. Xbox 360 Kernel Functions (20)

```
HalReturnToFirmware
KeBugCheck
KeSetAffinityThread
ObDereferenceObject
ObReferenceObjectByHandle
RtlEnterCriticalSection
RtlLeaveCriticalSection
_KeTlsAlloc_thunk
ke_KeTlsFree_624C
ke_KeTlsSetValue_622C
XGetVideoMode
NetDll_WSACleanup
NetDll_WSAStartup
NetDll_XNetStartup
__imp_ExLoadedCommandLine
__imp_ExThreadObjectType
```

---

## 2. CRT/Entry Functions (8)

```
main
__cinit_impl
__heap_init_impl
__run_table
_crt_spinlock_acquire
_crt_spinlock_release
_crt_tls_fiber_setup
_xe_strcpyn_10
```

---

## 3. RAGE Core Functions (30)

### Memory Management
```
rage_malloc
rage_free_00C0
rage_free(void*)
rage::rage_free(void*)
rage::rage_alloc_aligned(unsigned long, unsigned long)
rage_BlockAlloc
rage_InitMainThreadHeap
sysMemAllocator_Alloc
```

### String Functions
```
rage_StringCompareNoCase
rage_StringFindChar
```

### Singleton/Observer Pattern
```
rage_FindSingleton
rage::FindSingleton(void*)
atSingleton_Find
atSingleton_Remove
atSingleton_2598_w
atSingleton_29E8_p25(void*)
atSingleton_91E0_gen(unsigned int)
rage::BindObject(void*, void*, unsigned int, unsigned int)
rage::UnbindObject(void*, void*)
rage::NotifyObservers(void*, void*, unsigned int*)
rage::ClearPendingFlag(void*)
rage::InitializeSingleton(void*)
rage::ReleaseSingleton(void*)
rage::UnregisterSingleton(void const*)
```

### Initialization/Lifecycle
```
rage_AcquireReference
rage_InitializeNetSystem
rage_GetExecutableName
rage_RegisterThread
rage_ThreadPool_Cleanup
rage_game_obj_init_CB60
rage_main_6970
rage_CEF0
```

### Rendering/Debug
```
rage_RenderDebugOverlay
rage_SetRenderMode
rage_DebugLog
nop_8240E6D0(char const*, ...)
nop_8240E6D0(void const*, ...)
```

### UI Context
```
rage_RegisterUIContext(void*, unsigned int, char const*)
```

---

## 4. CM (Curve/Machine) Node System (5)

```
rage::cmMemory::Sync()
rage::cmNode_GetBool(rage::cmNodePort const*)
rage::g_cm_frameRate
rage::g_cm_frameTime
rage::g_cm_negateVec
rage::g_cm_zeroFloat
```

---

## 5. Graphics/Rendering Functions (40)

### Device Management
```
grcDevice_Present
grcDevice_SubInit
grcDevice_WaitFence
grcDevice_beginScene
grcDevice_clear
grcDevice_shutdown
grcDevice_shutdownAlt
grcDisplay_Create
```

### Render Targets/Config
```
CleanupRenderTargets
ConfigureRenderTargets
InitializeRenderConfig
SetupRenderFiber
```

### Texture System
```
rage::Release(rage::grcTexture*)
rage::grcTextureReferenceBase::vfn_1()
rage::grcTextureReferenceBase::vfn_2()
rage::grcTextureReferenceBase::vfn_3()
rage::grcTextureReferenceBase::vfn_5()
rage::grcTextureReferenceBase::vfn_6()
rage::grcTextureReferenceBase::vfn_8()
rage::grcTextureReferenceBase::vfn_9()
rage::grcTextureReferenceBase::vfn_10()
rage::grcTextureReferenceBase::vfn_13(void*, void*)
rage::grcTextureReferenceBase::vfn_14(void*, void*)
rage::grcTextureReferenceBase::vfn_15()
rage::grcTextureReferenceBase::vfn_16(void*)
rage::grcTextureReferenceBase::vfn_17(float, void*)
rage::grcTextureReferenceBase::vfn_18(void*, void*)
rage::grcTextureReferenceBase::vfn_19()
rage::grcTextureReferenceBase::vfn_21(void*, void*, void*)
rage::grcTextureReferenceBase::vfn_22(void*)
rage::grcTextureReferenceBase::vfn_23()
rage::grcTextureReferenceBase::vfn_24()
rage::grcTextureReferenceBase::vfn_25()
```

---

## 6. UI/Page System Functions (20)

```
DestroyPageGroup
GetPageGroupState
ProcessPageGroupInput
UpdatePageGroup
FadePageGroup(void*, int, int, float, float)
InitializePageGroup(void*)
DismissPageGroup(void*)
SetPageGroupVisible(void*)
PageGroup_Register(void*)
PageGroup_SetState(void*)
PageGroup_GetTextEntry(void*, char const*)
PageGroup_LookupText(unsigned int, char const*)
PostPageGroupMessage(int, int, int, int, int)
PostPageGroupMessage(unsigned int, unsigned char, unsigned int, unsigned int, unsigned int, unsigned int)
PostPageGroupMessage(void*, int, int, int, int)
Dialog_IsComplete(void*)
NotifyUIEvent
TextEntry_GetValue(void*)
```

---

## 7. File I/O Functions (10)

```
fiAsciiTokenizer_3650_g
fiAsciiTokenizer_3FB8_g
fiAsciiTokenizer_FinalizeTokenizer
fiAsciiTokenizer_InitializeTokenizer
fiAsciiTokenizer_ReadNextLine
fiAsciiTokenizer_SkipWhitespace
fiDeviceMemory_AllocateDeviceMemory
fiDevice_GetDevice
fiStream_Open
```

---

## 8. Audio System Functions (20)

### Core Audio
```
audSystem_configure_2930
audSystem_shutdown_29C0
audControl_Destructor(void*)
```

### Audio Effects (XAudio)
```
CDelayEffect::CalcInputFrames()
CDelayEffect::GetRegistrationProperties()
CDelayEffect::IsInputFormatSupported()
CDelayEffect::LockForProcess()
CDelayEffect::Process()
CPeakMeterEffect::CalcInputFrames()
CPeakMeterEffect::GetRegistrationProperties()
CPeakMeterEffect::IsInputFormatSupported()
CPeakMeterEffect::LockForProcess()
CPeakMeterEffect::Process()
CPeakMeterEffect::UnlockForProcess()
CShelvingFilterEffect::CalcInputFrames()
CShelvingFilterEffect::GetRegistrationProperties()
CShelvingFilterEffect::IsInputFormatSupported()
CShelvingFilterEffect::LockForProcess()
CShelvingFilterEffect::Process()
CShelvingFilterEffect::UnlockForProcess()
```

---

## 9. Physics System Functions (5)

```
phWorld_Construct
phMaterialMgrImpl_AE20_p46
phMaterialMgrImpl_C208_g(void*, void const*)
phBoundCapsule_A080_g(void*)
ph_21B0
```

---

## 10. Animation System Functions (10)

```
crAnimBlenderState_Init(void*)
pcrAnimBlender_Initialize(void*)
pcrAnimBlender_ApplyClipWeights(void*, unsigned int, float, float)
pcrAnimState_ComputePosition(vec3*, pongAnimState*)
pcrPostPointBlender_GetActiveClipIndex(pcrPostPointBlender*)
LocomotionStateAnim_BlendLocomotionAnims(unsigned int, void*, float)
LocomotionStateAnim_TransitionLocomotionState(unsigned int, void*, float)
mfMotionClipRAGE_B8D8_g(void*, unsigned int, unsigned int, unsigned int)
```

---

## 11. Game-Specific Functions (50+)

### Player Management
```
plrPlayerMgr::PostLoadChildren()
plrPlayerMgr::PostLoadProperties()
plrPlayerMgr::Validate()
plrPlayerMgr::vfn_23()
plrPlayerMgr::vfn_24()
plrPlayerMgr::vfn_25()
plrPlayerMgr::vfn_27()
plrPlayerMgr::vfn_28()
plrPlayerMgr::vfn_29()
plrPlayerMgr::vfn_30()
plrPlayerMgr::vfn_31()
plrPlayerMgr::vfn_32()
```

### Pong Player Functions
```
pongPlayer::Process(float)
pongPlayer_0508_g(void*, int, float*)
pongPlayer_1460_g(void*, int)
pongPlayer_42E0_g(void*, vec3*, vec3*, float)
pongPlayer_5B60_gen(pongPlayer*)
pongPlayer_73E8_g(pongPlayer*, float)
pongPlayer_9CD0_g(void*, int, void*, void*)
pongPlayer_AB48_g(vec3*, void*)
pongPlayer_B208_g(void*, vec3*, unsigned char)
pongPlayer_BF18_g(void*, int, vec3*, vec3*, int)
pongPlayer_C678_g(pongPlayer*)
pongPlayer_D238_g(pongPlayer*)
pongPlayer_DE98_g(void*)
```

### Pong Attract State
```
pongAttractState::GetName()
pongAttractState::OnEnter()
pongAttractState::OnExit()
pongAttractState::ProcessInput()
pongAttractState::Reset()
pongAttractState::Shutdown()
pongAttractState_Shutdown(void*)
typeinfo for pongAttractState
```

### Pong Draw/Render
```
pongDrawBucket::InitStatics()
pongDrawBucket::Load(char const*, int, void*)
pongPostEffects_ctor_F160
```

### Pong Creature
```
pongCreature_7CE8_g(void*, void*, int, int, int, int)
```

### Pong Lerp Queue
```
pongLerpQueue_3410_g(void*)
```

---

## 12. Network/Multiplayer Functions (15)

```
SinglesNetworkClient_EFB8_g
SinglesNetworkClient_58E8_g(unsigned char)
SinglesNetworkClient_6838_g(void*, unsigned char, int)
SinglesNetworkClient_8DF8_g(void*, void*, int)
SinglesNetworkClient_B1E8_g(void*)
SinglesNetworkClient_B2A8_g(void*)
SinglesNetworkClient_B320_g(void*)
CheckButtonPressed(void*)
ComputeNetworkHash(void*, int)
DeserializeNetworkData(void*, void*, int)
ReadBitsFromStream(void*, void*, int)
WriteBallHitDataToNetwork(void*, void*)
WriteFloatToNetworkStream(void*, float)
ServeStartedMessage_5728(void*, void*, void*, float)
netSystem_shutdown_B510
```

---

## 13. HSM (Hierarchical State Machine) Functions (5)

```
HSM_QueueNotification(void*)
PostStateTransitionRequest(void*, int)
hsmContext_SetNextState_2800
hsmContext_SetNextState_2800(void*, int)
```

---

## 14. SWF (Scaleform) Functions (10)

```
swfContext_Cleanup
swfFile_DestroyResources
swfInstance_Cleanup
rage::swfSCRIPTOBJECT::vfn_6()
rage::swfSCRIPTOBJECT::vfn_8()
rage::swfSCRIPTOBJECT::vfn_9()
rage::swfSCRIPTOBJECT::vfn_10()
rage::swfSCRIPTOBJECT::vfn_11()
rage::swfSCRIPTOBJECT::vfn_12()
rage::swfSCRIPTOBJECT::vfn_13()
```

---

## 15. XML/Serialization Functions (10)

```
xml_ReadInt(char const*)
xml_ReadString(char const*, char*, int)
xmlNodeStruct_vfn_2
xmlNodeStruct_Init(void*)
RegisterSerializedField(void*, char const*, void*, void const*, int)
RegisterSerializedField(void*, char const*, void*, void*, int)
sub_821A8F58(void*, char const*, void*, void*, int)
datRef_Release
```

---

## 16. Input System Functions (5)

```
io_Input_poll
io_Input_poll_9D68(void*)
```

---

## 17. Scene Graph Functions (5)

```
sgSceneGraph_FreeLists(void*)
gmLogic_vfn_3
```

---

## 18. Utility Functions (20)

```
util_6C20(void*, unsigned int)
util_7AE8
util_C930(void*)
util_CE30
util_CDF0(void*, void const*, unsigned int)
thunk_game_C330
atArray_Clear(void*)
atHashMap_Clear
CopyVectorThreadSafe(void*, void*)
QueueGameMessage(int, int, void*, int, void*, void*)
SendContextMessage(int, int, int, int)
SetTrainingState
```

---

## 19. XE (Xbox Executable) Functions (10)

```
xe_5BB0
xe_EC88
xe_EC88(int)
xe_EC88(unsigned int)
xe_main_thread_init
xe_main_thread_init_0038()
xe_phys_alloc_6AC8
xe_thread_ctx_init_6D40
xam_6C88_g
```

---

## 20. Game Utility Functions (10)

```
game_1620(void*)
game_8EE8(void*)
game_9358(void*)
game_AAF8(void*, int, int)
game_AD40(void*, unsigned int)
game_CD20(void*)
game_D060(void*)
game_D468(void*, vec3*)
pg_9C00_g(void*)
pg_9C00_g(void*, int)
pg_EDE0_gen
nt_0420
```

---

## 21. Global Variables (200+)

### Allocator/Memory Globals
```
g_allocatorInitFlag
g_pAllocator
g_pAllocatorBase
g_mainAllocTable
g_sda_base
```

### Command Line/Args
```
g_argc
g_argv
g_argv_buf
g_pCmdArgRegistry
g_pExeName
```

### Exit/Atexit
```
g_atexit_table_cur
g_atexit_table_end
g_exit_end
g_exit_start
g_on_exit_end
g_on_exit_start
g_doexit_called
g_doexit_flag
g_doexit_type
```

### Audio Globals
```
g_audio_obj_ptr
g_audio_config_ptr
g_audio_unknown_25436
g_audio_unknown_25440
g_audPathTable
```

### Graphics Globals
```
g_grcDeviceState
g_grcDeviceVtable
g_grcFlagsObj
g_grcInstanceIdSeed
g_grcReadyFlag
g_grcRenderConfig
g_grcSetup_vtable
g_grcSubsystem1
g_grcSubsystem2
g_grcDefaultNameRecord
g_pGrcDevice
g_pGrcDevice_beginScene
g_pGrcDevice_clear
g_pGrcRenderer
g_pGrcRenderer2
g_pGrcScene
g_pGrcSceneB
g_display_obj_ptr
g_pSecondaryDevice
g_displayModeBase
g_nScreenHeight
g_nScreenWidth
g_nonWsConfig
```

### Render Globals
```
g_render_obj_ptr
g_renderBeginTimestamp
g_renderClearRingDivisor
g_renderClearRingIdx
g_renderClearRingSaved
g_renderCmdBufBase
g_renderCmdRingBufPtr
g_renderCmdRingCounter
g_renderCmdRingCounterSaved
g_renderDeltaTime
g_renderFrameTime
g_renderStateFlags
g_renderTimingA
g_renderTimingB
g_postRenderConfig
g_pRenderSetupObj
g_frameRenderCounter
```

### Game Loop Globals
```
g_game_obj_ptr
g_loop_obj_ptr
g_pGameLoop
g_pRootGameObj
```

### HSM Globals
```
g_hsmContextPtr
g_hsmMgr
g_hsmMgrPtr
g_hsmMgr_flags
```

### Input Globals
```
g_input_obj_ptr
g_pInputObj
g_pInputScoreTable
g_pButtonStateTable
g_controllerArrayBase
```

### Player Globals
```
g_pPlayerSlotTable
g_pPlayerDataTable
g_pPlayerDataTable_slot
g_pPlayerLerpTable
g_plrPlayerMgr_state
```

### Network Globals
```
g_pNetSystem
g_pNetMsgPoolBase
g_pNetMsgSingleton
g_singles_network_client
```

### Physics Globals
```
g_pPhysicsWorld
g_phFrameCounter
g_phFrameIndex
g_matMgrGuard
```

### UI Globals
```
g_uiManagerPtr
g_pHudOverlay
g_pStencilMgrObj
```

### Credits/Dialog Globals
```
g_creditsRoll
g_creditsRollVisible
g_pCreditsRollObj
g_creditsAssetId_A
g_creditsAssetId_B
g_creditsAssetId_C
g_creditsColourSchema
g_creditsContextData
g_creditsEntriesFieldDesc
g_creditsEntriesSchema
g_creditsExitTable
g_creditsNameFieldDesc
g_creditsNameSchema
g_creditsSettingsSchema
g_creditsTransTable
g_dialogAssetId_A
g_dialogAssetId_B
g_dialogAssetId_C
g_dialogFieldSchema
g_dialogPageGroup
g_pDialogRollObj
```

### Frontend Globals
```
g_frontendAssetId_A
g_frontendAssetId_B
g_frontendAssetId_C
g_frontendFieldSchema_1
g_frontendFieldSchema_2
```

### Settings Globals
```
g_settingsAssetId_A
g_settingsAssetId_B
g_settingsAssetId_C
g_gameSettings
```

### Attract State Globals
```
g_attractStateCount
g_attractStateCtxPtrs
g_attractStateFlags
g_attractStateSizes
```

### Training/Drill Globals
```
g_pTrainingData
g_pTrainingStateMachine
g_pDrillSaveData
g_fDrillScoreScale
```

### Game State Globals
```
g_pGameState
g_bAppStarted
g_bInitComplete
g_bRestartPending
g_initStateFlag
g_prngState
g_fPrngNormalize
```

### Threading Globals
```
g_mainThreadHandle
g_mainThreadXtf
g_threadpool_head
g_pgThreadInitGate
g_semIO
g_semRender
g_semUpdate
```

### SWF Globals
```
g_swfInstancePool
g_swfScriptArrayPool
g_swfScriptObjectPool
```

### Misc Game Globals
```
g_pCamera
g_pClockObj
g_pGmLogic
g_pLerpQueue
g_pPostEffects
g_geomSingleton
g_objFactory
g_movieRecord
g_pgStreamerPool
g_numDrawBuckets
g_serializationRegistry
g_configCallback
g_bProfilingActive
g_bNonWidescreen
g_faceAnimDirty
g_fadeTime
g_hitVectorFlip
g_identityMatrixRef
g_vectorComparisonMask
```

### Swing/Shot Globals
```
g_swingCountFlag
g_swingInFlightFlag
g_swingPhaseThreshold
g_shotTypeNames
g_szServeStartedTypeName
g_recoveryTimerThreshold
```

### Physics Constants
```
g_kFrameAlpha
g_kFrameSnapThresh
g_kFrameToSecScale
g_kOneMinusAlpha
g_kPowerBlend
g_kSwingRadiusConst
g_kSwingScaleFactor
g_kTicksPerSec
g_kZero
g_ticksToSeconds
```

### Texture Globals
```
rage::g_pTextureFactory
rage::g_bTextureEndianDirty
```

### Gate Globals
```
g_gate_appStarted
g_gate_dumpLeaks
g_gate_logActiveFlag
g_gate_setProcessName
g_gate_startLogging
```

### Tip Globals
```
g_tipType_Play1
g_tipType_Play2
g_tipType_Play3
```

### XML Type Globals
```
g_xmlType_Bool
g_xmlType_Int
g_xmlType_ShotType
```

### Rage Main Globals
```
g_rage_main_call_depth
```

---

## 22. String Constants (50+)

```
g_empty_str
k_memtrack_key
k_unknown_opt_fmt
k_winsock_no_dll
k_winsock_bad_ver
k_subsysInitErrFmt
k_sceneRenderErr
k_sceneRenderTimingFmt
k_pgThreadNameFmt
k_rageThreadNameFmt
k_refreshRateOffset
k_pageSpacing
k_scrollSpeed
k_textScale
k_standardAspectScale
k_default_xex_path
k_disc_path_prefix
k_leaks_path
k_logmem_csv
k_audBasePath
k_audPathFmt
k_audPathFmt2
k_aspectHiDef
k_aspectStandard
k_buttonCountError
k_buttonCountField
k_colourField
k_defaultFPS
k_dialogTypeError
k_dialogTypeField
k_displayModeTolerance
k_factoryParam1
k_fadeInTime
k_fadeOutTime
k_fpsFmtStr
k_fpsLowerBound
k_fpsUpperBound
k_frontendFieldName_1
k_frontendFieldName_2
k_holdTime
k_initRefreshRate
k_initSceneTime
k_lineSpacing
k_SAVING
kFaceAnimStartPhase
```

---

## 23. Vtables (10)

```
rage_datBase_vtable
fsmMachine_vtable
gameLoop_vtable_derived
gameLoop_vtable_parent
grcDisplay_vtable
grcDisplay_vtable2
hsmContext_vtable_init
physicsWorld_vtable
k_factoryVtable
```

---

## Total Summary

- **Functions/Methods**: ~300
- **Global Variables**: ~200
- **String Constants**: ~50
- **Vtables**: ~10

**Grand Total**: ~560 symbols

---

## Priority Implementation Order

1. **CRT/Entry** (8) - Get program to start
2. **Memory** (10) - Basic allocation working
3. **RAGE Core** (20) - Engine initialization
4. **Graphics Init** (10) - Minimal rendering
5. **Game Loop** (5) - Basic frame loop
6. **Everything Else** - As needed for functionality

---

## Notes

- Many hex-based names (e.g., `rage_main_6970`) are temporary and should be replaced with meaningful names
- Virtual functions (`vfn_N`) need proper method names based on what they do
- Global variables need to be defined in appropriate source files
- String constants should go in a constants file or inline where used
- Vtables need proper class definitions

**DO NOT CREATE STUBS** - Each symbol must be properly researched and implemented using MCP decomp tools.
