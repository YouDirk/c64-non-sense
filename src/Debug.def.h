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


#ifndef DEBUG_DEF_H__
#define DEBUG_DEF_H__

#include "define.h"

header_ifndef(DEBUG)
header_define(DEBUG)

/* ***************************************************************  */

define_dec(_DEBUG_LIST_SIZE,            15)

typedef_enum_begin(_debug_msgtype_t)
  typedef_enum_hex(_debug_msgtype_t, _debug_error_e, 01)
  typedef_enum_hex(_debug_msgtype_t, _debug_warn_e,  02)
  typedef_enum_hex(_debug_msgtype_t, _debug_note_e,  03)
typedef_enum_end(_debug_msgtype_t)

typedef_struct_begin(_Debug_entry_t)
  typedef_struct_uint32(time)
  typedef_struct_enum(_debug_msgtype_t, type)
  typedef_struct_constchar_ptr(msg)
typedef_struct_end(_Debug_entry_t)

/* Structure of static members for module.  */
typedef_struct_begin(_Debug_t)
  typedef_struct_uint8(count)
  typedef_struct_nested_array(_Debug_entry_t, entry, _DEBUG_LIST_SIZE)
  typedef_struct_nested_ptr(_Debug_entry_t, end)
typedef_struct_end(_Debug_t)

/* ***************************************************************  */

/* Static members of this module.  */
extern_var(_Debug_t, _Debug)

/* ***************************************************************  */

header_endif(DEBUG)

#endif /* DEBUG_DEF_H__  */
