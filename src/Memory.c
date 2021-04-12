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
#pragma warn(unused-var, push, off)

#define _MEMORY_INIT_VALID_BUFSIZE           5
static uint8_t _Memory_init_valid_kernalrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x85, 0x56, 0x20, 0x0f, 0xbc};
static uint8_t _Memory_init_valid_basicrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x94, 0xe3, 0x7b, 0xe3, 0x43};

static void __fastcall__
_Memory_init_debug_exit(const char* errormsg)
{
  DEBUG_ERROR(errormsg);
  Engine.set.exit_code = ENGINE_EXIT_FAILURE_ENGINE;
  DEBUG_RELEASE_PRINT();

  exit(ENGINE_EXIT_FAILURE_ENGINE);
}

static bool __fastcall__
_Memory_init_debug_ramtest(volatile uint8_t* ram_ptr)
{
  static uint8_t i;

  for (i=0; i<_MEMORY_INIT_VALID_BUFSIZE; ++i)
    ram_ptr[i] = i;

  for (i=0; i<_MEMORY_INIT_VALID_BUFSIZE; ++i)
    if (ram_ptr[i] != i) return false;

  return true;
}

/*#pragma warn(unused-var, pop)  Not working yet  */
#endif /* DEBUG  */

void __fastcall__
Memory_init(void)
{
  MEMORY_MOS6510_IODDR = _MEMORY_MOS6510_IODDR_VAL;
  MEMORY_MOS6510_IODATA = _MEMORY_MOS6510_IODATA_VAL;

#ifdef DEBUG
#pragma warn(const-comparison, push, off)
/*#pragma warn(unreachable-code, push, off)
 *
 * Hopefully available in a future version of CC65.  Pull request
 * pending at https://github.com/cc65/cc65/pull/1459
 */

  /* Kernal ROM should be tested at first, to reduce chance that an
   * IRQ fails.
   */
  if ((_MEMORY_MOS6510_IODATA_VAL & MEMORY_MOS6510_KERNALROM_MASK)
      == MEMORY_BANKS_ISMAPPED_KERNAL) {
    if (0 != memcmp(&MEMORY_BANK_KERNALROM_BEGIN,
                    _Memory_init_valid_kernalrom,
                    _MEMORY_INIT_VALID_BUFSIZE)) {
      /* Without SEI instruction, it will crash and never terminate,
       * because we are using the KERNAL_{IRQ,NMI,}_USERENTRY entry
       * points instead of the MOS 6510 hard IRQ vectors at
       * 0xfff{a,c,e}.  Therefore if the Kernal ROM is not mapped into
       * the memory anymore calling the ISRs will be fail.
       */

      __asm__("sei");
      _Memory_init_debug_exit("memory init, invalid kernal rom!");
    }
  } else { /* MEMORY_BANKS_ISMAPPED_KERNAL  */
    if (!_Memory_init_debug_ramtest(&MEMORY_BANK_KERNALROM_BEGIN)) {
      __asm__("sei");
      _Memory_init_debug_exit("memory init, kernal rom mapped!");
    }
  } /* MEMORY_BANKS_ISMAPPED_KERNAL  */

  /*
  if ((MEMORY_MOS6510_IODATA
              & MEMORY_BANKS_ISMAPPED_IO_MASK)
             == MEMORY_BANKS_ISMAPPED_IO) {
    __asm__("sei");
    _Memory_init_debug_exit("memory init, io not mapped!");
  } else if ((MEMORY_MOS6510_IODATA
              & MEMORY_BANKS_ISMAPPED_BASIC_MASK)
             == MEMORY_BANKS_ISMAPPED_BASIC
             && 0 != memcmp(&MEMORY_BANK_BASICROM_BEGIN,
                            _Memory_init_valid_basicrom,
                            _MEMORY_INIT_VALID_BUFSIZE)) {
    __asm__("sei");
    _Memory_init_debug_exit("memory init, invalid basic rom!");
  }
  */

  // TODO: Some more validations ...

/*#pragma warn(unreachable-code, pop)
 *
 * Hopefully available in a future version of CC65.  Pull request
 * pending at https://github.com/cc65/cc65/pull/1459
 */
#pragma warn(const-comparison, pop)
#endif /* DEBUG  */
}

void __fastcall__
Memory_release(void)
{
  MEMORY_MOS6510_IODDR = MEMORY_MOS6510_IODDR_DEFAULT;
  MEMORY_MOS6510_IODATA = MEMORY_MOS6510_IODATA_DEFAULT;
}

/* ***************************************************************  */
