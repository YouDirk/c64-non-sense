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

static uint16_s _joy_port2_axes_mode[Input_axis_count];

/* ***************************************************************  */

/* Static members of this module.  */
Input_t Input;

void __fastcall__
Input_init(Input_device_t devices)
{
  /* will be initialized with zero automatically by .BSS segment
  memset(_joy_port2_axes_mode,
         0x00, Input_axis_count*sizeof(_joy_port2_axes_mode));
  */

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

/* *******************************************************************
 *
 * Every axis of an joystick has a 16 bit MODE variable which saves
 * the current state between engine ticks.  It looks like this
 *
 *   | 15 | 14 13 12 | 11 10  9  8 || 7  6  5  4 |  3  2  1  0  |
 *   |----------------------------------------------------------|
 *   |    |          |     mode    ||    pace    | tick-counter |
 *   '--|------|------------------------------------------------'
 *      |      |
 *      |     sequence-counter (for every n ticks to do sth.)
 *     sign bit
 */


#if 0
static Input_device_t __fastcall__
_Input_poll_joystick(uint8_t axis)
{

}
#endif

/* ***************************************************************  */

Input_device_t __fastcall__
Input_poll(void)
{
  Input_device_t result = Input_none_mask;
  uint8_t reg_buf;

  if (Input.enabled & Input_joy_port2_mask) {
    reg_buf = ~CIA1.pra & CIA1_PRAB_JOY_MASK;

    if (reg_buf & (CIA1_PRAB_JOYUP_MASK | CIA1_PRAB_JOYDOWN_MASK)) {
      if (!Input.joy_port2.axes_pressed[Input_axis_y]) {
        result |= Input_joy_port2_mask;
        Input.joy_port2.axes_pressed[Input_axis_y] = true;
      }

      _joy_port2_axes_mode[Input_axis_y].byte_high = 0x04 & 0x0f;
      _joy_port2_axes_mode[Input_axis_y].byte_low
        = (1 << 4) & 0xf0 | (0x04 & 0x0f);

      if (reg_buf & CIA1_PRAB_JOYDOWN_MASK)
        _joy_port2_axes_mode[Input_axis_y].byte_high |= 0x80;
    } else {
      Input.joy_port2.axes_pressed[Input_axis_y] = false;
    }

    if (reg_buf & CIA1_PRAB_JOYBTN1_MASK) {
      if (!Input.joy_port2.axes_pressed[Input_axis_y])
        result |= Input_joy_port2_mask;

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
  if (UINT16(_joy_port2_axes_mode[Input_axis_y])) {
    switch (_joy_port2_axes_mode[Input_axis_y].byte_high & 0x0f) {
    case 0x04:
      Input.joy_port2.axes_pace[Input_axis_y]
        = _joy_port2_axes_mode[Input_axis_y].byte_low >> 4;

      break;
    case 0x03:
      Input.joy_port2.axes_pace[Input_axis_y]
        = (_joy_port2_axes_mode[Input_axis_y].byte_high & 0x70)
          != 0;

      break;
    case 0x02:
      Input.joy_port2.axes_pace[Input_axis_y]
        = (_joy_port2_axes_mode[Input_axis_y].byte_high & (0x10 & 0x70))
          != 0;

      break;
    case 0x01:
      Input.joy_port2.axes_pace[Input_axis_y]
        = (_joy_port2_axes_mode[Input_axis_y].byte_high & 0x70)
          == 0;

      break;
    default:
      DEBUG_ERROR("input tick, joy mode y");
    case 0x00:
      Input.joy_port2.axes_pace[Input_axis_y] = 0;
      UINT16(_joy_port2_axes_mode[Input_axis_y])
        = (0x7000 & (1 << 12)) | (1 << 5);

      break;
    }

    if (_joy_port2_axes_mode[Input_axis_y].byte_high & 0x80) {
      Input.joy_port2.axes_pace[Input_axis_y]
        = -Input.joy_port2.axes_pace[Input_axis_y];
    }

    if ((_joy_port2_axes_mode[Input_axis_y].byte_high & 0x70) == 0) {
      _joy_port2_axes_mode[Input_axis_y].byte_high |= 0x40;
    }
    UINT16(_joy_port2_axes_mode[Input_axis_y])
      -= (0x7000 & (1 << 12)) | (1 << 5);
  }
}

/* ***************************************************************  */

void __fastcall__
Input_enable(Input_device_t devices)
{
  Input.enabled = devices;
}
