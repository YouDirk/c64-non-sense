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


.ifnblank DEBUG_INC_S__
.define DEBUG_INC_S__

.include "common.inc.s"

.include "chip-vic.gen.s"

;; *******************************************************************

.ifdef DEBUG_RENDERTIME_IRQ
  .macro DEBUG_RENDERTIME_IRQ_BEGIN color
        lda VIC_BORDERCOLOR             ; push current border color to
        pha                             ;   stack
        lda color
        sta VIC_BORDERCOLOR             ; set debug color
  .endmacro                             ; DEBUG_RENDERTIME_IRQ_BEGIN

  .macro DEBUG_RENDERTIME_IRQ_END
        pla
        sta VIC_BORDERCOLOR             ; pop backup and restore color
  .endmacro                             ; DEBUG_RENDERTIME_IRQ_END
.else ; DEBUG_RENDERTIME_IRQ
  .define DEBUG_RENDERTIME_IRQ_BEGIN(color)
  .define DEBUG_RENDERTIME_IRQ_END
.endif ; DEBUG_RENDERTIME_IRQ

;; *******************************************************************

.ifdef DEBUG_RENDERTIME_FREECPU
  .macro DEBUG_RENDERTIME_FREECPU_BEGIN color
        clc
        lda VIC_BORDERCOLOR             ; push current border color to
        pha                             ;   stack
        lda color
        pha                             ; push debug color to stack
  .endmacro                             ; DEBUG_RENDERTIME_FREECPU_BEGIN

  .macro DEBUG_RENDERTIME_COLOR_SET
        tsx                             ; bordercolor := CPU_STACK[0]
        lda STACK_BASE + 0, x
        sta VIC_BORDERCOLOR
  .endmacro                             ; DEBUG_RENDERTIME_COLOR_SET

  .macro DEBUG_RENDERTIME_FREECPU_END
        pla                             ; pop debug color
        pla
        sta VIC_BORDERCOLOR             ; pop backup and restore color
  .endmacro                             ; DEBUG_RENDERTIME_FREECPU_END
.else ; DEBUG_RENDERTIME_FREECPU
  .define DEBUG_RENDERTIME_FREECPU_BEGIN(color)
  .define DEBUG_RENDERTIME_FREECPU_COLOR_SET()
  .define DEBUG_RENDERTIME_FREECPU_END()
.endif ; DEBUG_RENDERTIME_FREECPU

;; *******************************************************************

.ifdef DEBUG
  ;; Call these macros for debugging output :)
  .define DEBUG_INIT           jsr _Debug_init
  .define DEBUG_RELEASE_PRINT  jsr _Debug_release_print

  ;; Use CC65 fastcall convention
  .define DEBUG_ERROR(msg)     _DEBUG_HELPER msg, Debug_error_isrsafe
  .define DEBUG_WARN(msg)      _DEBUG_HELPER msg, Debug_warn_isrsafe
  .define DEBUG_NOTE(msg)      _DEBUG_HELPER msg, Debug_note_isrsafe

  ;; Use callee-save implementation
  .macro DEBUG_ERROR_REGSAVE msg
        _DEBUG_HELPER_REGSAVE msg, Debug_error_isrsafe
  .endmacro
  .macro DEBUG_WARN_REGSAVE msg
        _DEBUG_HELPER_REGSAVE msg, Debug_warn_isrsafe
  .endmacro
  .macro DEBUG_NOTE_REGSAVE msg
        _DEBUG_HELPER_REGSAVE msg, Debug_note_isrsafe
  .endmacro

  ;; -----------------------------------------------------------------
  ;; Do not call directly!  Call the macros DEBUG_*() above instead.
  ;;
  .global _Debug_init
  .global _Debug_release_print
  .global Debug_error_isrsafe
  .global Debug_warn_isrsafe
  .global Debug_note_isrsafe

  ;; Use CC65 fastcall convention
  .macro _DEBUG_HELPER msg, function
  .scope
        .pushseg                        ; push current segment ("CODE")
        .rodata                         ; org read-only segment
debug_msg:
        .byte msg, $00                  ; dump message & null-terminate
        .popseg                         ; restore "CODE" segment

        lda #<debug_msg                 ; fast call: A=arg1, low byte
        ldx #>debug_msg                 ; fast call: X=arg1, high byte
        jsr function                    ; call it
  .endscope
  .endmacro                             ; _DEBUG_HELPER

  ;; Use callee-save implementation
  .macro _DEBUG_HELPER_REGSAVE msg, function
  .scope
        pha                             ; push A
        txa
        pha                             ; push X
        tya
        pha                             ; push Y
        _DEBUG_HELPER msg, function     ; call: CC65 fastcall convention
        pla
        tay                             ; pull Y
        pla
        tax                             ; pull X
        pla                             ; pull A
  .endscope
  .endmacro                             ; _DEBUG_HELPER_REGSAVE

;; *******************************************************************
.else ; DEBUG
  .define DEBUG_INIT
  .define DEBUG_RELEASE_PRINT
  .define DEBUG_ERROR(msg)
  .define DEBUG_WARN(msg)
  .define DEBUG_NOTE(msg)
  .define DEBUG_ERROR_REGSAVE(msg)
  .define DEBUG_WARN_REGSAVE(msg)
  .define DEBUG_NOTE_REGSAVE(msg)
.endif ; DEBUG

.endif                                  ; DEBUG_INC_S__
