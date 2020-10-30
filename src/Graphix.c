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
#define _VICBANK                   CIA2_PRA_VICBANK_MEMC
/* default is VIC_ADDR_DEFAULT_SCREENRAM  */
#define _SCREENRAM_x0X40           0x00
/* default 0x07, but just 0x00 or 0x08 possible  */
#define _BITMAPRAM_x0X400          0x08

#define _VIC_RAM_ADDR              CIA2_PRA_VICBANK_ADDR(_VICBANK)
#define _VIC_RAM                   ((void*) _VIC_RAM_ADDR)
#define _SCREEN_RAM                                                  \
  ((uint8_t*) VIC_ADDR_SCREENRAM_ADDR(_VIC_RAM_ADDR, _SCREENRAM_x0X40))
#define _BITMAP_RAM                                                  \
  ((uint8_t*) VIC_ADDR_BITMAP_ADDR(_VIC_RAM_ADDR, _BITMAPRAM_x0X400))


/* 'this'  */
static Graphix_t _singleton;
/* the shadow copy of this.  Read by ISR.  */
static Graphix_t _shadow_isr;

bool Graphix_ispal;

/* ***************************************************************  */

Graphix_t* __fastcall__
Graphix_new(Graphix_initCallback_t init_callback)
{
  /* black screen  */
  VIC.ctrl1 = VIC_CTRL1_DEFAULT & ~VIC_CTRL1_SCREEN_ON_MASK;
  VIC.bordercolor = VIC_COLOR_BLACK;

  /* find out if we are on a PAL or NTSC machine?  Needed for Timer
   * configuration.
   */
  __asm__("rasterline_not_overflow:\n"
          "    lda %w\n"
          "rasterline_not_inc:\n"
          "    cmp %w\n"
          "    beq rasterline_not_inc\n"
          "    bmi rasterline_not_overflow\n"
          /* akku = rasterline_max (without bit 8)
           * 0x37 -> 0..311 rasterlines (PAL with VIC 6569)
           * 0x06 -> 0..262 rasterlines (NTSC with VIC 6567R8)
           * 0x05 -> 0..261 rasterlines (NTSC with VIC 6567R56A)
           */
          "    and #%b\n"
          /* akku = 0x30 if PAL, 0x00 if NTSC  */
          "    sta %v\n",
          VIC_RASTERLINE, VIC_RASTERLINE,
          (uint8_t) VIC_RASTERLINE_PAL_MASK, Graphix_ispal);

  /* remap VIC memory  */
  CIA2.pra = (CIA2_PRA_DEFAULT & ~CIA2_PRA_VICBANK_MASK) | _VICBANK;
  VIC.addr = (VIC_ADDR_SCREENRAM_MASK & _SCREENRAM_x0X40)
    | (VIC_ADDR_BITMAP_MASK & _BITMAPRAM_x0X400);

  /* xscroll and multicolor stuff  */
  VIC.ctrl2 = VIC_CTRL2_MODE;

  /* (no restore) rasterline, where an IRQ is triggered  */
  VIC.rasterline = VIC_RASTERLINE_SCREENEND;

  /* initialize 'this'  */
  _singleton.screen_ram = _SCREEN_RAM;
  _singleton.bitmap_ram = _BITMAP_RAM;

  _singleton.bordercolor = GRAPHIX_BLACK;
  _singleton.scroll_x = 0;
  _singleton.scroll_y = 0;

  /* initialize all other stuff  */
  init_callback(&_singleton);

  /* after init_callback(), so it´s possible to initizialize it
   * inside
   */
  Graphix_swapBuffers();

  /* set screen on and VIC IRQs go!  */
  VIC.ctrl1 = VIC_CTRL1_MODE;
  VIC.imr = VIC_IMR_IRQMODE;

  return &_singleton;
}

void __fastcall__
Graphix_release(Graphix_releaseCallback_t release_callback)
{
  /* Disable VIC IRQs first, then black screen  */
  VIC.imr = VIC_IMR_DISABLEALL_MASK;
  VIC.ctrl1 = VIC_CTRL1_MODE & ~VIC_CTRL1_SCREEN_ON_MASK;

  /* release all other stuff  */
  release_callback();

  /* xscroll and multicolor stuff  */
  VIC.ctrl2 = VIC_CTRL2_DEFAULT;

  /* restore VIC memory mapping AND set character-set back to 1
   * (symbols, no lower case)
   */
  VIC.addr = VIC_ADDR_DEFAULT;
  CIA2.pra = CIA2_PRA_DEFAULT;

  /* switch back into text mode, set screen on   */
  VIC.bordercolor = VIC_BORDERCOLOR_DEFAULT;
  VIC.ctrl1 = VIC_CTRL1_DEFAULT;
}

/* ***************************************************************  */

void __fastcall__
Graphix_swapBuffers(void)
{
  _singleton.scroll_x &= VIC_CTRL2_XSCROLL_MASK;
  _singleton.scroll_y &= VIC_CTRL1_YSCROLL_MASK;

  /* mask VIC rasterline IRQs  */
  /* commented out, too inaccurate raster timing  */
  /*VIC.imr = VIC_IMR_IRQMODE & ~VIC_IMR_RASTERLINE_MASK;  */

  memcpy(&_shadow_isr, &_singleton, sizeof(Graphix_t));

  /* unmask VIC rasterline IRQs  */
  /* commented out, too inaccurate raster timing  */
  /*VIC.imr = VIC_IMR_IRQMODE;  */
}

/* ***************************************************************  */

/* Just use inline assembler instructions here.  This function will be
 * called by ISR during IRQ.  Maybe the compiler generate calls to the
 * runtime library, these are NOT MULTI-THREADING COMPATIBLE.
 *
 * We are using inline assembler here to have access to the members of
 * _shadow_isr, what is not possible with clean assembler code.
 */
void __fastcall__
Graphix_rasterline_isr(void)
{
#ifdef DEBUG_IRQ_RENDERTIME
  __asm__("    lda #%b\n"
          "    sta %w\n",
          VIC_COLOR_RED, VIC_BORDERCOLOR);
#endif

  __asm__("    lda %v+%w\n"
          "    ora #%b\n"
          "    sta %w\n",
          _shadow_isr, offsetof(Graphix_t, scroll_y), VIC_CTRL1_MODE,
          VIC_CTRL1);
  __asm__("    lda %v+%w\n"
          "    ora #%b\n"
          "    sta %w\n",
          _shadow_isr, offsetof(Graphix_t, scroll_x), VIC_CTRL2_MODE,
          VIC_CTRL2);

  /* must be the last statement, for correct DEBUG_IRQ_RENDERTIME  */
  __asm__("    lda %v+%w\n"
          "    sta %w\n",
          _shadow_isr, offsetof(Graphix_t, bordercolor),
          VIC_BORDERCOLOR);
#ifdef DEBUG_IRQ_RENDERTIME
  /* nothing to do, because VIC.bordercolor will be set above  */
#endif
}
