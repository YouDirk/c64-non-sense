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

#include <c64.h>
#include <peekpoke.h>
#include <conio.h>

#ifdef DEBUG
#else
#endif

#define TRUE                       ((bool) 1)
#define FALSE                      ((bool) 0)
#define STRING_LEN                 256

typedef int                        bool;
typedef char                       string_t[STRING_LEN];

#endif /* COMMON_H__  */
