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


#ifndef KERNAL_DEF_H__
#define KERNAL_DEF_H__

#include "define.h"

header_ifndef(KERNAL)
header_define(KERNAL)

define_hex(KERNAL_IRQ_USERENTRY,        0314)
define_hex(KERNAL_IRQ_DEFAULT,          ea31)
define_hex(KERNAL_IRQ_RETURN,           ea81)

header_endif(KERNAL)

#endif /* KERNAL_DEF_H__  */
