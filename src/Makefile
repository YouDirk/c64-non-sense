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


include ../makefile.config.mk

OUTPUT := non-sense.prg
OBJ := main

INCLUDE_PATHS :=
LD_PATHS :=
LIBS :=

EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
MAKEFILES := Makefile ../Makefile ../makefile.config.mk

CC := cl65
AS := cl65
LD := cl65
DEBUGGER := gdb
EBROWSE := ebrowse
CTAGS := ctags
ETAGS := etags
EMULATOR := x64

CEXT := c
HEXT := h
SEXT := S
OEXT := o
DEPEXT := d
LOGEXT := log

# --------------------------------------------------------------------

# Debug build?
ifeq (1,$(DEBUG_BUILD))
  DEBUGFLAGS := -g
  CCDEFINES += -DDEBUG
else
  DEBUGFLAGS :=
  CCDEFINES +=
endif

# --------------------------------------------------------------------

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))

FLAGS := $(DEBUGFLAGS) $(OPTFLAG)
CCFLAGS := $(FLAGS) --standard $(CCSTDFLAG) $(CCDEFINES) \
           $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(CCFLAGS)
LDFLAGS := $(FLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS := --quiet

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=

MAKEDEP := $(CC) $(CCFLAGS) --create-full-dep

.PHONY: all
all: $(OUTPUT)

.PHONY: recompile
recompile: clean all

.PHONY: run
run: all
	$(EMULATOR) -autostart $(OUTPUT)

.PHONY: debug
debug: all
	$(EMULATOR) -autostart $(OUTPUT)

.PHONY: ctags
ctags: $(CTAGSFILE)

.PHONY: etags
etags: $(ETAGSFILE)

.PHONY: ebrowse
ebrowse: $(EBROWSEFILE)

.PHONY: all-tags
all-tags: ctags etags ebrowse

.PHONY: clean-deps
clean-deps:
	-rm -f *.$(DEPEXT)

.PHONY: clean-tags
clean-tags:
	-rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)

.PHONY: clean
clean: clean-deps
	-rm -rf *.$(OEXT) *.$(LOGEXT) *~ *.bak

.PHONY: clean-all
clean-all: clean clean-tags
	-rm -f $(OUTPUT)

%.$(DEPEXT): %.$(CEXT)
	@$(MAKEDEP) $@ -E -o /dev/null $<
%.$(DEPEXT): %.$(SEXT)
	@$(MAKEDEP) $@ -E -o $*.$(OEXT) $<

%.$(OEXT): %.$(CEXT) $(MAKEFILES)
	$(CC) -c $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILES)
	$(AS) -c $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
	$(CTAGS) $(CTAGSFLAGS) -o $@ $^

$(ETAGSFILE): $(TAGEDFILES)
	$(ETAGS) $(ETAGSFLAGS) -o $@ $^

$(EBROWSEFILE): $(TAGEDFILES)
	$(EBROWSE) $(EBROWSEFLAGS) -o $@ $^

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

-include $(DEPFILES)
