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

  do {
    do {
      /* polling stuff between Engine ticks  */

      if (Input_poll(Input_joy1_mask)) break;

    } while (!Engine_tick_poll());

    /* ticking stuff  */

    if (Engine.tick_count % ENGINE_MS2TICKS(500) == 0) {
      ++Graphix.buffer.bordercolor;
    }

    Graphix.buffer.scroll_x += Input.joy1_pace_x;
    Graphix.buffer.scroll_y += Input.joy1_pace_y;

    Graphix_buffer_swap();
  } while (!Input.joy1_button_1);

  Engine_release();
  DEBUG_RELEASE_PRINT();

  return 0;
}
