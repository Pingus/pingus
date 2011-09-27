#!/bin/bash

for i in data/prefabs/*/*.prefab; do
    OUT="${i##data/}"
    OUT="data/images/${OUT%%.prefab}.png"
    echo "Writing $OUT"
    build/extra/pingus-level2png "$i" "$OUT"
done

# EOF #
