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


#include "common.h"

#include "Engine.h"
#include "Input.h"
#include "Graphix.h"

/* ***************************************************************  */

int
main(void)
{
  DEBUG_INIT();
  Engine_init();

  Input_enable(Input_joy_port2_mask);

  do {
    do {
      /* *************************************************************
       * polling stuff between engine ticks
       */

      if (Input_poll() & Input_joy_port2_mask) {
        DEBUG_NOTE("input break");
        break;
      }

      /*
       * *********************************************************  */
    } while (!Engine_tick_poll());

    /* ***************************************************************
     * time critical ticking stuff (input delay)
     */

    Graphix.buffer.scroll_x += Input.joy_port2.x_pace >> 5;
    Graphix.buffer.scroll_y += Input.joy_port2.y_pace >> 5;

    /*
     * ***********************************************************  */

    /* *** render, what we´ve done ***  */
    Graphix_buffer_swap();

    /* ***************************************************************
     * low priority ticking stuff
     */

    if (Engine.tick_count % ENGINE_MS2TICKS(500) == 0) {
      ++Graphix.buffer.bordercolor;
    }

    Input_tick();

    /*
     * ***********************************************************  */
  } while (!Input.joy_port2.button1_pressed);

  Engine_release();
  DEBUG_RELEASE_PRINT();

  return 0;
}
