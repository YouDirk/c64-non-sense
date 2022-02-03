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

#define ____                                      0
#define __ii                                      1
#define __DD                                      2
#define __gg                                      3
#define ii__                                      4
#define iiii                                      5
#define iiDD                                      6
#define iigg                                      7
#define DD__                                      8
#define DDii                                      9
#define DDDD                                      a
#define DDgg                                      b
#define gg__                                      c
#define ggii                                      d
#define ggDD                                      e
#define gggg                                      f

#define __(high, low)                             0x##high##low
#define _(high, low)                              __(high, low)

/* *******************************************************************
 *
 * This module just contains asset data and will be replaced by an
 * AssetLoader-module in the future.
 */

const Sprite_frame_t
AAAssets_anim_chartop_idle[AAASSETS_ANIM_CHARTOP_IDLE_COUNT] = {
  {
    {{_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(DDDD,DDDD),_(____,____)},
     {_(____,__DD),_(iiDD,DDii),_(DD__,____)},
     {_(____,__DD),_(DDDD,DDDD),_(DD__,____)},
     {_(____,__DD),_(ggDD,DDgg),_(DD__,____)},
     {_(____,____),_(DDgg,ggDD),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(iiii,iiii),_(ii__,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,ii__)},
     {_(____,iiii),_(iiii,iiii),_(iiii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,iiii)},
     {_(__ii,ii__),_(iiii,iiii),_(____,iiii)},
     {_(__ii,ii__),_(iiii,iiii),_(____,iiii)},
     {_(__ii,ii__),_(iiii,iiii),_(____,iiii)},
     {_(__ii,____),_(iiii,iiii),_(____,ii__)},
     {_(__DD,____),_(iiii,iiii),_(____,DD__)},
     {_(__DD,____),_(DDDD,DDDD),_(____,DD__)}},
    24
  },
  {
    {{_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(DDDD,DDDD),_(____,____)},
     {_(____,__DD),_(iiDD,DDii),_(DD__,____)},
     {_(____,__DD),_(DDDD,DDDD),_(DD__,____)},
     {_(____,__DD),_(ggDD,DDgg),_(DD__,____)},
     {_(____,____),_(DDgg,ggDD),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(iiii,iiii),_(____,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(__ii,ii__),_(__ii,iiii),_(iiii,ii__)},
     {_(__ii,ii__),_(__ii,iiii),_(iiii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(__ii,__ii),_(iiii,ii__),_(__ii,____)},
     {_(__DD,__ii),_(iiii,ii__),_(__DD,____)},
     {_(__DD,__DD),_(DDDD,DD__),_(__DD,____)}},
    7
  },
  {
    {{_(____,____),_(____,DDDD),_(____,____)},
     {_(____,____),_(__DD,DDDD),_(____,____)},
     {_(____,____),_(DDii,DDii),_(DD__,____)},
     {_(____,____),_(DDDD,DDDD),_(DDDD,____)},
     {_(____,____),_(DDgg,DDgg),_(DDDD,____)},
     {_(____,____),_(__DD,ggDD),_(DD__,____)},
     {_(____,____),_(____,DDDD),_(____,____)},
     {_(____,____),_(____,DDDD),_(____,____)},
     {_(____,__ii),_(iiii,iiii),_(____,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(__ii,iiii),_(iiii,iiii),_(iiii,____)},
     {_(__ii,iiii),_(iiii,iiii),_(iiii,____)},
     {_(__ii,ii__),_(__ii,iiii),_(iiii,ii__)},
     {_(__ii,ii__),_(__ii,iiii),_(iiii,ii__)},
     {_(iiii,ii__),_(iiii,iiii),_(__ii,ii__)},
     {_(iiii,____),_(iiii,iiii),_(__ii,ii__)},
     {_(iiii,____),_(iiii,iiii),_(__ii,ii__)},
     {_(iiii,____),_(iiii,iiii),_(__ii,ii__)},
     {_(ii__,__ii),_(iiii,ii__),_(__ii,____)},
     {_(DD__,__ii),_(iiii,ii__),_(__DD,____)},
     {_(DD__,__DD),_(DDDD,DD__),_(__DD,____)}},
    24
  },
  {
    {{_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(__DD,DDDD),_(____,____)},
     {_(____,____),_(DDii,DDii),_(DD__,____)},
     {_(____,__DD),_(DDDD,DDDD),_(DD__,____)},
     {_(____,__DD),_(DDgg,DDgg),_(DD__,____)},
     {_(____,____),_(DDDD,ggDD),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,____),_(__DD,DD__),_(____,____)},
     {_(____,__ii),_(iiii,iiii),_(ii__,____)},
     {_(____,iiii),_(iiii,iiii),_(iiii,____)},
     {_(__ii,iiii),_(iiii,iiii),_(iiii,ii__)},
     {_(__ii,iiii),_(iiii,iiii),_(iiii,ii__)},
     {_(__ii,ii__),_(iiii,ii__),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,ii__),_(__ii,ii__)},
     {_(__ii,ii__),_(iiii,ii__),_(__ii,iiii)},
     {_(iiii,____),_(iiii,ii__),_(____,iiii)},
     {_(iiii,__ii),_(iiii,ii__),_(____,iiii)},
     {_(iiii,__ii),_(iiii,ii__),_(____,iiii)},
     {_(__ii,__ii),_(iiii,ii__),_(____,__ii)},
     {_(__DD,__ii),_(iiii,ii__),_(____,__DD)},
     {_(__DD,__DD),_(DDDD,DD__),_(____,__DD)}},
    7
  },
};

/* ***************************************************************  */

const Sprite_frame_t
AAAssets_anim_charbot_idle[AAASSETS_ANIM_CHARBOT_IDLE_COUNT] = {
  {
    {{_(__gg,____),_(DDDD,DDDD),_(____,gg__)},
     {_(____,__gg),_(gggg,gggg),_(gg__,____)},
     {_(____,__gg),_(gggg,gggg),_(gg__,____)},
     {_(____,__gg),_(gggg,gggg),_(gg__,____)},
     {_(____,gggg),_(gggg,gggg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,gg__)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,DDDD),_(____,____),_(DDDD,____)},
     {_(____,DDDD),_(____,____),_(DDDD,DD__)},
     {_(____,iiii),_(____,____),_(iiii,ii__)}},
    24
  },
  {
    {{_(__gg,__DD),_(DDDD,DD__),_(__gg,____)},
     {_(____,gggg),_(gggg,gggg),_(gg__,____)},
     {_(____,gggg),_(gggg,gggg),_(gg__,____)},
     {_(____,gggg),_(gggg,gggg),_(gg__,____)},
     {_(____,gggg),_(gggg,gggg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(__gg,gggg),_(gg__,__gg),_(gggg,gg__)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,DDDD),_(____,____),_(DDDD,____)},
     {_(____,DDDD),_(____,____),_(DDDD,DD__)},
     {_(____,iiii),_(____,____),_(iiii,ii__)}},
    7
  },
  {
    {{_(gg__,__DD),_(DDDD,DD__),_(__gg,____)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(gg__,____)},
     {_(____,gggg),_(gg__,gggg),_(gg__,____)},
     {_(____,gggg),_(gg__,gggg),_(gg__,____)},
     {_(____,gggg),_(gg__,gggg),_(gg__,____)},
     {_(____,gggg),_(gg__,gggg),_(gggg,____)},
     {_(__gg,gggg),_(gg__,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(____,gggg),_(gg__,____),_(gggg,gg__)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,DDDD),_(____,____),_(DDDD,____)},
     {_(__DD,DDDD),_(____,____),_(DDDD,____)},
     {_(__ii,iiii),_(____,____),_(iiii,____)}},
    24
  },
  {
    {{_(__gg,__DD),_(DDDD,DD__),_(____,__gg)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(____,____)},
     {_(____,gggg),_(gggg,gggg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(____,gggg),_(gg__,__gg),_(gggg,____)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(__gg,gggg),_(____,____),_(gggg,gg__)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,gggg),_(____,____),_(gggg,____)},
     {_(____,DDDD),_(____,____),_(DDDD,____)},
     {_(____,DDDD),_(____,____),_(DDDD,____)},
     {_(____,iiii),_(____,____),_(iiii,____)}},
    7
  },
};

