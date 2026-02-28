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

} // namespace rage
