/*
 * Xbox 360 Serialization System Header
 * Professional serialization engine organization
 */

#ifndef SERIALIZATION_SYSTEM_HPP
#define SERIALIZATION_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Serialization constants - organized by category
namespace SerializationConstants {
    // File operations
    constexpr int FILE_MODE_READ = 0x01;
    constexpr int FILE_MODE_WRITE = 0x02;
    constexpr int FILE_MODE_APPEND = 0x04;
    constexpr int FILE_MODE_CREATE = 0x08;
    
    // Content types
    constexpr int CONTENT_TYPE_SAVE_GAME = 1;
    constexpr int CONTENT_TYPE_PROFILE = 2;
    constexpr int CONTENT_TYPE_SETTINGS = 3;
    constexpr int CONTENT_TYPE_THUMBNAIL = 4;
    
    // Serialization formats
    constexpr int FORMAT_BINARY = 1;
    constexpr int FORMAT_TEXT = 2;
    constexpr int FORMAT_JSON = 3;
    constexpr int FORMAT_XML = 4;
    
    // Save game versions
    constexpr int SAVE_VERSION_CURRENT = 1;
    constexpr int SAVE_VERSION_COMPATIBLE = 1;
    
    // File limits
    constexpr int MAX_FILENAME_LENGTH = 256;
    constexpr int MAX_FILEPATH_LENGTH = 512;
    constexpr int MAX_CONTENT_SIZE = 16 * 1024 * 1024;  // 16MB
    constexpr int MAX_THUMBNAIL_SIZE = 64 * 1024;        // 64KB
    
    // Profile settings
    constexpr int MAX_PROFILE_NAME_LENGTH = 64;
    constexpr int MAX_SETTINGS_ENTRIES = 128;
}

// Save game structure
struct SaveGame {
    uint32_t version;
    uint32_t timestamp;
    char player_name[SerializationConstants::MAX_PROFILE_NAME_LENGTH];
    
    // Game state
    struct {
        int player1_score;
        int player2_score;
        int current_set;
        int serving_player;
        bool is_paused;
        bool is_game_over;
    } game_state;
    
    // Ball state
    struct {
        float position_x;
        float position_y;
        float velocity_x;
        float velocity_y;
        float spin;
    } ball_state;
    
    // Paddle states
    struct {
        float position_x;
        float position_y;
        float velocity_x;
        float velocity_y;
    } paddle_states[2];
    
    // AI settings
    struct {
        int difficulty;
        int behavior_type;
        float performance_score;
        bool adaptive_learning;
    } ai_settings;
    
    // Game statistics
    struct {
        int total_rallies;
        int longest_rally;
        int total_hits;
        int total_misses;
        float play_time;
    } statistics;
    
    // Metadata
    struct {
        char level_name[64];
        int difficulty_level;
        bool multiplayer;
        uint32_t checksum;
    } metadata;
};

// User profile structure
struct UserProfile {
    char username[SerializationConstants::MAX_PROFILE_NAME_LENGTH];
    uint64_t user_id;
    
    // Profile settings
    struct {
        float master_volume;
        float music_volume;
        float sfx_volume;
        float voice_volume;
        bool vibration_enabled;
        bool subtitles_enabled;
        int language;
        int control_scheme;
    } settings;
    
    // Game progress
    struct {
        int total_games_played;
        int total_games_won;
        int total_games_lost;
        float win_percentage;
        int longest_winning_streak;
        int current_streak;
        uint64_t total_play_time;
    } progress;
    
    // Achievements
    struct {
        uint32_t achievements_unlocked;
        uint32_t total_achievements;
        uint32_t gamerscore;
        uint32_t total_gamerscore;
    } achievements;
    
    // Preferences
    struct {
        bool auto_save;
        bool show_tutorials;
        bool enable_ai_learning;
        float difficulty_preference;
        int favorite_character;
    } preferences;
};

// Content file structure
struct ContentFile {
    bool open;
    int mode;
    char filename[SerializationConstants::MAX_FILENAME_LENGTH];
    char filepath[SerializationConstants::MAX_FILEPATH_LENGTH];
    int content_type;
    uint32_t file_size;
    void* file_data;
    
    // File handle (in real implementation, would be FILE* or HANDLE)
    void* file_handle;
    
    // Thumbnail data
    struct {
        bool has_thumbnail;
        uint32_t thumbnail_size;
        uint8_t* thumbnail_data;
        int thumbnail_width;
        int thumbnail_height;
    } thumbnail;
    
