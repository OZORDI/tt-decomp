#pragma once

// Physics System
// General physics simulation

#ifdef __cplusplus
extern "C" {
#endif

// Physics system functions
void physics_system_init(void);
void physics_system_shutdown(void);
void physics_system_update(float deltaTime);

#ifdef __cplusplus
}
#endif
