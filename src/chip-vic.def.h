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


#ifndef CHIP_VIC_DEF_H__
#define CHIP_VIC_DEF_H__

#include "define.h"

header_ifndef(CHIP_VIC)
header_define(CHIP_VIC)

/* *******************************************************************
 * colors of C64
 */

define_hex(VIC_COLOR_BLACK,                  00)
define_hex(VIC_COLOR_WHITE,                  01)
define_hex(VIC_COLOR_RED,                    02)
define_hex(VIC_COLOR_CYAN,                   03)
define_hex(VIC_COLOR_VIOLET,                 04)
define_hex(VIC_COLOR_GREEN,                  05)
define_hex(VIC_COLOR_BLUE,                   06)
define_hex(VIC_COLOR_YELLOW,                 07)
define_hex(VIC_COLOR_ORANGE,                 08)
define_hex(VIC_COLOR_BROWN,                  09)
define_hex(VIC_COLOR_LIGHTRED,               0a)
define_hex(VIC_COLOR_GRAY1,                  0b)
define_hex(VIC_COLOR_GRAY2,                  0c)
define_hex(VIC_COLOR_LIGHTGREEN,             0d)
define_hex(VIC_COLOR_LIGHTBLUE,              0e)
define_hex(VIC_COLOR_GRAY3,                  0f)

/* end of colors
 * *******************************************************************
 * sprite enum masks
 */

define_hex(VIC_SPRITE_NONE_MASK,             00)
define_hex(VIC_SPRITE_0_MASK,                01)
define_hex(VIC_SPRITE_1_MASK,                02)
define_hex(VIC_SPRITE_2_MASK,                04)
define_hex(VIC_SPRITE_3_MASK,                08)
define_hex(VIC_SPRITE_4_MASK,                10)
define_hex(VIC_SPRITE_5_MASK,                20)
define_hex(VIC_SPRITE_6_MASK,                40)
define_hex(VIC_SPRITE_7_MASK,                80)

/* end of sprite enum masks
 * ***************************************************************  */

register_void_ptr_hex(d000,                  VIC_BASE)

/* ***************************************************************  */

register_uint8_hex(d000,                     VIC_SPR_POSITIONS)
define_hex(VIC_SPR_POSITIONS_BUFSIZE,        10)

register_uint8_hex(d000,                     VIC_SPR0_POSITION_X)
register_uint8_hex(d001,                     VIC_SPR0_POSITION_Y)
register_uint8_hex(d002,                     VIC_SPR1_POSITION_X)
register_uint8_hex(d003,                     VIC_SPR1_POSITION_Y)
register_uint8_hex(d004,                     VIC_SPR2_POSITION_X)
register_uint8_hex(d005,                     VIC_SPR2_POSITION_Y)
register_uint8_hex(d006,                     VIC_SPR3_POSITION_X)
register_uint8_hex(d007,                     VIC_SPR3_POSITION_Y)
register_uint8_hex(d008,                     VIC_SPR4_POSITION_X)
register_uint8_hex(d009,                     VIC_SPR4_POSITION_Y)
register_uint8_hex(d00a,                     VIC_SPR5_POSITION_X)
register_uint8_hex(d00b,                     VIC_SPR5_POSITION_Y)
register_uint8_hex(d00c,                     VIC_SPR6_POSITION_X)
register_uint8_hex(d00d,                     VIC_SPR6_POSITION_Y)
register_uint8_hex(d00e,                     VIC_SPR7_POSITION_X)
register_uint8_hex(d00f,                     VIC_SPR7_POSITION_Y)

register_uint8_hex(d010,                     VIC_SPRHI_POSITION_X)

/* ***************************************************************  */

register_uint8_hex(d011,                               VIC_CTRL1)
define_hex(VIC_CTRL1_RASTERLINE_MASK,                  80)
define_hex(VIC_CTRL1_EXTCOLOR_MASK,                    40)
define_hex(VIC_CTRL1_BITMAPMODE_MASK,                  20)
define_hex(VIC_CTRL1_SCREEN_ON_MASK,                   10)
define_hex(VIC_CTRL1_25ROWS_MASK,                      08)
define_hex(VIC_CTRL1_YSCROLL_MASK,                     07)
define_hex(VIC_CTRL1_YSCROLL_DEFAULT,                  03)
define(VIC_CTRL1_DEFAULT,                                            \
       VIC_CTRL1_SCREEN_ON_MASK | VIC_CTRL1_25ROWS_MASK              \
       | VIC_CTRL1_YSCROLL_DEFAULT)

