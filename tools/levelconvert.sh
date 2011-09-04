#!/bin/bash

set -e

function errmsg() {
    echo "ERROR: Critical failure :ERROR"
}

trap errmsg ERR

if [ ! \( -d "data/levels" \) ]; then
    echo "You must call this script from the top level Pingus directory"
    exit 1
fi

for IN in "$@"; do
    PREFIX=$(echo $IN | sed "s/\(.*\)data\/levels\/.*/\1/")
    TMP=$(tempfile)
    OUT="${IN##${PREFIX}}"
    OUT="${OUT%%.xml}"
    OUT="${OUT%%.plf}"    
    OUT="${OUT}.pingus"
    # echo "Prefix: $PREFIX"
    echo "IN:     $IN"
    echo "OUT:    $OUT"
    if true; then
        xsltproc \
            -o "$TMP" \
            tools/pingusv1tov2.xsl \
            "$IN"
        tools/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"        
    else
        xalan \
            -indent 0 \
            -xsl tools/pingusv1tov2.xsl \
            -in "$IN" \
            -out "$TMP"
        tools/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"
    fi
    rm "$TMP"
    echo "Conversion ok"
    echo ""
done

# EOF #
