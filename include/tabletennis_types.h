/*
 * Table Tennis Shared Type Definitions
 *
 * Single source of truth for all game-logic and physics structs.
 * Include this header instead of re-defining these structs in .cpp files.
 *
 * Matches the memory layout inferred from assembly analysis of:
 *   xbox360_table_tennis_logic.md
 */

#ifndef TABLETENNIS_TYPES_H
#define TABLETENNIS_TYPES_H

#include <cstdint>
#include <cstdbool>

/* ---------------------------------------------------------------------------
 * Physics objects
 * ------------------------------------------------------------------------- */

struct BallState {
    float positionX, positionY, positionZ;  // World-space position
    float velocityX, velocityY, velocityZ;  // Velocity in m/s
    float radius;                            // Standard: 0.02 m (20 mm ping-pong ball)
    bool  inPlay;                            // False while serving / out of bounds
};

struct PaddleState {
    float positionX, positionY, positionZ;
    float velocityX, velocityY, velocityZ;
    float width, height;                     // Paddle face dimensions (m)
    bool  isPlayer1;                         // True = near end, False = far end
};

/* ---------------------------------------------------------------------------
 * Game-logic objects
 * ------------------------------------------------------------------------- */

struct GameConfig {
    float tableWidth;        // Standard: 2.74 m
    float tableHeight;       // Standard: 1.525 m (half-length)
    float ballRadius;        // Standard: 0.02 m
    float paddleSpeed;       // Max paddle movement speed (m/s)
    float ballSpeed;         // Initial serve speed (m/s)
    int   maxScore;          // Points to win a game (11)
    bool  enablePhysics;
    float physicsTimeStep;   // Fixed step: 1/60 s
};

struct GameState {
    bool     isPlaying;
    bool     isPaused;
    bool     isServing;
    int      currentServer;          // 0 = Player 1, 1 = Player 2
    int      currentSet;
    bool     gameOver;
    uint32_t frameCount;
    float    gameTime;
    int      lastKnownTotalPoints;   // Tracks point count for serve-rotation (game_66E8)
};

struct Score {
    int player1Score;
    int player2Score;
    int player1Sets;
    int player2Sets;
};

#endif /* TABLETENNIS_TYPES_H */
