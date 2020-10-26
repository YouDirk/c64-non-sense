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

define_hex(VIC_CTRL1,                   d011)
define_hex(VIC_CTRL1_RASTERLINE_MASK,   80)
define_hex(VIC_CTRL1_EXTCOLOR_MASK,     40)
define_hex(VIC_CTRL1_BITMAPMODE_MASK,   20)
define_hex(VIC_CTRL1_SCREEN_ON_MASK,    10)
define_hex(VIC_CTRL1_25ROWS_MASK,       08)
define_hex(VIC_CTRL1_YSCROLL_MASK,      07)

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

/* no multicolor, no 40 cols in x, xscroll=0  */
define_hex(VIC_CTRL2_MODE,                                           \
           00)

define_hex(VIC_ADDR,                    d018)
define_hex(VIC_ADDR_SCREENRAM_MASK,     f0)
define_hex(VIC_ADDR_BITMAP_MASK,        0f)
define_hex(VIC_ADDR_BITMAP_CHARSET1,    05)  /* (default) symbols    */
define_hex(VIC_ADDR_BITMAP_CHARSET2,    07)  /* lower case possible  */

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

#endif /* CHIP_VIC_DEF_H__  */
