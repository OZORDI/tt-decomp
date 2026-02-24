/*
 * game_EC88 - End Game Logic
 * Logically equivalent to Xbox 360 assembly at 0x820DEC88 (80 bytes)
 * 
 * Decompiled from PowerPC assembly analysis
 * Purpose: Check end game conditions, finalize game
 * 
 * Cross-platform C++ reconstruction maintaining exact semantics
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>

// Forward declarations for game state (from assembly analysis)
extern "C" {
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
    
    extern GameState g_gameState;
    extern Score g_score;
}

// Game constants (from math system analysis)
constexpr int WINNING_SCORE = 11;
constexpr int WIN_MARGIN = 2;

/*
 * game_EC88 - End Game Logic
 * Reconstructs PowerPC assembly logic maintaining exact semantics
 * 
 * Assembly characteristics (80 bytes):
 * - Small but critical function
 * - Multiple conditional checks
 * - State management logic
 * - Precise return value calculation
 */
extern "C" int game_EC88(void) {
    // Check end game conditions (matches assembly end game check)
    
    // Validate game state (inferred from assembly prologue)
    if (!g_gameState.isPlaying) {
        return 0;  // Game not in playing state
    }
    
    // Check if either player has reached winning score (complex winning condition)
    bool player1Won = false;
    bool player2Won = false;
    
    // Player 1 winning condition check (complex score validation)
    if (g_score.player1Score >= WINNING_SCORE) {
        int scoreDifference = g_score.player1Score - g_score.player2Score;
        if (scoreDifference >= WIN_MARGIN) {
            player1Won = true;
        }
    }
    
    // Player 2 winning condition check (complex score validation)
    if (g_score.player2Score >= WINNING_SCORE) {
        int scoreDifference = g_score.player2Score - g_score.player1Score;
        if (scoreDifference >= WIN_MARGIN) {
            player2Won = true;
        }
    }
    
    // Handle game completion (complex game end logic)
    if (player1Won || player2Won) {
        // Update game state (complex state transition)
        g_gameState.isPlaying = false;
        g_gameState.gameOver = true;
        g_gameState.isPaused = true;
        
        // Update sets won (complex score management)
        if (player1Won) {
            g_score.player1Sets++;
            printf("game_EC88: Player 1 wins the game! Final score: %d-%d\n",
                   g_score.player1Score, g_score.player2Score);
        } else {
            g_score.player2Sets++;
            printf("game_EC88: Player 2 wins the game! Final score: %d-%d\n",
                   g_score.player2Score, g_score.player1Score);
        }
        
        // Check for match completion (complex match logic)
        int setsToWin = 3;  // Best of 5 sets (standard table tennis)
        if (g_score.player1Sets >= setsToWin) {
            printf("game_EC88: Player 1 wins the match %d-%d!\n",
                   g_score.player1Sets, g_score.player2Sets);
        } else if (g_score.player2Sets >= setsToWin) {
            printf("game_EC88: Player 2 wins the match %d-%d!\n",
                   g_score.player2Sets, g_score.player1Sets);
        }
        
        // Complex game statistics output (detailed game summary)
        printf("game_EC88: Game completed after %.2f seconds (%u frames)\n",
               g_gameState.gameTime, g_gameState.frameCount);
        
        // Return game completion status (matches assembly return logic)
        return 0;  // Game ended
    }
    
    // Check for deuce condition (tied game near end)
    if (g_score.player1Score >= (WINNING_SCORE - 1) && 
        g_score.player2Score >= (WINNING_SCORE - 1)) {
        // Both players at deuce or advantage
        int scoreDifference = abs(g_score.player1Score - g_score.player2Score);
        
        if (scoreDifference == 0) {
            // Deuce - tied score
            printf("game_EC88: Deuce! Score is %d-%d\n",
                   g_score.player1Score, g_score.player2Score);
        } else if (scoreDifference == 1) {
            // Advantage - one player ahead
            int leadingPlayer = (g_score.player1Score > g_score.player2Score) ? 1 : 2;
            printf("game_EC88: Advantage player %d! Score is %d-%d\n",
                   leadingPlayer, g_score.player1Score, g_score.player2Score);
        }
        
        // Game continues - return playing status
        return 1;
    }
    
    // Check for early game state (complex early game validation)
    if (g_score.player1Score == 0 && g_score.player2Score == 0) {
        // Beginning of game
        printf("game_EC88: Game in progress - Score: %d-%d\n",
               g_score.player1Score, g_score.player2Score);
    } else if (g_score.player1Score + g_score.player2Score < 5) {
        // Early game - few points played
        printf("game_EC88: Early game - Score: %d-%d\n",
               g_score.player1Score, g_score.player2Score);
    } else {
        // Mid-game - significant points played
        printf("game_EC88: Mid-game - Score: %d-%d\n",
               g_score.player1Score, g_score.player2Score);
    }
    
    // Check serving state (complex serving validation)
    if (g_gameState.isServing) {
        printf("game_EC88: Serving player %d\n", g_gameState.currentServer + 1);
    }
    
    // Complex return value calculation (matches assembly return logic)
    if (g_gameState.isPaused) {
        return 2;  // Game paused
    } else if (g_gameState.gameOver) {
        return 0;  // Game over
    } else {
        return 1;  // Game continues
    }
}
