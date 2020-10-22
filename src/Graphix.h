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


#ifndef GRAPHIX_H__
#define GRAPHIX_H__

#include "common.h"

#define _GRAPHIX_BANK_NUMBER_VIC_REG    0x00
#define _GRAPHIX_SCREENRAM_ADDR_REG     0x00
#define _GRAPHIX_BITMAP_ADDR_REG        0x08

#define GRAPHIX_VIC_RAM                                               \
  ((void*) (0xc000 - _GRAPHIX_BANK_NUMBER_VIC_REG*0x4000))
#define GRAPHIX_SCREEN_RAM                                            \
  ((void*) ((unsigned) GRAPHIX_VIC_RAM + _GRAPHIX_SCREENRAM_ADDR_REG*0x04))
#define GRAPHIX_BITMAP_RAM                                            \
  ((void*) ((unsigned) GRAPHIX_VIC_RAM + _GRAPHIX_BITMAP_ADDR_REG*0x0400))

extern void Graphix_init(void);
extern void Graphix_release(void);

#endif /* GRAPHIX_H__  */
