#!/bin/sh

TOPDIR=`pwd`

for i in $@; do
    echo "Generating makefiles for $i"
    DIRS=`find $i -maxdepth 1 -type d -and -not \( -name "old" -or -name "CVS" -or -name ".*" \) | tail +2`
    if [ -z "$DIRS" ]; then
	sh ./generate_makefile_images.sh $i
    else
	sh ./generate_makefile_subdir.sh $i
    fi
    echo 
done

# EOF #