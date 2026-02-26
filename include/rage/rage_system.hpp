/**
 * rage_system.hpp - RAGE Engine System Header
 * 
 * Core system types and forward declarations for the RAGE engine.
 * This header provides the minimal interface needed by initialization code.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for system initialization
void rage_subsystem_init(void);
void rage_subsystem_shutdown(void);

#ifdef __cplusplus
}
#endif
