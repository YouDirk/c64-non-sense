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


.ifndef COMMON_S__
.define COMMON_S__

.include "chip_vic.gen.s"

;; *******************************************************************

.macro isr_rendertime_begin color
.ifdef DEBUG_IRQ_RENDERTIME
        lda VIC_BORDERCOLOR             ; push current border color to
        pha                             ;   stack
        lda color
        sta VIC_BORDERCOLOR             ; set debug color
.endif
.endmacro                               ; isr_rendertime_begin

.macro isr_rendertime_end
.ifdef DEBUG_IRQ_RENDERTIME
        pla
        sta VIC_BORDERCOLOR             ; pop backup and restore color
.endif
.endmacro                               ; isr_rendertime_end

;; *******************************************************************

.endif                                  ; COMMON_S__
