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

#define _MODE_LOW_TICKCOUNTER_MASK      0x0f
#define _MODE_LOW_PACE_MASK             0xf0
#define _MODE_LOW_PACE_SHIFT            4
#define _MODE_HIGH_MODE_MASK            0x0f
#define _MODE_HIGH_MODE_STOPPED          0x0
#define _MODE_HIGH_MODE_1OF4             0x1
#define _MODE_HIGH_MODE_1OF2             0x2
#define _MODE_HIGH_MODE_3OF4             0x3
#define _MODE_HIGH_MODE_PACE             0x4
#define _MODE_HIGH_SEQCOUNTER_MASK      0x70
#define _MODE_HIGH_SEQCOUNTER_SHIFT     4
#define _MODE_HIGH_SEQCOUNTER_BIT0      0x10
#define _MODE_HIGH_SEQCOUNTER_MAX        0x4
#define _MODE_HIGH_SIGN_MASK            0x80

#define _MODE_16_INITIAL(mode, pace, delay)                          \
  ((mode & _MODE_HIGH_MODE_MASK) << 8                                \
   | (pace << _MODE_LOW_PACE_SHIFT)                                  \
   | (delay & _MODE_LOW_TICKCOUNTER_MASK))

#define _MODE_16_DECREMENT(brakerate)                                \
  ((_MODE_HIGH_SEQCOUNTER_MASK                                       \
    & (1 << _MODE_HIGH_SEQCOUNTER_SHIFT)) << 8 | (brakerate))

/* ***************************************************************  */

typedef uint16_s                   _joy_axis_mode_t;

typedef struct _joy_axis_t {
  _joy_axis_mode_t mode;
  uint16_t initial;
  uint16_t decrement;
} _joy_axis_t;

typedef struct _joy_status_t {
  _joy_axis_t y, x;
} _joy_status_t;

static _joy_status_t _joy_port2, _joy_port1;

/* ***************************************************************  */

/* Static members of this module.  */
Input_t Input;

