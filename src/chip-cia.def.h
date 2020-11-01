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

header_ifndef(CHIP_CIA)
header_define(CHIP_CIA)

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
define_hex(CIA2_PRA_VICBANK_ADDR0,      0000)
define_hex(CIA2_PRA_VICBANK_MEM4,       02)
define_hex(CIA2_PRA_VICBANK_ADDR4,      4000)
define_hex(CIA2_PRA_VICBANK_MEM8,       01)
define_hex(CIA2_PRA_VICBANK_ADDR8,      8000)
define_hex(CIA2_PRA_VICBANK_MEMC,       00)
define_hex(CIA2_PRA_VICBANK_ADDRC,      c000)
define_hex(CIA2_PRA_VICBANK_STEP,       4000)
define_hex(CIA2_PRA_USERPORT_MASK,      04)
define_hex(CIA2_PRA_SERIALOUT_MASK,     38)
define_hex(CIA2_PRA_SERIALIN_MASK,      c0)
define(CIA2_PRA_DEFAULT, \
       (CIA2_PRA_SERIALIN_MASK | CIA2_PRA_USERPORT_MASK
        | CIA2_PRA_VICBANK_MEM0))

/* CIA2_PRA_VICBANK_ADDR(CIA2_PRA_VICBANK_MEM?)  */
macro_arg1(CIA2_PRA_VICBANK_ADDR,                                    \
           (CIA2_PRA_VICBANK_ADDRC - (arg1)*CIA2_PRA_VICBANK_STEP))

/* ***************************************************************  */

define_hex(CIA1_DDRA,                   dc02)
define_hex(CIA2_DDRA,                   dd02)

define_hex(CIA1_DDRB,                   dc03)
define_hex(CIA2_DDRB,                   dd03)

define_hex(CIA1_TA_LO,                  dc04)
define_hex(CIA1_TA_HI,                  dc05)
define_hex(CIA1_TA_DEFAULT_PAL,         4025) /* default (PAL) setting  */
define_hex(CIA1_TA_DEFAULT_NTSC,        4295) /* (NTSC)  */

define_hex(CIA2_TA_LO,                  dd04)
define_hex(CIA2_TA_HI,                  dd05)

define_hex(CIA1_TB_LO,                  dc06)
define_hex(CIA1_TB_HI,                  dc07)

define_hex(CIA2_TB_LO,                  dd06)
define_hex(CIA2_TB_HI,                  dd07)

define_hex(CIA1_ICR,                    dc0d)
define_hex(CIA2_ICR,                    dd0d)
define_hex(CIA_ICR_TIMERAZERO_MASK,     01)
define_hex(CIA_ICR_TIMERBZERO_MASK,     02)
define_hex(CIA_ICR_RTCALARM_MASK,       04)
define_hex(CIA_ICR_SDREMPTY_MASK,       08)
define_hex(CIA1_ICR_CASS_SERIAL_MASK,   10)
define_hex(CIA2_ICR_RS232_MASK,         10)
define_hex(CIA1_ICR_IRQGLOBAL_MASK,     80)
define_hex(CIA2_ICR_NMIGLOBAL_MASK,     80)
define_hex(CIA_ICR_ALL,                 1f)

/* CIA_ICR_MASK(CIA_ICR_*_MASK)  */
macro_arg1(CIA_ICR_MASK,       ((arg1) & ~CIA1_ICR_IRQGLOBAL_MASK))
/* CIA_ICR_UNMASK(CIA_ICR_*_MASK)  */
macro_arg1(CIA_ICR_UNMASK,     ((arg1) | CIA1_ICR_IRQGLOBAL_MASK))

define(CIA1_ICR_IRQMODE,                                                \
       (CIA1_ICR_IRQGLOBAL_MASK | CIA_ICR_TIMERAZERO_MASK))

define_hex(CIA1_CRA,                    dc0e)
define_hex(CIA2_CRA,                    dd0e)
define_hex(CIA_CRA_CLKCNT_MASK,         20)
define_hex(CIA_CRA_SPWRITE_MASK,        40)
define_hex(CIA_CRA_50HZ_MASK,           80)

define_hex(CIA1_CRB,                    dc0f)
define_hex(CIA2_CRB,                    dd0f)
define_hex(CIA_CRB_CLK_MASK,            60)
define_hex(CIA_CRB_CLK_SYS,             00)
define_hex(CIA_CRB_CLK_CNT,             20)
define_hex(CIA_CRB_CLK_TA,              40)
define_hex(CIA_CRB_CLK_TACNT,           60)
define_hex(CIA_CRB_TODALARM_MASK,       80)

define_hex(CIA_CRAB_START_MASK,         01)
define_hex(CIA_CRAB_PBENABLE_MASK,      02)
define_hex(CIA_CRAB_PBHIGH_MASK,        04)
define_hex(CIA_CRAB_UNDERSTOP_MASK,     08)
define_hex(CIA_CRAB_LDLATCH_MASK,       10)

define(CIA1_CRA_DEFAULT,                                             \
       (CIA_CRAB_START_MASK | CIA_CRAB_LDLATCH_MASK))

define(CIA1_CRA_MODE,                                                \
       (CIA_CRAB_START_MASK | CIA_CRAB_PBENABLE_MASK                 \
        | CIA_CRAB_LDLATCH_MASK))

header_endif(CHIP_CIA)

#endif /* CHIP_CIA_DEF_H__  */
