{
  description = "C++ logging library";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";

    tinycmmc.url = "git+https://github.com/grumbel/tinycmmc.git";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, tinycmmc }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      {
        packages = rec {
          default = logmich;
          logmich = pkgs.callPackage ./logmich.nix {
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
          };
        };
      }
    );
}
