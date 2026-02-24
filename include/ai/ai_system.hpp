/*
 * Xbox 360 AI System Header
 * Professional AI engine organization
 */

#ifndef AI_SYSTEM_HPP
#define AI_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// AI constants - organized by category
namespace AIConstants {
    // AI difficulty levels
    constexpr int DIFFICULTY_EASY = 0;
    constexpr int DIFFICULTY_MEDIUM = 1;
    constexpr int DIFFICULTY_HARD = 2;
    constexpr int DIFFICULTY_EXPERT = 3;
    
    // AI states
    constexpr int AI_STATE_IDLE = 0;
    constexpr int AI_STATE_THINKING = 1;
    constexpr int AI_STATE_MOVING = 2;
    constexpr int AI_STATE_HITTING = 3;
    constexpr int AI_STATE_SERVING = 4;
    
    // AI behavior types
    constexpr int BEHAVIOR_AGGRESSIVE = 0;
    constexpr int BEHAVIOR_DEFENSIVE = 1;
    constexpr int BEHAVIOR_BALANCED = 2;
    constexpr int BEHAVIOR_ADAPTIVE = 3;
    
    // AI reaction times (in frames)
    constexpr int REACTION_TIME_EASY = 30;      // 0.5 seconds
    constexpr int REACTION_TIME_MEDIUM = 20;   // 0.33 seconds
    constexpr int REACTION_TIME_HARD = 10;     // 0.17 seconds
    constexpr int REACTION_TIME_EXPERT = 5;    // 0.08 seconds
    
    // AI prediction
    constexpr int PREDICTION_FRAMES = 60;      // 1 second prediction
    constexpr float PREDICTION_ACCURACY_EASY = 0.3f;
    constexpr float PREDICTION_ACCURACY_MEDIUM = 0.6f;
    constexpr float PREDICTION_ACCURACY_HARD = 0.8f;
    constexpr float PREDICTION_ACCURACY_EXPERT = 0.95f;
    
    // AI decision making
    constexpr int DECISION_TREE_DEPTH = 4;
    constexpr int MAX_DECISIONS_PER_FRAME = 10;
    constexpr float DECISION_CONFIDENCE_THRESHOLD = 0.7f;
    
    // Thread management
    constexpr int AI_THREAD_STACK_SIZE = 64 * 1024;  // 64KB
    constexpr int AI_THREAD_PRIORITY = 2;
    constexpr int AI_UPDATE_FREQUENCY = 60;  // 60 FPS
}

// AI opponent structure
struct AIOpponent {
    bool active;
    int difficulty;
    int behavior_type;
    int current_state;
    
    // AI decision making
    struct {
        int reaction_time;
        float prediction_accuracy;
        float decision_confidence;
        int frames_until_decision;
        bool is_thinking;
    } decision;
    
    // AI movement
    struct {
        float target_position;
        float current_position;
        float movement_speed;
        bool is_moving;
        int frames_until_move;
    } movement;
    
    // AI hitting
    struct {
        float hit_power;
        float hit_angle;
        float spin_amount;
        bool should_hit;
        int frames_until_hit;
    } hitting;
    
    // AI strategy
    struct {
        float aggression_level;
        float defensive_level;
        float adaptation_rate;
        int consecutive_hits;
        int consecutive_misses;
        float performance_score;
    } strategy;
    
    // AI learning
    struct {
        bool adaptive_learning;
        float player_pattern_recognition[8];
        float hit_success_rate;
        float serve_success_rate;
        int total_hits;
        int total_misses;
    } learning;
};

// AI thread context structure
struct AIThreadContext {
    bool initialized;
    void* thread_handle;
    uint32_t thread_id;
    int priority;
    uint32_t stack_size;
    void* stack_memory;
    
    // Thread synchronization
    struct {
        bool should_run;
        bool is_running;
        uint32_t frame_count;
        uint32_t last_update_time;
    } sync;
    
    // Thread data
    struct {
        AIOpponent* opponent;
        void* game_state;
        uint32_t update_frequency;
    } data;
};

// AI manager structure
struct AIManager {
    bool initialized;
    bool app_started;
    AIThreadContext thread_context;
    AIOpponent opponent;
    
    // AI processing
    struct {
        uint32_t frame_count;
        uint32_t last_update_time;
        float average_thinking_time;
        int decisions_per_frame;
    } processing;
    
    // AI vtable
    void* ai_vtable;
    
    // AI configuration
    struct {
        int default_difficulty;
        bool enable_adaptive_learning;
        bool enable_threading;
        float ai_update_rate;
    } config;
};

// AI decision structure
struct AIDecision {
    int type;
    float confidence;
    float priority;
    uint32_t execution_time;
    void* action_data;
    
    // Decision parameters
    struct {
        float target_x;
        float target_y;
        float power;
        float angle;
        float spin;
    } parameters;
};

// AI interface - clean API design
namespace AI {
    // Core AI functions
    int initialize_system();
    int shutdown_system();
    int start_ai();
    int stop_ai();
    
    // RAGE AI functions
    int main_ai();
    int run_ai();
    int setup_ai_args();
    bool is_app_started();
    int bind_thread_context();
    
    // Thread management
    int init_main_thread();
    int crt_main_entry();
    int allocate_thread_context();
    
    // Game AI functions
    int init_game_ai();
    int update_game_ai();
    int process_game_ai();
    
    // AI decision making
    int make_decision(AIOpponent* opponent, AIDecision* decision);
    int evaluate_decision(const AIDecision* decision);
    int execute_decision(const AIDecision* decision);
    
    // AI behavior
    int update_ai_behavior(AIOpponent* opponent);
    int set_difficulty(AIOpponent* opponent, int difficulty);
    int set_behavior_type(AIOpponent* opponent, int behavior_type);
    
    // AI prediction
    int predict_ball_position(AIOpponent* opponent, float* x, float* y);
    int predict_player_action(AIOpponent* opponent, int* action);
    int update_prediction_accuracy(AIOpponent* opponent, bool successful);
    
    // AI learning
    int learn_from_player(AIOpponent* opponent);
    int adapt_strategy(AIOpponent* opponent);
    int update_performance_metrics(AIOpponent* opponent, bool successful);
    
    // AI processing
    int process_ai_frame();
    int update_ai_state();
    int validate_ai_state();
    
    // Performance monitoring
    int get_ai_fps();
    float get_average_thinking_time();
    int get_decision_count();
    float get_ai_performance_score();
}

// Global AI state
extern AIManager g_ai_manager;

#endif // AI_SYSTEM_HPP
