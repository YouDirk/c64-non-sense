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


#include "Memory.h"

#include "Engine.h"

#include "chip-vic.gen.h"
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
static uint8_t _Memory_init_valid_charrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x3c, 0x66, 0x6e, 0x6e, 0x60};
static uint8_t _Memory_init_valid_basicrom[_MEMORY_INIT_VALID_BUFSIZE]
  = {0x94, 0xe3, 0x7b, 0xe3, 0x43};
static uint8_t _Memory_init_valid_ram[_MEMORY_INIT_VALID_BUFSIZE]
  = {0xff, 0xee, 0xdd, 0xcc, 0xbb};

static void __fastcall__
_Memory_init_debug_exit(const char* errormsg)
{
  Memory_release();

  DEBUG_ERROR(errormsg);
  Engine.set.exit_code = ENGINE_EXIT_FAILURE_ENGINE;
  DEBUG_RELEASE_PRINT();

  exit(ENGINE_EXIT_FAILURE_ENGINE);
}

static bool __fastcall__
_Memory_init_debug_valid_io(void)
{
  static uint8_t i, rasterline;

  rasterline = VIC_RASTERLINE;
  for (i=0; i<UINT8_MAX; ++i) {
    if (rasterline != VIC_RASTERLINE) return true;
  }

  return false;
}

/*#pragma warn(unused-var, pop)  Not working yet  */
#endif /* DEBUG  */

/* ***************************************************************  */

void __fastcall__
Memory_init(void)
{
  MEMORY_MOS6510_IODDR = _MEMORY_MOS6510_IODDR_VAL;
  MEMORY_MOS6510_IODATA = _MEMORY_MOS6510_IODATA_VAL;

#ifdef DEBUG
  __asm__("sei");
  /* Without SEI instruction, it could crash and never terminate,
   * because we are using the KERNAL_{IRQ,NMI,}_USERENTRY entry points
   * instead of the MOS 6510 hard IRQ vectors at 0xfff{a,c,e}.
   * Therefore, if the Kernal ROM is not mapped into the memory
   * anymore calling the ISRs will be fail.
   */

  /* -------------------------------------------------------------  */

#if (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_KERNAL_MASK) \
     == MEMORY_BANKS_ISMAPPED_KERNAL
  if (0 != memcmp(&MEMORY_BANK_KERNALROM_BEGIN,
        _Memory_init_valid_kernalrom, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid kernal rom!");
  }
#else /* MEMORY_BANKS_ISMAPPED_KERNAL  */
  memcpy(&MEMORY_BANK_KERNALROM_BEGIN, _Memory_init_valid_ram,
         _MEMORY_INIT_VALID_BUFSIZE);
  if (0 != memcmp(&MEMORY_BANK_KERNALROM_BEGIN,
        _Memory_init_valid_ram, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid ram at 0xe000!");
  }
#endif /* MEMORY_BANKS_ISMAPPED_KERNAL || RAM  */

  /* -------------------------------------------------------------  */

#if (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_IO_MASK)     \
     == MEMORY_BANKS_ISMAPPED_IO
  if (!_Memory_init_debug_valid_io()) {
    _Memory_init_debug_exit("memory init, io not mapped!");
  }
#elif (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_CHARS_MASK) \
       == MEMORY_BANKS_ISMAPPED_CHARS
  if (0 != memcmp(&MEMORY_BANK_IOCHIPS_BEGIN,
        _Memory_init_valid_charrom, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid character rom!");
  }
#else /* MEMORY_BANKS_ISMAPPED_IO || CHARS || RAM  */
  memcpy(&MEMORY_BANK_IOCHIPS_BEGIN, _Memory_init_valid_ram,
         _MEMORY_INIT_VALID_BUFSIZE);
  if (0 != memcmp(&MEMORY_BANK_IOCHIPS_BEGIN,
        _Memory_init_valid_ram, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid ram at 0xd000!");
  }
#endif /* MEMORY_BANKS_ISMAPPED_IO || CHARS || RAM  */

  /* -------------------------------------------------------------  */

#if (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_BASIC_MASK)  \
     == MEMORY_BANKS_ISMAPPED_BASIC
  if (0 != memcmp(&MEMORY_BANK_BASICROM_BEGIN,
        _Memory_init_valid_basicrom, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid basic rom!");
  }
#elif (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_CARTHI_MASK) \
       == MEMORY_BANKS_ISMAPPED_CARTHI
  /*
   * Nothing to validate for cartridge ROM High.
   */
#else /* MEMORY_BANKS_ISMAPPED_BASIC || CARTHI || RAM  */
  memcpy(&MEMORY_BANK_BASICROM_BEGIN, _Memory_init_valid_ram,
         _MEMORY_INIT_VALID_BUFSIZE);
  if (0 != memcmp(&MEMORY_BANK_BASICROM_BEGIN,
        _Memory_init_valid_ram, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid ram at 0xa000!");
  }
#endif /* MEMORY_BANKS_ISMAPPED_BASIC || CARTHI || RAM  */

  /* -------------------------------------------------------------  */

#if (_MEMORY_MOS6510_IODATA_VAL & MEMORY_BANKS_ISMAPPED_CARTLO_MASK) \
     == MEMORY_BANKS_ISMAPPED_CARTLO
  /*
   * Nothing to validate for cartridge ROM Low.
   */
#else /* MEMORY_BANKS_ISMAPPED_CARTLO || RAM  */
  memcpy(&MEMORY_BANK_CARTLO_BEGIN, _Memory_init_valid_ram,
         _MEMORY_INIT_VALID_BUFSIZE);
  if (0 != memcmp(&MEMORY_BANK_CARTLO_BEGIN,
        _Memory_init_valid_ram, _MEMORY_INIT_VALID_BUFSIZE)) {
    _Memory_init_debug_exit("memory init, invalid ram at 0x8000!");
  }
#endif /* MEMORY_BANKS_ISMAPPED_CARTLO || RAM  */

  /* -------------------------------------------------------------  */

  __asm__("cli");
#endif /* DEBUG  */
}

void __fastcall__
Memory_release(void)
{
  MEMORY_MOS6510_IODDR = MEMORY_MOS6510_IODDR_DEFAULT;
  MEMORY_MOS6510_IODATA = MEMORY_MOS6510_IODATA_DEFAULT;
}

/* ***************************************************************  */
