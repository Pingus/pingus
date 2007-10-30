#!/bin/sh

set -e

if [ $# -ne 1 ]; then
    echo "Usage $0 DIRECTORY"
else
    if [ ! -x ./pingus ]; then
        echo "Error: You have to compile pingus first before you can install it."
        echo "To compile it, type:"
        echo ""
        echo "  scons"
        echo ""
        exit 1
    fi

    BINDIR="$1/bin/"
    DATADIR="$1/share/pingus/"

    echo "Installing Pingus in: $1"

    mkdir -p "$BINDIR"
    mkdir -p "$DATADIR"

    install -v pingus "$BINDIR/pingus"

    find data/ \
        -type f -a  \( \
        -name "*.png" -o \
        -name "*.jpg" -o \
        -name "*.wav" -o \
        -name "*.scm" -o \
        -name "*.font" -o \
        -name "*.it" -o \
        -name "*.ogg" -o \
        -name "*.s3m" -o \
        -name "*.po" -o \
        -name "*.worldmap" -o \
        -name "*.res" -o \
        -name "*.pingus" -o \
        -name "*.levelset" \
        \) -exec cp --parents -v {} "$DATADIR" \;
        
   echo ""
   echo "Pingus installation complete, you can start the games with:"
   echo ""
   echo "  ${BINDIR}pingus"
   echo
fi

# EOF #
