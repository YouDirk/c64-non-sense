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
Input_init(void)
{
  Input.enabled = Input_none_mask;

  /* will be initialized with zero automatically by .BSS segment
  memset(&Input.joy_port2, 0x00, sizeof(Input_joystick_t));
  memset(&Input.joy_port1, 0x00, sizeof(Input_joystick_t));
  */

  /* set data direction of port A and port B  */
  CIA1.ddra = CIA_DDR_RONLY_ALL;
  CIA1.ddrb = CIA_DDR_RONLY_ALL;
}

void __fastcall__
Input_release(void)
{
  /* restore data direction of port A and port B  */
  CIA1.ddrb = CIA1_DDRB_DEFAULT;
  CIA1.ddra = CIA1_DDRA_DEFAULT;
}

/* ***************************************************************  */

void __fastcall__
Input_enable(Input_device_t devices)
{
  Input.enabled = devices;
}

Input_device_t __fastcall__
Input_poll(void)
{
  Input_device_t result = Input_none_mask;
  uint8_t reg_buf;

  if (Input.enabled & Input_joy_port2_mask) {
    reg_buf = ~CIA1.pra & CIA1_PRAB_JOY_MASK;

    if (reg_buf & CIA1_PRAB_JOYUP_MASK) {
      if (!Input.joy_port2.y_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.y_pressed = true;
      }

      Input.joy_port2.y_pace = 0x40;
    } else if (reg_buf & CIA1_PRAB_JOYDOWN_MASK) {
      if (!Input.joy_port2.y_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.y_pressed = true;
      }

      Input.joy_port2.y_pace = -0x30;
    } else {
      Input.joy_port2.y_pressed = false;
    }

    if (reg_buf & CIA1_PRAB_JOYLEFT_MASK) {
      if (!Input.joy_port2.x_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.x_pressed = true;
      }

      Input.joy_port2.x_pace = 0x40;
    } else if (reg_buf & CIA1_PRAB_JOYRIGHT_MASK) {
      if (!Input.joy_port2.x_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.x_pressed = true;
      }

      Input.joy_port2.x_pace = -0x30;
    } else {
      Input.joy_port2.x_pressed = false;
    }

    if (reg_buf & CIA1_PRAB_JOYBTN1_MASK) {
      if (!Input.joy_port2.x_pressed) result |= Input_joy_port2_mask;

      Input.joy_port2.button1_pressed = true;
    } else {
      Input.joy_port2.button1_pressed = false;
    }
  }

  return result;
}

void __fastcall__
Input_tick(void)
{
  if (!Input.joy_port2.y_pressed) {
    if (Input.joy_port2.y_pace > 0)      --Input.joy_port2.y_pace;
    else if (Input.joy_port2.y_pace < 0) ++Input.joy_port2.y_pace;
  }
  if (!Input.joy_port2.x_pressed) {
    if (Input.joy_port2.x_pace > 0)      --Input.joy_port2.x_pace;
    else if (Input.joy_port2.x_pace < 0) ++Input.joy_port2.x_pace;
  }
}
