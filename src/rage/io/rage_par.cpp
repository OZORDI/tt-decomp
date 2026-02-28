#include "rage_par.hpp"

#include <algorithm>
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
void rage_free_00C0(void* ptr);
void nop_8240E6D0(const char* fmt, ...);
void* phMaterialMgrImpl_C208_g(void* pHashBucketSet, const char* pLookupName);
void* rage_97A8(const void* pOperator, const char* pNodeName);
void util_9410(void* pOperator, void* pOwner, std::uint32_t duplicateOwner);
void jumptable_9498(
    void* pOperator,
    const char* pPrimaryName,
    const char* pSecondaryName,
    std::uint32_t copyPrimary,
    std::uint32_t copySecondary
);
void cmOperatorCtor_D8C0_w(void* pOperator, const void* pValueData, std::uint32_t valueSize);
void atSingleton_DA18_p46(void* pNode, void* pRootNode);
void atSingleton_D210_p46(void* pStructType, void* pStructInstance, void* pOperator);
std::uint8_t jumptable_E058_h(void* pMemberArray);
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

template <typename FnType, typename SelfType>
FnType GetParMemberVirtual(SelfType* self, std::size_t slot) {
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

constexpr std::uint32_t kParMemberVtable = 0x82065A38;
constexpr std::uint32_t kParMemberArrayVtable = 0x82065BD0;
constexpr Address32 kParArrayLegacyProbeString = 0x82065B68;
constexpr Address32 kParArrayTypeRootLabel = 0x82065B30;
constexpr Address32 kParArrayTypeLabelByte = 0x82065B6C;
constexpr Address32 kParArrayTypeLabelShort = 0x82065B78;
constexpr Address32 kParArrayTypeLabelInt = 0x82065B84;
constexpr Address32 kParArrayTypeLabelFloat = 0x82065B90;
constexpr Address32 kParArrayTypeLabelBool = 0x82065B9C;
constexpr Address32 kParArrayTypeLabelVector3 = 0x82065BAC;
constexpr Address32 kParArrayTypeLabelVector4 = 0x82065BBC;
constexpr std::uint32_t kParArrayLargeStorageOffset = 0x10000;

struct parArrayTypeInfo32 {
    std::uint8_t m_pad00[10];
    std::uint8_t m_valueType;
};
static_assert(offsetof(parArrayTypeInfo32, m_valueType) == 10, "parArrayTypeInfo32 layout mismatch");

struct parArrayResizeContext32 {
    Address32 m_pad00;
    Address32 m_pad04;
    Address32 m_pad08;
    Address32 m_pResizeFn;
};
static_assert(sizeof(parArrayResizeContext32) == 0x10, "parArrayResizeContext32 layout mismatch");

struct parMemberDescriptor32 {
    Address32 m_pOwner;
    Address32 m_pMemberBase;
    std::uint16_t m_flags;
    std::uint8_t m_valueType;
    std::uint8_t m_storageMode;
    std::uint32_t m_elementStride;
    std::uint32_t m_elementCapacity;
    Address32 m_pArrayTypeInfo;
    parArrayResizeContext32 m_resizeContext;
};
static_assert(offsetof(parMemberDescriptor32, m_storageMode) == 11, "parMemberDescriptor32 storage mode offset mismatch");
static_assert(offsetof(parMemberDescriptor32, m_elementStride) == 12, "parMemberDescriptor32 stride offset mismatch");
static_assert(offsetof(parMemberDescriptor32, m_elementCapacity) == 16, "parMemberDescriptor32 capacity offset mismatch");
static_assert(offsetof(parMemberDescriptor32, m_pArrayTypeInfo) == 20, "parMemberDescriptor32 type info offset mismatch");
static_assert(offsetof(parMemberDescriptor32, m_resizeContext) == 24, "parMemberDescriptor32 resize context offset mismatch");

struct cmOperatorArrayPayload32 {
    Address32 m_pOwner;
    std::uint8_t m_flags;
    std::uint8_t m_pad05[3];
    Address32 m_pInlineData;
    std::uint16_t m_pad0C;
    std::uint16_t m_pad0E;
    std::uint32_t m_runtimeToken;
    Address32 m_pad14;
    Address32 m_pad18;
    Address32 m_pFirstChild;
    Address32 m_pValueBuffer;
    std::uint32_t m_valueSize;
    std::uint8_t m_hasInlineValue;
    std::uint8_t m_pad29[3];
};
static_assert(offsetof(cmOperatorArrayPayload32, m_pFirstChild) == 28, "cmOperatorArrayPayload32 list offset mismatch");
static_assert(offsetof(cmOperatorArrayPayload32, m_pValueBuffer) == 32, "cmOperatorArrayPayload32 payload offset mismatch");
static_assert(offsetof(cmOperatorArrayPayload32, m_valueSize) == 36, "cmOperatorArrayPayload32 size offset mismatch");
static_assert(offsetof(cmOperatorArrayPayload32, m_hasInlineValue) == 40, "cmOperatorArrayPayload32 flag offset mismatch");
static_assert(sizeof(cmOperatorArrayPayload32) == 44, "cmOperatorArrayPayload32 size mismatch");

struct cmOperatorArrayNode32 {
    std::uint8_t m_pad00[24];
    Address32 m_pNext;
};
static_assert(offsetof(cmOperatorArrayNode32, m_pNext) == 24, "cmOperatorArrayNode32 next offset mismatch");

parMemberDescriptor32* GetArrayDescriptor(rage::parMemberArray* self) {
    return static_cast<parMemberDescriptor32*>(self->m_pMemberDesc);
}

Address32 GetArrayOwnerAddress(rage::parMemberArray* self) {
    using Fn = Address32 (*)(rage::parMemberArray*);
    return GetParMemberVirtual<Fn>(self, 1)(self);
}

Address32 GetArrayMemberBaseAddress(rage::parMemberArray* self) {
    using Fn = Address32 (*)(rage::parMemberArray*);
    return GetParMemberVirtual<Fn>(self, 2)(self);
}

std::uint8_t GetArrayValueType(rage::parMemberArray* self) {
    const parMemberDescriptor32* descriptor = GetArrayDescriptor(self);
    if (descriptor == nullptr || descriptor->m_pArrayTypeInfo == 0u) {
        return 0xFFu;
    }

    return ResolveAddress<parArrayTypeInfo32>(descriptor->m_pArrayTypeInfo)->m_valueType;
}

template <typename FnType>
FnType GetVirtualSlot(void* object, std::size_t slot) {
    return reinterpret_cast<FnType>((*reinterpret_cast<void***>(object))[slot]);
}

void SelectElementSerializerEntry(void* pSerializer, std::uint32_t offsetBytes) {
    using Fn = void* (*)(void* self, std::uint32_t offsetBytes);
    GetVirtualSlot<Fn>(pSerializer, 3)(pSerializer, offsetBytes);
}

rage::cmOperator* ExportSerializedElement(void* pSerializer, Address32 memberDataAddress) {
    using Fn = rage::cmOperator* (*)(void* self, Address32 memberDataAddress);
    return GetVirtualSlot<Fn>(pSerializer, 6)(pSerializer, memberDataAddress);
}

void ImportElementFromNode(void* pSerializer, const rage::cmOperator* pNode, Address32 memberDataAddress) {
    using Fn = void (*)(void* self, const rage::cmOperator* pNode, Address32 memberDataAddress);
    GetVirtualSlot<Fn>(pSerializer, 7)(pSerializer, pNode, memberDataAddress);
}

void RestoreSerializerDefault(void* pSerializer, Address32 memberDataAddress) {
    using Fn = void (*)(void* self, Address32 memberDataAddress);
    GetVirtualSlot<Fn>(pSerializer, 8)(pSerializer, memberDataAddress);
}

Address32 GetArrayTypeLabel(std::uint8_t valueType) {
    switch (valueType) {
        case 1:
        case 2:
            return kParArrayTypeLabelShort;
        case 3:
        case 4:
            return kParArrayTypeLabelInt;
        case 5:
        case 6:
            return kParArrayTypeLabelByte;
        case 7:
            return kParArrayTypeLabelFloat;
        case 8:
            return kParArrayTypeLabelBool;
        case 9:
            return kParArrayTypeLabelVector3;
        case 10:
            return kParArrayTypeLabelVector4;
        default:
            return 0u;
    }
}

void InitializeArrayOperator(cmOperatorArrayPayload32* pOperator) {
    if (pOperator == nullptr) {
        return;
    }

    pOperator->m_pOwner = 0u;
    pOperator->m_flags = 0u;
    pOperator->m_pInlineData = 0u;
    pOperator->m_pad0C = 0u;
    pOperator->m_pad0E = 0u;
    auto* rawOperator = reinterpret_cast<std::uint8_t*>(pOperator);
    pOperator->m_runtimeToken = *reinterpret_cast<std::uint16_t*>(rawOperator + 18u);
    *reinterpret_cast<std::uint16_t*>(rawOperator + 18u) = 0u;
    pOperator->m_pad14 = 0u;
    pOperator->m_pad18 = 0u;
    pOperator->m_pFirstChild = 0u;
    pOperator->m_pValueBuffer = 0u;
    pOperator->m_valueSize = 0u;
    pOperator->m_hasInlineValue = 0u;
}

void AttachOperatorOwner(cmOperatorArrayPayload32* pOperator, Address32 ownerAddress) {
    if (pOperator == nullptr) {
        return;
    }

    if ((pOperator->m_flags & 0x1u) != 0u && pOperator->m_pOwner != 0u) {
        rage_free_00C0(ResolveAddress<void>(pOperator->m_pOwner));
    }

    pOperator->m_pOwner = ownerAddress;
    pOperator->m_flags &= ~0x1u;
}

constexpr std::uintptr_t kParStructRegistryGlobalAddr = 0x8271A374;
constexpr Address32 kParStructNullTypeNameAddr = 0x8202E99C;
constexpr Address32 kParStructTypeNodeKeyAddr = 0x82065C00;
constexpr Address32 kParStructFactoryNodeKeyAddr = 0x82027738;

constexpr std::uint16_t kParStructFlagIndirectStorage = 0x0008;
constexpr std::uint16_t kParStructFlagFactoryCreate = 0x0010;
constexpr std::uint16_t kParStructFlagDirectType = 0x0020;
constexpr std::uint16_t kParStructFlagNullSentinel = 0x0080;

struct parMemberStructDescriptor32 {
    Address32 m_pOwner;
    Address32 m_pMemberBase;
    std::uint16_t m_flags;
    std::uint8_t m_valueType;
    std::uint8_t m_storageMode;
    Address32 m_pStructName;
    Address32 m_pDirectStructType;
    Address32 m_pResolveTypeByNameFn;
    Address32 m_pGetTypeNameFn;
    Address32 m_pCreateFromOperatorFn;
};
static_assert(offsetof(parMemberStructDescriptor32, m_pStructName) == 12, "parMemberStructDescriptor32 name offset mismatch");
static_assert(offsetof(parMemberStructDescriptor32, m_pDirectStructType) == 16, "parMemberStructDescriptor32 type offset mismatch");
static_assert(offsetof(parMemberStructDescriptor32, m_pResolveTypeByNameFn) == 20, "parMemberStructDescriptor32 resolve fn offset mismatch");
static_assert(offsetof(parMemberStructDescriptor32, m_pGetTypeNameFn) == 24, "parMemberStructDescriptor32 type-name fn offset mismatch");
static_assert(offsetof(parMemberStructDescriptor32, m_pCreateFromOperatorFn) == 28, "parMemberStructDescriptor32 create fn offset mismatch");

Address32 GetParStructRegistry() {
    return *ResolveAddress<Address32>(kParStructRegistryGlobalAddr);
}

parMemberStructDescriptor32* GetStructDescriptor(rage::parMemberStruct* self) {
    return static_cast<parMemberStructDescriptor32*>(self->m_pMemberDesc);
}

Address32 GetStructOwnerAddress(rage::parMemberStruct* self) {
    using Fn = Address32 (*)(rage::parMemberStruct*);
    return GetParMemberVirtual<Fn>(self, 1)(self);
}

Address32 GetStructMemberBaseAddress(rage::parMemberStruct* self) {
    using Fn = Address32 (*)(rage::parMemberStruct*);
    return GetParMemberVirtual<Fn>(self, 2)(self);
}

std::uint16_t GetStructFlags(rage::parMemberStruct* self) {
    using Fn = std::uint32_t (*)(rage::parMemberStruct*);
    return static_cast<std::uint16_t>(GetParMemberVirtual<Fn>(self, 4)(self));
}

std::uint8_t GetStructStorageMode(rage::parMemberStruct* self) {
    using Fn = std::uint32_t (*)(rage::parMemberStruct*);
    return static_cast<std::uint8_t>(GetParMemberVirtual<Fn>(self, 10)(self));
}

bool HasStructFlag(rage::parMemberStruct* self, std::uint16_t flagMask) {
    return (GetStructFlags(self) & flagMask) != 0u;
}

bool AreStructNamesEqual(Address32 lhsStringAddress, Address32 rhsStringAddress) {
    if (lhsStringAddress == 0u || rhsStringAddress == 0u) {
        return false;
    }

    return std::strcmp(
        ResolveAddress<const char>(lhsStringAddress),
        ResolveAddress<const char>(rhsStringAddress)
    ) == 0;
}

Address32 InvokeContextCall1(Address32 contextAddress, Address32 arg0) {
    if (contextAddress == 0u) {
        return 0u;
    }

    const Address32 callbackAddress = *ResolveAddress<Address32>(contextAddress + 12u);
    if (callbackAddress == 0u) {
        return 0u;
    }

    using Fn = Address32 (*)(Address32 self, Address32 arg0);
    return reinterpret_cast<Fn>(static_cast<std::uintptr_t>(callbackAddress))(contextAddress, arg0);
}

Address32 InvokeContextCall0(Address32 contextAddress) {
    if (contextAddress == 0u) {
        return 0u;
    }

    const Address32 callbackAddress = *ResolveAddress<Address32>(contextAddress + 12u);
    if (callbackAddress == 0u) {
        return 0u;
    }

    using Fn = Address32 (*)(Address32 self);
    return reinterpret_cast<Fn>(static_cast<std::uintptr_t>(callbackAddress))(contextAddress);
}

Address32 ResolveStructTypeByName(const parMemberStructDescriptor32* pDescriptor, Address32 typeNameAddress) {
    if (pDescriptor == nullptr || pDescriptor->m_pResolveTypeByNameFn == 0u || typeNameAddress == 0u) {
        return 0u;
    }

    using Fn = Address32 (*)(const char* pTypeName);
    return reinterpret_cast<Fn>(static_cast<std::uintptr_t>(pDescriptor->m_pResolveTypeByNameFn))(
        ResolveAddress<const char>(typeNameAddress)
    );
}

Address32 ResolveStructTypeName(const parMemberStructDescriptor32* pDescriptor, Address32 structInstanceAddress) {
    if (pDescriptor == nullptr || pDescriptor->m_pGetTypeNameFn == 0u) {
        return 0u;
    }

    using Fn = Address32 (*)(Address32 structInstanceAddress);
    return reinterpret_cast<Fn>(static_cast<std::uintptr_t>(pDescriptor->m_pGetTypeNameFn))(structInstanceAddress);
}

Address32 CreateStructFromOperator(const parMemberStructDescriptor32* pDescriptor, const rage::cmOperator* pValueOperator) {
    if (pDescriptor == nullptr || pDescriptor->m_pCreateFromOperatorFn == 0u) {
        return 0u;
    }

    using Fn = Address32 (*)(const rage::cmOperator* pValueOperator);
    return reinterpret_cast<Fn>(static_cast<std::uintptr_t>(pDescriptor->m_pCreateFromOperatorFn))(pValueOperator);
}

Address32 FindStructOperatorNode(const rage::cmOperator* pValueOperator, Address32 nodeNameAddress) {
    return static_cast<Address32>(reinterpret_cast<std::uintptr_t>(
        rage_97A8(pValueOperator, ResolveAddress<const char>(nodeNameAddress))
    ));
}

void ResetNestedStructMember(Address32 memberDescriptorAddress, Address32 structInstanceAddress) {
    if (memberDescriptorAddress == 0u) {
        return;
    }

    void* pMemberDescriptor = ResolveAddress<void>(memberDescriptorAddress);
    using Fn = void (*)(void* self, Address32 structInstanceAddress);
    GetVirtualSlot<Fn>(pMemberDescriptor, 8)(pMemberDescriptor, structInstanceAddress);
}

void DeserializeStruct(Address32 structTypeAddress, const rage::cmOperator* pValueOperator, Address32 structInstanceAddress) {
    if (structTypeAddress == 0u) {
        return;
    }

    void* pStructType = ResolveAddress<void>(structTypeAddress);
    using Fn = void (*)(void* self, const rage::cmOperator* pValueOperator, Address32 structInstanceAddress);
    GetVirtualSlot<Fn>(pStructType, 1)(pStructType, pValueOperator, structInstanceAddress);
}

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

/**
 * parMemberArray::vfn_0 @ 0x8234E088 | size: 0x50
 *
 * Scalar destructor wrapper that tears down array serializer state and
 * optionally frees this instance when the low bit in `freeSelf` is set.
 */
void* parMemberArray::vfn_0(std::uint32_t freeSelf) {
    E0D8_h();

    if ((freeSelf & 0x1u) != 0u) {
        rage_free_00C0(this);
    }

    return this;
}

/**
 * parMemberArray::E0D8_h @ 0x8234E0D8 | size: 0x60
 *
 * Drops the owned element serializer object and restores the base
 * rage::parMember vtable pointer.
 */
void parMemberArray::E0D8_h() {
    m_pVtable = reinterpret_cast<void**>(static_cast<std::uintptr_t>(kParMemberArrayVtable));

    if (m_pElementSerializer != nullptr) {
        using ScalarDtorFn = void (*)(void* self, std::uint32_t freeSelf);
        GetVirtualSlot<ScalarDtorFn>(m_pElementSerializer, 0)(m_pElementSerializer, 1u);
    }

    m_pVtable = reinterpret_cast<void**>(static_cast<std::uintptr_t>(kParMemberVtable));
}

/**
 * parMemberArray::E138 @ 0x8234E138 | size: 0x234
 *
 * Computes the backing data address and active element count for an array
 * member based on the descriptor storage mode.
 */
void parMemberArray::E138(
    std::uint32_t memberOffset,
    std::uint32_t* outDataAddress,
    std::uint32_t* outElementCount
) {
    if (outDataAddress == nullptr || outElementCount == nullptr) {
        return;
    }

    *outDataAddress = 0u;
    *outElementCount = 0u;

    parMemberDescriptor32* descriptor = GetArrayDescriptor(this);
    if (descriptor == nullptr) {
        return;
    }

    switch (descriptor->m_storageMode) {
        case 0: {
            const Address32 headerAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            const std::uint16_t elementCount = *ResolveAddress<std::uint16_t>(headerAddress + 4u);
            if (elementCount > 0u) {
                *outDataAddress = *ResolveAddress<Address32>(headerAddress);
                *outElementCount = elementCount;
            }
            return;
        }

        case 1: {
            nop_8240E6D0(ResolveAddress<const char>(kParArrayLegacyProbeString));

            const Address32 memberAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            Address32* pLargeCount = ResolveAddress<Address32>(memberAddress + kParArrayLargeStorageOffset);
            if (*pLargeCount == 0u) {
                *pLargeCount = 0u;
            }

            *outDataAddress = memberAddress;
            *outElementCount = *pLargeCount;
            return;
        }

        case 2:
            *outDataAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            *outElementCount = descriptor->m_elementCapacity;
            return;

        case 3:
            *outDataAddress = *ResolveAddress<Address32>(GetArrayMemberBaseAddress(this) + memberOffset);
            *outElementCount = descriptor->m_elementCapacity;
            return;

        case 4:
            *outDataAddress = descriptor->m_pMemberBase + memberOffset;
            *outElementCount = descriptor->m_elementCapacity;
            return;

        case 5: {
            const Address32 rangeAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            const Address32 rangeBegin = *ResolveAddress<Address32>(rangeAddress);
            const Address32 rangeEnd = *ResolveAddress<Address32>(rangeAddress + 4u);
            const std::uint32_t elementCount = (descriptor->m_elementStride != 0u)
                ? ((rangeEnd - rangeBegin) / descriptor->m_elementStride)
                : 0u;

            if (elementCount != 0u) {
                *outDataAddress = rangeBegin;
                *outElementCount = elementCount;
            }
            return;
        }

        case 6: {
            const Address32 headerAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            const std::uint8_t elementCount = *ResolveAddress<std::uint8_t>(headerAddress + 4u);
            if (elementCount != 0u) {
                *outDataAddress = *ResolveAddress<Address32>(headerAddress);
                *outElementCount = elementCount;
            }
            return;
        }

        case 7: {
            const Address32 headerAddress = GetArrayMemberBaseAddress(this) + memberOffset;
            const std::uint32_t elementCount = *ResolveAddress<std::uint32_t>(headerAddress + 4u);
            if (elementCount != 0u) {
                *outDataAddress = *ResolveAddress<Address32>(headerAddress);
                *outElementCount = elementCount;
            }
            return;
        }

        default:
            return;
    }
}

/**
 * parMemberArray::vfn_6 @ 0x8234E370 | size: 0x394
 *
 * Builds a cmOperator payload for this member. Primitive arrays are copied as
 * a single contiguous payload; complex element types are exported as child
 * operator nodes.
 */
cmOperator* parMemberArray::vfn_6(std::uint32_t memberOffset) {
    xe_main_thread_init_0038();

    MainThreadHeapAllocator* allocator = GetMainThreadHeapAllocator();
    auto* pOperatorData = (allocator != nullptr)
        ? static_cast<cmOperatorArrayPayload32*>(allocator->Allocate(44u, 16u))
        : nullptr;
    if (pOperatorData == nullptr) {
        return nullptr;
    }

    InitializeArrayOperator(pOperatorData);
    AttachOperatorOwner(pOperatorData, GetArrayOwnerAddress(this));

    std::uint32_t memberDataAddress = 0u;
    std::uint32_t elementCount = 0u;
    E138(memberOffset, &memberDataAddress, &elementCount);

    parMemberDescriptor32* descriptor = GetArrayDescriptor(this);
    const std::uint8_t valueType = GetArrayValueType(this);
    if (descriptor == nullptr || valueType > 13u) {
        return reinterpret_cast<cmOperator*>(pOperatorData);
    }

    const Address32 valueTypeLabel = GetArrayTypeLabel(valueType);
    if (valueTypeLabel != 0u) {
        jumptable_9498(
            pOperatorData,
            ResolveAddress<const char>(kParArrayTypeRootLabel),
            ResolveAddress<const char>(valueTypeLabel),
            1u,
            1u
        );
        cmOperatorCtor_D8C0_w(
            pOperatorData,
            ResolveAddress<const void>(memberDataAddress),
            descriptor->m_elementStride * elementCount
        );
        return reinterpret_cast<cmOperator*>(pOperatorData);
    }

    if (m_pElementSerializer == nullptr || elementCount == 0u) {
        return reinterpret_cast<cmOperator*>(pOperatorData);
    }

    for (std::int32_t elementIndex = static_cast<std::int32_t>(elementCount) - 1;
         elementIndex >= 0;
         --elementIndex) {
        SelectElementSerializerEntry(
            m_pElementSerializer,
            descriptor->m_elementStride * static_cast<std::uint32_t>(elementIndex)
        );
        cmOperator* pElementNode = ExportSerializedElement(m_pElementSerializer, memberDataAddress);
        atSingleton_DA18_p46(pElementNode, pOperatorData);
    }

    return reinterpret_cast<cmOperator*>(pOperatorData);
}

/**
 * parMemberArray::vfn_7 @ 0x8234E708 | size: 0x37C
 *
 * Imports cmOperator array data into member storage. It accepts both contiguous
 * payload form and linked child-node form, then applies default fill rules for
 * fixed-capacity array modes.
 */
void parMemberArray::vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset) {
    if (pValueOperator == nullptr) {
        return;
    }

    auto* descriptor = GetArrayDescriptor(this);
    if (descriptor == nullptr) {
        return;
    }

    const auto* sourceOperator = reinterpret_cast<const cmOperatorArrayPayload32*>(pValueOperator);
    const std::uint8_t valueType = GetArrayValueType(this);

    std::uint32_t incomingElementCount = 0u;
    if ((valueType >= 1u && valueType <= 10u) &&
        sourceOperator->m_hasInlineValue != 0u &&
        sourceOperator->m_pFirstChild == 0u &&
        descriptor->m_elementStride != 0u) {
        incomingElementCount = sourceOperator->m_valueSize / descriptor->m_elementStride;
    } else {
        for (Address32 node = sourceOperator->m_pFirstChild; node != 0u;
             node = ResolveAddress<cmOperatorArrayNode32>(node)->m_pNext) {
            ++incomingElementCount;
        }
    }

    if (descriptor->m_storageMode == 0u ||
        descriptor->m_storageMode == 3u ||
        descriptor->m_storageMode == 5u ||
        descriptor->m_storageMode == 6u ||
        descriptor->m_storageMode == 7u) {
        const Address32 memberAddress = GetArrayMemberBaseAddress(this) + memberOffset;
        if (descriptor->m_resizeContext.m_pResizeFn != 0u) {
            using ResizeFn = void (*)(parArrayResizeContext32* self, Address32 memberData, std::uint32_t elementCount);
            auto* resizeFn = reinterpret_cast<ResizeFn>(
                static_cast<std::uintptr_t>(descriptor->m_resizeContext.m_pResizeFn)
            );
            resizeFn(&descriptor->m_resizeContext, memberAddress, incomingElementCount);
        }
    }

    std::uint32_t memberDataAddress = 0u;
    std::uint32_t memberElementCount = 0u;
    E138(memberOffset, &memberDataAddress, &memberElementCount);

    const std::uint8_t resolvedValueType = GetArrayValueType(this);
    if (resolvedValueType > 13u) {
        return;
    }

    const bool requiresFixedTailFill = (jumptable_E058_h(this) != 0u);

    switch (resolvedValueType) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10: {
            if (sourceOperator->m_hasInlineValue != 0u && sourceOperator->m_pFirstChild == 0u) {
                const std::uint32_t memberCapacityBytes = descriptor->m_elementStride * memberElementCount;
                const std::uint32_t copiedBytes = std::min(sourceOperator->m_valueSize, memberCapacityBytes);
                std::memcpy(
                    ResolveAddress<void>(memberDataAddress),
                    ResolveAddress<const void>(sourceOperator->m_pValueBuffer),
                    copiedBytes
                );

                if (!requiresFixedTailFill ||
                    sourceOperator->m_valueSize >= memberCapacityBytes ||
                    descriptor->m_elementStride == 0u ||
                    m_pElementSerializer == nullptr) {
                    return;
                }

                std::uint32_t fillIndex = sourceOperator->m_valueSize / descriptor->m_elementStride;
                if (fillIndex >= descriptor->m_elementCapacity) {
                    return;
                }

                for (; fillIndex < descriptor->m_elementCapacity; ++fillIndex) {
                    SelectElementSerializerEntry(
                        m_pElementSerializer,
                        descriptor->m_elementStride * fillIndex
                    );
                    RestoreSerializerDefault(m_pElementSerializer, memberDataAddress);
                }
                return;
            }
            break;
        }

        default:
            break;
    }

    if (m_pElementSerializer == nullptr) {
        return;
    }

    Address32 nodeAddress = sourceOperator->m_pFirstChild;
    std::uint32_t elementIndex = 0u;
    while (nodeAddress != 0u) {
        SelectElementSerializerEntry(
            m_pElementSerializer,
            descriptor->m_elementStride * elementIndex
        );
        RestoreSerializerDefault(m_pElementSerializer, memberDataAddress);
        ImportElementFromNode(
            m_pElementSerializer,
            ResolveAddress<const cmOperator>(nodeAddress),
            memberDataAddress
        );

        ++elementIndex;
        if (requiresFixedTailFill && elementIndex == descriptor->m_elementCapacity) {
            break;
        }

        nodeAddress = ResolveAddress<cmOperatorArrayNode32>(nodeAddress)->m_pNext;
    }

    if (!requiresFixedTailFill || elementIndex >= descriptor->m_elementCapacity) {
        return;
    }

    for (; elementIndex < descriptor->m_elementCapacity; ++elementIndex) {
        SelectElementSerializerEntry(
            m_pElementSerializer,
            descriptor->m_elementStride * elementIndex
        );
        RestoreSerializerDefault(m_pElementSerializer, memberDataAddress);
    }
}

