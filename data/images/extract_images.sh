#!/bin/sh

cat ../data/*.scr | sed -n "s/^[ 	]*[-A-Za-z0-9_]*[ 	]*=[ 	]*..\/images\/\([^ ]*\) *(.*);/\1 \\\\/p"
echo 

