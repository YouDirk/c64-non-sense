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


#ifndef ENGINE_H__
#define ENGINE_H__

#include "common.h"
#include "Engine.gen.h"

#include "Timer.h"

/* ***************************************************************  */

/* Every multiple of the timer tickrate is an engine tick.  */
#define ENGINE_TICK_FACTOR    1

/* Every multiple of the timer tickrate is an engine tick.  */
#define ENGINE_TICKRATE_MS    (TIMER_1_TICKRATE_MS*ENGINE_TICK_FACTOR)
                                             /* 10*1 ms  */

/* Engine tick frequency is calculated as TIMER_FREQUENCY/TICK_FACTOR.  */
#define ENGINE_TICKFREQ_HZ    (TIMER_1_FREQUENCY_HZ/ENGINE_TICK_FACTOR)
                                             /* 100/1 Hz  */

/* Macros to convert ticks_t, timestamp_t and real world times
 * (milliseconds) between each other.
 */
#define ENGINE_TIMESTAMP2MS(timestamp)                               \
                         ((uint32_t) (timestamp)*TIMER_1_TICKRATE_MS)
#define ENGINE_MS2TIMESTAMP(time_ms)                                 \
                         ((timestamp_t) (time_ms)/TIMER_1_TICKRATE_MS)

#define ENGINE_TICKS2MS(ticks)                                       \
                         ((uint32_t) (ticks)*ENGINE_TICKRATE_MS)
#define ENGINE_MS2TICKS(time_ms)                                     \
                         ((ticks_t) (time_ms)/ENGINE_TICKRATE_MS)

#define ENGINE_TICKS2TIMESTAMP(ticks)                                \
                         ((timestamp_t) (ticks)*ENGINE_TICK_FACTOR)
#define ENGINE_TIMESTAMP2TICKS(timestamp)                            \
                         ((ticks_t) (timestamp)/ENGINE_TICK_FACTOR)

/* ***************************************************************  */

/* Initialize the mystic C64 NonSense Engine ^^
 *
 * returns: ENGINE.SET.EXIT_CODE
 */
extern void __fastcall__ Engine_init(void);

/* Undo the stuff which we´ve done in Engine_init().  */
extern void __fastcall__ Engine_release(void);

/* ***************************************************************  */

/* Let´s poll for an engine tick.
 *
 * returns: TRUE if an engine tick occurs
 */
extern bool __fastcall__ Engine_tick_poll(void);

#endif /* ENGINE_H__  */
