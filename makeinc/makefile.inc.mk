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

include ../.makefile.cache.mk
include ../makeinc/makefile.check.mk

include ../makeinc/makefile.variables.mk


# ********************************************************************
# Targets

.PHONY: all
all: $(OUTPUT).$(D64EXT)

.PHONY: recompile
recompile: clean all

.PHONY: run run-load run-mount8
run: all
	$(EMULATOR) -autostart $(OUTPUT).$(D64EXT)
run-load: all
	$(EMULATOR) -autoload $(OUTPUT).$(D64EXT)
run-mount8: all
	$(EMULATOR) -8 $(OUTPUT).$(D64EXT)

.PHONY: debug
debug: all
	$(EMULATOR) -autostart $(OUTPUT).$(D64EXT)

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
	-rm -rf *.$(OEXT) *.$(LOGEXT) *.$(PRGEXT) *~ *.bak

# _CLEAN_MAKECACHE must be the last one in the dependency list,
# because it will be regenerated during recursive CLEAN calls
.PHONY: _clean_makecache
_clean-makecache:
	-rm -f ../.makefile.cache.mk
.PHONY: clean-all
clean-all: clean clean-tags _clean-makecache
	-rm -f $(OUTPUT).$(D64EXT)

%.$(DEPEXT): %.$(CEXT)
	@$(MAKEDEP) $@ -E -o /dev/null $<
%.$(DEPEXT): %.$(SEXT)
	@$(MAKEDEP) $@ -E -o $*.$(OEXT) $<

%.$(OEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) -c $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILEZ)
	$(AS) -c $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
ifeq (,$(CTAGS_OPT))
	$(error $(ERRB) CTAGS command not found!  Try Debian '$$> \
          apt-get install emacs-bin-common' for installation.  After \
          install run '$$> make clean-all')
else
	$(CTAGS_OPT) $(CTAGSFLAGS) -o $@ $^
endif
$(ETAGSFILE): $(TAGEDFILES)
ifeq (,$(ETAGS_OPT))
	$(error $(ERRB) ETAGS command not found!  Try Debian '$$> \
          apt-get install emacs-bin-common' for installation.  After \
          install run '$$> make clean-all')
else
	$(ETAGS_OPT) $(ETAGSFLAGS) -o $@ $^
endif
$(EBROWSEFILE): $(TAGEDFILES)
ifeq (,$(EBROWSE_OPT))
	$(error $(ERRB) EBROWSE command not found!  Try Debian '$$> \
          apt-get install emacs-bin-common' for installation.  After \
          install run '$$> make clean-all')
else
	$(EBROWSE_OPT) $(EBROWSEFLAGS) -o $@ $^
endif

$(OUTPUT).$(PRGEXT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

$(OUTPUT).$(D64EXT): $(OUTPUT).$(PRGEXT)
	$(D64PACK) -format $*,xy d64 $@ -attach $@ -write $< $*

.PHONY: _cache
_cache:
../.makefile.cache.mk: $(MAKEFILEZ)
	echo '' > $@
	$(MAKE) _CACHE_FILE=$@ _cache

# Make sure that $(CC) was set by makefile.check.mk && _CACHE_FILE
# will not generated (empty string) in the current MAKE instance,
# before generating $(DEPFILES)
ifeq (,$(if $(D64PACK),$(_CACHE_FILE),1))
-include $(DEPFILES)
endif

# End of Targets
# ********************************************************************
