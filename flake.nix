{
  description = "A tiny CMake module collection";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      tinycmmc_lib = import ./. { inherit nixpkgs flake-utils; };
    in
      {
        lib = import ./. { inherit nixpkgs flake-utils; };
      } //
      tinycmmc_lib.eachSystemWithPkgs (pkgs:
        rec {
          packages = rec {
            default = tinycmmc;
            tinycmmc = pkgs.callPackage ./tinycmmc.nix {
              inherit self tinycmmc_lib;
            };
          };
        }
      );
}
