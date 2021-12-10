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
# too.  If FULL (full) is set then all DEBUG_OPT_* options are enabled
# too.
#
# values: [full 1 0], default: 1
#
DEBUG_BUILD := 1

# Log debug errors if non-<stdint.h> arguments (i.e. UINT6_T, INT4_T)
# in function calls are overflowing.
#
# values: [1 0], default: 1
#
DEBUG_OPT_OVERFLOW_CHECK := 1

# Mark as border color (black) the time how long the C64 NonSense
# Engine is polling stuff.  This is equivalent to the CPU time which
# is free to use your stuff to do in your game.
#
# values: [1 0], default: 0
#
DEBUG_OPT_RENDERTIME_FREECPU := 0

# Mark as border color at which position the Graphix_render_isr()
# routine will be executed.
#
# values: [1 0], default: 0
#
DEBUG_OPT_RENDERTIME_IRQ := 0

# Log debug warnings if the logical Timer 1 ISR is not in sync with
# physical CIA1 timer A.
#
# values: [1 0], default: 0
#
DEBUG_OPT_TIMER1_SYNCCHECK := 0

# Default is Triple Buffering (set value 0).
#
# Enable Double Buffering (set value 1) to reduce input delay.  But
# double buffering does also results in a cyclic stuttering FPS,
# depending on engine/timer tickrate.
#
# values: [1 0], default: 0
#
CONF_DOUBLE_BUFFERING := 0

# Symbol (prefixed with '.') or address (prefixed with '$$') of the
# initial break point for `make` target DEBUG.  It´s also possible to
# override this value using `make` like this
#
#   $> make BREAKPOINT=._Timer_1_get32 debug
#
# Additionally C symbols are namespaced with an underscore and
# beginning '_', see example values below.
#
# values: [.asm_symbol ._c_symbol $$<hex-address>], default: ._main
#
BREAKPOINT := ._main

# The removable disk (i.e. USB storage) where to copy the D64 file.
# Is used for `make` target DISK.
#
#   $> make disk
#
# values, such like: /dev/sdc1
#
DISK_DEVICE := /dev/sdc1

# Optimization flags for compiler
#
# values: [-O -Oi -Or -Os], default: -O
#
OPTFLAG := -O

# CC65 -W[-]<warning> warning flags.  To find out what is <warning>,
# see
#
#   $> cc65 --list-warnings
#
# values: -W[-]<warning>, default: <empty-string>
#
CCWARNINGS :=

# ********************************************************************
