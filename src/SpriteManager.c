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
#include "SpriteAnimation.h"

/* ***************************************************************  */

void __fastcall__
SpriteManager_init(void)
{
  /* disable all sprites  */
  VIC.spr_ena = VIC_SPRITE_NONE_MASK;
  Graphix.buffer.sprites.set.enabled = SpriteManager_sprites_none_mask;
  Graphix.buffer.sprites.set.multicolor_0b01 = Graphix_black;
  Graphix.buffer.sprites.set.multicolor_0b11 = Graphix_black;

  /* Reset all SpriteAnimation pointers.  Just pointers in it, reset
   * them all to NULL.
   */
  memset(
    &Graphix.anims.sprites, NULL, sizeof(SpriteManager_animations_t));

  /* initialize each sprite (unrolled loop)  */
  Sprite_new(Graphix.buffer.sprites.sprite + 0);
  Sprite_new(Graphix.buffer.sprites.sprite + 1);
  Sprite_new(Graphix.buffer.sprites.sprite + 2);
  Sprite_new(Graphix.buffer.sprites.sprite + 3);
  Sprite_new(Graphix.buffer.sprites.sprite + 4);
  Sprite_new(Graphix.buffer.sprites.sprite + 5);
  Sprite_new(Graphix.buffer.sprites.sprite + 6);
  Sprite_new(Graphix.buffer.sprites.sprite + 7);
}

void __fastcall__
SpriteManager_release(void)
{
  /* disable all sprites  */
  VIC.spr_ena = VIC_SPRITE_NONE_MASK;
  VIC.spr_mcolor0 = VIC_SPR_MCOLOR0_0B01_DEFAULT;
  VIC.spr_mcolor1 = VIC_SPR_MCOLOR1_0B11_DEFAULT;

  /* free each sprite (unrolled loop)  */
  Sprite_delete(Graphix.buffer.sprites.sprite + 0);
  Sprite_delete(Graphix.buffer.sprites.sprite + 1);
  Sprite_delete(Graphix.buffer.sprites.sprite + 2);
  Sprite_delete(Graphix.buffer.sprites.sprite + 3);
  Sprite_delete(Graphix.buffer.sprites.sprite + 4);
  Sprite_delete(Graphix.buffer.sprites.sprite + 5);
  Sprite_delete(Graphix.buffer.sprites.sprite + 6);
  Sprite_delete(Graphix.buffer.sprites.sprite + 7);

  /* free each sprite animation (unrolled loop)  */
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[0]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[1]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[2]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[3]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[4]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[5]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[6]);
  SpriteAnimation_delete(Graphix.anims.sprites.sprite[7]);

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
  /* tick each sprite (unrolled loop)  */
  Sprite_tick(Graphix.buffer.sprites.sprite + 0);
  Sprite_tick(Graphix.buffer.sprites.sprite + 1);
  Sprite_tick(Graphix.buffer.sprites.sprite + 2);
  Sprite_tick(Graphix.buffer.sprites.sprite + 3);
  Sprite_tick(Graphix.buffer.sprites.sprite + 4);
  Sprite_tick(Graphix.buffer.sprites.sprite + 5);
  Sprite_tick(Graphix.buffer.sprites.sprite + 6);
  Sprite_tick(Graphix.buffer.sprites.sprite + 7);
}
