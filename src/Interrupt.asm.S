;; C64 NonSense, the C64 Game Engine.
;; Copyright (C) 2020-2025  Dirk "YouDirk" Lehmann
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

.include "Timer.inc.s"
.include "Graphix.inc.s"

.include "chip-vic.gen.s"
.include "chip-cia.gen.s"

.include "kernal.gen.s"

.code

.export _Interrupt_init
.proc _Interrupt_init: near

        lda #CIA_ICR_MASK(CIA_ICR_ALL)  ; disabling timer IRQs from CIA1
        sta CIA1_ICR                    ;   for now

        sei                             ; mask all IRQs

        lda #<Interrupt_irq_isr
        sta KERNAL_IRQ_USERENTRY        ; set lower byte of isr
        lda #>Interrupt_irq_isr
        sta KERNAL_IRQ_USERENTRY+1      ; set higher byte of isr

        cli                             ; unmask all IRQs
        rts
.endproc                                ; _Interrupt_init

.export _Interrupt_release
.proc _Interrupt_release: near

        sei                             ; mask all IRQs

        lda #<KERNAL_IRQ_DEFAULT
        sta KERNAL_IRQ_USERENTRY        ; set lower byte of isr
        lda #>KERNAL_IRQ_DEFAULT
        sta KERNAL_IRQ_USERENTRY+1      ; set higher byte of isr

        cli                             ; unmask all IRQs

        lda #CIA_ICR_UNMASK(CIA_ICR_ALL); restoring timer IRQs from CIA1
        sta CIA1_ICR                    ;   for now

        rts
.endproc                                ; _Interrupt_release

.proc Interrupt_irq_isr: near           ; ISR of IRQs begins here

vic:
        lda #VIC_IMR_IRQMODE
        and VIC_IRR
        beq not_vic                     ; is VIC IRQ?
rasterline:
        and #VIC_IMR_RASTERLINE_MASK
        beq not_rasterline              ; is rasterline IRQ?

        jsr Graphix_rasterline_isr

        lda #VIC_IMR_RASTERLINE_MASK    ; tell VIC served rasterline IRQ
        sta VIC_IRR
        jmp KERNAL_IRQ_RETURN           ; (rti) return as fast as possible
not_rasterline:
not_vic:

cia1:
        lda #CIA1_ICR_IRQMODE
        and CIA1_ICR                    ; is CIA1 IRQ?
        beq not_cia1
cia1_timer_a:
        and #CIA_ICR_TIMERAZERO_MASK
        beq not_cia1_timer_a            ; is CIA1 timer A IRQ?

        jsr Timer_a_isr

        lda #CIA_ICR_UNMASK(CIA_ICR_TIMERAZERO_MASK)
        sta CIA1_ICR                    ; tell CIA1 served timer A IRQ
        jmp KERNAL_IRQ_RETURN           ; (rti) return as fast as possible
not_cia1_timer_a:
not_cia1:

.ifdef DEBUG                            ; increment border color if an
        inc VIC_BORDERCOLOR             ;   unexpected IRQ occurs
        DEBUG_WARN "interrupt, irq not served!"
.endif ; DEBUG
        jmp KERNAL_IRQ_DEFAULT          ; jump to default Kernal ISR
.endproc                                ; Interrupt_irq_isr
