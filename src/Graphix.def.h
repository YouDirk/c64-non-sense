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


#ifndef GRAPHIX_DEF_H__
#define GRAPHIX_DEF_H__

#include "define.h"

header_ifndef(GRAPHIX)
header_define(GRAPHIX)

include_def_h(chip-vic)
include_def_h(chip-cia)

/* *******************************************************************
 * colors of C64
 *
 * Full compatible with the common known color codes for the C64.
 */

typedef_enum_begin(Graphix_color_t)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_BLACK,   Graphix_black)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_WHITE,   Graphix_white)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_RED,     Graphix_red)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_CYAN,    Graphix_cyan)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_VIOLET,  Graphix_violet)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_GREEN,   Graphix_green)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_BLUE,    Graphix_blue)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_YELLOW,  Graphix_yellow)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_ORANGE,  Graphix_orange)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_BROWN,   Graphix_brown)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_LIGHTRED,Graphix_lightred)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_GRAY1,   Graphix_gray1)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_GRAY2,   Graphix_gray2)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_LIGHTGREEN,            \
                                                       Graphix_lightgreen)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_LIGHTBLUE,             \
                                                       Graphix_lightblue)
  typedef_enum_val(Graphix_color_t, VIC_COLOR_GRAY3,   Graphix_gray3)
typedef_enum_end(Graphix_color_t)

/* end of colors
 * ***************************************************************  */

/* SPRITEMANAGER.DEF.H requires GRAPHIX_COLOR_T.  */
include_def_h(SpriteManager)

/* *******************************************************************
 * screen constants
 */

define_dec(GRAPHIX_SCREEN_BYTES,                               8000)
define_dec(GRAPHIX_SCREEN_CELLS,                               1000)

define_dec(GRAPHIX_SCREEN_HIRES_PIXELS,                       64000)
define_dec(GRAPHIX_SCREEN_HIRES_RES_X,                          320)
define_dec(GRAPHIX_SCREEN_HIRES_RES_Y,                          200)
define_dec(GRAPHIX_SCREEN_HIRES_CELL_PIXELS,                     64)
define_dec(GRAPHIX_SCREEN_HIRES_CELL_WIDTH,                       8)
define_dec(GRAPHIX_SCREEN_HIRES_CELL_HEIGHT,                      8)

define_dec(GRAPHIX_SCREEN_MULTICOLOR_PIXELS,                  32000)
define_dec(GRAPHIX_SCREEN_MULTICOLOR_RES_X,                     160)
define_dec(GRAPHIX_SCREEN_MULTICOLOR_RES_Y,                     200)
define_dec(GRAPHIX_SCREEN_MULTICOLOR_CELL_PIXELS,                32)
define_dec(GRAPHIX_SCREEN_MULTICOLOR_CELL_WIDTH,                  4)
define_dec(GRAPHIX_SCREEN_MULTICOLOR_CELL_HEIGHT,                 8)

define_dec(GRAPHIX_SCREEN_CELLS_X,                               40)
define_dec(GRAPHIX_SCREEN_CELLS_Y,                               25)
define_dec(GRAPHIX_SCREEN_CELLS_BYTES,                            8)

/* end of screen constants
 * ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Graphix_buffer_set_t)
  /* Scrolls the vertical Y position of background.  It is recommended
   * to DISABLE the 25-ROWS mode of the screen, if you are using
   * scrolling.
   *
   * If 25-ROWS mode is enabled, then set this value to SCROLL_Y = 3
   * (default initialization), to see the whole SCREEN_RAM/BITMAP_RAM
   * on screen.
   *
   * TODO: How to enable/disable 25-ROWS mode.
   */
  typedef_struct_uint3(                                scroll_y)
  /* Scrolls the horizontal X position of background.  It is
   * recommended to DISABLE the 40-COLS mode of the screen, if you are
   * using scrolling.
   *
   * If 40-COLS mode is enabled, then set this value to SCROLL_X = 0
   * (default initialization), to see the whole SCREEN_RAM/BITMAP_RAM
   * on screen.
   *
   * TODO: How to enable/disable 40-COLS mode.
   */
  typedef_struct_uint3(                                scroll_x)

  /* Color for the border of the screen.  */
  typedef_struct_enum(Graphix_color_t,                 bordercolor)
typedef_struct_end(Graphix_buffer_set_t)

/* ---------------------------------------------------------------  */

