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


#ifndef SPRITE_H__
#define SPRITE_H__

#include "common.h"
#include "Sprite.gen.h"

/* ***************************************************************  */

/* Initialize empty sprite structure.  */
extern void __fastcall__ Sprite_new(Sprite_t* sprite);

/* Free the sprite structure.  */
#define Sprite_delete(sprite)
/* extern void __fastcall__ Sprite_delete(Sprite_t* sprite);  */

/* ***************************************************************  */

#endif /* SPRITE_H__  */
