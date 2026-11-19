#!/usr/bin/env bash
# SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Builds a static SDL3_image for wasm32 with Emscripten, offline. Uses the
# built-in stb_image (loading) and miniz (IMG_SavePNG) backends, so no
# libpng / zlib is needed.
#
# Env:
#   SDL_IMAGE_SRC - SDL3_image source tree (required)
#   SDL_PREFIX    - prebuilt static SDL3 prefix (required)
#   PREFIX        - install prefix (default: $PWD/prefix)
#   NIX_BUILD_CORES / JOBS - parallel jobs
set -euo pipefail

export EM_CACHE="${EM_CACHE:-${TMPDIR:-/tmp}/emcache}"
mkdir -p "$EM_CACHE"

PREFIX="${PREFIX:-$PWD/prefix}"

if [ -z "${SDL_IMAGE_SRC:-}" ]; then
  echo "error: SDL_IMAGE_SRC required" >&2
  exit 1
fi
if [ -z "${SDL_PREFIX:-}" ] || [ ! -f "$SDL_PREFIX/lib/cmake/SDL3/SDL3Config.cmake" ]; then
  echo "error: SDL_PREFIX with lib/cmake/SDL3 required" >&2
  exit 1
fi

echo "==> SDL3_image static (wasm32, stb backend) -> $PREFIX"
cp -a "$SDL_IMAGE_SRC" sdl-image-src
chmod -R u+w sdl-image-src
emcmake cmake -S sdl-image-src -B build-sdl-image \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PREFIX" \
  -DSDL3_DIR="$SDL_PREFIX/lib/cmake/SDL3" \
  -DBUILD_SHARED_LIBS=OFF \
  -DSDLIMAGE_SAMPLES=OFF \
  -DSDLIMAGE_TESTS=OFF \
  -DSDLIMAGE_VENDORED=OFF \
  -DSDLIMAGE_DEPS_SHARED=OFF \
  -DSDLIMAGE_BACKEND_STB=ON \
  -DSDLIMAGE_PNG_LIBPNG=OFF \
  -DSDLIMAGE_AVIF=OFF \
  -DSDLIMAGE_JXL=OFF \
  -DSDLIMAGE_TIF=OFF \
  -DSDLIMAGE_WEBP=OFF
cmake --build build-sdl-image --parallel "${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"
cmake --install build-sdl-image

if [ ! -f "$PREFIX/lib/libSDL3_image.a" ]; then
  echo "error: no libSDL3_image.a under $PREFIX/lib" >&2
  exit 1
fi
echo "==> SDL3_image ready under $PREFIX"
