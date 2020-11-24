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
#include "Debug.gen.h"

#include "Timer.h"

/* ***************************************************************  */

/* Static members of this module.  */
_Debug_t _Debug;

void __fastcall__
Debug_init(void)
{
  _Debug.count = 0;
  _Debug.end = _Debug.entry;
}

void __fastcall__
Debug_release_print(void)
{
  _Debug_entry_t* cur;
  const char* type_str;

  printf("debug: %u%c message%c%s\n",
         _Debug.count, _Debug.count >= _DEBUG_LIST_SIZE? '+': '\0',
         _Debug.count == 1? '\0': 's', _Debug.count == 0? " :)": "...");

  for (cur=_Debug.entry; cur<_Debug.end; ++cur) {
    switch (cur->type) {
    case _debug_error_e: type_str = "error"; break;
    case _debug_warn_e: type_str = "warn"; break;
    case _debug_note_e: type_str = "note"; break;
    default: type_str = "<unknown>";
    }

    printf(" [%lu] %s: %s\n", cur->time, type_str, cur->msg);
  }
}

/* ***************************************************************  */

void __fastcall__
Debug_error(const char* msg)
{
  if (_Debug.count >= _DEBUG_LIST_SIZE) return;

  _Debug.end->time = Timer_1_get32();
  _Debug.end->type = _debug_error_e;
  _Debug.end->msg = msg;

  ++_Debug.end; ++_Debug.count;
}

void __fastcall__
Debug_warn(const char* msg)
{
  if (_Debug.count >= _DEBUG_LIST_SIZE) return;

  _Debug.end->time = Timer_1_get32();
  _Debug.end->type = _debug_warn_e;
  _Debug.end->msg = msg;

  ++_Debug.end; ++_Debug.count;
}
