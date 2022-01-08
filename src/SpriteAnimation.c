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


#include "SpriteAnimation.h"

#include "Sprite.h"

/* ***************************************************************  */

/* Type of free list for memory allocation/free in Sprite RAM.
 */
typedef struct _freelist_block_t {
  /* Next block in free list.  If NULL then this is the last block in
   * the free list.
   */
  struct _freelist_block_t* next;

  /* Size of this block in free list.
   */
  uint8_t size;
} _freelist_block_t;

/* Points to the first free block in free list.  If NULL then there
 * are no free blocks and all possible blocks are filled in Sprite
 * RAM.
 */
static _freelist_block_t* _SpriteAnimation_freelist;

/* ***************************************************************  */

#ifdef DEBUG
void __fastcall__
_SpriteAnimation_debug_alloc_print(void)
{
  static _freelist_block_t *cur_block;

  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    printf("->0x%x %u", cur_block, cur_block->size);
  }

  printf("\n", cur_block);
}
#endif /* DEBUG  */

/* Allocation algorithm for sprite animations in Sprite RAM.
 */
static Sprite_frame_t* __fastcall__
_SpriteAnimation_alloc(uint8_t frame_count)
{
  static _freelist_block_t *cur_block, *tmp_block;
  static _freelist_block_t** prev_next_ptr;

  prev_next_ptr = &_SpriteAnimation_freelist;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {

      /* current block not large enough, try next  */
    if (cur_block->size < frame_count) {
      prev_next_ptr = &cur_block->next;

      continue;

      /* current block larger, move it up in memory  */
    } else if (cur_block->size > frame_count) {
      tmp_block = cur_block;

      (uint8_t*) cur_block = ((uint8_t*) cur_block) + (frame_count << 6);
      cur_block->next = tmp_block->next;
      cur_block->size = tmp_block->size - frame_count;

      *prev_next_ptr = cur_block;

      return (Sprite_frame_t*) tmp_block;

      /* current has same size, delete it from list  */
    } else if (cur_block->size == frame_count) {
      *prev_next_ptr = cur_block->next;

      return  (Sprite_frame_t*) cur_block;
    }

    DEBUG_ERROR("sprite anim, alloc impl!");
  }

  DEBUG_ERROR("sprite anim, alloc failed!");
  return NULL;
}

/* ***************************************************************  */

bool __fastcall__
SpriteAnimation_alloc(SpriteAnimation_t* animation,
                      const Sprite_frame_t* src, uint8_t frame_count)
{
  static Sprite_frame_t* alloc;

  if ((alloc = _SpriteAnimation_alloc(frame_count)) == NULL)
    return false;

  memcpy(alloc, src, frame_count*sizeof(Sprite_frame_t));

  animation->frame_count = frame_count;
  animation->first_frame = SPRITE_LOCATOR_FROMREF((uint16_t) alloc);
  animation->buffer = alloc;

  return true;
}

/* ***************************************************************  */

void __fastcall__
SpriteAnimation_freeall(void)
{
  /* initialize free list with the size of the while Sprite RAM  */
  _SpriteAnimation_freelist
    = (_freelist_block_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST);
  ((_freelist_block_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST))
    ->next = NULL;
  ((_freelist_block_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST))
    ->size = SPRITE_LOCATOR_COUNT;

  /* set all GRAPHIX.ANIMS.SPRITES.SPRITE to NULL  */
  memset(&Graphix.anims.sprites.sprite,
         NULL, sizeof(Graphix.anims.sprites.sprite));
}

/* ***************************************************************  */
