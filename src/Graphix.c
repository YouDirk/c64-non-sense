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

#include "chip_vic.gen.h"

#define _CIA2_PRA_BANK_MASK             0x03

/* default is 0x03  */
#define _BANK_NUMBER_VIC_REG            0x00
/* default is 0x10  */
#define _SCREENRAM_ADDR_REG             0x00
/* default is 0x07, but 0x00 or 0x08 should be used  */
#define _BITMAP_ADDR_REG                0x08

#define _VIC_RAM                                                     \
  ((void*) (0xc000 - _BANK_NUMBER_VIC_REG*0x4000))
#define _SCREEN_RAM                                                  \
  ((uint8_t*) ((unsigned) _VIC_RAM + _SCREENRAM_ADDR_REG*0x40))
#define _BITMAP_RAM                                                  \
  ((uint8_t*) ((unsigned) _VIC_RAM + _BITMAP_ADDR_REG*0x0400))


static struct {
  uint8_t ctrl1;                        /* control reg 1  */
  uint8_t ctrl2;                        /* control reg 2  */
  uint8_t addr;                         /* mapping: screen ram & bitmap  */
  uint8_t bordercolor;                  /* border color  */
} _vic_backups;

static struct {
  uint8_t pra;                          /* port A, VIC bank address  */
} _cia2_backups;


/* 'this' and a shadow copy of it  */
static Graphix_t _singleton, _shadow_4_isr;

Graphix_t* __fastcall__
Graphix_new(Graphix_initCallback_t init_callback)
{
  /* backup needed registers  */
  _vic_backups.ctrl1 = VIC.ctrl1;
  _vic_backups.ctrl2 = VIC.ctrl2;
  _vic_backups.addr = VIC.addr;
  _vic_backups.bordercolor = VIC.bordercolor;
  _cia2_backups.pra = CIA2.pra;

  /* black screen during initialization  */
  VIC.ctrl1 = _vic_backups.ctrl1 & ~VIC_CTRL1_SCREEN_ON_MASK;
  VIC.bordercolor = COLOR_BLACK;

  /* remap VIC memory  */
  CIA2.pra = (_cia2_backups.pra & ~_CIA2_PRA_BANK_MASK)
    | _BANK_NUMBER_VIC_REG;
  VIC.addr = (VIC_ADDR_SCREENRAM_MASK & _SCREENRAM_ADDR_REG)
    | (VIC_ADDR_BITMAP_MASK & _BITMAP_ADDR_REG);

  /* initialize all video rams  */
  init_callback(_SCREEN_RAM, _BITMAP_RAM);

  /* mode description above  */
  VIC.ctrl2 = VIC_CTRL2_MODE;

  /* (no backup needed) rasterline, where an IRQ is triggered  */
  VIC.rasterline = VIC_RASTERLINE_SCREENEND;

  /* mode description above  */
  VIC.ctrl1 = VIC_CTRL1_MODE;

  /* initialize 'this'  */
  _singleton.scroll_x = 0;
  _singleton.scroll_y = 0;

  /* initialize _shadow_4_isr  */
  Graphix_swapBuffers();

  /* (no backup needed) VIC IRQs GO!  */
  VIC.imr = VIC_IMR_IRQS;

  return &_singleton;
}

void __fastcall__
Graphix_release(void)
{
  /* Disable VIC IRQs first!  */
  VIC.imr = VIC_IMR_DISABLEALL_MASK;

  /* black screen during deinitialization  */
  VIC.ctrl1 = VIC_CTRL1_MODE & ~VIC_CTRL1_SCREEN_ON_MASK;

  /* used for xscroll and multicolor stuff  */
  VIC.ctrl2 = _vic_backups.ctrl2;

  /* restore VIC memory mapping AND set character-set back to 1
   * (symbols, no lower case).
   */
  VIC.addr = (_vic_backups.addr & VIC_ADDR_SCREENRAM_MASK)
    | (VIC_ADDR_BITMAP_MASK & VIC_ADDR_BITMAP_CHARSET1);
  CIA2.pra = _cia2_backups.pra;

  /* switch back into text mode  */
  VIC.bordercolor = _vic_backups.bordercolor;
  VIC.ctrl1 = _vic_backups.ctrl1;
}

void __fastcall__
Graphix_swapBuffers(void)
{
  _singleton.scroll_x &= VIC_CTRL2_XSCROLL_MASK;
  _singleton.scroll_y &= VIC_CTRL1_YSCROLL_MASK;


  /* Disable Rasterline IRQs.  Do not use the SEI and CLI assembler
   * instructions!  SEI/CLI does also disable the timer interrupts
   * which causes lags to the whole system.
   */
  /* commented out, too inaccurate timing of the raster line  */
  /* VIC.imr = VIC_IMR_IRQS & ~VIC_IMR_RASTERLINE_MASK;  */

  memcpy(&_shadow_4_isr, &_singleton, sizeof(Graphix_t));

  /* Rasterline IRQs can come back
   */
  /* commented out, too inaccurate timing of the raster line  */
  /* VIC.imr = VIC_IMR_IRQS;  */
}

void __fastcall__
_Graphix_render_isr(void)
{
#ifdef DEBUG_IRQ_RENDERTIME
  VIC.bordercolor = COLOR_RED;
#endif

  VIC.ctrl2 = VIC_CTRL2_MODE | _shadow_4_isr.scroll_x;
  VIC.ctrl1 = VIC_CTRL1_MODE | _shadow_4_isr.scroll_y;

#ifdef DEBUG_IRQ_RENDERTIME
  VIC.bordercolor = COLOR_BLACK;
#endif
}
