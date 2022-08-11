{
  description = "A free Lemmings clone with penguins";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";
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
    geomcpp.inputs.flake-utils.follows = "flake-utils";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    logmich.url = "github:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    priocpp.url = "github:grumbel/priocpp";
    priocpp.inputs.nixpkgs.follows = "nixpkgs";
    priocpp.inputs.flake-utils.follows = "flake-utils";
    priocpp.inputs.tinycmmc.follows = "tinycmmc";
    priocpp.inputs.logmich.follows = "logmich";
    priocpp.inputs.sexpcpp.follows = "sexpcpp";

    sexpcpp.url = "github:lispparser/sexp-cpp";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";

    strutcpp.url = "github:grumbel/strutcpp";
    strutcpp.inputs.nixpkgs.follows = "nixpkgs";
    strutcpp.inputs.flake-utils.follows = "flake-utils";
    strutcpp.inputs.tinycmmc.follows = "tinycmmc";

    tinygettext.url = "github:tinygettext/tinygettext";
    tinygettext.inputs.nixpkgs.follows = "nixpkgs";
    tinygettext.inputs.flake-utils.follows = "flake-utils";
    tinygettext.inputs.tinycmmc.follows = "tinycmmc";

    xdgcpp.url = "github:grumbel/xdgcpp";
    xdgcpp.inputs.nixpkgs.follows = "nixpkgs";
    xdgcpp.inputs.flake-utils.follows = "flake-utils";
    xdgcpp.inputs.tinycmmc.follows = "tinycmmc";

    wstsound.url = "github:WindstilleTeam/wstsound";
    wstsound.inputs.nixpkgs.follows = "nixpkgs";
    wstsound.inputs.flake-utils.follows = "flake-utils";
    wstsound.inputs.tinycmmc.follows = "tinycmmc";

    SDL2_src.url = "https://libsdl.org/release/SDL2-devel-2.0.22-mingw.tar.gz";
    SDL2_src.flake = false;

    SDL2_image_src.url = "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz";
    SDL2_image_src.flake = false;
  };

  outputs = { self, nixpkgs, flake-utils,
              tinycmmc, uitest, argpp, geomcpp, logmich, priocpp, sexpcpp, strutcpp, tinygettext, xdgcpp, wstsound,
              SDL2_src, SDL2_image_src }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      {
        packages = rec {
          default = pingus;

          SDL2 = pkgs.stdenv.mkDerivation {
            pname = "SDL2";
            version = "2.0.22";
            src = SDL2_src;
            installPhase = ''
              mkdir $out
              cp -vr ${pkgs.targetPlatform.config}/. $out/
              substituteInPlace $out/lib/pkgconfig/sdl2.pc \
                --replace "prefix=/opt/local/${pkgs.targetPlatform.config}" \
                          "prefix=$out"
            '';
          };

          SDL2_image = pkgs.stdenv.mkDerivation rec {
            pname = "SDL2";
            version = "2.0.22";
            src = SDL2_image_src;
            installPhase = ''
              mkdir $out
              cp -vr ${pkgs.targetPlatform.config}/. $out/
              substituteInPlace $out/lib/pkgconfig/SDL2_image.pc \
                --replace "prefix=/opt/local/${pkgs.targetPlatform.config}" \
                          "prefix=$out"
            '';
          };

          pingus = pkgs.callPackage ./pingus.nix {
            inherit self;
            argpp = argpp.packages.${pkgs.system}.default;
            geomcpp = geomcpp.packages.${pkgs.system}.default;
            logmich = logmich.packages.${pkgs.system}.default;
            priocpp = priocpp.packages.${pkgs.system}.default;
            sexpcpp = sexpcpp.packages.${pkgs.system}.default;
            strutcpp = strutcpp.packages.${pkgs.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.system}.default;
            tinygettext = tinygettext.packages.${pkgs.system}.default;
            uitest = uitest.packages.${pkgs.system}.default;
            wstsound = wstsound.packages.${pkgs.system}.default;
            xdgcpp = if !pkgs.targetPlatform.isWindows
                     then xdgcpp.packages.${pkgs.system}.default
                     else null;
            mcfgthreads = if pkgs.targetPlatform.isWindows
                          then pkgs.windows.mcfgthreads
                          else null;

            SDL2 = if pkgs.targetPlatform.isWindows then SDL2 else pkgs.SDL2;
            SDL2_image = if pkgs.targetPlatform.isWindows then SDL2_image else pkgs.SDL2_image;
          };

          pingus-win32 = pkgs.runCommand "pingus-win32" {} ''
            mkdir -p $out
            mkdir -p $out/data/

            cp -vr ${pingus}/bin/pingus.exe $out/
            cp -vLr ${pingus}/bin/*.dll $out/
            cp -vr ${pingus}/share/pingus/. $out/data/
          '';
        };
      }
    );
}
