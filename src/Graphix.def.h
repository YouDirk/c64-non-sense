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

/* SPRITE.DEF.H requires GRAPHIX_COLOR_T.  */
include_def_h(Sprite)

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

macro_arg1(GRAPHIX_SPRITES_2MASK,            1 << (arg1))

define_dec(GRAPHIX_SPRITES_MAXCOUNT,         8)

/* Selector for sprites to be used.  Full compatible with bit masks of
 * VIC-II sprites registers, i.e. enable register at 0xd015, etc.
 */
typedef_enum_begin(Graphix_sprite_selector_t)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_NONE_MASK,  \
                                              Graphix_sprite_none_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_0_MASK,     \
                                              Graphix_sprite_0_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_1_MASK,     \
                                              Graphix_sprite_1_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_2_MASK,     \
                                              Graphix_sprite_2_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_3_MASK,     \
                                              Graphix_sprite_3_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_4_MASK,     \
                                              Graphix_sprite_4_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_5_MASK,     \
                                              Graphix_sprite_5_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_6_MASK,     \
                                              Graphix_sprite_6_mask)
  typedef_enum_val(Graphix_sprite_selector_t, VIC_SPRITE_7_MASK,     \
                                              Graphix_sprite_7_mask)
typedef_enum_end(Graphix_sprite_selector_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Graphix_buffer_sprites_set_t)
  /* Sprites which will be enabled/visible.  Full compatible with
   * VIC-II register at 0xd015.
   */
  typedef_struct_enum(Graphix_sprite_selector_t,       enabled)

  /* Colors with bit combinations 0b01=0x01 and 0b11=0x03 for all
   * sprites which have GRAPHIX.BUFFER.SPRITES.SPRITE[i].SET.PROPS =
   * SPRITE_PROPS_MULTICOLOR_MASK set.
   */
  typedef_struct_enum(Graphix_color_t,                 multicolor_0b01)
  typedef_struct_enum(Graphix_color_t,                 multicolor_0b11)
typedef_struct_end(Graphix_buffer_sprites_set_t)

/* Sprites structure in graphic buffer.  */
typedef_struct_begin(Graphix_buffer_sprites_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Graphix_buffer_sprites_set_t,  set)

  /* Access to the 8 sprites of the C64 VIC-II graphic chip.  */
  typedef_struct_nested_array(Sprite_t, sprite,                      \
                                             GRAPHIX_SPRITES_MAXCOUNT)
  typedef_struct_nested(Sprite_t,            end)
typedef_struct_end(Graphix_buffer_sprites_t)

/* ***************************************************************  */

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

/* Size of GRAPHIX_BUFFER_T::SCREEN_RAM in bytes.  */
define(GRAPHIX_BUFFER_SCREENRAM_BUFSIZE,     GRAPHIX_SCREEN_CELLS)

/* Size of GRAPHIX_BUFFER_T::BITMAP_RAM in bytes.  */
define(GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE,     GRAPHIX_SCREEN_BYTES)

/* GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(color_bitset, color_bitzero)  */
macro_arg1_arg2(GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT,                 \
                                               ((arg1) << 4) | (arg2))

