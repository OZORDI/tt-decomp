#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * io_Input_poll
 * @ 0x821C9D68 | size: 0x5C
 * 
 * Poll input device readiness.
 * Returns true if input is ready to be read.
 */
bool io_Input_poll(void);

#ifdef __cplusplus
}
#endif
