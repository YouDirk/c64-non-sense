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

int
main(void)
{
  unsigned i;

  /* *****************************************************************
   * init  */

  /* backup registers  */
  unsigned char d020_border = *(unsigned char*) 0xd020;
  unsigned char dd00_cia2 = *(unsigned char*) 0xdd00;
  unsigned char d011_mode = *(unsigned char*) 0xd011;
  unsigned char d016_mode2 = *(unsigned char*) 0xd016;
  unsigned char d018_buffer = *(unsigned char*) 0xd018;

  Graphix_init();

  /* set screen ram  */
  memset(GRAPHIX_SCREEN_RAM,
    GRAPHIX_SCREENRAM_COLOR(COLOR_GREEN, COLOR_BLACK),
    GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(GRAPHIX_BITMAP_RAM, 0x18, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8)
    GRAPHIX_BITMAP_RAM[i] = 0xff;

  /* switch into graphic mode  */
  *(unsigned char*) 0xd011 = d011_mode | 0x20;


  /* *****************************************************************
   * do stuff  */

  {
    unsigned char scroll_x = d016_mode2 & 0x07;
    unsigned char scroll_y = d011_mode & 0x07;

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
      if (~joy_cntrl & (1<<0)) {
        scroll_y = (scroll_y + 1) & 7;
        *(unsigned char*) 0xd011
          = ((*(unsigned char*) 0xd011) & 0xf8) | scroll_y;
      }
      /* down  */
      if (~joy_cntrl & (1<<1)) {
        scroll_y = (scroll_y - 1) & 7;
        *(unsigned char*) 0xd011
          = ((*(unsigned char*) 0xd011) & 0xf8) | scroll_y;
      }
      /* left  */
      if (~joy_cntrl & (1<<2)) {
        scroll_x = (scroll_x + 1) & 7;
        *(unsigned char*) 0xd016
          = ((*(unsigned char*) 0xd016) & 0xf8) | scroll_x;
      }
      /* right  */
      if (~joy_cntrl & (1<<3)) {
        scroll_x = (scroll_x - 1) & 7;
        *(unsigned char*) 0xd016
          = ((*(unsigned char*) 0xd016) & 0xf8) | scroll_x;
      }
    }
  }


  /* *****************************************************************
   * deinit  */

  *(unsigned char*) 0xd016 = d016_mode2;

  /* black screen during deinitialization  */
  *(unsigned char*) 0xd011 = (d011_mode & 0xef) | 0x20;

  Graphix_release();

  return 0;
}
