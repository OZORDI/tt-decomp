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

} // namespace rage
