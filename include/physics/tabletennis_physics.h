#pragma once

// Table Tennis Physics System
// Ball trajectory, spin, collision detection

#ifdef __cplusplus
extern "C" {
#endif

// Physics structures
typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 spin;
    float mass;
    float radius;
} Ball;

// Physics functions
void physics_ball_init(Ball* ball);
void physics_ball_update(Ball* ball, float deltaTime);
void physics_ball_apply_force(Ball* ball, const Vector3* force);
int physics_ball_check_collision(const Ball* ball, const Vector3* point);

#ifdef __cplusplus
}
#endif
