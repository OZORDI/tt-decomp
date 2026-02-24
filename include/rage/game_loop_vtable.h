/*
 * Game Loop Vtable - Complete 34-slot definition
 * 
 * Based on vtable_analysis.md - vt74B4 @ 0x820274B4
 * 
 * This is the PRIMARY gameplay vtable with all render functions.
 * Two other variants exist (vt00EC for paused state, vt9B4 for loading).
 */

#ifndef RAGE_GAME_LOOP_VTABLE_H
#define RAGE_GAME_LOOP_VTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * GameLoopVTable - 34 function pointers (slots 0-33)
 * 
 * Offsets are byte offsets from vtable base.
 * Each slot is 4 bytes (32-bit function pointer).
 */
struct GameLoopVTable {
    /* Slots 0-4: Early lifecycle (before vtable dump in analysis) */
    void (*slot_0)(void* obj);     /* +0x00 - Unknown */
    void (*slot_1)(void* obj);     /* +0x04 - Unknown */
    void (*slot_2)(void* obj);     /* +0x08 - Unknown */
    void (*slot_3)(void* obj);     /* +0x0C - Unknown */
    void (*slot_4)(void* obj);     /* +0x10 - Update tick C */
    
    /* Slots 5-16: State management and lifecycle */
    void (*slot_5)(void* obj);     /* +0x14 - 0x82229F40 */
    void (*slot_6)(void* obj);     /* +0x18 - 0x82222BF0 */
    void (*slot_7)(void* obj);     /* +0x1C - 0x82229648 */
    void (*slot_8)(void* obj);     /* +0x20 - 0x822BC590 */
    void (*slot_9)(void* obj);     /* +0x24 - 0x822BC850 */
    void (*slot_10)(void* obj);    /* +0x28 - 0x8240E6D0 (NOP stub) */
    void (*slot_11)(void* obj);    /* +0x2C - 0x822BCF08 */
    void (*slot_12)(void* obj);    /* +0x30 - 0x822BD228 */
    void (*slot_13)(void* obj);    /* +0x34 - 0x822BD948 */
    void (*slot_14)(void* obj);    /* +0x38 - 0x8240E6D0 (NOP stub) */
    void (*slot_15)(void* obj);    /* +0x3C - 0x82229638 */
    void (*slot_16)(void* obj);    /* +0x40 - 0x822BDC38 */
    
    /* Slots 17-20: Update cycle */
    void (*slot_17)(void* obj);    /* +0x44 - 0x8240E6D0 (NOP stub) - Update tick A */
    void (*slot_18)(void* obj);    /* +0x48 - 0x82229768 - gameLoop_Update (physics) */
    void (*slot_19)(void* obj);    /* +0x4C - 0x822C1B10 */
    void (*slot_20)(void* obj);    /* +0x50 - 0x8240E6D0 (NOP stub) */
    
    /* Slots 21-30: Render cycle (called every frame) */
    void (*slot_21)(void* obj);    /* +0x54 - 0x82229938 - gameLoop_PreRender */
    void (*slot_22)(void* obj);    /* +0x58 - 0x822BD500 */
    void (*slot_23)(void* obj);    /* +0x5C - 0x82229950 - gameLoop_BeginRender */
    void (*slot_24)(void* obj);    /* +0x60 - 0x822BD598 */
    void (*slot_25)(void* obj);    /* +0x64 - 0x82229A58 - gameLoop_Render */
    void (*slot_26)(void* obj);    /* +0x68 - 0x822BD6C0 */
    void (*slot_27)(void* obj);    /* +0x6C - 0x82229AF8 - gameLoop_EndRender */
    void (*slot_28)(void* obj);    /* +0x70 - 0x822BD7B0 */
    void (*slot_29)(void* obj);    /* +0x74 - 0x82229BA0 - gameLoop_PostRender */
    void (*slot_30)(void* obj);    /* +0x78 - 0x82229F30 - gameLoop_Present */
    
    /* Slots 31-33: Cleanup and finalization */
    void (*slot_31)(void* obj);    /* +0x7C - 0x822BD8A8 */
    void (*slot_32)(void* obj);    /* +0x80 - 0x8248EC78 */
    void (*slot_33)(void* obj);    /* +0x84 - 0x8240E6D0 (NOP stub) */
};

/* Compile-time size verification */
_Static_assert(sizeof(GameLoopVTable) == 34 * sizeof(void*), "GameLoopVTable must have exactly 34 slots");

#ifdef __cplusplus
}
#endif

#endif /* RAGE_GAME_LOOP_VTABLE_H */
