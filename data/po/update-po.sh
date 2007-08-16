#!/bin/sh

for i in *.po; do
    echo -n "Updating $i:"
    msgmerge --update "$i" pingus.pot
done

# EOF #
