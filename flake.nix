{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    nix.inputs.nixpkgs.follows = "nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    tinycmmc.inputs.nix.follows = "nix";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    argparser.url = "gitlab:argparser/argparser/stable";
    argparser.inputs.nix.follows = "nix";
    argparser.inputs.nixpkgs.follows = "nixpkgs";
    argparser.inputs.flake-utils.follows = "flake-utils";
    argparser.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "gitlab:grumbel/geomcpp";
    geomcpp.inputs.nix.follows = "nix";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.flake-utils.follows = "flake-utils";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "gitlab:logmich/logmich";
    logmich.inputs.nix.follows = "nix";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    priocpp.url = "gitlab:grumbel/priocpp";
    priocpp.inputs.nix.follows = "nix";
    priocpp.inputs.nixpkgs.follows = "nixpkgs";
    priocpp.inputs.flake-utils.follows = "flake-utils";
    priocpp.inputs.tinycmmc.follows = "tinycmmc";
    priocpp.inputs.logmich.follows = "logmich";
    priocpp.inputs.sexpcpp.follows = "sexpcpp";

    sexpcpp.url = "gitlab:lispparser/sexp-cpp";
    sexpcpp.inputs.nix.follows = "nix";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";

    strutcpp.url = "gitlab:grumbel/strutcpp";
    strutcpp.inputs.nix.follows = "nix";
    strutcpp.inputs.nixpkgs.follows = "nixpkgs";
    strutcpp.inputs.flake-utils.follows = "flake-utils";
    strutcpp.inputs.tinycmmc.follows = "tinycmmc";

    tinygettext.url = "github:tinygettext/tinygettext";
    tinygettext.inputs.nix.follows = "nix";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    xdgcpp.url = "gitlab:grumbel/xdgcpp";
    xdgcpp.inputs.nix.follows = "nix";
    xdgcpp.inputs.nixpkgs.follows = "nixpkgs";
    xdgcpp.inputs.flake-utils.follows = "flake-utils";
    xdgcpp.inputs.tinycmmc.follows = "tinycmmc";

    wstsound.url = "gitlab:windstille/wstsound";
    wstsound.inputs.nix.follows = "nix";
    wstsound.inputs.nixpkgs.follows = "nixpkgs";
    wstsound.inputs.flake-utils.follows = "flake-utils";
    wstsound.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nix, nixpkgs, flake-utils,
              tinycmmc, argparser, geomcpp, logmich, priocpp, sexpcpp, strutcpp, tinygettext, xdgcpp, wstsound }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          pingus = pkgs.stdenv.mkDerivation {
            pname = "pingus";
            version = "0.8.0";
            src = nixpkgs.lib.cleanSource ./.;
            cmakeFlags = [ "-DBUILD_EXTRA=ON" ];
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.gcc
              pkgs.pkgconfig
              tinycmmc.defaultPackage.${system}
            ];
            buildInputs = [
              pkgs.fmt
              pkgs.SDL2
              pkgs.SDL2_image
              pkgs.boost
              pkgs.libpng
              pkgs.libGLU
              pkgs.libGL
              pkgs.jsoncpp
              pkgs.libsigcxx

              pkgs.openal
              pkgs.libvorbis
              pkgs.libogg
              pkgs.opusfile
              pkgs.mpg123
              pkgs.libmodplug

              argparser.defaultPackage.${system}
              logmich.defaultPackage.${system}
              geomcpp.defaultPackage.${system}
              priocpp.defaultPackage.${system}
              sexpcpp.defaultPackage.${system}
              strutcpp.defaultPackage.${system}
              tinygettext.defaultPackage.${system}
              wstsound.defaultPackage.${system}
              xdgcpp.defaultPackage.${system}
            ];
           };
        };
        defaultPackage = packages.pingus;
      });
}
