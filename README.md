> C64 NonSense, just playing around with C64 cross-compile tools.
> Copyright (C) 2020  Dirk "YouDirk" Lehmann
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

Just playing around with C64 cross-compile tools.

* Interesting stuff?  Then **Donate/Sponsor**  
  [![Donate/Sponsor][sponsor-pic]][sponsor-link] https://github.com/sponsors/YouDirk

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
  c64-non-sense$> make run
  ```
* **cc65** The `cl65` C64 C cross-compiler collection.
* **vice** The `x64` C64 emulator and `c1541` disk-image tool.  Make
           sure to have **`contrib`** enabled in
           `/etc/apt/sources.list`!  Additionally you need to install
           the Kernal/BASIC ROMs manually, see the section below.
* **pmount (optional)** To automatically copy the D64 file to a
           removable disk (i.e. USB storage) using the make target
           `make disk`.

```shell
$> sudo apt-get install git make cc65
$> sudo apt-get pmount                 # optional
```

### VICE/x64 (C64 emulator) Kernal/BASIC ROMs

If you are using the **Debian** `contrib` package of `vice` then you
need to download the Kernal and BASIC ROMs of the emulator manually
due to copyright purposes.  Here an how-to:

Download the source tarball `vice-3.x.tar.gz` (replace `3.x` with your
installed VICE version shown in `dpkg -l vice*`) from

* *ftp://ftp.zimmers.net/pub/cbm/crossplatform/emulators/VICE/*

Unpack it and copy it´s whole `data/*` directory into `/usr/lib/vice/*`.

```shell
tmp/$> tar zxf vice-3.x.tar.gz
tmp/$> sudo cp -rf vice-3.x/data/* /usr/lib/vice/
```

Done, now you can run VICE using `$> x64` :)

### Make targets

```make
all <default>: Compiles the current directory and all sub-directories
recompile:     Runs 'clean' followed by 'all'
clean:         Deletes temporary files / prepare for recompilation
               Useful on 'Header file not found' compilation errors
clean-all:     Deletes all files which are not under version control
run:           Make 'all' and `RUN` NonSense in VICE emulator
run-load:      Make 'all' and `LOAD"*",8,1` NonSense in VICE emulator
run-mount8:    Make 'all' and run VICE emulator (no `LOAD`, no `RUN`)
debug:         Make 'all' and 'run' using a debugger (not implemented)
disk:          Make 'all' and copy the D64 file to a removable disk (i.e.
               USB storage) as configured in makefile.config.mk
tags-all:      Make 'tags-ctags', 'tags-etags' and 'tags-ebrowse'
tags-ctags:    Runs 'ctags' indexer for TAB auto-completion
tags-etags:    Runs 'ebrowse' indexer for TAB auto-completion in Emacs
tags-ebrowse:  Runs 'etags' indexer for TAB auto-completion
```

Programmers references
----------------------

* Commodore 64 Programmer's Reference Guide
    - (pdf)  http://www.classiccmp.org/cini/pdf/Commodore/C64%20Programmer's%20Reference%20Guide.pdf
    - (html) http://www.devili.iki.fi/Computers/Commodore/C64/Programmers_Reference/page_iii.html
    - (txt)  http://www.zimmers.net/cbmpics/cbm/c64/c64prg.txt
    - (wiki) https://www.c64-wiki.com/wiki/Commodore_64_Programmer%27s_Reference_Guide

* Commodore 64 Schematic Diagram
    - (left)   https://www.c64-wiki.com/wiki/File:PRG_Schematic_(left).gif
    - (right)  https://www.c64-wiki.com/wiki/File:PRG_Schematic_(right).gif
    - (others) http://www.zimmers.net/anonftp/pub/cbm/schematics/computers/c64/

Credits
=======

* Donators:  
  [![Donate/Sponsor][sponsor-pic]][sponsor-link] *nobody1, nobody2,
  nobody3 D':*

* Regular contributors:  
  *Dirk "YouDirk" Lehmann*


[sponsor-link]: https://github.com/sponsors/YouDirk
[sponsor-pic]: trunk/button-sponsor.20.png
