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


#ifndef INPUT_H__
#define INPUT_H__

#include "common.h"

/* ***************************************************************  */

/* Selectors for one or more specific input devices.  */
#define Input_none_mask            ((Input_device_t) 0x00)
#define Input_joy1_mask            ((Input_device_t) 0x01)
#define Input_joy2_mask            ((Input_device_t) 0x02)
#define Input_all_mask             ((Input_device_t) 0xff)
typedef uint8_t                    Input_device_t;

/* Structure of static members for module.  */
typedef struct Input_t {

  /* Logical joystick 1: horizontal speed, vertical speed and fire
   * bottons.
   */
  int8_t joy1_pace_x;
  int8_t joy1_pace_y;
  bool   joy1_button_1;

  /* Logical joystick 2: horizontal speed, vertical speed and fire
   * bottons.
   */
  int8_t joy2_pace_x;
  int8_t joy2_pace_y;
  bool   joy2_button_1;

} Input_t;

/* ***************************************************************  */

/* Static members of this module.  */
extern Input_t Input;

/* Initialize input devices, such like joysticks, keyboard, etc.  */
extern void __fastcall__ Input_init(void);

/* Restore input configuration.  */
extern void __fastcall__ Input_release(void);

/* ***************************************************************  */

/* Polls all input devices for state changes.
 *
 * returns: FALSE/INPUT_NONE if nothing is changed in static module
 *          members Input.*.  Otherwise the device selectors are
 *          returned which state changed.
 */
extern Input_device_t __fastcall__ Input_poll(Input_device_t devices);

/* ***************************************************************  */

#endif /* INPUT_H__  */
