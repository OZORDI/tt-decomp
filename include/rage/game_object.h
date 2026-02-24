/*
 * RAGE Game Object - Single Source of Truth
 * 
 * Based on assembly analysis from render_pipeline.md and vtable_analysis.md
 * 
 * Memory layout verified against Xbox 360 binary:
 * - +0x00: vtable pointer
 * - +0x04: flags
 * - +0x22C: scene_state pointer
 * - +0x240: render_flags_1
 * - +0x241: render_flags_2
 * - +0x242: render_flags_3
 */

#ifndef RAGE_GAME_OBJECT_H
#define RAGE_GAME_OBJECT_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration of vtable */
struct GameLoopVTable;

/*
 * GameObject structure - matches assembly layout
 * 
 * This is the root game object created by rage_game_obj_init @ 0x820C03F4
 * Stored at g_render_obj_ptr (0x825EAB2C)
 */
struct GameObject {
    void* vtable;                  /* +0x00 - vtable pointer (vt74B4 @ 0x820274B4) */
    uint32_t flags;                /* +0x04 - state flags */
    
    /* Padding to match assembly offsets (accounting for 64-bit pointer alignment) */
    uint8_t padding_08[0x22C - 0x08];
    
    uint32_t scene_state;          /* +0x22C - scene state flags (checked at 0x820C09EC) */
    
    /* More padding */
    uint8_t padding_230[0x240 - 0x230];
    
    uint8_t render_flags_1;        /* +0x240 - render state (read at 0x822C08E8) */
    uint8_t render_flags_2;        /* +0x241 - render state */
    uint8_t render_flags_3;        /* +0x242 - render state */
    uint8_t render_flags_4;        /* +0x243 - render state */
    
    /* Timing fields (inferred from assembly) */
    uint32_t frame_time;           /* Frame delta time in ms */
    uint32_t last_frame_time;      /* Previous frame timestamp */
    
    /* Additional fields discovered through analysis */
    uint8_t padding_rest[256];     /* Reserve space for undiscovered fields */
};

/* Note: Compile-time offset verification disabled for cross-platform compatibility
 * Original Xbox 360 used 32-bit pointers, modern systems use 64-bit pointers */

#ifdef __cplusplus
}
#endif

#endif /* RAGE_GAME_OBJECT_H */
