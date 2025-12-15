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


#include "Sprite.h"

#include "Engine.h"

/* ***************************************************************  */

void __fastcall__
Sprite_new_all(void)
{
  static Sprite_buffer_t* cur_sprite;

  for (cur_sprite=Graphix.buffer.sprites.sprite;
       cur_sprite != &Graphix.buffer.sprites.end; ++cur_sprite) {
    cur_sprite->set.pos_y = SPRITE_POS_SMALLSCREEN_BEGIN_Y;
    cur_sprite->set.pos_x = SPRITE_POS_SMALLSCREEN_BEGIN_X;

    cur_sprite->set.color = Graphix_white;
    cur_sprite->set.props = Sprite_props_none_mask;

    cur_sprite->set.locator
      = SPRITE_LOCATOR_FROMREF(GRAPHIX_BUFFER_SPRITERAM_RVAL);
  }
}

/* Nothing to do.  Just an empty-macro for now.
 */

/* void __fastcall__
 * Sprite_delete_all(void) {}
 */

/* ***************************************************************  */
