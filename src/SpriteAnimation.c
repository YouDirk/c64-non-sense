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

  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    printf("->0x%04x %u", cur_block, cur_block->size);
  }

  printf("\n  %u(max %u)/%u free, frag=%u e=%u f=%u\n",
         SpriteAnimation_alloc_framesfree_sum(),
         SpriteAnimation_alloc_framesfree_animation(),
         SPRITE_LOCATOR_COUNT,
         SpriteAnimation_alloc_is_fragmented(),
         SpriteAnimation_alloc_is_empty(),
         SpriteAnimation_alloc_is_full());
}
#endif /* DEBUG  */

/* ***************************************************************  */

bool __fastcall__
SpriteAnimation_alloc_is_empty(void)
{
  return _SpriteAnimation_freelist != NULL
    && (_SpriteAnimation_freelist->size == SPRITE_LOCATOR_COUNT);
}

bool __fastcall__
SpriteAnimation_alloc_is_full(void)
{
  return _SpriteAnimation_freelist == NULL;
}

bool __fastcall__
SpriteAnimation_alloc_is_fragmented(void)
{
  return _SpriteAnimation_freelist != NULL
         && _SpriteAnimation_freelist->next != NULL;
}

uint8_t __fastcall__
SpriteAnimation_alloc_framesfree_animation(void)
{
  static _freelist_block_t* cur_block;
  static uint8_t max, cur;

  max = 0;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    cur = cur_block->size;

    if (cur > max) max = cur;
  }

  return max;
}

uint8_t __fastcall__
SpriteAnimation_alloc_framesfree_sum(void)
{
  static _freelist_block_t* cur_block;
  static uint8_t count;

  count = 0;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {
    count += cur_block->size;
  }

  return count;
}

/* ***************************************************************  */

/* Allocation algorithm for sprite animations in Sprite RAM.
 *
 * returns: Pointer to allocated sprite frame buffers, otherwise NULL.
 */
