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


#ifndef ENGINE_DEF_H__
#define ENGINE_DEF_H__

#include "define.h"

header_ifndef(ENGINE)
header_define(ENGINE)

/* ***************************************************************  */

/* Primitive datatype to store time.  Use the macros in Engine.h to
 * convert a timestamp_t into a real world time, i.e. milliseconds.
 */
typedef_uint32(                                        timestamp_t)

/* Primitive datatype to store counts of engine ticks.  Use the macros
 * in Engine.h to convert a ticks_t into a time, i.e. milliseconds.
 */
typedef_uint32(                                        ticks_t)

/* ***************************************************************  */

define_dec(ENGINE_EXIT_SUCCESS,                        0)
define_dec(ENGINE_EXIT_FAILURE,                        1)
define_hex(ENGINE_EXIT_FAILURE_ENGINE,                 7f)
define_dec(ENGINE_EXIT_NOTERMINATION,                 -1)

/* Configuration variables which can be set directly, without needing
 * to call setter functions.
 */
typedef_struct_begin(Engine_set_t)

  /* If >= 0 then the engine will terminate with this exit code at the
   * end of the current/next tick.
   *
   * set: 0 or ENGINE_EXIT_SUCCESS for regularly termination
   */
  typedef_struct_int8(                                 exit_code)

typedef_struct_end(Engine_set_t)

/* ***************************************************************  */

/* Structure of static members for module.  */
typedef_struct_begin(Engine_t)
  /* Some writable member variables.  */
  typedef_struct_nested(Engine_set_t,                  set)

  /* Time(-stamp) of the last poll for an engine tick.  */
  typedef_struct_primit(timestamp_t,                   poll_time)

  /* Time(-stamp) of the last engine tick.  */
  typedef_struct_primit(timestamp_t,                   tick_time)

  /* Number of engine ticks since init/reset.  */
  typedef_struct_primit(ticks_t,                       tick_count)

typedef_struct_end(Engine_t)

/* ***************************************************************  */

/* Static members of this module.  */
extern_var(Engine_t,                                   Engine)

/* ***************************************************************  */

header_endif(ENGINE)

#endif /* ENGINE_DEF_H__  */
