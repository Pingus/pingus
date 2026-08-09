#!/usr/bin/env bash
# Builds SDL2 (and optionally SDL2_mixer) native libraries via ndk-build,
# plus SDLActivity Java glue (javac/d8).
#
# Required environment:
#   ANDROID_HOME, BUILD_TOOLS_VERSION, COMPILE_PLATFORM, PACKAGE_PLATFORM
#   SDL_SRC            - SDL2 source tree
#   SDL_MIXER_SRC      - optional SDL2_mixer source tree
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

mkdir -p sdl-jni
cp -r "$SDL_SRC" sdl-jni/SDL
chmod -R u+w sdl-jni
cp "$APPLICATION_MK" sdl-jni/Application.mk
cp "$TOP_ANDROID_MK" sdl-jni/Android.mk

# Builds just SDL2 itself (module "SDL2").
"$NDK/ndk-build" \
  NDK_PROJECT_PATH="$PWD/sdl-jni" \
  APP_BUILD_SCRIPT="$PWD/sdl-jni/Android.mk" \
  NDK_APPLICATION_MK="$PWD/sdl-jni/Application.mk" \
  -j"$NIX_BUILD_CORES"

# Optional: SDL2_mixer linked against the SDL2 we just built.
# Disable heavy optional codecs that pull missing external modules on Android.
if [ -n "${SDL_MIXER_SRC:-}" ] && [ -d "$SDL_MIXER_SRC" ]; then
  echo "Building SDL2_mixer from $SDL_MIXER_SRC"
  mkdir -p mixer-jni/SDL mixer-jni/SDL2_mixer
  cp "$APPLICATION_MK" mixer-jni/Application.mk
  cp "$TOP_ANDROID_MK" mixer-jni/Android.mk

  # Prebuilt SDL2 for the mixer link step.
  cat > mixer-jni/SDL/Android.mk <<EOF
LOCAL_PATH := \$(call my-dir)
include \$(CLEAR_VARS)
LOCAL_MODULE := SDL2
LOCAL_SRC_FILES := $PWD/sdl-jni/libs/\$(TARGET_ARCH_ABI)/libSDL2.so
LOCAL_EXPORT_C_INCLUDES := $PWD/sdl-jni/SDL/include
include \$(PREBUILT_SHARED_LIBRARY)
EOF

  cp -a "$SDL_MIXER_SRC"/. mixer-jni/SDL2_mixer/
  chmod -R u+w mixer-jni

  # Optional: libxmp for module music under external/libxmp (Pingus uses ModPlug on desktop/wasm).
  MOD_XMP=false
  if [ -n "${LIBXMP_SRC:-}" ] && [ -d "$LIBXMP_SRC" ]; then
    echo "Vendoring libxmp into SDL2_mixer/external/libxmp"
    mkdir -p mixer-jni/SDL2_mixer/external/libxmp
    cp -a "$LIBXMP_SRC"/. mixer-jni/SDL2_mixer/external/libxmp/
    chmod -R u+w mixer-jni/SDL2_mixer/external/libxmp
    # CORE_PLAYER is enough for Pingus .mod/.xm. Only compile the core
    # player sources + the four core format loaders (MOD/S3M/XM/IT). A full
    # loaders/*.c wildcard pulls in 669/MED/FAR/… which reference FX_* enums
    # that LIBXMP_CORE_PLAYER deliberately omits → undeclared-identifier
    # errors (e.g. FX_669_PORTA_UP in 669_load.c).
    echo "Writing Android.mk for libxmp CORE_PLAYER (MOD/S3M/XM/IT)"
    cat > mixer-jni/SDL2_mixer/external/libxmp/Android.mk <<'XMPMK'
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := xmp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/src $(LOCAL_PATH)/src/loaders
LOCAL_CFLAGS += -DLIBXMP_CORE_PLAYER -DLIBXMP_NO_PROWIZARD -DLIBXMP_NO_DEPACKERS -DHAVE_ROUND
# Core player (src/*.c minus format-specific extras). Keep lfo.c — player,
# effects, and read_event call libxmp_lfo_* even under CORE_PLAYER.
XMP_SRC := $(wildcard $(LOCAL_PATH)/src/*.c)
XMP_SRC := $(filter-out %/extras.c %/far_extras.c %/med_extras.c %/hmn_extras.c,$(XMP_SRC))
# Core loaders only — matches libxmp's LIBXMP_CORE_PLAYER set.
XMP_LOADERS := \
  src/loaders/common.c \
  src/loaders/iff.c \
  src/loaders/it_load.c \
  src/loaders/itsex.c \
  src/loaders/mod_load.c \
  src/loaders/s3m_load.c \
  src/loaders/sample.c \
  src/loaders/xm_load.c
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(XMP_SRC)) $(XMP_LOADERS)
include $(BUILD_STATIC_LIBRARY)
XMPMK
    MOD_XMP=true
  else
    echo "warning: LIBXMP_SRC unset — MOD/XM music will not load" >&2
  fi

  # Prefer in-tree stb_vorbis OGG; disable codecs that need missing deps.
  "$NDK/ndk-build" \
    NDK_PROJECT_PATH="$PWD/mixer-jni" \
    APP_BUILD_SCRIPT="$PWD/mixer-jni/Android.mk" \
    NDK_APPLICATION_MK="$PWD/mixer-jni/Application.mk" \
    SUPPORT_WAVPACK=false \
    SUPPORT_GME=false \
    SUPPORT_MOD_XMP=$MOD_XMP \
    SUPPORT_OGG_STB=true \
    SUPPORT_OGG=false \
    -j"$NIX_BUILD_CORES"

  for abi_dir in mixer-jni/libs/*; do
    [ -d "$abi_dir" ] || continue
    abi=$(basename "$abi_dir")
    mkdir -p "sdl-jni/libs/$abi"
    cp -v "$abi_dir"/libSDL2_mixer.so "sdl-jni/libs/$abi/"
  done
  mkdir -p sdl-jni/SDL2_mixer_include
  if [ -d mixer-jni/SDL2_mixer/include ]; then
    cp -a mixer-jni/SDL2_mixer/include/. sdl-jni/SDL2_mixer_include/
  fi
fi

mkdir -p javasrc
cp -r sdl-jni/SDL/android-project/app/src/main/java/org javasrc/org
chmod -R u+w javasrc

mkdir -p classes
javac -encoding UTF-8 --release 8 -classpath "$COMPILE_JAR" -d classes \
  $(find javasrc -name '*.java')
"$BT/d8" --output classes --min-api "$PACKAGE_PLATFORM" $(find classes -name '*.class')
