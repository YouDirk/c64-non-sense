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


.include "Input.inc.s"

.include "chip-cia.gen.s"
.include "kernal.gen.s"

.code

;; *******************************************************************

;; void __fastcall__ _Input_joystick_port2_tick(void)
.export __Input_joystick_port2_tick
.proc __Input_joystick_port2_tick: near
        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
reg_bits:
        .res 1                          ; copy of CIA1 port registers
        .popseg                         ; restore "CODE" segment

        lda CIA1_PRA                    ; bits of joy port2->CIA1 portA
        eor #$ff                        ; bit set = PRESSED
        sta reg_bits
        ;; ---
  .define _P2Y Input_t::joy_port2 + Input_joystick_t::axis_y
        ldx _Input + _P2Y + Input_axis_t::direction ; X := previous P2Y
        lda #$00                        ; 0 by default
        lsr reg_bits
        rol                             ; 1 if UP
        lsr reg_bits
        bcc _p2y_not_down
        lda #$ff                        ; -1 if DOWN
_p2y_not_down:
        sta _Input + _P2Y + Input_axis_t::direction  ; save P2 Y-axis
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P2Y + Input_axis_t::direction
        beq _p2y_not_changed
        lda #$01
_p2y_not_changed:
        sta _Input + _P2Y + Input_axis_t::changed    ; save CHANGED P2Y
        ;; ---
  .define _P2X Input_t::joy_port2 + Input_joystick_t::axis_x
        ldx _Input + _P2X + Input_axis_t::direction ; X := previous P2X
        lda #$00                        ; 0 by default
        lsr reg_bits
        rol                             ; 1 if LEFT
        lsr reg_bits
        bcc _p2x_not_right
        lda #$ff                        ; -1 if RIGHT
_p2x_not_right:
        sta _Input + _P2X + Input_axis_t::direction  ; save P2 X-axis
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P2X + Input_axis_t::direction
        beq _p2x_not_changed
        lda #$01
_p2x_not_changed:
        sta _Input + _P2X + Input_axis_t::changed    ; save CHANGED P2X
        ;; ---
  .define _P2B Input_t::joy_port2 + Input_joystick_t::button1
        ldx _Input + _P2B + Input_button_t::pressed  ; X := previous P2B
        lda #$00                        ; FALSE by default
        lsr reg_bits
        rol                             ; TRUE if pressed
        sta _Input + _P2B + Input_button_t::pressed  ; save P2 button1
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P2B + Input_button_t::pressed
        beq _p2b_not_changed
        lda #$01
_p2b_not_changed:
        sta _Input + _P2B + Input_button_t::changed  ; save CHANGED P2B

        rts
.endproc                                ; __Input_joystick_port2_tick

;; *******************************************************************

;; void __fastcall__ _Input_joystick_port1_tick(void)
.export __Input_joystick_port1_tick
.proc __Input_joystick_port1_tick: near
        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
reg_bits:
        .res 1                          ; copy of CIA1 port registers
        .popseg                         ; restore "CODE" segment

        lda CIA1_PRB                    ; bits of joy port1->CIA1 portB
        eor #$ff                        ; bit set = PRESSED
        sta reg_bits
        ;; ---
  .define _P1Y Input_t::joy_port1 + Input_joystick_t::axis_y
        ldx _Input + _P1Y + Input_axis_t::direction ; X := previous P1Y
        lda #$00                        ; 0 by default
        lsr reg_bits
        rol                             ; 1 if UP
        lsr reg_bits
        bcc _p1y_not_down
        lda #$ff                        ; -1 if DOWN
_p1y_not_down:
        sta _Input + _P1Y + Input_axis_t::direction  ; save P1 Y-axis
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P1Y + Input_axis_t::direction
        beq _p1y_not_changed
        lda #$01
_p1y_not_changed:
        sta _Input + _P1Y + Input_axis_t::changed    ; save CHANGED P1Y
        ;; ---
  .define _P1X Input_t::joy_port1 + Input_joystick_t::axis_x
        ldx _Input + _P1X + Input_axis_t::direction ; X := previous P1X
        lda #$00                        ; 0 by default
        lsr reg_bits
        rol                             ; 1 if LEFT
        lsr reg_bits
        bcc _p1x_not_right
        lda #$ff                        ; -1 if RIGHT
_p1x_not_right:
        sta _Input + _P1X + Input_axis_t::direction  ; save P1 X-axis
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P1X + Input_axis_t::direction
        beq _p1x_not_changed
        lda #$01
