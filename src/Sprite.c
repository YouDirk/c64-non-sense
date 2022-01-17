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

void __fastcall__
Sprite_tick_all(void)
{
  static Sprite_buffer_t* cur_sprite;
  static SpriteAnimation_t **cur_anim_ptr, *cur_animation;

  static Sprite_locator_t tmp_locator;
  static Sprite_frame_t* tmp_frame;
  static uint8_t tmp_count;

  DEBUG_RENDERTIME_IRQ_BEGIN(Graphix_lightgreen);

  for (cur_anim_ptr=Graphix.anims.sprites.set.sprite,
         cur_sprite=Graphix.buffer.sprites.sprite;
       cur_anim_ptr != &Graphix.anims.sprites.set.end;
       ++cur_anim_ptr, ++cur_sprite) {

    cur_animation = *cur_anim_ptr;
    if (cur_animation == NULL) continue;

    /* --- not NULL animation ---  */

    /* Animation already ticked?  Then just set Sprite Locator  */
    if (cur_animation->_stamp_lasttick == (uint8_t) Engine.tick_count) {
      tmp_locator = cur_animation->current_locator; /* just optim.  */
      cur_sprite->set.locator = tmp_locator;

      continue;
    }
    cur_animation->_stamp_lasttick = (uint8_t) Engine.tick_count;

    /* Just tick and set Sprite Locator?  */
    tmp_frame = cur_animation->current_frame; /* just optim.  */
    tmp_count = tmp_frame->tick_count;
    if (++cur_animation->current_tick <= tmp_count) {
      tmp_locator = cur_animation->current_locator; /* just optim.  */
      cur_sprite->set.locator = tmp_locator;

      continue;
    }

    /* --- new sprite frame in animation ---  */

    cur_animation->current_tick = 0;

    tmp_count = cur_animation->frame_count; /* just optim.  */
    tmp_frame = cur_animation->current_frame;
    if (++cur_animation->current_frame_no < tmp_count) {
      ++cur_animation->current_locator;
      cur_animation->current_frame = tmp_frame + 1;
      ++cur_sprite->set.locator;

      continue;
    }

    /* --- replay animation ---  */

    tmp_locator = cur_animation->locator; /* just optim.  */
    tmp_frame = cur_animation->buffer;

    cur_animation->current_frame_no = 0;
    cur_animation->current_locator = tmp_locator;
    cur_animation->current_frame = tmp_frame;
    cur_sprite->set.locator = tmp_locator;
  } /* for (cur_anim_ptr=Graphix.anims.; ...; ++cur_anim_ptr)  */

  DEBUG_RENDERTIME_IRQ_END();
}

/* ***************************************************************  */
