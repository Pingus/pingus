#!/bin/sh

DIRECTORY=`echo $1 | sed -e "s/\//\\\\\\\\\//g"` # <- $!&@^%^&*!@...
sh ./generate_images_list.sh | sed "s/^\(.*\)\/.*$/$DIRECTORY\/\1/" | sort | uniq

# EOF #