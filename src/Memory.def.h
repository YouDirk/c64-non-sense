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


#ifndef MEMORY_DEF_H__
#define MEMORY_DEF_H__

#include "define.h"

header_ifndef(MEMORY)
header_define(MEMORY)

/* *******************************************************************
 *
 *         : Pre-initialzed       : Possible memory banks       :
 *         : **************       : *********************       :
 *         :                      :                             :
 *         .----------------------.-----------------------------.
 * 0x0000  | [CPU] MOS 6510, port access (pin P0-P5)            |
 *         |----------------------------------------------------|
 * 0x0002  | [RAM, Kernal] Zero Page                            |
 *         |----------------------------------------------------|
 * 0x0100  | [RAM, Kernal] Extended Zero Page                   |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x0400  | [RAM, VIC-II DMA] default Screen RAM               |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x0800  | [RAM, BASIC] BASIC RAM                             |
 *         |   0x0801: C64 NonSense entry point                 |
 *         |                                                    |
 *         |                                                    |
 *         |                                                    |
 *         |                                                    |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0x8000  | [RAM]                | [Cartridge] ROM Low         |
 *         |                      |                             |
 *         |                      |                             |
 *         |                      |                             |
 *         |----------------------------------------------------|
 * 0xa000  | [RAM]                | [ROM]        | [Cartridge]  |
 *         |                      |   BASIC ROM  |   ROM High   |
 *         |                      |              |              |
 *         |                      |              |              |
 *         |----------------------------------------------------|
 * 0xc000  | [RAM]                                              |
 *         |                                                    |
 *         |                                                    |
 *         |----------------------------------------------------|
 * 0xd000  | [I/O] VIC-II, SID,   | [ROM]        | [RAM]        |
 *         |       CIA 1/2,       |   Character  |              |
 *         |       Color RAM      |   ROM        |              |
 *         |----------------------------------------------------|
 * 0xe000  | [ROM] Kernal         | [RAM]        | [Cartridge]  |
 *         |                      |              |   ROM High   |
 *         |                      |              |              |
 *         |----------------------------------------------------|
 * 0xfffa  | [CPU] MOS 6510, vectors (NMI, RESET, IRQ)          |
 * 0xffff  |       defined in 'Kernal'                          |
 *         '----------------------------------------------------'
 */

/* ***************************************************************  */

register_uint8_hex(00,             MEMORY_ZEROPAGE_BEGIN)
register_uint8_hex(ff,             MEMORY_ZEROPAGE_END)

register_uint8_hex(0100,           MEMORY_EXTZEROPAGE_BEGIN)
register_uint8_hex(03ff,           MEMORY_EXTZEROPAGE_END)

register_uint8_hex(0100,           MEMORY_EXTZEROPAGE_BEGIN)
register_uint8_hex(03ff,           MEMORY_EXTZEROPAGE_END)

register_uint8_hex(0400,           MEMORY_SCREENRAM_DEFAULT_BEGIN)
register_uint8_hex(07ff,           MEMORY_SCREENRAM_DEFAULT_END)

register_uint8_hex(0800,           MEMORY_BASICRAM_BEGIN)
register_uint8_hex(0801,           MEMORY_BASICRAM_C64NONSENSE_BEGIN)
register_uint8_hex(7fff,           MEMORY_BASICRAM_END)

register_uint8_hex(8000,           MEMORY_BANK_CARTLO_BEGIN)
register_uint8_hex(9fff,           MEMORY_BANK_CARTLO_END)

register_uint8_hex(a000,           MEMORY_BANK_BASICROM_BEGIN)
register_uint8_hex(bfff,           MEMORY_BANK_BASICROM_END)

register_uint8_hex(c000,           MEMORY_RAM_C000_BEGIN)
register_uint8_hex(cfff,           MEMORY_RAM_C000_END)

register_uint8_hex(d000,           MEMORY_BANK_IOCHIPS_BEGIN)
register_uint8_hex(dfff,           MEMORY_BANK_IOCHIPS_END)

register_uint8_hex(e000,           MEMORY_BANK_KERNALROM_BEGIN)
register_uint8_hex(efff,           MEMORY_BANK_KERNALROM_END)

