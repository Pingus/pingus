#!/bin/sh

rm -f config.cache
aclocal
autoheader
automake --add-missing
autoconf2.50

# EOF #
