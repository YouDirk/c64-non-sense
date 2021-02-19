/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2021  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef PACE_H__
#define PACE_H__

#include "common.h"

/* ***************************************************************  */

/* Pace Increments are integers, which can be used as increment of
 * positions in *_TICK() functions.  They may be alter between engine
 * ticks, even if the mathematically velocity is constant.  Therefore
 * it´s possible to visualize paces which are not equals a multiple of
 * the engine tickrate.
 */
typedef int8_t                     Pace_increment_t;

/* Datatype of this structure.  */
typedef struct Pace_t {

  /* Use this as increment of positions.
   */
  Pace_increment_t pace;

  /* Private internal use  */
  uint16_s _status, _max, _decrement;
  uint16_s _decrement_brake, _decrement_accel;
} Pace_t;

/* ***************************************************************  */

/* Initialize the pace structure.  */
extern void __fastcall__ Pace_new(Pace_t* pace, uint8_t pace_max,
                uint8_t accelerate, uint8_t brakerate, uint8_t delay);

/* Free the pace structure.  */
#define Pace_delete(pace)
/* extern void __fastcall__ Pace_delete(Pace_t* pace);  */

/* ***************************************************************  */

/* Let´s the pace tick.  */
extern void __fastcall__ Pace_tick(Pace_t* pace);

/* ***************************************************************  */

/* Set PACE_MAX immediately.  Hold that pace until PACE_STOP(),
 * PACE_BRAKE() or PACE_IMPULSE_*() is called.
 */
extern void __fastcall__ Pace_start_pos(Pace_t* pace);

/* Set -PACE_MAX immediately.  Hold that pace until PACE_STOP(),
 * PACE_BRAKE() or PACE_IMPULSE_*() is called.
 */
extern void __fastcall__ Pace_start_neg(Pace_t* pace);

/* Set 0 pixel/s immediately.
 */
extern void __fastcall__ Pace_stop(Pace_t* pace);

/* Brake from current pace to 0 pixel/s with BRAKERATE.
 */
extern void __fastcall__ Pace_brake(Pace_t* pace);

/* Add an positive impulse and accelerate to PACE_MAX immediately.
 * After that brake to 0 pixel/s with BRAKERATE.
 */
extern void __fastcall__ Pace_impulse_pos(Pace_t* pace);

/* Add an negative impulse and accelerate to -PACE_MAX
 * immediately.  After that brake to 0 pixel/s with BRAKERATE.
 */
extern void __fastcall__ Pace_impulse_neg(Pace_t* pace);

/* Accelerate from current pace positive until PACE_MAX is reached.
 * Hold that pace until PACE_STOP(), PACE_BRAKE() or PACE_IMPULSE_*()
 * is called.
 */
extern void __fastcall__ Pace_accelerate_pos(Pace_t* pace);

/* Accelerate from current pace negative until -PACE_MAX is reached.
 * Hold that pace until PACE_STOP(), PACE_BRAKE() or PACE_IMPULSE_*()
 * is called.
 */
extern void __fastcall__ Pace_accelerate_neg(Pace_t* pace);

#endif /* PACE_H__  */