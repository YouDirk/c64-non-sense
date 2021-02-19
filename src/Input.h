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
#define Input_none_mask                 ((Input_devices_t) 0x00)
#define Input_joystick_port2_mask       ((Input_devices_t) 0x01)
#define Input_joystick_port1_mask       ((Input_devices_t) 0x02)
#define Input_joystick_all_mask         ((Input_devices_t) 0x03)
#define Input_keyboard_ascan_mask       ((Input_devices_t) 0x04)
#define Input_all_mask                  ((Input_devices_t) 0xff)
typedef uint8_t                         Input_devices_t;

/* *******************************************************************
 * C64 keyboard matrix
 *
 * legend: PA (CIA1 port A), joy2 (joystick port 2)
 *         PB (CIA1 port B), joy1 (joystick port 1)
 *
 *      |   PB7    PB6  PB5   PB4    PB3     PB2     PB1    PB0  | joy2
 * ---------------------------------------------------------------------
 * PA7  | STOPRUN   Q   C=   SPACE    2      CTRL    LEFT    1   |
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
 *
 * The C64 SCAN CODES (aka keyboard codes) are ordered from
 * bottom-right to left
 *
 *   0x00 DEL, 0x01 RETURN, 0x02 CRSR RT, 0x03 F7...
 *
 * and then upwards for each line
 *
 *   0x07 CRSR DN,                                  0x08 3, ...
 *   0x0f LSHIFT (unused in Kernal implementation), 0x10 5, ...
 *   0x17 X,                                        0x18 7, ...
 *     ...
 *     ...                                 0x3e Q, 0x3f STOPRUN
 *
 * These scan codes are full compatible to the implementation of the
 * Kernal PETSCII lookup table (C64 ASCII) at
 *
 *   (uint8_t*) 0xeb81, which is zeropage vector 0xf5 is pointing to.
 */
#define Input_sc_none_e                 ((Input_scancode_t) (0x40))

#define Input_sc_delinst_e              ((Input_scancode_t) (0x00))
#define Input_sc_return_e               ((Input_scancode_t) (0x01))
#define Input_sc_crsrright_e            ((Input_scancode_t) (0x02))
#define Input_sc_f7_e                   ((Input_scancode_t) (0x03))
#define Input_sc_f1_e                   ((Input_scancode_t) (0x04))
#define Input_sc_f3_e                   ((Input_scancode_t) (0x05))
#define Input_sc_f5_e                   ((Input_scancode_t) (0x06))
#define Input_sc_crsrdown_e             ((Input_scancode_t) (0x07))

#define Input_sc_3_e                    ((Input_scancode_t) (0x08))
#define Input_sc_w_e                    ((Input_scancode_t) (0x09))
#define Input_sc_a_e                    ((Input_scancode_t) (0x0a))
#define Input_sc_4_e                    ((Input_scancode_t) (0x0b))
#define Input_sc_z_e                    ((Input_scancode_t) (0x0c))
#define Input_sc_s_e                    ((Input_scancode_t) (0x0d))
#define Input_sc_e_e                    ((Input_scancode_t) (0x0e))
#define Input_sc_lshift_e               ((Input_scancode_t) (0x0f))

#define Input_sc_5_e                    ((Input_scancode_t) (0x10))
#define Input_sc_r_e                    ((Input_scancode_t) (0x11))
#define Input_sc_d_e                    ((Input_scancode_t) (0x12))
#define Input_sc_6_e                    ((Input_scancode_t) (0x13))
#define Input_sc_c_e                    ((Input_scancode_t) (0x14))
#define Input_sc_f_e                    ((Input_scancode_t) (0x15))
#define Input_sc_t_e                    ((Input_scancode_t) (0x16))
#define Input_sc_x_e                    ((Input_scancode_t) (0x17))

#define Input_sc_7_e                    ((Input_scancode_t) (0x18))
#define Input_sc_y_e                    ((Input_scancode_t) (0x19))
#define Input_sc_g_e                    ((Input_scancode_t) (0x1a))
#define Input_sc_8_e                    ((Input_scancode_t) (0x1b))
#define Input_sc_b_e                    ((Input_scancode_t) (0x1c))
#define Input_sc_h_e                    ((Input_scancode_t) (0x1d))
#define Input_sc_u_e                    ((Input_scancode_t) (0x1e))
#define Input_sc_v_e                    ((Input_scancode_t) (0x1f))