/* Points to the Screen RAM.  Every byte in this array is representing
 * a cell in the screen, ordered from left to right.  These are
 * containing the cells fore- and background color for every cell.
 * Use the GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT() macro to convert two
 * colors to the corresponding byte layout.
 *
 * size:        GRAPHIX_BUFFER_SCREENRAM_BUFSIZE = 1000
 *
 * byte layout: [msb..2^4: color bit set, 2^3..lsb: color bit zero]
 *              GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(color_bitset,
 *                                                  color_bitzero)
 *
 * screen layout:
 *   screen_ram[ 0][0]  screen_ram[ 0][1]  ...  screen_ram[ 0][39]
 *   screen_ram[ 1][0]  screen_ram[ 1][1]  ...  screen_ram[ 1][39]
 *   ...                ...                     ...
 *   ...                ...                     ...
 *   screen_ram[24][0]  screen_ram[24][1]  ...  screen_ram[24][39]
 *
 *
 * usage:       GRAPHIX_BUFFER_SCREENRAM[cell_y][cell_x]
 *                = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(
 *                                    Graphix_COLOR_FOREGROUND,
 *                                    Graphix_COLOR_BACKGROUND);
 */

/* Size of GRAPHIX_BUFFER_SCREENRAM in bytes.  */
define(GRAPHIX_BUFFER_SCREENRAM_BUFSIZE,  GRAPHIX_SCREEN_CELLS)

/* End of GRAPHIX_BUFFER_BITMAPRAM.  */
register_constnested(GRAPHIX_BUFFER_SCREENRAM_RVAL
        +GRAPHIX_BUFFER_SCREENRAM_BUFSIZE, Graphix_bitmapram_byte_t, \
                                          GRAPHIX_BUFFER_SCREENRAM_END)

typedef_uint8(                             Graphix_screenram_byte_t)
register_nested_array(GRAPHIX_BUFFER_SCREENRAM_RVAL,                 \
               Graphix_screenram_byte_t[GRAPHIX_SCREEN_CELLS_Y],     \
                                          GRAPHIX_BUFFER_SCREENRAM,  \
                                          GRAPHIX_SCREEN_CELLS_X)

/* GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(color_foreground,
 *                                     color_background)
 */
macro_arg1_arg2(GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT,                 \
                Graphix_screenram_byte_t, ((arg1) << 4) | (arg2))

/* ---------------------------------------------------------------  */

/* Points to the Bitmap RAM.  Every byte in this array is representing
 * the pixels of the bitmap, ordered for every 8 bytes (one cell of
 * the screen) from the top down, then cell-wise from left to right.
 *
 * size:        GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE = 8000
 *
 * byte layout:
 *   HiRes      - [msb: left pixel, pxl 2, ..., lsb: right pixel]
 *                Values: 0b0 cell background color of Screen RAM
 *                        0b1 cell foreground color of Screen RAM
 *   MultiColor - [left pixel, 2nd pixel, 3rd pixel, right pixel]
 *                Values: 0b00 common Graphix background color
 *                        0b01 cell foreground color of Screen RAM
 *                        0b10 cell background color of Screen RAM
 *                        0b11 cell color of Color RAM
 *
 * screen layout:
 *   bitmap_ram[ 0][0][0] bitmap_ram[ 0][1][0] ... bitmap_ram[ 0][39][0]
 *   bitmap_ram[ 0][0][1] bitmap_ram[ 0][1][1]     ...
 *   bitmap_ram[ 0][0][2] bitmap_ram[ 0][1][2]     ...
 *   bitmap_ram[ 0][0][3] bitmap_ram[ 0][1][3]     ...
 *   bitmap_ram[ 0][0][4] bitmap_ram[ 0][1][4]     ...
 *   bitmap_ram[ 0][0][5] bitmap_ram[ 0][1][5]     ...
 *   bitmap_ram[ 0][0][6] bitmap_ram[ 0][1][6]     ...
 *   bitmap_ram[ 0][0][7] bitmap_ram[ 0][1][7] ... bitmap_ram[ 0][39][7]
 *
 *   bitmap_ram[ 1][0][0] bitmap_ram[ 1][1][0] ... bitmap_ram[ 1][39][0]
 *   ...                  ...                      ...
 *   bitmap_ram[ 1][0][7] bitmap_ram[ 1][1][7] ... bitmap_ram[ 1][39][7]
 *
 *   ...                  ...                      ...
 *   ...                  ...                      ...
 *
 *   bitmap_ram[24][0][0] bitmap_ram[24][1][0] ... bitmap_ram[24][39][0]
 *   ...                  ...                      ...
 *   bitmap_ram[24][0][7] bitmap_ram[24][1][7] ... bitmap_ram[24][39][7]
 *
 *
 * usage:       GRAPHIX_BUFFER_BITMAPRAM[cell_y][cell_x][byte 0..7]
 *                = 0x81 | 0x30  // 0b10110001
 */

