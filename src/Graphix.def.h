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

header_ifndef(GRAPHIX)
header_define(GRAPHIX)

/* ***************************************************************  */

/* graphic buffer structure  */
typedef_struct_begin(Graphix_buffer_t)
  typedef_struct_uint8_ptr(screen_ram)
  typedef_struct_uint8_ptr(bitmap_ram)

  typedef_struct_uint8(bordercolor)
  typedef_struct_int8(scroll_x)
  typedef_struct_int8(scroll_y)
typedef_struct_end(Graphix_buffer_t)

/* VIC-II revision structure  */
typedef_enum_begin(Graphix_vicrev_t)
  /* default PAL revision  (europe)  */
  typedef_enum_hex(Graphix_vicrev_t, Graphix_vicrev_pal6569_e,      37)
  /* Drean PAL-N revision (south america)  */
  typedef_enum_hex(Graphix_vicrev_t, Graphix_vicrev_paln6572_e,     40)
  /* default NTSC revision (usa)  */
  typedef_enum_hex(Graphix_vicrev_t, Graphix_vicrev_ntsc6567r8_e,   06)
  /* old NTSC revision  */
  typedef_enum_hex(Graphix_vicrev_t, Graphix_vicrev_ntsc6567r56a_e, 05)
typedef_enum_end(Graphix_vicrev_t)

/* ***************************************************************  */

/* Used to select a character set from C64 Character ROM.  */
typedef_enum_begin(Graphix_charset_t)
  typedef_enum_hex(Graphix_charset_t, Graphix_charset1_symbols_e,   00)
  typedef_enum_hex(Graphix_charset_t, Graphix_charset2_lower_e,     01)
typedef_enum_end(Graphix_charset_t)

/* ***************************************************************  */

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Graphix_set_t)
  /* Charset which will be if set after termination of the engine.  */
  typedef_struct_enum(Graphix_charset_t,     charset_exit)
typedef_struct_end(Graphix_set_t)

/* Structure of static members for module.  */
typedef_struct_begin(Graphix_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Graphix_set_t,       set)

  /* TRUE if C64 has a PAL VIC, otherwise we are on a NTSC machine.  */
  typedef_struct_uint8(                      is_pal)

  /* Revision of VIC-II graphic chip.  */
  typedef_struct_enum(Graphix_vicrev_t,      vic_revision)

  /* The logical graphic buffer.  */
  typedef_struct_nested(Graphix_buffer_t,    buffer)

typedef_struct_end(Graphix_t)

/* ***************************************************************  */

/* Static members of this module.  */
extern_var(Graphix_t,                        Graphix)

/* ***************************************************************  */

#ifndef CONF_DOUBLE_BUFFERING
  extern_var(Graphix_buffer_t*, Graphix_buffer_shared_ptr)
#endif /* CONF_DOUBLE_BUFFERING  */
extern_var(Graphix_buffer_t*, Graphix_buffer_back_ptr)

/* ***************************************************************  */

header_endif(GRAPHIX)

#endif /* GRAPHIX_DEF_H__  */
