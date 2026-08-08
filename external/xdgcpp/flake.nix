{
  description = "XDG Base Directory Specification in C++11";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = rec {
          default = xdgcpp;

          xdgcpp = pkgs.stdenv.mkDerivation {
            pname = "xdgcpp";
            version = "0.1.0";

            src = ./.;

            cmakeFlags = [
              "-DXDG_BUILD_TESTS=ON"
            ];

            doCheck = true;

            nativeBuildInputs = with pkgs; [
              cmake
            ];

            checkInputs = with pkgs; [
              boost
            ];
          };
        };

        apps = rec {
          default = xdgcpp-info;

          xdgcpp-info = flake-utils.lib.mkApp {
            drv = packages.xdgcpp;
            exePath = "/bin/xdgcpp-info";
          };
        };
      }
    );
}
