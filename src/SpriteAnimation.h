/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef SPRITEANIMATION_H__
#define SPRITEANIMATION_H__

#include "common.h"
#include "SpriteAnimation.gen.h"

#include "Sprite.h"

/* ***************************************************************  */

/* Allocate a sprite ANIMATION in the Sprite RAM, but do not
 * initialize the allocated RAM.
 *
 * Do not forget to set ANIMATION->BUFFER[FRAME_COUNT-1].TICK_COUNT
 *                   to SPRITE_FRAME_TICKCOUNT_LAST_MASK
 * after the call of this function!
 *
 * returns: TRUE on success, otherwise FALSE if the allocation fails.
 */
extern bool __fastcall__ SpriteAnimation_alloc_uninit(
                   SpriteAnimation_t* animation, uint8_t frame_count);

/* Allocate a sprite ANIMATION in the Sprite RAM and initialize it
 * with the sprite frame buffer SRC.
 *
 * returns: TRUE on success, otherwise FALSE if the allocation fails.
 */
extern bool __fastcall__ SpriteAnimation_alloc(
                    SpriteAnimation_t* animation,
                    const Sprite_frame_t* src, uint8_t frame_count);

/* ---------------------------------------------------------------  */

/* Free the allocated sprite ANIMATION from Sprite RAM.  It will not
 * touch the member variables of ANIMATION, i.e. pointers will NOT BE
 * SET TO NULL.
 *
 * To heavy use of this function will result in an fragment heap.  In
 * that case think about to use SPRITEANIMATION_FREEALL() instead.
 */
extern void __fastcall__ SpriteAnimation_free(
                                  const SpriteAnimation_t* animation);

/* Free all currently allocated sprite animations from Sprite RAM.
 * Useful to avoid an fragmented heap in the Sprite RAM.
 */
extern void __fastcall__ SpriteAnimation_freeall(void);

/* ***************************************************************  */

#ifdef DEBUG

#define SPRITEANIMATION_DEBUG_ALLOC_PRINT()                          \
                                  _SpriteAnimation_debug_alloc_print()

/* Do not call directly!  Call the SPRITEANIMATION_DEBUG_ALLOC_PRINT()
 * macro above instead.
 */
extern void __fastcall__ _SpriteAnimation_debug_alloc_print(void);

#else /* DEBUG  */
#define SPRITEANIMATION_DEBUG_ALLOC_PRINT()
#endif /* DEBUG  */

/* ***************************************************************  */

#endif /* SPRITEANIMATION_H__  */
