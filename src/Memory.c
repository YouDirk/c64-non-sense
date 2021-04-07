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

/* ***************************************************************  */

/* Same as Kernal default configuration.  */
#define _MEMORY_MOS6510_IODDR_VAL       MEMORY_MOS6510_IODDR_DEFAULT


/* Same as Kernal default configuration.  */
#define _MEMORY_MOS6510_IODATA_VAL                                   \
  ((MEMORY_MOS6510_IODATA_DEFAULT & ~MEMORY_BANKS_IOPORT_MASK)       \
   | (MEMORY_BANKS_IOPORT_MASK                                       \
      & MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT_MASK          \
      & MEMORY_BANKS_NOTGAME_RAM_RAM_IO_KERNAL_DEFAULT))

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

  MEMORY_MOS6510_IODDR = _MEMORY_MOS6510_IODDR_VAL;
  MEMORY_MOS6510_IODATA = _MEMORY_MOS6510_IODATA_VAL;

#ifdef DEBUG
  /* Kernal ROM should be tested at first, to reduce chance that an
   * IRQ fails.
   */
  if ((_MEMORY_MOS6510_IODATA_VAL & MEMORY_MOS6510_KERNALROM_MASK)
      && 0 != memcmp(&MEMORY_BANK_KERNALROM_BEGIN,
                     _Memory_init_valid_kernalrom,
                     _MEMORY_INIT_VALID_BUFSIZE)) {
    __asm__("sei");
    errormsg = "memory init, invalid kernal rom!";
  } else if ((_MEMORY_MOS6510_IODATA_VAL
              & MEMORY_BANKS_ISMAPPED_BASIC_MASK)
             == MEMORY_BANKS_ISMAPPED_BASIC
             && 0 != memcmp(&MEMORY_BANK_BASICROM_BEGIN,
                            _Memory_init_valid_basicrom,
                            _MEMORY_INIT_VALID_BUFSIZE)) {
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
  MEMORY_MOS6510_IODDR = MEMORY_MOS6510_IODDR_DEFAULT;
  MEMORY_MOS6510_IODATA = MEMORY_MOS6510_IODATA_DEFAULT;
}

/* ***************************************************************  */
