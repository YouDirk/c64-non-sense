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

.include "chip-vic.gen.s"

.importzp ptr1                  ; shared zero page address of CC65 runtime

.code

.export Graphix_rasterline_isr
.proc Graphix_rasterline_isr: near
.ifdef DEBUG_IRQ_RENDERTIME
        lda #VIC_COLOR_RED              ; we don´t need to push border
        sta VIC_BORDERCOLOR             ; color, this isr set it activ
.endif ; DEBUG_IRQ_RENDERTIME

        lda ptr1
        pha                             ; backup low byte of shared zp
        lda ptr1+1
        pha                             ; backup high byte of shared zp

        lda _Graphix_buffer_back_ptr
        sta ptr1
        lda _Graphix_buffer_back_ptr+1
        sta ptr1+1                      ; ptr1 = _Graphix_buffer_back_ptr

        ldy #Graphix_buffer_t::scroll_y
        lda (ptr1), y
        ora #(VIC_CTRL1_MODE & ~VIC_CTRL1_YSCROLL_MASK)
        sta VIC_CTRL1                   ; set y scroll

        ldy #Graphix_buffer_t::scroll_x
        lda (ptr1), y
        ora #(VIC_CTRL2_MODE & ~VIC_CTRL2_XSCROLL_MASK)
        sta VIC_CTRL2                   ; set x scroll

        ;; must be the last statement, for correct DEBUG_IRQ_RENDERTIME
        ldy #Graphix_buffer_t::bordercolor
        lda (ptr1), y
        sta VIC_BORDERCOLOR             ; set bordercolor
.ifdef DEBUG_IRQ_RENDERTIME
        ;; nothing to do, because VIC.bordercolor will be set above
.endif ; DEBUG_IRQ_RENDERTIME

        pla
        sta ptr1+1                      ; restore high byte of shared zp
        pla
        sta ptr1                        ; restore low byte of shared zp

        rts
.endproc                                ; Graphix_rasterline_isr
