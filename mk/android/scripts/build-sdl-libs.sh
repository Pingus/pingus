#!/usr/bin/env bash
# Builds SDL3 (shared) and SDL3_image (static) via ndk-build, plus the
# SDLActivity Java glue (javac/d8).
#
# SDL3_image uses its built-in stb_image (PNG/JPEG loading) and miniz
# (IMG_SavePNG) code, so it needs no external image libraries.
#
# Required environment:
#   ANDROID_HOME, BUILD_TOOLS_VERSION, COMPILE_PLATFORM, PACKAGE_PLATFORM
#   SDL_SRC            - SDL3 source tree
#   SDL_IMAGE_SRC      - SDL3_image source tree
#   APPLICATION_MK, TOP_ANDROID_MK
set -euo pipefail


# Resolve NDK root: ndk-bundle (legacy) or ndk/<version> (current SDK layout).
resolve_ndk() {
  if [ -n "${ANDROID_NDK_HOME:-}" ] && [ -x "${ANDROID_NDK_HOME}/ndk-build" ]; then
    printf '%s' "$ANDROID_NDK_HOME"
    return
  fi
  if [ -z "${ANDROID_HOME:-}" ]; then
    echo "error: ANDROID_HOME is not set" >&2
    exit 1
  fi
  if [ -x "$ANDROID_HOME/ndk-bundle/ndk-build" ]; then
    printf '%s' "$ANDROID_HOME/ndk-bundle"
    return
  fi
  if [ -d "$ANDROID_HOME/ndk" ]; then
    # Prefer ANDROID_NDK_VERSION when set; else newest directory that has ndk-build.
    if [ -n "${ANDROID_NDK_VERSION:-}" ] && [ -x "$ANDROID_HOME/ndk/$ANDROID_NDK_VERSION/ndk-build" ]; then
      printf '%s' "$ANDROID_HOME/ndk/$ANDROID_NDK_VERSION"
      return
    fi
    newest=
    for d in "$ANDROID_HOME/ndk"/*; do
      [ -x "$d/ndk-build" ] || continue
      newest=$d
    done
    if [ -n "$newest" ]; then
      printf '%s' "$newest"
      return
    fi
  fi
  echo "error: no ndk-build under ANDROID_HOME=$ANDROID_HOME (tried ndk-bundle and ndk/*)" >&2
  exit 1
}

NDK="$(resolve_ndk)"
echo "==> NDK=$NDK"
BT="$ANDROID_HOME/build-tools/$BUILD_TOOLS_VERSION"
COMPILE_JAR="$ANDROID_HOME/platforms/android-$COMPILE_PLATFORM/android.jar"

# One ndk-build tree: the top Android.mk includes SDL/ and SDL_image/.
# SDL3_image is built as a static library and linked into libmain:
# SDLActivity only System.loadLibrary()s SDL3 and main, and before API 23
# the loader does not resolve a libSDL3_image.so DT_NEEDED from the APK.
mkdir -p sdl-jni
cp -r "$SDL_SRC" sdl-jni/SDL
cp -r "$SDL_IMAGE_SRC" sdl-jni/SDL_image
chmod -R u+rwX sdl-jni
cp "$APPLICATION_MK" sdl-jni/Application.mk
cp "$TOP_ANDROID_MK" sdl-jni/Android.mk

"$NDK/ndk-build" \
  NDK_PROJECT_PATH="$PWD/sdl-jni" \
  APP_BUILD_SCRIPT="$PWD/sdl-jni/Android.mk" \
  NDK_APPLICATION_MK="$PWD/sdl-jni/Application.mk" \
  APP_MODULES="SDL3 SDL3_image_static" \
  -j"${NIX_BUILD_CORES:-$(nproc)}"

mkdir -p javasrc classes
cp -r sdl-jni/SDL/android-project/app/src/main/java/org javasrc/org
chmod -R u+w javasrc
javac -encoding UTF-8 --release 8 -nowarn -classpath "$COMPILE_JAR" -d classes \
  $(find javasrc -name '*.java')
"$BT/d8" --output classes --min-api "$PACKAGE_PLATFORM" --lib "$COMPILE_JAR" \
  $(find classes -name '*.class')
