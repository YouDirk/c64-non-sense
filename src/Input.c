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


#include "Input.h"

#include "chip-cia.gen.h"

/* ***************************************************************  */

/* Static members of this module.  */
Input_t Input;

void __fastcall__
Input_init(Input_devices_t devices)
{
  Input.set.enabled = devices;

  /* will be initialized with zero automatically by .BSS segment
  memset(&Input.joy_port2, 0x00, sizeof(Input_joystick_t));
  memset(&Input.joy_port1, 0x00, sizeof(Input_joystick_t));
  */

  memset(Input.keyboard.pressed, Input_sc_none_e,
         INPUT_KEYBOARD_PRESSED_SIZE * sizeof(Input_scancode_t));
  Input.keyboard.changed = false;

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

static void __fastcall__
_joystick_axis_tick(Input_axis_t* result_axis,
                    uint8_t cia_port_inv_shifted)
{
  static int8_t result_direction;

#if (CIA1_PRAB_JOYUP_MASK | CIA1_PRAB_JOYDOWN_MASK) << 2 \
    != (CIA1_PRAB_JOYLEFT_MASK | CIA1_PRAB_JOYRIGHT_MASK)
#  error "_joystick_axis_tick(): CIA_INV_SHIFTED: Assertion 1 failed!"
#endif
#if CIA1_PRAB_JOYLEFT_MASK != CIA1_PRAB_JOYBTN1_MASK >> 2
#  error "_joystick_axis_tick(): CIA_INV_SHIFTED: Assertion 2 failed!"
#endif

  if (cia_port_inv_shifted & CIA1_PRAB_JOYLEFT_MASK) {
    result_direction = 1;
  } else if (cia_port_inv_shifted & CIA1_PRAB_JOYRIGHT_MASK) {
    result_direction = -1;
  } else {
    result_direction = 0;
  }

  if (result_direction == result_axis->direction) {
    result_axis->changed = false;
    return;
  }

  result_axis->direction = result_direction;
  result_axis->changed = true;
}

void __fastcall__
Input_tick(void)
{
  static uint8_t cia_port_inv;

  if (Input.set.enabled & Input_joystick_port2_mask) {
    cia_port_inv = ~CIA1.pra;

    _joystick_axis_tick(&Input.joy_port2.axis_y, cia_port_inv << 2);
    _joystick_axis_tick(&Input.joy_port2.axis_x, cia_port_inv);

    /* Asserting: Input_axis_t::direction and Input_button_t::pressed
     * must have SIZEOF 1 and be first member in this struct!
     */
    _joystick_axis_tick((Input_axis_t*) &Input.joy_port2.button1,
                        cia_port_inv >> 2);
  }
  if (Input.set.enabled & Input_joystick_port1_mask) {
    cia_port_inv = ~CIA1.prb;

    _joystick_axis_tick(&Input.joy_port1.axis_y, cia_port_inv << 2);
    _joystick_axis_tick(&Input.joy_port1.axis_x, cia_port_inv);

    /* Asserting: Input_axis_t::direction and Input_button_t::pressed
     * must have SIZEOF 1 and be first member in this struct!
     */
    _joystick_axis_tick((Input_axis_t*) &Input.joy_port1.button1,
                        cia_port_inv >> 2);
  }

  if (Input.set.enabled & Input_keyboard_ascan_mask) {
    Input.keyboard.pressed[0] = _Input_keyboard_scan();
  }
}
