/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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
#include "Graphix.h"
#include "Engine.h"

/* ***************************************************************  */

/* Static members of this module.  */
_Debug_t _Debug;

void __fastcall__
Debug_init(void)
{
  /* Memory not initialized here!  */

  _Debug.count = 0;
  _Debug.end = _Debug.entry;
}

void __fastcall__
Debug_release_print(void)
{
  /* Memory already released here!  */

  static _Debug_entry_t* cur;
  static const char *vic_str, *type_str;

  switch (Graphix.vic_revision) {
  /* default PAL revision  (europe)  */
  case Graphix_vicrev_pal6569_e: vic_str = "-6569"; break;
  /* Drean PAL-N revision (south america)  */
  case Graphix_vicrev_paln6572_e: vic_str = "n-6572"; break;
  /* default NTSC revision (usa)  */
  case Graphix_vicrev_ntsc6567r8_e: vic_str = "-6567r8"; break;
  /* old NTSC revision  */
  case Graphix_vicrev_ntsc6567r56a_e: vic_str = "-6567r56a"; break;
  default: vic_str = "<vic?>";
  }

  printf("sys: %s%s\n"
         "debug: %u%c message%c%s\n",
         Graphix.is_pal? "pal": "ntsc", vic_str,
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

  if (Engine.set.exit_code > 0)
    printf("\n*** exit code: %u ***\n", Engine.set.exit_code);
}

/* ***************************************************************  */
