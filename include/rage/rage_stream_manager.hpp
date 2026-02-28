/**
 * rage_stream_manager.hpp — stream buffer reset and tokenizer bootstrap helpers
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <cstdint>

namespace rage {

struct fiStreamBuf;

struct fiStreamBufVTable {
    void* m_aLifecycleSlots[3];

    int (*m_pRead)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t openFlags,
        std::uint8_t* pDestination,
        std::int32_t bytesToRead,
        std::int32_t streamCapacity
    );

    int (*m_pFlushBufferedWindow)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t openFlags,
        std::uint8_t* pBufferBase
    );

    int (*m_pCommitWriteWindow)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t openFlags,
        std::uint8_t* pRangeStart,
        std::int32_t rangeOffset,
        std::int32_t rangeSize
    );

    void (*m_pCloseStream)(fiStreamBuf* pStreamBuffer, std::uint32_t openFlags);
    void* m_pReservedDispatchSlot;
    void (*m_pOnTransferComplete)(fiStreamBuf* pStreamBuffer, std::uint32_t openFlags);
};

struct fiStreamBuf {
    fiStreamBufVTable* m_pVTable;
    std::uint32_t m_openFlags;
    std::uint8_t* m_pDataWindow;
    std::int32_t m_absoluteWriteOffset;
    std::int32_t m_windowReadOffset;
    std::int32_t m_windowEndOffset;
    std::int32_t m_windowCapacity;
};

struct fiAsciiTokenizerRuntimeState {
    void* m_pVTable;
    std::uint32_t m_objectFlags;
    std::int32_t m_lineNumber;
    fiStreamBuf* m_pStreamBuffer;
    std::int32_t m_currentChar;
    std::int32_t m_newlineEmitMode;
    std::int32_t m_pushbackDepth;
    char m_aPushbackChars[128];
    const char* m_pAdditionalTokenDelimiters;
    std::int32_t m_indentDepth;
};

int ResetStreamBuffer(fiStreamBuf* pStreamBuffer, std::int32_t resetWritePosition);
void ResetAsciiTokenizerRuntimeState(fiAsciiTokenizerRuntimeState* pTokenizerState);

} // namespace rage

extern "C" int fiStreamBuf_Reset(rage::fiStreamBuf* pBuf, int resetParam);
extern "C" void fiAsciiTokenizer_ResetState(
    rage::fiAsciiTokenizerRuntimeState* pTokenizerState
) __asm__("rage_52B0_1");
