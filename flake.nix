{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    argparser.url = "gitlab:argparser/argparser/stable";
    argparser.inputs.nixpkgs.follows = "nixpkgs";
    argparser.inputs.flake-utils.follows = "flake-utils";
    argparser.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "gitlab:grumbel/geomcpp";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.flake-utils.follows = "flake-utils";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "gitlab:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    priocpp.url = "gitlab:grumbel/priocpp";
    priocpp.inputs.nixpkgs.follows = "nixpkgs";
    priocpp.inputs.flake-utils.follows = "flake-utils";
    priocpp.inputs.tinycmmc.follows = "tinycmmc";
    priocpp.inputs.logmich.follows = "logmich";
    priocpp.inputs.sexpcpp.follows = "sexpcpp";

    sexpcpp.url = "gitlab:lispparser/sexp-cpp";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";

    strutcpp.url = "gitlab:grumbel/strutcpp";
    strutcpp.inputs.nixpkgs.follows = "nixpkgs";
    strutcpp.inputs.flake-utils.follows = "flake-utils";
    strutcpp.inputs.tinycmmc.follows = "tinycmmc";

    tinygettext.url = "github:tinygettext/tinygettext";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    xdgcpp.url = "gitlab:grumbel/xdgcpp";
    xdgcpp.inputs.nixpkgs.follows = "nixpkgs";
    xdgcpp.inputs.flake-utils.follows = "flake-utils";
    xdgcpp.inputs.tinycmmc.follows = "tinycmmc";

    wstsound.url = "gitlab:windstille/wstsound";
    wstsound.inputs.nixpkgs.follows = "nixpkgs";
    wstsound.inputs.flake-utils.follows = "flake-utils";
    wstsound.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils,
              tinycmmc, argparser, geomcpp, logmich, priocpp, sexpcpp, strutcpp, tinygettext, xdgcpp, wstsound }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        version_file = pkgs.lib.fileContents ./VERSION;
        pingus_version = if (((builtins.substring 0 1) version_file) != "v")
                         then ("0.8.0-${nixpkgs.lib.substring 0 8 self.lastModifiedDate}-${self.shortRev or "dirty"}")
                         else (builtins.substring 1 ((builtins.stringLength version_file) - 2) version_file);
      in rec {
        packages = flake-utils.lib.flattenTree {
          pingus = pkgs.stdenv.mkDerivation {
            pname = "pingus";
            version = pingus_version;
            src = nixpkgs.lib.cleanSource ./.;
            cmakeFlags = [ "-DBUILD_EXTRA=ON" ];
            postFixup = ''
              wrapProgram $out/libexec/pingus \
                --prefix LIBGL_DRIVERS_PATH ":" "${pkgs.mesa.drivers}/lib/dri" \
                --prefix LD_LIBRARY_PATH ":" "${pkgs.mesa.drivers}/lib"
            '';
            nativeBuildInputs = with pkgs; [
              cmake
              makeWrapper
              pkgconfig
            ] ++ [
              tinycmmc.defaultPackage.${system}
            ];
            buildInputs = with pkgs; [
              SDL2
              SDL2_image
              boost
              fmt
              gtest
              jsoncpp
              libGL
              libGLU
              libmodplug
              libogg
              libpng
              libsigcxx
              libvorbis
              mpg123
              openal
              opusfile
            ] ++ [
              argparser.defaultPackage.${system}
              geomcpp.defaultPackage.${system}
              logmich.defaultPackage.${system}
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
