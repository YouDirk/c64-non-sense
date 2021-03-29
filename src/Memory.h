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


#ifndef MEMORY_H__
#define MEMORY_H__

#include "common.h"

/* *******************************************************************
 *
 *         : Pre-initialzed        | Possible memory banks :
 *         : **************        | ********************* :
 *         :                       |                       :
 *         .-----------------------------------------------.
 * 0x0000  | [CPU] MOS 6510, port access (pin P0-P5)       |
 *         .-----------------------------------------------.
 * 0x0002  | [RAM, Kernal] Zero Page                       |
 *         .-----------------------------------------------.
 * 0x0100  | [RAM, Kernal] Extended Zero Page              |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0x0400  | [RAM, VIC-II DMA] Screen RAM                  |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0x0800  | [RAM, BASIC] BASIC RAM                        |
 *         |   0x0801: entry point of C64 NonSense         |
 *         |                                               |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0xa000  | [RAM]                | [ROM] BASIC ROM        |
 *         |                      |                        |
 *         |                      |                        |
 *         |                      |                        |
 *         |                      |                        |
 *         .-----------------------------------------------.
 * 0xc000  | [RAM]                                         |
 *         |                                               |
 *         |                                               |
 *         .-----------------------------------------------.
 * 0xd000  | [I/O] VIC-II, SID,   | [ROM]        | [RAM]   |
 *         |       CIA 1/2        |   Character  |         |
 *         |                      |   ROM        |         |
 *         .-----------------------------------------------.
 * 0xe000  | [ROM] Kernal         | [RAM]                  |
 *         |                      |                        |
 *         |                      |                        |
 *         .-----------------------------------------------.
 * 0xfffa  | [CPU] MOS 6510, vectors (NMI, RESET, IRQ)     |
 * 0xffff  |       defined in 'Kernal'                     |
 *         .-----------------------------------------------.
 */

/* ***************************************************************  */

/* Prepare memory layout by configure the PLA MOS 906114-01 using the
 * MOS6510 CPUs I/O port for bank switching.
 */
extern void __fastcall__ Memory_init(void);

/* Restore memory layout.  */
extern void __fastcall__ Memory_release(void);

/* ***************************************************************  */

#endif /* MEMORY_H__  */
