#!/bin/sh

set -e 

echo -n "Extracting strings from levels... "
./extract-levels.guile ../levels/*/*.pingus > levels.pot
echo "done"

echo -n "Extracting strings from worldmaps... "
./extract-levels.guile ../worldmaps/*.worldmap > worldmaps.pot
echo "done"

# EOF #
