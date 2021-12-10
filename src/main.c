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


#include "common.h"

#include "Engine.h"
#include "Graphix.h"
#include "Input.h"
#include "AASandbox.h"
#include "SpriteManager.h"

/* ***************************************************************  */

int
main(void)
{
  /* returns ENGINE.SET.EXIT_CODE  */
  Engine_init();

  while (Engine.set.exit_code < 0) {

    DEBUG_RENDERTIME_FREECPU_BEGIN(Graphix_black);
    do {
      /* *************************************************************
       * polling stuff between engine ticks
       */

      /* poll test- and staged code  */
      AASandbox_poll();

      DEBUG_RENDERTIME_FREECPU_COLOR_SET();
    } while (!Engine_tick_poll());
    DEBUG_RENDERTIME_FREECPU_END();

    /* ***************************************************************
     * time critical ticking stuff (input delay)
     */

    /* first in time critical section  */
    Input_tick();

    /* poll test- and staged code  */
    AASandbox_tick();

    /* sprite tick for animations, etc  */
    SpriteManager_tick();

    /* *** render, what we´ve done ***  */
    Graphix_buffer_swap();

    /* ***************************************************************
     * low priority ticking stuff
     */

    /* poll test- and staged code  */
    AASandbox_tick_low();

    /*
     * ***********************************************************  */
  }

  Engine_release();
  return Engine.set.exit_code;
}
