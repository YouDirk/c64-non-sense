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


.include "Input.inc.s"

.include "chip-cia.gen.s"

.code

;; *******************************************************************

;; Input_scancode_t __fastcall__ _Input_keyboard_scan(void);
.export __Input_keyboard_scan
.proc __Input_keyboard_scan: near

        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
cur_scan_value:
        .res 1                          ; scan value for output port
        .popseg                         ; restore "CODE" segment


        lda #CIA_DDR_RW_ALL
        sta CIA1_DDRA

        ldy #$ff
        lda #$fe
        sta cur_scan_value

scan_loop:
        ;; A = scan value for output port
        sta CIA1_PRA
        lda CIA1_PRB
inner_loop:
        ;; A = scan value for input port
        iny
        lsr
        bcs inner_next
        jmp scan_done
inner_next:
        bne inner_loop
inner_done:
        lda cur_scan_value
        sec
        rol
        sta cur_scan_value
        cmp #$ff
        bne scan_loop
        iny                     ; TODO
scan_done:

        lda #CIA_DDR_RONLY_ALL
        sta CIA1_DDRA

        sty _Input + (Input_t::keyboard) + (Input_keyboard_t::pressed)

        rts
.endproc                                ; __Input_keyboard_scan

;; *******************************************************************
