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


.include "chip_vic.gen.s"
.include "chip_cia.gen.s"

.include "kernal.gen.s"

.code

.export _Interrupt_init
.proc _Interrupt_init: near

        lda CIA_ICR_MASKALL_MASK        ; disabling timer IRQs from CIA1
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

        lda CIA_ICR_UNMASKALL_MASK      ; restoring timer IRQs from CIA1
        sta CIA1_ICR                    ;   for now

        rts

.endproc                                ; _Interrupt_release

.import __Graphix_render_isr
.proc Interrupt_irq_isr: near           ; ISR of IRQs begins here

        lda #VIC_IMR_RASTERLINE_MASK
        bit VIC_IRR                     ; is raster line IRQ?
        beq not_served

        jsr __Graphix_render_isr

        lda #VIC_IMR_RASTERLINE_MASK
                                        ; tell VIC that we served the
        sta VIC_IRR                     ;   IRQ
        jmp KERNAL_IRQ_RETURN           ; (rti) return as fast as possible

not_served:
.ifdef DEBUG                            ; increment border color if an
        inc VIC_BORDERCOLOR             ;   unexpected IRQ occurs
.endif
        jmp KERNAL_IRQ_DEFAULT          ; jump to default Kernal ISR

.endproc                                ; Interrupt_irq_isr
