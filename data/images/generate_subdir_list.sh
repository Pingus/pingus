#!/bin/sh

find . -type d -and -not \( -name ".*" -or -name "CVS" \) | grep -v "/old" | grep -v unsorted | sed "s/^\.\///"
