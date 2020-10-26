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
#include "chip_cia.gen.h"

/* default is CIA2_PRA_VICBANK_MEM0  */
#define _VICBANK                        CIA2_PRA_VICBANK_MEMC
/* default is VIC_ADDR_DEFAULT_SCREENRAM  */
#define _SCREENRAM_x0X40                0x00
/* default just 0x00 or 0x08 possible  */
#define _BITMAPRAM_x0X400               0x08

#define _VIC_RAM              ((void*) (0xc000 - _VICBANK*0x4000))
#define _SCREEN_RAM                                                  \
  ((uint8_t*) ((unsigned) _VIC_RAM + _SCREENRAM_x0X40*0x40))
#define _BITMAP_RAM                                                  \
  ((uint8_t*) ((unsigned) _VIC_RAM + _BITMAPRAM_x0X400*0x0400))


static struct {
  uint8_t pra;                          /* port A, VIC bank address  */
} _cia2_backups;


/* 'this' and a shadow copy of it  */
static Graphix_t _singleton, _shadow_4_isr;

Graphix_t* __fastcall__
Graphix_new(Graphix_initCallback_t init_callback)
{
  /* backup needed registers  */
  _cia2_backups.pra = CIA2.pra;

  /* black screen during initialization  */
  VIC.ctrl1 = VIC_CTRL1_DEFAULT & ~VIC_CTRL1_SCREEN_ON_MASK;
  VIC.bordercolor = VIC_COLOR_BLACK;

  /* remap VIC memory  */
  CIA2.pra = (_cia2_backups.pra & ~CIA2_PRA_VICBANK_MASK) | _VICBANK;
  VIC.addr = (VIC_ADDR_SCREENRAM_MASK & _SCREENRAM_x0X40)
    | (VIC_ADDR_BITMAP_MASK & _BITMAPRAM_x0X400);

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
  VIC.ctrl2 = VIC_CTRL2_DEFAULT;

  /* restore VIC memory mapping AND set character-set back to 1
   * (symbols, no lower case).
   */
  VIC.addr = VIC_ADDR_DEFAULT;
  CIA2.pra = _cia2_backups.pra;

  /* switch back into text mode  */
  VIC.bordercolor = VIC_BORDERCOLOR_DEFAULT;
  VIC.ctrl1 = VIC_CTRL1_DEFAULT;
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
  VIC.bordercolor = VIC_COLOR_RED;
#endif

  VIC.ctrl2 = VIC_CTRL2_MODE | _shadow_4_isr.scroll_x;
  VIC.ctrl1 = VIC_CTRL1_MODE | _shadow_4_isr.scroll_y;

#ifdef DEBUG_IRQ_RENDERTIME
  VIC.bordercolor = VIC_COLOR_BLACK;
#endif
}
