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


#ifndef TIMER_H__
#define TIMER_H__

#include "common.h"

/* *******************************************************************
 *
 * Logical timers (1, 2, ...) are controlled by physical timers (CIA1
 * A B, CIA2 A B).
 */

#define TIMER_1_FREQUENCY_HZ            100   /* 100 Hz  */
#define TIMER_1_PERIODICTIME_MS         (1000/TIMER_A_FREQUENCY_HZ)
                                              /* 10 ms  */

/* ***************************************************************  */

/* System clock of the MOS 6510 CPU.  Depending if we are on a PAL or
 * NTSC system.
 *
 * 16 bit is not wide enough to store 1 Mhz
 */
extern uint32_t Timer_system_clk;

/* Initialize the timers on CIA1 and CIA2 which we are using.
 */
extern void __fastcall__ Timer_init(void);

/* Restore timer configuration on CIA1 and CIA2 which we had used.
 */
extern void __fastcall__ Timer_release(void);

/* *******************************************************************
 *
 * Access to logical timers (1, 2, ...), controlled by physical timers
 * (CIA1 A B, CIA2 A B).
 */

/* Reset the logical timer 1 to t=0.  */
extern void __fastcall__ Timer_1_reset(void);

/* Return just the lowest byte of logical timer 1.  */
extern uint8_t __fastcall__ Timer_1_get8(void);

/* Return the lower 16 bit of logical timer 1.  */
extern uint16_t __fastcall__ Timer_1_get16(void);

/* Return the whole 32 bit of logical timer 1.  */
extern uint32_t __fastcall__ Timer_1_get32(void);

#endif /* TIMER_H__  */