static Sprite_frame_t* __fastcall__
_SpriteAnimation_alloc(uint8_t frame_count)
{
  static _freelist_block_t* cur_block;
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

  DEBUG_ERROR("sprite anim, new out of mem!");
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

/* *******************************************************************
 *
 * cases for SPRITE_ANIMATION_DELETE()
 * -----------------------------------
 *
 *                      ._________________________.
 *       SA_freelist ---'                         |
 *                          .-------.   .-------|^|^^^^^|
 *                      |   |   1   |   |   2   | '->   |
 *     begin Sprite RAM |   '-------'   '-------|_______|
 *
 *            .___________________________________.
 *            |                                   |
 *            | .-------------------------------. |
 *      |^^^^^|^|-------.   .-------.   .-------|^|^^^^^|
 *      |       |   3   | 6 |   4   | 6 |   5   | '->   |
 *      |_______|-------'   '-------'   '-------|_______|
 *              '-------------------------------'
 *
 *                        ._________________________.
 *                        |                         '-> NULL
 *                  |^^^^^|^|-------.   .-------.
 *                  |       |   7   |   |   8   |   |
 *                  |_______|-------'   '-------'   | end Sprite RAM
 *
 *
 * Useful for debugging, using the _DEBUG_CASES output.
 */

#define _DEBUG_CASES 1

#if _DEBUG_CASES == 1
#  define _DEBUG_CASE(no) DEBUG_NOTE("sprite del, case " no)
#else
#  define _DEBUG_CASE(no)
#endif /* _DEBUG_CASES  */

void __fastcall__
SpriteAnimation_delete(const SpriteAnimation_t* animation)
{
  static _freelist_block_t *cur_block, *prev_block;
  static uint8_t *alloc, *alloc_end, *prev_block_end;
  static uint16_t alloc_bufsize, prev_block_bufsize;

  static uint8_t alloc_framecount;

  alloc = (uint8_t*) animation->buffer;
  alloc_framecount = animation->frame_count;

#ifdef DEBUG_ASSERTION_CHECK
  if (alloc_framecount == 0) {
    DEBUG_ERROR("sprite anim, free zero size!");
    return;
  }
  if (alloc == NULL) {
    DEBUG_ERROR("sprite anim, free null ptr!");
    return;
  }
#endif /* DEBUG_ASSERTION_CHECK  */

  if (_SpriteAnimation_freelist == NULL) {
#ifdef DEBUG_ASSERTION_CHECK
    if (alloc < (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST)
        || alloc >= (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)
        || alloc_framecount > SPRITE_LOCATOR_COUNT) {
      DEBUG_ERROR("sprite anim, free useless!");
      return;
    }
#endif /* DEBUG_ASSERTION_CHECK  */

    _SpriteAnimation_freelist = (_freelist_block_t*) alloc;
    _SpriteAnimation_freelist->next = NULL;
    _SpriteAnimation_freelist->size = alloc_framecount;

    _DEBUG_CASE("1+8");
    return;

  } /* if (_SpriteAnimation_freelist == NULL)  */

  alloc_bufsize = alloc_framecount  << 6;
  alloc_end = (uint8_t*) alloc + alloc_bufsize;

  /* -------------------------------------------------------------  */

  prev_block = _SpriteAnimation_freelist;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {

    if (alloc >= (uint8_t*) cur_block) {
      prev_block = cur_block;
      continue;
    }

    prev_block_bufsize = prev_block->size << 6;
    prev_block_end = (uint8_t*) prev_block + prev_block_bufsize;

    /* ---  */

    if (alloc == prev_block_end) {
      if (alloc_end < (uint8_t*) cur_block) {
        prev_block->size += alloc_framecount;

        _DEBUG_CASE("3");
        return;
      }

      if (alloc_end == (uint8_t*) cur_block) {
        prev_block->next = cur_block->next;
        prev_block->size += alloc_framecount + cur_block->size;

        _DEBUG_CASE("6");
        return;
      }

      DEBUG_ERROR("sprite anim, free too large!");
      return;
    }

    /* ---  */

    if (alloc_end == (uint8_t*) cur_block) {
      ((_freelist_block_t*) alloc)->next = cur_block->next;
      ((_freelist_block_t*) alloc)->size = cur_block->size
                                                   + alloc_framecount;

      if (cur_block == prev_block) {
        _SpriteAnimation_freelist = (_freelist_block_t*) alloc;

        _DEBUG_CASE("2");
        return;
      }

      prev_block->next = (_freelist_block_t*) alloc;

      _DEBUG_CASE("5");
      return;
    }

    /* ---  */

    if (alloc > prev_block_end) {
      ((_freelist_block_t*) alloc)->next = cur_block;
      ((_freelist_block_t*) alloc)->size = alloc_framecount;

      prev_block->next = (_freelist_block_t*) alloc;

      _DEBUG_CASE("4");
      return;
    }

    /* ---  */

    if (cur_block == prev_block
        && alloc < (uint8_t*) _SpriteAnimation_freelist) {
      ((_freelist_block_t*) alloc)->next = cur_block;
      ((_freelist_block_t*) alloc)->size = alloc_framecount;

      _SpriteAnimation_freelist = (_freelist_block_t*) alloc;

      _DEBUG_CASE("1");
      return;
    }

    /* ---  */

    DEBUG_ERROR("sprite anim, double free!");
    return;
  } /* for (cur_block=; ...; cur_block=cur_block->next)  */

  /* -------------------------------------------------------------  */

#ifdef DEBUG_ASSERTION_CHECK
  if (alloc_end >= (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)) {
    DEBUG_ERROR("sprite anim, del outside ram!");
    return;
  }
#endif /* DEBUG_ASSERTION_CHECK  */

  prev_block_bufsize = prev_block->size << 6;
  prev_block_end = (uint8_t*) prev_block + prev_block_bufsize;

  /* ---  */

  if (alloc == prev_block_end) {
    prev_block->size += alloc_framecount;

    _DEBUG_CASE("7");
    return;
  }

  /* ---  */

  if (alloc > prev_block_end) {
    ((_freelist_block_t*) alloc)->next = NULL;
    ((_freelist_block_t*) alloc)->size = alloc_framecount;
    prev_block->next = (_freelist_block_t*) alloc;

    _DEBUG_CASE("8");
    return;
  }

  /* ---  */

  DEBUG_ERROR("sprite anim, double free 2!");
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
