#!/bin/sh

for filename in $@; do
    if [ ! -f $filename ]; then
	echo "$filename: file not found"
	continue;
    fi

    out=`tempfile`

#    echo "Identifing: $0 $out ..."

    identify -verbose $filename > $out
    width=`grep "  Geometry" $out | sed "s/  Geometry: \([0-9]*\)x[0-9]*/\1/"`
    height=`grep "  Geometry" $out | sed "s/  Geometry: [0-9]*x\([0-9]*\)/\1/"`
    name=`basename $filename | sed "s/.jpg$//" | sed "s/.pcx$//" | sed "s/.tga$//"`
    type=`grep "  Format:" $out | sed "s/  Format: \(.*\)/\1/"`

    #echo "Height: $height"
    #echo "Width:  $width"
    #echo "Name:   $name"
    #echo "Type:   $type"
    #echo "Datafile line:"

    if [ "$type" = "PCX (ZSoft IBM PC Paintbrush)" ]; then 
	echo -e "\t$name = $filename (type=surface, x=0, y=0, width=$width, height=$height, tcol=0);"
    else
	echo -e "\t$name = $filename (type=surface, x=0, y=0, width=$width, height=$height);"
    fi

done
