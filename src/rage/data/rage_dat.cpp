#include "rage_dat.hpp"

#include <cstdint>

extern "C" void xe_main_thread_init_0038(void);
extern "C" void rage_free(void* ptr);
extern "C" std::uint32_t* g_sda_base;

// Semantic aliases for recovered engine entry points.
extern "C" void DestroyDatParserImpl(rage::datParser* self, int deleteThis)
    __asm__("datParser_rtti_74A0_0");
extern "C" void DestroyDatParserRecordImpl(rage::datParserRecord* self, int deleteThis)
    __asm__("datParserRecord_vfn_0");
extern "C" void DestroyDatSerializeImpl(rage::datSerialize* self, int deleteThis)
    __asm__("datSerialize_vfn_0");

extern "C" void DestroyPolarCameraImpl(rage::dcamPolarCam* self, int deleteThis)
    __asm__("dcamPolarCam_vfn_0");
extern "C" void ConfigurePolarFrameImpl(
    rage::dcamPolarCam* self,
    const float* eyePosition,
    const float* lookAtPosition
) __asm__("dcamPolarCam_vfn_1");
extern "C" int UpdatePolarStateImpl(rage::dcamPolarCam* self, float deltaTime)
    __asm__("dcamPolarCam_vfn_3");
extern "C" void ApplyPolarStateImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_4");
extern "C" float* GetPolarStateBufferImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_5");
extern "C" void SetPolarStateBufferImpl(rage::dcamPolarCam* self, const float* stateBuffer)
    __asm__("dcamPolarCam_vfn_6");
extern "C" void SetFocusDirectionImpl(rage::dcamPolarCam* self, const float* direction)
    __asm__("dcamPolarCam_vfn_7");
extern "C" void DrawPolarDebugOverlayImpl(
    rage::dcamPolarCam* self,
    void* renderContext,
    const char* label,
    float blend
) __asm__("dcamPolarCam_vfn_8");
extern "C" void DrawPolarDebugValueImpl(
    rage::dcamPolarCam* self,
    void* renderContext,
    const char* label,
    float value
) __asm__("dcamPolarCam_vfn_9");
extern "C" const char* GetPolarCameraDebugNameImpl(rage::dcamPolarCam* self)
    __asm__("dcamPolarCam_vfn_10");

namespace {

class MainThreadHeapAllocator {
public:
    virtual ~MainThreadHeapAllocator() = default;
    virtual void* Allocate(std::uint32_t sizeBytes, std::uint32_t alignment) = 0;
};

MainThreadHeapAllocator* GetMainThreadHeapAllocator() {
    if (g_sda_base == nullptr || g_sda_base[0] == 0u) {
        return nullptr;
    }

    auto* allocatorState = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(g_sda_base[0])
    );
    if (allocatorState[1] == 0u) {
        return nullptr;
    }

    return reinterpret_cast<MainThreadHeapAllocator*>(
        static_cast<std::uintptr_t>(allocatorState[1])
    );
}

constexpr std::uint32_t kBitBufferAlignment = 16u;

} // namespace

namespace rage {

bool datBitBufferDefaultAllocator::CanAllocate(std::uint32_t sizeBytes) const {
    return sizeBytes > 0u;
}

void* datBitBufferDefaultAllocator::Allocate(std::uint32_t sizeBytes) {
    xe_main_thread_init_0038();

    MainThreadHeapAllocator* allocator = GetMainThreadHeapAllocator();
    if (allocator == nullptr) {
        return nullptr;
    }

    return allocator->Allocate(sizeBytes, kBitBufferAlignment);
}

void datBitBufferDefaultAllocator::Free(void* memory) {
    if (memory != nullptr) {
        rage_free(memory);
    }
}

datParser::~datParser() {
    DestroyDatParserImpl(this, 0);
}

datParserRecord::~datParserRecord() {
    DestroyDatParserRecordImpl(this, 0);
}

datSerialize::~datSerialize() {
    DestroyDatSerializeImpl(this, 0);
}

dcamPolarCam::~dcamPolarCam() {
    DestroyPolarCameraImpl(this, 0);
}

void dcamPolarCam::ConfigurePolarFrame(const float* eyePosition, const float* lookAtPosition) {
    ConfigurePolarFrameImpl(this, eyePosition, lookAtPosition);
}

bool dcamPolarCam::UpdatePolarState(float deltaTime) {
    return UpdatePolarStateImpl(this, deltaTime) != 0;
}

void dcamPolarCam::ApplyPolarState() {
    ApplyPolarStateImpl(this);
}

float* dcamPolarCam::GetPolarStateBuffer() {
    return GetPolarStateBufferImpl(this);
}

void dcamPolarCam::SetPolarStateBuffer(const float* stateBuffer) {
    SetPolarStateBufferImpl(this, stateBuffer);
}

void dcamPolarCam::SetFocusDirection(const float* direction) {
    SetFocusDirectionImpl(this, direction);
}

void dcamPolarCam::DrawDebugOverlay(void* renderContext, const char* label, float blend) {
    DrawPolarDebugOverlayImpl(this, renderContext, label, blend);
}

void dcamPolarCam::DrawDebugValue(void* renderContext, const char* label, float value) {
    DrawPolarDebugValueImpl(this, renderContext, label, value);
}

const char* dcamPolarCam::GetDebugName() const {
    return GetPolarCameraDebugNameImpl(const_cast<dcamPolarCam*>(this));
}

class netBBAllocator::Backend {
public:
    virtual ~Backend() = default;

    virtual void* Allocate(std::uint32_t sizeBytes, std::uint32_t allocationFlags) = 0;
    virtual void* GetBufferBase() = 0;
    virtual std::uint32_t GetAvailableBytes() const = 0;
};

netBBAllocator::~netBBAllocator() {
    delete m_pBackend;
    m_pBackend = nullptr;
}

bool netBBAllocator::CanAllocate(std::uint32_t sizeBytes) const {
    if (m_pBackend == nullptr) {
        return false;
    }

    return m_pBackend->GetAvailableBytes() >= sizeBytes;
}

void* netBBAllocator::Allocate(std::uint32_t sizeBytes) {
    if (m_pBackend == nullptr) {
        return nullptr;
    }

    return m_pBackend->Allocate(sizeBytes, 0u);
}

void* netBBAllocator::GetBufferBase() const {
    if (m_pBackend == nullptr) {
        return nullptr;
    }

    return const_cast<Backend*>(m_pBackend)->GetBufferBase();
}

} // namespace rage
