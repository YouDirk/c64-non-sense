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


# ********************************************************************

# Set to 1 for debug build (recommended during development)
#
# values: [1 0]
#
DEBUG_BUILD := 1

# The removable device (i.e. USB storage) where to copy the D64 file.
#
# values, such like: /dev/sdc1
#
DISK_DEVICE := /dev/sdc1

# Only used if DEBUG_BUILD = 1, otherwise the best opimization level
# will be used.
#
# values: [-O -Oi -Or -Os]
#
OPTFLAG := -O

# Ansi C standard
#
# values: [c99 c89 c65]
#
CCSTDFLAG := c99

# ********************************************************************
