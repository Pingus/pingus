{
  description = "Property I/O for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    logmich.url = "git+https://github.com/logmich/logmich.git";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    # logmich still pulls tinycmmc transitively; we no longer depend on it here

    sexpcpp.url = "git+https://github.com/lispparser/sexp-cpp.git";
    sexpcpp.inputs.nixpkgs.follows = "nixpkgs";
    sexpcpp.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, logmich, sexpcpp }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        logmichPkg = logmich.packages.${system}.default;
        sexpcppPkg = sexpcpp.packages.${system}.default;

        mkPriocpp = args: pkgs.callPackage ./priocpp.nix ({
          inherit self;
          logmich = logmichPkg;
          sexpcpp = sexpcppPkg;
        } // args);
      in
      {
        packages = rec {
          default = priocpp;

          # Default: both backends, tests + extras
          priocpp = mkPriocpp { };

          # Feature variants (still with tests)
          priocpp-sexp = mkPriocpp {
            withJsoncpp = false;
            withSexpcpp = true;
          };

          priocpp-json = mkPriocpp {
            withJsoncpp = true;
            withSexpcpp = false;
            sexpcpp = null;
          };
        };

        checks = {
          # Full default configuration (json + sexp), runs ctest
          priocpp = self.packages.${system}.priocpp;

          # Backend-only variants
          priocpp-sexp = self.packages.${system}.priocpp-sexp;
          priocpp-json = self.packages.${system}.priocpp-json;

          # Strict build: maximum warnings treated as errors
          priocpp-werror = mkPriocpp {
            enableWarnings = true;
            enableWerror = true;
          };
        };
      }
    );
}
