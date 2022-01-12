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


#include "AASandbox.h"

#include "AAAssets.h"

#include "Engine.h"
#include "Input.h"
#include "Pace.h"

#include "Graphix.h"
#include "SpriteAnimation.h"

/* ***************************************************************  */

#define _CONST_SPRITES                                               \
  SpriteManager_sprites_5_mask

#define _BLINKING_SPRITES                                            \
  (SpriteManager_sprites_4_mask | SpriteManager_sprites_0_mask       \
   | SpriteManager_sprites_6_mask | SpriteManager_sprites_7_mask)

/* ***************************************************************  */

static uint8_t AASandbox_bordercolor_time;

static Pace_t AASandbox_pace_y, AASandbox_pace_x;
static Pace_t AASandbox_pace_sprite_y, AASandbox_pace_sprite_x;

static SpriteAnimation_t AASandbox_anim_blank, AASandbox_anim_multic;
static SpriteAnimation_t AASandbox_anim_default;

static SpriteAnimation_t AASandbox_anim_test1, AASandbox_anim_test2;
static SpriteAnimation_t AASandbox_anim_test3, AASandbox_anim_test4;
static SpriteAnimation_t AASandbox_anim_test5, AASandbox_anim_test6;

#ifdef DEBUG
static string_t AASandbox_charout;
static char* AASandbox_charout_last;
#endif /* DEBUG  */

/* ***************************************************************  */

void __fastcall__
AASandbox_init(void)
{
  static uint16_t i;

  AASandbox_bordercolor_time = 0;

  /* set screen ram  */
  memset(GRAPHIX_BUFFER_SCREENRAM,
    GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_green, Graphix_black),
    GRAPHIX_BUFFER_SCREENRAM_BUFSIZE);

  /* set bitmap  */
  memset(GRAPHIX_BUFFER_BITMAPRAM, 0x01,
                                   GRAPHIX_BUFFER_BITMAPRAM_BUFSIZE);
  for (i=0; i<GRAPHIX_SCREEN_BYTES; i+=8)
    GRAPHIX_BUFFER_BITMAPRAM[0][0][i] = 0xff;

  GRAPHIX_BUFFER_SCREENRAM[6][7]
               = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_green,
                                                     Graphix_gray1);
  GRAPHIX_BUFFER_BITMAPRAM[6][7][4] = 0x19;

  /* allocate sprite animations  */
  SpriteAnimation_new_alloc(&AASandbox_anim_blank, 1);
  memset(AASandbox_anim_blank.buffer,
                                   0xff, SPRITE_FRAME_BUFFER_BUFSIZE);
  AASandbox_anim_blank.buffer[0].tick_count
                                   = SPRITE_FRAME_TICKCOUNT_LAST_MASK;

  SpriteAnimation_new_alloc(&AASandbox_anim_multic, 1);
  memset(AASandbox_anim_multic.buffer,
                                   0xe4, SPRITE_FRAME_BUFFER_BUFSIZE);
  AASandbox_anim_multic.buffer[0].tick_count
                                   = SPRITE_FRAME_TICKCOUNT_LAST_MASK;

  SpriteAnimation_new(
                    &AASandbox_anim_default, AAAssets_spranim_moving,
                    AAASSETS_SPRANIM_MOVING_COUNT);

  /* *** TODO: alloc() & free() tests here ***  */
  SpriteAnimation_new_alloc(&AASandbox_anim_test1, 2);
  SpriteAnimation_new_alloc(&AASandbox_anim_test2, 2);
  SpriteAnimation_new_alloc(&AASandbox_anim_test3, 2);
  SpriteAnimation_new_alloc(&AASandbox_anim_test4, 2);
  SpriteAnimation_new_alloc(&AASandbox_anim_test5, 2);
  SpriteAnimation_new_alloc(&AASandbox_anim_test6, 2);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test1);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test5);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test3);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test2);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_new_alloc(&AASandbox_anim_test1, 2);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test4);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  SpriteAnimation_delete(&AASandbox_anim_test6);
  SPRITEANIMATION_DEBUG_ALLOC_PRINT();
  /* *** TODO: end of alloc() & free() tests ***  */

  /* set sprite multi-color colors  */
  Graphix.buffer.sprites.set.multicolor_0b01 = Graphix_blue;
  Graphix.buffer.sprites.set.multicolor_0b11 = Graphix_orange;

  /* top, left  */
  Graphix.buffer.sprites.sprite[4].locator
    = AASandbox_anim_multic.first_frame;
  Graphix.buffer.sprites.sprite[4].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + 1;
  Graphix.buffer.sprites.sprite[4].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + 1;
  Graphix.buffer.sprites.sprite[4].set.color = Graphix_cyan;
  Graphix.buffer.sprites.sprite[4].set.props
    = Sprite_props_multicolor_mask | Sprite_props_scale_y_mask
    | Sprite_props_scale_x_mask;

  /* top, right  */
  Graphix.buffer.sprites.sprite[0].locator
    = AASandbox_anim_blank.first_frame;
  Graphix.buffer.sprites.sprite[0].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + 1;
  Graphix.buffer.sprites.sprite[0].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH
    - SPRITE_WIDTH - 1;
  Graphix.buffer.sprites.sprite[0].set.color = Graphix_lightred;
  Graphix.buffer.sprites.sprite[0].set.props
    = Sprite_props_scale_y_mask;

  /* bottom, left  */
  Graphix.buffer.sprites.sprite[6].locator
    = AASandbox_anim_blank.first_frame;
  Graphix.buffer.sprites.sprite[6].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT
    - SPRITE_HEIGHT - 1;
  Graphix.buffer.sprites.sprite[6].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + 1;
  Graphix.buffer.sprites.sprite[6].set.color = Graphix_yellow;
  Graphix.buffer.sprites.sprite[6].set.props
    = Sprite_props_scale_x_mask;

  /* bottom, right  */
  Graphix.buffer.sprites.sprite[7].locator
    = AASandbox_anim_blank.first_frame;
  Graphix.buffer.sprites.sprite[7].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT
    - SPRITE_HEIGHT - 1;
  Graphix.buffer.sprites.sprite[7].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH
    - SPRITE_WIDTH - 1;
  Graphix.buffer.sprites.sprite[7].set.color = Graphix_blue;
  Graphix.buffer.sprites.sprite[7].set.props
    = Sprite_props_prio_bground_mask;

  /* moving  */
  Graphix.buffer.sprites.sprite[5].locator
    = AASandbox_anim_default.first_frame;
  Graphix.buffer.sprites.sprite[5].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT
    - SPRITE_HEIGHT;
  Graphix.buffer.sprites.sprite[5].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH/2
    - SPRITE_WIDTH/2;

  Graphix.buffer.sprites.set.enabled
    = _CONST_SPRITES | _BLINKING_SPRITES;

  Pace_new(&AASandbox_pace_y, 12, 6, 14, 63);
  Pace_new(&AASandbox_pace_x, 3, 2, 32, 0);
  Pace_new(&AASandbox_pace_sprite_y, 14, 8, 13, 63);
  Pace_new(&AASandbox_pace_sprite_x, 5, 4, 31, 0);
  Pace_impulse_pos(&AASandbox_pace_y);
  Pace_impulse_neg(&AASandbox_pace_sprite_y);

