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


OUTPUT = non-sense.prg
OBJ = main

DEBUGFLAGS = -g
OPTFLAG = -O

INCLUDE_PATHS =
LD_PATHS =
LIBS =

EBROWSEFILE = BROWSE
CTAGSFILE = tags
ETAGSFILE = TAGS
MAKEFILE = Makefile

CC = cl65
AS = cl65
LD = cl65
DEBUGGER = gdb
EBROWSE = ebrowse
CTAGS = ctags
ETAGS = etags
EMULATOR = x64

CEXT = c
HEXT = h
SEXT = S
OEXT = o
DEPEXT = d
LOGEXT = log

OBJFILES = $(OBJ:=.$(OEXT))
DEPFILES = $(OBJ:=.$(DEPEXT))

FLAGS = $(DEBUGFLAGS) $(OPTFLAG)
CCFLAGS = $(FLAGS) -c -DDEBUG $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS = $(CCFLAGS)
LDFLAGS = $(FLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS = --quiet

TAGEDFILES = $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS =
ETAGSFLAGS =
EBROWSEFLAGS =

MAKEDEP = $(CC) $(CCFLAGS) --create-full-dep

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
	rm -f *.$(DEPEXT)

.PHONY: clean-tags
clean-tags:
	rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)

.PHONY: clean
clean: clean-deps
	rm -rf *.$(OEXT) *.$(LOGEXT) *~

.PHONY: clean-all
clean-all: clean clean-tags
	rm -f $(OUTPUT)

%.$(DEPEXT): %.$(CEXT)
	$(MAKEDEP) $@ -o $*.$(OEXT) $<
%.$(DEPEXT): %.$(SEXT)
	$(MAKEDEP) $@ -o $*.$(OEXT) $<

%.$(OEXT): %.$(CEXT) $(MAKEFILE)
	$(CC) $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILE)
	$(AS) $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
	$(CTAGS) $(CTAGSFLAGS) -o $@ $^

$(ETAGSFILE): $(TAGEDFILES)
	$(ETAGS) $(ETAGSFLAGS) -o $@ $^

$(EBROWSEFILE): $(TAGEDFILES)
	$(EBROWSE) $(EBROWSEFLAGS) -o $@ $^

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

-include $(DEPFILES)
