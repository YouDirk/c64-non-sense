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


#ifndef INPUT_H__
#define INPUT_H__

#include "common.h"
#include "Input.gen.h"

/* ***************************************************************  */

/* Select and initialize input devices, such like joysticks, keyboard,
 * for polling and ticking.
 */
extern void __fastcall__ Input_init(void);

/* Restore input configuration.  */
extern void __fastcall__ Input_release(void);

/* ***************************************************************  */

/* Check the enabled input devices for state changes.  */
extern void __fastcall__ Input_tick(void);

/* *******************************************************************
 * private
 */

extern void __fastcall__ _Input_keyboard_scan(void);
extern void __fastcall__ _Input_keyboard_petscii_convert(void);

/* ***************************************************************  */

#endif /* INPUT_H__  */
