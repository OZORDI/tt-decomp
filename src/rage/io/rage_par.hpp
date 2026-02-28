/**
 * rage_par.hpp — par runtime structure declarations
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace rage {

/**
 * rage::parRTStructure @ 0x821C5090
 *
 * Runtime serialization container used by the save/profile bootstrap path.
 * The constructor initializes five slot records and two UTF-16 labels.
 */
struct parRTStructure {
    static constexpr std::size_t kSlotCount = 5;
    static constexpr std::size_t kSlotStride = 0x3AC4;
    static constexpr std::size_t kPathChars = 129;

    std::uint32_t m_primaryVtable;
    std::uint32_t m_payloadPtr;
    std::uint16_t m_payloadCount;
    std::uint16_t m_payloadFlags;
    std::uint32_t m_secondaryVtable;

    std::array<char16_t, kPathChars> m_primaryPathUtf16;
    std::array<char16_t, kPathChars> m_secondaryPathUtf16;

    std::uint32_t m_slotRegistrationToken;
    std::array<std::byte, kSlotCount * kSlotStride> m_slotStorage;
    std::uint32_t m_runtimeHandle;
    std::uint8_t m_runtimeEnabled;
    std::uint8_t m_pad127F1[3];

    /**
     * parRTStructure::parRTStructure() @ 0x821C5090 | size: 0x180
     *
     * Builds five slot descriptors, registers this object in the global save
     * slot registry, and caches two UTF-16 labels for downstream lookups.
     */
    parRTStructure();
};

static_assert(offsetof(parRTStructure, m_slotStorage) == 0x218, "parRTStructure slot storage offset mismatch");
static_assert(offsetof(parRTStructure, m_runtimeHandle) == 0x127EC, "parRTStructure runtime handle offset mismatch");
static_assert(sizeof(parRTStructure) == 0x127F4, "parRTStructure size mismatch");

struct cmOperator;

/**
 * rage::parMemberString @ 0x82065AF0
 *
 * String-specialized parameter member descriptor used by the par serialization
 * system. These overrides build editor operators, import string values, and
 * reset storage to the empty-string default.
 */
struct parMemberString {
    void** m_pVtable;        // +0x00
    void* m_pMemberDesc;     // +0x04

    /**
     * parMemberString::vfn_6 @ 0x8234F408 | size: 0x138
     * Build a cmOperator from the current string value at memberOffset.
     */
    cmOperator* vfn_6(std::uint32_t memberOffset);

    /**
     * parMemberString::vfn_7 @ 0x8234F540 | size: 0x150
     * Apply the incoming cmOperator string payload into member storage.
     */
    void vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset);

    /**
     * parMemberString::vfn_8 @ 0x8234F360 | size: 0xA4
     * Reset member storage to an empty string/null pointer default.
     */
    void vfn_8(std::uint32_t memberOffset);
};

/**
 * rage::parMemberArray @ 0x82065BD0
 *
 * Array-specialized parameter descriptor used by the par serialization layer.
 * It resolves backing storage for array fields and marshals cmOperator lists.
 */
struct parMemberArray {
    void** m_pVtable;             // +0x00
    void* m_pMemberDesc;          // +0x04
    void* m_pElementSerializer;   // +0x08

    /**
     * parMemberArray::vfn_0 @ 0x8234E088 | size: 0x50
     * Scalar destructor with optional self-free.
     */
    void* vfn_0(std::uint32_t freeSelf);

    /**
     * parMemberArray::E0D8_h @ 0x8234E0D8 | size: 0x60
     * Releases the array serializer sub-object and restores base vtable state.
     */
    void E0D8_h();

    /**
     * parMemberArray::E138 @ 0x8234E138 | size: 0x234
     * Resolves the array data address and logical element count for a member.
     */
    void E138(
        std::uint32_t memberOffset,
        std::uint32_t* outDataAddress,
        std::uint32_t* outElementCount
    );

    /**
     * parMemberArray::vfn_6 @ 0x8234E370 | size: 0x394
     * Exports the array member into a cmOperator payload/list representation.
     */
    cmOperator* vfn_6(std::uint32_t memberOffset);

    /**
     * parMemberArray::vfn_7 @ 0x8234E708 | size: 0x37C
     * Imports cmOperator array data/list nodes back into member storage.
     */
    void vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset);
};

/**
 * rage::parMemberStruct @ 0x82065AAC
 *
 * Struct-specialized parameter descriptor. It resolves runtime struct types,
 * marshals nested members through cmOperator trees, and handles pointer-backed
 * polymorphic struct instances.
 */
struct parMemberStruct {
    void** m_pVtable;        // +0x00
    void* m_pMemberDesc;     // +0x04

    /**
     * parMemberStruct_BBC0 @ 0x8234BBC0 | size: 0x6C
     * Resolve a struct type by name through the global par structure registry.
     */
    static std::uint32_t BBC0(std::uint32_t structRegistryAddress, const char* pTypeName);

