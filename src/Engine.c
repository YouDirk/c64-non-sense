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


#include "Engine.h"

/* ***************************************************************  */

/* Static members of this module.  */
Engine_t Engine;

void __fastcall__
Engine_init(void)
{
  Engine.poll_timestamp = 0;
  Engine.tick_timestamp = 0;
}

void __fastcall__
Engine_release(void)
{
}

/* ***************************************************************  */

/* During implementation of this function on the radio ran "Clean
 * Bandit and Mabel - Tick Tock" ...  Not a joke xD
 */
bool __fastcall__
Engine_tick_poll(void)
{
  Engine.poll_timestamp = Timer_1_get32();

  /* Time to poll is
   *
   *  ENGINE_TICKINTERVAL/TIMER_INTERVAL
   *    = (ENGINE_TICK_FACTOR*TIMER_INTERVAL)/TIMER_INTERVAL
   *    = ENGINE_TICK_FACTOR
   */
  if (Engine.poll_timestamp
      < Engine.tick_timestamp + ENGINE_TICK_FACTOR) {
    return false;
  }

  Engine.tick_timestamp = Engine.poll_timestamp;
  return true;
}