/* *******************************************************************
 * Mapping table for the corresponding MEMORY_MOS6510_IODATA.
 *
 * (*): Not working by default, reason:
 *      (k) Kernal not mapped, (io) I/O not mapped
 *
 * x: Don´t care.  Will be set to 0, if MEMORY_BANKS_* are used, to
 *    switch memory banks.
 *
 *                     CartHi
 * GAME_I EXROM_I | IO KERNAL ALTER | 0x8000  0xa000  0xd000  0xe000
 * -----------------------------------------------------------------
 *      1      1     1    1    1    | RAM     BASIC   I/O     Kernal
 * def  1      x     1    1    0    | RAM     RAM     I/O     Kernal
 * (k)  1      x     1    0    1    | RAM     RAM     I/O     RAM
 * (k)  1      x     x    0    0    | RAM     RAM     RAM     RAM
 * (io) 1      1     0    1    1    | RAM     BASIC   Chars   Kernal
 * (io) 1      x     0    1    0    | RAM     RAM     Chars   Kernal
 * (k)  1      x     0    0    1    | RAM     RAM     Chars   RAM
 *      1      0     1    1    1    | CartLo  BASIC   I/O     Kernal
 * (io) 1      0     0    1    1    | CartLo  BASIC   Chars   Kernal
 * (k)  0      1     x    x    x    | CartLo  <unmap> I/O     CartHi
 *      0      0     1    1    1    | CartLo  CartHi  I/O     Kernal
 *      0      0     1    1    0    | RAM     CartHi  I/O     Kernal
 * (k)  0      0     1    0    1    | RAM     RAM     I/O     RAM
 * (k)  0      0     x    0    0    | RAM     RAM     RAM     RAM
 * (io) 0      0     0    1    1    | CartLo  CartHi  Chars   Kernal
 * (io) 0      0     0    1    0    | RAM     CartHi  Chars   Kernal
 * (k)  0      0     0    0    1    | RAM     RAM     RAM     RAM
 */
define_hex(MEMORY_BANKS_MASK,                                    1f)

define_hex(MEMORY_BANKS_CARTRIDGE_MASK,                          18)
define_hex(MEMORY_BANKS_IOPORT_MASK,                             07)

define_hex(MEMORY_BANKS_ISMAPPED_KERNAL_MASK,                    02)
define_hex(MEMORY_BANKS_ISMAPPED_KERNAL,                         02)
define_hex(MEMORY_BANKS_ISMAPPED_IO_MASK,                        04)
define_hex(MEMORY_BANKS_ISMAPPED_IO,                             04)
define_hex(MEMORY_BANKS_ISMAPPED_CHARS_MASK,                     06)
define_hex(MEMORY_BANKS_ISMAPPED_CHARS,                          02)
define_hex(MEMORY_BANKS_ISMAPPED_BASIC_MASK,                     13)
define_hex(MEMORY_BANKS_ISMAPPED_BASIC,                          13)
define_hex(MEMORY_BANKS_ISMAPPED_CARTHI_MASK,                    12)
define_hex(MEMORY_BANKS_ISMAPPED_CARTHI,                         02)
define_hex(MEMORY_BANKS_ISMAPPED_CARTLO_MASK,                    0b)
define_hex(MEMORY_BANKS_ISMAPPED_CARTLO,                         03)

