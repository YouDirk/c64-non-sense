/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
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


#include "Sandbox.h"

#include "Graphix.h"
#include "Input.h"
#include "Engine.h"
#include "Pace.h"

/* ***************************************************************  */

static Pace_t Sandbox_pace_y, Sandbox_pace_x;

/* ***************************************************************  */

void __fastcall__
Sandbox_init(void)
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

  Pace_new(&Sandbox_pace_y, 6, 2, 4, 0xff);
  Pace_new(&Sandbox_pace_x, 4, 2, 10, 0);
  Pace_impulse_pos(&Sandbox_pace_y);
}

void __fastcall__
Sandbox_release(void)
{
  Pace_delete(&Sandbox_pace_x);
  Pace_delete(&Sandbox_pace_y);
}

/* ***************************************************************  */

void __fastcall__
Sandbox_poll(void)
{

}

static Input_scancode_t prev_scancode = Input_sc_none_e;
void __fastcall__
Sandbox_tick(void)
{
  if (Input.joy_port2.axis_y.changed) {
    if (Input.joy_port2.axis_y.direction > 0) {
      Pace_start_pos(&Sandbox_pace_y);
      DEBUG_NOTE("forward");
    }
    else if (Input.joy_port2.axis_y.direction < 0) {
      Pace_accelerate_neg(&Sandbox_pace_y);
      DEBUG_NOTE("backward");
    }
    else {
      Pace_brake(&Sandbox_pace_y);
      DEBUG_NOTE("stop");
    }

  } else if (Input.joy_port1.axis_y.changed) {
    if (Input.joy_port1.axis_y.direction > 0)
      Pace_start_pos(&Sandbox_pace_y);
    else if (Input.joy_port1.axis_y.direction < 0)
      Pace_accelerate_neg(&Sandbox_pace_y);
    else Pace_brake(&Sandbox_pace_y);
  }

  // TODO: Input.keyboard.changed
  else if (prev_scancode != Input_sc_w_e
           && Input.keyboard.pressed[0] == Input_sc_w_e) {
    prev_scancode = Input_sc_w_e;
    Pace_start_pos(&Sandbox_pace_y);
  } else if (prev_scancode != Input_sc_none_e
             && Input.keyboard.pressed[0] == Input_sc_none_e) {
    prev_scancode = Input_sc_none_e;
    Pace_stop(&Sandbox_pace_y);
  }

  if (Input.joy_port2.axis_x.changed) {
    if (Input.joy_port2.axis_x.direction > 0)
      Pace_start_pos(&Sandbox_pace_x);
    else if (Input.joy_port2.axis_x.direction < 0)
      Pace_start_neg(&Sandbox_pace_x);
    else Pace_brake(&Sandbox_pace_x);

  } else if (Input.joy_port1.axis_x.changed) {
    if (Input.joy_port1.axis_x.direction > 0)
      Pace_start_pos(&Sandbox_pace_x);
    else if (Input.joy_port1.axis_x.direction < 0)
      Pace_start_neg(&Sandbox_pace_x);
    else Pace_brake(&Sandbox_pace_x);
  }

  Pace_tick(&Sandbox_pace_y);
  Pace_tick(&Sandbox_pace_x);

  Graphix.buffer.scroll_y += Sandbox_pace_y.pace;
  Graphix.buffer.scroll_x += Sandbox_pace_x.pace;
}

void __fastcall__
Sandbox_tick_low(void)
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
