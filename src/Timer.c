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

#include "Graphix.h"

#include "chip_vic.gen.h"
#include "chip_cia.gen.h"

/* default Timer A frequency is around 60 Hz = TIMER_CLK/TIMER_TA_DEFAULT
 */

#define _TIMER_CLK_PAL_HZ          985249 /* system clock 1 MHz (PAL) */
#define _TIMER_CLK_NTSC_HZ        1022727 /* (NTSC)  */

/* 16 bit is not wide enough to store 1 Mhz  */
uint32_t Timer_system_clk;

/* Default CIA1 Timer A configuration.  Depending if we are on a PAL
 * or NTSC system.
 */
uint16_t _ta_default;

void __fastcall__
Timer_init(void)
{
  /* Set CIA1 Timer A interval to 1/TIMER_A_FREQUENCY_HZ ms, depending
   * on PAL/NTSC.  Graphix_ispal was initialized before
   */
  if (Graphix_ispal) {
    Timer_system_clk = _TIMER_CLK_PAL_HZ;
    _ta_default = CIA1_TA_DEFAULT_PAL;

    /* writing TA LO and TA HI in one command  */
    *(uint16_t*) CIA1.ta_lo = _TIMER_CLK_PAL_HZ/TIMER_A_FREQUENCY_HZ;
  } else {
    Timer_system_clk = _TIMER_CLK_NTSC_HZ;
    _ta_default = CIA1_TA_DEFAULT_NTSC;

    /* writing TA LO and TA HI in one command  */
    *(uint16_t*) CIA1.ta_lo = _TIMER_CLK_NTSC_HZ/TIMER_A_FREQUENCY_HZ;
  }

  /* Start Timer, Port B cycle, auto-restart, load latch initial,
   * system clock, RTC 60 Hz
   */
  CIA1.cra = CIA1_CRA_MODE;

  /* IRQs go!
   */
  CIA1.icr = CIA1_ICR_IRQMODE;
}

void __fastcall__
Timer_release(void)
{
  /* restore interval for CIA1 Timer A  */
  *(uint16_t*) CIA1.ta_lo = _ta_default;

  /* load the set latch  */
  CIA1.cra = 0x11;

  /* mask all CIA1 IRQs  */
  CIA1.icr = CIA_ICR_MASKALL_MASK;
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
