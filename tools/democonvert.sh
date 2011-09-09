#!/bin/sh

for IN in "$@"; do
    OUT="${IN}"
    OUT="${OUT%%.xml}"
    OUT="${OUT}.pingus-demo"

    echo "IN:     $IN"
    echo "OUT:    $OUT"

    xsltproc \
        -o "$OUT" \
        tools/democonvert.xsl \
        "$IN"
done

# EOF #
