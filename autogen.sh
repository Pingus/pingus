#!/bin/sh

rm -f config.cache
aclocal
autoheader
automake --add-missing
autoconf

# HACK Mode on
cd po/
make -f Makefile.in Makefile
# HACK Mode off

# EOF #