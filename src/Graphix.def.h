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

/* ***************************************************************  */

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

/* Graphix buffer structure.  */
typedef_struct_begin(Graphix_buffer_t)
  typedef_struct_uint8_ptr(                            screen_ram)
  typedef_struct_uint8_ptr(                            bitmap_ram)
  typedef_struct_int8(                                 scroll_x)
  typedef_struct_int8(                                 scroll_y)

  typedef_struct_enum(Graphix_color_t,                 bordercolor)

  typedef_struct_nested(Graphix_buffer_sprites_t,      sprites)
typedef_struct_end(Graphix_buffer_t)

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

#ifndef CONF_DOUBLE_BUFFERING
  extern_var(Graphix_buffer_t*,              Graphix_buffer_shared_ptr)
#endif /* CONF_DOUBLE_BUFFERING  */
extern_var(Graphix_buffer_t*,                Graphix_buffer_back_ptr)

/* ***************************************************************  */

header_endif(GRAPHIX)

#endif /* GRAPHIX_DEF_H__  */
