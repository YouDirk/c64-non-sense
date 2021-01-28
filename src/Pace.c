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


#include "Pace.h"

/* *******************************************************************
 *
 * Every PACE has a 16 bit STATUS variable which saves the current
 * state between engine ticks.  It looks like this
 *
 *   | 15 | 14 13 12 | 11 10  9  8 ||  7  6  5  4  3  2 |  1  0  |
 *   |-----------------------------------------------------------|
 *   |    |          |     mode    ||    tick-counter   |        |
 *   '--|------|--------------------------------------------|----'
 *      |      |                                            |
 *      |     pixel pace                                  frac-counter
 *     sign bit
 */



/* ***************************************************************  */

void __fastcall__
Pace_new(Pace_t* pace,
         uint8_t pace_max, uint8_t brakerate, uint8_t delay)
{
}

void __fastcall__
Pace_delete(Pace_t* pace)
{
}

/* ***************************************************************  */

void __fastcall__
Pace_impulse_pos(Pace_t* pace)
{
}

void __fastcall__
Pace_impulse_neg(Pace_t* pace)
{
}
