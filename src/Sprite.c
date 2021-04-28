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


#include "Sprite.h"

/* ***************************************************************  */

void __fastcall__
Sprite_new(Sprite_t* sprite)
{
  sprite->set.pos_y = SPRITE_POSY_SCREEN_BEGIN;
  sprite->set.pos_x = SPRITE_POSX_SCREEN_BEGIN;

  sprite->set.color = Graphix_white;

  sprite->set.props = Sprite_props_none_mask;
}

/* Nothing to do.  Just an empty-macro for now.
 */

/* void __fastcall__ Sprite_delete(Sprite_t* sprite) {}  */

/* ***************************************************************  */
