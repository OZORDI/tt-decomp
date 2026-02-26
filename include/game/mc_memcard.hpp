/**
 * mc_memcard.hpp - Memory Card System
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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
