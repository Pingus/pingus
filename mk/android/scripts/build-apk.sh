#!/usr/bin/env bash
# Builds the Pingus APK, linking SDL2 as a prebuilt library.
# Required environment:
#   ANDROID_HOME, BUILD_TOOLS_VERSION, PACKAGE_PLATFORM, TARGET_ABIS
#   APP_NAME, APP_DIR          - android/ packaging dir (manifest, res, jni/)
#   GAME_SRC_DIR               - path to C++ sources (repo src/)
#   GAME_DATA_DIR              - required data/ tree packaged as assets
#   APPLICATION_MK, TOP_ANDROID_MK, SDL_PREBUILT_MK, SDL_ANDROID_LIBS
#   KEYSTORE, STB_IMAGE_H
#   PINGUS_VERSION           - full version string (e.g. 0.8.0-dev+gabc1234)
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
PACKAGE_JAR="$ANDROID_HOME/platforms/android-$PACKAGE_PLATFORM/android.jar"

if [ -z "${GAME_SRC_DIR:-}" ] || [ ! -d "$GAME_SRC_DIR" ]; then
  echo "error: GAME_SRC_DIR must point at the game C++ source tree" >&2
  exit 1
fi

if [ -z "${GAME_DATA_DIR:-}" ] || [ ! -d "$GAME_DATA_DIR" ]; then
  echo "error: GAME_DATA_DIR must point at the Pingus data/ tree" >&2
  echo "       (expected images/, levels/, levelsets/, music/, … under that path)" >&2
  exit 1
fi

mkdir -p src/jni/src src/jni/SDL
cp "$APPLICATION_MK" src/jni/Application.mk
cp "$TOP_ANDROID_MK" src/jni/Android.mk
cp "$APP_DIR/jni/Android.mk" src/jni/src/Android.mk
cp "$APP_DIR/AndroidManifest.xml" src/AndroidManifest.xml
cp -r "$APP_DIR/res" src/res

# Game C++ sources next to the module Android.mk.
cp -r "$GAME_SRC_DIR"/. src/jni/src/
chmod -R u+rwX src/jni/src

# Stage monorepo external/ headers + sources.
# Under Nix, GAME_SRC_DIR is a filtered ./src store path — parent is NOT the
# repo. Pass GAME_EXTERNAL_DIR (flake: ./external) and optional GLM_INCLUDE_DIR.
EXTERNAL_DIR="${GAME_EXTERNAL_DIR:-}"
if [ -z "$EXTERNAL_DIR" ]; then
  REPO_ROOT="$(cd "$GAME_SRC_DIR/.." && pwd)"
  if [ -d "$REPO_ROOT/external" ]; then
    EXTERNAL_DIR="$REPO_ROOT/external"
  fi
fi
if [ -z "$EXTERNAL_DIR" ] || [ ! -d "$EXTERNAL_DIR" ]; then
  echo "error: GAME_EXTERNAL_DIR must point at the repo external/ tree" >&2
  echo "       (contains geomcpp/, priocpp/, sexpcpp/, logmich/, …)" >&2
  exit 1
fi

mkdir -p src/jni/external_includes
# Nix store trees are often 0555/0444. cp -a preserves that and the next
# package cannot create e.g. external_includes/geom → Permission denied.
# Copy then force owner-writable on the staging tree.
# Header-only / public includes (layout: include/<ns>/… → external_includes/<ns>/…)
for name in argpp geomcpp logmich priocpp strutcpp sexpcpp tinygettext; do
  inc="$EXTERNAL_DIR/$name/include"
  if [ -d "$inc" ]; then
    cp -a "$inc"/. src/jni/external_includes/
    chmod -R u+rwX src/jni/external_includes
  else
    echo "error: missing $inc" >&2
    exit 1
  fi
