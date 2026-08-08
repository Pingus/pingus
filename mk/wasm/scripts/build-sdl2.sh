#!/usr/bin/env bash
# Build static SDL2 for wasm32 (project-agnostic).
#
# Env:
#   SDL_SRC     - SDL2 source tree (required)
#   PREFIX      - install prefix (default: $PWD/prefix)
#   NIX_BUILD_CORES / JOBS - parallel jobs
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
# shellcheck source=../../scripts/common.sh
[ -f "$SCRIPT_DIR/../../scripts/common.sh" ] && . "$SCRIPT_DIR/../../scripts/common.sh"

export EM_CACHE="${TMPDIR:-/tmp}/emcache"
mkdir -p "$EM_CACHE"

PREFIX="${PREFIX:-$PWD/prefix}"
mkdir -p "$PREFIX/lib" "$PREFIX/include"

if [ -z "${SDL_SRC:-}" ]; then
  echo "error: SDL_SRC required" >&2
  exit 1
fi

echo "==> SDL2 static (wasm32) → $PREFIX"
cp -a "$SDL_SRC" SDL2-src
chmod -R u+w SDL2-src
mkdir -p build-sdl2
cd build-sdl2
emcmake cmake ../SDL2-src \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PREFIX" \
  -DSDL_SHARED=OFF \
  -DSDL_STATIC=ON \
  -DSDL_TEST=OFF \
  -DSDL_STATIC_PIC=ON
emmake make -j"${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"
emmake make install
cd ..

if [ ! -f "$PREFIX/lib/libSDL2.a" ]; then
  find build-sdl2 -name 'libSDL2.a' -exec cp {} "$PREFIX/lib/" \; || true
fi
mkdir -p "$PREFIX/include"
if [ ! -d "$PREFIX/include/SDL2" ] && [ -d SDL2-src/include ]; then
  cp -a SDL2-src/include/. "$PREFIX/include/" || true
fi

echo "==> SDL2 ready under $PREFIX"
ls -la "$PREFIX/lib/libSDL2.a"
ls -la "$PREFIX/include/SDL2/SDL.h" 2>/dev/null || ls -la "$PREFIX/include/SDL.h" 2>/dev/null || true
