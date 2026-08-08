#!/usr/bin/env bash
# Collects sdl-libs build outputs into $out.
#   TARGET_ABIS - space-separated list, e.g. "armeabi-v7a arm64-v8a"
#   $out        - set by Nix installPhase
set -euo pipefail

mkdir -p "$out/lib" "$out/dex" "$out/include"
for abi in $TARGET_ABIS; do
  mkdir -p "$out/lib/$abi"
  cp sdl-jni/libs/"$abi"/*.so "$out/lib/$abi/"
done
cp -r sdl-jni/SDL/include/. "$out/include/"
if [ -d sdl-jni/SDL2_mixer_include ]; then
  # SDL_mixer.h alongside SDL headers (or under include/ as upstream lays out).
  cp -a sdl-jni/SDL2_mixer_include/. "$out/include/"
fi
cp classes/classes.dex "$out/dex/classes.dex"

echo "Installed native libs:"
find "$out/lib" -name '*.so' | sort
