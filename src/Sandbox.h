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


#ifndef SANDBOX_H__
#define SANDBOX_H__

#include "common.h"

/* *******************************************************************
 *
 * Stuff which has currently no component structure can be implemented
 * here.  For fast testing and staging code.
 */

/* Initialize this module.  */
extern void __fastcall__ Sandbox_init(void);

/* Deinitialize this module.  */
extern void __fastcall__ Sandbox_release(void);

/* ***************************************************************  */

/* Polls this module between engine ticks.  */
extern void __fastcall__ Sandbox_poll(void);

/* Let this module ticking.  */
extern void __fastcall__ Sandbox_tick(void);

/* Let this module ticking, graphix already rendered.  */
extern void __fastcall__ Sandbox_tick_low(void);

#endif /* SANDBOX_H__  */
