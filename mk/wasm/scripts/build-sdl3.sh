#!/usr/bin/env bash
# SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Builds a static SDL3 for wasm32 with Emscripten, offline (no port
# download).
#
# Env:
#   SDL_SRC      - SDL3 source tree (required)
#   SDL_REVISION - revision string SDL reports (there is no git tree)
#   PREFIX       - install prefix (default: $PWD/prefix)
#   NIX_BUILD_CORES / JOBS - parallel jobs
set -euo pipefail

export EM_CACHE="${EM_CACHE:-${TMPDIR:-/tmp}/emcache}"
mkdir -p "$EM_CACHE"

PREFIX="${PREFIX:-$PWD/prefix}"

if [ -z "${SDL_SRC:-}" ]; then
  echo "error: SDL_SRC required" >&2
  exit 1
fi

echo "==> SDL3 static (wasm32) -> $PREFIX"
cp -a "$SDL_SRC" sdl-src
chmod -R u+w sdl-src
emcmake cmake -S sdl-src -B build-sdl \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PREFIX" \
  -DSDL_SHARED=OFF \
  -DSDL_STATIC=ON \
  -DSDL_TESTS=OFF \
  -DSDL_TEST_LIBRARY=OFF \
  -DSDL_EXAMPLES=OFF \
  -DSDL_REVISION="${SDL_REVISION:-SDL3}"
cmake --build build-sdl --parallel "${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"
cmake --install build-sdl

if [ ! -f "$PREFIX/lib/libSDL3.a" ]; then
  echo "error: no libSDL3.a under $PREFIX/lib" >&2
  exit 1
fi
echo "==> SDL3 ready under $PREFIX"