/**
 * parMemberStruct_BBC0 @ 0x8234BBC0 | size: 0x6C
 *
 * Looks up a struct-type descriptor by name in the global par registry hash.
 * Names prefixed with "__" are normalized to match the stored key form.
 */
std::uint32_t parMemberStruct::BBC0(std::uint32_t structRegistryAddress, const char* pTypeName) {
    if (structRegistryAddress == 0u || pTypeName == nullptr) {
        return 0u;
    }

    const char* pLookupName = pTypeName;
    if (pLookupName[0] == '_' && pLookupName[1] == '_') {
        pLookupName += 2;
    }

    const Address32 slotAddress = static_cast<Address32>(reinterpret_cast<std::uintptr_t>(
        phMaterialMgrImpl_C208_g(ResolveAddress<void>(structRegistryAddress + 16u), pLookupName)
    ));
    if (slotAddress == 0u) {
        return 0u;
    }

    const Address32 entryAddress = *ResolveAddress<Address32>(slotAddress);
    if (entryAddress == 0u) {
        return 0u;
    }

    return *ResolveAddress<Address32>(entryAddress);
}

/**
 * parMemberStruct_D620 @ 0x8234D620 | size: 0x60
 *
 * Applies each child member's reset/default routine to the target struct
 * instance address.
 */