/* rasterline bit, bitmap mode, enable screen, no 25 rows, yscroll=0  */
define(VIC_CTRL1_MODE,                                               \
       VIC_CTRL1_BITMAPMODE_MASK | VIC_CTRL1_SCREEN_ON_MASK          \
       | (VIC_CTRL1_RASTERLINE_MASK & (VIC_RASTERLINE_VBLANK >> 1)))

register_uint8_hex(d012,                               VIC_RASTERLINE)
define_hex(VIC_RASTERLINE_MASK,                        ff)
define_hex(VIC_RASTERL_MAX_PAL_MASK,                   f0)
define_hex(VIC_RASTERL_CYCLES_PAL6569_VAL,             7f)
define_hex(VIC_RASTERL_CYCLES_PALN6572_VAL,            9b)
define    (VIC_RASTERL_CYCLES_PAL_THRESHOLD,                         \
           VIC_RASTERL_CYCLES_PAL6569_VAL                            \
           + (VIC_RASTERL_CYCLES_PALN6572_VAL                        \
              - VIC_RASTERL_CYCLES_PAL6569_VAL)/2)
define_hex(VIC_RASTERL_SCREENBEGIN_24ROWS,             34)
define_hex(VIC_RASTERL_SCREENEND_24ROWS,               f7)
define_hex(VIC_RASTERL_SCREENBEGIN_25ROWS,             30)
define_hex(VIC_RASTERL_SCREENEND_25ROWS,               fb)
define_hex(VIC_RASTERL_BACKPORCH_PAL_43,               0e)
define_hex(VIC_RASTERL_BACKPORCH_PAL_169,              21)
define_hex(VIC_RASTERL_BACKPORCH_NTSC_43,              1a)
define_hex(VIC_RASTERL_FRONTPORCH_PAL_43,             11f)
define_hex(VIC_RASTERL_FRONTPORCH_PAL_169,            10a)
define    (VIC_RASTERL_FRONTPORCH_NTSC_43,  VIC_RASTERL_MAX_NTSC_6567R56A)

define(VIC_RASTERLINE_VBLANK,            VIC_RASTERL_SCREENEND_24ROWS)
define(VIC_RASTERLINE_MODE,                                          \
       VIC_RASTERLINE_VBLANK & VIC_RASTERLINE_MASK)

/* sprite enable bits  */
register_uint8_hex(d015,                               VIC_SPR_ENABLE)

register_uint8_hex(d016,                               VIC_CTRL2)
define_hex(VIC_CTRL2_MULTICOLOR_MASK,                  10)
define_hex(VIC_CTRL2_40COLS_MASK,                      08)
define_hex(VIC_CTRL2_XSCROLL_MASK,                     07)
define_hex(VIC_CTRL2_XSCROLL_DEFAULT,                  00)
define_hex(VIC_CTRL2_HIGH_DEFAULT,                     c0)
define(VIC_CTRL2_DEFAULT,                                            \
       VIC_CTRL2_HIGH_DEFAULT | VIC_CTRL2_40COLS_MASK
       | VIC_CTRL2_XSCROLL_DEFAULT)

/* no multicolor, no 40 cols in x, xscroll=0  */
define(VIC_CTRL2_MODE,                                               \
       00)

/* sprite Y expansion enable bits  */
register_uint8_hex(d017,                               VIC_SPR_EXPY)

/* ***************************************************************  */

register_uint8_hex(d018,                               VIC_ADDR)

/* all bits [7..4] are used  */
define_hex(VIC_ADDR_SCREENRAM_SHIFT,                   4)
define_hex(VIC_ADDR_SCREENRAM_STEP,                    0400)
define_hex(VIC_ADDR_SCREENRAM_DEFAULT,                 01)

/* Bitmap mode: just bit 3 able to set, bits [2..0] unused
 * Text mode  : bits [3..1] able to set, bit 0 unused
 */
define_hex(VIC_ADDR_BITMAP_MASK,                       0f)
define_hex(VIC_ADDR_BITMAP_STEP,                       0400)
/* If VICBANK_MEM0 or VICBANK_MEM8 is selected, then VIC-II has DMA
 * access to the Character ROM at address (0x1000 - 0x1fff), which is
 * accessable via memory at location (0xd000 - 0xe000).
 *
 * The VIC-II DMA address is calculated in that case as
 * ((VIC_ADDR_BITMAP_CHARSET >> 1) << 11).
 */
define_hex(VIC_ADDR_BITMAP_CHARSET1,    05)  /* (default) symbols    */
define_hex(VIC_ADDR_BITMAP_CHARSET2,    07)  /* lower case possible  */
define_hex(VIC_ADDR_BITMAP_DEFAULT,     00)

