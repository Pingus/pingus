{
  description = "Property I/O for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    logmich.url = "git+https://github.com/logmich/logmich.git";
    logmich.inputs.nixpkgs.follows = "nixpkgs";

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
      in {
        packages = rec {
          default = priocpp;

          # Both backends (default)
          priocpp = mkPriocpp {
            withJsoncpp = true;
            withSexpcpp = true;
          };

          # S-expression only
          priocpp-sexp = mkPriocpp {
            withJsoncpp = false;
            withSexpcpp = true;
          };

          # JSON only
          priocpp-json = mkPriocpp {
            withJsoncpp = true;
            withSexpcpp = false;
            sexpcpp = null;
          };
        };

        # flake check runs ctest for every backend combination
        checks = {
          # json + sexp (default)
          priocpp = self.packages.${system}.priocpp;

          # sexp only — parameterized tests must not instantiate .json
          priocpp-sexp = self.packages.${system}.priocpp-sexp;

          # json only — parameterized tests must not instantiate .sexp
          priocpp-json = self.packages.${system}.priocpp-json;

          # both backends, maximum warnings as errors
          priocpp-werror = mkPriocpp {
            withJsoncpp = true;
            withSexpcpp = true;
            enableWarnings = true;
            enableWerror = true;
          };

          # each single backend with -Werror as well
          priocpp-sexp-werror = mkPriocpp {
            withJsoncpp = false;
            withSexpcpp = true;
            enableWarnings = true;
            enableWerror = true;
          };

          priocpp-json-werror = mkPriocpp {
            withJsoncpp = true;
            withSexpcpp = false;
            sexpcpp = null;
            enableWarnings = true;
            enableWerror = true;
          };
        };
      }
    );
}
