{
  description = "A puzzle game with mechanics similar to Lemmings";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";

    # FIXME: SCons in newer version seems bugged, ParseConfig() is not
    # returning the return status of pkg-config, it returns None all the
    # time.
    nixpkgs_old.url = "github:NixOS/nixpkgs/nixos-20.09";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    tinygettext.url = "github:tinygettext/tinygettext";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "github:logmich/logmich?ref=logmich-0.1.x";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, nixpkgs_old, flake-utils, tinycmmc, tinygettext, logmich }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pkgs_old = nixpkgs_old.legacyPackages.${system};
      in rec {
        packages = rec {
          default = pingus;

          pingus = pkgs.stdenv.mkDerivation rec {
            pname = "pingus";
            version = "0.7.7";
            src =  ./.;

            patchPhase = ''
              substituteInPlace SConscript \
                 --replace external/tinygettext/include/ "${tinygettext.packages.${system}.default}/include/" \
                 --replace external/logmich/include/ "${logmich.packages.${system}.default}/include/" \
                 --replace "self.env.Default(self.env.Program('pingus', ['src/main.cpp', libpingus]))" \
                           "self.env.Default(self.env.Program('pingus', ['src/main.cpp', libpingus, '${tinygettext.packages.${system}.default}/lib/libtinygettext.a', '${logmich.packages.${system}.default}/lib/liblogmich.a']))"
            '';

            enableParallelBuilding = true;

            installPhase = ''
              make install PREFIX=$out PROJECT_NAME=pingus-0.7
            '';

            nativeBuildInputs = [
              pkgs_old.scons
              pkgs.pkg-config
            ];

            buildInputs = with pkgs; [
              SDL
              SDL_image
              SDL_mixer
              libpng
              boost
            ];
          };
        };

        apps = rec {
          default = pingus;

          pingus = flake-utils.lib.mkApp {
            drv = packages.pingus;
            exePath = "/bin/pingus-0.7";
          };
        };
      }
    );
}
