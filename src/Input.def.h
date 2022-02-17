/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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
  /* Disables all input devices below.  */
  typedef_enum_hex(Input_devices_t, 00,             Input_none_mask)

  /* Enables the DEFAULT joystick at port 2 (CIA1 port A).  State
   * available at
   *
   *   * INPUT.JOY_PORT2
   */
  typedef_enum_hex(Input_devices_t, 01,   Input_joystick_port2_mask)

  /* Enables joystick at port 1 (CIA1 port B).  State available at
   *
   *   * INPUT.JOY_PORT1
   *
   * ATTENTION: Make sure to have INPUT_KEYBOARD_SCAN_MASK disabled,
   *            if joystick port 1 is in use.
   *
   * REASON: The keyboard scan routine selects CIA1 port A and reads
   *         CIA1 port B.  Because both, keyboard and joystick port 1,
   *         are hard wired to CIA1 port B, it exist no way to disable
   *         the joystick port 1 signal during keyboard scan.  As
   *         result the following scan codes will be detected during
   *         keyboard scan if there is an input at the same time on
   *         joystick port1:
   *
   *              * Up   : DEL    , 3, 5, 7 | 9, +, POUND , 1
   *                Down : RETURN , W, R, Y | I, P, *     , LEFT
   *                Left : CRSR RT, A, D, G | J, L, ;     , CTRL
   *                Right: F7     , 4, 6, 8 | 0, -, HOME  , 2
   *                Btn1 : F1     , Z, C, B | M, ., RSHIFT, SPACE
   *   SCPRESSED_MAXCOUNT -------4--------->|
   *
   * Therefore it is highly recommended NOT USE KEYBOARD AND
   * JOYSTICK_PORT1 at the same time together!
   *
   * Otherwise you need to take a detailed look to the "C64 keyboard
   * matrix" and INPUT_KEYBOARD_SCPRESSED_MAXCOUNT below to extract
   * which keys are possible to read concurrency to joystick port 1.
   */
  typedef_enum_hex(Input_devices_t, 02,   Input_joystick_port1_mask)

  /* Enables both joysticks above.  */
  typedef_enum_hex(Input_devices_t, 03,     Input_joystick_all_mask)

  /* Enables the scan (for reading SCAN CODES) of keybroard.  It coast
   * much CPU time.  Scanned codes are available at
   *
   *   * INPUT.KEYBOARD.SC_PRESSED[]
   *
   *   * via INPUT_SC_{keyname}_E below.
   *
   * Use this if you need key input during the game, such like WASD
   * control.
   *
   * ATTENTION: Make sure there is not a keyboard scan enabled during
   *            joystick port1 is in use!  For more details, see
   *            INPUT_DEVICE_T::INPUT_JOYSTICK_PORT1_MAST above.
   */
  typedef_enum_hex(Input_devices_t, 04,    Input_keyboard_scan_mask)

  /* Requires that INPUT_KEYBOARD_SCAN_MASK above is set too.  Enables
   * reading PETSCII codes, correct translated for output.  One per
   * engine tick via
   *
   *   * INPUT.KEYBOARD.PETSCII.CHARACTER
   *
   * Use this if you require to input a string from the user, such
   * like a player name.
   */
  typedef_enum_hex(Input_devices_t, 08, Input_keyboard_petscii_mask)

  /* Enables full keyboard support: scan codes + PETSCII codes */
  typedef_enum_hex(Input_devices_t, 0c,     Input_keyboard_all_mask)

  /* Enables all supported input devices above.  */
  typedef_enum_hex(Input_devices_t, ff,              Input_all_mask)
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
  typedef_enum_hex(Input_scancode_t, 40,          Input_sc_none_e)

  typedef_enum_hex(Input_scancode_t, 00,          Input_sc_delinst_e)
  typedef_enum_hex(Input_scancode_t, 01,          Input_sc_return_e)
  typedef_enum_hex(Input_scancode_t, 02,          Input_sc_crsrright_e)
  typedef_enum_hex(Input_scancode_t, 03,          Input_sc_f7_e)
  typedef_enum_hex(Input_scancode_t, 04,          Input_sc_f1_e)
  typedef_enum_hex(Input_scancode_t, 05,          Input_sc_f3_e)
  typedef_enum_hex(Input_scancode_t, 06,          Input_sc_f5_e)
  typedef_enum_hex(Input_scancode_t, 07,          Input_sc_crsrdown_e)

  typedef_enum_hex(Input_scancode_t, 08,          Input_sc_3_e)
  typedef_enum_hex(Input_scancode_t, 09,          Input_sc_w_e)
  typedef_enum_hex(Input_scancode_t, 0a,          Input_sc_a_e)
  typedef_enum_hex(Input_scancode_t, 0b,          Input_sc_4_e)
  typedef_enum_hex(Input_scancode_t, 0c,          Input_sc_z_e)
  typedef_enum_hex(Input_scancode_t, 0d,          Input_sc_s_e)
  typedef_enum_hex(Input_scancode_t, 0e,          Input_sc_e_e)
  typedef_enum_hex(Input_scancode_t, 0f,          Input_sc_lshift_e)

  typedef_enum_hex(Input_scancode_t, 10,          Input_sc_5_e)
  typedef_enum_hex(Input_scancode_t, 11,          Input_sc_r_e)
  typedef_enum_hex(Input_scancode_t, 12,          Input_sc_d_e)
  typedef_enum_hex(Input_scancode_t, 13,          Input_sc_6_e)
  typedef_enum_hex(Input_scancode_t, 14,          Input_sc_c_e)
  typedef_enum_hex(Input_scancode_t, 15,          Input_sc_f_e)
  typedef_enum_hex(Input_scancode_t, 16,          Input_sc_t_e)
  typedef_enum_hex(Input_scancode_t, 17,          Input_sc_x_e)

  typedef_enum_hex(Input_scancode_t, 18,          Input_sc_7_e)
  typedef_enum_hex(Input_scancode_t, 19,          Input_sc_y_e)
  typedef_enum_hex(Input_scancode_t, 1a,          Input_sc_g_e)
  typedef_enum_hex(Input_scancode_t, 1b,          Input_sc_8_e)
  typedef_enum_hex(Input_scancode_t, 1c,          Input_sc_b_e)
  typedef_enum_hex(Input_scancode_t, 1d,          Input_sc_h_e)
  typedef_enum_hex(Input_scancode_t, 1e,          Input_sc_u_e)
  typedef_enum_hex(Input_scancode_t, 1f,          Input_sc_v_e)

  typedef_enum_hex(Input_scancode_t, 20,          Input_sc_9_e)
  typedef_enum_hex(Input_scancode_t, 21,          Input_sc_i_e)
  typedef_enum_hex(Input_scancode_t, 22,          Input_sc_j_e)
  typedef_enum_hex(Input_scancode_t, 23,          Input_sc_0_e)
  typedef_enum_hex(Input_scancode_t, 24,          Input_sc_m_e)
  typedef_enum_hex(Input_scancode_t, 25,          Input_sc_k_e)
  typedef_enum_hex(Input_scancode_t, 26,          Input_sc_o_e)
  typedef_enum_hex(Input_scancode_t, 27,          Input_sc_n_e)

  typedef_enum_hex(Input_scancode_t, 28,          Input_sc_plus_e)
  typedef_enum_hex(Input_scancode_t, 29,          Input_sc_p_e)
  typedef_enum_hex(Input_scancode_t, 2a,          Input_sc_l_e)
  typedef_enum_hex(Input_scancode_t, 2b,          Input_sc_minus_e)
  typedef_enum_hex(Input_scancode_t, 2c,          Input_sc_dot_e)
  typedef_enum_hex(Input_scancode_t, 2d,          Input_sc_colon_e)
  typedef_enum_hex(Input_scancode_t, 2e,          Input_sc_atsign_e)
  typedef_enum_hex(Input_scancode_t, 2f,          Input_sc_comma_e)

  typedef_enum_hex(Input_scancode_t, 30,          Input_sc_pound_e)
  typedef_enum_hex(Input_scancode_t, 31,          Input_sc_asterisk_e)
  typedef_enum_hex(Input_scancode_t, 32,          Input_sc_semicolon_e)
  typedef_enum_hex(Input_scancode_t, 33,          Input_sc_homeclr_e)
  typedef_enum_hex(Input_scancode_t, 34,          Input_sc_rshift_e)
  typedef_enum_hex(Input_scancode_t, 35,          Input_sc_equal_e)
  typedef_enum_hex(Input_scancode_t, 36,          Input_sc_uparrow_e)
  typedef_enum_hex(Input_scancode_t, 37,          Input_sc_slash_e)

  typedef_enum_hex(Input_scancode_t, 38,          Input_sc_1_e)
  typedef_enum_hex(Input_scancode_t, 39,          Input_sc_leftarrow_e)
  typedef_enum_hex(Input_scancode_t, 3a,          Input_sc_ctrl_e)
  typedef_enum_hex(Input_scancode_t, 3b,          Input_sc_2_e)
  typedef_enum_hex(Input_scancode_t, 3c,          Input_sc_space_e)
  typedef_enum_hex(Input_scancode_t, 3d,          Input_sc_commodore_e)
  typedef_enum_hex(Input_scancode_t, 3e,          Input_sc_q_e)
  typedef_enum_hex(Input_scancode_t, 3f,          Input_sc_stoprun_e)
