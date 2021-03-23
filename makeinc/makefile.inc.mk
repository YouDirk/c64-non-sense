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


include ../makefile.config.mk

include ../.makefile.cache.mk
include ../makeinc/makefile.check.mk

include ../makeinc/makefile.variables.mk


# ********************************************************************
# Targets

.PHONY: all
all: $(OUTPUT).$(D64EXT)

.PHONY: recompile
recompile: clean
	$(MAKE) all

.PHONY: run run-ntsc run-load run-attach8 debug debug-ntsc
ifeq (,$(EMULATOR_OPT))
run run-ntsc run-load run-attach8 debug debug-ntsc: all
	$(error $(ERRB) C64 emulator not found!  Try Debian '$$> \
	  apt-get install vice' in Debian CONTRIB packages for \
	  installation.  After install run '$$> make clean-all')
else
run: EMUFLAGS += -pal -autostart
run-ntsc: EMUFLAGS += -ntsc -autostart
run-load: EMUFLAGS += -pal -autoload
run-attach8: EMUFLAGS += -pal -8
run run-ntsc run-load run-attach8: all
	$(EMULATOR_OPT) $(EMUFLAGS) $(OUTPUT).$(D64EXT)
debug: EMUFLAGS_DBG += -pal -autostart
debug-ntsc: EMUFLAGS_DBG += -ntsc -autostart
debug debug-ntsc: all $(OUTPUT).$(LABEXT_DEBUG)
	$(EMULATOR_OPT) $(EMUFLAGS_DBG) $(OUTPUT).$(D64EXT)
endif

.PHONY: disk
disk: all
ifeq (,$(if $(PMOUNT_OPT),$(PUMOUNT_OPT),))
	$(error $(ERRB) PMOUNT command not found!  Please copy the file \
          '$(OUTPUT).$(D64EXT)' manually to disk!  Or try Debian '$$> \
          apt-get install pmount' for installation.  After install run \
          '$$> make clean-all')
else
	@if [ ! -b "$(DISK_DEVICE)" ]; then \
	  echo "\nERROR: Device '$(DISK_DEVICE)' does not exist!  Please" \
	       "attach it to your computer or change the config" \
	       "'makefile.config.mk'!\n" > /dev/stderr; \
	  exit 1; \
	fi
	-$(PMOUNT_OPT) "$(DISK_DEVICE)" non-sense-disk
	cp -f $(OUTPUT).$(D64EXT) /media/non-sense-disk/
	sync && sleep 1
	$(PUMOUNT_OPT) "$(DISK_DEVICE)"
endif

.PHONY: tags-ctags
tags-ctags: $(CTAGSFILE)

.PHONY: tags-etags
tags-etags: $(ETAGSFILE)

.PHONY: tags-ebrowse
tags-ebrowse: $(EBROWSEFILE)

.PHONY: tags-all
tags-all: ctags etags ebrowse

.PHONY: clean-deps
clean-deps:
	-rm -f *.$(DEPEXT)

.PHONY: clean-tags
clean-tags:
	-rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)

.PHONY: clean
clean: clean-deps
	-rm -rf *.$(CCEXT) *.$(ASMEXT) *.$(OEXT) *.$(LOGEXT) *~ *.bak \
	  *.$(PRGEXT) *.$(DEF_GENHEXT) *.$(DEF_GENSEXT) *.$(MAPEXT) \
	  *.$(LABEXT)

# _CLEAN_MAKECACHE must be the last one in the dependency list,
# because it will be regenerated during recursive CLEAN calls
.PHONY: _clean_makecache
_clean-makecache:
	-rm -f ../.makefile.cache.mk
.PHONY: clean-all
clean-all: clean clean-tags _clean-makecache
	-rm -f $(OUTPUT).$(D64EXT)

%.$(DEPEXT): %.$(CEXT) $(MAKEFILEZ) | $(DEF_GENHFILES)
	@-$(CC) $(CCFLAGS) --create-full-dep $@ -o $*.$(OEXT) $< \
	  2> /dev/null; rm -f $*.$(OEXT)
%.$(DEPEXT): %.$(SEXT) $(MAKEFILEZ) | $(DEF_GENSFILES)
	@-$(AS) $(ASFLAGS) --create-full-dep $@ -o $*.$(OEXT) $< \
	  2> /dev/null; rm -f $*.$(OEXT)
%.$(DEF_DEPEXT): %.$(DEF_HEXT) $(MAKEFILEZ)
	@-echo "$*.$(DEF_GENHEXT) $*.$(DEF_GENSEXT): $< define.h" > $@

%.$(CCEXT): %.$(CEXT) $(MAKEFILEZ)
	$(LD) -E $(CCFLAGS) -o $@ $<
%.$(ASMEXT): %.$(CEXT) $(MAKEFILEZ)
	$(LD) -S $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(CEXT) $(MAKEFILEZ)
	$(LD) -c $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILEZ)
	$(AS) $(ASFLAGS) -o $@ $<

%.$(DEF_GENHEXT): %.$(DEF_HEXT) $(MAKEFILEZ)
	$(CC) -E $(CCFLAGS) -DGEN_C_HEADER -o $*.$(DEF_GENHEXT) $<
%.$(DEF_GENSEXT): %.$(DEF_HEXT) $(MAKEFILEZ)
	$(CC) -E $(CCFLAGS) -DGEN_ASM_HEADER -o $*.$(DEF_GENSEXT) $<

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

$(OUTPUT).$(PRGEXT) $(OUTPUT).$(MAPEXT) $(OUTPUT).$(LABEXT)&: \
  $(OBJFILES) $(DEF_GENFILES)
	$(LD) $(LDFLAGS) -m $(OUTPUT).$(MAPEXT) -Ln $(OUTPUT).$(LABEXT) \
	  -o $(OUTPUT).$(PRGEXT) $(OBJFILES) $(addprefix -l,$(LIBS))

%.$(LABEXT_DEBUG): %.$(LABEXT)
	cp -f $< $@ && echo 'break $(BREAKPOINT)' >> $@

%.$(D64EXT): %.$(PRGEXT)
	$(D64PACK) -format "$* disk",aa d64 $@ -attach $@ -write $< $*,p

.PHONY: _cache
_cache:
../.makefile.cache.mk: $(MAKEFILEZ)
	echo '' > $@
	$(MAKE) _CACHE_FILE=$@ _cache

# Make sure that $(CC) was set by makefile.check.mk and that
# _CACHE_FILE will not be generated (empty string) in the current MAKE
# instance, before generating $(DEPFILES)
ifeq (,$(if $(D64PACK),$(_CACHE_FILE),1))
-include $(DEF_DEPFILES)
-include $(DEPFILES)
endif

# End of Targets
# ********************************************************************
