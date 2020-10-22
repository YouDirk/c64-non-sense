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


#include "Graphix.h"

#define _VIC_CTRL1_SCREENOFF_MASK       0x10
#define _VIC_CTRL1_BITMAPMODE_MASK      0x20
#define _VIC_ADDR_SCREENRAM_MASK        0xf0
#define _VIC_ADDR_BITMAP_MASK           0x0f

#define _CIA2_PRA_BANK_MASK             0x03

#define _BANK_NUMBER_VIC_REG            0x00
#define _VIC_BANK        ((void*) (0xc000 - _BANK_NUMBER_VIC*0x4000))

static struct {
  uint8_t ctrl1;                        /* control reg 1  */
  uint8_t addr;                         /* mapping: screen ram & bitmap  */
  uint8_t bordercolor;                  /* border color  */
} _vic_backups;

static struct {
  uint8_t pra;                          /* port A, VIC bank address  */
} _cia2_backups;

void
Graphix_init(void)
{
  /* backup needed VIC registers  */
  _vic_backups.ctrl1 = VIC.ctrl1;
  _vic_backups.addr = VIC.addr;
  _vic_backups.bordercolor = VIC.bordercolor;
  _cia2_backups.pra = CIA2.pra;

  /* black screen during initialization  */
  VIC.ctrl1 = _vic_backups.ctrl1 & ~_VIC_CTRL1_SCREENOFF_MASK;
  VIC.bordercolor = COLOR_BLACK;

  /* remap VIC memory  */
  CIA2.pra = (_cia2_backups.pra & ~_CIA2_PRA_BANK_MASK)
    | _BANK_NUMBER_VIC_REG;
  VIC.addr = (_VIC_ADDR_SCREENRAM_MASK & _GRAPHIX_SCREENRAM_ADDR_REG)
    | (_VIC_ADDR_BITMAP_MASK & _GRAPHIX_BITMAP_ADDR_REG);
}

void
Graphix_release(void)
{
  /* restore VIC memory mapping  */
  VIC.addr = _vic_backups.addr;
  CIA2.pra = _cia2_backups.pra;

  /* switch back to text mode  */
  VIC.bordercolor = _vic_backups.bordercolor;
  VIC.ctrl1 = _vic_backups.ctrl1;
}
