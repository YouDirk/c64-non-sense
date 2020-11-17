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


.ifnblank DEBUG_INC_S__
.define DEBUG_INC_S__

.include "common.inc.s"

;; *******************************************************************

.if .defined(DEBUG_IRQ_RENDERTIME)
  .macro DEBUG_RENDERTIME_BEGIN color
        lda VIC_BORDERCOLOR             ; push current border color to
        pha                             ;   stack
        lda color
        sta VIC_BORDERCOLOR             ; set debug color
  .endmacro                             ; DEBUG_RENDERTIME_BEGIN

  .macro DEBUG_RENDERTIME_END
        pla
        sta VIC_BORDERCOLOR             ; pop backup and restore color
  .endmacro                             ; DEBUG_RENDERTIME_END
.else ; DEBUG_IRQ_RENDERTIME
  .define DEBUG_RENDERTIME_BEGIN(color)
  .define DEBUG_RENDERTIME_END
.endif ; DEBUG_IRQ_RENDERTIME

;; *******************************************************************

.ifdef DEBUG
  ;; Call these macros for debugging output :)
  .define DEBUG_INIT               jsr _Debug_init
  .define DEBUG_RELEASE_PRINT      jsr _Debug_release_print

  .define DEBUG_ERROR(msg)         _DEBUG_HELPER msg, _Debug_error
  .define DEBUG_WARN(msg)          _DEBUG_HELPER msg, _Debug_warn
  .define DEBUG_NOTE(msg)          _DEBUG_HELPER msg, _Debug_note

  ;; -----------------------------------------------------------------
  ;; Do not call directly!  Call the macros DEBUG_*() above instead.
  ;;
  .import _Debug_init
  .import _Debug_release_print
  .import _Debug_error
  .import _Debug_warn
  .import _Debug_note

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

;; *******************************************************************
.else ; DEBUG
  .define DEBUG_INIT
  .define DEBUG_RELEASE_PRINT
  .define DEBUG_ERROR(msg)
  .define DEBUG_WARN(msg)
  .define DEBUG_NOTE(msg)
.endif ; DEBUG

.endif                                  ; DEBUG_INC_S__
