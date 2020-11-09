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
# * print debug messages if C64 NonSense terminates regularly
#
# * indicate not served interrupts by changing the border color
#
# * include <std*.h> files
#
# If enabled (1) then you can configure the DEBUG_OPT_* options for
# yourself.  If disabled (0) then the DEBUG_OPT_* options are disabled
# too.  If ALL (all) is set then all DEBUG_OPT_* options are enabled
# too.
#
# values: [all 1 0]
#
DEBUG_BUILD := 1

# Mark as border color at which position the Graphix_render_isr()
# routine will be executed.
#
# values: [1 0]
#
DEBUG_OPT_IRQ_RENDERTIME := 0

# Output debug warnings if the logical Timer 1 ISR is not in sync with
# physical CIA1 timer A.
#
# values: [1 0]
#
DEBUG_OPT_TIMER1_SYNCCHECK := 0

# Symbol (prefixed with '.') or address (prefixed with '$$') of the
# initial break point for `make` target DEBUG.  It´s also possible to
# override this value using `make` like this
#
#   $> make BREAKPOINT=._Timer_1_get32 debug
#
# Additionally C symbols are namespaced with an underscore and
# beginning '_', see example values below.
#
# values: [.asm_symbol ._c_symbol $$<hex-address>]
#
BREAKPOINT := ._main

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
