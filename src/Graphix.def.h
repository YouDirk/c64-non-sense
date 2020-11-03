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


#ifndef GRAPHIX_DEF_H__
#define GRAPHIX_DEF_H__

#include "define.h"

/* ***************************************************************  */

/* graphic buffer structure  */
typedef_struct_begin(Graphix_buffer_t)
  typedef_struct_uint8_ptr(screen_ram)
  typedef_struct_uint8_ptr(bitmap_ram)

  typedef_struct_uint8(bordercolor)
  typedef_struct_int8(scroll_x)
  typedef_struct_int8(scroll_y)
typedef_struct_end(Graphix_buffer_t)

/* ***************************************************************  */

/* Structure of static members for module.  */
typedef_struct_begin(Graphix_t)

  /* TRUE if C64 has a PAL VIC, otherwise we are on a NTSC machine. */
  typedef_struct_int8(is_pal)

  /* The logical graphic buffer.  */
  typedef_struct_nested(Graphix_buffer_t, buffer)

typedef_struct_end(Graphix_t)

/* ***************************************************************  */

#endif /* GRAPHIX_DEF_H__  */
