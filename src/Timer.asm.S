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


.include "common.inc.s"

.include "Graphix.gen.s"

.include "chip-cia.gen.s"

.code

.import _timer_1_32
.export Timer_a_isr
.proc Timer_a_isr: near
        DEBUG_RENDERTIME_BEGIN #VIC_COLOR_BLUE

.ifdef DEBUG_TIMER1_SYNCCHECK
        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org zero-initialized segment
cia1_portb_backup:
        .res 1                          ; space for CIA1 port B backup
        .popseg                         ; restore "CODE" segment

        ldx CIA1_PRB                    ; X = portB
        txa                             ; A = portB
        eor cia1_portb_backup           ; A = B xor prevB = toggled
        and #CIA1_PRB_TIMERATOGGLE_MASK ; A = not zero, if portB toggled
        stx cia1_portb_backup           ; prevB = portB
        bne timer_1_sync                ; if toggled then a_is_sync
        DEBUG_WARN "timer 1, isr async with cia1"
timer_1_sync:
.endif ; DEBUG_TIMER1_SYNCCHECK

        ;; -----------------------------------------------------------
        ;; swap shared/back buffer for triple buffering

.ifndef CONF_DOUBLE_BUFFERING
        ldx _Graphix_buffer_shared_ptr   ; lowB: backup shared buffer
        lda _Graphix_buffer_back_ptr
        sta _Graphix_buffer_shared_ptr   ; lowB: shared = back buffer
        stx _Graphix_buffer_back_ptr     ; lowB: back = shared buffer

        ldx _Graphix_buffer_shared_ptr+1 ; highB: backup shared buffer
        lda _Graphix_buffer_back_ptr+1
        sta _Graphix_buffer_shared_ptr+1 ; highB: shared = back buffer
        stx _Graphix_buffer_back_ptr+1   ; highB: back = shared buffer
.endif ; CONF_DOUBLE_BUFFERING

        ;; *** end of swap ***
        ;; -----------------------------------------------------------

        inc _timer_1_32                 ; ++byte 0
        bne not_overflow                ; ready, if not overflow
        inc _timer_1_32+1               ; ++byte 1
        bne not_overflow                ; ready, if not overflow
        inc _timer_1_32+2               ; ++byte 2
        bne not_overflow                ; ready, if not overflow
        inc _timer_1_32+3               ; ++byte 3

.ifdef DEBUG
        bne not_overflow
        DEBUG_NOTE "timer 1, overflows"
.endif ; DEBUG
not_overflow:

        DEBUG_RENDERTIME_END
        rts
.endproc                                ; Timer_a_isr
