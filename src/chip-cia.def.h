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


#ifndef CHIP_CIA_DEF_H__
#define CHIP_CIA_DEF_H__

#include "define.h"

header_ifndef(CHIP_CIA)
header_define(CHIP_CIA)

register_void_ptr(dc00,                      CIA1_BASE)
register_void_ptr(dd00,                      CIA2_BASE)

/* ***************************************************************  */

register_uint8(dc00,                         CIA1_PRA)
register_uint8(dc01,                         CIA1_PRB)

define_hex(CIA1_PRA_DEFAULT,                 7f)
define_hex(CIA1_PRB_DEFAULT,                 ff)

/* ------------------------------
 * if CIA1 DDR[AB] set to 'read'
 */
define_hex(CIA1_PRAB_JOY_MASK,               1f)
define_hex(CIA1_PRAB_JOYUP_MASK,             01)
define_hex(CIA1_PRAB_JOYDOWN_MASK,           02)
define_hex(CIA1_PRAB_JOYLEFT_MASK,           04)
define_hex(CIA1_PRAB_JOYRIGHT_MASK,          08)
define_hex(CIA1_PRAB_JOYBTN1_MASK,           10)

define_hex(CIA1_PRAB_LIGHTPBTN1_MASK,        10)

define_hex(CIA1_PRA_PADD_MASK,               cc)
define_hex(CIA1_PRA_PADDBTN1_MASK,           04)
define_hex(CIA1_PRA_PADDBTN2_MASK,           08)
define_hex(CIA1_PRA_PADDSWA_MASK,            40)
define_hex(CIA1_PRA_PADDSWB_MASK,            80)

define_hex(CIA1_PRB_TIMERTOGGLE_MASK,        c0)
define_hex(CIA1_PRB_TIMERATOGGLE_MASK,       40)
define_hex(CIA1_PRB_TIMERBTOGGLE_MASK,       80)

/* end of CIA1 PR[AB]
 * ------------------------------
 */

/* If VICBANK_MEM0 or VICBANK_MEM8 is selected, then VIC-II has DMA
 * access to the Character ROM at address (0x1000 - 0x2000), which is
 * accessable via memory at location (0xd000 - 0xe000).
 */
register_uint8(dd00,                         CIA2_PRA)
define_hex(CIA2_PRA_VICBANK_MASK,            03)
define_hex(CIA2_PRA_VICBANK_MEM0_DEFAULT,    03)
define_hex(CIA2_PRA_VICBANK_ADDR0_DEFAULT,   0000)
define_hex(CIA2_PRA_VICBANK_MEM4,            02)
define_hex(CIA2_PRA_VICBANK_ADDR4,           4000)
define_hex(CIA2_PRA_VICBANK_MEM8,            01)
define_hex(CIA2_PRA_VICBANK_ADDR8,           8000)
define_hex(CIA2_PRA_VICBANK_MEMC,            00)
define_hex(CIA2_PRA_VICBANK_ADDRC,           c000)
define_hex(CIA2_PRA_VICBANK_STEP,            4000)
define_hex(CIA2_PRA_USERPORT_MASK,           04)
define_hex(CIA2_PRA_SERIALOUT_MASK,          38)
define_hex(CIA2_PRA_SERIALIN_MASK,           c0)
define(CIA2_PRA_DEFAULT,                                             \
       CIA2_PRA_SERIALIN_MASK | CIA2_PRA_USERPORT_MASK
       | CIA2_PRA_VICBANK_MEM0_DEFAULT)

/* CIA2_PRA_VICBANK_ADDR(CIA2_PRA_VICBANK_MEM?)  */
macro_arg1(CIA2_PRA_VICBANK_ADDR,                                    \
           CIA2_PRA_VICBANK_ADDRC - (arg1)*CIA2_PRA_VICBANK_STEP)

/* ***************************************************************  */

register_uint8(dc02,                         CIA1_DDRA)
register_uint8(dc03,                         CIA1_DDRB)
register_uint8(dd02,                         CIA2_DDRA)
register_uint8(dd03,                         CIA2_DDRB)
define_hex(CIA_DDR_RONLY_ALL,                00)
define_hex(CIA_DDR_RW_ALL,                   ff)

