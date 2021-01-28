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

#include "Input.h"
#include "Graphix.h"
#include "Engine.h"

#include "Pace.h"

Pace_t Sandbox_pace_init;

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

  Input_joy_config(Input_joy_port2_mask, Input_axes_y_mask, 2, 4, 0xf);
  Input_joy_config(Input_joy_port2_mask, Input_axes_x_mask, 1, 64, 0);

  Pace_new(&Sandbox_pace_init, 1<<2, 4, 0);
  Pace_impulse_pos(&Sandbox_pace_init);
}

void __fastcall__
Sandbox_release(void)
{
  Pace_delete(&init_pace);
}

/* ***************************************************************  */

void __fastcall__
Sandbox_poll(void)
{

}

void __fastcall__
Sandbox_tick(void)
{
  Pace_tick(&Sandbox_pace_init);

  Graphix.buffer.scroll_x += Sandbox_pace_init.pace;

  Graphix.buffer.scroll_y
    += Input.joy_port2.y_pace + Input.joy_port1.y_pace;
  Graphix.buffer.scroll_x
    += Input.joy_port2.x_pace + Input.joy_port1.x_pace;
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
  if (Engine.tick_count % ENGINE_MS2TICKS(1000) == 0)
    ++Graphix.buffer.bordercolor;

  if (Input.joy_port2.button1_pressed
      || Input.joy_port1.button1_pressed) Engine.set.exit_code = 0;
}
