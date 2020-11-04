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

#include "Interrupt.h"
#include "Graphix.h"

/* ***************************************************************  */

/* Static members of this module.  */
Engine_t Engine;

static void __fastcall__
_Engine_init_blackscreen(Graphix_buffer_t* graphix)
{
  unsigned i;

  /* must be the first of all  */
  Interrupt_init();

  /* own Engine init stuff  */
  Engine.poll_time = 0;
  Engine.tick_time = 0;

  /* set screen ram  */
  memset(graphix->screen_ram,
    GRAPHIX_SCREENRAM_COLOR(GRAPHIX_GREEN, GRAPHIX_BLACK),
    GRAPHIX_CELLS_PER_SCREEN);

  /* set bitmap  */
  memset(graphix->bitmap_ram, 0x41, GRAPHIX_BYTES_PER_SCREEN);
  for (i=0; i<GRAPHIX_BYTES_PER_SCREEN; i+=8)
    graphix->bitmap_ram[i] = 0xff;

  /* Timer init as last as possible  */
  Timer_init();
}

void __fastcall__
Engine_init(void)
{
  Graphix_init(_Engine_init_blackscreen);
}

/* ---------------------------------------------------------------  */

static void __fastcall__
_Engine_release_blackscreen(void)
{
  /* reversed order of init  */
  Timer_release();

  /* must be the last of all  */
  Interrupt_release();
}

void __fastcall__
Engine_release(void)
{
  Graphix_release(_Engine_release_blackscreen);
}

/* ***************************************************************  */

/* During implementation of this function on the radio ran "Clean
 * Bandit and Mabel - Tick Tock" ...  Not a joke xD
 */
bool __fastcall__
Engine_tick_poll(void)
{
  Engine.poll_time = Timer_1_get32();

  /* Time to poll is
   *
   *   ENGINE_TICKINTERVAL/TIMER_INTERVAL
   *     = (ENGINE_TICK_FACTOR*TIMER_INTERVAL)/TIMER_INTERVAL
   *     = ENGINE_TICK_FACTOR
   */
  if (Engine.poll_time < Engine.tick_time + ENGINE_TICK_FACTOR) {
    return false;
  }

  Engine.tick_time = Engine.poll_time;
  return true;
}
