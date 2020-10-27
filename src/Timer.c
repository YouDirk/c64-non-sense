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


#include "Timer.h"

#include "chip_vic.gen.h"

/* default Timer A frequency is around 60 Hz = TIMER_CLK/TIMER_TA_DEFAULT
 */

#define _TIMER_CLK_PAL_HZ          985248 /* system clock 1 MHz (PAL) */
#define _TIMER_CLK_NTSC_HZ        1022727 /* (NTSC)  */

#define _TIMER_TA_DEFAULT_PAL      0x4025 /* default (PAL) setting  */
#define _TIMER_TA_DEFAULT_NTSC     0x4295 /* (NTSC)  */

void __fastcall__
Timer_init(void)
{
}

void __fastcall__
Timer_release(void)
{
}

void __fastcall__
_Timer_timer_a_isr(void)
{
#ifdef DEBUG_IRQ_RENDERTIME
  VIC.bordercolor = VIC_COLOR_LIGHTBLUE;
#endif

#ifdef DEBUG_IRQ_RENDERTIME
  VIC.bordercolor = VIC_COLOR_BLACK;
#endif
}
