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

#define INPUT_JOY_PACE_DEFAULT          1
#define INPUT_JOY_BRAKERATE_DEFAULT     32
#define INPUT_JOY_DELAY_DEFAULT         4

/* ***************************************************************  */

/* Selectors for one or more specific input devices.  */
#define Input_none_mask            ((Input_devices_t) 0x00)
#define Input_joy_port2_mask       ((Input_devices_t) 0x01)
#define Input_joy_port1_mask       ((Input_devices_t) 0x02)
#define Input_joy_all_mask         ((Input_devices_t) 0x03)
#define Input_all_mask             ((Input_devices_t) 0xff)
typedef uint8_t                    Input_devices_t;

/* Selectors for one or more specific axis of joystick devices.  */
#define Input_axes_none_mask       ((Input_axes_t) 0x00)
#define Input_axes_y_mask          ((Input_axes_t) 0x01)
#define Input_axes_x_mask          ((Input_axes_t) 0x02)
#define Input_axes_all_mask        ((Input_axes_t) 0xff)
typedef uint8_t                    Input_axes_t;

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
  bool         y_pressed, x_pressed;

  Input_pace_t y_pace, x_pace;

  bool         button1_pressed;
} Input_joystick_t;

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef struct Input_set_t {

  /* Input devices which will be polled and are ticking in this
   * module.
   */
  Input_devices_t enabled;

} Input_set_t;

/* Structure of static members for module.  */
typedef struct Input_t {

  /* Some writable member variables.  */
  Input_set_t set;

  /* The two joysticks on physical Port 2 and Port 1 :)  */
  Input_joystick_t joy_port2, joy_port1;

} Input_t;

/* ***************************************************************  */

/* Static members of this module.  */
extern Input_t Input;

/* Select and initialize input devices, such like joysticks, keyboard,
 * for polling and ticking.
 */
extern void __fastcall__ Input_init(Input_devices_t devices);

/* Restore input configuration.  */
extern void __fastcall__ Input_release(void);

/* ***************************************************************  */

/* Polls the enabled input devices for state changes.
 *
 * returns: FALSE/INPUT_NONE if no enabled device has changed it state
 *          static module members Input.*.  Otherwise the device
 *          selectors are returned which state changed.
 */
extern Input_devices_t __fastcall__ Input_poll(void);

/* Let the enabled input devices ticking to update paces, etc.  */
extern void __fastcall__ Input_tick(void);

/* ***************************************************************  */

/* Configure selected joystick DEVICEs.  */
extern void __fastcall__ Input_joy_config(
  Input_devices_t devices, Input_axes_t axes, int4_t pace,
  uint8_t brakerate, uint4_t delay);

#endif /* INPUT_H__  */
