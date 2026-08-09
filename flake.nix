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

    # Source tarballs for Android / wasm SDL stacks (same pattern as SuperTux M1).
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
            , sdl2-src, sdl2-image-src, sdl2-mixer-src, libxmp-src }:
    # Host systems only. Windows is a *target* via pkgsCross (SuperTux pattern).
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

            argpp = call ./external/argpp/argpp.nix {
              inherit tinycmmc;
            };

            geomcpp = call ./external/geomcpp/geomcpp.nix {
              self = selfFor ./external/geomcpp;
              inherit tinycmmc tinycmmc_lib;
              glm = pkgs'.glm.overrideAttrs (_: { meta = { }; });
            };

            logmich = call ./external/logmich/logmich.nix {
              inherit tinycmmc;
            };

            sexpcpp = call ./external/sexpcpp/sexpcpp.nix {
              inherit tinycmmc;
            };

            priocpp = call ./external/priocpp/priocpp.nix {
              inherit logmich tinycmmc;
              sexpcpp = sexpcpp;
              withSexpcpp = true;
              withJsoncpp = true;
            };

            strutcpp = call ./external/strutcpp/strutcpp.nix {
              inherit tinycmmc;
            };

            tinygettext = call ./external/tinygettext/tinygettext.nix {
              inherit tinycmmc;
            };

            uitest = call ./external/uitest/uitest.nix {
              inherit tinycmmc;
            };

            wstsound = call ./external/wstsound/wstsound.nix {
              inherit tinycmmc;
              mcfgthreads =
                if pkgs'.stdenv.hostPlatform.isWindows
                then pkgs'.windows.mcfgthreads
                else null;
            };

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

        mkPingus = { pkgs', targetSystem, pname ? "pingus" }:
          let
            libs = mkLibs pkgs';
            win = pkgs'.stdenv.hostPlatform.isWindows;
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

            SDL2 =
              if win then SDL2-win32.packages.${targetSystem}.default
              else pkgs'.SDL2;
            SDL2_image =
              if win then SDL2_image-win32.packages.${targetSystem}.default
              else pkgs'.SDL2_image;
          };

        pingusNative = mkPingus {
          pkgs' = pkgs;
          targetSystem = system;
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

        mkWinZip = pkg: name:
          pkgs.runCommand name { } ''
            mkdir -p $out
            WORKDIR=$(mktemp -d)
            cp --no-preserve mode,ownership --verbose --recursive \
              ${pkg}/. "$WORKDIR"
            cd "$WORKDIR"
            ${pkgs.zip}/bin/zip \
              -r \
              $out/${name}-${pingusNative.version}-${system}.zip \
              .
          '';

        win64Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingwW64;
          targetSystem = "x86_64-windows";
        };
        win32Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingw32;
          targetSystem = "i686-windows";
        };

        win64Package = if isWin then null else mkWinFlat {
          game = win64Game;
          pname = "pingus-win32-x64";
        };
        win32Package = if isWin then null else mkWinFlat {
          game = win32Game;
          pname = "pingus-win32-x86";
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
            ndkVersion = "23.1.7779620";
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
              inherit (pkgs) lib stdenv stdenvNoCC fetchurl cmake pkg-config writeShellScript zip;
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
      in {
        packages = {
          default = pingusNative;
          pingus = pingusNative;
        } // lib.optionalAttrs (!isWin) {
          pingus-win32-x64 = win64Package;
          pingus-win32-x86 = win32Package;
          pingus-win32-x64-zip = mkWinZip win64Package "pingus";
          pingus-win32-x86-zip = mkWinZip win32Package "pingus";
        } // {
          # Optional: individual external libs for debugging
          inherit (libsNative)
            tinycmmc argpp geomcpp logmich sexpcpp priocpp
            strutcpp tinygettext uitest wstsound;
        } // lib.optionalAttrs (libsNative.xdgcpp != null) {
          xdgcpp = libsNative.xdgcpp;
        } // linuxExtras.packages;

        apps = {
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
        } // linuxExtras.apps;
      }
    );
}
