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

#include "Input.h"
#include "Graphix.h"
#include "Engine.h"
#include "Sandbox.h"

/* ***************************************************************  */

int
main(void)
{
  DEBUG_INIT();
  Engine_init();

  do {
    do {
      /* *************************************************************
       * polling stuff between engine ticks
       */

      /* poll test- and staged code  */
      Sandbox_poll();

    } while (!Engine_tick_poll());

    /* ***************************************************************
     * time critical ticking stuff (input delay)
     */

    /* first in time critical section  */
    Input_tick();

    /* poll test- and staged code  */
    Sandbox_tick();

    /* *** render, what we´ve done ***  */
    Graphix_buffer_swap();

    /* ***************************************************************
     * low priority ticking stuff
     */

    /* poll test- and staged code  */
    Sandbox_tick_low();

    /*
     * ***********************************************************  */
  } while (Engine.set.exit_code < 0);

  Engine_release();
  DEBUG_RELEASE_PRINT();

  return Engine.set.exit_code;
}
