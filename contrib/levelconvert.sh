#!/bin/bash

set -e

function errmsg() {
    echo "ERROR: Critical failure :ERROR"
}

trap errmsg ERR

if [ ! \( -d "trunk/pingus" \) ]; then
    echo "You must call this script from the top level of the SVN repository"
    exit 1
fi

for IN in "$@"; do
    PREFIX=$(echo $IN | sed "s/\(.*\)data\/levels\/.*/\1/")
    TMP=$(tempfile)
    OUT="trunk/pingus/${IN##${PREFIX}}"
    # echo "Prefix: $PREFIX"
    echo "IN:     $IN"
    echo "OUT:    $OUT"
    if true; then
        xsltproc \
            -o "$TMP" \
            trunk/pingus/contrib/pingusv1tov2.xsl \
            "$IN"
        trunk/pingus/contrib/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"        
    else
        xalan \
            -indent 0 \
            -xsl trunk/pingus/contrib/pingusv1tov2.xsl \
            -in "$IN" \
            -out "$TMP"
        trunk/pingus/contrib/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"
    fi
    rm "$TMP"
    echo "Conversion ok"
    echo ""
done

# EOF #
