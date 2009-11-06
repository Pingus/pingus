#!/bin/bash

for i in "$@"; do
    echo "(sprite"
    echo "   (name \"$(basename "${i%%.png}")\")"
    echo "   (image-file \"${i}\"))"
done

# EOF #