_p1x_not_changed:
        sta _Input + _P1X + Input_axis_t::changed    ; save CHANGED P1X
        ;; ---
  .define _P1B Input_t::joy_port1 + Input_joystick_t::button1
        ldx _Input + _P1B + Input_button_t::pressed  ; X := previous P1B
        lda #$00                        ; FALSE by default
        lsr reg_bits
        rol                             ; TRUE if pressed
        sta _Input + _P1B + Input_button_t::pressed  ; save P1 button1
        lda #$00                        ; CHANGED, FALSE by default
        cpx _Input + _P1B + Input_button_t::pressed
        beq _p1b_not_changed
        lda #$01
_p1b_not_changed:
        sta _Input + _P1B + Input_button_t::changed  ; save CHANGED P1B

        rts
.endproc                                ; __Input_joystick_port1_tick

;; *******************************************************************

;; void __fastcall__ _Input_keyboard_scan(void)
.export __Input_keyboard_scan
.proc __Input_keyboard_scan: near
        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
cur_scan_value:
        .res 1                          ; scan value for output port
cur_input_matrix:
        .res 1                          ; input from keyboard matrix
inner_loop_counter:
        .res 1                          ; index (i) for inner loop
        .popseg                         ; restore "CODE" segment

        lda #CIA_DDR_RW_ALL
        sta CIA1_DDRA                   ; scan value -> portA

        ldx #$00                        ; X: sc_pressed_count
        stx _Input + Input_t::keyboard + Input_keyboard_t::changed
                                        ; changed := false
        ldy #$ff                        ; Y: scan code
        lda #$fe                        ; A: scan value
        sta cur_scan_value              ;    | input from matirx

scan_loop:
        ;; A = scan value for output port
        sta CIA1_PRA

        lda #$08
        sta inner_loop_counter          ; for (i=8; ....)
bounced:
        lda CIA1_PRB                    ; read input matrix
        cmp CIA1_PRB                    ;   from portB
        bne bounced
inner_loop:
        ;; A = input from keyboard matrix
        iny                             ; input_matrix
        lsr                             ;    := (input_matrix >> 1)
        bcs inner_next                  ; bit0 will be tested first

        ;; --- current key was pressed
        sta cur_input_matrix            ; "push": input matrix
        tya                             ; A := scan_code
        cmp _Input+Input_t::keyboard + Input_keyboard_t::sc_pressed, x
        beq key_not_changed
               ; keyboard.sc_pressed[sc_pressed_count] := scan_code
        sta _Input+Input_t::keyboard + Input_keyboard_t::sc_pressed, x
        lda #TRUE
        sta _Input+Input_t::keyboard + Input_keyboard_t::changed
key_not_changed:
        lda cur_input_matrix            ; "pop": A := input matrix
        inx                             ; ++sc_pressed_count
        cpx #INPUT_KEYBOARD_SCPRESSED_MAXCOUNT ; if (sc_pressed_count
        beq scan_done                          ;   == MAXCOUNT) break;
        ;; --- end of: current key was pressed

inner_next:
        dec inner_loop_counter
        bne inner_loop                  ; for (i=8; i != 0; --i)
inner_done:
        lda cur_scan_value              ; A := scan value
        sec
        rol                             ; scan_value
        sta cur_scan_value              ;   := (scan_value << 1) | 1
        cmp #$ff                        ; bit0 scanned first, start: 0xfe
        bne scan_loop                   ; while (rolled_scanval != 0xff)

 ;; Check disabled by default.
.if 0 && .defined(DEBUG)
        ;; Y = 0x3f (Input_sc_none_e - 1)
        cpy #$3f
        beq debug_scannedall_ok
        DEBUG_ERROR_REGSAVE "input keyboard, not all scanned"
debug_scannedall_ok:
.endif ; DEBUG

scan_done:
                    ; keyboard.sc_pressed[sc_pressed_count] := 0x40
        lda #Input_sc_none_e
        sta _Input+Input_t::keyboard + Input_keyboard_t::sc_pressed, x

                    ; if (Input.keyboard.sc_pressed_count != X)
        cpx _Input+Input_t::keyboard+Input_keyboard_t::sc_pressed_count
        beq count_not_changed ;   then: I.keyboard.sc_pressed_count := X
        stx _Input+Input_t::keyboard+Input_keyboard_t::sc_pressed_count
        lda #TRUE             ;   then: I.keyboard.changed := true
        sta _Input+Input_t::keyboard+Input_keyboard_t::changed
