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

# Enable debugging symbols and debug code, such like
#
# * include <std*.h> files
#
# * indicate not served interrupts
#
# If disabled then the DEBUG_OPT_* options will be disabled too.
#
# values: [1 0]
#
DEBUG_BUILD := 1

# Mark as border color at which position the Graphix_render_isr()
# routine will be executed.
#
# values: [1 0]
#
DEBUG_OPT_IRQ_RENDERTIME := 0

# The removable disk (i.e. USB storage) where to copy the D64 file.
#
# values, such like: /dev/sdc1
#
DISK_DEVICE := /dev/sdc1

# Optimization flags for compiler
#
# values: [-O -Oi -Or -Os]
#
OPTFLAG := -O

# CC65 -W[-]<warning> warning flags.  To find out what is <warning>,
# see
#
#   $> cc65 --list-warnings
#
# values: -W[-]<warning>
#
CCWARNINGS :=

# ********************************************************************
