# C64 NonSense, just playing around with C64 cross-compile tools.
# Copyright (C) 2020  Dirk "YouDirk" Lehmann
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


# C and S files to be compiled
OBJ = Interrupt.asm Timer.asm Timer Graphix.asm Graphix Input Input.asm \
      Engine EngineConfig Pace Sandbox main

# *.gen.[hs] to be generated
DEFH = chip-vic chip-cia kernal Graphix Input

# compile and link only if DEBUG_BUILD=1 in makefile.config.mk
OBJ_DEBUG = Debug.asm Debug
DEFH_DEBUG = Debug

include ../makeinc/makefile.inc.mk
