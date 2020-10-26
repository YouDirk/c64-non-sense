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


#ifndef CHIP_VIC_DEF_H__
#define CHIP_VIC_DEF_H__

#include "define.h"

/* *******************************************************************
 * colors of C64
 */

define_hex(VIC_COLOR_BLACK,             00)
define_hex(VIC_COLOR_WHITE,             01)
define_hex(VIC_COLOR_RED,               02)
define_hex(VIC_COLOR_CYAN,              03)
define_hex(VIC_COLOR_VIOLET,            04)
define_hex(VIC_COLOR_GREEN,             05)
define_hex(VIC_COLOR_BLUE,              06)
define_hex(VIC_COLOR_YELLOW,            07)
define_hex(VIC_COLOR_ORANGE,            08)
define_hex(VIC_COLOR_BROWN,             09)
define_hex(VIC_COLOR_LIGHTRED,          0a)
define_hex(VIC_COLOR_GRAY1,             0b)
define_hex(VIC_COLOR_GRAY2,             0c)
define_hex(VIC_COLOR_LIGHTGREEN,        0d)
define_hex(VIC_COLOR_LIGHTBLUE,         0e)
define_hex(VIC_COLOR_GRAY3,             0f)

/*
 * end of colors
 * ***************************************************************  */

define_hex(VIC_BASE,                    d000)

define_hex(VIC_CTRL1,                   d011)
define_hex(VIC_CTRL1_RASTERLINE_MASK,   80)
define_hex(VIC_CTRL1_EXTCOLOR_MASK,     40)
define_hex(VIC_CTRL1_BITMAPMODE_MASK,   20)
define_hex(VIC_CTRL1_SCREEN_ON_MASK,    10)
define_hex(VIC_CTRL1_25ROWS_MASK,       08)
define_hex(VIC_CTRL1_YSCROLL_MASK,      07)
define_hex(VIC_CTRL1_DEFAULT_YSCROLL,   03)
define(VIC_CTRL1_DEFAULT,                                            \
       (VIC_CTRL1_SCREEN_ON_MASK | VIC_CTRL1_25ROWS_MASK             \
        | VIC_CTRL1_DEFAULT_YSCROLL))

/* rasterline=0, bitmap mode, enable screen, no 25 rows, yscroll=0  */
define(VIC_CTRL1_MODE,                                               \
       (VIC_CTRL1_BITMAPMODE_MASK | VIC_CTRL1_SCREEN_ON_MASK))

define_hex(VIC_RASTERLINE,              d012)
define_dec(VIC_RASTERLINE_SCREENBEGIN,  50)
define_dec(VIC_RASTERLINE_SCREENEND,    250)

define_hex(VIC_CTRL2,                   d016)
define_hex(VIC_CTRL2_MULTICOLOR_MASK,   10)
define_hex(VIC_CTRL2_40COLS_MASK,       08)
define_hex(VIC_CTRL2_XSCROLL_MASK,      07)
define_hex(VIC_CTRL2_DEFAULT_HIGH,      c0)
define(VIC_CTRL2_DEFAULT,                                            \
       (VIC_CTRL2_DEFAULT_HIGH | VIC_CTRL2_40COLS_MASK))

/* no multicolor, no 40 cols in x, xscroll=0  */
define_hex(VIC_CTRL2_MODE,                                           \
           00)

define_hex(VIC_ADDR,                    d018)
define_hex(VIC_ADDR_SCREENRAM_MASK,     f0)
define_hex(VIC_ADDR_SCREENRAM_STEP,     0040)
define_hex(VIC_ADDR_BITMAP_MASK,        0f)
define_hex(VIC_ADDR_BITMAP_STEP,        0400)
define_hex(VIC_ADDR_BITMAP_CHARSET1,    05)  /* (default) symbols    */
define_hex(VIC_ADDR_BITMAP_CHARSET2,    07)  /* lower case possible  */
define_hex(VIC_ADDR_DEFAULT_SCREENRAM,  10)
define(VIC_ADDR_DEFAULT,                                             \
       (VIC_ADDR_DEFAULT_SCREENRAM | VIC_ADDR_BITMAP_CHARSET1))

/* VIC_ADDR_SCREENRAM_ADDR(VICBANK_ADDR, VIC_ADDR_SCREENRAM)  */
macro_arg1_arg2(VIC_ADDR_SCREENRAM_ADDR,                             \
                (arg1 + arg2*VIC_ADDR_SCREENRAM_STEP))
/* VIC_ADDR_BITMAP_ADDR(VICBANK_ADDR, VIC_ADDR_BITMAP)  */
macro_arg1_arg2(VIC_ADDR_BITMAP_ADDR,                                \
                (arg1 + arg2*VIC_ADDR_BITMAP_STEP))

define_hex(VIC_IRR,                     d019)
define_hex(VIC_IMR,                     d01a)
define_hex(VIC_IMR_DISABLEALL_MASK,     00)
define_hex(VIC_IMR_RASTERLINE_MASK,     01)
define_hex(VIC_IMR_SHCOLLOSION_MASK,    02)
define_hex(VIC_IMR_SSCOLLOSION_MASK,    04)
define_hex(VIC_IMR_LIGHTPEN_MASK,       08)

define(VIC_IMR_IRQS,                                                 \
       (VIC_IMR_RASTERLINE_MASK))

define_hex(VIC_BORDERCOLOR,             d020)
define(VIC_BORDERCOLOR_DEFAULT,         VIC_COLOR_LIGHTBLUE)

#endif /* CHIP_VIC_DEF_H__  */
