/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2025  Dirk "YouDirk" Lehmann
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


#ifndef SPRITEMANAGER_H__
#define SPRITEMANAGER_H__

#include "common.h"
#include "SpriteManager.gen.h"

/* *******************************************************************
 * The SpriteManager manages the following resources:
 *
 *   - 8 VIC-II hardware sprites
 *
 *   - allocation/free of the Sprite RAM (depends on VIC bank)
 *
 * for the usage of SpriteAnimations (and virtual sprites in the
 * future).
 */

/* Initialize this module, free all resoruces and virtual sprites.
 */
extern void __fastcall__ SpriteManager_init(void);

/* Release this module, free all resources and virtual sprites.
 */
extern void __fastcall__ SpriteManager_release(void);

/* ***************************************************************  */

/* Tick for sprite animations, etc.  */
extern void __fastcall__ SpriteManager_tick(void);

/* ***************************************************************  */

#endif /* SPRITEMANAGER_H__  */
