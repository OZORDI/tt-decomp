/*
 * Table Tennis Physics System - Public API
 *
 * Include this in any TU that calls into src/physics/tabletennis_physics.cpp.
 *
 * IMPORTANT - pipeline ownership:
 *   fn_820E5E40 is the FULL pipeline wrapper.  It calls
 *   fn_820CE658 + fn_820D45A0 + fn_820D5C40 internally.
 *   Callers must only call fn_820E5E40, then fn_820E45A0 for response.
 *   Never call the three sub-stages separately alongside fn_820E5E40.
 *
 * Addresses from xbox360_table_tennis_logic.md:
 *   fn_820CE658 @ 0x820CE658  Physics calculations   (1304 bytes)
 *   fn_820D45A0 @ 0x820D45A0  Ball physics           (1880 bytes)
 *   fn_820D5C40 @ 0x820D5C40  Collision detection    (1284 bytes)
 *   fn_820E5E40 @ 0x820E5E40  Game physics pipeline  (2796 bytes)
 *   fn_820E45A0 @ 0x820E45A0  Collision processing   (6300 bytes)
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "tabletennis_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sub-stages (owned by fn_820E5E40 - don't call directly alongside it) */
int fn_820CE658(void);
int fn_820D45A0(void);
int fn_820D5C40(void);

/* Full pipeline wrapper - call this, then fn_820E45A0 */
int fn_820E5E40(void);

/* Collision response - must follow fn_820E5E40 each frame */
int fn_820E45A0(void);

/* Lifecycle */
int  physics_initialize(void);
void physics_get_stats(int* collisionCount, float* ballSpeed);

#ifdef __cplusplus
}
#endif

#endif /* PHYSICS_H */
