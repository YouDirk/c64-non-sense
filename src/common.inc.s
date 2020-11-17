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


.ifnblank COMMON_INC_S__
.define COMMON_INC_S__

.include "chip-vic.gen.s"

.include "Debug.inc.s"

;; *******************************************************************

.macro MUTEX_LOCK mutex
        lda #$01
        sta mutex
.endmacro                               ; MUTEX_LOCK

.macro MUTEX_UNLOCK mutex
        lsr mutex
.endmacro                               ; MUTEX_UNLOCK

        ;; if locked then Z-bit will be cleared (non-zero)
.macro MUTEX_ISLOCKED mutex
        lda #$ff
        bit mutex
.endmacro                               ; MUTEX_ISLOCKED

;; *******************************************************************

.endif                                  ; COMMON_INC_S__
