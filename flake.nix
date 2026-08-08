{
  description = "Windstille Sound Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "git+https://github.com/grumbel/tinycmmc.git";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    libmodplug-win32.url = "git+https://github.com/grumnix/libmodplug-win32.git";
    libmodplug-win32.inputs.nixpkgs.follows = "nixpkgs";
    libmodplug-win32.inputs.tinycmmc.follows = "tinycmmc";

    libogg-win32.url = "git+https://github.com/grumnix/libogg-win32.git";
    libogg-win32.inputs.nixpkgs.follows = "nixpkgs";
    libogg-win32.inputs.tinycmmc.follows = "tinycmmc";

    libvorbis-win32.url = "git+https://github.com/grumnix/libvorbis-win32.git";
    libvorbis-win32.inputs.nixpkgs.follows = "nixpkgs";
    libvorbis-win32.inputs.tinycmmc.follows = "tinycmmc";
    libvorbis-win32.inputs.libogg.follows = "libogg-win32";

    mpg123-win32.url = "git+https://github.com/grumnix/mpg123-win32.git";
    mpg123-win32.inputs.nixpkgs.follows = "nixpkgs";
    mpg123-win32.inputs.tinycmmc.follows = "tinycmmc";

    openal-soft-win32.url = "git+https://github.com/grumnix/openal-soft-win32.git";
    openal-soft-win32.inputs.nixpkgs.follows = "nixpkgs";
    openal-soft-win32.inputs.tinycmmc.follows = "tinycmmc";

    opus-win32.url = "git+https://github.com/grumnix/opus-win32.git";
    opus-win32.inputs.nixpkgs.follows = "nixpkgs";
    opus-win32.inputs.tinycmmc.follows = "tinycmmc";

    opusfile-win32.url = "git+https://github.com/grumnix/opusfile-win32.git";
    opusfile-win32.inputs.nixpkgs.follows = "nixpkgs";
    opusfile-win32.inputs.tinycmmc.follows = "tinycmmc";
    opusfile-win32.inputs.libogg.follows = "libogg-win32";
    opusfile-win32.inputs.opus.follows = "opus-win32";
  };

  outputs = { self, nixpkgs, flake-utils,
              tinycmmc, libmodplug-win32, libogg-win32, libvorbis-win32,
              mpg123-win32, openal-soft-win32, opusfile-win32, opus-win32 }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      rec {
        packages = rec {
          default = wstsound;

          wstsound = pkgs.callPackage ./wstsound.nix {
            stdenv = pkgs.stdenv;

            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;

            libmodplug = if pkgs.stdenv.hostPlatform.isWindows
                         then libmodplug-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                         else pkgs.libmodplug;

            libogg = if pkgs.stdenv.hostPlatform.isWindows
                     then libogg-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                     else pkgs.libogg;

            libvorbis = if pkgs.stdenv.hostPlatform.isWindows
                        then libvorbis-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                        else pkgs.libvorbis;

            mpg123 = if pkgs.stdenv.hostPlatform.isWindows
                     then mpg123-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                     else pkgs.mpg123;

            openal = if pkgs.stdenv.hostPlatform.isWindows
                     then openal-soft-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                     else pkgs.openal;

            opusfile = if pkgs.stdenv.hostPlatform.isWindows
                       then opusfile-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                       else pkgs.opusfile;

            libopus = if pkgs.stdenv.hostPlatform.isWindows
                      then opus-win32.packages.${pkgs.stdenv.hostPlatform.system}.default
                      else pkgs.libopus;

            mcfgthreads = if pkgs.stdenv.hostPlatform.isWindows
                          then pkgs.windows.mcfgthreads
                          else null;
          };
        };

        apps = rec {
          default = wstsound-play;

          wstsound-play = {
            type = "app";
            program = "${packages.wstsound}/bin/wstsound-play";
          };
        };
      }
    );
}
