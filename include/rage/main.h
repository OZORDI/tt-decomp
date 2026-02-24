/**
 * RAGE Engine Main Entry Point
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Function addresses from symbols.txt:
 * - main @ 0x820C0378 (size: 0x7C8)
 * - rage_main @ 0x82186970 (size: 0x278)
 * - rage_run @ 0x82186780 (size: 0x5C)
 */

#ifndef RAGE_MAIN_H
#define RAGE_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Main entry point called by rage_run */
int main(void);

/* RAGE engine initialization and main loop */
int rage_main(int argc, char **argv);
int rage_run(void);

/* Initialization functions called by main */
void xe_main_thread_init(void);
void rage_game_obj_init(void);
void rage_subsystem_init(void);
void grcDevice_init(void);
void audSystem_init(void);
void ioInput_init(void);
void grcRenderer_init(void);
void gameLoop_init(void);
void rage_obj_factory_create(void);
void rage_obj_bind(void);
void rage_obj_finalize(void);
void audSystem_start(void);
void rage_scene_load(void);

/* Test/debug functions */
void rage_init_test_render(void);

#ifdef __cplusplus
}
#endif

#endif /* RAGE_MAIN_H */
