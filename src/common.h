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
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUG

/* To fast implementing stuff for tests, not productive  */
#  include <stdio.h>
#  include <stdlib.h>

#else /* #ifdef DEBUG  */

#endif /* #ifdef DEBUG  */

#define STRING_LEN                 256
typedef char                       string_t[STRING_LEN];

#endif /* COMMON_H__  */
