/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2025  Dirk "YouDirk" Lehmann
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


#include "EngineConfig.h"

/* ***************************************************************  */

/* Static members of this module.  */
EngineConfig_t EngineConfig;

void __fastcall__
EngineConfig_init(void)
{
  /* Set game depending engine configurations here.
   */

  /* Charset which will be set after termination of the engine.
   */
  EngineConfig.charset_exit = Graphix_charset1_symbols_e;

  /* Input devices which will be polled and are ticking in INPUT
   * module.
   *
   * Just enable the input devices which are needed to improve the
   * perfomance.
   */
  EngineConfig.inputs_enabled = Input_all_mask;

  /* Should the Sprite Animations be enabled?  For more details take a
   * look to SPRITEMANAGER_ANIMTAIONS_SET_T::IS_TICKING.
   *
   * Just enable Sprite Animations if they are needed to improve the
   * perfomance.
   */
  EngineConfig.is_sprite_animations_ticking = true;
}

void __fastcall__
EngineConfig_release(void)
{
}

/* ***************************************************************  */
