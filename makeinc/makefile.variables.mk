# C64 NonSense, the C64 Game Engine.
# Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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

2BOOL = $(subst 0,,$(1))
2FULL = $(call 2BOOL,$(subst 1,full,$(1)))

ifneq (,$(call 2BOOL,$(CONF_DOUBLE_BUFFERING)))
  CCDEFINES += -DCONF_DOUBLE_BUFFERING
endif

# Debug build?
ifneq (,$(call 2BOOL,$(DEBUG_BUILD)))
  ASDEBUGFLAGS := -g
  CCDEBUGFLAGS := $(ASDEBUGFLAGS) -T
  OBJ += $(OBJ_DEBUG)
  DEFH += $(DEFH_DEBUG)
  CCDEFINES += -DDEBUG
  ifeq (full,$(or $(call 2FULL,$(DEBUG_OPT_OVERFLOW_CHECK)), \
                  $(call 2BOOL,$(DEBUG_BUILD))))
    CCDEFINES += -DDEBUG_OVERFLOW_CHECK
  endif
  ifeq (full,$(or $(call 2FULL,$(DEBUG_OPT_IRQ_RENDERTIME)), \
                  $(call 2BOOL,$(DEBUG_BUILD))))
    CCDEFINES += -DDEBUG_IRQ_RENDERTIME
  endif
  ifeq (full,$(or $(call 2FULL,$(DEBUG_OPT_TIMER1_SYNCCHECK)), \
                  $(call 2BOOL,$(DEBUG_BUILD))))
    CCDEFINES += -DDEBUG_TIMER1_SYNCCHECK
  endif
else
  ASDEBUGFLAGS :=
  CCDEBUGFLAGS :=
  OBJ +=
  DEFH +=
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
EMUFLAGS_DBG := -moncommands $(OUTPUT).$(LABEXT_DEBUG) -keepmonopen

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=
