/**
 * rage/types.h — Core RAGE engine type definitions
 *
 * Shared types, forward declarations, and platform-specific
 * definitions for the Table Tennis Xbox 360 decompilation.
 */

#ifndef RAGE_TYPES_H
#define RAGE_TYPES_H

#include <stddef.h>

/* ─── Platform types ─────────────────────────────────────────── */
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;
typedef float               f32;
typedef double              f64;

/* Boolean (RAGE uses int-sized bools) */
typedef int                 BOOL;
#define TRUE   1
#define FALSE  0

/* ─── Xbox 360 / XEX specifics ───────────────────────────────── */
typedef u32  DWORD;
typedef u16  WORD;
typedef u8   BYTE;
typedef int  LONG;
typedef unsigned int ULONG;
typedef void *HANDLE;
typedef void *PVOID;
typedef int  NTSTATUS;

/* HRESULT */
typedef long HRESULT;
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr)    ((HRESULT)(hr) < 0)

/* ─── Forward declarations — RAGE engine classes ─────────────── */

/* Memory */
typedef struct sysMemAllocator      sysMemAllocator;

/* Graphics */
typedef struct grcDevice            grcDevice;
typedef struct grcRenderer          grcRenderer;
typedef struct grcDisplayConfig     grcDisplayConfig;

/* Audio */
typedef struct audSystem            audSystem;
typedef struct audStream            audStream;

/* Input */
typedef struct ioInputSystem        ioInputSystem;

/* Framework */
typedef struct fwTimer              fwTimer;
typedef struct fwEntity             fwEntity;
typedef struct fwArchetype          fwArchetype;

/* Physics */
typedef struct phBound              phBound;
typedef struct phInst               phInst;

/* Streaming */
typedef struct pgStreamer           pgStreamer;
typedef struct pgDictionary         pgDictionary;

/* Scene */
typedef struct rage_scene           rage_scene;
typedef struct rage_scene_state     rage_scene_state;

/* ─── Common function pointer types ──────────────────────────── */
typedef void (*VoidFn)(void);
typedef void (*ObjFn)(void *self);
typedef void (*ObjArgFn)(void *self, void *arg);
typedef int  (*ObjIntFn)(void *self);
typedef void *(*ObjAllocFn)(void *allocator, int size, int align);

/* ─── TLS access ─────────────────────────────────────────────── */
/*
 * Xbox 360 XEX executables use PPC register r13 as the TLS base.
 * Thread-local storage layout (from XEX header):
 *   r13 + 0x00 → TLS block pointer
 *   TLS[0]     → reserved
 *   TLS[1]     → thread memory allocator context
 *   TLS[2]     → secondary allocator context
 */
extern void *_xe_tls_base;  /* r13 — XEX TLS base */

/* Inline helpers for TLS access */
static inline void **rage_tls_block(void)
{
    return (void **)*(void **)&_xe_tls_base;
}

static inline void *rage_tls_allocator(void)
{
    return rage_tls_block()[1];
}

/* ─── Vtable dispatch macros ─────────────────────────────────── */
/*
 * RAGE engine uses C++ vtable dispatch extensively.
 * These macros make the decompiled code more readable.
 *
 * VTABLE(obj)        → dereference the vtable pointer
 * VCALL(obj, idx)    → call vtable[idx](obj)
 * VCALL1(obj, idx, a) → call vtable[idx](obj, a)
 */
#define VTABLE(obj)           (*(void ***)(obj))
#define VFUNC(obj, idx, type) ((type)(VTABLE(obj)[idx]))
#define VCALL(obj, idx)       (VFUNC(obj, idx, ObjFn)(obj))
#define VCALL1(obj, idx, a)   (VFUNC(obj, idx, ObjArgFn)(obj, a))

#endif /* RAGE_TYPES_H */
