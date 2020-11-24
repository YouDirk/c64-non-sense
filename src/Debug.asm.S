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


.include "Debug.inc.s"
.include "Debug.gen.s"

.include "Timer.inc.s"

.code

;; *******************************************************************

;; CC65 calling conventions: fastcall 16 bit, last argument
;;
;;   * [byte1, byte0] => [X, A]
;;
.export _Debug_note
_Debug_note:

.export Debug_note_isrsafe
.proc Debug_note_isrsafe: near

        pha                             ; backup lowB arg msg_ptr
        txa
        pha                             ; backup highB arg msg_ptr

        lda __Debug + _Debug_t::count
        cmp #_DEBUG_LIST_SIZE
        bcs return_fast            ; if (_Debug.count >= size) return;

        lda ptr1
        pha                             ; backup low byte of shared zp
        lda ptr1 + 1
        pha                             ; backup high byte of shared zp

        ;; ---

        lda __Debug + _Debug_t::end
        sta ptr1
        lda __Debug + _Debug_t::end + 1
        sta ptr1+1                      ; ptr1 = _Debug.end


        jsr Timer_1_get32_isrsafe       ; uint32 Timer_1_get32(void);
        ldy #_Debug_entry_t::time
        sta (ptr1), y
        txa
        iny
        sta (ptr1), y
        lda sreg
        iny
        sta (ptr1), y
        lda sreg + 1

        lda #_debug_note_e
        ldy #_Debug_entry_t::type
        sta (ptr1), y

        tsx
        lda STACK_BASE + SIZEOF_ADDR + 1, x
        ldy #_Debug_entry_t::msg
        sta (ptr1), y
        lda STACK_BASE + SIZEOF_ADDR, x
        iny
        sta (ptr1), y

        lda #.sizeof(_Debug_entry_t)
        clc
        adc __Debug + _Debug_t::end
        sta __Debug + _Debug_t::end
        bcc inc_endptr_done
        inc __Debug + _Debug_t::end + 1
inc_endptr_done:

        inc __Debug + _Debug_t::count

        ;; ---
return:
        pla
        sta ptr1 + 1                    ; restore high byte of shared zp
        pla
        sta ptr1                        ; restore low byte of shared zp

return_fast:
        pla                             ; pull highB arg msg_ptr
        pla                             ; pull lowB arg msg_ptr

        rts
.endproc                                ; Debug_note_isrsafe

;; *******************************************************************
