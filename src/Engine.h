/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
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


#ifndef ENGINE_H__
#define ENGINE_H__

#include "common.h"

#include "Timer.h"

#define ENGINE_TICK_FACTOR     1

#define ENGINE_TICKINTERVAL_MS (TIMER_1_INTERVAL_MS*ENGINE_TICK_FACTOR)
                                             /* 10*1 ms  */
#define ENGINE_TICKRATE_HZ     (TIMER_1_FREQUENCY_HZ/ENGINE_TICK_FACTOR)
                                             /* 100/1 Hz  */

#define ENGINE_TIMESTAMP2MS(timestamp)                               \
                         ((uint32_t) (timestamp)*TIMER_1_INTERVAL_MS)
#define ENGINE_MS2TIMESTAMP(time_ms)                                 \
                         ((timestamp_t) (time_ms)/TIMER_1_INTERVAL_MS)

/* ***************************************************************  */

/* Datatype to store time.  Use the macros above to convert a
 * timestamp_t into a real world time, i.e. milliseconds.
 */
typedef uint32_t                   timestamp_t;

/* Structure of static members for module.  */
typedef struct Engine_t {

  /* The last time at which the Engine polls for a tick.
   */
  timestamp_t poll_time;

  /* The time at which the last Engine tick occurs.
   */
  timestamp_t tick_time;

} Engine_t;

/* ***************************************************************  */

/* Static members of this module.  */
extern Engine_t Engine;

/* Initialize the mystic NonSense Engine ^^
 */
extern void __fastcall__ Engine_init(void);


/* Undo the stuff which we´ve done in Engine_init().
 */
extern void __fastcall__ Engine_release(void);

/* ***************************************************************  */

/* Let´s poll for an Engine tick.
 *
 * returns: TRUE if an Engine tick occurs
 */
extern bool __fastcall__ Engine_tick_poll(void);

#endif /* ENGINE_H__  */
