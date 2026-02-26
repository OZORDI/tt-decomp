/**
 * Xbox 360 Audio API Implementation - SDL2 Audio Backend
 * Rockstar Presents Table Tennis
 * 
 * Cross-platform implementation of Xbox 360 XAudio2 using SDL2 audio.
 * XMA decoder functions are stubbed (game likely uses pre-decoded audio).
 */

#include "xam/audio.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

// Audio state
static struct {
    uint8_t initialized;
    uint8_t registered;
    SDL_AudioDeviceID device_id;
    SDL_AudioSpec audio_spec;
    float category_volumes[4];  // Music, SFX, Voice, System
    uint32_t volume_change_mask;
} g_audio_state = {0};

// Audio callback for SDL2
static void AudioCallback(void* userdata, uint8_t* stream, int len) {
    (void)userdata;
    
    // Fill with silence (game will submit frames via XAudioSubmitRenderDriverFrame)
    memset(stream, 0, len);
}

//=============================================================================
// XAudio Configuration Functions
//=============================================================================

/**
 * XAudioGetSpeakerConfig @ 0x8258626C
 * 
 * Returns the speaker configuration based on SDL2 audio device.
 */
int32_t XAudioGetSpeakerConfig(uint32_t* pdwSpeakerConfig) {
    if (!pdwSpeakerConfig) {
        return E_INVALIDARG;
    }
    
    // Initialize SDL2 audio if needed
    if (!g_audio_state.initialized) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            fprintf(stderr, "XAudio: Failed to initialize SDL2 audio: %s\n", SDL_GetError());
            *pdwSpeakerConfig = SPEAKER_STEREO;  // Default to stereo
            return S_OK;
        }
        
        // Set up audio spec (44.1kHz, 16-bit stereo)
        SDL_AudioSpec desired;
        SDL_zero(desired);
        desired.freq = 44100;
        desired.format = AUDIO_S16SYS;
        desired.channels = 2;
        desired.samples = 1024;
        desired.callback = AudioCallback;
        
        g_audio_state.device_id = SDL_OpenAudioDevice(NULL, 0, &desired, &g_audio_state.audio_spec, 0);
        if (g_audio_state.device_id == 0) {
            fprintf(stderr, "XAudio: Failed to open audio device: %s\n", SDL_GetError());
            *pdwSpeakerConfig = SPEAKER_STEREO;
            return S_OK;
        }
        
        // Initialize category volumes to full
        for (int i = 0; i < 4; i++) {
            g_audio_state.category_volumes[i] = 1.0f;
        }
        g_audio_state.volume_change_mask = 0;
        
        g_audio_state.initialized = 1;
        printf("XAudio: Initialized - %d Hz, %d channels, %d samples\n",
               g_audio_state.audio_spec.freq,
               g_audio_state.audio_spec.channels,
               g_audio_state.audio_spec.samples);
    }
    
    // Return speaker config based on channel count
    switch (g_audio_state.audio_spec.channels) {
        case 1:
            *pdwSpeakerConfig = SPEAKER_FRONT_CENTER;
            break;
        case 2:
            *pdwSpeakerConfig = SPEAKER_STEREO;
            break;
        case 4:
            *pdwSpeakerConfig = SPEAKER_QUAD;
            break;
        case 6:
            *pdwSpeakerConfig = SPEAKER_5POINT1;
            break;
        case 8:
            *pdwSpeakerConfig = SPEAKER_7POINT1;
            break;
        default:
            *pdwSpeakerConfig = SPEAKER_STEREO;
            break;
    }
    
    return S_OK;
}

//=============================================================================
// XAudio Render Driver Functions
//=============================================================================

/**
 * XAudioRegisterRenderDriverClient @ 0x8258655C
 * 
 * Registers the audio render client and starts audio playback.
 */
int32_t XAudioRegisterRenderDriverClient(void* pUnknown1, void* pUnknown2) {
    (void)pUnknown1;
    (void)pUnknown2;
    
    if (!g_audio_state.initialized) {
        // Initialize audio if not already done
        uint32_t config;
        XAudioGetSpeakerConfig(&config);
    }
    
    if (g_audio_state.registered) {
        return S_OK;  // Already registered
    }
    
    // Start audio playback
    if (g_audio_state.device_id != 0) {
        SDL_PauseAudioDevice(g_audio_state.device_id, 0);  // Unpause
        printf("XAudio: Render driver client registered, audio started\n");
    }
    
    g_audio_state.registered = 1;
    return S_OK;
}

/**
 * XAudioUnregisterRenderDriverClient @ 0x8258656C
 * 
 * Unregisters the audio render client and stops audio playback.
 */
int32_t XAudioUnregisterRenderDriverClient(void* pUnknown) {
    (void)pUnknown;
    
    if (!g_audio_state.registered) {
        return S_OK;  // Not registered
    }
    
    // Stop audio playback
    if (g_audio_state.device_id != 0) {
        SDL_PauseAudioDevice(g_audio_state.device_id, 1);  // Pause
        printf("XAudio: Render driver client unregistered, audio stopped\n");
    }
    
    g_audio_state.registered = 0;
    return S_OK;
}

