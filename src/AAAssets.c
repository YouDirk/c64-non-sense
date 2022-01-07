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


#include "AAAssets.h"

/* ***************************************************************  */

#define ____                                      0
#define ___B                                      1
#define __B_                                      2
#define __BB                                      3
#define _B__                                      4
#define _B_B                                      5
#define _BB_                                      6
#define _BBB                                      7
#define B___                                      8
#define B__B                                      9
#define B_B_                                      a
#define B_BB                                      b
#define BB__                                      c
#define BB_B                                      d
#define BBB_                                      e
#define BBBB                                      f

#define __(high, low)                             0x##high##low
#define _(high, low)                              __(high, low)

/* ***************************************************************  */

const Sprite_frame_t
AAAssets_spranim_moving[AAASSETS_SPRANIM_MOVING_COUNT] = {
  {
    {{_(____,____),_(____,____),_(____,____)},
     {_(____,BBBB),_(BB__,___B),_(BBBB,B___)},
     {_(___B,BBBB),_(BBBB,_BBB),_(BBBB,BB__)},
     {_(__BB,BBBB),_(BBBB,_BBB),_(BBBB,BBB_)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(__BB,BBBB),_(BBBB,BBBB),_(BBBB,BBB_)},
     {_(__BB,BBBB),_(BBBB,BBBB),_(BBBB,BBB_)},
     {_(____,BBBB),_(BBBB,BBBB),_(BBBB,B___)},
     {_(____,_BBB),_(BBBB,BBBB),_(BBBB,____)},
     {_(____,___B),_(BBBB,BBBB),_(BB__,____)},
     {_(____,____),_(_BBB,BBBB),_(____,____)},
     {_(____,____),_(__BB,BBB_),_(____,____)},
     {_(____,____),_(__BB,BBB_),_(____,____)},
     {_(____,____),_(___B,BB__),_(____,____)},
     {_(____,____),_(___B,BB__),_(____,____)},
     {_(____,____),_(____,B___),_(____,____)}},
    50
  },
  {
    {{_(____,____),_(____,____),_(____,____)},
     {_(____,BBBB),_(BB__,___B),_(BBBB,B___)},
     {_(___B,BBBB),_(BBBB,_BBB),_(BBBB,BB__)},
     {_(__BB,BBBB),_(BBBB,_BBB),_(BBBB,BBB_)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BBBB,BBBB),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BB__,___B),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BB__,___B),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BB__,___B),_(BBBB,BBBB)},
     {_(_BBB,BBBB),_(BB__,___B),_(BBBB,BBBB)},
     {_(__BB,BBBB),_(BB__,___B),_(BBBB,BBB_)},
     {_(__BB,BBBB),_(BB__,___B),_(BBBB,BBB_)},
     {_(____,BBBB),_(BB__,___B),_(BBBB,B___)},
     {_(____,_BBB),_(BB__,___B),_(BBBB,____)},
     {_(____,___B),_(BBBB,BBBB),_(BB__,____)},
     {_(____,____),_(_BBB,BBBB),_(____,____)},
     {_(____,____),_(__BB,BBB_),_(____,____)},
     {_(____,____),_(__BB,BBB_),_(____,____)},
     {_(____,____),_(___B,BB__),_(____,____)},
     {_(____,____),_(___B,BB__),_(____,____)},
     {_(____,____),_(____,B___),_(____,____)}},
    50 | SPRITE_FAME_TICKCOUNT_LAST_MASK
  }
};

/* ***************************************************************  */
