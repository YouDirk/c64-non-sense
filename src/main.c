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

#include "Graphix.h"

static void __fastcall__
_init_graphic_rams(uint8_t* screen_ram, uint8_t* bitmap_ram)
{
  unsigned i;

  /* set screen ram  */
  memset(screen_ram, GRAPHIX_SCREENRAM_COLOR(COLOR_GREEN, COLOR_BLACK),
         GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(bitmap_ram, 0x18, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8)
    bitmap_ram[i] = 0xff;
}

int
main(void)
{
  Graphix_t* graphix = Graphix_new(_init_graphic_rams);

  unsigned char joy_cntrl = 0x1f;
  int prev_time = -1;

  while (joy_cntrl & (1<<4)) {
    while (prev_time >= 0
           || !(~(*(unsigned char*) 0xdc00) & 0x1f)) {
      if (prev_time >= 0
          && (prev_time + 1) % 256 == *(unsigned char*) 0x00a2)
        prev_time = -1;
    }
    prev_time = *(unsigned char*) 0x00a2;
    joy_cntrl = *(unsigned char*) 0xdc00;

    /* up  */
    if (~joy_cntrl & (1<<0)) ++graphix->scroll_y;
    /* down  */
    if (~joy_cntrl & (1<<1)) --graphix->scroll_y;
    /* left  */
    if (~joy_cntrl & (1<<2)) ++graphix->scroll_x;
    /* right  */
    if (~joy_cntrl & (1<<3)) --graphix->scroll_x;

    Graphix_render();
  }

  Graphix_release();

  return 0;
}
