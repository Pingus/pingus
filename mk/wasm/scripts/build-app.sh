#!/usr/bin/env bash
# Cross-compiles Pingus to wasm32 + HTML via emcmake.
# Expects (set by nix/wasm.nix):
#   APP_NAME         - binary/html basename (e.g. pingus)
#   SRC_DIR          - repo root (contains CMakeLists.txt + src/)
#   SDL_WASM_LIBS    - prebuilt SDL2 (+ image) prefix (include/ + lib/)
#   ZLIB_WASM_LIBS   - prebuilt static zlib prefix (libz.a + zlib.h)
#   DATA_DIR         - optional path to data/ for --preload-file (may be absent)
#   ENABLE_SOUND     - 0|1 (default 0; ON → wstsound via Emscripten OpenAL + libmodplug)
#   ENABLE_GLES2     - 0|1 (default 1 — WebGL via GLES2 path)
#   CMAKE_VERBOSE    - if 1, pass --verbose to cmake --build
#   PROJECT_VERSION_FULL - e.g. 0.1.5-dev+gabc1234 (CMake PINGUS_VERSION)
#   GIT_REV / SOURCE_URL - stamped into the HTML shell footer when present
set -euo pipefail

export EM_CACHE="${TMPDIR:-/tmp}/emcache"
mkdir -p "$EM_CACHE"

# Static zlib comes from flake output wasm-zlib-libs (see nix/wasm.nix).
ZLIB_PREFIX="${ZLIB_WASM_LIBS:-}"
if [ -n "$ZLIB_PREFIX" ] && [ -f "$ZLIB_PREFIX/lib/libz.a" ]; then
  echo "==> using prebuilt wasm zlib: $ZLIB_PREFIX"
else
  echo "WARNING: ZLIB_WASM_LIBS unset or incomplete — configure may fail on zlib.h"
  ZLIB_PREFIX=""
fi

APP_NAME="${APP_NAME:-pingus}"
ENABLE_SOUND="${ENABLE_SOUND:-0}"
ENABLE_GLES2="${ENABLE_GLES2:-1}"

# ASYNCIFY: optional safety net for residual nested waits (fade/wait_for_event).
# Main path uses emscripten_set_main_loop + st_frame_delay() no-ops.
# Default OFF (ENABLE_ASYNCIFY=0); set enableAsyncify = true in mkApp if needed.
LINK_FLAGS=(
  "SHELL:-sALLOW_MEMORY_GROWTH=1"
  # Emscripten 6.x defaults GROWABLE_ARRAYBUFFERS=1 so WASM memory is a resizable
  # ArrayBuffer. TextDecoder.decode() rejects resizable buffers (black screen at
  # startup via UTF8ArrayToString / faccessat). 6.0.2's getUnsharedTextDecoderView
  # only handles SharedArrayBuffer, not resizable. Disable growable buffers so
  # growth uses the classic detach-and-replace model (non-resizable AB).
  # TEXTDECODER=0 is no longer allowed (must be 1 or 2).
  "SHELL:-sGROWABLE_ARRAYBUFFERS=0"
  # Fixed-function OpenGL client (glOrtho, SDL_opengl.h) via Emscripten emulation.
  "SHELL:-sFULL_ES2=1"
  "SHELL:-sMIN_WEBGL_VERSION=1"
  "SHELL:-sMAX_WEBGL_VERSION=2"
  "SHELL:-sFORCE_FILESYSTEM=1"
  "SHELL:-sEXIT_RUNTIME=0"
  "SHELL:-lidbfs.js"
  # Only symbols Pingus actually provides today (+ emscripten main-loop helpers).
  # SuperTux _st_emscripten_* hooks are optional in shell.html when missing.
  "SHELL:-sEXPORTED_FUNCTIONS=_main,_emscripten_pause_main_loop,_emscripten_resume_main_loop"
  "SHELL:-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS"
)
if [ "${ENABLE_ASYNCIFY:-0}" = 1 ]; then
  LINK_FLAGS+=("SHELL:-sASYNCIFY=1" "SHELL:-sASYNCIFY_STACK_SIZE=1048576")
  echo "==> ASYNCIFY enabled"
else
  echo "==> ASYNCIFY disabled (default; set enableAsyncify=true in mkApp if a path freezes)"
fi

if [ -n "${WASM_SHELL:-}" ] && [ -f "$WASM_SHELL" ]; then
  LINK_FLAGS+=("--shell-file" "$WASM_SHELL")
  echo "==> using HTML shell: $WASM_SHELL"
fi

