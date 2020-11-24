;; C64 NonSense, just playing around with C64 cross-compile tools.
;; Copyright (C) 2020  Dirk "YouDirk" Lehmann
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

.importzp ptr1           ; shared zero page address of CC65 runtime
.importzp sreg           ; shared virtual register of CC65 runtime

;; *******************************************************************

;; The high byte of the C64 processor stack address is hard wired to
;; 0x0100.  The Stack Pointer Register (S register) of the MOS 6510
;; CPU will be initialized to 0xff by the Kernal during RESET.
;; Therefore, if the stack is empty it points to 0x01ff.  On pushing
;; to it (PHA <VALUE> instruction) the VALUE will be saved to 0x01SS
;; (SS: value of S register) and AFTER that, SS will be decrmented by
;; 1.
;;
;; The top of C64 processor stack (TOS) can be calculated as
;;
;;   TOS = 0x0101 + SS
;;
;; In assembler the N-th byte of the processor stack can be ezly
;; addressed via
;;
;; ```asm
;;   tsx                      ; Transfer S register into X register
;;   lda STACK_BASE + N, x    ; Load the N-th byte of TOS into accu
;;   sta STACK_BASE + N, x    ; Store accu into the N-th byte of TOS
;; ```
;;
.define STACK_BASE                 $0101

;; *******************************************************************

.define SIZEOF_BYTE                1
.define SIZEOF_WORD                2
.define SIZEOF_DWORD               4
.define SIZEOF_ADDR                2

;; *******************************************************************

.macro MUTEX_LOCK mutex
        lda #$01
        sta mutex
.endmacro                               ; MUTEX_LOCK

.macro MUTEX_UNLOCK mutex
        lsr mutex
.endmacro                               ; MUTEX_UNLOCK

        ;; if locked then Z-bit will be cleared (non-zero)
.macro MUTEX_ISLOCKED mutex
        lda #$ff
        bit mutex
.endmacro                               ; MUTEX_ISLOCKED

;; *******************************************************************

.endif                                  ; COMMON_INC_S__
