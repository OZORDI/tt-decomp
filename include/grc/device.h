#pragma once

#include <cstdint>

struct grcDevice;

#ifdef __cplusplus
extern "C" {
#endif

int grcDevice_init(int width, int height);
void grcDevice_beginScene(void);
void grcDevice_clear(float r, float g, float b, float a);
void grcDevice_present(void);
void grcDevice_shutdown(void);
grcDevice* grcDevice_get(void);

#ifdef __cplusplus
}
#endif