    /**
     * parMemberStruct_D620 @ 0x8234D620 | size: 0x60
     * Reset every child member of the struct type into the target instance.
     */
    static void D620(std::uint32_t structTypeAddress, std::uint32_t structInstanceAddress);

    /**
     * parMemberStruct_EA88 @ 0x8234EA88 | size: 0x88
     * Resolve the effective struct type descriptor for this member.
     */
    std::uint32_t EA88();

    /**
     * parMemberStruct_EB10 @ 0x8234EB10 | size: 0xBC
     * Resolve the runtime struct type for a member slot (handles indirection).
     */
    std::uint32_t EB10(std::uint32_t memberOffset);

    /**
     * parMemberStruct::vfn_6 @ 0x8234EBD0 | size: 0x340
     * Export this struct member into a cmOperator representation.
     */
    cmOperator* vfn_6(std::uint32_t memberOffset);

    /**
     * parMemberStruct::vfn_7 @ 0x8234EF10 | size: 0x1DC
     * Import a cmOperator representation back into this struct member.
     */
    void vfn_7(const cmOperator* pValueOperator, std::uint32_t memberOffset);

    /**
     * parMemberStruct::vfn_8 @ 0x8234F0F0 | size: 0x94
     * Reset struct member storage to its default state.
     */
    void vfn_8(std::uint32_t memberOffset);

    /**
     * parMemberStruct::vfn_9 @ 0x8234F188 | size: 0x1D4
     * Acquire/create the target struct instance for this member slot.
     */
    std::uint32_t vfn_9(const cmOperator* pValueOperator, std::uint32_t memberOffset);
};

} // namespace rage

/**
 * MemberDescriptor — internal descriptor structure
 * Contains type information and default values for simple types
 */
struct MemberDescriptor {
    uint32_t m_typeId;              // +0x00
    uint32_t m_defaultValue;        // +0x04
    uint16_t m_dataType;            // +0x08
    uint8_t m_formatFlags;          // +0x0A
    uint8_t m_additionalFlags;      // +0x0B
    float m_defaultValueFloat;      // +0x0C
};

/**
 * rage::parMemberSimple @ 0x82065A68
 *
 * Simple-type parameter member descriptor for the par serialization system.
 * Handles primitive types (bool, int8, uint8, int16, uint16, int32, uint32, float)
 * and simple vector types (vec2, vec3, vec4) with type conversion and cmOperator
 * marshaling for editor integration.
 */
struct parMemberSimple {
    void** m_pVtable;                // +0x00
    MemberDescriptor* m_pMemberDesc; // +0x04

    /**
     * parMemberSimple::~parMemberSimple() @ 0x8234CA38 | size: 0x50
     * Destructor with optional self-free (vtable slot 0)
     */
    void* vfn_0(uint32_t freeSelf);

    /**
     * parMemberSimple::vfn_1 @ 0x8234F690 | size: 0xC
     * Get type identifier from member descriptor (vtable slot 1)
     */
    uint32_t vfn_1() const;

    /**
     * parMemberSimple::vfn_2 @ 0x8234F6A0 | size: 0xC
     * Get default value from member descriptor (vtable slot 2)
     */
    uint32_t vfn_2() const;

    /**
     * parMemberSimple::vfn_3 @ 0x8234F6B0 | size: 0xC
     * Set default value in member descriptor (vtable slot 3)
     */
    void vfn_3(uint32_t value);

    /**
     * parMemberSimple::vfn_4 @ 0x8234F6C0 | size: 0xC
     * Get data type from member descriptor (vtable slot 4)
     */
    uint16_t vfn_4() const;

    /**
     * parMemberSimple::vfn_5 @ 0x8234F6D0 | size: 0xC
     * Get data format flags from member descriptor (vtable slot 5)
     */
    uint8_t vfn_5() const;

    /**
     * parMemberSimple::vfn_6 @ 0x8234FA48 | size: 0x3FC
     * Export member value to cmOperator representation (vtable slot 6)
     */
    cmOperator* vfn_6(uint32_t memberOffset);

    /**
     * parMemberSimple::vfn_7 @ 0x8234FE48 | size: 0x390
     * Import cmOperator value into member storage (vtable slot 7)
     */
    void vfn_7(const cmOperator* pValueOperator, uint32_t memberOffset);

    /**
     * parMemberSimple::vfn_8 @ 0x8234F778 | size: 0x2CC
     * Write value to member storage with type conversion (vtable slot 8)
     */
    void vfn_8(uint32_t memberOffset);

    /**
     * parMemberSimple::vfn_10 @ 0x8234F6E0 | size: 0xC
     * Get additional flags from member descriptor (vtable slot 10)
     */
    uint8_t vfn_10() const;
};

} // namespace rage
