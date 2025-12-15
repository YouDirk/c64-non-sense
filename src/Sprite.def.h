/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2025  Dirk "YouDirk" Lehmann
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


#ifndef SPRITE_DEF_H__
#define SPRITE_DEF_H__

#include "define.h"

/* GRAPHIX.DEF.H is nesting this header file.  */
include_def_h(Graphix)

header_ifndef(SPRITE)
header_define(SPRITE)

/* ***************************************************************  */

define_dec(SPRITE_WIDTH,                                     24)
define_dec(SPRITE_WIDTH_BYTES,                                3)
define_dec(SPRITE_HEIGHT,                                    21)

define_dec(SPRITE_POS_DISPLAY_BEGIN_X,                        0)
define_dec(SPRITE_POS_DISPLAY_BEGIN_Y,                        0)
define_dec(SPRITE_POS_DISPLAY_WIDTH,              /* ??? */ 512)
define_dec(SPRITE_POS_DISPLAY_HEIGHT,             /* ??? */ 256)

define_dec(SPRITE_POS_SMALLSCREEN_BEGIN_X,                   31)
define_dec(SPRITE_POS_SMALLSCREEN_BEGIN_Y,                   54)
define_dec(SPRITE_POS_SMALLSCREEN_WIDTH,                    304)
define_dec(SPRITE_POS_SMALLSCREEN_HEIGHT,                   192)

define_dec(SPRITE_POS_LARGESCREEN_BEGIN_X,                   24)
define_dec(SPRITE_POS_LARGESCREEN_BEGIN_Y,                   50)
define_dec(SPRITE_POS_LARGESCREEN_WIDTH,                    320)
define_dec(SPRITE_POS_LARGESCREEN_HEIGHT,                   200)

/* ***************************************************************  */

/* Struct for one or more sprite visual properties.  */
typedef_enum_begin(Sprite_properties_t)
  /* NONE = 0x00  */
  typedef_enum_hex(Sprite_properties_t, 00,                          \
                                               Sprite_props_none_mask)
  /* Enables MULTICOLOR mode for this sprite, using up to 3 colors,
   * but reduces the resolution in horizontal X direction by 2,
   * i.e. from 24 pxl/sprite to 12 pxl/sprite.
   *
   * The bit combinations in the bitmap_ram for multicolor sprites are
   * the following:
   *
   *   0b00=0x00 color:    transparency
   *   0b01=0x01 color:    GRAPHIX.BUFFER.SPRITES.SET.MULTICOLOR_0B01
   *   0b10=0x02 color:    GRAPHIX.BUFFER.SPRITES.SPRITE[i].SET.COLOR
   *   0b11=0x03 color:    GRAPHIX.BUFFER.SPRITES.SET.MULTICOLOR_0B11
   */
  typedef_enum_hex(Sprite_properties_t, 01,                          \
                                         Sprite_props_multicolor_mask)
  /* EXPANDS the size of this sprite in VERTICAL Y direction by 2,
   * i.e. from 21 pxl/sprite to 42 pxl/sprite.  But it will also
   * expand the pixel size of the bitmap in the sprite by 2!
   */
  typedef_enum_hex(Sprite_properties_t, 02,                          \
                                            Sprite_props_scale_y_mask)
  /* EXPANDS the size of this sprite in HORIZONTAL X direction by 2,
   * i.e. from 24 pxl/sprite to 48 pxl/sprite (multicolor: from 12
   * pxl/sprite to 24 pxl/sprite).  But it will also expand the pixel
   * size of the bitmap in the sprite by 2!
   */
  typedef_enum_hex(Sprite_properties_t, 04,                          \
                                            Sprite_props_scale_x_mask)
  /* Set the PRIORITY for rendering BACKGROUND to higher than the
   * sprite itself.  Therefore the sprite looks like to be behind the
   * background scene.
   *
   * If more than one sprites are overlapping, then the sprite with
   * the highest priority (priorities: sprite_0 > sprite_1 > ... >
   * sprite_7) decides if the background will be rendered before or
   * behind the sprites itself.
   */
  typedef_enum_hex(Sprite_properties_t, 08,                          \
                                       Sprite_props_prio_bground_mask)
typedef_enum_end(Sprite_properties_t)

/* ***************************************************************  */

typedef_uint8(                                    Sprite_frame_byte_t)

define(SPRITE_FRAME_BUFFER_Y_COUNT,                     SPRITE_HEIGHT)
define(SPRITE_FRAME_BUFFER_X_COUNT,                SPRITE_WIDTH_BYTES)

/* Size of SPRITE_FRAME_T::BUFFER.  */
define(SPRITE_FRAME_BUFFER_BUFSIZE,  SPRITE_HEIGHT*SPRITE_WIDTH_BYTES)

