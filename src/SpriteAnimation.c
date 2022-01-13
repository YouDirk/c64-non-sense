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
 * Cases for SPRITEANIMATION_DELETE()
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

#define _DEBUG_CASES               1

#if _DEBUG_CASES == 1
#  define _DEBUG_CASE(no)          DEBUG_NOTE("sprite del, case " no)
#else
#  define _DEBUG_CASE(no)
#endif /* _DEBUG_CASES  */

/* The code was optimized to reach a CASE (see comment above) with as
 * small as possible conditions to test.  Therefore, some conditions
 * are redundant in code, but in a condition-path of one call from
 * begin to RETURN every condition is tested once, AND NO UNNECESSARY
 * conditions should be tested.
 *
 * For example: The test if an allocation is before (outside) of
 * Sprite RAM will be tested at the end of the condition-path, because
 * in just 2 CASES (case 1 and 2) it is necessary to test and
 * therefore in the most cases it would be a wasted condtition test.
 * But on the other hand, the check, if the allocation is before the
 * Sprite RAM, is implemented twice, in CASE 1 and for CASE 2.
 */
void __fastcall__
SpriteAnimation_delete(const SpriteAnimation_t* animation)
{
  static _freelist_block_t *cur_block, *prev_block;
  static uint8_t *alloc, *alloc_end, *prev_block_end;
  static uint16_t alloc_bufsize, prev_block_bufsize;

  static uint8_t alloc_framecount;

  /* reduce repeated dereferencing and calculations  */
  alloc = (uint8_t*) animation->buffer;
  alloc_framecount = animation->frame_count;

  alloc_bufsize = alloc_framecount  << 6;
  alloc_end = (uint8_t*) alloc + alloc_bufsize;

  /* first check some default bad arguments  */
#ifdef DEBUG_ASSERTION_CHECK
  if (alloc_framecount == 0) {
    DEBUG_ERROR("sprite anim, free zero size!");
    return;
  }
  /* ALLOC == NULL check not required.  It will result in an 'OUTSIDE
   * RAM (BEFORE)' error.
   */
#endif /* DEBUG_ASSERTION_CHECK  */

  /* Sprite RAM full?  We add a first block here and therefore
   * _SPRITEANIMATION_FREELIST will never be NULL after that
   * condition.
   */
  if (_SpriteAnimation_freelist == NULL) {
#ifdef DEBUG_ASSERTION_CHECK
    if (alloc < (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST)
        || alloc_end
           > (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)) {
      DEBUG_ERROR("sprite anim, free outside ram (one)!");
      return;
    }
#endif /* DEBUG_ASSERTION_CHECK  */

    _SpriteAnimation_freelist = (_freelist_block_t*) alloc;
    _SpriteAnimation_freelist->next = NULL;
    _SpriteAnimation_freelist->size = alloc_framecount;

    _DEBUG_CASE("1+8");
    return;
  } /* if (_SpriteAnimation_freelist == NULL)  */

  /* -----------------------------------------------------------------
   * Open CASES       : 1 2 3 4 5 6 7 8
   * Boundaries tested: -
   */

  /* Loop through all block in the freelist, until ALLOC < CUR_BLOCK.
   *
   * On first iteration the following condition is TRUE:
   *   CUR_BLOCK == PREV_BLOCK == _SPRITEANIMATION_FREELIST != NULL
   */
  prev_block = _SpriteAnimation_freelist;
  for (cur_block=_SpriteAnimation_freelist; cur_block != NULL;
       cur_block=cur_block->next) {

    /* iterate, until ALLOC < CUR_BLOCK (upper boundary for ALLOC)  */
    if (alloc >= (uint8_t*) cur_block) {
      prev_block = cur_block;
      continue;
    }

    /* reduce repeated dereferencing and calculations  */
    prev_block_bufsize = prev_block->size << 6;
    prev_block_end = (uint8_t*) prev_block + prev_block_bufsize;

    /* ---  */

    /* Here the condition path looks like this:
     *   ALLOC < CUR_BLOCK
     *
     * Open CASES       : 1 2 3 4 5 6 7 8
     * Boundaries tested: ALLOC < CUR_BLOCK
     *
     * CUR_BLOCK will never be NULL here, therefore not case 7 or 8
     * possible here.
     *
     * Test if the end of previous block is attaching ALLOC.  CASE 3
     * or 6.
     *
     * On first iteration:
     *   ALLOC < (CUR_BLOCK == PREV_BLOCK) --> ALLOC < PREV_BLOCK_END,
     *
     * therefore this condtion (ALLOC == PREV_BLOCK_END) is FALSE (not
     * case 1 or 2).
     */
    if (alloc == prev_block_end) {
      /* possible CASES: 3 6  */

      /* Here the condition path looks like this:
       *   ALLOC < CUR_BLOCK && ALLOC == PREV_BLOCK_END
       *   && !first_iter
       *
       * Open CASES       : 3 6
       * Boundaries tested: PREV_BLOCK_END = ALLOC < CUR_BLOCK
       *
       * The lower boundary is attched to the PREV_BLOCK.  If the
       * upper boundary is lower than CUR_BLOCK then it´s CASE 3.
       *
       * As result we enlarge PREV_BLOCK by appending the size of
       * ALLOC.
       */
      if (alloc_end < (uint8_t*) cur_block) {
        prev_block->size += alloc_framecount;

        _DEBUG_CASE("3");
        return;
      }

      /* Here the condition path looks like this:
       *   ALLOC < CUR_BLOCK && ALLOC == PREV_BLOCK_END
       *   && ALLOC_END >= CUR_BLOCK && !first_iter
       *
       * Open CASES       : 6
       * Boundaries tested: PREV_BLOCK_END = ALLOC
       *                                  && ALLOC_END >= CUR_BLOCK
       *
       * The lower boundary is attched to the PREV_BLOCK.  If the
       * upper boundary equals CUR_BLOCK it´s also attached to it.
       * Then this is CASE 6.
       *
       * As result we are merging PREV_BLOCK and ALLOC and CUR_BLOCK
       * together.
       */
      if (alloc_end == (uint8_t*) cur_block) {
        prev_block->next = cur_block->next;
        prev_block->size += alloc_framecount + cur_block->size;

        _DEBUG_CASE("6");
        return;
      }

      /* Here the condition path looks like this:
       *   ALLOC < CUR_BLOCK && ALLOC == PREV_BLOCK_END
       *   && ALLOC_END > CUR_BLOCK && !first_iter
       *
       * Open CASES       :
       * Boundaries tested: PREV_BLOCK_END = ALLOC
       *                                  && ALLOC_END > CUR_BLOCK
       *
       * The upper boundary overlaps to CUR_BLOCK.
       */
      DEBUG_ERROR("sprite anim, free overlap (after)!");
      return;
    }

    /* ---  */

    /* Here the condition path looks like this:
     *   ALLOC < CUR_BLOCK
     *   && ALLOC != PREV_BLOCK_END
     *
     * Open CASES       : 1 2 4 5 7 8
     * Boundaries tested: ALLOC < CUR_BLOCK
     *
     * CUR_BLOCK will never be NULL here, therefore not case 7 or 8
     * possible here.
     *
     * Test if the end of ALLOC is attaching the current block.  CASE
     * 5 (case 6 tested inside a condtion before).
     *
     * On first iteration:
     *   CUR_BLOCK == _SPRITEANIMATION_FREELIST != NULL
     *
     * therefore this condtion is working for CASE 2 as well.
     */
    if (alloc_end == (uint8_t*) cur_block) {
      /* possible CASES: 2 5  */

      /* Here the condition path looks like this:
       *   ALLOC < CUR_BLOCK && ALLOC_END == CUR_BLOCK
       *   && ALLOC != PREV_BLOCK_END
       *
       * Open CASES       : 2 5
       * Boundaries tested: ALLOC < CUR_BLOCK && ALLOC_END == CUR_BLOCK
       *
       * If not first iteration then its CASE 5.  Additionally an
       * lower boundary test is needed.
       *
       * As result we enlarge CUR_BLOCK by prepending the size of
       * ALLOC.  PREV_BLOCK->NEXT needed to be updated.
       */
      if (cur_block != _SpriteAnimation_freelist) {

        /* After that the boundaries are:
         *   PREV_BLOCK_END < ALLOC && ALLOC_END == CUR_BLOCK
         */
#ifdef DEBUG_ASSERTION_CHECK
        if (alloc <= prev_block_end) {
          DEBUG_ERROR("sprite anim, free overlap (before)!");
          return;
        }
#endif /* DEBUG_ASSERTION_CHECK  */

        ((_freelist_block_t*) alloc)->next = cur_block->next;
        ((_freelist_block_t*) alloc)->size = cur_block->size
                                                   + alloc_framecount;
        prev_block->next = (_freelist_block_t*) alloc;

        _DEBUG_CASE("5");
        return;
      }

      /* Here the condition path looks like this:
       *   ALLOC < CUR_BLOCK && ALLOC_END == CUR_BLOCK && first_iter
       *   && ALLOC != PREV_BLOCK_END
       *
       * Open CASES       : 2
       * Boundaries tested: ALLOC < CUR_BLOCK && ALLOC_END == CUR_BLOCK
       *
       * Just one CASE possible and we know that wer are in the first
       * iteration.  Just an lower boundary test is needed.
       *
       * As result we enlarge CUR_BLOCK by prepending the size of
       * ALLOC.  Because we are in first iteration, we need to update
       * _SPRITEANIMATION_FREELIST.
       */

      /* After that the boundaries are:
       *   SpriteRAM_begin <= ALLOC && ALLOC_END == CUR_BLOCK
       */
#ifdef DEBUG_ASSERTION_CHECK
      if (alloc
          < (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST)) {
        DEBUG_ERROR("sprite anim, free outside ram (before)!");
        return;
      }
#endif /* DEBUG_ASSERTION_CHECK  */

      ((_freelist_block_t*) alloc)->next = cur_block->next;
      ((_freelist_block_t*) alloc)->size = cur_block->size
                                                   + alloc_framecount;
      _SpriteAnimation_freelist = (_freelist_block_t*) alloc;

      _DEBUG_CASE("2");
      return;
    }

    /* ---  */

    /* After that the boundaries are:
     *   PREV_BLOCK_END != ALLOC && ALLOC_END < CUR_BLOCK
     */
#ifdef DEBUG_ASSERTION_CHECK
    if (alloc_end >= (uint8_t*) cur_block) {
      DEBUG_ERROR("sprite anim, free overlap (after)!!");
      return;
    }
#endif /* DEBUG_ASSERTION_CHECK  */


    /* Here the condition path looks like this:
     *   ALLOC < CUR_BLOCK && ALLOC_END < CUR_BLOCK
     *   && ALLOC != PREV_BLOCK_END && ALLOC_END != CUR_BLOCK
     *
     * Open CASES       : 1 4 7 8
     * Boundaries tested: PREV_BLOCK_END != ALLOC
     *                                   && ALLOC_END < CUR_BLOCK
     *
     * CUR_BLOCK will never be NULL here, therefore not case 7 or 8
     * possible here.
     *
     * In words, we tested that ALLOC < CUR_BLOCK and that ALLOC will
     * neither attach before the current block nor after previous
     * block.
     *
     * If ALLOC is located after the previous block (not ovlapping)
     * then it´s CASE 4.
     *
     * On first iteration:
     *   ALLOC < (CUR_BLOCK == PREV_BLOCK) --> ALLOC < PREV_BLOCK_END,
     *
     * therefore this condtion (ALLOC > PREV_BLOCK_END) is FALSE (not
     * case 1).
     */
    if (alloc > prev_block_end) {
      ((_freelist_block_t*) alloc)->next = cur_block;
      ((_freelist_block_t*) alloc)->size = alloc_framecount;
      prev_block->next = (_freelist_block_t*) alloc;

      _DEBUG_CASE("4");
      return;
    }

    /* ---  */

    /* Here the condition path looks like this:
     *   ALLOC < CUR_BLOCK && ALLOC <= PREV_BLOCK_END
     *   && ALLOC_END < CUR_BLOCK && ALLOC != PREV_BLOCK_END
     *   && ALLOC_END != CUR_BLOCK
     *
     * Open CASES       : 1 7 8
     * Boundaries tested: PREV_BLOCK_END (not first iteration)
     *                                   < ALLOC && ALLOC_END < CUR_BLOCK
     *
     * CUR_BLOCK will never be NULL here, therefore not case 7 or 8
     * possible here.
     *
     * Now just DOUBLE FREEs (boundaries are tested) or CASE 1 are
     * possible.  If we make sure, that this is the first iteration
     *   CUR_BLOCK == _SPRITEANIMATION_FREELIST
     *
     * and that the lacking lower boundary is inside the Sprite RAM, then
     * it´s CASE 1.
     */
    if (cur_block == _SpriteAnimation_freelist) {

      /* After that the boundaries are:
       *   SpriteRAM_begin <= ALLOC && ALLOC_END < CUR_BLOCK
       */
#ifdef DEBUG_ASSERTION_CHECK
      if (alloc
          < (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_FIRST)) {
        DEBUG_ERROR("sprite anim, free outside ram (before)!!");
        return;
      }
#endif /* DEBUG_ASSERTION_CHECK  */


      ((_freelist_block_t*) alloc)->next = cur_block;
      ((_freelist_block_t*) alloc)->size = alloc_framecount;
      _SpriteAnimation_freelist = (_freelist_block_t*) alloc;

      _DEBUG_CASE("1");
      return;
    }

    /* ---  */

    /* Here the condition path looks like this:
     *   ALLOC < CUR_BLOCK && ALLOC <= PREV_BLOCK_END
     *   && ALLOC_END < CUR_BLOCK && ALLOC != PREV_BLOCK_END
     *   && ALLOC_END != CUR_BLOCK && !first_iter
     *
     * Open CASES       : 7 8
     * Boundaries tested: PREV_BLOCK_END < ALLOC && ALLOC_END < CUR_BLOCK
     *
     * CUR_BLOCK will never be NULL here, therefore not case 7 or 8
     * possible here.
     *
     * Now just DOUBLE FREEs (boundaries are tested) are possible.
     */
    DEBUG_ERROR("sprite anim, double free!");
    return;
  } /* for (cur_block=; ...; cur_block=cur_block->next)  */

  /* -----------------------------------------------------------------
   * Open CASES       : 7 8
   * Boundaries tested: ALLOC >= PREV_BLOCK (loop condition)
   */

#ifdef DEBUG_ASSERTION_CHECK
  if (alloc_end
      > (uint8_t*) SPRITE_LOCATOR_DEREF(SPRITE_LOCATOR_END)) {
    DEBUG_ERROR("sprite anim, free outside ram (after)!");
    return;
  }
#endif /* DEBUG_ASSERTION_CHECK  */

  /* reduce repeated dereferencing and calculations  */
  prev_block_bufsize = prev_block->size << 6;
  prev_block_end = (uint8_t*) prev_block + prev_block_bufsize;

  /* ---  */

  /* Here the condition path looks like this:
   *   ALLOC >= PREV_BLOCK
   *   && ALLOC_END <= SpriteRAM_end
   *
   * Open CASES       : 7 8
   * Boundaries tested: PREV_BLOCK <= ALLOC && ALLOC_END <= RAM_end
   *
   * We know that PREV_BLOCK will never be NULL, also if the FOR loop
   * was never iterated.  And PREV_BLOCK->NEXT is pointing to NULL,
   * therefore PREV_BLOCK is the last block in the list.
   *
   * Now we test, if ALLOC can be attched after the last block in
   * list, which is CASE 7.  As result we just enlarge the last block.
   */
  if (alloc == prev_block_end) {
    prev_block->size += alloc_framecount;

    _DEBUG_CASE("7");
    return;
  }

  /* ---  */

  /* Here the condition path looks like this:
   *   ALLOC >= PREV_BLOCK && ALLOC != PREV_BLOCK_END
   *   && ALLOC_END <= SpriteRAM_end
   *
   * Open CASES       : 8
   * Boundaries tested: PREV_BLOCK <= ALLOC && ALLOC_END <= RAM_end
   *
   * Just still one case.  To make sure that ALLOC match to that CASE
   * 8, we need to test for the lower boundary, which need to be
   * greater than the end of the previous block.
   *
   * As result we create a new block, pointing from last block in list.
   */
  if (alloc > prev_block_end) {
    ((_freelist_block_t*) alloc)->next = NULL;
    ((_freelist_block_t*) alloc)->size = alloc_framecount;
    prev_block->next = (_freelist_block_t*) alloc;

    _DEBUG_CASE("8");
    return;
  }

  /* ---  */

  /* Here the condition path looks like this:
   *   ALLOC >= PREV_BLOCK && ALLOC > PREV_BLOCK_END
   *   && ALLOC_END <= SpriteRAM_end
   *
   * Open CASES       : -
   * Boundaries tested: PREV_BLOCK_END < ALLOC && ALLOC_END <= RAM_end
   *
   * Seems that there are just DOUBLE FREEs left.
   */
  DEBUG_ERROR("sprite anim, double free!!");
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