done
# glm is header-only (geom depends on it).
if [ -n "${GLM_INCLUDE_DIR:-}" ] && [ -d "$GLM_INCLUDE_DIR" ]; then
  # Expect GLM_INCLUDE_DIR to contain glm/… (nixpkgs glm) or be the glm/ dir itself.
  if [ -d "$GLM_INCLUDE_DIR/glm" ]; then
    cp -a "$GLM_INCLUDE_DIR/glm" src/jni/external_includes/
  elif [ "$(basename "$GLM_INCLUDE_DIR")" = "glm" ]; then
    cp -a "$GLM_INCLUDE_DIR" src/jni/external_includes/
  else
    echo "error: GLM_INCLUDE_DIR=$GLM_INCLUDE_DIR does not look like glm headers" >&2
    exit 1
  fi
  chmod -R u+rwX src/jni/external_includes
  echo "==> staged glm headers from $GLM_INCLUDE_DIR"
else
  echo "error: GLM_INCLUDE_DIR is required for Android (geom → glm)" >&2
  exit 1
fi
echo "==> staged external headers into jni/external_includes"

# Compile external .cpp into libmain (ndk-build RWILDCARD under jni/src/).
# Skip tests/benchmarks; skip priocpp JSON (no jsoncpp on Android).
mkdir -p src/jni/src/deps
stage_lib_src() {
  local name="$1"
  local srcdir="$EXTERNAL_DIR/$name/src"
  if [ ! -d "$srcdir" ]; then
    echo "warning: no sources for $name ($srcdir)" >&2
    return 0
  fi
  mkdir -p "src/jni/src/deps/$name"
  # top-level sources + private headers (float.hpp, prettyprinter.hpp, …)
  find "$srcdir" -maxdepth 1 -name '*.cpp' -exec cp -a {} "src/jni/src/deps/$name/" \;
  find "$srcdir" -maxdepth 1 \( -name '*.hpp' -o -name '*.h' \) -exec cp -a {} "src/jni/src/deps/$name/" \;
  chmod -R u+rwX "src/jni/src/deps/$name"
}
stage_lib_src argpp
stage_lib_src logmich
stage_lib_src sexpcpp
stage_lib_src strutcpp
stage_lib_src priocpp
stage_lib_src tinygettext
# Drop JSON backends (PRIO_USE_JSONCPP is off).
rm -f src/jni/src/deps/priocpp/json_*.cpp \
      src/jni/src/deps/priocpp/jsonpretty_*.cpp
# strut layout.cpp needs a missing polygon.hpp; Pingus does not use Layout.
rm -f src/jni/src/deps/strutcpp/layout.cpp
echo "==> staged external sources into jni/src/deps/"

# Minimal sigc++ headers (Pingus Android polyfill — full libsigc++ not required).
mkdir -p src/jni/external_includes/sigc++
cp -a "$APP_DIR/jni/sigc++/." src/jni/external_includes/sigc++/
chmod -R u+rwX src/jni/external_includes/sigc++
echo "==> staged Android sigc++ polyfill"

# IMG_* shim + headers.
cp "$APP_DIR/jni/img_stb_min.c" src/jni/src/img_stb_min.c
cp "$APP_DIR/jni/SDL_image.h" src/jni/src/SDL_image.h
if [ -n "${STB_IMAGE_H:-}" ] && [ -f "$STB_IMAGE_H" ]; then
  cp "$STB_IMAGE_H" src/jni/src/stb_image.h
elif [ -f "$APP_DIR/jni/stb_image.h" ]; then
  cp "$APP_DIR/jni/stb_image.h" src/jni/src/stb_image.h
else
  echo "error: need STB_IMAGE_H or android/jni/stb_image.h (upstream stb)" >&2
  exit 1
fi

cp "$SDL_PREBUILT_MK" src/jni/SDL/Android.mk
cp -r "$SDL_ANDROID_LIBS/include" src/jni/SDL/include
# SDL2_mixer headers (SDL_mixer.h) live next to SDL headers when present.
if [ -f src/jni/SDL/include/SDL_mixer.h ] || [ -f src/jni/SDL/include/SDL2/SDL_mixer.h ]; then
  echo "SDL2_mixer headers present"