void parMemberStruct::D620(std::uint32_t structTypeAddress, std::uint32_t structInstanceAddress) {
    if (structTypeAddress == 0u) {
        return;
    }

    const std::uint16_t childCount = *ResolveAddress<std::uint16_t>(structTypeAddress + 20u);
    const Address32 childArrayAddress = *ResolveAddress<Address32>(structTypeAddress + 16u);

    for (std::uint32_t childIndex = 0u; childIndex < childCount; ++childIndex) {
        const Address32 childMemberAddress = *ResolveAddress<Address32>(childArrayAddress + (childIndex * 4u));
        ResetNestedStructMember(childMemberAddress, structInstanceAddress);
    }
}

/**
 * parMemberStruct_EA88 @ 0x8234EA88 | size: 0x88
 *
 * Resolves the effective struct-type descriptor for this member, either via a
 * direct descriptor pointer flag or via global name lookup.
 */
std::uint32_t parMemberStruct::EA88() {
    auto* pDescriptor = GetStructDescriptor(this);
    if (pDescriptor == nullptr) {
        return 0u;
    }

    if (HasStructFlag(this, kParStructFlagDirectType)) {
        return pDescriptor->m_pDirectStructType;
    }

    const Address32 structRegistry = GetParStructRegistry();
    if (structRegistry == 0u) {
        return 0u;
    }

    return BBC0(structRegistry, ResolveAddress<const char>(pDescriptor->m_pStructName));
}

