{ self
, stdenv
, lib

, cmake
, gtest
, jsoncpp
, pkg-config

, logmich
, sexpcpp

, withSexpcpp ? true
, withJsoncpp ? true
, buildTests ? true
, buildExtra ? true
, enableWarnings ? false
, enableWerror ? false
}:

let
  versionBase = lib.strings.removeSuffix "\n" (builtins.readFile ./VERSION);
  gitRev = "${self.shortRev or self.dirtyShortRev or "dirty"}";
  version = "${versionBase}+g${gitRev}";
in
stdenv.mkDerivation {
  pname = "priocpp";
  inherit version;

  src = lib.cleanSource ./.;

  cmakeFlags = [
    "-DPROJECT_VERSION_FULL=${version}"
    (if buildExtra then "-DBUILD_EXTRA=ON" else "-DBUILD_EXTRA=OFF")
    (if buildTests then "-DBUILD_TESTS=ON" else "-DBUILD_TESTS=OFF")
    (if withJsoncpp then "-DPRIO_USE_JSONCPP=ON" else "-DPRIO_USE_JSONCPP=OFF")
    (if withSexpcpp then "-DPRIO_USE_SEXPCPP=ON" else "-DPRIO_USE_SEXPCPP=OFF")
    (if enableWarnings then "-DWARNINGS=ON" else "-DWARNINGS=OFF")
    (if enableWerror then "-DWERROR=ON" else "-DWERROR=OFF")
  ];

  # Run ctest (test_prio + priotool smoke test when BUILD_EXTRA is on).
  doCheck = buildTests;

  postFixup = lib.optionalString stdenv.hostPlatform.isWindows (
    ''
      # This is rather ugly, but functional. Nix has a 'win-dll-link.sh'
      # for this, but that's currently broken:
      # https://github.com/NixOS/nixpkgs/issues/38451
      mkdir -p $out/bin/
    ''
    + lib.optionalString withJsoncpp ''
      ln -sfv ${jsoncpp}/bin/*.dll $out/bin/
    ''
  );

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  # gtest is only needed to build/run the test binary, not by consumers of the library.
  buildInputs = lib.optionals buildTests [ gtest ];

  propagatedBuildInputs = [
    logmich
  ]
  ++ lib.optional withJsoncpp jsoncpp
  ++ lib.optional withSexpcpp sexpcpp;
}
