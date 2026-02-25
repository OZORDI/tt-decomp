/**
 * pcr_anim_blenders.hpp — Pong Creature Animation Blenders
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * Animation blending systems for player creatures:
 * - pcrFaceAnimBlender: Facial expression animation
 * - pcrEmoteBlender: Emote/celebration animations
 * - pcrSwingBlender: Swing motion blending
 * - pcrPostPointBlender: Post-point celebration blending
 * - pcrServeBlender: Serve animation blending
 */
#pragma once
#include <stdint.h>

// Forward declarations
struct pongCreatureState;

/**
 * pcrFaceAnimBlender — Facial Animation Blender
 * @ vtable: 0x820276D0
 * 
 * Manages facial expression animations for creatures during gameplay.
 * Blends between different facial states based on game events.
 */
class pcrFaceAnimBlender {
public:
    void** vtable;  // +0x00

    // ── virtual methods ──
    virtual ~pcrFaceAnimBlender();  // [0] @ 0x820C0C50

    // ── non-virtual methods ──
    /**
     * StartPostPoint @ 0x820CC490 | size: 0x114
     * 
     * Initiates facial animation after a point is scored.
     * Triggers appropriate expression based on point outcome.
     * 
     * @param creature The creature state to apply animation to
     * @return Success flag (non-zero if animation started)
     */
    uint8_t StartPostPoint(pongCreatureState* creature);
};

// Additional blender classes (stubs for now)
class pcrEmoteBlender {
public:
    void** vtable;
    virtual ~pcrEmoteBlender();
};

class pcrSwingBlender {
public:
    void** vtable;
    virtual ~pcrSwingBlender();
};

class pcrPostPointBlender {
public:
    void** vtable;
    virtual ~pcrPostPointBlender();
};

class pcrServeBlender {
public:
    void** vtable;
    virtual ~pcrServeBlender();
};