define(CIA1_DDRA_DEFAULT,                    CIA_DDR_RW_ALL)
define(CIA1_DDRB_DEFAULT,                    CIA_DDR_RONLY_ALL)

register_uint8(dc04,                         CIA1_TA_LO)
register_uint8(dc05,                         CIA1_TA_HI)
define_hex(CIA1_TA_DEFAULT_PAL,              4025) /* (PAL)   */
define_hex(CIA1_TA_DEFAULT_NTSC,             4295) /* (NTSC)  */

register_uint8(dd04,                         CIA2_TA_LO)
register_uint8(dd05,                         CIA2_TA_HI)

register_uint8(dc06,                         CIA1_TB_LO)
register_uint8(dc07,                         CIA1_TB_HI)

register_uint8(dd06,                         CIA2_TB_LO)
register_uint8(dd07,                         CIA2_TB_HI)

register_uint8(dc0d,                         CIA1_ICR)
register_uint8(dd0d,                         CIA2_ICR)
define_hex(CIA_ICR_TIMERAZERO_MASK,          01)
define_hex(CIA_ICR_TIMERBZERO_MASK,          02)
define_hex(CIA_ICR_RTCALARM_MASK,            04)
define_hex(CIA_ICR_SDREMPTY_MASK,            08)
define_hex(CIA1_ICR_CASS_SERIAL_MASK,        10)
define_hex(CIA2_ICR_RS232_MASK,              10)
define_hex(CIA1_ICR_IRQGLOBAL_MASK,          80)
define_hex(CIA2_ICR_NMIGLOBAL_MASK,          80)
define_hex(CIA_ICR_ALL,                      1f)

/* CIA_ICR_MASK(CIA_ICR_*_MASK)  */
macro_arg1(CIA_ICR_MASK,       (arg1) & ~CIA1_ICR_IRQGLOBAL_MASK)
/* CIA_ICR_UNMASK(CIA_ICR_*_MASK)  */
macro_arg1(CIA_ICR_UNMASK,     (arg1) | CIA1_ICR_IRQGLOBAL_MASK)

define(CIA1_ICR_IRQMODE,                                             \
       CIA1_ICR_IRQGLOBAL_MASK | CIA_ICR_TIMERAZERO_MASK)

register_uint8(dc0e,                         CIA1_CRA)
register_uint8(dd0e,                         CIA2_CRA)
define_hex(CIA_CRA_CLKCNT_MASK,              20)
define_hex(CIA_CRA_SPWRITE_MASK,             40)
define_hex(CIA_CRA_50HZ_MASK,                80)

register_uint8(dc0f,                         CIA1_CRB)
register_uint8(dd0f,                         CIA2_CRB)
define_hex(CIA_CRB_CLK_MASK,                 60)
define_hex(CIA_CRB_CLK_SYS,                  00)
define_hex(CIA_CRB_CLK_CNT,                  20)
define_hex(CIA_CRB_CLK_TA,                   40)
define_hex(CIA_CRB_CLK_TACNT,                60)
define_hex(CIA_CRB_TODALARM_MASK,            80)

define_hex(CIA_CRAB_START_MASK,              01)
define_hex(CIA_CRAB_PBENABLE_MASK,           02)
define_hex(CIA_CRAB_PBTOGGLE_MASK,           04)
define_hex(CIA_CRAB_UNDERSTOP_MASK,          08)
define_hex(CIA_CRAB_LDLATCH_MASK,            10)

define(CIA1_CRA_DEFAULT,                     CIA_CRAB_START_MASK)

/* CIA_CRAB_PBTOGGLE_MASK must be set for a possible
 * DEBUG_TIMER1_SYNCCHECK.  But CIA_CRAB_PBENABLE_MASK must be clear,
 * it will be set during Timer A ISR call.
 */
define(CIA1_CRA_MODE, CIA_CRAB_START_MASK | CIA_CRAB_PBTOGGLE_MASK)

header_endif(CHIP_CIA)

#endif /* CHIP_CIA_DEF_H__  */
