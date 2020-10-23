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

.code

.export _Interrupt_init
.proc _Interrupt_init: near

        lda #_CIA1_ICR_MASKALL_MASK     ; disabling timer irqs from CIA1
        sta _CIA1_ICR                   ; for now

        sei                                ; mask all IRQs

        lda #<(Interrupt_irq)
        sta _KERNAL_IRQ_USERENTRY_ADDR     ; set lower byte of isr
        lda #>(Interrupt_irq)
        sta _KERNAL_IRQ_USERENTRY_ADDR+1   ; set higher byte of isr

        cli                                ; unmask all IRQs
        rts

.endproc                                   ; _Interrupt_init

.export _Interrupt_release
.proc _Interrupt_release: near

        sei                                ; mask all IRQs

        lda #<(_KERNAL_IRQ_DEFAULT_ADDR)
        sta _KERNAL_IRQ_USERENTRY_ADDR     ; set lower byte of isr
        lda #>(_KERNAL_IRQ_DEFAULT_ADDR)
        sta _KERNAL_IRQ_USERENTRY_ADDR+1   ; set higher byte of isr

        cli                                ; unmask all IRQs

        lda #_CIA1_ICR_UNMASKALL_MASK   ; restoring timer irqs from CIA1
        sta _CIA1_ICR                   ; for now

        rts

.endproc                                   ; _Interrupt_release

.proc Interrupt_irq: near

        ldx $D020
        inx                             ; increment bg color if an IRQ
        stx $D020                       ; occurs

        jmp _KERNAL_IRQ_DEFAULT_ADDR

.endproc                                ; Interrupt_irq
