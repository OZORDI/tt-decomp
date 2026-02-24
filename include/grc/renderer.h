#pragma once

#include <cstdint>

/**
 * grc/renderer.h
 * 
 * Graphics Renderer - OpenGL 3.3 rendering state management
 * Xbox 360 Xenos GPU → OpenGL translation
 */

struct grcRenderer;

#ifdef __cplusplus
extern "C" {
#endif

// Renderer lifecycle
void grcRenderer_init(void);
void grcRenderer_shutdown(void);
grcRenderer* grcRenderer_get(void);

// Rendering state
void grcRenderer_setViewport(int x, int y, int width, int height);
void grcRenderer_setClearColor(float r, float g, float b, float a);

// Draw commands
void grcRenderer_drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
void grcRenderer_drawQuad(float x, float y, float width, float height);

#ifdef __cplusplus
}
#endif