/**
 * XAudioSubmitRenderDriverFrame @ 0x8258657C
 * 
 * Submits an audio frame to the hardware.
 * On Xbox 360, this is called every frame (~60 FPS).
 * 
 * For cross-platform, we queue the audio data to SDL2.
 */
int32_t XAudioSubmitRenderDriverFrame(XAUDIO_RENDER_FRAME* pFrame) {
    if (!pFrame || !pFrame->pAudioData) {
        return E_INVALIDARG;
    }
    
    if (!g_audio_state.initialized || !g_audio_state.registered) {
        return E_FAIL;
    }
    
    // Calculate frame size in bytes
    uint32_t frame_size = pFrame->dwSampleCount * pFrame->dwChannels * sizeof(int16_t);
    
    // Queue audio data to SDL2
    if (SDL_QueueAudio(g_audio_state.device_id, pFrame->pAudioData, frame_size) < 0) {
        fprintf(stderr, "XAudio: Failed to queue audio: %s\n", SDL_GetError());
        return E_FAIL;
    }
    
    // Keep queue size reasonable (prevent audio lag)
    uint32_t queued = SDL_GetQueuedAudioSize(g_audio_state.device_id);
    uint32_t max_queue = frame_size * 3;  // Allow 3 frames of buffering
    if (queued > max_queue) {
        SDL_ClearQueuedAudio(g_audio_state.device_id);
    }
    
    return S_OK;
}

//=============================================================================
// XAudio Volume Functions
//=============================================================================

/**
 * XAudioGetVoiceCategoryVolume @ 0x825865AC
 * 
 * Gets the volume for a specific voice category.
 */
int32_t XAudioGetVoiceCategoryVolume(uint32_t dwCategory, float* pfVolume) {
    if (!pfVolume) {
        return E_INVALIDARG;
    }
    
    if (dwCategory >= 4) {
        return E_INVALIDARG;
    }
    
    *pfVolume = g_audio_state.category_volumes[dwCategory];
    return S_OK;
}

/**
 * XAudioGetVoiceCategoryVolumeChangeMask @ 0x825865BC
 * 
 * Gets a bitmask indicating which categories have changed volume.
 */
int32_t XAudioGetVoiceCategoryVolumeChangeMask(uint32_t* pdwMask) {
    if (!pdwMask) {
        return E_INVALIDARG;
    }
    
    *pdwMask = g_audio_state.volume_change_mask;
    
    // Clear mask after reading
    g_audio_state.volume_change_mask = 0;
    
    return S_OK;
}

//=============================================================================
// XMA (Xbox Media Audio) Decoder Functions
//=============================================================================

/**
 * XMACreateContext @ 0x825865DC
 * 
 * Creates an XMA hardware decoder context.
 * 
 * Stubbed - game likely uses pre-decoded audio or software decoder.
 */
int32_t XMACreateContext(XMACONTEXT* pContext) {
    if (!pContext) {
        return E_INVALIDARG;
    }
    
    // Return a fake context handle
    *pContext = (XMACONTEXT)0x12345678;
    
    printf("XAudio: XMACreateContext called (stubbed)\n");
    return S_OK;
}

/**
 * XMAReleaseContext @ 0x825865CC
 * 
 * Releases an XMA decoder context.
 * 
 * Stubbed - no actual resources to release.
 */
void XMAReleaseContext(XMACONTEXT pContext) {
    (void)pContext;
    printf("XAudio: XMAReleaseContext called (stubbed)\n");
}

/**
 * XMASetLoopData
 * 
 * Sets loop points for XMA stream playback.
 */
int32_t XMASetLoopData(XMACONTEXT pContext, void* pLoopData) {
    (void)pContext;
    (void)pLoopData;
    printf("XAudio: XMASetLoopData called (stubbed)\n");
    return S_OK;
}

/**
 * XMAGetOutputBufferReadOffset
 * 
 * Gets the current read position in the output buffer.
 */
uint32_t XMAGetOutputBufferReadOffset(XMACONTEXT pContext) {
    (void)pContext;
    return 0;  // Return 0 (start of buffer)
}

/**
 * XMASetOutputBufferReadOffset
 * 
 * Sets the read position in the output buffer (seek).
 */
int32_t XMASetOutputBufferReadOffset(XMACONTEXT pContext, uint32_t dwOffset) {
    (void)pContext;
    (void)dwOffset;
    printf("XAudio: XMASetOutputBufferReadOffset called with offset %u (stubbed)\n", dwOffset);
    return S_OK;
}

/**
 * XMAEnableContext
 * 
 * Enables or disables an XMA decoder context.
 */
int32_t XMAEnableContext(XMACONTEXT pContext, int32_t bEnable) {
    (void)pContext;
    printf("XAudio: XMAEnableContext called with enable=%d (stubbed)\n", bEnable);
    return S_OK;
}

/**
 * XMABlockWhileInUse
 * 
 * Blocks until the XMA decoder is available.
 */
int32_t XMABlockWhileInUse(XMACONTEXT pContext) {
    (void)pContext;
    // Return immediately (decoder always available in stub)
    return S_OK;
}
