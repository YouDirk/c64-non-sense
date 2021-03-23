/* C64 NonSense, the C64 Game Engine.
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


#ifndef AASANDBOX_H__
#define AASANDBOX_H__

#include "common.h"

/* *******************************************************************
 *
 * Stuff which has currently no component structure can be implemented
 * here.  For fast testing and staging code.
 */

/* Initialize this component.  */
extern void __fastcall__ AASandbox_init(void);

/* Deinitialize this component.  */
extern void __fastcall__ AASandbox_release(void);

/* ***************************************************************  */

/* Polls this component between engine ticks.  */
extern void __fastcall__ AASandbox_poll(void);

/* Let this component ticking.  */
extern void __fastcall__ AASandbox_tick(void);

/* Let this component ticking, graphix already rendered.  */
extern void __fastcall__ AASandbox_tick_low(void);

#endif /* AASANDBOX_H__  */
