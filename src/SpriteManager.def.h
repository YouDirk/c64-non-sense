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


#ifndef SPRITEMANAGER_DEF_H__
#define SPRITEMANAGER_DEF_H__

#include "define.h"

/* GRAPHIX.DEF.H is nesting this header file.  */
include_def_h(Graphix)

header_ifndef(SPRITEMANAGER)
header_define(SPRITEMANAGER)

include_def_h(chip-vic)

/* SPRITE.DEF.H requires GRAPHIX_COLOR_T.  */
include_def_h(Sprite)
include_def_h(SpriteAnimation)

/* ***************************************************************  */

macro_arg1(SPRITEMANAGER_SPRITES_2MASK, uint8_t,  1 << (arg1))

define_dec(SPRITEMANAGER_SPRITES_COUNT,           8)

/* Selector for sprites to be used.  Full compatible with bit masks of
 * VIC-II sprites registers, i.e. enable register at 0xd015, etc.
 */
typedef_enum_begin(SpriteManager_sprites_t)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_NONE_MASK,    \
                                      SpriteManager_sprites_none_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_0_MASK,       \
                                         SpriteManager_sprites_0_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_1_MASK,       \
                                         SpriteManager_sprites_1_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_2_MASK,       \
                                         SpriteManager_sprites_2_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_3_MASK,       \
                                         SpriteManager_sprites_3_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_4_MASK,       \
                                         SpriteManager_sprites_4_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_5_MASK,       \
                                         SpriteManager_sprites_5_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_6_MASK,       \
                                         SpriteManager_sprites_6_mask)
  typedef_enum_val(SpriteManager_sprites_t, VIC_SPRITE_7_MASK,       \
                                         SpriteManager_sprites_7_mask)
typedef_enum_end(SpriteManager_sprites_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(SpriteManager_buffer_set_t)
  /* Sprites which will be enabled/visible.  Full compatible with
   * VIC-II register at 0xd015.
   */
  typedef_struct_enum(SpriteManager_sprites_t,         enabled)

  /* Colors with bit combinations 0b01=0x01 and 0b11=0x03 for all
   * sprites which have GRAPHIX.BUFFER.SPRITES.SPRITE[i].SET.PROPS =
   * SPRITE_PROPS_MULTICOLOR_MASK set.
   */
  typedef_struct_enum(Graphix_color_t,                 multicolor_0b01)
  typedef_struct_enum(Graphix_color_t,                 multicolor_0b11)
typedef_struct_end(SpriteManager_buffer_set_t)

/* ---------------------------------------------------------------  */

/* Sprites structure in graphic buffer.  */
typedef_struct_begin(SpriteManager_buffer_t)
  /* Some writable member variables.  */
  typedef_struct_nested(SpriteManager_buffer_set_t,         set)

  /* Access to the 8 hardware sprites of the C64 VIC-II graphic chip.
   *
   * The index of this array (0..7) is representing the Z-order of the
   * displayed sprites.  SPRITE[0] is on top of all others, SPRITE[7]
   * is behind all other sprites.  Additionally, sprites are behind
   * the background if SPRITE_PROPS_PRIO_BGROUND_MASK of it was set in
   * SPRITE_BUFFER_T::SET.PROPS, otherwise on top of background.
   */
  typedef_struct_nested_array(Sprite_buffer_t,              sprite,  \
                              SPRITEMANAGER_SPRITES_COUNT)
  typedef_struct_nested(Sprite_buffer_t,                    end)
typedef_struct_end(SpriteManager_buffer_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(SpriteManager_animations_set_t)
  /* Enables the ticking of Sprite Animations.  To improve performance
   * set this to FALSE if no sprite animations are used.
   *
   * If set to FALSE then the GRAPHIX.ANIMS.SPRITES.SET.SPRITE[x]
   * usage has no effects anymore.  Instead set
   *
   *   GRAPHIX.BUFFER.SPRITES.SPRITE[x].SET.LOCATOR
   *                           = SPRITEANIMATION_T::LOCATOR + frame_no
   *
   * to make a static Sprite Frame visible.
   */
  typedef_struct_bool(                                     is_ticking)

  /* Attach your Sprite Animation to 1 of the 8 hardware sprite here,
   * by setting the corresponding SPRITE[x] pointer.
   *
   * During playing the animation the corresponding
   *
   *   GRAPHIX.BUFFER.SPRITES.SPRITE[x].SET.LOCATOR
   *
   * will be incremented/adapted.  Therefore make sure that there is
   * currently no static sprite image in use for this sprite.
   *
   * The index of this array (0..7) is representing the Z-order of the
   * displayed sprites.  SPRITE[0] is on top of all others, SPRITE[7]
   * is behind all other sprites.  Additionally, sprites are behind
   * the background if SPRITE_PROPS_PRIO_BGROUND_MASK of it was set in
   * SPRITE_BUFFER_T::SET.PROPS, otherwise on top of background.
   */
  typedef_struct_nested_array_ofptr(SpriteAnimation_t,      sprite,  \
                                    SPRITEMANAGER_SPRITES_COUNT)
  typedef_struct_nested_ptr(SpriteAnimation_t,              end)
typedef_struct_end(SpriteManager_animations_set_t)

/* ---------------------------------------------------------------  */

/* Allocated sprite animations in Graphix structure.  */
typedef_struct_begin(SpriteManager_animations_t)
  /* Some writable member variables.  */
  typedef_struct_nested(SpriteManager_animations_set_t,     set)
typedef_struct_end(SpriteManager_animations_t)

/* ***************************************************************  */

header_endif(SPRITEMANAGER)

#endif /* SPRITEMANAGER_DEF_H__  */
