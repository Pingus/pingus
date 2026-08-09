#!/usr/bin/env bash
# Cross-build OpenAL Soft + libmodplug for each TARGET_ABI using the NDK.
# Required env:
#   ANDROID_HOME, PACKAGE_PLATFORM, TARGET_ABIS
#   OPENAL_SRC, MODPLUG_SRC   - source trees
# Optional:
#   OUT_DIR (default: ./audio-out)
set -euo pipefail

: "${ANDROID_HOME:?}"
: "${PACKAGE_PLATFORM:?}"
: "${TARGET_ABIS:?}"
: "${OPENAL_SRC:?}"
: "${MODPLUG_SRC:?}"

OUT_DIR="${OUT_DIR:-$PWD/audio-out}"
NDK_ROOT="$(ls -d "$ANDROID_HOME"/ndk/* 2>/dev/null | sort -V | tail -1)"
if [ -z "$NDK_ROOT" ] || [ ! -d "$NDK_ROOT" ]; then
  echo "error: no NDK under $ANDROID_HOME/ndk" >&2
  exit 1
fi
TOOLCHAIN="$NDK_ROOT/build/cmake/android.toolchain.cmake"
if [ ! -f "$TOOLCHAIN" ]; then
  echo "error: missing $TOOLCHAIN" >&2
  exit 1
fi

echo "==> NDK: $NDK_ROOT"
echo "==> ABIs: $TARGET_ABIS"
mkdir -p "$OUT_DIR"

for abi in $TARGET_ABIS; do
  echo "==> OpenAL Soft ($abi)"
  bdir="$OUT_DIR/build-openal-$abi"
  idir="$OUT_DIR/$abi"
  mkdir -p "$idir"
  cmake -S "$OPENAL_SRC" -B "$bdir" \
    -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
    -DANDROID_ABI="$abi" \
    -DANDROID_PLATFORM="android-${PACKAGE_PLATFORM}" \
    -DANDROID_STL=c++_shared \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$idir" \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DALSOFT_UTILS=OFF \
    -DALSOFT_EXAMPLES=OFF \
    -DALSOFT_TESTS=OFF \
    -DALSOFT_INSTALL=ON \
    -DALSOFT_EMBED_HRTF_DATA=ON \
    -DLIBTYPE=STATIC
  cmake --build "$bdir" -j"${NIX_BUILD_CORES:-$(nproc)}"
  cmake --install "$bdir"

  echo "==> libmodplug ($abi)"
  mbdir="$OUT_DIR/build-modplug-$abi"
  mkdir -p "$mbdir"
  # Android libc provides setenv(); tell load_abc.cpp not to polyfill it.
  # (Keeps ReadABC so sndfile.cpp links.)
  # In-tree CMake for a static lib (autotools is awkward under NDK).
  cat > "$mbdir/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.15)
project(modplug C CXX)
file(GLOB MODPLUG_SRC_FILES "${MODPLUG_SRC}/src/*.cpp" "${MODPLUG_SRC}/src/*.c")
add_library(modplug STATIC \${MODPLUG_SRC_FILES})
target_include_directories(modplug PUBLIC "${MODPLUG_SRC}/src" "${MODPLUG_SRC}/src/libmodplug")
target_compile_definitions(modplug PRIVATE MODPLUG_STATIC HAVE_STDINT_H HAVE_SINF HAVE_SETENV)
# libmodplug 0.8.9 still uses the C++ `register` keyword; NDK defaults to
# C++17 where that is an error (same fix as wasm emconfigure CXXFLAGS).
set_target_properties(modplug PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)
target_compile_options(modplug PRIVATE -std=gnu++14 -Wno-register -Wno-deprecated-register)
install(TARGETS modplug ARCHIVE DESTINATION lib)
install(DIRECTORY "${MODPLUG_SRC}/src/libmodplug/" DESTINATION include/libmodplug FILES_MATCHING PATTERN "*.h")
if(EXISTS "${MODPLUG_SRC}/src/modplug.h")
  install(FILES "${MODPLUG_SRC}/src/modplug.h" DESTINATION include)
endif()
EOF
  cmake -S "$mbdir" -B "$mbdir/build" \
    -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
    -DANDROID_ABI="$abi" \
    -DANDROID_PLATFORM="android-${PACKAGE_PLATFORM}" \
    -DANDROID_STL=c++_shared \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DCMAKE_CXX_FLAGS="-std=gnu++14 -Wno-register -Wno-deprecated-register" \
    -DCMAKE_INSTALL_PREFIX="$idir"
  cmake --build "$mbdir/build" -j"${NIX_BUILD_CORES:-$(nproc)}"
  cmake --install "$mbdir/build"
  # Public API is <libmodplug/modplug.h>; copy if install put it at include root only.
  if [ -f "$idir/include/modplug.h" ] && [ ! -f "$idir/include/libmodplug/modplug.h" ]; then
    mkdir -p "$idir/include/libmodplug"
    cp -a "$idir/include/modplug.h" "$idir/include/libmodplug/modplug.h"
  fi
  if [ -f "$MODPLUG_SRC/src/libmodplug/modplug.h" ]; then
    mkdir -p "$idir/include/libmodplug"
    cp -a "$MODPLUG_SRC/src/libmodplug/modplug.h" "$idir/include/libmodplug/modplug.h"
  fi
done

echo "==> audio libs installed under $OUT_DIR/{abi}/"
find "$OUT_DIR" -name 'libopenal*' -o -name 'libmodplug*' | head -40
