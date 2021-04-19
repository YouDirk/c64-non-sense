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

header_ifndef(SPRITE)
header_define(SPRITE)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Sprite_set_t)
  typedef_struct_bool(dummy)
typedef_struct_end(Sprite_set_t)

/* Datatype of this structure.  */
typedef_struct_begin(Sprite_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Sprite_set_t,                   set)

typedef_struct_end(Sprite_t)

/* ***************************************************************  */

header_endif(SPRITE)

#endif /* INPUT_DEF_H__  */
