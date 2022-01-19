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

static SpriteAnimation_t AASandox_char_idle;
static Pace_t AASandox_char_pace_x, AASandox_char_pace_y;
static Pace_t AASandox_char_pace_jump;

static bool AASandbox_char_isjumping;

/* ***************************************************************  */

void __fastcall__
AASandbox_init(void)
{
  static uint16_t i;
  static Graphix_bitmapram_byte_t cur_cell;
  static uint8_t mod;

  /* set screen ram  */
  for (i=0, mod=0; i<1000; ++i, ++mod) {
    if (mod == 3) mod = 0;

    switch (mod) {
    case 0:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray3,
                                              Graphix_black);
      break;
    case 1:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray2,
                                              Graphix_black);
      break;
    case 2:
      GRAPHIX_BUFFER_SCREENRAM[0][i]
        = GRAPHIX_BUFFER_SCREENRAM_BYTELAYOUT(Graphix_gray1,
                                              Graphix_black);
      break;
    }
  }

  /* set bitmap  */
  for (i=0, mod=0, cur_cell=0xff; i<1000; ++i, ++mod) {
    if (mod == 40) {
      cur_cell = ~cur_cell;
      mod = 0;
    }

    memset(GRAPHIX_BUFFER_BITMAPRAM[0][i],
           i%2 == 0? cur_cell: ~cur_cell, GRAPHIX_SCREEN_CELLS_BYTES);
  }

  /* Load sprite animation into Sprite RAM  */
  SpriteAnimation_new(&AASandox_char_idle, AAAssets_sprite_anim_charidle,
                      AAASSETS_SPRITE_ANIM_CHARIDLE_COUNT);

  /* Attach and run sprite animation on hardware sprite 4  */
  Graphix.anims.sprites.set.sprite[4] = &AASandox_char_idle;

  /* Initialize character hardware sprite  */
  Graphix.buffer.sprites.sprite[4].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH/2
    - SPRITE_WIDTH/2;
  Graphix.buffer.sprites.sprite[4].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT/2
    - SPRITE_HEIGHT/2;
  AASandbox_char_isjumping = false;

  /* Show hardware sprite 4 on screen  */
  Graphix.buffer.sprites.set.enabled = SpriteManager_sprites_4_mask;

  /* Initialize paces for character sprite in x and y direction  */
  Pace_new(&AASandox_char_pace_x, 5, 2, 63, 0);
  Pace_new(&AASandox_char_pace_y, 5, 2, 63, 0);
  Pace_new(&AASandox_char_pace_jump, 11, 63, 63, 0);
}

void __fastcall__
AASandbox_release(void)
{
  /* All *_NEW() initializations are needing to be *_DELETE()  */
  Pace_delete(&AASandox_char_pace_jump);
  Pace_delete(&AASandox_char_pace_y);
  Pace_delete(&AASandox_char_pace_x);

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
  static bool key_exit;

  /* --- inputs ---  */

  if (Input.keyboard.changed
      && Input.joy_port1.axis_x.direction == 0
      && Input.joy_port1.axis_y.direction == 0
      ) {
    key_exit = false;
    for (i=0; i<Input.keyboard.pressed_count; ++i) {
      switch (Input.keyboard.pressed[i]) {
      case Input_sc_return_e: key_exit = true; break;
      case Input_sc_space_e: key_exit = true; break;
      default: break;
      }
    }

    /* --- results of inputs ---  */

    if (key_exit) Engine.set.exit_code = ENGINE_EXIT_SUCCESS;
  }

  if (Input.joy_port2.axis_y.changed) {
    switch (Input.joy_port2.axis_y.direction) {
    case 1:
      Pace_start_neg(&AASandox_char_pace_y);
      break;
    case -1:
      Pace_start_pos(&AASandox_char_pace_y);
      break;
    default:
      Pace_brake(&AASandox_char_pace_y);
      break;
    }
  }
  if (Input.joy_port2.axis_x.changed) {
    switch (Input.joy_port2.axis_x.direction) {
    case 1:
      Pace_start_neg(&AASandox_char_pace_x);
      break;
    case -1:
      Pace_start_pos(&AASandox_char_pace_x);
      break;
    default:
      Pace_brake(&AASandox_char_pace_x);
      break;
    }
  }
  if (Input.joy_port2.button1.changed) {
    if (Input.joy_port2.button1.pressed && !AASandbox_char_isjumping) {
      AASandbox_char_isjumping = true;
      Pace_brakerate_set(&AASandox_char_pace_jump, 32);
      Pace_impulse_neg(&AASandox_char_pace_jump);
    }
  }

  /* --- events ---  */

  if (AASandbox_char_isjumping) {
    if (Pace_is_stopped(&AASandox_char_pace_jump)) {
      Pace_brakerate_set(&AASandox_char_pace_jump, 63);
      Pace_accelerate_pos(&AASandox_char_pace_jump);
    } else if (Pace_is_maxpace_pos(&AASandox_char_pace_jump)) {
      Pace_brake(&AASandox_char_pace_jump);
      AASandbox_char_isjumping = false;
    }
  }

  /* --- ticking stuff ---  */

  Pace_tick(&AASandox_char_pace_x);
  Pace_tick(&AASandox_char_pace_y);
  Pace_tick(&AASandox_char_pace_jump);

  /* --- reaction ---  */

  Graphix.buffer.sprites.sprite[4].set.pos_x
    += AASandox_char_pace_x.pace;
  Graphix.buffer.sprites.sprite[4].set.pos_y
    += AASandox_char_pace_y.pace + AASandox_char_pace_jump.pace;
}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick_low(void)
{

}

/* ***************************************************************  */