    // Metadata
    struct {
        uint32_t creation_time;
        uint32_t modification_time;
        uint32_t access_time;
        char creator[64];
        char description[256];
    } metadata;
};

// Content enumerator structure
struct ContentEnumerator {
    bool initialized;
    int content_type;
    char search_path[SerializationConstants::MAX_FILEPATH_LENGTH];
    char search_pattern[64];
    
    // Enumeration results
    struct {
        ContentFile files[64];
        int file_count;
        int current_index;
    } results;
    
    // Filter criteria
    struct {
        uint32_t min_size;
        uint32_t max_size;
        uint32_t min_date;
        uint32_t max_date;
        char creator_filter[64];
    } filter;
};

// Serialization manager structure
struct SerializationManager {
    bool initialized;
    
    // File operations
    struct {
        ContentFile files[16];
        int open_files;
        int max_open_files;
    } file_system;
    
    // Content management
    struct {
        ContentEnumerator enumerator;
        int total_files;
        uint32_t total_size;
    } content;
    
    // Profile management
    struct {
        UserProfile current_profile;
        bool profile_loaded;
        char profile_path[SerializationConstants::MAX_FILEPATH_LENGTH];
    } profile;
    
    // Save game management
    struct {
        SaveGame current_save;
        bool save_loaded;
        char save_path[SerializationConstants::MAX_FILEPATH_LENGTH];
        bool auto_save_enabled;
        uint32_t last_auto_save;
    } save_game;
    
    // Serialization settings
    struct {
        int default_format;
        bool compression_enabled;
        bool encryption_enabled;
        uint32_t checksum_salt;
    } settings;
};

// Serialization interface - clean API design
namespace Serialization {
    // Core serialization functions
    int initialize_system();
    int shutdown_system();
    int start_serialization();
    int stop_serialization();
    
    // XAM content functions
    int create_content(const char* filename, int content_type);
    int delete_content(const char* filename);
    int close_content(ContentFile* file);
    int set_content_thumbnail(ContentFile* file, const uint8_t* thumbnail_data, int width, int height);
    int create_content_enumerator(int content_type, const char* search_path);
    int enumerate_content(ContentEnumerator* enumerator);
    
    // XAM user functions
    int write_profile_settings(const UserProfile* profile);
    int read_profile_settings(UserProfile* profile);
    
    // XAM gamer functions
    int write_gamer_tile(const char* filename, const uint8_t* tile_data, int size);
    
    // CRT serialization functions
    size_t serialize_string(const char* str, char* buffer, size_t buffer_size);
    int serialize_printf(char* buffer, size_t buffer_size, const char* format, ...);
    
    // Game serialization functions
    int save_game_state(const char* filename);
    int load_game_state(const char* filename);
    int serialize_game_data(SaveGame* save_data);
    
    // File operations
    ContentFile* open_file(const char* filename, int mode);
    int close_file(ContentFile* file);
    int read_file(ContentFile* file, void* buffer, uint32_t size);
    int write_file(ContentFile* file, const void* data, uint32_t size);
    int seek_file(ContentFile* file, uint32_t offset);
    uint32_t get_file_size(ContentFile* file);
    
    // Serialization operations
    int serialize_data(const void* data, uint32_t size, char* buffer, uint32_t buffer_size);
    int deserialize_data(const char* buffer, uint32_t buffer_size, void* data, uint32_t size);
    uint32_t calculate_checksum(const void* data, uint32_t size);
    
    // Profile operations
    int create_profile(const char* username);
    int load_profile(const char* profile_path);
    int save_profile(const char* profile_path);
    int delete_profile(const char* profile_path);
    
    // Auto-save operations
    int enable_auto_save(bool enabled);
    int perform_auto_save();
    int set_auto_save_interval(uint32_t interval_seconds);
    
    // Validation and recovery
    int validate_save_file(const char* filename);
    int repair_save_file(const char* filename);
    int backup_save_file(const char* filename);
    
    // Performance monitoring
    int get_save_count();
    int get_load_count();
    float get_average_save_time();
    float get_average_load_time();
    uint32_t get_total_data_written();
    uint32_t get_total_data_read();
}

// Global serialization state
extern SerializationManager g_serialization_manager;

#endif // SERIALIZATION_SYSTEM_HPP
