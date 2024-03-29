> C64 NonSense, the C64 Game Engine.
> Copyright (C) 2020-2022  Dirk "YouDirk" Lehmann
>
> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU Affero General Public License as published
> by the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
>
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU Affero General Public License for more details.
>
> You should have received a copy of the GNU Affero General Public License
> along with this program.  If not, see <https://www.gnu.org/licenses/>.


C64 NonSense
============

Welcome the C64 NonSense Game Engine :)

* Interesting stuff?  Then **Donate/Sponsor**  
  [![Donate/Sponsor][sponsor-pic]][sponsor-link] https://github.com/sponsors/YouDirk

The progression of C64 NonSense is vlogged at [Youtube here
(German)][youtube-list], too :)

![The Screenshot](trunk/screenshot-promo.jpg)

### Features

* **Video**  
      PAL (optimized), NTSC (working, via [`src/Timer.h`](src/Timer.h)
      optimizable)
* **Graphic modes**  
      HiRes 320x200 with 2 colors/cell, Multicolor 160x200 with 4
      colors/cell *[WiP]*
* **Sprites**  
      8x real-time configurable sprites, with collision detection and
      bitmap animations *[WiP]*
* **Input**  
      joysticks (port 2 and 1), keyboard (via scan codes and PETSCII
      characters)

Development toolchain
---------------------

We are developing on **Linux** using a **Debian** distribution as host
platform.  You need the following tools to develop, which you are able
to install using the Debian package manager `apt`

* **git** To clone the source repository, i.e.
  ```shell
  $> git clone https://github.com/YouDirk/c64-non-sense.git c64-non-sense
  ```
* **make** To build and run the stuff.  See the section *"Make targets"*
           below.  I.e. to compile and run in VICE type
  ```shell
  c64-non-sense/$> make run
  ```
* **cc65** Command `cl65` for the C64 C cross-compiler collection.
* **vice** Command `c1541` for the disk-image tool and command `x64`
           (optional) for the C64 emulator.  Make sure to have
           **`contrib`** enabled in `/etc/apt/sources.list`!
           Additionally you need to install the Kernal/BASIC ROMs
           manually, see the section below.
* **pmount (optional)** To automatically copy the D64 file to a
           removable disk (i.e. USB storage) using the make target
           `$> make disk`.

```shell
$> sudo apt-get install git make cc65 vice
$> sudo apt-get pmount               # optional
```

### VICE/x64 (C64 emulator) Kernal/BASIC/CharGen ROMs

If you are using the **Debian** `contrib` package of `vice` then you
need to download manually the Kernal, BASIC and CharGen ROMs of the
emulator due to copyright purposes.  Here an how-to:

Download the source tarball `vice-3.x.tar.gz` (replace `3.x` with your
installed VICE version as listed in `$> dpkg -l 'vice*'`) from

* *https://sourceforge.net/projects/vice-emu/files/releases/*

Unpack it and copy it´s whole `data/*` directory into
`/usr/share/vice/`.

```shell
tmp/$> tar zxf vice-3.x.tar.gz
tmp/$> sudo cp -rf vice-3.x/data/* /usr/share/vice/
```

Done, now you can run VICE using `$> x64` :)

### Make targets

```make
all <default>: Compiles and packs the src/non-sense.d64 disk image file
recompile:     Runs 'clean' followed by 'all'
clean:         Deletes temporary files / prepare for recompilation
               Useful on 'Header file not found' compilation errors
clean-all:     Deletes all files which are not under version control
run:           Make 'all' and `RUN` NonSense in VICE, use PAL (VIC 6561)
run-ntsc:      Same as 'run', but using NTSC version of VIC (VIC 6560)
run-load:      Same as 'run', but just `LOAD"*",8,1` NonSense, no `RUN`
run-attach8:   Same as 'run', but no `LOAD`, no `RUN`
debug:         Make 'all', then as configured in makefile.config.mk set
               breakpoint and `RUN` NonSense with debugger attached (PAL)
debug-ntsc:    Same as 'debug', but using NTSC version of VIC emulated
disk:          Make 'all' and copy the D64 file to a removable disk
               (i.e. USB storage) as configured in makefile.config.mk
tags-all:      Make 'tags-ctags', 'tags-etags' and 'tags-ebrowse'
tags-ctags:    Runs 'ctags' indexer for TAB auto-completion
tags-etags:    Runs 'ebrowse' indexer for TAB auto-completion in Emacs
tags-ebrowse:  Runs 'etags' indexer for TAB auto-completion
```

Credits
-------

* Donators:  
  [![Donate/Sponsor][sponsor-pic]][sponsor-link] *nobody1, nobody2,
  nobody3 D':*

* Regular contributors:  
  *Dirk "YouDirk" Lehmann*

----------------------------------------------------------------------

### Appendix: Programmers references

* Commodore 64 wiki *(well beginning)*
    - (en)                     https://www.c64-wiki.com/wiki/C64
    - (de, *sometimes deeper*) https://www.c64-wiki.de/wiki/C64

* Commodore 64 Programmer's Reference Guide
    - (pdf)  http://www.classiccmp.org/cini/pdf/Commodore/C64%20Programmer's%20Reference%20Guide.pdf
    - (html) http://www.devili.iki.fi/Computers/Commodore/C64/Programmers_Reference/page_iii.html
    - (txt)  http://www.zimmers.net/cbmpics/cbm/c64/c64prg.txt
    - (wiki) https://www.c64-wiki.com/wiki/Commodore_64_Programmer%27s_Reference_Guide

* Commodore 64 ROMs disassembled
    - (Kernal, BASIC, etc.) http://unusedino.de/ec64/technical/aay/c64/

* Commodore 64 schematic diagrams
    - (left)   https://www.c64-wiki.com/wiki/File:PRG_Schematic_(left).gif
    - (right)  https://www.c64-wiki.com/wiki/File:PRG_Schematic_(right).gif
    - (others) http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c64/

* Commodore 64 datasheets
    - (MOS 6510, CPU)            http://www.6502.org/documents/datasheets/mos/mos_6510_mpu.pdf  
      *instruction set, opcodes* http://www.6502.org/tutorials/6502opcodes.html

* CC65 compiler specifications
    - (2x C stack: return & parameter) https://github.com/cc65/wiki/wiki/Parameter-and-return-stacks
    - (C calling conventions) https://github.com/cc65/wiki/wiki/Parameter-passing-and-calling-conventions


[sponsor-link]: https://github.com/sponsors/YouDirk
[sponsor-pic]: trunk/button-sponsor.20.png
[youtube-list]: https://www.youtube.com/playlist?list=PLSC2VXOJyxP01kz__GU2nvx4RmgCN4ZwO
