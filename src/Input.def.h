/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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


#ifndef INPUT_DEF_H__
#define INPUT_DEF_H__

#include "define.h"

header_ifndef(INPUT)
header_define(INPUT)

/* ***************************************************************  */

/* Selectors for one or more specific input devices.  */
typedef_enum_begin(Input_devices_t)
  typedef_enum_hex(Input_devices_t, Input_none_mask,             00)
  typedef_enum_hex(Input_devices_t, Input_joystick_port2_mask,   01)
  typedef_enum_hex(Input_devices_t, Input_joystick_port1_mask,   02)
  typedef_enum_hex(Input_devices_t, Input_joystick_all_mask,     03)
  typedef_enum_hex(Input_devices_t, Input_keyboard_scan_mask,    04)
  typedef_enum_hex(Input_devices_t, \
                              Input_keyboard_scan_petscii_mask,  08)
  typedef_enum_hex(Input_devices_t, Input_keyboard_all_mask,     0c)
  typedef_enum_hex(Input_devices_t, Input_all_mask,              ff)
typedef_enum_end(Input_devices_t)

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
 *
 *
 * --- defined in Input.def.h:
 * typedef uint8_t Input_scancode_t;
 *
 * ***************************************************************  */

/* Scanned key codes from keyboard (0x00-0x40).  */
typedef_enum_begin(Input_scancode_t)
  typedef_enum_hex(Input_scancode_t, Input_sc_none_e,            40)

  typedef_enum_hex(Input_scancode_t, Input_sc_delinst_e,         00)
  typedef_enum_hex(Input_scancode_t, Input_sc_return_e,          01)
  typedef_enum_hex(Input_scancode_t, Input_sc_crsrright_e,       02)
  typedef_enum_hex(Input_scancode_t, Input_sc_f7_e,              03)
  typedef_enum_hex(Input_scancode_t, Input_sc_f1_e,              04)
  typedef_enum_hex(Input_scancode_t, Input_sc_f3_e,              05)
  typedef_enum_hex(Input_scancode_t, Input_sc_f5_e,              06)
  typedef_enum_hex(Input_scancode_t, Input_sc_crsrdown_e,        07)

  typedef_enum_hex(Input_scancode_t, Input_sc_3_e,               08)
  typedef_enum_hex(Input_scancode_t, Input_sc_w_e,               09)
  typedef_enum_hex(Input_scancode_t, Input_sc_a_e,               0a)
  typedef_enum_hex(Input_scancode_t, Input_sc_4_e,               0b)
  typedef_enum_hex(Input_scancode_t, Input_sc_z_e,               0c)
  typedef_enum_hex(Input_scancode_t, Input_sc_s_e,               0d)
  typedef_enum_hex(Input_scancode_t, Input_sc_e_e,               0e)
  typedef_enum_hex(Input_scancode_t, Input_sc_lshift_e,          0f)

  typedef_enum_hex(Input_scancode_t, Input_sc_5_e,               10)
  typedef_enum_hex(Input_scancode_t, Input_sc_r_e,               11)
  typedef_enum_hex(Input_scancode_t, Input_sc_d_e,               12)
  typedef_enum_hex(Input_scancode_t, Input_sc_6_e,               13)
  typedef_enum_hex(Input_scancode_t, Input_sc_c_e,               14)
  typedef_enum_hex(Input_scancode_t, Input_sc_f_e,               15)
  typedef_enum_hex(Input_scancode_t, Input_sc_t_e,               16)
  typedef_enum_hex(Input_scancode_t, Input_sc_x_e,               17)

  typedef_enum_hex(Input_scancode_t, Input_sc_7_e,               18)
  typedef_enum_hex(Input_scancode_t, Input_sc_y_e,               19)
  typedef_enum_hex(Input_scancode_t, Input_sc_g_e,               1a)
  typedef_enum_hex(Input_scancode_t, Input_sc_8_e,               1b)
  typedef_enum_hex(Input_scancode_t, Input_sc_b_e,               1c)
  typedef_enum_hex(Input_scancode_t, Input_sc_h_e,               1d)
  typedef_enum_hex(Input_scancode_t, Input_sc_u_e,               1e)
  typedef_enum_hex(Input_scancode_t, Input_sc_v_e,               1f)

  typedef_enum_hex(Input_scancode_t, Input_sc_9_e,               20)
  typedef_enum_hex(Input_scancode_t, Input_sc_i_e,               21)
  typedef_enum_hex(Input_scancode_t, Input_sc_j_e,               22)
  typedef_enum_hex(Input_scancode_t, Input_sc_0_e,               23)
  typedef_enum_hex(Input_scancode_t, Input_sc_m_e,               24)
  typedef_enum_hex(Input_scancode_t, Input_sc_k_e,               25)
  typedef_enum_hex(Input_scancode_t, Input_sc_o_e,               26)
  typedef_enum_hex(Input_scancode_t, Input_sc_n_e,               27)

  typedef_enum_hex(Input_scancode_t, Input_sc_plus_e,            28)
  typedef_enum_hex(Input_scancode_t, Input_sc_p_e,               29)
  typedef_enum_hex(Input_scancode_t, Input_sc_l_e,               2a)
  typedef_enum_hex(Input_scancode_t, Input_sc_minus_e,           2b)
  typedef_enum_hex(Input_scancode_t, Input_sc_dot_e,             2c)
  typedef_enum_hex(Input_scancode_t, Input_sc_colon_e,           2d)
  typedef_enum_hex(Input_scancode_t, Input_sc_atsign_e,          2e)
  typedef_enum_hex(Input_scancode_t, Input_sc_comma_e,           2f)

  typedef_enum_hex(Input_scancode_t, Input_sc_pound_e,           30)
  typedef_enum_hex(Input_scancode_t, Input_sc_asterisk_e,        31)
  typedef_enum_hex(Input_scancode_t, Input_sc_semicolon_e,       32)
  typedef_enum_hex(Input_scancode_t, Input_sc_homeclr_e,         33)
  typedef_enum_hex(Input_scancode_t, Input_sc_rshift_e,          34)
  typedef_enum_hex(Input_scancode_t, Input_sc_equal_e,           35)
  typedef_enum_hex(Input_scancode_t, Input_sc_uparrow_e,         36)
  typedef_enum_hex(Input_scancode_t, Input_sc_slash_e,           37)

  typedef_enum_hex(Input_scancode_t, Input_sc_1_e,               38)
  typedef_enum_hex(Input_scancode_t, Input_sc_leftarrow_e,       39)
  typedef_enum_hex(Input_scancode_t, Input_sc_ctrl_e,            3a)
  typedef_enum_hex(Input_scancode_t, Input_sc_2_e,               3b)
  typedef_enum_hex(Input_scancode_t, Input_sc_space_e,           3c)
  typedef_enum_hex(Input_scancode_t, Input_sc_commodore_e,       3d)
  typedef_enum_hex(Input_scancode_t, Input_sc_q_e,               3e)
  typedef_enum_hex(Input_scancode_t, Input_sc_stoprun_e,         3f)