#define Input_sc_9_e                    ((Input_scancode_t) (0x20))
#define Input_sc_i_e                    ((Input_scancode_t) (0x21))
#define Input_sc_j_e                    ((Input_scancode_t) (0x22))
#define Input_sc_0_e                    ((Input_scancode_t) (0x23))
#define Input_sc_m_e                    ((Input_scancode_t) (0x24))
#define Input_sc_k_e                    ((Input_scancode_t) (0x25))
#define Input_sc_o_e                    ((Input_scancode_t) (0x26))
#define Input_sc_n_e                    ((Input_scancode_t) (0x27))

#define Input_sc_plus_e                 ((Input_scancode_t) (0x28))
#define Input_sc_p_e                    ((Input_scancode_t) (0x29))
#define Input_sc_l_e                    ((Input_scancode_t) (0x2a))
#define Input_sc_minus_e                ((Input_scancode_t) (0x2b))
#define Input_sc_dot_e                  ((Input_scancode_t) (0x2c))
#define Input_sc_colon_e                ((Input_scancode_t) (0x2d))
#define Input_sc_atsign_e               ((Input_scancode_t) (0x2e))
#define Input_sc_comma_e                ((Input_scancode_t) (0x2f))

#define Input_sc_pound_e                ((Input_scancode_t) (0x30))
#define Input_sc_asterisk_e             ((Input_scancode_t) (0x31))
#define Input_sc_semicolon_e            ((Input_scancode_t) (0x32))
#define Input_sc_homeclr_e              ((Input_scancode_t) (0x33))
#define Input_sc_rshift_e               ((Input_scancode_t) (0x34))
#define Input_sc_equal_e                ((Input_scancode_t) (0x35))
#define Input_sc_uparrow_e              ((Input_scancode_t) (0x36))
#define Input_sc_slash_e                ((Input_scancode_t) (0x37))

#define Input_sc_1_e                    ((Input_scancode_t) (0x38))
#define Input_sc_leftarrow_e            ((Input_scancode_t) (0x39))
#define Input_sc_ctrl_e                 ((Input_scancode_t) (0x3a))
#define Input_sc_2_e                    ((Input_scancode_t) (0x3b))
#define Input_sc_space_e                ((Input_scancode_t) (0x3c))
#define Input_sc_commodore_e            ((Input_scancode_t) (0x3d))
#define Input_sc_q_e                    ((Input_scancode_t) (0x3e))
#define Input_sc_stoprun_e              ((Input_scancode_t) (0x3f))

/* Scanned key codes from keyboard (0x00-0x40).  */
typedef uint8_t Input_scancode_t;

/* ***************************************************************  */

#define INPUT_KEYBOARD_PRESSED_SIZE     3

/* Information the keyboard  */
typedef struct Input_keyboard_t {

  /* Scan codes of the keys which are currently pressed.  The order in
   * this buffer is the same as the ORDER THEY WERE PRESSED DOWN.
   *
   * Therefore if your game does just support one simultaneously
   * pressed key then you just need to check the first item
   * INPUT.KEYBOARD.PRESSED[0] in this buffer.
   *
   * If currently no key was pressed down then the corresponding
   * position in this buffer is containing INPUT_SC_NONE_E (scan code
   * 0x40).
   */
  Input_scancode_t pressed[INPUT_KEYBOARD_PRESSED_SIZE];

  /* TRUE for exactly 1 tick, if PRESSED has changed it´s value.  */
  bool changed;

} Input_keyboard_t;

/* ***************************************************************  */

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

  /* The keyboard ^^  */
  Input_keyboard_t keyboard;

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

/* *******************************************************************
 * private
 */

extern Input_scancode_t __fastcall__ _Input_keyboard_scan(void);

/* ***************************************************************  */

#endif /* INPUT_H__  */
