;; C64 NonSense, the C64 Game Engine.
;; Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU Affero General Public License as published
;; by the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU Affero General Public License for more details.
;;
;; You should have received a copy of the GNU Affero General Public License
;; along with this program.  If not, see <https://www.gnu.org/licenses/>.


.ifnblank COMMON_INC_S__
.define COMMON_INC_S__

.include "Debug.inc.s"

;; *******************************************************************

.include "zeropage.inc"  ; shared zero page addresses of CC65 runtime

.define ZP_NONSENSE_BEGIN                    sp + zpspace

;; Dedicated to Interrupt Service Routines.  No backup and restore
;; needed, due to no recursive ISRs are occurring.
.define ZP_ISR_PTR1                          ZP_NONSENSE_BEGIN + 0
.define ZP_ISR_PTR2                          ZP_NONSENSE_BEGIN + 2

;; *******************************************************************

.define SIZEOF_BYTE                          1
.define SIZEOF_WORD                          2
.define SIZEOF_DWORD                         4
.define SIZEOF_ADDR                          2

;; *******************************************************************
;; Compatible with <stdbool.h> of CC65

.define SIZEOF_BOOL                          1
.define FALSE                                $00
.define TRUE                                 $01

;; *******************************************************************
;; Bit masks of the MOS 6510 Processor Status Register (P register)

.define MOS6510_PREG_CARRY_MASK              $01
.define MOS6510_PREG_ZERO_MASK               $02
.define MOS6510_PREG_IRQDISABLE_MASK         $04
.define MOS6510_PREG_DECIMALMODE_MASK        $08
.define MOS6510_PREG_BRKCOMMAND_MASK         $10
.define MOS6510_PREG_OVERFLOW_MASK           $40
.define MOS6510_PREG_NEGATIVE_MASK           $80

;; *******************************************************************

.endif                                  ; COMMON_INC_S__
