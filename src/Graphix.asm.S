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


.include "Graphix.inc.s"

.include "chip-vic.gen.s"

.code

;; *******************************************************************

;; Find out if we are on a PAL or NTSC machine?  Needed for Timer
;; configuration.
;;
;; void __fastcall__ _Graphix_init_vic_detect(void)
.export __Graphix_init_vic_detect
.proc __Graphix_init_vic_detect: near

        ldx #$00                        ; X := 0x00, cycle counter
        sei                             ; race between vic and cpu measured
        ;; -----------------------------------------------------------

rasterline_not_overflow:
        lda VIC_RASTERLINE
rasterline_not_inc:
        cmp VIC_RASTERLINE
        beq rasterline_not_inc          ; while not changed
        bmi rasterline_not_overflow     ; while not overflows
        ;; akku = rasterline_max (without bit 8)
        ;; 0x37 -> 0..311 rasterlines (PAL with VIC 6569)
        ;; 0x06 -> 0..262 rasterlines (NTSC with VIC 6567R8)
        ;; 0x05 -> 0..261 rasterlines (old NTSC with VIC 6567R56A)
        tay                             ; Y := rasterline_max
        and #VIC_RASTERL_MAX_PAL_MASK   ; A := 0x30 if PAL, 0x00 if NTSC
        sta _Graphix + Graphix_t::is_pal
        beq done                        ; if (NTSC): Y = 0x05|0x06

cycles_count:
        ;; Y = 0x37
        inx                             ; X++ := cycle counter
        lda VIC_RASTERLINE
        bpl cycles_count                ; while (RASTERLINE >= 0)
        ;; X := 0x7f cylces in VICE (PAL with VIC 6569)
        ;; X := 0x9b cylces in VICE (PAL-N with VIC 6572)
        txa                             ; A := cycles
        cmp #VIC_RASTERL_CYCLES_PAL_THRESHOLD
        bmi done                        ; if (cycles < 0): Y = 0x37
        ldy #Graphix_vicrev_paln6572_e  ; Y := 0x40

        ;; -----------------------------------------------------------
done:
        ;; Y = Graphix_vicrev_{ntsc/pal}_e
        cli                             ; restore I status flag
        sty _Graphix + Graphix_t::vic_revision

        rts
.endproc                                ; __Graphix_vic_detect

;; *******************************************************************

;; void __fastcall__ Graphix_rasterline_isr(void)
.export Graphix_rasterline_isr
.proc Graphix_rasterline_isr: near
.ifdef DEBUG_RENDERTIME_IRQ
        lda #VIC_COLOR_YELLOW           ; we don´t need to push border
        sta VIC_BORDERCOLOR             ; color, this isr set it activ
.endif ; DEBUG_RENDERTIME_IRQ

  .define _SPR_SPR SpriteManager_buffer_t::sprite

        clc
        lda _Graphix_buffer_back_ptr
        sta ZP_ISR_PTR1
        adc #Graphix_buffer_t::sprites + _SPR_SPR
        sta ZP_ISR_PTR2
            ;; carry set if addition overflows
        lda _Graphix_buffer_back_ptr + 1
        sta ZP_ISR_PTR1 + 1             ; PTR1 = _Graphix_buffer_back_ptr
        adc #$00
        sta ZP_ISR_PTR2 + 1   ; PTR2 = &Graphix.buffer.sprites.sprite[0]

        ;; -----------------------------------------------------------

  .define _SPR_SET SpriteManager_buffer_t::set
  .define _SPR_SET_ENA SpriteManager_buffer_set_t::enabled
  .define _SPR_SET_MC0B01 SpriteManager_buffer_set_t::multicolor_0b01
  .define _SPR_SET_MC0B11 SpriteManager_buffer_set_t::multicolor_0b11

        .pushseg                        ; push current segment ("CODE")
        .bss                            ; org uninitialized segment
sprites_hi_x:
        .res 1                          ; hi_x of pos_x for 0xd010
sprites_mcolors:
        .res 1                          ; mcolors for 0xd01c
sprites_expy:
        .res 1                          ; expy for 0xd017
sprites_expx:
        .res 1                          ; expx for 0xd01d
sprites_bgprio:
        .res 1                          ; bgprio for 0xd01b
        .popseg                         ; restore "CODE" segment

        ldy #Graphix_buffer_t::sprites + _SPR_SET + _SPR_SET_ENA
        lda (ZP_ISR_PTR1), y
        sta VIC_SPR_ENABLE              ; set sprite enable bits

        ldy #Graphix_buffer_t::sprites + _SPR_SET + _SPR_SET_MC0B01
        lda (ZP_ISR_PTR1), y
        sta VIC_SPR_MCOLOR0_0B01        ; set multicolor0_0b01

        ldy #Graphix_buffer_t::sprites + _SPR_SET + _SPR_SET_MC0B11
        lda (ZP_ISR_PTR1), y
        sta VIC_SPR_MCOLOR1_0B11        ; set multicolor1_0b11

        ldx #$00                        ; X := 0x00, sprite_counter
