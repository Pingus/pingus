#1/bin/bash

for i in "$@"; do
    echo "File: $i"; sed -i -e "s/<http:\/\/www.gnu.org\/licenses\/>./<http:\/\/www.gnu.org\/licenses\/>.\n\n#include \"$(sedesc "${i%%.cpp}.hpp")\"/" "$i"; 
done
