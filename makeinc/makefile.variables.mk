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
MAPEXT := map
LABEXT := lab
LABEXT_DEBUG := debug.$(LABEXT)
PRGEXT := prg
D64EXT := d64

DEF_HEXT := def.h
DEF_DEPEXT := def.d
DEF_GENHEXT := gen.h
DEF_GENSEXT := gen.s

# --------------------------------------------------------------------

# Debug build?
ifeq (1,$(DEBUG_BUILD))
  ASDEBUGFLAGS := -g
  CCDEBUGFLAGS := $(ASDEBUGFLAGS) -T
  CCDEFINES += -DDEBUG
  OBJ += $(OBJ_DEBUG)
  ifeq (1,$(DEBUG_OPT_IRQ_RENDERTIME))
    CCDEFINES += -DDEBUG_IRQ_RENDERTIME
  endif
else
  ASDEBUGFLAGS :=
  CCDEBUGFLAGS :=
  CCDEFINES +=
endif

# --------------------------------------------------------------------

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))

DEF_GENHFILES := $(DEFH:=.$(DEF_GENHEXT))
DEF_GENSFILES := $(DEFH:=.$(DEF_GENSEXT))
DEF_GENFILES := $(DEF_GENHFILES) $(DEF_GENSFILES)
DEF_DEPFILES := $(DEFH:=.$(DEF_DEPEXT))

FLAGS := -tc64
CCFLAGS := $(CCDEBUGFLAGS) $(OPTFLAG) $(FLAGS) --standard c99 \
  $(CCWARNINGS) $(CCDEFINES) $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(ASDEBUGFLAGS) $(FLAGS) $(CCDEFINES) \
  $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := $(CCDEBUGFLAGS) $(OPTFLAG) $(FLAGS) \
  $(addprefix -L,$(LD_PATHS))

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

EMUFLAGS := -moncommands $(OUTPUT).$(LABEXT)

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=