typedef_enum_end(Input_scancode_t)

/* ***************************************************************  */

/* Shift-key flags, compatible with zero-page Kernal variable 0x028d.
 */
typedef_enum_begin(Input_shiftkeys_t)
  typedef_enum_hex(Input_shiftkeys_t, Input_sk_shift_mask,       01)
  typedef_enum_hex(Input_shiftkeys_t, Input_sk_commodore_mask,   02)
  typedef_enum_hex(Input_shiftkeys_t, Input_sk_ctrl_mask,        04)
typedef_enum_end(Input_scancode_t)

/* ***************************************************************  */

/* Information of an axis  */
typedef_struct_begin(Input_axis_t)
  /* 1 or -1 depending on push direction during this engine tick, if
   * axis is pressed .
   *
   * Assert: Must have SIZEOF 1 and be first member in this struct!
   */
  typedef_struct_int8(direction)

  /* TRUE for exactly 1 tick, if DIRECTION has changed it´s value.  */
  typedef_struct_bool(changed)
typedef_struct_end(Input_axis_t)

/* Information of a button  */
typedef_struct_begin(Input_button_t)
  /* TRUE if button is pressed during this engine tick.
   *
   * Assert: Must have SIZEOF 1 and be first member in this struct!
   */
  typedef_struct_bool(pressed)

  /* TRUE for exactly 1 tick, if PRESSED has changed it´s value.  */
  typedef_struct_bool(changed)
typedef_struct_end(Input_button_t)

/* Information about a joystick  */
typedef_struct_begin(Input_joystick_t)
  /* State of the axes.  */
  typedef_struct_nested(Input_axis_t, axis_y)
  typedef_struct_nested(Input_axis_t, axis_x)

  /* State of the fire button.  */
  typedef_struct_nested(Input_button_t, button1)
typedef_struct_end(Input_joystick_t)

/* ***************************************************************  */

/* If PETSCII enabled: The current scan codes converted to the
 * equivalent PETSCII character.
 */
typedef_struct_begin(Input_keyboard_petscii_t)
  /* Shift-key flags, compatible with zero-page Kernal variable
   * 0x028d.
   */
  typedef_struct_enum(Input_shiftkeys_t,               shiftkeys)

  /* The PETSCII character code which is able to used for C strings of
   * type CHAR*.
   *
   * If no control character or visible character was pressed then it
   * will be set to null-terminator char '\0'.
   */
  typedef_struct_char(character)

  /* TRUE for exactly 1 tick, if CHARACTER has changed it´s value.  */
  typedef_struct_bool(changed)

typedef_struct_end(Input_keyboard_petscii_t)

/* ***************************************************************  */

