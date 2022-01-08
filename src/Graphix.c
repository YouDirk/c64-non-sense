/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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

#include "EngineConfig.h"

/* ***************************************************************  */

/* The shared and back buffer for triple buffering, read by ISR.  */
#define _GRAPHIX_BUFFERS_SHAREDBACK_0                                \
                              (((Graphix_buffer_t*) &GRAPHIX_RAM) + 0)

/* --------------------------------------  */
#ifndef CONF_DOUBLE_BUFFERING
#  define _GRAPHIX_BUFFERS_SHAREDBACK_1                              \
                              (((Graphix_buffer_t*) &GRAPHIX_RAM) + 1)
#endif /* CONF_DOUBLE_BUFFERING  */
/* --------------------------------------  */

#define _GRAPHIX_BUFFERS_SHAREDBACK_END                              \
                       (GRAPHIX_RAM_RVAL + 2*sizeof(Graphix_buffer_t))

#define _GRAPHIX_BUFFERS_END           _GRAPHIX_BUFFERS_SHAREDBACK_END

#if _GRAPHIX_BUFFERS_END > (GRAPHIX_RAM_RVAL + GRAPHIX_RAM_BUFSIZE)
#  error "Graphix RAM is overflowing!"
#endif

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

  /* remap VIC memory  */
  CIA2.pra = (CIA2_PRA_DEFAULT & ~CIA2_PRA_VICBANK_MASK)
             | _GRAPHIX_VICBANK_CIA2PRA;
  VIC.addr
    = (_GRAPHIX_SCREENRAM_x0X400_VICADDR << VIC_ADDR_SCREENRAM_SHIFT)
    | (VIC_ADDR_BITMAP_MASK & _GRAPHIX_BITMAPRAM_x0X400_VICADDR);

  /* xscroll and multicolor stuff  */
  VIC.ctrl2 = VIC_CTRL2_MODE;

  /* (no restore) rasterline, where an IRQ is triggered  */
  VIC.rasterline = VIC_RASTERLINE_MODE;

  /* initialize Graphix.set  */
  Graphix.set.charset_exit = EngineConfig.charset_exit;

  /* initialize Graphix.buffer  */
  Graphix.buffer.set.scroll_y   = VIC_CTRL1_YSCROLL_DEFAULT;
  Graphix.buffer.set.scroll_x   = VIC_CTRL2_XSCROLL_DEFAULT;

  Graphix.buffer.set.bordercolor = Graphix_black;

  /* initialize all other stuff  */
  init_callback();

  /* Initialize all Graphix buffers for triple buffering.  This is
   * done after init_callback() to make it possible that the callee
   * initialize these.
   */
#ifndef CONF_DOUBLE_BUFFERING
  Graphix_buffer_shared_ptr = _GRAPHIX_BUFFERS_SHAREDBACK_0;
  Graphix_buffer_swap();
  Graphix_buffer_shared_ptr = _GRAPHIX_BUFFERS_SHAREDBACK_1;
#endif /* CONF_DOUBLE_BUFFERING  */
  Graphix_buffer_back_ptr = _GRAPHIX_BUFFERS_SHAREDBACK_0;
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

  /* VIC IRQs go!  Screen will be set to ON during first rasterline
   * IRQ, to make sure that no uninitialized trash will be outputted.
   */
  VIC.ctrl1 = VIC_CTRL1_MODE & ~VIC_CTRL1_SCREEN_ON_MASK;
  VIC.imr   = VIC_IMR_IRQMODE;
}

void __fastcall__
Graphix_release(Graphix_releaseCallback_t release_callback)
{
  /* Disable VIC IRQs first, then black screen  */
  VIC.imr   = VIC_IMR_DISABLEALL_MASK;
  VIC.ctrl1 = VIC_CTRL1_MODE & ~VIC_CTRL1_SCREEN_ON_MASK;

  /* release all other stuff  */
  release_callback();

  /* xscroll and multicolor stuff  */
  VIC.ctrl2 = VIC_CTRL2_DEFAULT;

  /* restore VIC memory mapping AND set character-set back to symbols
   * or lower case
   */

  switch (Graphix.set.charset_exit) {
  case Graphix_charset2_lowercase_e:
    VIC.addr = (VIC_ADDR_DEFAULT
                & ~VIC_ADDR_BITMAP_MASK) | VIC_ADDR_BITMAP_CHARSET2;
    break;
  default:
    DEBUG_ERROR("graphix release, exit charset unknown");
  case Graphix_charset1_symbols_e:
    VIC.addr = (VIC_ADDR_DEFAULT
                & ~VIC_ADDR_BITMAP_MASK) | VIC_ADDR_BITMAP_CHARSET1;
    break;
  }
  CIA2.pra = CIA2_PRA_DEFAULT;

  /* switch back into text mode, set screen on   */
  VIC.bordercolor = VIC_BORDERCOLOR_DEFAULT;
  VIC.ctrl1       = VIC_CTRL1_DEFAULT;
}

/* ***************************************************************  */
