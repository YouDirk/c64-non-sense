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
extern bool __fastcall__ SpriteAnimation_new_alloc(
                   SpriteAnimation_t* animation, uint8_t frame_count);

/* Allocate a sprite ANIMATION in the Sprite RAM and initialize it
 * with the sprite frame buffer SRC.
 *
 * returns: TRUE on success, otherwise FALSE if the allocation fails.
 */
extern bool __fastcall__ SpriteAnimation_new(
                    SpriteAnimation_t* animation,
                    const Sprite_frame_t* src, uint8_t frame_count);

/* ---------------------------------------------------------------  */

/* Free the allocated sprite ANIMATION from Sprite RAM.  It will not
 * touch the member variables of ANIMATION, i.e. pointers will NOT BE
 * SET TO NULL.
 *
 * To heavy use of this function maybe result in an fragmented heap.
 * In that case think about to use SPRITEANIMATION_DELETE_ALL()
 * instead.
 *
 * Hints to avoid fragmentation:
 *
 *   - For every SPRITEANIMATION_DELETE() do allocate (i.e. using
 *     SPRITEANIMATION_NEW()) an sprite animation with the same
 *     ANIMATION.FRAME_COUNT.
 *
 *   - Allocate (i.e. using SPRITEANIMATION_NEW()) more than one
 *     sprite animation by descend ordering in ANIMATION.FRAME_COUNT
 *     (larger FRAME_COUNT before the smaller one).
 */
extern void __fastcall__ SpriteAnimation_delete(
                                  const SpriteAnimation_t* animation);

/* Free all currently allocated sprite animations from Sprite RAM.
 * Useful to avoid an fragmented heap in the Sprite RAM.
 */
extern void __fastcall__ SpriteAnimation_delete_all(void);

/* ***************************************************************  */

/* Returns TRUE if all sprite animations are free´d from Sprite RAM,
 * i.e. after using SPRITEANIMATION_DELETE_ALL() or
 * SPRITEANIMATION_DELETE().  Otherwise FALSE.
 */
extern bool __fastcall__ SpriteAnimation_alloc_is_empty(void);

/* Returns TRUE if there is no more free space for sprite animations to
 * Sprite RAM anymore.  Otherwise FALSE.
 *
 * Notice, it is possible that the function returns FALSE, but you
 * cannot allocate your animation, because the heap is fragmented.
 * The performantest way in that case is to try SPRITEANIMATION_NEW()
 * out and check the return value.
 */
extern bool __fastcall__ SpriteAnimation_alloc_is_full(void);

/* Returns TRUE if there exist more than one big free block in Sprite
 * RAM, which we call an 'fragmented heap'.  Otherwise FALSE.
 */
extern bool __fastcall__ SpriteAnimation_alloc_is_fragmented(void);

/* Returns the number of free sprite frames (in a continuous address
 * space) which can be allocated in a sprite animation, independent
 * from fragmentation of the heap.
 */
extern uint8_t __fastcall__
                    SpriteAnimation_alloc_framesfree_animation(void);

/* Returns the maximum number of free sprite frames in the Sprite RAM
 * as sum of all free fragments.
 *
 * Notice, it is possible that the function returns a number, which is
 * not possible to allocate with a single SPRITEANIMATION_NEW() call,
 * because of an fragmented heap.  The performantest way in that case
 * is to try SPRITEANIMATION_NEW() out and check the return value.  Or
 * use SPRITEANIMATION_ALLOC_FRAMESFREE_ANIMATION() if you need a
 * precise number.
 */
extern uint8_t __fastcall__
                    SpriteAnimation_alloc_framesfree_sum(void);

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
