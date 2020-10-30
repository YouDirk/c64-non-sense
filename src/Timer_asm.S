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


.include "common.s"

.include "chip_vic.gen.s"

.code

.import _timer_1_32
.export Timer_a_isr
.proc Timer_a_isr: near
        isr_rendertime_begin #VIC_COLOR_BLUE

.if 0
        ; smaller code but slower, urolled loop below
        ldx #0                          ; x = offset = 0
next_byte:
        inc _timer_1_32,x               ; *(timer+x) += 1
        bne not_overflow
        inx                             ; ++x
        cpx #4
        bne next_byte                   ; if (x != 4) goto next_byte
.else
        inc _timer_1_32
        bne not_overflow
        inc _timer_1_32+1
        bne not_overflow
        inc _timer_1_32+2
        bne not_overflow
        inc _timer_1_32+3
.endif
not_overflow:

        isr_rendertime_end
        rts
.endproc                                ; Timer_a_isr
