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

static SpriteAnimation_t AASandox_chartop_idle, AASandox_charbot_idle;
static SpriteAnimation_t AASandox_chartop_runright,
                         AASandox_charbot_runright;
static SpriteAnimation_t AASandox_chartop_runleft,
                         AASandox_charbot_runleft;

static Pace_t AASandox_char_pace_x, AASandox_char_pace_y;
static Pace_t AASandox_char_pace_jump;

static int8_t AASandbox_char_jump_pxl;
static bool AASandbox_char_is_moving, AASandbox_char_mark_stop_x;

/* ***************************************************************  */

static void
_AASandbox_spriteanim_flipped_x(
                SpriteAnimation_t* dest, const SpriteAnimation_t* src,
                bool is_multicolor)
{
  static uint8_t *src_byte, *dest_byte;
  static uint8_t mod, tmp;
  static int8_t byte_swap;
  static bool is_multic;

  static Sprite_frame_t* src_buffer;

  is_multic = is_multicolor;

  src_buffer = src->buffer;
  for (src_byte = (uint8_t*) src_buffer,
         dest_byte = (uint8_t*) dest->buffer, mod=0;
       src_byte < (2 << 6) + (uint8_t*) src_buffer;
       ++src_byte, ++dest_byte, ++mod) {

    if (((uint8_t) src_byte & 0x3f) == 0x3f) {
      *dest_byte = *src_byte;

      mod = -1;
      continue;
    }

    if (mod == 3) mod = 0;
    switch (mod) {
    case 0: byte_swap = 2;  break;
    case 1: byte_swap = 0;  break;
    case 2: byte_swap = -2; break;
    }

    __asm__ ("lda %v\n"
             "sta ptr4\n"
             "lda %v + 1\n"
             "sta ptr4 + 1\n"
             "ldy #$00\n"
             "lda (ptr4), y\n", src_byte, src_byte);

    /* A = SRC_BYTE  */

    __asm__ ("ldx #4\n"
             "ldy %v\n"
             "bne %g\n"
             "ldx #8\n", is_multic, loop_multic);
  loop_highres:
    __asm__ ("beq %g\n"
             "lsr\n"
             "rol %v\n"
             "dex\n"
             "jmp %g\n", loop_end, tmp, loop_highres);
  loop_multic:
    __asm__ ("beq %g\n"
             "lsr\n"
             "ror\n"
             "rol %v\n"
             "asl\n"
             "rol %v\n"
             "lsr\n"
             "dex\n"
             "jmp %g\n", loop_end, tmp, tmp, loop_multic);
  loop_end:

    /* TMP := DEST data  */

    __asm__ ("ldx #$00\n"
             "lda %v\n"
             "bpl add_ready\n"
             "dex\n"
             "add_ready:\n"
             "clc\n"
             "adc %v\n"
             "sta ptr4\n"
             "txa\n"
             "adc %v + 1\n"
             "sta ptr4 + 1\n"

             "lda %v\n"
             "ldy #$00\n"
             "sta (ptr4), y\n",
             byte_swap, dest_byte, dest_byte, tmp);
  }
}

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
  SpriteAnimation_new(
          &AASandox_chartop_idle, AAAssets_anim_chartop_idle,
          AAASSETS_ANIM_CHARTOP_IDLE_COUNT);
  SpriteAnimation_new(
          &AASandox_charbot_idle, AAAssets_anim_charbot_idle,
          AAASSETS_ANIM_CHARBOT_IDLE_COUNT);
  SpriteAnimation_new(
          &AASandox_chartop_runright, AAAssets_anim_chartop_runright,
          AAASSETS_ANIM_CHARTOP_RUNRIGHT_COUNT);
  SpriteAnimation_new(
          &AASandox_charbot_runright, AAAssets_anim_charbot_runright,
          AAASSETS_ANIM_CHARBOT_RUNRIGHT_COUNT);
  SpriteAnimation_new_alloc(
          &AASandox_chartop_runleft,
          AAASSETS_ANIM_CHARTOP_RUNRIGHT_COUNT);
  SpriteAnimation_new_alloc(
          &AASandox_charbot_runleft,
          AAASSETS_ANIM_CHARBOT_RUNRIGHT_COUNT);
  _AASandbox_spriteanim_flipped_x(
         &AASandox_chartop_runleft, &AASandox_chartop_runright, true);
  _AASandbox_spriteanim_flipped_x(
         &AASandox_charbot_runleft, &AASandox_charbot_runright, true);

  /* Attach and run sprite animation on hardware sprite 4  */
  Graphix.anims.sprites.set.sprite[4] = &AASandox_chartop_idle;
  Graphix.anims.sprites.set.sprite[5] = &AASandox_charbot_idle;

  /* Initialize character hardware sprite  */
  Graphix.buffer.sprites.sprite[4].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH/2
    - SPRITE_WIDTH/2;
  Graphix.buffer.sprites.sprite[4].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT/2
    - SPRITE_HEIGHT;
  Graphix.buffer.sprites.sprite[4].set.color = Graphix_green;
  Graphix.buffer.sprites.sprite[4].set.props
    = Sprite_props_multicolor_mask;
  Graphix.buffer.sprites.sprite[5].set.pos_x
    = SPRITE_POS_SMALLSCREEN_BEGIN_X + SPRITE_POS_SMALLSCREEN_WIDTH/2
    - SPRITE_WIDTH/2;
  Graphix.buffer.sprites.sprite[5].set.pos_y
    = SPRITE_POS_SMALLSCREEN_BEGIN_Y + SPRITE_POS_SMALLSCREEN_HEIGHT/2;
  Graphix.buffer.sprites.sprite[5].set.color = Graphix_yellow;
  Graphix.buffer.sprites.sprite[5].set.props
    = Sprite_props_multicolor_mask;
  AASandbox_char_jump_pxl = 0;
  AASandbox_char_is_moving = false;
  AASandbox_char_mark_stop_x = false;

  /* Show hardware sprite 4 on screen  */
  Graphix.buffer.sprites.set.multicolor_0b01 = Graphix_blue;
  Graphix.buffer.sprites.set.multicolor_0b11 = Graphix_red;
  Graphix.buffer.sprites.set.enabled
    = SpriteManager_sprites_4_mask | SpriteManager_sprites_5_mask;

  /* Initialize paces for character sprite in x and y direction  */
  Pace_new(&AASandox_char_pace_x, 8, 2, 32, 0);
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
      Graphix.anims.sprites.set.sprite[4] = &AASandox_chartop_runleft;
      Graphix.anims.sprites.set.sprite[5] = &AASandox_charbot_runleft;
      AASandbox_char_is_moving = true;
      break;
    case -1:
      Pace_start_pos(&AASandox_char_pace_x);
      Graphix.anims.sprites.set.sprite[4] = &AASandox_chartop_runright;
      Graphix.anims.sprites.set.sprite[5] = &AASandox_charbot_runright;
      AASandbox_char_is_moving = true;
      break;
    default:
      AASandbox_char_mark_stop_x = true;
      break;
    }
  }

  if (Input.joy_port2.button1.changed) {
    if (Input.joy_port2.button1.pressed) {
      Pace_brakerate_set(&AASandox_char_pace_jump, 31);
      Pace_impulse_neg(&AASandox_char_pace_jump);
    }
  } else if (AASandbox_char_jump_pxl < 0) {
    if (Pace_is_stopped(&AASandox_char_pace_jump)) {
      Pace_brakerate_set(&AASandox_char_pace_jump, 63);
      Pace_accelerate_pos(&AASandox_char_pace_jump);
    }
  } else if (AASandbox_char_jump_pxl >= 0
             && !Pace_is_stopped(&AASandox_char_pace_jump)) {
    AASandbox_char_jump_pxl = 0;
    Pace_stop(&AASandox_char_pace_jump);
  }

  if (AASandbox_char_mark_stop_x
      && AASandbox_char_jump_pxl == 0) {
    Pace_brake(&AASandox_char_pace_x);
    AASandbox_char_mark_stop_x = false;
  }

  /* --- ticking stuff ---  */

  Pace_tick(&AASandox_char_pace_x);
  Pace_tick(&AASandox_char_pace_y);
  Pace_tick(&AASandox_char_pace_jump);

  /* --- reaction ---  */

  if (AASandbox_char_is_moving
      && AASandbox_char_jump_pxl == 0
      && Pace_is_stopped(&AASandox_char_pace_x)) {
    Graphix.anims.sprites.set.sprite[4] = &AASandox_chartop_idle;
    Graphix.anims.sprites.set.sprite[5] = &AASandox_charbot_idle;
    AASandbox_char_is_moving = false;
  }

  AASandbox_char_jump_pxl += AASandox_char_pace_jump.pace;

  Graphix.buffer.sprites.sprite[4].set.pos_x
    += AASandox_char_pace_x.pace;
  Graphix.buffer.sprites.sprite[4].set.pos_y
    += AASandox_char_pace_y.pace + AASandox_char_pace_jump.pace;
  Graphix.buffer.sprites.sprite[5].set.pos_x
    += AASandox_char_pace_x.pace;
  Graphix.buffer.sprites.sprite[5].set.pos_y
    += AASandox_char_pace_y.pace + AASandox_char_pace_jump.pace;
}

/* ***************************************************************  */

void __fastcall__
AASandbox_tick_low(void)
{

}

/* ***************************************************************  */
