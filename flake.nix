{
  description = "A S-Expression Parser for C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      # Source of truth: top-level VERSION file.
      # Development builds (VERSION contains -dev) append .revCount+g<shortRev>
      # e.g. 0.2.0-dev.42+gf1fb306. Release builds use VERSION as-is.
      versionBase = nixpkgs.lib.strings.removeSuffix "\n" (builtins.readFile ./VERSION);
      gitRev = "${self.shortRev or self.dirtyShortRev or "dirty"}";
      isDev = nixpkgs.lib.strings.hasInfix "-dev" versionBase;
      version =
        if isDev then
          "${versionBase}.${toString (self.revCount or 0)}+g${gitRev}"
        else
          versionBase;
    in
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      rec {
        packages = rec {
          default = sexp-cpp;
          sexp-cpp = pkgs.callPackage ./sexpcpp.nix {
            inherit version;
          };
        };

        # `nix flake check` builds these (package runs tests via doCheck).
        checks = {
          sexp-cpp = packages.sexp-cpp;
        };
      }
    );
}
