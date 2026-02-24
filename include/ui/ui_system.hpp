/*
 * Xbox 360 UI System Header
 * Professional UI engine organization
 */

#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// UI constants - organized by category
namespace UIConstants {
    // Message box types
    constexpr int MSGBOX_OK = 0x01;
    constexpr int MSGBOX_OK_CANCEL = 0x02;
    constexpr int MSGBOX_YES_NO = 0x04;
    constexpr int MSGBOX_YES_NO_CANCEL = 0x08;
    constexpr int MSGBOX_RETRY_CANCEL = 0x10;
    constexpr int MSGBOX_ABORT_RETRY_IGNORE = 0x20;
    
    // Message box results
    constexpr int MSGBOX_RESULT_OK = 1;
    constexpr int MSGBOX_RESULT_CANCEL = 2;
    constexpr int MSGBOX_RESULT_YES = 6;
    constexpr int MSGBOX_RESULT_NO = 7;
    constexpr int MSGBOX_RESULT_RETRY = 4;
    constexpr int MSGBOX_RESULT_ABORT = 3;
    constexpr int MSGBOX_RESULT_IGNORE = 5;
    
    // UI types
    constexpr int UI_TYPE_MESSAGEBOX = 1;
    constexpr int UI_TYPE_SIGNIN = 2;
    constexpr int UI_TYPE_FRIENDS = 3;
    constexpr int UI_TYPE_GAMERCARD = 4;
    constexpr int UI_TYPE_ACHIEVEMENTS = 5;
    constexpr int UI_TYPE_PLAYER_REVIEW = 6;
    constexpr int UI_TYPE_DEVICE_SELECTOR = 7;
    constexpr int UI_TYPE_DIRTY_DISC_ERROR = 8;
    
    // Languages
    constexpr int LANG_ENGLISH = 1;
    constexpr int LANG_JAPANESE = 2;
    constexpr int LANG_GERMAN = 3;
    constexpr int LANG_FRENCH = 4;
    constexpr int LANG_SPANISH = 5;
    constexpr int LANG_ITALIAN = 6;
    constexpr int LANG_KOREAN = 7;
    constexpr int LANG_CHINESE = 8;
    constexpr int LANG_PORTUGUESE = 9;
    
    // UI dimensions
    constexpr int UI_WIDTH = 1280;
    constexpr int UI_HEIGHT = 720;
    constexpr int UI_MARGIN = 20;
    constexpr int UI_BUTTON_HEIGHT = 40;
    constexpr int UI_BUTTON_WIDTH = 200;
    
    // Animation
    constexpr float UI_FADE_DURATION = 0.3f;
    constexpr float UI_SLIDE_DURATION = 0.2f;
}

// UI message box structure
struct UIMessageBox {
    bool active;
    int type;
    char title[256];
    char message[512];
    char button1[64];
    char button2[64];
    char button3[64];
    int result;
    void* callback;
    uint32_t callback_param;
    
    // Animation state
    struct {
        float fade_alpha;
        float slide_offset;
        bool animating;
        float animation_time;
    } animation;
};

// UI context structure
struct UIContext {
    bool initialized;
    int current_language;
    bool ui_visible;
    int active_ui_type;
    void* active_ui_data;
    
    // Screen dimensions
    int screen_width;
    int screen_height;
    
    // Font and rendering
    void* font;
    void* renderer;
    
    // UI stack
    struct {
        void* elements[16];
        int stack_top;
    } ui_stack;
    
    // Message box queue
    struct {
        UIMessageBox message_boxes[8];
        int queue_head;
        int queue_tail;
        int active_count;
    } message_queue;
};

// Gamer card structure
struct GamerCard {
    uint64_t xuid;
    char gamertag[32];
    int gamerscore;
    int reputation;
    char zone[32];
    char presence[256];
    bool online;
    bool in_game;
};

// Achievement structure
struct Achievement {
    uint32_t id;
    char name[128];
    char description[256];
    int gamerscore;
    bool unlocked;
    float progress;
    uint64_t unlock_time;
};

// UI interface - clean API design
namespace UI {
    // Core UI functions
    int initialize_system();
    int shutdown_system();
    int start_ui();
    int stop_ui();
    
    // Message box functions
    int show_message_box(const char* title, const char* message, int type, void* callback, uint32_t callback_param);
    int show_message_box_ex(const char* title, const char* message, const char* button1, const char* button2, const char* button3, int type);
    int hide_message_box();
    int get_message_box_result();
    
    // XAM UI functions
    int show_signin_ui();
    int show_friends_ui();
    int show_gamer_card_ui(uint64_t xuid);
    int show_achievements_ui();
    int show_player_review_ui(uint64_t xuid);
    int show_device_selector_ui();
    int show_dirty_disc_error_ui();
    
    // Language functions
    int get_system_language();
    int set_language(int language);
    const char* get_language_name(int language);
    
    // Game UI functions
    int show_score_ui();
    int show_menu_ui();
    int show_settings_ui();
    int show_pause_ui();
    int show_game_over_ui();
    
    // UI processing
    int process_ui_frame();
    int update_animations();
    int render_ui();
    
    // UI validation
    int validate_ui_state();
    int validate_message_box(const UIMessageBox* msgbox);
    
    // Performance monitoring
    int get_ui_fps();
    float get_average_render_time();
    int get_ui_memory_usage();
}

// Global UI state
extern UIContext g_ui_context;

#endif // UI_SYSTEM_HPP
