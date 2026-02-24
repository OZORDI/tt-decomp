#pragma once

// Graphics System
// High-level graphics management

#ifdef __cplusplus
extern "C" {
#endif

// Graphics system functions
void graphics_system_init(void);
void graphics_system_shutdown(void);
void graphics_system_render(void);

#ifdef __cplusplus
}
#endif
