/**
 * Global Variables Declarations
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * These globals match the original Xbox 360 memory layout.
 * Addresses are documented for reference but not enforced in the rewrite.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Include shared type definitions */
#ifdef __cplusplus
#include "tabletennis_types.h"
#endif

/* Table tennis game state */
extern BallState    g_ball;
extern PaddleState  g_paddles[2];

/* RAGE engine global objects */
extern void* g_render_obj_ptr;   /* 0x825EAB2C - Root game object */
extern void* g_input_obj_ptr;    /* 0x825FAB2C - Input system */
extern void* g_scene_obj_ptr;    /* 0x825FAB28 - Scene object */
extern void* g_grc_device_ptr;   /* 0x8272A358 - Graphics device */
extern void* g_loop_obj_ptr;     /* 0x825EAB30 - Game loop object */

/* Frame counters */
extern uint32_t g_frame_counter;    /* Matches g_grc_frame_count @ 0x82606348 */
extern uint32_t g_frame_counter_1;  /* @ 0x826065DC - Used by rage_render_default */
extern uint32_t g_frame_counter_2;  /* @ 0x825C???? - Second frame counter */
extern void*    g_render_list_head;

/* CRT globals */
extern void* (*g_crt_tls_alloc)(void);
extern void* (*g_crt_tls_get)(void*);
extern int   (*g_crt_tls_set)(void*, void*);
extern void  (*g_crt_tls_free)(void*);
extern int    g_cinit_retval;
extern void*  g_doexit_fptr;

/* Monitor data */
extern void* __imp_KeDebugMonitorData;

#ifdef __cplusplus
}
#endif

#endif /* GLOBALS_H */
