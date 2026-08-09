# Reusable Android/ndk-build pipeline for any SDL2 + C++ app. Nothing
# project-specific here beyond what's passed in — another project can
# import this directly with its own app directory, keystore, and
# platform/ABI choices.
#
# Usage:
#   android = import ./nix/android.nix {
#     inherit pkgs;
#     sdlSrc = <path to SDL2's extracted source tree>;   # e.g. a flake input
#     sdlVersion = "2.30.3";                              # just for derivation labeling
#     androidSdk = ...;            # androidenv.composeAndroidPackages { ... }.androidsdk
#     buildToolsVersion = "30.0.3";
#     packagePlatform = "22";      # baked into the APK's manifest/resources
#     compilePlatform = "33";      # javac classpath only, see comment below
#     targetAbis = [ "armeabi-v7a" "arm64-v8a" ];
#   };
#   android.mkApk { appName = "myapp"; appDir = ./apps/myapp; outApkName = "myapp.apk"; keystore = ./mk/android/keystore/debug.keystore; }
{ pkgs
, sdlSrc
, sdlVersion
, sdlMixerSrc ? null
, sdlMixerVersion ? "2.8.0"
, libxmpSrc ? null
, androidSdk
, buildToolsVersion
, packagePlatform
, compilePlatform
, targetAbis
}:

