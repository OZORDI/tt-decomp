/**
 * rage_stream_manager.cpp — stream buffer reset and tokenizer bootstrap logic
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#include "rage/rage_stream_manager.hpp"

namespace rage {

namespace {

int FlushPendingStreamWindow(fiStreamBuf* pStreamBuffer) {
    if (pStreamBuffer->m_bufferedEndPosition != 0) {
        const std::int32_t bufferedEnd = pStreamBuffer->m_bufferedEndPosition;
        const std::int32_t readPosition = pStreamBuffer->m_readPosition;

        if (bufferedEnd != readPosition) {
            const std::int32_t rangeSize = bufferedEnd - readPosition;
            std::uint8_t* pRangeStart = pStreamBuffer->m_pBuffer + readPosition;

            if (pStreamBuffer->m_pVTable->m_pCommitRange(
                    pStreamBuffer,
                    pStreamBuffer->m_flags,
                    pRangeStart,
                    0,
                    rangeSize) != 0) {
                return -1;
            }
        } else {
            if (pStreamBuffer->m_pVTable->m_pFlushBuffer(
                    pStreamBuffer,
                    pStreamBuffer->m_flags,
                    pStreamBuffer->m_pBuffer) != 0) {
                return -1;
            }
        }

        pStreamBuffer->m_readPosition = 0;
        pStreamBuffer->m_bufferedEndPosition = 0;
        pStreamBuffer->m_writePosition += bufferedEnd;
    } else if (pStreamBuffer->m_readPosition != 0) {
        const std::int32_t readPosition = pStreamBuffer->m_readPosition;

        if (pStreamBuffer->m_pVTable->m_pFlushBuffer(
                pStreamBuffer,
                pStreamBuffer->m_flags,
                pStreamBuffer->m_pBuffer) != 0) {
            return -1;
        }

        pStreamBuffer->m_readPosition = 0;
        pStreamBuffer->m_bufferedEndPosition = 0;
        pStreamBuffer->m_writePosition += readPosition;
    } else {
        return 0;
    }

    pStreamBuffer->m_pVTable->m_pOnTransferComplete(pStreamBuffer, pStreamBuffer->m_flags);
    return 0;
}

} // namespace

int ResetStreamBuffer(fiStreamBuf* pStreamBuffer, std::int32_t resetWritePosition) {
    if (pStreamBuffer == nullptr || pStreamBuffer->m_pVTable == nullptr) {
        return -1;
    }

    if (FlushPendingStreamWindow(pStreamBuffer) < 0) {
        return -1;
    }

    pStreamBuffer->m_writePosition = resetWritePosition;
    pStreamBuffer->m_pVTable->m_pCommitRange(pStreamBuffer, pStreamBuffer->m_flags, nullptr, 0, 0);
    return 0;
}

void InitializeAsciiTokenizerState(fiAsciiTokenizerState* pTokenizerState) {
    if (pTokenizerState == nullptr) {
        return;
    }

    if (pTokenizerState->m_pInputStream != nullptr) {
        ResetStreamBuffer(pTokenizerState->m_pInputStream, 0);
    }

    pTokenizerState->m_pushbackCount = 0;
    pTokenizerState->m_lineNumber = 1;
    pTokenizerState->m_currentChar = ' ';
    pTokenizerState->m_aPushbackBuffer[0] = '\0';
    pTokenizerState->m_parseScratchState = 0;
    pTokenizerState->m_parseMode = 2;
}

} // namespace rage

extern "C" int fiStreamBuf_Reset(rage::fiStreamBuf* pBuf, int resetParam) {
    return rage::ResetStreamBuffer(pBuf, resetParam);
}

extern "C" void rage_52B0_1(rage::fiAsciiTokenizerState* pTokenizerState) {
    rage::InitializeAsciiTokenizerState(pTokenizerState);
}
