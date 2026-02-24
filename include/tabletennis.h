/*
 * Table Tennis Game Logic - Public API
 *
 * Include this in any TU that calls into src/game/tabletennis.cpp.
 * Do NOT scatter ad-hoc extern "C" forward-declarations around.
 *
 * Addresses from xbox360_table_tennis_logic.md:
 *   game_E2B8 @ 0x820CE2B8   game_2610 @ 0x820D2610
 *   game_28F0 @ 0x820D28F0   game_2950 @ 0x820D2950
 *   game_5BE0 @ 0x820D5BE0   game_6148 @ 0x820D6148
 *   game_61C8 @ 0x820D61C8   game_6550 @ 0x820D6550
 *   game_66E8 @ 0x820D66E8   game_E458 @ 0x820CE458
 *   game_EC88 @ 0x820DEC88   game_EDC0 @ 0x820DEDC0
 */

#ifndef TABLETENNIS_H
#define TABLETENNIS_H

#include "tabletennis_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialisation / configuration */
int tabletennis_initialize(void);
int game_E2B8(void);   /* State initialisation     (200 bytes) */
int game_2610(void);   /* Game configuration       (288 bytes) */

/* Per-frame update pipeline (called in order by game_E458) */
int game_E458(void);   /* Main game logic          (508 bytes) */
int game_28F0(void);   /* Frame processing          (92 bytes) */
int game_2950(void);   /* Game update / serve       (236 bytes) */
int game_5BE0(void);   /* State check               (92 bytes) */
int game_6148(void);   /* Event handling           (128 bytes) */
int game_61C8(void);   /* Complex processing       (904 bytes) */
int game_6550(void);   /* Game mechanics / scoring (404 bytes) */
int game_66E8(void);   /* Rule validation          (224 bytes) */
int game_EC88(void);   /* End-game check            (80 bytes) */
int game_EDC0(void);   /* Game completion          (260 bytes) */

/* State accessors */
GameState* tabletennis_get_state(void);
Score*     tabletennis_get_score(void);

#ifdef __cplusplus
}
#endif

#endif /* TABLETENNIS_H */
