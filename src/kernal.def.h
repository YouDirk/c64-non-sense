/* C64 NonSense, the C64 Game Engine.
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


#ifndef KERNAL_DEF_H__
#define KERNAL_DEF_H__

#include "define.h"

header_ifndef(KERNAL)
header_define(KERNAL)

/* *******************************************************************
 * vectors (IRQ/BRK, NMI, RESET)
 */

register_void(0314,           KERNAL_IRQ_USERENTRY)
register_void(ea31,           KERNAL_IRQ_DEFAULT)
register_void(ea81,           KERNAL_IRQ_RETURN)

register_void(0316,           KERNAL_BRK_USERENTRY)
register_void(fe66,           KERNAL_BRK_DEFAULT)
define(KERNAL_BRK_RETURN,     KERNAL_IRQ_RETURN)

register_void(0318,           KERNAL_NMI_USERENTRY)
register_void(fe47,           KERNAL_NMI_DEFAULT)
define(KERNAL_NMI_RETURN,     KERNAL_IRQ_RETURN)

register_void(fffe,           KERNAL_HARDIRQ_VECTOR)
register_void(ff48,           KERNAL_HARDIRQ_DEFAULT)

register_void(fffa,           KERNAL_HARDNMI_VECTOR)
register_void(fe43,           KERNAL_HARDNMI_DEFAULT)

register_void(fffc,           KERNAL_HARDRESET_VECTOR)
register_void(fce2,           KERNAL_HARDRESET_DEFAULT)

/* end of vectors
 * *******************************************************************
 * zero-page (0x0000 - 0x00ff)
 */

/* Will be set during keyboard scan routine in Kernal code at 0xeb28.
 *
 * We are setting it ourself during INPUT_KEYBOARD_PETSCII_CONVERT()
 * if INPUT_KEYBOARD_SCAN_PETSCII_MASK is set in INPUT.SET.ENABLED.
 *
 * Our implementation may differ from the Kernals one.  Therefore, it
 * is recommended to not use it.
 */
register_uint8(c5,            KERNAL_ZP_KEYBOARD_SCANCODE_PREV)

/* Will be set during keyboard scan routine in Kernal code at 0xeac9.
 *
 * We are setting it ourself during INPUT_KEYBOARD_PETSCII_CONVERT()
 * if INPUT_KEYBOARD_SCAN_PETSCII_MASK is set in INPUT.SET.ENABLED.
 */
register_uint8(cb,            KERNAL_ZP_KEYBOARD_SCANCODE)

/* Will be set during keyboard scan routine in Kernal code at 0xea9b.
 * Therefore, if you are using the argument `-autostart` in VICE it
 * will be never be set!
 *
 * We are setting it ourself during INPUT_KEYBOARD_PETSCII_CONVERT()
 * if INPUT_KEYBOARD_SCAN_PETSCII_MASK is set in INPUT.SET.ENABLED.
 */
register_constchar_ptr(f5,    KERNAL_ZP_PETSCIITABLE_VECTOR)

/* end of zero-page
 * *******************************************************************
 * advanced zero-page (0x0100 - 0x03ff)
 */

/* Will NOT BE SET, use INPUT.KEYBOARD.PETSCII.SHIFTKEYS instead.  It
 * is using the same bit layout.
 */
register_uint8(028d,          KERNAL_AZP_SHIFTFLAGS)

/* Will be set during screen editor init routine in Kernal code at
 * 0xff5b.
 *
 * We are using our own implementation with a more detailed
 * information about the VIC-II chip in GRAPHIX.VIC_REVISION.  Use
 * GRAPHIX.IS_PAL to just differ between PAL and NTSC.
 */
register_uint8(02a6,          KERNAL_AZP_ISPAL)

/* end of advanced zero-page
 * *******************************************************************
 * Kernal ROM (0xe000 - 0xfffa) [w/o MOS-6510 RESET/NMI/IRQ]
 */

register_constchar_ptr(eb79,  KERNAL_PETSCIITABLE_VECTORS)
register_constchar_ptr(eb79,  KERNAL_PETSCIITABLE_VECTOR_UNSHIFTED)
register_constchar_ptr(eb7b,  KERNAL_PETSCIITABLE_VECTOR_SHIFTED)
register_constchar_ptr(eb7d,  KERNAL_PETSCIITABLE_VECTOR_COMMODORE)
register_constchar_ptr(eb7f,  KERNAL_PETSCIITABLE_VECTOR_CTRL)
register_constchar(eb81,      KERNAL_PETSCIITABLE)

/* end of Kernal ROM
 * **************************************************************** */

header_endif(KERNAL)

#endif /* KERNAL_DEF_H__  */
