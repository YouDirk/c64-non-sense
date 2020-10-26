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


#ifndef CHIP_CIA_DEF_H__
#define CHIP_CIA_DEF_H__

#include "define.h"

define_hex(CIA1_BASE,                   dc00)
define_hex(CIA2_BASE,                   dd00)

/* ***************************************************************  */

define_hex(CIA1_PRA,                    dc00)
define_hex(CIA1_PRB,                    dc01)

/* ------------------------------
 * if CIA1 DDR[AB] set to 'read/write'
 */
define_hex(CIA1_PRA_KEYCOL_MASK,        ff)
define_hex(CIA1_PRB_KEYROW_MASK,        ff)

/* ------------------------------
 * if CIA1 DDR[AB] set to 'read'
 */
define_hex(CIA1_PRAB_JOY_MASK,          1f)
define_hex(CIA1_PRAB_JOYUP_MASK,        01)
define_hex(CIA1_PRAB_JOYDOWN_MASK,      02)
define_hex(CIA1_PRAB_JOYLEFT_MASK,      04)
define_hex(CIA1_PRAB_JOYRIGHT_MASK,     08)
define_hex(CIA1_PRAB_JOYBTN1_MASK,      10)

define_hex(CIA1_PRAB_LIGHTPBTN1_MASK,   10)

define_hex(CIA1_PRA_PADD_MASK,          cc)
define_hex(CIA1_PRA_PADDBTN1_MASK,      04)
define_hex(CIA1_PRA_PADDBTN2_MASK,      08)
define_hex(CIA1_PRA_PADDSWA_MASK,       40)
define_hex(CIA1_PRA_PADDSWB_MASK,       80)

define_hex(CIA1_PRB_TIMERTOGGLE_MASK,   c0)
define_hex(CIA1_PRB_TIMERATOGGLE_MASK,  40)
define_hex(CIA1_PRB_TIMERBTOGGLE_MASK,  80)

/* end of CIA1 PR[AB]
 * ------------------------------
 */

define_hex(CIA2_PRA,                    dd00)
define_hex(CIA2_PRA_VICBANK_MASK,       03)
define_hex(CIA2_PRA_VICBANK_MEM0,       03)
define_hex(CIA2_PRA_VICBANK_MEM4,       02)
define_hex(CIA2_PRA_VICBANK_MEM8,       01)
define_hex(CIA2_PRA_VICBANK_MEMC,       00)

/* ***************************************************************  */

define_hex(CIA1_DDRA,                   dc02)
define_hex(CIA2_DDRA,                   dd02)

define_hex(CIA1_DDRB,                   dc03)
define_hex(CIA2_DDRB,                   dd03)

#endif /* CHIP_CIA_DEF_H__  */
