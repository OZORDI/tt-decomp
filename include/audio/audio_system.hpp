/*
 * Xbox 360 Audio System Header
 * Professional audio engine organization
 */

#ifndef AUDIO_SYSTEM_HPP
#define AUDIO_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Audio constants - organized by category
namespace AudioConstants {
    // Audio format
    constexpr int SAMPLE_RATE = 48000;
    constexpr int CHANNELS = 2;
    constexpr int BITS_PER_SAMPLE = 16;
    constexpr int BUFFER_SIZE = 1024;
    
    // Voice categories
    constexpr int VOICE_CATEGORY_MASTER = 0;
    constexpr int VOICE_CATEGORY_MUSIC = 1;
    constexpr int VOICE_CATEGORY_SFX = 2;
    constexpr int VOICE_CATEGORY_VOICE = 3;
    
    // XMA compression
    constexpr int XMA_CHANNELS = 2;
    constexpr int XMA_SAMPLE_RATE = 48000;
    constexpr int XMA_BLOCK_SIZE = 2048;
    
    // Voice chat
    constexpr int VOICE_CHAT_MAX_PLAYERS = 4;
    constexpr int VOICE_CHAT_SAMPLE_RATE = 16000;
    constexpr int VOICE_CHAT_CHANNELS = 1;
}

// Audio device structure
struct AudioDevice {
    bool initialized;
    int sample_rate;
    int channels;
    int bits_per_sample;
    void* context;  // SDL_mixer context
    uint32_t driver_id;
    
    // Audio buffers
    struct {
        uint8_t* buffer;
        int size;
        int write_index;
        int read_index;
    } audio_buffer;
};

// Audio configuration structure
struct AudioConfig {
    float master_volume;
    float music_volume;
    float sfx_volume;
    float voice_volume;
    bool voice_chat_enabled;
    int speaker_config;
    uint32_t volume_change_mask;
};

// Voice chat structure
struct VoiceChatSession {
    bool active;
    int local_player_id;
    int remote_player_ids[AudioConstants::VOICE_CHAT_MAX_PLAYERS];
    void* voice_context;
    
    // Voice data
    struct {
        uint8_t* input_buffer;
        uint8_t* output_buffer;
        int buffer_size;
    } voice_buffers;
};

// XMA decompression context
struct XMAContext {
    bool initialized;
    void* decompressor;
    int channels;
    int sample_rate;
    uint8_t* compressed_data;
    uint8_t* decompressed_data;
    int compressed_size;
    int decompressed_size;
};

// Audio interface - clean API design
namespace Audio {
    // Core audio functions
    int initialize_system();
    int shutdown_system();
    int start_audio();
    int stop_audio();
    int configure_audio(const AudioConfig* config);
    
    // XAudio functions
    int register_render_driver_client();
    int unregister_render_driver_client();
    int submit_render_driver_frame(const void* audio_data, int size);
    float get_voice_category_volume(int category);
    uint32_t get_voice_category_volume_change_mask();
    int get_speaker_config();
    
    // XMA compression
    int create_xma_context(XMAContext* context);
    int release_xma_context(XMAContext* context);
    int decompress_xma_frame(const XMAContext* context, const uint8_t* input, int input_size, uint8_t* output, int output_size);
    
    // Voice chat
    int create_voice_chat_session(VoiceChatSession* session);
    int close_voice_chat_session(VoiceChatSession* session);
    int send_voice_data(const VoiceChatSession* session, const uint8_t* data, int size);
    int receive_voice_data(const VoiceChatSession* session, uint8_t* data, int max_size);
    
    // Audio processing
    int process_audio_frame();
    int update_volumes();
    int validate_audio_state();
    
    // Performance monitoring
    int get_audio_latency();
    float get_cpu_usage();
    int get_buffer_underruns();
}

// Global audio state
extern AudioDevice g_audio_device;
extern AudioConfig g_audio_config;
extern VoiceChatSession g_voice_chat;

#endif // AUDIO_SYSTEM_HPP
