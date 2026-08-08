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
  };

  outputs = { self, nixpkgs, flake-utils, SDL2-win32, SDL2_image-win32 }:
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
        selfFor = path: {
          outPath = path;
          shortRev = self.shortRev or "dirty";
          dirtyShortRev = self.dirtyShortRev or "dirty";
          lastModifiedDate = self.lastModifiedDate or "19700101";
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
        };

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
        };
      }
    );
}