sprites_loop:                           ; do {
        ldy #Sprite_t::set + Sprite_set_t::color
        lda (ZP_ISR_PTR2), y
        sta VIC_SPR_COLORS, x           ; set sprite color

        txa
        asl
        tax                             ; spr_cnt := 2*spr_cnt

        ldy #Sprite_t::set + Sprite_set_t::pos_x
        lda (ZP_ISR_PTR2), y
        sta VIC_SPR_ARRAY, x            ; set sprite x position
        inx                             ; spr_cnt := 2*spr_cnt + 1

        iny
        lda (ZP_ISR_PTR2), y            ; A := high byte of pos_x
        lsr
          ;; carry set if msb of high byte was set
        ror sprites_hi_x                ; hi_x
                                        ;   := (carry | hi_x) >> 1
        ldy #Sprite_t::set + Sprite_set_t::pos_y
        lda (ZP_ISR_PTR2), y
        sta VIC_SPR_ARRAY, x            ; set sprite y position

        txa
        lsr
        tax                             ; spr_cnt := sprite_counter

        ldy #Sprite_t::set + Sprite_set_t::props
        lda (ZP_ISR_PTR2), y
  .if .not Sprite_props_multicolor_mask = (1 << 0)
        .error "Sprite_props_multicolor_mask != 0x01!"
  .endif
        lsr                             ; mcolors
        ror sprites_mcolors             ;   := (carry | mcolors) >> 1
  .if .not Sprite_props_scale_y_mask = (1 << 1)
        .error "Sprite_props_scale_y_mask != 0x02!"
  .endif
        lsr                             ; expy
        ror sprites_expy                ;   := (carry | expy) >> 1
  .if .not Sprite_props_scale_x_mask = (1 << 2)
        .error "Sprite_props_scale_x_mask != 0x04!"
  .endif
        lsr                             ; expx
        ror sprites_expx                ;   := (carry | expx) >> 1
  .if .not Sprite_props_prio_bground_mask = (1 << 3)
        .error "Sprite_props_prio_bground_mask != 0x08!"
  .endif
        lsr                             ; bgprio
        ror sprites_bgprio              ;   := (carry | bgprio) >> 1

        clc
        lda #.sizeof(Sprite_t)
        adc ZP_ISR_PTR2
        sta ZP_ISR_PTR2
        bcc inc_highbyte_done
        inc ZP_ISR_PTR2 + 1             ; ++Graphix.[...].sprite
inc_highbyte_done:

        inx                             ; ++sprite_counter
        cpx #SPRITEMANAGER_SPRITES_MAXCOUNT
        bcc sprites_loop                ; } while (sprite_counter < 8);
sprites_done:

        lda sprites_hi_x                ; save properties to registers
        sta VIC_SPRHI_X
        lda sprites_mcolors
        sta VIC_SPR_MCOLOR
        lda sprites_expy
        sta VIC_SPR_EXPY
        lda sprites_expx
        sta VIC_SPR_EXPX
        lda sprites_bgprio
        sta VIC_SPR_BGPRIO

        ;; -----------------------------------------------------------

  .define _SET_SCROLL_Y Graphix_buffer_set_t::scroll_y
  .define _SET_SCROLL_X Graphix_buffer_set_t::scroll_x
  .define _SET_BORDERCOLOR Graphix_buffer_set_t::bordercolor

        ldy #Graphix_buffer_t::set + _SET_SCROLL_Y
        lda (ZP_ISR_PTR1), y
        and #VIC_CTRL1_YSCROLL_MASK
        ora #(VIC_CTRL1_MODE & ~VIC_CTRL1_YSCROLL_MASK)
        sta VIC_CTRL1                   ; set y scroll

        ldy #Graphix_buffer_t::set + _SET_SCROLL_X
        lda (ZP_ISR_PTR1), y
        and #VIC_CTRL2_XSCROLL_MASK
        ora #(VIC_CTRL2_MODE & ~VIC_CTRL2_XSCROLL_MASK)
        sta VIC_CTRL2                   ; set x scroll

        ;; must be the last statement, for correct DEBUG_RENDERTIME_IRQ
        ldy #Graphix_buffer_t::set + _SET_BORDERCOLOR
        lda (ZP_ISR_PTR1), y
        sta VIC_BORDERCOLOR             ; set bordercolor
.ifdef DEBUG_RENDERTIME_IRQ
        ;; nothing to do, because VIC.bordercolor will be set above
.endif ; DEBUG_RENDERTIME_IRQ

        rts
.endproc                                ; Graphix_rasterline_isr

;; *******************************************************************

;; void __fastcall__ Graphix_buffer_swap(void)
.export _Graphix_buffer_swap
.proc _Graphix_buffer_swap: near
        DEBUG_RENDERTIME_IRQ_BEGIN #VIC_COLOR_VIOLET

        ;; Loading GRAPHIX_BUFFER_SHARED_PTR to PTR1 is atomar,
        ;; because the high byte equals GRAPHIX_BUFFER_BACK_PTR, as
        ;; ASSERTED in GRAPHIX_INIT().
.ifndef CONF_DOUBLE_BUFFERING
        lda _Graphix_buffer_shared_ptr
        sta ptr1
        lda _Graphix_buffer_shared_ptr + 1
        sta ptr1 + 1                    ; PTR1 := shared_buffer
.else ; CONF_DOUBLE_BUFFERING
        lda _Graphix_buffer_back_ptr
        sta ptr1
        lda _Graphix_buffer_back_ptr + 1
        sta ptr1 + 1                    ; PTR1 := back_buffer
.endif ; CONF_DOUBLE_BUFFERING

        ldy #.sizeof(Graphix_buffer_t)  ; for (Y := sizeof(buffer_t);
loop:                                   ;      Y-- != 0;) {
        dey
        lda _Graphix + Graphix_t::buffer, y
        sta (ptr1), y                   ; shared_buffer[Y]
        cpy #$00                        ;   := Graphix.buffer[Y]
        bne loop
loop_done:                              ; }

        DEBUG_RENDERTIME_IRQ_END
        rts
.endproc                                ; _Graphix_buffer_swap

;; *******************************************************************
