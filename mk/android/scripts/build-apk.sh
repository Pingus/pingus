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

NDK="$ANDROID_HOME/ndk-bundle"
BT="$ANDROID_HOME/build-tools/$BUILD_TOOLS_VERSION"
PACKAGE_JAR="$ANDROID_HOME/platforms/android-$PACKAGE_PLATFORM/android.jar"

if [ -z "${GAME_SRC_DIR:-}" ] || [ ! -d "$GAME_SRC_DIR" ]; then
  echo "error: GAME_SRC_DIR must point at the game C++ source tree" >&2
  exit 1
fi

if [ -z "${GAME_DATA_DIR:-}" ] || [ ! -d "$GAME_DATA_DIR" ]; then
  echo "error: GAME_DATA_DIR must point at the Milestone 1 data/ tree" >&2
  echo "       (expected images/, levels/, etc. under that path)" >&2
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

# Stage public headers from repo external/ (sources still need linking — TODO).
REPO_ROOT="$(cd "$GAME_SRC_DIR/.." && pwd)"
if [ -d "$REPO_ROOT/external" ]; then
  mkdir -p src/jni/external_includes
  for name in argpp geomcpp logmich priocpp strutcpp sexpcpp tinygettext uitest wstsound xdgcpp; do
    inc="$REPO_ROOT/external/$name/include"
    if [ -d "$inc" ]; then
      cp -a "$inc"/. src/jni/external_includes/
    fi
  done
  echo "==> staged external/ headers into jni/external_includes"
fi

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
# Probe a well-known path used by the game at startup.
if [ ! -f src/assets/images/status/letters-white.png ]; then
  echo "error: missing src/assets/images/status/letters-white.png" >&2
  echo "       is GAME_DATA_DIR a full Milestone 1 data/ tree?" >&2
  exit 1
fi

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
