/**
 * rage/game_loop_types.h — gameLoop struct shared between translation units
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once
#include <stdint.h>

/* ── hsmContext (embedded state machine at gameLoop+0x18) ─────────────────── */
typedef struct hsmContext {
    void** vtable;   /* +0x00 */
    uint32_t _unk04; /* +0x04 */
    void**  pStates; /* +0x08 — array of state object pointers */
    int32_t nCurIdx; /* +0x0C — current state index */
    uint8_t _pad[16];/* +0x10..0x1F — total 32 bytes */
} hsmContext;

/* ── gameLoop struct (layout confirmed from constructor / init / phase funcs) */
typedef struct gameLoop {
    void**      vtable;             /* +0x000 (0)   */
    uint32_t    _unk004;            /* +0x004 (4)   — zeroed in ctor */
    uint32_t    _unk008;            /* +0x008 (8)   — zeroed in ctor */
    int32_t     _unk00C;            /* +0x00C (12)  — set to -1 in ctor */
    int32_t     _unk010;            /* +0x010 (16)  — set to -1 in ctor */
    uint8_t     _pad0[4];           /* +0x014..0x017 */
    hsmContext  m_stateMachine;     /* +0x018 (24)  — embedded, 32 bytes */
    float       m_fFrameTimeA;      /* +0x038 (56)  */
    float       m_fFrameTimeB;      /* +0x03C (60)  */
    float       m_fFrameTimeC;      /* +0x040 (64)  */
    float       m_fFrameTimeD;      /* +0x044 (68)  */
    uint8_t     _pad1[416];         /* +0x048..0x1E7 */
    uint8_t     m_bUnknown488;      /* +0x1E8 (488) — zeroed in ctor */
    uint8_t     m_bUpdateEnabled;   /* +0x1E9 (489) */
    uint8_t     m_bWidescreen;      /* +0x1EA (490) */
    uint8_t     m_bPostRenderOn;    /* +0x1EB (491) */
    uint8_t     m_bFullscreenMode;  /* +0x1EC (492) */
    uint8_t     _pad2[2];           /* +0x1ED..0x1EE (493-494) — zeroed */
    uint8_t     m_bSkipRender;      /* +0x1EF (495) */
    int32_t     m_nPendingUpdates;  /* +0x1F0 (496) */
    float       m_fTargetFPS;       /* +0x1F4 (500) */
    int32_t     m_nUnknown504;      /* +0x1F8 (504) — set to -1 in ctor */
    uint32_t    m_fiberHandle;      /* +0x1FC (508) */
    int32_t     m_nDisplayModeIdx;  /* +0x200 (512) — set to -1 in ctor */
    float       m_fDefaultFPS;      /* +0x204 (516) — copy of target FPS */
    uint32_t    m_nScreenWidth;     /* +0x208 (520) */
    float       m_fLastSceneTime;   /* +0x20C (524) */
    uint8_t     m_bFramePresented;  /* +0x210 (528) */
    uint8_t     _pad5[3];           /* +0x211..0x213 — alignment */
    uint32_t    m_nDisplayStateA;   /* +0x214 (532) */
    uint32_t    m_nDisplayStateB;   /* +0x218 (536) */
    uint32_t    m_nConfigParam540;  /* +0x21C (540) */
    float       m_fUnknown544;      /* +0x220 (544) */
    uint32_t    m_nUnknown548;      /* +0x224 (548) */
    uint32_t    m_nUnknown552;      /* +0x228 (552) */
    uint32_t    m_nUnknown556;      /* +0x22C (556) */
    uint32_t    m_nUnknown560;      /* +0x230 (560) */
    uint32_t    m_nUnknown564;      /* +0x234 (564) */
    uint8_t     m_bUnknown568;      /* +0x238 (568) */
    uint8_t     _pad6[7];           /* +0x239..0x23F — alignment */
    uint8_t     m_bClearColor;      /* +0x240 (576) */
    uint8_t     m_bClearDepth;      /* +0x241 (577) */
    uint8_t     m_bClearStencil;    /* +0x242 (578) */
    uint8_t     m_bClear3;          /* +0x243 (579) */
    uint8_t     m_bClear4;          /* +0x244 (580) */
    uint8_t     m_bClear5;          /* +0x245 (581) */
    uint8_t     m_bClear6;          /* +0x246 (582) */
    uint8_t     _pad7;              /* +0x247 (583) — alignment */
    uint32_t    m_nUnknown584;      /* +0x248 (584) */
} gameLoop;
