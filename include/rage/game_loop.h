/**
 * rage/game_loop.h — gameLoop lifecycle and frame phase functions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <stdint.h>
#include "rage/game_loop_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Lifecycle functions ────────────────────────────────────────────────── */

/**
 * gameLoop_init_C2D0 @ 0x822BC2D0 | size: 0x108
 * Constructor — initializes a gameLoop object to default state
 */
void gameLoop_init_C2D0(gameLoop* pLoop);

/**
 * gameLoop_Init_8F30 @ 0x82228F30 | size: 0x588
 * Post-construction initialization — performs extensive engine subsystem setup
 */
void gameLoop_Init_8F30(gameLoop* pLoop, void* pConfig);

/**
 * gameLoop_Shutdown_94B8 @ 0x822294B8 | size: 0x180
 * Teardown — destroys subsystems in reverse init order
 */
void gameLoop_Shutdown_94B8(gameLoop* pLoop);

/* ── Frame execution ────────────────────────────────────────────────────── */

/**
 * gameLoop_StepFrame_9760 @ 0x82229760 | size: 0x4
 * Thunk — tail-calls gmLogic::StepFrame
 */
void gameLoop_StepFrame(gameLoop* pLoop);

/* ── Frame phases ───────────────────────────────────────────────────────── */

/**
 * gameLoop_Update_9768 @ 0x82229768 | size: 0x6C
 * Per-frame logic update
 */
void gameLoop_Update(gameLoop* pLoop);

/**
 * gameLoop_Tick_97D8 @ 0x822297D8 | size: 0x160
 * Physics + AI tick
 */
void gameLoop_Tick(gameLoop* pLoop);

/**
 * gameLoop_PreRender_9938 @ 0x82229938 | size: 0x14
 * Pre-render state-machine dispatch
 */
void gameLoop_PreRender(gameLoop* pLoop);

/**
 * gameLoop_BeginRender_9950 @ 0x82229950 | size: 0x104
 * Begins render pass
 */
void gameLoop_BeginRender(gameLoop* pLoop);

/**
 * gameLoop_Render_9A58 @ 0x82229A58 | size: 0x9C
 * Main scene render
 */
void gameLoop_Render(gameLoop* pLoop, uint8_t bForce);

/**
 * gameLoop_EndRender_9AF8 @ 0x82229AF8 | size: 0xA8
 * Finalizes render pass
 */
void gameLoop_EndRender(gameLoop* pLoop);

/**
 * gameLoop_PostRender_9BA0 @ 0x82229BA0 | size: 0x38C
 * Post-render: FPS overlay, aspect-ratio management
 */
void gameLoop_PostRender(gameLoop* pLoop);

/**
 * gameLoop_Present_9F30 @ 0x82229F30 | size: 0xC
 * Marks frame as presented
 */
void gameLoop_Present(gameLoop* pLoop);

#ifdef __cplusplus
}
#endif
