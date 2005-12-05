#!/bin/sh
wget --reject=.zip,.exe,.diff,.patch,.pingus,.png -domains=cmhousing.net \
     --recursive --level=0 --convert-links -v http://www.cmhousing.net/pingus/wiki
find -type f -name "*action=*" -exec rm -rf \{\} \;
find -type f -name "Search@*" -exec rm -rf \{\} \;
rm -f mains=*

