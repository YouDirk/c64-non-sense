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


OUTPUT := non-sense

INCLUDE_PATHS :=
LD_PATHS :=
LIBS :=

EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
MAKEFILEZ := Makefile ../Makefile ../makefile.config.mk \
  ../makeinc/makefile.variables.mk ../makeinc/makefile.inc.mk \
  ../makeinc/makefile.check.mk

CEXT := c
HEXT := h
SEXT := S
CCEXT := cc
ASMEXT := asm
OEXT := o
DEPEXT := d
LOGEXT := log
PRGEXT := prg
D64EXT := d64

# --------------------------------------------------------------------

# Debug build?
ifeq (1,$(DEBUG_BUILD))
  DEBUGFLAGS := -g -T
  CCDEFINES += -DDEBUG
  ifeq (1,$(DEBUG_OPT_IRQ_RENDERTIME))
    CCDEFINES += -DDEBUG_IRQ_RENDERTIME
  endif
else
  DEBUGFLAGS :=
  CCDEFINES +=
endif

# --------------------------------------------------------------------

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))

FLAGS := $(DEBUGFLAGS) $(OPTFLAG) -tc64
CCFLAGS := $(FLAGS) --standard $(CCSTDFLAG) -W-unused-param \
    $(CCDEFINES) $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(FLAGS) --standard $(CCSTDFLAG) -W-unused-param \
    $(addprefix -Wa ,$(CCDEFINES)) $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := $(FLAGS) $(addprefix -L,$(LD_PATHS))

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=

MAKEDEP := $(CC) $(CCFLAGS) --create-full-dep
