/*
 * Table Tennis Physics System Header
 * Professional game engine organization
 */

#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <cstdint>
#include "../game/game_logic.hpp"

// Physics constants - organized by category
namespace PhysicsConstants {
    // Fundamental physics
    constexpr float GRAVITY = -9.81f;
    constexpr float AIR_RESISTANCE = 0.001f;
    constexpr float TIME_STEP = 1.0f / 60.0f;
    
    // Collision physics
    constexpr float RESTITUTION = 0.85f;
    constexpr float FRICTION = 0.1f;
    constexpr float MAX_IMPULSE = 50.0f;
    constexpr float MIN_VELOCITY = 0.01f;
    constexpr float SEPARATION_DISTANCE = 0.001f;
    
    // Ball physics
    constexpr float SPIN_FACTOR = 0.1f;
    constexpr float MAGNUS_FACTOR = 0.01f;
    constexpr float TABLE_BOUNCE_DAMPING = 0.85f;
    constexpr float MAX_VELOCITY = 15.0f;
    
    // Table physics
    constexpr float TABLE_FRICTION = 0.1f;
    constexpr float NET_HEIGHT = 0.1525f;
}

// Collision data structure
struct CollisionData {
    bool occurred;
    float normalX, normalY, normalZ;
    float penetration;
    float relativeVelocity;
    float impulse;
    int paddleIndex;
};

// Physics interface - clean API design
namespace Physics {
    // Core physics functions
    int initialize();
    int shutdown();
    int update(float deltaTime);
    
    // Ball physics
    int update_ball_physics();
    int apply_gravity();
    int apply_air_resistance();
    int apply_spin_effects();
    
    // Collision detection
    int detect_collisions();
    int detect_ball_paddle_collision(const BallState& ball, const PaddleState& paddle, CollisionData& collision);
    int detect_table_collision(CollisionData& collision);
    
    // Collision response
    int process_collisions();
    int calculate_collision_response(CollisionData& collision);
    int apply_impulse(BallState& ball, const CollisionData& collision);
    
    // Velocity management
    int limit_ball_velocity();
    int validate_physics_state();
    
    // Performance monitoring
    int get_collision_count();
    float get_average_physics_time();
}

// Global collision system
extern CollisionData g_collisions[8];
extern int g_collisionCount;

#endif // PHYSICS_SYSTEM_HPP