PRELOAD=()
if [ -n "${DATA_DIR:-}" ] && [ -d "$DATA_DIR" ]; then
  # Mount game assets at /data in the virtual FS; runtime datadir = "/data".
  PRELOAD+=("--preload-file" "${DATA_DIR}@/data")
  echo "==> preloading data/ → /data"
else
  echo "==> no DATA_DIR — building without assets (title will fail at runtime)"
fi

PROJECT_VERSION_FULL="${PROJECT_VERSION_FULL:-}"
if [ -z "$PROJECT_VERSION_FULL" ] && [ -f "$SRC_DIR/VERSION" ]; then
  PROJECT_VERSION_FULL="$(head -1 "$SRC_DIR/VERSION" | tr -d '\r\n')"
fi
echo "==> PROJECT_VERSION_FULL=${PROJECT_VERSION_FULL:-"(unset)"}"

cmake_args=(
  -S "$SRC_DIR"
  -B build
  -DCMAKE_BUILD_TYPE=Release
  -DBUILD_TESTS=OFF
  -DBUILD_EXTRA=OFF
  -DWARNINGS=OFF
  -DWERROR=OFF
  -DPINGUS_USE_GLES="$( [ "$ENABLE_GLES2" = 1 ] && echo ON || echo OFF )"
  -DPRIO_USE_JSONCPP=OFF
  -DPINGUS_ENABLE_SOUND="$( [ "$ENABLE_SOUND" = 1 ] && echo ON || echo OFF )"
  -DDATA_PREFIX="/data"
  -DSDL2_ROOT="$SDL_WASM_LIBS"
  -DEMSCRIPTEN_LINK_FLAGS="${LINK_FLAGS[*]} ${PRELOAD[*]}"
)
# Prebuilt glm / libsigc++ (and friends) live outside the emscripten sysroot.
if [ -n "${EXTRA_PREFIX_PATH:-}" ]; then
  cmake_args+=(
    -DCMAKE_PREFIX_PATH="${EXTRA_PREFIX_PATH}"
    -DCMAKE_FIND_ROOT_PATH="${EXTRA_PREFIX_PATH};${SDL_WASM_LIBS}"
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH
  )
  echo "==> EXTRA_PREFIX_PATH=${EXTRA_PREFIX_PATH}"
fi
if [ -n "$PROJECT_VERSION_FULL" ]; then
  cmake_args+=(-DPROJECT_VERSION_FULL="$PROJECT_VERSION_FULL")
fi
if [ -n "$ZLIB_PREFIX" ] && [ -d "$ZLIB_PREFIX" ]; then
  cmake_args+=(-DZLIB_ROOT="$ZLIB_PREFIX")
fi
if [ "$ENABLE_SOUND" = 1 ]; then
  # Prebuilt libmodplug for the in-tree wstsound subdirectory build.
  # Explicit paths avoid emscripten FIND_ROOT stripping host-store prefixes.
  MODPLUG_PREFIX="${MODPLUG_WASM_LIBS:-}"
  if [ -n "$MODPLUG_PREFIX" ] && [ -d "$MODPLUG_PREFIX" ]; then
    cmake_args+=(
      -DMODPLUG_DIR="$MODPLUG_PREFIX"
      -DMODPLUG_INCLUDE_DIRECTORY="$MODPLUG_PREFIX/include"
      -DMODPLUG_LIBRARY="$MODPLUG_PREFIX/lib/libmodplug.a"
    )
    echo "==> MODPLUG_WASM_LIBS=$MODPLUG_PREFIX"
  else
    echo "WARNING: ENABLE_SOUND=1 but MODPLUG_WASM_LIBS unset/incomplete — FindModPlug may fail"
  fi
fi

echo "==> emcmake configure ${APP_NAME}"
emcmake cmake "${cmake_args[@]}"

verbose=()
if [ "${CMAKE_VERBOSE:-0}" = 1 ]; then
  verbose=(--verbose)
fi

echo "==> cmake --build"
cmake --build build --parallel "${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}" "${verbose[@]}"

# Emscripten names the outputs after the CMake target (pingus).
out_base="build/${APP_NAME}"
for ext in html js wasm data; do
  if [ -f "${out_base}.${ext}" ]; then
    cp "${out_base}.${ext}" .
  fi
done
# Older emscripten sometimes writes .js next to a non-suffixed binary.
if [ ! -f "${APP_NAME}.html" ] && [ -f "build/${APP_NAME}" ]; then
  # May already be an html shell from SUFFIX.
  ls -la build/ || true
fi

ls -la "${APP_NAME}".* 2>/dev/null || ls -la build/
echo "==> wasm app build finished"
