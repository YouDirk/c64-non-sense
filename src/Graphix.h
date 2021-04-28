/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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

/* ***************************************************************  */

#define GRAPHIX_CELL_WIDTH_HIRES        8
#define GRAPHIX_CELL_WIDTH_MULTICOLOR   4
#define GRAPHIX_CELL_HEIGHT             8
#define GRAPHIX_CELL_CNT_X              40
#define GRAPHIX_CELL_CNT_Y              25

#define GRAPHIX_RES_X_HIRES                                          \
  (GRAPHIX_CELL_WIDTH_HIRES*GRAPHIX_CELL_CNT_X)      /* x=320 px  */
#define GRAPHIX_RES_X_MULTICOLOR                                     \
  (GRAPHIX_CELL_WIDTH_MULTICOLOR*GRAPHIX_CELL_CNT_X) /* x=160 px  */
#define GRAPHIX_RES_Y                                                \
  (GRAPHIX_CELL_HEIGHT*GRAPHIX_CELL_CNT_Y)           /* y=200 px  */

#define GRAPHIX_BYTES_PER_CELL          8
#define GRAPHIX_PIXEL_PER_CELL_HIRES                                 \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_HIRES)  /* 64 px/cell  */
#define GRAPHIX_PIXEL_PER_CELL_MULTICOLOR                            \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELL_WIDTH_MULTICOLOR)
                                                     /* 32 px/cell  */

#define GRAPHIX_CELLS_PER_SCREEN (GRAPHIX_CELL_CNT_X*GRAPHIX_CELL_CNT_Y)
                                             /* 1000 cells/screen  */
#define GRAPHIX_BYTES_PER_SCREEN                                     \
  (GRAPHIX_BYTES_PER_CELL*GRAPHIX_CELLS_PER_SCREEN)
                                             /* 8000 bytes/screen  */

#define GRAPHIX_PIXEL_PER_SCREEN_HIRES                               \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_HIRES)
                                             /* 64.000 px/screen  */
#define GRAPHIX_PIXEL_PER_SCREEN_MULTICOLOR                          \
  (GRAPHIX_CELLS_PER_SCREEN*GRAPHIX_PIXEL_PER_CELL_MULTICOLOR)
                                             /* 32.000 px/screen  */

#define GRAPHIX_SCREENRAM_COLOR(set_color, zero_color)               \
  (((set_color) << 4) | (zero_color))

/* ***************************************************************  */

/* callbacks are defined here  */
typedef void __fastcall__ (*Graphix_initCallback_t)(void);
typedef void __fastcall__ (*Graphix_releaseCallback_t)(void);

/* ***************************************************************  */

/* Initialize this module.
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

/* *******************************************************************
 * private
 */

extern void __fastcall__ _Graphix_init_vic_detect(void);

/* ***************************************************************  */

#endif /* GRAPHIX_H__  */
