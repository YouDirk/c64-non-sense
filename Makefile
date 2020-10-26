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


.PHONY: all recompile run run-load run-attach8 disk tags-ctags
all recompile run run-load run-attach8 disk tags-ctags:
	$(MAKE) -C src $@
.PHONY: tags-etags tags-ebrowse tags-all
tags-etags tags-ebrowse tags-all:
	$(MAKE) -C src $@

.PHONY: clean _clean
_clean:
	-rm -f *~ *.bak makeinc/*~ makeinc/*.bak
clean: _clean
	$(MAKE) -C src $@

.PHONY: clean-all
clean-all: _clean
	$(MAKE) -C src $@
