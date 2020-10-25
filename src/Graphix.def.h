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

define_hex(GRAPHIX_VIC_IRR,                  d019)
define_hex(GRAPHIX_VIC_IRR_RASTERLINE_MASK,  01)
define_hex(GRAPHIX_VIC_IRR_SHCOLLOSION_MASK, 02)
define_hex(GRAPHIX_VIC_IRR_SSCOLLOSION_MASK, 04)
define_hex(GRAPHIX_VIC_IRR_LIGHTPEN_MASK,    08)

define_hex(GRAPHIX_VIC_BORDERCOLOR,          d020)

#endif /* GRAPHIX_DEF_H__  */