/**
 * parMemberStruct_EB10 @ 0x8234EB10 | size: 0xBC
 *
 * Resolves the runtime struct type for a member slot. Pointer-backed members
 * route through the type's context resolver callback.
 */
std::uint32_t parMemberStruct::EB10(std::uint32_t memberOffset) {
    const Address32 structTypeAddress = EA88();
    if (!HasStructFlag(this, kParStructFlagIndirectStorage)) {
        return structTypeAddress;
    }

    const Address32 memberSlotAddress = GetStructMemberBaseAddress(this) + memberOffset;
    const Address32 structInstanceAddress = *ResolveAddress<Address32>(memberSlotAddress);
    if (structInstanceAddress == 0u || structTypeAddress == 0u) {
        return 0u;
    }

    const Address32 resolvedTypeAddress = InvokeContextCall1(structTypeAddress + 44u, structInstanceAddress);
    (void)GetStructFlags(this);
    return resolvedTypeAddress;
}

/**
 * parMemberStruct::vfn_6 @ 0x8234EBD0 | size: 0x340
 *
 * Serializes the member as a cmOperator node. For pointer-backed polymorphic
 * members this emits type-identification labels before serializing children.
 */
cmOperator* parMemberStruct::vfn_6(std::uint32_t memberOffset) {
    xe_main_thread_init_0038();

    MainThreadHeapAllocator* allocator = GetMainThreadHeapAllocator();
    auto* pOperatorData = (allocator != nullptr)
        ? static_cast<cmOperatorArrayPayload32*>(allocator->Allocate(44u, 16u))
        : nullptr;
    if (pOperatorData == nullptr) {
        return nullptr;
    }

    InitializeArrayOperator(pOperatorData);
    util_9410(pOperatorData, ResolveAddress<void>(GetStructOwnerAddress(this)), 0u);

    const bool isIndirectMember = HasStructFlag(this, kParStructFlagIndirectStorage);
    Address32 structInstanceAddress = GetStructMemberBaseAddress(this) + memberOffset;
    if (isIndirectMember) {
        structInstanceAddress = *ResolveAddress<Address32>(structInstanceAddress);
    }

    if (isIndirectMember) {
        if (GetStructStorageMode(this) == 1u) {
            if (structInstanceAddress == 0u) {
                if (!HasStructFlag(this, kParStructFlagNullSentinel)) {
                    jumptable_9498(
                        pOperatorData,
                        ResolveAddress<const char>(kParStructNullTypeNameAddr),
                        ResolveAddress<const char>(kParStructTypeNodeKeyAddr),
                        0u,
                        0u
                    );
                    return reinterpret_cast<cmOperator*>(pOperatorData);
                }
            }

            const Address32 typeNameAddress = ResolveStructTypeName(GetStructDescriptor(this), structInstanceAddress);
            jumptable_9498(
                pOperatorData,
                ResolveAddress<const char>(typeNameAddress),
                ResolveAddress<const char>(kParStructTypeNodeKeyAddr),
                0u,
                1u
            );
            return reinterpret_cast<cmOperator*>(pOperatorData);
        }

        if (structInstanceAddress == 0u) {
            jumptable_9498(
                pOperatorData,
                ResolveAddress<const char>(kParStructNullTypeNameAddr),
                ResolveAddress<const char>(kParStructFactoryNodeKeyAddr),
                0u,
                0u
            );
            return reinterpret_cast<cmOperator*>(pOperatorData);
        }
    }

    const Address32 structTypeAddress = EB10(memberOffset);
    if (structTypeAddress == 0u) {
        return reinterpret_cast<cmOperator*>(pOperatorData);
    }

    auto* rawOperator = reinterpret_cast<std::uint8_t*>(pOperatorData);
    const std::uint16_t existingTypeWord = *reinterpret_cast<std::uint16_t*>(rawOperator + 18u);
    const std::uint32_t structTypeWord = *ResolveAddress<std::uint32_t>(structTypeAddress + 24u);
    *reinterpret_cast<std::uint32_t*>(rawOperator + 16u) =
        (structTypeWord & 0xFFFF0000u) | existingTypeWord;
    *reinterpret_cast<std::uint16_t*>(rawOperator + 18u) =
        static_cast<std::uint16_t>(structTypeWord);

    if (isIndirectMember &&
        GetStructStorageMode(this) != 1u &&
        structInstanceAddress != 0u) {
        const Address32 typeNameAddress = *ResolveAddress<Address32>(structTypeAddress + 4u);
        jumptable_9498(
            pOperatorData,
            ResolveAddress<const char>(typeNameAddress),
            ResolveAddress<const char>(kParStructFactoryNodeKeyAddr),
            1u,
            1u
        );
    }

    (void)GetStructFlags(this);
    atSingleton_D210_p46(
        ResolveAddress<void>(structTypeAddress),
        ResolveAddress<void>(structInstanceAddress),
        pOperatorData
    );
    return reinterpret_cast<cmOperator*>(pOperatorData);
}

