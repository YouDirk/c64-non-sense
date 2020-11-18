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
#define Input_joy_port2_mask       ((Input_device_t) 0x01)
#define Input_joy_port1_mask       ((Input_device_t) 0x02)
#define Input_all_mask             ((Input_device_t) 0xff)
typedef uint8_t                    Input_device_t;

/* Paces >= 1 or Paces <= -1 are integers.  Paces which are slower
 * than 1 and greater than -1 will be alterate it´s pace to 1/-1 or 0
 * between engine ticks, depending on the pace.
 *
 * So you can use this value directly as increment for outputs in
 * ticking components.
 */
typedef int8_t                     Input_pace_t;

/* Information about a joystick  */
typedef struct Input_joystick_t {
  bool         y_pressed;
  bool         x_pressed;

  Input_pace_t y_pace;
  Input_pace_t x_pace;

  bool         button1_pressed;
} Input_joystick_t;

/* Structure of static members for module.  */
typedef struct Input_t {

  /* Input devices which will be polled and are ticking in this
   * module.
   */
  Input_device_t enabled;

  /* The two joysticks on physical Port 2 and Port 1 :)  */
  Input_joystick_t joy_port2;
  Input_joystick_t joy_port1;

} Input_t;

/* ***************************************************************  */

/* Static members of this module.  */
extern Input_t Input;

/* Select and initialize input devices, such like joysticks, keyboard,
 * for polling and ticking.
 */
extern void __fastcall__ Input_init(Input_device_t devices);

/* Restore input configuration.  */
extern void __fastcall__ Input_release(void);

/* ***************************************************************  */

/* Devices which will be polled and are ticking.  Initialized first
 * time during Input_init().
 */
extern void __fastcall__ Input_enable(Input_device_t devices);

/* Polls the enabled input devices for state changes.
 *
 * returns: FALSE/INPUT_NONE if no enabled device has changed it state
 *          static module members Input.*.  Otherwise the device
 *          selectors are returned which state changed.
 */
extern Input_device_t __fastcall__ Input_poll(void);

/* Let the enabled input devices ticking to update paces, etc.  */
extern void __fastcall__ Input_tick(void);

/* ***************************************************************  */

#endif /* INPUT_H__  */
