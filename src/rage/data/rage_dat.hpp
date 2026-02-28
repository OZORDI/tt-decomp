/**
 * rage_dat.hpp — data parsing and bit-buffer allocator interfaces
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <cstdint>

namespace rage {

// Common RTTI root for many small RAGE data objects.
struct datBase {
    void* m_pVtable;
};

namespace datBitBuffer {

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual bool CanAllocate(std::uint32_t sizeBytes) const = 0;
    virtual void* Allocate(std::uint32_t sizeBytes) = 0;
    virtual void Free(void* memory) = 0;
};

} // namespace datBitBuffer

class datBitBufferDefaultAllocator final : public datBitBuffer::Allocator {
public:
    bool CanAllocate(std::uint32_t sizeBytes) const override;
    void* Allocate(std::uint32_t sizeBytes) override;
    void Free(void* memory) override;
};

class datParser {
public:
    virtual ~datParser();
};

class datParserRecord {
public:
    virtual ~datParserRecord();
};

class datSerialize {
public:
    virtual ~datSerialize();
};

class dcamCam {
public:
    virtual ~dcamCam() = default;
};

class dcamPolarCam : public dcamCam {
public:
    ~dcamPolarCam() override;

    virtual void ConfigurePolarFrame(const float* eyePosition, const float* lookAtPosition);
    virtual bool UpdatePolarState(float deltaTime);
    virtual void ApplyPolarState();
    virtual float* GetPolarStateBuffer();
    virtual void SetPolarStateBuffer(const float* stateBuffer);
    virtual void SetFocusDirection(const float* direction);
    virtual void DrawDebugOverlay(void* renderContext, const char* label, float blend);
    virtual void DrawDebugValue(void* renderContext, const char* label, float value);
    virtual const char* GetDebugName() const;
};

class netBBAllocator {
public:
    virtual ~netBBAllocator();
    virtual bool CanAllocate(std::uint32_t sizeBytes) const;
    virtual void* Allocate(std::uint32_t sizeBytes);
    virtual void* GetBufferBase() const;

private:
    class Backend;
    Backend* m_pBackend = nullptr;
};

} // namespace rage
