#pragma once

/**
 * Xbox 360 Video Display (Vd) API - Audio/Video Synchronization
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks for Xbox 360 video hardware (Xenos GPU).
 * Critical for audio/video synchronization - audio frames must align with video frames.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Video mode structure
typedef struct _VIDEO_MODE {
    uint32_t dwWidth;            // Display width
    uint32_t dwHeight;           // Display height
    uint32_t dwRefreshRate;      // Refresh rate (Hz)
    uint32_t dwFlags;            // Mode flags
} VIDEO_MODE;

// Display information structure
typedef struct _DISPLAY_INFO {
    uint32_t dwWidth;            // Current width
    uint32_t dwHeight;           // Current height
    uint32_t dwRefreshRate;      // Current refresh rate
    uint32_t dwFlags;            // Display flags
    float    fGamma;             // Gamma value
} DISPLAY_INFO;

//=============================================================================
// Video Swap Functions (Critical for Audio Sync)
//=============================================================================

/**
 * VdSwap @ 0x8258637C | ordinal: varies
 * 
 * Swaps video buffers (vsync point).
 * This is the critical synchronization point for audio - audio frames must
 * be submitted before VdSwap to maintain A/V sync.
 * 
 * @param pPrimaryBuffer   Pointer to primary buffer
 * @param pSecondaryBuffer Pointer to secondary buffer (can be NULL)
 * @param dwFlags          Swap flags
 * @param pFenceValue      Pointer to receive fence value
 * @return 0 if successful
 * 
 * Audio timing: At 60 FPS, each frame is 16.67ms = ~735 samples at 44.1kHz
 */
uint32_t VdSwap(void* pPrimaryBuffer, void* pSecondaryBuffer, 
                uint32_t dwFlags, uint64_t* pFenceValue);

//=============================================================================
// Display Mode Functions
//=============================================================================

/**
 * VdSetDisplayMode @ 0x8258641C | ordinal: varies
 * 
 * Sets the display mode (resolution, refresh rate).
 * 
 * @param pMode Pointer to VIDEO_MODE structure
 * @return 0 if successful
 */
uint32_t VdSetDisplayMode(VIDEO_MODE* pMode);

/**
 * VdQueryVideoMode @ 0x825863DC | ordinal: varies
 * 
 * Queries the current video mode.
 * 
 * @param pMode Pointer to receive VIDEO_MODE
 * @return 0 if successful
 */
uint32_t VdQueryVideoMode(VIDEO_MODE* pMode);

/**
 * VdQueryVideoFlags @ 0x825863FC | ordinal: varies
 * 
 * Queries video capability flags.
 * 
 * @return Video flags bitmask
 */
uint32_t VdQueryVideoFlags(void);

/**
 * VdGetCurrentDisplayInformation @ 0x8258643C | ordinal: varies
 * 
 * Gets current display information.
 * 
 * @param pInfo Pointer to receive DISPLAY_INFO
 * @return 0 if successful
 */
uint32_t VdGetCurrentDisplayInformation(DISPLAY_INFO* pInfo);

/**
 * VdGetCurrentDisplayGamma @ 0x825863AC | ordinal: varies
 * 
 * Gets the current display gamma value.
 * 
 * @return Gamma value (typically 2.2)
 */
float VdGetCurrentDisplayGamma(void);

//=============================================================================
// GPU Hardware Functions
//=============================================================================

/**
 * VdGetGraphicsAsicID @ 0x825862BC | ordinal: varies
 * 
 * Gets the graphics ASIC identifier.
 * 
 * @return ASIC ID (Xenos GPU identifier)
 */
uint32_t VdGetGraphicsAsicID(void);

/**
 * VdInitializeRingBuffer @ 0x8258633C | ordinal: varies
 * 
 * Initializes the GPU command ring buffer.
 * 
 * @param pRingBuffer Pointer to ring buffer
 * @param dwSize      Size of ring buffer
 * @return 0 if successful
 */
uint32_t VdInitializeRingBuffer(void* pRingBuffer, uint32_t dwSize);

/**
 * VdGetSystemCommandBuffer @ 0x8258638C | ordinal: varies
 * 
 * Gets the system command buffer for GPU commands.
 * 
 * @return Pointer to command buffer
 */
void* VdGetSystemCommandBuffer(void);

/**
 * VdEnableRingBufferRPtrWriteBack @ 0x8258632C | ordinal: varies
 * 
 * Enables ring buffer read pointer write-back.
 * 
 * @param pRingBuffer Pointer to ring buffer
 * @param bEnable     TRUE to enable, FALSE to disable
 * @return 0 if successful
 */
uint32_t VdEnableRingBufferRPtrWriteBack(void* pRingBuffer, uint32_t bEnable);

//=============================================================================
// GPU Engine Functions
//=============================================================================

/**
 * VdInitializeEngines @ 0x8258645C | ordinal: varies
 * 
 * Initializes GPU engines (shader cores, texture units, etc.).
 * 
 * @return 0 if successful
 */
uint32_t VdInitializeEngines(void);

/**
 * VdShutdownEngines @ 0x8258647C | ordinal: varies
 * 
 * Shuts down GPU engines.
 * 
 * @return 0 if successful
 */
uint32_t VdShutdownEngines(void);

/**
 * VdPersistDisplay @ 0x8258636C | ordinal: varies
 * 
 * Persists the current display state.
 * Used during suspend/resume.
 * 
 * @return 0 if successful
 */
uint32_t VdPersistDisplay(void);

//=============================================================================
// Runtime Library Functions (Used by Audio Callbacks)
//=============================================================================

/**
 * RtlCaptureContext @ 0x8258628C | ordinal: varies
 * 
 * Captures the current thread context (registers, stack).
 * Used for exception handling in audio callbacks.
 * 
 * @param pContext Pointer to CONTEXT structure
 */
void RtlCaptureContext(void* pContext);

/**
 * RtlUnwind @ 0x8258629C | ordinal: varies
 * 
 * Unwinds the stack during exception handling.
 * Used when audio callbacks throw exceptions.
 * 
 * @param TargetFrame  Target frame pointer
 * @param TargetIp     Target instruction pointer
 * @param ExceptionRecord Exception record
 * @param ReturnValue  Return value
 */
void RtlUnwind(void* TargetFrame, void* TargetIp, void* ExceptionRecord, void* ReturnValue);

//=============================================================================
// Thread Creation (Audio Processing Threads)
//=============================================================================

/**
 * ExCreateThread @ 0x825864FC | ordinal: varies
 * 
 * Creates a new thread.
 * Used for audio processing threads, streaming threads, etc.
 * 
 * @param pHandle         Pointer to receive thread handle
 * @param dwStackSize     Stack size in bytes
 * @param pThreadId       Pointer to receive thread ID
 * @param pStartAddress   Thread start address
 * @param pParameter      Parameter to pass to thread
 * @param dwCreationFlags Creation flags
 * @return STATUS_SUCCESS if successful
 */
uint32_t ExCreateThread(void** pHandle, uint32_t dwStackSize, uint32_t* pThreadId,
                        void* pStartAddress, void* pParameter, uint32_t dwCreationFlags);

#ifdef __cplusplus
}
#endif
