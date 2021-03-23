;; C64 NonSense, just playing around with C64 cross-compile tools.
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


.ifnblank TIMER_INC_S__
.define TIMER_INC_S__

.include "common.inc.s"

;; *******************************************************************

;; extern void __fastcall__ Timer_a_isr(void);
.global Timer_a_isr

;; extern uint8_t __fastcall__ Timer_1_get8_isrsafe(void);
.global Timer_1_get8_isrsafe

;; extern uint16_t __fastcall__ Timer_1_get16_isrsafe(void);
.global Timer_1_get16_isrsafe

;; extern uint32_t __fastcall__ Timer_1_get32_isrsafe(void);
.global Timer_1_get32_isrsafe

;; extern void __fastcall__ Timer_1_reset_isrsafe(void);
.global Timer_1_reset_isrsafe

;; *******************************************************************
;; private

;; volatile uint32_t _timer_1_32
.import __timer_1_32

;; *******************************************************************

.endif                                  ; TIMER_INC_S__
