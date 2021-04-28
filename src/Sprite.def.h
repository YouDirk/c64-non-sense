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


#ifndef SPRITE_DEF_H__
#define SPRITE_DEF_H__

#include "define.h"

/* GRAPHIX.DEF.H is nesting this header file.  */
include_def_h(Graphix)

header_ifndef(SPRITE)
header_define(SPRITE)

/* ***************************************************************  */

/* Struct for one or more sprite visual properties.  */
typedef_enum_begin(Sprite_properties_t)
  /* NONE = 0x00  */
  typedef_enum_hex(Sprite_properties_t, 00,                          \
                                               Sprite_props_none_mask)
  /* TODO: Comment, where to set colors for multicolor mode?
   *
   * Enables MULTICOLOR mode this sprite, using up to 3 colors, but
   * reduces the resolution in horizontal X direction by 2, i.e. from
   * 24 pxl/sprite to 12 pxl/sprite.
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

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Sprite_set_t)
  typedef_struct_uint8(                           pos_y)
  typedef_struct_uint9(                           pos_x)

  typedef_struct_enum(Graphix_color_t,            color)

  typedef_struct_enum(Sprite_properties_t,        props)
typedef_struct_end(Sprite_set_t)

/* Datatype of this structure.  */
typedef_struct_begin(Sprite_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Sprite_set_t,             set)

typedef_struct_end(Sprite_t)

/* ***************************************************************  */

header_endif(SPRITE)

#endif /* SPRITE_DEF_H__  */