/**
 * parMemberStruct::vfn_7 @ 0x8234EF10 | size: 0x1DC
 *
 * Deserializes a cmOperator node into this member, handling polymorphic
 * pointer members and lazily creating target instances when required.
 */
void parMemberStruct::vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset) {
    if (pValueOperator == nullptr) {
        return;
    }

    if (HasStructFlag(this, kParStructFlagIndirectStorage)) {
        const Address32 memberSlotAddress = GetStructMemberBaseAddress(this) + memberOffset;
        Address32* pStructInstanceSlot = ResolveAddress<Address32>(memberSlotAddress);

        if (GetStructStorageMode(this) == 1u) {
            const Address32 typeNodeAddress = FindStructOperatorNode(pValueOperator, kParStructTypeNodeKeyAddr);
            if (typeNodeAddress == 0u) {
                *pStructInstanceSlot = 0u;
                return;
            }

            const Address32 typeNameAddress = *ResolveAddress<Address32>(typeNodeAddress + 4u);
            if (!HasStructFlag(this, kParStructFlagNullSentinel) &&
                AreStructNamesEqual(typeNameAddress, kParStructNullTypeNameAddr)) {
                *pStructInstanceSlot = 0u;
                return;
            }

            *pStructInstanceSlot = ResolveStructTypeByName(GetStructDescriptor(this), typeNameAddress);
            return;
        }

        if (*pStructInstanceSlot == 0u) {
            *pStructInstanceSlot = vfn_9(pValueOperator, memberOffset);
            if (*pStructInstanceSlot == 0u) {
                return;
            }
        }

        const Address32 structTypeAddress = EB10(memberOffset);
        DeserializeStruct(structTypeAddress, pValueOperator, *pStructInstanceSlot);
        return;
    }

    const Address32 structTypeAddress = EA88();
    const Address32 structInstanceAddress = GetStructMemberBaseAddress(this) + memberOffset;
    DeserializeStruct(structTypeAddress, pValueOperator, structInstanceAddress);
}

