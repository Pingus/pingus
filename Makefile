##  Pingus - A free Lemmings clone
##  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmx.de>
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##  
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##  
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.

DESTDIR = 
PREFIX  = /usr/local
DATADIR = $(PREFIX)/share/pingus
MANDIR  = $(PREFIX)/share/man
BINDIR  = $(PREFIX)/bin

build/pingus:
	mkdir -p build
	scons src
	scons
	ln -fs build/pingus

clean:
	rm -rf .sconf_temp/
	rm -f .sconsign.dblite
	rm -rf build/
	rm -f pingus
	scons -c

install: install-exec install-data install-man

install-exec: build/pingus
	install -d "$(DESTDIR)$(BINDIR)"

	install -D build/pingus "$(DESTDIR)$(BINDIR)/pingus.bin"
	echo -e "#!/bin/sh\nexec \"$(BINDIR)/pingus.bin\" --datadir \"$(DATADIR)\"" > "$(DESTDIR)$(BINDIR)/pingus"
	chmod 755 "$(DESTDIR)$(BINDIR)/pingus"

install-data:
	cd data/ && \
	find . \
        -type f -a  \( \
        -name "*.png" -o \
        -name "*.jpg" -o \
        -name "*.wav" -o \
        -name "*.scm" -o \
        -name "*.font" -o \
        -name "*.story" -o \
        -name "*.credits" -o \
        -name "*.prefab" -o \
        -name "*.it" -o \
        -name "*.ogg" -o \
        -name "*.s3m" -o \
        -name "*.po" -o \
        -name "*.worldmap" -o \
        -name "*.res" -o \
        -name "*.pingus" -o \
        -name "*.levelset" -o \
        -name "*.sprite" \
        \) -exec install -D {} $(DESTDIR)$(DATADIR)/{} \;

install-man:
	install -D doc/man/pingus.6 "$(DESTDIR)$(MANDIR)/man1/pingus.6"

.PHONY : clean install install-exec install-data install-man

# EOF #
