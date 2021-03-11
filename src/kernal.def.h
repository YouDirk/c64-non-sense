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


#ifndef KERNAL_DEF_H__
#define KERNAL_DEF_H__

#include "define.h"

header_ifndef(KERNAL)
header_define(KERNAL)

/* *******************************************************************
 * vectors (IRQ/BRK, NMI, RESET)
 */

define_hex(KERNAL_IRQ_USERENTRY,                  0314)
define_hex(KERNAL_IRQ_DEFAULT,                    ea31)
define_hex(KERNAL_IRQ_RETURN,                     ea81)

define_hex(KERNAL_BRK_USERENTRY,                  0316)
define_hex(KERNAL_BRK_DEFAULT,                    fe66)
define    (KERNAL_BRK_RETURN,                     KERNAL_IRQ_RETURN)

define_hex(KERNAL_NMI_USERENTRY,                  0318)
define_hex(KERNAL_NMI_DEFAULT,                    fe47)
define    (KERNAL_NMI_RETURN,                     KERNAL_IRQ_RETURN)

define_hex(KERNAL_HARDIRQ_VECTOR,                 fffe)
define_hex(KERNAL_HARDIRQ_DEFAULT,                ff48)

define_hex(KERNAL_HARDNMI_VECTOR,                 fffa)
define_hex(KERNAL_HARDNMI_DEFAULT,                fe43)

define_hex(KERNAL_HARDRESET_VECTOR,               fffc)
define_hex(KERNAL_HARDRESET_DEFAULT,              fce2)

/*
 * end of vectors
 * *******************************************************************
 * zero-page (0x0000 - 0x00ff)
 */

define_hex(KERNAL_ZP_PETSCIITABLE_VECTOR,         f5)

/*
 * end of zero-page
 * *******************************************************************
 * advanced zero-page (0x0100 - 0x03ff)
 */

define_hex(KERNAL_AZP_SHIFTFLAGS,                 028d)

/*
 * end of advanced zero-page
 * ***************************************************************  */

define_hex(KERNAL_FLAG_ISPAL,                     02a6)

header_endif(KERNAL)

#endif /* KERNAL_DEF_H__  */
