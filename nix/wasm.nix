{ pkgs
, sdlSrc
, sdlVersion
, sdlImageSrc ? null
, sdlMixerSrc ? null
, libxmpSrc ? null
}:

let
  lib = pkgs.lib;

  # Shared install helper bits for a single-component prefix under $PWD/prefix.
  installPrefixPhase = ''
    runHook preInstall
    mkdir -p $out
    if [ -d prefix ]; then
      cp -a prefix/. $out/
    else
      mkdir -p $out/lib $out/include
    fi
    runHook postInstall
  '';

  # --- SDL2 only -----------------------------------------------------------
  sdl2WasmLibs = pkgs.stdenv.mkDerivation {
    pname = "sdl2-wasm";
    version = sdlVersion;
    dontUnpack = true;
    dontConfigure = true;
    dontUseCmakeConfigure = true;
    nativeBuildInputs = [ pkgs.emscripten pkgs.cmake pkgs.python3 ];
    env = {
      SDL_SRC = "${sdlSrc}";
    };
    buildPhase = ''
      runHook preBuild
      bash ${../mk/wasm/scripts/build-sdl2.sh}
      runHook postBuild
    '';
    installPhase = ''
      runHook preInstall
      mkdir -p $out
      if [ -d prefix ]; then
        cp -a prefix/. $out/
      else
        mkdir -p $out/lib $out/include
        find . -name 'libSDL2.a' -exec cp {} $out/lib/ \;
        if [ -d SDL2-src/include ]; then cp -a SDL2-src/include/. $out/include/; fi
      fi
      mkdir -p $out/lib/pkgconfig
      cat > $out/lib/pkgconfig/sdl2.pc <<EOF
prefix=$out
exec_prefix=\''${prefix}
libdir=\''${prefix}/lib
includedir=\''${prefix}/include
Name: sdl2
Description: SDL2 (wasm static)
Version: ${sdlVersion}
Libs: -L\''${libdir} -lSDL2
Cflags: -I\''${includedir} -I\''${includedir}/SDL2
EOF
      runHook postInstall
    '';
  };

  # --- SDL2_image (depends on sdl2WasmLibs) --------------------------------
  sdl2Image = if sdlImageSrc == null then null else pkgs.stdenv.mkDerivation {
      pname = "sdl2-image-wasm";
      version = "2.8.2";
      dontUnpack = true;
      dontConfigure = true;
      dontUseCmakeConfigure = true;
      nativeBuildInputs = [ pkgs.emscripten pkgs.cmake pkgs.python3 ];
      env = {
        SDL_PREFIX = "${sdl2WasmLibs}";
        SDL_IMAGE_SRC = "${sdlImageSrc}";
      };
      buildPhase = ''
        runHook preBuild
        bash ${../mk/wasm/scripts/build-sdl2-image.sh}
        runHook postBuild
      '';
      installPhase = ''
        runHook preInstall
        mkdir -p $out/lib $out/include $out/lib/pkgconfig
        if [ -d prefix ]; then
          # Prefer only image artifacts if present; still copy tree for cmake configs.
          cp -a prefix/. $out/
          # Drop SDL2 core copies if the script staged any (should not with SDL_PREFIX).
          rm -f $out/lib/libSDL2.a $out/lib/libSDL2main.a 2>/dev/null || true
        fi
        if [ ! -f $out/lib/libSDL2_image.a ]; then
          find . -name 'libSDL2_image.a' -exec cp {} $out/lib/ \; || true
        fi
        if [ -f $out/lib/libSDL2_image.a ]; then
          cat > $out/lib/pkgconfig/SDL2_image.pc <<EOF
prefix=$out
libdir=\''${prefix}/lib
includedir=\''${prefix}/include
Name: SDL2_image
Description: SDL2_image (wasm static)
Version: 2.8.2
Requires: sdl2
Libs: -L\''${libdir} -lSDL2_image
Cflags: -I\''${includedir}
EOF
        fi
        runHook postInstall
      '';
    };

  # --- SDL2_mixer + libxmp (depends on sdl2WasmLibs) -----------------------
  sdl2Mixer = if sdlMixerSrc == null then null else pkgs.stdenv.mkDerivation {
      pname = "sdl2-mixer-wasm";
      version = "2.8.0";
      dontUnpack = true;
      dontConfigure = true;
      dontUseCmakeConfigure = true;
      nativeBuildInputs = [ pkgs.emscripten pkgs.cmake pkgs.python3 ];
      env = {
        SDL_PREFIX = "${sdl2WasmLibs}";
        SDL_MIXER_SRC = "${sdlMixerSrc}";
      } // lib.optionalAttrs (libxmpSrc != null) {
        LIBXMP_SRC = "${libxmpSrc}";
      };
      buildPhase = ''
        runHook preBuild
        bash ${../mk/wasm/scripts/build-sdl2-mixer.sh}
        runHook postBuild
      '';
      installPhase = ''
        runHook preInstall
        mkdir -p $out/lib $out/include $out/lib/pkgconfig
        if [ -d prefix ]; then
          cp -a prefix/. $out/
          rm -f $out/lib/libSDL2.a $out/lib/libSDL2main.a 2>/dev/null || true
        fi
        for lib in libSDL2_mixer.a libxmp.a; do
          if [ ! -f $out/lib/$lib ]; then
            find . -name "$lib" -exec cp {} $out/lib/ \; || true
          fi
        done
        if [ -f $out/lib/libSDL2_mixer.a ]; then
          xmp_libs=""
          if [ -f $out/lib/libxmp.a ]; then xmp_libs=" -lxmp"; fi
          cat > $out/lib/pkgconfig/SDL2_mixer.pc <<EOF
