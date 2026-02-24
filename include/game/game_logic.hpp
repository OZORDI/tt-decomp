/*
 * Table Tennis Game Logic Header
 * Organized like professional game developers would structure it
 */

#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <cstdint>

// Game state structures - properly organized
struct GameState {
    bool isPlaying;
    bool isPaused;
    bool isServing;
    int currentServer;
    int currentSet;
    bool gameOver;
    uint32_t frameCount;
    float gameTime;
};

struct Score {
    int player1Score;
    int player2Score;
    int player1Sets;
    int player2Sets;
};

struct BallState {
    float positionX, positionY, positionZ;
    float velocityX, velocityY, velocityZ;
    float radius;
    bool inPlay;
};

struct PaddleState {
    float positionX, positionY, positionZ;
    float velocityX, velocityY, velocityZ;
    float width, height;
    bool isPlayer1;
};

// Game constants - organized by category
namespace GameConstants {
    // Table dimensions
    constexpr float TABLE_WIDTH = 2.74f;
    constexpr float TABLE_HEIGHT = 1.525f;
    constexpr float TABLE_HALF_WIDTH = TABLE_WIDTH / 2.0f;
    constexpr float TABLE_HALF_LENGTH = TABLE_HEIGHT / 2.0f;
    
    // Ball properties
    constexpr float BALL_RADIUS = 0.02f;
    constexpr float BALL_MASS = 0.0027f;
    constexpr float SERVE_HEIGHT = 0.15f;
    constexpr float SERVE_DISTANCE = 0.5f;
    
    // Game rules
    constexpr int WINNING_SCORE = 11;
    constexpr int WIN_MARGIN = 2;
    constexpr int MAX_SETS = 5;
    constexpr int POINTS_PER_GAME = WINNING_SCORE + (WIN_MARGIN - 1);
    
    // Timing
    constexpr float FRAME_TIME_60FPS = 1.0f / 60.0f;
    constexpr float MAX_VELOCITY = 15.0f;
    constexpr float MIN_VELOCITY = 0.01f;
}

// Game logic interface - clean API design
namespace GameLogic {
    // Core game functions
    int initialize_game();
    int main_game_loop();
    int shutdown_game();
    
    // Frame processing
    int process_frame();
    int update_game_state();
    int validate_game_state();
    
    // Game mechanics
    int process_mechanics();
    int apply_rules();
    int handle_scoring();
    
    // State management
    int check_end_game();
    int complete_game();
    
    // Serving logic
    int init_serve();
    int process_serving();
    int validate_serve();
}

// Event handling
namespace GameEvents {
    int handle_input_events();
    int handle_system_events();
    int handle_game_events();
}

// Global state access
extern GameState g_gameState;
extern Score g_score;
extern BallState g_ball;
extern PaddleState g_paddles[2];

#endif // GAME_LOGIC_HPP
