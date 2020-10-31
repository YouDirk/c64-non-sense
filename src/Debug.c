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


#include "Debug.h"

#include "Timer.h"

#define _LIST_SIZE                 15

/* ***************************************************************  */

#define _error_e                   ((_msgtype_t) 1)
#define _warn_e                    ((_msgtype_t) 2)
#define _note_e                    ((_msgtype_t) 3)
typedef uint8_t _msgtype_t;

typedef struct Debug_entry_t {
  uint32_t time;
  _msgtype_t type;
  const char* msg;
} Debug_entry_t;

/* Structure of static members for module.  */
typedef struct Debug_t {
  uint8_t count;
  Debug_entry_t entry[_LIST_SIZE];
} Debug_t;

/* ***************************************************************  */

/* Static members of this module.  */
static Debug_t Debug;

void __fastcall__
Debug_init(void)
{
  Debug.count = 0;
}

void __fastcall__
Debug_release_print(void)
{
  uint8_t i; const char* type_str;

  printf("debug: %u%c message%c%s\n",
         Debug.count, Debug.count == _LIST_SIZE? '+': '\0',
         Debug.count == 1? '\0': 's', Debug.count == 0? " :)": "...");

  for (i=0; i<Debug.count; ++i) {
    switch (Debug.entry[i].type) {
    case _error_e: type_str = "error"; break;
    case _warn_e: type_str = "warn"; break;
    case _note_e: type_str = "note"; break;
    default: type_str = "<unknown>"; break;
    }

    printf(" [%lu] %s: %s\n",
           Debug.entry[i].time, type_str, Debug.entry[i].msg);
  }
}

void __fastcall__
Debug_error(const char* msg)
{
  if (Debug.count == _LIST_SIZE) return;

  Debug.entry[Debug.count].time = Timer_1_get32();
  Debug.entry[Debug.count].type = _error_e;
  Debug.entry[Debug.count].msg = msg;

  ++Debug.count;
}

void __fastcall__
Debug_warn(const char* msg)
{
  if (Debug.count == _LIST_SIZE) return;

  Debug.entry[Debug.count].time = Timer_1_get32();
  Debug.entry[Debug.count].type = _warn_e;
  Debug.entry[Debug.count].msg = msg;

  ++Debug.count;
}

void __fastcall__
Debug_note(const char* msg)
{
  if (Debug.count == _LIST_SIZE) return;

  Debug.entry[Debug.count].time = Timer_1_get32();
  Debug.entry[Debug.count].type = _note_e;
  Debug.entry[Debug.count].msg = msg;

  ++Debug.count;
}
