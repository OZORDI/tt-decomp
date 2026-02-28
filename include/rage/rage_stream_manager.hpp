/**
 * rage_stream_manager.hpp — stream buffer reset and tokenizer bootstrap helpers
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */
#pragma once

#include <cstdint>

namespace rage {

struct fiStreamBuf;

struct fiStreamBufVTable {
    void (*m_pReserved0)();
    void (*m_pReserved1)();
    void (*m_pReserved2)();

    int (*m_pReadFromDevice)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t streamFlags,
        std::uint8_t* pDestination,
        std::int32_t bytesToRead,
        std::int32_t streamCapacity
    );

    int (*m_pFlushBuffer)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t streamFlags,
        std::uint8_t* pBufferBase
    );

    int (*m_pCommitRange)(
        fiStreamBuf* pStreamBuffer,
        std::uint32_t streamFlags,
        std::uint8_t* pRangeStart,
        std::int32_t rangeOffset,
        std::int32_t rangeSize
    );

    void (*m_pClose)(fiStreamBuf* pStreamBuffer, std::uint32_t streamFlags);
    void (*m_pReserved7)();
    void (*m_pOnTransferComplete)(fiStreamBuf* pStreamBuffer, std::uint32_t streamFlags);
};

struct fiStreamBuf {
    fiStreamBufVTable* m_pVTable;
    std::uint32_t m_flags;
    std::uint8_t* m_pBuffer;
    std::int32_t m_writePosition;
    std::int32_t m_readPosition;
    std::int32_t m_bufferedEndPosition;
    std::int32_t m_capacityBytes;
};

struct fiAsciiTokenizerState {
    std::uint32_t m_objectWord0;
    std::uint32_t m_streamOpenFlags;
    std::int32_t m_lineNumber;
    fiStreamBuf* m_pInputStream;
    std::int32_t m_currentChar;
    std::int32_t m_parseMode;
    std::int32_t m_pushbackCount;
    char m_aPushbackBuffer[128];
    std::uint32_t m_parseScratchState;
};

int ResetStreamBuffer(fiStreamBuf* pStreamBuffer, std::int32_t resetWritePosition);
void InitializeAsciiTokenizerState(fiAsciiTokenizerState* pTokenizerState);

} // namespace rage

extern "C" int fiStreamBuf_Reset(rage::fiStreamBuf* pBuf, int resetParam);
extern "C" void rage_52B0_1(rage::fiAsciiTokenizerState* pTokenizerState);
