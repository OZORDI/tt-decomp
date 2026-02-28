#include "rage_par.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

extern "C" void xe_main_thread_init_0038(void);
extern "C" void* atSingleton_CAD0_g(void* self);
extern "C" void SinglesNetworkClient_8990_g(const char* src, char* dest, int maxSize);
extern "C" int RtlMultiByteToUnicodeN_6FA8_w(
    std::uint32_t codePage,
    std::uint32_t flags,
    const char* source,
    int sourceLength,
    char16_t* dest,
    int destChars
);
extern "C" std::uint32_t* g_sda_base;

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

template <typename T>
T& GlobalRef(std::uintptr_t address) {
    return *reinterpret_cast<T*>(address);
}

struct SlotHeader {
    std::uint32_t m_state;
    std::uint8_t m_hasPendingRead;
    std::uint8_t m_hasPendingWrite;
    std::uint8_t m_pad06[2];
    std::byte m_runtimeStruct[16];
    std::uint32_t m_bufferPtr;
    std::uint8_t m_pad1C[2];
    std::uint16_t m_recordLimit;
};

static_assert(offsetof(SlotHeader, m_runtimeStruct) == 0x8, "slot runtime offset mismatch");
static_assert(offsetof(SlotHeader, m_bufferPtr) == 0x18, "slot buffer offset mismatch");
static_assert(offsetof(SlotHeader, m_recordLimit) == 0x1E, "slot record limit offset mismatch");

constexpr std::uint32_t kPongSaveFilePrimaryVtable = 0x8203CB18;
constexpr std::uint32_t kPongSaveFileSecondaryVtable = 0x8203CB30;
constexpr std::uint32_t kParRTStructureVtable = 0x82065AE0;

constexpr std::size_t kSlotRuntimeOffset = 0x8;
constexpr std::uint32_t kSlotBufferSize = 0x200;
constexpr std::uint32_t kSlotBufferAlignment = 0x10;
constexpr std::uint16_t kSlotRecordLimit = 128;

constexpr std::uint32_t kRegistryTypeId = 16;
constexpr std::uintptr_t kRegistryCounterAddr = 0x825F3864;
constexpr std::uintptr_t kRegistryTargetArrayAddr = 0x825F3868;
constexpr std::uintptr_t kRegistryTypeArrayAddr = 0x825F3968;
constexpr std::uintptr_t kRegistryLimitArrayAddr = 0x825F3A68;

constexpr std::uintptr_t kPrimaryLabelAddr = 0x8203CA94;
constexpr std::uintptr_t kSecondaryLabelAddr = 0x8203CAAC;
constexpr std::uint32_t kUtf8CodePage = 65001;
constexpr int kUtf8LabelSize = 129;

} // namespace

namespace rage {

/**
 * parRTStructure::parRTStructure() @ 0x821C5090 | size: 0x180
 *
 * Initializes five runtime par slots, registers this object with the global
 * save/profile registry tables, and materializes two UTF-16 labels.
 */
parRTStructure::parRTStructure() {
    m_primaryVtable = kPongSaveFilePrimaryVtable;
    m_secondaryVtable = kPongSaveFileSecondaryVtable;
    m_payloadPtr = 0;
    m_payloadCount = 0;
    m_payloadFlags = 0;
    m_slotRegistrationToken = 0;
    m_runtimeHandle = 0;
    m_runtimeEnabled = 0;

    MainThreadHeapAllocator* allocator = GetMainThreadHeapAllocator();
    std::byte* slotBase = m_slotStorage.data();

    for (std::size_t slotIndex = 0; slotIndex < kSlotCount; ++slotIndex) {
        auto* slotHeader = reinterpret_cast<SlotHeader*>(slotBase);
        void* runtimeStruct = atSingleton_CAD0_g(slotBase + kSlotRuntimeOffset);
        if (runtimeStruct != nullptr) {
            *reinterpret_cast<std::uint32_t*>(runtimeStruct) = kParRTStructureVtable;
        }

        xe_main_thread_init_0038();

        slotHeader->m_bufferPtr = 0;
        if (allocator != nullptr) {
            void* slotBuffer = allocator->Allocate(kSlotBufferSize, kSlotBufferAlignment);
            slotHeader->m_bufferPtr = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(slotBuffer)
            );
        }
        slotHeader->m_recordLimit = kSlotRecordLimit;
        slotHeader->m_state = 0;
        slotHeader->m_hasPendingRead = 0;
        slotHeader->m_hasPendingWrite = 0;

        slotBase += kSlotStride;
    }

    auto& registryCounter = GlobalRef<std::uint32_t>(kRegistryCounterAddr);
    const std::uint32_t registryIndex = registryCounter++;

    auto* registryTargets = reinterpret_cast<std::uint32_t*>(kRegistryTargetArrayAddr);
    auto* registryTypes = reinterpret_cast<std::uint32_t*>(kRegistryTypeArrayAddr);
    auto* registryLimits = reinterpret_cast<std::uint8_t*>(kRegistryLimitArrayAddr);

    registryLimits[registryIndex] = static_cast<std::uint8_t>(kSlotRecordLimit);
    registryTargets[registryIndex] = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(&m_secondaryVtable)
    );
    registryTypes[registryIndex] = kRegistryTypeId;

    std::array<char, kUtf8LabelSize> utf8Label{};
    SinglesNetworkClient_8990_g(
        reinterpret_cast<const char*>(kPrimaryLabelAddr),
        utf8Label.data(),
        static_cast<int>(utf8Label.size())
    );
    RtlMultiByteToUnicodeN_6FA8_w(
        kUtf8CodePage,
        0,
        utf8Label.data(),
        -1,
        m_primaryPathUtf16.data(),
        static_cast<int>(m_primaryPathUtf16.size())
    );

    SinglesNetworkClient_8990_g(
        reinterpret_cast<const char*>(kSecondaryLabelAddr),
        utf8Label.data(),
        static_cast<int>(utf8Label.size())
    );
    RtlMultiByteToUnicodeN_6FA8_w(
        kUtf8CodePage,
        0,
        utf8Label.data(),
        -1,
        m_secondaryPathUtf16.data(),
        static_cast<int>(m_secondaryPathUtf16.size())
    );
}

} // namespace rage