/* Size of GRAPHIX_BUFFER_BITMAPRAM in bytes.  */
define(GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE,  GRAPHIX_SCREEN_BYTES)

/* End of GRAPHIX_BUFFER_BITMAPRAM.  */
register_constnested(GRAPHIX_BUFFER_BITMAPRAM_RVAL
        +GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE, Graphix_bitmapram_byte_t, \
                                          GRAPHIX_BUFFER_BITMAPRAM_END)

typedef_uint8(                             Graphix_bitmapram_byte_t)
register_nested_array(GRAPHIX_BUFFER_BITMAPRAM_RVAL,                 \
        Graphix_bitmapram_byte_t[GRAPHIX_SCREEN_CELLS_Y]             \
                                [GRAPHIX_SCREEN_CELLS_X],            \
                                          GRAPHIX_BUFFER_BITMAPRAM,  \
                                          GRAPHIX_SCREEN_CELLS_BYTES)

/* ---------------------------------------------------------------  */

register_uint8(GRAPHIX_BUFFER_SPRITERAM_RVAL,                        \
                                             GRAPHIX_BUFFER_SPRITERAM)

/* Size of GRAPHIX_BUFFER_SPRITERAM in bytes.  */
define(GRAPHIX_BUFFER_SPRITERAM_BUFSIZE,                             \
                                    _GRAPHIX_BUFFER_SPRITERAM_BUFSIZE)

/* ---------------------------------------------------------------  */

/* Graphix buffer structure.  */
typedef_struct_begin(Graphix_buffer_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Graphix_buffer_set_t,          set)

  /* The SpriteManager structure  */
  typedef_struct_nested(SpriteManager_buffer_t,        sprites)
typedef_struct_end(Graphix_buffer_t)

/* ***************************************************************  */

/* VIC-II revision structure  */
typedef_enum_begin(Graphix_vicrev_t)
  /* default PAL revision  (europe)  */
  typedef_enum_hex(Graphix_vicrev_t, 37,      Graphix_vicrev_pal6569_e)
  /* Drean PAL-N revision (south america)  */
  typedef_enum_hex(Graphix_vicrev_t, 40,     Graphix_vicrev_paln6572_e)
  /* default NTSC revision (usa)  */
  typedef_enum_hex(Graphix_vicrev_t, 06,   Graphix_vicrev_ntsc6567r8_e)
  /* old NTSC revision  */
  typedef_enum_hex(Graphix_vicrev_t, 05, Graphix_vicrev_ntsc6567r56a_e)
typedef_enum_end(Graphix_vicrev_t)

/* ***************************************************************  */

/* Used to select a character set from C64 Character ROM.  */
typedef_enum_begin(Graphix_charset_t)
  typedef_enum_hex(Graphix_charset_t, 00,   Graphix_charset1_symbols_e)
  typedef_enum_hex(Graphix_charset_t, 01, Graphix_charset2_lowercase_e)
typedef_enum_end(Graphix_charset_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Graphix_set_t)
  /* Charset which will be set after termination of the engine.  */
  typedef_struct_enum(Graphix_charset_t,     charset_exit)
typedef_struct_end(Graphix_set_t)

/* Structure of static members for module.  */
typedef_struct_begin(Graphix_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Graphix_set_t,       set)

  /* TRUE if C64 has a PAL VIC, otherwise we are on a NTSC machine.  */
  typedef_struct_uint8(                      is_pal)

  /* Revision of VIC-II graphic chip.  */
  typedef_struct_enum(Graphix_vicrev_t,      vic_revision)

  /* The logical graphic buffer.  */
  typedef_struct_nested(Graphix_buffer_t,    buffer)

typedef_struct_end(Graphix_t)

/* ***************************************************************  */

