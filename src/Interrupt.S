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
;;


.define _KERNAL_IRQ_USERENTRY_ADDR      $0314

.define _KERNAL_IRQ_DEFAULT_ADDR        $ea31
.define _KERNAL_IRQ_RETURN_ADDR         $ea81

.define _CIA1_ICR                       $dc0d

.define _CIA1_ICR_MASKALL_MASK          $7f
.define _CIA1_ICR_UNMASKALL_MASK        $ff

.define _VIC_IRR                        $d019
.define _VIC_IRR_RASTERLINE_MASK        $01
.define _VIC_IRR_SHCOLLOSION_MASK       $02
.define _VIC_IRR_SSCOLLOSION_MASK       $04
.define _VIC_IRR_LIGHTPEN_MASK          $08

.define _VIC_BORDERCOLOR                $d020

.code

.export _Interrupt_init
.proc _Interrupt_init: near

        lda #_CIA1_ICR_MASKALL_MASK     ; disabling timer IRQs from CIA1
        sta _CIA1_ICR                   ;   for now

        sei                                ; mask all IRQs

        lda #<Interrupt_irq_isr
        sta _KERNAL_IRQ_USERENTRY_ADDR     ; set lower byte of isr
        lda #>Interrupt_irq_isr
        sta _KERNAL_IRQ_USERENTRY_ADDR+1   ; set higher byte of isr

        cli                                ; unmask all IRQs
        rts

.endproc                                   ; _Interrupt_init

.export _Interrupt_release
.proc _Interrupt_release: near

        sei                                ; mask all IRQs

        lda #<_KERNAL_IRQ_DEFAULT_ADDR
        sta _KERNAL_IRQ_USERENTRY_ADDR     ; set lower byte of isr
        lda #>_KERNAL_IRQ_DEFAULT_ADDR
        sta _KERNAL_IRQ_USERENTRY_ADDR+1   ; set higher byte of isr

        cli                                ; unmask all IRQs

        lda #_CIA1_ICR_UNMASKALL_MASK   ; restoring timer IRQs from CIA1
        sta _CIA1_ICR                   ;   for now

        rts

.endproc                                ; _Interrupt_release

.import __Graphix_render_isr
.proc Interrupt_irq_isr: near           ; ISR of IRQs begins here

        lda #_VIC_IRR_RASTERLINE_MASK
        bit _VIC_IRR                    ; is raster line IRQ?
        beq not_served

        jsr __Graphix_render_isr

        lda #_VIC_IRR_RASTERLINE_MASK   ; tell VIC that we served the
        sta _VIC_IRR                    ;   IRQ
        jmp _KERNAL_IRQ_RETURN_ADDR     ; (rti) return as fast as possible

not_served:
.ifdef DEBUG                            ; increment border color if an
        inc _VIC_BORDERCOLOR            ;   unexpected IRQ occurs
.endif
        jmp _KERNAL_IRQ_DEFAULT_ADDR    ; jump to default Kernal ISR

.endproc                                ; Interrupt_irq_isr
