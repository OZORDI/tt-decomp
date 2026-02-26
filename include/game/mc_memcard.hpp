/**
 * mc_memcard.hpp - Memory Card System
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 */

#pragma once

#include <stdint.h>

namespace rage {
    class datBase;
}

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
struct mcMemcardControl;

/**
 * mcSegmentContainer
 * 
 * Container for memory card segment data. Inherits from rage::datBase.
 * Manages a dynamically allocated buffer for segment content.
 * 
 * Vtable @ 0x8204D9B0
 */
struct mcSegmentContainer /* : rage::datBase */ {
    void*    m_pVtable;        // +0x00  vtable pointer
    void*    m_pSegmentData;   // +0x04  dynamically allocated segment buffer
    uint32_t m_unk08;          // +0x08  unknown field
    uint16_t m_bHasData;       // +0x0A  flag: 1 if m_pSegmentData is allocated
    // ... additional fields
};

// Memory card control structure
typedef struct mcMemcardControl {
    uint32_t state;
    uint32_t flags;
} mcMemcardControl;

// Memory card functions
void mcMemcardControl_Init(mcMemcardControl* ctrl);
void mcMemcardControl_Shutdown(mcMemcardControl* ctrl);
void mcMemcardControl_Reset(mcMemcardControl* ctrl);
void mcMemcardControl_RequestOp(mcMemcardControl* ctrl, uint32_t op);

#ifdef __cplusplus
}
#endif
