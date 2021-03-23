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


#include "AASandbox.h"

#include "Graphix.h"
#include "Input.h"
#include "Engine.h"
#include "Pace.h"

/* ***************************************************************  */

static Pace_t AASandbox_pace_y, AASandbox_pace_x;

#ifdef DEBUG
static string_t AASandbox_charout;
static char* AASandbox_charout_last;
#endif /* DEBUG  */

/* ***************************************************************  */

void __fastcall__
AASandbox_init(void)
{
  unsigned i;

  /* set screen ram  */
  memset(Graphix.buffer.screen_ram,
         GRAPHIX_SCREENRAM_COLOR(GRAPHIX_GREEN, GRAPHIX_BLACK),
         GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(Graphix.buffer.bitmap_ram, 0x01, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8)
    Graphix.buffer.bitmap_ram[i] = 0xff; /* well optimized by CC65  */

  Pace_new(&AASandbox_pace_y, 12, 6, 14, 63);
  Pace_new(&AASandbox_pace_x, 3, 2, 32, 0);
  Pace_impulse_pos(&AASandbox_pace_y);

#ifdef DEBUG
  strcpy(AASandbox_charout, "PETSCII '");
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

  Pace_delete(&AASandbox_pace_x);
  Pace_delete(&AASandbox_pace_y);
}

/* ***************************************************************  */

void __fastcall__
AASandbox_poll(void)
{

}

void __fastcall__
AASandbox_tick(void)
{
  static uint8_t i;
  static bool key_w, key_s, key_a, key_d, key_space;

  if (Input.joy_port2.axis_y.changed) {
    if (Input.joy_port2.axis_y.direction > 0) {
      Pace_start_pos(&AASandbox_pace_y);
      DEBUG_NOTE("forward");
    }
    else if (Input.joy_port2.axis_y.direction < 0) {
      Pace_accelerate_neg(&AASandbox_pace_y);
      DEBUG_NOTE("backward");
    }
    else {
      Pace_brake(&AASandbox_pace_y);
      DEBUG_NOTE("stop");
    }

  } else if (Input.joy_port1.axis_y.changed) {
    if (Input.joy_port1.axis_y.direction > 0)
      Pace_start_pos(&AASandbox_pace_y);
    else if (Input.joy_port1.axis_y.direction < 0)
      Pace_accelerate_neg(&AASandbox_pace_y);
    else Pace_brake(&AASandbox_pace_y);
  }

  if (Input.joy_port2.axis_x.changed) {
    if (Input.joy_port2.axis_x.direction > 0)
      Pace_start_pos(&AASandbox_pace_x);
    else if (Input.joy_port2.axis_x.direction < 0)
      Pace_start_neg(&AASandbox_pace_x);
    else Pace_brake(&AASandbox_pace_x);

  } else if (Input.joy_port1.axis_x.changed) {
    if (Input.joy_port1.axis_x.direction > 0)
      Pace_start_pos(&AASandbox_pace_x);
    else if (Input.joy_port1.axis_x.direction < 0)
      Pace_start_neg(&AASandbox_pace_x);
    else Pace_brake(&AASandbox_pace_x);
  }

  if (Input.keyboard.changed) {
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
      && Input.keyboard.petscii.character != '\0'
      && AASandbox_charout_last - AASandbox_charout
      < STRING_BUFSIZE - 10) {
    *++AASandbox_charout_last = Input.keyboard.petscii.character;
  }

#endif /* DEBUG  */

  Pace_tick(&AASandbox_pace_y);
  Pace_tick(&AASandbox_pace_x);

  Graphix.buffer.scroll_y += AASandbox_pace_y.pace;
  Graphix.buffer.scroll_x += AASandbox_pace_x.pace;
}

void __fastcall__
AASandbox_tick_low(void)
{
  /* Just for testing here, but very unperformant code.  It calls the
   * CC65 runtime functions
   *
   *   * .tosumod0ax and .udiv32
   *
   * which are dividing a 32 bit wide unsigned integer.
   */
  if (Engine.tick_count % ENGINE_MS2TICKS(1000) == 0) {
    ++Graphix.buffer.bordercolor;
  }

  if (Input.joy_port2.button1.pressed
      || Input.joy_port1.button1.pressed) Engine.set.exit_code = 0;
}
