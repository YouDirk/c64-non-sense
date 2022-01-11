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

  /* Size (in 64 byte segments) of this block in the free list.
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
  static uint8_t count;

  count = 0;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    count += cur_block->size;

    printf("->0x%04x %u", cur_block, cur_block->size);
  }

  printf("\n%u of %u frames free.\n", count, SPRITE_LOCATOR_COUNT);
}
#endif /* DEBUG  */

/* Allocation algorithm for sprite animations in Sprite RAM.
 *
 * returns: Pointer to allocated sprite frame buffers, otherwise NULL.
 */
static Sprite_frame_t* __fastcall__
_SpriteAnimation_alloc(uint8_t frame_count)
{
  static _freelist_block_t *cur_block;
  static _freelist_block_t** prev_next_ptr;

  static uint8_t cur_size;

#ifdef DEBUG_ASSERTION_CHECK
  if (frame_count == 0) {
    DEBUG_ERROR("sprite anim, alloc zero size!");
    return NULL;
  }
#endif /* DEBUG_ASSERTION_CHECK  */

  prev_next_ptr = &_SpriteAnimation_freelist;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    cur_size = cur_block->size;

    /* IFs are ordered by likelihood, to improve performance.
     */

    /* current block not large enough, try next  */
    if (cur_size < frame_count) {
      prev_next_ptr = &cur_block->next;
      continue;
    }

    /* current block larger, use the upper part  */
    if (cur_size > frame_count) {
      cur_size -= frame_count;

      cur_block->size = cur_size;

      return (Sprite_frame_t*) ((uint8_t*) cur_block + (cur_size << 6));
    }

    /* current block has same size, delete it from list  */
    *prev_next_ptr = cur_block->next;
    return (Sprite_frame_t*) cur_block;
  }

  DEBUG_ERROR("sprite anim, out of memory!");
  return NULL;
}

/* ***************************************************************  */

bool __fastcall__
SpriteAnimation_new_alloc(SpriteAnimation_t* animation,
                          uint8_t frame_count)
{
  static Sprite_frame_t* alloc;

  if ((alloc = _SpriteAnimation_alloc(frame_count)) == NULL)
    return false;

  animation->frame_count = frame_count;
  animation->first_frame = SPRITE_LOCATOR_FROMREF((uint16_t) alloc);
  animation->buffer = alloc;

  return true;
}

bool __fastcall__
SpriteAnimation_new(SpriteAnimation_t* animation,
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
SpriteAnimation_delete(const SpriteAnimation_t* animation)
{
  static _freelist_block_t *cur_block, *tmp_block;
  static void* alloc;

  // TODO
  DEBUG_WARN("sprite anim, free not full implemented!");

  alloc = animation->buffer;

#ifdef DEBUG_ASSERTION_CHECK
  if (animation->frame_count == 0) {
    DEBUG_ERROR("sprite anim, free zero size!");
    return;
  }
#endif /* DEBUG_ASSERTION_CHECK  */

  if (_SpriteAnimation_freelist == NULL) {
#ifdef DEBUG_ASSERTION_CHECK
    if (alloc < SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST)
        || alloc >= SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)
        || animation->frame_count > SPRITE_LOCATOR_COUNT) {
      DEBUG_ERROR("sprite anim, free 1 useless!");
      return;
    }
#endif /* DEBUG_ASSERTION_CHECK  */

    _SpriteAnimation_freelist = alloc;
    _SpriteAnimation_freelist->next = NULL;
    _SpriteAnimation_freelist->size = animation->frame_count;

    return;
  }

  /* -------------------------------------------------------------  */

  for (cur_block=_SpriteAnimation_freelist; cur_block->next != NULL;
       cur_block=cur_block->next) {
    if (alloc >= cur_block->next) continue;

    if ((uint8_t*) cur_block + (cur_block->size << 6) == alloc) {

      if ((uint8_t*) alloc + (animation->frame_count << 6) < (uint8_t*) cur_block->next){
        cur_block->size += animation->frame_count;
        return;
      } else if ((uint8_t*) alloc + (animation->frame_count << 6) > (uint8_t*) cur_block->next) {
        DEBUG_ERROR("sprite anim, free too large!");
        return;
      }

      cur_block->size += animation->frame_count + cur_block->next->size;
      cur_block->next = cur_block->next->next;

      return;
    } else if ((uint8_t*) cur_block + (cur_block->size << 6) < alloc) {
      DEBUG_ERROR("sprite anim, double free!");
      return;
    }

    if ((uint8_t*) alloc + (animation->frame_count << 6) == (uint8_t*) cur_block->next) {
      (uint8_t*) tmp_block = (uint8_t*) cur_block->next - (animation->frame_count << 6);
      tmp_block->size = cur_block->next->size + animation->frame_count;
      tmp_block->next = cur_block->next->next;

      cur_block->next = tmp_block;

      return;
    } else if ((uint8_t*) alloc + (animation->frame_count << 6) < (uint8_t*) cur_block->next) {
      tmp_block = alloc;

      tmp_block->size = animation->frame_count;
      tmp_block->next = cur_block->next;
      cur_block->next = tmp_block;

      return;
    }

    DEBUG_ERROR("sprite anim, free 2 large!");
    return;
  }

  /* -------------------------------------------------------------  */

  if (alloc >= SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)) {
    DEBUG_ERROR("sprite anim, outside ram!");
    return;
  }

  // TODO
}

/* ***************************************************************  */

void __fastcall__
SpriteAnimation_delete_all(void)
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
