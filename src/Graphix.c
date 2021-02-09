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

#include "chip-vic.gen.h"
#include "chip-cia.gen.h"

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

/* The shared and back buffer for triple buffering, read by ISR.  */
#ifndef CONF_DOUBLE_BUFFERING
  static Graphix_buffer_t _Graphix_buffers_sharedback[2];
#else /* CONF_DOUBLE_BUFFERING  */
  static Graphix_buffer_t _Graphix_buffers_sharedback[1];
#endif /* CONF_DOUBLE_BUFFERING  */

/* Points to the current shared/back buffer of triple buffering.  Set
 * by Timer_a_isr() to reduce random noise in timing.  Read by
 * Graphix_rasterline_isr() for rendering.
 */
#ifndef CONF_DOUBLE_BUFFERING
  Graphix_buffer_t* Graphix_buffer_shared_ptr;
#endif /* CONF_DOUBLE_BUFFERING  */
Graphix_buffer_t* Graphix_buffer_back_ptr;

/* ***************************************************************  */

/* Static members of this module.  */
Graphix_t Graphix;

void __fastcall__
Graphix_init(Graphix_initCallback_t init_callback)
{
  /* black screen  */
  VIC.ctrl1 = VIC_CTRL1_DEFAULT & ~VIC_CTRL1_SCREEN_ON_MASK;
  VIC.bordercolor = VIC_COLOR_BLACK;

  _Graphix_init_vic_detect();
  // TODO printf("%02x\n", Graphix.vic_revision);

  /* remap VIC memory  */
  CIA2.pra = (CIA2_PRA_DEFAULT & ~CIA2_PRA_VICBANK_MASK) | _VICBANK;
  VIC.addr = (VIC_ADDR_SCREENRAM_MASK & _SCREENRAM_x0X40)
    | (VIC_ADDR_BITMAP_MASK & _BITMAPRAM_x0X400);

  /* xscroll and multicolor stuff  */
  VIC.ctrl2 = VIC_CTRL2_MODE;

  /* (no restore) rasterline, where an IRQ is triggered  */
  VIC.rasterline = VIC_RASTERLINE_MODE;

  /* initialize Graphix.buffer  */
  Graphix.buffer.screen_ram = _SCREEN_RAM;
  Graphix.buffer.bitmap_ram = _BITMAP_RAM;

  Graphix.buffer.bordercolor = GRAPHIX_BLACK;
  Graphix.buffer.scroll_x = 0;
  Graphix.buffer.scroll_y = 0;

  /* initialize all other stuff  */
  init_callback();

  /* Initialize buffers for triple buffering.  This is done after
   * init_callback() to make it possible that the callee initialize
   * these.
   */
#ifndef CONF_DOUBLE_BUFFERING
  Graphix_buffer_shared_ptr = &_Graphix_buffers_sharedback[1];
#endif /* CONF_DOUBLE_BUFFERING  */
  Graphix_buffer_back_ptr = &_Graphix_buffers_sharedback[0];
  Graphix_buffer_swap();
  /* If the high bytes of GRAPHIX_BUFFER_SHARED/BACK_PTR are equal,
   * then reading these pointers are atomar operations.
   */
#if defined(DEBUG) && !defined(CONF_DOUBLE_BUFFERING)
  if (((uint16_s) Graphix_buffer_shared_ptr).byte_high
      != ((uint16_s) Graphix_buffer_back_ptr).byte_high) {
    DEBUG_ERROR("graphix, buffers not aligned!");
  }
#endif /* defined(DEBUG) && !defined(CONF_DOUBLE_BUFFERING)  */

  /* set screen on and VIC IRQs go!  */
  VIC.ctrl1 = VIC_CTRL1_MODE;
  VIC.imr = VIC_IMR_IRQMODE;
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
Graphix_buffer_swap(void)
{
  Graphix.buffer.scroll_x &= VIC_CTRL2_XSCROLL_MASK;
  Graphix.buffer.scroll_y &= VIC_CTRL1_YSCROLL_MASK;

  /* -----------------------------------------------------------------
   *
   * Maybe a lock is needed here if something goes wrong with during
   * triple buffering.
   */
  DEBUG_RENDERTIME_BEGIN(VIC_COLOR_VIOLET);

  /* pushing GRAPHIX_BUFFER_SHARED_PTR to stack is atomar, because the
   * high byte equals GRAPHIX_BUFFER_BACK_PTR, as ASSERTED in
   * GRAPHIX_INIT()
   */
#ifndef CONF_DOUBLE_BUFFERING
  memcpy(Graphix_buffer_shared_ptr, &Graphix.buffer,
         sizeof(Graphix_buffer_t));
#else /* CONF_DOUBLE_BUFFERING  */
  memcpy(Graphix_buffer_back_ptr, &Graphix.buffer,
         sizeof(Graphix_buffer_t));
#endif /* CONF_DOUBLE_BUFFERING  */

  DEBUG_RENDERTIME_END();
  /* end of critical section
   *
   * -----------------------------------------------------------------
   */
}