/* Maximal possible items in INPUT.KEYBOARD.PRESSED  */
define_dec(INPUT_KEYBOARD_PRESSED_MAXCOUNT,                      4)

/* Real length of INPUT.KEYBOARD.PRESSED buffer, including 0x40
 * (INPUT_SC_NONE_E) termination.
 */
define_dec(INPUT_KEYBOARD_PRESSED_BUFSIZE,                           \
                                 INPUT_KEYBOARD_PRESSED_MAXCOUNT+1)

/* Information about the keyboard  */
typedef_struct_begin(Input_keyboard_t)
  /* Scan codes of the keys which are currently pressed.  The order in
   * this buffer is the same as they were scanned from the keyboard
   * matrix.
   *
   * This buffer will be terminated with 0x40 (INPUT_SC_NONE_E), as it
   * is commonly known from null-termination of C strings.
   *
   * It´s possible to use an index variable in a loop, because
   * INPUT.KEYBOARD.PRESSED is global static and SIZEOF this variable
   * is 1.  Otherwise use a loop such like this instead:
   *
   * ```C
   * void __fastcall__
   * MyModule_tick(void)
   * {
   *   Input_scancode_t* cur_key;
   *
   *   for (cur_key=Input.keyboard.pressed;
   *        *cur_key != Input_sc_none_e; ++cur_key) {
   *     do_something(*cur_key);
   *   }
   * }
   * ```
   */
  typedef_struct_enum_array(Input_scancode_t,                        \
                            pressed, INPUT_KEYBOARD_PRESSED_BUFSIZE)

  /* The number of items in INPUT.KEYBOARD.PRESSED.  For iteration do
   * not use an index variable.  See comment of INPUT.KEYBOARD.PRESSED
   * above.
   */
  typedef_struct_uint8(pressed_count)

  /* TRUE for exactly 1 tick, if PRESSED has changed it´s value.  */
  typedef_struct_bool(changed)

  /* Just filled if PETSCII enabled, using in INPUT.SET.ENABLED the
   * bit-mask INPUT_KEYBOARD_SCAN_PETSCII_MASK.
   */
  typedef_struct_nested(Input_keyboard_petscii_t,      petscii)
typedef_struct_end(Input_keyboard_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Input_set_t)
  /* Input devices which will be polled and are ticking in this
   * module.
   */
  typedef_struct_enum(Input_devices_t,                 enabled)
typedef_struct_end(Input_set_t)

/* Structure of static members for module.  */
typedef_struct_begin(Input_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Input_set_t,                   set)

  /* The two joysticks on physical Port 2 and Port 1 :)
   *
   * Recommended usage
   * ```
   * void __fastcall__
   * MyModule_tick(void)
   * {
   *   if (Input.joy_port2.axis_y.changed) {
   *     do_something_with(Input.joy_port2.axis_y.direction);
   *   }
   * }
   * ```
   */
  typedef_struct_nested(Input_joystick_t,              joy_port2)
  typedef_struct_nested(Input_joystick_t,              joy_port1)

  /* The keyboard ^^
   *
   * Recommended using SCAN_CODES with at least INPUT_KEYBOARD_SCAN_MASK
   * ```
   * void __fastcall__
   * MyModule_tick(void)
   * {
   *   static uint8_t i;
   *   static bool key_w, key_s, key_a, key_d;
   *
   *   if (Input.keyboard.changed) {
   *     key_w=false, key_s=false, key_a=false, key_d=false;
   *     for (i=0; i<Input.keyboard.pressed_count; ++i) {
   *       switch (Input.keyboard.pressed[i]) {
   *       case Input_sc_w_e: key_w = true; break;
   *       case Input_sc_s_e: key_s = true; break;
   *       case Input_sc_a_e: key_a = true; break;
   *       case Input_sc_d_e: key_d = true; break;
   *       default: break;
   *       }
   *     }
   *
   *     if (key_w) do_something_foward();
   *     else if (key_s) do_something_backward();
   *     else do_something_stop_vertical();
   *
   *     if (key_a) do_something_leftward();
   *     else if (key_d) do_something_rightward();
   *     else do_something_stop_horizontal();
   *   }
   * }
   * ```
   *
   * Recommended using PETSCII characters with at least
   * INPUT_KEYBOARD_SCAN_PETSCII_MASK
   * ```
   * AASandbox_tick(void)
   * {
   *   if (Input.keyboard.petscii.changed
   *       && Input.keyboard.petscii.character != '\0') {
   *     do_something_with(Input.keyboard.petscii.character);
   *   }
   * }
   * ```
   */
  typedef_struct_nested(Input_keyboard_t,              keyboard)
typedef_struct_end(Input_t)

/* ***************************************************************  */

/* Static members of this module.  */
extern_var(Input_t,                                    Input)

/* ***************************************************************  */

header_endif(INPUT)

#endif /* INPUT_DEF_H__  */
