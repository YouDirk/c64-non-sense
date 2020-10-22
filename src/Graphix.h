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


#ifndef GRAPHIX_H__
#define GRAPHIX_H__

#include "common.h"

#define GRAPHIX_CELL_WIDTH_HIRES        8
#define GRAPHIX_CELL_WIDTH_MULTICOLOR   4
#define GRAPHIX_CELL_HEIGHT             8
#define GRAPHIX_CELL_CNT_X              40
#define GRAPHIX_CELL_CNT_Y              25

#define GRAPHIX_RES_X_HIRES                                           \
  (GRAPHIX_CELL_WIDTH_HIRES*GRAPHIX_CELL_CNT_X)
#define GRAPHIX_RES_X_MULTICOLOR                                      \
  (GRAPHIX_CELL_WIDTH_MULTICOLOR*GRAPHIX_CELL_CNT_X)
#define GRAPHIX_RES_Y                                                 \
  (GRAPHIX_CELL_HEIGHT*GRAPHIX_CELL_CNT_Y)

#define GRAPHIX_BYTES_PER_CELL          8
#define GRAPHIX_PIXEL_PER_CELL_HIRES                                  \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_HIRES)
#define GRAPHIX_PIXEL_PER_CELL_MULTICOLOR                             \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_MULTICOLOR)

#define GRAPHIX_CELLS_PER_SCREEN (GRAPHIX_CELL_CNT_X*GRAPHIX_CELL_CNT_Y)
#define GRAPHIX_BYTES_PER_SCREEN                                      \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELLS_PER_SCREEN)

#define GRAPHIX_PIXEL_PER_SCREEN_HIRES                                \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_HIRES)
#define GRAPHIX_PIXEL_PER_SCREEN_MULTICOLOR                           \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_MULTICOLOR)


/* default is 0x03  */
#define _GRAPHIX_BANK_NUMBER_VIC_REG    0x00
/* default is 0x10  */
#define _GRAPHIX_SCREENRAM_ADDR_REG     0x00
/* default is 0x07, but 0x00 or 0x08 should be used  */
#define _GRAPHIX_BITMAP_ADDR_REG        0x08

#define GRAPHIX_VIC_RAM                                               \
  ((void*) (0xc000 - _GRAPHIX_BANK_NUMBER_VIC_REG*0x4000))
#define GRAPHIX_SCREEN_RAM                                            \
  ((uint8_t*) ((unsigned) GRAPHIX_VIC_RAM + _GRAPHIX_SCREENRAM_ADDR_REG*0x40))
#define GRAPHIX_BITMAP_RAM                                            \
  ((uint8_t*) ((unsigned) GRAPHIX_VIC_RAM + _GRAPHIX_BITMAP_ADDR_REG*0x0400))

#define GRAPHIX_SCREENRAM_COLOR(set_color, zero_color)                \
  ((set_color << 4) | zero_color)

extern void Graphix_init(void);
extern void Graphix_release(void);

#endif /* GRAPHIX_H__  */
