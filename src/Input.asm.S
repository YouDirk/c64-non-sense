;; C64 NonSense, just playing around with C64 cross-compile tools.
;; Copyright (C) 2021  Dirk "YouDirk" Lehmann
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

.include "chip-cia.gen.s"

.code

;; *******************************************************************

;; void __fastcall__ _Input_keyboard_scan(void);
.export __Input_keyboard_scan
.proc __Input_keyboard_scan: near

        lda #CIA_DDR_RW_ALL
        sta CIA1_DDRA

        lda #$fe

scan_loop:
        ;; A = scan value for output port
        sta CIA1_PRA
        pha
        lda CIA1_PRB
        lsr
        bcs scan_next
        DEBUG_NOTE "ok"
scan_next:
        pla
        ;; A = scan value for output port
        sec
        rol
        cmp #$ff
        bne scan_loop
scan_done:

        lda #CIA_DDR_RONLY_ALL
        sta CIA1_DDRA

        rts
.endproc                                ; __Input_keyboard_scan

;; *******************************************************************
