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

/* The default CIA1 timer A frequency is around
 *
 *  60 Hz = _TIMER_CLK_[PAL/NTSC]_HZ/CIA1_TA_DEFAULT_[PAL/NTSC]
 */

#define _TIMER_CLK_PAL_HZ          985249 /* system clock 1 MHz (PAL) */
#define _TIMER_CLK_NTSC_HZ        1022727 /* (NTSC)  */

/* Default CIA1 timer A configuration.  Depending if we are on a PAL
 * or NTSC system.
 */
static uint16_t _ta_default;

/* The logical timers :) ...  Will be incremented by ISR.  */
volatile uint32_t timer_1_32;

/* ***************************************************************  */

/* Static members of this module.  */
Timer_t Timer;

void __fastcall__
Timer_init(void)
{
  /* initialize timers.  Do not call Timer_?_reset() here, it unmask
   * IRQ before it was initialized.
   */
  timer_1_32 = 0;

  /* Set CIA1 timer A interval to 1/TIMER_1_FREQUENCY_HZ ms, depending
   * on PAL/NTSC.  Graphix_ispal was initialized before
   */
  if (Graphix.is_pal) {
    Timer.system_clk = _TIMER_CLK_PAL_HZ;
    _ta_default = CIA1_TA_DEFAULT_PAL;

    /* writing TA LO and TA HI in one command  */
    *(uint16_t*) &CIA1.ta_lo = _TIMER_CLK_PAL_HZ/TIMER_1_FREQUENCY_HZ;
  } else {
    Timer.system_clk = _TIMER_CLK_NTSC_HZ;
    _ta_default = CIA1_TA_DEFAULT_NTSC;

    /* writing TA LO and TA HI in one command  */
    *(uint16_t*) &CIA1.ta_lo = _TIMER_CLK_NTSC_HZ/TIMER_1_FREQUENCY_HZ;
  }

  /* Start Timer, Port B cycle, auto-restart, load latch initial,
   * system clock, RTC 60 Hz
   */
  CIA1.cra = CIA1_CRA_MODE;

  /* IRQs go!  */
  CIA1.icr = CIA1_ICR_IRQMODE;
}

void __fastcall__
Timer_release(void)
{
  /* restore interval for CIA1 timer A  */
  *(uint16_t*) &CIA1.ta_lo = _ta_default;

  /* load the set latch  */
  CIA1.cra = CIA1_CRA_DEFAULT;

  /* mask all CIA1 IRQs  */
  CIA1.icr = CIA_ICR_MASK(CIA_ICR_ALL);
}

/* ***************************************************************  */

void __fastcall__
Timer_1_reset(void)
{
  /* mask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK);

  timer_1_32 = 0;

  /* unmask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK);
}

uint8_t __fastcall__
Timer_1_get8(void)
{
  /* atomic operation, so we don´t need to mask IRQs  */
  return (uint8_t) timer_1_32;
}

uint16_t __fastcall__
Timer_1_get16(void)
{
  uint16_t result;

  /* mask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK);

  result = (uint16_t) timer_1_32;

  /* unmask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK);

  return result;
}

uint32_t __fastcall__
Timer_1_get32(void)
{
  uint32_t result;

  /* mask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK);

  result = timer_1_32;

  /* unmask CIA1 timer A IRQs  */
  CIA1.icr = CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK);

  return result;
}
