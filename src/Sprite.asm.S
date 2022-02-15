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


.include "Sprite.inc.s"

.include "chip-vic.gen.s"

.include "Graphix.gen.s"
.include "SpriteManager.gen.s"
.include "Engine.gen.s"

.code

;; *******************************************************************

;; void __fastcall__ Sprite_tick_all(void)
.export _Sprite_tick_all
.proc _Sprite_tick_all: near
        DEBUG_RENDERTIME_IRQ_BEGIN VIC_COLOR_LIGHTGREEN

  .define _BUF_SPRTS Graphix_t::buffer + Graphix_buffer_t::sprites
  .define _BUF_ARR _BUF_SPRTS + SpriteManager_buffer_t::sprite
  .define _BUF_LAST _BUF_ARR + .sizeof(Sprite_buffer_t)*(SPRITEMANAGER_SPRITES_COUNT-1)

  .define _ANI_SPRTS Graphix_t::anims + Graphix_animations_t::sprites
  .define _ANI_SET   _ANI_SPRTS + SpriteManager_animations_t::set
  .define _ANI_ARR _ANI_SET + SpriteManager_animations_set_t::sprite

        lda #>(_Graphix + _BUF_LAST)
        sta ptr2 + 1
        lda #<(_Graphix + _BUF_LAST)
        sta ptr2                        ; PTR2 := &Grx.buf...sprite[7]
        ldx #SPRITEMANAGER_SPRITES_COUNT*SIZEOF_ADDR - 1
                                        ; for(X=15; X>=0; X-=2) {
loop:
        lda _Graphix + _ANI_ARR, x
        sta ptr1 + 1
        dex
        lda _Graphix + _ANI_ARR, x
        sta ptr1                        ; PTR1 := Grx.anim...sprite[X]

        ora ptr1 + 1
        bne *+5                         ; absolute addressed branch
        jmp loop_next                   ; if (PTR1 == NULL) continue

        ;; --- not NULL animation ---

        ;; Animation already ticked?  Then just set Sprite Locator
        ldy #SpriteAnimation_t::_stamp_lasttick
        lda _Engine + Engine_t::tick_count
        cmp (ptr1), y
        bne _already_ticked_done        ; if (anim:stamp == cur_stamp) {
_already_ticked:
        ldy #SpriteAnimation_t::current_locator
        lda (ptr1), y
        ldy #Sprite_buffer_t::set + Sprite_buffer_set_t::locator
        sta (ptr2), y                   ;   buf:locator := anim:locator
        jmp loop_next                   ;   continue
_already_ticked_done:                   ; } if
        ; A = Engine.tick_count; Y = SpriteAnimation_t::_stamp_lasttick
        sta (ptr1), y                   ; anim:stamp := cur_stamp

        ;; Just tick and set Sprite Locator, w/o changing frame?
        ldy #SpriteAnimation_t::current_frame
        lda (ptr1), y
        sta ptr3
        iny
        lda (ptr1), y
        sta ptr3 + 1                    ; PTR3 := anim:cur_frame
        ldy #SpriteAnimation_t::current_tick
        lda (ptr1), y
        ldy #Sprite_frame_t::tick_count
        cmp (ptr3), y
        bcs _same_frame_done            ; if (anim:tick < frame:ticks) {
_same_frame:
        ; A = anim:cur_tick;
        clc
        adc #$01
        ldy #SpriteAnimation_t::current_tick
        sta (ptr1), y                   ;   ++anim:cur_tick

        ldy #SpriteAnimation_t::current_locator
        lda (ptr1), y
        ldy #Sprite_buffer_t::set + Sprite_buffer_set_t::locator
        sta (ptr2), y                   ;   buf:locator := anim:locator
        jmp loop_next                   ;   continue
_same_frame_done:                       ; } if
        ; A = anim:cur_tick;

        ;; --- new sprite frame in animation ---

        ldy #SpriteAnimation_t::current_tick
        lda #$00
        sta (ptr1), y                   ; anim:cur_tick := 0

        ;; Just next sprite frame in animation, w/o replay?
        ldy #SpriteAnimation_t::current_frame_no
        lda (ptr1), y
        clc
        adc #$01
        ldy #SpriteAnimation_t::frame_count
        cmp (ptr1), y
        bcs _no_replay_done             ; if (a:frame_no+1 < a:count) {
_no_replay:
        ; A = anim:current_frame_no + 1;
        ldy #SpriteAnimation_t::current_frame_no
        sta (ptr1), y                   ;   ++anim:current_frame_no
        ldy #SpriteAnimation_t::current_locator
        lda (ptr1), y
        clc
        adc #$01
        sta (ptr1), y                   ;   ++anim:current_locator
        ldy #Sprite_buffer_t::set + Sprite_buffer_set_t::locator
        sta (ptr2), y                   ;   ++buf:locator
        ldy #SpriteAnimation_t::current_frame
        lda ptr3
        clc
        adc #.sizeof(Sprite_frame_t)
        sta (ptr1), y
        iny
        lda ptr3 + 1
        adc #$00
        sta (ptr1), y                   ;   ++anim:current_frame
        jmp loop_next                   ;   continue
_no_replay_done:                        ; } if

        ;; --- replay animation ---

        ldy #SpriteAnimation_t::current_frame_no
        lda #$00
        sta (ptr1), y                   ; anim:current_frame_no := 0
        ldy #SpriteAnimation_t::locator
        lda (ptr1), y
        ldy #SpriteAnimation_t::current_locator
        sta (ptr1), y                   ; anim:cur_locator := a:locator
        ldy #Sprite_buffer_t::set + Sprite_buffer_set_t::locator
        sta (ptr2), y                   ; buf:locator := anim:locator
        ldy #SpriteAnimation_t::buffer
        lda (ptr1), y
        ldy #SpriteAnimation_t::current_frame
        sta (ptr1), y
        ldy #SpriteAnimation_t::buffer + 1
        lda (ptr1), y
        ldy #SpriteAnimation_t::current_frame + 1
        sta (ptr1), y                   ; anim:cur_frame := a:buffer

loop_next:
        lda ptr2
        sec
        sbc #.sizeof(Sprite_buffer_t)
        sta ptr2
        bcs _ptr2_dec_done
        dec ptr2 + 1                    ; --Grx.buf...sprite
_ptr2_dec_done:
        dex
        bmi *+5
        jmp loop                        ; absolute addressed branch
loop_done:                              ; } while (X >= 0)

        DEBUG_RENDERTIME_IRQ_END
        rts
.endproc                                ; _Sprite_tick_all

;; *******************************************************************