/* Static members of this module.  */
extern_var(Graphix_t,                        Graphix)

/* ***************************************************************  */

define_dec(GRAPHIX_MMAPPING,                 0)

/* GRAPHIX_MMAPPING = 0
 *
 * Memory mapping for Graphix with VIC-II bank mapped to 0xc000.
 *
 *         : Read access          | Write access                :
 *         : ***********          | ************                :
 *         :                      |                             :
 *         .----------------------|-----------------------------.
 * 0xb000  | [RAM, GRAPHIX RAM] 4096 bytes                      |
 *         |   Graphix_buffer_shared, Graphix_buffer_back       |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xc000  | [SCREEN RAM] 1000 bytes                            |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xc3e8  | [RAM] 16 bytes                                     |
 *         |----------------------------------------------------|
 * 0xc3f8  | [RAM, SPRITE LOCATORS] 8 bytes                     |
 *         |   sprite_ptr = (0xc000 + (locator << 6))           |
 *         |----------------------------------------------------|
 * 0xc400  | [RAM, SPRITE RAM] 3072 bytes                       |
 *         |   = 8 sprites * 64 bytes/frame * 6 frames/sprite   |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xd000  | [I/O] VIC-II, SID,  [ROM]        [RAM]             |
 *         |       CIA 1/2,        Character                    |
 *         |       Color RAM       ROM                          |
 *         |----------------------.-----------------------------|
 * 0xe000  | [ROM] Kernal         | [BITMAP RAM] 8000 bytes     |
 *         |                      |                             |
 *         |                      |                             |
 *         |                      |                             |
 *         |----------------------|-----------------------------|
 * 0xff40  | [ROM] Kernal         | [RAM] 186 bytes             |
 *         |----------------------'-----------------------------|
 * 0xfffa  | [CPU] MOS 6510, vectors (NMI, RESET, IRQ)          |
 * 0xffff  |       defined in 'Kernal'                          |
 *         '----------------------------------------------------'
 */

prep_if(GRAPHIX_MMAPPING equals 0)
  register_uint8_hex(b000,                      GRAPHIX_RAM)
  define_hex(GRAPHIX_RAM_RVAL,                  b000)
  define_hex(GRAPHIX_RAM_BUFSIZE,               1000)

  define_hex(GRAPHIX_BUFFER_SCREENRAM_RVAL,     c000)
  define_hex(GRAPHIX_BUFFER_BITMAPRAM_RVAL,     e000) /* not reabable  */

  define_hex(GRAPHIX_BUFFER_SPRITERAM_RVAL,     c400)
  define_hex(_GRAPHIX_BUFFER_SPRITERAM_BUFSIZE,  c00) /* 3072 bytes  */

  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEMC)
  define    (_GRAPHIX_VICBANK_RVAL,             CIA2_PRA_VICBANK_ADDRC)

  define_hex(_GRAPHIX_SCREENRAM_x0X400_VICADDR,                    00)
  define_hex(_GRAPHIX_BITMAPRAM_x0X400_VICADDR,                    08)

/* ---------------------------------------------------------------  */

/* GRAPHIX_MMAPPING = 1
 *
 * Memory mapping for Graphix with VIC-II bank mapped to 0x8000.
 *
 *         : CPU access           | VIC DMA access              :
 *         : **********           | **************              :
 *         :                      |                             :
 *         .----------------------|-----------------------------.
 * 0x7000  | [RAM, GRAPHIX RAM] 4096 bytes                      |
 *         |   Graphix_buffer_shared, Graphix_buffer_back       |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x8000  | [SCREEN RAM] 1000 bytes                            |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x83e8  | [RAM] 16 bytes                                     |
 *         |----------------------------------------------------|
 * 0x83f8  | [RAM, SPRITE LOCATORS] 8 bytes                     |
 *         |   sprite_ptr = (0x8000 + (locator << 6))           |
 *         |----------------------------------------------------|
 * 0x8400  | [RAM, SPRITE RAM] 3072 bytes                       |
 *         |   = 8 sprites * 64 bytes/frame * 6 frames/sprite   |
 *         |                                                    |
 *         |----------------------.-----------------------------|
 * 0x9000  | [RAM] 4096 bytes     | [ROM] Character ROM         |
 *         |                      |                             |
 *         |                      |                             |
 *         |----------------------'-----------------------------|
 * 0xa000  | [BITMAP RAM] 8000 bytes                            |
 *         |                                                    |
 *         |                                                    |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xbf40  | [RAM] 192 bytes                                    |
 * 0xbfff  |                                                    |
 *         '----------------------------------------------------'
 */

