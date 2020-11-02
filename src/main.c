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
  uint32_t cur_time, prev_time;

  DEBUG_INIT();

  Graphix_init(_main_init);

  joy_cntrl = CIA1_PRAB_JOY_MASK, prev_time = -1;
  while (joy_cntrl & CIA1_PRAB_JOYBTN1_MASK) {
    while (prev_time != -1 || !(~CIA1.pra & CIA1_PRAB_JOY_MASK)) {
      cur_time = Timer_1_get32();

      if (prev_time != -1 && prev_time + 0 < cur_time) prev_time = -1;

      if (cur_time % 100 == 50) {
        ++Graphix.buffer.bordercolor; Graphix_buffers_swap();
        while (Timer_1_get32() % 100 == 50);
      }
    }
    prev_time = cur_time;
    joy_cntrl = CIA1.pra;

    if (~joy_cntrl & CIA1_PRAB_JOYUP_MASK)    ++Graphix.buffer.scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYDOWN_MASK)  --Graphix.buffer.scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYLEFT_MASK)  ++Graphix.buffer.scroll_x;
    if (~joy_cntrl & CIA1_PRAB_JOYRIGHT_MASK) --Graphix.buffer.scroll_x;

    Graphix_buffers_swap();
  }

  Graphix_release(_main_release);

  DEBUG_RELEASE_PRINT();
  return 0;
}
