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

/* *******************************************************************
 * C64 keyboard matrix
 *
 * legend: PA (CIA1 port A), PB (CIA1 port B)
 *         joy2 (joystick port 2), joy1 (joystick port 1)
 *
 *      |   PB7    PB6  PB5   PB4    PB3     PB2     PB1    PB0  | joy2
 * ---------------------------------------------------------------------
 * PA7  | RUNSTOP   Q   C=   SPACE    2      CTRL    LEFT    1   |
 * PA6  |    /     UP    =   RSHIFT  HOME     ;       *    POUND |
 * PA5  |    ,      @    :     .      -       L       P      +   |
 * PA4  |    N      O    K     M      0       J       I      9   | Btn1
 * PA3  |    V      U    H     B      8       G       Y      7   | Right
 * PA2  |    X      T    F     C      6       D       R      5   | Left
 * PA1  | LSHIFT    E    S     Z      4       A       W      3   | Down
 * PA0  | CRSR DN  F5   F3    F1     F7    CRSR RT  RETURN  DEL  | Up
 * ---------------------------------------------------------------------
 * joy1 |                    Btn1   Right    Left    Down   Up   |
 *
 */

/* ***************************************************************  */

/* Selectors for one or more specific input devices.  */
#define Input_none_mask            ((Input_devices_t) 0x00)
#define Input_joy_port2_mask       ((Input_devices_t) 0x01)
#define Input_joy_port1_mask       ((Input_devices_t) 0x02)
#define Input_joy_all_mask         ((Input_devices_t) 0x03)
#define Input_all_mask             ((Input_devices_t) 0xff)
typedef uint8_t                    Input_devices_t;

/* Information of an axis  */
typedef struct Input_axis_t {

  /* 1 or -1 depending on push direction during this engine tick, if
   * axis is pressed .
   *
   * Assert: Must have SIZEOF 1 and be first member in this struct!
   */
  int8_t direction;

  /* TRUE for exactly 1 tick, if DIRECTION has changed it´s value.  */
  bool changed;

} Input_axis_t;

/* Information of a button  */
typedef struct Input_button_t {

  /* TRUE if button is pressed during this engine tick.
   *
   * Assert: Must have SIZEOF 1 and be first member in this struct!
   */
  bool pressed;

  /* TRUE for exactly 1 tick, if PRESSED has changed it´s value.  */
  bool changed;

} Input_button_t;

/* Information about a joystick  */
typedef struct Input_joystick_t {

  /* State of the axes.  */
  Input_axis_t axis_y, axis_x;

  /* State of the fire button.  */
  Input_button_t button1;

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

/* Check the enabled input devices for state changes.  */
extern void __fastcall__ Input_tick(void);

#endif /* INPUT_H__  */
