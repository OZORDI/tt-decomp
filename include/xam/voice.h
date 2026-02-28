#pragma once

/**
 * Xbox 360 Voice Chat API - XamVoice Functions
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks for Xbox 360 voice chat functionality.
 * Used for in-game voice communication over Xbox Live.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Voice chat error codes
#define VOICE_E_NO_SLOTS_AVAILABLE  0x80004005  // E_FAIL - no voice slots available

// Voice chat flags
#define VOICE_FLAG_ENABLED          0x00000001

// Voice handle type
typedef void* XVOICE_HANDLE;

//=============================================================================
// Voice Chat Functions
//=============================================================================

/**
 * XamVoiceCreate @ 0x8258681C | ordinal: varies
 * 
 * Creates a voice chat context for a user.
 * 
 * @param dwUserIndex   User index (0-3 for local users)
 * @param dwFlags       Voice flags (usually 15 = all features enabled)
 * @param pVoiceHandle  Pointer to receive voice handle
 * @return S_OK if successful, error code otherwise
 */
int32_t XamVoiceCreate(uint32_t dwUserIndex, uint32_t dwFlags, XVOICE_HANDLE* pVoiceHandle);

/**
 * XamVoiceClose @ 0x825867EC | ordinal: varies
 * 
 * Closes a voice chat context.
 * 
 * @param hVoice Voice handle to close
 * @return S_OK if successful
 */
int32_t XamVoiceClose(XVOICE_HANDLE hVoice);

/**
 * XamVoiceSetMicrophoneState @ 0x8258682C | ordinal: varies
 * 
 * Enables or disables the microphone for a voice context.
 * 
 * @param hVoice   Voice handle
 * @param bEnabled TRUE to enable microphone, FALSE to disable
 * @return S_OK if successful
 */
int32_t XamVoiceSetMicrophoneState(XVOICE_HANDLE hVoice, uint32_t bEnabled);

/**
 * XamVoiceGetMicrophoneState @ 0x8258683C | ordinal: varies
 * 
 * Gets the current microphone state.
 * 
 * @param hVoice    Voice handle
 * @param pbEnabled Pointer to receive enabled state
 * @return S_OK if successful
 */
int32_t XamVoiceGetMicrophoneState(XVOICE_HANDLE hVoice, uint32_t* pbEnabled);

#ifdef __cplusplus
}
#endif
