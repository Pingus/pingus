#!/bin/sh

set -e

AUTOMAKE=""
ACLOCAL=""

if [ `which automake-1.7` ]; then
    AUTOMAKE="automake-1.7"
    ACLOCAL="aclocal-1.7"
elif [ `which automake-1.6` ]; then
    AUTOMAKE="automake-1.6"
    ACLOCAL="aclocal-1.6"
else
    AUTOMAKE="automake"
    ACLOCAL="aclocal"
fi

echo "Generating ./configure, this may take a while..."
autoheader
$ACLOCAL
$AUTOMAKE --force-missing --add-missing --copy
autoconf

# EOF #
