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


#ifndef MEMORY_DEF_H__
#define MEMORY_DEF_H__

#include "define.h"

header_ifndef(MEMORY)
header_define(MEMORY)

/* *******************************************************************
 *
 *         : Pre-initialzed        | Possible memory banks :
 *         : **************        | ********************* :
 *         :                       |                       :
 *         .-----------------------------------------------.
 * 0x0000  | [CPU] MOS 6510, port access (pin P0-P5)       |
 *         .-----------------------------------------------.
 * 0x0002  | [RAM, Kernal] Zero Page                       |
 *         .-----------------------------------------------.
 * 0x0100  | [RAM, Kernal] Extended Zero Page              |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0x0400  | [RAM, VIC-II DMA] default Screen RAM          |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0x0800  | [RAM, BASIC] BASIC RAM                        |
 *         |   0x0801: entry point of C64 NonSense         |
 *         |                                               |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0xa000  | [RAM]                | [ROM] BASIC ROM        |
 *         |                      |                        |
 *         |                      |                        |
 *         |                      |                        |
 *         |                      |                        |
 *         .-----------------------------------------------.
 * 0xc000  | [RAM]                                         |
 *         |                                               |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0xd000  | [I/O] VIC-II, SID,   | [ROM]        | [RAM]   |
 *         |       CIA 1/2,       |   Character  |         |
 *         |       Color RAM      |   ROM        |         |
 *         .-----------------------------------------------.
 * 0xe000  | [ROM] Kernal         | [RAM]                  |
 *         |                      |                        |
 *         |                      |                        |
 *         .-----------------------------------------------.
 * 0xfffa  | [CPU] MOS 6510, vectors (NMI, RESET, IRQ)     |
 * 0xffff  |       defined in 'Kernal'                     |
 *         .-----------------------------------------------.
 */

/* ***************************************************************  */

register_uint8(00,                 MEMORY_ZEROPAGE_BEGIN)
register_uint8(ff,                 MEMORY_ZEROPAGE_END)

register_uint8(0100,               MEMORY_EXTZEROPAGE_BEGIN)
register_uint8(03ff,               MEMORY_EXTZEROPAGE_END)

register_uint8(0100,               MEMORY_EXTZEROPAGE_BEGIN)
register_uint8(03ff,               MEMORY_EXTZEROPAGE_END)

register_uint8(0400,               MEMORY_SCREENRAM_DEFAULT_BEGIN)
register_uint8(07ff,               MEMORY_SCREENRAM_DEFAULT_END)

register_uint8(0800,               MEMORY_BASICRAM_BEGIN)
register_uint8(0801,               MEMORY_BASICRAM_C64NONSENSE_BEGIN)
register_uint8(9fff,               MEMORY_BASICRAM_END)

register_uint8(a000,               MEMORY_BANK_BASICROM_BEGIN)
register_uint8(bfff,               MEMORY_BANK_BASICROM_END)

register_uint8(c000,               MEMORY_RAM_C000_BEGIN)
register_uint8(cfff,               MEMORY_RAM_C000_END)

register_uint8(d000,               MEMORY_BANK_IOCHIPS_BEGIN)
register_uint8(dfff,               MEMORY_BANK_IOCHIPS_END)

register_uint8(e000,               MEMORY_BANK_KERNALROM_BEGIN)
register_uint8(efff,               MEMORY_BANK_KERNALROM_END)

/* ***************************************************************  */

header_endif(MEMORY)

#endif /* MEMORY_DEF_H__  */