define_hex(MEMORY_BANKS_NOTGAME_RAM_BASIC_IO_KERNAL_MASK,        1f)
define_hex(MEMORY_BANKS_NOTGAME_RAM_BASIC_IO_KERNAL,             1f)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT_MASK,  17)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT,       16)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_IO_RAM_MASK,             17)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_IO_RAM,                  15)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_RAM_RAM_MASK,            13)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_RAM_RAM,                 10)
define_hex(MEMORY_BANKS_NOTGAME_RAM_BASIC_CHARS_KERNAL_MASK,     1f)
define_hex(MEMORY_BANKS_NOTGAME_RAM_BASIC_CHARS_KERNAL,          1b)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_CHARS_KERNAL_MASK,       17)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_CHARS_KERNAL,            12)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_CHARS_RAM_MASK,          17)
define_hex(MEMORY_BANKS_NOTGAME_RAM_RAM_CHARS_RAM,               11)
define_hex(MEMORY_BANKS_NOTGAME_CARTLO_BASIC_IO_KERNAL_MASK,     1f)
define_hex(MEMORY_BANKS_NOTGAME_CARTLO_BASIC_IO_KERNAL,          17)
define_hex(MEMORY_BANKS_NOTGAME_CARTLO_BASIC_CHARS_KERNAL_MASK,  1f)
define_hex(MEMORY_BANKS_NOTGAME_CARTLO_BASIC_CHARS_KERNAL,       13)
define_hex(MEMORY_BANKS_GAME_CARTLO_UNMAP_IO_CARTHI_MASK,        18)
define_hex(MEMORY_BANKS_GAME_CARTLO_UNMAP_IO_CARTHI,             08)
define_hex(MEMORY_BANKS_GAME_CARTLO_CARTHI_IO_KERNAL_MASK,       1f)
define_hex(MEMORY_BANKS_GAME_CARTLO_CARTHI_IO_KERNAL,            07)
define_hex(MEMORY_BANKS_GAME_RAM_CARTHI_IO_KERNAL_MASK,          1f)
define_hex(MEMORY_BANKS_GAME_RAM_CARTHI_IO_KERNAL,               06)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_IO_RAM_MASK,                1f)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_IO_RAM,                     05)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_RAM_RAM_MASK,               1b)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_RAM_RAM,                    00)
define_hex(MEMORY_BANKS_GAME_CARTLO_CARTHI_CHARS_KERNAL_MASK,    1f)
define_hex(MEMORY_BANKS_GAME_CARTLO_CARTHI_CHARS_KERNAL,         03)
define_hex(MEMORY_BANKS_GAME_RAM_CARTHI_CHARS_KERNAL_MASK,       1f)
define_hex(MEMORY_BANKS_GAME_RAM_CARTHI_CHARS_KERNAL,            02)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_RAM_RAM2_MASK,              1f)
define_hex(MEMORY_BANKS_GAME_RAM_RAM_RAM_RAM2,                   01)

/* ***************************************************************  */

/* Access to MOS 6510 CPU IO port (pin P0-P5).
 *
 * IODDR: Data Direction Register (1=output, 0=input)
 * IODATA: Data Input/Output depending on IODDR
 *
 * _I_MASK means that the signal which is connected to the
 * corresponding pin P[0-5] is active-low.
 */
register_uint8_hex(00,                       MEMORY_MOS6510_IODDR)
register_uint8_hex(01,                       MEMORY_MOS6510_IODATA)
define_hex(MEMORY_MOS6510_ALTER_MASK,                  01)
define_hex(MEMORY_MOS6510_KERNALROM_MASK,              02)
define_hex(MEMORY_MOS6510_IOCHIPS_MASK,                04)
define_hex(MEMORY_MOS6510_CASSETTE_OUT_MASK,           08)
define_hex(MEMORY_MOS6510_CASSETTE_CLOSED_MASK,        10)
define_hex(MEMORY_MOS6510_CASSETTE_MOTOREN_I_MASK,     20)

/* Default Kernal configuration: read-only just for cassette switch
 * bit with mask 0x10.
 */
define(MEMORY_MOS6510_IODDR_DEFAULT,                                 \
       MEMORY_MOS6510_CASSETTE_MOTOREN_I_MASK                        \
       | MEMORY_MOS6510_CASSETTE_OUT_MASK                            \
       | MEMORY_BANKS_IOPORT_MASK)

/* Default Kernal configuration: BASIC ROM is mapped out.  Make sure
 * that cassette motor disabled.
 */
define(MEMORY_MOS6510_IODATA_DEFAULT,                                \
       MEMORY_MOS6510_CASSETTE_MOTOREN_I_MASK                        \
       | MEMORY_MOS6510_CASSETTE_CLOSED_MASK                         \
       | (MEMORY_BANKS_IOPORT_MASK                                   \
          & MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT_MASK      \
          & MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT))

/* ***************************************************************  */

header_endif(MEMORY)

#endif /* MEMORY_DEF_H__  */