#ifdef DEBUG
  strcpy(AASandbox_charout, "petscii '");
  AASandbox_charout_last = AASandbox_charout + 8;
#endif /* DEBUG  */
}

void __fastcall__
AASandbox_release(void)
{
#ifdef DEBUG
  strcpy(++AASandbox_charout_last, "'");
  DEBUG_NOTE(AASandbox_charout);
#endif /* DEBUG  */

  Pace_delete(&AASandbox_pace_sprite_x);
  Pace_delete(&AASandbox_pace_sprite_y);
  Pace_delete(&AASandbox_pace_x);
  Pace_delete(&AASandbox_pace_y);

  SpriteAnimation_delete_all();
}

/* ***************************************************************  */

void __fastcall__
AASandbox_poll(void)
{

}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick(void)
{
  static uint8_t i;
  static bool key_w, key_s, key_a, key_d, key_space;

  if (Input.joy_port2.axis_y.changed) {
    if (Input.joy_port2.axis_y.direction > 0) {
      Pace_start_pos(&AASandbox_pace_y);
      Pace_start_pos(&AASandbox_pace_sprite_y);
    }
    else if (Input.joy_port2.axis_y.direction < 0) {
      Pace_accelerate_neg(&AASandbox_pace_y);
      Pace_accelerate_neg(&AASandbox_pace_sprite_y);
    }
    else {
      Pace_brake(&AASandbox_pace_y);
      Pace_brake(&AASandbox_pace_sprite_y);
    }

  } else if (Input.joy_port1.axis_y.changed) {
    if (Input.joy_port1.axis_y.direction > 0)
      Pace_start_pos(&AASandbox_pace_y);
    else if (Input.joy_port1.axis_y.direction < 0)
      Pace_accelerate_neg(&AASandbox_pace_y);
    else Pace_brake(&AASandbox_pace_y);
  }

  if (Input.joy_port2.axis_x.changed) {
    if (Input.joy_port2.axis_x.direction > 0) {
      Pace_start_pos(&AASandbox_pace_x);
      Pace_start_pos(&AASandbox_pace_sprite_x);
    } else if (Input.joy_port2.axis_x.direction < 0) {
      Pace_start_neg(&AASandbox_pace_x);
      Pace_start_neg(&AASandbox_pace_sprite_x);
    } else {
      Pace_brake(&AASandbox_pace_x);
      Pace_brake(&AASandbox_pace_sprite_x);
    }

  } else if (Input.joy_port1.axis_x.changed) {
    if (Input.joy_port1.axis_x.direction > 0)
      Pace_start_pos(&AASandbox_pace_x);
    else if (Input.joy_port1.axis_x.direction < 0)
      Pace_start_neg(&AASandbox_pace_x);
    else Pace_brake(&AASandbox_pace_x);
  }

  if (Input.keyboard.changed
      && Input.joy_port1.axis_x.direction == 0
      && Input.joy_port1.axis_y.direction == 0) {
    key_w=false, key_s=false, key_a=false, key_d=false, key_space=false;

    for (i=0; i<Input.keyboard.pressed_count; ++i) {
      switch (Input.keyboard.pressed[i]) {
      case Input_sc_w_e: key_w = true; break;
      case Input_sc_s_e: key_s = true; break;
      case Input_sc_a_e: key_a = true; break;
      case Input_sc_d_e: key_d = true; break;
      case Input_sc_space_e: key_space = true; break;
      default: break;
      }
    }

    if (key_space) {
      Pace_brakerate_set(&AASandbox_pace_y, 50);
      Pace_brake(&AASandbox_pace_y);
    }
    else if (key_w) Pace_start_pos(&AASandbox_pace_y);
    else if (key_s) Pace_accelerate_neg(&AASandbox_pace_y);
    else {
      Pace_brakerate_set(&AASandbox_pace_y, 14);
      Pace_brake(&AASandbox_pace_y);
    }

    if (key_a) Pace_start_pos(&AASandbox_pace_x);
    else if (key_d) Pace_start_neg(&AASandbox_pace_x);
    else Pace_brake(&AASandbox_pace_x);
  }

#ifdef DEBUG

  if (Input.keyboard.petscii.changed
      && Input.joy_port1.axis_x.direction == 0
      && Input.joy_port1.axis_y.direction == 0
      && Input.keyboard.petscii.character != '\0'
      && AASandbox_charout_last - AASandbox_charout
      < STRING_BUFSIZE - 10) {
    *++AASandbox_charout_last = Input.keyboard.petscii.character;
  }

#endif /* DEBUG  */

  Pace_tick(&AASandbox_pace_y);
  Pace_tick(&AASandbox_pace_x);
  Pace_tick(&AASandbox_pace_sprite_y);
  Pace_tick(&AASandbox_pace_sprite_x);

  Graphix.buffer.set.scroll_y += AASandbox_pace_y.pace;
  Graphix.buffer.set.scroll_x += AASandbox_pace_x.pace;
  Graphix.buffer
    .sprites.sprite[5].set.pos_y += AASandbox_pace_sprite_y.pace;
  Graphix.buffer
    .sprites.sprite[5].set.pos_x += AASandbox_pace_sprite_x.pace;
}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick_low(void)
{
  if (++AASandbox_bordercolor_time >= ENGINE_MS2TICKS(1000)) {
    AASandbox_bordercolor_time = 0;

    ++Graphix.buffer.set.bordercolor;

    if ((--Graphix.buffer.sprites.sprite[5].set.color & UINT4_MAX)
        == Graphix_black) {
      --Graphix.buffer.sprites.sprite[5].set.color;
    }

    Graphix.buffer.sprites.sprite[5].set.props
      = Graphix.buffer.sprites.sprite[5].set.props
        & Sprite_props_prio_bground_mask
      ? Graphix.buffer.sprites.sprite[5].set.props
        & ~Sprite_props_prio_bground_mask
      : Graphix.buffer.sprites.sprite[5].set.props
        | Sprite_props_prio_bground_mask;

    Graphix.buffer.sprites.set.enabled
      = Graphix.buffer.sprites.set.enabled & _BLINKING_SPRITES
      ? _CONST_SPRITES
      : (_CONST_SPRITES | _BLINKING_SPRITES);
  }

  if (Input.joy_port2.button1.pressed || Input.joy_port1.button1.pressed)
    Engine.set.exit_code = ENGINE_EXIT_SUCCESS;
}

/* ***************************************************************  */
