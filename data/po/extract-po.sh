#!/bin/sh

set -e

TMP=$(tempfile)
OUT="data/po/pingus.pot"

cd ../.. # go to toplevel dir
echo -n "Generating C++ file list... "
find src/ -name "*.?pp" > "$TMP"
echo "done"

echo -n "Extracting strings from C++..."
xgettext \
    --c++ \
    --boost \
    --directory "." \
    --output "${OUT}.tmp" \
    --from=code=ASCII \
    --keyword=_ \
    --keyword=N_ \
    --files-from "$TMP"
echo "done"

echo "" >> "${OUT}.tmp"

echo -n "Extracting strings from levels... "
data/po/extract-levels.guile data/levels/*/*.pingus >> "${OUT}.tmp"
echo "done"

echo -n "Extracting strings from worldmaps... "
data/po/extract-levels.guile data/worldmaps/*.worldmap >> "${OUT}.tmp"
echo "done"

sed -i "s/\"Content-Type: text\\/plain; charset=CHARSET\\n\\/\"Content-Type: text\\/plain; charset=ASCII\\n\"" \
    "${OUT}.tmp"

echo -n "Removing duplicate entries... "
msguniq -o "$OUT" "${OUT}.tmp"
echo "done"

echo "Output written to pingus.pot"

rm $TMP

# EOF #