typedef_enum_end(Input_scancode_t)

/* ***************************************************************  */

/* Shift-key flags, compatible with zero-page Kernal variable 0x028d.
 */
typedef_enum_begin(Input_shiftkeys_t)
  typedef_enum_hex(Input_shiftkeys_t, 01,       Input_sk_shift_mask)
  typedef_enum_hex(Input_shiftkeys_t, 02,   Input_sk_commodore_mask)
  typedef_enum_hex(Input_shiftkeys_t, 04,        Input_sk_ctrl_mask)
typedef_enum_end(Input_scancode_t)

/* ***************************************************************  */

/* State of an axis.  */
typedef_struct_begin(Input_axis_t)
  /* 1 or -1 depending on push direction.
   *
   *   *  1 if UP   or LEFT  is pressed
   *   * -1 if DOWN or RIGHT is pressed
   *   *  0 otherwise
   */
  typedef_struct_int8(                                 direction)

  /* TRUE for exactly 1 tick, if DIRECTION has changed it´s value.  */
  typedef_struct_bool(                                 changed)
typedef_struct_end(Input_axis_t)

/* Information of a button  */
typedef_struct_begin(Input_button_t)
  /* TRUE if button is pressed.  */
  typedef_struct_bool(                                 pressed)

  /* TRUE for exactly 1 tick, if PRESSED has changed it´s value.  */
  typedef_struct_bool(                                 changed)
