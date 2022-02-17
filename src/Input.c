/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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


#include "Input.h"

#include "EngineConfig.h"

#include "chip-cia.gen.h"
#include "kernal.gen.h"

/* ***************************************************************  */

/* Static members of this module.  */
Input_t Input;

void __fastcall__
Input_init(void)
{
  Input.set.enabled = EngineConfig.inputs_enabled;

  memset(&Input.joy_port2, 0x00, sizeof(Input_joystick_t));
  memset(&Input.joy_port1, 0x00, sizeof(Input_joystick_t));

  memset(&Input.keyboard, 0x00, sizeof(Input_keyboard_t));
  Input.keyboard.sc_pressed[0] = Input_sc_none_e; /* 0x40  */

  /* zero-page variables which we are setting, instead of Kernal  */
  KERNAL_ZP_KEYBOARD_SCANCODE_PREV = Input_sc_none_e;
  KERNAL_ZP_KEYBOARD_SCANCODE = Input_sc_none_e;
  KERNAL_ZP_PETSCIITABLE_VECTOR = &KERNAL_PETSCIITABLE;

  /* direction of CIA1 port A and B is read-only by default  */
  CIA1.ddra = CIA_DDR_RONLY_ALL;
  CIA1.ddrb = CIA_DDR_RONLY_ALL;
}

void __fastcall__
Input_release(void)
{
  /* restore data direction of CIA1 port A and port B  */
  CIA1.ddrb = CIA1_DDRB_DEFAULT;
  CIA1.ddra = CIA1_DDRA_DEFAULT;

  CIA1.pra = CIA1_PRA_DEFAULT;
  CIA1.prb = CIA1_PRB_DEFAULT;
}

/* ***************************************************************  */

void __fastcall__
Input_tick(void)
{
  DEBUG_RENDERTIME_IRQ_BEGIN(Graphix_lightred);

  if (Input.set.enabled & Input_joystick_port2_mask)
    _Input_joystick_port2_tick();
  if (Input.set.enabled & Input_joystick_port1_mask)
    _Input_joystick_port1_tick();

  if (Input.set.enabled & Input_keyboard_scan_mask) {

    _Input_keyboard_scan();

    if (Input.set.enabled & Input_keyboard_petscii_mask)
      _Input_keyboard_petscii_convert();
  }

  DEBUG_RENDERTIME_IRQ_END();
}

/* ***************************************************************  */