let
  targetAbisStr = pkgs.lib.concatStringsSep " " targetAbis;

  # Generic ndk-build entry point: just recurses into whatever subdirs
  # have their own Android.mk (SDL2's prebuilt one, and the app's own).
  topAndroidMk = pkgs.writeTextFile {
    name = "Android.mk";
    text = "include $(call all-subdir-makefiles)\n";
  };

  applicationMk = pkgs.writeTextFile {
    name = "Application.mk";
    text = ''
      APP_STL := c++_shared
      APP_ABI := ${targetAbisStr}
      APP_PLATFORM := android-${packagePlatform}
      # std::format / format_to / vformat (C++20). Needs a recent NDK libc++.
      APP_CPPFLAGS := -std=c++20 -D_LIBCPP_ENABLE_EXPERIMENTAL=1
    '';
  };

  # ---------------------------------------------------------------
  # SDL2 (+ optional SDL2_mixer) native libs and SDLActivity Java glue.
  # Built once and reused across app rebuilds.
  # ---------------------------------------------------------------
  sdlAndroidLibs = pkgs.stdenvNoCC.mkDerivation {
    pname = "sdl2-android-libs";
    version = if sdlMixerSrc != null then "${sdlVersion}+mixer-${sdlMixerVersion}" else sdlVersion;

    dontUnpack = true;
    nativeBuildInputs = [ androidSdk pkgs.jdk17 pkgs.gnumake ];

    env = {
      BUILD_TOOLS_VERSION = buildToolsVersion;
      COMPILE_PLATFORM = compilePlatform;
      PACKAGE_PLATFORM = packagePlatform;
      SDL_SRC = "${sdlSrc}";
      APPLICATION_MK = applicationMk;
      TOP_ANDROID_MK = topAndroidMk;
    } // pkgs.lib.optionalAttrs (sdlMixerSrc != null) {
      SDL_MIXER_SRC = "${sdlMixerSrc}";
    } // pkgs.lib.optionalAttrs (libxmpSrc != null) {
      LIBXMP_SRC = "${libxmpSrc}";
    };

    buildPhase = ''
      runHook preBuild
      export ANDROID_HOME=${androidSdk}/libexec/android-sdk
      bash ${../mk/android/scripts/build-sdl-libs.sh}
      runHook postBuild
    '';

    installPhase = ''
      runHook preInstall
      TARGET_ABIS=${pkgs.lib.escapeShellArg targetAbisStr} \
        bash ${../mk/android/scripts/install-sdl-libs.sh}
      runHook postInstall
    '';
  };


  # ---------------------------------------------------------------
  # OpenAL Soft + libmodplug for wstsound (per APP_ABI).
  # ---------------------------------------------------------------
  openalSrc = pkgs.fetchurl {
    url = "https://github.com/kcat/openal-soft/archive/refs/tags/1.23.1.tar.gz";
    hash = "sha256-393zofYQWYU8Ylt7sD3oQztFXy95+JVIy8vV7co9Sko=";
  };

  modplugSrc = pkgs.fetchurl {
    url = "https://downloads.sourceforge.net/project/modplug-xmms/libmodplug/0.8.9.0/libmodplug-0.8.9.0.tar.gz";
    hash = "sha256-RXylpsF5ZW1mwBUFwNlfr66tQym526oPmX0Ao1CK2d4=";
  };

  audioAndroidLibs = pkgs.stdenvNoCC.mkDerivation {
    pname = "pingus-android-audio-libs";
    version = "openal-1.23.1+modplug-0.8.9.0";
    dontUnpack = true;
    nativeBuildInputs = [ androidSdk pkgs.cmake pkgs.gnumake pkgs.python3 ];
    env = {
      PACKAGE_PLATFORM = packagePlatform;
      OPENAL_SRC = "${openalSrc}";
      MODPLUG_SRC = "${modplugSrc}";
    };
    buildPhase = ''
      runHook preBuild
      export ANDROID_HOME=${androidSdk}/libexec/android-sdk
      # Unpack modplug tarball to a stable path
      mkdir -p "$TMPDIR/modplug-src" "$TMPDIR/openal-src"
      tar -xzf "$MODPLUG_SRC" -C "$TMPDIR/modplug-src" --strip-components=1
      tar -xzf "${openalSrc}" -C "$TMPDIR/openal-src" --strip-components=1
      export MODPLUG_SRC="$TMPDIR/modplug-src"
      export OPENAL_SRC="$TMPDIR/openal-src"
      export TARGET_ABIS=${pkgs.lib.escapeShellArg targetAbisStr}
      export PACKAGE_PLATFORM=${packagePlatform}
      export OUT_DIR="$PWD/audio-out"
      bash ${../mk/android/scripts/build-audio-libs.sh}
      runHook postBuild
    '';
    installPhase = ''
      runHook preInstall
      mkdir -p $out
      cp -a audio-out/. $out/
      # Flat layout: $out/<abi>/{lib,include}
      runHook postInstall
    '';
  };

    # Prebuilt SDL2 + SDL2_mixer for the app's ndk-build tree.
  sdlPrebuiltAndroidMk = pkgs.writeTextFile {
    name = "SDL2-prebuilt-Android.mk";
    text = ''
      LOCAL_PATH := $(call my-dir)
      include $(CLEAR_VARS)
      LOCAL_MODULE := SDL2
      LOCAL_SRC_FILES := ${sdlAndroidLibs}/lib/$(TARGET_ARCH_ABI)/libSDL2.so
      include $(PREBUILT_SHARED_LIBRARY)

      include $(CLEAR_VARS)
      LOCAL_MODULE := SDL2_mixer
      LOCAL_SRC_FILES := ${sdlAndroidLibs}/lib/$(TARGET_ARCH_ABI)/libSDL2_mixer.so
      include $(PREBUILT_SHARED_LIBRARY)
    '';
  };

  # ---------------------------------------------------------------
  # Shared build pipeline for any SDL2-based app directory containing:
  #   AndroidManifest.xml, res/, jni/Android.mk (native module), main.cpp
  # Everything else (SDK/NDK setup, linking the prebuilt SDL2 layer,
  # aapt/zipalign/apksigner, ABI packaging) is identical across apps and
  # lives here once.
  # ---------------------------------------------------------------
  mkApk = {
    appName,
    appDir,
    outApkName,
    keystore,
    # Pingus (and similar): C++ sources live outside appDir.
    gameSrcDir ? null,
    # Repo external/ (geomcpp, priocpp, …). Required under Nix — GAME_SRC_DIR
    # is only ./src and its parent is not the monorepo root.
    gameExternalDir ? null,
    # glm include parent (contains glm/); required by geomcpp.
    glmIncludeDir ? null,
    # Optional SDL2_image source tree (stb backend compiled into libmain).
    sdl2ImageSrc ? null,
    # Optional game data directory packaged as APK assets.
    gameDataDir ? null,
    stbImageH ? null,
    # Full version for PINGUS_VERSION (e.g. 0.8.0-dev+gabc1234).
    gameVersion ? "0.8.0-dev",
  }:
    pkgs.stdenvNoCC.mkDerivation {
      pname = appName;
      version = gameVersion;

      dontUnpack = true;
      nativeBuildInputs = [ androidSdk pkgs.jdk17 pkgs.zip pkgs.gnumake pkgs.unzip ];

      env = {
        BUILD_TOOLS_VERSION = buildToolsVersion;
        PACKAGE_PLATFORM = packagePlatform;
        APP_NAME = appName;
        APP_DIR = "${appDir}";
        APPLICATION_MK = applicationMk;
        TOP_ANDROID_MK = topAndroidMk;
        SDL_PREBUILT_MK = sdlPrebuiltAndroidMk;
        SDL_ANDROID_LIBS = sdlAndroidLibs;
        AUDIO_ANDROID_LIBS = audioAndroidLibs;
        KEYSTORE = "${keystore}";
        PINGUS_VERSION = gameVersion;
      } // pkgs.lib.optionalAttrs (gameSrcDir != null) {
        GAME_SRC_DIR = "${gameSrcDir}";
      } // (
        if gameExternalDir == null then
          throw "mkApk: gameExternalDir is required (repo external/ tree)"
        else {
          GAME_EXTERNAL_DIR = "${gameExternalDir}";
        }
      ) // (
        if glmIncludeDir == null then
          throw "mkApk: glmIncludeDir is required (geom → glm headers)"
        else {
          GLM_INCLUDE_DIR = "${glmIncludeDir}";
        }
      ) // pkgs.lib.optionalAttrs (sdl2ImageSrc != null) {
        SDL2_IMAGE_SRC = "${sdl2ImageSrc}";
      } // (
        if gameDataDir == null then
          throw "mkApk: gameDataDir is required for Pingus Android APK"
        else {
          GAME_DATA_DIR = "${gameDataDir}";
        }
      ) // pkgs.lib.optionalAttrs (stbImageH != null) {
        STB_IMAGE_H = "${stbImageH}";
      };

      buildPhase = ''
        runHook preBuild
        export ANDROID_HOME=${androidSdk}/libexec/android-sdk
        TARGET_ABIS=${pkgs.lib.escapeShellArg targetAbisStr} \
          bash ${../mk/android/scripts/build-apk.sh}
        runHook postBuild
      '';

      installPhase = ''
        mkdir -p $out
        cp out/${appName}.apk $out/${outApkName}
      '';
    };

  mkInstallApp = { pkg, apkFileName, description ? "Install ${apkFileName} to a connected Android device via adb" }: {
    type = "app";
    program = toString (pkgs.writeShellScript "adb-install-${apkFileName}" ''
      exec ${pkgs.android-tools}/bin/adb install -r ${pkg}/${apkFileName}
    '');
    meta.description = description;
  };
in {
  inherit sdlAndroidLibs audioAndroidLibs mkApk mkInstallApp;
}
