#pragma once

/**
 * Xbox 360 Audio API - XAudio2 and XMA Functions
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks for Xbox 360 audio hardware.
 * XAudio2 is the high-level audio API, XMA is the hardware decoder.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// HRESULT codes
#define S_OK                     0x00000000
#define E_FAIL                   0x80004005
#define E_INVALIDARG             0x80070057
#define E_NOTIMPL                0x80004001

// Speaker configuration flags (SPEAKER_* from XAudio2)
#define SPEAKER_FRONT_LEFT              0x00000001
#define SPEAKER_FRONT_RIGHT             0x00000002
#define SPEAKER_FRONT_CENTER            0x00000004
#define SPEAKER_LOW_FREQUENCY           0x00000008
#define SPEAKER_BACK_LEFT               0x00000010
#define SPEAKER_BACK_RIGHT              0x00000020
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x00000040
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x00000080
#define SPEAKER_BACK_CENTER             0x00000100
#define SPEAKER_SIDE_LEFT               0x00000200
#define SPEAKER_SIDE_RIGHT              0x00000400

// Common speaker configurations
#define SPEAKER_STEREO              (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
#define SPEAKER_QUAD                (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | \
                                     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define SPEAKER_5POINT1             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | \
                                     SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | \
                                     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define SPEAKER_7POINT1             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | \
                                     SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | \
                                     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | \
                                     SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT)

// Voice categories (for volume control)
typedef enum _XAUDIO_VOICE_CATEGORY {
    XAUDIO_VOICE_MUSIC = 0,
    XAUDIO_VOICE_SFX = 1,
    XAUDIO_VOICE_VOICE = 2,
    XAUDIO_VOICE_SYSTEM = 3
} XAUDIO_VOICE_CATEGORY;

// XMA context structure (opaque handle)
typedef void* XMACONTEXT;

// Audio frame structure
typedef struct _XAUDIO_RENDER_FRAME {
    void*    pAudioData;         // Pointer to audio samples
    uint32_t dwSampleCount;      // Number of samples
    uint32_t dwChannels;         // Number of channels
    uint32_t dwSampleRate;       // Sample rate (Hz)
} XAUDIO_RENDER_FRAME;

// XMA stream buffer structure
typedef struct _XMA_STREAM_BUFFER {
    void*    pAudioData;         // Compressed XMA data
    uint32_t dwAudioBytes;       // Size of compressed data
    uint32_t dwMaxSize;          // Maximum buffer size
    uint32_t dwPlayBegin;        // Start offset for playback
    uint32_t dwPlayLength;       // Length to play
    uint32_t dwLoopBegin;        // Loop start offset
    uint32_t dwLoopLength;       // Loop length
    uint32_t dwLoopCount;        // Number of loops (0 = infinite)
} XMA_STREAM_BUFFER;

//=============================================================================
// XAudio Configuration Functions
//=============================================================================

/**
 * XAudioGetSpeakerConfig @ 0x8258626C | ordinal: varies
 * 
 * Retrieves the current speaker configuration.
 * 
 * @param pdwSpeakerConfig Pointer to receive speaker config flags
 * @return S_OK if successful
 */
int32_t XAudioGetSpeakerConfig(uint32_t* pdwSpeakerConfig);

//=============================================================================
// XAudio Render Driver Functions
//=============================================================================

/**
 * XAudioRegisterRenderDriverClient @ 0x8258655C | ordinal: varies
 * 
 * Registers an audio render client with the hardware driver.
 * Must be called before submitting audio frames.
 * 
 * @param pUnknown1 Reserved parameter (usually NULL)
 * @param pUnknown2 Reserved parameter (usually NULL)
 * @return S_OK if successful
 */
int32_t XAudioRegisterRenderDriverClient(void* pUnknown1, void* pUnknown2);

/**
 * XAudioUnregisterRenderDriverClient @ 0x8258656C | ordinal: varies
 * 
 * Unregisters an audio render client.
 * Should be called during shutdown.
 * 
 * @param pUnknown Reserved parameter (usually NULL)
 * @return S_OK if successful
 */
