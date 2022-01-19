/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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


#include "AASandbox.h"

#include "AAAssets.h"

#include "Graphix.h"

/* ***************************************************************  */

void __fastcall__
AASandbox_init(void)
{
  static uint16_t i;
  static Graphix_bitmapram_byte_t cur_cell;
  static uint8_t mod;

  /* set screen ram  */
  for (i=0, mod=0; i<1000; ++i, ++mod) {
    if (mod == 3) mod = 0;

    switch (mod) {
    case 0:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray3,
                                              Graphix_black);
      break;
    case 1:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray2,
                                              Graphix_black);
      break;
    case 2:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray1,
                                              Graphix_black);
      break;
    }
  }

  /* set bitmap  */
  for (i=0, mod=0, cur_cell=0xff; i<1000; ++i, ++mod) {
    if (mod == 40) {
      cur_cell = ~cur_cell;
      mod = 0;
    }

    memset(GRAPHIX_BUFFER_BITMAPRAM[0][i],
           i%2 == 0? cur_cell: ~cur_cell, GRAPHIX_SCREEN_CELLS_BYTES);
  }
}

void __fastcall__
AASandbox_release(void)
{

}

/* ***************************************************************  */

void __fastcall__
AASandbox_poll(void)
{

}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick(void)
{

}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick_low(void)
{

}

/* ***************************************************************  */
