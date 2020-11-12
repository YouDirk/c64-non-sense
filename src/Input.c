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

static uint8_t _joy_port2_x_mode, _joy_port2_y_mode;
static uint8_t _joy_port1_x_mode, _joy_port1_y_mode;

/* ***************************************************************  */

/* Static members of this module.  */
Input_t Input;

void __fastcall__
Input_init(Input_device_t devices)
{
  Input.enabled = devices;

  /* will be initialized with zero automatically by .BSS segment
  memset(&Input.joy_port2, 0x00, sizeof(Input_joystick_t));
  memset(&Input.joy_port1, 0x00, sizeof(Input_joystick_t));
  */

  /* set data direction of CIA1 port A and port B  */
  CIA1.ddra = CIA_DDR_RONLY_ALL;
  CIA1.ddrb = CIA_DDR_RONLY_ALL;
}

void __fastcall__
Input_release(void)
{
  /* restore data direction of CIA1 port A and port B  */
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

    if (reg_buf & (CIA1_PRAB_JOYUP_MASK | CIA1_PRAB_JOYDOWN_MASK)) {
      if (!Input.joy_port2.y_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.y_pressed = true;
      }

      _joy_port2_y_mode = (0x40 & 0x60) | ((1 << 3) & 0x18) | (0x04 & 0x07);

      if (reg_buf & CIA1_PRAB_JOYDOWN_MASK) _joy_port2_y_mode |= 0x80;
    } else {
      Input.joy_port2.y_pressed = false;
    }

    if (reg_buf & (CIA1_PRAB_JOYLEFT_MASK | CIA1_PRAB_JOYRIGHT_MASK)) {
      if (!Input.joy_port2.x_pressed) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.x_pressed = true;
      }

      _joy_port2_x_mode = (0x40 & 0x60) | ((1 << 3) & 0x18) | (0x04 & 0x07);

      if (reg_buf & CIA1_PRAB_JOYRIGHT_MASK) _joy_port2_x_mode |= 0x80;
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
  if (_joy_port2_y_mode) {
    switch (_joy_port2_y_mode & 0x60) {
    case 0x40:
      Input.joy_port2.y_pace = (_joy_port2_y_mode >> 3) & 0x03;
      break;
    case 0x20:
      Input.joy_port2.y_pace = _joy_port2_y_mode & (0x01 << 0);
      break;
    case 0x00:
      Input.joy_port2.y_pace = (_joy_port2_y_mode & (0x03 << 0)) == 0;
      break;
    case 0x60:
      _joy_port2_y_mode = 0x00;
      Input.joy_port2.y_pace = 0;
      break;
    default:
      DEBUG_ERROR("input tick, joy mode y");
      break;
    }

    if (_joy_port2_y_mode & 0x80)
      Input.joy_port2.y_pace = -Input.joy_port2.y_pace;

    _joy_port2_y_mode -= (1 << 0);
  }

  if (_joy_port2_x_mode) {
    switch (_joy_port2_x_mode & 0x60) {
    case 0x40:
      Input.joy_port2.x_pace = (_joy_port2_x_mode >> 3) & 0x03;
      break;
    case 0x20:
      Input.joy_port2.x_pace = _joy_port2_x_mode & (0x01 << 0);
      break;
    case 0x00:
      Input.joy_port2.x_pace = (_joy_port2_x_mode & (0x03 << 0)) == 0;
      break;
    case 0x60:
      _joy_port2_x_mode = 0x00;
      Input.joy_port2.x_pace = 0;
      break;
    default:
      DEBUG_ERROR("input tick, joy mode x");
      break;
    }

    if (_joy_port2_x_mode & 0x80)
      Input.joy_port2.x_pace = -Input.joy_port2.x_pace;

    _joy_port2_x_mode -= (1 << 0);
  }
}
