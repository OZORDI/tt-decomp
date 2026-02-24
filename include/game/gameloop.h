/*
 * Game Loop - Main game update and render cycle
 * 
 * Based on assembly analysis from vtable_analysis.md:
 * - gameLoop_init @ 0x822BC2D0
 * - gameLoop_Update @ 0x82229768 (vtable slot 18)
 * - gameLoop_PreRender @ 0x82229938 (vtable slot 21)
 * - gameLoop_BeginRender @ 0x82229950 (vtable slot 23)
 * - gameLoop_Render @ 0x82229A58 (vtable slot 25)
 * - gameLoop_EndRender @ 0x82229AF8 (vtable slot 27)
 * - gameLoop_PostRender @ 0x82229BA0 (vtable slot 29)
 * - gameLoop_Present @ 0x82229F30 (vtable slot 30)
 */

#ifndef GAME_GAMELOOP_H
#define GAME_GAMELOOP_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * GameLoop object layout (from vtable_analysis.md)
 * 
 * This is the main game loop controller object.
 * Created by gameLoop_init @ 0x822BC2D0
 */
struct GameLoop {
    void*    vtable;          /* 0x8205E9AC (vt74B4)   */
    uint32_t flags_1;         /* +0x4                   */
    uint32_t flags_2;         /* +0x8                   */
    int32_t  field_C;         /* +0xC  = -1             */
    int32_t  field_10;        /* +0x10 = -1             */
    
    uint8_t  padding_14[4];   /* +0x14                  */
    
    void*    sub_object;      /* +0x18                  */
    void*    sub_vtable;      /* +0x1C                  */
    
    /* Padding - exact layout TBD from assembly */
    uint8_t  padding_20[0x100];
    
    /* Counter array and flags */
    uint32_t counters[8];     /* Performance counters */
    uint8_t  loop_active;     /* Loop running flag */
    
    /* More padding */
    uint8_t  padding_mid[0x80];
    
    uint32_t frame_time;      /* Frame delta time */
    uint32_t frame_rate;      /* Target frame rate */
    uint32_t frame_period;    /* Frame period in ms */
};

/*
 * Lifecycle functions
 */

/* Initialize game loop - called at 0x820C08A4 in main() */
int gameLoop_init(void);

/* Shutdown game loop */
int gameLoop_shutdown(void);

/* Get game loop instance */
struct GameLoop* gameLoop_get_instance(void);

/*
 * Per-frame vtable functions (called by main loop)
 */

/* Update tick - physics and game logic */
int gameLoop_Update(void* obj);

/* Pre-render setup */
int gameLoop_PreRender(void* obj);

/* Begin render - camera setup */
int gameLoop_BeginRender(void* obj);

/* Main render dispatch */
int gameLoop_Render(void* obj);

/* End render - framebuffer resolve */
int gameLoop_EndRender(void* obj);

/* Post-render - HUD and debug overlay */
int gameLoop_PostRender(void* obj);

/* Present - swap buffers, clear in-frame flag */
int gameLoop_Present(void* obj);

#ifdef __cplusplus
}
#endif

#endif /* GAME_GAMELOOP_H */
