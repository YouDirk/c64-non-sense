/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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


#ifndef DEBUG_H__
#define DEBUG_H__

#include "common.h"

/* ***************************************************************  */

#ifdef DEBUG_IRQ_RENDERTIME
#  define DEBUG_RENDERTIME_BEGIN(color)                              \
  __asm__("  lda %w\n"                                               \
          "  pha\n"                                                  \
          "  lda #%b\n"                                              \
          "  sta %w\n"                                               \
          , VIC_BORDERCOLOR, (color), VIC_BORDERCOLOR)
#  define DEBUG_RENDERTIME_END()                                     \
  __asm__("  pla\n"                                                  \
          "  sta %w\n"                                               \
          , VIC_BORDERCOLOR)
#else /* DEBUG_IRQ_RENDERTIME  */
#  define DEBUG_RENDERTIME_BEGIN(color)
#  define DEBUG_RENDERTIME_END()
#endif /* DEBUG_IRQ_RENDERTIME  */

/* ***************************************************************  */

#ifdef DEBUG

/* For fast implementing stuff for tests, not productive  */
#  include <stdio.h>
#  include <stdlib.h>

/* Call these macros for debugging output :)  */
#  define DEBUG_INIT()             Debug_init()
#  define DEBUG_RELEASE_PRINT()    Debug_release_print()

#  define DEBUG_ERROR(msg)         Debug_error(msg)
#  define DEBUG_WARN(msg)          Debug_warn(msg)
#  define DEBUG_NOTE(msg)          Debug_note(msg)

/* -------------------------------------------------------------------
 * Do not call directly!  Call the macros DEBUG_*() above instead.
 */
extern void __fastcall__ Debug_init(void);
extern void __fastcall__ Debug_release_print(void);
extern void __fastcall__ Debug_error(const char* msg);
extern void __fastcall__ Debug_warn(const char* msg);
extern void __fastcall__ Debug_note(const char* msg);

/* ***************************************************************  */
#else /* DEBUG  */
#  define DEBUG_INIT()
#  define DEBUG_RELEASE_PRINT()
#  define DEBUG_ERROR(msg)
#  define DEBUG_WARN(msg)
#  define DEBUG_NOTE(msg)
#endif /* DEBUG  */

#endif /* DEBUG_H__  */
