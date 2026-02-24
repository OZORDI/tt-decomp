#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rage_render_default
 * @ 0x822C08D8 | size: 0x240
 * 
 * Main rendering function for default path (menus, loading screens).
 * Called every frame when no active scene is present.
 */
void rage_render_default(void);

/**
 * rage_render_scene
 * @ 0x822C0B18 | size: 0x130
 * 
 * Render scene with game objects.
 * Called when an active scene is present.
 */
void rage_render_scene(void);

#ifdef __cplusplus
}
#endif