define(VIC_ADDR_DEFAULT,                                             \
               VIC_ADDR_SCREENRAM_DEFAULT << VIC_ADDR_SCREENRAM_SHIFT
               | VIC_ADDR_BITMAP_DEFAULT)

/* VIC_ADDR_SCREENRAM_ADDR(VICBANK_ADDR, VIC_ADDR_SCREENRAM)  */
macro_arg1_arg2(VIC_ADDR_SCREENRAM_ADDR, void*,                      \
                              (arg1) + (arg2)*VIC_ADDR_SCREENRAM_STEP)
/* VIC_ADDR_BITMAP_ADDR(VICBANK_ADDR, VIC_ADDR_BITMAP)  */
macro_arg1_arg2(VIC_ADDR_BITMAP_ADDR, void*,                         \
                                 (arg1) + (arg2)*VIC_ADDR_BITMAP_STEP)

/* ***************************************************************  */

register_uint8_hex(d019,                               VIC_IRR)
register_uint8_hex(d01a,                               VIC_IMR)
define_hex(VIC_IMR_DISABLEALL_MASK,                    00)
define_hex(VIC_IMR_RASTERLINE_MASK,                    01)
define_hex(VIC_IMR_SHCOLLOSION_MASK,                   02)
define_hex(VIC_IMR_SSCOLLOSION_MASK,                   04)
define_hex(VIC_IMR_LIGHTPEN_MASK,                      08)
define_hex(VIC_IRR_IRQGLOBAL_MASK,                     80)

define(VIC_IMR_IRQMODE,                                              \
       VIC_IRR_IRQGLOBAL_MASK | VIC_IMR_RASTERLINE_MASK)

/* sprite background priority enable bits  */
register_uint8_hex(d01b,                     VIC_SPR_BGPRIO)

/* sprite multicolor enable bits  */
register_uint8_hex(d01c,                     VIC_SPR_MCOLOR)

/* sprite X expansion enable bits  */
register_uint8_hex(d01d,                     VIC_SPR_EXPX)

register_uint8_hex(d020,                     VIC_BORDERCOLOR)
define_hex(VIC_BORDERCOLOR_RVAL,             d020) /* inline asm  */
define(VIC_BORDERCOLOR_DEFAULT,              VIC_COLOR_LIGHTBLUE)

/* ***************************************************************  */

/* multicolor for bit combination 0b01 = 0x01  */
register_uint8_hex(d025,                     VIC_SPR_MCOLOR0_0B01)
define_hex(VIC_SPR_MCOLOR0_0B01_DEFAULT,     f4)

/* multicolor for bit combination 0b11 = 0x03  */
register_uint8_hex(d026,                     VIC_SPR_MCOLOR1_0B11)
define_hex(VIC_SPR_MCOLOR1_0B11_DEFAULT,     f0)

/* ***************************************************************  */

/* sprite color (multicolor: bit combination 0b10 = 0x02)  */
register_uint8_hex(d027,                     VIC_SPR_COLORS)
define_hex(VIC_SPR_COLORS_BUFSIZE,           08)

register_uint8_hex(d027,                     VIC_SPR0_COLOR)
define_hex(VIC_SPR0_COLOR_DEFAULT,           f1)
register_uint8_hex(d028,                     VIC_SPR1_COLOR)
define_hex(VIC_SPR1_COLOR_DEFAULT,           f2)
register_uint8_hex(d029,                     VIC_SPR2_COLOR)
define_hex(VIC_SPR2_COLOR_DEFAULT,           f3)
register_uint8_hex(d02a,                     VIC_SPR3_COLOR)
define_hex(VIC_SPR3_COLOR_DEFAULT,           f4)
register_uint8_hex(d02b,                     VIC_SPR4_COLOR)
define_hex(VIC_SPR4_COLOR_DEFAULT,           f5)
register_uint8_hex(d02c,                     VIC_SPR5_COLOR)
define_hex(VIC_SPR5_COLOR_DEFAULT,           f6)
register_uint8_hex(d02d,                     VIC_SPR6_COLOR)
define_hex(VIC_SPR6_COLOR_DEFAULT,           f7)
register_uint8_hex(d02e,                     VIC_SPR7_COLOR)
define_hex(VIC_SPR7_COLOR_DEFAULT,           fc)

/* ***************************************************************  */

header_endif(CHIP_VIC)

#endif /* CHIP_VIC_DEF_H__  */
