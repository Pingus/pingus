# Reusable Android/ndk-build pipeline for an SDL3 + SDL3_image C++ app.
# Nothing project-specific here beyond what's passed in — another project
# can import this directly with its own app directory, keystore, and
# platform/ABI choices.
#
# Usage:
#   android = import ./nix/android.nix {
#     inherit pkgs;
#     sdlSrc = pkgs.sdl3.src;  sdlVersion = pkgs.sdl3.version;
#     sdlImageSrc = pkgs.sdl3-image.src;  sdlImageVersion = pkgs.sdl3-image.version;
#     androidSdk = ...;            # androidenv.composeAndroidPackages { ... }.androidsdk
#     buildToolsVersion = "35.0.0";
#     packagePlatform = "22";      # minSdk, linked against by ndk-build
#     compilePlatform = "35";      # javac classpath for SDL3's Java side
#     targetAbis = [ "armeabi-v7a" "arm64-v8a" ];
#   };
#   android.mkApk { appName = "myapp"; appDir = ./apps/myapp; outApkName = "myapp.apk"; keystore = ./mk/android/keystore/debug.keystore; }
{ pkgs
, sdlSrc
, sdlVersion
, sdlImageSrc
, sdlImageVersion
, androidSdk
, buildToolsVersion
, packagePlatform
, compilePlatform
, targetAbis
}:

let
  targetAbisStr = pkgs.lib.concatStringsSep " " targetAbis;

  # Generic ndk-build entry point: just recurses into whatever subdirs
  # have their own Android.mk (SDL3, SDL3_image, and the app's own).
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
      # C++23 (std::format, std::print when available). Needs a recent NDK libc++.
      APP_CPPFLAGS := -std=c++23 -D_LIBCPP_ENABLE_EXPERIMENTAL=1
    '';
  };

  # ---------------------------------------------------------------
  # SDL3 (shared) + SDL3_image (static) and SDLActivity Java glue.
  # Built once and reused across app rebuilds.
  # ---------------------------------------------------------------
  sdlAndroidLibs = pkgs.stdenvNoCC.mkDerivation {
    pname = "sdl3-android-libs";
    version = "${sdlVersion}+image-${sdlImageVersion}";

    dontUnpack = true;
    nativeBuildInputs = [ androidSdk pkgs.jdk17 pkgs.gnumake ];

    env = {
      BUILD_TOOLS_VERSION = buildToolsVersion;
      COMPILE_PLATFORM = compilePlatform;
      PACKAGE_PLATFORM = packagePlatform;
      SDL_SRC = "${sdlSrc}";
      SDL_IMAGE_SRC = "${sdlImageSrc}";
      APPLICATION_MK = applicationMk;
      TOP_ANDROID_MK = topAndroidMk;
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
    # Sources are tarballs referenced via env; unpack inside buildPhase.
    # cmake is only used by build-audio-libs.sh — do not run the generic
    # cmake configurePhase against an empty /build.
    dontUnpack = true;
    dontConfigure = true;
    dontUseCmakeConfigure = true;
    # Outputs are Android ELF (armeabi-v7a / arm64); host patchelf/strip fail.
    dontPatchELF = true;
    dontStrip = true;
    noAuditTmpdir = true;
    nativeBuildInputs = [ androidSdk pkgs.cmake pkgs.gnumake pkgs.python3 pkgs.gnutar pkgs.gzip ];
    env = {
      PACKAGE_PLATFORM = packagePlatform;
      OPENAL_SRC = "${openalSrc}";
      MODPLUG_SRC = "${modplugSrc}";
    };
    buildPhase = ''
      runHook preBuild
      export ANDROID_HOME=${androidSdk}/libexec/android-sdk
      mkdir -p "$TMPDIR/modplug-src" "$TMPDIR/openal-src"
      tar -xzf "$MODPLUG_SRC" -C "$TMPDIR/modplug-src" --strip-components=1
      tar -xzf "$OPENAL_SRC" -C "$TMPDIR/openal-src" --strip-components=1
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
      runHook postInstall
    '';
  };

  # Prebuilt SDL3 + SDL3_image for the app's ndk-build tree.
  sdlPrebuiltAndroidMk = pkgs.writeTextFile {
    name = "SDL3-prebuilt-Android.mk";
    text = ''
      LOCAL_PATH := $(call my-dir)
      include $(CLEAR_VARS)
      LOCAL_MODULE := SDL3
      LOCAL_SRC_FILES := ${sdlAndroidLibs}/lib/$(TARGET_ARCH_ABI)/libSDL3.so
      LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
      include $(PREBUILT_SHARED_LIBRARY)

      include $(CLEAR_VARS)
      LOCAL_MODULE := SDL3_image
      LOCAL_SRC_FILES := ${sdlAndroidLibs}/lib/$(TARGET_ARCH_ABI)/libSDL3_image.a
      LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
      include $(PREBUILT_STATIC_LIBRARY)
    '';
  };

  # ---------------------------------------------------------------
  # Shared build pipeline for any SDL3-based app directory containing:
  #   AndroidManifest.xml, res/, jni/Android.mk (native module), main.cpp
  # Everything else (SDK/NDK setup, linking the prebuilt SDL3 layer,
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
    # Optional game data directory packaged as APK assets.
    gameDataDir ? null,
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
      ) // (
        if gameDataDir == null then
          throw "mkApk: gameDataDir is required for Pingus Android APK"
        else {
          GAME_DATA_DIR = "${gameDataDir}";
        }
      );

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
