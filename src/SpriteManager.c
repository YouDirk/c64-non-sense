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


#include "SpriteManager.h"

#include "Sprite.h"

/* ***************************************************************  */

void __fastcall__
SpriteManager_init(void)
{
  static Sprite_t* cur_sprite;
  static uint8_t i;

  /* disable all sprites  */
  VIC.spr_ena = VIC_SPRITE_NONE_MASK;
  Graphix.buffer.sprites.set.enabled = SpriteManager_sprites_none_mask;
  Graphix.buffer.sprites.set.multicolor_0b01 = Graphix_black;
  Graphix.buffer.sprites.set.multicolor_0b11 = Graphix_black;

  // --- TODO ---
  memset(&GRAPHIX_BUFFER_SPRITERAM, 0xff, 3*21);
  for (i=0; i<8; ++i) {
    *((uint8_t*) GRAPHIX_BUFFER_SCREENRAM + 1016 + i)
      = (((unsigned) &GRAPHIX_BUFFER_SPRITERAM - _GRAPHIX_VICBANK_RVAL) >> 6) + 0;
  }
  memset(&GRAPHIX_BUFFER_SPRITERAM + 0x0040, 0xe4, 3*21);
  *((uint8_t*) GRAPHIX_BUFFER_SCREENRAM + 1016 + 4)
    = (((unsigned) &GRAPHIX_BUFFER_SPRITERAM - _GRAPHIX_VICBANK_RVAL) >> 6) + 1;
  // --- end of TODO ---

  for (cur_sprite = Graphix.buffer.sprites.sprite;
       cur_sprite < &Graphix.buffer.sprites.end; ++cur_sprite)
    Sprite_new(cur_sprite);
}

void __fastcall__
SpriteManager_release(void)
{
  static Sprite_t* cur_sprite;

  /* disable all sprites  */
  VIC.spr_ena = VIC_SPRITE_NONE_MASK;
  VIC.spr_mcolor0 = VIC_SPR_MCOLOR0_0B01_DEFAULT;
  VIC.spr_mcolor1 = VIC_SPR_MCOLOR1_0B11_DEFAULT;

  for (cur_sprite = Graphix.buffer.sprites.sprite;
       cur_sprite < &Graphix.buffer.sprites.end; ++cur_sprite)
    Sprite_delete(cur_sprite);

  /* reset sprites stuff  */
  memset(&VIC_SPR_POSITIONS, 0x00, VIC_SPR_POSITIONS_BUFSIZE);
  VIC.spr_hi_x    = 0x00;
  VIC.spr_mcolor  = 0x00;
  VIC.spr_exp_y   = 0x00;
  VIC.spr_exp_x   = 0x00;
  VIC.spr_bg_prio = 0x00;

  VIC.spr0_color = VIC_SPR0_COLOR_DEFAULT;
  VIC.spr1_color = VIC_SPR1_COLOR_DEFAULT;
  VIC.spr2_color = VIC_SPR2_COLOR_DEFAULT;
  VIC.spr3_color = VIC_SPR3_COLOR_DEFAULT;
  VIC.spr4_color = VIC_SPR4_COLOR_DEFAULT;
  VIC.spr5_color = VIC_SPR5_COLOR_DEFAULT;
  VIC.spr6_color = VIC_SPR6_COLOR_DEFAULT;
  VIC.spr7_color = VIC_SPR7_COLOR_DEFAULT;
}

/* ***************************************************************  */

void __fastcall__
SpriteManager_tick(void)
{
  //DEBUG_NOTE("spritemanager_tick()");
}