/* Memory layout of a sprite frame buffer.
 *
 * size:        SPRITE_HEIGHT*SPRITE_WIDTH_BYTES + 1 = 64
 *
 * byte layout:
 *   HiRes      - [msb: left pixel, pxl 2, ..., lsb: right pixel]
 *     Values:  0b0 transparent
 *              0b1 foreground, SPRITE_BUFFER_T::SET.COLOR
 *   MultiColor - [left pixel, 2nd pixel, 3rd pixel, right pixel]
 *     Values: 0b00 transparent
 *             0b01 shared, SPRITEMANAGER_BUFFER_T::SET.MULTICOLOR_0B01
 *             0b10 foreground, SPRITE_BUFFER_T::SET.COLOR
 *             0b11 shared, SPRITEMANAGER_BUFFER_T::SET.MULTICOLOR_0B11
 *
 * screen layout:
 *             buffer[ 0][0]  buffer[ 0][1]  buffer[ 0][2]
 *             buffer[ 1][0]  buffer[ 1][1]  buffer[ 1][2]
 *             buffer[ 2][0]  buffer[ 2][1]  buffer[ 2][2]
 *                 ...            ...            ...
 *                 ...            ...            ...
 *             buffer[20][0]  buffer[20][1]  buffer[20][2]
 *             tick_count
 *
 * usage:      SPRITE_FRAME_T::BUFFER[y_line][x_byte]
 *                = 0x81 | 0x30  // 0b10110001
 */
typedef_struct_begin(Sprite_frame_t)
  /* The screen part of the frame.  */
  typedef_struct_primit_array2(Sprite_frame_byte_t,           buffer,\
                                         SPRITE_FRAME_BUFFER_Y_COUNT,\
                                         SPRITE_FRAME_BUFFER_X_COUNT)

  /* Time in EINGINE::TICKS_T how long the frame will be displayed for
   * sprite animations.  Bit 7 (SPRITE_FRAME_TICKCOUNT_LAST_MASK)
   * indicates that this frame is the last frame in this animation.
   *
   *   tick_count == 0x00: frame displayed for 1 engine tick
   *   tick_count == 0x05: frame displayed for 6 engine ticks
   */
  typedef_struct_uint8(                                    tick_count)
typedef_struct_end(Sprite_frame_t)

/* ***************************************************************  */

/* Sprite Locator to a sprite frame buffer SPRITE_FRAME_T,
 * dereferencable using SPRITE_LOCATOR_DEREF().  Sprite Locators are
 * full compatible with the C64 VIC-II Sprite Pointer, default at
 * 0x07f8..0x07ff.
 */
typedef_uint8(                                    Sprite_locator_t)

/* Sprite Locator which points to the beginning of the Sprite RAM.  */
define(SPRITE_LOCATOR_FIRST,                                         \
       0+(GRAPHIX_BUFFER_SPRITERAM_RVAL - GRAPHIX_VICBANK_RVAL) >> 6)

/* Sprite Locator which points to the first item outside of the Sprite
 * RAM.
 */
define(SPRITE_LOCATOR_END,                                           \
       0+(GRAPHIX_BUFFER_SPRITERAM_RVAL +                            \
       GRAPHIX_BUFFER_SPRITERAM_BUFSIZE - GRAPHIX_VICBANK_RVAL) >> 6)

/* Number of Sprite Locators between SPRITE_LOCATOR_FIRST and
 * SPRITE_LOCATOR_END.
 */
define(SPRITE_LOCATOR_COUNT,   GRAPHIX_BUFFER_SPRITERAM_BUFSIZE >> 6)

/* Use this for dereferencing a sprite locator SPRITE_LOCATOR_T to a
 * sprite frame buffer SPPRITE_FRAME_T.
 */
macro_arg1(SPRITE_LOCATOR_DEREF, Sprite_frame_t*,                    \
           ((arg1) << 6) + GRAPHIX_VICBANK_RVAL)

/* Use this to create a sprite locator SPRITE_LOCATOR_T from a pointer
 * to a sprite frame buffer SPPRITE_FRAME_T.
 */
macro_arg1(SPRITE_LOCATOR_FROMREF, Sprite_locator_t,                 \
           ((arg1) - GRAPHIX_VICBANK_RVAL) >> 6)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Sprite_buffer_set_t)
  /* Vertical Y position of this sprite.  Common absolute positions
   * are defined above in SPRITE_POSY_*.
   */
  typedef_struct_uint8(                           pos_y)
  /* Horizontal X position of this sprite.  Common absolute positions
   * are defined above in SPRITE_POSX_*.
   */
  typedef_struct_uint9(                           pos_x)

  /* Color of sprite. Or color for bit combination 0b10=0x02, if
   * GRAPHIX.BUFFER.SPRITES.SPRITE[i].SET.PROPS =
   * SPRITE_PROPS_MULTICOLOR_MASK is set.
   */
  typedef_struct_enum(Graphix_color_t,            color)

  /* Properties of this sprite which are able to set via bit masks,
   * see above.
   */
  typedef_struct_enum(Sprite_properties_t,        props)

  /* Sprite locator, pointing to the current frame buffer of this
   * sprite.  Set this if you like to display a static sprite image
   * and don´t need spite animations.
   *
   * Make sure to set
   *
   *   GRAPHIX.ANIMS.SPRITES.SET.SPRITE[x] = NULL
   *
   * otherwise this LOCATOR will be automatically incremented/adapted
   * for sprite animation depending on SPRITE_FRAME_T::TICK_COUNT.
   */
  typedef_struct_primit(Sprite_locator_t,         locator)
typedef_struct_end(Sprite_buffer_set_t)

/* Datatype of this structure.  */
typedef_struct_begin(Sprite_buffer_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Sprite_buffer_set_t,      set)
typedef_struct_end(Sprite_buffer_t)

/* ***************************************************************  */

header_endif(SPRITE)

#endif /* SPRITE_DEF_H__  */
