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
  Input.joy1_pace_x = 0;
  Input.joy1_pace_y = 0;
  Input.joy1_button_1 = false;

  Input.joy2_pace_x = 0;
  Input.joy2_pace_y = 0;
  Input.joy2_button_1 = false;

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

Input_device_t __fastcall__
Input_poll(Input_device_t devices)
{
  Input_device_t result = Input_none_mask;
  uint8_t reg_buf;

  if (devices & Input_joy1_mask) {
    reg_buf = ~CIA1.pra & CIA1_PRAB_JOY_MASK;

    if (reg_buf & CIA1_PRAB_JOYUP_MASK)    Input.joy1_pace_y = 1;
    if (reg_buf & CIA1_PRAB_JOYDOWN_MASK)  Input.joy1_pace_y = -1;
    if (reg_buf & CIA1_PRAB_JOYLEFT_MASK)  Input.joy1_pace_x = 1;
    if (reg_buf & CIA1_PRAB_JOYRIGHT_MASK) Input.joy1_pace_x = -1;

    Input.joy1_button_1 = reg_buf & CIA1_PRAB_JOYBTN1_MASK;
  }

  return result;
}
