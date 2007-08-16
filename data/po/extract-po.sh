#!/bin/sh

set -e

TMP=$(tempfile)
OUT="pingus.pot"

echo -n "Generating file list... "
find ../../src/ -name "*.?pp" | sed "s/..\/..\///" > "$TMP"
echo "done"

echo -n "Extracting strings..."
xgettext \
    --c++ \
    --boost \
    --directory "../../" \
    --output "$OUT" \
    --keyword=_ \
    --keyword=N_ \
    --files-from "$TMP"
echo "done"

echo "Output written to $OUT"

rm $TMP

# EOF #
