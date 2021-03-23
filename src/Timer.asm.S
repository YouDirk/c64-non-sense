;; C64 NonSense, just playing around with C64 cross-compile tools.
;; Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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


.include "Timer.inc.s"

.include "Graphix.inc.s"

.include "chip-cia.gen.s"

.code

;; *******************************************************************

.export Timer_a_isr
.proc Timer_a_isr: near
        DEBUG_RENDERTIME_BEGIN #VIC_COLOR_BLUE

.ifdef DEBUG_TIMER1_SYNCCHECK
        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
cia1_portb_backup:
        .res 1                          ; space for CIA1 port B backup
        .popseg                         ; restore "CODE" segment

        lda #CIA1_CRA_MODE | CIA_CRAB_PBENABLE_MASK
        sta CIA1_CRA                    ; portB bit6 disabled by default

        ldx CIA1_PRB                    ; X = portB
        txa                             ; A = portB
        eor cia1_portb_backup           ; A = B xor prevB = toggled
        and #CIA1_PRB_TIMERATOGGLE_MASK ; A = not zero, if portB toggled
        stx cia1_portb_backup           ; prevB = portB
        bne timer_1_sync                ; if toggled then a_is_sync
        DEBUG_WARN "timer 1, isr async with cia1"
timer_1_sync:

        lda #CIA1_CRA_MODE
        sta CIA1_CRA                    ; restore CIA1 portB bit6
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

        inc __timer_1_32                ; ++byte 0
        bne not_overflow                ; ready, if not overflow
        inc __timer_1_32 + 1            ; ++byte 1
        bne not_overflow                ; ready, if not overflow
        inc __timer_1_32 + 2            ; ++byte 2
        bne not_overflow                ; ready, if not overflow
        inc __timer_1_32 + 3            ; ++byte 3

.ifdef DEBUG
        bne not_overflow
        DEBUG_NOTE "timer 1, overflows"
.endif ; DEBUG
not_overflow:

        DEBUG_RENDERTIME_END
        rts
.endproc                                ; Timer_a_isr

;; *******************************************************************

;; CC65 calling conventions: 32 bit values will be returned in
;;
;;   * [byte3, byte2, byte1, byte0] => [sreg+1, sreg, X, A]
;;
;; !!! CALLER NEEDS TO SAVE (backup) SREG!
;;

;; uint32_t __fastcall__ Timer_1_get32(void);
.export _Timer_1_get32
_Timer_1_get32:

.export Timer_1_get32_isrsafe
.proc Timer_1_get32_isrsafe: near

        lda #CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK)
        sta CIA1_ICR                    ; mask CIA1 timer A IRQs

        lda __timer_1_32 + 3
        sta sreg + 1                    ; return byte 3 of timer
        lda __timer_1_32 + 2
        sta sreg                        ; return byte 2 of timer
        ldx __timer_1_32 + 1            ; return byte 1 of timer
        lda __timer_1_32                ; return byte 0 of timer

        ldy #CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK)
        sty CIA1_ICR                    ; unmask CIA1 timer A IRQs

        rts
.endproc                                ; Timer_1_get32_isrsafe

;; *******************************************************************

;; uint16_t __fastcall__ Timer_1_get16(void);
.export _Timer_1_get16
_Timer_1_get16:

.export Timer_1_get16_isrsafe
.proc Timer_1_get16_isrsafe: near

        lda #CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK)
        sta CIA1_ICR                    ; mask CIA1 timer A IRQs

        ldx __timer_1_32 + 1            ; return byte 1 of timer
        lda __timer_1_32                ; return byte 0 of timer

        ldy #CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK)
        sty CIA1_ICR                    ; unmask CIA1 timer A IRQs

        rts
.endproc                                ; Timer_1_get16_isrsafe

;; *******************************************************************

;; uint8_t __fastcall__ Timer_1_get8(void);
.export _Timer_1_get8
_Timer_1_get8:

.export Timer_1_get8_isrsafe
.proc Timer_1_get8_isrsafe: near

        lda __timer_1_32                ; return byte 0 of timer

        rts
.endproc                                ; Timer_1_get8_isrsafe

;; *******************************************************************

;; void __fastcall__ Timer_1_reset(void);
.export _Timer_1_reset
_Timer_1_reset:

.export Timer_1_reset_isrsafe
.proc Timer_1_reset_isrsafe: near

        lda #CIA_ICR_MASK(CIA_ICR_TIMERAZERO_MASK)
        sta CIA1_ICR                    ; mask CIA1 timer A IRQs

        lda #$00
        sta __timer_1_32 + 3            ; reset byte 3 of timer
        sta __timer_1_32 + 2            ; reset byte 2 of timer
        sta __timer_1_32 + 1            ; reset byte 1 of timer
        sta __timer_1_32                ; reset byte 0 of timer

        lda #CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK)
        sta CIA1_ICR                    ; unmask CIA1 timer A IRQs

        rts
.endproc                                ; Timer_1_reset_isrsafe

;; *******************************************************************
