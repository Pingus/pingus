#!/bin/sh

for i in "$@"; do
    echo $i
    NAME=$(echo $i | sed "s/[\/\.]/_/g;s/^/HEADER_PINGUS_/" | tr [a-z] [A-Z])
    sed  -i "s/^\(#[a-z]*\) *HEADER_.*/\1 $NAME/" $i; 
done

# EOF #
