#!/usr/bin/env bash
# Collects sdl-libs build outputs into $out: lib/<abi>/{libSDL3.so,libSDL3_image.a},
# include/SDL3/ and include/SDL3_image/, dex/classes.dex.
#   TARGET_ABIS - space-separated list, e.g. "armeabi-v7a arm64-v8a"
#   $out        - set by Nix installPhase
set -euo pipefail

mkdir -p "$out/lib" "$out/dex" "$out/include"
for abi in $TARGET_ABIS; do
  mkdir -p "$out/lib/$abi"
  cp sdl-jni/libs/"$abi"/libSDL3.so sdl-jni/obj/local/"$abi"/libSDL3_image.a "$out/lib/$abi/"
done
cp -r sdl-jni/SDL/include/. "$out/include/"
cp -r sdl-jni/SDL_image/include/. "$out/include/"
cp classes/classes.dex "$out/dex/classes.dex"

echo "Installed native libs:"
find "$out/lib" -type f | sort