prefix=$out
libdir=\''${prefix}/lib
includedir=\''${prefix}/include
Name: SDL2_mixer
Description: SDL2_mixer (wasm static)
Version: 2.8.0
Requires: sdl2
Libs: -L\''${libdir} -lSDL2_mixer$xmp_libs
Cflags: -I\''${includedir}
EOF
        fi
        if [ -f $out/lib/libxmp.a ]; then
          cat > $out/lib/pkgconfig/libxmp.pc <<EOF
prefix=$out
libdir=\''${prefix}/lib
includedir=\''${prefix}/include
Name: libxmp
Description: libxmp (wasm static)
Version: 4.6.0
Libs: -L\''${libdir} -lxmp
Cflags: -I\''${includedir}
EOF
        fi
        runHook postInstall
      '';
    };

  # Combined prefix for the game (and legacy attribute name).
  sdlWasmLibs = pkgs.symlinkJoin {
    name = "sdl2-wasm-libs-${sdlVersion}";
    paths =
      [ sdl2WasmLibs ]
      ++ lib.optional (sdl2Image != null) sdl2Image
      ++ lib.optional (sdl2Mixer != null) sdl2Mixer;
  };

  # Static zlib for wasm (lispreader / .gz levels). Offline — no -sUSE_ZLIB port.
  zlibWasmLibs = pkgs.stdenv.mkDerivation {
    pname = "zlib-wasm-libs";
    version = pkgs.zlib.version;

    dontUnpack = true;
    dontConfigure = true;
    nativeBuildInputs = [ pkgs.emscripten ];

    env = {
      ZLIB_SRC = "${pkgs.zlib.src}";
    };

    buildPhase = ''
      runHook preBuild
      bash ${../mk/wasm/scripts/build-zlib.sh}
      runHook postBuild
    '';

    installPhase = ''
      runHook preInstall
      mkdir -p $out
      if [ -d prefix ]; then
        cp -a prefix/. $out/
      else
        echo "error: build-wasm-zlib.sh did not produce prefix/" >&2
        exit 1
      fi
      # Optional pkg-config for completeness.
      mkdir -p $out/lib/pkgconfig
      cat > $out/lib/pkgconfig/zlib.pc <<EOF
prefix=$out
exec_prefix=\''${prefix}
libdir=\''${prefix}/lib
includedir=\''${prefix}/include
Name: zlib
Description: zlib (wasm static)
Version: ${pkgs.zlib.version}
Libs: -L\''${libdir} -lz
Cflags: -I\''${includedir}
EOF
      runHook postInstall
    '';

    meta = with pkgs.lib; {
      description = "Static zlib built for wasm32-emscripten";
      license = licenses.zlib;
      platforms = platforms.linux;
    };
  };


  # HTML shell: template under mk/wasm/shell.html (@versionFull@ @gitRev@ @sourceUrl@ @revUrl@).
  mkWasmShell = { versionFull, gitRev, sourceUrl }:
    let
      revUrl =
        if gitRev == "dirty" || gitRev == "" then sourceUrl
        else "${sourceUrl}/tree/${gitRev}";
    in
    pkgs.replaceVars ../mk/wasm/shell.html {
      inherit versionFull gitRev sourceUrl revUrl;
    };

  mkApp = {
    appName
  , srcDir
  , dataDir ? null
  , enableSound ? false
  , enableGles2 ? true
  , enableAsyncify ? false   # main path uses app_loop; set true if residual waits freeze
  , versionFull ? "0.1.5-dev"
  , gitRev ? "dirty"
  , sourceUrl ? "https://github.com/Pingus-Origins/pingus"
  }:
    let
      shell = mkWasmShell { inherit versionFull gitRev sourceUrl; };
    in
    pkgs.stdenv.mkDerivation {
      pname = "${appName}-wasm";
      version = versionFull;

      dontUnpack = true;
      dontConfigure = true;
      dontUseCmakeConfigure = true;
      nativeBuildInputs = [ pkgs.emscripten pkgs.cmake pkgs.python3 pkgs.pkg-config ];

      env = {
        APP_NAME = appName;
        SRC_DIR = "${srcDir}";
        SDL_WASM_LIBS = sdlWasmLibs;
        ENABLE_SOUND = if enableSound then "1" else "0";
        ENABLE_GLES2 = if enableGles2 then "1" else "0";
        ENABLE_ASYNCIFY = if enableAsyncify then "1" else "0";
        PROJECT_VERSION_FULL = versionFull;
        WASM_SHELL = "${shell}";
        PKG_CONFIG_PATH = "${sdlWasmLibs}/lib/pkgconfig";
        ZLIB_WASM_LIBS = zlibWasmLibs;
      } // pkgs.lib.optionalAttrs (dataDir != null) {
        DATA_DIR = "${dataDir}";
      };

      buildPhase = ''
        runHook preBuild
        # Emscripten + pkg-config stubs for our static libs.
        export EM_PKG_CONFIG_PATH="${sdlWasmLibs}/lib/pkgconfig"
        export PKG_CONFIG_PATH="${sdlWasmLibs}/lib/pkgconfig''${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
        bash ${../mk/wasm/scripts/build-app.sh}
        runHook postBuild
      '';

      installPhase = ''
        mkdir -p $out
        for f in ${appName}.html ${appName}.js ${appName}.wasm ${appName}.data; do
          if [ -f "$f" ]; then cp "$f" $out/; fi
        done
        # Fallback: whatever emscripten left in build/
        if [ ! -f $out/${appName}.html ]; then
          find build -maxdepth 1 -type f \( -name '*.html' -o -name '*.js' -o -name '*.wasm' -o -name '*.data' \) \
            -exec cp {} $out/ \; || true
        fi
        ls -la $out
      '';

      meta = with pkgs.lib; {
        description = "Pingus (WebAssembly / Emscripten)";
        license = licenses.gpl3Plus;
        platforms = platforms.linux;
      };
    };

  mkOpenBrowserApp = {
    pkg
  , appName
  , description ? "Serve and open the ${appName} wasm build in a browser"
  }: {
    type = "app";
    program = toString (pkgs.writeShellScript "serve-${appName}-wasm" ''
      set -euo pipefail
      export PKG=${pkg}
      export APP_NAME=${appName}
      export PATH=${pkgs.python3}/bin:${pkgs.xdg-utils}/bin:$PATH
      exec bash ${../mk/wasm/scripts/serve.sh}
    '');
    meta.description = description;
  };

in {
  inherit sdl2WasmLibs sdlWasmLibs zlibWasmLibs mkApp mkOpenBrowserApp;
  inherit sdl2Image sdl2Mixer;
}