typedef_struct_end(Input_button_t)

/* State of an joystick.  */
typedef_struct_begin(Input_joystick_t)
  /* State of the Y-axis.
   *
   *   *  1 if UP   is pressed
   *   * -1 if DOWN is pressed
   *   *  0 otherwise
   */
  typedef_struct_nested(Input_axis_t,                  axis_y)
  /* State of the X-axis.
   *
   *   *  1 if LEFT  is pressed
   *   * -1 if RIGHT is pressed
   *   *  0 otherwise
   */
  typedef_struct_nested(Input_axis_t,                  axis_x)

  /* State of the fire button.  */
  typedef_struct_nested(Input_button_t,                button1)
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

/* Maximal possible items in INPUT.KEYBOARD.SC_PRESSED  */
define_dec(INPUT_KEYBOARD_SCPRESSED_MAXCOUNT,                    4)

/* Real length of INPUT.KEYBOARD.SC_PRESSED buffer, including 0x40
 * (INPUT_SC_NONE_E) termination.
 */
define_dec(INPUT_KEYBOARD_SCPRESSED_BUFSIZE,                         \
                                 INPUT_KEYBOARD_SCPRESSED_MAXCOUNT+1)

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
   * INPUT.KEYBOARD.SC_PRESSED is global static and SIZEOF this
   * variable is 1.  Otherwise use a loop such like this instead:
   *
   * ```C
   * void __fastcall__
   * MyModule_tick(void)
   * {
   *   Input_scancode_t* cur_key;
   *
   *   for (cur_key=Input.keyboard.sc_pressed;
   *        *cur_key != Input_sc_none_e; ++cur_key) {
   *     do_something(*cur_key);
   *   }
   * }
   * ```
   */
  typedef_struct_enum_array(Input_scancode_t,                        \
                         sc_pressed, INPUT_KEYBOARD_SCPRESSED_BUFSIZE)

  /* The number of items in INPUT.KEYBOARD.SC_PRESSED.  For iteration
   * do not use an index variable.  See comment of
   * INPUT.KEYBOARD.SC_PRESSED above.
   */
  typedef_struct_uint8(sc_pressed_count)

  /* TRUE for exactly 1 tick, if SC_PRESSED has changed it´s value.
   */
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

/* ***************************************************************  */

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

  /* Joystick at port 1 does not work simultaneously together with the
   * keyboard.  Therefore, if the keyboard status changed then make
   * sure that there is no input from joystick at port 1.
   *
   * For recommended code if INPUT.SET.ENABLED =
   * Input_joystick_port1_mask | Input_keyboard_*_mask is set together
   *
   *   * take a look to INPUT_T::INPUT_SET_T::KEYBOARD
   */
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
   *   if (Input.keyboard.changed
   *
   *       // needed, if INPUT.SET.ENABLED = Input_joystick_port1_mask
   *       && Input.joy_port1.axis_x.direction == 0
   *       && Input.joy_port1.axis_y.direction == 0
   *
   *       ) {
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
