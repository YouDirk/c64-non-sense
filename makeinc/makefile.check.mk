# C64 NonSense, the C64 Game Engine.
# Copyright (C) 2020-2025  Dirk "YouDirk" Lehmann
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


_BLANK :=
define NL

$(_BLANK)
endef
ERRB  = $(NL)$(NL)  ERROR:
WARNB = $(NL)$(NL)  warning:

# ********************************************************************
# Linux commands, feature check

ifneq (,$(_CACHE_FILE))

_CMD_TEST = \
  $(eval _CMD_TEST_BUF:=checking for $(2)... )$(\
  )$(shell which $(1) 2> /dev/null)
_CMD_TEST_NOOUT = $(shell which $(1) 2> /dev/null)
_CMD_TEST_RESULT = $(info $(_CMD_TEST_BUF)$(1))
_CMD_TEST_RESNO = $(info $(_CMD_TEST_BUF)no - Debian package $(1))

CC := $(call _CMD_TEST,cc65,C64 C cross-compiler)
ifeq (,$(CC))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) C64 C cross-compiler not found!  Try Debian '$$> \
    apt-get install cc65' for installation.  After install run \
    '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(CC))
  $(shell echo 'CC = $(CC)' >> $(_CACHE_FILE))
endif

AS := $(call _CMD_TEST,ca65,C64 ASM cross-compiler)
ifeq (,$(AS))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) C64 ASM cross-compiler not found!  Try Debian '$$> \
    apt-get install cc65' for installation.  After install run \
    '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(AS))
  $(shell echo 'AS = $(AS)' >> $(_CACHE_FILE))
endif

LD := $(call _CMD_TEST,cl65,C64 linker)
ifeq (,$(LD))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) C64 linker not found!  Try Debian '$$> \
    apt-get install cc65' for installation.  After install run \
    '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(LD))
  $(shell echo 'LD = $(LD)' >> $(_CACHE_FILE))
endif

D64PACK := $(call _CMD_TEST,c1541,D64 disk-image packer)
ifeq (,$(D64PACK))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) D64 disk-image packer not found!  Try Debian '$$> \
    apt-get install vice' in Debian CONTRIB packages for installation. \
    After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(D64PACK))
  $(shell echo 'D64PACK = $(D64PACK)' >> $(_CACHE_FILE))
endif

# --------------------------------------------------------------------
# Optional

EMULATOR_OPT := $(call _CMD_TEST,x64,C64 emulator (optional))
ifeq (,$(EMULATOR_OPT))
  $(call _CMD_TEST_RESNO,vice)
  $(shell echo 'EMULATOR_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(EMULATOR_OPT))
  $(shell echo 'EMULATOR_OPT = $(EMULATOR_OPT)' >> $(_CACHE_FILE))
endif

PMOUNT_OPT := $(call _CMD_TEST,pmount,pmount (optional))
ifeq (,$(PMOUNT_OPT))
  $(call _CMD_TEST_RESNO,pmount)
  $(shell echo 'PMOUNT_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(PMOUNT_OPT))
  $(shell echo 'PMOUNT_OPT = $(PMOUNT_OPT)' >> $(_CACHE_FILE))
endif

PUMOUNT_OPT := $(call _CMD_TEST,pumount,pumount (optional))
ifeq (,$(PUMOUNT_OPT))
  $(call _CMD_TEST_RESNO,pmount)
  $(shell echo 'PUMOUNT_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(PUMOUNT_OPT))
  $(shell echo 'PUMOUNT_OPT = $(PUMOUNT_OPT)' >> $(_CACHE_FILE))
endif

CTAGS_OPT := $(call _CMD_TEST,ctags,ctags (optional))
ifeq (,$(CTAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'CTAGS_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(CTAGS_OPT))
  $(shell echo 'CTAGS_OPT = $(CTAGS_OPT)' >> $(_CACHE_FILE))
endif

ETAGS_OPT := $(call _CMD_TEST,etags,etags (optional))
ifeq (,$(ETAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'ETAGS_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(ETAGS_OPT))
  $(shell echo 'ETAGS_OPT = $(ETAGS_OPT)' >> $(_CACHE_FILE))
endif

EBROWSE_OPT := $(call _CMD_TEST,ebrowse,ebrowse (optional))
ifeq (,$(EBROWSE_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'EBROWSE_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(EBROWSE_OPT))
  $(shell echo 'EBROWSE_OPT = $(EBROWSE_OPT)' >> $(_CACHE_FILE))
endif

# End of Optional
# --------------------------------------------------------------------

endif # ifneq (,$(_CACHE_FILE))

# End of Linux commands, feature check
# ********************************************************************
