/**
 * mc_memcard.hpp — Memory card save-system state machine
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#pragma once

#include <cstdint>

struct mcMemcardState {
    void* m_pVtable;
};

struct mcMemcardTypeRegistration {
    const void*   m_pTypeInfo;
    const void*   m_pTypeOps;
    void*         m_pTypeStorage;
    std::uint32_t m_typeStorageSize;
};

struct mcMemcardSaveOperation {
    void*         m_pVtable;
    std::int32_t  m_deviceResultCode;
    std::int32_t  m_operationResultCode;
    bool          m_hasPendingRequest;
    std::uint8_t  m_aContentData[4936];
    char          m_szContentFileName[43];
};

struct fsmMachine {
    void*            m_pVtable;
    std::int32_t     m_registeredStateCount;
    mcMemcardState** m_ppRegisteredStates;

    ~fsmMachine();
};

struct mcMemcardControl : fsmMachine {
    enum State : std::int32_t {
        STATE_IDLE = 0,
        STATE_FIND,
        STATE_SELECT,
        STATE_LOAD,
        STATE_SAVE,
        STATE_SAVE_ICON,
        STATE_SEARCH,
        STATE_REMOVE,
        STATE_GET_FREE_SPACE,
        STATE_FORMAT,
        STATE_UNFORMAT,
    };

    struct StateRecord : mcMemcardState {
        std::int32_t m_statePhase;
    };

    struct ExtendedStateRecord : mcMemcardState {
        std::int32_t m_statePhase;
        std::int32_t m_stateToken;
    };

    std::int32_t              m_activeState;
    std::int32_t              m_requestedState;
    std::int32_t              m_primaryResultCode;
    std::int32_t              m_secondaryResultCode;
    mcMemcardTypeRegistration m_typeRegistration;
    mcMemcardSaveOperation    m_saveOperation;

    ~mcMemcardControl();
    void Init();
    void Shutdown();
    void Reset();
    bool IsTransitionAllowed(std::int32_t targetState) const;
    void RequestOp();
};

struct mcSegmentContainer {
    void*         m_pVtable;
    void*         m_pSegmentData;
    std::uint32_t m_segmentCount;
    std::uint16_t m_hasSegmentData;

    ~mcSegmentContainer();
};

struct mcFileSegment {};
