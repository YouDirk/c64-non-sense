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

#include "Graphix.gen.h"

/* *******************************************************************
 * colors of C64
 *
 * Already defined in chip_vic.def.h, but for higher level programming
 * again at this position.
 */

#define GRAPHIX_BLACK                   0x00
#define GRAPHIX_WHITE                   0x01
#define GRAPHIX_RED                     0x02
#define GRAPHIX_CYAN                    0x03
#define GRAPHIX_VIOLET                  0x04
#define GRAPHIX_GREEN                   0x05
#define GRAPHIX_BLUE                    0x06
#define GRAPHIX_YELLOW                  0x07
#define GRAPHIX_ORANGE                  0x08
#define GRAPHIX_BROWN                   0x09
#define GRAPHIX_LIGHTRED                0x0a
#define GRAPHIX_GRAY1                   0x0b
#define GRAPHIX_GRAY2                   0x0c
#define GRAPHIX_LIGHTGREEN              0x0d
#define GRAPHIX_LIGHTBLUE               0x0e
#define GRAPHIX_GRAY3                   0x0f

/*
 * end of colors
 * ***************************************************************  */

#define GRAPHIX_CELL_WIDTH_HIRES        8
#define GRAPHIX_CELL_WIDTH_MULTICOLOR   4
#define GRAPHIX_CELL_HEIGHT             8
#define GRAPHIX_CELL_CNT_X              40
#define GRAPHIX_CELL_CNT_Y              25

#define GRAPHIX_RES_X_HIRES                                          \
  (GRAPHIX_CELL_WIDTH_HIRES*GRAPHIX_CELL_CNT_X)
#define GRAPHIX_RES_X_MULTICOLOR                                     \
  (GRAPHIX_CELL_WIDTH_MULTICOLOR*GRAPHIX_CELL_CNT_X)
#define GRAPHIX_RES_Y                                                \
  (GRAPHIX_CELL_HEIGHT*GRAPHIX_CELL_CNT_Y)

#define GRAPHIX_BYTES_PER_CELL          8
#define GRAPHIX_PIXEL_PER_CELL_HIRES                                 \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_HIRES)
#define GRAPHIX_PIXEL_PER_CELL_MULTICOLOR                            \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_MULTICOLOR)

#define GRAPHIX_CELLS_PER_SCREEN (GRAPHIX_CELL_CNT_X*GRAPHIX_CELL_CNT_Y)
#define GRAPHIX_BYTES_PER_SCREEN                                     \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELLS_PER_SCREEN)

#define GRAPHIX_PIXEL_PER_SCREEN_HIRES                               \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_HIRES)
#define GRAPHIX_PIXEL_PER_SCREEN_MULTICOLOR                          \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_MULTICOLOR)


#define GRAPHIX_SCREENRAM_COLOR(set_color, zero_color)               \
  (((set_color) << 4) | (zero_color))

/* ***************************************************************  */

/* callbacks are defined here  */
typedef void __fastcall__
  (*Graphix_initCallback_t)(Graphix_buffer_t* graphix);
typedef void __fastcall__ (*Graphix_releaseCallback_t)(void);

/* ***************************************************************  */

/* Static members of this module.  */
extern Graphix_t Graphix;

/* Returns a singleton.  So it´s not needed to pass it as argument
 * into other functions of this Graphix module.
 *
 * init_callback: Is called after all graphic initialization is done,
 *                but SCREEN IS STILL BLACK and VIC IRQs ARE DISABLED.
 */
extern void __fastcall__
  Graphix_init(Graphix_initCallback_t init_callback);

/* Release all resources, restore regiters and disable interrupts.
 *
 * release_callback: Is called before all graphic will be released,
 *                   but SCREEN IS ALREADY BLACK and VIC ORQs ARE
 *                   DISABLED.
 */
extern void __fastcall__
  Graphix_release(Graphix_releaseCallback_t release_callback);

/* Should feels like a swap in a double buffered system of
 * Graphix.buffer
 */
extern void __fastcall__ Graphix_buffer_swap(void);

#endif /* GRAPHIX_H__  */
