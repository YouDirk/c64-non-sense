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


#ifndef COMMON_H__
#define COMMON_H__

/* We should really use this.  Basic stuff such like memset() for
 * better performance, instead of implementing copy-loops.
 */
#include <c64.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Debug.h"

#define STRING_LEN                 256
typedef char                       string_t[STRING_LEN];

/*****************************************************************  */

/* Not STDINT, but better assoziation for bit-width of integers.  A
 * whole byte will be allocated, no checks for overflow.
 */
typedef uint8_t                    uint4_t;
typedef int8_t                     int4_t;

/*****************************************************************  */

#define UINT16(uint16_struct)      ((uint16_t) (uint16_struct))
#define INT16(int16_struct)        ((int16_t) (int16_struct))

/* Do NOT CHANGE THE ORDER of the members of this struct.
 *
 * The MOS 6510 CPU is a little endian machine.  In composition with
 * the ANSI C standard, the order of the members will be preserved in
 * the memory.
 */
struct uint16_s {
  uint8_t byte_low;
  uint8_t byte_high;
};

typedef struct uint16_s            uint16_s;
typedef struct uint16_s            int16_s;


#define UINT32(uint32_struct)      ((uint32_t) (uint32_struct))
#define INT32(int32_struct)        ((int32_t) (int32_struct))

/* Do NOT CHANGE THE ORDER of the members of this struct.
 *
 * The MOS 6510 CPU is a little endian machine.  In composition with
 * the ANSI C standard, the order of the members will be preserved in
 * the memory.
 */
struct uint32_s {
  uint16_s word_low;
  uint16_s word_high;
};

typedef struct uint32_s            uint32_s;
typedef struct uint32_s            int32_s;

/*****************************************************************  */

#define MUTEX_LOCK(mutex)          ((mutex) = 1)
#define MUTEX_UNLOCK(mutex)        ((mutex) = 0)
#define MUTEX_ISLOCKED(mutex)      ((mutex) != 0)

/* The MOS 6510 CPU is an 8-bit machine.  Writing an 8-bit value to
 * memory will be compiled to an atomar operation (STA mutex).
 */
typedef uint8_t                    mutex_t;

/*****************************************************************  */

#endif /* COMMON_H__  */
