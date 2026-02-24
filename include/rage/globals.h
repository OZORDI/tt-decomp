/**
 * rage/globals.h — Global variable declarations
 *
 * Extern declarations for all known global symbols used
 * across multiple translation units.
 */

#ifndef RAGE_GLOBALS_H
#define RAGE_GLOBALS_H

#include "rage/types.h"

/* ─── Application state ──────────────────────────────────────── */
extern u8     g_app_running;        /* 0x825EBCA1 — set by rage_app_init */
extern void  *g_app_obj_ptr;        /* 0x825EBCF4 */
extern int    g_app_argc;           /* 0x825EBCA4 */
extern char **g_app_argv;           /* 0x825EBCA8 */
extern char  *g_app_exe_name;       /* 0x825EBCAC */

/* ─── Config flags ───────────────────────────────────────────── */
extern void  *g_cfg_check_a;        /* 0x825CA074 */
extern void  *g_option_list_head;   /* 0x825EBCB0 — cmdline option linked list */

/* ─── Game loop state ────────────────────────────────────────── */
extern void  *g_display_obj_ptr;    /* display singleton */
extern void  *g_audio_obj_ptr;      /* audio singleton */
extern void  *g_audio_config_ptr;   /* audio config */
extern void  *g_threadpool_head;    /* thread pool list head */

/* ─── Subsystem singletons ───────────────────────────────────── */
extern void  *g_rage_factory;       /* object factory singleton */
extern void  *g_scene_current;      /* current active scene */

/* ─── Timing defaults (.rdata) ───────────────────────────────── */
extern f32    g_timing_default_val;  /* 0x8202D110 — 0.0f */
extern f32    g_timing_a;           /* 0x8202D110 */
extern f32    g_timing_b;           /* 0x8202D10C */
extern f32    g_timing_c;           /* 0x8202D108 */

/* ─── String constants (.rdata) ──────────────────────────────── */
extern const char str_unknown_exe[];     /* "unknown" */
extern const char str_unknown_opt_fmt[]; /* "*** Unknown option '%s'..." */
extern const char str_stats_fmt[];       /* FPS stats format string */
extern const char str_asset_path_dev[];  /* "T:\\pong\\assets" */
extern const char str_asset_path_rel[];  /* "D:\\assets" */

/* ─── Vtable pointers (.rdata) ───────────────────────────────── */
extern void *vtable_gameLoop;        /* 0x82059154 */
extern void *vtable_gameLoop_base;   /* 0x8204E984 */
extern void *vtable_sub_obj;         /* 0x82035068 */
extern void *vtable_input_obj;       /* 0x8203CDF4 */
extern void *vtable_renderer_base;   /* 0x8203008C */
extern void *vtable_display_ctx;     /* display context vtable */
extern void *vtable_render_sub_a;    /* 0x82030CB8 */
extern void *vtable_render_sub_b;    /* 0x8203F258 */

/* ─── Sub-object default data (.rdata) ───────────────────────── */
extern char  sub_obj_defaults[];     /* 14 bytes at 0x82035010 */

#endif /* RAGE_GLOBALS_H */
