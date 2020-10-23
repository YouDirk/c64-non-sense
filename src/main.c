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

static void __fastcall__
_init_graphic_rams(uint8_t* screen_ram, uint8_t* bitmap_ram)
{
  unsigned i;

  /* set screen ram  */
  memset(screen_ram, GRAPHIX_SCREENRAM_COLOR(COLOR_GREEN, COLOR_BLACK),
         GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(bitmap_ram, 0x41, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8) bitmap_ram[i] = 0xff;
}

int
main(void)
{
  Graphix_t* graphix;
  unsigned char joy_cntrl = 0x1f;
  int prev_time = -1;

  Interrupt_init();
  graphix = Graphix_new(_init_graphic_rams);

  while (joy_cntrl & JOY_BTN_1_MASK) {
    while (prev_time >= 0
           || !(~(*(unsigned char*) 0xdc00) & 0x1f)) {
      if (prev_time >= 0
          && (prev_time + 1) % 256 == *(unsigned char*) 0x00a2)
        prev_time = -1;
    }
    prev_time = *(unsigned char*) 0x00a2;
    joy_cntrl = *(unsigned char*) 0xdc00;

    if (~joy_cntrl & JOY_UP_MASK)    ++graphix->scroll_y;
    if (~joy_cntrl & JOY_DOWN_MASK)  --graphix->scroll_y;
    if (~joy_cntrl & JOY_LEFT_MASK)  ++graphix->scroll_x;
    if (~joy_cntrl & JOY_RIGHT_MASK) --graphix->scroll_x;

    Graphix_render();
  }

  Graphix_release();
  Interrupt_release();

  return 0;
}
