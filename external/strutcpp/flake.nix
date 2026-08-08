{
  description = "A collection of string utilities for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, tinycmmc }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      {
        packages = rec {
          default = strutcpp;

          strutcpp = pkgs.callPackage ./strutcpp.nix {
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
          };
        };
      }
    );
}
