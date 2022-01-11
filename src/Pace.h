/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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
 *
 * Don´t use values of this data type as condition to test the paces,
 * use the PACE_IS_*() functions instead.
 */
typedef int8_t                     Pace_increment_t;

/* Datatype of this structure.  */
typedef struct Pace_t {

  /* Use this as increment of positions.  It will be alter between
   * engine ticks!
   *
   * Don´t use this value as condition to test the paces, use the
   * PACE_IS_*() functions instead.
   */
  Pace_increment_t pace;

  /* Private internal use  */
  uint16_s _status, _max, _decrement;
  uint16_s _decrement_brake, _decrement_accel;
} Pace_t;

/* ***************************************************************  */

/* Initialize the pace structure.  */
extern void __fastcall__ Pace_new(Pace_t* pace, uint7_t velocity_max,
               uint8_t accel_rate, uint6_t brake_rate, uint6_t delay);

/* Free the pace structure.  */
#define Pace_delete(pace)
/* extern void __fastcall__ Pace_delete(const Pace_t* pace);  */

/* ***************************************************************  */

/* Let´s the pace tick.  */
extern void __fastcall__ Pace_tick(Pace_t* pace);

/* ***************************************************************  */

/* Set a new VELOCITY_MAX and DELAY.  */
extern void __fastcall__ Pace_velocitymax_set(
                    Pace_t* pace, uint7_t velocity_max, uint6_t delay);

/* Set a new ACCELERATION.  */
extern void __fastcall__  Pace_accelerate_set(
                    Pace_t* pace, uint6_t accel_rate);

/* Set a new BRAKE_RATE for this pace.  */
extern void __fastcall__ Pace_brakerate_set(
                    Pace_t* pace, uint6_t brake_rate);

/* ***************************************************************  */

/* TRUE if PACE has a velocity of 0 pixel/s.  */
extern bool __fastcall__ Pace_is_stopped(Pace_t* pace);

/* TRUE if PACE has a positive or negative maximal velocity.  */
extern bool __fastcall__ Pace_is_maxpace(Pace_t* pace);

/* TRUE if PACE has a positive maximal velocity.  */
extern bool __fastcall__ Pace_is_maxpace_pos(Pace_t* pace);

/* TRUE if PACE has a negative maximal velocity.  */
extern bool __fastcall__ Pace_is_maxpace_neg(Pace_t* pace);

/* ***************************************************************  */

/* Returns the current absolute VELOCITY.  The MAXPACE equals
 * VELOCITY_MAX + 1.
 */
extern uint8_t __fastcall__ Pace_velocity_get_abs(Pace_t* pace);

/* Returns the current the sign correct VELOCITY.  The MAXPACE equals
 * VELOCITY_MAX + 1.
 */
extern int8_t __fastcall__ Pace_velocity_get(Pace_t* pace);

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

/* Brake from current pace to 0 pixel/s with BRAKE_RATE.
 */
extern void __fastcall__ Pace_brake(Pace_t* pace);

/* Add an positive impulse and accelerate to VELOCITY_MAX immediately.
 * After that brake to 0 pixel/s with BRAKE_RATE.
 */
extern void __fastcall__ Pace_impulse_pos(Pace_t* pace);

/* Add an negative impulse and accelerate to -VELOCITY_MAX
 * immediately.  After that brake to 0 pixel/s with BRAKE_RATE.
 */
extern void __fastcall__ Pace_impulse_neg(Pace_t* pace);

/* Accelerate from current pace positive until VELOCITY_MAX is
 * reached.  Hold that pace until PACE_STOP(), PACE_BRAKE() or
 * PACE_IMPULSE_*() is called.
 */
extern void __fastcall__ Pace_accelerate_pos(Pace_t* pace);

/* Accelerate from current pace negative until -VELOCITY_MAX is
 * reached.  Hold that pace until PACE_STOP(), PACE_BRAKE() or
 * PACE_IMPULSE_*() is called.
 */
extern void __fastcall__ Pace_accelerate_neg(Pace_t* pace);

#endif /* PACE_H__  */
