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

/* ***************************************************************  */

/* Initialize empty sprite animation structure.  */
extern void __fastcall__ SpriteAnimation_new(
                    SpriteAnimation_t* animation, uint8_t frame_count,
                    Sprite_locator_t first_frame);

/* Free the sprite animation structure.  */
#define SpriteAnimation_delete(animation)
/* extern void __fastcall__ SpriteAnimation_delete(
 *                                      SpriteAnimation_t* animation);
 */

/* ***************************************************************  */

#endif /* SPRITEANIMATION_H__  */
