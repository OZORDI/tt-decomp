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
 * Scene render controller.  Wraps rage_render_default with profiling
 * and post-render stream flush.
 * pThis: rageSceneRenderCtx* — scene render context object.
 */
void rage_render_scene(void* pThis);

#ifdef __cplusplus
}
#endif
