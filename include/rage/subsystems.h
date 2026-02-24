/*
 * RAGE Subsystem Declarations
 * 
 * Based on assembly analysis from rage_engine_subsystems.md
 * 
 * These are the core RAGE engine initialization functions called from main()
 */

#ifndef RAGE_SUBSYSTEMS_H
#define RAGE_SUBSYSTEMS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Thread and CRT initialization
 */

/* XDK thread context setup - called at 0x820C03AC */
int xe_main_thread_init(void);

/*
 * RAGE core object system
 */

/* Constructs root game object, installs vtable - called at 0x820C03F4 */
int rage_game_obj_init(void);

/* Registers RAGE subsystems - called at 0x820C041C */
int rage_subsystem_init(void);

/* Returns executable name string - called at 0x820C0414 */
const char *rage_get_exe_name(void);

/* Checks if app has been started - called at 0x820C03D0 */
int rage_is_app_started(void);

/*
 * Audio subsystem
 */

/* Audio system initialization - called at 0x820C05D8 */
int audSystem_init(void);

/* Start audio thread - called at 0x820C06BC */
int audSystem_start(void);

/*
 * Input subsystem
 */

/* Input system initialization - called at 0x820C06E8 */
int ioInput_init(void);

/*
 * Scene and object management
 */

/* Creates game/scene instance - called at 0x820C062C */
int rage_obj_factory_create(void);

/* Binds scene to game object - called at 0x820C0684 */
int rage_obj_bind(void);

/* Finalizes scene object - called at 0x820C06B4 */
int rage_obj_finalize(void);

/* Loads scene data - called at 0x820C0938 */
int rage_scene_load(void);

/*
 * Render functions
 */

/* Default render path - called from main loop @ 0x822C08D8 */
int rage_render_default(void);

/* Scene render guard - called from main loop @ 0x822C0B18 */
int rage_render_scene(void);

/* Initialize test render system (temporary for early development) */
int rage_init_test_render(void);

#ifdef __cplusplus
}
#endif

#endif /* RAGE_SUBSYSTEMS_H */