int32_t XAudioUnregisterRenderDriverClient(void* pUnknown);

/**
 * XAudioSubmitRenderDriverFrame @ 0x8258657C | ordinal: varies
 * 
 * Submits an audio frame to the hardware for playback.
 * Called every frame (typically 60 FPS = ~735 samples at 44.1kHz).
 * 
 * @param pFrame Pointer to audio frame data
 * @return S_OK if successful
 */
int32_t XAudioSubmitRenderDriverFrame(XAUDIO_RENDER_FRAME* pFrame);

//=============================================================================
// XAudio Volume Functions
//=============================================================================

/**
 * XAudioGetVoiceCategoryVolume @ 0x825865AC | ordinal: varies
 * 
 * Gets the volume for a specific voice category.
 * 
 * @param dwCategory Voice category (XAUDIO_VOICE_CATEGORY)
 * @param pfVolume   Pointer to receive volume (0.0 = silent, 1.0 = full)
 * @return S_OK if successful
 */
int32_t XAudioGetVoiceCategoryVolume(uint32_t dwCategory, float* pfVolume);

/**
 * XAudioGetVoiceCategoryVolumeChangeMask @ 0x825865BC | ordinal: varies
 * 
 * Gets a bitmask indicating which voice categories have changed volume.
 * Used to detect when user adjusts volume in dashboard.
 * 
 * @param pdwMask Pointer to receive change mask
 * @return S_OK if successful
 */
int32_t XAudioGetVoiceCategoryVolumeChangeMask(uint32_t* pdwMask);

//=============================================================================
// XMA (Xbox Media Audio) Decoder Functions
//=============================================================================

/**
 * XMACreateContext @ 0x825865DC | ordinal: varies
 * 
 * Creates an XMA hardware decoder context.
 * XMA is the Xbox 360's compressed audio format (similar to AAC).
 * 
 * @param pContext Pointer to receive context handle
 * @return S_OK if successful
 */
int32_t XMACreateContext(XMACONTEXT* pContext);

/**
 * XMAReleaseContext @ 0x825865CC | ordinal: varies
 * 
 * Releases an XMA decoder context.
 * 
 * @param pContext Context handle to release
 */
void XMAReleaseContext(XMACONTEXT pContext);

/**
 * XMASetLoopData @ ordinal: varies
 * 
 * Sets loop points for XMA stream playback.
 * 
 * @param pContext     XMA context handle
 * @param pLoopData    Pointer to loop data structure
 * @return S_OK if successful
 */
int32_t XMASetLoopData(XMACONTEXT pContext, void* pLoopData);

/**
 * XMAGetOutputBufferReadOffset @ ordinal: varies
 * 
 * Gets the current read position in the output buffer.
 * 
 * @param pContext XMA context handle
 * @return Current read offset in bytes
 */
uint32_t XMAGetOutputBufferReadOffset(XMACONTEXT pContext);

/**
 * XMASetOutputBufferReadOffset @ ordinal: varies
 * 
 * Sets the read position in the output buffer (seek).
 * 
 * @param pContext XMA context handle
 * @param dwOffset New read offset in bytes
 * @return S_OK if successful
 */
int32_t XMASetOutputBufferReadOffset(XMACONTEXT pContext, uint32_t dwOffset);

/**
 * XMAEnableContext @ ordinal: varies
 * 
 * Enables or disables an XMA decoder context.
 * 
 * @param pContext XMA context handle
 * @param bEnable  TRUE to enable, FALSE to disable
 * @return S_OK if successful
 */
int32_t XMAEnableContext(XMACONTEXT pContext, int32_t bEnable);

/**
 * XMABlockWhileInUse @ ordinal: varies
 * 
 * Blocks until the XMA decoder is available.
 * 
 * @param pContext XMA context handle
 * @return S_OK if successful
 */
int32_t XMABlockWhileInUse(XMACONTEXT pContext);

#ifdef __cplusplus
}
#endif
