{ self
, stdenv
, lib
, cmake
, glm
, gtest
}:

stdenv.mkDerivation {
  pname = "geomcpp";
  version =
    let
      raw = lib.strings.removeSuffix "\n" (builtins.readFile ./VERSION);
    in
      if lib.hasPrefix "$Format" raw then "0.0.0"
      else lib.removePrefix "v" raw;

  src = lib.cleanSource ./.;

  doCheck = true;

  cmakeFlags = [
    "-DWARNINGS=ON"
    "-DWERROR=ON"
    "-DBUILD_TESTS=ON"
  ];

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = [
    gtest
  ];

  propagatedBuildInputs = [
    glm
  ];
}
