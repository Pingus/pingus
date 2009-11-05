#!/bin/bash

for i in "$@"; do
    echo "File: $i"; sed  -i -e "/#include \"$(sedesc "${i%%.cpp}.hpp")\"/d" "$i"; 
done
