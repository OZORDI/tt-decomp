#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * io_Input_poll
 * @ 0x821C9D68 | size: 0x5C
 *
 * Poll input device readiness.
 * pInput: pointer to the ioInput singleton (@ 0x825EAB28).
 * Returns non-zero if input is ready to be read.
 */
uint8_t io_Input_poll(void* pInput);

#ifdef __cplusplus
}
#endif
