#!/usr/bin/env bash
# Builds static zlib for wasm32 via emconfigure/emmake.
# Expects (set by nix/wasm.nix):
#   ZLIB_SRC  - path to zlib source tree or tarball (pkgs.zlib.src)
#
# Installs into $PWD/prefix (lib/libz.a + include/zlib.h).
# Offline — does not use Emscripten's -sUSE_ZLIB network port.
set -euo pipefail

export EM_CACHE="${TMPDIR:-/tmp}/emcache"
mkdir -p "$EM_CACHE"

PREFIX="$PWD/prefix"
mkdir -p "$PREFIX"

if [ -z "${ZLIB_SRC:-}" ]; then
  echo "error: ZLIB_SRC is not set" >&2
  exit 1
fi

echo "==> zlib static (wasm32) → $PREFIX"
rm -rf zlib-src
if [ -d "$ZLIB_SRC" ]; then
  cp -a "$ZLIB_SRC" zlib-src
else
  mkdir -p zlib-src
  tar -xf "$ZLIB_SRC" -C zlib-src --strip-components=1
fi
chmod -R u+w zlib-src

# Prefer classic configure: zlib's CMake still add_library(SHARED), which
# Emscripten rejects even with -DBUILD_SHARED_LIBS=OFF.
(
  cd zlib-src
  if [ -x ./configure ] || [ -f ./configure ]; then
    emconfigure ./configure --static --prefix="$PREFIX"
    emmake make -j"${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"
    emmake make install
  else
    echo "zlib: no configure — compiling sources with emcc"
    mkdir -p "$PREFIX/lib" "$PREFIX/include"
    objs=()
    for c in adler32 compress crc32 deflate gzclose gzlib gzread gzwrite \
             infback inffast inflate inftrees trees uncompr zutil; do
      if [ -f "${c}.c" ]; then
        emcc -O2 -c "${c}.c" -o "${c}.o"
        objs+=("${c}.o")
      fi
    done
    emar rcs "$PREFIX/lib/libz.a" "${objs[@]}"
    cp zlib.h zconf.h "$PREFIX/include/" 2>/dev/null || cp zlib.h "$PREFIX/include/"
  fi
)

if [ ! -f "$PREFIX/lib/libz.a" ]; then
  echo "error: libz.a was not installed to $PREFIX/lib" >&2
  exit 1
fi
if [ ! -f "$PREFIX/include/zlib.h" ]; then
  echo "error: zlib.h was not installed to $PREFIX/include" >&2
  exit 1
fi

echo "==> wasm zlib build finished"
ls -la "$PREFIX/lib" "$PREFIX/include"
