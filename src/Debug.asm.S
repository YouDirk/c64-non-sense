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


.include "Debug.inc.s"
.include "Debug.gen.s"

.include "Timer.inc.s"

.code

;; *******************************************************************

;; void __Debug_msg_isrsafe([TOS+1, TOS+2] msg, TOS type)
.proc __Debug_msg_isrsafe: near

        lda __Debug + _Debug_t::count
        cmp #_DEBUG_LIST_SIZE           ; if (_Debug.count >= size)
        bcs return_fast                 ;   return;

        lda ptr1
        pha                             ; backup low byte, shared ptr
        lda ptr1 + 1
        pha                             ; backup high byte, shared ptr
        lda sreg
        pha                             ; backup low byte, shared reg
        lda sreg + 1
        pha                             ; backup high byte, shared reg

        php                             ; backup processor status reg
        sei                             ; mask all IRQs for exclusive
                                        ;   access to __DEBUG

  .define _TOS STACK_BASE + SIZEOF_WORD + SIZEOF_ADDR + SIZEOF_BYTE

        ;; ---

        lda __Debug + _Debug_t::end
        sta ptr1
        lda __Debug + _Debug_t::end + 1
        sta ptr1 + 1                    ; ptr1 = &_Debug.end


        jsr Timer_1_get32_isrsafe       ; uint32 Timer_1_get32(void);
        ldy #_Debug_entry_t::time       ; returns [sreg+1, sreg, X, A]
        sta (ptr1), y
        txa
        iny
        sta (ptr1), y
        lda sreg
        iny
        sta (ptr1), y
        lda sreg + 1             ; _Debug.end->time = Timer_1_get32()

        tsx                      ; X := TOS (Top Of processor Stack)

        lda _TOS, x
        ldy #_Debug_entry_t::type
        sta (ptr1), y                   ; _Debug.end->type = arg1;

        lda _TOS + _debug_msgtype_t + 1, x
        ldy #_Debug_entry_t::msg
        sta (ptr1), y
        lda _TOS + _debug_msgtype_t, x
        iny
        sta (ptr1), y                   ; _Debug.end->msg = arg0;

        lda #.sizeof(_Debug_entry_t)
        clc
        adc __Debug + _Debug_t::end
        sta __Debug + _Debug_t::end
        bcc inc_endptr_done
        inc __Debug + _Debug_t::end + 1 ; ++_Debug.end;
inc_endptr_done:

        inc __Debug + _Debug_t::count   ; ++_Debug.count;

        ;; ---
return:
        pla                                  ; if IRQDISABLE was set
        and #MOS6510_PREG_IRQDISABLE_MASK    ;   then we are in an ISR
        bne irqs_were_masked
        cli                                  ; otherwise restore I flag
irqs_were_masked:

        pla
        sta sreg + 1                    ; restore high byte, shared reg
        pla
        sta sreg                        ; restore low byte, shared reg
        pla
        sta ptr1 + 1                    ; restore high byte, shared ptr
        pla
        sta ptr1                        ; restore low byte, shared ptr

return_fast:
        pla                             ; pull arg1 type
        pla                             ; pull highB arg0 msg
        pla                             ; pull lowB arg0 msg

        rts
.endproc                                ; __Debug_msg_isrsafe

;; *******************************************************************

;; CC65 calling conventions: fastcall 16 bit, last argument
;;
;;   * [byte1, byte0] => [X, A]
;;
.export _Debug_error
_Debug_error:

.export Debug_error_isrsafe
.proc Debug_error_isrsafe: near

        pha                             ; __Debug_msg_isrsafe: lowB arg0
        txa
        pha                             ; __Debug_msg_isrsafe: highB arg0

        lda #_debug_error_e
        pha                             ; __Debug_msg_isrsafe: arg1

        jmp __Debug_msg_isrsafe
.endproc                                ; Debug_error_isrsafe

.export _Debug_warn
_Debug_warn:

.export Debug_warn_isrsafe
.proc Debug_warn_isrsafe: near

        pha                             ; __Debug_msg_isrsafe: lowB arg0
        txa
        pha                             ; __Debug_msg_isrsafe: highB arg0

        lda #_debug_warn_e
        pha                             ; __Debug_msg_isrsafe: arg1

        jmp __Debug_msg_isrsafe
.endproc                                ; Debug_warn_isrsafe

.export _Debug_note
_Debug_note:

.export Debug_note_isrsafe
.proc Debug_note_isrsafe: near

        pha                             ; __Debug_msg_isrsafe: lowB arg0
        txa
        pha                             ; __Debug_msg_isrsafe: highB arg0

        lda #_debug_note_e
        pha                             ; __Debug_msg_isrsafe: arg1

        jmp __Debug_msg_isrsafe
.endproc                                ; Debug_note_isrsafe

;; *******************************************************************
