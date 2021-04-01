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


#include "Memory.h"

#include "Engine.h"

#include "kernal.gen.h"

/* *******************************************************************
 * Mapping table for the corresponding MEMORY_IOPORT_IODATA.
 *
 *                     CartHi CartLo
 * GAME_I EXROM_I | IO KERNAL BASIC | 0x8000  0xa000  0xd000  0xe000
 * -----------------------------------------------------------------
 *      1      1     1    1    1    | RAM     BASIC   I/O     Kernal
 * def  1      x     1    1    0    | RAM     RAM     I/O     Kernal
 *      1      x     1    0    1    | RAM     RAM     I/O     RAM
 *      1      x     x    0    0    | RAM     RAM     RAM     RAM
 *      1      1     0    1    1    | RAM     BASIC   Chars   Kernal
 *      1      x     0    1    0    | RAM     RAM     Chars   Kernal
 *      1      x     0    0    1    | RAM     RAM     Chars   RAM
 *      1      0     1    1    1    | CartLo  BASIC   I/O     Kernal
 *      1      0     0    1    1    | CartLo  BASIC   Chars   Kernal
 *      0      1     x    x    x    | CartLo  <unmap> I/O     CartHi
 *      0      0     1    1    1    | CartLo  CartHi  I/O     Kernal
 *      0      0     1    1    0    | RAM     CartHi  I/O     Kernal
 *      0      0     1    0    1    | RAM     RAM     I/O     RAM
 *      0      0     x    0    0    | RAM     RAM     RAM     RAM
 *      0      0     0    1    1    | CartLo  CartHi  Chars   Kernal
 *      0      0     0    1    0    | RAM     CartHi  Chars   Kernal
 *      0      0     0    0    1    | RAM     RAM     RAM     RAM
 */
// TODO: Define MEMORY_IOPORT_IODATA_MASK and MEMORY_IOPORT_IODATA_VAL

/* Same as Kernal default configuration: read-only just for cassette
 * switch bit with mask 0x10.
 */
#define MEMORY_IOPORT_DDR                                            \
                    (KERNAL_ZP_MOS6510_CASSETTE_MOTOREN_I_MASK       \
                     | KERNAL_ZP_MOS6510_CASSETTE_OUT_MASK           \
                     | KERNAL_ZP_MOS6510_IOCHIPS_MASK                \
                     | KERNAL_ZP_MOS6510_KERNALROM_MASK              \
                     | KERNAL_ZP_MOS6510_BASICROM_MASK)


/* Same as Kernal default configuration: BASIC ROM is mapped out.
 * Make sure that cassette motor disabled.
 */
#define MEMORY_IOPORT_IODATA                                         \
                    (KERNAL_ZP_MOS6510_CASSETTE_MOTOREN_I_MASK       \
                     | KERNAL_ZP_MOS6510_CASSETTE_CLOSED_MASK        \
                     | KERNAL_ZP_MOS6510_IOCHIPS_MASK                \
                     | KERNAL_ZP_MOS6510_KERNALROM_MASK)

/* ***************************************************************  */

/* private validation arrays  */
#ifdef DEBUG

#define _MEMORY_INIT_VALID_BUFSIZE           5
static uint8_t _Memory_init_valid_kernalrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x85, 0x56, 0x20, 0x0f, 0xbc};
static uint8_t _Memory_init_valid_basicrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x94, 0xe3, 0x7b, 0xe3, 0x43};

#endif /* DEBUG  */

void __fastcall__
Memory_init(void)
{
#ifdef DEBUG
  static const char* errormsg;
#endif /* DEBUG  */

  KERNAL_ZP_MOS6510_IODDR = MEMORY_IOPORT_DDR;
  KERNAL_ZP_MOS6510_IODATA = MEMORY_IOPORT_IODATA;

#ifdef DEBUG
  /* Kernal ROM should be tested at first, to reduce chance that an
   * IRQ fails.
   */
  if (MEMORY_IOPORT_IODATA & KERNAL_ZP_MOS6510_KERNALROM_MASK
      && (0 != memcmp(&MEMORY_BANK_KERNALROM_BEGIN,
                      _Memory_init_valid_kernalrom,
                      _MEMORY_INIT_VALID_BUFSIZE))) {
    __asm__("sei");
    errormsg = "memory init, invalid kernal rom!";
  } else if (MEMORY_IOPORT_IODATA & KERNAL_ZP_MOS6510_BASICROM_MASK
             && (0 != memcmp(&MEMORY_BANK_BASICROM_BEGIN,
                             _Memory_init_valid_basicrom,
                             _MEMORY_INIT_VALID_BUFSIZE))) {
    __asm__("sei");
    errormsg = "memory init, invalid basic rom!";
  } else {
    // TODO: Some more validations ...
    return;
  }

  /* Without SEI instruction, it will crash and never terminate,
   * because we are using the KERNAL_{IRQ,NMI,}_USERENTRY entry points
   * instead of the MOS 6510 hard IRQ vectors at 0xfff{a,c,e}.
   * Therefore if the Kernal ROM is not mapped into the memory anymore
   * calling the ISRs will be fail.
   */

  DEBUG_ERROR(errormsg);
  Engine.set.exit_code = ENGINE_EXIT_FAILURE_ENGINE;
  DEBUG_RELEASE_PRINT();

  exit(ENGINE_EXIT_FAILURE_ENGINE);
#endif /* DEBUG  */
}

void __fastcall__
Memory_release(void)
{
  KERNAL_ZP_MOS6510_IODDR = KERNAL_ZP_MOS6510_IODDR_DEFAULT;
  KERNAL_ZP_MOS6510_IODATA = KERNAL_ZP_MOS6510_IODATA_DEFAULT;
}

/* ***************************************************************  */