/* ***************************************************************  */

const Sprite_frame_t
AAAssets_anim_chartop_runright[AAASSETS_ANIM_CHARTOP_RUNRIGHT_COUNT] = {
  {
    {{_(____,____),_(____,____),_(DDDD,____)},
     {_(____,____),_(____,__DD),_(DDDD,DD__)},
     {_(____,____),_(____,DDDD),_(DDDD,iiDD)},
     {_(____,____),_(____,DDDD),_(DDDD,DDDD)},
     {_(____,____),_(____,DDDD),_(ggDD,DDDD)},
     {_(____,____),_(____,__DD),_(DDgg,gg__)},
     {_(____,____),_(____,____),_(DDDD,____)},
     {_(____,____),_(____,____),_(DDDD,____)},
     {_(____,____),_(iiii,iiii),_(ii__,____)},
     {_(____,____),_(iiii,iiii),_(ii__,____)},
     {_(____,__ii),_(iiii,iiii),_(iiii,____)},
     {_(____,__ii),_(ii__,iiii),_(iiii,____)},
     {_(____,__ii),_(ii__,iiii),_(iiii,____)},
     {_(____,__ii),_(iiii,iiii),_(iiii,____)},
     {_(____,__ii),_(iiii,iiii),_(ii__,____)},
     {_(____,____),_(iiii,iiii),_(ii__,____)},
     {_(____,____),_(iiii,iiii),_(____,____)},
     {_(____,____),_(iiii,iiii),_(ii__,____)},
     {_(____,____),_(iiii,iiii),_(iiii,____)},
     {_(____,____),_(iiii,iiii),_(iiii,iiDD)},
     {_(____,____),_(DDDD,DDDD),_(__ii,iiDD)}},
    29
  },
};