/**
 * parMemberStruct::vfn_8 @ 0x8234F0F0 | size: 0x94
 *
 * Writes the default value for this member. Pointer-backed members are reset
 * to null; direct members recurse through child-member defaults.
 */
void parMemberStruct::vfn_8(std::uint32_t memberOffset) {
    if (HasStructFlag(this, kParStructFlagIndirectStorage)) {
        *ResolveAddress<Address32>(GetStructMemberBaseAddress(this) + memberOffset) = 0u;
        return;
    }

    const Address32 structTypeAddress = EA88();
    const Address32 structInstanceAddress = GetStructMemberBaseAddress(this) + memberOffset;
    D620(structTypeAddress, structInstanceAddress);
}

/**
 * parMemberStruct::vfn_9 @ 0x8234F188 | size: 0x1D4
 *
 * Returns the target struct instance for a member slot, creating a new
 * instance for pointer-backed members when the incoming operator requests one.
 */
std::uint32_t parMemberStruct::vfn_9(const cmOperator* pValueOperator, std::uint32_t memberOffset) {
    if (!HasStructFlag(this, kParStructFlagIndirectStorage)) {
        return GetStructMemberBaseAddress(this) + memberOffset;
    }

    const Address32 factoryNodeAddress = FindStructOperatorNode(pValueOperator, kParStructFactoryNodeKeyAddr);
    if (factoryNodeAddress == 0u) {
        return 0u;
    }

    const Address32 structTypeNameAddress = *ResolveAddress<Address32>(factoryNodeAddress + 4u);
    if (AreStructNamesEqual(structTypeNameAddress, kParStructNullTypeNameAddr)) {
        return 0u;
    }

    (void)EA88();
    (void)GetStructFlags(this);

    Address32 structTypeAddress = BBC0(
        GetParStructRegistry(),
        ResolveAddress<const char>(structTypeNameAddress)
    );

    const parMemberStructDescriptor32* pDescriptor = GetStructDescriptor(this);
    if (pDescriptor != nullptr &&
        (pDescriptor->m_flags & kParStructFlagFactoryCreate) != 0u &&
        structTypeAddress == 0u &&
        pDescriptor->m_pCreateFromOperatorFn != 0u) {
        Address32* pMemberSlot = ResolveAddress<Address32>(GetStructMemberBaseAddress(this) + memberOffset);
        *pMemberSlot = CreateStructFromOperator(pDescriptor, pValueOperator);
        return *pMemberSlot;
    }

    if (structTypeAddress == 0u) {
        return 0u;
    }

    const Address32 structInstanceAddress = InvokeContextCall0(structTypeAddress + 28u);
    D620(structTypeAddress, structInstanceAddress);

    Address32* pMemberSlot = ResolveAddress<Address32>(GetStructMemberBaseAddress(this) + memberOffset);
    *pMemberSlot = structInstanceAddress;
    return *pMemberSlot;
}

} // namespace rage
