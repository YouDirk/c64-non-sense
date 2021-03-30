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

#include "kernal.gen.h"

/* ***************************************************************  */

/* Same as Kernal default configuration: read-only just for cassette
 * switch bit with mask 0x10.
 */
#define MEMORY_IOPORT_DDR                                            \
                    KERNAL_ZP_MOS6510_CASSETTE_MOTOREN_I_MASK        \
                    | KERNAL_ZP_MOS6510_CASSETTE_OUT_MASK            \
                    | KERNAL_ZP_MOS6510_IOCHIPS_MASK                 \
                    | KERNAL_ZP_MOS6510_KERNALROM_MASK               \
                    | KERNAL_ZP_MOS6510_BASICROM_MASK


/* Same as Kernal default configuration: BASIC ROM is mapped out.
 * Make sure that cassette motor disabled.
 */
#define MEMORY_IOPORT_IODATA                                         \
                    KERNAL_ZP_MOS6510_CASSETTE_MOTOREN_I_MASK        \
                    | KERNAL_ZP_MOS6510_CASSETTE_CLOSED_MASK         \
                    | KERNAL_ZP_MOS6510_IOCHIPS_MASK                 \
                    | KERNAL_ZP_MOS6510_KERNALROM_MASK               \
                    | KERNAL_ZP_MOS6510_BASICROM_MASK

/* ***************************************************************  */

void __fastcall__
Memory_init(void)
{

  KERNAL_ZP_MOS6510_IODDR = MEMORY_IOPORT_DDR;
  KERNAL_ZP_MOS6510_IODATA = MEMORY_IOPORT_IODATA;

#ifdef DEBUG
  if (MEMORY_IOPORT_IODATA & KERNAL_ZP_MOS6510_KERNALROM_MASK) {
    // TODO: Check if ROM is reallay mapped
    printf("'%s'\n", &MEMORY_BANK_KERNALROM_BEGIN);
  }

  printf("0x%02x 0x%02x\n",
         KERNAL_ZP_MOS6510_IODDR, KERNAL_ZP_MOS6510_IODATA);
#endif
}

void __fastcall__
Memory_release(void)
{
  KERNAL_ZP_MOS6510_IODDR = KERNAL_ZP_MOS6510_IODDR_DEFAULT;
  KERNAL_ZP_MOS6510_IODATA = KERNAL_ZP_MOS6510_IODATA_DEFAULT;
}

/* ***************************************************************  */
