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

.code

;; *******************************************************************

;; CC65 calling conventions: fastcall 16 bit, last argument
;;
;;   * [byte1, byte0] => [X, A]
;;
.export _Debug_note_TODO
_Debug_note_TODO:

.export Debug_note_isrsafe
.proc Debug_note_isrsafe: near

        pha                        ; backup lowB arg msg_ptr
        txa
        pha                        ; backup highB arg msg_ptr

        ldy #_Debug_t::count
        lda __Debug, y
        cmp #_DEBUG_LIST_SIZE
        bcs return                 ; if (_Debug.count >= size) return;

        

return:
        pla                        ; restore highB arg msg_ptr
        pla                        ; restore lowB arg msg_ptr

        rts
.endproc                                ; Debug_note_isrsafe

;; *******************************************************************
