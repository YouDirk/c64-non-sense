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

#include <string.h>

int
main(void)
{
  unsigned char* x;

  /* *****************************************************************
   * init  */

  /* backup registers  */
  unsigned char d020_border = *(unsigned char*) 0xd020;
  unsigned char dd00_cia2 = *(unsigned char*) 0xdd00;
  unsigned char d011_mode = *(unsigned char*) 0xd011;
  unsigned char d016_mode2 = *(unsigned char*) 0xd016;
  unsigned char d018_buffer = *(unsigned char*) 0xd018;

  /* black screen during initialization  */
  *(unsigned char*) 0xd011 = d011_mode & 0xef;
  *(unsigned char*) 0xd020 = 0x00;

  /* set Screen RAM  */
  memset((void*) 0xc000, 0x50, 40*25);

  /* set Bitmap  */
  memset((void*) 0xe000, 0x18, 8*40*25);
  for (x = (void*) 0xe000; x < (void*) (0xe000 + 8*40*25); x+=8)
    *x = 0xff;

  /* Remap VIC memory  */
  *(unsigned char*) 0xdd00 = (dd00_cia2 & 0xfc) | 0x00;
  *(unsigned char*) 0xd018 = 0x08;

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

  /* restore VIC memory mapping  */
  *(unsigned char*) 0xd018 = d018_buffer;
  *(unsigned char*) 0xdd00 = dd00_cia2;

  /* switch back to text mode  */
  *(unsigned char*) 0xd020 = d020_border;
  *(unsigned char*) 0xd011 = d011_mode;

  return 0;
}