/* Graphix buffer structure.  */
typedef_struct_begin(Graphix_buffer_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Graphix_buffer_set_t,          set)

  /* Points to the screen RAM.  Every byte in this array is
   * representing a cell in the screen, ordered from left to right.
   * These are containing the cells fore- and background color for
   * every cell.  Use the GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT() macro
   * to convert two colors to the corresponding byte layout.
   *
   *   size:        GRAPHIX_BUFFER_SCREENRAM_BUFSIZE
   *   byte layout: [msb..2^4: color bit set, 2^3..lsb: color zero bit]
   *                GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(color_bitset,
   *                                                    color_bitzero)
   *
   *   cell layout: screen_ram[0]   screen_ram[1]   ... screen_ram[39]
   *                screen_ram[40]  screen_ram[41]  ... screen_ram[79]
   *                ...             ...                 ...
   *                ...             ...                 ...
   *                screen_ram[960] screen_ram[961] ... screen_ram[999]
   */
  typedef_struct_uint8_ptr(                            screen_ram)
  /* Points to the bitmap RAM.  Every byte in this array is
   * representing the pixels of the bitmap, ordered for every 8 bytes
   * (one cell of the screen) from the top down, then cell-wise from
   * left to right.
   *
   *   size:          GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE
   *   byte layout:   [msb: left pixel, pxl 2, ..., lsb: right pixel]
   *
   *   screen layout: bitmap_ram[0]  bitmap_ram[8]   bitmap_ram[16]
   *                  bitmap_ram[1]  bitmap_ram[9]   bitmap_ram[17]
   *                  bitmap_ram[2]  bitmap_ram[10]  bitmap_ram[18]
   *                  bitmap_ram[3]  bitmap_ram[11]  bitmap_ram[19]
   *                  bitmap_ram[4]  bitmap_ram[12]  bitmap_ram[20]
   *                  bitmap_ram[5]  bitmap_ram[13]  bitmap_ram[21]
   *                  bitmap_ram[6]  bitmap_ram[14]  bitmap_ram[22]
   *                  bitmap_ram[7]  bitmap_ram[15]  bitmap_ram[23]
   */
  typedef_struct_uint8_ptr(                            bitmap_ram)

  /* The sprites structure  */
  typedef_struct_nested(Graphix_buffer_sprites_t,      sprites)
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
 * 0xc000  | [SCREEN RAM] 1000 bytes                            |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xc3e8  | [RAM] 16 bytes                                     |
 *         |----------------------------------------------------|
 * 0xc3f8  | [RAM, SPRITE LOCATORS] 8 bytes                     |
 *         |   sprite_ptr = (0xc000 + (locator << 6))           |
 *         |----------------------------------------------------|
 * 0xc400  | [RAM, SPRITE BUFFERS] 3072 bytes                   |
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
  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEMC)
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
 * 0x8000  | [SCREEN RAM] 1000 bytes                            |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x83e8  | [RAM] 16 bytes                                     |
 *         |----------------------------------------------------|
 * 0x83f8  | [RAM, SPRITE LOCATORS] 8 bytes                     |
 *         |   sprite_ptr = (0x8000 + (locator << 6))           |
 *         |----------------------------------------------------|
 * 0x8400  | [RAM, SPRITE BUFFERS] 3072 bytes                   |
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
  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEM8)
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
 * 0x6400  | [RAM, SPRITE BUFFERS] 7168 bytes                   |
 *         |   = 8 sprites * 64 bytes/frame * 14 frames/sprite  |
 *         |                                                    |
 * 0x7fff  |                                                    |
 *         '----------------------------------------------------'
 */

prep_elif(GRAPHIX_MMAPPING equals 2)
  define    (_GRAPHIX_VICBANK_CIA2PRA,          CIA2_PRA_VICBANK_MEM4)
  define_hex(_GRAPHIX_SCREENRAM_x0X400_VICADDR,                    08)
  define_hex(_GRAPHIX_BITMAPRAM_x0X400_VICADDR,                    00)

/* ---------------------------------------------------------------  */

prep_else("GRAPHIX_MMAPPING")
  prep_error(                             "GRAPHIX_MMAPPING unknown!")
  define_hex(_GRAPHIX_VICBANK_CIA2PRA,                             00)
  define_hex(_GRAPHIX_SCREENRAM_x0X400_VICADDR,                    00)
  define_hex(_GRAPHIX_BITMAPRAM_x0X400_VICADDR,                    00)
prep_endif("GRAPHIX_MMAPPING")

/* ***************************************************************  */

#ifndef CONF_DOUBLE_BUFFERING
  extern_var(Graphix_buffer_t*,              Graphix_buffer_shared_ptr)
#endif /* CONF_DOUBLE_BUFFERING  */
extern_var(Graphix_buffer_t*,                Graphix_buffer_back_ptr)

/* ***************************************************************  */

header_endif(GRAPHIX)

#endif /* GRAPHIX_DEF_H__  */
