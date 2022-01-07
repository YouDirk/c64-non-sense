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


#ifndef SPRITEANIMATION_DEF_H__
#define SPRITEANIMATION_DEF_H__

#include "define.h"

/* GRAPHIX.DEF.H is nesting this header file.  */
include_def_h(Graphix)

header_ifndef(SPRITEANIMATION)
header_define(SPRITEANIMATION)

include_def_h(Sprite)

/* ***************************************************************  */

/* Datatype of this structure.  */
typedef_struct_begin(SpriteAnimation_t)
  /* Number of frames in this animation.  */
  typedef_struct_uint8(                           frame_count)

  /* Sprite locator, pointing to the first frame buffer of this
   * animation.
   */
  typedef_struct_primit(Sprite_locator_t,         first_frame)

  /* Sprite pointer to the first frame buffer of this animation.  It´s
   * just a dereferenced version of SPRITEANIMATION_T::FIRST_FRAME
   * above.
   */
  typedef_struct_nested_ptr(Sprite_frame_t,       buffer)
typedef_struct_end(SpriteAnimation_t)

/* ***************************************************************  */

header_endif(SPRITEANIMATION)

#endif /* SPRITEANIMATION_DEF_H__  */
