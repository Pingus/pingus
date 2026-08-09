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
PREBUILT="$(ls -d "$NDK_ROOT"/toolchains/llvm/prebuilt/* 2>/dev/null | head -1)"
if [ -z "$PREBUILT" ] || [ ! -d "$PREBUILT/sysroot" ]; then
  echo "error: NDK llvm prebuilt/sysroot not found under $NDK_ROOT" >&2
  exit 1
fi
SYSROOT="$PREBUILT/sysroot"
OPENSL_INC="$SYSROOT/usr/include"
if [ ! -f "$OPENSL_INC/SLES/OpenSLES.h" ]; then
  echo "error: OpenSLES.h not under $OPENSL_INC/SLES" >&2
  exit 1
fi

echo "==> NDK: $NDK_ROOT"
echo "==> sysroot: $SYSROOT"
echo "==> ABIs: $TARGET_ABIS"
mkdir -p "$OUT_DIR"

for abi in $TARGET_ABIS; do
  echo "==> OpenAL Soft ($abi)"
  case "$abi" in
    armeabi-v7a) opensl_triple="arm-linux-androideabi" ;;
    arm64-v8a)   opensl_triple="aarch64-linux-android" ;;
    x86)         opensl_triple="i686-linux-android" ;;
    x86_64)      opensl_triple="x86_64-linux-android" ;;
    *) echo "error: unknown ABI $abi" >&2; exit 1 ;;
  esac
  # Prefer API-level lib dir; fall back to generic.
  OPENSL_LIB=""
  for candidate in \
      "$SYSROOT/usr/lib/$opensl_triple/${PACKAGE_PLATFORM}/libOpenSLES.so" \
      "$SYSROOT/usr/lib/$opensl_triple/libOpenSLES.so" \
      "$PREBUILT/sysroot/usr/lib/$opensl_triple/libOpenSLES.so"
  do
    if [ -f "$candidate" ]; then OPENSL_LIB="$candidate"; break; fi
  done
  if [ -z "$OPENSL_LIB" ]; then
    echo "error: libOpenSLES.so not found for $abi" >&2
    find "$SYSROOT/usr/lib" -name 'libOpenSLES.so' 2>/dev/null | head -10 >&2 || true
    exit 1
  fi
  echo "    OpenSL: $OPENSL_LIB"

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
    -DOPENSL_INCLUDE_DIR="$OPENSL_INC" \
    -DOPENSL_ANDROID_INCLUDE_DIR="$OPENSL_INC" \
    -DOPENSL_LIBRARY="$OPENSL_LIB" \
    -DALSOFT_UTILS=OFF \
    -DALSOFT_EXAMPLES=OFF \
    -DALSOFT_TESTS=OFF \
    -DALSOFT_INSTALL=ON \
    -DALSOFT_EMBED_HRTF_DATA=ON \
    -DALSOFT_BACKEND_OPENSL=ON \
    -DALSOFT_REQUIRE_OPENSL=ON \
    -DALSOFT_BACKEND_WAVE=OFF \
    -DALSOFT_BACKEND_NULL=OFF \
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