void __fastcall__
Input_init(Input_device_t devices)
{
  Input.set.enabled = devices;

  Input_joy_config(Input_joy_all_mask, INPUT_JOY_PACE_DEFAULT,
    INPUT_JOY_BRAKERATE_DEFAULT, INPUT_JOY_DELAY_DEFAULT);

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

static bool __fastcall__
_joystick_axis_poll(_joy_axis_t* result, bool* result_pressed,
                    uint8_t cia_port_inv_shifted)
{
#if (CIA1_PRAB_JOYLEFT_MASK | CIA1_PRAB_JOYRIGHT_MASK) >> 2 \
    != (CIA1_PRAB_JOYUP_MASK | CIA1_PRAB_JOYDOWN_MASK)
#  error "_joystick_axis_poll(): CIA_PORT_INV_SHIFTED: Assertion failed!"
#endif

  if ((cia_port_inv_shifted
       & (CIA1_PRAB_JOYUP_MASK | CIA1_PRAB_JOYDOWN_MASK)) == 0) {
    *result_pressed = false;
    return false;
  }

  UINT16(result->mode) = result->initial;

  if (cia_port_inv_shifted & CIA1_PRAB_JOYDOWN_MASK)
    result->mode.byte_high |= _MODE_HIGH_SIGN_MASK;

  /* just trigger poll result one time per pressing down  */
  if (*result_pressed) return false;

  *result_pressed = true;
  return true;
}

Input_device_t __fastcall__
Input_poll(void)
{
  Input_device_t result = Input_none_mask;
  uint8_t cia_port_inv;
  bool tmp;

  if (Input.set.enabled & Input_joy_port2_mask) {
    cia_port_inv = ~CIA1.pra;

    tmp = _joystick_axis_poll(
      &_joy_port2.y, &Input.joy_port2.y_pressed, cia_port_inv);
    tmp |= _joystick_axis_poll(
      &_joy_port2.x, &Input.joy_port2.x_pressed, cia_port_inv >> 2);
    result |= tmp? Input_joy_port2_mask: Input_none_mask;

    if (cia_port_inv & CIA1_PRAB_JOYBTN1_MASK) {
      if (!Input.joy_port2.button1_pressed)
        result |= Input_joy_port2_mask;

      Input.joy_port2.button1_pressed = true;
    } else
      Input.joy_port2.button1_pressed = false;
  }

  if (Input.set.enabled & Input_joy_port1_mask) {
    cia_port_inv = ~CIA1.prb;

    tmp = _joystick_axis_poll(
      &_joy_port1.y, &Input.joy_port1.y_pressed, cia_port_inv);
    tmp |= _joystick_axis_poll(
      &_joy_port1.x, &Input.joy_port1.x_pressed, cia_port_inv >> 2);
    result |= tmp? Input_joy_port1_mask: Input_none_mask;

    if (cia_port_inv & CIA1_PRAB_JOYBTN1_MASK) {
      if (!Input.joy_port1.button1_pressed)
        result |= Input_joy_port1_mask;

      Input.joy_port1.button1_pressed = true;
    } else
      Input.joy_port1.button1_pressed = false;
  }

  return result;
}

/* ***************************************************************  */

static void __fastcall__
_joystick_axis_tick(Input_pace_t* result_pace, _joy_axis_t* axis)
{
  if (!UINT16(axis->mode)) return;

  switch (axis->mode.byte_high & _MODE_HIGH_MODE_MASK) {
  case _MODE_HIGH_MODE_PACE:
    *result_pace = axis->mode.byte_low >> _MODE_LOW_PACE_SHIFT;
    break;
  case _MODE_HIGH_MODE_3OF4:
    *result_pace = (axis->mode.byte_high & _MODE_HIGH_SEQCOUNTER_MASK)
      != 0;
    break;
  case _MODE_HIGH_MODE_1OF2:
    *result_pace = (axis->mode.byte_high
      & (_MODE_HIGH_SEQCOUNTER_BIT0 & _MODE_HIGH_SEQCOUNTER_MASK))
      != 0;
    break;
  case _MODE_HIGH_MODE_1OF4:
    *result_pace = (axis->mode.byte_high & _MODE_HIGH_SEQCOUNTER_MASK)
      == 0;
    break;
  default:
    DEBUG_ERROR("input tick, joy axis mode!");
  case _MODE_HIGH_MODE_STOPPED:
    *result_pace = 0;
    UINT16(axis->mode) = axis->decrement;
    break;
  }

  if (axis->mode.byte_high & _MODE_HIGH_SIGN_MASK) {
    /* same '*result_pace = -*result_pace', but better performance  */
    *result_pace = ~*result_pace + 1;
  }

  if ((axis->mode.byte_high & _MODE_HIGH_SEQCOUNTER_MASK) == 0) {
    axis->mode.byte_high
      |= _MODE_HIGH_SEQCOUNTER_MAX << _MODE_HIGH_SEQCOUNTER_SHIFT;
  }

  UINT16(axis->mode) -= axis->decrement;
}

void __fastcall__
Input_tick(void)
{
  if (Input.set.enabled & Input_joy_port2_mask) {
    _joystick_axis_tick(&Input.joy_port2.y_pace, &_joy_port2.y);
    _joystick_axis_tick(&Input.joy_port2.x_pace, &_joy_port2.x);
  }
  if (Input.set.enabled & Input_joy_port1_mask) {
    _joystick_axis_tick(&Input.joy_port1.y_pace, &_joy_port1.y);
    _joystick_axis_tick(&Input.joy_port1.x_pace, &_joy_port1.x);
  }
}

/* ***************************************************************  */

void __fastcall__
Input_joy_config(Input_device_t device, int4_t pace, uint8_t brakerate,
                 uint4_t delay)
{
  _joy_status_t* joystick;

  while (device != 0) {
    if (device & Input_joy_port2_mask) {
      joystick = &_joy_port2;
      device &= ~Input_joy_port2_mask;
    } else if (device & Input_joy_port1_mask) {
      joystick = &_joy_port1;
      device &= ~Input_joy_port1_mask;
    } else {
      DEBUG_ERROR("input config, joy device!");
      return;
    }

    UINT16(joystick->y.mode) = 0, UINT16(joystick->x.mode) = 0;

    joystick->y.initial
      = _MODE_16_INITIAL(_MODE_HIGH_MODE_PACE, pace, delay);
    joystick->x.initial
      = _MODE_16_INITIAL(_MODE_HIGH_MODE_PACE, pace, delay);

    joystick->y.decrement = _MODE_16_DECREMENT(brakerate);
    joystick->x.decrement = _MODE_16_DECREMENT(brakerate);
  }
}
