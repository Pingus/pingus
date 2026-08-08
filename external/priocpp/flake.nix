{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "git+https://github.com/grumbel/tinycmmc.git";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    logmich.url = "git+https://github.com/logmich/logmich.git";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    sexpcpp.url = "git+https://github.com/lispparser/sexp-cpp.git";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
    sexpcpp.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc, logmich, sexpcpp }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      {
        packages = rec {
          default = priocpp;

          priocpp = pkgs.callPackage ./priocpp.nix {
            logmich = logmich.packages.${pkgs.stdenv.hostPlatform.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
            sexpcpp = sexpcpp.packages.${pkgs.stdenv.hostPlatform.system}.default;
          };

          priocpp-sexp = pkgs.callPackage ./priocpp.nix {
            logmich = logmich.packages.${pkgs.stdenv.hostPlatform.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
            sexpcpp = sexpcpp.packages.${pkgs.stdenv.hostPlatform.system}.default;

            withJsoncpp = false;
            withSexpcpp = true;
          };

          priocpp-json = pkgs.callPackage ./priocpp.nix {
            logmich = logmich.packages.${pkgs.stdenv.hostPlatform.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.stdenv.hostPlatform.system}.default;
            sexpcpp = null;

            withJsoncpp = true;
            withSexpcpp = false;
          };
        };
      }
    );
}
