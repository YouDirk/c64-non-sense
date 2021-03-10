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

#define STRING_BUFSIZE             256
typedef char                       string_t[STRING_BUFSIZE];

/*****************************************************************  */

/* Not STDINT, but better assoziation for bit-width of integers.  A
 * whole byte will be allocated, no checks for overflow.
 */
typedef uint8_t                    uint7_t;
typedef uint8_t                    uint6_t;
typedef uint8_t                    uint5_t;
typedef uint8_t                    uint4_t;
typedef uint8_t                    uint3_t;
typedef uint8_t                    uint2_t;
typedef int8_t                     int7_t;
typedef int8_t                     int6_t;
typedef int8_t                     int5_t;
typedef int8_t                     int4_t;
typedef int8_t                     int3_t;
typedef int8_t                     int2_t;

#define UINT7_MAX                  ((uint7_t) 0x7f)
#define UINT6_MAX                  ((uint6_t) 0x3f)
#define UINT5_MAX                  ((uint5_t) 0x1f)
#define UINT4_MAX                  ((uint4_t) 0x0f)
#define UINT3_MAX                  ((uint3_t) 0x07)
#define UINT2_MAX                  ((uint2_t) 0x03)

#define INT7_MIN                   ((int7_t) 0xc0)
#define INT7_MAX                   ((int7_t) 0x3f)
#define INT6_MIN                   ((int6_t) 0xe0)
#define INT6_MAX                   ((int6_t) 0x1f)
#define INT5_MIN                   ((int5_t) 0xf0)
#define INT5_MAX                   ((int5_t) 0x0f)
#define INT4_MIN                   ((int4_t) 0xf8)
#define INT4_MAX                   ((int4_t) 0x07)
#define INT3_MIN                   ((int3_t) 0xfc)
#define INT3_MAX                   ((int3_t) 0x03)
#define INT2_MIN                   ((int2_t) 0xfe)
#define INT2_MAX                   ((int2_t) 0x01)

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

#endif /* COMMON_H__  */
