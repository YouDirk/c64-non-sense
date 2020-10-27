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
_main_init(uint8_t* screen_ram, uint8_t* bitmap_ram)
{
  unsigned i;

  /* must be the first of all  */
  Interrupt_init();

  /* set screen ram  */
  memset(screen_ram,
    GRAPHIX_SCREENRAM_COLOR(GRAPHIX_GREEN, GRAPHIX_BLACK),
    GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(bitmap_ram, 0x41, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8) bitmap_ram[i] = 0xff;

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
  int prev_time;
  bool change_border;

  graphix = Graphix_new(_main_init);

  joy_cntrl = CIA1_PRAB_JOY_MASK, prev_time = -1, change_border = true;
  while (joy_cntrl & CIA1_PRAB_JOYBTN1_MASK) {
    while (prev_time >= 0 || !(~CIA1.pra & CIA1_PRAB_JOY_MASK)) {
      if (prev_time >= 0 && prev_time < Timer_A) prev_time = -1;
      if (change_border && Timer_A % 100 == 0) {
        ++VIC.bordercolor;
        change_border = false;
      }

      if (!change_border && Timer_A % 100 == 50) change_border = true;
    }
    prev_time = Timer_A;
    joy_cntrl = CIA1.pra;

    if (~joy_cntrl & CIA1_PRAB_JOYUP_MASK)    ++graphix->scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYDOWN_MASK)  --graphix->scroll_y;
    if (~joy_cntrl & CIA1_PRAB_JOYLEFT_MASK)  ++graphix->scroll_x;
    if (~joy_cntrl & CIA1_PRAB_JOYRIGHT_MASK) --graphix->scroll_x;

    Graphix_swapBuffers();
  }

  Graphix_release(_main_release);

#ifdef DEBUG
  printf("timer a: 0x%04x = %u0ms\n", Timer_A, Timer_A);
#endif

  return 0;
}
