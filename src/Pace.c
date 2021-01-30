/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2021  Dirk "YouDirk" Lehmann
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


#include "Pace.h"

/* *******************************************************************
 *
 * Every PACE has a 16 bit STATUS variable which saves the current
 * state between engine ticks.  It looks like this
 *
 *   | 15 | 14 13 12 11 10 |  9  8 ||  7  6  5  4  3  2 |  1  0  |
 *   |-----------------------------------------------------------|
 *   |    |                |  mode ||    tick-counter   |        |
 *   '--|---------|-----------------------------------------|----'
 *      |         |                                         |
 *      |        pixel pace                               frac-counter
 *     sign bit
 */

#define _STATUS_HIGH_SIGN_MASK          0x80
#define _STATUS_HIGH_PXLPACE_MASK       0x7c
#define _STATUS_HIGH_PXLPACE_SHIFT      2
#define _STATUS_HIGH_MODE_MASK          0x03
#define _STATUS_HIGH_MODE_PACE           0x0
#define _STATUS_HIGH_MODE_1OF4           0x1
#define _STATUS_HIGH_MODE_1OF2           0x2
#define _STATUS_HIGH_MODE_3OF4           0x3
#define _STATUS_LOW_TICKCOUNTER_SHIFT   2
#define _STATUS_LOW_FRACCOUNTER_MASK    0x03
#define _STATUS_LOW_FRACCOUNTER_BIT0    0x01

/* ***************************************************************  */

void __fastcall__
Pace_new(Pace_t* pace,
         uint8_t pace_max, uint8_t accelerate, uint8_t brakerate,
         uint8_t delay)
{
  pace->pace = 0;

  UINT16(pace->_status) = 0x0000;

  pace->_max.byte_high
    = (_STATUS_HIGH_PXLPACE_MASK | _STATUS_HIGH_MODE_MASK) & pace_max;
  pace->_max.byte_low = delay << _STATUS_LOW_TICKCOUNTER_SHIFT;

  pace->_decrement_brake.byte_high = 0x00;
  pace->_decrement_brake.byte_low
    = brakerate << _STATUS_LOW_TICKCOUNTER_SHIFT
    | _STATUS_LOW_FRACCOUNTER_BIT0;

  UINT16(pace->_decrement_accel)
    = ~UINT16(accelerate << _STATUS_LOW_TICKCOUNTER_SHIFT
              | _STATUS_LOW_FRACCOUNTER_BIT0) + 1;
}

/* Nothing to do.  Just an empty-macro for now.
 */

/* void __fastcall__ Pace_delete(Pace_t* pace) {}  */

/* ***************************************************************  */

void __fastcall__
Pace_tick(Pace_t* pace)
{
  static uint8_t pace_result, fraccounter, pacemode_abs;

  fraccounter
    = _STATUS_LOW_FRACCOUNTER_MASK & pace->_status.byte_low;
  pacemode_abs
    = ~_STATUS_HIGH_SIGN_MASK & pace->_status.byte_high;

  if (pacemode_abs == 0) {
    pace->pace = 0;
    return;
  }

  pace_result = pacemode_abs >> _STATUS_HIGH_PXLPACE_SHIFT;

  switch (_STATUS_HIGH_MODE_MASK & pacemode_abs) {
  case _STATUS_HIGH_MODE_3OF4:
    pace_result += fraccounter != 0;
    break;
  case _STATUS_HIGH_MODE_1OF2:
    pace_result += (_STATUS_LOW_FRACCOUNTER_BIT0 & fraccounter) != 0;
    break;
  case _STATUS_HIGH_MODE_1OF4:
    pace_result += fraccounter == 0;
    break;
  case _STATUS_HIGH_MODE_PACE:
    pace_result += 0;
    break;
  default:
    DEBUG_ERROR("pace tick, switch mode!");
    pace_result = 0;
    UINT16(pace->_status) = UINT16(pace->_decrement);
  }

  if (_STATUS_HIGH_SIGN_MASK & pace->_status.byte_high) {
    /* same 'pace_result = -pace_result', but better performance  */
    pace_result = ~pace_result + 1;
  }
  pace->pace = pace_result;

  if (pacemode_abs <= pace->_max.byte_high)
    UINT16(pace->_status) -= UINT16(pace->_decrement);
  else
    pace->_status.byte_low -= _STATUS_LOW_FRACCOUNTER_BIT0;
}

/* ***************************************************************  */

void __fastcall__
Pace_impulse_pos(Pace_t* pace)
{
  pace->_status.byte_high = pace->_max.byte_high;
  pace->_status.byte_low
    = (_STATUS_LOW_FRACCOUNTER_MASK & pace->_status.byte_low)
    | pace->_max.byte_low;

  UINT16(pace->_decrement) = UINT16(pace->_decrement_brake);
}

void __fastcall__
Pace_impulse_neg(Pace_t* pace)
{
  pace->_status.byte_high
    = _STATUS_HIGH_SIGN_MASK | pace->_max.byte_high;
  pace->_status.byte_low
    = (_STATUS_LOW_FRACCOUNTER_MASK & pace->_status.byte_low)
    | pace->_max.byte_low;

  UINT16(pace->_decrement) = UINT16(pace->_decrement_brake);
}

void __fastcall__
Pace_accelerate_pos(Pace_t* pace)
{
  pace->_status.byte_high = _STATUS_HIGH_MODE_1OF4;
  pace->_status.byte_low = 0x00;

  UINT16(pace->_decrement) = UINT16(pace->_decrement_accel);
}

void __fastcall__
Pace_accelerate_neg(Pace_t* pace)
{
  pace->_status.byte_high
    = _STATUS_HIGH_SIGN_MASK | _STATUS_HIGH_MODE_1OF4;
  pace->_status.byte_low = 0x00;

  UINT16(pace->_decrement) = UINT16(pace->_decrement_accel);
}
