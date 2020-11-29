/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
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


#ifndef ENGINECONFIG_H__
#define ENGINECONFIG_H__

#include "common.h"

#include "Input.h"

/* ***************************************************************  */

/* Structure of static members for module.  */
typedef struct EngineConfig_t {

  /* Passed through Input_init() as argument.  */
  Input_devices_t inputs_enabled;

} EngineConfig_t;

/* ***************************************************************  */

/* Static members of this module.  */
extern EngineConfig_t EngineConfig;

/* Set game depeneding default configuration for modules which will be
 * passed through <module>_init(some* config) functions as argument.
 */
extern void __fastcall__ EngineConfig_init(void);

/* Deinitialize this module.  */
extern void __fastcall__ EngineConfig_release(void);

/* ***************************************************************  */

#endif /* ENGINECONFIG_H__  */