prep_elif(GRAPHIX_MMAPPING equals 1)
  register_uint8_hex(7000,                      GRAPHIX_RAM)
  define_hex(GRAPHIX_RAM_RVAL,                  7000)
  define_hex(GRAPHIX_RAM_BUFSIZE,               1000)

  define_hex(GRAPHIX_BUFFER_SCREENRAM_RVAL,     8000)
  define_hex(GRAPHIX_BUFFER_BITMAPRAM_RVAL,     a000)

  define_hex(GRAPHIX_BUFFER_SPRITERAM_RVAL,     8400)
  define_hex(_GRAPHIX_BUFFER_SPRITERAM_BUFSIZE,  c00) /* 3072 bytes  */

  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEM8)
  define    (_GRAPHIX_VICBANK_RVAL,             CIA2_PRA_VICBANK_ADDR8)

  define_hex(_GRAPHIX_SCREENRAM_x0X400_VICADDR,                    00)
  define_hex(_GRAPHIX_BITMAPRAM_x0X400_VICADDR,                    08)

/* ---------------------------------------------------------------  */

/* GRAPHIX_MMAPPING = 2
 *
 * Memory mapping for Graphix with VIC-II bank mapped to 0x4000.
 *
 *         : CPU access           | VIC DMA access              :
 *         : **********           | **************              :
 *         :                      |                             :
 *         .----------------------|-----------------------------.
 * 0x3000  | [RAM, GRAPHIX RAM] 4096 bytes                      |
 *         |   Graphix_buffer_shared, Graphix_buffer_back       |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x4000  | [BITMAP RAM] 8000 bytes                            |
 *         |                                                    |
 *         |                                                    |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x5f40  | [RAM] 192 bytes                                    |
 *         |----------------------------------------------------|
 * 0x6000  | [SCREEN RAM] 1000 bytes                            |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x63e8  | [RAM] 16 bytes                                     |
 *         |----------------------------------------------------|
 * 0x63f8  | [RAM, SPRITE LOCATORS] 8 bytes                     |
 *         |   sprite_ptr = (0x4000 + (locator << 6))           |
 *         |----------------------------------------------------|
 * 0x6400  | [RAM, SPRITE RAM] 7168 bytes                       |
 *         |   = 8 sprites * 64 bytes/frame * 14 frames/sprite  |
 *         |                                                    |
 * 0x7fff  |                                                    |
 *         '----------------------------------------------------'
 */

prep_elif(GRAPHIX_MMAPPING equals 2)
  register_uint8_hex(3000,                      GRAPHIX_RAM)
  define_hex(GRAPHIX_RAM_RVAL,                  3000)
  define_hex(GRAPHIX_RAM_BUFSIZE,               1000)

  define_hex(GRAPHIX_BUFFER_SCREENRAM_RVAL,     6000)
  define_hex(GRAPHIX_BUFFER_BITMAPRAM_RVAL,     4000)

  define_hex(GRAPHIX_BUFFER_SPRITERAM_RVAL,     6400)
  define_hex(_GRAPHIX_BUFFER_SPRITERAM_BUFSIZE, 1c00) /* 7168 bytes  */

  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEM4)
  define    (_GRAPHIX_VICBANK_RVAL,             CIA2_PRA_VICBANK_ADDR4)

  define_hex(_GRAPHIX_SCREENRAM_x0X400_VICADDR,                    08)
  define_hex(_GRAPHIX_BITMAPRAM_x0X400_VICADDR,                    00)

/* ---------------------------------------------------------------  */

prep_else("GRAPHIX_MMAPPING")
  prep_error(                             "GRAPHIX_MMAPPING unknown!")
prep_endif("GRAPHIX_MMAPPING")

/* ***************************************************************  */

#ifndef CONF_DOUBLE_BUFFERING
  extern_var(Graphix_buffer_t*,              Graphix_buffer_shared_ptr)
#endif /* CONF_DOUBLE_BUFFERING  */
extern_var(Graphix_buffer_t*,                Graphix_buffer_back_ptr)

/* ***************************************************************  */

header_endif(GRAPHIX)

#endif /* GRAPHIX_DEF_H__  */
