;; C64 NonSense, the C64 Game Engine.
;; Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
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


.ifnblank INPUT_INC_S__
.define INPUT_INC_S__

.include "common.inc.s"
.include "Input.gen.s"

;; *******************************************************************
;; private
;;

;; extern void __fastcall__ _Input_keyboard_scan(void)
.global __Input_keyboard_scan

;; extern void __fastcall__ _Input_keyboard_petscii_convert(void)
.global __Input_keyboard_petscii_convert

;; *******************************************************************

.endif                                  ; INPUT_INC_S__