count_not_changed:

        lda #CIA_DDR_RONLY_ALL          ; restore data direction of
        sta CIA1_DDRA                   ;   portA

        rts
.endproc                                ; __Input_keyboard_scan

;; *******************************************************************

;; void __fastcall__ _Input_keyboard_petscii_convert(void)
.export __Input_keyboard_petscii_convert
.proc __Input_keyboard_petscii_convert: near
  .define _PETSCII _Input + Input_t::keyboard + Input_keyboard_t::petscii

        ldy KERNAL_ZP_KEYBOARD_SCANCODE ; scan_code_prev := scan_code
        sty KERNAL_ZP_KEYBOARD_SCANCODE_PREV
        ldy #Input_sc_none_e       ; ZP_SCANCODE := 0x40
        sty KERNAL_ZP_KEYBOARD_SCANCODE
        lda #$00                   ; I..petscii.shiftkeys := 0x00
        sta _PETSCII + Input_keyboard_petscii_t::shiftkeys

        ldx #$ff                   ; X := counter = 0xff
loop:
        inx                        ; ++counter = X
                                   ; Y := I.keyboard.sc_pressed[X]
        ldy _Input+Input_t::keyboard + Input_keyboard_t::sc_pressed, x
        cpy #Input_sc_none_e       ; while (Input.keyboard.sc_pressed[X]
        beq loop_done              ;        != Input_sc_none_e)

        lda KERNAL_PETSCIITABLE, y      ; A := PETSCII w/o shift-keys
        cmp #$05
        bcs not_shiftkey
        cmp #$03                   ; if (PETSCII >= 5 || PETSCII == 3)
        beq not_shiftkey           ;   not_shiftkey
        ora _PETSCII + Input_keyboard_petscii_t::shiftkeys
        sta _PETSCII + Input_keyboard_petscii_t::shiftkeys
        jmp loop                   ; I.shiftkeys |= PETSCII
not_shiftkey:
        sty KERNAL_ZP_KEYBOARD_SCANCODE ; ZP_SCANCODE := I..sc_pressed[X]
        jmp loop
loop_done:
        ;; set: KERNAL_ZP_KEYBOARD_SCANCODE, I..petscii.shiftkeys

        lda _PETSCII + Input_keyboard_petscii_t::shiftkeys
        asl                        ; A is dividable by 2
        cmp #$08                   ; A = 0x00|02|04|06|08|0a|0c|0e
        bcc petscii_offset_done
        lda #$06                   ; if ((A << 1) >= 0x08) A := 0x06
petscii_offset_done:               ; A = 0x00|02|04|06
        ;; A = 0x00 => UNSHIFT, 0x02 => exactly SHIFT pressed
        ;; 0x04 => exactly COMMODORE pressed
        ;; 0x06 => at least CTRL pressed

        tax                        ; X := offset of PETSCIITABLE_VECTORS
        lda KERNAL_PETSCIITABLE_VECTORS, x
        sta KERNAL_ZP_PETSCIITABLE_VECTOR
        lda KERNAL_PETSCIITABLE_VECTORS+1, x
        sta KERNAL_ZP_PETSCIITABLE_VECTOR+1  ; depending on shift-keys
                                   ; set KERNAL_ZP_PETSCIITABLE_VECTOR

        ;; INPUT_SC_NONE_E will be translated to 0xff instead of
        ;; '\0' = 0x00 in KERNAL_PETSCIITABLE_VECTORS
        lda #$00                        ; A := '\0'
        sta _PETSCII + Input_keyboard_petscii_t::changed
                                        ; I..petscii.changed := false
        ldy KERNAL_ZP_KEYBOARD_SCANCODE      ; Y := scan_code
        cpy KERNAL_ZP_KEYBOARD_SCANCODE_PREV ; if (scan_code ==
        beq charkey_not_changed              ;   scan_code_prev) return
        cpy #Input_sc_none_e                 ; if (scan_code == 0x40)
        beq nullchar_done                    ;   I...char := '\0'
        lda (KERNAL_ZP_PETSCIITABLE_VECTOR), y
nullchar_done:                               ; else I...char := PETSCII
        sta _PETSCII + Input_keyboard_petscii_t::character
                    ; I..petscii.character := PETSCII, using shift-keys
        lda #TRUE                       ; I..petscii.changed := true
        sta _PETSCII + Input_keyboard_petscii_t::changed

charkey_not_changed:          ; any shift-key released before char-key
        rts
.endproc                           ; __Input_keyboard_petscii_convert

;; *******************************************************************