else
  echo "warning: SDL_mixer.h not found under SDL include — sound build may fail" >&2
fi

# Game data → APK assets/ (AssetManager root).
mkdir -p src/assets
cp -a "$GAME_DATA_DIR"/. src/assets/
# Nix store files are often 0444; aapt/zip need readable tree we can scan.
chmod -R u+rwX src

ASSET_COUNT=$(find src/assets -type f | wc -l)
ASSET_SIZE=$(du -sh src/assets | awk '{print $1}')
echo "Packaging $ASSET_COUNT asset files ($ASSET_SIZE) from $GAME_DATA_DIR"
if [ "$ASSET_COUNT" -lt 10 ]; then
  echo "error: asset tree looks empty (found $ASSET_COUNT files)" >&2
  ls -la src/assets >&2 || true
  exit 1
fi
# Probe well-known Pingus data paths (not SuperTux Milestone 1 leftovers).
for probe in   images/fonts/chalk-40px.font   images/fonts/chalk-40px.png   levelsets   levels   music   sounds
do
  if [ ! -e "src/assets/$probe" ]; then
    echo "error: missing src/assets/$probe" >&2
    echo "       is GAME_DATA_DIR a full Pingus data/ tree?" >&2
    exit 1
  fi
done

cp "$KEYSTORE" debug.keystore

# Bake VERSION+g<rev> into PINGUS_VERSION (see jni/Android.mk).
PINGUS_VERSION="${PINGUS_VERSION:-0.8.0-dev}"
echo "==> PINGUS_VERSION=$PINGUS_VERSION"

"$NDK/ndk-build" \
  NDK_PROJECT_PATH="$PWD/src" \
  APP_BUILD_SCRIPT="$PWD/src/jni/Android.mk" \
  NDK_APPLICATION_MK="$PWD/src/jni/Application.mk" \
  PINGUS_VERSION="$PINGUS_VERSION" \
  -j"${NIX_BUILD_CORES:-${JOBS:-$(nproc)}}"

mkdir -p out

# Package resources + manifest. Assets are added via zip below: old aapt's
# -A path has been unreliable with large trees in this pipeline.
"$BT/aapt" package -f \
  -M src/AndroidManifest.xml \
  -S src/res \
  -I "$PACKAGE_JAR" \
  -F out/base.apk

cp "$SDL_ANDROID_LIBS/dex/classes.dex" out/classes.dex
for abi in $TARGET_ABIS; do
  mkdir -p out/lib/"$abi"
  cp src/libs/"$abi"/*.so out/lib/"$abi"/
done

( cd out && "$BT/aapt" add base.apk classes.dex )
( cd out && zip -r base.apk lib )

# Inject assets/ into the APK (same layout AssetManager expects).
# Run from src/ so paths inside the zip are assets/...
( cd src && zip -r -9 ../out/base.apk assets )
echo "APK contents (assets sample):"
#unzip -l out/base.apk | grep -E 'assets/(images|levels)/' | head -20
#ASSET_IN_APK=$(unzip -l out/base.apk | grep -c ' assets/' || true)
#echo "Asset entries in APK: $ASSET_IN_APK"
#if [ "${ASSET_IN_APK:-0}" -lt 10 ]; then
#  echo "error: APK still has almost no assets after zip inject" >&2
#  exit 1
#fi

"$BT/zipalign" -f 4 out/base.apk out/aligned.apk

"$BT/apksigner" sign \
  --ks debug.keystore --ks-pass pass:android --key-pass pass:android \
  --out "out/$APP_NAME.apk" out/aligned.apk

echo "Final APK size: $(du -h "out/$APP_NAME.apk" | awk '{print $1}')"
"$BT/aapt" dump badging "out/$APP_NAME.apk"
