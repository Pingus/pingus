{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    # Windows MinGW SDL ports (still external flakes; not under external/)
    SDL2-win32.url = "git+https://github.com/grumnix/SDL2-win32.git";
    SDL2-win32.inputs.nixpkgs.follows = "nixpkgs";

    SDL2_image-win32.url = "git+https://github.com/grumnix/SDL2_image-win32.git";
    SDL2_image-win32.inputs.nixpkgs.follows = "nixpkgs";

    # Prebuilt MinGW OpenAL Soft + libmodplug (avoids nixpkgs openal → ffmpeg-headless).
    openal-soft-win32.url = "git+https://github.com/grumnix/openal-soft-win32.git";
    openal-soft-win32.inputs.nixpkgs.follows = "nixpkgs";

    libmodplug-win32.url = "git+https://github.com/grumnix/libmodplug-win32.git";
    libmodplug-win32.inputs.nixpkgs.follows = "nixpkgs";

    # Source tarballs for Android / wasm SDL stacks (prebuilt port deps).
    sdl2-src = {
      url = "https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-2.30.3.tar.gz";
      flake = false;
    };
    sdl2-image-src = {
      url = "https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.tar.gz";
      flake = false;
    };
    sdl2-mixer-src = {
      url = "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-2.8.0.tar.gz";
      flake = false;
    };
    libxmp-src = {
      url = "https://github.com/libxmp/libxmp/releases/download/libxmp-4.6.0/libxmp-4.6.0.tar.gz";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, flake-utils, SDL2-win32, SDL2_image-win32
            , openal-soft-win32, libmodplug-win32
            , sdl2-src, sdl2-image-src, sdl2-mixer-src, libxmp-src }:
    # Host systems only. Windows is a *target* via pkgsCross.
    # x86_64-darwin is omitted: nixpkgs unstable (26.11+) dropped support.
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" "aarch64-darwin" ] (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        lib = nixpkgs.lib;
        isWin = pkgs.stdenv.hostPlatform.isWindows;

        # tinycmmc lib helpers (versionFromVERSION, …) from vendored sources
        tinycmmc_lib = import ./external/tinycmmc {
          inherit nixpkgs flake-utils;
        };

        # Build helper libraries from external/ (no flake inputs).
        # `selfFor path` makes versionFromVERSION / versionFromFile read the
        # right VERSION file under external/<name>.
        # Do not inject the monorepo shortRev into external package versions —
        # that forced a rebuild of every external/* derivation on every commit.
        selfFor = path: {
          outPath = path;
          shortRev = "vendored";
          dirtyShortRev = "vendored";
          lastModifiedDate = "19700101";
        };

        mkLibs = pkgs':
          let
            call = path: args:
              pkgs'.callPackage path args;
          in
          rec {
            tinycmmc = call ./external/tinycmmc/tinycmmc.nix {
              self = selfFor ./external/tinycmmc;
              inherit tinycmmc_lib;
            };

            argpp = call ./external/argpp/argpp.nix { };

            geomcpp = call ./external/geomcpp/geomcpp.nix {
              self = selfFor ./external/geomcpp;
              inherit tinycmmc tinycmmc_lib;
              glm = pkgs'.glm.overrideAttrs (_: { meta = { }; });
            };

            logmich = call ./external/logmich/logmich.nix { };

            sexpcpp = call ./external/sexpcpp/sexpcpp.nix { };

            priocpp = call ./external/priocpp/priocpp.nix {
              inherit logmich tinycmmc;
              sexpcpp = sexpcpp;
              withSexpcpp = true;
              withJsoncpp = true;
            };

            strutcpp = call ./external/strutcpp/strutcpp.nix { };

            tinygettext = call ./external/tinygettext/tinygettext.nix { };

            uitest = call ./external/uitest/uitest.nix { };

            wstsound =
              let
                win = pkgs'.stdenv.hostPlatform.isWindows;
                # Builder system (Linux); Win ABIs are package names, not flake systems.
                winSuffix =
                  if pkgs'.stdenv.hostPlatform.is32bit then "win32" else "win64";
              in
              call ./external/wstsound/wstsound.nix ({
                inherit tinycmmc;
                mcfgthreads =
                  if win then pkgs'.windows.mcfgthreads else null;
              } // (if win then {
                # Official/prebuilt MinGW packages (not pkgsCross openal → ffmpeg).
                openal = openal-soft-win32.packages.${system}."openal-soft-${winSuffix}";
                libmodplug = libmodplug-win32.packages.${system}."libmodplug-${winSuffix}";
              } else { }));

            xdgcpp =
              if pkgs'.stdenv.hostPlatform.isWindows then null
              else
                pkgs'.stdenv.mkDerivation {
                  pname = "xdgcpp";
                  version = "0.1.0";
                  src = lib.cleanSource ./external/xdgcpp;
                  nativeBuildInputs = [ pkgs'.cmake ];
                  # Tests need Boost; skip for the game dependency.
                  cmakeFlags = [ "-DXDG_BUILD_TESTS=OFF" ];
                };
          };

        mkPingus = { pkgs', pname ? "pingus", useGLES2 ? false }:
          let
            libs = mkLibs pkgs';
            win = pkgs'.stdenv.hostPlatform.isWindows;
            winSuffix =
              if pkgs'.stdenv.hostPlatform.is32bit then "win32" else "win64";
          in
          pkgs'.callPackage ./pingus.nix {
            inherit self;
            stdenv = pkgs'.stdenv;
            tinycmmc_lib = tinycmmc_lib;

            argpp = libs.argpp;
            geomcpp = libs.geomcpp;
            logmich = libs.logmich;
            priocpp = libs.priocpp;
            strutcpp = libs.strutcpp;
            tinycmmc = libs.tinycmmc;
            tinygettext = libs.tinygettext;
            uitest = libs.uitest;
            wstsound = libs.wstsound;
            xdgcpp = libs.xdgcpp;
            mcfgthreads = if win then pkgs'.windows.mcfgthreads else null;
            libGL = if win then null else pkgs'.libGL;
            libGLU = if win then null else pkgs'.libGLU;
            inherit useGLES2;
            libglvnd = if win || !useGLES2 then null else pkgs'.libglvnd;
            addDriverRunpath = if win || !useGLES2 then null else pkgs'.addDriverRunpath;

            SDL2 =
              if win then SDL2-win32.packages.${system}."SDL2-${winSuffix}"
              else pkgs'.SDL2;
            SDL2_image =
              if win then SDL2_image-win32.packages.${system}."SDL2_image-${winSuffix}"
              else pkgs'.SDL2_image;
          };

        pingusNative = mkPingus {
          pkgs' = pkgs;
        };

        # Desktop GLES2 build for validating the Android/wasm GL path on Linux.
        # Named like other GLES2 embedded targets in the flake (r36s / wasm).
        pingusGles2 = mkPingus {
          pkgs' = pkgs;
          useGLES2 = true;
        };

        mkWinFlat = { game, pname }:
          pkgs.runCommand pname { } ''
            mkdir -p $out/data
            cp -vr ${game}/bin/pingus.exe $out/ 2>/dev/null \
              || cp -vr ${game}/bin/*.exe $out/
            cp -vLr ${game}/bin/*.dll $out/ 2>/dev/null || true
            if [ -d ${game}/share/pingus ]; then
              cp -vr ${game}/share/pingus/. $out/data/
            fi
          '';

        mkWinZip = pkg: name: winsys:
          pkgs.runCommand name { } ''
            mkdir -p $out
            WORKDIR=$(mktemp -d)
            cp --no-preserve mode,ownership --verbose --recursive \
              ${pkg}/. "$WORKDIR"
            cd "$WORKDIR"
            ${pkgs.zip}/bin/zip \
              -r \
              $out/${name}-${pingusNative.version}-${winsys}.zip \
              .
          '';

        # MinGW target packages, hosted under packages.${system} (Linux builder).
        # Sound uses openal-soft-win32 / libmodplug-win32 — not pkgsCross openal (ffmpeg).
        win64Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingwW64;
        };
        win32Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingw32;
        };

        win64Package = if isWin then null else mkWinFlat {
          game = win64Game;
          pname = "pingus-win64";
        };
        win32Package = if isWin then null else mkWinFlat {
          game = win32Game;
          pname = "pingus-win32";
        };


        # ---- Linux-only: Android + wasm + R36S (must not eval on Windows hosts) ----
        versionBase = lib.strings.removeSuffix "\n" (builtins.readFile ./VERSION);
        gitRev = "${self.shortRev or self.dirtyShortRev or "dirty"}";
        pingusVersion = "${versionBase}+g${gitRev}";

        linuxExtras =
          if isWin || !(pkgs.stdenv.hostPlatform.isLinux) then { packages = { }; apps = { }; }
          else
          let
            androidPkgs = import nixpkgs {
              system = pkgs.stdenv.hostPlatform.system;
              config.allowUnfree = true;
              config.android_sdk.accept_license = true;
            };
            buildToolsVersion = "30.0.3";
            packagePlatform = "22";
            compilePlatform = "33";
            # Need full std::format (format_to/vformat). r23: no <format>;
            # r26.1: header present but incomplete in practice; r27 is solid.
            ndkVersion = "27.0.12077973";
            targetAbis = [ "armeabi-v7a" "arm64-v8a" ];
            androidSdk = (androidPkgs.androidenv.composeAndroidPackages {
              platformVersions = [ packagePlatform compilePlatform ];
              buildToolsVersions = [ buildToolsVersion ];
              includeNDK = true;
              inherit ndkVersion;
              includeEmulator = false;
              includeSources = false;
            }).androidsdk;

            r36s = import ./nix/r36s.nix {
              inherit (pkgs) lib stdenv stdenvNoCC fetchurl cmake pkg-config writeShellScript zip glm;
              pkgsCross = pkgs.pkgsCross;
            };
            pingusR36s = r36s.mkPingusR36s {
              src = lib.cleanSource ./.;
              version = pingusVersion;
              pname = "pingus-r36s";
            };
            pingusR36sPortMaster = r36s.mkPingusR36sPortMaster {
              r36sPkg = pingusR36s;
              version = pingusVersion;
              pname = "pingus-r36s-portmaster";
            };

            android = import ./nix/android.nix {
              pkgs = androidPkgs;
              sdlSrc = sdl2-src;
              sdlVersion = "2.30.3";
              sdlMixerSrc = sdl2-mixer-src;
              sdlMixerVersion = "2.8.0";
              libxmpSrc = libxmp-src;
              inherit androidSdk buildToolsVersion packagePlatform compilePlatform targetAbis;
            };

            wasm = import ./nix/wasm.nix {
              inherit pkgs;
              sdlSrc = sdl2-src;
              sdlImageSrc = sdl2-image-src;
              sdlMixerSrc = sdl2-mixer-src;
              libxmpSrc = libxmp-src;
              sdlVersion = "2.30.3";
            };

            wasmDataDir = if builtins.pathExists ./data then ./data else null;
            gitDate =
              if self ? lastModifiedDate then builtins.substring 0 8 self.lastModifiedDate
              else "00000000";
            androidApkName = "pingus-${gitDate}-${gitRev}.apk";
            stbImageH = androidPkgs.fetchurl {
              url = "https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h";
              sha256 = "sha256-WUwv411JSItDgtv67I+YNm3vyoGdkWrJW+zz519CALM=";
            };
          in {
            packages = {
              arkos-sysroot = r36s.arkosSysroot;
              pingus-r36s = pingusR36s;
              pingus-r36s-portmaster = pingusR36sPortMaster;
              pingus-r36s-portmaster-zip = r36s.mkPingusR36sPortMasterZip {
                portMasterPkg = pingusR36sPortMaster;
                version = pingusVersion;
                pname = "pingus-r36s-portmaster-zip";
              };
              android-sdl-libs = android.sdlAndroidLibs;
              pingus-android = android.mkApk {
                appName = "pingus";
                appDir = ./mk/android/app;
                outApkName = androidApkName;
                keystore = ./mk/android/keystore/debug.keystore;
                gameSrcDir = ./src;
                gameExternalDir = ./external;
                glmIncludeDir = "${androidPkgs.glm}/include";
                gameDataDir = ./data;
                stbImageH = stbImageH;
                gameVersion = pingusVersion;
              };
              wasm-sdl2 = wasm.sdl2WasmLibs;
              wasm-sdl-libs = wasm.sdlWasmLibs;
              # Isolated audio deps for wstsound-on-wasm (build without the full game):
              #   nix build .#libmodplug-wasm
              libmodplug-wasm = wasm.modplugWasm;
              wstsound-wasm = wasm.wstsoundWasm;
              pingus-wasm = wasm.mkApp {
                appName = "pingus";
                srcDir = ./.;
                dataDir = wasmDataDir;
                enableSound = true;  # Emscripten OpenAL + wstsound (modplug + wav)
                enableGles2 = true;
                enableAsyncify = false;
                versionFull = pingusVersion;
                gitRev = gitRev;
                sourceUrl = "https://github.com/Pingus/pingus";
              };
            };
            apps = {
              install-android-pingus = android.mkInstallApp {
                pkg = android.mkApk {
                  appName = "pingus";
                  appDir = ./mk/android/app;
                  outApkName = androidApkName;
                  keystore = ./mk/android/keystore/debug.keystore;
                  gameSrcDir = ./src;
                  gameExternalDir = ./external;
                  glmIncludeDir = "${androidPkgs.glm}/include";
                  gameDataDir = ./data;
                  stbImageH = stbImageH;
                  gameVersion = pingusVersion;
                };
                apkFileName = androidApkName;
              };
              pingus-wasm = wasm.mkOpenBrowserApp {
                pkg = wasm.mkApp {
                  appName = "pingus";
                  srcDir = ./.;
                  dataDir = wasmDataDir;
                  enableSound = true;  # Emscripten OpenAL + wstsound (modplug + wav)
                  enableGles2 = true;
                  enableAsyncify = false;
                  versionFull = pingusVersion;
                  gitRev = gitRev;
                  sourceUrl = "https://github.com/Pingus/pingus";
                };
                appName = "pingus";
              };
            };
          };


        libsNative = mkLibs pkgs;

        # Wine runner for Windows cross builds.
        # Defined in let so apps can reference it; must not be an outputs attr.
        mkWineApp = pkg: name: description:
          if isWin || !pkgs.stdenv.hostPlatform.isLinux then null
          else {
            type = "app";
            program = toString (pkgs.writeShellScript name ''
              set -euo pipefail
              export WINEPREFIX=$(mktemp -d)
              export WINEARCH=win64
              export WINEDLLOVERRIDES="mscoree,mshtml="
              export WINEDLLOVERRIDES="SDL2=n,$WINEDLLOVERRIDES"
              trap 'rm -rf "$WINEPREFIX"' EXIT
              ${pkgs.wineWow64Packages.stable}/bin/wineboot --init >/dev/null 2>&1 || true
              cd ${pkg}
              exe=
              for c in pingus.exe *.exe; do
                if [ -f "$c" ]; then exe="$c"; break; fi
              done
              if [ -z "$exe" ]; then
                echo "error: no .exe found in ${pkg}" >&2
                exit 1
              fi
              exec ${pkgs.wineWow64Packages.stable}/bin/wine "./$exe" "$@"
            '');
            meta.description = description;
          };
      in
      let
        packages =
          {
            default = pingusNative;
            pingus = pingusNative;
            pingus-gles2 = pingusGles2;
          }
          // lib.optionalAttrs (!isWin) {
            # Cross-built on this host; Windows is the run target, not the flake system.
            pingus-win64 = win64Package;
            pingus-win32 = win32Package;
            pingus-win64-zip = mkWinZip win64Package "pingus" "win64";
            pingus-win32-zip = mkWinZip win32Package "pingus" "win32";
          }
          // {
            # Optional: individual external libs for debugging
            inherit (libsNative)
              tinycmmc argpp geomcpp logmich sexpcpp priocpp
              strutcpp tinygettext uitest wstsound;
          }
          // lib.optionalAttrs (libsNative.xdgcpp != null) {
            xdgcpp = libsNative.xdgcpp;
          }
          // linuxExtras.packages;

        apps =
          {
            default = {
              type = "app";
              program = "${pingusNative}/bin/pingus";
              meta.description = "Pingus (native)";
            };
            pingus = {
              type = "app";
              program = "${pingusNative}/bin/pingus";
              meta.description = "Pingus (native)";
            };
            pingus-gles2 = {
              type = "app";
              program = "${pingusGles2}/bin/pingus";
              meta.description = "Pingus (native, OpenGL ES 2.0)";
            };
          }
          // lib.optionalAttrs (!isWin && pkgs.stdenv.hostPlatform.isLinux) {
            pingus-win64 = mkWineApp win64Package "pingus-win64" "Pingus (MinGW x86_64) via Wine";
            pingus-win32 = mkWineApp win32Package "pingus-win32" "Pingus (MinGW i686) via Wine";
          }
          // linuxExtras.apps;

        # App names overlap packages (default/pingus/…); prefix so checks can hold both.
        appChecks = lib.mapAttrs' (name: app:
          lib.nameValuePair "app-${name}" (
            pkgs.runCommand "check-app-${name}" {
              meta.description = "flake check: apps.${name} program exists";
            } ''
              set -euo pipefail
              if [ ! -e "${app.program}" ]; then
                echo "apps.${name}: program missing: ${app.program}" >&2
                exit 1
              fi
              touch "$out"
            ''
          )
        ) apps;
      in {
        inherit packages apps;
        # Build every package; verify every app's program path resolves.
        checks = packages // appChecks;
      }
    );
}
