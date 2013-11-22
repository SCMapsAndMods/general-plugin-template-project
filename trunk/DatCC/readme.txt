DatCC v0.2
Made by pastelmind
Contact me at crimboween@naver.com

== Introduction ==

DatCC is a command-line compiler/decompiler for StarCraft's DAT files. It can:

 * Decompile DAT files into human-readable INI files with helpful comments.
 * Compile modified INI files into DAT files. You can merge partial INI files
   into existing DAT files as well.
 * Compare two DAT files and output the differences between them to an INI file.

The following DAT files are supported:
 * units.dat
 * weapons.dat
 * flingy.dat
 * sprites.dat
 * images.dat
 * sfxdata.dat
 * techdata.dat
 * upgrades.dat
 * orders.dat

For information on usage, run DatCC.exe with the --help option.

DatCC makes use of the following 3rd-party libraries:
 * TCLAP (http://tclap.sourceforge.net/) for parsing command line arguments.
 * SimpleIni (http://github.com/brofield/simpleini) for handling INI files.

== License ==

Copyright (c) 2013, pastelmind
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the <ORGANIZATION> nor the names of its contributors may
   be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.