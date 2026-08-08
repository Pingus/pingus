{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "git+https://github.com/grumbel/tinycmmc.git";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    uitest.url = "git+https://github.com/grumbel/uitest.git";
    uitest.inputs.nixpkgs.follows = "nixpkgs";
    uitest.inputs.flake-utils.follows = "flake-utils";
    uitest.inputs.tinycmmc.follows = "tinycmmc";

    argpp.url = "git+https://github.com/grumbel/argpp.git?ref=stable";
    argpp.inputs.nixpkgs.follows = "nixpkgs";
    argpp.inputs.flake-utils.follows = "flake-utils";
    argpp.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "git+https://github.com/grumbel/geomcpp.git";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "git+https://github.com/logmich/logmich.git";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    sexpcpp.url = "git+https://github.com/lispparser/sexp-cpp.git";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";

    priocpp.url = "git+https://github.com/grumbel/priocpp.git";
    priocpp.inputs.nixpkgs.follows = "nixpkgs";
    priocpp.inputs.flake-utils.follows = "flake-utils";
    priocpp.inputs.tinycmmc.follows = "tinycmmc";
    priocpp.inputs.logmich.follows = "logmich";
    priocpp.inputs.sexpcpp.follows = "sexpcpp";

    strutcpp.url = "git+https://github.com/grumbel/strutcpp.git";
    strutcpp.inputs.nixpkgs.follows = "nixpkgs";
    strutcpp.inputs.tinycmmc.follows = "tinycmmc";

    tinygettext.url = "git+https://github.com/tinygettext/tinygettext.git";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    xdgcpp.url = "git+https://github.com/grumbel/xdgcpp.git";
    xdgcpp.inputs.nixpkgs.follows = "nixpkgs";
    xdgcpp.inputs.flake-utils.follows = "flake-utils";

    wstsound.url = "git+https://github.com/WindstilleTeam/wstsound.git";
    wstsound.inputs.nixpkgs.follows = "nixpkgs";
    wstsound.inputs.flake-utils.follows = "flake-utils";
    wstsound.inputs.tinycmmc.follows = "tinycmmc";

    # Official MinGW SDL2 devel packages (same pattern as SuperTux Milestone 1).
    # Exposed under packages.x86_64-windows / i686-windows by those flakes.
    SDL2-win32.url = "git+https://github.com/grumnix/SDL2-win32.git";
    SDL2-win32.inputs.nixpkgs.follows = "nixpkgs";
    SDL2-win32.inputs.tinycmmc.follows = "tinycmmc";

    SDL2_image-win32.url = "git+https://github.com/grumnix/SDL2_image-win32.git";
    SDL2_image-win32.inputs.nixpkgs.follows = "nixpkgs";
    SDL2_image-win32.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils,
              tinycmmc, uitest, argpp, geomcpp, logmich, priocpp, strutcpp, sexpcpp,
              tinygettext, xdgcpp, wstsound, SDL2-win32, SDL2_image-win32
            }:
    # Host systems only (Linux/mac/…). Windows is a *target* built via pkgsCross
    # from a Linux host — packages live under packages.<host>.pingus-win32-x64,
    # not under packages.x86_64-windows (that confused hostSystem vs buildSystem).
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        lib = nixpkgs.lib;
        isWin = pkgs.stdenv.hostPlatform.isWindows;

        # Resolve a helper-library package for a given target system attr
        # (e.g. "x86_64-linux" or "x86_64-windows").
        dep = flake: attr: system':
          flake.packages.${system'}.${attr};

        mkPingus = {
          pkgs',
          targetSystem,
          pname ? "pingus",
        }:
          pkgs'.callPackage ./pingus.nix {
            inherit self;
            stdenv = pkgs'.stdenv;
            tinycmmc_lib = tinycmmc.lib;

            argpp = dep argpp "default" targetSystem;
            geomcpp = dep geomcpp "default" targetSystem;
            logmich = dep logmich "default" targetSystem;
            priocpp = dep priocpp "priocpp-sexp" targetSystem;
            strutcpp = dep strutcpp "default" targetSystem;
            tinycmmc = dep tinycmmc "default" targetSystem;
            tinygettext = dep tinygettext "default" targetSystem;
            uitest = dep uitest "default" targetSystem;
            wstsound = dep wstsound "default" targetSystem;
            xdgcpp = if targetSystem == "x86_64-windows" || targetSystem == "i686-windows"
                     then null
                     else dep xdgcpp "default" targetSystem;
            mcfgthreads = if pkgs'.stdenv.hostPlatform.isWindows
                          then pkgs'.windows.mcfgthreads
                          else null;
            libGL = if pkgs'.stdenv.hostPlatform.isWindows then null else pkgs'.libGL;
            libGLU = if pkgs'.stdenv.hostPlatform.isWindows then null else pkgs'.libGLU;

            SDL2 = if pkgs'.stdenv.hostPlatform.isWindows
                   then SDL2-win32.packages.${targetSystem}.default
                   else pkgs'.SDL2;
            SDL2_image = if pkgs'.stdenv.hostPlatform.isWindows
                         then SDL2_image-win32.packages.${targetSystem}.default
                         else pkgs'.SDL2_image;
          };

        pingusNative = mkPingus {
          pkgs' = pkgs;
          targetSystem = system;
          pname = "pingus";
        };

        # Flat redistributable: .exe + DLLs + data/ at the root (pingus-style).
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

        # Windows cross only from non-Windows build hosts (same idea as SuperTux).
        win64Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingwW64;
          targetSystem = "x86_64-windows";
          pname = "pingus-win32-x64";
        };
        win32Game = if isWin then null else mkPingus {
          pkgs' = pkgs.pkgsCross.mingw32;
          targetSystem = "i686-windows";
          pname = "pingus-win32-x86";
        };

        win64Package = if isWin then null else mkWinFlat {
          game = win64Game;
          pname = "pingus-win32-x64";
        };
        win32Package = if isWin then null else mkWinFlat {
          game = win32Game;
          pname = "pingus-win32-x86";
        };
      in {
        packages = {
          default = pingusNative;
          pingus = pingusNative;
        } // lib.optionalAttrs (!isWin) {
          # Host = Linux (or other Unix); target = Windows PE via pkgsCross.
          #   nix build .#pingus-win32-x64
          #   nix build .#packages.x86_64-linux.pingus-win32-x86
          pingus-win32-x64 = win64Package;
          pingus-win32-x86 = win32Package;
          pingus-win32-x64-zip = mkWinZip win64Package "pingus";
          pingus-win32-x86-zip = mkWinZip win32Package "pingus";
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
