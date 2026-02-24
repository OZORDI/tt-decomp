#pragma once

#include <cstdint>

namespace rage {

bool fiDevice_Mount(const char* device, const char* path, uint32_t flags);
void* fiDevice_GetDevice(const char* device);
void* fiStream_Open(const char* path, const char* mode, uint32_t flags);
void* fiPackfile_Open(const char* path, uint32_t flags);

}
