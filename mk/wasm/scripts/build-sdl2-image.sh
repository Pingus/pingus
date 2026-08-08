#!/usr/bin/env bash
# Build static SDL2_image for wasm32 (stb backend). Project-agnostic.
#
# Env:
#   SDL_IMAGE_SRC - SDL2_image source tree (required)
#   SDL_PREFIX    - prebuilt SDL2 prefix with libSDL2.a (required)
#   PREFIX        - install prefix (default: $PWD/prefix)
set -euo pipefail

export EM_CACHE="${TMPDIR:-/tmp}/emcache"
mkdir -p "$EM_CACHE"

PREFIX="${PREFIX:-$PWD/prefix}"
mkdir -p "$PREFIX/lib" "$PREFIX/include"

if [ -z "${SDL_IMAGE_SRC:-}" ]; then
  echo "error: SDL_IMAGE_SRC required" >&2
  exit 1
fi
if [ -z "${SDL_PREFIX:-}" ] || [ ! -f "${SDL_PREFIX}/lib/libSDL2.a" ]; then
  echo "error: SDL_PREFIX with libSDL2.a required" >&2
  exit 1
fi

SDL2_LIB="$SDL_PREFIX/lib/libSDL2.a"
if [ -f "$SDL_PREFIX/include/SDL2/SDL.h" ]; then
  SDL2_INC="$SDL_PREFIX/include/SDL2"
elif [ -f "$SDL_PREFIX/include/SDL.h" ]; then
  SDL2_INC="$SDL_PREFIX/include"
else
  SDL2_INC="$SDL_PREFIX/include/SDL2"
fi
export CMAKE_PREFIX_PATH="$SDL_PREFIX${CMAKE_PREFIX_PATH:+:$CMAKE_PREFIX_PATH}"
export PKG_CONFIG_PATH="$SDL_PREFIX/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"

echo "==> SDL2_image static (wasm32, stb backend)"
cp -a "$SDL_IMAGE_SRC" SDL2_image-src
chmod -R u+w SDL2_image-src
mkdir -p build-sdl2-image
cd build-sdl2-image

common_args=(
  -DCMAKE_BUILD_TYPE=Release
  -DCMAKE_INSTALL_PREFIX="$PREFIX"
  -DCMAKE_PREFIX_PATH="$SDL_PREFIX"
  -DSDL2_DIR="$SDL_PREFIX/lib/cmake/SDL2"
  -DSDL2_LIBRARY="$SDL2_LIB"
  -DSDL2_INCLUDE_DIR="$SDL2_INC"
  -DPrivateSDL2_LIBRARY="$SDL2_LIB"
  -DPrivateSDL2_INCLUDE_DIR="$SDL2_INC"
  -DBUILD_SHARED_LIBS=OFF
  -DSDL2IMAGE_SAMPLES=OFF
)

set +e
emcmake cmake ../SDL2_image-src \
  "${common_args[@]}" \
  -DSDL2IMAGE_VENDORED=ON \
  -DSDL2IMAGE_BACKEND_STB=ON \
  -DSDL2IMAGE_AVIF=OFF \
  -DSDL2IMAGE_BMP=ON \
  -DSDL2IMAGE_GIF=ON \
  -DSDL2IMAGE_JPG=ON \
  -DSDL2IMAGE_JXL=OFF \
  -DSDL2IMAGE_LBM=OFF \
  -DSDL2IMAGE_PCX=OFF \
  -DSDL2IMAGE_PNG=ON \
  -DSDL2IMAGE_PNM=OFF \
  -DSDL2IMAGE_QOI=OFF \
  -DSDL2IMAGE_SVG=OFF \
  -DSDL2IMAGE_TGA=OFF \
  -DSDL2IMAGE_TIF=OFF \
  -DSDL2IMAGE_WEBP=OFF \
  -DSDL2IMAGE_XCF=OFF \
  -DSDL2IMAGE_XPM=OFF \
  -DSDL2IMAGE_XV=OFF
cfg=$?
set -e
if [ "$cfg" -ne 0 ]; then
  echo "retry SDL2_image with minimal options + explicit SDL2 paths..."
  rm -rf ./*
  emcmake cmake ../SDL2_image-src "${common_args[@]}"
fi
emmake make -j"${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"
emmake make install || true
if [ ! -f "$PREFIX/lib/libSDL2_image.a" ]; then
  find . -name 'libSDL2_image.a' -exec cp {} "$PREFIX/lib/" \; || true
fi
if [ ! -f "$PREFIX/include/SDL_image.h" ]; then
  if [ -f ../SDL2_image-src/include/SDL_image.h ]; then
    cp ../SDL2_image-src/include/SDL_image.h "$PREFIX/include/"
  elif [ -f ../SDL2_image-src/SDL_image.h ]; then
    cp ../SDL2_image-src/SDL_image.h "$PREFIX/include/"
  fi
fi
if [ -f "$PREFIX/include/SDL_image.h" ] && [ ! -f "$PREFIX/include/SDL2/SDL_image.h" ]; then
  mkdir -p "$PREFIX/include/SDL2"
  cp "$PREFIX/include/SDL_image.h" "$PREFIX/include/SDL2/"
fi
cd ..
echo "==> SDL2_image ready under $PREFIX"
ls -la "$PREFIX/lib/libSDL2_image.a" 2>/dev/null || true