/* ***************************************************************  */

const Sprite_frame_t
AAAssets_anim_charbot_runright[AAASSETS_ANIM_CHARBOT_RUNRIGHT_COUNT] = {
  {
    {{_(____,____),_(DDDD,DDDD),_(____,____)},
     {_(____,____),_(gggg,gggg),_(____,____)},
     {_(____,____),_(gggg,gggg),_(____,____)},
     {_(____,__gg),_(gggg,gggg),_(gg__,____)},
     {_(____,__gg),_(gggg,gggg),_(gggg,____)},
     {_(____,__gg),_(gg__,gggg),_(gggg,____)},
     {_(____,__gg),_(gg__,__gg),_(gggg,gg__)},
     {_(____,__gg),_(gggg,__gg),_(gggg,gg__)},
     {_(__ii,ii__),_(gggg,____),_(gggg,gg__)},
     {_(iiDD,DD__),_(gggg,gg__),_(gggg,gggg)},
     {_(DDDD,DD__),_(gggg,gg__),_(__gg,gggg)},
     {_(__gg,gggg),_(__gg,gggg),_(gggg,gg__)},
     {_(__gg,gggg),_(gggg,gggg),_(gggg,____)},
     {_(____,gggg),_(gggg,__gg),_(gggg,____)},
     {_(____,____),_(____,__gg),_(gg__,____)},
     {_(____,____),_(____,gggg),_(gg__,____)},
     {_(____,____),_(____,gggg),_(____,____)},
     {_(____,____),_(____,DDDD),_(____,____)},
     {_(____,____),_(____,DDDD),_(____,____)},
     {_(____,____),_(____,iiii),_(DD__,____)},
     {_(____,____),_(____,____),_(ii__,____)}},
    29
  },
};

/* ***************************************************************  */
