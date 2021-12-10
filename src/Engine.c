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


#include "Engine.h"
#include "EngineConfig.h"

#include "Memory.h"
#include "Interrupt.h"
#include "Graphix.h"
#include "SpriteManager.h"
#include "Input.h"
#include "AASandbox.h"

/* ***************************************************************  */

/* Static members of this module.  */
Engine_t Engine;

static void __fastcall__
_Engine_init_blackscreen(void)
{
  /* must be the first of all  */
  Interrupt_init();

  /* Engine_t init stuff  */

  /* already set in ENGINE_INIT() as return value
   * Engine.set.exit_code = ENGINE_EXIT_NOTERMINATION;
   */
  Engine.poll_time = ENGINE_MS2TIMESTAMP(0);
  Engine.tick_time = ENGINE_MS2TIMESTAMP(0);
  Engine.tick_count = ENGINE_MS2TICKS(0);

  /* init VIC-II hardware sprites and free all depending resources  */
  SpriteManager_init();

  /* init input stuff, such like joystick, keyboard, etc  */
  Input_init();

  /* init test and staged code  */
  AASandbox_init();
}

void __fastcall__
Engine_init(void)
{
  /* return value of this funtion call  */
  Engine.set.exit_code = ENGINE_EXIT_NOTERMINATION;

  DEBUG_INIT();

  Memory_init();

  /* init static member of EngineConfig  */
  EngineConfig_init();

  Graphix_init(_Engine_init_blackscreen);

  /* init as last as possible  */
  Timer_init();

  /* returns ENGINE.SET.EXIT_CODE  */
}

/* ---------------------------------------------------------------  */

static void __fastcall__
_Engine_release_blackscreen(void)
{
  /* Reversed order of _Engine_init_blackscreen().
   */

  /* also test- and staged code should be deinitialized  */
  AASandbox_release();

  /* hmmmm ...  */
  Input_release();

  /* free all sprite resources and virtual sprites  */
  SpriteManager_release();

  /* must be the last of all  */
  Interrupt_release();
}

void __fastcall__
Engine_release(void)
{
  /* Reversed order of Engine_init().
   */

  Timer_release();

  Graphix_release(_Engine_release_blackscreen);

  EngineConfig_release();

  Memory_release();
  DEBUG_RELEASE_PRINT();
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
   *   delta Engine.tick_time
   *     = ENGINE_TICKRATE/TIMER_TICKRATE
   *     = (ENGINE_TICK_FACTOR*TIMER_TICKRATE)/TIMER_TICKRATE
   *     = ENGINE_TICK_FACTOR
   */
  if (Engine.poll_time < Engine.tick_time + ENGINE_TICK_FACTOR) {
    return false;
  }

#ifdef DEBUG
  if (Engine.poll_time >= Engine.tick_time + 2*ENGINE_TICK_FACTOR) {
    DEBUG_WARN("engine tick, is lagging");
  }
#endif

  Engine.tick_time = Engine.poll_time;
  ++Engine.tick_count;

  return true;
}
