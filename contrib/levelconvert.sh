#!/bin/sh

set -e
trap errmsg ERR

function errmsg() {
    echo "ERROR: Critical failure :ERROR"
}

if [ ! \( -d "branches/pingus_sdl" \) ]; then
    echo "You must call this script from the top level of the SVN repository"
    exit 1
fi

for IN in "$@"; do
    PREFIX=$(echo $IN | sed "s/\(.*\)data\/levels\/.*/\1/")
    TMP=$(tempfile)
    OUT="branches/pingus_sdl/${IN##${PREFIX}}"
    # echo "Prefix: $PREFIX"
    echo "IN:     $IN"
    echo "OUT:    $OUT"
    if true; then
        xsltproc \
            -o "$TMP" \
            branches/pingus_sdl/contrib/pingusv1tov2.xsl \
            "$IN"
        branches/pingus_sdl/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"        
    else
        xalan \
            -indent 0 \
            -xsl branches/pingus_sdl/contrib/pingusv1tov2.xsl \
            -in "$IN" \
            -out "$TMP"
        branches/pingus_sdl/xml2sexpr.rb "$TMP" "$IN" > "${OUT}"
    fi
    rm "$TMP"
    echo "Conversion ok"
    echo ""
done

# EOF #
