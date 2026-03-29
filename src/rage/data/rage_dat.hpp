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

    /**
     * Cleanup @ 0x82177A68 | size: 0x68
     * Destructor body helper — conditionally deletes the owned sub-object
     * when m_type == 10 (pointer-owning record).
     */
    void Cleanup();

private:
    std::int32_t m_type;            // +0x04  record type (10 = pointer-owning)
    std::uint8_t m_pad08[0x44];     // +0x08..+0x4B
    datBase* m_pOwnedObject;        // +0x4C  owned sub-object (deleted if type==10)
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

    /**
     * ProcessPolarInput @ 0x823F7128 | size: 0x5D8
     * Processes camera input in orbit mode (azimuth/elevation/distance)
     * and look mode (pan lookAt target). Returns true if state was modified.
     */
    bool ProcessPolarInput(float dt);
};

/**
 * DrawDebugText @ 0x8227DEC0 | size: 0x114
 * Draws debug text with a drop shadow at (x, y), returns next Y position.
 * Free function in the same translation unit as dcamPolarCam.
 */
int DrawDebugText(int x, int y, const char* text, std::uint32_t color);

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
