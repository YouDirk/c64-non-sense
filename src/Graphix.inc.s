;; C64 NonSense, the C64 Game Engine.
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


.ifnblank GRAPHIX_INC_S__
.define GRAPHIX_INC_S__

.include "common.inc.s"
.include "Graphix.gen.s"

;; *******************************************************************

;; extern void __fastcall__ Graphix_rasterline_isr(void);
.global Graphix_rasterline_isr

;; *******************************************************************

.endif                                  ; GRAPHIX_INC_S__
