{
  description = "A collection of point, size and rect classes for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = rec {
          default = geomcpp;
          geomcpp = pkgs.callPackage ./geomcpp.nix {
            inherit self;
            # Allow the glm package on systems other than Unix
            glm = pkgs.glm.overrideAttrs (oldAttrs: { meta = {}; });
          };
        };
      }
    );
}
