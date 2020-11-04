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


#include "common.h"

#include "Engine.h"
#include "Interrupt.h"
#include "Graphix.h"
#include "Timer.h"

#include "chip-cia.gen.h"

/* ***************************************************************  */

static void __fastcall__
_main_init(Graphix_buffer_t* graphix)
{
  unsigned i;

  /* must be the first of all  */
  Interrupt_init();

  /* set screen ram  */
  memset(graphix->screen_ram,
    GRAPHIX_SCREENRAM_COLOR(GRAPHIX_GREEN, GRAPHIX_BLACK),
    GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(graphix->bitmap_ram, 0x41, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8)
    graphix->bitmap_ram[i] = 0xff;

  Timer_init();
}

static void __fastcall__
_main_release(void)
{
  Timer_release();

  /* must be the last of all  */
  Interrupt_release();
}

/* ***************************************************************  */

int
main(void)
{
  unsigned char joy_cntrl;

  DEBUG_INIT();

  Engine_init();
  Graphix_init(_main_init);

  do {
    joy_cntrl = 0x00;

    do {
      /* polling stuff between Engine ticks  */

      if (~CIA1.pra & CIA1_PRAB_JOY_MASK) joy_cntrl = ~CIA1.pra;

    } while (!Engine_tick_poll());

    /* ticking stuff  */

    if (Engine.tick_timestamp % ENGINE_MS2TIMESTAMP(1000) == 0) {
      ++Graphix.buffer.bordercolor;
    }

    if (joy_cntrl & CIA1_PRAB_JOYUP_MASK)    ++Graphix.buffer.scroll_y;
    if (joy_cntrl & CIA1_PRAB_JOYDOWN_MASK)  --Graphix.buffer.scroll_y;
    if (joy_cntrl & CIA1_PRAB_JOYLEFT_MASK)  ++Graphix.buffer.scroll_x;
    if (joy_cntrl & CIA1_PRAB_JOYRIGHT_MASK) --Graphix.buffer.scroll_x;

    Graphix_buffers_swap();
  } while (~joy_cntrl & CIA1_PRAB_JOYBTN1_MASK);

  Graphix_release(_main_release);
  Engine_release();

  DEBUG_RELEASE_PRINT();
  return 0;
}
