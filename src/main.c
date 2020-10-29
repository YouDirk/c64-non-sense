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

#include "chip_cia.gen.h"

/* ***************************************************************  */

static void __fastcall__
_main_init(Graphix_t* graphix)
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
  Graphix_t* graphix;
  unsigned char joy_cntrl;
  int32_t cur_time, prev_time;

  graphix = Graphix_new(_main_init);

  joy_cntrl = CIA1_PRAB_JOY_MASK, prev_time = -1;
  while (joy_cntrl & CIA1_PRAB_JOYBTN1_MASK) {
    while (prev_time >= 0 || !(~CIA1.pra & CIA1_PRAB_JOY_MASK)) {
      cur_time = Timer_1_get32();

      if (prev_time >= 0 && prev_time + 0 < cur_time) prev_time = -1;

      if (cur_time % 100 == 50) {
        ++graphix->bordercolor; Graphix_swapBuffers();
        while (Timer_1_get32() % 100 == 50);
      }
    }
    prev_time = cur_time;
    joy_cntrl = CIA1.pra;

    if (~joy_cntrl & CIA1_PRAB_JOYUP_MASK)    ++graphix->scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYDOWN_MASK)  --graphix->scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYLEFT_MASK)  ++graphix->scroll_x;
    if (~joy_cntrl & CIA1_PRAB_JOYRIGHT_MASK) --graphix->scroll_x;

    Graphix_swapBuffers();
  }

  Graphix_release(_main_release);

#ifdef DEBUG
  cur_time = Timer_1_get32();
  printf("timer a: 0x%08lx = %lu0ms\n", cur_time*10, cur_time);
#endif

  return 0;
}
