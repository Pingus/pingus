{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    uitest.url = "github:grumbel/uitest";
    uitest.inputs.nixpkgs.follows = "nixpkgs";
    uitest.inputs.flake-utils.follows = "flake-utils";
    uitest.inputs.tinycmmc.follows = "tinycmmc";

    argpp.url = "github:grumbel/argpp?ref=stable";
    argpp.inputs.nixpkgs.follows = "nixpkgs";
    argpp.inputs.flake-utils.follows = "flake-utils";
    argpp.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "github:grumbel/geomcpp";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "github:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    priocpp.url = "github:grumbel/priocpp";
    priocpp.inputs.nixpkgs.follows = "nixpkgs";
    priocpp.inputs.flake-utils.follows = "flake-utils";
    priocpp.inputs.tinycmmc.follows = "tinycmmc";
    priocpp.inputs.logmich.follows = "logmich";

    strutcpp.url = "github:grumbel/strutcpp";
    strutcpp.inputs.nixpkgs.follows = "nixpkgs";
    strutcpp.inputs.tinycmmc.follows = "tinycmmc";

    tinygettext.url = "github:tinygettext/tinygettext";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    xdgcpp.url = "github:grumbel/xdgcpp";
    xdgcpp.inputs.nixpkgs.follows = "nixpkgs";
    xdgcpp.inputs.flake-utils.follows = "flake-utils";

    wstsound.url = "github:WindstilleTeam/wstsound";
    wstsound.inputs.nixpkgs.follows = "nixpkgs";
    wstsound.inputs.flake-utils.follows = "flake-utils";
    wstsound.inputs.tinycmmc.follows = "tinycmmc";

    SDL2-win32.url = "github:grumnix/SDL2-win32";
    SDL2-win32.inputs.nixpkgs.follows = "nixpkgs";
    SDL2-win32.inputs.tinycmmc.follows = "tinycmmc";

    SDL2_image-win32.url = "github:grumnix/SDL2_image-win32";
    SDL2_image-win32.inputs.nixpkgs.follows = "nixpkgs";
    SDL2_image-win32.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils,
              tinycmmc, uitest, argpp, geomcpp, logmich, priocpp, strutcpp,
              tinygettext, xdgcpp, wstsound, SDL2-win32, SDL2_image-win32
            }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      rec {
        packages = rec {
          default = pingus;

          pingus = pkgs.callPackage ./pingus.nix {
            inherit self;
            stdenv = pkgs.stdenv;

            argpp = argpp.packages.${pkgs.stdenv.hostPlatform.system}.default;
            geomcpp = geomcpp.packages.${pkgs.stdenv.hostPlatform.system}.default;
            logmich = logmich.packages.${pkgs.stdenv.hostPlatform.system}.default;
            priocpp = priocpp.packages.${pkgs.stdenv.hostPlatform.system}.priocpp-sexp;
            strutcpp = strutcpp.packages.${pkgs.stdenv.hostPlatform.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
            tinygettext = tinygettext.packages.${pkgs.stdenv.hostPlatform.system}.default;
            uitest = uitest.packages.${pkgs.stdenv.hostPlatform.system}.default;
            wstsound = wstsound.packages.${pkgs.stdenv.hostPlatform.system}.default;
            xdgcpp = if !pkgs.stdenv.targetPlatform.isWindows
                     then xdgcpp.packages.${pkgs.stdenv.hostPlatform.system}.default
                     else null;
            mcfgthreads = if pkgs.stdenv.targetPlatform.isWindows
                          then pkgs.windows.mcfgthreads
                          else null;
            libGL = if pkgs.stdenv.targetPlatform.isWindows
                    then null
                    else pkgs.libGL;
            libGLU  = if pkgs.stdenv.targetPlatform.isWindows
                      then null
                      else pkgs.libGLU;

            SDL2 = if pkgs.stdenv.targetPlatform.isWindows
                   then SDL2-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                   else pkgs.SDL2;
            SDL2_image = if pkgs.stdenv.targetPlatform.isWindows
                         then SDL2_image-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                         else pkgs.SDL2_image;
          };
        } // (pkgs.lib.optionalAttrs (pkgs.stdenv.targetPlatform.isWindows) rec {
          pingus-win32 = pkgs.runCommand "pingus-win32" {} ''
            mkdir -p $out
            mkdir -p $out/data/

            cp -vr ${packages.pingus}/bin/pingus.exe $out/
            cp -vLr ${packages.pingus}/bin/*.dll $out/
            cp -vr ${packages.pingus}/share/pingus/. $out/data/
          '';

          pingus-win32-zip = pkgs.runCommand "pingus-win32-zip" {} ''
            mkdir -p $out
            WORKDIR=$(mktemp -d)

            cp --no-preserve mode,ownership --verbose --recursive \
              ${pingus-win32}/. "$WORKDIR"

            cd "$WORKDIR"
            ${nixpkgs.legacyPackages.x86_64-linux.zip}/bin/zip \
              -r \
              $out/pingus-${packages.pingus.version}-${pkgs.stdenv.hostPlatform.system}.zip \
              .
          '';
        });
      }
    );
}
