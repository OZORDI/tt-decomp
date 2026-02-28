#include "rage_par.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

extern "C" {
void xe_main_thread_init_0038(void);
void* atSingleton_CAD0_g(void* self);
void SinglesNetworkClient_8990_g(const char* src, char* dest, int maxSize);
int RtlMultiByteToUnicodeN_6FA8_w(
    std::uint32_t codePage,
    std::uint32_t flags,
    const char* source,
    int sourceLength,
    char16_t* dest,
    int destChars
);
std::uint32_t* g_sda_base;

void* cmOperatorCtor_DC80_w(void* pOwner, const char* pText, int flags);
void fiAsciiTokenizer_CFA8_w(void* pStringValue, const char* pBegin, const char* pEnd);
void rage_EC58(void* pStringValue, const char* pText);
void* xe_EC88(std::uint32_t size);
}

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

using Address32 = std::uint32_t;

template <typename T = void>
T* ResolveAddress(Address32 address) {
    return reinterpret_cast<T*>(static_cast<std::uintptr_t>(address));
}

template <typename FnType>
FnType GetParMemberVirtual(const rage::parMemberString* self, std::size_t slot) {
    return reinterpret_cast<FnType>(self->m_pVtable[slot]);
}

Address32 GetOwnerAddress(rage::parMemberString* self) {
    using Fn = Address32 (*)(rage::parMemberString*);
    return GetParMemberVirtual<Fn>(self, 1)(self);
}

Address32 GetMemberBaseAddress(rage::parMemberString* self) {
    using Fn = Address32 (*)(rage::parMemberString*);
    return GetParMemberVirtual<Fn>(self, 2)(self);
}

std::uint32_t GetMemberFlags(rage::parMemberString* self) {
    using Fn = std::uint32_t (*)(rage::parMemberString*);
    return GetParMemberVirtual<Fn>(self, 4)(self);
}

std::uint8_t GetStringStorageMode(rage::parMemberString* self) {
    using Fn = std::uint32_t (*)(rage::parMemberString*);
    return static_cast<std::uint8_t>(GetParMemberVirtual<Fn>(self, 10)(self));
}

constexpr std::uint32_t kFlagIndirectStorage = 0x8u;

struct parPackedStringValue32 {
    Address32 m_pHeapText;          // +0x00
    std::uint8_t m_pad04[12];       // +0x04
    Address32 m_pTextBuffer;        // +0x10
    Address32 m_pStorageMarker;     // +0x14
};
static_assert(sizeof(parPackedStringValue32) == 0x18, "parPackedStringValue32 must match PPC layout");

struct cmOperatorStringPayload32 {
    std::uint8_t m_pad00[32];       // +0x00
    Address32 m_pText;              // +0x20
    std::uint32_t m_textLength;     // +0x24
};
static_assert(sizeof(cmOperatorStringPayload32) == 0x28, "cmOperatorStringPayload32 must match PPC layout");

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

cmOperator* parMemberString::vfn_6(std::uint32_t memberOffset) {
    const std::uint8_t storageMode = GetStringStorageMode(this);
    const Address32 memberBase = GetMemberBaseAddress(this);

    Address32 selectedTextAddress = 0;

    if (storageMode >= 1u) {
        if (storageMode == 1u) {
            const Address32 memberAddress = memberBase + memberOffset;
            auto* packedString = ResolveAddress<parPackedStringValue32>(memberAddress);

            // Clear to empty first, then choose the pointer representation that
            // cmOperatorCtor expects for this storage mode.
            *ResolveAddress<char>(packedString->m_pTextBuffer) = '\0';
            selectedTextAddress = memberAddress;
            if (packedString->m_pStorageMarker == (memberAddress + 16u)) {
                selectedTextAddress = packedString->m_pHeapText;
            }
        } else if (storageMode < 3u) {
            selectedTextAddress = *ResolveAddress<Address32>(memberBase + memberOffset);
        }
    } else if ((GetMemberFlags(this) & kFlagIndirectStorage) != 0u) {
        selectedTextAddress = *ResolveAddress<Address32>(memberBase + memberOffset);
    } else {
        selectedTextAddress = memberBase + memberOffset;
    }

    return static_cast<cmOperator*>(cmOperatorCtor_DC80_w(
        ResolveAddress<void>(GetOwnerAddress(this)),
        ResolveAddress<const char>(selectedTextAddress),
        0
    ));
}

void parMemberString::vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset) {
    const auto* payload = reinterpret_cast<const cmOperatorStringPayload32*>(pValueOperator);
    const Address32 sourceText = payload->m_pText;
    const std::uint32_t sourceLength = payload->m_textLength;
    const std::uint8_t storageMode = GetStringStorageMode(this);
    const Address32 memberBase = GetMemberBaseAddress(this);
    const Address32 memberAddress = memberBase + memberOffset;

    if (storageMode >= 1u) {
        if (storageMode == 1u) {
            fiAsciiTokenizer_CFA8_w(
                ResolveAddress<void>(memberAddress),
                ResolveAddress<const char>(sourceText),
                ResolveAddress<const char>(sourceText + sourceLength)
            );
            return;
        }

        if (storageMode < 3u) {
            rage_EC58(
                ResolveAddress<void>(memberAddress),
                ResolveAddress<const char>(sourceText)
            );
        }
        return;
    }

    if ((GetMemberFlags(this) & kFlagIndirectStorage) != 0u) {
        Address32* pStorage = ResolveAddress<Address32>(memberAddress);
        *pStorage = static_cast<Address32>(
            reinterpret_cast<std::uintptr_t>(xe_EC88(sourceLength + 1u))
        );
        std::memcpy(
            ResolveAddress<void>(*pStorage),
            ResolveAddress<const void>(sourceText),
            sourceLength
        );
        *ResolveAddress<char>(*pStorage + sourceLength) = '\0';
        return;
    }

    std::memcpy(
        ResolveAddress<void>(memberAddress),
        ResolveAddress<const void>(sourceText),
        sourceLength
    );
    *ResolveAddress<char>(memberAddress + sourceLength) = '\0';
}

void parMemberString::vfn_8(std::uint32_t memberOffset) {
    if (GetStringStorageMode(this) != 0u) {
        return;
    }

    const Address32 memberAddress = GetMemberBaseAddress(this) + memberOffset;
    if ((GetMemberFlags(this) & kFlagIndirectStorage) != 0u) {
        *ResolveAddress<Address32>(memberAddress) = 0u;
        return;
    }

    *ResolveAddress<char>(memberAddress) = '\0';
}

} // namespace rage
