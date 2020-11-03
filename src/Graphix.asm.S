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

.code

.import _Graphix
.import _shadow_isr
.export Graphix_rasterline_isr
.proc Graphix_rasterline_isr: near
.ifdef DEBUG_IRQ_RENDERTIME
        lda #VIC_COLOR_RED              ; we don´t need to push border
        sta VIC_BORDERCOLOR             ; color, this isr set it activ
.endif

        lda _shadow_isr + Graphix_buffer_t::scroll_y
        ora VIC_CTRL1_MODE & ~VIC_CTRL1_YSCROLL_MASK
        sta VIC_CTRL1                   ; set x scroll

        lda _shadow_isr + Graphix_buffer_t::scroll_x
        ora VIC_CTRL2_MODE & ~VIC_CTRL2_XSCROLL_MASK
        sta VIC_CTRL2                   ; set x scroll

        ;; must be the last statement, for correct DEBUG_IRQ_RENDERTIME
        lda _Graphix + Graphix_t::buffer + Graphix_buffer_t::bordercolor
        sta VIC_BORDERCOLOR             ; set bordercolor
.ifdef DEBUG_IRQ_RENDERTIME
        ;; nothing to do, because VIC.bordercolor will be set above
.endif
        rts
.endproc                                ; Graphix_rasterline_isr
